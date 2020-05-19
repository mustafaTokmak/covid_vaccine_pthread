// C Program for Message Queue (Writer Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

 

// structure for message queue 
struct mesg_buffer { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 

int mode;
int get_order(){}
int main(int argc, char *argv[]) 
{ 
	key_t key; 
	int msgid; 

    mode = atoi(argv[1]);
    if(mode == 1){
        printf("Client mode : Auto\n");
    }
    else if(mode == 0) {
        printf("Client mode : Manuel\n");
    }
    else {
        printf("Client mode should be Auto(1) or Manuel(0) \n");
        exit(0);
    }

    // ftok to generate unique key 
    key = ftok("progfile", 65); 

    // msgget creates a message queue 
    // and returns identifier 
    message.mesg_type = 1; 
    char country[50];

    printf("Enter country name: ");
    scanf("%s", country);
    
    int total_order = 0;
     // auto mode 
    if(mode == 1){
        int quantity;
        char quantity_str[50]; 
        for(int i=0; i<3;i++){
            float sec = (float)(rand()%1000)/(float)(1000);
            printf(" %f \n", sec); 
            sleep(sec); 
            quantity = rand() % 100000;
            total_order = total_order + quantity;
            
            printf("%d\n",quantity);
            sprintf(quantity_str, "%d", quantity); 
            strcpy(message.mesg_text, country);
            strcat(message.mesg_text, ",");
            strcat(message.mesg_text, quantity_str);
            msgid = msgget(key, 0666 | IPC_CREAT); 
            msgsnd(msgid, &message, sizeof(message), 0); 
        }
        quantity = 0;
        sleep(1);
        printf("%d\n",quantity);
        sprintf(quantity_str, "%d", quantity); 
        strcpy(message.mesg_text, country);
        strcat(message.mesg_text, ",");
        strcat(message.mesg_text, quantity_str);
        msgid = msgget(key, 0666 | IPC_CREAT); 
        msgsnd(msgid, &message, sizeof(message), 0); 
        

        
        printf("Toplam siparis miktari: %d \n",total_order);
        printf("Siparis Tamamlandi");
        exit(0);
    }
    //manual mode
    if(mode == 0){
        while (1){
            int quantity;

            printf("Enter quantity of order: (Enter 0 for exit) ");
            scanf("%d", &quantity);
            total_order = total_order + quantity;
            char quantity_str[50]; 
            sprintf(quantity_str, "%d", quantity); 
            strcpy(message.mesg_text, country);
            strcat(message.mesg_text, ",");
            strcat(message.mesg_text, quantity_str);
            msgid = msgget(key, 0666 | IPC_CREAT); 
            msgsnd(msgid, &message, sizeof(message), 0); 
            printf("Data send is : %s \n",message.mesg_text); 
            if(quantity == 0){
                printf("Toplam siparis miltari: %d \n",total_order);
                printf("Siparis Tamamlandi");
                exit(0);
            }
        }
    }
   
	return 0; 
} 
