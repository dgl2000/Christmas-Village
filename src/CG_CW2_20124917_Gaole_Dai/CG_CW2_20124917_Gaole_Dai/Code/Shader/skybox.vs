// vertex shader for skybox
// @file skybox.vs
// @source https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.skybox.vs
// @author Gaole Dai (20124917)
// @date 30/11/21

#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  
