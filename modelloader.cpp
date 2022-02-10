#include <QtOpenGL>
#include <qmath.h>

#include "modelloader.h"

bool ModelLoader::CreateCube(float width, float depth, float height, float **vertices, quint32 *facetCount)
{
    bool retval = false;
    int counter = 0;
    float* renderArray = nullptr;

    if(height > 0 && width > 0 && depth > 0)
    {
		renderArray = new float[6 * 2 * 6 * 3]; // 6 sides, 2 facets per side, 6 vertices per facet, 3 values per vertex

        // front
        AddVertices(QVector3D(-0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D(-0.5f*width,-0.5f*depth, 0.5f*height),
                    renderArray, counter);
        AddVertices(QVector3D(-0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width,-0.5f*depth, 0.5f*height),
                    renderArray, counter);

        // back
        AddVertices(QVector3D(-0.5f*width, 0.5f*depth,-0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth,-0.5f*height),
                    renderArray, counter);
        AddVertices(QVector3D(-0.5f*width, 0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth,-0.5f*height),
                    renderArray, counter);

        // left
        AddVertices(QVector3D(-0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D(-0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth,-0.5f*height),
                    renderArray, counter);
        AddVertices(QVector3D(-0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth, 0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth,-0.5f*height),
                    renderArray, counter);

        // right
        AddVertices(QVector3D( 0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width,-0.5f*depth, 0.5f*height),
                    renderArray, counter);
        AddVertices(QVector3D( 0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth, 0.5f*height),
                    renderArray, counter);

        // top
        AddVertices(QVector3D(-0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth, 0.5f*height),
                    renderArray, counter);
        AddVertices(QVector3D( 0.5f*width,-0.5f*depth, 0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth, 0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth, 0.5f*height),
                    renderArray, counter);

        // bottom
        AddVertices(QVector3D(-0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width,-0.5f*depth,-0.5f*height),
                    renderArray, counter);
        AddVertices(QVector3D( 0.5f*width,-0.5f*depth,-0.5f*height),
                    QVector3D(-0.5f*width, 0.5f*depth,-0.5f*height),
                    QVector3D( 0.5f*width, 0.5f*depth,-0.5f*height),
                    renderArray, counter);

        *vertices = renderArray;
        *facetCount = 18 * 2;

        retval = true;
    }
    return retval;
}

void ModelLoader::AddVertices(QVector3D vertex1, QVector3D vertex2, QVector3D vertex3, float* renderArray, int &counter)
{
    QVector3D normaleVector = GetNormaleVector(vertex1, vertex2, vertex3);

	renderArray[counter] = vertex1.x(); counter++;
	renderArray[counter] = vertex1.y(); counter++;
	renderArray[counter] = vertex1.z(); counter++;
	renderArray[counter] = normaleVector.x(); counter++;
	renderArray[counter] = normaleVector.y(); counter++;
	renderArray[counter] = normaleVector.z(); counter++;

	renderArray[counter] = vertex2.x(); counter++;
	renderArray[counter] = vertex2.y(); counter++;
	renderArray[counter] = vertex2.z(); counter++;
	renderArray[counter] = normaleVector.x(); counter++;
	renderArray[counter] = normaleVector.y(); counter++;
	renderArray[counter] = normaleVector.z(); counter++;

	renderArray[counter] = vertex3.x(); counter++;
	renderArray[counter] = vertex3.y(); counter++;
	renderArray[counter] = vertex3.z(); counter++;
	renderArray[counter] = normaleVector.x(); counter++;
	renderArray[counter] = normaleVector.y(); counter++;
	renderArray[counter] = normaleVector.z(); counter++;
}

QVector3D ModelLoader::GetNormaleVector(QVector3D vertex1, QVector3D vertex2, QVector3D vertex3)
{
	QVector3D vector1 = QVector3D(vertex2.x() - vertex1.x(),
								  vertex2.y() - vertex1.y(),
								  vertex2.z() - vertex1.z());

	QVector3D vector2 = QVector3D(vertex3.x() - vertex1.x(),
								  vertex3.y() - vertex1.y(),
								  vertex3.z() - vertex1.z());

    QVector3D normaleVector = QVector3D::crossProduct(vector1, vector2);
    normaleVector.normalize();

    return normaleVector;
}

quint32 ModelLoader::GetSTLFacetCount(const char *stlData, const quint32 fileSize)
{
    // check if header exists
    if(fileSize < sizeof(STLHeader))
    {
        return 0;
    }

    const STLHeader* header = reinterpret_cast<const STLHeader*>(stlData);

    // check if number of facets matches filesize
    if(fileSize != (header->facetCount * sizeof(STLFacet)) + sizeof(STLHeader))
    {
        return 0;
    }
    return header->facetCount;
}

bool ModelLoader::ReadSTLFile(QString fileName, float** vertices, quint32 *vertexCount)
{
    quint32 facetCount = 0;
    float* renderArray = nullptr;

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray byteArray = file.readAll();
        const char* stlData = byteArray.constData();
        facetCount = GetSTLFacetCount(stlData, quint32(byteArray.size()));

        if(facetCount > 0)
        {
            const STLFacet* facets = reinterpret_cast<const STLFacet*>(stlData + sizeof(STLHeader));

            renderArray = new float[facetCount * 18];

            for(quint32 i = 0; i < facetCount; i++, facets++)
            {
                // repair normale vector
				QVector3D normalVector(facets->n.x(), facets->n.y(), facets->n.z());
                if(normalVector.length() < 0.1f)
                {
					QVector3D v1 = QVector3D(facets->v2.x() - facets->v1.x(),
											 facets->v2.y() - facets->v1.y(),
											 facets->v2.z() - facets->v1.z());

					QVector3D v2 = QVector3D(facets->v3.x() - facets->v1.x(),
											 facets->v3.y() - facets->v1.y(),
											 facets->v3.z() - facets->v1.z());

                    normalVector = QVector3D::crossProduct(v1, v2);
                    normalVector.normalize();
                }
				renderArray[i * 18 +  0] = facets->v1.x();
				renderArray[i * 18 +  1] = facets->v1.y();
				renderArray[i * 18 +  2] = facets->v1.z();
				renderArray[i * 18 +  3] = normalVector.x();
				renderArray[i * 18 +  4] = normalVector.y();
				renderArray[i * 18 +  5] = normalVector.z();

				renderArray[i * 18 +  6] = facets->v2.x();
				renderArray[i * 18 +  7] = facets->v2.y();
				renderArray[i * 18 +  8] = facets->v2.z();
				renderArray[i * 18 +  9] = normalVector.x();
				renderArray[i * 18 + 10] = normalVector.y();
				renderArray[i * 18 + 11] = normalVector.z();

				renderArray[i * 18 + 12] = facets->v3.x();
				renderArray[i * 18 + 13] = facets->v3.y();
				renderArray[i * 18 + 14] = facets->v3.z();
				renderArray[i * 18 + 15] = normalVector.x();
				renderArray[i * 18 + 16] = normalVector.y();
				renderArray[i * 18 + 17] = normalVector.z();
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    *vertices = renderArray;
    *vertexCount = quint32(facetCount * 3);
    return true;
}
