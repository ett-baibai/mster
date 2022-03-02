#include "paintWidget.h"
#include <complex>

paintWidget::paintWidget(QWidget *parent) : QWidget(parent),
    m_constWindowWidth(1900),
    m_constWindowHeight(1000),
    m_minAxisX(0),
    m_maxAxisX(100)
{
    this->setWindowTitle("paint");

    m_chart = new QChart();
    m_chartView = new QChartView(m_chart,this);
    m_smoothCurve = new QSplineSeries;
    m_brokenLine = new QLineSeries;
    m_pointDiagram = new QScatterSeries;
    m_lineList.clear();
    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
    m_dataQueue.clear();

    mSetCanvas();
    mDrawCoordinateAxes();
    mInitImg();

    m_addPointTimer = new QTimer;
    QObject::connect(m_addPointTimer, SIGNAL(timeout()), this, SLOT(on_TimerOutToDraw()));
    m_addPointTimer->start(100);
}

paintWidget::~paintWidget()
{
    if(m_addPointTimer->isActive())
        m_addPointTimer->stop();
    delete m_addPointTimer;

    delete m_smoothCurve;
    delete m_brokenLine;
    delete m_pointDiagram;
    delete m_axisX;
    delete m_axisY;
    delete m_chart;
    delete m_chartView;
}

void paintWidget::mSetCanvas()
{
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    move(3, 3);

    m_chart = m_chartView->chart();
    //m_chartView->setRubberBand(QChartView::NoRubberBand);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->resize(m_constWindowWidth - 10, m_constWindowHeight - 10);
    m_chartView->setContentsMargins(0,0,0,0);
    //m_chart->legend()->hide();
    m_chartView->show();
}

void paintWidget::mDrawCoordinateAxes()
{
    //set axis x
    m_axisX->setRange(m_minAxisX, m_maxAxisX);
    m_axisX->setLabelFormat("%d"); //format of value of axis x
    m_axisX->setGridLineVisible(true);
    m_axisX->setTickCount(10);   //main scale
    m_axisX->setMinorTickCount(5);//mini scale
    m_axisX->setTitleText("t");//title
    m_chart->addAxis(m_axisX, Qt::AlignBottom);//show axis

    //set axis y
    m_axisY->setRange(-110, 110);
    m_axisY->setLabelFormat("%d");
    m_axisY->setGridLineVisible(true);
    m_axisY->setTickCount(10);
    //m_axisY->setMinorTickCount(5);
    m_axisY->setTitleText("v");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
}

void paintWidget::mInitImg()
{
    //m_lineList.append(m_smoothCurve);
    m_lineList.append(m_brokenLine);
    //m_lineList.append(m_pointDiagram);
    QPen pen[3];
    QColor color[3] = {Qt::black, Qt::red, Qt::green};
    QString lineName[3] = {QString::fromLocal8Bit("«˙œﬂ"), QString::fromLocal8Bit("’€œﬂ"), QString::fromLocal8Bit("µ„Õº")};

    for(int  i = 0; i < m_lineList.size() ; i++)
    {
        m_lineList[i]->setName(lineName[i]);
        m_chart->addSeries(m_lineList[i]);//set line to chart
        m_lineList[i]->attachAxis(m_axisX);//bind datas with axis, this code must be next to "addSeries"
        m_lineList[i]->attachAxis(m_axisY);
        pen[i].setColor(color[i]);
        pen[i].setBrush(color[i]);
        pen[i].setWidth(1);
        m_lineList[i]->setPen(pen[i]);
    }
}

void paintWidget::on_PaintPoint(int data)
{
    m_dataQueue.enqueue(data * 250 / 4194303);//100 times of the real voltage
}

void paintWidget::on_TimerOutToDraw()
{
    int queueSize = m_dataQueue.size();
    qDebug()<<"size: "<<queueSize;
    if(queueSize == 0)return;

    QVector<QPointF> pointData;
    pointData = m_lineList[0]->pointsVector();
    int dataCount = pointData.size();
    if(dataCount >= m_maxAxisX - m_minAxisX)
    {
        pointData.removeFirst();
        m_minAxisX++;
        m_maxAxisX++;
        m_axisX->setRange(m_minAxisX, m_maxAxisX);
    }
    pointData.append(QPointF(m_axisXIndex, m_dataQueue.dequeue()));
    m_axisXIndex++;
    m_lineList[0]->replace(pointData);

/*
    static unsigned int axisX = 0;
    QVector<QPointF> pointData;
    pointData = m_lineList[0]->pointsVector();
    int dataCount = pointData.size();
    if(dataCount < m_maxAxisX)
    {
        pointData.append(QPointF(axisX, m_dataQueue.dequeue()));
        axisX++;
    }
    else
    {
        pointData.removeFirst();
        for(int i = 0; i < dataCount - 1; i++)
        {
            pointData[i].rx() -= 1;
        }
        pointData.append(QPointF(m_maxAxisX - 1, m_dataQueue.dequeue()));
    }
    m_lineList[0]->replace(pointData);
    */
}
