#ifndef PBS_PARTICLE_SIMULATION_H
#define PBS_PARTICLE_SIMULATION_H

#include "Simulation.h"
#include "Particle.h"
#include "Scene.h"
#include "Fluid.h"
#include "Source.h"

class SurfaceExtractor; 

class FluidSimulation : public Simulation {
public:
    explicit FluidSimulation();

    std::vector<Fluid*> m_fluids;
    std::vector<Source*> m_sources;

	bool m_use_particle_color;
	Scene* m_scene;

    virtual void init() override;
	virtual void resetMembers() override;
	virtual bool advance() override;

	void updateRenderGeometry() override;
    void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

	void getMinMaxParticlePosition(Eigen::Vector3d& minPosition, Eigen::Vector3d& maxPosition);

	void toggleRecording();
	void exportParticles(std::string exportPath);
	void exportMesh(std::string exportPath);
	
	double m_level;

protected:

	static void runParallel(int elementCount, const std::function<void(int, int)>& f);

private:
	Eigen::MatrixXd V;  // Vertex positions
    Eigen::MatrixXi F;  // Faces (indices of vertices)
	Eigen::MatrixXd C;  // Colors per face
	bool m_save_simulation;
	int m_save_freq;
	SurfaceExtractor* m_surface_extractor;

};

#endif // PBS_PARTICLE_SIMULATION_H