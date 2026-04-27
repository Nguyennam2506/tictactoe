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
#include <vector>

const std::string VERSION = "0.4.25022955";

const bool ALGORITHM_FLAG = true;
const bool TIME_ENABLED = true;

const int RANDOM_SEED = 2013;

const int BOARD_N_MAX = 12;
const int SLEEP_TIME = 1500;

std::mt19937 generator(RANDOM_SEED);

enum class BotLevel {
    EASY,
    MEDIUM,
    HARD,
    INVALID_LV
};

enum class GameMode {
    PVP,
    PVE,
    EVE,
    INVALID_MODE
};

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

enum class EndRule {
    NONE,
    OPEN_ONE,
    OPEN_TWO
};

typedef std::pair<int, int> pII;

struct RunConfig {
    bool interactive = true;
    bool judge_mode = false;
    std::string input_file;

    bool to_file = true;
    std::string log_file = "log.txt";
};

struct GameSetup {
    char board[BOARD_N_MAX][BOARD_N_MAX];
    int size;
    int goal;
    GameMode mode;
    BotLevel levels[2];
};

struct GameResult {
    int winner;
    bool isBot;
    int turns;
};

constexpr int DRAW_RESULT = -1;

namespace GameLogger {

enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    MSG,
};

inline std::string levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARN";
        case Level::ERROR: return "ERROR";
        case Level::MSG: return "";
        default: return "UNKNOWN";
    }
}
inline static Level min_level = Level::DEBUG;

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";

inline std::string getColor(Level level) {
    switch (level) {
        case Level::DEBUG: return BLUE;
        case Level::INFO: return GREEN;
        case Level::WARNING: return YELLOW;
        case Level::ERROR: return RED;
        default: return RESET;
    }
}

inline static std::ofstream log_file;
inline static bool write_to_file = false;
inline static bool is_judge_mode = false;

void init(bool judge_mode, bool to_file = true, const std::string& path = "log.txt");
void log(const std::string& msg, Level level = Level::INFO);
void close();
}

RunConfig parseArgs(int argc, char* argv[]);

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

void clearScreen();
void showSelectMenu(SelectType selectType);
void displayBoard(const char board[][BOARD_N_MAX], const int size);
void showMove(const int row, const int col);
void showInvalidMove();
void showPlayer(const int player, const bool is_bot);
void showResult(const int winner, const bool is_bot);
void printResult(const GameResult& gameResult);

void startGame(const RunConfig& config, GameSetup& gameSetup);
GameResult playGame(const RunConfig& config, GameSetup& gameSetup);
void endGame(const RunConfig& config, GameSetup& gameSetup, GameResult& gameResult);

void initBoard(char board[][BOARD_N_MAX], const int size);
bool isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col);
void makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol);
bool isEmptyHead(char board[][BOARD_N_MAX], int size, int x, int y, const char symbol);
bool checkWin(char board[][BOARD_N_MAX], const int size, const int row, const int col, const char symbol, const int goal, EndRule rule = EndRule::OPEN_TWO);
bool checkDraw(char board[][BOARD_N_MAX], const int size);

pII botMove(char board[][BOARD_N_MAX], const int size, const int goal, const char symbol, const BotLevel level);
pII random_pick(char board[][BOARD_N_MAX], const int size);
pII simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);
pII hard_level(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);

template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled) -> std::invoke_result_t<Function>;

