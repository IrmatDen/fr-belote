#include "Card.h"

#include <QtGui/QGraphicsScene>

Card::Card(QString cardName, QGraphicsItem *parent)
    :   QGraphicsPixmapItem(QString("./data/") + cardName, parent),
        mCardName(cardName), mIsActive(false)
{
    mHoveredGlowItem = new QGraphicsPixmapItem(QString("./data/hovered_glow.png"), this);
    mHoveredGlowItem->setOffset(-3, -3);
    mHoveredGlowItem->setVisible(false);
    mHoveredGlowItem->setTransformationMode(Qt::SmoothTransformation);
    mHoveredGlowItem->setZValue(1);

    setTransformationMode(Qt::SmoothTransformation);
}

void Card::SetActive(bool active)
{
    setAcceptHoverEvents(active);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mHoveredGlowItem->setVisible(true);

    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mHoveredGlowItem->setVisible(false);

    QGraphicsPixmapItem::hoverLeaveEvent(event);
}
