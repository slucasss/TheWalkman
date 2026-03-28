uniform sampler2D texture;
uniform float time;
uniform float glitchIntensity = 1.0; // 0.0 → 1.0

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    // Décale des bandes horizontales aléatoirement
    float band = floor(uv.y * 20.0);
    float offset = sin(band * 453.5 + time * 8.0) * glitchIntensity * 0.05;
    
    // Décale les canaux RGB séparément
    float r = texture2D(texture, uv + vec2(offset * 2.0, 0.0)).r;
    float g = texture2D(texture, uv + vec2(-offset, 0.0)).g;
    float b = texture2D(texture, uv + vec2(offset, 0.0)).b;
    float a = texture2D(texture, uv).a;
    
    gl_FragColor = vec4(r, g, b, a);
}