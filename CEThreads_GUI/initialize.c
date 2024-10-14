#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "initialize.h"

CanalConfig Initialize_Configuration(const char *nombreArchivo) {
    CanalConfig config;

    // Usar QFile para abrir el archivo desde los recursos Qt
    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo de configuración";
        return config;  // Devolver una configuración vacía
    }

    QTextStream in(&archivo);
    QString linea;
    while (in.readLineInto(&linea)) {
        if (linea.startsWith("CHANNEL_SIZE:")) {
            sscanf(linea.toStdString().c_str(), "CHANNEL_SIZE: %d", &config.CHANNEL_SIZE);
        }else if (linea.startsWith("MAX_SHIPS:")) {
            sscanf(linea.toStdString().c_str(), "MAX_SHIPS: %d", &config.MAX_SHIPS);
        }else if (linea.startsWith("scheduler:")) {
            sscanf(linea.toStdString().c_str(), "scheduler: %d", &config.scheduler);
        } else if (linea.startsWith("workflow:")) {
            sscanf(linea.toStdString().c_str(), "workflow: %d", &config.workflow);
        } else if (linea.startsWith("EQUITY_W:")) {
            sscanf(linea.toStdString().c_str(), "EQUITY_W: %d", &config.EQUITY_W);
        } else if (linea.startsWith("SIGN_TIME:")) {
            sscanf(linea.toStdString().c_str(), "SIGN_TIME: %d", &config.SIGN_TIME);
        }else if (linea.startsWith("QUANTUM:")) {
            sscanf(linea.toStdString().c_str(), "QUANTUM: %d", &config.QUANTUM);
        }
    }

    archivo.close();
    return config;
}
