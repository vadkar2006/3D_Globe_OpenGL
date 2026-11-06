#pragma once

#include <cmath>
#include <random>





struct vec3{
    float x;
    float y;
    float z;

    vec3(float x1 = 0.0, float y1 = 0.0, float z1 = 0.0): x(x1), y(y1), z(z1){}


    float Norm(){return sqrt(x*x + y*y + z*z);}




    vec3 operator -() const{return vec3(-x, -y, -z);}

    vec3 operator +(float a) const{return vec3(x + a, y + a, z + a);}
    vec3 operator -(float a) const{return vec3(x - a, y - a, z - a);}
    vec3 operator *(float a) const{return vec3(x * a, y * a, z * a);}
    vec3 operator /(float a) const{return vec3(x / a, y / a, z / a);}

    vec3 operator +(const vec3& new_vec) const{return vec3(x + new_vec.x, y + new_vec.y, z + new_vec.z);}
    vec3 operator -(const vec3& new_vec) const{return vec3(x - new_vec.x, y - new_vec.y, z - new_vec.z);}


    void operator +=(float a) {x += a, y += a, z += a;}
    void operator -=(float a) {x -= a, y -= a, z -= a;}
    void operator *=(float a) {x *= a, y *= a, z *= a;}
    void operator /=(float a) {x /= a, y /= a, z /= a;}

    void operator +=(const vec3& new_vec) {x += new_vec.x, y += new_vec.y, z += new_vec.z;}
    void operator -=(const vec3& new_vec) {x -= new_vec.x, y -= new_vec.y, z -= new_vec.z;}


    //Скалярное произведение
    float operator &(const vec3& new_vec) const{return x*new_vec.x + y*new_vec.y + z*new_vec.z;}
    //Векторное произведение
    vec3 operator |(const vec3& new_vec) const {return vec3(y*new_vec.z - z*new_vec.y, -(x*new_vec.z - z*new_vec.x), x*new_vec.y - y*new_vec.x);}
    //Нормализация
    vec3& operator ~(){*this /= sqrt(x*x + y*y + z*z); return *this;}

};


class Figure{
public:
    enum Type_Figure{Polygon, Parallelogram, Floor, Tetrahedron, Parallelepiped, Ball, Other};
private:
    Type_Figure figure_name;

    float coordinate_system[4][4];//сначала вектор ([i]) потом координата ([i][j])

    std::vector<float> all_static_vertices;//10d вектор: 3 локальные координаты, id фигуры, 3 цвета, имя фигуры, 2 координаты текстуры
    std::vector<unsigned int> all_static_indices;// масив последовательной отрисовки вершин

    GLuint Texture;

public:
    /*    // Конструктор перемещения
    Figure(Figure&& other) noexcept
        : figure_name(other.figure_name),
          all_static_vertices(std::move(other.all_static_vertices)),
          all_static_indices(std::move(other.all_static_indices))
    {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                coordinate_system[i][j] = other.coordinate_system[i][j];

        
        other.figure_name = Type_Figure::Other;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                other.coordinate_system[i][j] = 0.0;
        for(int i = 0; i < 3; i++)
            other.coordinate_system[i+1][i] = 1.0;
    }*/

