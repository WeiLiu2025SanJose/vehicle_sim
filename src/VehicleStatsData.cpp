#include "VehicleStatsData.h"
#include "Vehicle.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>

void VehicleStatsData::record(const Vehicle& v,StatType type) {
    std::lock_guard<std::mutex> lock(statsMutex);
    switch (type) {
        case StatType::TotalTestVehicle:
            totalTestVehicle++;
            averageTime = totalTestVehicle!=0?totalTime / totalTestVehicle:0;
            break;

        case StatType::TotalTime:
            totalTime += v.getRunningTime();
            averageTime = totalTestVehicle!=0?totalTime / totalTestVehicle:0;
            totalDistance=totalTime*v.getCruiseSpeed()/3600;
            averageDistance = totalDistance/totalTestVehicle;
            totalFaults=totalTime*v.getFaultPerHour()/3600;
            totalPassengersMiles=totalTime*v.getPassengers()*v.getCruiseSpeed()/3600;
            break;

        case StatType::TotalChargeCycle:
            totalChargedVehicle++;;
	    averageChargeTime = totalChargedVehicle!=0?totalChargeTime / totalChargedVehicle:0;
            break;

        case StatType::TotalChargeTime:
	    totalChargeTime += v.getChargingTime();
	    averageChargeTime = totalChargedVehicle!=0?totalChargeTime / totalChargedVehicle:0;	 
            break;

        default:
            break;
    }
}

void VehicleStatsData::log(const std::string& type) const {
    std::lock_guard<std::mutex> lock(statsMutex);

    // Format the log line
    std::ostringstream oss;
    oss << type
        << " → averageTime: " << averageTime << " s"
        << " totalTestVehicle: " << totalTestVehicle
        << " totalChargedVehicle: " << totalChargedVehicle
        << " averageDistance: " << averageDistance << " miles"
        << " averageChargeTime: " << averageChargeTime << " s"
        << " totalFaults: " << totalFaults
        << " totalPassengersMiles: " << totalPassengersMiles <<" miles";

    std::string line = oss.str();

    // console print ----
    std::cout << line << std::endl;

    // file logging
    std::ofstream out("stats_log.txt", std::ios::app);
    if (out) {
        out << line << "\n";
    }
}
