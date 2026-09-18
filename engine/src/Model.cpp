
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

void Model::draw(const Shader *shader) {
    shader->use();
    for (auto &mesh: m_meshes) { mesh.draw(shader); }
}

void Model::draw(const Shader *texture_shader, const Shader *no_texture_shader) {
    for (auto &mesh: m_meshes) {
        const Shader *shader = mesh.has_diffuse_texture()
                                       ? texture_shader
                                       : no_texture_shader;

        shader->use();
        mesh.draw(shader);
    }
}

void Model::destroy() {
    for (auto &mesh: m_meshes) { mesh.destroy(); }
}
}// namespace engine::resources
