#include "../include/Scene.h"


Scene::Scene(const Eigen::Vector3d &m, const Eigen::Vector3d &M) {

    // compute vertices
    m_V.resize(8, 3);
    m_V <<
        m(0), m(1), m(2),
            M(0), m(1), m(2),
            M(0), M(1), m(2),
            m(0), M(1), m(2),
            m(0), m(1), M(2),
            M(0), m(1), M(2),
            M(0), M(1), M(2),
            m(0), M(1), M(2);


    m_E.resize(12, 2);
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
            7, 3;
}


void Scene::draw(igl::opengl::glfw::Viewer &viewer) {
    for (int i = 0; i < m_E.rows(); i++) {
        viewer.data().add_edges(
                m_V.row(m_E(i, 0)),
                m_V.row(m_E(i, 1)),
                Eigen::RowVector3d(1, 0, 0));
    }
}

// TODO: find out with which face it has collided and return surface normal
Eigen::Vector3d Scene::getSurfaceNormal(const Eigen::Vector3d &pos1, const Eigen::Vector3d &pos2) {
    return {1, 0, 0};
}

bool Scene::outOfBoundary(const Eigen::Vector3d &pos) {
    return pos.x() > m_V(1, 0) || pos.x() < m_V(0, 0) ||
           pos.y() > m_V(2, 1) || pos.y() < m_V(0, 1) ||
           pos.z() > m_V(4, 2) || pos.z() < m_V(0, 2);
}
