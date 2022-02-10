#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <QVector3D>
#include <stdint.h>

#pragma pack (1)
// defined by standard
struct STLHeader
{
	quint8 headerData[80];
	quint32 facetCount;
};

struct STLFacet
{
	QVector3D n;  // normale vector
	QVector3D v1; // first vertex
	QVector3D v2; // second vertex
	QVector3D v3; // third vertex
	quint16 attribute;
};
#pragma pack ()

class ModelLoader
{
public:
    static bool CreateCube(float width, float depth, float height, float **vertices, quint32* facetCount);
    static bool ReadSTLFile(QString fileName, float** vertices, quint32* vertexCount);

private:
	static QVector3D GetNormaleVector(QVector3D vertex1, QVector3D vertex2, QVector3D vertex3);
	static void AddVertices(QVector3D vertex1, QVector3D vertex2, QVector3D vertex3, float* m_RenderArray, int& counter);
	static quint32 GetSTLFacetCount(const char *stlData, const quint32 fileSize);
};

#endif // MODELLOADER_H