    Figure(Type_Figure name, std::vector<vec3>& all_mas_vertices_new, std::vector<vec3>& all_mas_color_new){
        figure_name = name;

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                coordinate_system[i][j] = 0.0;
        for(int i = 0; i < 3; i++)
            coordinate_system[i+1][i] = 1.0;
        


        //на вход просто 3 координаты вершин
        if(name == Polygon){
            vec3 center = (all_mas_vertices_new[0] + all_mas_vertices_new[1] + all_mas_vertices_new[2])/3;

            coordinate_system[0][0] = center.x;
            coordinate_system[0][1] = center.y;
            coordinate_system[0][2] = center.z;

            
            all_static_vertices = std::vector<float>(10 * 3, 0.0);

            for(int i = 0; i < 3; i++){
                vec3 pos = all_mas_vertices_new[i] - center;

                all_static_vertices[i*10] = pos.x;
                all_static_vertices[i*10 + 1] = pos.y;
                all_static_vertices[i*10 + 2] = pos.z;

                all_static_vertices[i*10 + 3] = -1;

                all_static_vertices[i*10 + 4] = all_mas_color_new[i].x;
                all_static_vertices[i*10 + 5] = all_mas_color_new[i].y;
                all_static_vertices[i*10 + 6] = all_mas_color_new[i].z;

                all_static_vertices[i*10 + 7] = figure_name;

                all_static_vertices[i*10 + 7] = 0.0;
                all_static_vertices[i*10 + 7] = 0.0;
            }

            all_static_indices = std::vector<unsigned int>(3);

            all_static_indices[0] = 0;
            all_static_indices[1] = 1;
            all_static_indices[2] = 2;

            return;
        }
        //на вход точки ABC параллелепипеда ABCD
        if(name == Parallelogram){
                vec3 center = (all_mas_vertices_new[2] - all_mas_vertices_new[0])/2 + all_mas_vertices_new[0];
                vec3 color = (all_mas_color_new[0] + all_mas_color_new[1] + all_mas_color_new[2])/3;

                coordinate_system[0][0] = center.x;
                coordinate_system[0][1] = center.y;
                coordinate_system[0][2] = center.z;

                
                all_static_vertices = std::vector<float>(10 * 4, 0.0);
                //3 вершины
                for(int i = 0; i < 3; i++){
                    vec3 pos = all_mas_vertices_new[i] - center;

                    all_static_vertices[i*10] = pos.x;
                    all_static_vertices[i*10 + 1] = pos.y;
                    all_static_vertices[i*10 + 2] = pos.z;

                    all_static_vertices[i*10 + 3] = -1;

                    all_static_vertices[i*10 + 4] = all_mas_color_new[i].x;
                    all_static_vertices[i*10 + 5] = all_mas_color_new[i].y;
                    all_static_vertices[i*10 + 6] = all_mas_color_new[i].z;

                    all_static_vertices[i*10 + 7] = figure_name;

                    all_static_vertices[i*10 + 8] = 0;
                    all_static_vertices[i*10 + 9] = 0;
                }
                //последняя вершина
                vec3 pos = all_mas_vertices_new[0] + (all_mas_vertices_new[2] - all_mas_vertices_new[1]) - center;
                
                all_static_vertices[3*10] = pos.x;
                all_static_vertices[3*10 + 1] = pos.y;
                all_static_vertices[3*10 + 2] = pos.z;

                all_static_vertices[3*10 + 3] = -1;

                all_static_vertices[3*10 + 4] = color.x;
                all_static_vertices[3*10 + 5] = color.y;
                all_static_vertices[3*10 + 6] = color.z;

                all_static_vertices[3*10 + 7] = figure_name;

                all_static_vertices[3*10 + 8] = 0;
                all_static_vertices[3*10 + 9] = 0;




                //порядок отрисовки
                all_static_indices = {0, 1, 2, 0, 2, 3};

                return;
            }
        //на вход точки A_B_C_B1 параллелепипеда ABCDA1B1C1D1
        if(name == Parallelepiped){
            vec3 pos;

            vec3 BA = all_mas_vertices_new[0] - all_mas_vertices_new[1];
            vec3 BC = all_mas_vertices_new[2] - all_mas_vertices_new[1];
            vec3 BB1 = all_mas_vertices_new[3] - all_mas_vertices_new[1];

            vec3 center = (BA + BC + BB1)/2 + all_mas_vertices_new[1];


            //недостоющий цвет будет получен из среднего известных на общей грани
            vec3 color_D = (all_mas_color_new[0] + all_mas_color_new[1] + all_mas_color_new[2])/3;
            vec3 color_A1 = (all_mas_color_new[0] + all_mas_color_new[1] + all_mas_color_new[3])/3;
            vec3 color_C1 = (all_mas_color_new[1] + all_mas_color_new[2] + all_mas_color_new[3])/3;
            vec3 color_D1 = (color_A1 + all_mas_color_new[3] + color_C1)/3;


            coordinate_system[0][0] = center.x;
            coordinate_system[0][1] = center.y;
            coordinate_system[0][2] = center.z;

            
            all_static_vertices = std::vector<float>(10 * 4 * 2, 0.0);
            //3 вершины ABC
            for(int i = 0; i < 3; i++){
                pos = all_mas_vertices_new[i] - center;

                all_static_vertices[i*10] = pos.x;
                all_static_vertices[i*10 + 1] = pos.y;
                all_static_vertices[i*10 + 2] = pos.z;

                all_static_vertices[i*10 + 3] = -1;

                all_static_vertices[i*10 + 4] = all_mas_color_new[i].x;
                all_static_vertices[i*10 + 5] = all_mas_color_new[i].y;
                all_static_vertices[i*10 + 6] = all_mas_color_new[i].z;

                all_static_vertices[i*10 + 7] = figure_name;

                all_static_vertices[i*10 + 8] = 0;
                all_static_vertices[i*10 + 9] = 0;
            }
            int k = 3;
            //вершина D
            pos = all_mas_vertices_new[0] + BC - center;
            all_static_vertices[k*10] = pos.x;
            all_static_vertices[k*10 + 1] = pos.y;
            all_static_vertices[k*10 + 2] = pos.z;
            all_static_vertices[k*10 + 3] = -1;
            all_static_vertices[k*10 + 4] = color_D.x;
            all_static_vertices[k*10 + 5] = color_D.y;
            all_static_vertices[k*10 + 6] = color_D.z;
            all_static_vertices[k*10 + 7] = figure_name;
            all_static_vertices[k*10 + 8] = 0;
            all_static_vertices[k*10 + 9] = 0;
            k++;

            //вершина A1
            pos = all_mas_vertices_new[0] + BB1 - center;
            all_static_vertices[k*10] = pos.x;
            all_static_vertices[k*10 + 1] = pos.y;
            all_static_vertices[k*10 + 2] = pos.z;
            all_static_vertices[k*10 + 3] = -1;
            all_static_vertices[k*10 + 4] = color_A1.x;
            all_static_vertices[k*10 + 5] = color_A1.y;
            all_static_vertices[k*10 + 6] = color_A1.z;
            all_static_vertices[k*10 + 7] = figure_name;
            all_static_vertices[k*10 + 8] = 0;
            all_static_vertices[k*10 + 9] = 0;
            k++;

            //вершина B1
            pos = all_mas_vertices_new[3] - center;
            all_static_vertices[k*10] = pos.x;
            all_static_vertices[k*10 + 1] = pos.y;
            all_static_vertices[k*10 + 2] = pos.z;
            all_static_vertices[k*10 + 3] = -1;
            all_static_vertices[k*10 + 4] = all_mas_color_new[3].x;
            all_static_vertices[k*10 + 5] = all_mas_color_new[3].y;
            all_static_vertices[k*10 + 6] = all_mas_color_new[3].z;
            all_static_vertices[k*10 + 7] = figure_name;
            all_static_vertices[k*10 + 8] = 0;
            all_static_vertices[k*10 + 9] = 0;
            k++;

            //вершина C1
            pos = all_mas_vertices_new[2] + BB1 - center;
            all_static_vertices[k*10] = pos.x;
            all_static_vertices[k*10 + 1] = pos.y;
            all_static_vertices[k*10 + 2] = pos.z;
            all_static_vertices[k*10 + 3] = -1;
            all_static_vertices[k*10 + 4] = color_C1.x;
            all_static_vertices[k*10 + 5] = color_C1.y;
            all_static_vertices[k*10 + 6] = color_C1.z;
            all_static_vertices[k*10 + 7] = figure_name;
            all_static_vertices[k*10 + 8] = 0;
            all_static_vertices[k*10 + 9] = 0;
            k++;

            //вершина D1
            pos = all_mas_vertices_new[0] + BC + BB1 - center;
            all_static_vertices[k*10] = pos.x;
            all_static_vertices[k*10 + 1] = pos.y;
            all_static_vertices[k*10 + 2] = pos.z;
            all_static_vertices[k*10 + 3] = -1;
            all_static_vertices[k*10 + 4] = color_D1.x;
            all_static_vertices[k*10 + 5] = color_D1.y;
            all_static_vertices[k*10 + 6] = color_D1.z;
            all_static_vertices[k*10 + 7] = figure_name;
            all_static_vertices[k*10 + 8] = 0;
            all_static_vertices[k*10 + 9] = 0;
            k++;


            //порядок отрисовки
            all_static_indices = {
                0, 1, 2,
                2, 3, 0,

                0, 1, 4,
                1, 4, 5,

                1, 2, 6,
                1, 6, 5,

                2, 3, 7,
                2, 7, 6,

                3, 0, 4,
                3, 4, 7,

                4, 5, 6,
                4, 6, 7
            };

            return;
        }
    }
    /*Figure(int count_dot, vec3 min_xyz, vec3 max_xyz){
        std::random_device rd;
        std::mt19937 gen(rd());

        if(min_xyz.x > max_xyz.x){
            float swap = min_xyz.x;
            min_xyz.x = max_xyz.x;
            max_xyz.x = swap;
        }
        if(min_xyz.y > max_xyz.y){
            float swap = min_xyz.y;
            min_xyz.y = max_xyz.y;
            max_xyz.y = swap;
        }
        if(min_xyz.z > max_xyz.z){
            float swap = min_xyz.z;
            min_xyz.z = max_xyz.z;
            max_xyz.z = swap;
        }

        // Равномерное распределение для дробных чисел
        std::uniform_real_distribution<float> dist_x(min_xyz.x, max_xyz.x);
        std::uniform_real_distribution<float> dist_y(min_xyz.y, max_xyz.y);
        std::uniform_real_distribution<float> dist_z(min_xyz.z, max_xyz.z);

        std::uniform_real_distribution<float> dist_r(0.0, 1.0);
        std::uniform_real_distribution<float> dist_g(0.0, 1.0);
        std::uniform_real_distribution<float> dist_b(0.0, 1.0);

        if(count_dot < 3)
            count_dot = 3;

        std::vector<vec3> raw_vertices;
        std::vector<vec3> raw_color;

        raw_vertices.reserve(count_dot);
        raw_color.reserve(count_dot);
        for(int i = 0; i < count_dot; i++){
            raw_vertices.emplace_back(vec3(dist_x(gen), dist_y(gen), dist_z(gen)));
            raw_color.emplace_back(vec3(dist_r(gen), dist_g(gen), dist_b(gen)));
        }
        
        if(count_dot == 3){
            Figure trig(Polygon, raw_vertices, raw_color);
            *this = std::move(trig);
            return;
        }
        if(count_dot == 4){
            Figure trig(Tetrahedron, raw_vertices, raw_color);
            *this = std::move(trig);
            return;
        }

        
        //далее создание выпуклой оболочки
        

    }*/
    
