/**
 * ============================================================
 * TIC-TAC-TOE PROGRAMMING ASSIGNMENT
 * ============================================================
 *
 * This file contains the starter code for the Tic-Tac-Toe
 * programming assignment.
 *
 * The purpose of this starter code is to provide the basic
 * infrastructure and program architecture so that students
 * can focus on implementing the core game logic and AI.
 *
 * ------------------------------------------------------------
 * PROVIDED (GIVEN CODE)
 * ------------------------------------------------------------
 *
 * The following components are already implemented and
 * should NOT need modification:
 *
 *   ✔ GameLogger
 *       Logging system for debugging and program output
 *
 *   ✔ Command line argument parser
 *       Handles --judge, --input, --log options
 *
 *   ✔ Game configuration structures
 *       RunConfig, GameSetup, GameResult
 *
 *   ✔ Program architecture
 *       Interaction / Renderer / Engine / Logic / Bot modules
 *
 *   ✔ Game engine structure (skeleton)
 *       startGame(), playGame(), endGame()
 *
 * These parts define the overall program flow.
 *
 * ------------------------------------------------------------
 * REQUIRED IMPLEMENTATION (STUDENTS MUST COMPLETE)
 * ------------------------------------------------------------
 *
 * Students must implement the following core functions:
 *
 *   Game Interaction
 *       validateInput()
 *       getInput()
 *       selectSize()
 *       selectGoal()
 *       selectGameMode()
 *       selectBotLevel()
 *       getPlayerMove()
 *
 *   Game Renderer
 *       displayBoard()
 *       showMove()
 *       showInvalidMove()
 *       showPlayer()
 *       showResult()
 *       printResult()
 *
 *   Game Logic
 *       initBoard()
 *       isValidMove()
 *       makeMove()
 *       isEmptyHead()
 *       checkWin()
 *       checkDraw()
 *
 *   Bot Logic
 *       random_pick()   (EASY bot - REQUIRED)
 *
 *   Game Engine
 *       startGame()
 *       playGame()
 *       endGame()
 *
 * ------------------------------------------------------------
 * OPTIONAL / BONUS TASKS
 * ------------------------------------------------------------
 *
 * Students may optionally implement stronger bot strategies:
 *
 *   simple_heuristic()   (MEDIUM difficulty)
 *   hard_level()         (HARD difficulty)
 *
 * These are not required for a basic passing solution but
 * may receive additional credit depending on the assignment
 * grading policy.
 *
 * ------------------------------------------------------------
 * IMPORTANT
 * ------------------------------------------------------------
 *
 * • Do NOT remove the provided program structure.
 * • You may add helper functions if needed.
 * • You may modify implementation details as long as the
 *   overall program still compiles and runs correctly.
 *
 * Please read the full assignment specification (PDF)
 * before starting the implementation.
 *
 * ============================================================
 */

/* ----------------------------------------------------- */
/* -------------------- [IMPORTING] -------------------- */
/* ----------------------------------------------------- */
/**
 * ============================================================
 * STANDARD LIBRARY HEADERS
 * ============================================================
 *
 * This section includes C++ standard library headers used
 * throughout the starter code.
 *
 * Students are ALLOWED to:
 *   ✔ Add additional headers if needed
 *   ✔ Remove unused headers
 *   ✔ Replace headers with alternatives
 *
 * As long as the program compiles and works correctly.
 *
 * The headers below are provided to support common tasks
 * such as:
 *
 *   - input/output
 *   - random number generation
 *   - timing and benchmarking
 *   - string manipulation
 *   - file input/output
 *
 * Quick overview:
 *
 *   <iostream>   : console input/output
 *   <string>     : string handling
 *   <sstream>    : string stream processing
 *   <iomanip>    : formatted output
 *
 *   <random>     : random number generator (bot moves)
 *
 *   <chrono>     : timing utilities
 *   <thread>     : sleep/delay functions
 *
 *   <fstream>    : file input/output (logging / judge mode)
 *
 *   <functional> : function wrappers / lambdas
 *   <type_traits>: template utilities
 *
 *   <limits>     : numeric limits
 *   <utility>    : std::pair and helper utilities
 *
 *   <ctime>      : timestamp for logging
 *   <format>     : string formatting (C++20)
 *
 * IMPORTANT:
 *   Do NOT use external libraries.
 *   Only C++ standard library is allowed.
 *
 * ============================================================
 */

#include <chrono>
#include <ctime>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

/* ------------------------------------------------------------ */
/* -------------------- [GLOBAL VARIABLES] -------------------- */
/* ------------------------------------------------------------ */
/**
 * ============================================================
 * GLOBAL CONFIGURATION (MUST READ)
 * ============================================================
 *
 * This section defines global constants, enums, and data
 * structures used throughout the program.
 *
 * Students should read this section carefully to understand
 * the configuration of the game.
 *
 * Most values here should NOT need to be modified.
 * However, you are allowed to adjust them if necessary
 * for experimentation or testing.
 *
 * Contents of this section:
 *
 *   1. Version information
 *   2. Global flags
 *   3. Game constants
 *   4. Random number generator
 *   5. Enumerations (game modes, bot levels, UI types)
 *   6. Data structures used by the game engine
 *
 * ============================================================
 */

// version
/**
 * Current version of the game program.
 * Used mainly for logging and debugging.
 *
 * TODO: Modify <STUDENT_ID> with your id.
 * Example: "0.4.2502xxxx"
 */
const std::string VERSION = "0.4.25022955";

// flag
/**
 * Global feature flags.
 *
 * ALGORITHM_FLAG
 *   Used to enable or disable advanced algorithms.
 *
 * TIME_ENABLED
 *   If true, execution time of certain functions
 *   will be logged (useful for performance testing).
 *
 * RANDOM_SEED
 *   Seed used for random number generation.
 *   Keeping the seed fixed ensures deterministic
 *   behavior for testing and debugging.
 */
const bool ALGORITHM_FLAG = true;
const bool TIME_ENABLED = true;

// Fixed seed ensures deterministic behavior during testing.
// Change the seed if you want different random games.
const int RANDOM_SEED = 2013;

// constants
/**
 * Game-related constants.
 *
 * BOARD_N_MAX
 *   Maximum allowed board size.
 *
 * SLEEP_TIME
 *   Delay (milliseconds) used for bot moves
 *   or UI animations.
 *
 */

// Maximum board dimension allowed by the program.
// Increasing this value may affect performance of bot algorithms.
const int BOARD_N_MAX = 12;
const int SLEEP_TIME = 1500;

/**
 * Global random number generator.
 *
 * Used by bot algorithms to generate random moves.
 *
 * std::mt19937 is a high-quality pseudo-random
 * number generator based on the Mersenne Twister algorithm.
 */
std::mt19937 generator(RANDOM_SEED);

