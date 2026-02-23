#ifndef LCD_GRAPHICS_H_
#define LCD_GRAPHICS_H_

#include <stdint.h>

#define LCD_NGRAPH 8
#define LCD_GRAPH_HEIGHT 8
#define LCD_DEFAULT_GRAPHICS_LEN (LCD_NGRAPH * LCD_GRAPH_HEIGHT)
extern uint8_t const lcd_default_graphics[LCD_NGRAPH][LCD_GRAPH_HEIGHT];

/* Macros for custom characters */
#define POUND_CHR 01
#define POUND_STR "\001"
#define EURO_CHR 02
#define EURO_STR "\002"
#define PLUSMINUS_CHR 03
#define PLUSMINUS_STR "\003"
#define SQUARED_CHR 04
#define SQUARED_STR "\004"
#define CUBED_CHR 05
#define CUBED_STR "\005"
#define UARROW_CHR 06
#define UARROW_STR "\006"
#define DARROW_CHR 07
#define DARROW_STR "\007"

/* macros for built-in characters */
#define YEN_CHR 0134
#define YEN_STR "\134"
#define RARROW_CHR 0176
#define RARROW_STR "\176"
#define LARROW_CHR 0177
#define LARROW_STR "\117"
#define DEGREE_CHR 0337
#define DEGREE_STR "\337"
#define ALPHA_CHR 0340
#define ALPHA_STR "\340"
#define BETA_CHR 0342
#define BETA_STR "\342"
#define EPSILON_CHR 0343
#define EPSILON_STR "\343"
#define MU_CHR 0344
#define MU_STR "\344"
#define SIGMA_CHR 0345
#define SIGMA_STR "\345"
#define RHO_CHR 0346
#define RHO_STR "\346"
#define SQRT_CHR 0350
#define SQRT_STR "\350"
#define THETA_CHR 0362
#define THETA_STR "\362"
#define OMEGA_CHR 0364
#define OMEGA_STR "\364"
#define CAPSIGMA_CHR 0366
#define CAPSIGMA_STR "\366"
#define PI_CHR 0367
#define PI_STR "\367"
#define XBAR_CHR 0370
#define XBAR_STR "\370"
#define DIVIDE_CHR 0375
#define DIVIDE_STR "\375"

#endif