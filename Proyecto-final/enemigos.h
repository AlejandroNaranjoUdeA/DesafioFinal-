#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "jugador.h"
#include "sprite.h"

enum TipoEnemigo { FUEGO, DODORIA, ZARBON, SAIBAMAN };

class Enemigos : public QObject {
    Q_OBJECT

private:
    Sprite* sprite;
    Jugador* jugador;
    TipoEnemigo tipo;
    QTimer* temporizador;
    QTimer* timerDisparo = nullptr;
    QTimer* timer;


    float velocidadY;
    float gravedad;
    int vidas;

    float posicionX;
    float posicionY;

    QList<QGraphicsPixmapItem*> bloquesEscenario;

public:
    Enemigos(QGraphicsScene* escena, Jugador* jugador, TipoEnemigo tipo,
             QString rutaImagen, int anchoSprite, int altoSprite);

    void iniciarComportamiento();
    void setBloques(const QList<QGraphicsPixmapItem*>& bloques);

    Sprite* getSprite() const { return sprite; }

private slots:
    void actualizarPosicion();
    void disparar();
};

#endif // ENEMIGOS_H
