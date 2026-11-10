module;
#include <cstdint>

export module Board.Win11_pin_table;
import Platform.Mockup_IO_pin;
import Domain.IO_pins;
import Type.Abstract_IO_pin;
import Type.Abstract_pin_table;

export namespace Board {
    /// One mock pin per Domain pin id. Chip selects idle high, like on the real board.
    class Win11_pin_table : public Abstract::Abstract_pin_table {
        Mockup::Mockup_IO_pin the_pin_table[Domain::Number_of_pins]{};

    public:
        Win11_pin_table();

        void initialize() override;

        Abstract::Abstract_IO_pin &get_pin(Abstract::Abstract_pin_id id) override;

        Status_code set_phase(Abstract::Abstract_IO_pin::Pin_phase) override { return Status_code::Success(); }

        Status_code for_each_pin(void *user, void (*func)(void *, Abstract::Abstract_IO_pin &)) override;

        [[nodiscard]] uint8_t get_pin_count() const override { return Domain::Number_of_pins; }

        void print_diagnostics() const override;
    };
} // Board
