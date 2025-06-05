#include "ventanas.h"
#include <QVBoxLayout>
#include <QLabel>

Ventanas::Ventanas(QWidget *parent) : QWidget(parent) {
    stack = new QStackedWidget(this);

    // ----- Menú principal -----
    menuPrincipal = new QWidget;
    QVBoxLayout *layoutMenu = new QVBoxLayout(menuPrincipal);

    QLabel *titulo = new QLabel("Menú Principal");
    QPushButton *botonNivel1 = new QPushButton("Iniciar Nivel 1");

    layoutMenu->addWidget(titulo);
    layoutMenu->addWidget(botonNivel1);

    connect(botonNivel1, &QPushButton::clicked, this, &Ventanas::iniciarNivel1);

    // ----- Nivel 1 (juego) -----
    nivel1 = new Juego();

    // Agregar widgets al stack
    stack->addWidget(menuPrincipal); // Índice 0
    stack->addWidget(nivel1);        // Índice 1

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stack);

    setLayout(mainLayout);

    mostrarMenu();
}

void Ventanas::mostrarMenu() {
    stack->setCurrentWidget(menuPrincipal);
}

void Ventanas::iniciarNivel1() {
    stack->setCurrentWidget(nivel1);
}