/**
 * Bot difficulty levels.
 *
 * EASY
 *   Random move selection (required).
 *
 * MEDIUM
 *   Heuristic-based strategy (optional).
 *
 * HARD
 *   Advanced AI strategy (bonus challenge).
 *
 * INVALID_LV
 *   Used internally to represent invalid input.
 */
enum class BotLevel {
    EASY,
    MEDIUM,
    HARD,
    INVALID_LV
};

/**
 * Game modes supported by the program.
 *
 * PVP
 *   Player vs Player
 *
 * PVE
 *   Player vs Bot
 *
 * EVE
 *   Bot vs Bot
 *
 * INVALID_MODE
 *   Used when user input is invalid.
 */
enum class GameMode {
    PVP,
    PVE,
    EVE,
    INVALID_MODE
};

/**
 * UI menu types.
 *
 * Used by showSelectMenu() to determine which
 * menu should be displayed.
 *
 * Example:
 *
 *   showSelectMenu(SelectType::SIZE_UI);
 *
 * will display the board size selection menu.
 */
enum class SelectType {
    TITLE_UI,
    SIZE_UI,
    GOAL_UI,
    GAME_MODE_UI,
    BOT_LEVEL_UI,
    PLAYER_UI,
    MUL_BOT_LEVEL_UI,
    INVALID_UI
};

/**
 * End-point rule for detecting winning sequences.
 *
 * These rules are commonly used in Gomoku-style games
 * to determine whether a line is considered valid.
 *
 * NONE
 *   Ignore endpoint checking.
 *
 * OPEN_ONE
 *   At least one end of the sequence must be open.
 *
 * OPEN_TWO
 *   Both ends must be open.
 *
 * This rule can affect advanced bot evaluation.
 */
enum class EndRule {
    NONE,
    OPEN_ONE,
    OPEN_TWO
};

/**
 * Alias for a board coordinate.
 *
 * pII stands for:
 *   pair<int, int>
 *
 * Representing:
 *
 *   (row, column)
 */
typedef std::pair<int, int> pII;

/**
 * Runtime configuration for the program.
 *
 * This struct stores options parsed from
 * command line arguments.
 *
 * interactive
 *   true  -> show UI and menus
 *   false -> run in automated mode
 *
 * judge_mode
 *   used for automated grading systems.
 *
 * input_file
 *   path to file used as stdin.
 *
 * to_file
 *   whether logs should be written to file.
 *
 * log_file
 *   path of the log file.
 */
struct RunConfig {
    bool interactive = true;
    bool judge_mode = false;
    std::string input_file;

    bool to_file = true;
    std::string log_file = "log.txt";
};

/**
 * Stores configuration and current state of the game.
 *
 * board
 *   2D board storing symbols ('X', 'O', '-')
 *
 * size
 *   board dimension
 *
 * goal
 *   number of consecutive symbols required to win
 *
 * mode
 *   game mode (PVP / PVE / EVE)
 *
 * levels
 *   bot difficulty levels for players
 */
struct GameSetup {
    char board[BOARD_N_MAX][BOARD_N_MAX];
    int size;
    int goal;
    GameMode mode;
    BotLevel levels[2];
};

/**
 * Final result returned by the game engine.
 *
 * winner
 *   0  -> Player 1
 *   1  -> Player 2
 *   -1 -> Draw
 *
 * isBot
 *   true  -> winner is a bot
 *   false -> winner is a human
 *
 * turns
 *   total number of turns played.
 */
struct GameResult {
    int winner;
    bool isBot;
    int turns;
};

constexpr int DRAW_RESULT = -1;

/* -------------------------------------------------------- */
/* -------------------- [DECLARATIONS] -------------------- */
/* -------------------------------------------------------- */
/**
 * ============================================================
 * FUNCTION DECLARATIONS (MUST READ)
 * ============================================================
 *
 * This section lists all function prototypes used in the program.
 *
 * The implementation of these functions appears later in the file.
 *
 * Functions are grouped by module:
 *
 *   1. GameLogger      → logging system
 *   2. RunConfig       → command line arguments
 *   3. GameInteraction → input handling
 *   4. GameRenderer    → console UI
 *   5. GameEngine      → main game flow
 *   6. GameLogic       → board rules
 *   7. BotLogic        → AI move generation
 *   8. Helper          → utility functions
 *
 * You do not need to modify most of these declarations.
 *
 * ============================================================
 */

// Game Logger
/**
 * Logging system used throughout the program.
 *
 * Supports multiple log levels and optional
 * logging to file.
 *
 * Students do NOT need to modify this module.
 */
namespace GameLogger {
// Logging Level
/**
 * Severity level for log messages.
 */
enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    MSG,
};

/**
 * Convert logging level to printable string.
 */
inline std::string levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARNING:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::MSG:
            return "";
        default:
            return "UNKNOWN";
    }
}
inline static Level min_level = Level::DEBUG;

// Level Color
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";

/**
 * Return terminal color code corresponding to a log level.
 */
inline std::string getColor(Level level) {
    switch (level) {
        case Level::DEBUG:
            return BLUE;
        case Level::INFO:
            return GREEN;
        case Level::WARNING:
            return YELLOW;
        case Level::ERROR:
            return RED;
        default:
            return RESET;
    }
}

// Static Variables
inline static std::ofstream log_file;
inline static bool write_to_file = false;
inline static bool is_judge_mode = false;

// Functions
/**
 * Initialize logging system.
 */
void init(bool judge_mode, bool to_file = true, const std::string& path = "log.txt");

/**
 * Print a log message.
 */
void log(const std::string& msg, Level level = Level::INFO);

/**
 * Close log file (if used).
 */
void close();
}  // namespace GameLogger

// Run Config
/**
 * Parse command line arguments.
 */
RunConfig parseArgs(int argc, char* argv[]);

// Game Interaction
/**
 * This module handles all input-related operations of the game.
 *
 * Responsibilities:
 *
 *   1. Input redirection
 *      - Read from keyboard (interactive mode)
 *      - Read from file (judge / testing mode)
 *
 *   2. Input validation
 *      - Ensure numeric input is valid
 *      - Prevent program crashes from bad input
 *
 *   3. User selections
 *      - Board size
 *      - Win condition (goal)
 *      - Game mode
 *      - Bot difficulty
 *
 *   4. Player moves
 *      - Read move coordinates (row, col)
 *
 * Notes for students:
 *
 *   • Functions here should ONLY handle input parsing.
 *   • Game rules are checked later in GameLogic.
 *   • Invalid input should return false so the caller
 *     can ask the user again.
 *
 */
namespace GameInteraction {
static std::ifstream global_file_in;
}

std::streambuf* initInteraction(const RunConfig& config);
void closeInteraction(std::streambuf* cin_backup);
bool validateInput(std::string& input);
bool getInput(int* val);
bool selectSize(int* size);
bool selectGoal(int* goal, const int size);
bool selectGameMode(GameMode* mode);
bool selectBotLevel(BotLevel* levels, const int index);
bool getPlayerMove(int* row, int* col);

