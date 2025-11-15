#include "ChargeStationManager.h"
#include <iostream>

ChargeStationManager::ChargeStationManager(int totalStations)
    : availableStations(totalStations) {}

void ChargeStationManager::acquire(std::atomic<bool>& stopFlag) {
    std::unique_lock<std::mutex> lock(mtx);
    
    cv.wait(lock, [this, &stopFlag] {
        return stopFlag.load() || availableStations > 0;
    });

    if (stopFlag.load()) return;
    
    --availableStations;
}

void ChargeStationManager::release() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ++availableStations;
    }
    cv.notify_one();
}

int ChargeStationManager::getAvailable() const {
    std::lock_guard<std::mutex> lock(mtx);
    return availableStations;
}
void ChargeStationManager::stopAll()
{
    // wake all threads stuck in acquire()
    cv.notify_all();
}
