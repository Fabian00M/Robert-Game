#include "Windows.h"
#include "BitMap.h"
#include "GameEngine.h"
#include "Resource.h"
#include <string>
#include <algorithm>

using namespace std;

GameEngine* game;
BitMap* cards[10][10];
bool matchCard[10][10];
int cardWidth, cardHeight;  // Declare globally
BitMap* wowzer;
int wowzerWidth, wowzerHeight; // Dimensions of wowzer
int wowzerX = 0;  // X position of wowzer in the grid
int wowzerY = 0;  // Y position of wowzer in the grid
int bitmapSpacing = 1;  // Default spacing in pixels
int wowzerLives = 3;  // Global variable for wowzer's lives

// Book bitmaps and positions
BitMap* books[3];
Sprite* bookSprites[3]; // Sprites for books
Sprite* wowzerSprite; //Sprite for wowzer

BOOL GameInitialize(HINSTANCE currInstance) {
    game = new GameEngine(currInstance, L"R-bert", L"R-bert", IDI_WANDERER, IDI_WANDERER_SM, 540, 540);
    if (game == NULL) {
        return FALSE;
    }
    game->setFrameRate(15);
    return TRUE;
}

void GameStart(HWND hwnd) {
    srand(GetTickCount());
    HDC hdc = GetDC(hwnd);
    BitMap blue(hdc, L"Res/Blue.bmp");  // Load card back
    BitMap red(hdc, L"Res/Red.bmp");    // Load card front
    wowzer = new BitMap(hdc, L"Res/Wowzer.bmp");  // Load the Wowzer bitmap image
    wowzerLives = 3;  // Initialize wowzer's lives

    // Assign global card dimensions
    cardWidth = red.getWidth();
    cardHeight = red.getHeight();
    wowzerWidth = wowzer->getWidth(); // Set wowzer dimensions
    wowzerHeight = wowzer->getHeight();

    // Initialize cards and their flipped state
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            cards[i][j] = new BitMap(hdc, L"Res/Red.bmp"); // All cards start with the front face 'Red'
            matchCard[i][j] = false; // Initially, all cards are not flipped
        }
    }

    // Load book bitmaps and initialize positions and velocities as sprites

    books[0] = new BitMap(hdc, L"Res/book1.bmp");
    books[1] = new BitMap(hdc, L"Res/book1.bmp");
    books[2] = new BitMap(hdc, L"Res/book1.bmp");

    RECT bookBounds = { 0, 0, game->getWidth(), game->getHeight() };
    for (int i = 0; i < 3; ++i) {
        bookSprites[i] = new Sprite(books[i], bookBounds, BA_WRAP); // Create book sprites with bounce behavior
        bookSprites[i]->setVelocity(rand() % 1 + 1, rand() % 18 + 1); // Set random initial velocities
        game->addSprite(bookSprites[i]); // Add book sprites to the game
    }

    // Create the wowzer sprite with slightly larger bounds than the bitmap itself
    RECT wowzerBounds = { 0, 0, wowzer->getWidth() + 1, wowzer->getHeight() + 1 };
    wowzerSprite = new Sprite(wowzer, wowzerBounds, BA_WRAP);
    wowzerSprite->setVelocity(0, 0); // Wowzer might not move initially or could be controlled by other inputs
    RECT clientRect;
    GetClientRect(game->getWnd(), &clientRect);
    int totalGridWidth = 10 * (cardWidth + bitmapSpacing) - bitmapSpacing;  // Total width of all cards + spacing between them
    int totalGridHeight = 10 * (cardHeight + bitmapSpacing) - bitmapSpacing; // Total height of all cards + spacing between them

    int startX = (clientRect.right - totalGridWidth) / 2;  // Start x-coordinate centered
    int startY = (clientRect.bottom - totalGridHeight) / 2; // Start y-coordinate centered
    wowzerSprite->setPosition(startX, startY);
    game->addSprite(wowzerSprite);

    ReleaseDC(hwnd, hdc);
    InvalidateRect(hwnd, NULL, TRUE);
}


void GameLoop() {
    HandleKeys();

    // Update sprite positions
    game->updateSprites();

    // Check for collisions between wowzer and each book sprite
    /*for (int i = 0; i < 3; ++i) {
        if (SpriteCollision(wowzerSprite, bookSprites[i])) {
            // If there's a collision, the response is handled within the SpriteCollision function
        }
    }*/

    // Check if all cards are flipped
    bool allFlipped = true;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (!matchCard[i][j]) {
                allFlipped = false;
                break;
            }
        }
        if (!allFlipped) break;
    }

    if (allFlipped) {
        MessageBox(NULL, L"Congratulations! You have won the game!", L"Game Over", MB_OK);
        GameEnd();
    }

    InvalidateRect(game->getWnd(), NULL, TRUE);
}

