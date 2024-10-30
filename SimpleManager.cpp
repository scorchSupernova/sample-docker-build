
#include "stdafx.h"
#include "manager.h"
#include "redis_client.h"
#include "logger.h"
#include "config_manager.h"
#include "httpRequest.h"
#include "InteruptThread.h"
#include <future>
#include <chrono>
#include "Kafka.h"

using namespace std::chrono;

//---
#define MT5_TIMEOUT_CONNECT   300000
#define MT5_TIMEOUT_DEALER    10000

auto logger = Logger::get();
std::string testPayload;
boolean isUpdate = false;
boolean isDelete = false;
std::string redis_key_prefix;
std::string redis_publish_channel;
u_int64 error_login;
int is_error_log_enable;

inline std::string createKey(int ticket, int login) {
	std::string unique_id = redis_key_prefix;
	unique_id += std::to_string(login);
	unique_id += "-";
	unique_id += std::to_string(ticket);
	return unique_id;
}

inline std::string getPayloadWithId(std::string payload, int ticket, int login) {
	std::string unique_id = createKey(ticket, login);
	std::string payloadWithID = payload;
	payloadWithID.pop_back();
	payloadWithID += ", ";
    payloadWithID += "\"key\": \"" + unique_id + "\"}";
	//std::cout << unique_id << " " << payloadWithID << std::endl;
    return payloadWithID;
}

class MyTickSink : public IMTTickSink {
public:
	virtual void OnTick(LPCWSTR symbol, const MTTickShort& tick) {
		//std::wcout << L"Received tick for symbol " << symbol << L", bid=" << tick.bid << L", ask=" << tick.ask << std::endl;
	}
};

class MyDealSink : public IMTDealSink {
public:

	MyDealSink(RedisClient& redisClient, CManager& manager, Kafka& kafka_cl) : m_redisClient(redisClient), cm_manager(manager), kafka_client(kafka_cl) {}

	virtual void OnDealAdd(const IMTDeal* deal) {
		
		if (isDelete || isUpdate) {
			if (deal->Action() == 0 || deal->Action() == 1) {
				std::string dealPayload = cm_manager.convertDealToJson(deal,isUpdate);
				nlohmann::json dealDetailsJson = nlohmann::json::parse(dealPayload);
				nlohmann::json posDetailsJson = nlohmann::json::parse(testPayload);
                std::string unique_key = createKey(isUpdate ? deal->Deal() : deal->PositionID(), deal->Login());
				dealDetailsJson["object"]["open_time_str"] = posDetailsJson["object"]["open_time_str"];
				dealDetailsJson["object"]["open_time"] = posDetailsJson["object"]["open_time"];
				dealDetailsJson["object"]["close_price"] = posDetailsJson["object"]["close_price"];
				dealDetailsJson["object"]["open_price"] = posDetailsJson["object"]["open_price"];
				dealDetailsJson["object"]["type"] = posDetailsJson["object"]["type"];
				dealDetailsJson["object"]["type_str"] = posDetailsJson["object"]["type_str"];
				dealDetailsJson["object"]["lots"] = posDetailsJson["object"]["lots"];
				dealDetailsJson["key"] = unique_key;
				std::string updatedPayload = dealDetailsJson.dump();
                //m_redisClient.Publish(redis_publish_channel, unique_key, updatedPayload);
				kafka_client.pushData(unique_key, updatedPayload);
			}
		}

	}

private:
	RedisClient& m_redisClient;
	CManager& cm_manager;
	Kafka& kafka_client;

};

class MyUserSink : public IMTUserSink {
public:
	MyUserSink(RedisClient& redisClient, CManager& manager, Kafka& kafka_cl) : m_redisClient(redisClient), cm_manager(manager), kafka_client(kafka_cl) {}

	virtual void OnUserAdd(const IMTUser* user) {

		std::cout << "New user added: " << user->Login() << std::endl;
		IMTAccount* account = cm_manager.GetAccountInfo(user->Login());
		
		if (account != NULL) {
			if (!m_redisClient.CacheSet("margin:" + std::to_string(user->Login()), cm_manager.convertUserToJson(user, account))) {
				logger->error("Failed to set margin cache value for user login {}", user->Login());
			}
		}

	}

private:
	RedisClient& m_redisClient;
	CManager& cm_manager;
	Kafka& kafka_client;
};

