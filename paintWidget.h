#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QtCharts/QChart>
#include <QtCharts\QChartView>
#include <QPointF>
#include <QList>
#include <QWidget>
#include <QGraphicsItem>
#include <QSplineSeries>
#include <QValueAxis>
#include <QMainWindow>
#include <QTimer>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;                 //just introduce these two classes rather than their head file
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE //it's necessary to add this code for using QT chart

class paintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit paintWidget(QWidget *parent = nullptr);
    ~paintWidget();

    void mSetCanvas();
    void mSetAxes();
    void mSetGraphStyle(QList<QPointF> &data);

public slots:
    void on_GetPointData(unsigned char array[2048]);
    void on_TimerOutToEnqueue();
    void on_TimerOutToPaint();

private:
    QChart * m_chart;   //pen
    QChartView *m_chartView;   //canvas
    QList<QSplineSeries *> m_serieslist;
    QSplineSeries *m_series;

    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    const int m_constWindowWidth;
    const int m_constWindowHeight;
    const int mk_axisXRange;
    const int mk_axisYRange;













    QImage m_image;
    QPainter *m_painter;

    int m_startX;
    int m_startY;
    int m_chartWidth;
    int m_chartHeight;

    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_kx;
    double m_ky;



    QVector<unsigned char> m_pointDataQueue;
    int m_enqueueIndex;
    int m_DequeueNum;
    unsigned int m_lastXaxisStart;
    unsigned int m_paintIndex;

    QTimer *m_testEnqueueTimer;
    QTimer *m_dequeueTimer;

};

#endif // PAINTWIDGET_H
