//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

void main() {
    vec3 object_color = vec3(0.65, 0.65, 0.65);
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    vec3 ambient = 0.25 * light_color;

    vec3 norm = normalize(Normal);
    vec3 light_direction = normalize(vec3(-0.4, 1.0, 0.6));
    float diffuse_strength = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diffuse_strength * light_color;

    vec3 result = (ambient + diffuse) * object_color;
    FragColor = vec4(result, 1.0);
}