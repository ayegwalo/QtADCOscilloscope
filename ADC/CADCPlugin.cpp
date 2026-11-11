
#include "CADCPlugin.h"

#include <cmath>
#include <algorithm>

namespace
{
constexpr double PI = 3.14159265358979323846;
constexpr double SIGNAL_FREQUENCY = 100.0; // 100 Hz
}

CADCPlugin::CADCPlugin()
{
    // Initialise four independent ADC channels.
    for (int i = 0; i < 4; ++i)
    {
        m_channels[i].resolution = 10;
        m_channels[i].interval = 100;
        m_channels[i].signalType = 0;
        m_channels[i].sampleCounter = 0;
    }
}


// Set ADC resolution.
void CADCPlugin::SetScale(int ch_num, int resolution)
{
    if (ch_num < 1 || ch_num > 4)
        return;

    if (resolution < 2 || resolution > 16)
        return;

    m_channels[ch_num - 1].resolution = resolution;
}


// Set sampling interval in microseconds.
void CADCPlugin::SetInterval(int ch_num, int interval)
{
    if (ch_num < 1 || ch_num > 4)
        return;

    if (interval < 1)
        return;

    m_channels[ch_num - 1].interval = interval;
}


// Select signal type.
// 0 = Null, 1 = Sine, 2 = Square, 3 = Sawtooth.
void CADCPlugin::SetSignal(int ch_num, int type)
{
    if (ch_num < 1 || ch_num > 4)
        return;

    if (type < 0 || type > 3)
        return;

    m_channels[ch_num - 1].signalType = type;
}


// Generate the selected analogue signal.
double CADCPlugin::generateSignal(int type, double time)
{
    const double phase =
        2.0 * PI * SIGNAL_FREQUENCY * time;

    switch (type)
    {
    case 0:
        return 0.0;

    case 1:
        return std::sin(phase);

    case 2:
        return std::sin(phase) >= 0.0 ? 1.0 : -1.0;

    case 3:
    {
        const double cycle = phase / (2.0 * PI);
        return 2.0 * (cycle - std::floor(cycle + 0.5));
    }

    default:
        return 0.0;
    }
}


// Read samples from a selected ADC channel.
int CADCPlugin::ReadChannel(
    int ch_num,
    int* pData,
    std::size_t max_data)
{
    if (ch_num < 1 || ch_num > 4)
        return -1;

    if (pData == nullptr)
        return -1;

    if (max_data == 0)
        return 0;

    Channel &channel = m_channels[ch_num - 1];

    const int resolution = channel.resolution;

    const int maxADCValue =
        (1 << resolution) - 1;

    const double sampleInterval =
        channel.interval * 1.0e-6;

    const std::size_t numberOfSamples =
        std::min(max_data,
                 static_cast<std::size_t>(10000));

    for (std::size_t i = 0; i < numberOfSamples; ++i)
    {
        const double time =
            channel.sampleCounter * sampleInterval;

        // Generate analogue signal.
        const double analogueValue =
            generateSignal(channel.signalType, time);

        // Convert [-1, +1] to [0, 1].
        const double normalized =
            (analogueValue + 1.0) / 2.0;

        // Simulate ADC digital conversion.
        pData[i] = static_cast<int>(
            std::round(normalized * maxADCValue)
            );

        channel.sampleCounter++;
    }

    return static_cast<int>(numberOfSamples);
}
