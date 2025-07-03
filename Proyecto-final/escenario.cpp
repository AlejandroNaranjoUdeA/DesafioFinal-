#include "escenario.h"
#include <QPixmap>
#include <QRandomGenerator>

Escenario::Escenario(QGraphicsScene* escena) {
    this->escena = escena;
}

void Escenario::crearPiso() {
    QPixmap tileset(":/imagenes/bloque lava.jpg");
    QPixmap bloqueSuelo = tileset.copy(0, 0, 105, 105).scaled(50, 50);

    int columnas = (1024 * 3) / 50;  // Piso más largo que la pantalla
    int alturaBase = 3;
    int alturaActual = alturaBase;


    for (int x = 0; x < columnas; ++x) {
        if (x < 3) {
            alturaActual = 6;  // Forzar altura máxima al comienzo
        } else {
            int variacion = QRandomGenerator::global()->bounded(-1, 2); // -1, 0 o 1
            alturaActual += variacion;
            if (alturaActual < 2) alturaActual = 2;
            if (alturaActual > 6) alturaActual = 6;
        }

        for (int y = 0; y < alturaActual; ++y) {
            QGraphicsPixmapItem* tile = new QGraphicsPixmapItem(bloqueSuelo);
            tile->setPos(x * 50, 768 - (y + 1) * 50);
            tile->setZValue(0);
            escena->addItem(tile);
            bloques.append(tile);
        }
    }

    anchoTotal = columnas * bloqueAncho;  // ← importante
}


QList<QGraphicsPixmapItem*> Escenario::obtenerBloques() {
    return bloques;
}

//actualizar el escenario para que aparezca de forma infinita

void Escenario::actualizarEscenario(double posicionJugador) {
    /*for (auto bloque : bloques) {
        bloque->setX(bloque->x() - 2);  // mover todo a la izquierda (puede cambiarse)
    }

    // Si hay bloques que quedaron muy atrás, reciclalos
    for (auto bloque : bloques) {
        if (bloque->x() + bloqueAncho < posicionJugador - 1024 / 2) {
            qreal nuevaX = bloque->x() + anchoTotal;
            bloque->setX(nuevaX);
        }
    }*/
}



