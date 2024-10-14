#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <iostream>
#include <vector>
#include "prueba.c"
#include "initialize.c"
#include "linkedList.c"
#include "../SchedulingShips/src/scheduler.c"
#include "cToSerial.c"

char buffer[256];

int leftCounter = 1;
int rightCounter = 1;
int CHANNEL_SIZE_DEFINED;
int MAX_SHIPS_DEFINED;
scheduler_t schedulerDefined;
workflow_t workflowDefined;
int EQUITY_W_DEFINED;
int SIGN_TIME_DEFINED;
int QUANTUM_DEFINED;

channelSide_t letrero = NONE;
QLabel *label_letrero;

ShipList leftList;
ShipList rightList;

ship_t *ship_return;

std::vector<QLabel*> canal;
std::vector<QLabel*> colaIzquierda;
std::vector<QLabel*> colaDerecha;

struct List *dummy_list_left = createList(sizeof(DummieStruct));
struct List *dummy_list_right = createList(sizeof(DummieStruct));
DummieStruct *dummy_return = new DummieStruct;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1400,700);
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    QPixmap bkgnd(":/canalResize.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->centralWidget()->setAutoFillBackground(true);
    this->centralWidget()->setPalette(palette);
    label_letrero = new QLabel(this);

    //Reading the config.txt
    CanalConfig initial_configuration = Initialize_Configuration(":/config.txt");
    qDebug() << "Datos obtenidos desde el txt:";
    qDebug() << "1) Largo del Canal-> "<<initial_configuration.CHANNEL_SIZE;
    qDebug() << "2) Máximo de barcos-> "<<initial_configuration.MAX_SHIPS;
    qDebug() << "3) Calendarizador-> "<<getScheduler(initial_configuration.scheduler);
    qDebug() << "4) Algoritmo de flujo-> "<<getWorkflow(initial_configuration.workflow);
    qDebug() << "5) Parámetro W-> "<<initial_configuration.EQUITY_W;
    qDebug() << "6) Tiempo de Letrero-> "<<initial_configuration.SIGN_TIME;
    qDebug() << "7) Tiempo de Quantum-> "<<initial_configuration.QUANTUM;

    CHANNEL_SIZE_DEFINED = initial_configuration.CHANNEL_SIZE;
    MAX_SHIPS_DEFINED = initial_configuration.MAX_SHIPS;
    schedulerDefined = (scheduler_t)initial_configuration.scheduler;
    workflowDefined = (workflow_t)initial_configuration.workflow;
    EQUITY_W_DEFINED = initial_configuration.EQUITY_W;
    SIGN_TIME_DEFINED = initial_configuration.SIGN_TIME;
    QUANTUM_DEFINED = initial_configuration.QUANTUM;

    set_scheduling_workflow_parameters(CHANNEL_SIZE_DEFINED, MAX_SHIPS_DEFINED, EQUITY_W_DEFINED, SIGN_TIME_DEFINED, QUANTUM_DEFINED);

    setupQueues();
    setupCanal(CHANNEL_SIZE_DEFINED);

    initList(&leftList);
    initList(&rightList);

    initCEthreads();
    CEmutex_init();

    srand(time(0));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::Ships_movement);
    timer->start(5);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionleft_triggered()
{
    QPixmap *boat1 = new QPixmap(":/boat1.jpg");
    std::cout << "left action pressed" << std::endl;
    //QPixmap scaledBoat1 = boat1->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //ui->canal09->setScaledContents(true);
    //ui->canal09->setPixmap(scaledBoat1);

    delete boat1; //liberar la memoria del boat1

    struct List *prueba_lista = createList(sizeof(int));

    for (int i = 1; i <= 5; i++) {
        int *element = new int(i); // Crear un nuevo int en el heap
        addLast(prueba_lista, element);
    }

    printIntList(prueba_lista);
}

void MainWindow::on_actionright_triggered()
{
    //QPixmap *boat2 = new QPixmap(":/boat2.jpg");
    //std::cout << "right action pressed" << std::endl;
    //QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //ui->canal00->setScaledContents(true);
    //ui->canal00->setPixmap(scaledBoat2);
    //delete boat2;
}

void MainWindow::on_pruebaStructs_clicked()
{
    qDebug() << "on prueba clicked ";

    displayQueues();
    handle_scheduler(schedulerDefined, &leftList, &rightList);
    displayQueues();

    QThread *updatingThread = QThread::create([=]() {
        handle_workflow(workflowDefined, &leftList, &rightList, &letrero);
    });
    connect(updatingThread, &QThread::finished, updatingThread, &QObject::deleteLater);
    updatingThread->start();
}