void GameLogger::init(bool judge_mode, bool to_file, const std::string& path) {
    write_to_file = to_file;
    is_judge_mode = judge_mode;

    if (write_to_file) {
        log_file.open(path, std::ios::out | std::ios::trunc);
        if (!log_file.is_open()) {
            std::cerr << "[Logger] Cannot open log file: " << path << ". Falling back to console only." << std::endl;
            write_to_file = false;
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
    if (static_cast<int>(level) < static_cast<int>(min_level)) {
        return;
    }

    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    std::stringstream ss_lv;
    std::string formatted_lv;
    if (level != Level::MSG) {
        ss_lv << "[" << levelToString(level) << "]";
        formatted_lv = ss_lv.str();
    }

    std::stringstream ss_msg;
    ss_msg << (formatted_lv.empty() ? "" : " - ") << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] ";
    ss_msg << msg;
    std::string formatted_msg = ss_msg.str();

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

RunConfig parseArgs(int argc, char* argv[]) {
    RunConfig config;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-j" || arg == "--judge") {
            config.judge_mode = true;
            config.interactive = false;
        } else if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            config.input_file = argv[++i];
        } else if ((arg == "-l" || arg == "--log") && i + 1 < argc) {
            config.log_file = argv[++i];
            if (config.log_file == "") {
                config.to_file = false;
            }
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Tic-tac-toe Game (Version: " << VERSION << ")\n";
            std::cout << "Usage: \n"
                      << "\t--judge, -j\tEnable judge mode (no UI, standard output only)\n"
                      << "\t--input, -i\tPath to input file\n"
                      << "\t--log, -l\tPath to log file (default: log.txt)\n";
            exit(0);
        }
    }
    return config;
}

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

void closeInteraction(std::streambuf* cin_backup) {
    if (cin_backup) {
        std::cin.rdbuf(cin_backup);
        GameLogger::log("fallback using 'std::cin' input stream.");
    }
    if (GameInteraction::global_file_in.is_open()) {
        GameInteraction::global_file_in.close();
    }
}

bool validateInput(std::string& input) {
    if (input.empty()) {
        return false;
    }
    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool getInput(int* val) {
    std::string input;
    std::cin >> input;
    if (!validateInput(input)) {
        return false;
    }
    *val = std::stoi(input);
    return true;
}

bool selectSize(int* size) {
    int input;
    if (!getInput(&input)) {
        return false;
    }
    if (input < 3 || input > BOARD_N_MAX) {
        return false;
    }
    *size = input;
    return true;
}

bool selectGoal(int* goal, const int size) {
    int input;
    if (!getInput(&input)) {
        return false;
    }
    if (input < 3 || input > size) {
        return false;
    }
    *goal = input;
    return true;
}

bool selectGameMode(GameMode* mode) {
    int input;
    if (!getInput(&input)) {
        return false;
    }
    if (input < 1 || input > 3) {
        return false;
    }
    switch (input) {
        case 1: *mode = GameMode::PVP; break;
        case 2: *mode = GameMode::PVE; break;
        case 3: *mode = GameMode::EVE; break;
    }
    return true;
}

bool selectBotLevel(BotLevel* levels, const int index) {
    int input;
    if (!getInput(&input)) {
        return false;
    }
    if (input < 1 || input > 3) {
        return false;
    }
    switch (input) {
        case 1: levels[index] = BotLevel::EASY; break;
        case 2: levels[index] = BotLevel::MEDIUM; break;
        case 3: levels[index] = BotLevel::HARD; break;
    }
    return true;
}

bool getPlayerMove(int* row, int* col) {
    std::string input;
    do {
        std::getline(std::cin, input);
    } while (input.empty());

    std::istringstream stream(input);
    int x, y;
    char extra;

    if (stream >> x >> y && !(stream >> extra)) {
        *row = x;
        *col = y;
        return true;
    }
    return false;
}

void clearScreen() {
    std::cout << "\n\x1B[2J\x1B[H\n";
}

void showSelectMenu(SelectType selectType) {
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;
        case SelectType::SIZE_UI:
            std::cout << std::format(">----- Select Size (NxN, 3 <= N <= {}) -----<\n\n", BOARD_N_MAX);
            break;
        case SelectType::GOAL_UI:
            std::cout << ">----- Select Goal (3 - 5, goal <= size) -----<\n\n";
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << ">----- Select Game Mode ( >1< PvP | >2< PvE | >3< EvE) -----<\n\n";
            break;
        case SelectType::BOT_LEVEL_UI:
            std::cout << ">----- Select Bot Level ( >1< EASY | >2< MEDIUM | >3< HARD) -----<\n\n";
            break;
        case SelectType::PLAYER_UI:
            std::cout << ">----- Select Position ( \"row col\" E.g. \"1 2\") -----<\n\n";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << ">----- Select Multiple Bot Level ( \"bot1_level bot2_level\" E.g. \"1 2\") -----<\n\n";
            break;
        default:
            break;
    }
}

