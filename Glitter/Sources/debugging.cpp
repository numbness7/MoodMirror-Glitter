#include <debugging.hpp>
namespace debug{
    void periodicallyPrint(PrintInfo *printInfo)
    {
        bool tempTerminate = false;
        float tempWaitTime;
        float pauseTime = 0.0f;
        bool queuedItems = false;
        while(!tempTerminate){
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1000.0f*pauseTime)));
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1000.0f*tempWaitTime)));
            {
                std::lock_guard<std::mutex>(printInfo->lock);
                tempTerminate = printInfo->terminate;
                tempWaitTime = printInfo->waitTime;
                queuedItems = !printInfo->printQueue.empty();
            }
            while(queuedItems){
                std::lock_guard<std::mutex>(printInfo->lock);
                if(printInfo->pauseTime > 0.0f)
                    break;
                if(printInfo->output == OUTPUT::TERMINAL)
                    std::cout << printInfo->printQueue.front();
                else{
                    writeToFile(printInfo->logPath, getWorldTime() + ": " + printInfo->printQueue.front(), true);
                }
                printInfo->printQueue.pop();
                queuedItems = !printInfo->printQueue.empty();
            }
            {
                std::lock_guard<std::mutex>(printInfo->lock);
                pauseTime = printInfo->pauseTime;
                printInfo->pauseTime = 0.0f;
            }
        }
    }
    void checkPrint(std::string message, bool check, bool debugIt)
    {
        if(check && debugIt)
            std::cout << message;
    }

    RunawayPrinter::RunawayPrinter(float waitTime)
    {
        printInfo.waitTime = waitTime;
        printInfo.terminate = false;
        printInfo.output = OUTPUT::TERMINAL;
        startPrinterThread();
    }

    RunawayPrinter::RunawayPrinter(float waitTime, std::string logPath)
    {
        printInfo.waitTime = waitTime;
        printInfo.terminate = false;
        printInfo.output = OUTPUT::FILE;
        printInfo.logPath = logPath;
        startPrinterThread();
        }

    RunawayPrinter::~RunawayPrinter()
    {
        end();
    }

    void RunawayPrinter::pause(float pauseTime)
    {
        std::lock_guard<std::mutex> guard(this->printInfo.lock);
        printInfo.pauseTime = pauseTime;
    }

    void RunawayPrinter::reset()
    {
        end();
        printInfo.output = OUTPUT::TERMINAL;
        startPrinterThread();
    }

    void RunawayPrinter::reset(float waitTime)
    {
        end();
        printInfo.waitTime = waitTime;
        printInfo.output = OUTPUT::TERMINAL;
        startPrinterThread();
    }

    void RunawayPrinter::reset(float waitTime, std::string logPath)
    {
        end();
        printInfo.waitTime = waitTime;
        printInfo.logPath = logPath;
        printInfo.output = OUTPUT::FILE;
    }

    void RunawayPrinter::reset(std::string logPath)
    {
        end();
        printInfo.logPath = logPath;
        printInfo.output = OUTPUT::FILE;
    }


    void RunawayPrinter::terminateThread()
    {
        {
            std::lock_guard<std::mutex>(printInfo.lock);
            printInfo.terminate = true;
            printThread.join();
            printInfo.terminate = false;
        }
    }

    void RunawayPrinter::pushMessage(std::string message, char end)
    {
        std::lock_guard<std::mutex>(printInfo.lock);
        printInfo.printQueue.push(message + end);
    }

    void RunawayPrinter::startPrinterThread()
    {
        printThread = std::thread(periodicallyPrint, &printInfo);
    }

    void RunawayPrinter::emptyMessages()
    {
        std::lock_guard<std::mutex>(printInfo.lock);
        while(!printInfo.printQueue.empty())
            printInfo.printQueue.pop();
    }

    void RunawayPrinter::end()
    {
        terminateThread();
        emptyMessages();
    }

    StopWatch::StopWatch()
    {
        start();
    }

    void StopWatch::start()
    {
        start_time = getNow();
    }

    uint64_t StopWatch::time()
    {
        auto end_time = getNow();
        return duration(start_time, end_time);
    }

}