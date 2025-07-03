#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QObject>

class Proyectil : public QObject {
    Q_OBJECT

public:
    enum Direccion { Izquierda, Derecha };

    Proyectil(QGraphicsScene* escena, Direccion dir, QPixmap spriteSheet, int velocidad, int dano);
    void setPos(qreal x, qreal y);
    QRectF boundingRect() const;

private slots:
    void actualizarMovimiento();
    void mostrarAnimacionInicio();

private:
    QGraphicsPixmapItem* sprite;
    QGraphicsScene* escena;

    QPixmap spriteSheet;
    int frameActual;
    int frameAncho;
    int frameAlto;

    Direccion direccion;
    int velocidad;
    int dano;
};

#endif // PROYECTIL_H
