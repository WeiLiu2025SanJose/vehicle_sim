#include "Vehicle.h"
#include "VehicleStatsManager.h"
#include "VehicleStatsData.h"
#include <algorithm>
#include <cmath>
using namespace std;

Vehicle::Vehicle(const std::string& type, int speed, int capacity, double timeHours,
                 double energy, int passenger, double fault)
    : vehicleType(type),
      cruiseSpeed(speed),
      batteryCapacity(capacity),
      timeToCharge(timeHours),
      energyUse(energy),
      passengers(passenger),
      faultPerHour(fault),
      batteryRatio(1.0),
      timeSliceMs(100)
{
    // registerStats be called by concrete vehicle constructors
}

void Vehicle::registerStats() {
    auto& vs = VehicleStatsManager::getInstance();
    vs.setStatData(getType(), std::make_unique<VehicleStatsData>());
}

void Vehicle::run() {
    double driveSec = 3600.0 * (this->getBatteryCapacity()/this->getEnergyUse() ) / this->getCruiseSpeed();
    runningTime++;
    if (runningTime >= driveSec) this->batteryRatio = 0.0;
}

void Vehicle::charge() {
    double timeToChargeSec = std::max(1.0, getTimeToCharge() * 3600.0);
    chargingTime++;
    if (chargingTime >= timeToChargeSec) this->batteryRatio = 1.0;
}

bool Vehicle::needsCharge() const {
    return batteryRatio <= 0.0;
}

bool Vehicle::isFullyCharged() const {
    return batteryRatio >= 1.0;
}