// Game Renderer
/**
 * Responsible for displaying all visual elements in the console.
 *
 * Includes:
 *
 *   • Clearing the screen
 *   • Displaying menus
 *   • Rendering the game board
 *   • Showing player actions
 *   • Showing final results
 *
 * This module does NOT contain game logic.
 * It only prints information to the console.
 *
 */

void clearScreen();
void showSelectMenu(SelectType selectType);
void displayBoard(const char board[][BOARD_N_MAX], const int size);
void showMove(const int row, const int col);
void showInvalidMove();
void showPlayer(const int player, const bool is_bot);
void showResult(const int winner, const bool is_bot);
void printResult(const GameResult& gameResult);

// Game Engine
/**
 * This module controls the overall flow of the game.
 *
 * Responsibilities:
 *
 *   1. Game setup
 *      - read configuration from user
 *      - initialize the board
 *
 *   2. Game loop
 *      - display board
 *      - determine current player
 *      - get move (human or bot)
 *      - apply move
 *      - check win / draw
 *      - switch player
 *
 *   3. Game termination
 *      - show final board
 *      - print result
 *
 * Important:
 *
 *   • The Engine manages WHEN things happen.
 *   • The actual rules of the game are implemented
 *     in the GameLogic module.
 *
 */

void startGame(const RunConfig& config, GameSetup& gameSetup);
GameResult playGame(const RunConfig& config, GameSetup& gameSetup);
void endGame(const RunConfig& config, GameSetup& gameSetup, GameResult& gameResult);

// Game Logic
/**
 * This module implements the rules of the game.
 *
 * Responsibilities:
 *
 *   • Board initialization
 *   • Move validation
 *   • Applying moves to the board
 *   • Checking win conditions
 *   • Detecting draw situations
 *
 * These functions operate directly on the board state.
 *
 * Important:
 *
 *   • GameLogic defines WHAT the rules are.
 *   • GameEngine decides WHEN these rules are checked.
 *
 * Students will mainly implement:
 *
 *   - initBoard()
 *   - isValidMove()
 *   - makeMove()
 *   - isEmptyHead()
 *   - checkWin()
 *   - checkDraw()
 *
 */

void initBoard(char board[][BOARD_N_MAX], const int size);
bool isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col);
void makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol);
bool isEmptyHead(char board[][BOARD_N_MAX], int size, int x, int y, const char symbol);
bool checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule = EndRule::OPEN_TWO);
bool checkDraw(char board[][BOARD_N_MAX], const int size);

// Bot Move Logic
/**
 * Generate the AI move for the current board state.
 *
 * This function dispatches to different strategies depending
 * on the selected difficulty level.
 *
 * The internal algorithms are intentionally modular so they can
 * be replaced or extended without affecting the rest of the engine.
 *
 * Developers are encouraged to experiment with their own AI logic
 * (pattern evaluation, Monte Carlo methods, minimax, etc.).
 *
 * @param board  Current board state
 * @param size   Board dimension (NxN)
 * @param goal   Number of consecutive symbols needed to win
 * @param symbol Bot symbol
 * @param level  Difficulty level
 *
 * @return Coordinate of the chosen move
 */
pII botMove(char board[][BOARD_N_MAX], const int size, const int goal, const char symbol, const BotLevel level);

// Level 1
/**
 * EASY difficulty.
 *
 * Selects a random valid move from the board.
 *
 * This mode exists mainly for testing and casual gameplay.
 *
 * @return Random empty cell
 */
pII random_pick(char board[][BOARD_N_MAX], const int size);

// Level 2
/**
 * MEDIUM difficulty.
 *
 * Uses a simple heuristic evaluation:
 * - try to win if possible
 * - block opponent winning moves
 * - otherwise choose a reasonable position
 *
 * This function is intentionally lightweight so the engine
 * remains fast even on large boards.
 *
 * @return Heuristically chosen move
 */
pII simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);

/*
 * NOTE FOR DEVELOPERS
 *
 * More advanced AI implementations can be added here if desired.
 *
 * Example approaches include:
 *
 * - Minimax with alpha-beta pruning
 * - Monte Carlo Tree Search (MCTS)
 * - Pattern scoring systems
 * - Hybrid evaluation + search
 *
 * The engine is designed so that new difficulty levels can be
 * implemented without modifying the rest of the game logic.
 */
pII hard_level(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);

// Game Helper
/**
 * Utility helper functions used across modules.
 */

template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled)
    -> std::invoke_result_t<Function>;

/* ------------------------------------------------------- */
/* -------------------- [DEFINITIONS] -------------------- */
/* ------------------------------------------------------- */
/**
 * ============================================================
 * TODO LIST FOR STUDENTS
 * ============================================================
 *
 * Interaction:
 *   validateInput
 *   getInput
 *   selectSize
 *   selectGoal
 *   selectGameMode
 *   selectBotLevel
 *   getPlayerMove
 *
 * Renderer:
 *   displayBoard
 *   showMove
 *   showInvalidMove
 *   showPlayer
 *   showResult
 *   printResult
 *
 * Game Logic:
 *   initBoard
 *   isValidMove
 *   makeMove
 *   isEmptyHead
 *   checkWin
 *   checkDraw
 *
 * Bot:
 *   random_pick (required)
 *   simple_heuristic (optional)
 *   hard_level (bonus)
 *
 * Game Engine
 *   startGame
 *   playGame
 *   endGame
 *
 * ============================================================
 */

/**
 * ============================================================
 * LOGGER MODULE (GIVEN)
 * ============================================================
 *
 * This module provides a simple logging system used throughout
 * the program.
 *
 * Students DO NOT need to modify this module.
 *
 * Usage examples:
 *
 *   GameLogger::log("Game started");
 *   GameLogger::log("Invalid move", GameLogger::Level::WARNING);
 *
 * Log levels:
 *   DEBUG   : detailed information for debugging
 *   INFO    : normal program information
 *   WARNING : suspicious behaviour
 *   ERROR   : invalid input or unexpected error
 *
 * Logs are printed to console and optionally written to file.
 *
 * ============================================================
 */
void GameLogger::init(bool judge_mode, bool to_file, const std::string& path) {
    write_to_file = to_file;
    is_judge_mode = judge_mode;

    if (write_to_file) {
        // not append
        // log_file.open(path, std::ios::out | std::ios::app);

        // write new log contents
        log_file.open(path, std::ios::out | std::ios::trunc);
        if (!log_file.is_open()) {
            std::cerr << "[Logger] Cannot open log file: " << path
                      << ". Falling back to console only." << std::endl;
            write_to_file = false;  // fallback
        }
    }

    std::string header = "Tic-tac-toe Game (Version: " + std::string(VERSION) + ")\n";
    header += std::string(48, '-');

    if (write_to_file) {
        log_file << header << std::endl;
    }

    if (!is_judge_mode) {
        std::cout << header << std::endl;
    }
}

