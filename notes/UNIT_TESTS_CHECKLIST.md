# PipeX - Comprehensive Unit Tests Checklist

## Overview
This document outlines comprehensive unit tests to validate the PipeX Pipelined Data Processing Network based on the December 28, 2025 refactoring and enhancements.

---

## 1. Exception Handling Tests

### 1.1 PipeXException (Base Exception)
- [ ] **Test_PipeXException_InheritanceFromStdRuntimeError**
  - Verify PipeXException properly inherits from std::runtime_error
  - Verify what() returns correct message with "PipeX Library exception:" prefix

### 1.2 InvalidPipelineException
- [ ] **Test_InvalidPipelineException_MessageFormatting**
  - Verify exception message contains pipeline name and error details
  - Verify pipelineName_ and message_ members are correctly set
  
- [ ] **Test_InvalidPipelineException_InheritanceFromPipeXException**
  - Verify InvalidPipelineException inherits from PipeXException
  - Verify can be caught as PipeXException

### 1.3 TypeMismatchException
- [ ] **Test_TypeMismatchException_MessageFormatting**
  - Verify exception message contains node name, expected type, and actual type
  - Verify getNodeName(), getExpectedType(), getActualType() accessors work correctly
  
- [ ] **Test_TypeMismatchException_InheritanceFromPipeXException**
  - Verify TypeMismatchException inherits from PipeXException

### 1.4 NodeNameConflictException
- [ ] **Test_NodeNameConflictException_MessageFormatting**
  - Verify exception message contains pipeline name and conflicting node name
  - Verify pipelineName_ and nodeName_ members are correctly set
  
- [ ] **Test_NodeNameConflictException_InheritanceFromPipeXException**
  - Verify NodeNameConflictException inherits from PipeXException

---

## 2. Pipeline Construction and Management Tests

### 2.1 Pipeline Constructors
- [x] **Test_Pipeline_DefaultConstructor** (partially covered in existing tests)
  - Verify pipeline name is generated from pointer value
  - Verify empty node list after construction
  
- [x] **Test_Pipeline_NamedConstructor** (partially covered in existing tests)
  - Verify pipeline name is correctly set
  - Verify empty node list after construction

- [x] **Test_Pipeline_CopyConstructor** (EXISTING: CopyPipeline test)
  - Verify deep copy of nodes via clone()
  - Verify copied pipeline has "_copy" suffix
  - Verify nodes are independent copies
  - Verify hasSourceNode and hasSinkNode flags are copied

- [x] **Test_Pipeline_MoveConstructor** (EXISTING: MovePipeline test)
  - Verify ownership transfer of nodes
  - Verify source pipeline is left in valid but empty state
  - Verify hasSourceNode and hasSinkNode flags are transferred

### 2.2 Node Addition
- [x] **Test_Pipeline_AddNode_BasicChaining** (EXISTING: BasicPipeline test)
  - Verify addNode() returns reference for chaining
  - Verify nodes are added in correct order

- [ ] **Test_Pipeline_AddNode_LvalueReference**
  - Verify lvalue addNode() works correctly
  - Verify proper node ownership transfer

- [ ] **Test_Pipeline_AddNode_RvalueReference**
  - Verify rvalue addNode() works correctly on temporary pipelines
  - Verify proper forwarding to lvalue version

### 2.3 Pipeline Integrity Validation (NEW FEATURE)
- [ ] **Test_Pipeline_SourceNode_MustBeFirst**
  - Verify adding Source node when other nodes exist throws InvalidPipelineException
  - Verify error message is descriptive

- [ ] **Test_Pipeline_SourceNode_OnlyOne**
  - Verify adding second Source node throws InvalidPipelineException
  - Verify first Source node is retained

- [ ] **Test_Pipeline_SinkNode_MustBeLast**
  - Verify adding non-Sink node after Sink throws InvalidPipelineException
  - Verify error message is descriptive

- [ ] **Test_Pipeline_SinkNode_OnlyOne**
  - Verify adding second Sink node throws InvalidPipelineException
  - Verify first Sink node is retained

- [ ] **Test_Pipeline_NodeNameUniqueness**
  - Verify adding node with duplicate name throws InvalidPipelineException
  - Verify nodesNameSet prevents duplicates
  - Verify error message contains conflicting node name

### 2.4 Node Removal (NEW FEATURE)
- [ ] **Test_Pipeline_RemoveNodeByName_ExistingNode**
  - Verify node is removed by name
  - Verify node is removed from nodesNameSet
  - Verify method returns reference for chaining

