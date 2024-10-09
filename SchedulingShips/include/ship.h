#include "../lib/cethreads.c"

#define CHANNEL_SIZE 10
#define EQUITY_W 2
int maxShips = 10;

typedef enum {
    NORMAL = 1,
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
ship_t* create_ship(shipType_t type, channelSide_t side, short priority, int position);


/**
 * @brief moves the given ship through the channel
 * 
 * @param ship ship needed to move
 */
void move_ship(ship_t *ship);

// Estructura para un nodo de la lista enlazada de barcos
typedef struct ShipNode {
    ship_t *ship;
    struct ShipNode *next;
} ShipNode;


// Estructura de la lista de barcos
typedef struct {
    ShipNode *head;
} ShipList;


// Function declarations for linked list management
/**
 * @brief Initializes an empty ship list.
 * @param list Pointer to the ship list to be initialized.
 */
void initList(ShipList* list);

/**
 * @brief Adds a ship to the beginning of the ship list.
 * @param list Pointer to the ship list.
 * @param ship Pointer to the ship to be added.
 */
void addShip(ShipList* list, ship_t* ship);

/**
 * @brief Removes a ship from the list by its thread ID.
 * @param list Pointer to the ship list.
 * @param id The thread ID of the ship to be removed.
 */
void removeShip(ShipList* list, int id);

/**
 * @brief Finds a ship in the list by its thread ID.
 * @param list Pointer to the ship list.
 * @param id The thread ID of the ship to find.
 * @return Pointer to the found ship node, or NULL if not found.
 */
ShipNode* findShip(ShipList* list, int id);

ship_t* getLastShip(ShipList* list);

int getNextShipPosition(ShipList* list, channelSide_t side);

/**
 * @brief Sorts the ships in the list by their priority in descending order.
 * @param list Pointer to the ship list to be sorted.
 */
void sortShipsByPriority(ShipList* list);

/**
 * @brief Prints the details of all ships in the list.
 * @param list Pointer to the ship list to be printed.
 */
void printList(const ShipList* list);
