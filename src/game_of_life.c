#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define HEIGHT 23
#define WIDTH 78
#define MAX_GENERATIONS 1000

int initialize_matrix(int matrix[HEIGHT][WIDTH]);
void draw_cells(int matrix[HEIGHT][WIDTH]);
void update_field(int matrix[HEIGHT][WIDTH], int future_mat[HEIGHT][WIDTH]);
int count_neighbors(int matrix[HEIGHT][WIDTH], int i, int j);
void apply_rules(int matrix[HEIGHT][WIDTH], int neighbors, int future_mat[HEIGHT][WIDTH], int i, int j);
void copy_matrix(int matrix1[HEIGHT][WIDTH], int matrix2[HEIGHT][WIDTH]);
void check_mat(int matrix[HEIGHT][WIDTH], int fut_mat[HEIGHT][WIDTH], int *play);

int main() {
    int first_mat[HEIGHT][WIDTH];
    int play = 1;
    int generation_count = 0;
    char key;

    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    clear();
    if (initialize_matrix(first_mat)) {
        int old_mat[HEIGHT][WIDTH], real_mat[HEIGHT][WIDTH], future_mat[HEIGHT][WIDTH];
        short int velocity = 4;
        copy_matrix(first_mat, old_mat);
        copy_matrix(first_mat, real_mat);
        while (play == 1) {
            key = getch();
            generation_count++;

            if (key == 'q' || generation_count >= MAX_GENERATIONS) play = 0;
            if ((velocity >= 0) && (velocity < 50) && key == '2') velocity++;
            if ((velocity > 1) && key == '1') velocity--;

            (void)freopen("/dev/tty", "r+", stdin);

            usleep(240000 / velocity);

            clear();
            draw_cells(real_mat);
            update_field(real_mat, future_mat);

            check_mat(first_mat, future_mat, &play);
            check_mat(old_mat, future_mat, &play);

            copy_matrix(real_mat, old_mat);
            copy_matrix(future_mat, real_mat);
            refresh();
        }
    } else {
        printw("n/a");
        refresh();
    }
    endwin();
    clear();
    if (generation_count == MAX_GENERATIONS)
        printf("Max generations achieved");
    else if (key == 'q')
        printf("Left the game");
    else
        printf("Game is over");
    return 0;
}

int initialize_matrix(int matrix[HEIGHT][WIDTH]) {
    int flag = 1;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int num;
            if (scanf("%1d", &num) != 1 || (num != 0 && num != 1)) {
                flag = 0;
            } else {
                matrix[i][j] = num;
            }
        }
    }
    return flag;
}

void draw_cells(int matrix[HEIGHT][WIDTH]) {
    for (int i = -1; i < HEIGHT + 1; i++) {
        for (int j = -1; j < WIDTH + 1; j++) {
            if (i == -1 || i == HEIGHT)
                printw("*");
            else if (j == -1 || j == WIDTH)
                printw("*");
            else if (matrix[i][j] == 0) {
                printw(" ");
            } else {
                printw("#");
            }
        }
        if (i != HEIGHT + 1) printw("\n");
    }
}

void check_mat(int matrix[HEIGHT][WIDTH], int new_mat[HEIGHT][WIDTH], int *play) {
    int flag = 1;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (matrix[i][j] != new_mat[i][j]) {
                flag = 0;
                break;
            }
        }
        if (flag == 0) {
            break;
        }
    }

    if (flag == 1) {
        *play = 0;
    }
}

void update_field(int matrix[HEIGHT][WIDTH], int future_mat[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = count_neighbors(matrix, i, j);
            apply_rules(matrix, neighbors, future_mat, i, j);
        }
    }
    copy_matrix(future_mat, matrix);
}

int count_neighbors(int matrix[HEIGHT][WIDTH], int i, int j) {
    int neighbors = 0;

    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            if (k == 0 && l == 0) continue;
            int x = (i + k + HEIGHT) % HEIGHT;
            int y = (j + l + WIDTH) % WIDTH;
            neighbors += matrix[x][y];
        }
    }
    return neighbors;
}

void apply_rules(int matrix[HEIGHT][WIDTH], int neighbors, int future_mat[HEIGHT][WIDTH], int i, int j) {
    if (matrix[i][j] == 1) {
        if (neighbors == 2 || neighbors == 3) {
            future_mat[i][j] = 1;
        } else {
            future_mat[i][j] = 0;
        }
    } else {
        if (neighbors == 3) {
            future_mat[i][j] = 1;
        } else {
            future_mat[i][j] = 0;
        }
    }
}

void copy_matrix(int matrix1[HEIGHT][WIDTH], int matrix2[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            matrix2[i][j] = matrix1[i][j];
        }
    }
}