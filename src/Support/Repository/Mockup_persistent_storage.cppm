

module;
#include <cstdint>

export module Support.Mockup_persistent_storage;
export import Support.Persistent_storage;

export namespace MOCKUP {
    class Mockup_persistent_storage : public Repository::Persistent_storage {
    public:
        [[nodiscard]] Status_code initialize() override { return Status_code::Success(); }

        [[nodiscard]] Status_code clean() override { return Status_code::Success(); }

        [[nodiscard]] Status_code open_reading() override { return Status_code::Success(); }

        [[nodiscard]] Status_code read(uint32_t id, int32_t &value) override { return Status_code::Success(); }

        [[nodiscard]] Status_code open_writing(int dirty_count) override { return Status_code::Success(); }

        [[nodiscard]] Status_code write_cache(uint32_t id, int32_t value) override { return Status_code::Success(); }

        [[nodiscard]] Status_code program_flash() override { return Status_code::Success(); }

        void dump() const override {
        }

        void print_diag() const override {
        }

        static uint32_t get_page_size() { return 0; }
    };
} // MOCKUP
