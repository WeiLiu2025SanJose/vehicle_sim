#pragma once
#include "BaseStats.h"
#include <iostream>

// common Vehicle Stat dataset and the way to accumulate
class VehicleStatsData : public BaseStats {
public:
    double totalTime = 0;
    double averageTime = 0;
    double totalTestVehicle = 0;
    double totalDistance = 0;
    double averageDistance = 0;
    double totalChargeTime = 0;
    double averageChargeTime = 0;
    double totalFaults = 0;
    double totalPassengersMiles = 0;
    std::mutex statsMutex;
    
    void record(double ratio, const Vehicle& v) override;
    void log(const std::string& type) const override;
};
/* in case we have new vehicle with different stat data:
class FlyVehicleStatsData : public BaseStats {
public:
    double totalFlightHours = 0;
    double totalDistance = 0;
    double totalPassengerMiles = 0;
    double totalEnergyUsed = 0;
    double totalFaults = 0;
    mutable std::mutex statsMutex;

    void record(double ratio, const Vehicle& v) override {
        std::lock_guard<std::mutex> lock(statsMutex);
        double hours = ratio;
        totalFlightHours += hours;
        totalDistance += v.getCruiseSpeed() * hours;
        totalPassengerMiles += v.getPassengers() * v.getCruiseSpeed() * hours;
        totalEnergyUsed += v.getEnergyUse() * hours;
        totalFaults += v.getFaultPerHour() * hours;
    }

    void log(const std::string& type) const override {
        std::lock_guard<std::mutex> lock(statsMutex);
        std::cout<<"call fly"<<std::endl;
        std::cout << "\n=== " << type << " Vehicle Statistics ===\n"
                  << "Total Flight Hours: " << totalFlightHours << "\n"
                  << "Total Distance: " << totalDistance << " miles\n"
                  << "Total Passenger Miles: " << totalPassengerMiles << "\n"
                  << "Total Energy Used: " << totalEnergyUsed << "\n"
                  << "Total Faults: " << totalFaults << "\n";
    }
};*/
