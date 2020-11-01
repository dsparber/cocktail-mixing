#include "Simulation.h"
#include "Particle.h"
#include "UniformGridNeighborSearch.h"



class Box {
public:
	Box(const Eigen::Vector3d& m, const Eigen::Vector3d& M) {

		// compute vertices
		m_V.resize(8,3);
		m_V <<
		m(0), m(1), m(2),
		M(0), m(1), m(2),
		M(0), M(1), m(2),
		m(0), M(1), m(2),
		m(0), m(1), M(2),
		M(0), m(1), M(2),
		M(0), M(1), M(2),
		m(0), M(1), M(2);


		m_E.resize(12,2);
		m_E <<
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		7 ,3;
	}


	void draw(igl::opengl::glfw::Viewer &viewer){
		for(int i = 0; i < m_E.rows(); i++) {
			viewer.data().add_edges(
									m_V.row(m_E(i, 0)),
									m_V.row(m_E(i, 1)),
									Eigen::RowVector3d(1,0,0));
		}
	}

	// TODO: find out with which face it has collided and return surface normal
	Eigen::Vector3d getSurfaceNormal(const Eigen::Vector3d& pos1, const Eigen::Vector3d& pos2) {
		return Eigen::Vector3d(1, 0, 0);
	}

	bool outOfBoundary(const Eigen::Vector3d& pos) {
		return 	pos.x() > m_V(1,0) || pos.x() < m_V(0,0) ||
				pos.y() > m_V(2,1) || pos.y() < m_V(0,1) ||
				pos.z() > m_V(4,2) || pos.z() < m_V(0,2);
	}

private:
	Eigen::MatrixXd m_V;
	Eigen::MatrixXi m_E;
};

class ParticleSimulation : public Simulation {
public:
    explicit ParticleSimulation(double gridWidth);

    virtual void init() override;
	virtual void resetMembers() override;
	virtual bool advance() override;

	void updateRenderGeometry() override;
    void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

	void postAdvance();

protected:
    std::vector<Particle> m_particles;
    NeighborSearch* m_neighborSearch;
	Box* m_box;

private:
	Eigen::MatrixXd V;  // Vertex positions
	Eigen::MatrixXi F;  // Faces (indices of vertices)
	Eigen::MatrixXd C;  // Colors per face
};