#version 330 core
out float FragColor;

in vec2 TexCoords;

//the SSAO map created by ssao.fs
uniform sampler2D ssaoInput;

void main() 
{
    //ge get the size of a texel
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    //we sample surrounding texels in a 2x2 radius
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, TexCoords + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
}