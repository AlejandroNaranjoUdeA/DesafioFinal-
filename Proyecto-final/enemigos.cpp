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
    }else if (tipo == SAIBAMAN) {
        sprite->setPixmap(QPixmap(":/imagenes/saibaman-Photoroom.png").scaled(50, 60));
        sprite->setPos(QRandomGenerator::global()->bounded(500, 1500), 500);

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Enemigos::perseguirGoku);
        timer->start(40);
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

/*void Enemigos::iniciarComportamiento() {
    temporizador->start(16);  // ~60 FPS

}*/

Enemigos::~Enemigos(){
    if (temporizador) temporizador->stop();
    if (timerDisparo) timerDisparo->stop();
    if (timer) timer->stop();
}

void Enemigos::iniciarComportamiento() {

    temporizador->start(16);  // ~60 FPS

    /*
    if (tipo == DODORIA || tipo == ZARBON) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Enemigos::moverHorizontalmente);
        timer->start(50);
    } else if (tipo == FUEGO) {
        sprite->setPixmap(QPixmap(":/imagenes/bola de fuego.png").scaled(50, 50));
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Enemigos::moverFuego);
        timer->start(30);
    } else if (tipo == SAIBAMAN) {
        sprite->setPixmap(QPixmap(":/imagenes/saibaman.png").scaled(50, 60));
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Enemigos::perseguirGoku);
        timer->start(40);
    }*/
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

void Enemigos::perseguirGoku() {
    /*if (!goku || !sprite) return;

    QPointF posGoku = goku->getItem()->pos();
    QPointF posSaibaman = sprite->pos();

    qreal dx = 0;
    qreal dy = 0;

    if (posGoku.x() < posSaibaman.x()) dx = -2;
    else if (posGoku.x() > posSaibaman.x()) dx = 2;

    if (posGoku.y() < posSaibaman.y()) dy = -1;
    else if (posGoku.y() > posSaibaman.y()) dy = 1;

    sprite->moveBy(dx, dy);*/

    if (!goku || !sprite || !sprite->scene()) return;

    QPointF posGoku = goku->getItem()->pos();
    QPointF posSaibaman = sprite->pos();

    qreal dx = (posGoku.x() < posSaibaman.x()) ? -2 : 2;
    qreal dy = (posGoku.y() < posSaibaman.y()) ? -1 : 1;

    sprite->moveBy(dx, dy);
}

void Enemigos::setBloques(const QList<QGraphicsPixmapItem*>& bloques) {
    bloquesEscenario = bloques;
}


