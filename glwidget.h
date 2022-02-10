#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QtOpenGL/qglshaderprogram.h>
#include <QMatrix4x4>
#include <QMap>

class QOpenGLShaderProgram;
class Model;
class Trajectory;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    void createModel(quint32 id);
    void deleteModel(quint32 id);
    void beginSample();
    void finish();
    void setColor(quint32 id, QColor color);
    void setModel(quint32 id, QString model);
    void setWaypoint(quint32 id, QVector3D position, QVector3D orientation);
    void setIndex(int index) { m_index = index; repaint(); }
    void setReference(quint32 id) {m_reference = id;}

public slots:
    void Cleanup();

protected:
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
	int m_width;
	int m_height;
	qreal m_fov;
	int m_rotationX;
	int m_rotationZ;
    QPoint LastPos;

    int m_index;
    int m_sampleCount;
    quint32 m_reference;

	QMatrix4x4 m_projection;
	QMatrix4x4 m_camera;
	QMap<quint32, Model*> m_entities;
};

#endif // GLWIDGET_H
