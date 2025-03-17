# Nome do executável
EXECUTABLE = main.exe

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra

# Arquivos fonte
SOURCE = main.c

# Regra para construir o executável
$(EXECUTABLE): $(SOURCE)
		$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE)

# Regra para limpar o executável
clean:
		rm -f $(EXECUTABLE)