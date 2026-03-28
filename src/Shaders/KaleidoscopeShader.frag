#version 120
uniform sampler2D texture;
uniform float time;
uniform float segments = 6.0; // ex: 6.0, 8.0, 12.0

void main() {
    vec2 uv = gl_TexCoord[0].xy - 0.5;
    
    // Coordonnées polaires
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    
    // Divise en segments
    float segAngle = 3.14159 / segments;
    angle = mod(angle + time * 0.2, segAngle * 2.0);
    if (angle > segAngle) angle = segAngle * 2.0 - angle;
    
    // Repasse en cartésien
    vec2 newUV = vec2(cos(angle), sin(angle)) * radius + 0.5;
    
    gl_FragColor = texture2D(texture, newUV);
}