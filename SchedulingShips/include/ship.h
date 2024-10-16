#ifdef __cplusplus
extern "C" {
#endif

#include "../lib/cethreads.c"

int CHANNEL_SIZE = 10;
int MAX_SHIPS = 10;

typedef enum {
    NORMAL = 1,
    FISHING,
    PATROL
} shipType_t; // Determines the ship speed

typedef enum{
    RIGHT,
    LEFT,
    NONE
} channelSide_t;

typedef struct
{
    shipType_t type;
    channelSide_t side;
    short priority;
    int position;
    int threadId;
    int time; // Process time remaining to cross the channel
} ship_t;

typedef enum {
    RR,             // Round Robin
    PRIORITY,
    SJF,            // Shortest Job First
    FCFS,           // FIFO First In First Out
    REAL_TIME
} scheduler_t; // Scheduler algorithm

typedef enum {
    EQUITY,
    SIGN,
    TICO
} workflow_t; // Workflow algorithm


scheduler_t scheduler = PRIORITY;
workflow_t workflow = TICO;
int EQUITY_W = 2;
int SIGN_TIME = 4;
int QUANTUM = 2;

/**
 * @brief Creates a new ship with specified properties and starts its associated thread.
 * @param type The type of the ship (e.g., NORMAL, FISHING, PATROL).
 * @param side The side of the channel where the ship starts (LEFT or RIGHT).
 * @param priority The priority level of the ship.
 * @param position The starting position of the ship in the channel.
 * @return Pointer to the newly created ship structure.
 */
ship_t* create_ship(shipType_t type, channelSide_t side, short priority, int position);

/**
 * @brief Returns the execution context from the current thread back to the main context.
 * This function is used to temporarily suspend the execution of the current thread and
 * return control back to the main scheduler or main execution flow.
 */
void returnContext();

/**
 * @brief Simulates the movement of a ship through the channel.
 * This function is executed by each thread representing a ship.
 * The ship keeps moving until it crosses the channel completely.
 * @param ship Pointer to the ship structure that contains its movement information.
 */
void move_ship(ship_t *ship);

/*<><><><><><><> Linked List <><><><><><><>*/

// Structure for a ship linked list node
typedef struct ShipNode {
    ship_t *ship;
    struct ShipNode *next;
} ShipNode;

// Structure for the ship list
typedef struct {
    ShipNode *head;
} ShipList;

// Function declarations for linked list management
/**
 * @brief Initializes an empty ship list.
 * @param list Pointer to the ship list to be initialized.
 */
void initList(ShipList* list);

void print_desde_Ship();

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
 * @brief Counts the number of ships in the list.
 * @param list Pointer to the ship list.
 * @return The number of ships in the list.
 */
int getShipCount(ShipList* list);

/**
 * @brief Finds a ship in the list by its thread ID.
 * @param list Pointer to the ship list.
 * @param id The thread ID of the ship to find.
 * @return Pointer to the found ship node, or NULL if not found.
 */
ShipNode* findShip(ShipList* list, int id);

/**
 * @brief Retrieves a ship node from the list by its index.
 * @param list Pointer to the ship list.
 * @param index The index of the ship node to retrieve (0-based).
 * @return Pointer to the ship node at the specified index, or NULL if the index is out of bounds.
 */
ShipNode* getShipByIndex(ShipList* list, int index);

/**
 * @brief Retrieves the ID of the first ship in the list.
 * @param list Pointer to the ship list.
 * @return The ID of the first ship, or -1 if the list is empty.
 */
int getFirstShipID(ShipList* list);

/**
 * @brief Retrieves the thread ID of a ship at a specific position in the list.
 * @param list Pointer to the ship list.
 * @param position The index position (0-based) in the list.
 * @return The thread ID of the ship at the specified position, or -1 if the position is invalid.
 */
int getShipIdByPosition(ShipList* list, int position);

/**
 * @brief Retrieves a ship by its position in the list.
 * @param list Pointer to the ship list.
 * @param position The index position of the ship in the list (starting from 0).
 * @return Pointer to the ship if found, or NULL if the position is invalid or the ship does not exist.
 */
ship_t* getShipByPosition(ShipList* list, int position);

/**
 * @brief Retrieves a ship by its ID from the list.
 * @param list Pointer to the ship list.
 * @param id The ID of the ship to search for.
 * @return Pointer to the ship if found, or NULL if no ship with the specified ID exists.
 */
ship_t* getShipById(ShipList* list, int id);

/**
 * @brief Retrieves the last ship in the list.
 * @param list Pointer to the ship list.
 * @return Pointer to the last ship node, or NULL if the list is empty.
 */
ship_t* getLastShip(ShipList* list);

/**
 * @brief Determines the next position for a ship based on the side of the channel.
 * @param list Pointer to the ship list.
 * @param side The side of the channel (LEFT or RIGHT) where the ship is located.
 * @return The next position for the ship in the list.
 *         If the list is empty, returns 0 for LEFT side or CHANNEL_SIZE + 1 for RIGHT side.
 */
int getNextShipPosition(ShipList* list, channelSide_t side);

/**
 * @brief Updates the position of all ships in the waiting line.
 * @param list Pointer to the ship list.
 */
void updateWaitingLine(ShipList* list);

/**
 * @brief Reassign ship position according to the scheduler.
 * @param ship Pointer to the ship.
 * @param index Relative position in the list.
 */
void asignShipPosition(ship_t* ship, int index);

/**
 * @brief Sorts the ships in the list by their priority in descending order.
 * @param list Pointer to the ship list to be sorted.
 */
void sortShipsByPriority(ShipList* list);

/**
 * @brief Sorts the ships in the list based on their remaining time in ascending order.
 * @param list Pointer to the ship list to be sorted.
 */
void sortShipsByShortestTime(ShipList* list) ;

/**
 * @brief Combines two ship lists into a single list.
 * @param leftList Pointer to the left-side ship list.
 * @param rightList Pointer to the right-side ship list.
 * @return Pointer to the combined ship list.
 */
ShipList* combineShipLists(ShipList* leftList, ShipList* rightList);

/**
 * @brief Moves a ship identified by its thread ID to the end of the list.
 * @param list Pointer to the ship list.
 * @param shipId The thread ID of the ship to be moved.
 */
void moveShipToEnd(ShipList* list, int shipId);

/**
 * @brief Prints the details of all ships in the list.
 * @param list Pointer to the ship list to be printed.
 */
void printList(const ShipList* list);

#ifdef __cplusplus
}
#endif
