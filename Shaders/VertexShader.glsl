#version 330

// TODO: get vertex attributes from each location

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 tex_coordinate;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// TODO: output values to fragment shader

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_tex_coordinate;
out vec3 frag_color;


float rand(vec2 co){
  return fract(sin(dot(co.xy, vec2(20.f))) * 0.1f);
}

void main()
{
	// TODO: send output to fragment shader

	frag_color = vec3 (1,0,0) + vec3(sin(Time), cos(Time), sin(Time));

	// TODO: compute gl_Position
	

	vec3 new_pos = v_position + v_normal * vec3 (rand(vec2 (Time, v_position[0])) - 0.7f, rand(vec2 (Time, v_position[1]))- 0.7f, 
	rand(vec2 (Time, v_position[2]))- 0.7f) ;

	gl_Position =  Projection * View * Model * vec4(new_pos, 1.0);
}
