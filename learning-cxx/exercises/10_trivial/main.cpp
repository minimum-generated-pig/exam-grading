#include "../exercise.h"

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16]={0};
    int cached=0;
};

// TODO: 实现正确的缓存优化斐波那契计算
static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    if (i <= 1) {
        return i;  // 斐波那契数列的基础情况，0 或 1
    }
    
    // 只有当缓存中没有计算过斐波那契数值时，才进行计算
    for (int j = 2; j <= i; ++j) {
        if (cache.cache[j] == 0) {  // 判断是否已缓存
            cache.cache[j] = cache.cache[j - 1] + cache.cache[j - 2];  // 使用前两个值来计算
        }
    }
    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    FibonacciCache fib;
      fib.cache[0] = 0;  // 确保cache[0]是0
    fib.cache[1] = 1;
    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