void MainWindow::setupQueues()
{
    qDebug() << "setting up";

    qDebug() << "starting setup";
    colaIzquierda.push_back(ui->waitingL0);
    colaIzquierda.push_back(ui->waitingL1);
    colaIzquierda.push_back(ui->waitingL2);
    colaIzquierda.push_back(ui->waitingL3);
    colaIzquierda.push_back(ui->waitingL4);

    colaDerecha.push_back(ui->waitingR0);
    colaDerecha.push_back(ui->waitingR1);
    colaDerecha.push_back(ui->waitingR2);
    colaDerecha.push_back(ui->waitingR3);
    colaDerecha.push_back(ui->waitingR4);
    qDebug() << "finishing setup";
}

void MainWindow::displayQueues()
{


    for (int i = 0; i <= colaIzquierda.size()-1; i++) {
        //int *element = new int(i); // Crear un nuevo int en el heap
        //addLast(prueba_lista, element);
        QLabel *temp = colaIzquierda.at(i);
        temp->clear();
        //temp->setPixmap(scaledBoat2);

    }

    for (int i = 0; i <= colaDerecha.size()-1; i++) {
        //int *element = new int(i); // Crear un nuevo int en el heap
        //addLast(prueba_lista, element);
        QLabel *temp = colaDerecha.at(i);
        temp->clear();
        //temp->setPixmap(scaledBoat2);

    }


    ShipNode* current1 = leftList.head;
    int i = 0;
    while (current1 != NULL) {

        QLabel *temp = colaIzquierda.at(i);
        temp->setScaledContents(true);

        QPixmap scaledBoat = selectShipSprite(current1->ship->type).transformed(QTransform().scale(-1,1));

        temp->setPixmap(scaledBoat);
        current1 = current1->next;
        i++;

    }
    //i = 0;

    ShipNode* current2  = rightList.head;
    int j= 0;
    while (current2 != NULL) {

        QLabel *temp = colaDerecha.at(j);
        temp->setScaledContents(true);

        QPixmap scaledBoat = selectShipSprite(current2->ship->type);

        temp->setPixmap(scaledBoat);

        current2 = current2->next;
        j++;
    }




}

QPixmap MainWindow::selectShipSprite(int type){
    if(type == 1){
        QPixmap *boat1 = new QPixmap(":/NORMAL.png");
        QPixmap scaledBoat1 = boat1->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        return scaledBoat1;

    }else if(type == 2){
        QPixmap *boat2 = new QPixmap(":/FISHING.png");
        QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


        return scaledBoat2;

    }else if(type == 3){
        QPixmap *boat3 = new QPixmap(":/PATROL.png");
        QPixmap scaledBoat3 = boat3->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        return scaledBoat3 ;

    }else{
        QPixmap *boat1 = new QPixmap(":/NORMAL.png");
        QPixmap scaledBoat1 = boat1->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        return scaledBoat1;
    }
}



void MainWindow::create_struct(int queue){
    int position;
    shipType_t type;
    ship_t *shipTemp;
    int priority;

    if(rightCounter + leftCounter < 12  ){
        if(queue == 0 && leftCounter < 6){
            position = getNextShipPosition(&leftList, LEFT);
            type = (shipType_t)((rand() % 3) + 1);
            priority = rand() % 6;
            shipTemp = create_ship(type, LEFT, priority, position);
            addShip(&leftList, shipTemp);
            leftCounter++;
        }
        else if(queue == 1 && rightCounter < 6){
            position = getNextShipPosition(&rightList, RIGHT);
            type = (shipType_t)((rand() % 3) + 1);
            priority = rand() % 6;
            shipTemp = create_ship(type, RIGHT, priority, position);
            addShip(&rightList, shipTemp);
            rightCounter++;
        }
    }
}

void MainWindow::delete_first_struct(int queue)
{
    if(rightCounter != 0 && leftCounter != 0){
        if(queue == 0 && leftCounter > 1){
            ship_t *temp = getLastShip(&leftList);
            removeShip(&leftList,temp->threadId);
            leftCounter--;
        }
        else if(queue == 1 && rightCounter > 1){
            ship_t *temp = getLastShip(&rightList);
            removeShip(&rightList,temp->threadId);
            rightCounter--;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Q) {
        create_struct(0); // Agregar barco a la izquierda
        displayQueues();
    }
    else if (event->key() == Qt::Key_E) {
        create_struct(1); // Agregar barco a la derecha
        displayQueues();
    }
    else if (event->key() == Qt::Key_A) {
        delete_first_struct(0); // Eliminar último barco de la izquierda
        displayQueues();
    }
    else if (event->key() == Qt::Key_D) {
        delete_first_struct(1); // Eliminar último barco de la derecha
        displayQueues();
    }
    else if(event->key() == Qt::Key_G){

        int fd = serial_init("/dev/ttyUSB0",9600);
        serial_send(fd,"sentFromCpp");
        serial_close(fd);
    }

    // Call the base class event handler for default handling
    QMainWindow::keyPressEvent(event);
}

// Handle key release event
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) {
        qDebug() << "Key_W: Quit";
        QCoreApplication::quit(); // Termina la aplicación
    }
    else if (event->key() == Qt::Key_Space) {
        //qDebug() << "Key_Space";
    }

    QMainWindow::keyReleaseEvent(event); // Propaga el evento hacia el controlador predeterminado
}

