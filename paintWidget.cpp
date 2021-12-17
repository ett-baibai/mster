#include "paintWidget.h"

paintWidget::paintWidget(QWidget *parent) : QWidget(parent),
    m_constXAxisPointNum(50),
    m_constYAxisPointNum(20),
    m_constWindowWidth(1900),
    m_constWindowHeight(1000)
{
    this->setWindowTitle("paint");
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
    m_testEnqueueTimer->stop();
    m_dequeueTimer->stop();
    delete m_testEnqueueTimer;
    delete m_dequeueTimer;
    delete m_painter;
}

void paintWidget::mSetCanvas()
{
    resize(m_constWindowWidth, m_constWindowHeight); //reset window
    move(0,0);
    m_image = QImage(m_constWindowWidth, m_constWindowHeight, QImage::Format_RGB32);
    QColor backColor = qRgb(255, 255, 255);
    m_image.fill(backColor);

    m_painter = new QPainter(&m_image);
    m_painter->setRenderHint(QPainter::Antialiasing, true);
}

void paintWidget::mDrawCoordinateAxes()
{
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

void paintWidget::on_GetPointData(unsigned char array[2048])
{
    mSetCanvas();
    mDrawCoordinateAxes();
    mResetAxis(0, 2048, 0, 255);

    for(int i = 0; i <= 100; i++)
    {
        //mDrawPoint(i + 1,array[i]);
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
        mDrawPoint(m_paintIndex, m_pointDataQueue[i]);
        m_paintIndex = (m_paintIndex + 1 ) % 2048;
    }
}
