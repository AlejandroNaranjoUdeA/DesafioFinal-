#include "juego.h"
#include <QRandomGenerator>

Juego::Juego(int nivelSeleccionado, QWidget *parent) : QGraphicsView(parent), nivel(nivelSeleccionado) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 8000, 768);
    setScene(escena);

    QPixmap fondoCompleto;
    if (nivel == 1)
        fondoCompleto.load(":/imagenes/fondo fuego.jpeg");
    else if (nivel == 2)
        fondoCompleto.load(":/imagenes/fondo bosque.jpg");
    else if (nivel == 3)
        fondoCompleto.load(":/imagenes/fondo ciudad.jpg");

    int anchoBloque = 1024;
    int altoFondo = fondoCompleto.height();
    int numBloques = fondoCompleto.width() / anchoBloque + 1;

    for (int i = 0; i < numBloques; ++i) {
        QPixmap fragmento = fondoCompleto.copy(i * anchoBloque, 0, anchoBloque, altoFondo);
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fragmento);
        fondoItem->setZValue(-1);
        fondoItem->setPos(i * anchoBloque, 0);
        fondoItems.append(fondoItem);
        escena->addItem(fondoItem);
    }

    goku = new Jugador(escena, this);
    goku->setPos(100, 350);
    escena->addItem(goku->getItem());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Juego::actualizarJuego);
    connect(timer, &QTimer::timeout, this, [=]() {
        goku->actualizar();
    });
    timer->start(16);

    connect(goku, &Jugador::vidasActualizadas, this, &Juego::actualizarTextoVidas);

    textoVidas = new QGraphicsTextItem();
    QColor colorTexto = escena->backgroundBrush().color().value() > 128 ? Qt::black : Qt::white;
    textoVidas->setDefaultTextColor(colorTexto);
    textoVidas->setFont(QFont("Arial", 12));
    textoVidas->setPos(120, 10);
    escena->addItem(textoVidas);
    actualizarTextoVidas();

    escenario = new Escenario(escena);
    escenario->crearPiso();
    goku->setBloques(escenario->obtenerBloques());

    musicaFondo = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicaFondo->setAudioOutput(audioOutput);

    if (nivel == 1) {
        Enemigos* dodoria = new Enemigos(escena, goku, DODORIA, ":/imagenes/dodoria sin fondo.png", 57, 63);
        dodoria->iniciarComportamiento();

        Enemigos* zarbon = new Enemigos(escena, goku, ZARBON, ":/imagenes/zarbon sin fondo.png", 41, 60);
        zarbon->iniciarComportamiento();

        dodoria->setBloques(escenario->obtenerBloques());
        zarbon->setBloques(escenario->obtenerBloques());

        musicaFondo->setSource(QUrl("qrc:/sonidos/musica de fondo (1).mp3"));
        musicaFondo->setLoops(QMediaPlayer::Infinite);  // Reproducir en bucle
        audioOutput->setVolume(50);  // Puedes ajustar el volumen
        musicaFondo->play();
    }

    generadorEnemigos = new QTimer(this);
    connect(generadorEnemigos, &QTimer::timeout, [=]() {
        Enemigos* enemigo = new Enemigos(escena, goku, FUEGO, "", 42, 60);
        enemigo->iniciarComportamiento();
        escena->addItem(enemigo->getSprite());

        int intervalo = QRandomGenerator::global()->bounded(200, 2000);
        generadorEnemigos->start(intervalo);
    });
    generadorEnemigos->start(500);



    centerOn(goku->getItem()->x(), goku->getItem()->y());
}

void Juego::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        QPixmap sprite(":/imagenes/kamehameha.png");
        QGraphicsPixmapItem* iDisparo = new QGraphicsPixmapItem(sprite);
        iDisparo->setScale(0.6);

        Proyectil* p = new Proyectil(
            escena,
            Proyectil::Derecha,
            sprite,
            15,
            2
            );
        p->setPos(goku->getItem()->x() + 50, goku->getItem()->y() + 20);
    }

    goku->mover(event, true);
}

void Juego::actualizarTextoVidas() {
    textoVidas->setPlainText("Vidas: " + QString::number(goku->getVidas()));
}

void Juego::moverFondo(int dx) {
    for (QGraphicsPixmapItem* fondo : fondoItems) {
        fondo->moveBy(dx, 0);
    }

    for (QGraphicsPixmapItem* fondo : fondoItems) {
        if (dx < 0 && fondo->x() <= -1024) {
            qreal maxX = fondoItems[0]->x();
            for (QGraphicsPixmapItem* f : fondoItems)
                if (f->x() > maxX)
                    maxX = f->x();
            fondo->setX(maxX + 1024);
        } else if (dx > 0 && fondo->x() >= 1024 * fondoItems.size()) {
            qreal minX = fondoItems[0]->x();
            for (QGraphicsPixmapItem* f : fondoItems)
                if (f->x() < minX)
                    minX = f->x();
            fondo->setX(minX - 1024);
        }
    }
}

void Juego::actualizarJuego() {
    goku->actualizar();
    escenario->actualizarEscenario(goku->getItem()->x());

    int dx = goku->obtenerDeltaX();
    moverFondo(-dx);

    qreal nuevaX = goku->getItem()->x();
    centerOn(nuevaX, 384);
}

void Juego::moverEscenario() {
    escenario->actualizarEscenario(goku->getItem()->x());
}

void Juego::mostrarNombreUsuario(const QString& nombre) {
    QGraphicsTextItem* textoUsuario = new QGraphicsTextItem("Usuario: " + nombre);
    QColor colorTexto = escena->backgroundBrush().color().value() > 128 ? Qt::black : Qt::white;
    textoUsuario->setDefaultTextColor(colorTexto);
    textoUsuario->setFont(QFont("Arial", 12));
    textoUsuario->setPos(10, 10);
    escena->addItem(textoUsuario);
}
