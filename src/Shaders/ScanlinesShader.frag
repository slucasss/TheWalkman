

uniform sampler2D texture;       // la texture de la scène
uniform float time;              // temps écoulé
uniform vec2 resolution;         // taille de l'écran


void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);

    // Scanlines
    float scanline = sin(uv.y * resolution.y * 3.14159) * 0.04;
    pixel.rgb -= scanline;

    // Légère distortion barrel
    vec2 center = uv - 0.5;
    float dist = dot(center, center);
    uv += center * dist * 0.1;

    gl_FragColor = texture2D(texture, uv) - scanline;
}