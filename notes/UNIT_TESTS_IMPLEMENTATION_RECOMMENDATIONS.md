# PipeX - Comprehensive Unit Tests Checklist

## Overview
This document outlines comprehensive unit tests to validate the PipeX Pipelined Data Processing Network based on the December 28, 2025 refactoring and enhancements.

---

## Test Categories Summary

| Category                  | Priority  | Tests Count | Status     |
|---------------------------|-----------|-------------|------------|
| **Exception Handling**    | 🔴 High   | 10          | Needed     |
| **Pipeline Construction** | 🟡 Medium | 11          | 4 Existing |
| **Pipeline Integrity**    | 🔴 High   | 7           | Needed     |
| **Node Removal**          | 🔴 High   | 5           | Needed     |
| **Node Interface Tests**  | 🔴 High   | 3           | Needed     |
| **Individual Nodes**      | 🟡 Medium | 12          | 4 Existing |
| **Pipeline Execution**    | 🔴 High   | 8           | 1 Existing |
| **PipeXEngine**           | 🟡 Medium | 8           | Needed     |
| **Memory Management**     | 🟢 Low    | 3           | Needed     |
| **Integration Tests**     | 🟡 Medium | 5           | Needed     |
| **Type Safety**           | 🟢 Low    | 2           | Needed     |
| **Debug/Logging**         | 🟢 Low    | 3           | Needed     |
| **Concurrency**           | 🟢 Future | 2           | Future     |

**Current Coverage:** 7 tests ✅  
**Recommended Total:** 81 tests  
**Completion:** 8.6%

---

## 🔴 HIGH PRIORITY - Tests for Today's Changes

### 1. Exception Handling Tests (NEW Exception System)

#### 1.1 PipeXException (Base Exception)
```cpp
TEST(ExceptionTest, PipeXException_InheritanceFromStdRuntimeError)
TEST(ExceptionTest, PipeXException_MessageFormatting)
```

#### 1.2 InvalidPipelineException
```cpp
TEST(ExceptionTest, InvalidPipelineException_MessageFormatting)
TEST(ExceptionTest, InvalidPipelineException_InheritanceChain)
TEST(ExceptionTest, InvalidPipelineException_MemberAccess)
```

#### 1.3 TypeMismatchException
```cpp
TEST(ExceptionTest, TypeMismatchException_MessageFormatting)
TEST(ExceptionTest, TypeMismatchException_TypeInfoAccessors)
```

#### 1.4 NodeNameConflictException
```cpp
TEST(ExceptionTest, NodeNameConflictException_MessageFormatting)
TEST(ExceptionTest, NodeNameConflictException_MemberAccess)
```

---

### 2. Pipeline Integrity Validation (NEW FEATURE)

```cpp
TEST(PipelineIntegrityTest, SourceNode_MustBeFirst)
// Add Filter, then try to add Source → expect InvalidPipelineException

TEST(PipelineIntegrityTest, SourceNode_OnlyOne)
// Add Source, then try to add another Source → expect InvalidPipelineException

TEST(PipelineIntegrityTest, SinkNode_MustBeLast)
// Add Source, Sink, then try to add Filter → expect InvalidPipelineException

TEST(PipelineIntegrityTest, SinkNode_OnlyOne)
// Add Source, Sink, then try to add another Sink → expect InvalidPipelineException

TEST(PipelineIntegrityTest, NodeNameUniqueness)
// Add node with name "MyNode" twice → expect InvalidPipelineException

TEST(PipelineIntegrityTest, ValidPipeline_NoExceptions)
// Source → Filter → Transformer → Sink → should succeed
```

---

### 3. Node Removal Tests (NEW FEATURE)

```cpp
TEST(PipelineManagementTest, RemoveNodeByName_ExistingNode)
// Add 3 nodes, remove middle one, verify count and order

TEST(PipelineManagementTest, RemoveNodeByName_NonExistentNode)
// Remove non-existent node, verify no exception and unchanged pipeline

TEST(PipelineManagementTest, RemoveNodeByName_SourceNode)
// Remove Source node, verify hasSourceNode flag is false

TEST(PipelineManagementTest, RemoveNodeByName_SinkNode)
// Remove Sink node, verify hasSinkNode flag is false

TEST(PipelineManagementTest, RemoveNodeByName_Chaining)
// Test method chaining: removeNodeByName("A").removeNodeByName("B")
```

