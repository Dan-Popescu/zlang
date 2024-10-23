# Nom de l'exécutable
EXEC = mon_language

# Fichiers source (.c)
SRC = main.c 

# Fichiers objets correspondants (.o)
OBJ = $(SRC:.c=.o)

# Le compilateur à utiliser
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra 

# Règle par défaut : compiler l'exécutable
all: $(EXEC)

# Règle pour compiler l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Règle pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ) $(EXEC)

# Règle pour nettoyer uniquement les fichiers objets
clean_obj:
	rm -f $(OBJ)

# Règle pour forcer la recompilation de l'exécutable
re: fclean all

# Règle pour nettoyer tous les fichiers (objets et exécutable)
fclean: clean
	rm -f $(EXEC)

# Phonies (cibles qui ne correspondent pas à des fichiers)
.PHONY: all clean clean_obj re fclean
