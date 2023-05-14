#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = a_TexCoord;
	gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}

#type fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_Screen;

void main()
{
    vec3 col = texture(u_Screen, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}
