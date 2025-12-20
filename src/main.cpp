#include <iostream>
#include <numeric>
#include <PipeX/nodes/Filter.h>
#include <PipeX/nodes/Transformer.h>

//#include "debug/print_debug.h"
#include "PipeX/nodes/Aggregator.h"

template <typename T>
void printVector(const std::vector<T>& vec);

int main() {

    PipeX::Node<int, int> *node = new PipeX::Filter<int>("Odd filter", [](const int &value) {
        return value % 2 != 0; // Keep only odd numbers
    });

    PipeX::Filter<int> filterNode("Even filter", [](const int& value) {
        return value % 2 == 0; // Keep only even numbers
    });
    PipeX::Transformer<int, int> transformer("*3 +1 transformer", [](const int &value) {
        return value * 3 + 1; // Example transformation: triple the value and add one
    });
    PipeX::Aggregator<int, float> aggregator("Average", [](const std::vector<int>& input) {
        return std::accumulate(input.begin(), input.end(), 0.0f) / static_cast<float>(input.size());
    });



    std::vector<int> input;
    for (int i = 1; i <= 10; ++i) {
        input.push_back(i);
    }
    const std::vector<int> evenOut = filterNode(input);
    const std::vector<int> oddOut = (*node)(input);
    const std::vector<int> transformedOut = transformer(input);
    const std::vector<float> aggregatedOut = aggregator(input);

    std::cout << std::endl;
    std::cout<< "Input vector: ";
    printVector(input);

    std::cout << "Filtered output (even numbers): ";
    printVector(evenOut);

    std::cout << "Filtered output (odd numbers): ";
    printVector(oddOut);

    std::cout << "Transformed output (x * 3 + 1): ";
    printVector(transformedOut);

    std::cout << "Aggregated output (average): ";
    printVector(aggregatedOut);


    delete node;

    return 0;
}

template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "[";
    for (const auto& value : vec) {
        std::cout << value;
        if (&value != &vec.back()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

/* ========================================================
 * Searching REGEXs
 * Select all lines starting with '[DEBUG_INFO]' but '[DEBUG_INFO] [PipeX::Node]': ^\[DEBUG_INFO\](?! \[PipeX::Node\]).*$
 * ======================================================== */