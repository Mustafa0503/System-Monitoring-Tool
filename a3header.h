#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <utmp.h>
#include <sys/resource.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct {
    double preVirMem;
    char storeMemory[1024];

    
} infoMemory;
typedef struct {
    char cpuString[1024];
    double currentCPU;
    double previousTime;
    double previousUtilization;
    char abc[1024];
    char def[1024];
} CPUData;
void ctrl_c_handler(int sig);
void printSysOnly();
void printUserOnly();
void getCpuStats(int i, double *cpu, double *t1, double *c1, int gf, char arrForCpu[][1024], char arrForStorCpu[][1024], int samples, int tdelay, char storeCPUs[1024]);
void getMemoryStats(int i, double *preVirMem, int gf,char arr[][1024], int samples);
void getMemoryStatsForChild(int i, double *preVirMem, int gf,char arr[1024], int samples);
void printIterationInfo(int i, int tdelay, int sef, int samples);
void printMemorySession(int i, int samples, int sef, char memoryArray[][1024]) ;
void print_num_of_cores();
void printCPUUsage(int i, int gf, double cpu, int samples, int tdelay, char arrForCpu[][1024], char arrForStorCpu[][1024]);
void getUserForChild(char storeUserSession[1024]);
int digits(char *str);
void printCPU(int i, int samples, int graphics, int sequential, char cpuArray[][1024]);

