#include "a3header.h"


void printSysOnly()
{
    printf("---------------------------------\n");
    //get the information from struct utsname and print it in given order
    struct utsname sysinfo;
    uname(&sysinfo);
    printf("### System Information ###\n");
    printf("System Name = %s\n", sysinfo.sysname);
    printf("Machine Name = %s\n", sysinfo.nodename);
    printf("Version Name = %s\n", sysinfo.version);
    printf("Release Name = %s\n", sysinfo.release);
    printf("Architecture Name = %s\n", sysinfo.machine);
    printf("---------------------------------\n");
}
void printUserOnly()
{
    
    //get the pointer to first user in file using struct utmp and loop it till end of the file, print out all the information needed
    struct utmp *users;
    //rewinds the file pointer to the beginning of the utmp file. It is generally a good idea to call it before any of the other functions
    setutent();
    //reads a line from the current file position in the utmp file. It returns a pointer to a structure containing the fields of the line.
    users = getutent();
    while(users!=NULL)
    {
        if(users->ut_type == USER_PROCESS)
            printf("%s       %s(%s)\n",users->ut_user, users->ut_line, users->ut_host);
        users = getutent();
    }
}
void getCpuStats(int i, double *cpu, double *t1, double *c1, int gf, char arrForCpu[][1024], char arrForStorCpu[][1024], int samples, int tdelay, char storeCPUs[1024])
{
    FILE *fp;
    long int user, nice, system, i2, iowait, irq, softirq;
    double curr=0;
    //char buff[255];
    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        // Handle error opening file
    }
    else
    {
        fscanf(fp, "cpu %ld %ld %ld %ld %ld %ld %ld", &user, &nice, &system, &i2, &iowait, &irq, &softirq);

        fclose(fp);
    }
    
    //calculation of the CPU usage which was given by Marcelo
    curr = user + nice + system + iowait + irq + softirq + i2;
    double Ut = curr - i2;
    *cpu = fabs(((double)(Ut - *c1) / (curr - *t1)) * 100);
    *c1 = Ut;
    // Update the values for the next iteration
    *t1 = curr;
    



    char per = '%';
    if (i == 0) {
        sprintf(storeCPUs, "total cpu use:%f%c\n", 0.00, per);
        if (gf == 1) {
            strcpy(arrForStorCpu[0], "@0.00");
        }
    } else {
        sprintf(storeCPUs + strlen(storeCPUs), "total cpu use:%.2f%c\n", *cpu, per);
        sprintf(arrForStorCpu[i], "%f", *cpu);

        if (gf == 1) {
            int numB = (int)(((*cpu * 10000) / 200));
            char b[numB];
            memset(b, '|', numB);
            b[numB] = '\0';
            strcpy(arrForCpu[i], b);

            for (int p = 0; p < i; p++) {
                sprintf(storeCPUs + strlen(storeCPUs), "          %s", arrForCpu[p]);
                sprintf(storeCPUs + strlen(storeCPUs), "%s\n", arrForStorCpu[p]);

                if (i == (samples - 1) && p == (samples - 2)) {
                    sleep(0.001);
                    sprintf(storeCPUs + strlen(storeCPUs), "          %s", arrForCpu[samples - 1]);
                    sprintf(storeCPUs + strlen(storeCPUs), "%s\n", arrForStorCpu[samples - 1]);
                }
            }
        }
    }

    
    
}


