// C Program for Message Queue (Reader Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure for message queue 


struct mesg_buffer { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 


const int QUEUE_SIZE = 100;

int n=0;

struct pthreadid_order_queue_table{    
    int pthreadid;
    char country[50];   
    int order_queue[100];
    int head;
    int tail;
};  

struct pthreadid_order_queue_table id_queue_table[300];
int table_index = 0;

int get_pthreadid(char country[50]){
    for(int i=0 ; i < 300 ; i++ ){
        if(strcmp(id_queue_table[i].country, country) == 0){
            return id_queue_table[i].pthreadid;
        }
    }
    return 0;
}

char * get_country(int pthreadid){
    for(int i=0 ; i < 300 ; i++ ){
        if(id_queue_table[i].pthreadid == pthreadid){
            return id_queue_table[i].country;
        }
    }
}

int cancel_thread(int pthreadid){
    for(int i=0 ; i < 300 ; i++ ){
        if(id_queue_table[i].pthreadid == pthreadid){
            //TODO Head and tail eşit olmalı 
            //taile henüz eklenmemiş order olabilir ele alınacak
            int head = id_queue_table[i].head;
            int tail = id_queue_table[i].tail;
        }
    }
}
void add_order(int quantity,int pthreadid){
    //printf("quantity %d\n",quantity);
    for(int i=0 ; i < 300 ; i++ ){
        if(id_queue_table[i].pthreadid == pthreadid){
            //printf("add_order to  %d\n",pthreadid);            
            int tail = id_queue_table[i].tail;
            //printf("tail : %d\n",tail);
            id_queue_table[i].order_queue[tail] = quantity;
            id_queue_table[i].tail = (tail + 1 ) % QUEUE_SIZE;


        }
    }
}

int run_order(int pthreadid){
    for(int i=0 ; i < 300 ; i++ ){
        if(id_queue_table[i].pthreadid == pthreadid){
            int head = id_queue_table[i].head;
            int tail = id_queue_table[i].tail;

            if (head == tail){
                return 0;
            }
            int quantity = id_queue_table[i].order_queue[head];
            id_queue_table[i].head = ( id_queue_table[i].head + 1 ) % QUEUE_SIZE;
            return quantity;
        }
    }
}


void *print_message_function( int a )
{   int count = 0;
    int pthreadid = pthread_self();
    char *country;
    country = get_country(pthreadid);
    while (1)
    {   
        int quantity = run_order(pthreadid);
        
        if(quantity){
            printf("WORKER country : %s\n",country);
            printf("WORKER quantity : %d\n",quantity);
        }
        sleep(1);

    }
    pthread_exit(NULL);
    
    
}

void create_country_thread(char country[50]){
    pthread_t thread1;
    
    int iret1 = pthread_create( &thread1, NULL, print_message_function, 1);
    
    int pthreadid = (int)thread1;
    
    struct pthreadid_order_queue_table str;
    strncpy(str.country,country,50);
    str.head = 0;
    str.tail = 0;
    str.pthreadid = pthreadid;
    int order_queue[300];
    for(int i=0;i<300;i++){
        order_queue[i] = 0;
    }
    memcpy(str.order_queue,order_queue,sizeof(order_queue));
    id_queue_table[table_index] = str;
    table_index = table_index + 1;
}


int main() 
{   key_t key; 
    int msgid; 

    // ftok to generate unique key 

    // msgget creates a message queue 
    // and returns identifier 
    key = ftok("progfile", 65); 
	while(1){
        msgid = msgget(key, 0666 | IPC_CREAT); 
        msgrcv(msgid, &message, sizeof(message), 1, 0); 
    
        char *country = strtok(message.mesg_text, ","); 

        char *quantity_str = strtok(NULL, ","); 
        int quantity = atoi(quantity_str); 
        
        //printf("%s\n",country);
        printf("quantity : %d\n",quantity);
        
        
        //printf("Data Received is : %s \n", message.mesg_text); 

        msgctl(msgid, IPC_RMID, NULL); 
        int pthreadid;

        pthreadid = get_pthreadid(country);
        //printf("pthreadid: %d  \n",pthreadid);
        if(quantity == 0){
            cancel_thread(pthreadid);

        }
        if(pthreadid == 0){
            create_country_thread(country);
            //threadin  yaratılması beklenecek
            pthreadid = get_pthreadid(country);
            //printf("new pthreadid: %d  \n",pthreadid);
            add_order(quantity,pthreadid);
        }
        else{
            //printf("else pthreadid: %d  \n",pthreadid);
            add_order(quantity,pthreadid);
        } 
        int count = 0;  
        
        /*
        for(int i = 0; i < 300; i++){
            if(id_queue_table[i].order_queue[0]){
                printf("%s  ",id_queue_table[i].country);
            
                printf("%d  ",id_queue_table[i].order_queue[0]);
                printf("%d  ",id_queue_table[i].order_queue[1]);
                printf("%d  ",id_queue_table[i].order_queue[2]); 
            }
            
            
        }
         */

        

    

    
    }

	return 0; 
} 

