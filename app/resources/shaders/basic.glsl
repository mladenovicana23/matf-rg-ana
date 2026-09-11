//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool has_diffuse_texture;
uniform vec3 texture_tint;
uniform vec3 fallback_color;

uniform vec3 directional_direction;
uniform vec3 directional_color;

uniform vec3 point_position;
uniform vec3 point_color;

uniform bool emissive;
uniform vec3 emissive_color;

void main() {
    vec3 object_color;

    if (has_diffuse_texture) {
        vec4 tex_color = texture(texture_diffuse1, TexCoords);
        object_color = mix(vec3(1.0), tex_color.rgb, tex_color.a) * texture_tint;
    }
    else {
        object_color = fallback_color;
    }

    vec3 norm = normalize(Normal);

    vec3 ambient = 0.30 * object_color;

    vec3 dir = normalize(-directional_direction);
    float directional_strength = max(dot(norm, dir), 0.0);
    vec3 directional =
        directional_strength *
        directional_color *
        object_color;

    vec3 point_direction = normalize(point_position - FragPos);
    float point_strength = max(dot(norm, point_direction), 0.0);

    float distance = length(point_position - FragPos);
    float attenuation =
        1.0 /
        (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec3 point =
        point_strength *
        point_color *
        object_color *
        attenuation;

    vec3 result = ambient + directional + point;

    if (emissive) {
        result += emissive_color;
    }

    FragColor = vec4(result, 1.0);

    if (emissive) {
        BrightColor = vec4(emissive_color, 1.0);
    }
    else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}