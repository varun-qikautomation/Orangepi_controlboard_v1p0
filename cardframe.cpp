#include "cardframe.h"

CardFrame::CardFrame(QWidget *parent) : QFrame(parent)
{
    // Add shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setOffset(3,3);
    shadow->setColor(QColor(0,0,0,100));
    setGraphicsEffect(shadow);

    // Card style
    setStyleSheet(
        "QFrame { "
        "background-color: white; "
        "border-radius: 10px; "
        "border: none; "
        "outline: none;"       // remove default focus/hover blue
        "}"
        );
}

// Hover enter
void CardFrame::enterEvent(QEnterEvent *event)
{
    QFrame::enterEvent(event);

    QGraphicsDropShadowEffect *shadow = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect());
    if(shadow){
        shadow->setBlurRadius(25);  // increase shadow for pop-up
        shadow->setOffset(5,5);
    }

    // Only shadow changes; no border or color change
}

// Hover leave
void CardFrame::leaveEvent(QEvent *event)
{
    QFrame::leaveEvent(event);

    QGraphicsDropShadowEffect *shadow = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect());
    if(shadow){
        shadow->setBlurRadius(15);  // original shadow
        shadow->setOffset(3,3);
    }

    // No border change
}
