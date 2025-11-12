#include "VehicleStats.h"
#include "VehicleStatsData.h"
#include <mutex>

VehicleStatsManager& VehicleStatsManager::getInstance() {
    static VehicleStatsManager instance;
    return instance;
}

// Record for a vehicle type
void VehicleStatsManager::record(const std::string& type, double ratio, const Vehicle& v) {
    std::lock_guard<std::mutex> lock(statsMutex);

    if (statsMap.find(type) == statsMap.end()) {
    
        // Safe: VehicleStatsData fully defined here
        statsMap[type] = std::make_unique<VehicleStatsData>();
    }
    statsMap[type]->record(ratio, v);
}
void VehicleStatsManager::setStatData(const std::string& type, std::unique_ptr<BaseStats> stats){
    std::lock_guard<std::mutex> lock(statsMutex);
    if (statsMap.find(type) == statsMap.end()) {
        statsMap[type] = std::move(stats);
    }
}
// Log all types
void VehicleStatsManager::printAll() {
    for (const auto& [type, stats] : statsMap) {
        stats->log(type);
    }
}
