#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;
uniform sampler2D _NoiseTexture;

void main(){
	vec4 tex1 = texture(_Texture, UV);
	vec4 tex2 = texture(_NoiseTexture, UV);
	vec3 color = mix(tex1.rgb, tex2.rgb, tex2.a*0.75);
	FragColor = vec4(color, 1.0);

}
