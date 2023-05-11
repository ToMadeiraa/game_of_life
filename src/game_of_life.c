#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define X1 10
#define Y1 0
#define X2 91
#define Y2 26

void input(char** a);
void check_everything(char** a, char** b);
void copy(char** a, char** b, int n2, int m2);
void full_arrays_with_pointers(char** p_a, char** p_b, char* a, char* b, int n2, int m2);
void make_indents(char** a, char** b, int n1, int m1, int n2);
void print_field(char** a, char** b, int m1, int n2, int m2);
void print_left_and_right_borders(char** a, char** b, int n1, int m1, int n2, int m2);
void print_top_and_bottom_borders(char** a, char** b, int n1, int m1, int n2, int m2);
int print_matrix(char** a, int n, int m, int c_s, int s);
int check(char** a, int n, int m);
int check_top(char** a, int n, int m);
int check_bot(char** a, int n, int m);
int check_left(char** a, int n, int m);
int check_right(char** a, int n, int m);
int check_left_top(char** a, int n, int m);
int check_left_bot(char** a, int n, int m);
int check_right_top(char** a, int n, int m);
int check_right_bot(char** a, int n, int m);
void turn(char** a, char** b, int n, int m, int c);
void input(char** matrix);

int main(void) {
    char** pointer_array = malloc(Y2 * sizeof(char*));
    char* array = malloc(X2 * Y2 * sizeof(char));
    char** pointer_array_hidden = malloc(Y2 * sizeof(char*));
    char* array_hidden = malloc(X2 * Y2 * sizeof(char));
    full_arrays_with_pointers(pointer_array, pointer_array_hidden, array, array_hidden, Y2, X2);
    make_indents(pointer_array, pointer_array_hidden, Y1, X1, Y2);
    print_top_and_bottom_borders(pointer_array, pointer_array_hidden, Y1, X1, Y2, X2);
    print_left_and_right_borders(pointer_array, pointer_array_hidden, Y1, X1, Y2, X2);
    print_field(pointer_array, pointer_array_hidden, X1, Y2, X2);
    input(pointer_array);
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        initscr();
        noecho();
        cbreak();
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        char s;
        int speed = 10;
        int count;
        int count_step = 0;
        count = print_matrix(pointer_array, Y2, X2, count_step, speed);
        count_step++;
        while ((s = getch()) != 'q' && count != 0) {
            if (s == 'w' && speed > 1) {
                speed = speed / 2;
            }
            else if (s == 's' && speed < 30) {
                speed = speed * 2;
            }
            halfdelay(speed);
            check_everything(pointer_array_hidden, pointer_array);
            copy(pointer_array, pointer_array_hidden, Y2, X2);
            count = print_matrix(pointer_array, Y2, X2, count_step, speed);
            count_step++;
        }
        endwin();
    }
    free(array_hidden);
    free(pointer_array_hidden);
    free(array);
    free(pointer_array);
    return 0;
}
void turn(char** a, char** b, int n, int m, int c) {
    if (c == 0 || c == 1) {
        a[n][m] = ' ';
    }
    else if (c == 2) {
        a[n][m] = b[n][m];
    }
    else if (c == 3) {
        a[n][m] = 'O';
    }
    else if (c >= 4) {
        a[n][m] = ' ';
    }
    else {
        a[n][m] = ' ';
    }
}

int print_matrix(char** a, int n, int m, int c_s, int s) {
    int count_local = 0;
    clear();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (a[i][j] == 'O') {
                count_local++;
            }
            attron(COLOR_PAIR(1));
            printw("%c", a[i][j]);
            attroff(COLOR_PAIR(1));
        }
        printw("\n");
    }
    printw("                   Amount of alive cells : %d | Current step : %d | Delay : %d\n\n", count_local,
        c_s, s);
    printw("           Press any key to start, W to increase speed, S to decrease speed, Q to quit");
    refresh();
    return count_local;
}

void print_top_and_bottom_borders(char** a, char** b, int n1, int m1, int n2, int m2) {
    for (int i = n1; i < n2; i += 25) {
        for (int j = m1; j < m2; j++) {
            a[i][j] = '-';
            b[i][j] = '-';
        }
    }
}

void print_left_and_right_borders(char** a, char** b, int n1, int m1, int n2, int m2) {
    for (int i = n1 + 1; i < n2; i++) {
        for (int j = m1; j < m2; j += 80) {
            a[i][j] = '|';
            b[i][j] = '|';
        }
    }
}

void print_field(char** a, char** b, int m1, int n2, int m2) {
    for (int i = 1; i < n2 - 1; i++) {
        for (int j = m1 + 1; j < m2 - 1; j++) {
            a[i][j] = ' ';
            b[i][j] = ' ';
        }
    }
}

void make_indents(char** a, char** b, int n1, int m1, int n2) {
    for (int i = n1; i < n2; i++) {
        for (int j = 0; j < m1 - 1; j++) {
            a[i][j] = ' ';
            b[i][j] = ' ';
        }
    }
}

void full_arrays_with_pointers(char** p_a, char** p_b, char* a, char* b, int n2, int m2) {
    for (int i = 0; i < n2; i++) {
        p_a[i] = a + m2 * i;
        p_b[i] = b + m2 * i;
    }
}

void copy(char** a, char** b, int n2, int m2) {
    for (int i = 1; i < n2 - 1; i++) {
        for (int j = 1; j < m2 - 1; j++) {
            a[i][j] = b[i][j];
        }
    }
}

