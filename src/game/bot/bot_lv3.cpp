/**
 * Bot LV3 cpp implementation
 *
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel3.
 */
BotLevel3::BotLevel3(const BotLevel &_level, const char &_symbol)
    : BotLevel2(_level, _symbol)
{
}

/**
 * Mô tả:
 *   Destructor của BotLevel3.
 */
BotLevel3::~BotLevel3()
{
}

/**
 * Mô tả:
 *   Trả về nước đi của bot HARD.
 *
 * TODO:
 *   - Bước 1: Gọi hard_level để tìm nước đi tối ưu
 *   - Bước 2: Nếu không tìm được → fallback về logic của BotLevel2
 */
pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal)
{
    // TODO: gọi hard_level
    // TODO: fallback nếu cần
    pII move = hard_level(board, size, goal);
    if (move.first == -1)
        return BotLevel2::getMove(board, size, goal);
    return move;
}

/**
 * Mô tả:
 *   Logic nâng cao cho bot HARD.
 *   Có thể sử dụng:
 *   - minimax
 *   - alpha-beta pruning
 *   - heuristic scoring
 *
 * TODO:
 *   - Bước 1: Duyệt các nước đi hợp lệ
 *   - Bước 2: Đánh giá từng nước (score)
 *   - Bước 3: Chọn nước có score tốt nhất
 *   - Trường hợp biên: không còn nước đi
 */

int BotLevel3::minimax(char board[][BOARD_N_MAX], const int size, const int goal, int depth, bool isMax)
{
    if (Logic::checkWin(board, size, symbol, goal))
        return 100000;
    if (Logic::checkWin(board, size, op_symbol, goal))
        return -100000;
    if (Logic::checkDraw(board, size))
        return 0;
    if (depth == 0)
    {
        int bestscore = 0;
        int dr[] = {0, 1, 1, 1};
        int dc[] = {1, 0, 1, -1};
        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
            {
                for (int d = 0; d < 4; d++)
                {
                    int countSym = 0;
                    int countOp = 0;
                    bool outOfBounds = false;
                    for (int k = 0; k < goal; k++)
                    {
                        int nr = r + k * dr[d];
                        int nc = c + k * dc[d];
                        if (nr < 0 || nr >= size || nc < 0 || nc >= size)
                        {
                            outOfBounds = true;
                            break;
                        }
                        if (board[nr][nc] == symbol)
                            countSym++;
                        else if (board[nr][nc] == op_symbol)
                        {
                            countOp++;
                        }
                    }
                    if (outOfBounds)
                        continue;
                    if (countSym > 0 && countOp > 0)
                        continue;
                    if (countSym > 0)
                    {
                        if (countSym == goal - 1)
                            bestscore += 1000;
                        else
                            bestscore += countSym * 10;
                    }
                    else if (countOp > 0)
                    {
                        if (countOp == goal - 1)
                            bestscore -= 5000;
                        else
                            bestscore -= countOp * 15;
                    }
                }
            }
        }
        return bestscore;
    }
    if (isMax)
    {
        int maxPoint = INT_MIN;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == '-')
                {
                    board[i][j] = symbol;
                    int point = minimax(board, size, goal, depth - 1, false);

                    board[i][j] = '-';
                    maxPoint = std::max(maxPoint, point);
                }
            }
        }
        return maxPoint;
    }
    else
    {
        int minPoint = INT_MAX;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == '-')
                {
                    board[i][j] = op_symbol;
                    int point = minimax(board, size, goal, depth - 1, true);
                    board[i][j] = '-';
                    minPoint = std::min(minPoint, point);
                }
            }
        }
        return minPoint;
    }
}
pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal)
{
    // TODO: implement thuật toán nâng cao
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != '-')
                continue;
            board[i][j] = symbol;
            if (Logic::checkWin(board, size, symbol, goal))
            {
                board[i][j] = '-';
                return {i, j};
            }
            board[i][j] = '-';
        }
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != '-')
                continue;
            board[i][j] = op_symbol;
            if (Logic::checkWin(board, size, op_symbol, goal))
            {
                board[i][j] = '-';
                return {i, j};
            }
            board[i][j] = '-';
        }
    }
    bool isempty = true;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != '-')
            {
                isempty = false;
            }
        }
    }
    if (isempty)
        return random_pick(board, size);
    std::vector<pII> bestmove;
    int bestscore = -200000;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {

            if (board[i][j] != '-')
                continue;
            board[i][j] = symbol;
            int points = minimax(board, size, goal, (size > 7) ? 2 : 3, false);
            board[i][j] = '-';

            if (points > bestscore)
            {
                bestscore = points;
                bestmove.clear();
                bestmove.push_back({i, j});
            }
            else if (points == bestscore)
            {
                bestmove.push_back({i, j});
            }
        }
    }
    if (bestmove.empty())
        return {-1, -1};
    std::uniform_int_distribution<int> dist(0, bestmove.size() - 1);
    return bestmove[dist(generator)];
}