class MyPositionSink : public IMTPositionSink {
public:
	MyPositionSink(RedisClient& redisClient, CManager& manager, Kafka& kafka_cl) : m_redisClient(redisClient), cm_manager(manager),kafka_client(kafka_cl) {
		lastTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		requestCounter = 0;
	}
	virtual void OnPositionAdd(const IMTPosition* position) {
		//std::cout << "New position added: " << position->Position() << std::endl;
		std::string payload = cm_manager.convertTradeDetailsToJson(position, true);
		isDelete = false;
		isUpdate = false;
		requestCounter++;
		u_int64 curTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
		if (!is_error_log_enable && (curTime - lastTime >= 1000)) {
			std::cout << "===> Got " << requestCounter << " data in " << curTime - lastTime << " miliseconds.\n";
			logger->info("===> Got {} data in {} miliseconds.", requestCounter, curTime - lastTime);
			requestCounter = 0;
			lastTime = curTime;
		}
        std::string payloadWithId = getPayloadWithId(payload, position->Position(), position->Login());
		// m_redisClient.Publish(redis_publish_channel, createKey(position->Position(), position->Login()),  payloadWithId);
		kafka_client.pushData(createKey(position->Position(), position->Login()), payloadWithId);
	}
	virtual void OnPositionDelete(const IMTPosition* position) {
		//std::cout << "Position delete: " << position->Position() << std::endl;
        testPayload = cm_manager.convertTradeDetailsToJson(position,false);
		requestCounter++;
		u_int64 curTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
		if (!is_error_log_enable && (curTime - lastTime >= 1000)) {
			std::cout << "===> Got " << requestCounter << " data in " << curTime - lastTime << " miliseconds.\n";
			logger->info("===> Got {} data in {} miliseconds.", requestCounter, curTime - lastTime);
			requestCounter = 0;
			lastTime = curTime;
		}
        testPayload = getPayloadWithId(testPayload, position->Position(), position->Login());;
        isDelete =true;
        isUpdate = false;
	}

	virtual void OnPositionUpdate(const IMTPosition* position) {
		//std::cout << "Position update: " << position->Position() << std::endl;
		std::string payload = cm_manager.convertTradeDetailsToJson(position, true);
		testPayload = payload;
		isUpdate = true;
		isDelete = false;
		requestCounter++;
		u_int64 curTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
		if (!is_error_log_enable && (curTime - lastTime >= 1000)) {
			std::cout << "===> Got " << requestCounter << " data in " << curTime - lastTime << " miliseconds.\n";
			logger->info("===> Got {} data in {} miliseconds.", requestCounter, curTime - lastTime);
			requestCounter = 0;
			lastTime = curTime;
		}
        std::string payloadWithId = getPayloadWithId(payload, position->Position(), position->Login());
        
        //m_redisClient.Publish(redis_publish_channel, createKey(position->Position(), position->Login()), payloadWithId);
		kafka_client.pushData(createKey(position->Position(), position->Login()), payloadWithId);
	}

private:
	RedisClient& m_redisClient;
	CManager& cm_manager;
	Kafka& kafka_client;
	u_int64 lastTime;
	u_int64 requestCounter;
};