void GameLogger::log(const std::string& msg, Level level) {
    // Only log if current level is important as or more than min_level
    if (static_cast<int>(level) < static_cast<int>(min_level)) {
        return;
    }

    auto now = std::time(nullptr);

    // NOTE: std::localtime is not thread-safe but acceptable here
    // since the program is single-threaded.
    auto tm = *std::localtime(&now);

    // format: '[Level] - [YYYY-MM-DD HH:MM:SS] Message'
    std::stringstream ss_lv;
    std::string formatted_lv;
    if (level != Level::MSG) {
        ss_lv << "[" << levelToString(level) << "]";
        formatted_lv = ss_lv.str();
    }

    std::stringstream ss_msg;
    ss_msg << (formatted_lv.empty() ? "" : " - ")
           << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] ";
    ss_msg << msg;
    std::string formatted_msg = ss_msg.str();

    // to output stream
    if (write_to_file) {
        log_file << formatted_lv << formatted_msg << std::endl;
        log_file.flush();
    }

    if (!is_judge_mode) {
        std::cout << getColor(level) << formatted_lv;
        std::cout << getColor(Level::MSG) << formatted_msg << RESET << std::endl;
    }
}

void GameLogger::close() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

/**
 * ============================================================
 * ARGUMENT PARSER (GIVEN)
 * ============================================================
 *
 * Parse command line arguments for the program.
 *
 * Supported arguments:
 *
 *   --judge, -j
 *       Enable judge mode (no UI, output minimal result)
 *
 *   --input <file>
 *       Read input from file instead of keyboard
 *
 *   --log <file>
 *       Write log output to file
 *
 * Students DO NOT need to modify this function.
 *
 * ============================================================
 */
RunConfig parseArgs(int argc, char* argv[]) {
    RunConfig config;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-j" || arg == "--judge") {
            config.judge_mode = true;
            // auto turn off interactive
            config.interactive = false;

        } else if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            config.input_file = argv[++i];

        } else if ((arg == "-l" || arg == "--log") && i + 1 < argc) {
            config.log_file = argv[++i];
            if (config.log_file == "") {
                config.to_file = false;
            }

        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Tic-tac-toe Game (Version: " << VERSION << " by baluong.87)\n";
            std::cout << "Usage: \n"
                      << "\t" << "--judge, -j" << "\t" << "Enable judge mode (no UI, standard output only)\n"
                      << "\t" << "--input, -i" << "\t" << "Path to input file\n"
                      << "\t" << "--log, -l" << "\t" << "Path to log file (default: log.txt)\n";
            exit(0);
        }
    }

    return config;
}

/* ---------- Game Interaction ---------- */
/**
 * ============================================================
 * INTERACTION MODULE (STUDENT IMPLEMENTATION)
 * ============================================================
 *
 * This module handles all user input operations of the game.
 *
 * Responsibilities:
 *
 *   • Reading input from the console or redirected input file
 *   • Parsing numeric values
 *   • Validating input format
 *   • Handling menu selections
 *   • Reading player moves
 *
 * Important design rule:
 *
 *   The Interaction module ONLY deals with input handling.
 *   It should NOT implement game rules or game logic.
 *
 * Game rule validation such as:
 *
 *   - checking if a move is inside the board
 *   - checking if a cell is empty
 *   - checking win or draw conditions
 *
 * must be handled in the GameLogic module instead.
 *
 * If invalid input is detected:
 *   • the function should return false
 *   • the caller (usually the GameEngine) will ask again
 *
 * Students are responsible for implementing the input
 * parsing and validation functions below.
 *
 * ============================================================
 */

// Given
std::streambuf* initInteraction(const RunConfig& config) {
    std::streambuf* cin_backup = nullptr;

    if (!config.interactive && !config.input_file.empty()) {
        GameInteraction::global_file_in.open(config.input_file);
        if (GameInteraction::global_file_in.is_open()) {
            cin_backup = std::cin.rdbuf();
            std::cin.rdbuf(GameInteraction::global_file_in.rdbuf());

            GameLogger::log(std::format("redirected cin to: {}", config.input_file));
        } else {
            GameLogger::log("failed to open input file, using console.", GameLogger::Level::ERROR);
        }
    }
    return cin_backup;
}

// Given
void closeInteraction(std::streambuf* cin_backup) {
    if (cin_backup) {
        std::cin.rdbuf(cin_backup);
        GameLogger::log("fallback using 'std::cin' input stream.");
    }

    if (GameInteraction::global_file_in.is_open()) {
        GameInteraction::global_file_in.close();
    }
}

/**
 * Validate raw input string.
 *
 * Purpose:
 *   Ensure the input contains only numeric characters
 *   and can be safely converted to an integer.
 *
 * Example:
 *   "12"  -> valid
 *   "3a"  -> invalid
 *   ""    -> invalid
 *
 * Parameters:
 *   input : raw string read from std::cin
 *
 * Return:
 *   true  -> valid numeric input
 *   false -> invalid input
 *
 * Hint:
 *   - Check each character using std::isdigit()
 *   - Only digits 0-9 are allowed.
 *   - Negative numbers are not accepted.
 */
bool validateInput(std::string& input) {
    // Check if input is empty
    if (input.empty()) {
        return false;
    }

    // Check each character is a digit (0-9)
    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    // All characters are digits, input is valid
    return true;
}

/**
 * Read a numeric value from input stream.
 *
 * Steps:
 *   1. Read a string from std::cin
 *   2. Validate the string using validateInput()
 *   3. Convert the string to integer
 *
 * Parameters:
 *   val : pointer to store the parsed integer
 *
 * Return:
 *   true  -> successful read
 *   false -> invalid input format
 *
 * Notes:
 *   - Do NOT crash on invalid input
 *   - Return false so caller can ask again
 */
bool getInput(int* val) {
    // Read input from user
    std::string input;
    std::cin >> input;

    // Validate the input
    if (!validateInput(input)) {
        return false;
    }

    // Convert to integer
    *val = std::stoi(input);
    return true;
}

/**
 * Ask user to choose board size.
 *
 * Allowed range:
 *   3 <= size <= BOARD_N_MAX
 *
 * Example:
 *   Input: 5
 *   Board will be 5 x 5
 *
 * Parameters:
 *   size : pointer to store selected board size
 *
 * Return:
 *   true  -> valid selection
 *   false -> invalid input or out of range
 *
 * Hint:
 *   Use getInput() and check range.
 */
bool selectSize(int* size) {
    // Get input from user
    int input;
    if (!getInput(&input)) {
        return false;
    }

    // Check if input is within valid range
    if (input < 3 || input > BOARD_N_MAX) {
        return false;
    }

    // Input is valid, store it
    *size = input;
    return true;
}

