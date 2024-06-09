#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>

class Widget: public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private Q_SLOTS:

private:


};



#endif // WIDGET_H
