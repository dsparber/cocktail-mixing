#ifndef PBS_PARTICLE_SIMULATION_H
#define PBS_PARTICLE_SIMULATION_H

#include "Simulation.h"
#include "Particle.h"
#include "Scene.h"
#include "Fluid.h"
#include "Source.h"

class FluidSimulation : public Simulation {
public:
    explicit FluidSimulation();

    std::vector<Fluid*> m_fluids;
    std::vector<Source*> m_sources;

    virtual void init() override;
	virtual void resetMembers() override;
	virtual bool advance() override;

	void updateRenderGeometry() override;
    void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

	bool m_use_particle_color;

	Scene* m_scene;

protected:

	static void runParallel(int elementCount, const std::function<void(int, int)>& f);

private:
	Eigen::MatrixXd V;  // Vertex positions
    Eigen::MatrixXi F;  // Faces (indices of vertices)
	Eigen::MatrixXd C;  // Colors per face
};

#endif // PBS_PARTICLE_SIMULATION_H