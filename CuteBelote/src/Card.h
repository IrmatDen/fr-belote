#ifndef CUTE_BELOTE_CARD_H
#define CUTE_BELOTE_CARD_H

#include <QtGui/QGraphicsPixmapItem>

class Card : public QGraphicsPixmapItem
{
public:
    Card(QString cardName, QGraphicsItem *parent = nullptr);

    void SetActive(bool active);

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QString                 mCardName;
    QGraphicsPixmapItem *   mHoveredGlowItem;
    bool                    mIsActive;
};

#endif
