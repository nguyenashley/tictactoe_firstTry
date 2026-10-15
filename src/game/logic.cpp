/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size)
{
    // TODO: implement
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = '-';
        }
    }
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col)
{
    // TODO: implement
    if (row < 0 || col < 0 || row >= size || col >= size)
        return false;
    if (board[row][col] != '-')
        return false;
    return true;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol)
{
    // TODO: implement
    board[row][col] = symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol)
{
    // TODO: implement
    if (x < 0 || x >= size || y < 0 || y >= size)
        return true;
    if (board[x][y] == '-' || board[x][y] == symbol)
        return true;
    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule)
{
    // TODO: implement
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (board[r][c] != symbol)
                continue;
            else
            {
                for (int d = 0; d < 4; d++)
                {
                    int count = 0;
                    for (int k = 0; k < goal; k++)
                    {
                        int nr = r + k * dr[d];
                        int nc = c + k * dc[d];
                        if (nr < 0 || nr >= size || nc < 0 || nc >= size)
                            break;
                        if (board[nr][nc] == symbol)
                            count++;
                        else
                            break;
                    }
                    if (count < goal)
                        continue;
                    if (rule == EndRule::NONE)
                        return true;
                    int preR = r - dr[d];
                    int preC = c - dc[d];
                    int afterR = r + goal * dr[d];
                    int afterC = c + goal * dc[d];
                    if (rule == EndRule::OPEN_ONE)
                    {
                        if (isEmptyHead(board, size, preR, preC, symbol) || isEmptyHead(board, size, afterR, afterC, symbol))
                            return true;
                    }
                    else if (rule == EndRule::OPEN_TWO)
                    {
                        if (isEmptyHead(board, size, preR, preC, symbol) && isEmptyHead(board, size, afterR, afterC, symbol))
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size)
{
    // TODO: implement
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == '-')
                return false;
        }
    }
    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule)
{
    // TODO: implement
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (board[r][c] == symbol)
            {
                for (int d = 0; d < 4; d++)
                {
                    int count = 0;
                    for (int k = 0; k < goal; k++)
                    {
                        int nr = r + k * dr[d];
                        int nc = c + k * dc[d];
                        if (nr < 0 || nr >= size || nc < 0 || nc >= size)
                            break;
                        if (board[nr][nc] == symbol)
                            count++;
                        else
                            break;
                    }
                    if (count < goal)
                        continue;
                    bool valid = false;
                    int preR = r - dr[d];
                    int preC = c - dc[d];
                    int afterR = r + goal * dr[d];
                    int afterC = c + goal * dc[d];
                    if (rule == EndRule::NONE)
                        valid = true;
                    else if (rule == EndRule::OPEN_ONE)
                    {
                        valid = (isEmptyHead(board, size, preR, preC, symbol) || isEmptyHead(board, size, afterR, afterC, symbol));
                    }
                    else if (rule == EndRule::OPEN_TWO)
                    {
                        valid = (isEmptyHead(board, size, preR, preC, symbol) && isEmptyHead(board, size, afterR, afterC, symbol));
                    }
                    if (valid)
                    {
                        WinLine winline;
                        for (int dou = 0; dou < goal; dou++)
                        {
                            winline.cells.push_back({r + dou * dr[d], c + dou * dc[d]});
                        }
                        return winline;
                    }
                }
            }
        }
    }
    return std::nullopt;
}