

#version 420 core                                            
                                                             
layout (location = 0) in vec3 position;                                            
                                                             
out VS_OUT                                                   
{                                                            
    vec4 color;                                              
} vs_out;                                                    
                                                             
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)                                              
{                                                            
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
    vs_out.color = vec4(position, 1.0) * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}                                                            