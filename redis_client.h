#include "stdafx.h"
#include <atomic>
#include <thread>
#include "ThreadSafeUnorderedMap.h"
#include "ThreadSafeQueue.h"

#define REDIS_CONFIRMATION_CHANNEL "REDIS_TRADE_UPDATE_PUB"

struct RedisData {
    RedisData() {}
    RedisData(std::string _payload, int retry, u_int64 tm, std::string key) {
        payload = _payload;
        retry_num = retry;
        last_submit_time = tm;
        unique_key = key;
    }
    std::string payload;
    int retry_num;
    u_int64 last_submit_time;
    std::string unique_key;
};

class RedisClient {

public:
    RedisClient(const std::string& server, const int port);
    RedisClient(const std::string& server, const int port, const std::string& tserver, const int tport);
    ~RedisClient(void);

    bool    Connect();
    void    Disconnect();
    bool    CacheSet(const std::string& key, const std::string& value);
    std::vector<std::string>    CacheGetList(const std::string& key);
    std::vector<std::pair<std::string, std::string>> RedisClient::CacheGetListByPattern(const std::string& pattern);
    bool    CacheSetMulti(const std::string& key, const std::vector<std::string>& values);
    bool    CacheClearSingle(const std::string& key);
    bool    CacheClearMulti(const std::string& id);
    bool    CacheGetKeys(const std::string& pattern, std::vector<std::string>& keys);
    bool    CacheExists(const std::string& key, bool& exists);
    bool    CacheSAdd(const std::string& key, const std::string& value);
    bool    CacheClearPattern(const std::string& keyPattern);
    void Subscribe(const std::string& channel);
    bool Publish(const std::string& channel, const std::string& unique_id, const std::string& message, int retryNum = 0);
    void setConfigParams(int retry_time, std::string channel, std::string key_prefix, int _is_error_log_enable, u_int64 _login);

private:
    void runFailDataPushThread();
    void runPubSubDataPushThread();

private:
    redisContext* m_redis; // For pubsub publish
    std::string m_server;
    int m_port;

    redisContext* t_redis; // For retrying the failed data
    std::string t_server;
    int t_port;

    redisContext* c_redis; // For pushing data to cache

    std::atomic <bool> redisPubSubThreadRunning;
    std::thread redisPubSubThread;

    std::atomic <bool> failDataRetryThreadRunning;
    std::thread failDataRetryThread;

    std::atomic <bool> pubsubDataPushThreadRunning;
    std::thread pubsubDataPushThread;

    //ThreadSafeUnorderedMap <std::string, RedisData> pubsubMap;
    ThreadSafeQueue <RedisData> *Queue;

    std::atomic <int> successCount = 0;
    std::atomic <int> failCount = 0;

    int redis_retry_time_in_second;
    std::string redis_publish_channel;
    std::string redis_key_prefix;
    int is_error_log_enable;
    u_int64 login;
};
