#include "../exercise.h"

// READ: 析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>
// READ: RAII <https://learn.microsoft.com/zh-cn/cpp/cpp/object-lifetime-and-resource-management-modern-cpp?view=msvc-170>

/// @brief 任意缓存容量的斐波那契类型。
/// @details 可以在构造时传入缓存容量，因此需要动态分配缓存空间。
class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]), cached(1) {
        cache[0] = 0;  // 初始化 Fibonacci(0)
        cache[1] = 1;  // 初始化 Fibonacci(1)
        for (int i = 2; i < capacity; ++i) {
            cache[i] = 0;  // 未初始化部分置为 0
        }
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci()
    {
         delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        // 如果缓存未命中，则计算并存储
        for (int j = 2; j <= i; ++j) {
            if (cache[j] == 0) {  // 仅当值未被缓存时计算
                cache[j] = cache[j - 1] + cache[j - 2];
            }
        }

        return cache[i];  // 返回缓存值
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
