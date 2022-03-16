#include "base/LoggerUtility.hpp"
#include <cstring>
#include <filesystem>

using namespace Chaining::base;

constexpr size_t MILL_PER_SECONDS { 1000 };

namespace {
void ensure_dirent_exist(std::string_view file_name)
{
    size_t lastSlash { file_name.find_last_of('/') };
    if (lastSlash != std::string::npos) {
        std::filesystem::path logPath { file_name.substr(0, lastSlash) };
        std::filesystem::directory_entry entry { logPath };
        if (!entry.exists()) {
            std::filesystem::create_directories(entry);
        }
    }
}
}

FileUtility::FileUtility(std::string_view base_file_name, double flush_every,
    size_t round_robin, size_t logger_file_max_size)
    : base_file_name_ { base_file_name }
    , last_flush_time_point_ { time::currentTime() }
    , flush_every_millseconds_ { static_cast<uint64_t>(flush_every * MILL_PER_SECONDS) }
    , round_robin_ { round_robin }
    , logger_file_max_size_ { logger_file_max_size }
    , already_write_bytes_ { 0 }
    , appendix_ {}
    , current_append_index_ { 0 }
{
    for (int i = 0; i < round_robin_; i++) {
        appendix_.push_back("_" + std::to_string(i + 1));
    }

    std::string open_file_name;
    if (round_robin_ > 0) {
        open_file_name = base_file_name_ + appendix_[0] + ".log";
    } else {
        open_file_name = base_file_name_ + ".log";
    }

    ensure_dirent_exist(open_file_name);

    writer_.open(open_file_name, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!writer_.is_open()) {
        throw std::runtime_error { "FileUtilty " + open_file_name + " open failed: " + ::strerror(errno) };
    }
}

void FileUtility::open_new_file()
{
    ++current_append_index_;
    current_append_index_ %= appendix_.size();

    std::string new_file_name { base_file_name_ + appendix_[current_append_index_] + ".log" };
    writer_.flush();
    writer_.close();

    writer_.open(new_file_name, std::ios::out | std::ios::binary);
    if (!writer_.is_open()) {
        throw std::runtime_error { "FileUtility open_new_file: " + new_file_name + " open failed: " + ::strerror(errno) };
    }
    already_write_bytes_ = 0;
}

void FileUtility::reopen_current_file()
{
    writer_.flush();
    writer_.close();

    std::string logger_file_name { base_file_name_ + ".log" };
    writer_.open(logger_file_name, std::ios::out | std::ios::binary | std::ios::app);
    if (!writer_.is_open()) {
        throw std::runtime_error { "FileUtility reopen_current_file: " + logger_file_name + " open failed: " + ::strerror(errno) };
    }
    std::string new_block_message { "#--------------------- Current create the new block of logger Content------------------------- #" };

    writer_.write("\n", 1);
    writer_.write(new_block_message.c_str(), new_block_message.size());
    writer_.write("\n", 1);

    already_write_bytes_ = 0;
}

void FileUtility::append(std::string_view msg)
{
    append(msg.data(), msg.size());
}

void FileUtility::append(const void* buffer, size_t size)
{
    if (already_write_bytes_ + size > logger_file_max_size_ && round_robin_ > 0) {
        open_new_file();
    } else if (already_write_bytes_ + size > logger_file_max_size_) {
        reopen_current_file();
    }

    writer_.write(static_cast<const char*>(buffer), size);
    already_write_bytes_ += size;
    if (time::isPassedTime(last_flush_time_point_ + std::chrono::milliseconds(flush_every_millseconds_))) {
        last_flush_time_point_ = time::currentTime();
        writer_.flush();
    }
}