#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool prueba_bool = true;
    int testing =  0;

private slots:
    //void on_prueba_boton_clicked();

    void on_actionleft_triggered();

    void on_actionright_triggered();

    void on_pruebaStructs_clicked();

    void setupQueues();

    void displayQueues();

    QPixmap selectShipSprite(int type);


    void create_struct(int queue);

    void delete_first_struct(int queue);

    void on_pruebaAddLabel_clicked();

    void setupCanal(int ancho);

    void Ships_movement();

    void on_pushButton_clicked();

protected:
               void keyPressEvent(QKeyEvent *event) override;   // Handle key press
    void keyReleaseEvent(QKeyEvent *event) override; // Handle key release

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
