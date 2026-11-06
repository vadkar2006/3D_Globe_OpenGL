#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float id_figure;
layout (location = 2) in vec3 aColor;
layout (location = 3) in float name_figure;
layout (location = 4) in vec2 coord_tex;


out vec3 Color;
out vec2 Coord_Tex;
out float Id_Texture;


layout(std430, binding = 0) buffer Buffer_Camera {vec3 camera[];};
layout(std430, binding = 1) buffer Buffer_Local_Coordinates {vec3 local_coordinates[];};


uniform ivec2 window;




void main() {
    int id_figure_int = int(id_figure);

    mat3 A = mat3(local_coordinates[id_figure_int*4 + 1], local_coordinates[id_figure_int*4 + 2], local_coordinates[id_figure_int*4 + 3]);
    mat3 B = mat3(camera[1], camera[2], camera[3]);


    vec3 global_pos = A * (aPos + local_coordinates[id_figure_int*4]);
    vec3 Pos_In_Camera = transpose(B) * (global_pos - camera[0]);


    float aspect = 1.0;

    mat4 projection = mat4(
        2.4142 / aspect,  0.0,      0.0,       0.0,    
        0.0,                2.4142,   0.0,       0.0,    
        0.0,                0.0,     -1.002,    -1.0,    
        0.0,                0.0,     -0.2002,    0.0     
    );

    float a = 1.0 * window.y / window.x;
    gl_Position = projection * vec4(Pos_In_Camera.x * a, Pos_In_Camera.yz, 1.0);
    
    Color = aColor;
    Coord_Tex = coord_tex;
    Id_Texture = id_figure;
}
