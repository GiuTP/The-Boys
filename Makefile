# Makefile - The Boys
# Carlos Maziero - DINF/UFPR, 2024/2

CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -g -std=c99 -I include
LDLIBS   = -lm
MAIN     = theboys
ENTREGA  = $(MAIN)

BINDIR   = bin
BUILDDIR = build
SRCDIR   = src
INCDIR   = include

# lista de arquivos de cabeçalho
HDR = $(wildcard $(INCDIR)/*.h)

# lista de arquivos-objeto (não inclui conjunto.o para não ser removido com "make clean")
OBJ = $(BUILDDIR)/main.o \
      $(BUILDDIR)/fila.o \
      $(BUILDDIR)/fprio.o \
      $(BUILDDIR)/eventos.o \
      $(BUILDDIR)/entidades.o

# construir o executável (alvo padrão)
$(BINDIR)/$(MAIN): $(OBJ) $(BUILDDIR)/conjunto.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# garante que os diretórios de saída existam
$(BINDIR) $(BUILDDIR):
	mkdir -p $@

# construir os arquivos-objeto
$(BUILDDIR)/main.o: $(SRCDIR)/main.c $(HDR) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/fila.o: $(SRCDIR)/fila.c $(INCDIR)/fila.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/fprio.o: $(SRCDIR)/fprio.c $(INCDIR)/fprio.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/eventos.o: $(SRCDIR)/eventos.c $(HDR) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/entidades.o: $(SRCDIR)/entidades.c $(HDR) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# executar
run: $(BINDIR)/$(MAIN)
	./$(BINDIR)/$(MAIN)

# testar no Valgrind
valgrind: $(BINDIR)/$(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(MAIN)

# gerar compile_commands.json para ferramentas clang/IDE
compile_commands: clean
	bear -- $(MAKE) $(BINDIR)/$(MAIN)

# gerar arquivo TGZ para entregar
tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp $(SRCDIR)/*.c $(INCDIR)/*.h Makefile /tmp/$(USER)/$(ENTREGA)
	tar czvf $(ENTREGA).tgz -C /tmp/$(USER) $(ENTREGA)
	rm -rf /tmp/$(USER)
	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

# limpar arquivos temporários (conjunto.o NÃO é removido)
clean:
	rm -f $(OBJ) $(BINDIR)/$(MAIN) $(ENTREGA).tgz
	rm -rf $(BINDIR)

.PHONY: run valgrind compile_commands tgz clean
