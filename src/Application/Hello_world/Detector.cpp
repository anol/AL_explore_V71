module;

module Application.Detector;

using namespace IDE3380;

namespace Application
{
    void Detector::initialize_readout()
    {
        the_IDE3380.initialize(
            this,
            [](void* user)
            {
                if (user)
                {
                    static_cast<Detector*>(user)->wakeup_from_sleep();
                }
            },
            [](void* user, const TXD_data_t& data)
            {
                if (user)
                {
                    static_cast<Detector*>(user)->the_histogram.update_histogram(data);
                }
            });
    }
}
