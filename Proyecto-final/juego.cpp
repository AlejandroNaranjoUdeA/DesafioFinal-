#include "juego.h"
#include <QRandomGenerator>

Juego::Juego(QWidget *parent) : QGraphicsView(parent) {

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // Crear escena
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 8000, 768);  // Tamaño de la escena
    setScene(escena);



    /*
    //agregar fondo
    QPixmap fondoPixmap(":/imagenes/fondo fuego.jpeg");
    fondoPixmap = fondoPixmap.scaled(1024, 768);  // Escalar fondo al tamaño de la ventana

    // Crear los dos fondos lado a lado
    fondo1 = new QGraphicsPixmapItem(fondoPixmap);
    fondo2 = new QGraphicsPixmapItem(fondoPixmap);

    fondo1->setZValue(-1);  // Detrás de todo
    fondo2->setZValue(-1);

    fondo1->setPos(0, 0);
    fondo2->setPos(1024, 0);  // Justo a la derecha

    escena->addItem(fondo1);
    escena->addItem(fondo2);
    */

    QPixmap fondoCompleto(":/imagenes/fondo fuego.jpeg");

    // Tamaño de bloque visible (ventana): 1024x716
    int anchoBloque = 1024;
    int altoFondo = fondoCompleto.height();

    // Calcular cuántos bloques se necesitan
    int numBloques = fondoCompleto.width() / anchoBloque + 1;

    for (int i = 0; i < numBloques; ++i) {
        // Extraer porciones del fondo
        QPixmap fragmento = fondoCompleto.copy(i * anchoBloque, 0, anchoBloque, altoFondo);
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fragmento);
        fondoItem->setZValue(-1);  // al fondo
        fondoItem->setPos(i * anchoBloque, 0);
        fondoItems.append(fondoItem);
        escena->addItem(fondoItem);
    }

    goku = new Jugador(escena, this);
    goku->setPos(100, 350);// Posición inicial
    escena->addItem(goku->getItem());


    // Timer para actualizar física
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Juego::actualizarJuego);

    connect(timer, &QTimer::timeout, this, [=]() {
        goku->actualizar();

    });

    timer->start(16); // ~60 FPS

    connect(goku, &Jugador::vidasActualizadas, this, &Juego::actualizarTextoVidas);

    // Mostrar vidas en pantalla
    textoVidas = new QGraphicsTextItem();
    QColor colorTexto = escena->backgroundBrush().color().value() > 128 ? Qt::black : Qt::white;
    textoVidas->setDefaultTextColor(colorTexto);
    textoVidas->setFont(QFont("Arial", 12));
    textoVidas->setPos(120, 10);
    escena->addItem(textoVidas);
    actualizarTextoVidas();  // mostrar el valor inicial


    //DODORIA
    Enemigos* dodoria = new Enemigos(escena, goku, DODORIA, ":/imagenes/dodoria sin fondo.png", 57, 63);
    dodoria->iniciarComportamiento();

    Enemigos* zarbon = new Enemigos(escena, goku, ZARBON, ":/imagenes/zarbon sin fondo.png", 41, 60);
    zarbon->iniciarComportamiento();

    //crear fuego:
    generadorEnemigos = new QTimer(this);
    connect(generadorEnemigos, &QTimer::timeout, [=]() {

        //FUEGO:
        Enemigos* enemigo = new Enemigos(escena, goku, FUEGO, "", 42, 60);  // pasar escena si usas Sprite
        //escena->addItem(enemigo);
        enemigo->iniciarComportamiento();

        escena->addItem(enemigo->getSprite());

        // Intervalo aleatorio entre 500ms y 1500ms
        int intervalo = QRandomGenerator::global()->bounded(200, 2000);
        generadorEnemigos->start(intervalo);
    });

    generadorEnemigos->start(500); // Generar un enemigo cada 1 segundos

    // Crear escenario (piso y demás)
    escenario = new Escenario(escena);
    escenario->crearPiso();

    dodoria->setBloques(escenario->obtenerBloques());
    zarbon->setBloques(escenario->obtenerBloques());


    goku->setBloques(escenario->obtenerBloques());

    // Centrar la cámara sobre Goku desde el inicio
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
            15,  // velocidad
            2    // daño
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
            // Recolocar a la derecha del más a la derecha
            qreal maxX = fondoItems[0]->x();
            for (QGraphicsPixmapItem* f : fondoItems)
                if (f->x() > maxX)
                    maxX = f->x();
            fondo->setX(maxX + 1024);
        } else if (dx > 0 && fondo->x() >= 1024 * fondoItems.size()) {
            // Recolocar a la izquierda del más a la izquierda
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
    moverFondo(-dx);  // El fondo se mueve al revés de Goku

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
    textoUsuario->setPos(10, 10);  // esquina superior izquierda
    escena->addItem(textoUsuario);
}

