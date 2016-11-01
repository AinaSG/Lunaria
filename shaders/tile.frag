#version 130

uniform vec4 color;
uniform sampler2D tex;

in vec2 texCoordFrag;
in float heightFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.a < 0.5f)
		discard;
		
	outColor = color * texColor;
	
	if (color.a < 0.9f)
        outColor *= vec4(heightFrag*0.99);
	else
	    outColor *= (1.- heightFrag*0.9);
}

