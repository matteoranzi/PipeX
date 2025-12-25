//
// Created by Matteo Ranzi on 25/12/25.
//

#ifndef PIPEX_IPIPELINE_H
#define PIPEX_IPIPELINE_H
namespace PipeX {
    class IPipeline {
    public:
        virtual ~IPipeline() = default;
        virtual void start() const = 0;
    };
}
#endif //PIPEX_IPIPELINE_H