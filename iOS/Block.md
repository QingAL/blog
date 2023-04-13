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

上述 oc 代码中，在 block 中 使用 NSLog 打印一句日志，接下来将 oc 代码转换为 c 代码，看看 block 块到底做了什么

```objc
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

    static int staticIntValue = 1;
    
    char * charValue = "hello";
    
    @autoreleasepool {
        void (^block)(void) =  ^{
          NSLog(@"int value: %d, char value: %s, static int value: %d", intValue, charValue, staticIntValue);
        };
        block();
    }
    return 0;
}
```

这次我们在 block 块外，分别声明了一个 int 变量和一个 char * 变量，并且在 block 内使用。

```objc
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
  int *staticIntValue;
  __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, int _intValue,
                      char *_charValue, int *_staticIntValue, int flags = 0)
      : intValue(_intValue),
        charValue(_charValue),
        staticIntValue(_staticIntValue) {
    impl.isa = &_NSConcreteStackBlock;
    impl.Flags = flags;
    impl.FuncPtr = fp;
    Desc = desc;
  }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
  int intValue = __cself->intValue;               // bound by copy
  char *charValue = __cself->charValue;           // bound by copy
  int *staticIntValue = __cself->staticIntValue;  // bound by copy

  NSLog(
      (NSString
           *)&__NSConstantStringImpl__var_folders_w5_8h00xnpd7jv_ht4xm6r1n7_80000gn_T_main_a2c26b_mii_0,
      intValue, charValue, (*staticIntValue));
}

static struct __main_block_desc_0 {
  size_t reserved;
  size_t Block_size;
} __main_block_desc_0_DATA = {0, sizeof(struct __main_block_impl_0)};

int main(int argc, const char *argv[]) {
  int intValue = 1;
  char *charValue = "hello";
  static int staticIntValue = 1;
  /* @autoreleasepool */ {
    __AtAutoreleasePool __autoreleasepool;
    void (*block)() =
        ((void (*)()) & __main_block_impl_0((void *)__main_block_func_0,
                                            &__main_block_desc_0_DATA, intValue,
                                            charValue, &staticIntValue));
    ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)(
        (__block_impl *)block);
  }
  return 0;
}
```

在本次示例代码中 block 块底层结构体 `__main_block_impl_0` 和上一节中发生了变化，在 block 内部用到的外部变量放入了结构体的构造当中，将 intValue 和 charValue 传入结构体内部，可以看出 block 内的 intValue 和 外部的 intValue 是不同地址，而 charValue 则是相同地址，若为 static 变量，则在构造的时传入变量的地址。

## __block 做了什么？

如果想在 block 块内修改外部变量，需要加上 __block 修饰，接下来看下 __block 修饰变量做了什么。

```objc
#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    
    __block int intValue = 1;
    __block char * charValue = "hello";
    @autoreleasepool {
        void (^block)() = ^{
            NSLog(@"int value: %d, char value: %s", intValue, charValue);
         };
        block();
    }
    return 0;
}
```

```objc
struct __Block_byref_intValue_0 {
  void *__isa;
  __Block_byref_intValue_0 *__forwarding;
  int __flags;
  int __size;
  int intValue;
};

struct __Block_byref_charValue_1 {
  void *__isa;
  __Block_byref_charValue_1 *__forwarding;
  int __flags;
  int __size;
  char *charValue;
};

struct __main_block_impl_0 {
  struct __block_impl impl;
  struct __main_block_desc_0 *Desc;
  __Block_byref_intValue_0 *intValue;    // by ref
  __Block_byref_charValue_1 *charValue;  // by ref
  __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc,
                      __Block_byref_intValue_0 *_intValue,
                      __Block_byref_charValue_1 *_charValue, int flags = 0)
      : intValue(_intValue->__forwarding), charValue(_charValue->__forwarding) {
    impl.isa = &_NSConcreteStackBlock;
    impl.Flags = flags;
    impl.FuncPtr = fp;
    Desc = desc;
  }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
  __Block_byref_intValue_0 *intValue = __cself->intValue;     // bound by ref
  __Block_byref_charValue_1 *charValue = __cself->charValue;  // bound by ref

  NSLog(
      (NSString
           *)&__NSConstantStringImpl__var_folders_w5_8h00xnpd7jv_ht4xm6r1n7_80000gn_T_main_83dab1_mii_0,
      (intValue->__forwarding->intValue), (charValue->__forwarding->charValue));
}

static void __main_block_copy_0(struct __main_block_impl_0 *dst,
                                struct __main_block_impl_0 *src) {
  _Block_object_assign((void *)&dst->intValue, (void *)src->intValue,
                       8 /*BLOCK_FIELD_IS_BYREF*/);
  _Block_object_assign((void *)&dst->charValue, (void *)src->charValue,
                       8 /*BLOCK_FIELD_IS_BYREF*/);
}

static void __main_block_dispose_0(struct __main_block_impl_0 *src) {
  _Block_object_dispose((void *)src->intValue, 8 /*BLOCK_FIELD_IS_BYREF*/);
  _Block_object_dispose((void *)src->charValue, 8 /*BLOCK_FIELD_IS_BYREF*/);
}

static struct __main_block_desc_0 {
  size_t reserved;
  size_t Block_size;
  void (*copy)(struct __main_block_impl_0 *, struct __main_block_impl_0 *);
  void (*dispose)(struct __main_block_impl_0 *);
} __main_block_desc_0_DATA = {0, sizeof(struct __main_block_impl_0),
                              __main_block_copy_0, __main_block_dispose_0};

int main(int argc, const char *argv[]) {
  __attribute__((__blocks__(byref))) __Block_byref_intValue_0 intValue = {
      (void *)0, (__Block_byref_intValue_0 *)&intValue, 0,
      sizeof(__Block_byref_intValue_0), 1};
  __attribute__((__blocks__(byref))) __Block_byref_charValue_1 charValue = {
      (void *)0, (__Block_byref_charValue_1 *)&charValue, 0,
      sizeof(__Block_byref_charValue_1), "hello"};
  /* @autoreleasepool */ {
    __AtAutoreleasePool __autoreleasepool;
    void (*block)() =
        ((void (*)()) &
         __main_block_impl_0(
             (void *)__main_block_func_0, &__main_block_desc_0_DATA,
             (__Block_byref_intValue_0 *)&intValue,
             (__Block_byref_charValue_1 *)&charValue, 570425344));
    ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)(
        (__block_impl *)block);
  }
  return 0;
}
```

变量使用 `__block` 修饰后，block 底层结构 `__main_block_impl_0` 中捕获的变量也由上一小节中的基础数据类型 `int` 和 `char *` 变为了结构体 `__Block_byref_intValue_0` 和 `__Block_byref_charValue_1`，在 main 函数构造中可以看出，`__forwarding` 为对应结构体的指针，`__flag` 为 0，`__size`
 为结构体的大小，最后一个成员变量为存储具体变量的地方。

在 block 块构造时，可以看出传入的参数变为了结构体的地址，在 block 构造中，将 main 函数中声明的 `intValue` 和 `charValue` 对应的结构体地址传入 `__main_block_impl_0` 中进行构造，将地址赋值给 `__forwarding`，在后续调用 block 块 `__main_block_func_0` 时，通过 `__forwarding` 指针指向对应的 `intValue` 和 `charValue` 由于 `__forwarding` 指向 `main` 函数中对应的 `__block` 修饰的结构体地址，便可以拿到对应的值并且修改。