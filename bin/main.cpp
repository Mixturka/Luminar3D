#include "../lib/Luminar.h"

int main(int arc, char* argv[]) {
  Camera cam;
  Matrix4 m(0.718762, 0.615033, -0.324214, 0, -0.393732, 0.744416, 0.539277, 0,
            0.573024, -0.259959, 0.777216, 0, 0.526967, 1.254234, -2.53215, 1);
  // Matrix4 m2(m.Inverse());
  Matrix4 m2 = m.Inverse();
  Vec3<float> Pworld(-0.315792, 1.4489, -2.48901);
  Vec3<float> Plocal;

  Matrix4 m_rotation_x(1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1);
  Vec3<float> test(1.22, 0, 1.97);

  Vec3 test1 = test * m_rotation_x;

  Plocal = Pworld * m2;

  // std::cout << Plocal.x << ' ' << Plocal.y << ' ' << Plocal.z;
  std::cout << test1.x << ' ' << test1.y << ' ' << test1.z << std::endl;

  Mesh mesh_("/Users/deniv/Documents/Luminar3D/res/cube.obj");

  cam.focal_length_ = 35;
  cam.aperture_.width = 0.98;
  cam.aperture_.height = 0.735;
  cam.near_clipping_ = 0.1;
  cam.far_clipping_ = 1000;

  cam.SetCanvas();

  Luminar lum;
  lum.Init(cam, mesh_);
  //lum.Draw();
  return 0;
}
