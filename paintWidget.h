#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QtCharts/QChart>
#include <QtCharts\QChartView>
#include <QPointF>
#include <QList>
#include <QTimer>
#include <QWidget>
#include <QGraphicsItem>
//#include <QSplineSeries>
#include <QLineSeries>
//#include <QScatterSeries>
#include <QValueAxis>
#include <QQueue>

//just include double classes, not head file
QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE   //need include this code for using chart

class paintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit paintWidget(QWidget *parent = nullptr);
    ~paintWidget();

    void mSetCanvas();
    void mDrawCoordinateAxes();
    void mInitImg();

public slots:
    void on_TimerOutToDraw();
    void on_PaintPoint(unsigned int data);

private:
    const int m_constWindowWidth;
    const int m_constWindowHeight;
    static const int m_sk_maxChannleNum = 4;
    static const int m_sk_channleNum = 3;
    QChart * m_chart[m_sk_channleNum];
    QChartView *m_chartView[m_sk_channleNum];
    QLineSeries *m_cruveChannel[m_sk_channleNum];
    QValueAxis *m_axisX[m_sk_channleNum];
    QValueAxis *m_axisY[m_sk_channleNum];
    int  m_minAxisX[m_sk_channleNum];
    int  m_maxAxisX[m_sk_channleNum];
    unsigned int m_axisXIndex[m_sk_channleNum];
    QTimer *m_addPointTimer;
    QQueue<double> m_dataQueue[m_sk_channleNum];
};

#endif // PAINTWIDGET_H
