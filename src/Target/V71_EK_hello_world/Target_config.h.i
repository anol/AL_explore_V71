//

#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#define BUILD_INFORMATION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@ @CMAKE_BUILD_TYPE@ @GIT_BRANCH@"
#define GIT_REPO "@GIT_REPO@"
#define TARGET_NAME "@TARGET@"
#define IDEAS_PRODUCT_ID "@APP@"
#define ARCH_NAME "@ARCH_NAME@"

namespace Target_config{
    enum{
        Default_priority = 1,
        Default_stack_size = 256,
        //
        Heartbeat_task_priority = Default_priority,
        Heartbeat_task_stack_size = Default_stack_size,
        //
        Console_transmit_task_priority = Default_priority,
        Console_transmit_task_stack_size = Default_stack_size,
        //
        Console_receive_task_priority = 2,
        Console_receive_task_stack_size = 1024,
        Console_receive_task_queue_size = 16,
        Console_receive_task_buffer_size = 128,
        //
        Primary_task_priority = Default_priority,
        Primary_task_stack_size = Default_stack_size,
    };
}
#endif // TARGET_CONFIG_H