void displayBoard(const char board[][BOARD_N_MAX], const int size) {
    for (int i = 0; i < size + 3; i++) {
        for (int j = 0; j < size + 1; j++) {
            if (i == 0) {
                if (j == 0) std::cout << "   0   ";
                else if (j == size) std::cout << "\n";
                else std::cout << ((j < 10) ? std::format("{}   ", j) : std::format("{}  ", j));
            } else if (i == 1 || i == size + 2) {
                std::cout << "  " << std::string(size * 4 - 1, '-') << "\n";
                break;
            } else {
                if (j == 0) std::cout << ((i - 2 < 10) ? std::format("{} |", i - 2) : std::format("{}|", i - 2));
                else if (j == size) {
                    if (i == size + 1) std::cout << board[i - 2][j - 1] << "|\n";
                    else std::cout << board[i - 2][j - 1] << "|\n\n";
                } else std::cout << std::format("{}   ", board[i - 2][j - 1]);
            }
        }
    }
}

void showPlayer(int player, bool is_bot) {
    if (is_bot) {
        std::cout << std::format("Bot (Player {}) is thinking...\n", player);
    } else {
        std::cout << std::format("Player {}'s turn \n", player);
    }
}

void showMove(const int row, const int col) {
    std::cout << std::format("Move played at R:{} C:{}\n", row, col);
}

void showInvalidMove() {
    std::cout << "Invalid Move! (Already selected or Out of range)\n";
}

void showResult(const int winner, const bool is_bot) {
    if (winner == -1) {
        std::cout << "It's a draw!" << std::endl;
    } else {
        if (is_bot) std::cout << "Bot wins!" << std::endl;
        else std::cout << std::format("Player {} wins!", winner) << std::endl;
    }
}

void printResult(const GameResult& gameResult) {
    if (gameResult.winner == -1) {
        std::cout << "Draw\n";
    } else if (gameResult.isBot) {
        std::cout << "Bot wins\n";
    } else {
        std::cout << std::format("Player {} wins\n", gameResult.winner);
    }
    std::cout << std::format("Game took {} turn(s)\n", gameResult.turns);
}

void startGame(const RunConfig& config, GameSetup& gameSetup) {
    if (config.interactive) {
        clearScreen();
        showSelectMenu(SelectType::TITLE_UI);
    }

    do {
        if (config.interactive) showSelectMenu(SelectType::SIZE_UI);
    } while (!selectSize(&gameSetup.size));

    do {
        if (config.interactive) showSelectMenu(SelectType::GOAL_UI);
    } while (!selectGoal(&gameSetup.goal, gameSetup.size));

    do {
        if (config.interactive) showSelectMenu(SelectType::GAME_MODE_UI);
    } while (!selectGameMode(&gameSetup.mode));

    if (gameSetup.mode == GameMode::PVE) {
        do {
            if (config.interactive) showSelectMenu(SelectType::BOT_LEVEL_UI);
        } while (!selectBotLevel(gameSetup.levels, 1));
    } else if (gameSetup.mode == GameMode::EVE) {
        do {
            if (config.interactive) showSelectMenu(SelectType::MUL_BOT_LEVEL_UI);
        } while (!selectBotLevel(gameSetup.levels, 0) || !selectBotLevel(gameSetup.levels, 1));
    }

    initBoard(gameSetup.board, gameSetup.size);
}

