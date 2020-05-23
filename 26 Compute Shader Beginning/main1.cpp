#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

float angle2Randian(float angle)
{
    return angle *(MY_PI / 180);
}
Eigen::Vector3f rotate(Eigen::Vector3f n,Eigen::Vector3f v,float angle)
{
    auto vProjN = n.dot(v) * n;

    auto v_1 = v - vProjN;

    auto w = n.cross(v);

    auto newV_1 = v_1 * cosf(angle) + w * sinf(angle);

    return vProjN + newV_1;
}
using namespace std;
Eigen::Matrix4f get_rotation(Eigen::Vector3f axis,float angle)
{
    Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();

    auto newX = rotate(axis,Vector3f(1, 0, 0), angle);
    auto newY = rotate(axis,Vector3f(0, 1, 0), angle);
    auto newZ = rotate(axis,Vector3f(0, 0, 1), angle);

    for (size_t i = 0; i < 3; i++)
    {
        rotation(0,i) = newX[i];
        rotation(1,i) = newY[i];
        rotation(2,i) = newZ[i];
    }
    
    return rotation;
}
Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    rotation_angle = angle2Randian(rotation_angle);
    // just for z Axis
    model << cosf(rotation_angle), sinf(rotation_angle), 0, 0,
             -sinf(rotation_angle), cosf(rotation_angle), 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;   

    //model = get_rotation(Eigen::Vector3f(1, 0, 0),rotation_angle);
    //model = get_rotation(Eigen::Vector3f(0, 1, 0),rotation_angle);
    //model = get_rotation(Eigen::Vector3f(0, 0, 1),rotation_angle);
  
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    // orthogonal projection
    Eigen::Matrix4f ortho_Proj;
    
    
    auto height = 2 * (zNear *  tanf(M_PI * (eye_fov / 360)));
    auto width = height * aspect_ratio;


    Eigen::Matrix4f ortho_Trans = Eigen::Matrix4f::Identity();
    ortho_Trans(2,3) = -(zNear + zFar) / 2;


    Eigen::Matrix4f ortho_Scale = Eigen::Matrix4f::Identity();


    ortho_Scale(0,0) = 2 / width;
    ortho_Scale(1,1) = 2 / height;
    ortho_Scale(2,2) = 2 / (zFar - zNear);


    ortho_Proj = ortho_Scale * ortho_Trans;
    

    // perspective projection
    Eigen::Matrix4f projection_Sub ;
    projection_Sub.fill(0);

    projection_Sub(0,0) = zNear;
    projection_Sub(1,1) = zNear;
    projection_Sub(2,2) = zNear + zFar;
    projection_Sub(2,3) = - zFar*zNear;
    projection_Sub(3,2) = 1;

    auto temp = ortho_Proj * projection_Sub;

    
    return temp;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
            
        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
