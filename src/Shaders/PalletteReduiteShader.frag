
uniform sampler2D texture;       // la texture de la scène

uniform int colorCount = 8; // ex: 8
void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);

    // Quantize les couleurs
    pixel.rgb = floor(pixel.rgb * float(colorCount)) / float(colorCount);

    gl_FragColor = pixel;
}