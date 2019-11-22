#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


pid_t Sel(char *n, pid_t c11, pid_t c22, pid_t c33, pid_t c44, pid_t c55){
	if (strcmp(n,"1")==0)
		return c11;
	if (strcmp(n,"2")==0)
		return c22;
	if (strcmp(n,"3")==0)
		return c33; 
	if (strcmp(n,"4")==0) 
		return c44;
       	if (strcmp(n,"5")==0) 
		return c55;
	return getpid();
}



void handler1(int n){
	return;
}

void term(pid_t c11, pid_t c22, pid_t c33, pid_t c44, pid_t c55){
	kill(c11,SIGTERM);
	kill(c22,SIGTERM);
	kill(c33,SIGTERM);
	kill(c44,SIGTERM);
	kill(c55,SIGTERM);
	printf("Wrong input\nTerminated all processes\n");
	exit(0);
}



int main(int argc, char **argv){
	pid_t c1,c2,c3,c4,c5;
	int i=1;
	// 1st child
	c1 = fork();
	if (c1<0){
		perror("fork");
		exit(-1);
	}
	else if (c1==0){
		signal(SIGCONT,handler1);
		pause();
		while(1){
			printf("Child1 %d is executed (%d)\n", getpid(), i);
			i++;
			sleep(1);
		}		
		exit(0);		
	}
	// 2nd child
	c2 = fork();
	if (c2<0){
		perror("fork");
		exit(-1);
	}
	else if (c2==0){
		signal(SIGCONT,handler1);
		pause();
		while(1){
			printf("Child2 %d is executed (%d)\n", getpid(), i);
			i++;
			sleep(1);
		}		
		exit(0);
	}
	//3rd child
	c3 = fork();
	if (c3<0){
		perror("fork");
		exit(-1);
	}
	else if (c3==0){
		signal(SIGCONT,handler1);
		pause();
		while(1){
			printf("Child3 %d is executed (%d)\n", getpid(), i);
			i++;
			sleep(1);
		}		
		exit(0);
	}
	//4th child
	c4 = fork();
	if (c4<0){
		perror("fork");
		exit(-1);
	}
	else if (c4==0){
		signal(SIGCONT,handler1);
		pause();
		while(1){
			printf("Child4 %d is executed (%d)\n", getpid(), i);
			i++;
			sleep(1);
		}		
		exit(0);
	}
	//5th child
	c5 = fork();
	if (c5<0){
		perror("fork");
		exit(-1);
	}
	else if (c5==0){
		kill(getppid(),SIGUSR1);	// signal father that you are almost ready		
		signal(SIGCONT,handler1);
		pause();
		while(1){
			printf("Child5 %d is executed (%d)\n", getpid(), i);
			i++;
			sleep(1);
		}		
		exit(0);
	}

	signal(SIGUSR1,handler1);
	pause(); 				// "wait" for the 5th child to be in the correct state
	
	if(argc!=6){
		printf("a");
		term(c1,c2,c3,c4,c5);
	}
	if (strcmp(argv[1],argv[2])==0 || strcmp(argv[1],argv[3])==0 || strcmp(argv[1],argv[4])==0 || strcmp(argv[1],argv[5])==0 || strcmp(argv[2],argv[3])==0 || strcmp(argv[2],argv[4])==0 || strcmp(argv[2],argv[5])==0 || strcmp(argv[3],argv[4])==0 || strcmp(argv[3],argv[5])==0 || strcmp(argv[4],argv[5])==0) {
		term(c1,c2,c3,c4,c5);
	}
	for(int k=0; k<4; k++){
		for(int j=1; j<argc; j++){
			if(Sel(argv[j],c1,c2,c3,c4,c5)==getpid()){
				term(c1,c2,c3,c4,c5);
			}			
			kill(Sel(argv[j],c1,c2,c3,c4,c5),SIGCONT);
			sleep(3);
			if(k==3)
				kill(Sel(argv[j],c1,c2,c3,c4,c5),SIGTERM);
			else
				kill(Sel(argv[j],c1,c2,c3,c4,c5),SIGSTOP);
		}
	}
	exit(0);
}
