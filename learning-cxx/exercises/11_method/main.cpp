#include "../exercise.h"

struct Fibonacci {
    unsigned long long cache[128]={0};
    int cached=2;

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
       if (i < cached) {  // 如果缓存中已有此值，直接返回
            return cache[i];
        }

        // 否则，计算并更新缓存
        for (int j = cached; j <= i; ++j) {
            cache[j] = cache[j - 1] + cache[j - 2];
        }
        cached = i + 1;  // 更新已缓存的最大索引
        return cache[i];
    }
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib;
     fib.cache[0] = 0;  // 确保cache[0]是0
    fib.cache[1] = 1;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