void GameEnd() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            delete cards[i][j];
        }
    }
    delete wowzer;
    memset(cards, 0, sizeof(cards));

    // Delete book bitmaps and sprites
    for (int i = 0; i < 3; ++i) {
        delete books[i];
        delete bookSprites[i];
    }

    PostQuitMessage(0);
}

void GamePaint(HDC hdc) {
    RECT clientRect;
    GetClientRect(game->getWnd(), &clientRect);
    HBRUSH hBrush = CreateSolidBrush(RGB(38, 68, 182));
    FillRect(hdc, &clientRect, hBrush);
    DeleteObject(hBrush);

    BitMap blue(hdc, L"Res/Blue.bmp");

    int totalGridWidth = 10 * (cardWidth + bitmapSpacing) - bitmapSpacing;  // Total width of all cards + spacing between them
    int totalGridHeight = 10 * (cardHeight + bitmapSpacing) - bitmapSpacing; // Total height of all cards + spacing between them

    int startX = (clientRect.right - totalGridWidth) / 2;  // Start x-coordinate centered
    int startY = (clientRect.bottom - totalGridHeight) / 2; // Start y-coordinate centered

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            int x = startX + i * (cardWidth + bitmapSpacing);
            int y = startY + j * (cardHeight + bitmapSpacing);
            if (matchCard[i][j]) {
                cards[i][j]->draw(hdc, x, y, false);
            }
            else {
                blue.draw(hdc, x, y, false);
            }
        }
    }

    // Draw wowzer at the current position
    wowzerSprite->setPosition(startX + wowzerX * (cardWidth + bitmapSpacing), startY + wowzerY * (cardHeight + bitmapSpacing));
   // wowzer->draw(hdc, startX + wowzerX * (cardWidth + bitmapSpacing), startY + wowzerY * (cardHeight + bitmapSpacing), false);

    game->drawSprites(hdc);

}

void HandleKeys() {
    static DWORD lastMoveTime = 0;
    DWORD currentTime = GetTickCount();

    if (currentTime - lastMoveTime < 250) { // 200 milliseconds delay between moves
        return;
    }

    bool moved = false;  // Track whether a move occurred

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (wowzerX > 0) {
            wowzerX--;
            moved = true;
        }
    }

    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (wowzerX < 9) {
            wowzerX++;
            moved = true;
        }
    }
    else if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (wowzerY > 0) {
            wowzerY--;
            moved = true;
        }
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (wowzerY < 9) {
            wowzerY++;
            moved = true;
        }
    }

    if (moved) {
        matchCard[wowzerX][wowzerY] = !matchCard[wowzerX][wowzerY];
        lastMoveTime = currentTime;
        InvalidateRect(game->getWnd(), NULL, TRUE); // Redraw the game window
    }
}

void GameActivate(HWND hwnd) {
    // Placeholder for window activation code
}

void GameDeactivate(HWND hwnd) {
    // Placeholder for window deactivation code
}

void MouseButtonDown(int x, int y, bool left) {
    // Placeholder for mouse button down handling
}

void MouseButtonUp(int x, int y, bool left) {
    // Placeholder for mouse button up handling
}

void MouseMove(int x, int y) {
    // Placeholder for mouse move handling
}

bool SpriteCollision(Sprite* hitter, Sprite* hittee) {
    RECT hitterBounds = hitter->getCollision();
    RECT hitteeBounds = hittee->getCollision();
    if(hittee == wowzerSprite)
    {
        //for (int i = 0; i < 3; ++i) {
        //if (hitter == wowzerSprite && hittee == bookSprites[i]) {
        //    if (hitterBounds.right >= hitteeBounds.left && hitterBounds.left <= hitteeBounds.right &&
        //        hitterBounds.bottom >= hitteeBounds.top && hitterBounds.top <= hitteeBounds.bottom) {
                // Collision detected, reverse the vertical velocity
                POINT vel = hitter->getVelocity();
                vel.y = -vel.y;
                hitter->setVelocity(vel);

                // Reduce wowzer's lives and check game over condition
                if (--wowzerLives <= 0) {
                    MessageBox(NULL, L"Wowzer has no more lives!", L"Game Over", MB_OK);
                    GameEnd();
                }
                else {
                    MessageBox(NULL, L"Wowzer was hit by a book!", L"Alert", MB_OK);
                }
                return true;
       //     }
       // }
    }
    return false;
}
