import QtQuick3D 1.15
import QtQuick3D.Materials 1.15

CustomMaterial {

    shaderInfo: ShaderInfo {
        shaderKey: ShaderInfo.Diffuse
        type: "GLSL"
    }

    property TextureInput inputTexture: TextureInput {
        texture: Texture {
            id: inputImg
            source: "textures/Text_Sample.png"
        }
    }

    Shader {
        id: sample_tex_vert
        stage: Shader.Vertex
        shader: "shaders/sample_texture.vert"
    }

    Shader {
        id: sample_tex_frag
        stage: Shader.Fragment
        shader: "shaders/sample_texture.frag"
    }

    passes: [
        Pass {
            shaders: [ sample_tex_vert, sample_tex_frag ]
        }
    ]
}
