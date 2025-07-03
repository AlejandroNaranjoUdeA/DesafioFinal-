#include "proyectil.h"

Proyectil::Proyectil(QGraphicsScene* escena, Direccion dir, QPixmap spriteSheet, int velocidad, int dano)
    : QObject(), escena(escena), spriteSheet(spriteSheet), direccion(dir), velocidad(velocidad), dano(dano)
{
    frameAncho = 75;  // Ajusta según tu sprite
    frameAlto = 74;
    frameActual = 0;

    QPixmap primerFrame = spriteSheet.copy(0, 0, frameAncho, frameAlto);
    sprite = new QGraphicsPixmapItem(primerFrame);
    sprite->setZValue(1);
    escena->addItem(sprite);

    // Mostrar animación de inicio (una sola vez)
    QTimer::singleShot(0, this, SLOT(mostrarAnimacionInicio()));

    // Iniciar movimiento regular
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Proyectil::actualizarMovimiento);
    timer->start(16);  // ~60 FPS
}

void Proyectil::setPos(qreal x, qreal y) {
    if (sprite) sprite->setPos(x, y);
}

QRectF Proyectil::boundingRect() const {
    if (sprite) return sprite->boundingRect();
    return QRectF();
}

// Animación inicial del disparo: recorre 3 frames una vez
void Proyectil::mostrarAnimacionInicio() {
    const int totalFrames = 6;
    const int duracionFrame = 80; // ms por frame

    for (int i = 0; i < totalFrames; ++i) {
        QTimer::singleShot(i * duracionFrame, this, [=]() {
            QPixmap frame = spriteSheet.copy(i * frameAncho, 0, frameAncho, frameAlto);
            sprite->setPixmap(frame);
        });
    }
}

// Movimiento del proyectil
void Proyectil::actualizarMovimiento() {
    if (!sprite) return;

    qreal dx = (direccion == Derecha) ? velocidad : -velocidad;
    sprite->moveBy(dx, 0);

    // Eliminar si sale de la pantalla
    if (sprite->x() < -100 || sprite->x() > escena->width() + 100) {
        escena->removeItem(sprite);
        delete sprite;
        delete this;
    }
}
