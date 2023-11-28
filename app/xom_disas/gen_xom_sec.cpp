#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

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
    for(auto func : CIter::functions(main_module)) 
        funcList.push_back(func);

    std::sort(funcList.begin(), funcList.end(), [](Function *a, Function *b){
        if(a->getAddress() < b->getAddress()) return true;
        if(a->getAddress() == b->getAddress()) return a->getName() < b->getName();
        return false;});

    std::ostringstream oss;

    for(auto func : funcList) 
    {
        auto startAddr = func->getAddress();
        auto endAddr = startAddr + func->getSize();
        std::string functionName = func->getName(); 
        oss << "0x" << std::hex << std::setw(8) << std::setfill('0') << startAddr
            << " 0x" << std::hex << std::setw(8) << std::setfill('0') << endAddr
            << " " << functionName << std::endl;
        //std::printf("0x%08lx 0x%08lx %s\n",
        //func->getAddress(), func->getSize(), func->getName().c_str());
    }

    std::string finalStr = oss.str();
    std::cout << finalStr << std::endl;

    return 0;
}