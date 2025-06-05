#include "jugador.h"
#include "juego.h"

Jugador::Jugador(QGraphicsScene* escena, Juego* juego): QObject(), juegoRef(juego)
{
    sprite = new Sprite(":/imagenes/goku sin fondo.png", 90, 94); // usa tu tamaño real de frame
    sprite->setFila(0);        // por ejemplo: parado
    sprite->iniciarAnimacion();

    // Calcular la posición inicial de Goku justo sobre el piso
    int alturaPiso = 150;
    int altoGoku = 94;
    int yInicial = 768 - alturaPiso - altoGoku;
    sprite->setPos(100, yInicial);

    //agregamos sonido
    sonidoSalto = new QMediaPlayer(this);
    saltoAudioOutput = new QAudioOutput(this);
    sonidoSalto->setAudioOutput(saltoAudioOutput);
    sonidoSalto->setSource(QUrl("qrc:/sonidos/sonido salto.mp3"));
    saltoAudioOutput->setVolume(70);

    escena->addItem(sprite);

    velocidadY = 0;
    enElAire = false;

    vidas=3;
}

void Jugador::mover(QKeyEvent* event, bool moverFisicamente) {
    qreal dx = 0;

    if (event->key() == Qt::Key_Left) {
        dx = -10;
        if (moverFisicamente) {
            sprite->setX(sprite->x() + dx);  // mueve físicamente a Goku
        }
        sprite->setFila(1);  // animación caminar izquierda
    } else if (event->key() == Qt::Key_Right) {
        dx = 10;
        sprite->setFila(2);  // animación caminar derecha
    }

    if (dx != 0) {
        QRectF futuroRect = sprite->sceneBoundingRect().translated(dx, 0);
        bool hayColision = false;

        for (QGraphicsPixmapItem* bloque : bloques) {
            if (futuroRect.intersects(bloque->sceneBoundingRect())) {
                hayColision = true;
                break;
            }
        }

        if (!hayColision) {
            if (!moverFisicamente) {
                sprite->moveBy(dx, 0);  // mover sprite si no se mueve el mundo
                if (dx > 0 && juegoRef) {
                    juegoRef->moverFondo();  // mover fondo si Goku va hacia la derecha
                }
            }
        }
    }

    if (event->key() == Qt::Key_Up && !enElAire) {
        velocidadY = -15;
        enElAire = true;
        sprite->setFila(5);  // animación salto

        sonidoSalto->setPosition(0);
        sonidoSalto->play();
    }
}


void Jugador::actualizar() {
    sprite->moveBy(0, velocidadY);

    bool enPiso = false;

    QRectF gokuRect = sprite->sceneBoundingRect();

    for (QGraphicsPixmapItem* bloque : bloques) {
        QRectF bloqueRect = bloque->sceneBoundingRect();

        // ¿Goku está cayendo y toca la parte superior del bloque?
        if (velocidadY >= 0 &&
            gokuRect.bottom() >= bloqueRect.top() &&
            gokuRect.bottom() <= bloqueRect.top() + 15 &&  // margen de tolerancia
            gokuRect.right() > bloqueRect.left() + 5 &&
            gokuRect.left() < bloqueRect.right() - 5) {

            sprite->setY(bloque->y() - sprite->boundingRect().height());
            velocidadY = 0;
            enElAire = false;
            sprite->setFila(0);
            enPiso = true;
            break;
        }
    }

    if (!enPiso) {
        velocidadY += 1; // gravedad suave
        if (velocidadY > 20) velocidadY = 20;  // velocidad terminal
    }
}


QGraphicsPixmapItem* Jugador::getItem() {
    return sprite;
}

void Jugador::setPos(qreal x, qreal y) {
    sprite->setPos(x, y);
}


//Vidas:
int Jugador::getVidas() const {
    return vidas;
}

void Jugador::perderVida() {
    vidas--;
    emit vidasActualizadas();  // <== EMITIR señal
    qDebug() << "Vidas restantes:" << vidas;
    if (vidas <= 0) {
        qDebug() << "¡Game Over!";
        // emit gameOver();  // si querés más adelante
    }
}


//detectar bloques:

void Jugador::setBloques(const QList<QGraphicsPixmapItem*>& bloques) {
    this->bloques = bloques;
}

