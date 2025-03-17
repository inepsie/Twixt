#ifndef QUAD_H_
#define QUAD_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string> // Ajouté ici
#include "ShaderManager.h"

class Quad {
public:
    Quad(std::shared_ptr<Shader> shader);
    ~Quad();

    void draw(glm::mat4 proj, glm::mat4 view);
    void setPosition(const glm::vec3& position);

private:
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    glm::mat4 model;
};
#endif // QUAD_H_
