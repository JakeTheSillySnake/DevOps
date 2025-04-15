SHELL := /bin/bash
FLAGS = -Wall -Wextra -Werror
FILE = grep.c

all: clean s21_grep test

s21_grep: clean
	gcc $(FLAGS) grep.c -o s21_grep

test:
	-diff <(./s21_grep e $(FILE)) <(grep e $(FILE))
	-diff <(./s21_grep -e main $(FILE)) <(grep -e main $(FILE))  
	-diff <(./s21_grep -i ERRor $(FILE)) <(grep -i ERRor $(FILE))  
	-diff <(./s21_grep -n e $(FILE)) <(grep -n e $(FILE))
	-diff <(./s21_grep -ni ERRor $(FILE)) <(grep -ni ERRor $(FILE))  
	-diff <(./s21_grep -v e $(FILE)) <(grep -v e $(FILE))
	-diff <(./s21_grep -c if $(FILE)) <(grep -c if $(FILE))
	-diff <(./s21_grep -l if $(FILE)) <(grep -l if $(FILE))
	-diff <(./s21_grep -ci R $(FILE)) <(grep -ci R $(FILE))
	-diff <(./s21_grep -cn h $(FILE)) <(grep -cn h $(FILE))
	-diff <(./s21_grep -cn h $(FILE) hi.txt) <(grep -cn h $(FILE) hi.txt)
	-diff <(./s21_grep -cnh e $(FILE)) <(grep -cnh e $(FILE))
	-diff <(./s21_grep -cnh e $(FILE) hi.txt) <(grep -cnh e $(FILE) hi.txt)
	-diff <(./s21_grep -vn el $(FILE) hi.txt) <(grep -vn el $(FILE) hi.txt)
	-diff <(./s21_grep -s R badpath) <(grep -s R badpath)
	-diff <(./s21_grep -e R -e h $(FILE)) <(grep -e R -e h $(FILE))
	-diff <(./s21_grep -f regex.txt *.txt) <(grep -f regex.txt *.txt)
#	-diff <(./s21_grep -o e *.txt) <(grep -o e *.txt)
	-diff <(./s21_grep -oich e *.txt) <(grep -oich e *.txt)
	-diff <(./s21_grep -olv e *.txt) <(grep -olv e *.txt)
#	-diff <(./s21_grep -oihlnc -e he -e E -f regex.txt *.txt) <(grep -oihlnc -e he -e E -f regex.txt *.txt)     

clean:
	rm -rf s21_grep

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	clang-format -n *.c  