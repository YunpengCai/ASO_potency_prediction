/* Previous name: color_ann.h  */

#include <stdio.h>
#include <math.h>

char            color_ann_filename[120];
int             g_color_ann_file_exists;

/* For non logarithmic formulas */
#define NUM_COLORS 40

/* keep NUM_COLORS<=100
 * edit the line below to change where the web page points to for
 * documentation on color annotation when a html color table is
 * generated by plt22gif or plt22ps, it points here
 */

const char     *http_color_doc = URL;

int             color_table[NUM_COLORS + 1] = { /* log probabilities: */
  0xff0000,		/* red *//* > .999 */
  0xff1f00,
  0xff3f00,
  0xff5f00,
  0xff7f00,
  0xff9f00,
  0xffbf00,
  0xffdf00,		/* > .99  */
  0xffff00,		/* yellow */
  0xdfff00,
  0xbfff00,
  0x9fff00,
  0x7fff00,
  0x5fff00,
  0x3fff00,		/* > .9   */
  0x1fff00,
  0x00ff00,		/* green */
  0x00ff3f,
  0x00ff7f,
  0x00ffbf,
  0x00ffff,		/* cyan *//* ~ .5   */
  0x00bfff,
  0x007fff,
  0x003fff,
  0x0000ff,		/* blue  */
  0x1f00ff,		/* < .1   */
  0x3f00ff,
  0x5f00ff,
  0x7f00ff,
  0x9f00ff,
  0xbf00ff,
  0xdf00ff,		/* dark magenta */
  0xaf00cf,		/* < .01  */
  0x7f009f,
  0x5f007f,
  0x3f005f,
  0x1f003f,
  0x09001f,
  0x040009,
  0x000000,		
  0x000000,		
};
/* 39 black < .001
 * 40 must repeat black for position NUM_COLORS*1.00
 * 39 and 40 should be the same for background
 and text colors */ 

#define WHITE_BLACK_SWITCH 12

/* set previous at 16, for dot/char annotation option of boxplot
 * colors less than WHITE_BLACK_SWITCH for dots will have black letters
 * rest will have white letters , used to be 23 */

#define LOG_WHITE_BLACK_SWITCH 5
/* Letters over dots with colors creater than LOG_WHITE_BLACK will be white */


/* Redefine NUM_COLORS for log scale */
#define LOG_NUM_COLORS 8
/* It is assumed that LOG_NUM_COLORS is <= than NUM_COLORS */

int log_color_table[LOG_NUM_COLORS + 1] = { /* log probabilities: */
  0xff0000,		/* red  .999 and up  > .999 */
  0xff7f00,		/* orange .99 and up */
  0xffff00,		/* yellow  .9 and up */
  0x00ff00,		/* green .9 to .65  */
  0x22ffff,		/* cyan .65 to .35 */
  0x6060ff,		/* blue .35 to .1  *//* > .99  */
  0x9f00ff,		/* Magenta .1 to .01 */
  0x000000,		/* black .01 to 0 */
  0x000000		/* must repeat black for position
			 * NUM_COLORS*1.00
			 * 7 & 8 must be the same */
};


/***********************************************************************
 * Function getcolor() accepts 'x', a float within the range between
 * 0 and 1, and returns an integer RGB color value.
 * If 'inverse' is true, the color scheme is inversed.
 * If 'logarithmic' is true, the color value is adjusted to a
 * logarithmic scale.
 */

