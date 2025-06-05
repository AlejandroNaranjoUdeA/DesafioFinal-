#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "jugador.h"
#include "enemigos.h"
#include "escenario.h"


class Juego : public QGraphicsView {
    Q_OBJECT
public:
    explicit Juego(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;

    void moverFondo();
    void actualizarJuego();
    void moverEscenario();



private:
    QGraphicsScene* escena;
    Jugador* goku;
    QTimer* timer;
    QTimer* generadorEnemigos; // <-- nuevo
    QList<Enemigos*> enemigos;
    Escenario* escenario;
    QGraphicsTextItem* textoVidas;  // NUEVO
    QGraphicsPixmapItem* fondo1;
    QGraphicsPixmapItem* fondo2;
    QGraphicsPixmapItem* fondo;

protected:
    void actualizarTextoVidas();  // NUEVO

};



#endif // JUEGO_H
