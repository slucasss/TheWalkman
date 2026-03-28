uniform sampler2D texture;
uniform vec2 resolution;
uniform float time;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);
    
    // Convertit en niveaux de gris
    float gray = dot(pixel.rgb, vec3(0.299, 0.587, 0.114));
    
    // Teinte verte + bruit léger
    float noise = fract(sin(dot(uv, vec2(12.9898, 78.233)) + time) * 43758.5);
    gray += noise * 0.05;
    
    // Vignette
    vec2 center = uv - 0.5;
    float vignette = 1.0 - dot(center, center) * 3.0;
    
    gl_FragColor = vec4(0.0, gray * vignette, 0.0, pixel.a);
}