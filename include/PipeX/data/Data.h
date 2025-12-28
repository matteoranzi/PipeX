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
        explicit Data(const T value): value(value) {}
        Data(const Data&) = default;
        Data(Data&&) = default;
        ~Data() override = default;

        std::unique_ptr<IData> clone() const override {
            return make_unique<Data<T>>(*this);
        }

        friend std::ostream& operator<<(std::ostream& os, const Data& obj) {
            return os << obj.value;
        }
    };
}

#endif // PIPEX_DATA_HP