#ifndef MODEL_H
#define MODEL_H

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QColor>

#include "trajectory.h"

class Model
{
public:
	Model(quint32 start);
	~Model();

    void initializeGL();

    void beginSample();
    void setColor(QColor color);
    void setModel(float* vertexData, quint32 vertexCount);
    void setWaypoint(QVector3D position, QVector3D orientation);
	void setOffset(QVector3D offset) {m_trajectory.setOffset(offset);}
    QVector3D getPosition(int index);
    void render(QMatrix4x4 projection, QMatrix4x4 camera, int index);

private:
    void initializeVBO();

	QOpenGLVertexArrayObject	m_vao;
	QOpenGLBuffer				m_vbo;
    QOpenGLShaderProgram		m_program;
    QMatrix4x4					m_matTransform;
	QVector3D					m_translation;
	QVector3D					m_totation;
	GLfloat*					m_vertexData;
	int							m_vertexCount;
	Trajectory					m_trajectory;
    quint32                     m_start;

    int VertexAttr;
    int HeightAttr;
    int MaxHeightLoc;
    int MinHeightLoc;
    int CameraLoc;
    int ProjectionLoc;
    int WorldLoc;
    int NormalMatrixLoc;
    int ColorLoc;

    QColor Color;
    QVector3D Position;
    QVector3D Orientation;
};

#endif // MODEL_H
