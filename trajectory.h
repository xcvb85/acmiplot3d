#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QVector>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QColor>

class Trajectory
{
public:
    Trajectory();
    void initializeGL();
    void render(QMatrix4x4 projection, QMatrix4x4 camera);
    void setOffset(QVector3D offset) {m_offset=offset;}
    void setColor(QColor color);
    void addPosition(QVector3D position, QVector3D orientation);
    QVector3D getPosition(int index);
    QMatrix4x4 getTransformation(int index);

private:
    void initializeVBO();

    QOpenGLVertexArrayObject	m_vao;
    QOpenGLBuffer				m_vbo;
    QOpenGLShaderProgram		m_program;
    QMatrix4x4					m_matTransform;
    QVector3D					m_translation;
    QVector3D					m_rotation;
    QVector3D					m_offset;

    QVector<QVector3D>			m_data;
    QVector<QVector3D>			m_orientations;
    int							m_vertexCount;
    QColor Color;

    int VertexAttr;
    int HeightAttr;
    int CameraLoc;
    int ProjectionLoc;
    int WorldLoc;
    int NormalMatrixLoc;
    int ColorLoc;
};

#endif // TRAJECTORY_H
