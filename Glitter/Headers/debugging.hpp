#ifndef DEBUGGING_H
#define DEBUGGING_H
#include <iostream>
#include <thirdparty.hpp>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
// #Debug -------------------------------------------------------------

namespace debug{

    class StopWatch{
        public:
            StopWatch();
            void start();
            uint64_t time();
        private:
            simple_time_point start_time;
    };        

    enum class OUTPUT {
        FILE,
        TERMINAL
    };

    struct PrintInfo{
        std::mutex lock;
        std::queue<std::string> printQueue;
        float waitTime;
        std::thread printThread;
        bool terminate;
        float pauseTime;
        std::string logPath;
        OUTPUT output;
    };

    void periodicallyPrint(PrintInfo* printInfo);

    class RunawayPrinter {
        public:
            RunawayPrinter(float waitTime);
            RunawayPrinter(float waitTime, std::string logPath);
            ~RunawayPrinter();
            void pause(float pauseTime);
            void reset();
            void reset(float waitTime);
            void reset(float waitTime, std::string logPath);
            void reset(std::string logPath);
            void terminateThread();
            void pushMessage(std::string message, char end='\n');
            void startPrinterThread();
            void emptyMessages();
            void end();
        private:
            PrintInfo printInfo;
            std::thread printThread;
    };

        

    /// @brief Prints a message if a condition is fulfilled and if this print has been enabled.
    /// @param message printed.
    /// @param check condition to check, true if fulfilled, false otherwise.
    /// @param debugIt true if enabled, false otherwise.

    void checkPrint(std::string message, bool check, bool debugIt);


}

#endif