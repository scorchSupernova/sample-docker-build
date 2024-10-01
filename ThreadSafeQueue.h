#pragma once

#include <mutex>


template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue(int size)
    {
        this->size = size;
        data = new T[size];
        front = 0;
        rear = 0;
        datasize = 0;
    }

    ~ThreadSafeQueue()
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        delete[] data;
        data = NULL;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        rear = front = size = 0;
        datasize = 0;
    }

    void push(T element)
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        data[front] = element;
        front = (front + 1) % size;
        if (datasize != size)
            datasize++;
    }

    bool isEmpty()
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        if (datasize == 0)
            return true;
        else
        {
            return false;
        }
    }

    int Size()
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        return datasize;
    }

    T Front()
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        T d = data[rear];
        return d;
    }

    void pop()
    {
        std::lock_guard<std::mutex> lock(guradMutex);
        rear = (rear + 1) % size;
        if (datasize != 0)
        {
            datasize--;
        }
    }

private:
    int size;
    int front;
    int rear;
    int datasize;
    T* data;
    std::mutex guradMutex;
};
