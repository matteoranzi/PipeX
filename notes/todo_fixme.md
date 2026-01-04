//------------------------------------------------
// THIS SECTION IS A COLLECTION OF GENERAL TODOS AND REQUIRED FIXES FOR THE PROJECT
//------------------------------------------------

# TODO
- [x] Add proper include directives in each source/header file to ensure all dependencies are met.
- [ ] Implement unit tests for all classes and methods to ensure correctness and robustness.
- [ ] Compile time pipeline validation via static_cast, by using [simplified custom implementation of std::any](https://medium.com/@sonudgr82013/understanding-type-erasure-idiom-in-c-bca2374956ac) (is it possible, or it is still runtime?)
- [ ] Add method to get the list of nodes in the pipeline (e.g. for visualization or debugging purposes)
- [x] Improve extraction/wrapping logic in NodeCRTP (currently each pass copies data multiple times)
- [ ] Implement tests for Sink and Source nodes
- [ ] Verify if nodes datatypes are compatible before running/adding them in the pipeline, currently this is only checked at runtime when the pipeline is run (pipeline may throw an exception in the last node after processing all the previous nodes successfully).
Currently, pipeline integrity check don't consider compatibility of InputT/OutputT between nodes, only the presence of source/sink nodes and unique names is checked

# FIXME
- [x] Update all logLifeCycle prints with the proper values