    Figure(int count_polygon_dot, float radius, std::vector<float> start_dot = std::vector<float>(3, 0.0)){
        const static double PI = std::acos(-1.0);

        if(count_polygon_dot <= 3)
            count_polygon_dot = 4;
        
        figure_name = Ball;

        if(radius <= 0)
            radius = 1.0;
        if(start_dot.size() < 3)
            start_dot = std::vector<float>(3, 0.0);

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                coordinate_system[i][j] = 0.0;
        for(int i = 0; i < 3; i++)
            coordinate_system[i+1][i] = 1.0;
        for(int i = 0; i < 3; i++)
            coordinate_system[0][i] = start_dot[i];



        std::vector<vec3> all_mas_vertices_new;


        for(int j = 0; j <= count_polygon_dot; j++)
            for(int i = 0; i <= count_polygon_dot; i++) 
                all_mas_vertices_new.push_back(vec3(std::sin(PI*i/count_polygon_dot) * std::cos(PI*2*j/count_polygon_dot), std::sin(PI*i/count_polygon_dot) * std::sin(PI*2*j/count_polygon_dot), std::cos(PI*i/count_polygon_dot)));
        

        all_static_vertices = std::vector<float>(10 * all_mas_vertices_new.size(), 0.0);

        for(int i = 0; i < all_mas_vertices_new.size(); i++){
            vec3 pos = all_mas_vertices_new[i];

            int teta = i % (count_polygon_dot + 1);
            int fhi = i / (count_polygon_dot + 1);

            all_static_vertices[i*10] = radius * pos.x;
            all_static_vertices[i*10 + 1] = radius * pos.y;
            all_static_vertices[i*10 + 2] = radius * pos.z;

            all_static_vertices[i*10 + 3] = -1;

            all_static_vertices[i*10 + 4] = (float)teta / count_polygon_dot;
            all_static_vertices[i*10 + 5] = (float)fhi / count_polygon_dot;
            all_static_vertices[i*10 + 6] =  std::atan2(teta, fhi);

            all_static_vertices[i*10 + 7] = figure_name;

            all_static_vertices[i*10 + 8] = (float)fhi / count_polygon_dot;
            all_static_vertices[i*10 + 9] = (float)teta / count_polygon_dot;
        }


        all_static_indices = std::vector<unsigned int>();
        int stacks = count_polygon_dot;
        int sectors = count_polygon_dot;

        for(int i = 0; i < stacks; i++){
            for(int j = 0; j < sectors; j++){
                int curr = i * (sectors + 1) + j;
                int next = curr + (sectors + 1);

                all_static_indices.push_back(curr);
                all_static_indices.push_back(next);
                all_static_indices.push_back(curr + 1);

                all_static_indices.push_back(next);
                all_static_indices.push_back(next + 1);
                all_static_indices.push_back(curr + 1);
            }
        }
        return;     
    }

