#ifndef CARDFRAME_H
#define CARDFRAME_H
#pragma once
#include <QMouseEvent>
#include <QFrame>
#include <QEnterEvent>
#include <QGraphicsDropShadowEffect>

class CardFrame : public QFrame
{
    Q_OBJECT
public:
    explicit CardFrame(QWidget *parent = nullptr);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked();
        QFrame::mousePressEvent(event);
    }
    void enterEvent(QEnterEvent *event) override;  // Qt6 requires QEnterEvent*
    void leaveEvent(QEvent *event) override;
};

#endif // CARDFRAME_H
