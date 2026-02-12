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

    // Shine line
    // https://graphtoy.com/?f1(x,t)=6&v1=true&f2(x,t)=1&v2=true&f3(x,t)=frac(x*f1(x,t)*f2(x,t))&v3=true&f4(x,t)=frac((x*(1/2)))*2-(1/f2(x,t))&v4=true&f5(x,t)=step(f4(x,t),0)&v5=true&f6(x,t)=f3(x,t)*f5(x,t)&v6=true&grid=1&coords=0,0,1.3333333333333333
    float a = 2;// final saw tooth wave teeth, after being cut.
    float b = 3;//space between teeth showing, in units on "a" teeth.
    float c = 2;// wave duration or speed
    float sawWaveA = fract(time * a * c);
    float sawWaveB = fract((time * (1/b))) * b - (1/c);
    float squareWaveB = step(sawWaveB, 0);
    float sawWaveWithATeethAndBPeriod = sawWaveA * squareWaveB;
    sawWaveWithATeethAndBPeriod = sawWaveWithATeethAndBPeriod * 2.1 - 1.1;//Adjusting to fit the entire card

    float linePos = sawWaveWithATeethAndBPeriod;

    //Drawing the line on top.
    backColor += vec4(vec3(smoothstep(0.2, 0.0, abs(fragTexCoord.y - fragTexCoord.x * 1.2 + linePos))), 0.0);

    // Output to screen
    finalColor = backColor;
}