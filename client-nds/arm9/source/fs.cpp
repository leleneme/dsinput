#include "fs.hpp"

#include <cstdio>
#include <cstddef>
#include <cstdint>

namespace dsinput {

std::optional<std::string> file_read_to_string(const char* path) {
    FILE* fd = fopen(path, "r");
    if (!fd)
        return {};

    if (fseek(fd, 0, SEEK_END) != 0) {
        fclose(fd);
        return {};
    }
    size_t file_size = static_cast<size_t>(ftell(fd));

    std::string contents;
    contents.resize(file_size);

    rewind(fd);
    fread(&contents[0], sizeof(char), file_size, fd);

    fclose(fd);
    return contents;
}

}
