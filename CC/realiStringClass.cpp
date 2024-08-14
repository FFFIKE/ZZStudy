#include <iostream>
#include <cstring>

class String {
public:
    // 默认构造函数
    String() : data_(nullptr), size_(0) {
        data_ = new char[1];
        data_[0] = '\0';
    }

    // 构造函数
    String(const char* str) {
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::strcpy(data_, str);
    }

    // 拷贝构造函数
    String(const String& other) {
        size_ = other.size_;
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    // 移动构造函数
    String(String&& other) noexcept : data_(nullptr), size_(0) {
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 赋值运算符
    String& operator=(const String& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        size_ = other.size_;
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
        return *this;
    }

    // 移动赋值运算符
    String& operator=(String&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
        return *this;
    }

    // 返回字符串的长度
    std::size_t size() const {
        return size_;
    }

    // 获取 C 风格字符串
    const char* c_str() const {
        return data_;
    }

    // 析构函数
    ~String() {
        delete[] data_;
    }

    // 重载 [] 操作符，返回字符的引用
    char& operator[](std::size_t index) {
        return data_[index];
    }

    // 重载 [] 操作符，返回常量字符的引用
    const char& operator[](std::size_t index) const {
        return data_[index];
    }

private:
    char* data_;       // 存储字符串的字符数组
    std::size_t size_; // 字符串的长度
};

// 重载输出流操作符
std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

int main() {
    String s1("Hello");
    String s2 = s1; // 拷贝构造
    String s3;
    s3 = s1;        // 赋值运算符
    String s4 = std::move(s1); // 移动构造

    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    std::cout << "s4: " << s4 << std::endl;

    return 0;
}
