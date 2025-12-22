#ifndef PIPEX_DATA_HP
    #define PIPEX_DATA_HP

#include <type_traits>
#include "GenericData.h"

//Fixme: allow only base data types (T = int, float, double, char, bool, etc...)?
//Fixme: arithmetic operators only work on the same type T, should we allow mixed types (e.g., Data<int> + Data<float>)?
namespace PipeX {
    template <typename T>
    class Data final: public GenericData {
    public:
        using type = T;
        T value;

        Data() = default;
        explicit Data(const T value): value(value) {}
        Data(const Data&) = default;
        Data(Data&&) = default;

        std::unique_ptr<GenericData> clone() const override {
            return make_unique<Data<T>>(*this);
        }

        Data& operator=(const Data&) = default;
        Data& operator=(Data&&) = default;
        Data& operator=(const T& v) { value = v; return *this; }

        // conversions
        explicit operator T() const noexcept { return value; }
        explicit operator T&() & noexcept { return value; }
        explicit operator const T&() const & noexcept { return value; }

        // compound assignments (allow mixing with other Data<U> and raw U)
        template<typename U>
        Data& operator+=(const Data<U>& rhs) { value += rhs.value; return *this; }
        template<typename U>
        Data& operator+=(const U& rhs) { value += rhs; return *this; }

        template<typename U>
        Data& operator-=(const Data<U>& rhs) { value -= rhs.value; return *this; }
        template<typename U>
        Data& operator-=(const U& rhs) { value -= rhs; return *this; }

        template<typename U>
        Data& operator*=(const Data<U>& rhs) { value *= rhs.value; return *this; }
        template<typename U>
        Data& operator*=(const U& rhs) { value *= rhs; return *this; }

        template<typename U>
        Data& operator/=(const Data<U>& rhs) { value /= rhs.value; return *this; }
        template<typename U>
        Data& operator/=(const U& rhs) { value /= rhs; return *this; }

        template<typename U>
        Data& operator%=(const Data<U>& rhs) { value %= rhs.value; return *this; }
        template<typename U>
        Data& operator%=(const U& rhs) { value %= rhs; return *this; }

        template<typename U>
        Data& operator&=(const Data<U>& rhs) { value &= rhs.value; return *this; }
        template<typename U>
        Data& operator&=(const U& rhs) { value &= rhs; return *this; }

        template<typename U>
        Data& operator|=(const Data<U>& rhs) { value |= rhs.value; return *this; }
        template<typename U>
        Data& operator|=(const U& rhs) { value |= rhs; return *this; }

        template<typename U>
        Data& operator^=(const Data<U>& rhs) { value ^= rhs.value; return *this; }
        template<typename U>
        Data& operator^=(const U& rhs) { value ^= rhs; return *this; }

        template<typename U>
        Data& operator<<=(const Data<U>& rhs) { value <<= rhs.value; return *this; }
        template<typename U>
        Data& operator<<=(const U& rhs) { value <<= rhs; return *this; }

        template<typename U>
        Data& operator>>=(const Data<U>& rhs) { value >>= rhs.value; return *this; }
        template<typename U>
        Data& operator>>=(const U& rhs) { value >>= rhs; return *this; }

        // unary
        Data operator+() const { return Data(+value); }
        Data operator-() const { return Data(-value); }
        Data operator~() const { return Data(~value); }
        Data operator!() const { return Data(!value); }

        // increment / decrement
        Data& operator++() { ++value; return *this; }
        Data operator++(int) { Data tmp = *this; ++value; return tmp; }
        Data& operator--() { --value; return *this; }
        Data operator--(int) { Data tmp = *this; --value; return tmp; }

        // binary arithmetic / bitwise (support mixed Data<U> and raw U)
        // use decltype to deduce result element type
        template<typename U>
        friend auto operator+(const Data& a, const Data<U>& b) -> Data<decltype(a.value + b.value)> {
            using R = decltype(a.value + b.value);
            return Data<R>(a.value + b.value);
        }
        template<typename U>
        friend auto operator+(const Data& a, const U& b) -> Data<decltype(a.value + b)> {
            using R = decltype(a.value + b);
            return Data<R>(a.value + b);
        }
        template<typename U>
        friend auto operator+(const U& a, const Data& b) -> Data<decltype(a + b.value)> {
            using R = decltype(a + b.value);
            return Data<R>(a + b.value);
        }

