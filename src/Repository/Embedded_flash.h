//
// Created by aeols on 2026-03-09.
//

#ifndef REPOSITORY_EMBEDDED_FLASH_H
#define REPOSITORY_EMBEDDED_FLASH_H
#include <cstdint>

namespace Repository {
    class Embedded_flash {
    public:
        virtual ~Embedded_flash() = default;
        virtual void open_for_write(bool open) = 0;
        virtual bool is_open_for_write() = 0;
        virtual bool erase_page(uint32_t bank, uint32_t page) = 0;
        virtual bool read_quad(const uint32_t* address, uint32_t* quadword) = 0;
        virtual bool program_quad(uint32_t* address, const uint32_t quadword[4]) = 0;
        virtual void read_page(uint32_t* buffer, const uint32_t* page, uint32_t size_8) = 0;
        virtual void begin_programming() = 0;
        virtual void end_programming() = 0;
        virtual void print_diag() = 0;
        virtual uint32_t* get_left_page() = 0;
        virtual uint32_t* get_right_page() = 0;
        virtual bool erase_left_page() = 0;
        virtual bool erase_right_page() = 0;
        [[nodiscard]] virtual bool assert_address(const uint32_t* address) const = 0;
    };
} // namespace Repository

#endif // REPOSITORY_EMBEDDED_FLASH_H
