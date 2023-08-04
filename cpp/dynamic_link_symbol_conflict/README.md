# 动态库链接中的符号冲突问题
## 包含两个相同符号的动态库链接
在此 demo 中，main 函数中依赖了一个外部符号 function，而在动态链接库中 MyLib1 和 MyLib2 中都有此符号的实现，那么 main 函数调用的是哪一个呢？

与 CMake 中的 target_link_libraries link 顺序相关