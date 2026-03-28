
uniform sampler2D texture;       // la texture de la scène
uniform float time;              // temps écoulé
uniform vec2 resolution;         // taille de l'écran

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 texelSize = 1.0 / resolution;

    vec4 sum = vec4(0.0);
    // Flou autour du pixel
    for(int x = -4; x <= 4; x++)
        for(int y = -4; y <= 4; y++)
            sum += texture2D(texture, uv + vec2(x, y) * texelSize);

    sum /= 81.0;

    vec4 original = texture2D(texture, uv);
    gl_FragColor = original + sum * 0.8; // ajoute le glow
}