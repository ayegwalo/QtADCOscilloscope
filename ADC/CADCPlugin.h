#ifndef CADCPLUGIN_H
#define CADCPLUGIN_H

#include "CADCInterface.h"

class CADCPlugin : public CADCInterface
{
public:
    CADCPlugin();

    void SetScale(int ch_num, int resolution) override;

    void SetInterval(int ch_num, int interval) override;

    void SetSignal(int ch_num, int type) override;

    int ReadChannel(
        int ch_num,
        int* pData,
        std::size_t max_data
        ) override;

private:
    struct Channel
    {
        int resolution = 10;
        int interval = 100;
        int signalType = 0;
        std::size_t sampleCounter = 0;
    };

    Channel m_channels[4];

    double generateSignal(int type, double time);
};

#endif // CADCPLUGIN_H