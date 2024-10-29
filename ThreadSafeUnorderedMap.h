#include "stdafx.h"

#include <unordered_map>
#include <mutex>

template<typename Key, typename Value>
class ThreadSafeUnorderedMap {

public:
    void insert(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        map_[key] = value;
    }

    void erase(const Key& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        map_.erase(key);
    }

    Value& operator[](const Key& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_[key];
    }

    void iterate(std::function<void(const Key&, const Value&)> callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& pair : map_) {
            callback(pair.first, pair.second);
        }
    }
    
private:
    std::unordered_map <Key, Value> map_;
    std::mutex mutex_;
};

