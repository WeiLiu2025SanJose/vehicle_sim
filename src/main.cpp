#include "Simulation.h"
#include <iostream>

int main(int argc, char* argv[]) {
    int durationSec = 20; // default duration

    // ---------------------------------------------------
    // Parse command line argument for timer duration
    // ---------------------------------------------------
    if (argc > 1) {
        try {
            durationSec = std::stoi(argv[1]);
            if (durationSec <= 0) {
                std::cerr << "Invalid duration: must be positive. Using default (20s).\n";
                durationSec = 20;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid argument: " << argv[1]
                      << " — using default duration (20s).\n";
            durationSec = 20;
        }
    }

    std::cout << "Starting simulation for " << durationSec << " seconds...\n";

    // ---------------------------------------------------
    // Initialize and run simulation
    // ---------------------------------------------------
    const int chargeStations = 3;
    Simulation sim(chargeStations);
    sim.runSimulation(std::chrono::seconds(durationSec));

    std::cout << "Simulation completed.\n";
    return 0;
}
