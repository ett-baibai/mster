#include "paintWidget.h"
#include <complex>

paintWidget::paintWidget(QWidget *parent) : QWidget(parent),
    m_constWindowWidth(1900),
    m_constWindowHeight(1000)
{
    this->setWindowTitle("paint");

    m_chart = new QChart();
    m_chartView = new QChartView(m_chart,this);
    m_line = new QSplineSeries;
    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
    m_dataQueue.clear();

    mSetCanvas();
    mDrawCoordinateAxes();
    mInitImg();

    m_addPointTimer = new QTimer;
    QObject::connect(m_addPointTimer, SIGNAL(timeout()), this, SLOT(on_TimerOutToDraw()));
    m_addPointTimer->start(1);
}

paintWidget::~paintWidget()
{
    if(m_addPointTimer->isActive())
        m_addPointTimer->stop();
    delete m_addPointTimer;

    delete m_line;
    delete m_axisX;
    delete m_axisY;
    delete m_chart;
    delete m_chartView;
}

void paintWidget::mSetCanvas()
{
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    move(3, 3);

    m_chart = m_chartView->chart();//m_chart: pen
    //m_chartView->setRubberBand(QChartView::NoRubberBand);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->resize(m_constWindowWidth - 10, m_constWindowHeight - 10);
    m_chartView->setContentsMargins(0,0,0,0);
    m_chart->legend()->hide();
    m_chartView->show();
}

void paintWidget::mDrawCoordinateAxes()
{
    //set axis x
    m_axisX->setRange(0, 100);
    m_axisX->setLabelFormat("%d"); //format of value of axis x
    m_axisX->setGridLineVisible(true);
    m_axisX->setTickCount(10);   //main scale
    m_axisX->setMinorTickCount(5);//mini scale
    m_axisX->setTitleText("t");//title
    m_chart->addAxis(m_axisX, Qt::AlignBottom);//show axis

    //set axis y
    m_axisY->setRange(-10, 10);
    m_axisY->setLabelFormat("%d");
    m_axisY->setGridLineVisible(true);
    m_axisY->setTickCount(10);
    //m_axisY->setMinorTickCount(5);
    m_axisY->setTitleText("v");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
}

void paintWidget::mInitImg()
{
    m_chart->addSeries(m_line);//set line to chart
    m_line->attachAxis(m_axisX);//bind datas with axis, this code must be next to "addSeries"
    m_line->attachAxis(m_axisY);
    QPen splinePen;
    splinePen.setColor(Qt::red);
    splinePen.setWidth(2);
    m_line->setPen(splinePen);
}

void paintWidget::on_PaintPoint(unsigned int data)
{
    m_dataQueue.enqueue(data);
}

void paintWidget::on_TimerOutToDraw()
{
    int queueSize = m_dataQueue.size();
    if(queueSize == 0)return;

    static unsigned int axisX = 0;
    QVector<QPointF> pointData;
    pointData = m_line->pointsVector();
    int dataCount = pointData.size();
    if(dataCount < 100)
    {
        pointData.append(QPointF(axisX, sin(m_dataQueue.dequeue()) * 10));
        axisX++;
    }
    else
    {
        pointData.removeFirst();
        for(int i = 0; i < dataCount - 1; i++)
        {
            pointData[i].rx() -= 1;
        }
        pointData.append(QPointF(99, sin(m_dataQueue.dequeue()) * 10));
    }
    m_line->replace(pointData);
}
