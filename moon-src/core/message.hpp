#pragma once
#include "common/buffer.hpp"
#include "config.hpp"

namespace moon {
class message final {
public:
    static message with_empty() {
        // 初始化data_成员
        return message { buffer_ptr_t {} };
    }

    // 冒号后面的是成员初始化列表
    message(): data_(buffer::make_unique()) {}

    // explicit关键字的作用是防止构造函数的隐式转换,必须要用显式转换方式比如static_cast等
    explicit message(size_t capacity): data_(buffer::make_unique(capacity)) {}

    // 两个&&表示右值引用
    explicit message(buffer_ptr_t&& v): data_(std::move(v)) {}

    void set_sender(uint32_t serviceid) {
        sender_ = serviceid;
    }

    uint32_t sender() const {
        return sender_;
    }

    void set_receiver(uint32_t serviceid) {
        receiver_ = serviceid;
    }

    uint32_t receiver() const {
        return receiver_;
    }

    void set_sessionid(int64_t v) {
        sessionid_ = v;
    }

    int64_t sessionid() const {
        return sessionid_;
    }

    void set_type(uint8_t v) {
        type_ = v;
    }

    uint8_t type() const {
        return type_;
    }

    void write_data(std::string_view s) {
        // 这是校验data_是否为空?
        assert(data_);
        data_->write_back(s.data(), s.size());
    }

    const char* data() const {
        return data_ ? data_->data() : nullptr;
    }

    size_t size() const {
        return data_ ? data_->size() : 0;
    }

    buffer_ptr_t into_buffer() {
        return std::move(data_);
    }

    buffer* as_buffer() {
        return data_ ? data_.get() : nullptr;
    }

private:
    uint8_t type_ = 0;
    uint32_t sender_ = 0;
    uint32_t receiver_ = 0;
    int64_t sessionid_ = 0;
    // 对于std::unique_ptr类型的成员变量，需要显式初始化指向一个有效的buffer或者nullptr，否则可能指向无效的内存地址，导致未定义行为，比如违规访问、程序崩溃等。
    std::unique_ptr<buffer> data_;
};
}; // namespace moon