/**
 * Ask user to choose the number of consecutive symbols required to win.
 *
 * Constraints:
 *   3 <= goal <= size
 *
 * Example:
 *   size = 5
 *   goal can be: 3, 4, or 5
 *
 * Parameters:
 *   goal : pointer to store win condition
 *   size : board size
 *
 * Return:
 *   true  -> valid selection
 *   false -> invalid selection
 */
bool selectGoal(int* goal, const int size) {
    // Get input from user
    int input;
    if (!getInput(&input)) {
        return false;
    }

    // Check if input is within valid range
    if (input < 3 || input > size) {
        return false;
    }

    // Input is valid, store it
    *goal = input;
    return true;
}

/**
 * Ask user to select game mode.
 *
 * Available modes:
 *
 *   1 -> Player vs Player
 *   2 -> Player vs Bot
 *   3 -> Bot vs Bot
 *
 * Parameters:
 *   mode : pointer to store selected mode
 *
 * Return:
 *   true  -> valid choice
 *   false -> invalid input
 */
bool selectGameMode(GameMode* mode) {
    // Get input from user
    int input;
    if (!getInput(&input)) {
        return false;
    }

    // Check if input is within valid range
    if (input < 1 || input > 3) {
        return false;
    }

    // Set the corresponding game mode
    switch (input) {
        case 1:
            *mode = GameMode::PVP;
            break;
        case 2:
            *mode = GameMode::PVE;
            break;
        case 3:
            *mode = GameMode::EVE;
            break;
    }

    // Input is valid
    return true;
}

/**
 * Ask the user to select the bot difficulty level.
 *
 * Available level:
 *
 *   1 -> EASY
 *   2 -> MEDIUM
 *   3 -> HARD
 *
 * The selected level will be written into the provided
 * BotLevel array at the given index.
 *
 * Parameters:
 *
 *   levels  - Pointer to the bot level array used in the game
 *   index   - Position in the array where the selected level
 *             will be stored
 *
 * Return:
 *
 *   true  -> Level selected successfully
 *   false -> Invalid input
 *
 * Notes:
 *   - Player vs Bot -> index = 1
 *   - Bot vs Bot -> index in [0, 1]
 *
 */
bool selectBotLevel(BotLevel* levels, const int index) {
    // Get input from user
    int input;
    if (!getInput(&input)) {
        return false;
    }

    // Check if input is within valid range
    if (input < 1 || input > 3) {
        return false;
    }

    // Set the corresponding bot level
    switch (input) {
        case 1:
            levels[index] = BotLevel::EASY;
            break;
        case 2:
            levels[index] = BotLevel::MEDIUM;
            break;
        case 3:
            levels[index] = BotLevel::HARD;
            break;
    }

    // Input is valid
    return true;
}

/**
 * Read a player's move from input.
 *
 * Expected input format:
 *
 *   row col
 *
 * Example:
 *
 *   1 2
 *
 * Means:
 *   place symbol at board[1][2]
 *
 * Parameters:
 *   row : pointer storing row index
 *   col : pointer storing column index
 *
 * Return:
 *   true  -> input successfully parsed
 *   false -> invalid format
 *
 * Notes:
 *   - Range checking is done later using isValidMove().
 *   - Coordinates are ZERO-BASED.
 *
 * Example:
 * 0 0 -> top-left corner
 *
 */
bool getPlayerMove(int* row, int* col) {
    // Read the entire line from input
    std::string input;
    std::getline(std::cin, input);

    // Parse the input using stringstream
    std::istringstream stream(input);
    int x, y;
    char extra;

    // Check if exactly two integers are provided
    if (stream >> x >> y && !(stream >> extra)) {
        *row = x;
        *col = y;
        return true;
    }

    // Input format is invalid
    return false;
}

/* ---------- Game Renderer ---------- */
/**
 * ============================================================
 * RENDERER MODULE (STUDENT IMPLEMENTATION)
 * ============================================================
 *
 * This module is responsible for displaying information
 * to the console.
 *
 * It includes:
 *
 *   • Clearing the screen
 *   • Rendering the game board
 *   • Showing player actions
 *   • Displaying error messages
 *   • Showing the final result
 *
 * Important design rule:
 *
 *   The Renderer should ONLY print output.
 *   It must NOT implement any game logic.
 *
 * Game rules such as:
 *   - validating moves
 *   - checking win conditions
 *   - detecting draws
 *
 * are handled in the GameLogic module.
 *
 * Students need to implement the rendering functions below
 * to produce a clear and readable console interface.
 *
 * ============================================================
 */

/**
 * Clear the console screen. (Given)
 *
 * This function uses ANSI escape sequences to clear
 * the terminal and move the cursor to the top-left.
 *
 * It works on most Unix-like terminals and modern
 * Windows terminals.
 *
 * If the terminal does not support ANSI escape codes,
 * the game will still run but the screen may not clear.
 */
void clearScreen() {
    std::cout << "\n"
              << "\x1B[2J\x1B[H"
              << "\n";
    return;
}

/**
 * Display a selection menu depending on the UI type.
 *
 * This function prints different menus used during
 * the game setup and interaction phase.
 *
 * Parameters:
 *
 *   selectType : type of menu to display
 *
 * Possible menu types:
 *
 *   TITLE_UI           -> show game title
 *   SIZE_UI            -> ask for board size
 *   GOAL_UI            -> ask for win goal
 *   GAME_MODE_UI       -> select PvP / PvE / EvE
 *   BOT_LEVEL_UI       -> select bot difficulty
 *   PLAYER_UI          -> ask player move input
 *   MUL_BOT_LEVEL_UI   -> select bot levels for EvE
 */
void showSelectMenu(SelectType selectType) {
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
            // TODO: display board size selection
            // Example: "Size Input (NxN, 3 <= N <= BOARD_N_MAX)"
            std::cout << std::format(">----- Select Size (NxN, 3 <= N <= {}) -----<\n\n", BOARD_N_MAX);
            break;

        case SelectType::GOAL_UI:
            // TODO: display goal selection
            // Example: "Goal Input (3 - 5, goal <= size)"
            std::cout << ">----- Select Goal (3 - 5, goal <= size) -----<\n\n";
            break;

        case SelectType::GAME_MODE_UI:
            // TODO: display game mode selection
            // Example: "(1) PvP | (2) PvE | (3) EvE"
            std::cout << ">----- Select Game Mode ( >1< PvP | >2< PvE | >3< EvE) -----<\n\n";
            break;

        case SelectType::BOT_LEVEL_UI:
            // TODO: display bot level selection
            // Example: "(1) EASY | (2) MEDIUM | (3) HARD"
            std::cout << ">----- Select Bot Level ( >1< EASY | >2< MEDIUM | >3< HARD) -----<\n\n";
            break;

        case SelectType::PLAYER_UI:
            // TODO: display player move prompt
            // Example input format: "(row, col)"
            std::cout << ">----- Select Position ( \"row col\" E.g. \"1 2\") -----<\n\n";
            break;

        case SelectType::MUL_BOT_LEVEL_UI:
            // TODO: display multiple bot level selection
            // Example input format: "(bot1_level, bot2_level)"
            std::cout << ">----- Select Multiple Bot Level ( \"bot1_level bot2_level\" E.g. \"1 2\") -----<\n\n";
            break;

        default:
            break;
    }

    return;
}

