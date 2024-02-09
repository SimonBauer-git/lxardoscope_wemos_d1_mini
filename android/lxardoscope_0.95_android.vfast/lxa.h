/*
 *
 * This file is part of the LXARDOSCOPE package.
 *
 * LXARDOSCOPE is an Arduino based oscilloscope for Linux, using the Xforms library.
 *
 * Copyright (C) 2013 Oskar Leuthold
 * 
 * LXARDOSCOPE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LXARDOSCOPE is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with LXARDOSCOPE; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */
# define DEFAULT_REFRESH_TIME 0.3
// update rate for the display (seconds)
#define DEFAULT_SAMPLING_RATE 2500
// baud rate of 115200 programmed as B115200
#define DEFAULT_BAUD_RATE 2000000
// DEFAULT_SAMPLING_RATE and DEFAULT_BAUD_RATE and DEFAULT_REFRESH_RATE
// determine the following interrelated buffer sizes. Do not change!
#define TBUF_CHUNK_SIZE 256 * 8
#define TBUF_NB_CHUNKS 128
#define TBUF_SIZE (TBUF_NB_CHUNKS * TBUF_CHUNK_SIZE)
#define RBUF_CHUNK_SIZE (2 * TBUF_CHUNK_SIZE)
#define RBUF_NB_CHUNKS 8
#define RBUF_SIZE (RBUF_CHUNK_SIZE * RBUF_NB_CHUNKS)
#define DATXY_SIZE 32768
#define VBUF_SIZE 65536
//
#define DEFAULT_REFERENCE  5.0
#define DEFAULT_TIME_OFFSET 0.0
#define DEFAULT_TIME_SCALE 10.0
#define DEFAULT_TRIG_LEVEL 0
#define DEFAULT_HYST 0.01
#define DEFAULT_TRIG_CHANNEL 0
#define DEFAULT_V_SCALE 2.0
#define DEFAULT_ADD_MODE 1			// use interpolation between channels
#define DEFAULT_COEFF 0.01			// highpass coefficient
									// -6dB @ 3Hz with 3000 samples/second

enum OPERATION_MODE {
			    OPERATION_MODE_NORMAL=0,
			    OPERATION_MODE_ADD=1,
			    OPERATION_MODE_XY=2
			   };
typedef enum{ ASC_TRIG=0, DESC_TRIG=1 } TRIG_TYPE;
typedef enum{ TRIG_OFF=0, TRIG_ON=1 } TRIG_ENB;

typedef struct {
 float reference;
 float sampling_rate;
 int baud_rate;
 int op_mode;
 int nbsamples; 		// number of samples per chunk 
 int disp_size;			// this is the full number of data points to be plotted,
						// as a function of Full Scale and effective sampling rate. 
						// Typically is some round value 
						// plus one, due to the data point at zero.
 int disp_mode;			// 0: normal  1: review, if stopped
 int disp_start;		// index of first sample to be displayed in normal mode
 int disp_actual;		// this is the actual number of data points to be plotted.  
 int disp_max;			// this is the maximum number of data points
 char devices[12][20]; 	// for serial port names: /dev/ttySX
 int verbose;
 char device_name[20];
 int dev_h;				// soundcard device handle
 int rchunk;     		// number of chunks read in
 int cchunk;     		// number of chunks copied
 int tchunk;			// number of chunks in tbuf (from 0 to 63)
 int datend;			// index of last valid data point in tbuf
 int trig;				// trigger index
 float delta;			// display shift due to trigger (less than one sample)
 int threadenb;			// enable flag for reading thread
 int record;			// flag for writing data to raw file 
 const char *rfname;	// name of output raw file
 int rhandle;			// handle of output raw file
 int ifile;				// flag for reading data from raw file instead of Arduino
						// 0: Arduino  1: file open, reading  2: finished, file closed 
 const char *ifname;	// input file name
 int ifsize;			// input file size (samples for each channel)
 FILE *ihandle;			// input file handle
 long int ifdatapos;  	// input data position
 long int ifvb_last;		// input file sample copied into last place of vbuf
 long int ifvb_first;	// input file sample copied into first place of vbuf
 float input_pos;			// user input value for file position
 int run;				// flag for processing and displaying data
						// 0 stopped (initially waiting for user input
						// 1 processing and displaying running
						// 2 review mode (must have run before)
 int vcount;			// number of samples in vbuf (used with file input)
 int vstart;			// first sample to be displayed (with file input)
 float charspersec;		// characters per second (as calculated in lx_run)
 float wx;				// pointer x value
 int pulse;
 int orchunk;
 char *texts[10];		// text pointers for annotating display
 int nbtexts;			// number of texts
 char tdisp[30];		// stores string for time legend
 char msgdisp[100];		// screen message
 float coeff;
} base_info;

typedef struct {
 int trig_chan; 
 float T_pos;         	// time offset
 float T_scale;       	// this is value from GUI counter, full display time
                      	// due to scaling issues, the notion of time per div
						// is much less convenient.
 float trig_level;
 float hyst;
 TRIG_TYPE trig_mode;
 TRIG_ENB trig_enb;
} horizontal_info;

typedef struct {
 int on[2];
 FL_COLOR color[2];
 float V_pos[2];
 float V_scale[2];
 int inv[2];
 float min[2],max[2],pp[2],rms[2],avg[2];
 int minvalue[2],maxvalue[2];
 int gndcalseq;			// sequencer for gnd cal; mostly advanced by display timer
 int gndcal1,gndcal2;	// flag for GND calibration ( 0 cal not performed
						// 1 when cal performed, 
						// 2 in progress etc)
 int acdc1,acdc2;		// flag for AC/DC (0 @ DC, 1 @ AC with gain=1, 2 @ AC with gain)
 float calvaldc1,calvaldc2;
 int acgain1,acgain2;
} channel_info;

extern FD_lxa *lxa;
extern base_info LX;
extern channel_info CHAN;
extern horizontal_info HOR;
extern float datx[2][DATXY_SIZE];
extern float daty[2][DATXY_SIZE];
extern float tbuf[2][TBUF_SIZE];   	//temp buffer for channel 0
extern signed short rbuf[];	//read-in buffer
extern 	char uparrow[];
extern 	char dnarrow[];
extern 	char *CH[2];
extern	char tdisp[];
extern char msg[100];

int timeval_subtract(struct timeval *tresult, struct timeval *tx, struct timeval *ty);
int pro_display(FL_OBJECT *xyplot, int mode);
int plot_file(FL_OBJECT *xyplot, int button);
int open_dev();
void close_dev();
void *lx_run(void *arg);
double rmsq(int r, double c, double d, double e);
void set_counter_Tscale(FL_OBJECT *ob);
void set_f_counter_Pos(float val);
void dispmsg(char *);
