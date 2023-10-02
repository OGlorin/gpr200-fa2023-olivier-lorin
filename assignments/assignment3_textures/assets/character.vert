#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uvPos;

uniform float _Time;

out vec2 UV;
void main(){
	vec3 position = vPos*abs(sin(_Time));
	vec2 move = uvPos*abs(cos(_Time));
	UV = move;
	gl_Position = vec4(position,2.0);
}