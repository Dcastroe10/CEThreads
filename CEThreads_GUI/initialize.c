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
        if (linea.startsWith("MetodoControlFlujo:")) {
            sscanf(linea.toStdString().c_str(), "MetodoControlFlujo: %s", config.metodoControlFlujo);
        } else if (linea.startsWith("LargoCanal:")) {
            sscanf(linea.toStdString().c_str(), "LargoCanal: %d", &config.largoCanal);
        } else if (linea.startsWith("VelocidadBarco:")) {
            sscanf(linea.toStdString().c_str(), "VelocidadBarco: %d", &config.velocidadBarco);
        } else if (linea.startsWith("CantidadBarcos:")) {
            sscanf(linea.toStdString().c_str(), "CantidadBarcos: %d", &config.cantidadBarcos);
        } else if (linea.startsWith("TiempoLetrero:")) {
            sscanf(linea.toStdString().c_str(), "TiempoLetrero: %d", &config.tiempoLetrero);
        } else if (linea.startsWith("ParametroW:")) {
            sscanf(linea.toStdString().c_str(), "ParametroW: %d", &config.parametroW);
        }
    }

    archivo.close();
    return config;
}
