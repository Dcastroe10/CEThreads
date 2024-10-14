#ifndef INITIALIZE_H
#define INITIALIZE_H

#ifdef __cplusplus
extern "C" {
#endif


// Definición de la estructura CanalConfig
typedef struct {
    int MAX_SHIPS;
    int CHANNEL_SIZE;
    int scheduler;
    int workflow;
    int EQUITY_W;
    int SIGN_TIME;
    int QUANTUM;
} CanalConfig;


// Declaración de la función
CanalConfig Initialize_Configuration(const char *nombreArchivo);

#ifdef __cplusplus
}
#endif

#endif // INITIALIZE_H
