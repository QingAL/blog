# 动态库链接中的符号冲突问题
## 包含两个相同符号的动态库链接
在此 demo 中，main 函数中依赖了一个外部符号 function，而在动态链接库中 MyLib1 和 MyLib2 中都有此符号的实现，那么 main 函数调用的是哪一个呢？

与 CMake 中的 target_link_libraries link 顺序相关

## LD_PRELOAD 全局符号介入
Linux 中的环境变量，可以影响程序运行时的链接，优先加载动态库，覆盖符号。

## demo
```shell
mdkir build
cd build
cmake ..
cmake --build .
// 执行 main
./main
// 全局符号介入
export LD_PRELOAD='./PreLoad/libPreLoad.so'
./main
unset LD_PRELOAD
```