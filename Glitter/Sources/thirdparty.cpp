#include <thirdparty.hpp>


// ChatGPT Start





std::string getWorldTime() {
    std::ostringstream ss;
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int writeToFile(const std::string& filename,
                 const std::string& content,
                 bool append) {

    std::ofstream file;

    if (append)
        file.open(filename, std::ios::app);
    else
        file.open(filename, std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Failed to open file!\n";
        return -1;
    }

    file << content;
    file.close();
    return 0;
}

int runCommand (std::string command) {
    pid_t pid = fork();

    if (pid == 0) {
        execl("/bin/bash", "bash", "-c",
              command.c_str(),
              (char*)nullptr);
        _exit(127);
    }

    if (pid > 0) {
        setpgid(pid, pid);
        return pid;

    }
    return -1;
}


void killCommand(int pid){
    // Later, terminate the entire process group:
    kill(pid, SIGKILL);
    waitpid(pid, nullptr, 0);
}
// ChatGPT End