void getMemoryStats(int i, double *preVirMem, int gf,char arr[][1024], int samples)
{
    
    struct sysinfo mem;
    sysinfo(&mem);
    double gb_mem_total, gb_mem_avai, gb_mem_totswap, gb_mem_avaiswap;
    gb_mem_total = (double)mem.totalram / 1024 / 1024 / 1024;
    gb_mem_avai = (double)mem.freeram / 1024 / 1024 / 1024;
    gb_mem_totswap = (double)mem.totalswap / 1024 / 1024 / 1024;
    gb_mem_avaiswap = (double)mem.freeswap / 1024 / 1024 / 1024;

    // Calculate current virtual memory usage
    double curVirMem = (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap);

    // Update the previous virtual memory value
    if(i == 0)
    {
        *preVirMem = curVirMem;
    }
    
    int change = (int)((curVirMem - *preVirMem) * 1000);
    int sign = 0;

    if (change < 0)
    {
        sign = 1;
    }

    change = abs(change);

    if (gf == 1)
    {
        if (change == 0)
        {
            sprintf(arr[i], " %.2f Gb/%.2f Gb  --  %.5f Gb/%.2f Gb       |*%.2f(%.2f)", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap, curVirMem - *preVirMem, curVirMem);
        }
        else
        {
            if (sign == 0)
            {
                char bar[change];
                memset(bar, '#', change);
                bar[change] = '\0';
                sprintf(arr[i], " %.2f Gb/%.2f Gb  --  %.5f Gb/%.2f Gb       |%s*%.2f(%.2f)", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap, bar, curVirMem - *preVirMem, curVirMem);
            }
            if (sign == 1)
            {
                char bar[change];
                memset(bar, ':', change);
                bar[change] = '\0';
                sprintf(arr[i], " %.2f Gb/%.2f Gb  --  %.5f Gb/%.2f Gb       |%s@%.2f(%.2f)", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap, bar, curVirMem - *preVirMem, curVirMem);
            }
        }
    }
    else
    {
        sprintf(arr[i], " %.2f Gb/%.2f Gb  --  %.2f Gb/%.2f Gb", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap);
    }
        //print out all the sotored information in array called arr, and reserve samples space for furture iteration 
        for(int j=0;j<samples;j++)
        {
            printf("%s\n",arr[j]);
        }
    *preVirMem = curVirMem;
}

void getMemoryStatsForChild(int i, double *preVirMem, int gf,char arr[1024], int samples)
{
    
    struct sysinfo mem;
    sysinfo(&mem);
    double gb_mem_total, gb_mem_avai, gb_mem_totswap, gb_mem_avaiswap;
    gb_mem_total = (double)mem.totalram / 1024 / 1024 / 1024;
    gb_mem_avai = (double)mem.freeram / 1024 / 1024 / 1024;
    gb_mem_totswap = (double)mem.totalswap / 1024 / 1024 / 1024;
    gb_mem_avaiswap = (double)mem.freeswap / 1024 / 1024 / 1024;

    // Calculate current virtual memory usage
    double curVirMem = (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap);

    // Update the previous virtual memory value
    if(i == 0)
    {
        *preVirMem = curVirMem;
    }
    
    int change = (int)((curVirMem - *preVirMem) * 1000);
    int sign = 0;

    if (change < 0)
    {
        sign = 1;
    }

    change = abs(change);

    if (gf == 1)
    {
        if (change == 0)
        {
            sprintf(arr, " %.2f Gb/%.2f Gb  --  %.5f Gb/%.2f Gb       |*%.2f(%.2f)", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap, curVirMem - *preVirMem, curVirMem);
        }
        else
        {
            if (sign == 0)
            {
                char bar[change];
                memset(bar, '#', change);
                bar[change] = '\0';
                sprintf(arr, " %.2f Gb/%.2f Gb  --  %.5f Gb/%.2f Gb       |%s*%.2f(%.2f)", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap, bar, curVirMem - *preVirMem, curVirMem);
            }
            if (sign == 1)
            {
                char bar[change];
                memset(bar, ':', change);
                bar[change] = '\0';
                sprintf(arr, " %.2f Gb/%.2f Gb  --  %.5f Gb/%.2f Gb       |%s@%.2f(%.2f)", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap, bar, curVirMem - *preVirMem, curVirMem);
            }
        }
    }
    else
    {
        sprintf(arr, " %.2f Gb/%.2f Gb  --  %.2f Gb/%.2f Gb", gb_mem_total - gb_mem_avai, gb_mem_total, (gb_mem_total + gb_mem_totswap) - (gb_mem_avai + gb_mem_avaiswap), gb_mem_total + gb_mem_totswap);
    }
        //print out all the sotored information in array called arr, and reserve samples space for furture iteration 

    *preVirMem = curVirMem;
        //print out all the sotored information in array called arr, and reserve samples space for furture iteration 

}


