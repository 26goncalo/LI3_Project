CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS =
all: folders orchestrator client

orchestrator: bin/orchestrator
client: bin/client
folders:
		@mkdir -p src include obj bin tmp
bin/orchestrator: obj/orchestrator.o obj/utilities.o
		$(CC) $(LDFLAGS) $^ -o $@
bin/client: obj/client.o obj/utilities.o
		$(CC) $(LDFLAGS) $^ -o $@
obj/%.o: src/%.c
		$(CC) $(CFLAGS) -c $< -o $@
clean:
		rm -rf obj tmp bin


# CC = gcc
# CFLAGS = -Wall -g -Iinclude
# LDFLAGS =
# VALGRIND = valgrind
# VALGRIND_OPTS = --leak-check=full --show-leak-kinds=all
# all: folders orchestrator client

# #orchestrator: bin/orchestrator
# orchestrator: bin/orchestrator
# 	cp bin/orchestrator orchestrator
# #client: bin/client
# client: bin/client
# 	cp bin/client client
# folders:
# 	@mkdir -p src include obj bin tmp
# bin/orchestrator: obj/orchestrator.o obj/utilities.o
# 	$(CC) $(LDFLAGS) $^ -o $@
# bin/client: obj/client.o obj/utilities.o
# 	$(CC) $(LDFLAGS) $^ -o $@
# obj/%.o: src/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@
# 	$(CC) $(CFLAGS) -c $<	
# valgrind: bin/orchestrator bin/client
# 	$(VALGRIND) $(VALGRIND_OPTS) bin/orchestrator pasta 2 SJF
# clean:
# 	rm -rf obj tmp bin