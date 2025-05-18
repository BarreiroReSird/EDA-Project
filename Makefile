# Nome do executável (Remover .exe para Linux/macOS)
EXECUTABLE = projectEDA.exe

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra -lm

# Ficheiros fonte
SOURCE = main.c aerial.c fileUtils.c gridUtils.c interference.c graph.c graphIO.c graphSearch.c graphOp.c

# Regra para construir o executável
$(EXECUTABLE): $(SOURCE)
		$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE)

# Regra para limpar o executável
clean:
		rm -f $(EXECUTABLE)

# Regra para recompilar tudo
rebuild: clean all