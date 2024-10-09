
int maxShips = 10;

typedef enum {
    NORMAL,
    FISHING,
    PATROL
} shipType_t; // determines the ship speed

typedef enum{
    RIGHT,
    LEFT
} channelSide_t;

typedef struct
{
    shipType_t type;
    channelSide_t side;
    short priority;
    int position;
    int threadId;
} ship_t;

/**
 * @brief creates a ship
 * 
 * @param type of the ship, determining its speed
 * @param side side of the channel where the ship starts
 * @param priority for the movement of the ship (thread priority)
 * @return a pointer of the created ship
 */
ship_t* create_ship(shipType_t type, channelSide_t side, short priority);


/**
 * @brief moves the given ship through the channel
 * 
 * @param ship ship needed to move
 */
void move_ship(ship_t *ship);
