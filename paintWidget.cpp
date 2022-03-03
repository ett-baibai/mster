#include "paintWidget.h"
#include <complex>

paintWidget::paintWidget(QWidget *parent) : QWidget(parent),
    m_constWindowWidth(1900),
    m_constWindowHeight(1000)
{
    this->setWindowTitle("paint");

    for(int i = 0; i < m_sk_channleNum; i++)
    {
        m_chart[i] = new QChart();
        m_chartView[i] = new QChartView(m_chart[i],this);
        m_cruveChannel[i] = new QLineSeries;
        m_axisX[i] = new QValueAxis;
        m_axisY[i] = new QValueAxis;
        m_minAxisX[i] = 0;
        m_maxAxisX[i] = 100;
        m_dataQueue[i].clear();
    }

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

    for(int i = 0; i < m_sk_channleNum; i++)
    {
        delete m_cruveChannel[i];
        delete m_axisX[i];
        delete m_axisY[i];
        delete m_chart[i];
        delete m_chartView[i];
    }
}

void paintWidget::mSetCanvas()
{
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    move(3, 3);

    int x[m_sk_maxChannleNum] = {0, (m_constWindowWidth - 10) / 2, 0, (m_constWindowWidth - 10) / 2};
    int y[m_sk_maxChannleNum] = {0, 0, (m_constWindowHeight - 10) / 2, (m_constWindowHeight - 10) / 2};
    QString title[m_sk_maxChannleNum] = {"ch0", "ch1", "ch2", "ch3"};
    //int right[] = {(m_constWindowWidth - 10) / 2, 0, (m_constWindowWidth - 10) / 2, 0};
    //int bottom[] = {(m_constWindowHeight - 10) / 2, (m_constWindowHeight - 10) / 2, 0, 0};
    for(int i = 0; i < m_sk_channleNum; i++)
    {
        m_chart[i] = m_chartView[i]->chart();
        m_chartView[i]->setRenderHint(QPainter::Antialiasing);
        m_chartView[i]->resize(m_constWindowWidth / 2 - 5, m_constWindowHeight / 2 - 5);
        //m_chartView[i]->setContentsMargins(left[i], top[i], right[i], bottom[i]);
        m_chartView[i]->move(x[i], y[i]);
        m_chart[i]->legend()->hide();
        m_chart[i]->setTitle(title[i]);
        m_chartView[i]->show();
    }
}

void paintWidget::mDrawCoordinateAxes()
{
    for(int i = 0; i < m_sk_channleNum; i++)
    {
        //set axis x
        m_axisX[i]->setRange(m_minAxisX[i], m_maxAxisX[i]);
        m_axisX[i]->setLabelFormat("%d"); //format of value of axis x
        m_axisX[i]->setGridLineVisible(true);
        m_axisX[i]->setTickCount(10);   //main scale
        m_axisX[i]->setMinorTickCount(5);//mini scale
        m_axisX[i]->setTitleText("t");//title
        m_chart[i]->addAxis(m_axisX[i], Qt::AlignBottom);//show axis

        //set axis y
        m_axisY[i]->setRange(-110, 110);
        m_axisY[i]->setLabelFormat("%d");
        m_axisY[i]->setGridLineVisible(true);
        m_axisY[i]->setTickCount(10);
        //m_axisY->setMinorTickCount(5);
        m_axisY[i]->setTitleText("v");
        m_chart[i]->addAxis(m_axisY[i], Qt::AlignLeft);
    }
}

void paintWidget::mInitImg()
{
    QPen pen[m_sk_maxChannleNum];
    //QColor color[m_sk_maxChannleNum] = {Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::black};
    QString lineName[m_sk_maxChannleNum] = {QString::fromLocal8Bit("ch1"), QString::fromLocal8Bit("ch2"), QString::fromLocal8Bit("ch3"), QString::fromLocal8Bit("ch3")};

    for(int  i = 0; i < m_sk_channleNum ; i++)
    {
        m_cruveChannel[i]->setName(lineName[i]);
        m_chart[i]->addSeries(m_cruveChannel[i]);//set line to chart
        m_cruveChannel[i]->attachAxis(m_axisX[i]);//bind datas with axis, this code must be next to "addSeries"
        m_cruveChannel[i]->attachAxis(m_axisY[i]);
        pen[i].setColor(Qt::black);
        pen[i].setBrush(Qt::black);
        pen[i].setWidth(1);
        m_cruveChannel[i]->setPen(pen[i]);
    }
}

void paintWidget::on_PaintPoint(unsigned int data)
{
    qDebug()<< "data:" << data;
    unsigned int ch = (0x03000000 & data) >> 24;
    unsigned int realData = 0x00FFFFFF & data;
    int finalData = 0;
    if(realData & 0x800000)//extend 24-bit signed negative number to 32-bit signed number
    {
        //qDebug()<< "negative";
        realData &= 0x7FFFFF;
        //finalData = (int)realData;
        //finalData = realData + 0xFF000000;
        finalData = -(int)realData;
    }
    else
    {
        finalData =(int)realData;
    }
    qDebug()<< "ch:" << ch << "finalData:" << finalData;
    m_dataQueue[ch].enqueue(finalData * 250 / 4194303);//100 times of the real voltage
}

void paintWidget::on_TimerOutToDraw()
{
    int queueSize = 0, dataCount = 0;
    QVector<QPointF> pointData[m_sk_maxChannleNum];
    for(int  i = 0; i < m_sk_channleNum ; i++)
    {
        queueSize = m_dataQueue[i].size();
        //qDebug()<<"size["<<i<<"]: "<<queueSize;
        if(queueSize == 0)continue;

        pointData[i] = m_cruveChannel[i]->pointsVector();
        dataCount = pointData[i].size();
        if(dataCount >= m_maxAxisX[i] - m_minAxisX[i])
        {
            pointData[i].removeFirst();
            m_minAxisX[i]++;
            m_maxAxisX[i]++;
            m_axisX[i]->setRange(m_minAxisX[i], m_maxAxisX[i]);
        }
        pointData[i].append(QPointF(m_axisXIndex[i], m_dataQueue[i].dequeue()));
        m_axisXIndex[i]++;
        m_cruveChannel[i]->replace(pointData[i]);
    }
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
