uniform sampler2D texture;
uniform float time;
uniform float drunkStrength = 1.0; // 0.0 → 1.0

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    // Distortion en spirale
    vec2 center = uv - 0.5;
    float angle = length(center) * 5.0 - time * 2.0;
    float s = sin(angle) * drunkStrength * 0.03;
    float c = cos(angle) * drunkStrength * 0.03;
    uv += vec2(s, c);
    
    // Aberration chromatique légère
    float r = texture2D(texture, uv + vec2(0.005, 0.0) * drunkStrength).r;
    float g = texture2D(texture, uv).g;
    float b = texture2D(texture, uv - vec2(0.005, 0.0) * drunkStrength).b;
    
    gl_FragColor = vec4(r, g, b, texture2D(texture, uv).a);
}