#version 460

#define M_PI 3.1415926535897932384626433832795

layout(points) in;
layout(triangle_strip, max_vertices = 66) out;

in uint id[];
out uint frag_id[1];

uniform mat4 MVP;
uniform mat4 view;
uniform int board_size;

void main(void) {

  frag_id[0] = id[0];                  // Transmission de l'id au fs
  vec3 pos = gl_in[0].gl_Position.xyz; // Position du point reçu
  float ratio = 1.0 / float(board_size + 1);
  float size = ratio * 0.2;
  int nb_slices = 32;
  float slice_angle = (2 * M_PI) / nb_slices;
  if(id[0] == 1) size /= 4.0;

  for (int i = 0; i <= nb_slices; ++i) {
    gl_Position =
        MVP *
        (gl_in[0].gl_Position +
         (size * vec4(cos(i * slice_angle), sin(i * slice_angle), 0.0, 0.0)));
    EmitVertex();
    gl_Position = MVP * gl_in[0].gl_Position;
    EmitVertex();
  }
}
