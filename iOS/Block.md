# Block

## Block 底层原理

objective-c 是 c 语言的超集，因此可以使用 clang 将 objectiv-c 代码转换为 c 代码查看具体实现。

`xcrun -sdk iphoneos clang -arch arm64 -rewrite-objc main.m`

```objc
#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        void (^block)(void) =  ^{
            NSLog(@"hello");
         };
        block();
    }
    return 0;
}
```
```cpp
struct __block_impl {
  void *isa;
  int Flags;
  int Reserved;
  void *FuncPtr;
};

struct __main_block_impl_0 {
  struct __block_impl impl;
  struct __main_block_desc_0 *Desc;
  __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc,
                      int flags = 0) {
    impl.isa = &_NSConcreteStackBlock;
    impl.Flags = flags;
    impl.FuncPtr = fp;
    Desc = desc;
  }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
  NSLog((
      NSString
          *)&__NSConstantStringImpl__var_folders_w5_8h00xnpd7jv_ht4xm6r1n7_80000gn_T_main_5d448c_mii_0);
}

static struct __main_block_desc_0 {
  size_t reserved;
  size_t Block_size;
} __main_block_desc_0_DATA = {0, sizeof(struct __main_block_impl_0)};

int main(int argc, const char *argv[]) {
  /* @autoreleasepool */ {
    __AtAutoreleasePool __autoreleasepool;
    void (*block)(void) =
        ((void (*)()) & __main_block_impl_0((void *)__main_block_func_0,
                                            &__main_block_desc_0_DATA));
    ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)(
        (__block_impl *)block);
  }
  return 0;
}
```
可以看出本次示例中 block 的底层结构是一个名为 `__main_block_impl_0` 的结构体，在我们声明 block 块时，即为 `__main_block_impl_0` 的构造。

本次示例中，分别传入了 `__main_block_func_0` 和 `__main_block_desc_0_DATA` 构造，其中 `__main_block_func_0` 为函数指针，为我们在 block 块中的具体实现 `NSLog();` 最后赋值给 block 结构体中的 `__block_impl.FuncPtr` 指针，`__main_block_desc_0_DATA` 则为我们声明结构体的大小。

最后调用 FuncPrt，并且将 block 自身传入。

## Block 变量捕获

```objc
#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    
    int intValue = 1;
    
    char * charValue = "hello";
    
    @autoreleasepool {
        void (^block)(void) =  ^{
            NSLog(@"int value is: %d, char value is: %s", intValue, charValue);
         };
        block();
    }
    return 0;
}
```
```cpp
struct __block_impl {
  void *isa;
  int Flags;
  int Reserved;
  void *FuncPtr;
};

struct __main_block_impl_0 {
  struct __block_impl impl;
  struct __main_block_desc_0 *Desc;
  int intValue;
  char *charValue;
  __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, int _intValue,
                      char *_charValue, int flags = 0)
      : intValue(_intValue), charValue(_charValue) {
    impl.isa = &_NSConcreteStackBlock;
    impl.Flags = flags;
    impl.FuncPtr = fp;
    Desc = desc;
  }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
  int intValue = __cself->intValue;      // bound by copy
  char *charValue = __cself->charValue;  // bound by copy

  NSLog(
      (NSString
           *)&__NSConstantStringImpl__var_folders_w5_8h00xnpd7jv_ht4xm6r1n7_80000gn_T_main_8c666d_mii_0,
      intValue, charValue);
}

static struct __main_block_desc_0 {
  size_t reserved;
  size_t Block_size;
} __main_block_desc_0_DATA = {0, sizeof(struct __main_block_impl_0)};

int main(int argc, const char *argv[]) {
  int intValue = 1;
  char *charValue = "hello";
  /* @autoreleasepool */ {
    __AtAutoreleasePool __autoreleasepool;
    void (*block)(void) =
        ((void (*)()) & __main_block_impl_0((void *)__main_block_func_0,
                                            &__main_block_desc_0_DATA, intValue,
                                            charValue));
    ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)(
        (__block_impl *)block);
  }
  return 0;
}
```
可以看出在本次示例代码中 block 块底层结构体 `__main_block_impl_0` 和上一节中发生了变化，将在 block 内部用到的外部变量放入了结构体的构造当中，