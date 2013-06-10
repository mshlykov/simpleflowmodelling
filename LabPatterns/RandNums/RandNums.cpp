#include<iostream>
#include<cmath>
#include<ctime>
#include<fstream>

int main()
  {
  srand((int)time(0));
  int K1 = 0, K2 = 0, K3 = 0, K4 = 0;
  std::cin >> K1 >> K2 >> K3 >> K4;
  double x, y, nx, ny, PI = 3.14159, 
    shift_x1 = 2, shift_y1 = 2 , shift_x2 = -2, shift_y2 = -2, ro = 0.5;
  std::ofstream f("../Proj1/Teaching1.txt"), g("../Proj1/Teaching2.txt"),
    h("../Proj1/Test1.txt"), t("../Proj1/Test2.txt");
  f << K1 << '\n';
  for(int i = 0; i < K1; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    nx = cos(2 * PI * x) * sqrt(-2 * log(y)) + shift_x1;
    ny = sin(2 * PI * x) * sqrt(-2 * log(y)) + shift_y1;
    f << nx << ' ' << ny << '\n';
    }

  g << K2 << '\n';
  for(int i = 0; i < K2; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    nx = cos(2 * PI * x) * sqrt(-2 * log(y)) + shift_x2;
    ny = sin(2 * PI * x) * sqrt(-2 * log(y)) + shift_y2;
    g << nx << ' ' << ny << '\n';
    }

  h << K3 << '\n';
  for(int i = 0; i < K3; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    nx = cos(2 * PI * x) * sqrt(-2 * log(y)) + shift_x1;
    ny = sin(2 * PI * x) * sqrt(-2 * log(y)) + shift_y1;
    h << nx << ' ' << ny << '\n';
    }

  t << K4 << '\n';
  for(int i = 0; i < K4; ++i)
    {
    x = rand() / (double)RAND_MAX;
    y = rand() / (double)RAND_MAX;

    nx = cos(2 * PI * x) * sqrt(-2 * log(y)) + shift_x2;
    ny = sin(2 * PI * x) * sqrt(-2 * log(y)) + shift_y2;
    t << nx << ' ' << ny << '\n';
    }
  return 0;
  }