#version 120
uniform sampler2D texture;
uniform float time;
uniform float depth ; // 0.0 → 1.0, profondeur de l'eau

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    // Ondulation lente
    uv.x += sin(uv.y * 8.0 + time * 1.5) * 0.008 * depth;
    uv.y += cos(uv.x * 6.0 + time * 1.2) * 0.005 * depth;
    
    vec4 pixel = texture2D(texture, uv);
    
    // Caustics (reflets lumineux sous l'eau)
    float caustic = sin(uv.x * 20.0 + time) * sin(uv.y * 20.0 + time * 0.7);
    caustic = caustic * 0.5 + 0.5;
    caustic *= 0.15 * depth;
    
    // Teinte bleue + caustics
    pixel.r *= 0.6;
    pixel.g *= 0.85;
    pixel.b = min(pixel.b + caustic, 1.0);
    
    gl_FragColor = pixel;
}