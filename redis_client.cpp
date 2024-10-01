#include "stdafx.h"
#include "redis_client.h"
#include "logger.h"
#include <chrono>
#include <vector>
#include <stdio.h>

using namespace std::chrono;

#define MAX_DATA_PUSH_RETRY 5

RedisClient::RedisClient(const std::string& server, const int port) : m_redis(nullptr), m_server(server), m_port(port) {
    redisPubSubThreadRunning = false;
    failDataRetryThreadRunning = false;
    successCount = 0;
    failCount = 0;
    Queue = new ThreadSafeQueue<RedisData>(30000);
}

RedisClient::RedisClient(const std::string& server, const int port, const std::string& tserver, const int tport) : m_redis(nullptr), m_server(server), m_port(port), t_server(tserver), t_port(tport) {
    redisPubSubThreadRunning = false;
    failDataRetryThreadRunning = false;
    successCount = 0;
    failCount = 0;
    Queue = new ThreadSafeQueue<RedisData>(30000);
}

RedisClient::~RedisClient() { 
    if (redisPubSubThreadRunning.load()) {
      redisPubSubThreadRunning = false;
      if (redisPubSubThread.joinable()) {
          redisPubSubThread.join();
      }
    }

    if (failDataRetryThreadRunning.load()) {
        failDataRetryThreadRunning = false;
        if (failDataRetryThread.joinable()) {
            failDataRetryThread.join();
        }
    }

    if (pubsubDataPushThreadRunning.load()) {
        pubsubDataPushThreadRunning = false;
        if (pubsubDataPushThread.joinable()) {
            pubsubDataPushThread.join();
        }
    }

    Disconnect(); 
}

bool RedisClient::Connect() {
    m_redis = redisConnect(m_server.c_str(), m_port);
    if (m_redis == NULL || m_redis->err) {
        Logger::get()->error("Disconnected from Redis server at {} : {} ", m_server, m_port);
        std::cerr << "Error connecting to Redis: " << m_redis->errstr << std::endl;
        return false;
    }
    std::cout << "Connected to Redis server at " << m_server << ":" << m_port << std::endl;

    t_redis = redisConnect(t_server.c_str(), t_port);
    if (t_redis == NULL || t_redis->err) {
        Logger::get()->error("Disconnected from Redis server at {} : {} ", t_server, t_port);
        std::cerr << "Error connecting to Redis: " << t_redis->errstr << std::endl;
        return false;
    }

    c_redis = redisConnect(t_server.c_str(), t_port);
    if (c_redis == NULL || c_redis->err) {
        Logger::get()->error("Disconnected from Redis server at {} : {} ", t_server, t_port);
        std::cerr << "Error connecting to Redis: " << c_redis->errstr << std::endl;
        return false;
    }
    std::cout << "Connected to Redis server at " << t_server << ":" << t_port << std::endl;

    //Subscribe(REDIS_CONFIRMATION_CHANNEL);
    
    //runPubSubDataPushThread();
    //runFailDataPushThread();
    return true;
}

void RedisClient::Disconnect() {
    if (m_redis != nullptr) {
        redisFree(m_redis);
        m_redis = nullptr;
        Logger::get()->error("Disconnected from Redis server at {} : {} ", m_server, m_port);
        std::cout << "Disconnected from Redis server at " << m_server << ":" << m_port << std::endl;
    }

    if (t_redis != nullptr) {
        redisFree(t_redis);
        t_redis = nullptr;
    }

    if (c_redis != nullptr) {
        redisFree(c_redis);
        c_redis = nullptr;
    }
}

bool RedisClient::CacheSet(const std::string& key, const std::string& value) {
    if (c_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }
    // Push data to trade redis server
    redisReply* reply = (redisReply*)redisCommand(c_redis, "SET %s %s", key.c_str(), value.c_str());
    if (reply == nullptr) {
        std::cerr << "Error setting cache value: " << c_redis->errstr << std::endl;
        exit(100);
        return false;
    }
    bool success = (reply->type == REDIS_REPLY_STATUS && strcmp(reply->str, "OK") == 0);
    freeReplyObject(reply);

    return success;
}
std::vector<std::string> RedisClient::CacheGetList(const std::string& key) {
    std::vector<std::string> values;
    if (t_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return values;
    }
    redisReply* reply = (redisReply*)redisCommand(t_redis, "LRANGE %s 0 -1", key.c_str());
    if (reply == nullptr) {
        std::cerr << "Error getting cache value: " << t_redis->errstr << std::endl;
        return values;
    }
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (int i = 0; i < reply->elements; i++) {
            std::string value(reply->element[i]->str, reply->element[i]->len);
            values.push_back(value);
        }
    }
    else if (reply->type == REDIS_REPLY_NIL) {
        std::cout << "Key " << key << " does not exist in cache" << std::endl;
    }
    else {
        std::cerr << "Unexpected reply type" << std::endl;
    }
    freeReplyObject(reply);
    return values;
}

