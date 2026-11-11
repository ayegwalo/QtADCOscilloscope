#include "CWaveformWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPainterPath>
#include <QtMath>

CWaveformWidget::CWaveformWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(500, 250);

    // Temporary test signals so we can see the display
    // before connecting the ADC plugin.

    const int numberOfSamples = 500;

    for (int i = 0; i < numberOfSamples; ++i)
    {
        double t = static_cast<double>(i) / numberOfSamples;

        // CH1: Sine wave
        m_channelData[0].append(qSin(2.0 * M_PI * 3.0 * t));

        // CH2: Square wave
        m_channelData[1].append(
            qSin(2.0 * 6.283185307179586 * 2.0 * t) >= 0.0 ? 0.7 : -0.7
            );

        // CH3: Sawtooth wave
        m_channelData[2].append(2.0 * (2.0 * t - qFloor(2.0 * t + 0.5)));

        // CH4: Null signal
        m_channelData[3].append(0.0);
    }
}

void CWaveformWidget::setChannelData(
    int channel,
    const QVector<double> &samples)
{
    if (channel < 0 || channel >= 4)
        return;

    m_channelData[channel] = samples;
    update();
}

void CWaveformWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    painter.fillRect(rect(), QColor("#101820"));

    drawGrid(painter);
    drawWaveforms(painter);
}

void CWaveformWidget::drawGrid(QPainter &painter)
{
    const int left = 45;
    const int right = width() - 15;
    const int top = 15;
    const int bottom = height() - 25;

    painter.setPen(QPen(QColor("#34434D"), 1));

    // Vertical grid lines
    for (int i = 0; i <= 10; ++i)
    {
        int x = left + i * (right - left) / 10;
        painter.drawLine(x, top, x, bottom);
    }

    // Horizontal grid lines
    for (int i = 0; i <= 8; ++i)
    {
        int y = top + i * (bottom - top) / 8;
        painter.drawLine(left, y, right, y);
    }

    // Centre reference line
    painter.setPen(QPen(QColor("#718096"), 1, Qt::DashLine));

    int centreY = (top + bottom) / 2;
    painter.drawLine(left, centreY, right, centreY);

    // Channel labels
    painter.setPen(Qt::white);
    painter.drawText(5, 25, "CH1");
    painter.drawText(5, 43, "CH2");
    painter.drawText(5, 61, "CH3");
    painter.drawText(5, 79, "CH4");
}

void CWaveformWidget::drawWaveforms(QPainter &painter)
{
    const QColor channelColors[4] = {
        QColor("#00FF66"),  // CH1 Green
        QColor("#3399FF"),  // CH2 Blue
        QColor("#FFD700"),  // CH3 Yellow
        QColor("#FF4444")   // CH4 Red
    };

    const int left = 45;
    const int right = width() - 15;
    const int top = 15;
    const int bottom = height() - 25;

    const double centreY = (top + bottom) / 2.0;
    const double amplitude = (bottom - top) / 2.5;

    for (int channel = 0; channel < 4; ++channel)
    {
        const QVector<double> &samples =
            m_channelData[channel];

        if (samples.size() < 2)
            continue;

        painter.setPen(
            QPen(channelColors[channel], 2)
            );

        QPainterPath path;

        for (int i = 0; i < samples.size(); ++i)
        {
            double x = left +
                       static_cast<double>(i) *
                           (right - left) /
                           (samples.size() - 1);

            double y = centreY - samples[i] * amplitude;

            if (i == 0)
                path.moveTo(x, y);
            else
                path.lineTo(x, y);
        }

        painter.drawPath(path);
    }
}