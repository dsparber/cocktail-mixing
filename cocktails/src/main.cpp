#include <igl/writeOFF.h>
#include "Gui.h"
#include "../include/SphSimulation.h"
#include "../include/BlockSource.h"
#include "../include/GeneratingSource.h"
#include "../include/FluidDefinitons.h"
#include "../include/BoxScene.h"
/*
 * This class is a GUI for our dummy simulation. It extends the basic GUI
 * defined in Gui.h. We could add more controls and visuals here, but we don't
 * need any additional functionality for this dummy simulation.
 */
class MainGui : public Gui {
public:
	SphSimulation *simulation = nullptr;
    double m_scale;

	MainGui() {
        m_scale = 1.0;

        simulation = new SphSimulation();
        simulation->m_fluids = fluids::all;
        simulation->m_sources.push_back(new GeneratingSource(fluids::water));

		setSimulation(simulation);
        simulation->init();
		start();
	}

	void drawSimulationParameterMenu() override {

		// Simulation GUI
		ImGui::InputDouble("Simulation dt", &simulation->m_dt);

        // SPH GUI
        ImGui::InputDouble("Kernel Radius", &simulation->m_kernelRadius);
        ImGui::InputDouble("Grid Width", &simulation->m_gridWidth);

        if(ImGui::CollapsingHeader("Boundary Box")) {
            ImGui::InputDouble("Box scale", &m_scale);
            if(ImGui::Button("Reset boundary", ImVec2(-1, 0))) {
                simulation->m_scene = new BoxScene(Eigen::Vector3d(0,0,0), Eigen::Vector3d(m_scale, m_scale, m_scale));
            }
        }

        if (ImGui::CollapsingHeader("Fluids")) {
            for (auto &fluid : fluids::all) {
                if (ImGui::CollapsingHeader(fluid->m_name.c_str())) {
                    // fluid properties
                    ImGui::InputDouble("Stiffness", &fluid->m_stiffness);
                    ImGui::InputDouble("Viscosity", &fluid->m_viscosity);
                    ImGui::InputDouble("Particle Mass", &fluid->m_particleMass);
                    ImGui::InputDouble("Rest Density", &fluid->m_restDensity);
                }
            }
        }

        if (ImGui::CollapsingHeader("Sources")) {


            if (ImGui::Button("Add block source", ImVec2(-1, 0))) {
                simulation->m_sources.push_back(new BlockSource(fluids::water));
                simulation->m_sources.back()->init();
            }

            if (ImGui::Button("Add generating source", ImVec2(-1, 0))) {
                simulation->m_sources.push_back(new GeneratingSource(fluids::water));
                simulation->m_sources.back()->init();
            }

            if (ImGui::Button("Remove sources", ImVec2(-1, 0))) {
                simulation->m_sources.clear();
            }

            int i = 0;
            for (auto &source : simulation->m_sources) {

                if (ImGui::CollapsingHeader(("Source " + to_string(++i) + ": " + source->toString()).c_str())) {

                    auto blockSource = dynamic_cast<BlockSource *>(source);
                    if (blockSource != nullptr) {
                        ImGui::InputDouble("Particle distance", &blockSource->m_initialSpacing);

                        ImGui::InputInt("Particels: x", &blockSource->m_initialDimension[0]);
                        ImGui::InputInt("Particles: y", &blockSource->m_initialDimension[1]);
                        ImGui::InputInt("Particles: z", &blockSource->m_initialDimension[2]);

                        ImGui::InputDouble("Offset: x", &blockSource->m_initialOffset[0]);
                        ImGui::InputDouble("Offset: y", &blockSource->m_initialOffset[1]);
                        ImGui::InputDouble("Offset: z", &blockSource->m_initialOffset[2]);
                    }

                    auto generatingSource = dynamic_cast<GeneratingSource *>(source);
                    if (generatingSource != nullptr) {
                        ImGui::InputInt("Max. particles", &generatingSource->m_maxParticles);
                        ImGui::InputDouble("Particles per second", &generatingSource->m_particlesPerSecond);

                        ImGui::InputDouble("Position: x", &generatingSource->m_position[0]);
                        ImGui::InputDouble("Position: y", &generatingSource->m_position[1]);
                        ImGui::InputDouble("Position: z", &generatingSource->m_position[2]);
                        ImGui::InputDouble("Position: std. deviation", &generatingSource->m_positionStdDeviation);

                        ImGui::InputDouble("Particle Velocity: x", &generatingSource->m_particleVelocity[0]);
                        ImGui::InputDouble("Particle Velocity: y", &generatingSource->m_particleVelocity[1]);
                        ImGui::InputDouble("Particle Velocity: z", &generatingSource->m_particleVelocity[2]);
                        ImGui::InputDouble("Particle Velocity: std. deviation", &generatingSource->m_particleVelocityStdDeviation);
                    }

                }
            }
        }



	}

	void updateSimulationParameters() override {

	};
};

int main(int argc, char *argv[]) {
	// create a new instance of the GUI for the dummy simulation
	new MainGui();

	return 0;
}