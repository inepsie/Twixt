#version 460

layout(std430, binding = 0) buffer BlockData {
    uint data[]; // Le tableau de données
};

uniform mat4 MVP;
uniform mat4 view;
uniform int board_size;

out uint id;

vec2 index_1dto2d(uint ind, uint size){
    return vec2(float(ind % size), float(ind / size));
}

void main(void) {
  int index1D = gl_VertexID;  // Indice automatique
  float ratio = 1.0 / float(board_size + 1);
  vec2 xy = ratio + index_1dto2d(index1D, board_size) * ratio;
  id = data[index1D];
  gl_Position = vec4(xy, 0.0, 1.0);
}
