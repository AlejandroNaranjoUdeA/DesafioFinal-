#ifndef JUEGO_H
#define JUEGO_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include <QGraphicsTextItem>
#include "jugador.h"
#include "enemigos.h"
#include "escenario.h"
#include "proyectil.h"


class Juego : public QGraphicsView {
    Q_OBJECT
public:
    explicit Juego(int nivelSeleccionado, QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void moverFondo(int dx);
    void actualizarJuego();
    void moverEscenario();
    void mostrarNombreUsuario(const QString& nombre);


private:
    QGraphicsScene* escena;
    Jugador* goku;
    QTimer* timer;
    QTimer* generadorEnemigos;
    QList<Enemigos*> enemigos;
    Escenario* escenario;
    QGraphicsTextItem* textoVidas;
    QVector<QGraphicsPixmapItem*> fondoItems;
    int nivel;  // Nuevo: para distinguir entre nivel 1, 2 o 3
    QMediaPlayer* musicaFondo;
    QAudioOutput* audioOutput;

    void actualizarTextoVidas();
};

#endif // JUEGO_H
