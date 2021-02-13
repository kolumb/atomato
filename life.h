// Here we keep the common code for all of the Cellular Atomata
// from the Game of Life Family:
// https://en.wikipedia.org/wiki/Life-like_cellular_automaton

#ifndef LIFE_H_
#define LIFE_H_

typedef int Cell;

typedef struct {
    Cell cells[ROWS][COLS];
} Board;

int board_nbors(const Board *board, int row0, int col0, Cell cell)
{
    int result = 0;
    for (int drow = -1; drow <= 1; ++drow) {
        for (int dcol = -1; dcol <= 1; ++dcol) {
            if (drow != 0 || dcol != 0) {
                const int row = mod(row0 + drow, ROWS);
                const int col = mod(col0 + dcol, COLS);

                if (board->cells[row][col] == cell) {
                    result += 1;
                }
            }
        }
    }
    return result;
}

void next_board(const Board *prev, Board *next,
                int (*rule)(const Board *prev, int row, int col))
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            next->cells[row][col] = rule(prev, row, col);
        }
    }
}

void random_board(Board *board, int cell_state)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            board->cells[row][col] = rand() % cell_state;
        }
    }
}

int life_event_loop(void (*init_board)(Board *board),
                    int (*rule)(const Board *prev, int row, int col),
                    void (*render_board)(const Board *board))
{
    Board board[2] = {0};
    int fg = 0;

    init_board(&board[fg]);

    atomato_begin();

    while (!atomato_time_to_quit()) {
        atomato_poll_events(NULL);

        if (atomato_is_next_gen()) {
            const int bg = 1 - fg;
            next_board(&board[fg], &board[bg], rule);
            fg = bg;
        }

        atomato_begin_rendering();
        {
            render_board(&board[fg]);
        }
        atomato_end_rendering();
    }

    atomato_end();

    return 0;
}

#endif // LIFE_H_