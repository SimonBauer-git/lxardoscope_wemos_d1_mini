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

#include "xforms-1.0.93sp1_ext/forms.h"
#include "xforms-1.0.93sp1_ext/iscaler.h"
#include "lxa_gui.h"
#include "lxa.h"
#include <sys/dir.h>
#include <stdlib.h>


#ifndef _REENTRANT
#error ACK! You need to compile with _REENTRANT defined since this uses threads
#endif

// see lxa.h for an explanation of these sizes
float datx[2][DATXY_SIZE];	
float daty[2][DATXY_SIZE];
float tbuf[2][TBUF_SIZE];   	
signed short rbuf[RBUF_SIZE];	


FD_lxa *lxa=NULL;
FD_about *about=NULL;
FD_option *option=NULL;

base_info LX;	
horizontal_info	HOR;
channel_info CHAN;


void find_tty_devices()
{
	int i=1;
	int done=0;
	DIR *pDIR;
	struct dirent *entry;
	pDIR=opendir("/tmp/");
	if(pDIR!=NULL) {
		while (done==0) {
			entry = readdir(pDIR);
			if(entry!=NULL) {
				if (strncmp(entry->d_name, "ttyS", 4)== 0) {
					strcpy(LX.devices[i],"/tmp/");
					strcat(LX.devices[i],entry->d_name);
					i++;
				}
				else if (strncmp(entry->d_name, "ttyACM", 6)== 0) {
					strcpy(LX.devices[i],"/tmp/");
					strcat(LX.devices[i],entry->d_name);
					i++;
				}
				else if (strncmp(entry->d_name, "ttyUSB", 6)== 0) {
					strcpy(LX.devices[i],"/tmp/");
					strcat(LX.devices[i],entry->d_name);
					i++;
				}
				if(i>9) done=1;
			}
			else done=1;
		}
		closedir(pDIR);
		sprintf(LX.devices[0],"%d",i-1);
	}
}


int init_lxa(char *device_name) {  
	const char *undef="undefined";
	const char *tfile="file";
	const char *tty = "/tmp/tty";
	int n,ch;
	char str[20];
	find_tty_devices();
	fl_clear_choice(lxa->droplist_dev);
	strcpy(LX.device_name,device_name);
	LX.baud_rate=DEFAULT_BAUD_RATE;
	if(strcmp(device_name,undef)==0) {     // no external input
		fl_addto_choice(lxa->droplist_dev,undef);
		fl_set_object_color(lxa->droplist_dev,FL_DARKORANGE,FL_INACTIVE);
	}
	else if(strlen(device_name)<9 || strlen(device_name)>16 || strncmp(device_name,tty,8)!=0) {
		printf("Error: device name %s is not valid\n",device_name); 
		printf("       expecting string beginning with /dev/tty\n");
		printf("please select device on lxa Graphical User Interface\n\n");
		fl_addto_choice(lxa->droplist_dev,undef);
		fl_set_object_color(lxa->droplist_dev,FL_DARKORANGE,FL_INACTIVE);
	}
	else {
		if (open_dev()==0) {
			printf("can not initialize device %s\n",device_name);
			fl_addto_choice(lxa->droplist_dev,undef);
			fl_set_object_color(lxa->droplist_dev,FL_DARKORANGE,FL_INACTIVE);
		}
		else {
			fl_addto_choice(lxa->droplist_dev,device_name);
			fl_set_object_color(lxa->droplist_dev,FL_MCOL,FL_INACTIVE);
			fl_set_timer(lxa->timer_refresh,DEFAULT_REFRESH_TIME);  
			fl_activate_object(lxa->button_run);
			fl_show_object(lxa->button_run);
			fl_activate_object(lxa->verbose);		
			LX.rchunk=0;
			LX.cchunk=0;
			LX.threadenb=1;
			LX.run=1;
			fl_deactivate_object(lxa->record);
		}
	}
	int nbdev=atoi(LX.devices[0]);
	for(n=1;n<=nbdev;n++) {
		if(strcmp(device_name,LX.devices[n])!=0)
			fl_addto_choice(lxa->droplist_dev,LX.devices[n]);  
	} 
	fl_addto_choice(lxa->droplist_dev,tfile); 
	LX.reference= DEFAULT_REFERENCE;
	LX.sampling_rate=DEFAULT_SAMPLING_RATE;
	LX.op_mode=OPERATION_MODE_NORMAL;
	HOR.trig_chan=DEFAULT_TRIG_CHANNEL;
	HOR.T_pos=DEFAULT_TIME_OFFSET;
	HOR.T_scale=DEFAULT_TIME_SCALE;
	HOR.trig_level=DEFAULT_TRIG_LEVEL;
	HOR.trig_mode=ASC_TRIG;
	HOR.trig_enb=TRIG_OFF;
	HOR.hyst=DEFAULT_HYST;
	LX.delta=0;
	LX.trig=0;
	LX.record=0;
	LX.ifile=0;
	LX.verbose=0;  
	LX.disp_actual=0;
	LX.nbtexts=0;
	CHAN.color[0]=FL_RED;
	CHAN.color[1]=FL_GREEN;
	CHAN.V_pos[0]=2.0;
	CHAN.V_pos[1]=-2.0;
	for(ch=0;ch<2;ch++) {
		CHAN.on[ch]=1;
		CHAN.V_scale[ch]=DEFAULT_V_SCALE;
		CHAN.inv[ch]=0;
		CHAN.minvalue[ch]=0;
		CHAN.maxvalue[ch]=0;
	}
	CHAN.gndcalseq=0;
	CHAN.acdc1=0;
	CHAN.acdc2=0;
	CHAN.gndcal1=0;
	CHAN.gndcal2=0;
	CHAN.calvaldc1=0;
	CHAN.calvaldc2=0;
	CHAN.acgain1=10;
	CHAN.acgain2=10;
	sprintf(str, " %.*f", 1,DEFAULT_TIME_SCALE);
	fl_set_iscaler_input(lxa->counter_Tscale,str);	
	fl_set_iscaler_input(lxa->f_counter_Tscale,str);	
	sprintf(str, "   %.*f", 1,DEFAULT_V_SCALE);
	fl_set_iscaler_input(lxa->counter_Vscale[0],str);
	fl_set_iscaler_input(lxa->counter_Vscale[1],str);
	printf("** initial sampling rate set to %d\n",(int)LX.sampling_rate);
	fl_show_object(lxa->trigger_Group);
	fl_hide_object(lxa->filehandling_Group);
	return 1;
}

int main(int argc, char *argv[])
{
	FD_lxa *fd_lxa;
	FD_about *fd_about;
	FD_option *fd_option;
	fl_initialize(&argc,argv,"lxardoscope",0,0);
	char msg[]="Select a port or a file to display";
	strcpy(LX.msgdisp,msg);
	fd_lxa = create_form_lxa(LX.msgdisp);
	fd_about = create_form_about();
	about=fd_about;
	fd_option = create_form_option();
	option=fd_option;
	lxa=fd_lxa;
	if (argc > 1) init_lxa(argv[1]);
	else init_lxa("undefined");
	// show the GUI; its title is defined here
	fl_show_form(fd_lxa->lxa,FL_PLACE_CENTERFREE,FL_FULLBORDER,"lxardoscope_0.95");
	fl_set_atclose(CB_at_close,NULL);
	fl_do_forms();
	return 0;
}
