#ifndef PBS_PARTICLE_SIMULATION_H
#define PBS_PARTICLE_SIMULATION_H

#include "Simulation.h"
#include "Particle.h"
#include "Scene.h"
#include "Fluid.h"
#include "Source.h"
#include "SurfaceExtractor.h"

class FluidSimulation : public Simulation {
public:
    explicit FluidSimulation();

    std::vector<Fluid*> m_fluids;
    std::vector<Source*> m_sources;

	bool m_use_particle_color;

    virtual void init() override;
	virtual void resetMembers() override;
	virtual bool advance() override;

	void updateRenderGeometry() override;
    void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

	void getMinMaxParticlePosition(Eigen::Vector3d& minPosition, Eigen::Vector3d& maxPosition);

	void toggleRecording();
	void exportParticles();
	void exportMesh();

	void setScene(Scene* scene);

	SurfaceExtractor* m_surface_extractor;

	std::string m_mesh_path;
	std::string m_particles_path;

protected:

	static void runParallel(int elementCount, const std::function<void(int, int)>& f);
	Scene* m_scene;

private:
	Eigen::MatrixXd V;  // Vertex positions
    Eigen::MatrixXi F;  // Faces (indices of vertices)
	Eigen::MatrixXd C;  // Colors per face
	bool m_save_simulation;
	int m_save_freq;

};

#endif // PBS_PARTICLE_SIMULATION_H