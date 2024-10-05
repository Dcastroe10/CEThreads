#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_prueba_boton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
