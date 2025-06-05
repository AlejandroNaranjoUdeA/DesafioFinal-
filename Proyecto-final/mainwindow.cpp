#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "juego.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stack = new QStackedWidget(this);
    setCentralWidget(stack);


    // Menú principal
    menuPrincipal = new QWidget;

    //imagen de menu principal
    menuPrincipal->setStyleSheet("background-image: url(:/imagenes/fondo menu.jpg);"
                                 "background-repeat: no-repeat;"
                                 "background-position: center;"
                                 "background-size: cover;");

    QVBoxLayout *layoutMenu = new QVBoxLayout(menuPrincipal);

    //musica de fondo

    musicaMenu = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicaMenu->setAudioOutput(audioOutput);

    // Ruta del audio (puede ser en tu .qrc como ":/sonidos/menu.mp3")
    musicaMenu->setSource(QUrl("qrc:/sonidos/goku_dragon-ball-z-cha-la-head-cha-la.mp3"));
    musicaMenu->setLoops(QMediaPlayer::Infinite);  // para que se reproduzca de forma infinita
    audioOutput->setVolume(70); // volumen del 0 al 100
    musicaMenu->play();



    QLabel *titulo = new QLabel("Menú Principal");
    QPushButton *botonNivel1 = new QPushButton("Iniciar Nivel 1");

    layoutMenu->addWidget(titulo);
    layoutMenu->addWidget(botonNivel1);

    connect(botonNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);

    // Nivel 1 (juego)
    nivel1 = new Juego();

    stack->addWidget(menuPrincipal);
    stack->addWidget(nivel1);

    mostrarMenu();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mostrarMenu() {
    stack->setCurrentWidget(menuPrincipal);
}

void MainWindow::iniciarNivel1() {
    musicaMenu->stop(); // Detener música del menú
    stack->setCurrentWidget(nivel1);
}
