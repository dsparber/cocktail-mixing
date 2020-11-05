#include <igl/writeOFF.h>
#include "Gui.h"
#include "../include/SphSimulation.h"
#include "../include/Fluid.h"
#include "../include/FluidDefinitons.h"

/*
 * This class is a GUI for our dummy simulation. It extends the basic GUI
 * defined in Gui.h. We could add more controls and visuals here, but we don't
 * need any additional functionality for this dummy simulation.
 */
class MainGui : public Gui {
public:
	SphSimulation *simulation = nullptr;
	std::vector<Fluid> m_fluids;
	double m_kernelRadius;
	double m_gridWidth;
	double m_dt;

	MainGui() {
		m_dt = 0.008;

		// initialize GUI variables
		m_fluids.push_back(fluids::waterBlock());

		m_kernelRadius = 0.2;
		m_gridWidth = 0.2;
		
		// create simulation
		simulation = new SphSimulation();
		simulation->init();

		// set this simulation as the simulation that is running in our GUI
		setSimulation(simulation);

		// start the GUI
		start();
	}

	void drawSimulationParameterMenu() override {

		// Simulation GUI
		ImGui::InputDouble("Simulation dt", &m_dt);
		
		// Fluid GUI
		if (ImGui::Button("Add fluid", ImVec2(-1, 0))) {
			m_fluids.push_back(fluids::waterBlock());
		}
		
		for(int i = 0; i < m_fluids.size(); i++) {

			std::string header = "Fluid " + std::to_string(i);
			if (ImGui::CollapsingHeader(header.c_str(),
				ImGuiTreeNodeFlags_DefaultOpen)) {
				auto& fluid = m_fluids[i];
				// fluid properties
				ImGui::InputDouble("Stiffness", &fluid.m_stiffness);
				ImGui::InputDouble("Viscosity", &fluid.m_viscosity);
				ImGui::InputDouble("Particle Mass", &fluid.m_particleMass);
				ImGui::InputDouble("Rest Density", &fluid.m_restDensity);
				
				// particle arrangement
				ImGui::InputDouble("Particle distance", &fluid.m_initialSpacing);

				ImGui::InputInt("Particels: x", &fluid.m_initialDimension[0]);
				ImGui::InputInt("Particles: y", &fluid.m_initialDimension[1]);
				ImGui::InputInt("Particles: z", &fluid.m_initialDimension[2]);

				ImGui::InputDouble("Offset: x", &fluid.m_initialOffset[0]);
				ImGui::InputDouble("Offset: y", &fluid.m_initialOffset[1]);
				ImGui::InputDouble("Offset: z", &fluid.m_initialOffset[2]);

			}
		}

		// SPH GUI
		ImGui::InputDouble("Kernel Radius", &m_kernelRadius);
		ImGui::InputDouble("Grid Width", &m_gridWidth);

	}

	void updateSimulationParameters() override {
		// Simulation parameter
 		simulation->setTimestep(m_dt);
		// FluidSimulation parameter
		simulation->updateFluids(m_fluids);
		// SPH parameter
		simulation->setKernelRadius(m_kernelRadius);
		simulation->setGridWidth(m_gridWidth);
	};
};

int main(int argc, char *argv[]) {
	// create a new instance of the GUI for the dummy simulation
	new MainGui();

	return 0;
}