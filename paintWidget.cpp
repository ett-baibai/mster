#include "paintWidget.h"
#include <complex>

paintWidget::paintWidget(QWidget *parent) : QWidget(parent),
    m_constXAxisPointNum(50),
    m_constYAxisPointNum(20),
    m_constWindowWidth(1900),
    m_constWindowHeight(1000)
{
    this->setWindowTitle("paint");

    m_chart = new QChart();
    m_chartView = new QChartView(m_chart,this);//画布
    m_line = new QSplineSeries;


    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;

    mSetCanvas();
    mDrawCoordinateAxes();

    m_addPointTimer = new QTimer;
    QObject::connect(m_addPointTimer, SIGNAL(timeout()),
            this, SLOT(on_TimerOutToAddPoint()));
    /*
    m_startX = 100;
    m_startY = m_constWindowHeight - 100;

    //the width and height of axis
    m_chartWidth = m_constWindowWidth - m_startX - 50;
    m_chartHeight = m_constWindowHeight - m_startX - 50;

    m_xMin = 0.0;
    m_xMax = 0.0;
    m_yMin = 0.0;
    m_yMax = 0.0;
    m_kx = 0.0;
    m_ky = 0.0;
    */
}

paintWidget::~paintWidget()
{
    if(m_addPointTimer->isActive()) m_addPointTimer->stop();
    delete m_addPointTimer;
    delete m_line;
    delete m_axisX;
    delete m_axisY;
    delete m_chart;
    delete m_chartView;
    //delete m_painter;
}

void paintWidget::mSetCanvas()
{
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    move(3, 3);

    m_chart = m_chartView->chart();//画笔
    //m_chartView->setRubberBand(QChartView::NoRubberBand);  //矩形缩放
    m_chartView->setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    m_chartView->resize(m_constWindowWidth - 10, m_constWindowHeight - 10);   //画布大小
    m_chartView->setContentsMargins(0,0,0,0);
    m_chart->legend()->hide();
    m_chartView->show(); //显示

    /*
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    m_image = QImage(m_constWindowWidth, m_constWindowHeight, QImage::Format_RGB32);
    QColor backColor = qRgb(255, 255, 255);
    m_image.fill(backColor);

    m_painter = new QPainter(&m_image);
    m_painter->setRenderHint(QPainter::Antialiasing, true);
    */

}

void paintWidget::mDrawCoordinateAxes()
{
    //设置x坐标轴
    m_axisX->setRange(0, 100);  //范围
    m_axisX->setLabelFormat("%d"); //图例的格式  %d为十进制显示
    m_axisX->setGridLineVisible(true);//网格
    m_axisX->setTickCount(10);   //主要刻度
    m_axisX->setMinorTickCount(5);//小刻度
    m_axisX->setTitleText("time/(s)");//标题

    //设置y坐标轴
    m_axisY->setRange(-10, 10);
    m_axisY->setLabelFormat("%d");
    m_axisY->setGridLineVisible(true);
    m_axisY->setTickCount(10);//轴上有多少个标记数目
    //m_axisY->setMinorTickCount(5);//主要刻度之间有多少网格线
    m_axisY->setTitleText("altitude/(%)");

    m_chart->addAxis(m_axisX, Qt::AlignBottom);//将坐标轴加到chart上，居下
    m_chart->addAxis(m_axisY, Qt::AlignLeft);//居左
    /*
    // x,y axis
    QPointF xStartPoint(m_startX, m_startY);
    QPointF xEndPoint(m_chartWidth + m_startX, m_startY);
    m_painter->drawLine(xStartPoint, xEndPoint);
    m_painter->drawText(m_startX + m_chartWidth + 10, m_startY + 35, QString("index"));

    QPointF yStartPoint(m_startX, m_startY - m_chartHeight);
    QPointF yEndPoint(m_startX, m_startY);
    m_painter->drawLine(yStartPoint, yEndPoint);
    m_painter->drawText(m_startX - 45, m_startY - m_chartHeight - 20, QString("value"));

    //painting grand
    QPen penDotLine;
    penDotLine.setStyle(Qt::DotLine);
    m_painter->setPen(penDotLine);
    for (int i = 0; i < m_constYAxisPointNum; ++i)
    {
        //perpendicular
        m_painter->drawLine(m_startX + (i+1)* m_chartWidth/m_constYAxisPointNum, m_startY,
                         m_startX + (i+1)* m_chartWidth/m_constYAxisPointNum, m_startY - m_chartHeight);
    }
    for (int i = 0; i < m_constXAxisPointNum; ++i)
    {
        //horizontal
        m_painter->drawLine(m_startX, m_startY-(i+1)*m_chartHeight/m_constXAxisPointNum,
                         m_startX + m_chartWidth, m_startY-(i+1)*m_chartHeight/m_constXAxisPointNum);
    }
    */
}

