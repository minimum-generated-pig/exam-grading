#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache; // 动态数组，用于存储斐波那契数列
    int cached;    // 已缓存的最大索引 + 1

public:
    // 动态设置容量的构造器
    DynFibonacci(int capacity) : cache(new size_t[capacity]()), cached(2) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // 移动构造器
    DynFibonacci(DynFibonacci &&other) noexcept : cache(other.cache), cached(other.cached) {
        other.cache = nullptr;
        other.cached = 0;
    }

    // 移动赋值
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        if (this != &other) { // 避免移动到自身
            delete[] cache;
            cache = other.cache;
            cached = other.cached;
            other.cache = nullptr;
            other.cached = 0;
        }
        return *this;
    }

    // 析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // 缓存优化的斐波那契计算
    size_t operator[](int i) {
        for (int j = cached; j <= i; ++j) {
            cache[j] = cache[j - 1] + cache[j - 2];
        }
        cached = std::max(cached, i + 1); // 更新缓存的最大索引
        return cache[i];
    }

    // 不修改的 const 版本
    size_t operator[](int i) const {
        ASSERT(i < cached, "i out of range");
        return cache[i];
    }

    // 判断对象是否有效
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0); // 自身移动
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
