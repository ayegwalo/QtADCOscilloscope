#ifndef CWAVEFORMWIDGET_H
#define CWAVEFORMWIDGET_H

#include <QWidget>
#include <QVector>

class QPainter;

class CWaveformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CWaveformWidget(QWidget *parent = nullptr);

    void setChannelData(int channel,
                        const QVector<double> &samples);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> m_channelData[4];

    void drawGrid(QPainter &painter);
    void drawWaveforms(QPainter &painter);
};

#endif // CWAVEFORMWIDGET_H