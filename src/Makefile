.PHONY: all install uninstall clean dvi dist clang test build calc.a gcov_report leaks

CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic -std=c++17
LINUX_FLAGS = -lm -lsubunit -lrt -pthread
TEST_FILES = ./tests.cc ./SmartCalcCPP/model/*.cc
TEST_FLAGS = -lgtest -pthread -lstdc++
OBJ=$(patsubst %.c,%.o,$(SRC))
FLAGS_TEST_LIBS = -lgtest -lm -lpthread
OS = $(shell uname)

all: clean build calc.a

install: uninstall
	@mkdir build
	@mkdir build/SmartCalcCPP
	@chmod -R 777 build
	@qmake SmartCalcCPP/s21_calc.pro -o build/SmartCalcCPP/Makefile && make -C build/SmartCalcCPP	
	@rm -rf build/SmartCalcCPP/*.o

uninstall:
	@rm -rf build/

clean:
	@rm -rf *.a *.o test *.gcno *.gcda *.info *.out gcov_test s21_model s21_model_credit s21_model_deposit report dist make build .qmake.stash 

dvi:
	@nano dvi.md

dist:
	@mkdir dist
	@cp -R SmartCalcCPP dvi.md Makefile dist
	@cd dist && tar cvzf SmartCalcCPP.0.tgz *
	@cd dist && rm -rf *.c *.cpp *.h *.ui calc* dvi.md Makefile
	@rm -rf Archive_calc_v2.0

clang:
	@clang-format -n -style=Google ./SmartCalcCPP/*.cc ./tests.cc ./SmartCalcCPP/model/*.cc ./SmartCalcCPP/model/*.h ./SmartCalcCPP/view/*.cc ./SmartCalcCPP/view/*.h ./SmartCalcCPP/controller/*.cc ./SmartCalcCPP/controller/*.h

test:
	@if [ "$(OS)" = "Darwin" ]; then \
		$(CC) $(FLAGS) $(TEST_FILES) -o tests.out $(TEST_FLAGS); \
	else \
		$(CC) $(FLAGS) $(TEST_FILES) -o tests.out $(TEST_FLAGS) $(FLAGS_TEST_LIBS); \
	fi
	@./tests.out

build: ./SmartCalcCPP/model/*.cc
	@$(CC) $(FLAGS) -c ./SmartCalcCPP/model/s21_model_calc.cc $(LINUX_FLAGS) -o s21_model_calc.o
	@$(CC) $(FLAGS) -c ./SmartCalcCPP/model/s21_model_credit.cc $(LINUX_FLAGS) -o s21_model_credit.o
	@$(CC) $(FLAGS) -c ./SmartCalcCPP/model/s21_model_deposit.cc $(LINUX_FLAGS) -o s21_model_deposit.o

calc.a: build
	@ar crs calc.a s21_model_calc.o s21_model_credit.o s21_model_deposit.o

gcov_report: calc.a
ifeq ($(OS), Linux)
	@$(CC) $(FLAGS)  --coverage $(TEST_FILES) -o tests.out $(TEST_FLAGS) calc.a -o gcov_test -lgtest $(FLAGS_TEST_LIBS);
	@chmod +x *
	@./gcov_test
	@lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	@genhtml -o report/ gcov_test.info
	@firefox ./report/index.html
	@make clean
else
	@$(CC) $(CFLAGS) --coverage $(TEST_FILES) calc.a $(FLAGS_TEST_LIBS) -o gcov_test 
	@chmod +x *
	@./gcov_test
	@lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	@genhtml -o report/ gcov_test.info
	@open ./report/index.html
	@make clean
endif

leaks: test
	@$(CC) $(FLAGS) $(TEST_FILES) -o tests.out $(TEST_FLAGS) $(FLAGS_TEST_LIBS)
	@leaks -atExit -- ./tests.out

valgrind:
	@$(CC) $(FLAGS) $(TEST_FILES) -o tests.out $(TEST_FLAGS) $(FLAGS_TEST_LIBS)
	@valgrind --trace-children=yes --track-fds=yes --track-origins=yes -s --leak-check=full --show-leak-kinds=all ./tests.out