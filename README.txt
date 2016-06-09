# bruteForce_crawler
A program that uses a brute force method to crawl websites

Checks for the existance of "baseURL/fileName", where baseURL is given and fileName is a given length.

Essentially a remade https://github.com/jinxmaster/puush_crawler.
Differences:
	Designed to work with any website and any number of fileName lengths
	Uses xargs to thread the download process
	Filenames are stored in a list in one file, instead of a folder of empty files with the appropriate names
	Better usage of "wget"

TODO:
	Implement automated method for opening the files