using namespace std::chrono;
int wmain(int argc, wchar_t* argv[])
{

	NV::InterruptHandler::hookSIGINT();

	ConfigManager::Load("config.json");
	string address = ConfigManager::Get("mt5.server");
	string server_pass = ConfigManager::Get("mt5.password");
	UINT64 login = ConfigManager::Get("mt5.login", 0);
	string redis_host_main = ConfigManager::Get("redis.main_redis.url");
	int redis_port_main = ConfigManager::Get("redis.main_redis.port", 6379);
    string redis_host_trade = ConfigManager::Get("redis.trade_redis.url");
	int redis_port_trade = ConfigManager::Get("redis.trade_redis.port", 6379);
	string call_back_url=ConfigManager::Get("callBack.url");
	int redis_retry_time_in_second = ConfigManager::Get("redis.config.redis_retry_time_in_second", 60);
	redis_publish_channel = ConfigManager::Get("redis.config.redis_publish_channel");
	redis_key_prefix = ConfigManager::Get("redis.config.redis_key_prefix");
	is_error_log_enable =  ConfigManager::Get("error_log.enable",0);
	error_login = ConfigManager::Get("error_log.login", 0);
	std::map <std::string, std::string> kafka_properties = ConfigManager::GetArray("kafka.kafka_properties");
	std::string kafka_topic = ConfigManager::Get("kafka.topic");
	std::cout<<"Config Variable taken done"<<std::endl;

	wchar_t* server = new wchar_t[256];
	wchar_t* password = new wchar_t[256];
	std::mbstowcs(server, address.c_str(), 256);
	std::mbstowcs(password, server_pass.c_str(), 256);


	CManager manager;
	RedisClient client(redis_host_main, redis_port_main, redis_host_trade, redis_port_trade);
	std::cout<<"Redis initialized done"<<std::endl;
	client.setConfigParams(redis_retry_time_in_second, redis_publish_channel, redis_key_prefix,is_error_log_enable, error_login);
	//HTTPRequest request(call_back_url);

	Kafka kafka_client(kafka_topic);
	kafka_client.setParams(kafka_properties);

	// Log initialization information
	logger->info("Initializing MT5 client...");
	std::cout<<"Initializing MT5 client..."<<std::endl;

	try {
		if (manager.Initialize()) {
			logger->info("Inited server successfull");
			std::cout<<"Inited server successfull"<<std::endl;


			if (manager.Login(server, login, password)) {
				logger->debug("Successfully logged in: {}", login);
				std::cout<<"Successfully logged in: "<<login<<std::endl;

				
				if (!kafka_client.connect()) {
					logger->error("Failed to connect to Kafka server");
					std::cout<<"Failed to connect to Kafka server"<<std::endl;

					return -1;
				}
				logger->debug("Kafka enabled");
				std::cout<<"Kafka enabled"<<std::endl;

				
				if (!client.Connect()) {
					logger->error("Failed to connect to redis server");
					std::cout<<"Failed to connect to redis server"<<std::endl;

					return -1;
				}
				logger->debug("Redis enabled");
				std::cout<<"Redis enabled"<<std::endl;



				MyDealSink dealSink(client, manager, kafka_client);
				MTAPIRES deaLres = manager.m_manager->DealSubscribe(&dealSink);

				if (deaLres != MT_RET_OK) {
					logger->error("Failed to subscribe to deal events, error code {} ", deaLres);
					std::cout<<"Failed to subscribe to deal events"<<std::endl;

					return 1;
				}

				MyUserSink myUserSink(client, manager, kafka_client);
				MTAPIRES userSink=manager.m_manager->UserSubscribe(&myUserSink);

				if (userSink != MT_RET_OK) {
					logger->error("Failed to subscribe to tick events, error code {}", userSink);
					std::cout<<"Failed to subscribe to tick events"<<std::endl;

					return 1;
				}

				MyPositionSink  positionSink(client, manager, kafka_client);
				MTAPIRES posSink = manager.m_manager->PositionSubscribe(&positionSink);

				if (posSink != MT_RET_OK) {
					logger->error("Failed to subscribe to tick events, error code {}", posSink);
					std::cout<<"Failed to subscribe to tick events"<<std::endl;

					return 1;
				}


				MTAPIRES result = manager.m_manager->SelectedAddAll();

				uint64_t prevTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

				NV::InterruptHandler::waitForUserInterrupt();

				manager.Logout();

			}
			else {
				logger->error("Login failed {} ", login);
				std::cout<<"Login failed: "<<login<<std::endl;

			}
		}
		else {
			logger->error("Initialize failed");
			std::cout<<"Initialize failed."<<std::endl;

		}
	}
	catch (int e) {
		cout << "An exception occurred. Exception number: " << e << endl;
		exit(1);
	}

}