        template<typename U>
        friend auto operator-(const Data& a, const Data<U>& b) -> Data<decltype(a.value - b.value)> {
            using R = decltype(a.value - b.value);
            return Data<R>(a.value - b.value);
        }
        template<typename U>
        friend auto operator-(const Data& a, const U& b) -> Data<decltype(a.value - b)> {
            using R = decltype(a.value - b);
            return Data<R>(a.value - b);
        }
        template<typename U>
        friend auto operator-(const U& a, const Data& b) -> Data<decltype(a - b.value)> {
            using R = decltype(a - b.value);
            return Data<R>(a - b.value);
        }

        template<typename U>
        friend auto operator*(const Data& a, const Data<U>& b) -> Data<decltype(a.value * b.value)> {
            using R = decltype(a.value * b.value);
            return Data<R>(a.value * b.value);
        }
        template<typename U>
        friend auto operator*(const Data& a, const U& b) -> Data<decltype(a.value * b)> {
            using R = decltype(a.value * b);
            return Data<R>(a.value * b);
        }
        template<typename U>
        friend auto operator*(const U& a, const Data& b) -> Data<decltype(a * b.value)> {
            using R = decltype(a * b.value);
            return Data<R>(a * b.value);
        }

        template<typename U>
        friend auto operator/(const Data& a, const Data<U>& b) -> Data<decltype(a.value / b.value)> {
            using R = decltype(a.value / b.value);
            return Data<R>(a.value / b.value);
        }
        template<typename U>
        friend auto operator/(const Data& a, const U& b) -> Data<decltype(a.value / b)> {
            using R = decltype(a.value / b);
            return Data<R>(a.value / b);
        }
        template<typename U>
        friend auto operator/(const U& a, const Data& b) -> Data<decltype(a / b.value)> {
            using R = decltype(a / b.value);
            return Data<R>(a / b.value);
        }

        template<typename U>
        friend auto operator%(const Data& a, const Data<U>& b) -> Data<decltype(a.value % b.value)> {
            using R = decltype(a.value % b.value);
            return Data<R>(a.value % b.value);
        }
        template<typename U>
        friend auto operator%(const Data& a, const U& b) -> Data<decltype(a.value % b)> {
            using R = decltype(a.value % b);
            return Data<R>(a.value % b);
        }
        template<typename U>
        friend auto operator%(const U& a, const Data& b) -> Data<decltype(a % b.value)> {
            using R = decltype(a % b.value);
            return Data<R>(a % b.value);
        }

        template<typename U>
        friend auto operator&(const Data& a, const Data<U>& b) -> Data<decltype(a.value & b.value)> {
            using R = decltype(a.value & b.value);
            return Data<R>(a.value & b.value);
        }
        template<typename U>
        friend auto operator&(const Data& a, const U& b) -> Data<decltype(a.value & b)> {
            using R = decltype(a.value & b);
            return Data<R>(a.value & b);
        }
        template<typename U>
        friend auto operator&(const U& a, const Data& b) -> Data<decltype(a & b.value)> {
            using R = decltype(a & b.value);
            return Data<R>(a & b.value);
        }

        template<typename U>
        friend auto operator|(const Data& a, const Data<U>& b) -> Data<decltype(a.value | b.value)> {
            using R = decltype(a.value | b.value);
            return Data<R>(a.value | b.value);
        }
        template<typename U>
        friend auto operator|(const Data& a, const U& b) -> Data<decltype(a.value | b)> {
            using R = decltype(a.value | b);
            return Data<R>(a.value | b);
        }
        template<typename U>
        friend auto operator|(const U& a, const Data& b) -> Data<decltype(a | b.value)> {
            using R = decltype(a | b.value);
            return Data<R>(a | b.value);
        }