    int Get_Count_Vertices() const{return all_static_vertices.size() / 10;}
    int Get_Count_Triangles() const{return all_static_indices.size() / 3;}

    void Push_Texture(GLuint tex){Texture = tex;}
    GLuint Get_Texture() const{return Texture;}


    const float* Get_coordinate_system() const{return &(coordinate_system[0][0]);}
    const std::vector<float>& Get_all_static_vertices() const{return all_static_vertices;}
    const std::vector<unsigned int>& Get_all_static_indices() const{return all_static_indices;}

    Figure& operator=(Figure&& other) noexcept{
        if (this == &other)
            return *this;
        
        figure_name = other.figure_name;
        all_static_vertices = std::move(other.all_static_vertices);
        all_static_indices = std::move(other.all_static_indices);
        
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                coordinate_system[i][j] = other.coordinate_system[i][j];
        
        other.figure_name = Type_Figure::Other;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                other.coordinate_system[i][j] = 0.0f;
        for(int i = 0; i < 3; i++)
            other.coordinate_system[i+1][i] = 1.0f;
        
        return *this;
    }
    
};

class Figure_Conveyor{
    std::vector<Figure*> queue_figure;

    GLuint SSBO_Coordinate_Systems;
    
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<GLuint> textures_mas;

    std::vector<float> coordinate_systems;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int count_vertices_in_mas;
    unsigned int count_figure_in_mas;
    unsigned int count_triangles_in_mas;
public:
    Figure_Conveyor(): count_vertices_in_mas(0), count_figure_in_mas(0), count_triangles_in_mas(0){}

