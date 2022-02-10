#include "trajectory.h"

#include <QtOpenGL>
#include <qmath.h>

Trajectory::Trajectory()
{
}

void Trajectory::setColor(QColor color)
{
    Color = color;
}

void Trajectory::addPosition(QVector3D position, QVector3D orientation)
{
    if(std::isnan(position.z())) {
        if(m_data.size() > 0) {
            position.setZ(m_data.last().z());
        }
        else {
            position.setZ(0);
        }
    }

    if(std::isnan(position.x()) || std::isnan(position.y())) {
        if(m_data.size() > 0) {
            m_data.append(m_data.last());
        }
    }
    else {
        m_data.append(QVector3D(position.x(), position.y(), position.z()));
    }

    if((orientation.x()==0 || orientation.y()==0 || orientation.z()==0)
            && m_orientations.size() > 0) {
        m_orientations.append(m_orientations.last());
    }
    else {
        m_orientations.append(orientation);
    }
}

QVector3D Trajectory::getPosition(int index)
{
    if(m_data.size() <= index) return QVector3D();
    return m_data[index];
}

QMatrix4x4 Trajectory::getTransformation(int index)
{
    QMatrix4x4 transformation;
    transformation.setToIdentity();
    transformation.translate(QVector3D(m_data[index].x() + m_offset.x(),
                                       m_data[index].y() + m_offset.y(),
                                       m_data[index].z() + m_offset.z()));
    transformation.rotate(-m_orientations[index].z(), 0, 0, 1);
    transformation.rotate(m_orientations[index].y(), 1, 0, 0);
    transformation.rotate(m_orientations[index].x(), 0, 1, 0);
    return transformation;
}

void Trajectory::initializeGL()
{
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/lines.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/lines.frag");

    m_program.bindAttributeLocation("vertex", 0);

    m_program.link();
    m_program.bind();

    CameraLoc = m_program.uniformLocation("camera");
    ProjectionLoc = m_program.uniformLocation("projection");
    WorldLoc = m_program.uniformLocation("world");
    ColorLoc = m_program.uniformLocation("color");

    initializeVBO();
    m_program.release();
}

void Trajectory::initializeVBO()
{
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_data.constData(), int(sizeof(GLfloat)) * 3 * m_data.size());

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    m_vbo.release();
}

void Trajectory::render(QMatrix4x4 projection, QMatrix4x4 camera)
{
    QMatrix4x4 transformation;
    transformation.setToIdentity();
    transformation.translate(m_offset.x(), m_offset.y(), m_offset.z());

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_program.bind();
    m_program.setUniformValue(CameraLoc, camera);
    m_program.setUniformValue(ProjectionLoc, projection);
    m_program.setUniformValue(WorldLoc, transformation);
    QMatrix3x3 normalMatrix = camera.normalMatrix();
    m_program.setUniformValue(NormalMatrixLoc, normalMatrix);
    m_program.setUniformValue(ColorLoc, Color);

    glDrawArrays(GL_LINE_STRIP, 0, m_data.size());

    m_program.release();
}