---

### 4. Pipeline Validation Tests (NEW FEATURE)

```cpp
TEST(PipelineValidationTest, IsValid_MissingSource)
// Pipeline without Source, isValid() returns false with details

TEST(PipelineValidationTest, IsValid_MissingSink)
// Pipeline without Sink, isValid() returns false with details

TEST(PipelineValidationTest, IsValid_CompletePipeline)
// Pipeline with Source and Sink, isValid() returns true

TEST(PipelineValidationTest, Run_ThrowsWhenMissingSource)
// Try to run pipeline without Source → expect InvalidPipelineException

TEST(PipelineValidationTest, Run_ThrowsWhenMissingSink)
// Try to run pipeline without Sink → expect InvalidPipelineException
```

---

### 5. INode Interface Tests (NEW isSource/isSink methods)

```cpp
TEST(NodeInterfaceTest, Source_IsSourceReturnsTrue)
TEST(NodeInterfaceTest, Source_IsSinkReturnsFalse)
TEST(NodeInterfaceTest, Sink_IsSinkReturnsTrue)
TEST(NodeInterfaceTest, Sink_IsSourceReturnsFalse)
TEST(NodeInterfaceTest, Filter_BothReturnFalse)
TEST(NodeInterfaceTest, Transformer_BothReturnFalse)
TEST(NodeInterfaceTest, Processor_BothReturnFalse)
TEST(NodeInterfaceTest, Aggregator_BothReturnFalse)
```

---

## 🟡 MEDIUM PRIORITY - Robustness Tests

### 6. Individual Node Tests

#### Source Node
```cpp
TEST(SourceNodeTest, Constructor_WithFunction)
TEST(SourceNodeTest, Constructor_WithNameAndFunction)
TEST(SourceNodeTest, Process_GeneratesData)
TEST(SourceNodeTest, Clone_CreatesIndependentCopy)
```

#### Sink Node
```cpp
TEST(SinkNodeTest, Constructor_WithFunction)
TEST(SinkNodeTest, Constructor_WithNameAndFunction)
TEST(SinkNodeTest, Process_ConsumesData)
TEST(SinkNodeTest, Clone_CreatesIndependentCopy)
```

#### Processing Nodes
```cpp
TEST(TransformerTest, Clone_PreservesFunction) // ✅ Process test exists
TEST(FilterTest, Clone_PreservesPredicate)     // ✅ Process test exists
TEST(ProcessorTest, Clone_PreservesFunction)   // ✅ Process test exists
TEST(AggregatorTest, Clone_PreservesFunction)  // ✅ Process test exists
```

---

### 7. Complex Pipeline Execution

```cpp
TEST(PipelineExecutionTest, MultiStepTransformation)
// Source → Transformer → Filter → Transformer → Sink

TEST(PipelineExecutionTest, WithAggregator)
// Source → Filter → Aggregator → Sink (reduces to single value)

TEST(PipelineExecutionTest, WithProcessor)
// Source → Processor (sort) → Filter → Sink

TEST(PipelineExecutionTest, EmptyDataFlow)
// Source returns empty vector, verify Sink receives empty vector

TEST(PipelineExecutionTest, TypeMismatchException_Propagation)
// Create incompatible node chain, verify exception thrown with details
```

---

### 8. PipeXEngine Tests

#### Singleton Pattern
```cpp
TEST(PipeXEngineTest, Singleton_SameInstance)
TEST(PipeXEngineTest, Singleton_DeletedCopyConstructor) // compile-time
TEST(PipeXEngineTest, Singleton_DeletedAssignment)     // compile-time
```

#### Pipeline Management
```cpp
TEST(PipeXEngineTest, NewPipeline_CreatesAndReturns)
TEST(PipeXEngineTest, NewPipeline_Multiple)
TEST(PipeXEngineTest, ClearPipelines_RemovesAll)
```

