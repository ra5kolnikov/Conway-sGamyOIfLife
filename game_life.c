#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define ROWS 25
#define COLS 80

int exitcondition(int* flag, int** nextGeneration, int** prevGen);
void massiveTransformation(int **mass, int **buff, int **previousGen);
void isAlive(int **mass, int *nums, int i, int j);
int outOfRange(int i, int k, char board);
int inputFromFile(int **mass);
void printScreen(int **mass);

int main() {
    int **nextGeneretion = malloc(ROWS * sizeof(int*));
    for (int i = 0; i < ROWS; i++) {
        nextGeneretion[i] = malloc(COLS * sizeof(int));
    }

    int **buff = malloc(ROWS * sizeof(int*));
    for (int i = 0; i < ROWS; i++) {
        buff[i] = malloc(COLS * sizeof(int));
    }

    int **previousGeneration = malloc(ROWS * sizeof(int*));
    for (int i = 0; i < ROWS; i++) {
        previousGeneration[i] = malloc(COLS * sizeof(int));
    }


    if (inputFromFile(nextGeneretion) == 1) {
        if (freopen("/dev/tty", "r", stdin)) {
        initscr();

        noecho();

        nodelay(stdscr, TRUE);
        printScreen(nextGeneretion);
        int flag = 1;
        int speed = 500000;
        while (flag == 1) {
            refresh();
            massiveTransformation(nextGeneretion, buff, previousGeneration);
            flag = 0;
            exitcondition(&flag, nextGeneretion, previousGeneration);

            if (speed < 1) {
                speed = 50000;
            }
            printScreen(nextGeneretion);
            usleep(speed);
            switch (getch()) {
            case '1':
                speed -= 50000;
                break;
            case '2':
                speed += 50000;
                break;
            case 'q':
                flag = 0;
                break;
            default:
                break;
            }
        }
    } else {
        printf("Incorrect Stream");
    }
        endwin();
        printf("Life is End!");
} else {
    printf("Input Error");
}
    for (int i = 0; i < ROWS; i++) {
        free(buff[i]);
        free(nextGeneretion[i]);
        free(previousGeneration[i]);
    }
    free(nextGeneretion);
    free(buff);
    free(previousGeneration);
    return 0;
}

int inputFromFile(int **mass) {
    int flag = 1;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (!scanf("%d", &mass[i][j])) {
                flag = 0;
            }
        }
    }
    return flag;
}

void printScreen(int **mass) {
    clear();
    int **Screen = malloc((ROWS + 2) * sizeof(int*));
    for (int i = 0; i < ROWS + 2; i++) {
        Screen[i] = malloc((COLS +2)* sizeof(int));
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Screen[i+1][j+1] = mass[i][j];
            }
        }
    for (int i = 0; i < ROWS + 2; i++) {
        for (int j = 0; j < COLS + 2; j++) {
            if (i == 0 || j == 0) {
                Screen[i][j] = 2;
            }
            if (i == ROWS + 1 || j == COLS + 1) {
                Screen[i][j] = 2;
            }
        }
    }
    for (int i = 0; i < ROWS + 2; i++) {
        for (int j = 0; j < COLS + 2; j++) {
            if (Screen[i][j] == 1) {
                printw("O");
            } else if (Screen[i][j] == 0) {
                printw(" ");
            } else {
                printw("#");
            }
        }
        printw("\n");
    }
    for (int i = 0; i < ROWS + 2; i++) {
        free(Screen[i]);
    }
    free(Screen);
}

void massiveTransformation(int **mass, int **buff, int **previousGen) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int nums = 0;
            buff[i][j] = mass[i][j];
            previousGen[i][j] = mass[i][j];
            isAlive(mass, &nums, i, j);
            if (mass[i][j] == 0 && nums == 3) {
                buff[i][j] = 1;
            }
            if (mass[i][j] == 1 && (nums < 2 || nums > 3)) {
                buff[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            mass[i][j] = buff[i][j];
        }
    }
}

void isAlive(int **mass, int *nums, int i, int j) {
    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            if (mass[outOfRange(i, k, 'y')][outOfRange(j, l, 'x')] == 1 && !(k == 1 && l == 1)) {
                *nums += 1;
            }
        }
    }
}

int outOfRange(int i, int k, char board) {
    int pos = 0;
    int boardValue = 0;
    if (board == 'x') {
        boardValue = 80;
    } else if (board == 'y') {
        boardValue = 25;
    }
    if ((i - 1 + k) >= 0 && (i - 1 + k) < boardValue) {
        pos = i - 1 + k;
    } else {
        if ((i - 1 + k) < 0) {
            pos = boardValue - 1;
        } else if ((i - 1 + k) >= boardValue) {
            pos = 0;
        }
    }
    return pos;
}

int exitcondition(int* flag, int** nextGeneration, int** prevGen) {
    int counter = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if ((nextGeneration[i][j]) == 1) {
                *flag = 1;
            }
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (nextGeneration[i][j] == prevGen[i][j]) {
                counter++;
            }
        }
    }
    if (counter >= ROWS * COLS) {
        *flag = 0;
    }
    return *flag;
}
