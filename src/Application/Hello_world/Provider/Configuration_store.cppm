export module Application.Configuration_store;
import Application.Default_configuration;
import Support.Configuration_repository;
import Support.Mockup_persistent_storage;

export namespace Application
{
    class Configuration_store
    {
        const Default_configuration the_default_configuration;
        MOCKUP::Mockup_persistent_storage the_storage{};
        Repository::Configuration_repository the_repository{the_default_configuration, the_storage};

    public:
        Repository::Configuration_repository& get_repository() { return the_repository; }
    };
} // Application
