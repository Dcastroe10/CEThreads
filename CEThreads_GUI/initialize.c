#include <stdio.h>
#include "initialize.h"


CanalConfig Initialize_Configuration(const char *nombreArchivo) {
    CanalConfig config;

    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de configuración\n");
        return config;  // Devolver una configuración vacía
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (sscanf(linea, "MetodoControlFlujo: %s", config.metodoControlFlujo) == 1) continue;
        if (sscanf(linea, "LargoCanal: %d", &config.largoCanal) == 1) continue;
        if (sscanf(linea, "VelocidadBarco: %d", &config.velocidadBarco) == 1) continue;
        if (sscanf(linea, "CantidadBarcos: %d", &config.cantidadBarcos) == 1) continue;
        if (sscanf(linea, "TiempoLetrero: %d", &config.tiempoLetrero) == 1) continue;
        if (sscanf(linea, "ParametroW: %d", &config.parametroW) == 1) continue;
    }

    fclose(archivo);

    // Imprimir la configuración cargada
    //printf("Método de Control de Flujo: %s\n", config.metodoControlFlujo);
    //printf("Largo del Canal: %d\n", config.largoCanal);
    //printf("Velocidad del Barco: %d\n", config.velocidadBarco);
    //printf("Cantidad de Barcos: %d\n", config.cantidadBarcos);
    //printf("Tiempo que cambia el Letrero: %d\n", config.tiempoLetrero);
    //printf("Parámetro W: %d\n", config.parametroW);

    return config;
}


