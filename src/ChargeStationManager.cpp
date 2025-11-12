#include "ChargeStationManager.h"

ChargeStationManager::ChargeStationManager(int totalStations)
    : availableStations(totalStations) {}

void ChargeStationManager::acquire(const std::string& carName) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return availableStations > 0; });
    --availableStations;
    std::cout << carName << " acquired a charging station. "
              << availableStations << " left.\n";
}

void ChargeStationManager::release(const std::string& carName) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ++availableStations;
        std::cout << carName << " released a charging station. "
                  << availableStations << " available now.\n";
    }
    cv.notify_one();
}
