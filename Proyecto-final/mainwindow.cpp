#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "juego.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QLineEdit>
#include <QTextStream>
#include <QWidget>
#include <QMessageBox>  // arriba del archivo


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stack = new QStackedWidget(this);
    setCentralWidget(stack);


    // Menú principal
    menuPrincipal = new QWidget;

    //imagen de menu principal
    menuPrincipal->setStyleSheet("background-image: url(:/imagenes/imagen inicio.png);"
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
    QPushButton *botonNivel1 = new QPushButton("Jugar");

    layoutMenu->addWidget(titulo);
    layoutMenu->addWidget(botonNivel1);

    // ------------------ Pantalla de autenticación ------------------
    pantallaAuth = new QWidget;
    QVBoxLayout *layoutAuth = new QVBoxLayout(pantallaAuth);
    QPushButton *botonLogin = new QPushButton("Iniciar sesión");
    QPushButton *botonRegistro = new QPushButton("Registrarse");

    layoutAuth->addWidget(new QLabel("Bienvenido"));
    layoutAuth->addWidget(botonLogin);
    layoutAuth->addWidget(botonRegistro);

    // ------------------ Formulario de Login ------------------
    formularioLogin = new QWidget;
    QVBoxLayout *layoutLogin = new QVBoxLayout(formularioLogin);
    entradaUsuarioLogin = new QLineEdit();
    entradaContrasenaLogin = new QLineEdit();
    entradaContrasenaLogin->setEchoMode(QLineEdit::Password);
    QPushButton *botonEntrar = new QPushButton("Entrar");

    layoutLogin->addWidget(new QLabel("Usuario:"));
    layoutLogin->addWidget(entradaUsuarioLogin);
    layoutLogin->addWidget(new QLabel("Contraseña:"));
    layoutLogin->addWidget(entradaContrasenaLogin);
    layoutLogin->addWidget(botonEntrar);

    // ------------------ Formulario de Registro ------------------
    formularioRegistro = new QWidget;
    QVBoxLayout *layoutRegistro = new QVBoxLayout(formularioRegistro);
    entradaUsuarioRegistro = new QLineEdit();
    entradaContrasenaRegistro = new QLineEdit();
    entradaContrasenaRegistro->setEchoMode(QLineEdit::Password);
    QPushButton *botonCrear = new QPushButton("Crear cuenta");

    layoutRegistro->addWidget(new QLabel("Nuevo usuario:"));
    layoutRegistro->addWidget(entradaUsuarioRegistro);
    layoutRegistro->addWidget(new QLabel("Contraseña:"));
    layoutRegistro->addWidget(entradaContrasenaRegistro);
    layoutRegistro->addWidget(botonCrear);

    //revisar archivos:

    // CONEXIÓN para crear cuenta
    connect(botonCrear, &QPushButton::clicked, this, [this]() {
        QString usuario = entradaUsuarioRegistro->text();
        QString contrasena = entradaContrasenaRegistro->text();

        if (usuario.isEmpty() || contrasena.isEmpty()) {
            QMessageBox::warning(this, "Error", "Debe llenar todos los campos.");
            return;
        }

        QFile archivo("usuarios.txt");
        if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
            return;
        }

        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList partes = linea.split(",");
            if (partes.size() == 2 && partes[0] == usuario) {
                QMessageBox::warning(this, "Error", "El usuario ya existe.");
                archivo.close();
                return;
            }
        }
        archivo.close();

        if (!archivo.open(QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escribir.");
            return;
        }

        QTextStream out(&archivo);
        out << usuario << "," << contrasena << "\n";
        archivo.close();

        QMessageBox::information(this, "Éxito", "Usuario registrado con éxito.");
        entradaUsuarioRegistro->clear();
        entradaContrasenaRegistro->clear();
        stack->setCurrentWidget(pantallaAuth);  // volver a pantalla inicial
    });

    // CONEXIÓN para iniciar sesión
    connect(botonEntrar, &QPushButton::clicked, this, [this]() {
        QString usuario = entradaUsuarioLogin->text();
        QString contrasena = entradaContrasenaLogin->text();

        if (usuario.isEmpty() || contrasena.isEmpty()) {
            QMessageBox::warning(this, "Error", "Debe llenar todos los campos.");
            return;
        }

        QFile archivo("usuarios.txt");
        if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
            return;
        }

        QTextStream in(&archivo);
        bool encontrado = false;
        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList partes = linea.split(",");
            if (partes.size() == 2 && partes[0] == usuario && partes[1] == contrasena) {
                encontrado = true;
                break;
            }
        }
        archivo.close();

        if (encontrado) {
            QMessageBox::information(this, "Éxito", "Inicio de sesión exitoso.");
            entradaUsuarioLogin->clear();
            entradaContrasenaLogin->clear();

            usuarioActivo = usuario;  // Guardamos el nombre
            nivel1->mostrarNombreUsuario(usuarioActivo);  // Se lo pasamos al juego

            musicaMenu->stop();  // detener música del menú
            stack->setCurrentWidget(nivel1);  // iniciar el juego
        } else {
            QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos.");
        }
    });


    // ------------------ Nivel 1 ------------------
    nivel1 = new Juego();

    // ------------------ Agregar todas las pantallas al stack ------------------
    stack->addWidget(menuPrincipal);        // índice 0
    stack->addWidget(pantallaAuth);         // índice 1
    stack->addWidget(formularioLogin);      // índice 2
    stack->addWidget(formularioRegistro);   // índice 3
    stack->addWidget(nivel1);               // índice 4

    // ------------------ Conexiones ------------------
    connect(botonNivel1, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(pantallaAuth);  // Ir a autenticación
    });

    connect(botonLogin, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(formularioLogin);  // Ir a login
    });

    connect(botonRegistro, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(formularioRegistro);  // Ir a registro
    });

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




