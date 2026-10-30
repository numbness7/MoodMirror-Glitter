#ifndef THIRD_PARTY
#define THIRD_PARTY
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>

// ChatGPT Start

// ChatGPT Start
/// @brief Runs bash command
/// @param command the command, don't allow untrusted user input
/// @return Command Status
int runCommand (std::string command);
/// @brief Terminates program created with runCommand
/// @param pid The program to be terminated
void killCommand (int pid);
// ChatGPT End
#endif