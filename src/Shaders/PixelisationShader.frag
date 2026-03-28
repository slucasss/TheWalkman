uniform sampler2D texture;
uniform float pixelSize = 4.0; // ex: 4.0, 8.0, 16.0
uniform vec2 resolution;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    // Arrondit les UVs à la taille d'un "gros pixel"
    vec2 pixelUV = floor(uv * resolution / pixelSize) * pixelSize / resolution;
    
    gl_FragColor = texture2D(texture, pixelUV);
}