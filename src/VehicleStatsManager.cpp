#include "VehicleStatsManager.h"
#include "VehicleStatsData.h"

VehicleStatsManager& VehicleStatsManager::getInstance() {
    static VehicleStatsManager instance;
    return instance;
}

void VehicleStatsManager::record(const std::string& type, const Vehicle& v,StatType statType) {
    std::lock_guard<std::mutex> lock(statsMutex);
    // check first if the type already is in the map, assume stat data will be same for one type.
    if (statsMap.find(type) == statsMap.end()) {
        statsMap[type] = std::make_unique<VehicleStatsData>();
    }
    statsMap[type]->record(v,statType);
}

void VehicleStatsManager::setStatData(const std::string& type, std::unique_ptr<BaseStats> stats) {
    std::lock_guard<std::mutex> lock(statsMutex);
    if (statsMap.find(type) == statsMap.end()) {
        statsMap[type] = std::move(stats);
    }
}

void VehicleStatsManager::printAll() {
    std::lock_guard<std::mutex> lock(statsMutex);
    for (const auto& kv : statsMap) {
        kv.second->log(kv.first);
    }
}