GameResult playGame(const RunConfig& config, GameSetup& gameSetup) {
    GameResult result;
    int currentPlayer = 0;
    char symbols[2] = {'X', 'O'};
    int turns = 0;

    while (true) {
        if (config.interactive) {
            displayBoard(gameSetup.board, gameSetup.size);
        }

        bool isBot = false;
        if (gameSetup.mode == GameMode::EVE) {
            isBot = true;
        } else if (gameSetup.mode == GameMode::PVE && currentPlayer == 1) {
            isBot = true;
        }

        if (config.interactive) {
            showPlayer(currentPlayer + 1, isBot);
        }

        int row, col;
        bool valid = false;

        do {
            if (!isBot) {
                if (config.interactive) showSelectMenu(SelectType::PLAYER_UI);

                if (getPlayerMove(&row, &col)) {
                    valid = isValidMove(gameSetup.board, gameSetup.size, row, col);
                } else {
                    valid = false;
                }

                if (!valid && config.interactive) {
                    showInvalidMove();
                }
            } else {
                pII point = measureExecutionTime(
                    "botMove",
                    [&]() {
                        return botMove(gameSetup.board, gameSetup.size, gameSetup.goal, symbols[currentPlayer], gameSetup.levels[currentPlayer]);
                    },
                    TIME_ENABLED);

                row = point.first;
                col = point.second;
                valid = isValidMove(gameSetup.board, gameSetup.size, row, col);

                if (config.interactive) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
                }
            }
        } while (!valid);

        makeMove(gameSetup.board, row, col, symbols[currentPlayer]);

        if (config.interactive) {
            showMove(row, col);
        }

        turns++;

        if (checkWin(gameSetup.board, gameSetup.size, row, col, symbols[currentPlayer], gameSetup.goal, EndRule::NONE)) {
            result.winner = currentPlayer + 1;
            result.isBot = isBot;
            result.turns = turns;
            break;
        }

        if (checkDraw(gameSetup.board, gameSetup.size)) {
            result.winner = DRAW_RESULT;
            result.isBot = false;
            result.turns = turns;
            break;
        }

        currentPlayer = 1 - currentPlayer;
    }

    return result;
}

void endGame(const RunConfig& config, GameSetup& gameSetup, GameResult& gameResult) {
    if (config.interactive) {
        clearScreen();
        displayBoard(gameSetup.board, gameSetup.size);
        showResult(gameResult.winner, gameResult.isBot);
    }

    if (config.judge_mode) {
        if (gameResult.winner == DRAW_RESULT) {
            std::cout << gameResult.winner << " " << gameResult.turns << "\n";
        } else {
            std::cout << gameResult.winner - 1 << " " << gameResult.turns << "\n";
        }
    }

    std::string log_msg;
    if (gameResult.winner == DRAW_RESULT) {
        log_msg = "Game ended in a draw after " + std::to_string(gameResult.turns) + " turns.";
    } else {
        std::string playerType = gameResult.isBot ? "Bot" : "Human";
        log_msg = "Game ended. Player " + std::to_string(gameResult.winner) + " (" + playerType + ") won after " + std::to_string(gameResult.turns) + " turns.";
    }
    GameLogger::log(log_msg, GameLogger::Level::INFO);
}

void initBoard(char board[][BOARD_N_MAX], const int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = '-';
        }
    }
}

bool isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    if (row >= size || row < 0 || col >= size || col < 0 || board[row][col] != '-')
        return false;
    return true;
}

void makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    board[row][col] = symbol;
}

bool isEmptyHead(char board[][BOARD_N_MAX], int size, int x, int y, const char symbol) {
    if (x >= size || x < 0 || y >= size || y < 0) {
        return true;
    }
    if (board[y][x] == '-') {
        return true;
    }
    return false;
}