/**
 * Render the game board to the console.
 *
 * Example output (size = 3):
 *
 *     0 1 2
 *   --------
 * 0 |X - O
 * 1 |- X -
 * 2 |O - X
 *
 * Parameters:
 *
 *   board : 2D board array representing the game state
 *   size  : dimension of the board (NxN)
 *
 * Hint:
 *
 *   Use nested loops to print rows and columns.
 *   Each cell contains either:
 *
 *     'X' -> player 1
 *     'O' -> player 2
 *     '-' -> empty cell
 */
void displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // Loop through rows including header and separators
    for (int i = 0; i < size + 3; i++) {
        // Loop through columns
        for (int j = 0; j < size + 1; j++) {
            // Print column headers
            if (i == 0) {
                if (j == 0) {
                    std::cout << "   0   ";
                } else if (j == size) {
                    std::cout << "\n";
                } else {
                    std::cout << ((j < 10) ? std::format("{}   ", j) : std::format("{}  ", j));
                }
            }
            // Print horizontal separators
            else if (i == 1 || i == size + 2) {
                std::cout << "  " << std::string(size * 4 - 1, '-') << "\n";
                break;
            }
            // Print board rows
            else {
                if (j == 0) {
                    std::cout << ((i - 2 < 10) ? std::format("{} |", i - 2) : std::format("{}|", i - 2));
                } else if (j == size) {
                    if (i == size + 1) {
                        std::cout << board[i - 2][j - 1] << "|\n";
                    } else {
                        std::cout << board[i - 2][j - 1] << "|\n\n";
                    }
                } else {
                    std::cout << std::format("{}   ", board[i - 2][j - 1]);
                }
            }
        }
    }
    return;
}

/**
 * Display which player is taking the turn.
 *
 * Parameters:
 *
 *   player  : player number (1 or 2)
 *   is_bot  : true if the player is controlled by the bot
 *
 * Example output:
 *
 *   Player 1's turn
 *   Bot (Player 2) is thinking...
 */
void showPlayer(int player, bool is_bot) {
    // Check if the player is a bot
    if (is_bot) {
        std::cout << std::format("Bot (Player {}) is thinking...\n", player);
        return;
    }

    // Player is human
    std::cout << std::format("Player {}'s turn \n", player);
    return;
}

/**
 * Display the move selected by a player.
 *
 * Parameters:
 *
 *   row : row index of the move
 *   col : column index of the move
 *
 * Example output:
 *
 *   Move placed at (1, 2)
 */
void showMove(const int row, const int col) {
    // Display the move coordinates
    std::cout << std::format("Move played at R:{} C:{}\n", row ,col);
    return;
}

/**
 * Display an error message when a move is invalid.
 *
 * This occurs when:
 *
 *   - The selected cell is already occupied
 *   - The move is outside the board
 */
void showInvalidMove() {
    // Display invalid move message
    std::cout << "Invalid Move! (Already selected or Out of range)\n";
    return;
}
/**
 * Display the final result of the game.
 *
 * Parameters:
 *
 *   winner : player number (1 or 2), or 0 if draw
 *   is_bot : true if the winner is a bot
 *
 * Example output:
 *
 *   Player 1 wins!
 *   Bot wins!
 *   It's a draw!
 */
void showResult(const int winner, const bool is_bot) {
    // Check if it's a draw
    if (winner == -1) {
        std::cout << "It's a draw!" << std::endl;
    } else {
        // Determine winner message
        if (is_bot) {
            std::cout << "Bot wins!" << std::endl;
        } else {
            std::cout << std::format("Player {} wins!", winner) << std::endl;
        }
    }
}


/**
 * Print the result in non-interactive mode.
 *
 * This function is used when the game is executed
 * in automated or testing environments where
 * interactive UI is disabled.
 *
 * Parameters:
 *
 *   gameResult : structure containing the final
 *                result of the game.
 */
void printResult(const GameResult& gameResult) {
    // Check if it's a draw
    if (gameResult.winner == -1) {
        std::cout << "Draw\n";
    } else if (gameResult.isBot) {
        std::cout << "Bot wins\n";
    } else {
        std::cout << std::format("Player {} wins\n", gameResult.winner);
    }

    // Print number of turns
    std::cout << std::format("Game took {} turn(s)\n", gameResult.turns);
}

/* ---------- Game Engine ---------- */
/**
 * ============================================================
 * GAME ENGINE MODULE (STUDENT IMPLEMENTATION)
 * ============================================================
 *
 * This module controls the overall flow of the game.
 *
 * Responsibilities:
 *
 *   • Setting up the game configuration
 *   • Running the main game loop
 *   • Coordinating Interaction, Renderer, Logic, and Bot modules
 *   • Producing the final GameResult
 *
 * Important design rule:
 *
 *   The GameEngine decides WHEN actions happen,
 *   but it does NOT implement the game rules itself.
 *
 * Game rules such as:
 *   - move validation
 *   - win detection
 *   - draw detection
 *
 * are handled by the GameLogic module.
 *
 * Students must implement the skeleton functions below
 * to connect all modules together.
 *
 * ============================================================
 */

void startGame(const RunConfig& config,
               GameSetup& gameSetup) {
    // TODO:
    // 1. Clear screen if interactive mode
    // if (config.interactive) clearScreen();

    // 2. Show game title
    // showSelectMenu(SelectType::TITLE_UI);

    // 3. Ask user for board size
    // repeat until valid:
    // showSelectMenu(SelectType::SIZE_UI);
    // selectSize(&gameSetup.size);

    // 4. Ask user for win condition (goal)
    // showSelectMenu(SelectType::GOAL_UI);
    // selectGoal(&gameSetup.goal, gameSetup.size);

    // 5. Ask for game mode
    // showSelectMenu(SelectType::GAME_MODE_UI);
    // selectGameMode(&gameSetup.mode);

    // 6. If mode == PVE
    // ask bot difficulty for player 2

    // 7. If mode == EVE
    // ask bot difficulty for both bots

    // 8. Initialize board
    // initBoard(gameSetup.board, gameSetup.size);
}

