#define QUIET
//#define VERBOSE

//number of threads that will be spawned
const static int MAXTHREADS = 8;
//name of file to create in order to tell the program to stop
const static char* stopFile = "stop";

//flags for the spawned threads
static int turn_to_write;
static int run = 1;

//ensures all threads can access data
static FILE *outFile;
static char* command;
static int cmdLen;
static char charSet[62];
