#ifndef THIRD_PARTY
#define THIRD_PARTY
#include <string>
#include <fstream>

// Source - https://stackoverflow.com/a/116220
// Posted by Konrad Rudolph, modified by community. See post 'Timeline' for change history
// Retrieved 2026-06-05, License - CC BY-SA 4.0

auto read_file(std::string_view path) -> std::string;

#endif