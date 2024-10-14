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
//#include "../SchedulingShips/src/ship.c"
#include "../SchedulingShips/src/scheduler.c"
#include "cToSerial.c"



char buffer[256];

int leftCounter = 1;
int rightCounter = 1;

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

/*
    //Doing the SetUp
    QString rutaConfig = QCoreApplication::applicationDirPath() + "/config.txt"; //SetUp("/home/dcastroe/Desktop/En_Git/CEThreads/CEThreads_GUI/config.txt");
    // Convertir QString a const char*
    QByteArray ba = rutaConfig.toLocal8Bit();
    const char *rutaC = ba.data();
    CanalConfig initial_configuration = Initialize_Configuration(rutaC);
*/

    CanalConfig initial_configuration = Initialize_Configuration(":/config.txt");
    qDebug() << "Datos obtenidos desde el txt:";
    qDebug() << "1) Método de Control de Flujo-> "<<initial_configuration.metodoControlFlujo;
    qDebug() << "2) Largo del Canal-> "<<initial_configuration.largoCanal;
    qDebug() << "3) Velocidad del Barco-> "<<initial_configuration.velocidadBarco;
    qDebug() << "4) Cantidad de Barcos-> "<<initial_configuration.cantidadBarcos;
    qDebug() << "5) Tiempo que cambia el Letrero-> "<<initial_configuration.tiempoLetrero;
    qDebug() << "6) Parámetro W-> "<<initial_configuration.parametroW;


     //QLabel *element = ui->canal00 ;// Crear un nuevo int en el heap
    setupQueues();
    setupCanal(initial_configuration.largoCanal);

    initList(&leftList);
    initList(&rightList);




    initCEthreads();
    CEmutex_init();

    srand(time(0));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::Ships_movement);
    timer->start(1000);


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionleft_triggered()
{
    QPixmap *boat1 = new QPixmap(":/boat1.jpg");
    std::cout << "left action pressed" << std::endl;
    QPixmap scaledBoat1 = boat1->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //ui->canal09->setScaledContents(true);
    //ui->canal09->setPixmap(scaledBoat1);  //---------------------------------------------------------------------

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
    QPixmap *boat2 = new QPixmap(":/boat2.jpg");
    std::cout << "right action pressed" << std::endl;
    QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //ui->canal00->setScaledContents(true);
    //ui->canal00->setPixmap(scaledBoat2);
    //delete boat2;
}

void MainWindow::on_pruebaStructs_clicked()
{

    qDebug() << "on prueba clicked ";
/*
    qDebug()<<"Left waiting list \n";


    ShipNode* current1 = leftList.head;
    while (current1 != NULL) {
        qDebug() << "Ship ID:" << current1->ship->threadId
                 << ", Type:" << current1->ship->type
                 << ", Time:" << current1->ship->time
                 << ", Priority:" << current1->ship->priority
                 << ", Side:" << current1->ship->side
                 << ", Position:" << current1->ship->position;

        current1 = current1->next;

    }



    qDebug()<<"right waiting list \n";


    ShipNode* current2 = rightList.head;
    while (current2 != NULL) {
        qDebug() << "Ship ID:" << current2->ship->threadId
                 << ", Type:" << current2->ship->type
                 << ", Time:" << current2->ship->time
                 << ", Priority:" << current2->ship->priority
                 << ", Side:" << current2->ship->side
                 << ", Position:" << current2->ship->position;

        current2 = current2->next;

    }

*/
    handle_scheduler(PRIORITY, &leftList, &rightList);//___________________________________

    displayQueues();

    QThread *updatingThread = QThread::create([=]() {
        handle_workflow(TICO, &leftList, &rightList);
    });
    connect(updatingThread, &QThread::finished, updatingThread, &QObject::deleteLater);
    updatingThread->start();

    qDebug() << "Workflow started in a separate thread.";


}

