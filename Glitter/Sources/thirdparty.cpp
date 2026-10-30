#include <thirdparty.hpp>

int runCommand (std::string command) {
    pid_t pid = fork();

    if (pid == 0) {
        setpgid(0, 0);
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
    kill(-pid, SIGTERM);
    waitpid(pid, nullptr, 0);
}
// ChatGPT End