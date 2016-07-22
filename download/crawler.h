#define QUIET
//#define VERBOSE

//number of threads that will be spawned
const static int MAXTHREADS = 8;
//name of file to create in order to tell the program to stop
const static char *stopFile = "stop\0";

//flags for the spawned threads
static int run = 1;

//ensures all threads can access data
static char* command;
static int cmdLen;
static char charSet[62]; // a-z, A-Z, 0-9
static int numRunning;
