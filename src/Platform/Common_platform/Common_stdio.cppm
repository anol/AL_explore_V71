//
// Created by anolsen on 05.09.2019.
//

export module Platform.Common_stdio;
import Type.Abstract_UART;



export namespace Platform {
    class Common_stdio {
        Abstract::Abstract_UART *optional_UART;

    public:
        explicit Common_stdio(Abstract::Abstract_UART *UART) : optional_UART(UART) {
        }

        void initialize() const;
    };
}
