#include "Simulation.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // default simulated seconds
    int durationSec = 2000;
    // default charging stations
    int stations = 3;
    // real ms per simulated second for test
    int timeSliceMs = 10;

    if (argc > 1) {
        try { durationSec = std::stoi(argv[1]); }
        catch (...) { durationSec = 20; }
    }
    if (argc > 2) {
        try { stations = std::stoi(argv[2]); }
        catch (...) { stations = 3; }
    }
    if (argc > 3) {
        try { timeSliceMs = std::stoi(argv[3]); }
        catch (...) { timeSliceMs = 100; }
    }

    std::cout << "Starting simulation for " << durationSec << " simulated seconds (real ms/slice=" << timeSliceMs << "), stations=" << stations << "\n";

    Simulation sim(stations, timeSliceMs);
    sim.runSimulation(std::chrono::seconds(durationSec));

    std::cout << "Simulation completed.\n";
    return 0;
}
