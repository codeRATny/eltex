/*  
    F1 - open file
    F2 - save
    F3 - exit
*/

#include <fcntl.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "header.h"

#define msleep(msec) usleep(msec * 1000)

enum CODE {
    NOEXIT_CODE = 0,
    EXIT_CODE = 1,
    SAVE_CODE = 2,
};

struct Controller {
    char str[1024];
    int line_ends[256];
    char associated_file[32];
    int y;
    int x;
    int pos;
    int cur_line_idx;
    int modified;
    int fd;
} typedef Controller;

void init(Controller* idxer) {
    idxer->cur_line_idx = 0;
    idxer->x = 0;
    idxer->y = 0;
    idxer->pos = 0;
    for (int i = 0; i < 31; i++) {
        idxer->associated_file[i] = ' ';
    }
    idxer->associated_file[31] = '\0';
    idxer->modified = 0;
    idxer->fd = 0;
}

void save(Controller* idxer) {
    WINDOW* wnd = newwin(2, 44, getmaxy(stdscr) - 1, 0);
    if (idxer->modified)
    {
        if (idxer->associated_file[0] == ' ') {
            waddstr(wnd, "Filename: ");
            wrefresh(wnd);
            wgetnstr(wnd, idxer->associated_file, 31);
            idxer->associated_file[31] = '\0';
        }
        idxer->fd = open(idxer->associated_file, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
        if (idxer->fd == -1) {
            perror("open error");
        }
        int res = write(idxer->fd, idxer->str, strlen(idxer->str));
        if (res != -1) {
            waddstr(wnd, "Saved\n");
        } else {
            perror("write error");
        }
        wrefresh(wnd);
        msleep(270);
        close(idxer->fd);

        idxer->modified = 0;
    }

    refresh();
    msleep(3000);

    wclear(wnd);
    wrefresh(wnd);
    delwin(wnd);
    refresh();
}

int confirm_exit(Controller* idxer) {
    if (idxer->modified) {
        char y_n[5];
        WINDOW* wnd = newwin(2, 44, getmaxy(stdscr) - 2, 0);
        waddstr(wnd, "Found unsaved changes.(yes/no/save)");
        wgetnstr(wnd, y_n, 4);
        y_n[4] = '\0';
        wclear(wnd);
        wrefresh(wnd);
        delwin(wnd);
        if (y_n[0] != 's' && y_n[0] != 'y') {
            return NOEXIT_CODE;
        }
        if (y_n[0] == 'y') {
            close(idxer->fd);
            return EXIT_CODE;
        }
        if (y_n[0] == 's') {
            return SAVE_CODE;
        }
    } else {
        return EXIT_CODE;
    }
}

int open_file(Controller* idxer) {
    WINDOW* wnd = newwin(2, 44, getmaxy(stdscr) - 1, 0);
    waddstr(wnd, "Filename: ");
    wrefresh(wnd);
    wgetnstr(wnd, idxer->associated_file, 31);
    idxer->associated_file[31] = '\0';

    wclear(wnd);
    wrefresh(wnd);
    delwin(wnd);
    if (idxer->fd != 0) {
        close(idxer->fd);
    }

    idxer->fd = open(idxer->associated_file, O_RDWR);

    int res = read(idxer->fd, idxer->str, 1022);
    for (int i = 0; i < res; i++) {
        if (idxer->str[i] == '\n') {
            idxer->line_ends[idxer->cur_line_idx++] = idxer->x;
            idxer->y++;
            idxer->x = 0;
        } else {
            idxer->x++;
        }
        idxer->pos++;
    }
    addstr(idxer->str);
    move(idxer->y, idxer->x);

    refresh();
    msleep(3000);
}

int main() {
    initscr();
    keypad(stdscr, true);
    Controller idxer;
    init(&idxer);

    int ex = 0;

    while (!ex) {
        int ch = mvgetch(idxer.y, idxer.x);

        switch (ch) {
            case ERR:
                mvaddstr(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, "err");
                break;

            case KEY_F(3):
                ex = confirm_exit(&idxer);
                if (ex == SAVE_CODE) {
                    ex = EXIT_CODE;
                } else {
                    break;
                }

            case KEY_F(2):
                save(&idxer);
                break;
            case KEY_F(1):
                for (int i = 0; i < idxer.pos; i++) {
                    idxer.str[i] = ' ';
                }
                init(&idxer);
                open_file(&idxer);
                break;
            case KEY_BACKSPACE:
                if (idxer.x - 1 > -1 || idxer.y > 0) {
                    delch();
                    idxer.str[--idxer.pos] = '\0';
                    idxer.x--;
                }
                if (idxer.x < 0) {
                    idxer.y--;
                    idxer.x = idxer.line_ends[--idxer.cur_line_idx];
                }
                if (!idxer.modified) idxer.modified = 1;
                break;
            case KEY_ENTER:
            case 10:
            case 13:
                idxer.line_ends[idxer.cur_line_idx++] = idxer.x;
                idxer.x = 0;
                idxer.str[idxer.pos++] = '\n';
                idxer.str[idxer.pos] = '\0';
                if (idxer.y + 1 != getmaxy(stdscr)) {
                    idxer.y++;
                }
                if (!idxer.modified) idxer.modified = 1;
                break;
            default:
                idxer.str[idxer.pos++] = ch;
                if (idxer.pos > 1023) {
                    idxer.pos--;
                }
                if (idxer.x + 1 != getmaxx(stdscr)) {
                    idxer.x++;
                }
                idxer.str[idxer.pos] = '\0';
                if (!idxer.modified) idxer.modified = 1;
                break;
        }

        refresh();
    }
    mvaddstr(getmaxy(stdscr) - 1, 0, "Press any key");
    refresh();
    getch();
    endwin();
    return 0;
}