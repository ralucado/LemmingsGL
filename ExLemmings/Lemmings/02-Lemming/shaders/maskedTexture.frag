#version 330

uniform vec4 color;
uniform sampler2D tex, mask, back;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5 or mask is black
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	vec4 maskColor = texture(mask, texCoordFrag);
	vec4 backColor = texture(back, gl_FragCoord.st/30);
	if((maskColor.r < 0.1f))
		discard;
	if (texColor.a >= 0.5f){
		outColor = color * texColor;
	}
	else outColor = color * backColor;
}