/**
 * Core game loop.
 *
 * Responsibilities:
 *
 *   1. Initialize game variables
 *      - current player
 *      - symbol mapping
 *      - turn counter
 *
 *   2. Repeat until game ends:
 *
 *        a) Display board
 *
 *        b) Determine current player type
 *           - human
 *           - bot
 *
 *        c) Get move
 *           - getPlayerMove() or botMove()
 *           - using measureExecutionTime()
 *
 *        d) Validate move
 *
 *        e) Apply move using makeMove()
 *
 *        f) Check win condition
 *           checkWin(...)
 *
 *        g) Check draw condition
 *           checkDraw(...)
 *
 *        h) Switch player
 *
 *   3. Return GameResult
 *
 * Return:
 *   GameResult structure containing:
 *      - winner
 *      - number of turns
 *      - whether winner is bot
 */
GameResult playGame(const RunConfig& config,
                    GameSetup& gameSetup) {
    GameResult result;

    // TODO:
    // 1. Initialize variables
    //    - current player index
    //    - symbol mapping ('X', 'O')
    //    - turn counter

    // Example idea:
    // int currentPlayer = 0;
    // char symbols[2] = {'X','O'};
    // int turns = 0;

    // 2. Main game loop
    // while(true)

    //      a) display board
    //      displayBoard(...)

    //      b) determine if player is human or bot

    //      c) get move
    //         human -> getPlayerMove()
    //         bot   -> botMove()

    //         log bot runtime -> measureExecutionTime()

    // // Running Bot Move with meansure runtime
    // pII point = measureExecutionTime(
    //     "botMove",
    //     [&]() {
    //         return botMove(gameSetup.board,
    //                        gameSetup.size,
    //                        gameSetup.goal,
    //                        symbols[player],
    //                        gameSetup.levels[player]);
    //     },
    //     TIME_ENABLED);

    //      d) validate move
    //         isValidMove(...)

    //      e) apply move
    //         makeMove(...)

    //      f) check win
    //         checkWin(...)

    //      g) check draw
    //         checkDraw(...)

    //      h) switch player
    //         currentPlayer = 1 - currentPlayer

    // 3. fill GameResult structure
    // result.winner
    // result.turns
    // result.isBot

    return result;
}

void endGame(const RunConfig& config,
             GameSetup& gameSetup,
             GameResult& gameResult) {
    // TODO:
    // 1. If interactive mode
    //      clear screen
    //      display final board
    //      show result

    // Example:
    // clearScreen();
    // displayBoard(gameSetup.board, gameSetup.size);
    // showResult(gameResult.winner, gameResult.isBot);

    // 2. If judge mode
    //      print minimal result
    // Example:
    // printResult(gameResult);

    // 3. (optional) log result using GameLogger
}

/* ---------- Game Logic ---------- */
/**
 * ============================================================
 * GAME LOGIC MODULE (STUDENT IMPLEMENTATION)
 * ============================================================
 *
 * This module implements the core rules of the game.
 *
 * Responsibilities:
 *
 *   • Validating player moves
 *   • Updating the game board
 *   • Checking win conditions
 *   • Detecting draw situations
 *   • Determining the current game state
 *
 * Important design rule:
 *
 *   The Logic module defines HOW the game rules work,
 *   but it does NOT control when actions happen.
 *
 * For example:
 *
 *   - GameEngine decides whose turn it is
 *   - Renderer prints the board
 *   - Interaction collects player input
 *   - Bot decides AI moves
 *
 * The Logic module only processes the board state
 * and determines whether moves are valid or if the
 * game has been won or drawn.
 *
 * Typical functions in this module include:
 *
 *   - checking if a move is valid
 *   - placing a move on the board
 *   - scanning the board for winning sequences
 *   - determining if the board is full
 *
 * Students must implement the skeleton functions
 * below to correctly enforce the rules of the game.
 *
 * ============================================================
 */

/**
 * Initialize board cells.
 *
 * All cells should be set to '-'
 * representing an empty cell.
 *
 * Example for size = 3:
 *
 *   - - -
 *   - - -
 *   - - -
 *
 * Hint:
 *   Use nested loops.
 */
void initBoard(char board[][BOARD_N_MAX],
               const int size) {
    // TODO: initialize board
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = '-';
        }
    }
    return;
}

bool isValidMove(const char board[][BOARD_N_MAX],
                 const int size,
                 const int row,
                 const int col) {
    // TODO: validate move
    if (row >= size || row < 0 || col >= size || col < 0 || board[row][col] != '-')
        return false;
    return true;
}

void makeMove(char board[][BOARD_N_MAX],
              const int row,
              const int col,
              const char symbol) {
    board[row][col] = symbol;
}

bool isEmptyHead(char board[][BOARD_N_MAX],
                 int size,
                 int x,
                 int y,
                 const char symbol) {
    // TODO: checking empty head, using for checkWin
    // An empty head can be:
    // - on board boundary
    // - is empty symbol ('-')
    // - equal to current symbol
    if (x == size || x == -1 || y == size || y == -1 || board[y][x] == '-' || board[y][x] == symbol)
    {
        return true;
    }
    return false;
}

/**
 * TODO:
 * Determine whether a player wins the game.
 *
 * A player wins if there are 'goal' consecutive symbols
 * in one of the following directions:
 *
 *   → horizontal
 *   ↓ vertical
 *   ↘ main diagonal
 *   ↙ anti diagonal
 *
 * Example (goal = 3):
 *
 *   X X X   -> win
 *
 * Parameters:
 *   board  : current board
 *   size   : board size
 *   symbol : player symbol ('X' or 'O')
 *   goal   : number of consecutive symbols required
 *   rule   : endpoint rule for advanced variants
 *
 * Return:
 *   true  -> player wins
 *   false -> no win detected
 */
bool checkWin(char board[][BOARD_N_MAX],
              const int size,
              const char symbol,
              const int goal,
              EndRule rule) {
    // TODO: student implementation

    return false;
}

/**
 * Determine whether the game is a draw.
 *
 * A draw occurs when:
 *
 *   - The board is full
 *   - No player has won
 *
 * Parameters:
 *   board : current board
 *   size  : board size
 *
 * Return:
 *   true  -> draw
 *   false -> game can continue
 */
bool checkDraw(char board[][BOARD_N_MAX],
               const int size) {
    // TODO: detect if the board has no empty cells
    return false;
}

/* ---------- Bot Move Logic ---------- */
/**
 * ============================================================
 * BOT MOVE LOGIC (STUDENT IMPLEMENTATION)
 * ============================================================
 *
 * Students are required to implement bot strategies for different levels.
 *
 * EASY:
 *   - Required.
 *   - Implement a simple random move selection.
 *
 * MEDIUM:
 *   - Optional but recommended.
 *   - Suggested idea: heuristic evaluation.
 *
 * HARD:
 *   - Bonus challenge.
 *   - No restriction on algorithm.
 *   - You may design your own search / evaluation strategy.
 *
 * ============================================================
 */

