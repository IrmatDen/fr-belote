#include "Card.h"

#include <QtGui/QGraphicsScene>

Card::Card(QString cardName, QGraphicsItem *parent)
    :   QGraphicsPixmapItem(QString("./data/") + cardName, parent),
        mCardName(cardName)
{
    mHoveredGlowItem = new QGraphicsPixmapItem(QString("./data/hovered_glow.png"), this);
    mHoveredGlowItem->setOffset(-3, -3);
    mHoveredGlowItem->setVisible(false);
    setAcceptHoverEvents(true);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mHoveredGlowItem->setVisible(true);
    mHoveredGlowItem->setPos(pos());

    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mHoveredGlowItem->setVisible(false);

    QGraphicsPixmapItem::hoverLeaveEvent(event);
}
