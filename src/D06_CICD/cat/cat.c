#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE

typedef struct args {
  int n, b, s, E, T, v;
} args;

void get_args(int argc, char** argv, args* arg);
void set_args(int opt, args* arg);
void read_file(args* arg, char* path);
char v_convert(char c);
void output(args* arg, char* line, int length, int* line_count);

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("usage: ./s21_cat [OPTIONS] [FILE]\n");
    exit(1);
  }
  args arg = {0};
  get_args(argc, argv, &arg);

  for (int i = optind; i < argc; i++) {
    read_file(&arg, argv[i]);
  }

  return 0;
}

void get_args(int argc, char** argv, args* arg) {
  struct option long_opts[] = {{"number", no_argument, NULL, 'n'},
                               {"number-nonblank", no_argument, NULL, 'b'},
                               {"squeeze-blank", no_argument, NULL, 's'},
                               {0, 0, 0, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "nbsEeTt", long_opts, 0)) != -1) {
    set_args(opt, arg);
  }
}

void set_args(int opt, args* arg) {
  if (opt == 'n') {
    arg->n = 1;
  } else if (opt == 'b') {
    arg->b = 1;
  } else if (opt == 's') {
    arg->s = 1;
  } else if (opt == 'E') {
    arg->E = 1;
  } else if (opt == 'T') {
    arg->T = 1;
  } else if (opt == 'e') {
    arg->v = 1;
    arg->E = 1;
  } else if (opt == 't') {
    arg->v = 1;
    arg->T = 1;
  } else if (opt == '?') {
    exit(1);
  }
}

void read_file(args* arg, char* path) {
  FILE* fptr = fopen(path, "r");
  if (!fptr) {
    perror(path);
    exit(1);
  }
  char* line = NULL;
  size_t length = 0;
  int read = 0, line_count = 1, empty_count = 0;
  read = getline(&line, &length, fptr);

  while (read != -1) {
    if (line[0] == '\n') {
      empty_count++;
    } else {
      empty_count = 0;
    }
    if (empty_count < 2 || !arg->s) {
      output(arg, line, read, &line_count);
    }
    read = getline(&line, &length, fptr);
  }
  free(line);
  fclose(fptr);
}

char v_convert(char c) {
  if (c != '\n' && c != '\t') {
    if ((signed)c < 0) {
      printf("M-");
      c = c & 0x7F;
    }
    if (c <= 31) {
      printf("^");
      c += 64;
    } else if (c == 127) {
      printf("^");
      c = '?';
    }
  }
  return c;
}

void output(args* arg, char* line, int length, int* line_count) {
  for (int i = 0; i < length; i++) {
    if (arg->E && line[i] == '\n') {
      printf("$");
    }
    if (arg->v) {
      line[i] = v_convert(line[i]);
    }
    if (arg->T && line[i] == '\t') {
      printf("^");
      line[i] = 'I';
    }
    if (arg->n && !i) {
      printf("%6d\t", *line_count);
      (*line_count)++;
    }
    if (arg->b && !i && line[i] != '\n') {
      printf("%6d\t", *line_count);
      (*line_count)++;
    }
    printf("%c", line[i]);
  }
}
