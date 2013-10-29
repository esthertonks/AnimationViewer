#version 330

in vec3 TexCoord;
layout (location = 0) out vec4 FragColor;

uniform BlobSettings{
    vec4 InnerColor;
    vec4 OuterColor;
    float RadiusInner;
    float RadiusOuter;
};

void main() {
    float dx = TexCoord.x - 0.5;
    float dy = TexCoord.y - 0.5;
    float distance = sqrt(dx * dx + dy * dy);

    FragColor = mix(BlobSettings.InnerColor, BlobSettings.OuterColor, smoothstep(BlobSettings.RadiusInner, BlobSettings.RadiusOuter, distance));
}
