#pragma once
#include "Persistent_storage.h"

namespace MOCKUP {
    class Mockup_persistent_storage : public Repository::Persistent_storage {
    public:
        [[nodiscard]] bool initialize() override { return true; }

        [[nodiscard]] bool clean() override { return true; }

        [[nodiscard]] bool open_reading() override { return true; }

        [[nodiscard]] bool read(uint32_t id, int32_t &value) override { return true; }

        [[nodiscard]] bool open_writing(int dirty_count) override { return true; }

        [[nodiscard]] bool write_cache(uint32_t id, int32_t value) override { return true; }

        [[nodiscard]] bool program_flash() override { return true; }

        void dump() const override {
        }

        void print_diag() const override {
        }

        static uint32_t get_page_size() { return 0; }
    };
} // MOCKUP
