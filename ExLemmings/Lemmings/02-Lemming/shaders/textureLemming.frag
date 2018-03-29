#version 330

uniform vec4 color;
//uniform vec2 center;
uniform sampler2D tex;
uniform float time;
in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);

	if(texColor.a < 0.5f)
		discard;
	outColor = color * texColor;
	outColor.rb = outColor.rb*cos(time*0.02);
	outColor.gr = outColor.gr*sin(time*0.02);
	outColor.bg = outColor.bg*tan(time*0.03);
}

