#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Sprite : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Sprite(QString rutaSprite, int anchoFrame, int altoFrame, int fila = 0);

    void setFila(int fila);               // Cambiar animación (ej. caminar, saltar)
    void setFrame(int indice);            // Establecer frame manualmente
    void avanzarFrame();                  // Pasar al siguiente frame

    void iniciarAnimacion(int ms = 100);  // Iniciar animación automática
    void detenerAnimacion();             // Detener animación

    void setTotalFrames(int cantidad);   // Por si la fila tiene menos frames

private:
    QPixmap spriteSheet;
    QTimer* timer;

    int ancho;
    int alto;
    int filaActual;
    int frameActual;
    int totalFrames;
};

#endif // SPRITE_H
