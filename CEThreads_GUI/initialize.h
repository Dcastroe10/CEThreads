#ifndef INITIALIZE_H
#define INITIALIZE_H

#ifdef __cplusplus
extern "C" {
#endif


// Definición de la estructura CanalConfig
typedef struct {
    char metodoControlFlujo[20];
    int largoCanal;
    int velocidadBarco;
    int cantidadBarcos;
    int tiempoLetrero;
    int parametroW;
} CanalConfig;


// Declaración de la función
CanalConfig Initialize_Configuration(const char *nombreArchivo);

#ifdef __cplusplus
}
#endif

#endif // INITIALIZE_H
