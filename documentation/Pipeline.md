What to do with copy-constructors of Pipeline?
- Move semantics?
- Copying the internal state?

--> but what about the pipeline creation logic? (pipeline.addnode<>().addnode<>()...)?

**Go check factory pattern**: should it be used only with pointers?
