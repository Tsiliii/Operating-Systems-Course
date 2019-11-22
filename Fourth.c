#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

void my_signal (int arr[],bool condition){
    char *tmp;
    if (condition)
        read(arr[0],&tmp,sizeof(tmp));
    else
        write(arr[1],&tmp,sizeof(tmp));
}

void my_wait(int finish[]){
    char *tmp;
    read(finish[0],&tmp,sizeof(tmp));
    read(finish[0],&tmp,sizeof(tmp));
    read(finish[0],&tmp,sizeof(tmp));
}

void critical_section(int arr[],int child){
    my_signal(arr,true);
    for(int i = 0; i<5; i++){
        if(child == 1)
            printf("Child %d %d is executing a critical section !\n",child,getpid());
        else if (child == 2)
            printf("Child %d %d is executing a critical section !!\n",child,getpid());
        else
            printf("Child %d %d is executing a critical section !!!\n",child,getpid());
    }
    my_signal(arr,false);
}

void non_critical_section(int arr[],int child){
    for(int i=0; i<7; i++){
        printf("Child %d %d is executing a non critical section\n",child,getpid());
    }
}

int main(){
    int finish[2];
    int arr[2];
    pipe(finish);
    pipe(arr);
    char *tmp;
    write(arr[1],&tmp,sizeof(tmp));
    pid_t child1,child2,child3;
    child1 = fork();
    if (child1<0){
		perror("fork");
		exit(-1);
	}
    else if(child1==0){
        critical_section(arr,1);
        non_critical_section(arr,1);
        non_critical_section(arr,1);
        critical_section(arr,1);
        non_critical_section(arr,1);
        critical_section(arr,1);
        write(finish[1],&tmp,sizeof(tmp));
        return 0;
    }
    child2 = fork();
    if (child2<0){
		perror("fork");
		exit(-1);
	}
    else if(child2==0){
        non_critical_section(arr,2);
        critical_section(arr,2);
        non_critical_section(arr,2);
        critical_section(arr,2);
        non_critical_section(arr,2);
        critical_section(arr,2);
        write(finish[1],&tmp,sizeof(tmp));
        return 0;
    }
    child3 = fork();
    if (child3<0){
		perror("fork");
		exit(-1);
	}
    else if(child3==0){
        critical_section(arr,3);
        non_critical_section(arr,3);
        critical_section(arr,3);
        non_critical_section(arr,3);
        critical_section(arr,3);
        non_critical_section(arr,3);
        write(finish[1],&tmp,sizeof(tmp));
        return 0;
    }
    my_wait(finish);
    return(0);
}
