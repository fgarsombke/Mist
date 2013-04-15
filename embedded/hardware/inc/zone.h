// Initializes zone control using SSI0
void Zone_Init(void);

// Enable a zone
void Zone_Enable(unsigned long zone);

// Disables a zone
void Zone_Disable(void);

// Returns the current zone
unsigned long Zone_Current(void);
