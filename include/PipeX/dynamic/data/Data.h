#ifndef PIPEX_DATA_HP
    #define PIPEX_DATA_HP

    #include "Data_.h"

    template <typename T>
    class Data final: public Data_ {
    public:
        T value;

        Data() = default;
        explicit Data(const T value): value(value) {}
        Data(const Data&) = default;
        Data(Data&&) = default;
        Data& operator=(const Data&) = default;
        Data& operator=(Data&&) = default;
        Data& operator=(const T& v) { value = v; return *this; }

        // conversions
        explicit operator T() const noexcept { return value; }
        explicit operator T&() & noexcept { return value; }
        explicit operator const T&() const & noexcept { return value; }

        // compound assignments
        Data& operator+=(const T& rhs) { value += rhs; return *this; }
        Data& operator-=(const T& rhs) { value -= rhs; return *this; }
        Data& operator*=(const T& rhs) { value *= rhs; return *this; }
        Data& operator/=(const T& rhs) { value /= rhs; return *this; }
        Data& operator%=(const T& rhs) { value %= rhs; return *this; }
        Data& operator&=(const T& rhs) { value &= rhs; return *this; }
        Data& operator|=(const T& rhs) { value |= rhs; return *this; }
        Data& operator^=(const T& rhs) { value ^= rhs; return *this; }
        Data& operator<<=(const T& rhs) { value <<= rhs; return *this; }
        Data& operator>>=(const T& rhs) { value >>= rhs; return *this; }

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

        // binary arithmetic / bitwise (Data op Data, Data op T, T op Data)
        friend Data operator+(const Data& a, const Data& b) { return Data(a.value + b.value); }
        friend Data operator+(const Data& a, const T& b) { return Data(a.value + b); }
        friend Data operator+(const T& a, const Data& b) { return Data(a + b.value); }

        friend Data operator-(const Data& a, const Data& b) { return Data(a.value - b.value); }
        friend Data operator-(const Data& a, const T& b) { return Data(a.value - b); }
        friend Data operator-(const T& a, const Data& b) { return Data(a - b.value); }

        friend Data operator*(const Data& a, const Data& b) { return Data(a.value * b.value); }
        friend Data operator*(const Data& a, const T& b) { return Data(a.value * b); }
        friend Data operator*(const T& a, const Data& b) { return Data(a * b.value); }

        friend Data operator/(const Data& a, const Data& b) { return Data(a.value / b.value); }
        friend Data operator/(const Data& a, const T& b) { return Data(a.value / b); }
        friend Data operator/(const T& a, const Data& b) { return Data(a / b.value); }

        friend Data operator%(const Data& a, const Data& b) { return Data(a.value % b.value); }
        friend Data operator%(const Data& a, const T& b) { return Data(a.value % b); }
        friend Data operator%(const T& a, const Data& b) { return Data(a % b.value); }

        friend Data operator&(const Data& a, const Data& b) { return Data(a.value & b.value); }
        friend Data operator&(const Data& a, const T& b) { return Data(a.value & b); }
        friend Data operator&(const T& a, const Data& b) { return Data(a & b.value); }

        friend Data operator|(const Data& a, const Data& b) { return Data(a.value | b.value); }
        friend Data operator|(const Data& a, const T& b) { return Data(a.value | b); }
        friend Data operator|(const T& a, const Data& b) { return Data(a | b.value); }

        friend Data operator^(const Data& a, const Data& b) { return Data(a.value ^ b.value); }
        friend Data operator^(const Data& a, const T& b) { return Data(a.value ^ b); }
        friend Data operator^(const T& a, const Data& b) { return Data(a ^ b.value); }

        friend Data operator<<(const Data& a, const Data& b) { return Data(a.value << b.value); }
        friend Data operator<<(const Data& a, const T& b) { return Data(a.value << b); }
        friend Data operator<<(const T& a, const Data& b) { return Data(a << b.value); }

        friend Data operator>>(const Data& a, const Data& b) { return Data(a.value >> b.value); }
        friend Data operator>>(const Data& a, const T& b) { return Data(a.value >> b); }
        friend Data operator>>(const T& a, const Data& b) { return Data(a >> b.value); }

        // comparisons
        friend bool operator==(const Data& a, const Data& b) { return a.value == b.value; }
        friend bool operator==(const Data& a, const T& b) { return a.value == b; }
        friend bool operator==(const T& a, const Data& b) { return a == b.value; }

        friend bool operator!=(const Data& a, const Data& b) { return a.value != b.value; }
        friend bool operator!=(const Data& a, const T& b) { return a.value != b; }
        friend bool operator!=(const T& a, const Data& b) { return a != b.value; }

        friend bool operator<(const Data& a, const Data& b) { return a.value < b.value; }
        friend bool operator<(const Data& a, const T& b) { return a.value < b; }
        friend bool operator<(const T& a, const Data& b) { return a < b.value; }

        friend bool operator<=(const Data& a, const Data& b) { return a.value <= b.value; }
        friend bool operator<=(const Data& a, const T& b) { return a.value <= b; }
        friend bool operator<=(const T& a, const Data& b) { return a <= b.value; }

        friend bool operator>(const Data& a, const Data& b) { return a.value > b.value; }
        friend bool operator>(const Data& a, const T& b) { return a.value > b; }
        friend bool operator>(const T& a, const Data& b) { return a > b.value; }

        friend bool operator>=(const Data& a, const Data& b) { return a.value >= b.value; }
        friend bool operator>=(const Data& a, const T& b) { return a.value >= b; }
        friend bool operator>=(const T& a, const Data& b) { return a >= b.value; }
    };

#endif // PIPEX_DATA_HP