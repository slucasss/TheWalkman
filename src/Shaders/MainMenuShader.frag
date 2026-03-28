uniform sampler2D texture;       // la texture de la scène
uniform float time;              // temps écoulé
uniform vec2 resolution;         // taille de l'écran

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);

    float intensity = 2.5;
    float radius    = 0.6;
    float minLight = 0.1;

    vec2 center = uv - 0.5;

    float aspect = resolution.x / resolution.y;
    center.x *= aspect;
    center.y *= 0.3;

    //center.y -= 0.1;

    float dist = dot(center, center);
    float vignette = 1.0 - pow(dist, radius) * intensity;
    vignette = clamp(vignette, minLight, 1.0);

    gl_FragColor = vec4(pixel.rgb * vignette, pixel.a);
}