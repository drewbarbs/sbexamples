#include "util.h"

#include <fstream>
#include <sstream>

std::string sb::read_file(const std::string &fname) {
    std::ifstream in;
    in.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    in.open(fname, std::ifstream::in);

    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}