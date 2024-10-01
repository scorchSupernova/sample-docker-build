#ifndef KAFKA_H
#define KAFKA_H

#include "stdafx.h"
#include "ThreadSafeQueue.h"
#include <string>
#include <atomic>
#include <thread>

struct TradeData {
    TradeData() {}
    TradeData(std::string _payload, std::string key) {
        payload = _payload;
        unique_key = key;
    }
    std::string payload;
    std::string unique_key;
};

class Kafka {
public:
    Kafka ();
    Kafka(std::string _topic);
    ~Kafka ();
    void pushData(std::string key, std::string payload);
    void setParams(std::map <std::string, std::string> properties);
    bool connect();

private:
    void runDataPushThread();

private:
    rd_kafka_conf_t* conf;
    rd_kafka_t* producer;
    ThreadSafeQueue <TradeData>* Queue;
    std::string topic;

    std::atomic <bool> dataPushThreadRunning;
    std::thread dataPushThread;

    std::map <std::string, std::string> kafkaProperties;
};

#endif