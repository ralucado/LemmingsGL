#version 330

uniform vec4 color;
uniform vec2 center;
uniform sampler2D tex;
uniform float time;

in vec2 position;
in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	outColor = color * texColor;
	if(texColor.a < 0.5f){
			outColor = vec4(0.5, 0.5, 0.5, 1);
			outColor.rb = outColor.rb*cos(time*0.02);
			outColor.gr = outColor.gr*sin(time*0.02);
			outColor.bg = outColor.bg*tan(time*0.03);
			// Base color for the effect

    // size of rectangle in terms of uv 
    vec2 rectangleSize = vec2(2.0); 

    // distance field, 0.0 - point is inside rectangle, 1.0 point is on the far edge of the border.
    float distanceField = length(max(abs(position.uv)-rectangleSize,0.0) / 0.2);

    // calculate alpha accordingly to the value of the distance field
    float alpha = 1.0 - distanceField;

    outColor.a = alpha;

	}
}

