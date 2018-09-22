# Mechmania C++ Starter Pack

Here's all the code you need to get started with making a bot for Mechmania in C++. Just do these steps:

* Pre-Setup -- install Docker, Node, and the `mm` command line tools
* Setup -- Clone this repository and start running your bot!

# Windows Pre-Setup

1. First, install Node. To do this, go [here](https://nodejs.org/en/download/) and download the Windows Installer.

2. Next, install Docker Toolbox for Windows.
   * Go to [this](https://docs.docker.com/toolbox/toolbox_install_windows/) site and click the button that says "Get Docker Toolbox for Windows".
   * Open the installer and follow the instructions to install Docker
   * If you don't already have Oracle VM VirtualBox, you will also go through a series of prompts to install VirtualBox.
   * Once Docker is installed, open the "Docker Quickstart Terminal" application (It will take a few minutes to get set up the first time you start it.  This process may involve more Windows prompts for permissions.)
3. Within Docker, run `npm install -g mechmania`.  This gets the `mm` command line tools, which are used to test and submit bots for the tournament.

# Mac Pre-Setup

1. First, install Node. To do this, go [here](https://nodejs.org/en/download/) and download the macOS Installer.

2. Next, install Docker for Mac.
   * You can find the installer [here](https://store.docker.com/editions/community/docker-ce-desktop-mac).
   * Open the installer and follow the instructions to install Docker

3. Run `npm install -g mechmania`.  This gets the `mm` command line tools, which are used to test and submit bots for the tournament.

# Setup

1. Clone this repo (or fork it or download it somewhere as a ZIP)
2. Modify the script at `MyBot.cpp`.
    * You may add other files or dependencies, just make sure to update the `.dockerignore` and `Dockerfile`s accordingly. If you have any questions about this, we're here to help!
3. Run `mm play .`
    * This will build the bot in the given directory (`.`) and then starts a game in which your bot fights against itself.
4. To run two different bots against each other, run `mm play bot1_directory bot2_directory`.
    * You must have valid `.dockerignore` and `Dockerfile` files in each directory.  We reccomend cloning this repository multiple times to achieve this.


Use `mm help` for more information.

# Game_Api Information

Game_Api is the class that allows your bot to communicate with the game_engine. This class is initialized at the beginning of your script and is updated each turn to hold the current state of the game. 
Game_Api defines three structs(classes whose members are all public) that you will be using: **Player, Monster** and **DeathEffects**. **DO NOT CHANGE THESE**. You will most likely be handling objects of these types so you should take a look at what member variables you can access from them. However tampering with the struct definitions themselves will make your script incompatible with our engine. Some helper functions in the API will return objects of this type since these objects represent the state of the game. You can alter these however you like but we recommend only using them as a reference to the current game state. 

**Note**: The class **Node** is also defined however you will not be interacting with any Node objects.

There is also a type that is used in the API called **node_id_t**. This type is the same as an Int and acts the exact same way as an Int. It is basically an Int with a fancy name. In the c++ API we use this notation simply to denote when we are working with node locations in the map. You do not need to follow the same convention. 

##### The following fields are in the `Player` class:
- `_name` : `str` (either "Player1" or "Player2")
- `_stance` : `str` -- string representation of the player's stance -- "Rock", "Paper", or "Scissors" (Note: on the first turn, each player's stance will be "Invalid Stance", since neither player has yet chosen a stance)
- `_health` : `int` -- the player's health.  If a player's health reaches 0, they die.
- `_speed` : `int` -- the player's speed.
- `_movement_counter` : `int` -- the movement counter will go down by 1 each turn.  Once a player's movement counter is equal to their speed, they will move to their destination.
- `_location` : `int` (or `node_id_t`)
- `_destination` : `int` (or `node_id_t`) (Note: on the first turn, the player's destination will be -1, since the player hasn't yet set a destination)
- `_dead` : `bool` (This should always be `True`, since once a player dies, the game is over)
- `_rock` : `int` -- the player's Rock attack stat
- `_paper` : `int` -- the player's Paper attack stat
- `_scissors` : `int` -- the player's Scissors attack stat

##### The following fields are in the `Monster` class:
- `_name` : `str` -- represents the class of this monster.  All monsters with the same name should have the same base stats (health, attack, respawn_rate, death_effects, and attack)
- `_stance` : `str`
- `_health` : `int`
- `_respawn_rate` : `int` -- number of turns for the monster to respawn after dying
- `_respawn_counter` : `int` -- turns until this monster will respawn (Note: if `dead=False`, then you should ignore the value of this field.)
- `_location` : `int` (or `node_id_t`)
- `_dead` : `bool`
- `_death_effects` : `DeathEffects` -- Gives information on the buffs given to the player when this monster dies on the same node as them.
- `_attack` : `int` -- the amount of damage the monster deals per turn
- `_base_health` : `int` -- the health that the monster will have after respawning

##### The following fields are in the `DeathEffects` class.
Each field corresponds to the buff provided to a player's stat from defeating a monster:
- `_rock` : `int`
- `_paper` : `int`
- `_scissors` : `int`
- `_health` : `int`
- `_speed` : `int`

#### The following are all functions in `Game_API`
(Note: For any function that returns Player, Monster, or DeathEffects structs, the corresponding data in the API may change between one turn and another to reflect the new game state, so you should make sure to only use structs returned during the current turn)

`void log(string message)`
Logs the string `message` to `stderr` for debugging.

`int get_duel_turn_num()`
Returns the turn number when the endgame duel will occur

`vector<node_id_t> get_adjacent_nodes(node_id_t location)`
Takes an int `node` and returns a vector of `int`s representing the nodes adjacent to `node`.

`vector<Monster> get_all_monsters()`
Returns a vector of all the monsters in the game. **This vector contains COPIES of the Monster objects within the API. Because of this, the Monsters within the returned vector will NOT change when the API is updated each turn!**. 

`Player get_self()`
Returns a `Player` object representing the player you are controlling. **Also a COPY of the data held within the API. Do not use the same returned Player object for multiple turns!**.

`Player get_opponent()`
Returns a `Player` object representing your opponent. (Note: the opponent's destination will always appear to be -1.  This is by design -- neither player can see the other's destination) **Also a COPY of the data held within the API. Do not use the same returned Player object for multiple turns!**.

`void submit_decision(node_id_t destination, string stance)`
Takes an `int` `destination` and a string `stance` and sends this decision to the game engine.  You should call this method exactly once per turn.

`vector<vector<node_id_t>> shortest_paths(node_id_t start, node_id_t destination)`
Returns a vector containing all the shortest paths between `start` and `end` nodes (passed in as `int`s).
Each element of the returned vector will be a vector of integers, representing the steps required to follow the path to `end`.  These lists will include `end`, but will not include `start` unless `start==end`.

So, as a hypothetical example (that doesn't necessarily match the game map), `shortest_paths(1,5)` could return the following:
```
[[2, 3, 4, 5], [2, 6, 7, 5], [9, 8, 7, 5]]
```
In this example, there are 3 paths, each of length 4, that can get from node `1` to node `5`.

`bool has_monster(node_id_t node)`
Returns a `bool` indicating whether there is a monster at node `node`.

`Monster get_monster(node_id_t node)`
Returns a `Monster` struct for the monster located at `node`.  It is recommended to check if there is a monster at `node` first, using `has_monster(node)`.  If you call `get_monster` for a node without a monster, an invalid monster will be returned.
**Also a COPY of the data held within the API. Do not use the same returned Monster object for multiple turns!**.

`vector<Monster> nearest_monsters(node_id_t node, int search_mode)`
Returns a list of `Monster` structs for all monsters nearest to `node` (including on `node` itself), only considering monsters according to the `search_mode` parameter:
- `search_mode = 0`: Searches all monsters
- `search_mode = 1`: Only searches for live monsters
- `search_mode = 2`: Only searches for dead monsters

`vector<Monster> nearest_monsters(node_id_t node, string name, int search_mode)`
Same as `nearest_monsters`, but only considers monsters with name `name`. 
(Note: since, on our map, there is only one monster with any given name, the list returned by this method will contain a maximum of 1 monster)
