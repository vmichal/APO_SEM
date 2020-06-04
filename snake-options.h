/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	File snake-options.h holds configuration values of the program.
*/

#ifdef __cplusplus 

/*Dimenstion of the builtin LCD in pixels.*/
constexpr int LCD_WIDTH = 480;
constexpr int LCD_HEIGHT = 320;

//Side (in pixels) of one elementary square
constexpr int SIDE = 8;

//Number of squares vertically and horizontally
constexpr int COLUMNS = LCD_WIDTH / SIDE;
constexpr int ROWS = LCD_HEIGHT / SIDE;



#else 


#define LCD_WIDTH 480
#define LCD_HEIGHT 320
#define SIDE 8

#define COLUMNS (LCD_WIDTH / SIDE)
#define ROWS (LCD_HEIGHT / SIDE)




#endif
