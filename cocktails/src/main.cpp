#include <igl/writeOFF.h>
#include "Gui.h"
#include "../include/DCSPHSimulation.h"
#include "../include/BlockSource.h"
#include "../include/EmittingSource.h"
#include "../include/GeneratingSource.h"
#include "../include/CustomSource.h"
#include "../include/FluidDefinitons.h"
#include "../include/BoxScene.h"
#include "../include/SimulationLoader.h"

/*
 * This class is a GUI for our dummy simulation. It extends the basic GUI
 * defined in Gui.h. We could add more controls and visuals here, but we don't
 * need any additional functionality for this dummy simulation.
 */
class MainGui : public Gui {
public:
	SphSimulation *simulation = nullptr;
    int m_fluid_chooser;
    int m_solver_chooser;
    vector<string> m_solver_names;
    vector<string> m_fluid_names;
    Eigen::Vector3d m_scene_max;
    Eigen::Vector3d m_scene_min;

    string m_particles_init_file;
    string m_boundary_particles_path;

	MainGui() {
        m_fluid_chooser = 0;
        for(auto& fluid : fluids::all) {
            m_fluid_names.push_back(fluid->m_name);
        }

        m_solver_chooser = 0;
        m_solver_names = {"SPH", "DCSPH"};

        // m_scene_max << 100, 100, 100;
        // m_scene_min << -100, 0., -100;
        m_scene_max << 1, 10, 1;
        m_scene_min << -1, 0., -1;

        m_boundary_particles_path = "../../data/boundary.xyz";

        simulation = new DCSPHSimulation();
        simulation->init();

        simulation->m_sources.push_back(new CustomSource(fluids::boundary, m_boundary_particles_path));
        simulation->m_sources.back()->init();

        simulation->m_sources.push_back(new EmittingSource(fluids::water));
        simulation->m_sources.back()->init();

        // simulation->m_sources.push_back(new BlockSource(fluids::water, Eigen::Vector3i(40, 40, 40), 0.11, Eigen::Vector3d(-2, 8, -2)));
        // simulation->m_sources.back()->init();
        simulation->setScene(new BoxScene(m_scene_min, m_scene_max));

        setSimulation(simulation);
        start();
	}

