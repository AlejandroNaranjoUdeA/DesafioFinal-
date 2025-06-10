#include "juego.h"
#include <QRandomGenerator>

Juego::Juego(QWidget *parent) : QGraphicsView(parent) {

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // Crear escena
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1024, 768);  // Tamaño de la escena
    setScene(escena);



    //agregar fondo
    QPixmap fondoPixmap(":/imagenes/fondo3.jpg");
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

    // Crear jugador (sprite con animación)
    goku = new Jugador(escena, this);
    goku->setPos(100, 524);  // Posición inicial
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
    QColor colorTexto = escena->backgroundBrush().color().value() > 128 ? Qt::white : Qt::black;
    textoVidas->setDefaultTextColor(colorTexto);
    textoVidas->setFont(QFont("Arial", 12));
    textoVidas->setPos(120, 10);
    escena->addItem(textoVidas);
    actualizarTextoVidas();  // mostrar el valor inicial

    //crear fuego:
    generadorEnemigos = new QTimer(this);
    connect(generadorEnemigos, &QTimer::timeout, [=]() {
        Enemigos* enemigo = new Enemigos(escena, goku);  // pasar escena si usas Sprite
        escena->addItem(enemigo);
        enemigo->iniciarCaida();
        // Intervalo aleatorio entre 500ms y 1500ms
        int intervalo = QRandomGenerator::global()->bounded(500, 1500);
        generadorEnemigos->start(intervalo);
    });

    generadorEnemigos->start(1000); // Generar un enemigo cada 1 segundos

    // Crear escenario (piso y demás)
    escenario = new Escenario(escena);
    escenario->crearPiso();

    goku->setBloques(escenario->obtenerBloques());


}

void Juego::keyPressEvent(QKeyEvent *event) {
    goku->mover(event, true);  // Se mueve Goku

}


void Juego::actualizarTextoVidas() {
    textoVidas->setPlainText("Vidas: " + QString::number(goku->getVidas()));
}

void Juego::moverFondo() {
    fondo1->moveBy(-2, 0);
    fondo2->moveBy(-2, 0);

    if (fondo1->x() <= -1024) {
        fondo1->setX(fondo2->x() + 1024);
    }
    if (fondo2->x() <= -1024) {
        fondo2->setX(fondo1->x() + 1024);
    }
}


void Juego::actualizarJuego() {
    goku->actualizar();
    escenario->actualizarEscenario(goku->getItem()->x());
    moverFondo();
    qreal nuevaX = goku->getItem()->x();
    if (nuevaX > 512)  // sólo sigue a Goku si pasa de la mitad de la pantalla
        centerOn(nuevaX, 384);  // 384 es la mitad vertical (768 / 2)

}

void Juego::moverEscenario() {
    escenario->actualizarEscenario(goku->getItem()->x());
}

void Juego::mostrarNombreUsuario(const QString& nombre) {
    QGraphicsTextItem* textoUsuario = new QGraphicsTextItem("Usuario: " + nombre);
    QColor colorTexto = escena->backgroundBrush().color().value() > 128 ? Qt::white : Qt::black;
    textoUsuario->setDefaultTextColor(colorTexto);
    textoUsuario->setFont(QFont("Arial", 12));
    textoUsuario->setPos(10, 10);  // esquina superior izquierda
    escena->addItem(textoUsuario);
}

