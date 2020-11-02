#ifndef PBS_SCENE_H
#define PBS_SCENE_H

#include <Eigen/Dense>
#include <igl/opengl/glfw/Viewer.h>

class Scene {
public:
    Scene(const Eigen::Vector3d& m, const Eigen::Vector3d& M);

	void draw(igl::opengl::glfw::Viewer &viewer);

	Eigen::Vector3d getSurfaceNormal(const Eigen::Vector3d& pos1, const Eigen::Vector3d& pos2);

	bool outOfBoundary(const Eigen::Vector3d& pos);

private:
	Eigen::MatrixXd m_V;
	Eigen::MatrixXi m_E;
};

#endif