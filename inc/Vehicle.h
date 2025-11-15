#pragma once
#include <string>
#include <functional>

//Represents a single electric vehicle in simulation.
class Vehicle {
public:
    Vehicle(const std::string& type, int speed, int capacity, double timeHours,
            double energy, int passenger, double fault);
    virtual ~Vehicle() = default;

    // run function to Simulate running
    void run();

    // charge func to Simulate charging
    void charge();

    // battery running out
    bool needsCharge() const;
    // charging complete
    bool isFullyCharged() const;   

    // getters 
    const std::string& getType() const { return vehicleType; }
    int getCruiseSpeed() const { return cruiseSpeed; }
    int getBatteryCapacity() const { return batteryCapacity; }
    double getTimeToCharge() const { return timeToCharge; } // hours
    double getEnergyUse() const { return energyUse; }
    int getPassengers() const { return passengers; }
    double getFaultPerHour() const { return faultPerHour; }
    double getRunningTime() const{ return runningTime; }
    double getChargingTime() const { return chargingTime; }
    
    void setTimeSliceMs(int ms) { timeSliceMs = ms; }
    
    // reset running time or charging time when new cycle start
    void resetRunningTime() { runningTime=0; }
    void resetChargingTime() { chargingTime=0; }

private:
    //Vehicle parameter
    std::string vehicleType;
    int cruiseSpeed; //mph
    int batteryCapacity; //kwh
    double timeToCharge; // hours
    double energyUse; //kwh/miles
    int passengers;
    double faultPerHour;
    
    //Runtime status
    double runningTime=0;
    double chargingTime=0;
    double batteryRatio=1.0; 
    int timeSliceMs = 100;
protected:
    // may override by concrete vehicle
    virtual void registerStats(); 
#ifdef UNIT_TESTING
    friend class VehicleRegisterStatsTest;
    friend class VehicleStatsManagerTest;
    friend class RunnerLogicTest;
#endif
};

