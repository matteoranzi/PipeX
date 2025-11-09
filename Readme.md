> # THIS README IS A WORK IN PROGRESS AND WILL BE UPDATED SOON.
> # PLEASE CHECK BACK LATER FOR THE COMPLETE DOCUMENTATION.
> The following is a preliminary version of the README for the PipeX project and may lack some details.

# PipeX

> **A lightweight C++11 header-only data processing pipeline framework.**

---

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/build-CMake-green.svg)](https://cmake.org/)
[![Tests](https://img.shields.io/badge/tests-GoogleTest-lightgrey.svg)](https://github.com/google/googletest)
[![Status](https://img.shields.io/badge/status-active-success.svg)]()

---

## 📘 Overview

**PipeX** is a **header-only C++11 framework** for building modular, composable, and efficient **data processing pipelines**.  
Each pipeline consists of simple processing nodes that transform or filter data streams in a linear fashion.  
Future versions will introduce **parallel execution** and optimized node scheduling.

The framework aims to:
- Provide a clean, modern C++ interface for creating pipelines.
- Encourage modularity and reusability via template-based nodes.
- Offer optional debug tools to trace data flow between nodes.

---

## 🧩 Features

- **Header-only** — no linking required.
- **C++11 compatible**.
- **Debug print system** (configurable via compile-time macros).
- **Minimal dependencies** — only [GoogleTest](https://github.com/google/googletest) (auto-fetched by CMake for unit tests).
- **Extendable architecture** — custom node types can be easily implemented.
- **Future-ready** — designed for eventual parallelization.

---

## ⚙️ Building

### Requirements
- **C++11** or newer compiler (GCC, Clang, or MSVC).
- **CMake 3.15+**

### Build instructions

```bash
git clone https://github.com/yourusername/PipeX.git
cd PipeX
cmake -S . -B build
cmake --build build
```

To run unit tests:
```bash
cd build
ctest
```

---

## 🧠 Usage Example

`main.cpp` contains a minimal example of how to use PipeX.  
For instance:

```cpp
#include <PipeX/Pipeline.h>
#include <PipeX/nodes/Filter.h>

int main() {
    PipeX::Pipeline<int> pipeline;
    pipeline.addNode(std::make_shared<PipeX::Filter<int>>([](int x){ return x > 0; }));
    pipeline.run({ -1, 0, 1, 2, 3 });
}
```

Future examples (including parallel nodes and transformers) will be provided in the `examples/` folder.

---

## 🧩 Debugging Options

PipeX includes a lightweight, compile-time configurable debug print system.

### Enable debug output
```bash
cmake -DPIPEX_PRINT_DEBUG_ENABLED=ON -DPRINT_DEBUG_LEVEL=INFO ..
```

### Available debug levels
| Level | Description |
|:------|:-------------|
| `INFO`  | General information about pipeline execution. |
| `WARN`  | Warning messages about potential issues. |
| `ERROR` | Only error messages are printed. |
| `NONE`  | Disable all debug output. |

---

## 🧪 Testing

PipeX uses **GoogleTest**, which is automatically downloaded during configuration.

Run all unit tests:
```bash
ctest --output-on-failure
```

All tests are located in the `tests/` directory.

---

## 🚀 Roadmap

- [x] Initial header-only pipeline framework
- [x] Debug utilities
- [x] Unit tests integration
- [ ] Parallel pipeline execution
- [ ] Extended examples and benchmarks
- [ ] Optional JSON/CSV IO adapters

---

## 👥 Author

**Matteo Ranzi**  
📧 [GitHub Profile](https://github.com/yourusername)

---

## ⚖️ License

*Free for use in personal and non-commercial projects.*  
(Official license **TBD** — e.g., MIT, BSD, or custom permissive license.)

---

## 💡 Contributing

Contributions, bug reports, and suggestions are welcome!  
To contribute:
1. Fork the repository.
2. Create a new feature branch.
3. Submit a pull request with a clear description.

---

## 🧱 Acknowledgements

- [GoogleTest](https://github.com/google/googletest) for testing infrastructure.
- Inspired by modular dataflow architectures and functional pipeline designs in modern C++.

---

> “Build complex systems out of simple, composable parts.” — *PipeX Philosophy*

---

# NOTES FOR THE DEVELOPER
- In class inheritance, if no visibility mode is specified, then by default the visibility mode is considered as private