void MainWindow::setupQueues()
{
    qDebug() << "setting up ";

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

        QPixmap scaledBoat = selectShipSprite(current1->ship->type);

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
        //std::cout << "right action pressed" << std::endl;
        QPixmap scaledBoat1 = boat1->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


        return scaledBoat1;

    }else if(type == 2){

        QPixmap *boat2 = new QPixmap(":/FISHING.png");
        //std::cout << "right action pressed" << std::endl;
        QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


        return scaledBoat2;

    }else if(type == 3){

        QPixmap *boat3 = new QPixmap(":/PATROL.png");
        //std::cout << "right action pressed" << std::endl;
        QPixmap scaledBoat3 = boat3->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


        return scaledBoat3 ;

    }else{

    }



}



void MainWindow::create_struct(int queue)
{


    int position = 0;


    if(rightCounter + leftCounter < 12  ){



    if(queue == 0){
        position =  getNextShipPosition(&leftList, LEFT) ;

            if(leftCounter < 6){
                shipType_t type = (shipType_t)((rand() % 3) + 1);
                ship_t *shipTemp = create_ship(type, LEFT, (rand() % 21),position);
                addShip(&leftList,shipTemp);
                leftCounter++;

            }
    }else if(queue == 1){

        position = getNextShipPosition(&rightList, RIGHT);

        if(rightCounter < 6){
                shipType_t type = (shipType_t)((rand() % 3) + 1);
                ship_t *shipTemp = create_ship(type, RIGHT, (rand() % 21),position);
                addShip(&rightList,shipTemp);
                rightCounter++;

        }

    }else if(queue == 2){

    }else{

    }

    }
}

void MainWindow::delete_first_struct(int queue)
{

    if(rightCounter != 0 && leftCounter != 0){




    if(queue == 0){

        if( leftCounter > 1){


        ship_t *temp = getLastShip(&leftList);
        removeShip(&leftList,temp->threadId);
        leftCounter--;

        }
    }else if(queue == 1){

        if(rightCounter > 1 ){
        ship_t *temp = getLastShip(&rightList);
        removeShip(&rightList,temp->threadId);
        rightCounter--;

        }
    }else if(queue == 2){

    }else{

    }
        }


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Q) {
        //qDebug() << "Key A pressed";


        create_struct(0);
        //std::cout << "left action pressed" << std::endl;



        displayQueues();

    }
    else if (event->key() == Qt::Key_E) {
        //qDebug() << "Key E pressed";


        create_struct(1);

        displayQueues();

    }
    else if (event->key() == Qt::Key_A) {
        //qDebug() << "Key E pressed";

        delete_first_struct(0);
        displayQueues();

    } else if (event->key() == Qt::Key_D) {
        //qDebug() << "Key E pressed";


        delete_first_struct(1);
        displayQueues();

    } else if(event->key() == Qt::Key_G){

        int fd = serial_init("/dev/ttyUSB0",9600);
        serial_send(fd,"sentFromCpp");

        serial_close(fd);
    }
    else{
        //qDebug() << "Key pressed: " << event->text();


    }

    // Call the base class event handler for default handling
    QMainWindow::keyPressEvent(event);
}

// Handle key release event
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) {
        //qDebug() << "Key A released";
    }
    else if (event->key() == Qt::Key_Space) {
        //qDebug() << "Spacebar released";


    }

    QMainWindow::keyReleaseEvent(event);
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
    }
}


void MainWindow::Ships_movement() {
    ShipNode* current1 = leftList.head;
    if (current1) {
        qDebug() << "Ship ID---------------------:" << current1->ship->threadId
                 << ", Type:" << current1->ship->type
                 << ", Time:" << current1->ship->time
                 << ", Priority:" << current1->ship->priority
                 << ", Side:" << current1->ship->side
                 << ", Position:" << current1->ship->position;
    }
    //handle_workflow(EQUITY, &leftSideShips, &rightSideShips);
    //get_index(leftList)  PARA OBTENER DATOS DE LAS LISTAS(en tiempo real)
    //QPixmap *boat2 = new QPixmap(":/boat2.jpg");
    //QPixmap scaledBoat2 = boat2->scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //QLabel *labelespacio0 = canal[0];
    //labelespacio0->setPixmap(scaledBoat2);
}




void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"Llamada a ships_movement: ";
    Ships_movement();
}

