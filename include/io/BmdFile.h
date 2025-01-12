#pragma once

#include "io/BaseFile.h"

#include "rendering/GX.h"

#include <vector>
#include <array>
#include <glm/glm.hpp>

#include <QColor>

class BmdFile
{
    struct SceneGraphNode
    {
        uint16_t materialID = -1;

        int16_t parentIndex;
        uint32_t nodeType; // 0: shape, 1: joint
        uint16_t nodeID;
    };

    struct MultiMatrix
    {
        uint32_t count;
        std::vector<uint16_t> indices;
        std::vector<glm::mat4> matrices;
        std::vector<float> weights;
    };

    struct MatrixType
    {
        bool weighted;
        uint16_t index;
    };

    struct Joint
    {
        uint16_t unk1;
        uint8_t unk2;

        glm::vec3 scale, rotation, translation;
        glm::mat4 matrix;
        glm::mat4 finalMatrix; // matrix with parents' transforms applied
    };

    struct Batch
    {
        struct Packet
        {
            struct Primitive
            {
                uint32_t numIndices;
                uint32_t primitiveType;

                uint32_t arrayMask;
                std::vector<uint32_t> posMatrixIndices;
                std::vector<uint32_t> positionIndices;
                std::vector<uint32_t> normalIndices;
                std::array<std::vector<uint32_t>, 2> colorIndices;
                std::array<std::vector<uint32_t>, 8> texcoordIndices;
            };


            std::vector<Primitive> primitives;
            std::vector<uint16_t> matrixTable;
        };


        uint8_t matrixType;

        std::vector<Packet> packets;

        float unk;
    };

    enum TexMatrixProjection {
        MTX3x4 = 0,
        MTX2x4 = 1,
    };

    struct TexMatrix
    {
        bool isNull = true;

        uint32_t info;
        TexMatrixProjection projection;
        glm::mat4 effectMatrix;
        glm::mat4 matrix;
    };

    struct Material
    {
        uint32_t index;
        QString name;
        uint32_t materialMode;
        bool translucent;

        std::vector<int16_t> indices;

        std::vector<TexMatrix> texMatrices; // let's leak all the memory!
        std::vector<float> indTexMatrices;

        GX::Material gxMaterial;


        std::vector<QColor> colorMatRegs;
        std::vector<QColor> colorAmbRegs;
        std::vector<QColor> colorConstants;
        std::vector<QColor> colorRegisters;

        GX::FogBlock fogBlock;
    };

    struct Sampler {
        uint32_t index;

        QString name;

        GX::WrapMode_t wrapS;
        GX::WrapMode_t wrapT;

        GX::TexFilter_t minFilter;
        GX::TexFilter_t magFilter;

        float minLOD;
        float maxLOD;
        float lodBias;
        int32_t textureDataIndex;
    };

    // actual class starts here
    BaseFile* file;

    void readINF1();
    void readVTX1();
    void readEVP1();
    void readDRW1();
    void readJNT1();
    void readSHP1();
    void readMAT3();
    void readMDL3();
    void readTEX1();

    GX::BTI_Texture readBTI(uint32_t absoluteStartIndex, const QString& name);

    std::vector<QString> readStringTable(uint32_t absoluteOffset);

    float readArrayShort(uint8_t fixedPoint);
    float readArrayFloat();
    float readArrayValue(uint32_t type, uint8_t fixedPoint);

    QColor readColorValue(uint32_t type);
    QColor readColor_RGBA8();
    QColor readColor_RGBX8();
    QColor readColor_RGBA16();

    GX::ColorChannelControl readColorChannel(uint32_t absoluteColorChanTableOffset, uint16_t colorChanIndex);

    glm::vec3 readVec3();

public:
    BmdFile() = default;

    BmdFile(BaseFile* inRarcFile);

    void save();
    void close();

    // INF1
    uint32_t m_vertexCount;
    std::vector<SceneGraphNode> m_sceneGraph;

    // VTX1
    uint32_t m_arrayMask;
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::array<std::vector<QColor>, 2> m_colors;
    std::array<std::vector<glm::vec2>, 8> m_texCoords;

    // EVP1
    std::vector<MultiMatrix> m_multiMatrices;

    // DRW1
    std::vector<MatrixType> m_matrixTypes;

    // JNT1
    std::vector<Joint> m_joints;

    // SHP1
    std::vector<Batch> m_batches;

    // MAT3
    std::vector<Material> m_materials;

    // TEX1
    std::vector<GX::BTI_Texture> m_textures;
    std::vector<Sampler> m_samplers;
};
