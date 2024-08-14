#include <iostream>
#include <cstring>

class String {
public:
    // Ĭ�Ϲ��캯��
    String() : data_(nullptr), size_(0) {
        data_ = new char[1];
        data_[0] = '\0';
    }

    // ���캯��
    String(const char* str) {
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::strcpy(data_, str);
    }

    // �������캯��
    String(const String& other) {
        size_ = other.size_;
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    // �ƶ����캯��
    String(String&& other) noexcept : data_(nullptr), size_(0) {
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // ��ֵ�����
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

    // �ƶ���ֵ�����
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

    // �����ַ����ĳ���
    std::size_t size() const {
        return size_;
    }

    // ��ȡ C ����ַ���
    const char* c_str() const {
        return data_;
    }

    // ��������
    ~String() {
        delete[] data_;
    }

    // ���� [] �������������ַ�������
    char& operator[](std::size_t index) {
        return data_[index];
    }

    // ���� [] �����������س����ַ�������
    const char& operator[](std::size_t index) const {
        return data_[index];
    }

private:
    char* data_;       // �洢�ַ������ַ�����
    std::size_t size_; // �ַ����ĳ���
};

// ���������������
std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

int main() {
    String s1("Hello");
    String s2 = s1; // ��������
    String s3;
    s3 = s1;        // ��ֵ�����
    String s4 = std::move(s1); // �ƶ�����

    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    std::cout << "s4: " << s4 << std::endl;

    return 0;
}
