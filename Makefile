CC = g++
FLAGS = -Wall 
FLAGS2 = -Wall -std=c++11

build: minlexbfs disjcnt rtd revedges

minlexbfs: minlexbfs.o
	$(CC) $(FLAGS) $< -o $@

minlexbfs.o: minlexbfs.cpp
	$(CC) $(FLAGS) $< -c -o $@

disjcnt: disjcnt.o
	$(CC) $(FLAGS2) $< -o $@

disjcnt.o: disjcnt.cpp
	$(CC) $(FLAGS2) $< -c -o $@

rtd: rtd.o
	$(CC) $(FLAGS2) $< -o $@

rtd.o: rtd.cpp
	$(CC) $(FLAGS2) -c $< -o $@

revedges: revedges.o
	$(CC) $(FLAGS2) $< -o $@

revedges.o: revedges.cpp
	$(CC) $(FLAGS2) $< -c -o $@

.PHONY: run-p1 run-2 run-3 pun-4 clean

run-p1: minlexbfs
	./minlexbfs

run-p2: disjcnt
	./disjcnt
	
run-p3: rtd
	./rtd

run-p4: revedges
	./revedges

clean:
	rm -f minlexbfs disjcnt rtd revedges *.o