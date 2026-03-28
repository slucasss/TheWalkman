uniform sampler2D texture;       // la texture de la scène
uniform float time;              // temps écoulé
uniform vec2 resolution;         // taille de l'écran

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);

    vec2 center = uv - 0.5;
    float vignette = 1.0 - dot(center, center) * 2.5;

    gl_FragColor = vec4(pixel.rgb * vignette, pixel.a);
}