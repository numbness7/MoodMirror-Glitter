#ifndef THIRD_PARTY
#define THIRD_PARTY
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>

// ChatGPT Start

/// @brief Runs bash command
/// @param command the command, don't allow untrusted user input
/// @return Command Status
int runCommand (std::string command);
void killCommand (int pid);
// ChatGPT End
#endif