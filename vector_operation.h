#ifndef VECTOR_OPERATION_H
#define VECTOR_OPERATION_H
#include <iostream>
#include <vector>

template <typename element_type>
element_type add_vectors(element_type element1, element_type element2) {
  return element1 + element2;
}

template <typename element_type>
std::vector<element_type> add_vectors(std::vector<element_type> &vec1,
                                      std::vector<element_type> &vec2) {
  if (vec1.size() != vec2.size()) {
    throw std::invalid_argument("Both vectors must have the same size");
  }

  std::vector<element_type> result(vec1.size());
  for (size_t i = 0; i < vec1.size(); ++i) {
    result[i] = add_vectors(vec1[i], vec2[i]);
  }

  return result;
}

template <typename element_type>
element_type minus_vectors(element_type element1, element_type element2) {
  return element1 - element2;
}

template <typename element_type>
std::vector<element_type> minus_vectors(std::vector<element_type> &vec1,
                                        std::vector<element_type> &vec2) {
  if (vec1.size() != vec2.size()) {
    throw std::invalid_argument("Both vectors must have the same size");
  }

  std::vector<element_type> result(vec1.size());
  for (size_t i = 0; i < vec1.size(); ++i) {
    result[i] = minus_vectors(vec1[i], vec2[i]);
  }

  return result;
}

template <typename element_type>
element_type add_num_to_vector(element_type element, element_type num) {
  return element + num;
}

template <typename vector_type, typename element_type>
std::vector<vector_type> add_num_to_vector(std::vector<vector_type> &vec,
                                           element_type num) {
  std::vector<vector_type> result(vec.size());
  for (size_t i = 0; i < vec.size(); ++i) {
    result[i] = add_num_to_vector(vec[i], num);
  }
  return result;
}

template <typename T>
void init_shape(std::vector<T> &vec, std::vector<int> &shape, size_t depth) {
  if ((shape.empty()) || (depth >= shape.size())) {
    return;
  }

  vec.resize(shape[depth]);
}

template <typename T>
void init_shape(std::vector<std::vector<T>> &vec, std::vector<int> &shape,
                size_t depth) {
  if ((shape.empty()) || (depth >= shape.size())) {
    return;
  }

  vec.resize(shape[depth]);
  if (depth < shape.size() - 1) {
    for (auto &subvec : vec) {
      init_shape(subvec, shape, depth + 1);
    }
  }
}

template <typename T>
void init_shape(std::vector<T> &vec, std::vector<int> &shape) {
  if (shape.empty())
    return;
  init_shape(vec, shape, 0);
}

#endif // VECTOR_OPERATION_H