- [ ] **Test_Pipeline_RemoveNodeByName_NonExistentNode**
  - Verify removing non-existent node does not throw
  - Verify pipeline remains unchanged

- [ ] **Test_Pipeline_RemoveNodeByName_SourceNode**
  - Verify hasSourceNode flag is reset when Source removed
  - Verify pipeline state is consistent

- [ ] **Test_Pipeline_RemoveNodeByName_SinkNode**
  - Verify hasSinkNode flag is reset when Sink removed
  - Verify pipeline state is consistent

- [ ] **Test_Pipeline_RemoveNodeByName_MiddleNode**
  - Verify removing middle node does not affect Source/Sink flags
  - Verify order of remaining nodes is preserved

### 2.5 Pipeline Validation
- [ ] **Test_Pipeline_IsValid_MissingSource**
  - Verify isValid() returns false when no Source node
  - Verify details contains "missing Source node"

- [ ] **Test_Pipeline_IsValid_MissingSink**
  - Verify isValid() returns false when no Sink node
  - Verify details contains "missing Sink node"

- [ ] **Test_Pipeline_IsValid_Complete**
  - Verify isValid() returns true with Source and Sink
  - Verify details is empty for valid pipeline

---

## 3. Node Tests

### 3.1 INode Interface
- [ ] **Test_INode_IsSource_SourceNode**
  - Verify Source<T>::isSource() returns true
  - Verify Source<T>::isSink() returns false

- [ ] **Test_INode_IsSink_SinkNode**
  - Verify Sink<T>::isSink() returns true
  - Verify Sink<T>::isSource() returns false

- [ ] **Test_INode_IsSourceAndIsSink_ProcessingNodes**
  - Verify Filter<T>::isSource() returns false
  - Verify Filter<T>::isSink() returns false
  - Verify Transformer<In,Out>::isSource() returns false
  - Verify Transformer<In,Out>::isSink() returns false
  - Verify Processor<T>::isSource() returns false
  - Verify Processor<T>::isSink() returns false
  - Verify Aggregator<In,Out>::isSource() returns false
  - Verify Aggregator<In,Out>::isSink() returns false

### 3.2 Source Node
- [ ] **Test_Source_Constructor_WithFunction**
  - Verify Source constructs with function
  - Verify name is auto-generated

- [ ] **Test_Source_Constructor_WithNameAndFunction**
  - Verify Source constructs with custom name
  - Verify function is stored correctly

- [ ] **Test_Source_Process_GeneratesData**
  - Verify process() generates data from stored function
  - Verify returns vector of IData wrapped values

- [ ] **Test_Source_Clone**
  - Verify clone() creates independent copy
  - Verify cloned Source generates same data

### 3.3 Sink Node
- [ ] **Test_Sink_Constructor_WithFunction**
  - Verify Sink constructs with function
  - Verify name is auto-generated

- [ ] **Test_Sink_Constructor_WithNameAndFunction**
  - Verify Sink constructs with custom name
  - Verify function is stored correctly

- [ ] **Test_Sink_Process_ConsumesData**
  - Verify process() invokes sink function with data
  - Verify side effects of sink function occur

- [ ] **Test_Sink_Clone**
  - Verify clone() creates independent copy
  - Verify cloned Sink has same behavior

### 3.4 Transformer Node
- [x] **Test_Transformer_Process** (EXISTING: NodeTest.Transformer)
  - Verify transforms input type to output type
  - Verify correct transformation logic
  - Verify all input elements are transformed

- [ ] **Test_Transformer_Clone**
  - Verify clone() creates independent copy with same transform function

### 3.5 Filter Node
- [x] **Test_Filter_Process** (EXISTING: NodeTest.Filter)
  - Verify filters elements based on predicate
  - Verify filtered elements are removed
  - Verify passing elements are retained

- [ ] **Test_Filter_Clone**
  - Verify clone() creates independent copy with same predicate

### 3.6 Processor Node
- [x] **Test_Processor_Process** (EXISTING: NodeTest.Processor)
  - Verify processes entire vector (e.g., sorting)
  - Verify input/output type is the same
  - Verify transformation is applied to whole collection

- [ ] **Test_Processor_Clone**
  - Verify clone() creates independent copy with same processor function

### 3.7 Aggregator Node
- [x] **Test_Aggregator_Process** (EXISTING: NodeTest.Aggregator)
  - Verify aggregates vector to single value (e.g., sum)
  - Verify output is single element
  - Verify correct aggregation logic

