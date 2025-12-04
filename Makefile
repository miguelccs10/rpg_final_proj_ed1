# ==========================================
# Makefile para RPG Initiative Tracker
# ==========================================

# Compilador e Flags
CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -I src
# -std=c++11: Garante suporte ao RNG e features modernas
# -Wall -Wextra: Ativa avisos para ajudar a achar erros
# -I src: Permite que os includes achem a pasta hpp facilmente

# Diretórios
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Nome do Executável Final
TARGET  := rpg

# Encontrar todos os arquivos .cpp dentro de src/
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Gerar nomes dos arquivos objeto (.o) correspondentes em build/
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# --- Regras ---

# Regra Padrão (apenas 'make')
all: $(TARGET)

# Linkagem: Cria o executável final juntando os objetos
$(TARGET): $(OBJS)
	@echo "🔨 Linkando o executável: $@"
	$(CXX) $(OBJS) -o $@
	@echo "✅ Sucesso! Execute com: ./$@"

# Compilação: Cria os arquivos .o a partir dos .cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "📝 Compilando: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para rodar o programa
run: all
	@echo "🚀 Executando o RPG..."
	@./$(TARGET)

# Regra de limpeza (apaga arquivos gerados)
clean:
	@echo "🧹 Limpando arquivos de build..."
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "✨ Limpo."

# Marca estas regras como não sendo arquivos
.PHONY: all clean run