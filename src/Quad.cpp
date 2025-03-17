#include "Quad.h"
#include "Constants.h"
#include "Shader.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace C = Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

Quad::Quad(std::shared_ptr<Shader> shader) {
  shaderProgram = shader->getID();  // Utilise l'opérateur -> pour accéder à l'ID

GLfloat size = C::QUAD_SIZE / C::WINDOW_HEIGHT;
GLfloat quadVertices[16] = {
    -size,  size,  0, 0, // Coin haut gauche
     size,  size,  0, 0, // Coin haut droit
     size, -size,  0, 0, // Coin bas droit
    -size, -size,  0, 0  // Coin bas gauche
};

GLuint quadIndices[6] = {
    0, 1, 2,  // Premier triangle
    0, 2, 3   // Deuxième triangle
};



  // Génération des buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

// Destructeur
Quad::~Quad() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

// Fonction de rendu pour un quad rempli
void Quad::draw(glm::mat4 proj, glm::mat4 view) {
  glUseProgram(shaderProgram);

  glm::mat4 mvp = proj * view * model;
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE,
                     &mvp[0][0]);

  glBindVertexArray(VAO);

  // Dessiner un quad rempli avec 2 triangles
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);
}

void Quad::setPosition(const glm::vec3 &position) {
  model = glm::translate(glm::mat4(1.0f), position);
}
