#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

const int width = 40;
const int height = 20;

// Game state
int playerY, computerY, ballX, ballY;
int ballDirX, ballDirY;
int playerScore = 0, computerScore = 0;
int winningScore = 5; // Default
bool running = false;

// Difficulty settings
int frameDelay = 100;
int cpuReactChance = 2;
int paddleStep = 1;

void gotoxy(int x, int y) {
    COORD coord = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void drawMidLine() {
    for (int i = 1; i < height; i++) {
        if (i % 2 == 0) {
            gotoxy(width / 2, i);
            cout << "|";
        }
    }
}

void drawBorder() {
    system("cls");
    for (int i = 0; i <= height; i++) {
        for (int j = 0; j <= width; j++) {
            if (j == 0 || j == width)
                cout << "|";
            else if (i == 0 || i == height)
                cout << "-";
            else if (j == width / 2 && i % 2 == 0)
                cout << "|";
            else
                cout << " ";
        }
        cout << endl;
    }
}

void drawPaddles() {
    for (int i = 1; i < height; i++) {
        gotoxy(2, i); cout << " ";
        gotoxy(width - 2, i); cout << " ";
    }

    for (int i = -1; i <= 2; i++) {
        if (playerY + i > 0 && playerY + i < height)
            gotoxy(2, playerY + i), cout << "|";
    }

    for (int i = -1; i <= 2; i++) {
        if (computerY + i > 0 && computerY + i < height)
            gotoxy(width - 2, computerY + i), cout << "|";
    }
}

void drawBall() {
    gotoxy(ballX, ballY); cout << "O";
}

void clearBall() {
    gotoxy(ballX, ballY); cout << " ";
}

void drawScore() {
    gotoxy(width / 2 - 10, 0);

    setColor(10); cout << "Player";
    setColor(7);  cout << ": ";
    setColor(14); cout << playerScore;

    setColor(7);  cout << "  ";

    setColor(12); cout << "CPU";
    setColor(7);  cout << ": ";
    setColor(14); cout << computerScore;

    setColor(7);
}

void resetBall() {
    ballX = width / 2;
    ballY = height / 2;
    ballDirX = (rand() % 2 == 0) ? -1 : 1;
    ballDirY = (rand() % 2 == 0) ? -1 : 1;
}

void resetGame() {
    playerY = height / 2;
    computerY = height / 2;
    playerScore = 0;
    computerScore = 0;
    resetBall();
    drawBorder();
    drawScore();
}

void selectDifficulty() {
    system("cls");
    cout << "\n\n";
    cout << "    Select Difficulty:\n";
    cout << "    ------------------\n";
    cout << "    1. Easy\n";
    cout << "    2. Medium\n";
    cout << "    3. Hard\n\n";
    cout << "    Enter choice (1-3): ";

    char choice;
    while (true) {
        choice = _getch();
        if (choice == '1') {
            frameDelay = 130;
            cpuReactChance = 3;
            paddleStep = 1;
            break;
        }
        else if (choice == '2') {
            frameDelay = 100;
            cpuReactChance = 2;
            paddleStep = 2;
            break;
        }
        else if (choice == '3') {
            frameDelay = 70;
            cpuReactChance = 1;
            paddleStep = 2;
            break;
        }
    }
}

void selectWinningScore() {
    system("cls");
    cout << "\n\n";
    cout << "   Enter Winning Score (any number > 0): ";
    int score = 0;

    while (true) {
        cin >> score;
        if (cin.fail() || score < 1) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "   Invalid input. Please enter a number greater than 0: ";
        }
        else {
            winningScore = score;
            break;
        }
    }
}

void gameMenu() {
    system("cls");
    cout << "\n\n";
    cout << "  ==========================================\n";
    cout << "  ||         V I B E   C O D E D           ||\n";
    cout << "  ||             P O N G !                 ||\n";
    cout << "  ==========================================\n";
    cout << "\n";
    cout << "     Use UP/DOWN arrows to move paddle\n";
    cout << "     First to reach target score wins.\n\n";
    cout << "     [ PRESS P TO PLAY ]\n";

    while (true) {
        if (_kbhit() && tolower(_getch()) == 'p') break;
    }

    selectDifficulty();
    selectWinningScore();
}

bool ballHitsPaddle(int nextX, int nextY, int paddleX, int paddleY) {
    return (nextX == paddleX && nextY >= paddleY - 1 && nextY <= paddleY + 2);
}

void logic() {
    clearBall();

    int nextX = ballX + ballDirX;
    int nextY = ballY + ballDirY;

    if (ballHitsPaddle(nextX, nextY, 3, playerY) || ballHitsPaddle(nextX, nextY, width - 3, computerY)) {
        ballDirX *= -1;
    }

    ballX += ballDirX;
    ballY += ballDirY;

    if (ballY == 1 || ballY == height - 1)
        ballDirY *= -1;

    if (ballX <= 1) {
        computerScore++;
        resetBall();
        Sleep(1000);
    }
    if (ballX >= width - 1) {
        playerScore++;
        resetBall();
        Sleep(1000);
    }

    // CPU logic
    if (rand() % cpuReactChance == 0) {
        if (computerY < ballY)
            computerY += paddleStep;
        else if (computerY > ballY)
            computerY -= paddleStep;

        if (computerY < 2) computerY = 2;
        if (computerY > height - 2) computerY = height - 2;
    }

    drawScore();
    drawMidLine();
    drawPaddles();
    drawBall();
}

void input() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 72 && playerY > 1) playerY -= paddleStep;
        if (ch == 80 && playerY < height - 1) playerY += paddleStep;

        if (playerY < 2) playerY = 2;
        if (playerY > height - 2) playerY = height - 2;
    }
}

int main() {
    hideCursor();
    gameMenu();
    resetGame();
    running = true;

    while (running) {
        input();
        logic();
        Sleep(frameDelay);

        if (playerScore == winningScore || computerScore == winningScore) {
            system("cls");

            string winner = (playerScore == winningScore) ? "YOU WIN!" : "CPU WINS!";
            string border = "****************************";

            for (int i = 0; i < 6; i++) {
                system("cls");

                int color = (i % 2 == 0) ? 14 : 7;
                setColor(color);

                cout << "\n\n\n";
                cout << "         " << border << "\n";
                cout << "         *                          *\n";
                cout << "         *     " << winner << "             *\n";
                cout << "         *                          *\n";
                cout << "         " << border << "\n";

                setColor(7);
                Sleep(400);
            }

            gotoxy(0, height + 5);
            break;
        }
    }

    system("pause");
    return 0;
}