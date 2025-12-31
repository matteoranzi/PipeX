#ifndef PIPEX_DATA_HP
#define PIPEX_DATA_HP

#include "IData.h"

namespace PipeX {
    template <typename T>
    class Data final: public IData {
    public:
        using type = T;
        T value;

        Data() = default;
        Data(const Data&) = default;
        Data(Data&&) = default;
        explicit Data(T&& val) noexcept: value(std::forward<T>(val)) {}

        ~Data() override = default;

        friend std::ostream& operator<<(std::ostream& os, const Data& obj) {
            return os << obj.value;
        }
    };
}

#endif // PIPEX_DATA_HP