// C Program for Message Queue (Writer Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

// structure for message queue 
struct mesg_buffer { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 

int main() 
{ 
    key_t key; 
    int msgid; 
    key = ftok("progfile", 65); 
    message.mesg_type = 1; 

    // ftok to generate unique key 
    
    // msgget creates a message queue 
        // and returns identifier 
	while (1)
    {
        printf("Write Data : "); 
        gets(message.mesg_text); 
        msgid = msgget(key, 0666 | IPC_CREAT); 
        msgsnd(msgid, &message, sizeof(message), 0); 
        printf("Data send is : %s \n", message.mesg_text); 

    }
    
	return 0; 
} 
