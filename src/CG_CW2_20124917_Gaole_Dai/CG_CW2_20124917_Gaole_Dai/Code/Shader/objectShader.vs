// vertex shader for object
// @file objectShader.vs
// @source https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.vs
// @author Gaole Dai (20124917)
// @date 30/11/21

#version 330 core
layout (location = 0) in vec3 oPos;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 oTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} vs_out;

void main()
{
    vs_out.TexCoords = oTexCoords;
    vs_out.Normal = transpose(inverse(mat3(model))) * oNormal;
    vs_out.FragPos = vec3(model * vec4(oPos, 1.0));
    gl_Position = projection * view * model * vec4(oPos, 1.0);
}