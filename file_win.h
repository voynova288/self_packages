#ifndef FILE_WIN_H
#define FILE_WIN_H

#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

std::wstring to_wstring(const std::string &str);

template <typename FILEPATH_T = std::filesystem::path,
          typename = std::enable_if_t<
              std::is_same_v<FILEPATH_T, std::filesystem::path> ||
              std::is_same_v<FILEPATH_T, std::string> ||
              std::is_same_v<FILEPATH_T, std::wstring>>>
class MemoryMapper {
  //*适用于indows的内存映射类
  //*std::string仅支持英文和一部分符号，std::filesystem::path和std::wstring支持中文和更多符号
private:
  std::filesystem::path filepath;
  size_t filesize;
  HANDLE h_file;
  HANDLE h_map_file;
  void *pbuf;

  bool create_if_not_exists;

public:
  MemoryMapper(const FILEPATH_T &filepath_, size_t filesize_,
               bool create_if_not_exists_ = 0);
  ~MemoryMapper();

  bool map_memory();
  void unmap_memory();

  template <typename DATA_T> DATA_T *get_mapped_memory();
};

MemoryMapper(const std::filesystem::path &,
             size_t) -> MemoryMapper<std::filesystem::path>;
MemoryMapper(const std::string &, size_t) -> MemoryMapper<std::string>;
MemoryMapper(const std::wstring &, size_t) -> MemoryMapper<std::wstring>;

std::wstring to_wstring(const std::string &str) {
  std::wstring wstr(str.begin(), str.end());
  return wstr;
}

template <typename FILEPATH_T, typename DUMMY>
MemoryMapper<FILEPATH_T, DUMMY>::MemoryMapper(const FILEPATH_T &filepath_,
                                              size_t filesize_,
                                              bool create_if_not_exists_)
    : filepath(filepath_), filesize(filesize_),
      create_if_not_exists(create_if_not_exists_), h_file(NULL),
      h_map_file(NULL), pbuf(NULL) {
  //*构造函数，创建内存映射
  //*参数：文件路径和映射内存的大小，以及文件创建的方式
  map_memory();
}

template <typename FILEPATH_T, typename DUMMY>
MemoryMapper<FILEPATH_T, DUMMY>::~MemoryMapper() {
  //*析构函数，解除内存映射
  unmap_memory();
}

template <typename FILEPATH_T, typename DUMMY>
bool MemoryMapper<FILEPATH_T, DUMMY>::map_memory() {
  DWORD create_mode = create_if_not_exists ? OPEN_ALWAYS : OPEN_EXISTING;

  h_file = CreateFileW(filepath.c_str(), GENERIC_ALL, 0, NULL, create_mode,
                       FILE_ATTRIBUTE_NORMAL, NULL);
  if (h_file == INVALID_HANDLE_VALUE) {
    std::cerr << "Error opening file: " << filepath << "\n"
              << "Error: " << GetLastError() << std::endl;
    return false;
  }

  h_map_file =
      CreateFileMapping(h_file, NULL, PAGE_READWRITE, 0, filesize, NULL);
  if (h_map_file == NULL) {
    std::cerr << "Error creating mapping" << GetLastError() << std::endl;
    return false;
  }

  pbuf = MapViewOfFile(h_map_file, FILE_MAP_ALL_ACCESS, 0, 0, filesize);
  if (pbuf == NULL) {
    std::cerr << "Error mapping file to memory" << GetLastError() << std::endl;
    CloseHandle(h_map_file);
    CloseHandle(h_file);
    return false;
  }

  return true;
}

template <typename FILEPATH_T, typename DUMMY>
template <typename DATA_T>
DATA_T *MemoryMapper<FILEPATH_T, DUMMY>::get_mapped_memory() {
  //*返回映射内存的指针
  //*使用方法（以double为例）
  //*ptr = mapper.get_memory<double>();
  return static_cast<DATA_T *>(pbuf);
}

template <typename FILEPATH_T, typename DUMMY>
void MemoryMapper<FILEPATH_T, DUMMY>::unmap_memory() {
  if (pbuf != NULL) {
    UnmapViewOfFile(pbuf);
    pbuf = NULL;
  }

  if (h_map_file != NULL) {
    CloseHandle(h_map_file);
    h_map_file = NULL;
  }

  if (h_file != NULL) {
    CloseHandle(h_file);
    h_file = NULL;
  }
}

#endif // FILE_WIN_H