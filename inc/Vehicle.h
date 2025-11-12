#pragma once
#include <string>
#include <atomic>
#include <memory>
#include "ChargeStationManager.h"

// base vehicle class for the simulation test
class Vehicle {
public:
    Vehicle(const std::string& type, int speed, int capacity, double time,
            double energy, int passenger, double fault);
    virtual ~Vehicle()= default;

    virtual void run(std::atomic<bool>& stopFlag, ChargeStationManager& stationManager);
    virtual void charge(ChargeStationManager& stationManager);

    const std::string& getType() const { return vehicleType; }
    int getCruiseSpeed() const { return cruiseSpeed; }
    int getBatteryCapacity() const { return batteryCapacity; }
    double getTimeToCharge() const { return timeToCharge; }
    double getEnergyUse() const { return energyUse; }
    int getPassengers() const { return passengers; }
    double getFaultPerHour() const { return faultPerHour; }

protected:
    std::string vehicleType;
    int cruiseSpeed;
    int batteryCapacity;
    double timeToCharge;
    double energyUse;
    int passengers;
    double faultPerHour;
};