std::vector<std::pair<std::string, std::string>> RedisClient::CacheGetListByPattern(const std::string& pattern) {
    std::vector<std::pair<std::string, std::string>> keyValuePairs;
    if (t_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return keyValuePairs;
    }
    redisReply* reply = (redisReply*)redisCommand(t_redis, "KEYS %s", pattern.c_str());
    if (reply == nullptr) {
        std::cerr << "Error getting keys matching pattern: " << t_redis->errstr << std::endl;
        return keyValuePairs;
    }
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (int i = 0; i < reply->elements; i++) {
            std::string key(reply->element[i]->str, reply->element[i]->len);
            redisReply* valueReply = (redisReply*)redisCommand(t_redis, "GET %s", key.c_str());
            if (valueReply != nullptr && valueReply->type == REDIS_REPLY_STRING) {
                std::string value(valueReply->str, valueReply->len);
                keyValuePairs.push_back(std::make_pair(key, value));
            }
            freeReplyObject(valueReply);
        }
    }
    else if (reply->type == REDIS_REPLY_NIL) {
        std::cout << "No keys matching pattern: " << pattern << std::endl;
    }
    else {
        std::cerr << "Unexpected reply type " << reply->type << std::endl;
    }
    freeReplyObject(reply);
    return keyValuePairs;
}

bool RedisClient::CacheSetMulti(const std::string& key, const std::vector<std::string>& values) {
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    // Delete the existing list (if any)
    redisReply* reply = (redisReply*)redisCommand(m_redis, "DEL %s", key.c_str());
    if (reply == nullptr) {
        std::cerr << "Error deleting cache value: " << m_redis->errstr << std::endl;
        return false;
    }
    freeReplyObject(reply);

    // Construct the command to add all values to the new list
    std::string command = "RPUSH " + key + " ";
    for (const auto& value : values) {
        command += value + " ";
    }

    // Execute the command to create the new list
    reply = (redisReply*)redisCommand(m_redis, command.c_str());
    if (reply == nullptr) {
        std::cerr << "Error setting cache value: " << m_redis->errstr << std::endl;
        return false;
    }

    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);

    return success;
}

bool RedisClient::CacheClearSingle(const std::string& key) {

    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    redisReply* reply = (redisReply*)redisCommand(m_redis, "DEL %s", key.c_str());
    if (reply == nullptr) {
        std::cerr << "Error deleting cache value: " << m_redis->errstr << std::endl;
        return false;
    }

    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1);
    freeReplyObject(reply);

    return success;
}

bool RedisClient::CacheClearPattern(const std::string& keyPattern) {
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    // Get all keys matching the pattern
    redisReply* reply = (redisReply*)redisCommand(m_redis, "KEYS %s", keyPattern.c_str());
    if (reply == nullptr || reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Error retrieving keys for pattern " << keyPattern << ": " << m_redis->errstr << std::endl;
        freeReplyObject(reply);
        return false;
    }

    // Delete all keys and their associated values
    for (int i = 0; i < reply->elements; i++) {
        redisReply* delReply = (redisReply*)redisCommand(m_redis, "DEL %s", reply->element[i]->str);
        if (delReply == nullptr || delReply->type != REDIS_REPLY_INTEGER || delReply->integer != 1) {
            std::cerr << "Error deleting key " << reply->element[i]->str << ": " << m_redis->errstr << std::endl;
            freeReplyObject(delReply);
            freeReplyObject(reply);
            return false;
        }
        freeReplyObject(delReply);
    }
    freeReplyObject(reply);

    return true;
}

bool RedisClient::CacheClearMulti(const std::string& id) {
    std::string key = "orders:" + id;

    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    // Delete the existing list
    redisReply* reply = (redisReply*)redisCommand(m_redis, "DEL %s", key.c_str());
    if (reply == nullptr) {
        std::cerr << "Error deleting cache value: " << m_redis->errstr << std::endl;
        return false;
    }
    freeReplyObject(reply);

    return true;
}

bool RedisClient::CacheGetKeys(const std::string& pattern, std::vector<std::string>& keys) {
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    redisReply* reply = (redisReply*)redisCommand(m_redis, "KEYS %s", pattern.c_str());
    if (reply == nullptr) {
        std::cerr << "Error getting cache keys: " << m_redis->errstr << std::endl;
        return false;
    }

    if (reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Error getting cache keys: unexpected reply type" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    for (size_t i = 0; i < reply->elements; i++) {
        keys.push_back(reply->element[i]->str);
    }

    freeReplyObject(reply);

    return true;
}

bool RedisClient::CacheExists(const std::string& key, bool& exists) {
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    redisReply* reply = (redisReply*)redisCommand(m_redis, "EXISTS %s", key.c_str());
    if (reply == nullptr) {
        std::cerr << "Error checking cache existence: " << m_redis->errstr << std::endl;
        return false;
    }

    exists = (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1);
    freeReplyObject(reply);

    return true;
}

bool RedisClient::CacheSAdd(const std::string& key, const std::string& value) {
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }
    redisReply* reply = (redisReply*)redisCommand(m_redis, "SADD %s %s", key.c_str(), value.c_str());
    if (reply == nullptr) {
        std::cerr << "Error adding value to set: " << m_redis->errstr << std::endl;
        return false;
    }
    freeReplyObject(reply);

    return true;
}

