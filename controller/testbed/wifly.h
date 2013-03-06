void WiFly_Init(void);
int WiFly_Send_Cmd(char *, char *);
int WiFly_Send(char *, char *) ;
void WiFly_Flush(void);
unsigned long WiFly_Time(void);
int UART_Match(char *);
void UART_Send(const unsigned char *, unsigned long);

