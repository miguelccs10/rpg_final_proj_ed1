# ==========================================
# Makefile para RPG Initiative Tracker
# ==========================================

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -I src

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

TARGET  := rpg

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "Linkando o executável: $@"
	$(CXX) $(OBJS) -o $@
	@echo "Sucesso! Execute com: ./$@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "📝 Compilando: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@


run: all
	@echo "Executando o RPG..."
	@./$(TARGET)


clean:
	@echo "Limpando arquivos de build..."
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "✨ Limpo."


.PHONY: all clean run