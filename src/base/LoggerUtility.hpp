#ifndef CHAINING_BASE_LOGGER_UTILITY_HPP__
#define CHAINING_BASE_LOGGER_UTILITY_HPP__

#include "Timestamp.hpp"
#include <fstream>
#include <string_view>
#include <vector>

namespace Chaining {
namespace base {

#define KBYTES 1024
#define MBYTES (1024 * (KBYTES))
#define GBYTES (1024 * (MBYTES))

    class FileUtility {
    public:
        explicit FileUtility(std::string_view base_file_name = "log/log",
            double flush_every = 1.0,
            size_t round_robin = 0,
            size_t logger_file_max_size = GBYTES);

        void append(std::string_view msg);
        void append(const void* buffer, size_t size);

    private:
        void open_new_file();
        void reopen_current_file();

    private:
        std::ofstream writer_;
        const std::string base_file_name_;
        time::Timestamp last_flush_time_point_;
        uint64_t flush_every_millseconds_;
        size_t round_robin_;
        const size_t logger_file_max_size_;
        size_t already_write_bytes_;
        std::vector<std::string> appendix_;
        size_t current_append_index_;
    };

}
}

#endif