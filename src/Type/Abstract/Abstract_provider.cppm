//
// Created by AndersEmilOlsen on 14.04.2026.
//

module;
#include <cstdint>

export module Type.Abstract_provider;

namespace Abstract
{
    /// The Request type is opaque to this interface: it is what the provider is asked to handle (the received
    /// instruction). Keeping it a template parameter avoids any dependency from Type on the caller.
    export template <class Request>
    class Abstract_provider
    {
        const uint8_t the_key;

    public:
        explicit Abstract_provider(const uint8_t key) : the_key(key)
        {
        }

        virtual ~Abstract_provider() = default;

        virtual bool on_indication(Request&) = 0;

        [[nodiscard]] uint8_t get_key() const { return the_key; }
    };
} // Abstract
