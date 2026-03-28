#version 120

uniform sampler2D texture;
uniform float time;
uniform float beatTime;    // moment où le beat s'est produit
uniform float waveStrength ; // force de la déformation ex: 0.03

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 center = uv - 0.5;
    float dist = length(center);
    
    // Rayon calculé depuis le temps écoulé depuis le beat
    float waveRadius = (time - beatTime) * 0.8;
    
    // Anneau gaussien fin qui se propage
    float wave = exp(-pow(dist - waveRadius, 2.0) * 80.0);
    
    // Déformation depuis le centre
    vec2 dir = normalize(center + vec2(0.0001));
    uv += dir * wave * waveStrength;
    
    gl_FragColor = texture2D(texture, uv);
}