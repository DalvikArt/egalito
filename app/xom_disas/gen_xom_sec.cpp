#include <iostream>
#include <cstdio>
#include <vector>

#include "analysis/controlflow.h"
#include "conductor/setup.h"
#include "conductor/conductor.h"
#include "conductor/passes.h"
#include "chunk/chunkiter.h"
#include "chunk/function.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " Binary" << std::endl;
        exit(0);
    }

    char *binary_file = argv[1];

    ConductorSetup setup;

    // parse elf file
    setup.parseElfFiles(binary_file, false, false);

    auto main_module = *CIter::children(setup.getConductor()->getProgram()).begin();

    std::vector<Function *> funcList;
            for(auto func : CIter::functions(main_module)) {
                funcList.push_back(func);
            }

            std::sort(funcList.begin(), funcList.end(),
                [](Function *a, Function *b) {
                    if(a->getAddress() < b->getAddress()) return true;
                    if(a->getAddress() == b->getAddress()) {
                        return a->getName() < b->getName();
                    }
                    return false;
                });

            for(auto func : funcList) {
                std::printf("0x%08lx 0x%08lx %s\n",
                    func->getAddress(), func->getSize(), func->getName().c_str());
            }

    return 0;
}