void paintWidget::mSetAxisSpace()
{
    //painting scale of x axis
    m_painter->drawText(m_startX + 3, m_startY + 12, QString::number(0));//the first scale
    for (int i = 0; i < m_constXAxisPointNum - 1; ++i)
    {
        m_painter->drawText(m_startX + (i+0.9) * m_chartWidth / m_constXAxisPointNum, m_startY + 12,
                         QString::number((int)(m_xMin + (i+1) * ((m_xMax-m_xMin)/m_constXAxisPointNum))));
    }
    m_painter->drawText(m_startX + (m_constXAxisPointNum - 1 + 0.8) * m_chartWidth / m_constXAxisPointNum, m_startY + 12,
                        QString::number(m_xMax));//the last scale

    //painting scale of x axis
    m_painter->drawText(m_startX - 45, m_startY - 3, QString::number(m_yMin));//the first scale
    for (int i = 0; i < m_constYAxisPointNum - 1; ++i)
    {
        m_painter->drawText(m_startX - 45, m_startY - (i+0.85) * m_chartHeight/m_constYAxisPointNum,
                         QString::number(m_yMin + (i+1) * ((m_yMax-m_yMin)/m_constYAxisPointNum)));
    }
    m_painter->drawText(m_startX - 45, m_startY - (m_constYAxisPointNum - 1 + 0.85) * m_chartHeight/m_constYAxisPointNum,
                     QString::number(m_yMax));//the last scale
}

void paintWidget::mResetAxis(double xMin, double xMax, double yMin, double yMax)
{
    //onRefresh();
    m_xMin = xMin;
    m_xMax = xMax;
    m_yMin = yMin;
    m_yMax = yMax;

    mSetAxisSpace();

    m_kx = (double)(m_chartWidth / (xMax-xMin)); // scale factor of x axis
    m_ky = (double)(m_chartHeight / (yMax-yMin));  // scale factor of y axis
}

void paintWidget::mDrawPoint(double x, double y)
{
    QPen penPoint;
    penPoint.setColor(Qt::red);
    penPoint.setWidth(3);

    double dXStart = m_startX + m_kx * (x - m_xMin);
    m_painter->setPen(penPoint);
    m_painter->drawPoint(dXStart, m_startY - (y - m_yMin) * m_ky);
}

void paintWidget::mInitImg()
{
    //QPointF initPoint(0,0);
    //m_line->append(initPoint);
    m_chart->addSeries(m_line);//将曲线增入chart中
    m_line->attachAxis(m_axisX);//将坐标轴与数据绑定，此步需在addSeries之后
    m_line->attachAxis(m_axisY);
    QPen splinePen;
    splinePen.setColor(Qt::red);
    splinePen.setWidth(2);
    m_line->setPen(splinePen);
}

void paintWidget::mDrawLine(unsigned int data)
{
    //static unsigned int index = 0;
    static unsigned int xxx = 0;
    QVector<QPointF> pointData = m_line->pointsVector();
    int dataCount = pointData.size();
    if(dataCount < 100)
    {
        //pointData.append(QPointF(xxx, sin(data) * 10));
        pointData.append(QPointF(xxx, sin(data) * 10));
        xxx++;
    }
    else
    {
        pointData.removeFirst();
        for(int i = 0; i < dataCount - 1; i++)
        {
            pointData[i].rx() -= 1;
        }
        pointData.append(QPointF(99, sin(data) * 10));
    }

/*
    qDebug()<<"vector "<<index<<": ";
    qDebug()<<"count: "<<pointData.size();
    for(int i = 0; i < pointData.size(); i++)
        qDebug()<<pointData[i];
    qDebug()<<"";
    index++;
    */
    m_line->replace(pointData);
/*
    m_chart->addSeries(m_line);//将曲线增入chart中
    m_line->attachAxis(m_axisX);//将坐标轴与数据绑定，此步需在addSeries之后
    m_line->attachAxis(m_axisY);
*/

    //QPen splinePen;
    //splinePen.setColor(Qt::red);
    //splinePen.setWidth(2);
    //series->setPen(splinePen);

}

void paintWidget::onRefresh()
{
    m_painter->fillRect(0, 0, m_constWindowWidth, m_constWindowHeight, Qt::white);
    mSetCanvas();
    mDrawCoordinateAxes();
    mResetAxis(m_xMin, m_xMax, m_yMin, m_yMax);
    update();
}

void paintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}

void paintWidget::on_PaintPoint(unsigned int array[100])
{
    mInitImg();
    m_addPointTimer->start(100);
    /*
    for(int i = 0; i < 10; i++)
    {
        mDrawLine(array[i]);
        Sleep(500);
    }
*/
/*
    mResetAxis(0, 2048, 0, 255);

    for(int i = 0; i <= 2048; i++)
    {
        mDrawPoint(i + 1,array[i]);
    }
*/
}

void paintWidget::on_TimerOutToAddPoint()
{
    static unsigned int xxx = 0;
    mDrawLine(xxx);
    xxx++;
}

