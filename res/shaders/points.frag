#version 460

in flat uint frag_id[1];
out vec4 fragColor;

uniform mat4 MVP;
uniform mat4 view;

void main(void) {

    if(frag_id[0] == 0){  discard;}  // Vide
    if(frag_id[0] == 1){
        fragColor = vec4(0.2, 0.2, 0.2, 0.4);
    }

    if(frag_id[0] == 2){
        fragColor = vec4(0.75, 0.0, 0.0, 1.0);
    }
    else if(frag_id[0] == 3){
        fragColor = vec4(0.2, 0.2, 0.2, 1.0);
        //fragColor = vec4(1.0, 0.9, 0.0, 1.0);
    }
}
