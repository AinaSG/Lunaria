#version 130

uniform mat4 projection, model, view;
uniform vec2 texCoordDispl;

in vec2 position;
in vec2 texCoord;
in float height;

out float heightFrag;
out vec2 texCoordFrag;

void main()
{
	// Pass texture coordinates to access a given texture atlas
	texCoordFrag = texCoord + texCoordDispl;
	
	heightFrag = height;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = (projection * view * model) * vec4(position, 0.0, 1.0);
}

