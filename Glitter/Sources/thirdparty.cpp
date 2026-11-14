#include <thirdparty.hpp>


// ChatGPT Start


int64_t duration(std::chrono::_V2::system_clock::time_point start,std::chrono::_V2::system_clock::time_point end){
    auto duration = end - start;
    auto seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return seconds;
}

std::chrono::_V2::system_clock::time_point getNow(){
    auto now = std::chrono::system_clock::now();
    return now;
}

std::string unixTimeStamp(){
    std::ostringstream ss;
    auto now = std::chrono::system_clock::now();

    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();

    ss << timestamp;
    return ss.str();
}


std::string getWorldTime() {
    std::ostringstream ss;
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}





// Source - https://stackoverflow.com/a/116220
// Posted by Konrad Rudolph, modified by community. See post 'Timeline' for change history
// Retrieved 2026-11-14, License - CC BY-SA 4.0
auto read_file(std::filesystem::path const& path) -> std::string {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path);
    stream.exceptions(std::ios_base::badbit);

    if (not stream) {
        throw std::ios_base::failure("file does not exist");
    }
    
    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

auto deleteFile(std::filesystem::path const& path) -> void {
    std::remove(path.c_str());
}
    


// ChatGPT Start
int writeToFile(const std::string& filename,
                 const std::string& content,
                 bool append, bool prepend) {

    assert(!(append && prepend));
    std::ofstream file;
    std::string newContent = content;
    if(prepend){
        try{
            newContent = newContent + read_file(filename);
        }
        catch(std::ios_base::failure){
            // Do nothing
        }
    }

    if (append)
        file.open(filename, std::ios::app);
    else
        file.open(filename, std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Failed to open file!\n";
        return -1;
    }

    file << newContent;
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