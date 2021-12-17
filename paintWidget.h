#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QPaintEvent>
#include <QMainWindow>
#include <QVector>
#include <QTimer>

class paintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit paintWidget(QWidget *parent = nullptr);
    ~paintWidget();

    void mSetCanvas();
    void mDrawCoordinateAxes();
    void mSetAxisSpace();
    void mResetAxis(double xMin, double xMax, double yMin, double yMax);
    void mDrawPoint(double x, double y);

public slots:
    void onRefresh();
    void on_GetPointData(unsigned char array[2048]);
    void on_TimerOutToEnqueue();
    void on_TimerOutToPaint();

protected:
     void paintEvent(QPaintEvent *);


private:
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

    QVector<unsigned char> m_pointDataQueue;
    int m_enqueueIndex;
    int m_DequeueNum;
    unsigned int m_lastXaxisStart;
    unsigned int m_paintIndex;

    QTimer *m_testEnqueueTimer;
    QTimer *m_dequeueTimer;

};

#endif // PAINTWIDGET_H
