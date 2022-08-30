// fragement shader for skybox
// @file skybox.fs
// @source https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.skybox.fs
// @author Gaole Dai (20124917)
// @date 30/11/21

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
