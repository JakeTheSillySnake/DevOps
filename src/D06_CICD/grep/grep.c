#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE

typedef struct args {
  int e, i, v, c, l, n, h, s, f, o;
  char pattern[1024];
  int len_pattern;
} args;

void get_args(int argc, char** argv, args* arg);
void set_args(int opt, args* arg);
void add_patterns(args* arg, char* pattern);
void read_file(args arg, char* path, regex_t* reg, int argc);
void read_regex_file(args* arg, char* path);
void print_line(char* line, int length);
void print_match(regex_t* regex, char* line);
void output(args arg, int argc, char** argv);

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("Usage: ./s21_grep [OPTIONS] [FILE]\n");
    exit(1);
  }
  args arg = {0};
  get_args(argc, argv, &arg);
  output(arg, argc, argv);

  return 0;
}

void get_args(int argc, char** argv, args* arg) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    set_args(opt, arg);
  }
  if (!arg->len_pattern) {
    add_patterns(arg, argv[optind]);
    optind++;
  }
}

void set_args(int opt, args* arg) {
  if (opt == 'e') {
    arg->e = 1;
    add_patterns(arg, optarg);
  } else if (opt == 'i') {
    arg->i = REG_ICASE;
  } else if (opt == 'v') {
    arg->v = 1;
  } else if (opt == 'c') {
    arg->c = 1;
  } else if (opt == 'l') {
    arg->l = 1;
  } else if (opt == 'n') {
    arg->n = 1;
  } else if (opt == 'h') {
    arg->h = 1;
  } else if (opt == 's') {
    arg->s = 1;
  } else if (opt == 'f') {
    arg->f = 1;
    read_regex_file(arg, optarg);
  } else if (opt == 'o') {
    arg->o = 1;
  } else if (opt == '?') {
    exit(1);
  }
}

void add_patterns(args* arg, char* pattern) {
  if (arg->len_pattern) {
    strcat(arg->pattern + arg->len_pattern, "|");
    arg->len_pattern++;
  }
  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

void read_file(args arg, char* path, regex_t* regex, int argc) {
  FILE* fptr = fopen(path, "r");
  if (!fptr) {
    if (!arg.s) {
      perror(path);
    }
    exit(1);
  }
  char* line = NULL;
  size_t length = 0;
  int read = getline(&line, &length, fptr), line_count = 1, match_count = 0;
  while (read != -1) {
    int result = regexec(regex, line, 0, NULL, 0);
    if ((!result && !arg.v) || (result && arg.v)) {
      if (!arg.c && !arg.l) {
        if (!arg.h && argc - optind > 1) printf("%s:", path);
        if (arg.n) printf("%d:", line_count);
        if (arg.o && !arg.v) {
          print_match(regex, line);
        } else {
          print_line(line, read);
        }
      }
      match_count++;
    }
    read = getline(&line, &length, fptr);
    line_count++;
  }
  if (arg.c) {
    if (!arg.h && argc - optind > 1) printf("%s:", path);
    if (arg.l && match_count) match_count = 1;
    printf("%d\n", match_count);
  }
  if (arg.l && match_count) printf("%s\n", path);
  free(line);
  fclose(fptr);
}

void read_regex_file(args* arg, char* path) {
  FILE* fptr = fopen(path, "r");
  if (!fptr) {
    if (!arg->s) {
      perror(path);
    }
    exit(1);
  }
  char* line = NULL;
  size_t length = 0;
  int read = getline(&line, &length, fptr);
  while (read != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    add_patterns(arg, line);
    read = getline(&line, &length, fptr);
  }
  free(line);
  fclose(fptr);
}

void print_line(char* line, int length) {
  for (int i = 0; i < length; i++) {
    printf("%c", line[i]);
  }
  if (line[length - 1] != '\n') printf("\n");
}

void print_match(regex_t* regex, char* line) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(regex, line + offset, 1, &match, 0);
    if (result) {
      break;
    }
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      printf("%c", line[i + offset]);
    }
    printf("\n");
    offset += match.rm_eo;
  }
}

void output(args arg, int argc, char** argv) {
  regex_t regex;
  int error = 0;
  error = regcomp(&regex, arg.pattern, REG_EXTENDED | arg.i);
  if (error) {
    perror("Error");
    exit(1);
  }
  for (int i = optind; i < argc; i++) {
    read_file(arg, argv[i], &regex, argc);
  }
  regfree(&regex);
}
