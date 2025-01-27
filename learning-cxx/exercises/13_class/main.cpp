#include "../exercise.h"

// C++ 中，`class` 和 `struct` 之间的**唯一区别**是
// `class` 默认访问控制符是 `private`，
// `struct` 默认访问控制符是 `public`。
// READ: 访问说明符 <https://zh.cppreference.com/w/cpp/language/access>

// 这个 class 中的字段被 private 修饰，只能在 class 内部访问。
// 因此必须提供构造器来初始化字段。
// READ: 构造器 <https://zh.cppreference.com/w/cpp/language/constructor>
class Fibonacci {
    size_t cache[16];  // 缓存数组，最多存储16个斐波那契数
    int cached;        // 当前缓存的最大索引

public:
    // 构造器
    Fibonacci() : cached(1) {
        cache[0] = 0;  // 初始化 Fibonacci(0)
        cache[1] = 1;  // 初始化 Fibonacci(1)
        for (int i = 2; i < 16; ++i) {
            cache[i] = 0;  // 初始化其余值为 0 表示未计算
        }
    }
    // 缓存优化的斐波那契计算
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
    // 现在类型拥有无参构造器，声明时会直接调用。
    // 这个写法不再是未定义行为了。
    Fibonacci fib;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