int check(char** a, int n, int m) {
    int count_local = 0;
    if (a[n - 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n - 1][m] == 'O') {
        count_local++;
    }
    if (a[n - 1][m + 1] == 'O') {
        count_local++;
    }
    if (a[n][m - 1] == 'O') {
        count_local++;
    }
    if (a[n][m + 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m] == 'O') {
        count_local++;
    }
    if (a[n + 1][m + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_left(char** a, int n, int m) {
    int count_local = 0;
    if (a[n - 1][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n - 1][m] == 'O') {
        count_local++;
    }
    if (a[n - 1][m + 1] == 'O') {
        count_local++;
    }
    if (a[n][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n][m + 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n + 1][m] == 'O') {
        count_local++;
    }
    if (a[n + 1][m + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_right(char** a, int n, int m) {
    int count_local = 0;
    if (a[n - 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n - 1][m] == 'O') {
        count_local++;
    }
    if (a[n - 1][X1 + 1] == 'O') {
        count_local++;
    }
    if (a[n][m - 1] == 'O') {
        count_local++;
    }
    if (a[n][X1 + 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m] == 'O') {
        count_local++;
    }
    if (a[n + 1][X1 + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_top(char** a, int n, int m) {
    int count_local = 0;
    if (a[Y2 - 2][m - 1] == 'O') {
        count_local++;
    }
    if (a[Y2 - 2][m] == 'O') {
        count_local++;
    }
    if (a[Y2 - 2][m + 1] == 'O') {
        count_local++;
    }
    if (a[n][m - 1] == 'O') {
        count_local++;
    }
    if (a[n][m + 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m] == 'O') {
        count_local++;
    }
    if (a[n + 1][m + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_bot(char** a, int n, int m) {
    int count_local = 0;

    if (a[n - 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n - 1][m] == 'O') {
        count_local++;
    }
    if (a[n - 1][m + 1] == 'O') {
        count_local++;
    }
    if (a[n][m - 1] == 'O') {
        count_local++;
    }
    if (a[n][m + 1] == 'O') {
        count_local++;
    }
    if (a[1][m - 1] == 'O') {
        count_local++;
    }
    if (a[1][m] == 'O') {
        count_local++;
    }
    if (a[1][m + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_left_top(char** a, int n, int m) {
    int count_local = 0;

    if (a[Y2 - 2][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[Y2 - 2][m] == 'O') {
        count_local++;
    }
    if (a[Y2 - 2][m + 1] == 'O') {
        count_local++;
    }
    if (a[n][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n][m + 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n + 1][m] == 'O') {
        count_local++;
    }
    if (a[n + 1][m + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_left_bot(char** a, int n, int m) {
    int count_local = 0;
    if (a[n - 1][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n - 1][m] == 'O') {
        count_local++;
    }
    if (a[n - 1][m + 1] == 'O') {
        count_local++;
    }
    if (a[n][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[n][m + 1] == 'O') {
        count_local++;
    }
    if (a[1][X2 - 2] == 'O') {
        count_local++;
    }
    if (a[1][m] == 'O') {
        count_local++;
    }
    if (a[1][m + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_right_top(char** a, int n, int m) {
    int count_local = 0;
    if (a[Y2 - 2][m - 1] == 'O') {
        count_local++;
    }
    if (a[Y2 - 2][m] == 'O') {
        count_local++;
    }
    if (a[Y2 - 2][X1 + 1] == 'O') {
        count_local++;
    }
    if (a[n][m - 1] == 'O') {
        count_local++;
    }
    if (a[n][X1 + 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n + 1][m] == 'O') {
        count_local++;
    }
    if (a[n + 1][X1 + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

int check_right_bot(char** a, int n, int m) {
    int count_local = 0;
    if (a[n - 1][m - 1] == 'O') {
        count_local++;
    }
    if (a[n - 1][m] == 'O') {
        count_local++;
    }
    if (a[n - 1][X1 + 1] == 'O') {
        count_local++;
    }
    if (a[n][m - 1] == 'O') {
        count_local++;
    }
    if (a[n][X1 + 1] == 'O') {
        count_local++;
    }
    if (a[1][m - 1] == 'O') {
        count_local++;
    }
    if (a[1][m] == 'O') {
        count_local++;
    }
    if (a[1][X1 + 1] == 'O') {
        count_local++;
    }
    return count_local;
}

void check_everything(char** a, char** b) {
    for (int i = 1; i < Y2 - 1; i++) {
        for (int j = X1 + 1; j < X2 - 1; j++) {
            if (i == 1 && j == X1 + 1) {
                int count = check_left_top(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if (i == 1 && j == X2 - 2) {
                int count = check_right_top(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if (i == Y2 - 2 && j == X2 - 2) {
                int count = check_right_bot(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if (i == Y2 - 2 && j == X1 + 1) {
                int count = check_left_bot(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if (i == 1 && (j != X1 + 1 && j != X2 - 2)) {
                int count = check_top(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if (i == Y2 - 2 && (j != X1 + 1 && j != X2 - 2)) {
                int count = check_bot(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if ((i != 1 && i != Y2 - 2) && j == X1 + 1) {
                int count = check_left(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else if ((i != 1 && i != Y2 - 2) && j == X2 - 2) {
                int count = check_right(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
            else {
                int count = check(b, i, j);
                turn(a, b, i, j, count);
                count = 0;
            }
        }
    }
}

void input(char** matrix) {
    int z, n, m;
    scanf("%d", &z);
    int i = 0;
    while (i < z) {
        scanf("%d %d", &n, &m);
        matrix[n][m] = 'O';
        i++;
    }
}
