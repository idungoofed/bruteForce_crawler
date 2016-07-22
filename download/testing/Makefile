crawler: rr_crawler.h rr_crawler.c
	gcc -Wall -std=c99 rr_crawler.c -o crawler -pthread -U DEBUG

debug: rr_crawler.h rr_crawler.c
	gcc -Wall -std=c99 rr_crawler.c -o crawler -pthread -D DEBUG

hack:
	$(shell touch rr_crawler.c)

clean:
	rm *~ *.out crawler results.txt stop
