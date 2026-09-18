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

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 fallback_color;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
};

uniform DirectionalLight directional_light;
uniform PointLight point_light;

uniform bool emissive;
uniform vec3 emissive_color;

void main() {
    vec3 object_color = fallback_color;

    vec3 norm = normalize(Normal);

    vec3 ambient = 0.30 * object_color;

    vec3 dir = normalize(-directional_light.direction);
    float directional_strength = max(dot(norm, dir), 0.0);

    vec3 directional =
        directional_strength *
        directional_light.color *
        object_color;

    vec3 point_direction = normalize(point_light.position - FragPos);
    float point_strength = max(dot(norm, point_direction), 0.0);

    float distance = length(point_light.position - FragPos);

    float attenuation =
        1.0 /
        (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec3 point =
        point_strength *
        point_light.color *
        object_color *
        attenuation;

    vec3 emission = emissive_color * float(emissive);

    vec3 result = ambient + directional + point + emission;

    FragColor = vec4(result, 1.0);
    BrightColor = vec4(emission, 1.0);
}