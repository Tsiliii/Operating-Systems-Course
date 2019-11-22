#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv){
        int Ppid = getpid();
        int status;
        int N = atoi(argv[1]);
        int K = atoi(argv[2]);
        int i,j;
        unsigned long long int result=0;

        if(N<1 || N>10){
            printf("Wrong Input\n");
            exit(0);
        }
        if(K<N){
            N=K;
        }
        if(K>20){
            printf("Can't calculate this factorial\n");
            exit(0);
        }
        // children array
        pid_t childpid[10];
        for (i=0; i<10; i++){
            childpid[i]=-1;
        }

        // pipe array
        int arr[11][2];
        for (i=0; i<N+1; i++){
            if (pipe(arr[i])== -1){
                perror("pipe");
                return 1;
            }
        }

        for(i=0; i<N; i++){
            childpid[i]=fork();
            if (childpid[i]==-1){
                perror("fork");
        		exit(-1);
            }
            if(childpid[i]==0){
                j=i;
                for(int k=0; k<N; k++){
                    if(k!=i){
                        close(arr[k][0]);
                        //printf("Close read end %d for child %d\n",k,i);
                    }
                    if(k!=i+1){
                        if((j==N-1 && k==0)){
                        }
                        else{
                            close(arr[k][1]);
                            //printf("Close write end %d for child %d\n",k,i);
                        }
                    }
                }
                break;
            }
            else{
                continue;
            }
        }

        for(i=0; i<N; i++){
            if(childpid[i]==0){
                while(true){
                    if(read(arr[i][0],&result,sizeof(result))!=-1){
                        if(j==K){
                            printf("Last Child (%d) Wrote at pipe %d the result is %lld\n",i+1,N,result);
                            write(arr[N][1],&result,sizeof(result));
                            close(arr[(i+1)%N][1]);
                            close(arr[i][0]);
                            close(arr[N][0]);
                            close(arr[N][1]);
                            exit(0);
                        }
                        result = result * (j+1);
                        j=j+N;
                        write(arr[(i+1)%N][1],&result,sizeof(result));
                        //printf("Child %d Wrote at pipe %d the result is %lld\n\n",i+1,(i+1)%N,result);
                        if(j>K){
                            close(arr[(i+1)%N][1]);
                            close(arr[i][0]);
                            close(arr[N][0]);
                            close(arr[N][1]);
                            exit(0);
                        }
                    }
                    else {
                        perror("pipe");
                        return 1;
                    }
                }
            }
        }

        if(getpid()==Ppid){
            //printf("A\n");
            result = 1;
            for(i=0; i<N; i++){
                close(arr[i][0]);
            }
            write(arr[0][1],&result,sizeof(result));
            for(i=0; i<N+1; i++){
                close(arr[i][1]);
                }
                for(int i = 0; i<N ; i++){
                    wait(&status);
                }
                printf("Result before read = %lld\n",result);
                read(arr[N][0],&result,sizeof(result));
                close(arr[N][0]);
                printf("Result = %lld \n",result);
                return(0);
            }
        }