void RedisClient::Subscribe(const std::string& channel) {
    if (redisPubSubThreadRunning.load()) {
        std::cout << "Redis Pub-Sub thread already running" << std::endl;
        return;
    }
    redisContext* l_context = redisConnect(m_server.c_str(), m_port);
    if (l_context == nullptr || l_context->err) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return;
    }

    redisReply* reply = (redisReply*)redisCommand(l_context, "SUBSCRIBE %s", channel.c_str());
    if (reply == nullptr) {
        std::cout << "Failed to subscribe" << std::endl;
        return;
    }
    freeReplyObject(reply);
    std::cout << "Starting Redis data push success confirmation thread" << std::endl;

    redisPubSubThreadRunning = true;
    redisPubSubThread = std::thread([&, channel, l_context] {
        
        while(redisPubSubThreadRunning.load()) {
            redisReply* reply = nullptr;

            int ret = redisGetReply(l_context, (void**)&reply);

            if (ret == REDIS_OK && reply != nullptr) {
                if (reply->type == REDIS_REPLY_ARRAY && reply->elements >= 3) {
                    std::string receivedChannel = reply->element[1]->str;
                    std::string message = reply->element[2]->str;

                    if (channel == receivedChannel) {
                        // Process the received message
                        successCount++; 
                        // pubsubMap.erase(message);
                    }
                }
            }
            freeReplyObject(reply);
        }
    });
}

bool RedisClient::Publish(const std::string& channel, const std::string& unique_id, const std::string& message, int retryNum) {
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return false;
    }

    // Publish the message to the specified channel
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    RedisData redisData = RedisData(message, retryNum, ms, unique_id);
    
    Queue->push(redisData);

    return true;
}

void RedisClient::runFailDataPushThread() {
    if (failDataRetryThreadRunning.load()) {
        std::cout << "Fail Data Push thread is already running." << std::endl;
        return;
    }

    uint64_t lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    std::cout << "Starting Fail Data Push thread" << std::endl;
    failDataRetryThreadRunning = true;
    failDataRetryThread = std::thread([&] {
        while(failDataRetryThreadRunning.load()) {
            std::string pattern = redis_key_prefix + "*";
            std::vector<std::pair<std::string, std::string>> tradeData = CacheGetListByPattern(pattern);
            printf("Failed data size: %d\n", tradeData.size());
            for (const auto& keyValue : tradeData) {
                const std::string& key = keyValue.first;
                const std::string& value = keyValue.second;
                
                Publish(redis_publish_channel, key, value, 0);
            }

            Sleep(redis_retry_time_in_second * 1000);
        }
    });
}

void RedisClient::runPubSubDataPushThread() {
    if (pubsubDataPushThreadRunning.load()) {
        std::cout << "Data push thread is already running\n";
        return;
    }
    if (m_redis == nullptr) {
        std::cerr << "Error: Redis client not connected" << std::endl;
        return;
    }
    std::cout << "Starting data push thread\n";
    pubsubDataPushThreadRunning = true;
    pubsubDataPushThread = std::thread([&] {
        while (pubsubDataPushThreadRunning.load()) {
            if (Queue->isEmpty()) {
                Sleep(5);
                continue;
            }
            RedisData data = Queue->Front();
            //printf("===> %s\n", data.payload.c_str());
            uint64_t  time1 = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            ).count();
            redisReply* reply = (redisReply*)redisCommand(m_redis, "PUBLISH %s %s", redis_publish_channel.c_str(), data.payload.c_str());

            std::string login_str = std::to_string(login);


            if (is_error_log_enable) {
                if (data.payload.find(login_str) != std::string::npos) {
                    std::cout << data.payload << std::endl;
                }
            }
            
            
            if (reply == nullptr || m_redis->err) {
                std::cerr << "Error publishing message: " << m_redis->errstr << std::endl;
            }
            uint64_t time2 = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            ).count();
            freeReplyObject(reply);

            if (!is_error_log_enable && (time2 - time1 > 10)) {
              std::cout << "Time to publish: " << time2 - time1 << std::endl;
            }

            int ret = CacheSet(data.unique_key, data.payload);
            if (!ret) {
                std::cout << "redis cache set failed for: " << data.payload << std::endl;
            }
            uint64_t time3 = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            ).count();

            if (!is_error_log_enable && (time3 - time2 > 10)) {
               std::cout << "Time to set Cache: " << time3 - time2 << " " << Queue->Size() << std::endl;
            }

            Queue->pop();
        }
    });
}

void RedisClient::setConfigParams(int retry_time, std::string channel, std::string key_prefix, int _is_error_log_enable, u_int64 _login) {
    redis_retry_time_in_second = retry_time;
    redis_publish_channel = channel;
    redis_key_prefix = key_prefix;
    is_error_log_enable=_is_error_log_enable;
    login=_login;
    std::cout << "Redis retry time: " << redis_retry_time_in_second << ", redis publish channel: " << redis_publish_channel << ", redis_key_prefix: " << redis_key_prefix << std::endl;
}
