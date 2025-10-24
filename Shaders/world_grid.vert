#version 330 core



uniform vec3 viewPos;
uniform float gridSize = 100.0;
uniform mat4 projection;
uniform mat4 view;

out vec3 worldPos;
const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),      // bottom left
    vec3( 1.0, 0.0, -1.0),      // bottom right
    vec3( 1.0, 0.0,  1.0),      // top right
    vec3(-1.0, 0.0,  1.0)       // top left
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);




void main() {
			
 int Index = Indices[gl_VertexID];
 vec3 vPos = Pos[Index] * gridSize;

//
vPos.x += viewPos.x;
vPos.z += viewPos.z;

vPos.y = -0.1;

worldPos =	vPos;

gl_Position = projection * view * vec4(vPos, 1.0);

}