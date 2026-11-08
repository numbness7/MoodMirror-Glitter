#ifndef THIRD_PARTY
#define THIRD_PARTY
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>


// ChatGPT Start
typedef std::chrono::_V2::system_clock::time_point simple_time_point;

int64_t duration(std::chrono::_V2::system_clock::time_point start,std::chrono::_V2::system_clock::time_point end);
std::chrono::_V2::system_clock::time_point getNow();
std::string unixTimeStamp();
std::string getWorldTime();
/// @brief 
/// @param filename 
/// @param content what is written to the file
/// @param append
/// @return 0 if successful -1 if failed
int writeToFile(const std::string& filename,
                 const std::string& content,
                 bool append = false);
/// @brief Runs bash command
/// @param command the command, don't allow untrusted user input
/// @return Command Status
int runCommand (std::string command);
/// @brief Terminates program created with runCommand
/// @param pid The program to be terminated
void killCommand (int pid);
std::string unixTimeStamp();
// ChatGPT End
#endif