#version 120

uniform sampler2D texture;
uniform float time;
uniform vec3 holoColor = vec3(0.0, 1.0, 0.8); // ex: cyan (0.0, 1.0, 0.8)

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);
    
    // Lignes de scan horizontales
    float scan = sin(uv.y * 100.0 + time * 5.0) * 0.5 + 0.5;
    
    // Scintillement global
    float flicker = sin(time * 30.0) * 0.03 + 0.97;
    
    // Convertit en gris puis applique couleur holo
    float gray = dot(pixel.rgb, vec3(0.299, 0.587, 0.114));
    vec3 holo = holoColor * gray * scan * flicker;
    
    // Garde un peu de transparence
    float alpha = pixel.a * (0.7 + scan * 0.3);
    
    gl_FragColor = vec4(holo, alpha);
}