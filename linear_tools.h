#ifndef LITOOLS_H
#define LITOOLS_H

#include <Eigen\Dense>
#include <complex>
#include <string>
#include <vector>

using namespace Eigen;
using namespace std::complex_literals;

//*下面这些eig主要用于特征值和特征向量的储存和排序

/* 
*排序用法演示：
* Eigen::MatrixXd mat;

* 与h相关的代码

* eig example;
* EigenSolver solver(mat);
* eig.value = solver.eigenvalues();
* eig.vec = solver.eigenvectors();
* sort_eig(example); 
*/

//*于是就完成了对mat本征值和本征向量的排序，默认升序



//*复矩阵的特征值和特征向量
struct eig {
  Eigen::VectorXcd value;
  Eigen::MatrixXcd vec;

  eig() = default;

  eig(int dim) : value(dim), vec(dim, dim) {}
};

//*厄米矩阵的特征值和特征向量
struct eig_h {
  Eigen::VectorXd value;
  Eigen::MatrixXcd vec;

  eig_h() = default;

  eig_h(int dim) : value(dim), vec(dim, dim) {}
};

//*实矩阵的特征值和特征向量
struct eig_re {
  Eigen::VectorXd value;
  Eigen::MatrixXd vec;

  eig_re() = default;

  eig_re(int dim) : value(dim), vec(dim, dim) {}
};

//*对根据本征值的实部对本征值和本征向量排序
//*eig_system为包含本征值和本征向量的结构体，可以为上面的三种
//*sort_method为排序方式，asc为升序，desc为降序
template <typename eig_system>
eig_system sort_eig(eig_system &eig_data, std::string sort_method = "asc") {
  int dim = eig_data.value.size();
  Eigen::VectorXi indices(dim);
  Eigen::VectorXd real_value;
  Eigen::VectorXd sorted_value(dim);
  decltype(eig_data.vec) sorted_vec(dim, dim);

  std::iota(indices.begin(), indices.end(), 0);
  if (std::is_same_v<decltype(eig_data.value[0]), std::complex<double>>) {
    real_value = eig_data.value.real();
  } else {
    real_value = eig_data.value;
  }
  std::sort(indices.begin(), indices.end(),
            [&real_value, &sort_method](int i, int j) {
              if (sort_method == "asc") {
                return real_value(i) < real_value(j);
              } else if (sort_method == "desc") {
                return real_value(i) > real_value(j);
              } else {
                return false; // 如果 sort_method 不为 "asc" 或 "desc"
              }
            });

  for (int i = 0; i < dim; i++) {
    sorted_value(i) = eig_data.value(indices[i]);
    sorted_vec.col(i) = eig_data.vec.col(indices[i]);
  }

  eig_data.value = sorted_value;
  eig_data.vec = sorted_vec;

  return eig_data;
}

#endif // LITOOLS_H
