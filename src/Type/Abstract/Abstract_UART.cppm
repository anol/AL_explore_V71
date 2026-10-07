module;
#include <cstdint>

export module Type.Abstract_UART;
export import Type.Status_code;
export import Type.Misc_type;

namespace Abstract {
    export class Abstract_UART {
    public:
        Abstract_UART() = default;

        virtual ~Abstract_UART() = default;

        virtual void initialize() = 0;

        virtual bool for_each_input(Optional_user, Optional_func) = 0;

        virtual bool is_ready() = 0;

        virtual int print(const char *data, int len) = 0;

        virtual bool toggle_echo() = 0;

        [[nodiscard]] virtual Status_code put(uint8_t data) = 0;

        [[nodiscard]] virtual Status_code get(uint8_t *data) = 0;
    };
}
