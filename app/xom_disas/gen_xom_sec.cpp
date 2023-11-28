#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>

#include "analysis/controlflow.h"
#include "conductor/setup.h"
#include "conductor/conductor.h"
#include "conductor/passes.h"
#include "chunk/chunkiter.h"
#include "chunk/function.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " Binary OutputResult" << std::endl;
        exit(0);
    }

    const char *binaryFile = argv[1];
    const char *outputFile = argv[2];

    std::cout << "Start disassembling " << binaryFile << "..." << std::endl;

    ConductorSetup setup;

    // parse elf file
    setup.parseElfFiles(binaryFile, false, false);

    // the first module is expected to be "module-(executable)"
    auto mainModule = *CIter::children(setup.getConductor()->getProgram()).begin();

    // get function list
    std::vector<Function *> funcList;
    for(auto func : CIter::functions(mainModule)) 
        funcList.push_back(func);

    // sort list by address
    std::sort(funcList.begin(), funcList.end(), [](Function *a, Function *b){
        if(a->getAddress() < b->getAddress()) return true;
        if(a->getAddress() == b->getAddress()) return a->getName() < b->getName();
        return false;});

    // start building log str
    std::ostringstream oss;

    for(auto func : funcList) 
    {
        auto startAddr = func->getAddress();
        auto endAddr = startAddr + func->getSize();
        std::string functionName = func->getName(); 
        oss << "0x" << std::hex << std::setw(8) << std::setfill('0') << startAddr
            << " 0x" << std::hex << std::setw(8) << std::setfill('0') << endAddr
            << " " << functionName << std::endl;
    }

    std::string finalStr = oss.str();
    std::cout << finalStr << std::endl;

    // write result to file
    std::ofstream ofs(outputFile);
    ofs << finalStr;
    ofs.close();

    std::cout << "Done. Result is written into " << outputFile << std::endl;

    return 0;
}