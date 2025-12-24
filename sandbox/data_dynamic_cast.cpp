//
// Created by Matteo Ranzi on 20/12/25.
//

#include <iostream>

#include "extended_cpp_standard/memory.h"


class IData {
public:
    virtual ~IData() = default;
};

template <typename T>
class Data: public IData {
public:
    T value;
    explicit Data(const T value) : value(value) {}
};

class Integer final : public IData {
public:
    int value = 0;
    explicit Integer(const int value) : value(value) {}

};

class Float final : public IData {
public:
    float value = 0.0f;
    explicit Float(const float value) : value(value) {}
};

int bar(const int k) {
    return k * 2;
}

template <typename InputT, typename OutputT>
OutputT foo(const IData* input) {
    const auto castedInput = dynamic_cast<const Data<InputT>*>(input);
    if (!castedInput) {
        throw std::bad_cast();
    }
    return OutputT(bar(castedInput->value));
}

int main() {

    std::vector<std::unique_ptr<IData>> dataset;
    // dataset.push_back(make_unique<Integer>(10));
    // dataset.push_back(make_unique<Float>(3.14f));
    // dataset.push_back(make_unique<Integer>(5));
    // dataset.push_back(make_unique<Float>(7.2f));

    dataset.push_back(make_unique<Data<int>>(10));
    dataset.push_back(make_unique<Data<float>>(3.14f));
    dataset.push_back(make_unique<Data<int>>(20));
    dataset.push_back(make_unique<Data<float>>(2.71f));

    const IData* element = dataset[0].get();
    auto result = foo<int, int>(element);

    std::cout << "Result: " << result << std::endl;

    return 0;
}