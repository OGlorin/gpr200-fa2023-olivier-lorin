//fragment shader
#version 450
out vec4 fragColor;
in vec2 UV;

uniform vec3 _skyColor[3];
uniform vec3 _sunColor[2];
uniform float _sunRadius;
uniform float _sunSpeed;
uniform vec3 _hillColor[2];
uniform vec2 _Resolution;
uniform float _Time;

float circleSDF(vec2 p, float r){
    return length(p)-r;
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = UV;

    //colors
    vec3 bgDay = mix(vec3(_skyColor[0]), vec3(_skyColor[1]), uv.y);
    vec3 bgNight = vec3(_skyColor[2]);
    vec4 sunColor = vec4(mix(vec3(_sunColor[0]), vec3(_sunColor[1]), uv.y), 1.0);
    
    vec3 hillColor = mix(vec3(_hillColor[0]), vec3(_hillColor[0]), (uv.y) + sin(_Time));
    //output
    vec3 dayShift = mix(bgNight, bgDay, uv.y+sin(_Time));

    // make the sun actually a circle
    uv = uv * 2.0 - 1.0;
    uv.x *= _Resolution.x / _Resolution.y;
    
    //sun
    vec2 sunLoc = vec2(0.0, sin(_Time*_sunSpeed)*0.7);
    
    float theSun = circleSDF(uv - sunLoc, _sunRadius);
    theSun = 0.5 - smoothstep(-0.05, 0.05, theSun);
    
    dayShift = mix(dayShift, sunColor.rgb, theSun*sunColor.a);
    
    //hills
    float hills = -0.40 + (0.1*(sin(uv.x), 3.0) + 0.2*pow(cosh(3.0*uv.x), -1.0));
    hills = step(abs(hills)-0.25,uv.y);
    
    dayShift = mix(hillColor, dayShift, hills*1.0);

    // Output to screen
    fragColor = vec4(dayShift,1.0);
}

