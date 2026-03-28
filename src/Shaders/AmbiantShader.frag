#version 120

#define MAX_LIGHTS 100
#define MAX_CONES 100


uniform float time = 0.0;


uniform sampler2D texture;
uniform vec2 resolution;
uniform float darkness = 0.6;
uniform vec3 darkColor = {0, 0, 0};

// Lumières ponctuelles : xy = pos, z = radius, w = intensity
uniform vec4 lights[MAX_LIGHTS];

//Couleur de la lumière, rgb
uniform vec3 lightsColor[MAX_LIGHTS];

uniform int lightCount;


// Cones : xy = pos, z = hauteur, w = intensity
uniform vec4  cones[MAX_CONES];

//couleur du cone, rgb
uniform vec3 conesColor[MAX_CONES];

// xy = dir, z = demi-angle in radians, w=0.0
uniform vec4 conesDirs[MAX_CONES];   

uniform int coneCount;


uniform float drunkStrength = 0.0;


uniform float beatTime = 0.0;
uniform float waveStrength = 0.0;


uniform vec4 player = vec4(0.0, 0.0, 0.0, 0.0);
uniform vec3 playerColor = vec3(0.0, 0.0,0.0);

vec3 calcLight(vec2 fragPos, vec4 light, vec3 color) {
    float dist = distance(fragPos, light.xy);
    float factor = 1.0 - smoothstep(0.0, light.z, dist);
    return color * factor * light.w;
}


vec3 calcCone(vec2 fragPos, vec4 cone, vec3 color, vec4 coneDir) {
    vec2  toFrag = fragPos -cone.xy;
    float dist = length(toFrag);

    if (dist < 0.001) return color * cone.w;

    vec2 direction = normalize(vec2(coneDir.x, -coneDir.y));
    float projDist = dot(toFrag, direction);

    if (projDist < 0.0 || projDist > cone.z) return vec3(0.0);

    float angle = acos(clamp(dot(normalize(toFrag), direction), -1.0, 1.0));

    if (angle > coneDir.z) return vec3(0.0);

    float distFactor = 1.0 - smoothstep(0.0, cone.z,projDist);
    float angleFactor = 1.0 - smoothstep(0.0, coneDir.z, angle);

    return color * distFactor * angleFactor * cone.w;
}


vec3 calcAllLights(vec2 fragPos) {
    vec3 total = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (i >= lightCount) break;
        total += calcLight(fragPos, lights[i], lightsColor[i]);
    }

    for (int i = 0; i < MAX_CONES; i++) {
        if (i >= coneCount) break;
        total += calcCone(fragPos, cones[i], conesColor[i], conesDirs[i]);
    }
    total+=calcLight(fragPos, player, playerColor);

    return clamp(total, 0.0, 1.0);
}


vec4 applyDrunk(vec2 uv, out vec2 offset) {
    offset = vec2(0.0);
    if (drunkStrength < 0.001) return texture2D(texture, uv);

    vec2 center = uv - 0.5;
    float angle = length(center) * 5.0 - time * 2.0;
    offset.x = sin(angle) * drunkStrength * 0.03;
    offset.y = cos(angle) * drunkStrength * 0.03;

    vec2 distortedUV = uv + offset;

    float r = texture2D(texture, distortedUV + vec2(0.005, 0.0) * drunkStrength).r;
    float g = texture2D(texture, distortedUV).g;
    float b = texture2D(texture, distortedUV - vec2(0.005, 0.0) * drunkStrength).b;

    return vec4(r, g, b, texture2D(texture, distortedUV).a);
}


vec4 applyBeatWave(vec2 uv, out vec2 offset) {
    offset = vec2(0.0);
    if (waveStrength < 0.001) return texture2D(texture, uv);

    vec2 center = uv - 0.5;
    float dist = length(center);

    float waveRadius = (time - beatTime) * 0.8;
    float wave = exp(-pow(dist - waveRadius, 2.0) * 80.0);

    vec2 dir = normalize(center + vec2(0.0001));
    offset = dir * wave * waveStrength;

    return texture2D(texture, uv + offset);
}

void main() {
    vec2 uv      = gl_TexCoord[0].xy;
    vec2 fragPos = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);

    vec2 beatOffset;
    vec2 warpedUV = uv;
    applyBeatWave(uv, beatOffset);
    warpedUV = uv + beatOffset;

    vec2 drunkOffset;
    vec4 pixel = applyDrunk(warpedUV, drunkOffset);

    vec2 totalOffset = beatOffset + drunkOffset;
    vec2 distortedFragPos = fragPos + totalOffset * resolution;

    vec3  lightColor = calcAllLights(distortedFragPos);
    float luminance = max(lightColor.r, max(lightColor.g, lightColor.b));
    float finalDark = darkness * (1.0 - luminance);

    vec3 tinted = pixel.rgb * mix(vec3(1.0 - finalDark), lightColor, luminance);

    gl_FragColor = vec4(tinted, pixel.a);
}