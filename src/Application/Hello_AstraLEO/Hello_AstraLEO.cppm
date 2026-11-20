//
// Created by aeols on 12.08.2026.
//

module;

export module Application.Hello_AstraLEO;
import Type.Abstract_application;
import Type.Abstract_board;
import Application.Service_provider;
import Application.SpectraNode_console;
import Support.Heartbeat_task;
import Domain.IO_pins;

export namespace Application {
    class Hello_AstraLEO : public Abstract::Abstract_application {
        Abstract::Abstract_UART &use_UART;
        Abstract::Abstract_IO_pin &use_LED0;
        Abstract::Abstract_IO_pin &use_LED1;
        SpectraNode_console the_console{use_UART};
        Support::Heartbeat_task the_heartbeat_task{use_LED0, use_LED1};
        Service_provider the_service_provider;

    public:
        explicit Hello_AstraLEO(Abstract::Abstract_board &board)
            : use_UART(board.get_UART()),
              use_LED0(board.get_pin(Domain::Pin_LED0)),
              use_LED1(board.get_pin(Domain::Pin_LED1)),
              the_service_provider(board, the_console.get_console()) {
            the_console.attach_router(the_service_provider);
        }

        void initialize() override;

        void run() override;
    };
} // Application
