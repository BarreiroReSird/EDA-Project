# Nome do executável (Remover .exe para Linux/macOS)
EXECUTABLE = projectEDA.exe

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra -lm

# Arquivos fonte
SOURCE = main.c aerial.c fileUtils.c gridUtils.c interference.c

# Regra para construir o executável
$(EXECUTABLE): $(SOURCE)
		$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE)

# Regra para limpar o executável
clean:
		rm -f $(EXECUTABLE)

# Regra para recompilar tudo
rebuild: clean all