#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "sprite.h"
#include "jugador.h"

class Enemigos : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Enemigos(QGraphicsScene* escena, Jugador* jugador);  // escena para agregar sprite
    void iniciarCaida();
    void actualizarPosicion();

private:
    Sprite* sprite;
    qreal posicionX;
    qreal posicionY;
    qreal velocidadY;
    qreal gravedad;

    QTimer* temporizador;
    Jugador* jugador;
};

#endif // ENEMIGOS_H