- [ ] **Test_Aggregator_Clone**
  - Verify clone() creates independent copy with same aggregator function

---

## 4. Pipeline Execution Tests

### 4.1 Basic Execution
- [x] **Test_Pipeline_Run_BasicDataFlow** (EXISTING: BasicPipeline test)
  - Verify data flows through all nodes
  - Verify transformations are applied in order
  - Verify final output is correct

### 4.2 Execution Validation
- [ ] **Test_Pipeline_Run_ThrowsWhenMissingSource**
  - Verify run() throws InvalidPipelineException when no Source
  - Verify exception message describes the issue

- [ ] **Test_Pipeline_Run_ThrowsWhenMissingSink**
  - Verify run() throws InvalidPipelineException when no Sink
  - Verify exception message describes the issue

### 4.3 Error Handling During Execution
- [ ] **Test_Pipeline_Run_PropagatesTypeMismatchException**
  - Create pipeline with incompatible node types
  - Verify TypeMismatchException is thrown
  - Verify exception contains node name and type info

- [ ] **Test_Pipeline_Run_HandlesNodeException**
  - Create node that throws exception during process()
  - Verify exception is caught and logged
  - Verify exception is rethrown

### 4.4 Complex Pipelines
- [ ] **Test_Pipeline_Run_MultiStepTransformation**
  - Source → Transformer → Filter → Transformer → Sink
  - Verify data flows correctly through all steps

- [ ] **Test_Pipeline_Run_WithAggregator**
  - Source → Filter → Aggregator → Sink
  - Verify aggregation reduces vector to single value

- [ ] **Test_Pipeline_Run_WithProcessor**
  - Source → Processor → Filter → Sink
  - Verify processor operates on entire vector

- [ ] **Test_Pipeline_Run_EmptyDataFlow**
  - Source generates empty vector
  - Verify pipeline completes successfully
  - Verify Sink receives empty vector

---

## 5. PipeXEngine Tests

### 5.1 Singleton Pattern
- [ ] **Test_PipeXEngine_Singleton_GetInstance**
  - Verify getPipexEngine() returns same instance
  - Verify only one instance exists

- [ ] **Test_PipeXEngine_Singleton_NoCopyConstruction**
  - Verify copy constructor is deleted (compile-time check)

- [ ] **Test_PipeXEngine_Singleton_NoAssignment**
  - Verify assignment operator is deleted (compile-time check)

### 5.2 Pipeline Management
- [ ] **Test_PipeXEngine_NewPipeline_CreatesAndReturns**
  - Verify newPipeline() creates pipeline with correct name
  - Verify returns reference to created pipeline

- [ ] **Test_PipeXEngine_NewPipeline_MultipleCreations**
  - Create multiple pipelines
  - Verify all are stored in engine
  - Verify each has unique reference

- [ ] **Test_PipeXEngine_ClearPipelines**
  - Create multiple pipelines
  - Call clearPipelines()
  - Verify pipelines vector is empty

### 5.3 Parallel Execution
- [ ] **Test_PipeXEngine_Start_RunsSinglePipeline**
  - Create one pipeline
  - Call start()
  - Verify pipeline executes to completion

- [ ] **Test_PipeXEngine_Start_RunsMultiplePipelines**
  - Create multiple pipelines
  - Call start()
  - Verify all pipelines execute in parallel
  - Verify all threads are joined before returning

- [ ] **Test_PipeXEngine_Start_HandlesException**
  - Create pipeline that throws TypeMismatchException
  - Verify exception is caught and logged
  - Verify engine continues to run other pipelines

- [ ] **Test_PipeXEngine_Start_HandlesInvalidPipeline**
  - Create invalid pipeline (missing Source/Sink)
  - Verify InvalidPipelineException is caught and logged
  - Verify engine continues

---

## 6. Memory Management Tests

### 6.1 Data Wrappers
- [ ] **Test_Data_WrapUnwrap**
  - Verify Data<T> correctly wraps primitive type
  - Verify value is accessible
  - Verify type information is preserved

- [ ] **Test_IData_PolymorphicStorage**
  - Create vector of unique_ptr<IData>
  - Store Data<int>, Data<float>, Data<string>
  - Verify dynamic_cast works correctly

### 6.2 Node Ownership
- [ ] **Test_Pipeline_NodeOwnership_UniquePtr**
  - Verify nodes are stored as unique_ptr
  - Verify no memory leaks after pipeline destruction

