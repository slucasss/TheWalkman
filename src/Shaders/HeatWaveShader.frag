uniform sampler2D texture;
uniform float time;
uniform float intensity = 0.1; // ex: 0.01

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    // Distortion verticale qui monte
    float distX = sin(uv.y * 15.0 + time * 3.0) * intensity;
    float distY = cos(uv.x * 10.0 + time * 2.0) * intensity * 0.5;
    
    // Plus fort en bas de l'écran
    float heightFactor = 1.0 - uv.y;
    uv += vec2(distX, distY) * heightFactor;
    
    gl_FragColor = texture2D(texture, uv);
}