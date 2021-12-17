#include "paintWidget.h"
#include <QPen>
#include <QLegend>
#include <QLegendMarker>
#include <QBrush>
#include <QTime>
#include <QDebug>

paintWidget::paintWidget(QWidget *parent) : QWidget(parent),
    m_constWindowWidth(1900),
    m_constWindowHeight(1000),
    mk_axisXRange(100),
    mk_axisYRange(255)
{
    this->setWindowTitle("graph");
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    move(0,0);

    m_chart = new QChart();
    m_chartView = new QChartView(m_chart,this);
    m_chart = m_chartView->chart();
    m_series = new QSplineSeries(this);

    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;







    m_startX = 100;
    m_startY = m_constWindowHeight - 100;

    //set width and height of axis
    m_chartWidth = m_constWindowWidth - m_startX - 50;
    m_chartHeight = m_constWindowHeight - m_startX - 50;

    m_xMin = 0.0;
    m_xMax = 0.0;
    m_yMin = 0.0;
    m_yMax = 0.0;
    m_kx = 0.0;
    m_ky = 0.0;

    m_pointDataQueue.clear();
    m_enqueueIndex = 0;
    m_DequeueNum = 0;
    m_lastXaxisStart = 0;
    m_paintIndex = 0;

    m_testEnqueueTimer = new QTimer;
    QObject::connect(m_testEnqueueTimer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToEnqueue()));

    m_dequeueTimer = new QTimer;
    QObject::connect(m_dequeueTimer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToPaint()));
}

paintWidget::~paintWidget()
{
    delete m_axisX;
    delete m_axisY;

    delete m_series;
    delete m_chartView;
    delete m_chart;

    m_testEnqueueTimer->stop();
    m_dequeueTimer->stop();
    delete m_testEnqueueTimer;
    delete m_dequeueTimer;
}

void paintWidget::mSetCanvas()
{
    m_chartView->setRubberBand(QChartView::NoRubberBand);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->resize(m_constWindowWidth, m_constWindowHeight);
    m_chartView->setContentsMargins(0,0,0,0);
}

void paintWidget::mSetAxes()
{
    m_axisX->setRange(0, mk_axisXRange);
    m_axisX->setLabelFormat("%d");
    m_axisX->setGridLineVisible(true);
    m_axisX->setTickCount(10);
    m_axisX->setTitleText("t");

    m_axisY->setRange(0, mk_axisYRange);
    m_axisY->setLabelFormat("%d");
    m_axisY->setGridLineVisible(true);
    m_axisY->setTickCount(128);
    //m_axisY->setMinorTickCount(5);
    m_axisY->setTitleText("value");

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
}

void paintWidget::mSetGraphStyle(QList<QPointF> &data)
{
    m_serieslist.append(m_series);
    m_series->append(data);
    m_chart->addSeries(m_series);
    m_axisX->setRange(0, m_series->count());

    QPen splinePen;
    splinePen.setBrush(Qt::red);
    splinePen.setColor(Qt::red);
    m_series->setPen(splinePen);
}

void paintWidget::on_GetPointData(unsigned char array[2048])
{
    mSetCanvas();
    mSetAxes();

    for(int i = 0; i <= 100; i++)
    {
        m_pointDataQueue.append(array[i]);
    }

    m_testEnqueueTimer->start(1000);
    m_dequeueTimer->start(500);
}

void paintWidget::on_TimerOutToEnqueue()
{
    int num = m_pointDataQueue.count();
    qDebug()<<"en size: "<<num;
    if(num >= 2048)
    {
        qDebug()<<"de first: "<<m_pointDataQueue[0];
        m_pointDataQueue.remove(0);
    }
    m_pointDataQueue.append((unsigned char)((m_enqueueIndex++) % 256));
}

void paintWidget::on_TimerOutToPaint()
{
    int num = m_pointDataQueue.count();
    qDebug()<<"de size: "<<num;
    if(num <= 0)return;
    for(int i = 0; i < num; i++)
    {
        qDebug()<<"dequeue: "<<m_pointDataQueue[i];
        //mDrawPoint(m_paintIndex, m_pointDataQueue[i]);
        m_paintIndex = (m_paintIndex + 1 ) % 2048;
    }
}
