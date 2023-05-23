
#include "a3header.h"
//gcc -g b09a3.c -lm -o b09a3&&./b09a3

void ctrl_c_handler(int sig) {
    char answer;
    printf("\nDo you really want to quit? [y/n] ");
    scanf(" %c", &answer);
    if (answer == 'y' || answer == 'Y') {
        exit(0); // exit the program
    }
    // else, resume the program
}


int main(int argc, char**argv)
{ 

    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, ctrl_c_handler);
    //sets to default samples(10) and time(0) as we as all the flags
    int samples = 10;
    int tdelay = 1;
   
    int order=0;
    int sf=0;
    int gf=0;
    int uf=0;
    int sef=0;
    int num_pos_arg=0;
    //keeps track the number of valid argument(needed if all arguments are in valid in future)
    int validArg=0;
    //this for loop checks all the indication of the commands and stores the information accordingly and add the number of valud arguments
    for(int i=0;i<argc;i++)
    {
        if(strcmp(argv[i], "--system")==0)
        {
            sf=1;
            validArg++;
        }
        if(strcmp(argv[i], "--user")==0)
        {
            uf=1;
            validArg++;
        }
        if(strcmp(argv[i], "--graphics")==0||strcmp(argv[i], "--g")==0)
        {
            gf=1;
            validArg++;
        }
        if(strcmp(argv[i], "--sequential")==0)
        {
            sef=1;
            validArg++;
        }
    }
    //gets the number of samples and sleep time
    for(int i=0;i<argc;i++)
    {

        //checks if it is a positional argument
        if(digits(argv[i])==1)
        {
            num_pos_arg++;
            //make sure this is the first occurance of positional argument(which means it's the samples not tdelay)
            if(order==0)
            {
                //convert string of numbers to int
                samples = atoi(argv[i]);
                //indicate the first occurance of positional argument
                order=1;
                validArg++;
                continue;
            }
        }
        //checks if it is a positional argument
        if(digits(argv[i])==1)
        {
            
            //make sure this is the second occurance of positional argument(which means it's the tdelay not samples)
            if(order==1)
            {
                //convert string of numbers to int
                validArg++;
                tdelay = atoi(argv[i]);
            }
        }
        //gets the number samples if argument 
        else if(strstr(argv[i], "--samples=")!=NULL)
        {
            //gets the char after'=' and convert it to int which is the number of the samples
            char st[1024];
            int length = strlen(argv[i]) - 10 ;
            //gets the part only after the '=' sign
            strncpy(st, argv[i] + 10, length);
            st[length] = '\0';
            samples = atoi(st);
            validArg++;
        }
        //gets the time of sleep if argument 
        else if(strstr(argv[i], "--tdelay=")!=NULL)
        {
            char st[1024];
            //gets the char after'=' and convert it to int which is the time for sleep
            int length = strlen(argv[i]) - 9 ;
            //gets the part only after the '=' sign
            strncpy(st, argv[i] + 9, length);
            st[length] = '\0';
            tdelay = atoi(st); 
            validArg++;
        }
    }
    //when there is no argument (ie only ./a.out)
    if(validArg>0||argc==1)
    {
        typedef struct {
            char cpuString[1024];
            double currentCPU;
            double previousTime;
            double previousUtilization;
            char arrForStorCpu[samples][1024];
            char arrForCpu[samples][1024];
        } CPUData;
        //reserve space for the samples and cpu for graphics part
     
        char memoryArray[samples][1024];
      
        
        char arrForCpu1[samples][1024];
        char arrForStorCpu1[samples][1024];
        char storeUserSession[1024];
        
        
        char cpuArray[samples][1024];
        
        
        
        //initializing the previos iteration varibale for the calculation of cpua and memory usage

      
       
        infoMemory memoryInfo;
        CPUData cpuData;
        for(int i=0;i<samples;i++)
        {
            //initializing the array
           
            strcpy(cpuData.arrForCpu[i],"");
        }
        //loop thorugh the samples
        for(int i=0; i<samples;i++)
        {
        
        strcpy(cpuData.cpuString, "");
        strcpy(cpuData.arrForStorCpu[i], "");
        strcpy(cpuData.arrForCpu[i], "");
        strcpy(memoryInfo.storeMemory, "");

            strcpy(storeUserSession, "");
            int retval; // variable to hold return values of system calls
            int memoryPipe[2], userPipe[2], cpuPipe[2];
        
        if (pipe(memoryPipe) != 0 || pipe(userPipe) != 0 || pipe(cpuPipe) != 0) {
            fprintf(stderr, "Failed to create pipe");
            exit(EXIT_FAILURE);
        }

        pid_t child_pids[3]; // global array to store child PIDs

        for (int j=0; j<3; j++) {
              if (j == 0) {
                if ((child_pids[j] = fork()) == -1) {
                    fprintf(stderr, "Failed to fork");
                } 
                else if (child_pids[j] == 0) {
                    close(memoryPipe[0]);
                    getMemoryStatsForChild(i, &memoryInfo.preVirMem, gf, memoryInfo.storeMemory, samples);

                    if ((retval = write(memoryPipe[1], &memoryInfo, sizeof(memoryInfo))) < 0) {
                        fprintf(stderr, "Error writing to pipe");
                        exit(EXIT_FAILURE);
                    }
                    close(memoryPipe[1]);
                }
                else {
                    while(wait(NULL) > 0);
                    close(memoryPipe[1]);
                    if ((retval = read(memoryPipe[0], &memoryInfo, sizeof(memoryInfo))) < 0) {
                        fprintf(stderr, "Error writing to pipe");
            
                        exit(EXIT_FAILURE);
                    }
                    
                    strcpy(memoryArray[i], memoryInfo.storeMemory);

                    close(memoryPipe[0]);
                }
            }
               if (j == 1) {
                if ((child_pids[j] = fork()) == -1) {
                    fprintf(stderr, "Failed to fork");
                } 
                else if (child_pids[j] == 0) {
                    close(userPipe[0]);
                    getUserForChild(storeUserSession);
                    if ((retval = write(userPipe[1], storeUserSession, strlen(storeUserSession) + 1)) < 0) {
                        fprintf(stderr, "Error writing to pipe");
                        exit(EXIT_FAILURE);
                    }
                    //write(userPipe[1], userString, strlen(userString)+1);
                    close(userPipe[1]);
                }
                else {
                    while(wait(NULL) > 0);
                    close(userPipe[1]);
                    if ((retval = read(userPipe[0], storeUserSession, sizeof(storeUserSession))) < 0) {
                        fprintf(stderr, "Error reading from pipe");
                        exit(EXIT_FAILURE);
                    }
                    //read(userPipe[0], userString, sizeof(userString));
                    close(userPipe[0]);
                }
            }
                else if (j == 2) {
                if ((child_pids[j] = fork()) == -1) {
                    fprintf(stderr, "Failed to fork");
                } 
                else if (child_pids[j] == 0) {
                    close(cpuPipe[0]);
                    //printf("sd%s\n", cpuData.arrForStorCpu[i]);
                    getCpuStats(i, &cpuData.currentCPU, &cpuData.previousTime, &cpuData.previousUtilization, gf, arrForCpu1, arrForStorCpu1, samples, tdelay, cpuData.cpuString);
                    strcpy(cpuData.arrForCpu[i], arrForCpu1[i]);
                    strcpy(cpuData.arrForStorCpu[i], arrForStorCpu1[i]);
                    if ((retval = write(cpuPipe[1], &cpuData, sizeof(cpuData))) < 0) {
                        fprintf(stderr, "Error writing to pipe");
                        exit(EXIT_FAILURE);
                    }
                    //printf("sd%s\n", cpuData.arrForStorCpu[i]);

                    

                    //write(cpuPipe[1], &cpuData, sizeof(cpuData));
                    close(cpuPipe[0]);
                    //memoryUsage += getMemoryUsage();
                }
                else {
                    while(wait(NULL) > 0);
                    close(cpuPipe[1]);
                    if ((retval = read(cpuPipe[0], &cpuData, sizeof(cpuData))) < 0) {
                        fprintf(stderr, "Error reading from pipe");
                        exit(EXIT_FAILURE);
                    }
                    //read(cpuPipe[0], &cpuData, sizeof(cpuData));
                    //strcpy(storeCPUs, cpuData.cpuString);
                    strcpy(cpuArray[i], cpuData.cpuString);
                    strcpy(arrForCpu1[i], cpuData.arrForCpu[i]);
                    strcpy(arrForStorCpu1[i], cpuData.arrForStorCpu[i]);
                    close(cpuPipe[0]);      

                    
                    //memoryUsage += getMemoryUsage();
                }
            }

        }

            if (child_pids[0] == 0) {
                exit(EXIT_SUCCESS);
            }
        
            if (child_pids[1] == 0) {
                exit(EXIT_SUCCESS);
            }
            if (child_pids[2] == 0) {
                exit(EXIT_SUCCESS);
            }
        
            printIterationInfo(i, tdelay, sef, samples);
            //get all the usage informationa and convert from bytes to gb
            if(sf==1||gf==1||sef==1||num_pos_arg>0||argc==1)
            {
                //calculate the virtual used memory
                printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
                printMemorySession(i, samples, sef, memoryArray);
            }
                

            //get the previos iteration value of the memory usage
         
            print_num_of_cores();
            if(sf==1||gf==1||sef==1||num_pos_arg>0||argc==1)
                //set the previos iteration for the cpu calculation
                //getCpuStats(i,  &cpu, &t1, &c1, gf,arrForCpu, arrForStorCpu, samples, tdelay, storeCPUs);
 
               
                //for(int j=0;j<i;j++){
                printf("%s", cpuArray[i]);


                //}
            //print out the user session for edge case(ie only --user is not indicated or --user is indicated)
            if(uf==1||argc==1)
            {
                printf("---------------------------------\n");
                printf("### Sessions/users ###\n");
                printf("%s", storeUserSession);

            }

        }
        printSysOnly();
    }
}