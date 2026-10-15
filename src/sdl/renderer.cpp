/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"
#include <chrono>

/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer()
{
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer()
{
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */
void SDLRenderer::init(const RunConfig &config)
{
    // read config
    screenWidth = config.screenWidth;
    screenHeight = config.screenHeight;
    boardPadding = config.boardPadding;
    // int boardPadding = config.boardPadding;
    // init(...)

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // load font
    // font = TTF_OpenFont("assets/font.ttf", font_size);
    font = TTF_OpenFont("D:/dowload/level-2/assets/VHVOGU_0.TTF", 32);
    fontTitle = TTF_OpenFont("D:/dowload/level-2/assets/VHVOGU_0.TTF", 60);
    SDL_Surface *bgSurface = IMG_Load("D:/dowload/level-2/assets/meo.png");
    if (bgSurface)
    {
        bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen()
{
    frameStart = std::chrono::high_resolution_clock::now();
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // dark background
    SDL_RenderClear(renderer);
    if (bgTexture)
    {
        SDL_Rect dst = {0, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &dst);
        drawRect(0, 0, screenWidth, screenHeight, {0, 0, 0, 120}, true);
    }
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent()
{
    SDL_RenderPresent(renderer);
    auto FrameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = FrameEnd - frameStart;
    float renderTime = duration.count();
    totalFrame++;
    totalRenderTime += renderTime;
    float avgRenderTime = totalRenderTime / totalFrame;
    float avgFps = (avgRenderTime > 0) ? (1000.0f / avgRenderTime) : 0.0f;
    float fps = (renderTime > 0) ? (1000.0f / renderTime) : 0.0f;
    std::cout << "[LOG TOTAL] Chay duoc: " << totalFrame << " frames | "
              << "Thoi gian render TB: " << avgRenderTime << " ms | "
              << "FPS trung binh: " << avgFps << "\n";
}

void SDLRenderer::renderText(const std::string &text, int x, int y, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void SDLRenderer::renderTitleText(const std::string &text, int x, int y, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(fontTitle, text.c_str(), color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context)
{
    // TODO: Render menu UI tương ứng với selectType
    if (selectType != SelectType::PLAYER_UI)
    {
        clearScreen();
    }
    std::string text;
    int textW, textH;
    switch (selectType)
    {
    case SelectType::TITLE_UI:
    {
        std::string titleText = "TIC - TAC - TOE";
        TTF_SizeText(fontTitle, titleText.c_str(), &textW, &textH);
        int titleX = screenWidth / 2 - textW / 2;
        int titleY = screenHeight / 3 - textH / 2;
        renderTitleText(titleText, titleX, titleY, {255, 215, 0, 255});

        std::string playtext = "PLAY";
        TTF_SizeText(font, playtext.c_str(), &textW, &textH);
        int nodeX = screenWidth / 2 - textW / 2;
        int nodeY = screenHeight / 2;
        drawRect(nodeX - 30, nodeY - 15, textW + 60, textH + 30, {35, 45, 60, 150}, true);
        drawRect(nodeX - 30, nodeY - 15, textW + 60, textH + 30, {255, 215, 0, 255}, false);
        renderText(playtext, nodeX, nodeY, {255, 215, 0, 255});
        break;
    }
    case SelectType::SIZE_UI:
        text = "Size Input (NxN, 3<= N <=" + std::to_string(BOARD_N_MAX) + "): ";
        break;
    case SelectType::GOAL_UI:
        text = "Goal Input (3 - " + std::to_string(context) + "): ";
        break;
    case SelectType::GAME_MODE_UI:
        text = "(1) PvP | (2) PvE | (3) EvE: ";
        break;
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        text = "(1) EASY | (2) MEDIUM | (3) HARD: ";
        break;
    case SelectType::PLAYER_UI:
        break;
    default:
        break;
    }
    if (!text.empty())
    {
        TTF_SizeText(fontTitle, text.c_str(), &textW, &textH);
        renderTitleText(text, screenWidth / 2 - textW / 2, screenHeight / 2 - textH / 2 + 1, {255, 215, 0, 255});
    }
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context)
{
    // TODO: Render thông báo lỗi
    std::string text;
    switch (selectType)
    {
    case SelectType::SIZE_UI:
        text = "Invalid size! Must be between 3 and " + std::to_string(BOARD_N_MAX);
        break;
    case SelectType::GOAL_UI:
        text = "Invalid goal! Must be between 3 and gameSize";
        break;
    case SelectType::GAME_MODE_UI:
        text = "Invalid mode! Choose 1, 2 or 3.";
        break;
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        text = "Invalid bot level! Choose 1, 2 or 3.";
        break;
    case SelectType::PLAYER_UI:
        text = "Invalid move! Please try again.";
        break;
    default:
        text = "Invalid input! please try again.";
        break;
    }
    drawRect(50, screenHeight - 60, screenWidth - 100, 40, {180, 30, 30, 200}, true);
    renderText(text, 60, screenHeight - 50, {255, 255, 255, 255});
    renderPresent();
    SDL_Delay(1500);
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context)
{
    // TODO: Render thông báo thành công
    std::string text;
    bool shouldDelay = true;
    switch (selectType)
    {
    case SelectType::SIZE_UI:
        text = "Size selected: " + std::to_string(context);

        break;
    case SelectType::GOAL_UI:
        text = "Goal selected: " + std::to_string(context);

        break;
    case SelectType::GAME_MODE_UI:
        text = "Mode selected: " + std::to_string(context + 1);

        break;
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        text = "Bot level selected: " + std::to_string(context + 1);

        break;
    default:
        shouldDelay = false;
        break;
    }
    if (!text.empty())
    {
        drawRect(50, screenHeight - 110, screenWidth - boardPadding, 60, {30, 180, 30, 200}, true);
        renderText(text, 60, screenHeight - 90, {255, 255, 255, 255});
        renderPresent();
        if (shouldDelay)
        {
            SDL_Delay(1000);
        }
    }
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size)
{
    // TODO: Render board
    gamesize = size;
    int boardsize = std::min(screenWidth, screenHeight) - boardPadding * 2;
    int cellsize = boardsize / gamesize;

    drawRect((screenWidth - boardsize) / 2 - 10, boardPadding - 10, boardsize + 20, boardsize + 20, {0, 0, 0, 120}, true);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int x = (screenWidth - boardsize) / 2 + j * cellsize;
            int y = boardPadding + i * cellsize;
            drawRect(x, y, cellsize, cellsize, {200, 200, 200, 255}, false);
            if (board[i][j] == 'X')
            {
                renderTitleText("X", x + cellsize / 2 - 19, y + cellsize / 2 - 18, {255, 50, 50, 255});
            }
            else if (board[i][j] == 'O')
            {
                renderTitleText("O", x + cellsize / 2 - 19, y + cellsize / 2 - 18, {0, 255, 0, 255});
            }
        }
    }
    if (lastmoveRow >= 0 && lastmoveCol >= 0)
    {
        int x = (screenWidth - boardsize) / 2 + lastmoveCol * cellsize;
        int y = boardPadding + lastmoveRow * cellsize;
        drawRect(x, y, cellsize, cellsize, {255, 255, 0, 80}, true);
    }

    renderPresent();
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col)
{
    // TODO: Highlight move
    lastmoveRow = row;
    lastmoveCol = col;
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove()
{
    // TODO: Render invalid move message
    drawRect(50, screenHeight - 60, screenWidth - 100, 40, {180, 30, 30, 200}, true);
    renderText("Invalid move! Cell already taken or out of range.", 60, screenHeight - 50, {255, 255, 255, 255});
    renderPresent();
}

/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot)
{
    // TODO: Render player info
    std::string text;
    if (is_bot)
    {
        text = "Bot (Player " + std::to_string(player + 1) + ") is thinking...";
    }
    else
    {
        text = "Player " + std::to_string(player + 1) + "'s turn";
    }
    renderText(text, 10, 15, {255, 215, 0, 255});
    renderPresent();
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine *winLine)
{
    // TODO: Render result
    if (winLine != nullptr)
    {
        int boardsize = std::min(screenHeight, screenWidth) - boardPadding * 2;
        int cellsize = boardsize / gamesize;
        for (auto [r, c] : winLine->cells)
        {
            int x = (screenWidth - boardsize) / 2 + c * cellsize;
            int y = boardPadding + r * cellsize;
            drawRect(x, y, cellsize, cellsize, {255, 255, 0, 120}, true);
        }
    }
    SDL_Delay(500);
    drawRect(0, 0, screenWidth, screenHeight, {0, 0, 0, 150}, true);
    renderPresent();
    SDL_Delay(800);

    std::string text;
    if (winner == DRAW_RESULT)
    {
        text = "It's a draw!";
        drawRect(boardPadding, screenHeight / 2 - 80, screenWidth - boardPadding * 2, 120, {255, 255, 0, 150}, true);
    }
    else if (is_bot)
    {
        text = "Bot (player " + std::to_string(winner + 1) + ") wins!";
        ;
        drawRect(boardPadding, screenHeight / 2 - 80, screenWidth - boardPadding * 2, 120, {255, 0, 0, 150}, true);
    }
    else
    {
        text = "Player " + std::to_string(winner + 1) + " wins!";
        drawRect(boardPadding, screenHeight / 2 - 80, screenWidth - boardPadding * 2, 110, {0, 255, 0, 150}, true);
    }
    renderTitleText(text, boardPadding + 50, screenHeight / 2 - 45, {255, 255, 255, 255});
    renderPresent();
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult &gameResult)
{
    // TODO: Print result
    std::cout << gameResult.winner << " " << gameResult.turns << "\n";
}

/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close()
{
    // if (font) {
    //     TTF_CloseFont(font);
    //     font = nullptr;
    // }
    if (font)
        TTF_CloseFont(font);
    if (fontTitle)
        TTF_CloseFont(fontTitle);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}