    Figure_Conveyor(const Figure_Conveyor& f) = delete;
    Figure_Conveyor(Figure_Conveyor&& f) = delete;

    GLuint Get_VAO() const{return VAO;}

    unsigned int Get_count_vertices_in_mas() const{return count_vertices_in_mas;};
    unsigned int Get_count_figure_in_mas() const{return count_figure_in_mas;};
    unsigned int Get_count_triangles_in_mas() const{return count_triangles_in_mas;};
    unsigned int Get_count_texture() const{return textures_mas.size();}

    void Push_Figure(Figure* new_figure){
        queue_figure.push_back(new_figure);
        textures_mas.push_back(new_figure -> Get_Texture());
    }


    GLuint Get_Texture(int i) const{return textures_mas[i];}
     


    void Preparation_To_VideoCard(){
        while(count_figure_in_mas < queue_figure.size()){
            for(int i = 0; i < 16; i++)
                coordinate_systems.push_back((queue_figure[count_figure_in_mas] -> Get_coordinate_system())[i]);
            
            for(int i = 0; i < (queue_figure[count_figure_in_mas] -> Get_Count_Vertices()) * 10; i++){
                if(i % 10 == 3)
                    vertices.push_back(count_figure_in_mas);
                else
                    vertices.push_back((queue_figure[count_figure_in_mas] -> Get_all_static_vertices())[i]);
            }
            
            for(int i = 0; i < (queue_figure[count_figure_in_mas] -> Get_Count_Triangles()) * 3; i++)
                indices.push_back((queue_figure[count_figure_in_mas] -> Get_all_static_indices())[i] + count_vertices_in_mas);

            count_vertices_in_mas += queue_figure[count_figure_in_mas] -> Get_Count_Vertices();
            count_triangles_in_mas += queue_figure[count_figure_in_mas] -> Get_Count_Triangles();
            count_figure_in_mas++;
        }
    }
    void Go_To_VideoCard(){
        // Создаем VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        // Создаем VBO и загружаем вершины
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
        
        // Создаем EBO и загружаем индексы
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        // Настраиваем атрибуты вершин
        // Атрибут 0: позиция (3 float)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Атрибут 1: id фигуры (1 float)
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // Атрибут 2: цвет (3 float)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Атрибут 3: имя фигуры (1 float)
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Атрибут 4: координаты текстуры (2 float)
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);


        glCreateBuffers(1, &SSBO_Coordinate_Systems);
        glNamedBufferStorage(SSBO_Coordinate_Systems, 
                            count_figure_in_mas * 4 * 4 * sizeof(float),
                            nullptr, 
                            GL_DYNAMIC_STORAGE_BIT);
        
        
        glNamedBufferSubData(SSBO_Coordinate_Systems, 0, 
                            count_figure_in_mas * 4 * 4 * sizeof(float), 
                            coordinate_systems.data());
        
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_Coordinate_Systems);
    }


    ~Figure_Conveyor(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &SSBO_Coordinate_Systems);
    }
};
