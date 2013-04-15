void Flash_Init(unsigned char systemClockFreqMHz);
int Flash_Write(unsigned long addr, unsigned long data);
int Flash_ProgramArray(unsigned long *source, unsigned long addr, unsigned short count);
int Flash_Erase(unsigned long addr);