bool checkWin(char board[][BOARD_N_MAX], const int size, const int row, const int col, const char symbol, const int goal, EndRule rule) {
    int axes[4][2][2] = {
        {{0, -1}, {0, 1}},
        {{-1, 0}, {1, 0}},
        {{-1, -1}, {1, 1}},
        {{-1, 1}, {1, -1}}
    };

    for (int i = 0; i < 4; i++) {
        int count = 1;
        int head_r[2];
        int head_c[2];

        for (int dir = 0; dir < 2; dir++) {
            int dx = axes[i][dir][0];
            int dy = axes[i][dir][1];
            int r = row + dx;
            int c = col + dy;

            while (r >= 0 && r < size && c >= 0 && c < size && board[r][c] == symbol) {
                count++;
                r += dx;
                c += dy;
            }
            head_r[dir] = r;
            head_c[dir] = c;
        }

        if (count >= goal) {
            if (rule == EndRule::NONE) {
                return true;
            } else {
                bool empty1 = isEmptyHead(board, size, head_c[0], head_r[0], symbol);
                bool empty2 = isEmptyHead(board, size, head_c[1], head_r[1], symbol);

                if (rule == EndRule::OPEN_ONE && (empty1 || empty2)) {
                    return true;
                }
                if (rule == EndRule::OPEN_TWO && (empty1 && empty2)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkDraw(char board[][BOARD_N_MAX], const int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') return false;
        }
    }
    return true;
}

pII botMove(char board[][BOARD_N_MAX], const int size, const int goal, const char symbol, const BotLevel level) {
    char opponent = (symbol == 'X') ? 'O' : 'X';
    switch (level) {
        case BotLevel::EASY: return random_pick(board, size);
        case BotLevel::MEDIUM: return simple_heuristic(board, size, goal, symbol, opponent);
        case BotLevel::HARD: return random_pick(board, size);
        default: return random_pick(board, size);
    }
}

pII random_pick(char board[][BOARD_N_MAX], const int size) {
    std::vector<pII> empty_cells;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') {
                empty_cells.push_back(std::make_pair(i, j));
            }
        }
    }
    if (empty_cells.empty()) return std::make_pair(-1, -1);
    std::uniform_int_distribution<int> distribution(0, empty_cells.size() - 1);
    return empty_cells[distribution(generator)];
}

pII simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') {
                board[i][j] = botSymbol;
                if (checkWin(board, size, i, j, botSymbol, goal, EndRule::NONE)) {
                    board[i][j] = '-';
                    return std::make_pair(i, j);
                }
                board[i][j] = '-';
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') {
                board[i][j] = playerSymbol;
                if (checkWin(board, size, i, j, playerSymbol, goal, EndRule::NONE)) {
                    board[i][j] = '-';
                    return std::make_pair(i, j);
                }
                board[i][j] = '-';
            }
        }
    }

    int centerRow = size / 2;
    int centerCol = size / 2;
    if (board[centerRow][centerCol] == '-') {
        return std::make_pair(centerRow, centerCol);
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') {
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < size && nj >= 0 && nj < size &&
                            (board[ni][nj] == botSymbol || board[ni][nj] == playerSymbol)) {
                            return std::make_pair(i, j);
                        }
                    }
                }
            }
        }
    }
    return random_pick(board, size);
}

pII hard_level(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
    return random_pick(board, size);
}

template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled) -> std::invoke_result_t<Function> {
    using ReturnT = std::invoke_result_t<Function>;
    auto start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<ReturnT>) {
        func();
        auto end = std::chrono::high_resolution_clock::now();
        if (enabled) {
            std::chrono::duration<double> duration = end - start;
            std::stringstream msg;
            msg << "execution time of [" << label << "()] = " << duration.count() << "s";
            GameLogger::log(msg.str(), GameLogger::Level::DEBUG);
        }
        return;
    } else {
        ReturnT result = func();
        auto end = std::chrono::high_resolution_clock::now();
        if (enabled) {
            std::chrono::duration<double> duration = end - start;
            std::stringstream msg;
            msg << "execution time of [" << label << "()] = " << duration.count() << "s";
            GameLogger::log(msg.str(), GameLogger::Level::DEBUG);
        }
        return result;
    }
}

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