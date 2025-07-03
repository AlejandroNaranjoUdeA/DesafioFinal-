#include "enemigos.h"
#include "proyectil.h"
#include <QRandomGenerator>

Enemigos::Enemigos(QGraphicsScene* escena, Jugador* jugador, TipoEnemigo tipo, QString rutaImagen, int anchoSprite, int altoSprite)
    : jugador(jugador), tipo(tipo), velocidadY(0), gravedad(0.5), vidas(3)
{
    QString imagen;
    if (tipo == FUEGO) {
        imagen = ":/imagenes/fuego sin fondo.png";
    } else {
        imagen = rutaImagen;
    }

    sprite = new Sprite(imagen, anchoSprite, altoSprite);
    sprite->setFila(0);
    sprite->iniciarAnimacion();

    if (tipo == FUEGO) {
        posicionX = QRandomGenerator::global()->bounded(0, 1024);
        posicionY = 0;
        sprite->setPos(posicionX, posicionY);
    } else if (tipo == DODORIA) {
        sprite->setPos(1000, 500);
    } else if (tipo == ZARBON) {
        sprite->setPos(10, 500);
    }

    escena->addItem(sprite);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Enemigos::actualizarPosicion);

    // Timer para disparar automáticamente
    if (tipo == DODORIA || tipo == ZARBON) {
        timerDisparo = new QTimer(this);
        connect(timerDisparo, &QTimer::timeout, this, &Enemigos::disparar);
        timerDisparo->start(3000); // cada 3 segundos
    }
}

void Enemigos::iniciarComportamiento() {
    temporizador->start(16);  // ~60 FPS
}

void Enemigos::actualizarPosicion() {
    if (tipo == FUEGO) {
        velocidadY += gravedad;
        posicionY += velocidadY;
        sprite->setY(posicionY);

        if (sprite->collidesWithItem(jugador->getItem())) {
            jugador->perderVida();
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

    } else if (tipo == DODORIA || tipo == ZARBON) {
        if (sprite->collidesWithItem(jugador->getItem())) {
            jugador->perderVida();
        }

        for (QGraphicsPixmapItem* bloque : bloquesEscenario) {
            if (sprite->collidesWithItem(bloque)) {
                qreal nuevaY = bloque->y() - sprite->boundingRect().height();
                sprite->setY(nuevaY);
                break;
            }
        }
    }
}

void Enemigos::disparar() {
    if (!sprite || !sprite->scene()) return;

    QPixmap spriteDisparo(":/imagenes/kamehameha.png");
    QGraphicsPixmapItem* itemDisparo = new QGraphicsPixmapItem(spriteDisparo);
    itemDisparo->setScale(0.6);

    Proyectil::Direccion direccion;

    // Dirección según el tipo
    if (tipo == DODORIA) {
        direccion = Proyectil::Izquierda;
    } else if (tipo == ZARBON) {
        direccion = Proyectil::Derecha;
    } else {
        return;  // No disparan otros
    }

    Proyectil* p = new Proyectil(
        sprite->scene(),
        direccion,
        spriteDisparo,
        10,  // velocidad
        1    // daño
        );

    int offsetX = (direccion == Proyectil::Derecha) ? sprite->boundingRect().width() : -20;

    p->setPos(sprite->x() + offsetX, sprite->y() -10);
}

void Enemigos::setBloques(const QList<QGraphicsPixmapItem*>& bloques) {
    bloquesEscenario = bloques;
}