void MainWindow::on_pruebaAddLabel_clicked()
{
    // Crear un nuevo QLabel con el texto "holaN" donde N puede ser un número o identificador______________________-
    QLabel *label = new QLabel("holaN", this);

    // Asumiendo que ya tienes un QHBoxLayout llamado "El_canal"
    QHBoxLayout *layout = findChild<QHBoxLayout*>("El_canal");

    // Verificar que el layout exista
    if(layout)
    {
        // Agregar el QLabel al QHBoxLayout
        layout->addWidget(label);
    }
    else
    {
        // Si el layout no existe, podrías manejar el error o crear uno nuevo
        qDebug() << "No se encontró el layout 'El_canal'";
    }
}

void MainWindow::setupCanal(int ancho){
    QHBoxLayout *layout = findChild<QHBoxLayout*>("El_canal");
    for (int i = 0;i < ancho; i++){
        QLabel *label = new QLabel("hola" + QString::number(i), this);
        layout->addWidget(label);
        canal.push_back(label);
    }
}


void MainWindow::Ships_movement() {
    //qDebug()<<CHANNEL_SIZE_DEFINED<<" START ship_movement";
    displayQueues();
    ShipNode* current_shipR  = rightList.head;
    for (QLabel* label : canal) {
        label->clear();
    }

    while (current_shipR != NULL) {
        if (1 < current_shipR->ship->position && current_shipR->ship->position < CHANNEL_SIZE_DEFINED){ //&&esta_activo
            QPixmap scaledBoat = selectShipSprite(current_shipR->ship->type);
            QLabel *temp = canal[current_shipR->ship->position - 1];
            temp->setPixmap(scaledBoat);
            //enviar posición del barco al arduino "current_shipL->ship->position"
        }
        current_shipR = current_shipR->next;
    }

    ShipNode* current_shipL  = leftList.head;
    while (current_shipL != NULL) {
        if (1 < current_shipL->ship->position && current_shipL->ship->position < CHANNEL_SIZE_DEFINED){
            QPixmap scaledBoat = selectShipSprite(current_shipL->ship->type).transformed(QTransform().scale(-1,1));
            QLabel *temp = canal[current_shipL->ship->position - 1];
            temp->setPixmap(scaledBoat);
            //enviar posición del barco al arduino "current_shipL->ship->position"
        }
        current_shipL = current_shipL->next;
    }

    if(letrero == RIGHT){
        qDebug()<<"DERECHA";
        QPixmap signal (":/DERECHA.png");
        label_letrero->clear();
        label_letrero->setPixmap(signal.transformed(QTransform().scale(-1,1)));
        label_letrero->setGeometry((this->width()-100)/2, 100, 100, 100);
        label_letrero->setScaledContents(true);
        label_letrero->show();

    }else if(letrero == LEFT){
        QPixmap signal (":/DERECHA.png");
        label_letrero->clear();
        label_letrero->setPixmap(signal);
        label_letrero->setGeometry((this->width()-100)/2, 100, 100, 100);
        label_letrero->setScaledContents(true);
        label_letrero->show();
    }
}




void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"Llamada a ships_movement: ";
    Ships_movement();
}

QString MainWindow::getScheduler(int scheduler) {
    if (scheduler == 0) {
        return "RR";
    }else if (scheduler == 1){
        return "PRIORITY";
    }else if (scheduler == 2){
        return "SJF";
    }else if (scheduler == 3){
        return "FCFS";
    }else if (scheduler == 4){
        return "REAL_TIME";
    }else{
        return "FCFS";
    }
}

QString MainWindow::getWorkflow(int workflow) {
    if (workflow == 0) {
        return "EQUITY";
    }else if (workflow == 1){
        return "SIGN";
    }else if (workflow == 2){
        return "TICO";
    }else{
        return "TICO";
    }
}

