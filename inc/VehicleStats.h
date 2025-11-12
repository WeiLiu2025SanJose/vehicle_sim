#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "BaseStats.h"
#include "Vehicle.h"
#include <mutex>

//Singleton Vehicle Stats to store all the accumulated stat per type to statsMap
class VehicleStatsManager {
public:
    static VehicleStatsManager& getInstance();
    // Record a vehicle's run
    void record(const std::string& type, double ratio, const Vehicle& v);
    // it will set to common VehicleStatsData, it may be set to specific stat data per type
    void setStatData(const std::string& type, std::unique_ptr<BaseStats> stats);

    // print all stats
    void printAll();
    // flush to file
    //int flush(const string& fileName);
private:
        VehicleStatsManager() = default;
    ~VehicleStatsManager() = default;
    VehicleStatsManager(const VehicleStatsManager&) = delete;
    VehicleStatsManager& operator=(const VehicleStatsManager&) = delete; 
    VehicleStatsManager(VehicleStatsManager&&) = delete;          
    VehicleStatsManager& operator=(VehicleStatsManager&&) = delete;
protected:
    // Map vehicle type -> polymorphic stats object for each type
    std::unordered_map<std::string, std::unique_ptr<BaseStats>> statsMap;
    std::mutex statsMutex;
};

