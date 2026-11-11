#ifndef CADCINTERFACE_H
#define CADCINTERFACE_H

#include <cstddef>

class CADCInterface
{
public:

    virtual ~CADCInterface() = default;

    // Configure ADC resolution for a channel.
    virtual void SetScale(int ch_num, int resolution) = 0;

    // Configure sampling interval in microseconds.
    virtual void SetInterval(int ch_num, int interval) = 0;

    // Select the signal type for a channel.
    virtual void SetSignal(int ch_num, int type) = 0;

    // Read ADC samples from a channel.
    virtual int ReadChannel(
        int ch_num,
        int* pData,
        std::size_t max_data
        ) = 0;
};

#endif // CADCINTERFACE_H