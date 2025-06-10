#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsScene>
#include <QObject>
#include <QKeyEvent>
#include "sprite.h"+
//para reproducir sonido
#include <QMediaPlayer>
#include <QAudioOutput>


class Juego;

class Jugador : public QObject {
    Q_OBJECT
public:
    Jugador(QGraphicsScene* escena, Juego* juego); // ahora recibimos la escena
    void mover(QKeyEvent* event, bool moverFisicamente);
    void actualizar();
    QGraphicsPixmapItem* getItem(); // para acceso externo
    void setPos(qreal x, qreal y);
    int getVidas() const;
    void perderVida();

    void setBloques(const QList<QGraphicsPixmapItem*>& bloques);
    void moverEscenario() ;




private:
    Sprite* sprite;
    Juego* juegoRef; // nuevo puntero
    int velocidadY;
    bool enElAire;
    int vidas;

    QMediaPlayer *sonidoSalto;
    QAudioOutput *saltoAudioOutput;

    //detectar bloques
    QList<QGraphicsPixmapItem*> bloques;


signals:
    void vidasActualizadas();
};

#endif // JUGADOR_H