int get_color(double x, int inverse, int logarithmic) {
  if (inverse) {
    x = 1 - x;
  }
  /*
    if(logarithmic) {
    x = (x * 2) - 1;
    for(; logarithmic; logarithmic--) {
    x = ((x < 0)? -1 : 1) * x * x;
    }
    x = (x + 1) / 2;
    }
  */
  /* 7.04 causes the extreme colors to occur past .999 or .001 */
  if (logarithmic) {
    if (x < .001)
      return 0;
    else if (x < .01)
      return 1;
    else if (x < .1)
      return 2;
    else if (x < .35)
      return 3;
    else if (x < .65)
      return 4;
    else if (x < .9)
      return 5;
    else if (x < .99)
      return 6;
    else
      return 7;
    /* end of new method, method below no longer used */
    /* start of general formula */
    /* You might change NUM_COLORS to LOG_NUM_COLORS */
    /* return 0 when x is within epsilon of 0 */
    /* the -.0001 below ensures this */
    /* return 1 when x is within epsilon of 1 */
    /* return NUM_COLORS/2 when x=.5 */
    /* Use log scales when x > .5 or x < .5 */
    /*
     * epsilon=.001;  when x < epsilon, return 0 if(x>=1)  keep
     * numbers in bounds x=.99999; else if(x<.00001) x=.00001;
     * result=NUM_COLORS/2;
     * constant1=((NUM_COLORS/2-1))/log10(2*epsilon)-.0001;
     * if(x<.5) result=result - constant1*log10(2*x); else
     * result=result + constant1*log10(2-2*x); if(result < 0)
     * keep results in bounds result = 0; if(result > NUM_COLORS)
     * result = NUM_COLORS; end of new method
     * return((int)floor(result));
     */
  }
  return ((int) (x * NUM_COLORS));
}

/* open and define user defined colors from file for annotation */
FILE           *COLOR_ANN_FP;

int try_open_ann_color(void) {
  if ((COLOR_ANN_FP = fopen(color_ann_filename, "r")) == NULL) {
    printf("Error, Could not open annotation_color file %s\n",
	   color_ann_filename);
    return FALSE;
  } else
    return TRUE;
}

void set_annotation_colors_from_file(void) {
  int     color_file_exists, red_i, green_i, blue_i, i;
  float   red, green, blue;
  char    record[120];
  color_file_exists = try_open_ann_color();
  if (!color_file_exists)
    return;
  /* read and skip first line */
  if (fgets(record, 120, COLOR_ANN_FP) == NULL) {
    printf("Line %d of color file %s does not exist\n",
	   1, color_ann_filename);
    try_exit(32);
  }
  /* read and skip 2nd line */
  if (fgets(record, 120, COLOR_ANN_FP) == NULL) {
    printf("Line %d of color file %s does not exist\n",
	   2, color_ann_filename);
    try_exit(33);
  }
  for (i = 0; i < NUM_COLORS; i++) {
    if (fgets(record, 120, COLOR_ANN_FP) == NULL) {
      printf("Line %d of color file %s does not exist\n", i + 2, 
	     color_ann_filename);
      try_exit(34);
    }
    sscanf(record, "%f %f %f", &red, &green, &blue);
    red_i = (int) (255 * red + .5);
    if (red_i > 255)
      red_i = 255;
    green_i = (int) (255 * green + .5);
    if (green_i > 255)
      green_i = 255;
    blue_i = (int) (255 * blue + .5);
    if (blue_i > 255)
      blue_i = 255;
    color_table[i] = (16*16*16*16)*red_i + (16*16)*green_i + blue_i;
  }
  /* set the last color to be the same as the one before */
  color_table[NUM_COLORS] = color_table[NUM_COLORS - 1];
  /* skip a line */
  if (fgets(record, 120, COLOR_ANN_FP) == NULL) {
    printf("Line %d of color file %s does not exist\n",
	   NUM_COLORS + 3, color_ann_filename);
    try_exit(35);
  }
  for (i = 0; i < LOG_NUM_COLORS; i++) {
    if (fgets(record, 120, COLOR_ANN_FP) == NULL) {
      printf("Line %d of color file %s does not exist. (prob)\n",
	     i + NUM_COLORS + 3,
	     color_ann_filename);
      try_exit(36);
    }
    sscanf(record, "%f %f %f", &red, &green, &blue);
    red_i = (int) (255 * red + .5);
    if (red_i > 255)
      red_i = 255;
    green_i = (int) (255 * green + .5);
    if (green_i > 255)
      green_i = 255;
    blue_i = (int) (255 * blue + .5);
    if (blue_i > 255)
      blue_i = 255;
    log_color_table[i] = (16*16*16* 6)*red_i + (16*16)* green_i + blue_i;
  }
  log_color_table[LOG_NUM_COLORS] = log_color_table[LOG_NUM_COLORS - 1];
  fclose(COLOR_ANN_FP);
}
