#ifndef PIPEX_DATA_HP
#define PIPEX_DATA_HP

#include <utility>
#include <ostream>
#include "IData.h"

namespace PipeX {
    /**
     * @brief Concrete data wrapper for a specific type T.
     *
     * Data<T> wraps a value of type T and inherits from IData, allowing it to be
     * passed through the pipeline in a type-erased manner.
     *
     * @tparam T The type of the data being wrapped.
     */
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