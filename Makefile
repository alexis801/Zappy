##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

CXX			=	clang++
CXXFLAGS	=	-Wall -Wextra -std=c++20 -iquote ./include
DEPFLAGS	=	-MMD -MP

SRC_DIR		=	src
OBJ_DIR		=	obj

SERVER		=	zappy_server
GUI			=	zappy_gui

SERVER_LIBS	=
GUI_LIBS	=	-lsfml-graphics -lsfml-window -lsfml-system

COMMON_SRC	=	$(shell find $(SRC_DIR)/common -name '*.cpp' 2>/dev/null)
SERVER_SRC	=	$(shell find $(SRC_DIR)/server -name '*.cpp' 2>/dev/null) $(COMMON_SRC)
GUI_SRC		=	$(shell find $(SRC_DIR)/gui    -name '*.cpp' 2>/dev/null) $(COMMON_SRC)

SERVER_OBJ	=	$(SERVER_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GUI_OBJ		=	$(GUI_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

ALL_OBJ		=	$(sort $(SERVER_OBJ) $(GUI_OBJ))

all:	$(SERVER) $(GUI)

$(SERVER):	$(SERVER_OBJ)
	@if [ -z "$(strip $(SERVER_OBJ))" ]; then \
		echo "  [skip] $@ : aucune source dans $(SRC_DIR)/server/"; \
	else \
		$(CXX) $(CXXFLAGS) -o $@ $(SERVER_OBJ) $(SERVER_LIBS); \
		echo "  [ok]   $@"; \
	fi

$(GUI):	$(GUI_OBJ)
	@if [ -z "$(strip $(GUI_OBJ))" ]; then \
		echo "  [skip] $@ : aucune source dans $(SRC_DIR)/gui/"; \
	else \
		$(CXX) $(CXXFLAGS) -o $@ $(GUI_OBJ) $(GUI_LIBS); \
		echo "  [ok]   $@"; \
	fi

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean:	clean tests_clean
	$(RM) $(SERVER) $(GUI)

re:	fclean all

dev:	CXXFLAGS += -Werror
dev:	re

TEST_SRC	=	$(shell find tests -name '*.cpp' 2>/dev/null)
ALL_SRC		=	$(shell find $(SRC_DIR) -name '*.cpp' 2>/dev/null)
SRC_NO_MAIN	=	$(filter-out %/main.cpp, $(ALL_SRC))

TEST_OBJ_DIR	=	obj_tests
TEST_OBJ	=	$(SRC_NO_MAIN:%.cpp=$(TEST_OBJ_DIR)/%.o) \
				$(TEST_SRC:%.cpp=$(TEST_OBJ_DIR)/%.o)

$(TEST_OBJ_DIR)/%.o:	%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) --coverage -c $< -o $@

tests_run:	$(TEST_OBJ)
	$(CXX) $(CXXFLAGS) --coverage -o unit_tests $(TEST_OBJ) -lcriterion $(GUI_LIBS)
	./unit_tests
	gcovr -r . --exclude tests/ --gcov-executable "llvm-cov gcov"

tests_clean:
	$(RM) unit_tests
	$(RM) -r $(TEST_OBJ_DIR)
	$(RM) *.gcno *.gcda

docs:
	doxygen Doxyfile
	xdg-open docs/html/index.html

doc_clean:
	$(RM) -r docs/html docs/latex docs/xml

-include $(ALL_OBJ:.o=.d)

.PHONY:	all clean fclean re dev tests_run tests_clean docs doc_clean
