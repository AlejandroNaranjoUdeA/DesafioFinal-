#include "enemigos.h"
#include <QRandomGenerator>

Enemigos::Enemigos(QGraphicsScene* escena, Jugador* jugador): velocidadY(0), gravedad(0.5), jugador(jugador)
{
    sprite = new Sprite(":/imagenes/fuego sin fondo.png", 42, 60);
    sprite->setFila(0);
    sprite->iniciarAnimacion();

    posicionX = QRandomGenerator::global()->bounded(0, 1024);
    posicionY = 0;
    sprite->setPos(posicionX, posicionY);

    escena->addItem(sprite);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Enemigos::actualizarPosicion);
}

void Enemigos::iniciarCaida() {
    temporizador->start(16);
}

void Enemigos::actualizarPosicion() {
    velocidadY += gravedad;
    posicionY += velocidadY;
    sprite->setY(posicionY);


    // Detectar colisión con el jugador
    if (sprite->collidesWithItem(jugador->getItem())) {
        jugador->perderVida(); // NUEVO
        temporizador->stop();
        sprite->scene()->removeItem(sprite);
        delete sprite;
        delete this;
        return;
    }

    if (posicionY > 650) {
        temporizador->stop();
        sprite->scene()->removeItem(sprite);
        delete sprite;
        delete this;
    }
}

