#include "stdafx.h"
#include "Kafka.h"
#include <chrono>

using namespace std::chrono;

Kafka::Kafka() {
    Queue = new ThreadSafeQueue<TradeData>(30000);
    dataPushThreadRunning = false;
}

Kafka::Kafka(std::string _topic) {
    Queue = new ThreadSafeQueue<TradeData>(30000);
    dataPushThreadRunning = false;
    topic = _topic;
}

void Kafka::setParams(std::map <std::string, std::string> properties) {
    kafkaProperties = properties;
}

bool Kafka::connect() {
    char errstr[200];
    conf = rd_kafka_conf_new();
    for (auto prop : kafkaProperties) {
        std::cout << "Key: " << prop.first << ", Value: " << prop.second << std::endl;

        if (rd_kafka_conf_set(conf, prop.first.c_str(), prop.second.c_str(),
            errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
            printf("%% Error: %s\n", errstr);
            return false;
        }
    }

    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        printf("Failed to create new producer: %s", errstr);
        return false;
    }

    std::cout << "Kafka connection okay\n";
    runDataPushThread();
    return true;
}

void Kafka::pushData(std::string key, std::string payload) {
    TradeData tradeData = TradeData(payload, key);
    Queue->push(tradeData);
}

void Kafka::runDataPushThread() {
    if (dataPushThreadRunning.load() == true) {
        return;
    }
    std::cout << "Starting data push thread\n";
    dataPushThreadRunning = true;
    dataPushThread = std::thread([&] {
        while (dataPushThreadRunning.load()) {
            if (Queue->isEmpty()) {
                rd_kafka_flush(producer, 1000);
                Sleep(5);
                continue;
            }

            TradeData data = Queue->Front();
            uint64_t  time1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            int ret = rd_kafka_producev(producer, RD_KAFKA_V_TOPIC(topic.c_str()),
                RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                RD_KAFKA_V_KEY((void*)data.unique_key.c_str(), data.unique_key.size()),
                RD_KAFKA_V_VALUE((void*)data.payload.c_str(), data.payload.size()),
                RD_KAFKA_V_OPAQUE(NULL),
                RD_KAFKA_V_END);
            if (ret != 0) {
                std::cout << "============> Failed to push data with error code: " << ret << std::endl;
                continue;
            }
            uint64_t  time2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            if (time2 - time1 > 5) {
                std::cout << "Time to push data to kafka: " << time2 - time1 << " miliseconds" << std::endl;
            }
            Queue->pop();
        }

    });
}

Kafka::~Kafka() {
    if (dataPushThreadRunning.load()) {
        dataPushThreadRunning = false;
        if (dataPushThread.joinable()) {
            dataPushThread.join();
        }
    }
    rd_kafka_flush(producer, 10000);
    delete producer;
    delete conf;
}