pII botMove(char board[][BOARD_N_MAX],
            const int size,
            const int goal,
            const char symbol,
            const BotLevel level) {
    char opponent = (symbol == 'X') ? 'O' : 'X';

    switch (level) {
        case BotLevel::EASY:
            // REQUIRED
            return random_pick(board, size);

        case BotLevel::MEDIUM:
            // OPTIONAL
            // Suggestion:
            // - try winning move
            // - block opponent winning move
            // - otherwise choose a heuristic position

            return simple_heuristic(board, size, goal, symbol, opponent);

        case BotLevel::HARD:
            // BONUS
            // Design your own strategy.
            // Possible approaches:
            //   - search algorithm
            //   - evaluation function
            //   - pattern detection

            // return hard_level(board, size, goal, symbol, opponent)

            // fallback (avoid crash)
            return random_pick(board, size);

        default:
            return random_pick(board, size);
    }
}

// Level 1
/* ---------- EASY BOT ---------- */
/**
 * TODO (Required)
 *
 * Implement a random move generator.
 *
 * Idea:
 *   1. Scan the board
 *   2. Collect all empty cells
 *   3. Randomly choose one
 *
 * Hint:
 *   board[i][j] == '-' means empty cell
 */

pII random_pick(char board[][BOARD_N_MAX],
                const int size) {
    // TODO: student implementation

    // placeholder
    return std::make_pair(-1, -1);
}

// Level 2
/* ---------- MEDIUM BOT ---------- */
/**
 * TODO (Optional)
 *
 * Implement a heuristic bot.
 *
 * Suggested strategy:
 *
 *   1. If bot has a winning move → play it
 *   2. If opponent has a winning move → block it
 *   3. Otherwise choose a good position
 *
 * Possible heuristics:
 *   - prefer center
 *   - prefer cells near existing pieces
 *   - extend current lines
 */

pII simple_heuristic(char board[][BOARD_N_MAX],
                     const int size,
                     const int goal,
                     const char botSymbol,
                     const char playerSymbol) {
    // TODO: student implementation

    // fallback
    return random_pick(board, size);
}

// Level 3
/* ---------- HARD BOT ---------- */
/**
 * BONUS CHALLENGE
 *
 * Design a stronger AI for the game.
 *
 * There is no restriction on the algorithm.
 *
 * Possible directions:
 *   - search algorithm
 *   - board evaluation
 *   - pattern recognition
 *
 * You may also combine multiple techniques.
 */

pII hard_level(char board[][BOARD_N_MAX],
               const int size,
               const int goal,
               const char botSymbol,
               const char playerSymbol) {
    // TODO: optional bonus implementation

    // fallback
    return random_pick(board, size);
}

/* ---------- Game Helper ---------- */
/**
 * ============================================================
 * HELPER FUNCTION (GIVEN)
 * ============================================================
 *
 * Measure execution time of a function.
 *
 * Example usage:
 *
 *   auto result = measureExecutionTime(
 *       "botMove",
 *       [&]() {
 *           return botMove(...);
 *       },
 *       true
 *   );
 *
 * If 'enabled = true', execution time will be printed to log.
 *
 * Students DO NOT need to modify this function.
 *
 * ============================================================
 */
template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled)
    -> std::invoke_result_t<Function> {
    // Get Return Type
    using ReturnT = std::invoke_result_t<Function>;

    // Exercute func() and Meansure Runtime
    auto start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<ReturnT>) {
        func();
        auto end = std::chrono::high_resolution_clock::now();

        if (enabled) {
            std::chrono::duration<double> duration = end - start;
            std::stringstream msg;
            msg << "execution time of " << "[" << label << "()]" << " = " << duration.count() << "s";
            GameLogger::log(msg.str(), GameLogger::Level::DEBUG);
        }
        return;

    } else {
        ReturnT result = func();
        auto end = std::chrono::high_resolution_clock::now();

        if (enabled) {
            std::chrono::duration<double> duration = end - start;
            std::stringstream msg;
            msg << "execution time of " << "[" << label << "()]" << " = " << duration.count() << "s";
            GameLogger::log(msg.str(), GameLogger::Level::DEBUG);
        }

        return result;
    }
}

/* ----------------------------------------------------- */
/* -------------------- [MAIN GAME] -------------------- */
/* ----------------------------------------------------- */
/**
 * ============================================================
 * MAIN GAME (GIVEN)
 * ============================================================
 *
 * This section contains the main game flow and core control
 * logic of the application.
 *
 * Responsibilities of this section include:
 *
 *   - Initializing the game environment
 *   - Starting the main game loop
 *   - Coordinating interaction between:
 *         + Game Engine
 *         + Game Renderer
 *         + Game Interaction
 *   - Managing turn order (player / bot)
 *   - Determining game state (running, win, draw, exit)
 *
 * The main game loop repeatedly:
 *
 *   1. Render the current board
 *   2. Receive player interaction
 *   3. Update the game state
 *   4. Let the bot perform its move
 *   5. Check win/draw conditions
 *
 * This section acts as the central controller that connects
 * all major modules together.
 *
 * ============================================================
 */
int main(int argc, char* argv[]) {
    RunConfig config = parseArgs(argc, argv);

    GameLogger::init(config.judge_mode, true, config.log_file);
    GameLogger::log("GameLogger initialized!");

    std::streambuf* cin_backup = initInteraction(config);
    GameLogger::log("GameInteraction initialized!");

    GameSetup gameSetup;
    startGame(config, gameSetup);
    GameLogger::log("GameEngine initialized!");

    GameResult gameResult = playGame(config, gameSetup);
    GameLogger::log("GameEngine playing done!");

    endGame(config, gameSetup, gameResult);
    GameLogger::log("GameEngine show endgame done!");

    closeInteraction(cin_backup);
    GameLogger::log("GameInteraction closed!");

    GameLogger::log("GameLogger closing . . .");
    GameLogger::close();

    return 0;
}

/* -------------------- [COMPILE] -------------------- */
/**
 * RECOMMENDED: Using clang++ (v21.1.4+) installed via MSYS2
 *
 * Compile (C++20 with full std::format support):
 *   clang++ -std=c++20 starter-code.cpp -o game.exe
 *
 * (optional) Enable warnings:
 *   clang++ -std=c++20 -Wall -Wextra starter-code.cpp -o game.exe
 *
 * (optional) Debug build:
 *   clang++ -std=c++20 -Wall -Wextra -g starter-code.cpp -o game.exe
 *
 * Note: Clang provides better C++20 support than GCC for std::format
 *
 * ALTERNATIVE: Using g++ (>= v15.2.0) installed via MSYS2 (MinGW64)
 *   g++ -std=gnu++20 starter-code.cpp -o game.exe
 *   (Note: Use -std=gnu++20 instead of -std=c++20)
 *
 * OPTION 2: Using "Run Build Task" in VS Code
 *
 * - Create tasks.json (compiler configuration)
 * - (optional) Enable IntelliSense (C++20 compatible):
 *   create c_cpp_properties.json
 *
 * Compile:
 *   Ctrl + Shift + B
 */