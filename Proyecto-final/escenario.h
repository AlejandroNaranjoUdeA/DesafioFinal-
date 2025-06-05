#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QList>

class Escenario {
public:
    Escenario(QGraphicsScene* escena);
    void crearPiso();

    void actualizarEscenario(double posicionJugador);

    QList<QGraphicsPixmapItem*> obtenerBloques();

private:
    QGraphicsScene* escena;
    QList<QGraphicsPixmapItem*> bloques;

    int anchoTotal;     // Ancho total del escenario
    int bloqueAncho = 50;
    int bloqueAlto = 50;

};

#endif // ESCENARIO_H
