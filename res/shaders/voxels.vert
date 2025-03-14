#version 460

layout(std430, binding = 0) buffer BlockData {
    uint data[]; // Le tableau de données
};

uniform mat4 MVP;
uniform mat4 view;

out uint id;

vec2 index_1dto2d(uint ind, uint size){
    return vec2(ind / size, ind % size);
}

void main(void) {
  int index1D = gl_VertexID;  // Indice automatique
  vec2 xy = index1dTo2d(index1D, board_size);
  id = data[index1D];
  gl_Position = vec4(xy, 0.0, 1.0);
}
