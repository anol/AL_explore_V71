module;

export module Platform.SamV71_platform;

namespace SamV71 {
    export class SamV71_platform {
    public:
        static void initialize();

    private:
        static void enable_cache();

        static void initialize_matrix();
    };
} // SamV71
