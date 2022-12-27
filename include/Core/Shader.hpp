#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <memory>
#include <unordered_map>

enum ShaderType : GLenum {
    Vert = GL_VERTEX_SHADER,
    Tesc = GL_TESS_CONTROL_SHADER,
    Tese = GL_TESS_EVALUATION_SHADER,
    Geom = GL_GEOMETRY_SHADER,
    Frag = GL_FRAGMENT_SHADER,
    Comp = GL_COMPUTE_SHADER,
};

class Shader {
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
    ~Shader();

    void Start() const { glUseProgram(m_id); }
    void Stop() const { glUseProgram(0); }

    void SetInt(const std::string& uniform_name, int value);
    void SetBool(const std::string& uniform_name, bool value);
    void SetFloat(const std::string& uniform_name, float value);
    void SetVec2(const std::string& uniform_name, const glm::vec2& vector);
    void SetVec3(const std::string& uniform_name, const glm::vec3& vector);
    void SetVec4(const std::string& uniform_name, const glm::vec4& vector);
    void SetMat4(const std::string& uniform_name, const glm::mat4& matrix);

protected:
    GLuint m_id;
    std::unordered_map<std::string, GLuint> uniform_location_caches;

    GLuint GetUniformLocation(const std::string& uniform_name);

private:
    GLuint m_vertex_id;
    GLuint m_fragment_id;
    GLuint m_geometry_id;

    GLuint CreateShader(const std::string& shader_filepath, ShaderType shader_type);
    GLboolean CompileShader(const GLuint shader_id);
    GLboolean LinkShaderProgram();

    void Destroy() const;
};

#endif
