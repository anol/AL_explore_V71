module;
#include <cstdint>

export module Component.IDE3380_request;
import Type.Abstract_semaphore;
import Support.Configuration_repository;

export namespace IDE3380 {
    enum class IDE3380_operation : uint8_t {
        Read_register,          // the_address -> the_result
        Update_register,        // the_address, the_mode (nRead_Write), the_data -> the_result
        Update_registers,       // optional_repository, the_data (base id) -> the_success
        Dump,                   // optional_title
        Print_diagnostics,
        Set_ASIC,               // the_address (ASIC index) -> the_success
        Enable_external_hold,
        Disable_external_hold,
        Override_all_thresholds,
        Disable_all_channels,
        Restore_all_channels,
        Enable_channel,         // the_address (channel index)
        Disable_channel,        // the_address (channel index)
        Set_channel_threshold,  // the_address (channel index), the_mode (threshold) -> the_result
    };

    /**
     * One operation queued to the IDE3380 service. The request lives on the caller's stack; the caller
     * blocks until the service has executed it, so the service may read and write it freely.
     */
    struct IDE3380_request {
        IDE3380_operation the_operation;
        uint8_t the_address{};
        uint8_t the_mode{};
        uint32_t the_data{};
        const char *optional_title{};
        Repository::Configuration_repository *optional_repository{};
        uint32_t the_result{};
        bool the_success{};
        Abstract::Abstract_semaphore *optional_done{};  // set by the service, given when executed
    };
}