void printIterationInfo(int i, int tdelay, int sef, int samples)
{
    sleep(tdelay);
    if (sef == 0)
    {
        system("clear");
    }
   
    if (sef == 1)
    {
        printf("Iteration----->%d\n", i + 1);
    }
    printf("Nbr of samples: %d -- every %d seconds\n", samples, tdelay);
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Memory usage: %ld kilobytes\n", usage.ru_maxrss);
    printf("---------------------------------\n");
}
void printMemorySession(int i, int samples, int sef, char memoryArray[][1024]) { // PRINTS MEMORY INFORMATION
   
    if (sef==1) { // if sequential flag is called, only print current iteration
        for (int j=0; j<i; j++) printf("\n");
        printf("%s\n", memoryArray[i]);
    }
    else { // otherwise, print previous + current iterations


        for (int j=0; j<=i; j++) printf("%s\n", memoryArray[j]);
    }
    for (int j=samples-1; j>i; j--) printf("\n");
}

void print_num_of_cores() {
    printf("---------------------------------\n");
    //get the number of the cores(confirmed by Marcelo sysconf can be used, and double checked with cat /proc/cpuinfo)
    int numOfCores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Number of cores: %d\n", numOfCores);
}
void printCPUUsage(int i, int gf, double cpu, int samples, int tdelay, char arrForCpu[][1024], char arrForStorCpu[][1024]) {
    char per = '%';
    if (i == 0) {
        printf("total cpu use:%f%c\n", 0.00, per);
        if (gf == 1) {
            strcpy(arrForStorCpu[0], "@0.00");
        }
    } else {
        printf("total cpu use:%f%c\n", cpu, per);
        sprintf(arrForStorCpu[i], "%f", cpu);

        if (gf == 1) {
            int numB = (int)(((cpu * 10000) / 80));
            char b[numB];
            memset(b, '|', numB);
            b[numB] = '\0';
            strcpy(arrForCpu[i], b);

            for (int p = 0; p < i; p++) {
                printf("          %s", arrForCpu[p]);
                printf("%s\n", arrForStorCpu[p]);

                if (i == (samples - 1) && p == (samples - 2)) {
                    sleep(tdelay);
                    printf("          %s", arrForCpu[samples - 1]);
                    printf("%s\n", arrForStorCpu[samples - 1]);
                }
            }
        }
    }
}
void getUserForChild(char storeUserSession[1024])
{
    //int num = 0;
    //get the pointer to first user in file using struct utmp and loop it till end of the file, print out all the information needed
    struct utmp *users;
    //rewinds the file pointer to the beginning of the utmp file. It is generally a good idea to call it before any of the other functions
    setutent();
    //reads a line from the current file position in the utmp file. It returns a pointer to a structure containing the fields of the line.
    users = getutent();
    while(users!=NULL)
    {
        if(users->ut_type == USER_PROCESS)
            // printf("%s       %s(%s)\n",users->ut_user, users->ut_line, users->ut_host);
            sprintf(storeUserSession + strlen(storeUserSession), "%s       %s(%s)\n", users->ut_user, users->ut_line, users->ut_host);
        users = getutent();
    }
}
int digits(char *str) {
    //very simple function it makes sure that all the char in *str in integer
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}
void printCPU(int i, int samples, int graphics, int sequential, char cpuArray[][1024]) { // PRINTS CPU INFORMATION

    if (graphics) {
        if (sequential) { // if sequential flag is raised, only print the information of current iteration
            for (int j=0; j<=i; j++) printf("\n");
            printf("%s", cpuArray[i]);
        }
        else { // otherwise, print information from previous + current iteration
            for (int j=0; j<=i; j++) printf("%s", cpuArray[j]);
        }
        for (int j=samples; j>i; j--) printf("\n");
    }
}