#include <QMouseEvent>
#include <QtMath>

#include "glwidget.h"
#include "trajectory.h"
#include "modelloader.h"
#include "model.h"
#include "trajectory.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
	, m_width(800)
	, m_height(600)
	, m_fov(90)
	, m_rotationX(315*16)
	, m_rotationZ(45*16)
    , m_index(0)
    , m_sampleCount(-1) // acmi file starts with timestamp, but timestamp is used to finish a sample here
{
}

GLWidget::~GLWidget()
{
    //Cleanup();
}

void GLWidget::createModel(quint32 id)
{
	m_entities.insert(id, new Model(m_sampleCount));
}

void GLWidget::deleteModel(quint32 id)
{
	if(!m_entities.contains(id)) {
    }
}

void GLWidget::beginSample()
{
	foreach(auto entity, m_entities) {
        entity->beginSample();
    }
    m_sampleCount++;
}

void GLWidget::finish()
{
    makeCurrent();

	foreach(auto entity, m_entities) {
        entity->initializeGL();
    }
    doneCurrent();
}

void GLWidget::setWaypoint(quint32 id, QVector3D position, QVector3D orientation)
{
	if(!m_entities.contains(id)) {
        createModel(id);
    }
	m_entities[id]->setWaypoint(position, orientation);
}

void GLWidget::setColor(quint32 id, QColor color)
{
	if(!m_entities.contains(id)) {
        createModel(id);
    }
	m_entities[id]->setColor(color);
}

void GLWidget::setModel(quint32 id, QString model)
{
	if(!m_entities.contains(id)) {
        createModel(id);
    }

    float* vertices;
    quint32 vertexcount;

    if(!ModelLoader::ReadSTLFile(QString("%1.stl").arg(model.toLower()), &vertices, &vertexcount))
    {
        ModelLoader::CreateCube(0.1f, 0.1f, 0.1f, &vertices, &vertexcount);
    }
	m_entities[id]->setModel(vertices, vertexcount);
}

void GLWidget::Cleanup()
{
    makeCurrent();
	qDeleteAll(m_entities);
	m_entities.clear();
    doneCurrent();
    m_index = 0;
    m_sampleCount = -1;
}

void GLWidget::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	// set m_camera to reference model
    QVector3D position;
    QVector3D offset;
	if(m_entities.contains(m_reference))
    {
		position = m_entities[m_reference]->getPosition(m_index*m_sampleCount*0.001);
        offset = QVector3D(-position.x(), -position.y(), -position.z());
    }

	m_camera.setToIdentity();
	m_camera.translate(0, 0, -10000);
	m_camera.rotate(m_rotationX / 16.0f, 1, 0, 0);
	m_camera.rotate(m_rotationZ / 16.0f, 0, 0, 1);

    // render all objects
	foreach(auto item, m_entities)
    {
        item->setOffset(offset);
		item->render(m_projection, m_camera, m_index*m_sampleCount*0.001);
    }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    float pNear =     1.0f;
    float pFar = 100000.0f;

	m_width = width;
	m_height = height;

	m_projection.setToIdentity();
	float tangent = float(qFabs(qTan(qDegreesToRadians(m_fov/2))));
	float pScale = (200*tangent)/qMin(width, height);
	m_projection.ortho(-pScale*width, pScale*width, -pScale*height, pScale*height, pNear, pFar);
//	m_projection.perspective(float(m_fov), GLfloat(width) / height, pNear, pFar);
}

static void qNormalizeAngle(int &angle)
{
    while(angle < 0) {
        angle += 360 * 16;
    }

    while(angle > 360 * 16) {
        angle -= 360 * 16;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    LastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
	if(event->angleDelta().y() < 0 && m_fov > 0.1) {
        // zoom in
		m_fov -= (10 * sin(m_fov * M_PI/180.0));
    }
	else if(event->angleDelta().y() > 0 && m_fov < 120) {
        // zoom out
		m_fov += (10 * sin(m_fov * M_PI/180.0));
    }

    event->accept();
	resizeGL(m_width, m_height);
    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - LastPos.x();
    int dy = event->y() - LastPos.y();
    LastPos = event->pos();

	m_rotationX = m_rotationX + 8 * dy;
	m_rotationZ = m_rotationZ + 8 * dx;
	qNormalizeAngle(m_rotationX);
	qNormalizeAngle(m_rotationZ);
    update();
}
