import QtQuick3D 1.15
import QtQuick3D.Effects 1.15
import QtQuick 2.12

Effect {

    Texture {
        id: areaImg
        source: "textures/AreaTex.png"
    }

    Texture {
        id: searchImg
        source: "textures/SearchTex.png"
    }

    property TextureInput areaTex: TextureInput {
        texture: areaImg
    }

    property TextureInput searchTex: TextureInput {
        texture: searchImg
    }

    Shader {
        id: blendweight_vert
        stage: Shader.Vertex
        shader: "shaders/SMAA_BlendingWeight.vert"
    }

    Shader {
        id: blendweight_frag
        stage: Shader.Fragment
        shader: "shaders/SMAA_BlendingWeight.frag"
    }

    Shader {
        id: edgedetection_vert
        stage: Shader.Vertex
        shader: "shaders/SMAA_EdgeDetection.vert"
    }

    Shader {
        id: edgedetection_frag
        stage: Shader.Fragment
        shader: "shaders/SMAA_EdgeDetection.frag"
    }

    Shader {
        id: neighbourhoodblend_vert
        stage: Shader.Vertex
        shader: "shaders/SMAA_NeighbourhoodBlending.vert"
    }

    Shader {
        id: neighbourhoodblend_frag
        stage: Shader.Fragment
        shader: "shaders/SMAA_NeighbourhoodBlending.frag"
    }

    Buffer {
        id: edgeFBO
        name: "edgeFBO"
        bufferFlags: Buffer.None
        format: Buffer.RGBA8
        textureCoordOperation: Buffer.ClampToEdge
        textureFilterOperation: Buffer.Linear
    }

    Buffer {
        id: blendWeightFBO
        name: "blendWeightFBO"
        bufferFlags: Buffer.None
        format: Buffer.RGBA8
        textureCoordOperation: Buffer.ClampToEdge
        textureFilterOperation: Buffer.Linear
    }

    passes: [
        Pass {
            shaders: [ edgedetection_vert, edgedetection_frag ]
            output: edgeFBO
        },
        Pass {
            shaders: [ blendweight_vert, blendweight_frag ]
            output: blendWeightFBO
            commands: [
                BufferInput {
                    buffer: edgeFBO
                }
            ]
        },
        Pass {
            shaders: [ neighbourhoodblend_vert, neighbourhoodblend_frag ]
            commands: [
                // Refers Input Color Buffer for this Effect in Texture0 slot
                // And blendWeightFBO output texture goes to blendTexture slot
                BufferInput {
                    buffer: blendWeightFBO
                    param: "blendTexture"
                }

            ]
        }
    ]
}
