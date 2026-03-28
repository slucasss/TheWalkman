uniform sampler2D texture;
uniform float time;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);
    
    // Teinte qui change selon la position et le temps
    vec3 rainbow;
    rainbow.r = sin(uv.x * 6.28 + time) * 0.5 + 0.5;
    rainbow.g = sin(uv.x * 6.28 + time + 2.09) * 0.5 + 0.5;
    rainbow.b = sin(uv.x * 6.28 + time + 4.18) * 0.5 + 0.5;
    
    // Applique seulement sur les pixels non transparents
    if (pixel.a > 0.0)
        gl_FragColor = vec4(pixel.rgb * rainbow, pixel.a);
    else
        gl_FragColor = pixel;
}