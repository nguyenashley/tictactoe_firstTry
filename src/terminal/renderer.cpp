/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer()
{
    // TODO: init state
    // không cần khởi tạo gì
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer()
{
    // TODO: cleanup nếu cần
    // không có tài nguyên nào cần giải phóng
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig &config)
{
    // TODO: init renderer theo config
    // terminal dùng cout có sẵn, không cần setup gì cả
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen()
{
    // TODO: clear screen
    std::cout << "\n"
              << "\x1B[2J\x1B[H" << "\n";
    // trong ANSI escape sequence "\x1B[2J" là xóa màn hình
    // "\x1B[H" là đưa con trỏ về góc bên trái
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context)
{
    // TODO: render menu theo loại
    switch (selectType)
    {
    case SelectType::TITLE_UI:
        std::cout << ">----- Tic-tac-toe [Terminal] -----<\n";
        break;
    case SelectType::SIZE_UI:
        std::cout << "Size Input (NxN, 3<=N<=" << BOARD_N_MAX << "):";
        break;
    case SelectType::GOAL_UI:
        std::cout << "Goal Input (3 - " << context << "): ";
        break;
    case SelectType::GAME_MODE_UI:
        std::cout << "(1) PvP | (2)PvE | (3) EvE: ";
        break;
    case SelectType::BOT_LEVEL_UI:
        std::cout << "(1) EASY | (2) MEDIUM | (3) HARD: ";
        break;
    case SelectType::PLAYER_UI:
        std::cout << "(row, col): ";
        break;
    case SelectType::MUL_BOT_LEVEL_UI:
        std::cout << "(1) EASY | (2) MEDIUM | (3) HARD: ";
        break;
    default:
        break;
    }
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context)
{
    // TODO: render invalid message
    switch (selectType)
    {
    case SelectType::SIZE_UI:
        std::cout << "Invalid size! Must be between 3 and " << BOARD_N_MAX << ".\n";
        break;
    case SelectType::GOAL_UI:
        std::cout << "Invalid goal! Must be between 3 and " << context << ".\n";
        break;
    case SelectType::GAME_MODE_UI:
        std::cout << "Invalid mode! Choose 1, 2 or 3.\n";
        break;
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        std::cout << "Invalid bot level! Choose 1, 2 or 3.\n";
        break;
    case SelectType::PLAYER_UI:
        std::cout << "Invalid move! Please try again.\n";
        break;
    default:
        std::cout << "Invalid input, please try again.\n";
        break;
    }
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context)
{
    // TODO: render success message
    switch (selectType)
    {
    case SelectType::SIZE_UI:
        std::cout << "Size selected: " << context << "\n";
        break;
    case SelectType::GOAL_UI:
        std::cout << "Goal selected: " << context << "\n";
        break;
    case SelectType::GAME_MODE_UI:
        std::cout << "Game mode selected: " << context << "\n";
        break;
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        std::cout << "Bot level selected: " << context << "\n";
        break;
    default:
        break;
    }
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size)
{
    // TODO: render board
    std::cout << "   ";
    for (int i = 0; i < size; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
    std::cout << "  ";
    for (int i = 0; i < size * 2 + 1; i++)
    {
        std::cout << "-";
    }
    std::cout << "\n";
    for (int i = 0; i < size; i++)
    {
        std::cout << i << " |";
        for (int j = 0; j < size; j++)
        {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot)
{
    // TODO: render player info
    if (is_bot)
    {
        std::cout << "Bot (Player " << (player + 1) << ") is thinking...\n";
    }
    else
    {
        std::cout << "Player " << (player + 1) << "'s turn\n";
    }
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col)
{
    // TODO: render move
    std::cout << "Move placed at (" << row << ", " << col << ").\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove()
{
    // TODO: render invalid move message
    std::cout << "Invalid move!\n";
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine *winLine)
{
    // TODO: render result
    if (winner == DRAW_RESULT)
    {
        std::cout << "It's a draw!\n";
    }
    else if (is_bot)
    {
        std::cout << "Bot (Player " << (winner + 1) << ") wins!\n";
    }
    else
    {
        std::cout << "Player " << (winner + 1) << "win!\n";
    }
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult &gameResult)
{
    // TODO: print result đơn giản
    std::cout << gameResult.winner << " " << gameResult.turns << "\n";
}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close()
{
    // TODO: cleanup renderer
    // không có tài nguyên nào giải phóng
}