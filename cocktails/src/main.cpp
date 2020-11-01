#include <igl/writeOFF.h>
#include "Gui.h"
#include "../include/SphSimulation.h"

/*
 * This class is a GUI for our dummy simulation. It extends the basic GUI
 * defined in Gui.h. We could add more controls and visuals here, but we don't
 * need any additional functionality for this dummy simulation.
 */
class MainGui : public Gui {
public:
	Simulation *simulation = nullptr;

	MainGui() {
		// create simulation
		simulation = new SphSimulation();
		simulation->init();

		// set this simulation as the simulation that is running in our GUI
		setSimulation(simulation);

		// start the GUI
		start();
	}

	void updateSimulationParameters() override {
		// We don't have any simulation parameters to update periodically so we
		// don't need to do anything here
	};
};

int main(int argc, char *argv[]) {
	// create a new instance of the GUI for the dummy simulation
	new MainGui();

	return 0;
}