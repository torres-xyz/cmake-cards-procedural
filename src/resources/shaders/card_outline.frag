#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 textureSize;
uniform float time;
uniform vec3 color;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 backColor = vec4(vec3(color), 1.0);

    float aspectRatio = textureSize.y / textureSize.x;
    float smoothFrameSpread = 20;
    float closeness = -0.1;
    float waveSpeed = 5.0;
    float waveHeight = 0.005;

    float sinCoordY = sin(fragTexCoord.y * 20.0 + (time * waveSpeed)) * waveHeight + 0.01;
    float cosCoordX = cos(fragTexCoord.x * 25.0 + (time * waveSpeed)) * waveHeight + 0.01;

    // Just controling the alpha channel
    vec4 whiteFrameLeft = vec4(vec3(1.), smoothstep(0.0, 1.0, (fragTexCoord.x + sinCoordY) * smoothFrameSpread + closeness));
    vec4 whiteFrameRight = vec4(vec3(1.), smoothstep(0.0, 1.0, (1 - fragTexCoord.x + sinCoordY) * smoothFrameSpread + closeness));
    vec4 whiteFrameTop = vec4(vec3(1.), smoothstep(0.0, 1.0, (fragTexCoord.y * aspectRatio + cosCoordX) * smoothFrameSpread + closeness));
    vec4 whiteFrameBottom = vec4(vec3(1.), smoothstep(0.0, 1.0, ((1 - fragTexCoord.y) * aspectRatio + cosCoordX) * smoothFrameSpread + closeness));

    backColor *= whiteFrameLeft;
    backColor *= whiteFrameRight;
    backColor *= whiteFrameTop;
    backColor *= whiteFrameBottom;

    //Shine line
    float lineFrequency = 20.0;
    float lineSpeed = 5.0;
    float linePos = mod(time * lineSpeed, lineFrequency) * 2. - 1.;

    backColor += vec4(vec3(smoothstep(0.2, 0.0, abs(fragTexCoord.y - fragTexCoord.x * 1.2 + linePos))), 0.0);

    // Output to screen
    finalColor = backColor;
}