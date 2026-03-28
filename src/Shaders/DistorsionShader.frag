uniform sampler2D texture;       // la texture de la scène
uniform float time;              // temps écoulé
uniform vec2 resolution;         // taille de l'écran

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv.x += sin(uv.y * 20.0 + time * 2.0) * 0.005;
    uv.y += cos(uv.x * 20.0 + time * 2.0) * 0.005;

    gl_FragColor = texture2D(texture, uv);
}