#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QPaintEvent>
#include <QMainWindow>

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
    void on_PaintPoint(unsigned char array[2048]);

protected:
     void paintEvent(QPaintEvent *);


private:
    QImage m_image;
    QPainter *m_painter;

    int m_startX;//x����ʼ��
    int m_startY;//y����ʼ��
    int m_chartWidth;
    int m_chartHeight;

    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_kx; // x���ϵ��
    double m_ky;

    const int m_constXAxisPointNum;
    const int m_constYAxisPointNum;
    const int m_constWindowWidth;
    const int m_constWindowHeight;

};

#endif // PAINTWIDGET_H
