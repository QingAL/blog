#define EXPORT_SYMBOL __declspec(dllexport)

class EXPORT_SYMBOL MyDll {
 public:
  static void hello();
};