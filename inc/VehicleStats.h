#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

class Vehicle;

class VehicleStats {
public:
    void record(const std::string& type, double ratio, const Vehicle& v);
    void logAll() const;

private:
    struct Stats {
        double totalTime = 0;
        double totalTestVehicle = 0;
        double totalDistance = 0;
        double totalChargeTime = 0;
        double totalPassengersMiles = 0;
        double totalFaults = 0;
    };

    std::unordered_map<std::string, Stats> statsMap;
};
