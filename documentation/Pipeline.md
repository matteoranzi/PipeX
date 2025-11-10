What to do with copy-constructors of Pipeline?
- Move semantics?
- Copying the internal state?

--> but what about the pipeline creation logic? (pipeline.addnode<>().addnode<>()...)?

**Go check factory pattern**: should it be used only with pointers?


---
Currently the Nodes in the Pipeline have the same InputT and OutputT types as the Pipeline itself.
Future work could involve allowing Nodes to have different InputT and OutputT types, enabling more complex data processing workflows.
->How to solve this problem with: https://github.com/claudiofantacci/any/blob/master/src/libany/include/libany/any.h#L213