        template<typename U>
        friend auto operator^(const Data& a, const Data<U>& b) -> Data<decltype(a.value ^ b.value)> {
            using R = decltype(a.value ^ b.value);
            return Data<R>(a.value ^ b.value);
        }
        template<typename U>
        friend auto operator^(const Data& a, const U& b) -> Data<decltype(a.value ^ b)> {
            using R = decltype(a.value ^ b);
            return Data<R>(a.value ^ b);
        }
        template<typename U>
        friend auto operator^(const U& a, const Data& b) -> Data<decltype(a ^ b.value)> {
            using R = decltype(a ^ b.value);
            return Data<R>(a ^ b.value);
        }

        template<typename U>
        friend auto operator<<(const Data& a, const Data<U>& b) -> Data<decltype(a.value << b.value)> {
            using R = decltype(a.value << b.value);
            return Data<R>(a.value << b.value);
        }
        template<typename U>
        friend auto operator<<(const Data& a, const U& b) -> Data<decltype(a.value << b)> {
            using R = decltype(a.value << b);
            return Data<R>(a.value << b);
        }
        template<typename U>
        friend auto operator<<(const U& a, const Data& b) -> Data<decltype(a << b.value)> {
            using R = decltype(a << b.value);
            return Data<R>(a << b.value);
        }

        template<typename U>
        friend auto operator>>(const Data& a, const Data<U>& b) -> Data<decltype(a.value >> b.value)> {
            using R = decltype(a.value >> b.value);
            return Data<R>(a.value >> b.value);
        }
        template<typename U>
        friend auto operator>>(const Data& a, const U& b) -> Data<decltype(a.value >> b)> {
            using R = decltype(a.value >> b);
            return Data<R>(a.value >> b);
        }
        template<typename U>
        friend auto operator>>(const U& a, const Data& b) -> Data<decltype(a >> b.value)> {
            using R = decltype(a >> b.value);
            return Data<R>(a >> b.value);
        }

        // comparisons (mixed types)
        template<typename U>
        friend bool operator==(const Data& a, const Data<U>& b) { return a.value == b.value; }
        template<typename U>
        friend bool operator==(const Data& a, const U& b) { return a.value == b; }
        template<typename U>
        friend bool operator==(const U& a, const Data& b) { return a == b.value; }

        template<typename U>
        friend bool operator!=(const Data& a, const Data<U>& b) { return a.value != b.value; }
        template<typename U>
        friend bool operator!=(const Data& a, const U& b) { return a.value != b; }
        template<typename U>
        friend bool operator!=(const U& a, const Data& b) { return a != b.value; }

        template<typename U>
        friend bool operator<(const Data& a, const Data<U>& b) { return a.value < b.value; }
        template<typename U>
        friend bool operator<(const Data& a, const U& b) { return a.value < b; }
        template<typename U>
        friend bool operator<(const U& a, const Data& b) { return a < b.value; }

        template<typename U>
        friend bool operator<=(const Data& a, const Data<U>& b) { return a.value <= b.value; }
        template<typename U>
        friend bool operator<=(const Data& a, const U& b) { return a.value <= b; }
        template<typename U>
        friend bool operator<=(const U& a, const Data& b) { return a <= b.value; }

        template<typename U>
        friend bool operator>(const Data& a, const Data<U>& b) { return a.value > b.value; }
        template<typename U>
        friend bool operator>(const Data& a, const U& b) { return a.value > b; }
        template<typename U>
        friend bool operator>(const U& a, const Data& b) { return a > b.value; }

        template<typename U>
        friend bool operator>=(const Data& a, const Data<U>& b) { return a.value >= b.value; }
        template<typename U>
        friend bool operator>=(const Data& a, const U& b) { return a.value >= b; }
        template<typename U>
        friend bool operator>=(const U& a, const Data& b) { return a >= b.value; }
        // ===========================================================================

        friend std::ostream& operator<<(std::ostream& os, const Data& obj) {
            return os << obj.value;
        }
    };
}

#endif // PIPEX_DATA_HP