#include "Vehicle.h"
#include "VehicleStats.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "VehicleStatsData.h"

using namespace std;
using namespace std::chrono;

Vehicle::Vehicle(const string& type,int speed, int capacity, double time, double energy, int passenger, double fault)
    : vehicleType(type), cruiseSpeed(speed), batteryCapacity(capacity),
      timeToCharge(time), energyUse(energy), passengers(passenger),
      faultPerHour(fault) {}
      
// Base class registers standard VehicleStatsData
void Vehicle::registerStats() {
    auto& vs = VehicleStatsManager::getInstance();
    vs.setStatData(vehicleType, std::make_unique<VehicleStatsData>());
}

void Vehicle::run(atomic<bool>& stopFlag, ChargeStationManager& stationManager) {
    while (!stopFlag) {
        double driveSec = 3600 * (batteryCapacity / energyUse) / cruiseSpeed;
        double elapsed = 0;
        while (elapsed < driveSec && !stopFlag) {
            double slice = min(1.0, driveSec - elapsed);
            this_thread::sleep_for(duration<double>(slice));
            elapsed += slice;
        }
        double ratio = stopFlag ? elapsed / driveSec : 1.0;
        VehicleStatsManager::getInstance().record(vehicleType, ratio, *this);
        if (!stopFlag)
            charge(stationManager);
    }
}

void Vehicle::charge(ChargeStationManager& stationManager) {
    stationManager.acquire(vehicleType);
    this_thread::sleep_for(chrono::milliseconds(100));
    stationManager.release(vehicleType);
}

