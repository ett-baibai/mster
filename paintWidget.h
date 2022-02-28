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
#include <QSplineSeries>
#include <QValueAxis>
#include <QMainWindow>


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
    void mDrawLine(unsigned int data);
    void mSetAxisSpace();
    void mResetAxis(double xMin, double xMax, double yMin, double yMax);
    void mDrawPoint(double x, double y);


public slots:
    void on_TimerOutToAddPoint();



    void onRefresh();
    void on_PaintPoint(unsigned int array[2048]);

protected:
     void paintEvent(QPaintEvent *);


private:
    QChart * m_chart;
    QChartView *m_chartView;
    QSplineSeries *m_line;
    QValueAxis *m_axisX;             //x坐标轴
    QValueAxis *m_axisY;             //y坐标轴
    QList<QSplineSeries *> m_serieslist;   //曲线列表，splineseries为光滑曲线
    QTimer *m_addPointTimer;

























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

    const int m_constXAxisPointNum;
    const int m_constYAxisPointNum;
    const int m_constWindowWidth;
    const int m_constWindowHeight;

};

#endif // PAINTWIDGET_H
