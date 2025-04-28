#include <windows.h>
#include <stdio.h>


#define RET_INSTRUCTION 0xC3
#define JMP_INSTRUCTION 0xE9


#define LOG_FMT() printf("----------- line: %d -----------\n", __LINE__);

void new_function() {
  printf("This line should print\n");
}


void target_function() {
    printf("This line should not print\n");
}


void hook_function_install(void* target_func, void* new_func) {
  DWORD oldProtect;

  if (VirtualProtect(target_func, 16, PAGE_EXECUTE_READWRITE, &oldProtect)) {
      unsigned char patch[5];

      patch[0] = JMP_INSTRUCTION;
      // The next 4 bytes are the address to jump to, which is the new function
      *(DWORD*)(patch + 1) = (DWORD)new_func - (DWORD)target_func - 5;

      memcpy(target_func, patch, sizeof(patch));

      VirtualProtect(target_func, 16, oldProtect, &oldProtect);
  } else {
      printf("VirtualProtect failed: %d\n", GetLastError());
  }
}

int main() {
    LOG_FMT();

    void* func_addr = (void*)target_function;

    DWORD oldProtect;
    // ret instruction, target_function directly returns to the caller
    if (VirtualProtect(func_addr, 16, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        unsigned char patch[] = { RET_INSTRUCTION };
        memcpy(func_addr, patch, sizeof(patch));
        VirtualProtect(func_addr, 16, oldProtect, &oldProtect);
    } else {
        printf("VirtualProtect failed: %d\n", GetLastError());
    }

    LOG_FMT();
    target_function();


    hook_function_install((void*)target_function, (void*)new_function);
    target_function();

    LOG_FMT();

    return 0;
}