	void drawSimulationParameterMenu() override {

        ImGui::Combo("Choose Solver:", &m_solver_chooser, m_solver_names);

        if(ImGui::Button("Confirm", ImVec2(-1, 0))) {
            SphSimulation* tmp = NULL;
            switch(m_solver_chooser){
                case 0:
                    tmp = new SphSimulation();
                    cout << "Switched to SPH\n";
                    break;
                case 1:
                    tmp = new DCSPHSimulation();
                    cout << "Switched to DCSPH\n";
                    break;
                default:
                    tmp = new SphSimulation();
            }

            delete simulation;
            simulation = NULL;
            simulation = tmp;
            setSimulation(simulation);
            simulation->init();

        }

		// Simulation GUI
		ImGui::InputDouble("Simulation dt", &simulation->m_dt);

        // SPH GUI
        ImGui::InputDouble("Kernel Radius", &simulation->m_kernelRadius);
        ImGui::InputDouble("Grid Width", &simulation->m_gridWidth);

        // Coloring
        ImGui::Checkbox("Particle/Fluid coloring", &simulation->m_use_particle_color);

        // Recording
        if(ImGui::CollapsingHeader("Custom Recording")) {


            if(ImGui::Button("Save Sim", ImVec2(-1, 0))) {
                SimulationLoader::saveSimulation("./test.sim", simulation);
            }

            if(ImGui::Button("Load Sim", ImVec2(-1, 0))) {
                simulation = SimulationLoader::loadSimulation("./test.sim");

                cout << "Simulation has " << simulation->m_sources.size() << "sources\n";

                for(auto fluid : simulation->m_fluids) {
                    cout << fluid->m_name << " " << fluid->m_particles.size() << endl;
                }
                setSimulation(simulation);

            }

            if(ImGui::Button("num Particles", ImVec2(-1, 0))) {
                cout << "Simulation has " << simulation->m_sources.size() << "sources\n";
                for(auto fluid : simulation->m_fluids) {
                    cout << fluid->m_name << " " << fluid->m_particles.size() << endl;
                }
            }

            ImGui::InputText("Store Particles (.xyz) in", simulation->m_particles_path);
            ImGui::InputText("Store Meshes (.obj) in", simulation->m_mesh_path);

            if(ImGui::Button("Record", ImVec2(-1, 0))) {
                simulation->toggleRecording();
                std::cout<<"Toggle Recording\n";
            }

            if(ImGui::Button("Save Particles", ImVec2(-1, 0))) {
                simulation->exportParticles();
            }

            if(ImGui::Button("Save Mesh", ImVec2(-1, 0))) {
                simulation->exportMesh();
            }
            ImGui::InputDouble("Isolevel", &simulation->m_surface_extractor->m_isolevel);
            ImGui::InputDouble("Resolution", &simulation->m_surface_extractor->m_res);
            ImGui::InputDouble("Kernel Radius Rec", &simulation->m_surface_extractor->m_kernel_radius);
        }

        if(ImGui::CollapsingHeader("Boundary Box")) {

            ImGui::InputDouble("Box min x", &(m_scene_min[0]));
            ImGui::InputDouble("Box min y", &(m_scene_min[1]));
            ImGui::InputDouble("Box min z", &(m_scene_min[2]));

            ImGui::InputDouble("Box max x", &(m_scene_max[0]));
            ImGui::InputDouble("Box max y", &(m_scene_max[1]));
            ImGui::InputDouble("Box max z", &(m_scene_max[2]));

            if(ImGui::Button("Reset boundary", ImVec2(-1, 0))) {
                simulation->setScene(new BoxScene(m_scene_min, m_scene_max));
            }

            if(ImGui::Button("Fit Bounding Box", ImVec2(-1, 0))) {
                simulation->getMinMaxParticlePosition(m_scene_min, m_scene_max);

                for(auto& source : simulation->m_sources) {
                    auto generatingSource = dynamic_cast<GeneratingSource *>(source);
                    if (generatingSource != nullptr) {
                        for(int i = 0; i < 3; i++) {
                            m_scene_min[i] = std::min(generatingSource->m_position[i] - generatingSource->m_positionStdDeviation, m_scene_min[i]);
                            m_scene_max[i] = std::max(generatingSource->m_position[i] + generatingSource->m_positionStdDeviation, m_scene_max[i]);
                        }
                    }
                }

                m_scene_min -= Eigen::Vector3d::Constant(0.3);
                m_scene_max += Eigen::Vector3d::Constant(0.3);

                m_scene_min.y() = std::min(0.0, m_scene_min.y());

                simulation->setScene(new BoxScene(m_scene_min, m_scene_max));
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

            ImGui::Combo("Fluid Type Chooser:", &m_fluid_chooser, m_fluid_names);

            if (ImGui::Button("Add block source", ImVec2(-1, 0))) {
                simulation->m_sources.push_back(new BlockSource(fluids::all[m_fluid_chooser]));
                simulation->m_sources.back()->init();
            }

            if (ImGui::Button("Add emitting source", ImVec2(-1, 0))) {
                simulation->m_sources.push_back(new EmittingSource(fluids::all[m_fluid_chooser]));
                simulation->m_sources.back()->init();
            }


            if (ImGui::Button("Add generating source", ImVec2(-1, 0))) {
                simulation->m_sources.push_back(new GeneratingSource(fluids::all[m_fluid_chooser]));
                simulation->m_sources.back()->init();
            }

            if (ImGui::Button("Add source from file", ImVec2(-1, 0))) {
                m_particles_init_file = igl::file_dialog_open();
                simulation->m_sources.push_back(new CustomSource(fluids::all[m_fluid_chooser], m_particles_init_file));
                simulation->m_sources.back()->init();
            }

            if (ImGui::Button("Remove all sources", ImVec2(-1, 0))) {
                simulation->m_sources.clear();
            }

            int i = 0;
            for (auto &source : simulation->m_sources) {

                if (ImGui::CollapsingHeader(("Source " + to_string(++i) + ": " + source->toString()).c_str())) {

                    if(ImGui::Button("Remove", ImVec2(-1, 0))){
                        auto it = std::find(simulation->m_sources.begin(), simulation->m_sources.end(), source);
                        simulation->m_sources.erase(it);
                    }

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

                    auto emittingSource = dynamic_cast<EmittingSource *>(source);
                    if (emittingSource != nullptr) {
                        ImGui::InputInt("Max. particles", &emittingSource->m_maxParticles);
                        ImGui::InputDouble("Particles per second", &emittingSource->m_particlesPerSecond);

                        ImGui::InputDouble("Particle distance", &emittingSource->m_spacing);

                        ImGui::InputInt("Particels: x", &emittingSource->m_x);
                        ImGui::InputInt("Particles: y", &emittingSource->m_y);
                        ImGui::InputInt("Particles: z", &emittingSource->m_z);

                        ImGui::InputDouble("Offset: x", &emittingSource->m_position[0]);
                        ImGui::InputDouble("Offset: y", &emittingSource->m_position[1]);
                        ImGui::InputDouble("Offset: z", &emittingSource->m_position[2]);

                        ImGui::InputDouble("Particle Velocity: x", &emittingSource->m_particleVelocity[0]);
                        ImGui::InputDouble("Particle Velocity: y", &emittingSource->m_particleVelocity[1]);
                        ImGui::InputDouble("Particle Velocity: z", &emittingSource->m_particleVelocity[2]);
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