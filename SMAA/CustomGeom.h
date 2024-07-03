#ifndef CUSTOMGEOM_H
#define CUSTOMGEOM_H

#include <QtQuick3D/QQuick3DGeometry>

class Quad : public QQuick3DGeometry
{
    Q_OBJECT
public:
    Quad()
        :QQuick3DGeometry::QQuick3DGeometry{}
    {
        rebuildGeometry();
    }
    ~Quad() = default;

    void rebuildGeometry()
    {
        QByteArray vertices;
        QByteArray indices;

        fillGeom( vertices, indices );
        setPrimitiveType(PrimitiveType::Triangles);
        setVertexData(vertices);
        setIndexData(indices);
        setStride( (3 + 2) * sizeof(float) );
        addAttribute( Attribute::Semantic::PositionSemantic, 0, Attribute::ComponentType::F32Type );
        addAttribute( Attribute::Semantic::TexCoordSemantic, 3 * sizeof(float), Attribute::ComponentType::F32Type );
        addAttribute( Attribute::Semantic::IndexSemantic, 0, Attribute::ComponentType::U16Type );
    }

    void fillGeom( QByteArray& vertices, QByteArray& indices )
    {
        vertices.resize( 4 * ( 3 + 2 ) * sizeof(float) );
        indices.resize( 6 * sizeof(uint16_t) );

        float* fVertices = reinterpret_cast<float*>( vertices.data() );

        fVertices[0] = -1.0; fVertices[1] = 1.0; fVertices[2] = 0.0;
        fVertices[3] = 0.0; fVertices[4] = 1.0;

        fVertices[5] = -1.0; fVertices[6] = -1.0; fVertices[7] = 0.0;
        fVertices[8] = 0.0; fVertices[9] = 0.0;

        fVertices[10] = 1.0; fVertices[11] = -1.0; fVertices[12] = 0.0;
        fVertices[13] = 1.0; fVertices[14] = 0.0;

        fVertices[15] = 1.0; fVertices[16] = 1.0; fVertices[17] = 0.0;
        fVertices[18] = 1.0; fVertices[19] = 1.0;

        uint16_t* iIndices = reinterpret_cast<uint16_t*>( indices.data() );
        iIndices[0] = 0; iIndices[1] = 1; iIndices[2] = 2;
        iIndices[3] = 0; iIndices[4] = 2; iIndices[5] = 3;
    }
};

#endif // CUSTOMGEOM_H
