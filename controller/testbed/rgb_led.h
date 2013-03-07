typedef enum color_e {
  NONE,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  PURPLE,
  TURQUOISE,
  WHITE
} color_t;

void RGB_Init(void);
void RGB_Set(color_t);
void RGB_On(color_t color);
void RGB_Off(color_t color);
