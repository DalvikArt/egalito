#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <functional>

#include "loop.h"
#include "disass.h"
#include "conductor/setup.h"

void TopLevelCommand::invokeDefault(Arguments args) {
    std::cout << "unknown command, try \"help\"\n";
}

static void printUsageHelper(Command *command, int level);
static void printUsage(TopLevelCommand *command) {
    std::cout << "usage:\n";
    for(auto c : command->getMap()) {
        printUsageHelper(c.second, 1);
    }
}
static void printUsageHelper(Command *command, int level) {
    for(int i = 0; i < level; i ++) std::cout << "    ";
    std::cout << std::left << std::setw(10) << command->getName()
        << " " << command->getDescription() << std::endl;
    if(auto composite = dynamic_cast<CompositeCommand *>(command)) {
        for(auto c : composite->getMap()) {
            printUsageHelper(c.second, level + 1);
        }
    }
}

void mainLoop() {
    ConductorSetup setup;
    ConductorSetup *pSetup = &setup;
    bool running = true;

    TopLevelCommand topLevel;
    TopLevelCommand *pTopLevel = &topLevel;
    topLevel.add("quit", [&] (Arguments) { running = false; },
        "quit the egalito shell");
    topLevel.add("help", [&] (Arguments) { printUsage(&topLevel); },
        "prints this help message");
    topLevel.add("q", [&] (Arguments args) { (*topLevel.getMap()["quit"])(args); });

    topLevel.add("parse", [&] (Arguments args) {
        args.shouldHave(1);
        setup.parseElfFiles(args.front().c_str(), false, false);
    }, "parses the given ELF executable");
    topLevel.add("parse2", [&] (Arguments args) {
        args.shouldHave(1);
        setup.parseElfFiles(args.front().c_str(), true, true);
    }, "parses the given ELF and all its shared libraries");

    registerDisassCommands(pTopLevel, pSetup);

    while(running) {
        std::cout << "egalito> ";
        std::cout.flush();

        std::string line;
        std::getline(std::cin, line);
        std::istringstream sstream(line);
        Arguments args;
        std::string arg;
        while(sstream >> arg) args.add(arg);

        if(args.size() > 0) {
            try {
                topLevel(args);
            }
            catch(const char *s) {
                std::cout << "error: " << s << std::endl;
            }
            catch(const std::string &s) {
                std::cout << "error: " << s << std::endl;
            }
        }
    }
}
