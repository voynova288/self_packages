#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H
#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <matplotlibcpp.h>
#include <numeric>
#include <string>
#include <vector>
#include <vector_operation.h>

using namespace std;
using namespace Eigen;
using namespace std::complex_literals;
namespace plt = matplotlibcpp;

int round_(double x) { return static_cast<int>(std::round(x)); }

bool dirac_func(int i, int j){
  return (i==j);
}

bool dirac_func(double x, double y,double tor = 1e-8){
  return (std::abs(x-y)<tor);
}

//*泡利矩阵
Eigen::MatrixXcd sigma(int i) {
  Eigen::MatrixXcd mat(2, 2);
  switch (i) {
  case 1:
    mat << 0, 1, 1, 0;
    break;
  case 2:
    mat << 0, -1.0i, 1.0i, 0;
    break;
  case 3:
    mat << 1, 0, 0, -1;
    break;
  default:
    mat << 1, 0, 0, 1;
  }

  return mat;
}

double gaussian_1d(double x, double mu = 0.0, double sigma = 1.0) {
  return exp(-0.5 * std::pow(x - mu, 2) / std::pow(sigma, 2)) /
         std::pow(2 * M_PI * sigma, 0.5);
}

double gaussian_2d(Eigen::Vector2d x, double mu = 0.0, double sigma = 1.0) {
  Eigen::Vector2d x_mu;
  x_mu << x[0] - mu, x[1] - mu;
  return exp(-0.5 * std::pow(x_mu.norm(), 2) / std::pow(sigma, 2)) /
         (2 * M_PI * sigma);
}

double gaussian_2d(Eigen::Vector2d x, Eigen::Vector2d mu,
                   Eigen::Matrix2d sigma) {
  return exp(-0.5 * (x - mu).transpose() * sigma.inverse() * (x - mu)) /
         (2 * M_PI * std::pow(sigma.determinant(), 0.5));
}

double gaussian_3d() {}

double gaussian_xd() {}

#endif // MATH_FUNCTIONS_H