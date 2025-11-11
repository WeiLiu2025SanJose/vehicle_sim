#pragma once
#include <string>
#include <mutex>
#include <condition_variable>
#include <iostream>

class ChargeStationManager {
public:
    explicit ChargeStationManager(int totalStations);

    void acquire(const std::string& carName);
    void release(const std::string& carName);

private:
    int availableStations;
    std::mutex mtx;
    std::condition_variable cv;
};
