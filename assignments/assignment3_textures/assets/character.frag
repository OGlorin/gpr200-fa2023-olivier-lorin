#version 450
out vec4 FragColor;
in vec2 UV;
uniform float _characterSpeed;
uniform float _Time;

uniform sampler2D _Texture;
void main(){
	vec4 tex1 = texture(_Texture, UV);
	vec2 characterLoc = vec2(0.0, sin(_Time*_characterSpeed)*0.7);
	FragColor = tex1;
}
