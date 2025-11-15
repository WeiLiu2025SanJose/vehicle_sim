#pragma once
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

//Manages the charging station resources and synchronizes access using a mutex and condition variable.
class ChargeStationManager {
public:
    explicit ChargeStationManager(int totalStations);

    // Acquire blocks until a slot is available
    void acquire(std::atomic<bool>& stopFlag);
    void release();

    int getAvailable() const;
    void stopAll();

private:
    int availableStations;
    mutable std::mutex mtx;
    std::condition_variable cv;
};
