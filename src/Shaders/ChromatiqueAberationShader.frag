uniform sampler2D texture;
uniform float aberrationStrength = 0.005; // ex: 0.003
uniform float time;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 center = uv - 0.5;
    
    // Décale chaque canal dans une direction différente
    float strength = aberrationStrength * (1.0 + sin(time) * 0.5);
    
    float r = texture2D(texture, uv + center * strength).r;
    float g = texture2D(texture, uv).g;
    float b = texture2D(texture, uv - center * strength).b;
    float a = texture2D(texture, uv).a;
    
    gl_FragColor = vec4(r, g, b, a);
}