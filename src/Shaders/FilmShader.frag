#version 120
uniform sampler2D texture;
uniform float time;
uniform float grainStrength = 0.08; // ex: 0.2

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);
    
    // Génère du bruit pseudo-aléatoire qui change chaque frame
    float noise = fract(sin(dot(uv, vec2(12.9898, 78.233)) + time * 0.1) * 43758.5453);
    noise = (noise - 0.5) * grainStrength;
    
    pixel.rgb += noise;
    
    gl_FragColor = pixel;
}