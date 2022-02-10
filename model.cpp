#include "model.h"

Model::Model(quint32 start)
	: m_vertexCount(0)
    , m_start(start)
{
}

Model::~Model()
{
	delete m_vertexData;
}

void Model::beginSample()
{
	m_trajectory.addPosition(Position, Orientation);
}

void Model::setColor(QColor color)
{
    Color = color;
	m_trajectory.setColor(color);
}

void Model::setModel(float *vertexData, quint32 vertexCount)
{
	m_vertexData = vertexData;
	m_vertexCount = int(vertexCount);
}

void Model::setWaypoint(QVector3D position, QVector3D orientation)
{
    Position = position;
    Orientation = orientation;
}

void Model::initializeGL()
{
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/facets.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/facets.frag");
    m_program.bindAttributeLocation("vertex", 0);
    m_program.bindAttributeLocation("normal", 1);
    m_program.link();
    m_program.bind();

    NormalMatrixLoc = m_program.uniformLocation("normalMatrix");
    ColorLoc = m_program.uniformLocation("color");
    CameraLoc = m_program.uniformLocation("camera");
    ProjectionLoc = m_program.uniformLocation("projection");
    WorldLoc = m_program.uniformLocation("world");

    initializeVBO();
    m_program.release();
	m_trajectory.initializeGL();
}

void Model::initializeVBO()
{
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(m_vertexData, m_vertexCount * int(sizeof(GLfloat)) * 6);

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glEnableVertexAttribArray(0); // vertices
    f->glEnableVertexAttribArray(1); // normals
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

	m_vbo.release();
}

QVector3D Model::getPosition(int index)
{
    index-=m_start;
    if(index < 0) index=0;
	return m_trajectory.getPosition(index);
}

void Model::render(QMatrix4x4 projection, QMatrix4x4 camera, int index)
{
    if(!m_vao.isCreated()) return;
	m_trajectory.render(projection, camera);

    index-=m_start;
    if(index < 0) return;
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_program.bind();
    m_program.setUniformValue(CameraLoc, camera);
    m_program.setUniformValue(ProjectionLoc, projection);
	m_program.setUniformValue(WorldLoc, m_trajectory.getTransformation(index));
    m_program.setUniformValue(NormalMatrixLoc, camera.normalMatrix());
    m_program.setUniformValue(ColorLoc, Color);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    m_program.release();
}
