#include "VehicleStatsManager.h"
#include "VehicleStatsData.h"

#include <iostream>
#include <mutex>

void VehicleStatsData::record(double ratio, const Vehicle& v) {
    std::lock_guard<std::mutex> lock(statsMutex);
    totalTestVehicle += ratio;
    totalTime += ratio * 3600.0 * (v.getBatteryCapacity() / v.getEnergyUse()) / v.getCruiseSpeed();
    averageTime = totalTime/totalTestVehicle;
    totalDistance += ratio * (v.getBatteryCapacity() / v.getEnergyUse());
    averageDistance = totalDistance/totalTestVehicle;
    totalChargeTime += ratio * v.getTimeToCharge();
    averageChargeTime = totalChargeTime/totalTestVehicle;
    totalFaults += ratio * v.getFaultPerHour() * (v.getBatteryCapacity() / v.getEnergyUse()) / v.getCruiseSpeed();
    totalPassengersMiles += ratio * v.getPassengers() * (v.getBatteryCapacity() / v.getEnergyUse());
}

void VehicleStatsData::log(const std::string& type) const {
    std::cout << type << " → averageTime: " << averageTime
              << " averageDistance: " << averageDistance
              << " averageChargeTime: " << averageChargeTime
              << " totalFaults: " << totalFaults
              << " totalPassengersMiles: " << totalPassengersMiles << std::endl;
}

