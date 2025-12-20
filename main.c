#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define getlen(x) sizeof(x)/sizeof(x[0])
#define EX_LEN 8
#define VALUE_LEN 64

const char *menu[] = {
  "Upper A",
  "Lower A",
  "Upper B",
  "Lower B",
  "exit"
};
size_t n_option = getlen(menu);

const char *exercises_UA[] = {
  "Paused Larsen Press 2x3-5",
  "Kroc Rows 2x4-8",
  "Weighted Chinups 1x4-8",
  "Strict BTN Press 1x4-6",
  "Incline Curls 2x4-8",
  "SA Tricep Pushdown 2x4-8",
  "",
  "Date(d/m/y):"
};
size_t e_option = getlen(exercises_UA);

const char *exercises_UB[] = {
  "Weighted Pull-ups 2x4-8",
  "Weighted Dips 2x4-8",
  "Cable Neck Extensions 2x8-12",
  "Barbell Curls 2x4-8",
  "SA Triceps Extension 2x4-8",
  "Sit-Ups 1xAMRAP Weight BTN",
  "Wrist Curls 1x8-10, Extensions 1x8-10",
  "Date(d/m/y):"
};

const char *exercises_LA[] = {
  "SLDLs 2x6-12",
  "Zercher Bulgarian Split Squat 2x4-8",
  "Full ROM SL Paused Calf Raises 1xAMRAP",
  "Sissy squats 1xAMRAP",
  "Neck Curls 1xAMRAP",
  "",
  "",
  "Date(d/m/y):"
};

const char *exercises_LB[] = {
  "Zercher Squat 2x4-8",
  "Nordic Curls 2x6-10",
  "DB Calf Raises 2xAMRAP",
  "Zercher Jcurls 1x6-10",
  "",
  "",
  "",
  "Date(d/m/y):"
};

char values[EX_LEN][VALUE_LEN];

void read_file(const char *filename) {
  FILE *fptr;
  fptr = fopen(filename, "r");

  if(!fptr) {
    for (size_t i = 0; i < EX_LEN; ++i)
      strcpy(values[i], "");
    return;
  }

  char line[VALUE_LEN];
  size_t i = 0;

  while(i < EX_LEN && fgets(line, sizeof(line), fptr)) {
      line[strcspn(line, "\n")] = '\0';
      strncpy(values[i], line, VALUE_LEN - 1);
      i+=1;
    }
  fclose(fptr);
}

void save_values(const char *filename) {
  FILE *fptr = fopen(filename, "w");
  if (!fptr) return;

  for (int i = 0; i < EX_LEN; i++) {
    fprintf(fptr, "%s\n", values[i]);
  }

  fclose(fptr);
}

void program(const size_t n, const char* filename) {
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  clear();

  int ch = 0;
  size_t h = 0;

  while(ch != 24) {
    clear();
    mvprintw(0, 0, "Upper A\t\tC-S to save\t\tC-X to exit\t\t(AUTOSAVE ENABLED)");

    for (size_t i = 0; i < e_option; ++i) {

      if(n == 1) {
        if(h == i) attron(A_REVERSE);
        mvprintw(i + 2, 2, "%-25s\t%-10s", exercises_UA[i], values[i]);
        if(h == i) attroff(A_REVERSE);
      }

      if(n == 2) {
        if(h == i) attron(A_REVERSE);
        mvprintw(i + 2, 2, "%-25s\t%-10s", exercises_LA[i], values[i]);
        if(h == i) attroff(A_REVERSE);
      }

      if(n == 3) {
        if(h == i) attron(A_REVERSE);
        mvprintw(i + 2, 2, "%-25s\t%-10s", exercises_UB[i], values[i]);
        if(h == i) attroff(A_REVERSE);
      }

      if(n == 4) {
        if(h == i) attron(A_REVERSE);
        mvprintw(i + 2, 2, "%-25s\t%-10s", exercises_LB[i], values[i]);
        if(h == i) attroff(A_REVERSE);
      }

    }
    refresh();
    ch = getch();

    switch (ch) {
      case KEY_UP:
      case 'k':
        h = (h == 0) ? e_option-1 : h - 1;
        break;

      case KEY_DOWN:
      case 'j':
        h = (h == e_option - 1) ? 0 : h + 1;
        break;
      case '\n':
        echo();
        mvprintw(h + 2, 30, " ");
        mvgetnstr(h + 2, 30, values[h], VALUE_LEN - 1);
        noecho();
        save_values(filename);
        break;
      case 19:
        save_values(filename);
    }
  }

  endwin();
}

int main() {
  WINDOW *main_win;
  main_win = initscr();

  if (!has_colors()) {
      printf("ERROR:\tYour Terminal does not support Colour!");
      endwin();
      return 1;
  }

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  noecho();
  cbreak();
  keypad(main_win, TRUE);

  wbkgd(main_win, COLOR_PAIR(1) | ' ');

  size_t highlight = 0;
  size_t choice;

  while (true) {
    wclear(main_win);
    for (size_t i = 0; i<n_option; ++i) {
      if(i==highlight) attron(A_REVERSE);
      mvprintw(i+1, 1, menu[i]);
      if(i==highlight)
        attroff(A_REVERSE);
    }
    wrefresh(main_win);
    choice = wgetch(main_win);

    switch (choice) {
      case KEY_UP:
      case 'k':
        highlight= (highlight == 0) ? n_option-1 : highlight - 1;
        break;

      case KEY_DOWN:
      case 'j':
        highlight= (highlight == n_option - 1) ? 0 : highlight + 1;
        break;
      case 10:
        if (highlight == n_option - 1) {
          endwin();
          return 0;

        }


        if (highlight == 0) {
          wclear(main_win);
          read_file("upper_a.csv");
          program(1, "upper_a.csv");
        }

        if (highlight == 1) {
          wclear(main_win);
          read_file("lower_a.csv");
          program(2, "lower_a.csv");
        }

        if (highlight == 2) {
          wclear(main_win);
          read_file("upper_b.csv");
          program(3, "upper_b.csv");
        }

        if (highlight == 3) {
          wclear(main_win);
          read_file("lower_b.csv");
          program(4, "lower_b.csv");
        }
        
        break;
    }
  }

  endwin();

  return 0;
}