#### Parallel Execution
```cpp
TEST(PipeXEngineTest, Start_SinglePipeline)
TEST(PipeXEngineTest, Start_MultiplePipelinesInParallel)
TEST(PipeXEngineTest, Start_HandlesExceptionInOnePipeline)
```

---

## 🟢 LOW PRIORITY - Enhancement Tests

### 9. Memory Management

```cpp
TEST(MemoryTest, Data_WrapUnwrap)
TEST(MemoryTest, IData_PolymorphicStorage)
TEST(MemoryTest, Pipeline_NoMemoryLeaks) // Use valgrind or sanitizers
```

---

### 10. Integration Tests

```cpp
TEST(IntegrationTest, RealWorld_DataProcessingPipeline)
// Complete end-to-end scenario with multiple transformations

TEST(IntegrationTest, MultipleEngineRuns)
// Create, run, clear, create again, run again

TEST(IntegrationTest, PipelineReuse)
// Run same pipeline multiple times

TEST(IntegrationTest, EdgeCase_SingleNodePipeline)
// Only Source + Sink

TEST(IntegrationTest, EdgeCase_LargeDataSet)
// Process 1M+ elements, verify memory efficiency
```

---

### 11. Type Safety

```cpp
TEST(TypeSafetyTest, CompileTime_NodeMustDeriveFromINode)
// Compile-time check via static_assert

TEST(TypeSafetyTest, MyTypeTraits_Utilities)
// Test custom type traits from my_type_traits.h
```

---

### 12. Debug & Logging

```cpp
TEST(DebugTest, Constructor_LogsCorrectly)
TEST(DebugTest, Destructor_LogsCorrectly)
TEST(DebugTest, Process_LogsNodeExecution)
```

---

## Implementation Recommendations

### Test Fixtures

Create reusable test fixtures:

```cpp
class PipelineTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        pipeline = std::extended_std::make_unique<Pipeline>("TestPipeline");
    }
    
    std::unique_ptr<Pipeline> pipeline;
};

class NodeTestFixture : public ::testing::Test {
protected:
    std::vector<int> createTestData(int count) {
        std::vector<int> data;
        for (int i = 0; i < count; ++i) {
            data.push_back(i);
        }
        return data;
    }
};
```

### Parameterized Tests

Use Google Test parameterized tests for similar scenarios:

```cpp
class NodeTypeTest : public ::testing::TestWithParam</* node type */> {
    // Test isSource/isSink for all node types
};
```

---

## Test Execution Strategy

1. **Phase 1:** Implement all 🔴 HIGH PRIORITY tests (33 tests)
2. **Phase 2:** Implement 🟡 MEDIUM PRIORITY tests (28 tests)
3. **Phase 3:** Implement 🟢 LOW PRIORITY tests (20 tests)

### Commands

```bash
# Run all tests
cd cmake-build-debug
./tests/PipeX_all_tests

# Run specific test suite
./tests/PipeX_all_tests --gtest_filter=ExceptionTest.*

# Run with verbose output
./tests/PipeX_all_tests --gtest_filter=PipelineIntegrityTest.* --gtest_verbose
```

---

## Additional Validation

### Code Coverage
```bash
# Use gcov/lcov for coverage analysis
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON ..
make
./tests/PipeX_all_tests
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

### Memory Leak Detection
```bash
# Use valgrind
valgrind --leak-check=full --show-leak-kinds=all ./tests/PipeX_all_tests
```

### Thread Sanitizer (for future concurrency tests)
```bash
# Compile with thread sanitizer
cmake -DCMAKE_CXX_FLAGS="-fsanitize=thread" ..
make
./tests/PipeX_all_tests
```

---

## Notes

- ✅ = Test already exists in codebase
- 🔴 = High priority (critical for today's changes)
- 🟡 = Medium priority (important for robustness)
- 🟢 = Low priority (nice to have)

This checklist reflects the refactoring done on December 28, 2025, which included:
- New exception hierarchy
- Pipeline integrity validation
- Node removal functionality
- isSource/isSink methods
- Memory management refactoring
