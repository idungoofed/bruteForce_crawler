crawler: crawler.h crawler.c
	gcc -Wall -std=c99 crawler.c -o crawler -pthread -U DEBUG

debug: crawler.h crawler.c
	gcc -Wall -std=c99 crawler.c -o crawler -pthread -D DEBUG

hack:
	$(shell touch crawler.c)

clean:
	rm *~ *.out crawler results.txt stop
