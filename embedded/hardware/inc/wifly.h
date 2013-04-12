void WiFly_Init(void);
long WiFly_Send_Cmd(char *, char *);
long WiFly_Send(char *, char *) ;
void WiFly_Flush(void);
unsigned long WiFly_Time(void);
void WiFly_Open(char *); 
long UART_Match(char *);
void UART_Send(const unsigned char *, unsigned long);