- [ ] **Test_Pipeline_NodeOwnership_Clone**
  - Clone pipeline
  - Verify original and copy have independent nodes
  - Destroy original, verify copy still works

### 6.3 Custom Memory Utilities (my_memory.h)
- [ ] **Test_MyMemory_MakeUnique**
  - Verify custom extended_std::make_unique works for various types
  - Verify compatibility with standard library

---

## 7. Integration Tests

### 7.1 Real-World Scenarios
- [ ] **Test_Integration_DataProcessingPipeline**
  - Read data from source
  - Apply multiple transformations
  - Filter based on criteria
  - Aggregate results
  - Output to sink
  - Verify end-to-end correctness

- [ ] **Test_Integration_MultipleEngineRuns**
  - Create engine, add pipelines, run
  - Clear pipelines
  - Add new pipelines, run again
  - Verify engine can be reused

- [ ] **Test_Integration_PipelineReuse**
  - Create pipeline
  - Run multiple times with different data
  - Verify results are consistent

### 7.2 Edge Cases
- [ ] **Test_EdgeCase_SingleNodePipeline**
  - Pipeline with only Source and Sink
  - Verify data passes through correctly

- [ ] **Test_EdgeCase_LargeDataSet**
  - Process large vector (e.g., 1 million elements)
  - Verify memory efficiency
  - Verify correctness

- [ ] **Test_EdgeCase_ComplexTypes**
  - Use custom structs/classes as data types
  - Verify transformations work correctly
  - Verify type safety

---

## 8. Type Safety and Template Tests

### 8.1 Compile-Time Checks
- [ ] **Test_CompileTime_NodeMustDeriveFromINode**
  - Verify static_assert prevents invalid node types (compile-time check)

### 8.2 Type Traits
- [ ] **Test_TypeTraits_MyTypeTraits**
  - Verify custom type traits in my_type_traits.h work correctly
  - Test any is_specialization_of or similar utilities

---

## 9. Debug and Logging Tests

### 9.1 Debug Output
- [ ] **Test_Debug_ConstructorLogging**
  - Verify PIPEX_PRINT_DEBUG_INFO outputs constructor messages
  - Verify format includes name and pointer

- [ ] **Test_Debug_DestructorLogging**
  - Verify PIPEX_PRINT_DEBUG_INFO outputs destructor messages

- [ ] **Test_Debug_ProcessLogging**
  - Verify each node logs when processing

---

## 10. Concurrency Tests (Future)

### 10.1 Thread Safety
- [ ] **Test_Concurrency_PipelineModificationWhileRunning**
  - Attempt to modify pipeline during execution
  - Verify proper locking mechanism (TODO in code)

- [ ] **Test_Concurrency_EngineStateManagement**
  - Verify state flags (running, stopped, error) work correctly (TODO in code)

---

## Summary Statistics

### Current Test Coverage (Existing Tests)
- ✅ **3 Pipeline tests** (BasicPipeline, CopyPipeline, MovePipeline)
- ✅ **4 Node tests** (Transformer, Processor, Filter, Aggregator)
- **Total: 7 tests**

### Recommended Additional Tests
- **Exception Tests:** 10
- **Pipeline Management:** 18
- **Node Tests:** 15
- **Execution Tests:** 8
- **Engine Tests:** 8
- **Memory Tests:** 3
- **Integration Tests:** 5
- **Type Safety:** 2
- **Debug Tests:** 3
- **Concurrency:** 2 (future)

### Total Recommended Tests: **81 tests**

---

## Priority Levels

### 🔴 High Priority (Critical for today's changes)
1. All Exception Handling Tests (1.1-1.4)
2. Pipeline Integrity Validation Tests (2.3)
3. Node Removal Tests (2.4)
4. Pipeline Validation Tests (2.5)
5. INode Interface Tests (3.1)
6. Execution Validation Tests (4.2)

### 🟡 Medium Priority (Important for robustness)
1. Node Clone Tests (3.2-3.7)
2. PipeXEngine Tests (5.1-5.3)
3. Error Handling During Execution (4.3)
4. Complex Pipeline Tests (4.4)

### 🟢 Low Priority (Nice to have)
1. Memory Management Tests (6)
2. Integration Tests (7)
3. Debug and Logging Tests (9)

---

## Notes
- Tests marked with ✅ already exist in the codebase
- Tests marked with [ ] need to be implemented
- Some TODO comments in code suggest future features (thread safety, state management)
- Consider using Google Test fixtures for common pipeline setup
- Consider parameterized tests for testing multiple node types with same logic

