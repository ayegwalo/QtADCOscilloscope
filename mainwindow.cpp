
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Display/CWaveformWidget.h"

#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    // Refresh display approximately 30 times per second.
    m_timer->setInterval(33);

    connect(m_timer, &QTimer::timeout,
            this, &MainWindow::updateWaveforms);

    // START button
    connect(ui->btnStart, &QPushButton::clicked,
            this, [this]()
            {
                if (!m_timer->isActive())
                {
                    updateWaveforms();
                    m_timer->start();

                    ui->statusbar->showMessage(
                        "Oscilloscope running - ADC Plugin"
                        );
                }
            });

    // STOP button
    connect(ui->btnStop, &QPushButton::clicked,
            this, [this]()
            {
                m_timer->stop();

                ui->statusbar->showMessage(
                    "Oscilloscope stopped"
                    );
            });
}


// Read ADC samples and update all four waveform displays.
void MainWindow::updateWaveforms()
{
    const int numberOfSamples = 500;

    // Channel controls
    QComboBox *signalControls[4] = {
        ui->signalTypeCH1,
        ui->signalTypeCH2,
        ui->signalTypeCH3,
        ui->signalTypeCH4
    };

    QSpinBox *resolutionControls[4] = {
        ui->scaleCH1,
        ui->scaleCH2,
        ui->scaleCH3,
        ui->scaleCH4
    };

    QSpinBox *intervalControls[4] = {
        ui->intervalCH1,
        ui->intervalCH2,
        ui->intervalCH3,
        ui->intervalCH4
    };

    // Process each ADC channel independently.
    for (int channel = 0; channel < 4; ++channel)
    {
        const int channelNumber = channel + 1;

        const int signalType =
            signalControls[channel]->currentIndex();

        const int resolution =
            resolutionControls[channel]->value();

        const int interval =
            intervalControls[channel]->value();

        // Configure the ADC plugin.
        m_adcPlugin.SetSignal(channelNumber, signalType);

        m_adcPlugin.SetScale(channelNumber, resolution);

        m_adcPlugin.SetInterval(channelNumber, interval);

        // Read digital ADC samples.
        int adcData[numberOfSamples];

        const int samplesRead =
            m_adcPlugin.ReadChannel(
                channelNumber,
                adcData,
                numberOfSamples
                );

        if (samplesRead <= 0)
            continue;

        // Convert digital ADC values to display values.
        QVector<double> samples;
        samples.reserve(samplesRead);

        const int maxADCValue =
            (1 << resolution) - 1;

        for (int i = 0; i < samplesRead; ++i)
        {
            const double normalizedValue =
                (2.0 * adcData[i] / maxADCValue) - 1.0;

            samples.append(normalizedValue);
        }

        // Update the corresponding waveform.
        ui->waveformWidget->setChannelData(
            channel,
            samples
            );
    }
}


// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}
