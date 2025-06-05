#include "sprite.h"

Sprite::Sprite(QString rutaSprite, int anchoFrame, int altoFrame, int fila)
    : ancho(anchoFrame), alto(altoFrame), filaActual(fila), frameActual(0), totalFrames(0)
{
    spriteSheet.load(rutaSprite);
    totalFrames = spriteSheet.width() / ancho;

    setPixmap(spriteSheet.copy(0, fila * alto, ancho, alto));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sprite::avanzarFrame);
}

void Sprite::avanzarFrame() {
    frameActual = (frameActual + 1) % totalFrames;
    setPixmap(spriteSheet.copy(frameActual * ancho, filaActual * alto, ancho, alto));
}

void Sprite::setFila(int fila) {
    filaActual = fila;
    frameActual = 0;
    setPixmap(spriteSheet.copy(0, filaActual * alto, ancho, alto));
}

void Sprite::setFrame(int indice) {
    frameActual = indice % totalFrames;
    setPixmap(spriteSheet.copy(frameActual * ancho, filaActual * alto, ancho, alto));
}

void Sprite::iniciarAnimacion(int ms) {
    timer->start(ms);
}

void Sprite::detenerAnimacion() {
    timer->stop();
}

void Sprite::setTotalFrames(int cantidad) {
    totalFrames = cantidad;
}
