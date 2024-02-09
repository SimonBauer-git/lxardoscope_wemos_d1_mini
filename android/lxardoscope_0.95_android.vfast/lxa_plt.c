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
#include "lxa_gui.h"
#include "lxa.h"

char uparrow[]="@+28->";
char dnarrow[]="@+26->";
char *CH[2];
char tdisp[]="time(ms)";


void disp_axes(FL_OBJECT *xyplot,int disp_size) {
	float mdx[6]={0,0,0,0,0,disp_size},mdy[6]={0,0,5.0,-5.0,0,0}; 
	float tx[2],ty[2];   //for trigger line
	int chan[2];
	float delta=LX.delta;
	CH[0]="Channel 1";
	CH[1]="Channel 2";
	chan[0]=HOR.trig_chan; 
	chan[1]=HOR.trig_chan==0?1:0; 

	// delete all texts
	int txt;
	if(LX.nbtexts>0) {
		for(txt=0;txt<LX.nbtexts;txt++) {
			fl_delete_xyplot_text(xyplot,LX.texts[txt]);
		}
		LX.nbtexts=0;
	}

	//float ymin, ymax;
	//fl_get_xyplot_ybounds(xyplot, &ymin, &ymax);
	//printf("%f %f\n",ymin,ymax);
	switch (LX.op_mode) {
		case OPERATION_MODE_NORMAL:
		case OPERATION_MODE_ADD: {
															 //      datx contains shift by delta; remove for bounds 		
															 mdx[0]=datx[chan[0]][0]+delta;
															 mdx[5]=datx[chan[0]][disp_size-1]+delta;
															 fl_set_xyplot_xbounds(xyplot,mdx[0],mdx[5]);	   
															 fl_get_xyplot_ybounds(xyplot,&mdy[2],&mdy[3]);	   
															 LX.nbtexts=1;
															 LX.texts[0]=tdisp;
															 ///	   	fl_add_xyplot_text(xyplot,0.5*(mdx[0]+mdx[5]),-5.5,tdisp,FL_ALIGN_CENTER,FL_WHITE);
															 fl_set_xyplot_data(xyplot,mdx,mdy,6,"","","");
															 //     want yellow lines for x=0 and y=0:
															 fl_add_xyplot_overlay(xyplot,1,mdx,mdy,6,FL_YELLOW);
															 break;
														 }
		case OPERATION_MODE_XY: {
															mdx[0]=-5.0;
															mdx[5]=5.0;
															fl_set_xyplot_xbounds(xyplot,-5.0,5.0);
															fl_set_xyplot_alphaxtics(xyplot,
																	"@-5.0|@-4.0|@-3.0|-@-2.0|@-1.0|0@0.0|@1.0|+@2.0|@3.0|@4.0|@5.0","");
															LX.nbtexts=4;
															LX.texts[0]=uparrow;
															LX.texts[1]=dnarrow;
															LX.texts[2]=CH[0];
															LX.texts[3]=CH[1];
															fl_add_xyplot_text(xyplot,-4.8,-5.45,uparrow,FL_ALIGN_RIGHT,FL_WHITE);
															fl_add_xyplot_text(xyplot,2.4,-5.45,dnarrow,FL_ALIGN_RIGHT,FL_WHITE);
															fl_add_xyplot_text(xyplot,-4.3,-5.45,CH[chan[0]],FL_ALIGN_RIGHT,FL_WHITE);
															fl_add_xyplot_text(xyplot,2.9,-5.45,CH[chan[1]],FL_ALIGN_RIGHT,FL_WHITE);
															fl_set_xyplot_data(xyplot,mdx,mdy,6,"","","");	
															fl_add_xyplot_overlay(xyplot,1,mdx,mdy,6,FL_YELLOW); 	 
														}
	}
	// trigger line
	if(HOR.trig_enb) {
		tx[0]=mdx[0];tx[1]=mdx[5];
		ty[0]=ty[1]=CHAN.V_pos[HOR.trig_chan]+HOR.trig_level/CHAN.V_scale[HOR.trig_chan];
		fl_set_xyplot_overlay_type(xyplot,4,FL_DASHED_XYPLOT);
		fl_add_xyplot_overlay(xyplot,4,tx,ty,2,FL_CYAN);        	
		if(HOR.hyst>0.005) {
			switch (HOR.trig_mode) {
				case ASC_TRIG:
					ty[0]=ty[1]=ty[0]-HOR.hyst/CHAN.V_scale[HOR.trig_chan];
					break;
				case DESC_TRIG:
					ty[0]=ty[1]=ty[0]+HOR.hyst/CHAN.V_scale[HOR.trig_chan];
			}
			fl_set_xyplot_overlay_type(xyplot,5,FL_DOTTED_XYPLOT);
			fl_add_xyplot_overlay(xyplot,5,tx,ty,2,FL_MAGENTA);
		}
		else fl_delete_xyplot_overlay(xyplot,5);
	}
	else {
		fl_delete_xyplot_overlay(xyplot,4);
		fl_delete_xyplot_overlay(xyplot,5);
	}
}

// assuming three equally spaced data points c,d and e, this function 
// calculates integral, 
// with range=0 from first to second data point
// with range=1 from first to third data point
double rmsq(int range,double c, double d, double e) {
	double a,b;
	a=(c-d-d+e);
	b=(d+d+d+d-c-c-c-e);
	if(range) return 2.0*(a*a*0.8+a*b+(4.0*a*c+b*b)/3.0+(b+c)*c);
	else return a*a/20.0+a*b/8.0+a*c/3.0+b*b/12.0+b*c/2.0+c*c;
}


//************************************************************************
// This subroutine processes and displays data from the Arduino
//
//
int pro_display(FL_OBJECT *xyplot, int mode)
{
	int trig_pos=0;
	int disp_size=HOR.T_scale*LX.sampling_rate/1000+1;
	// cannot change during execution
	float sampling_rate=LX.sampling_rate;	//cannot change during execution
	int ds=disp_size;							// for starting
	int chan[2],ch;
	float mult[2];
	int toffset=0;
	int shift=0;
	char cad[16];
	float delta=0;
	static float lastrbuf[3];  // needed for subsequent routine calls
	int rstart,tstart,tstop,datend;
	int k,n,cont;
	int update=0;
	int armed=0;

	chan[0]=HOR.trig_chan; 
	chan[1]=HOR.trig_chan==0?1:0;        // not trig channel

	// local copy, to make sure the variable does not change while we are updating
	// note that these are the numbers of chunks, not indices
	int rch=LX.rchunk;	// number of chunks read by lx_run
	int cch=LX.cchunk;	// number of chunks copied previously
	int tch=LX.tchunk;	// number of chunks in tbuf (from 0 to 63)
	int nch; // number of new chunks to be copied from lx_run thread
	int ech; // number of chunks in excess of 64 
	// this is normal mode, with display being updated regularly from Arduino data
	if(mode==0) {
		if (rch<cch) {
			printf("illegal condition (1)\n");
			return 0;
		}
		if (rch==cch) return 0;
		nch=rch-cch;

		if (nch>TBUF_NB_CHUNKS-4) {     		// we can not catch up more than a full tbuf
			// leave some margin 
			cch=rch-TBUF_NB_CHUNKS+4;
			nch=rch-cch;
		}
		// do not take data from the last two chunks, they might be filling)
		if (nch>RBUF_NB_CHUNKS-2) nch=RBUF_NB_CHUNKS-2;

		// make sure there is no garbage in the buffer at startup
		if(cch==0) { 
			for (k=0;k<TBUF_SIZE;k++) {
				tbuf[0][k]=0;
				tbuf[1][k]=0;
			}
			LX.trig=0;
			LX.tchunk=0;
			lastrbuf[0]=0;
			lastrbuf[1]=0;
			lastrbuf[2]=0;
		}

		// multiplier for each channel; accounting for inversion and off condition	
		for(ch=0;ch<2;ch++) {
			mult[ch]=LX.reference/1024.0;
			if (CHAN.inv[ch]) mult[ch]*=-1;
			if (CHAN.on[ch]==0) mult[ch]=0;
		}
		if(CHAN.acdc1==2) mult[0]=mult[0]/(float)CHAN.acgain1;
		if(CHAN.acdc2==2) mult[1]=mult[1]/(float)CHAN.acgain2;
		// interpolation in ADD mode is special: the last pair can not be processed, before
		// first data point from new set is available. Therefore processing is one step behind:
		// the last three samples from rbuf are stored (static!). Sample 2 is used for 
		// interpolating channel 1.  Samples 1 and 3 for interpolating channel 2 to create
		// the first sample of the new set. 

		if(nch>0 && tch+nch<=TBUF_NB_CHUNKS) { 
			for (n=0;n<nch;n++) {
				rstart=RBUF_CHUNK_SIZE*((rch-nch+n)%RBUF_NB_CHUNKS);
				tstart=(tch+n)*TBUF_CHUNK_SIZE;
				if(LX.op_mode==1) {
					if(DEFAULT_ADD_MODE==0) {
						for (k=0;k<TBUF_CHUNK_SIZE;k++) {
							tbuf[0][tstart+k]=rbuf[rstart+2*k]*mult[0]+rbuf[rstart+2*k+1]*mult[1];
						}
					}
					else {
						tbuf[0][tstart]=0.25*((3.0*lastrbuf[1]+rbuf[rstart])*mult[0]+
								(lastrbuf[0]+3.0*lastrbuf[2])*mult[1]);
						tbuf[0][tstart+1]=0.25*((3.0*rbuf[rstart]+rbuf[rstart+2])*mult[0]
								+(lastrbuf[2]+3.0*rbuf[rstart+1])*mult[1]);
						for (k=2;k<TBUF_CHUNK_SIZE;k++) {
							tbuf[0][tstart+k]=0.25*((3.0*rbuf[rstart+2*k-2]+rbuf[rstart+2*k])*mult[0]
									+(rbuf[rstart+2*k-3]+3.0*rbuf[rstart+2*k-1])*mult[1]);
						}
						lastrbuf[0]=rbuf[rstart+2*TBUF_CHUNK_SIZE-3];
						lastrbuf[1]=rbuf[rstart+2*TBUF_CHUNK_SIZE-2];
						lastrbuf[2]=rbuf[rstart+2*TBUF_CHUNK_SIZE-1];
					}
				}
				else {
					for (k=0;k<TBUF_CHUNK_SIZE;k++) {
						tbuf[0][tstart+k]=rbuf[rstart+2*k]*mult[0];
						tbuf[1][tstart+k]=rbuf[rstart+2*k+1]*mult[1];				
					}
				}
			}
			LX.tchunk=LX.tchunk+nch;
			LX.cchunk=LX.cchunk+nch;
			datend=LX.tchunk*TBUF_CHUNK_SIZE;
		}
		else if(nch>0 && tch+nch>TBUF_NB_CHUNKS) { 
			ech=tch+nch-TBUF_NB_CHUNKS;
			if(ech<0) ech=0;
			tstart=ech*TBUF_CHUNK_SIZE;
			for (k=0;k<TBUF_CHUNK_SIZE*(tch-ech);k++) {
				tbuf[0][k]=tbuf[0][tstart+k];
				tbuf[1][k]=tbuf[1][tstart+k];
			}
			for (n=0;n<nch;n++) {
				rstart=RBUF_CHUNK_SIZE*((rch-nch+n)%RBUF_NB_CHUNKS);
				tstart=(tch-ech+n)*TBUF_CHUNK_SIZE;
				if(LX.op_mode==1) {
					if(DEFAULT_ADD_MODE==0) {
						for (k=0;k<TBUF_CHUNK_SIZE;k++) {
							tbuf[0][tstart+k]=rbuf[rstart+2*k]*mult[0]+rbuf[rstart+2*k+1]*mult[1];
						}
					}
					else {
						tbuf[0][tstart]=0.25*((3.0*lastrbuf[1]+rbuf[rstart])*mult[0]
								+(lastrbuf[0]+3.0*lastrbuf[2])*mult[1]);
						tbuf[0][tstart+1]=0.25*((3.0*rbuf[rstart]+rbuf[rstart+2])*mult[0]
								+(lastrbuf[2]+3.0*rbuf[rstart+1])*mult[1]);
						for (k=2;k<TBUF_CHUNK_SIZE;k++) {
							tbuf[0][tstart+k]=0.25*((3.0*rbuf[rstart+2*k-2]+rbuf[rstart+2*k])*mult[0]
									+(rbuf[rstart+2*k-3]+3.0*rbuf[rstart+2*k-1])*mult[1]);
						}
						lastrbuf[0]=rbuf[rstart+2*TBUF_CHUNK_SIZE-3];
						lastrbuf[1]=rbuf[rstart+2*TBUF_CHUNK_SIZE-2];
						lastrbuf[2]=rbuf[rstart+2*TBUF_CHUNK_SIZE-1];			
					}
				}
				else {
					for (k=0;k<TBUF_CHUNK_SIZE;k++) {
						tbuf[0][tstart+k]=rbuf[rstart+2*k]*mult[0];
						tbuf[1][tstart+k]=rbuf[rstart+2*k+1]*mult[1];				
					}
				}
			}
			LX.tchunk=tch-ech+nch;
			LX.cchunk=LX.cchunk+nch;
			datend=LX.tchunk*TBUF_CHUNK_SIZE;
			LX.trig=LX.trig-ech*TBUF_CHUNK_SIZE;
		}
		else {
			printf("illegal condition (2)\n");
			return 0; 
		}
		if(LX.trig<0) LX.trig=0;
		switch (HOR.trig_enb)  {
			case TRIG_OFF:       
				if(ds>datend) {
					ds=datend;
					tstart=0; 
				}
				else tstart=datend-ds;
				for (cont=0;cont<disp_size;cont++) {
					datx[0][cont]=cont*1000.0/sampling_rate;
					datx[1][cont]=(cont+0.5)*1000.0/sampling_rate;
				}
				for (ch=0;ch<2;ch++) {
					for (cont=0;cont<ds;cont++)
						daty[ch][cont]=tbuf[ch][cont+tstart];
				}	
				LX.disp_start=tstart;
				break;
			case TRIG_ON:
				update=0;
				toffset=HOR.T_pos*sampling_rate/1000.0;
				armed=0;
				if(HOR.hyst<0.001) armed=2;
				//
				// in this first part, try to find trigger point such that full display is possible.
				// search is in descending time, because we want to find latest possible trigger point.
				// If trigger point is found, and hysteresis requested, then search for hysteresis.
				// While this search is going on, look for earlier trigger points as well, such that 
				// the trigger point is selected which is closest to a hysteresis point.
				// armed flag:
				// 0 no hysteresis point found
				// 1 hysteresis point found before trigger point (actually later in time!)
				// 2 hysteresis point found after trigger point (actually earlier in time)
				//    or no hysteresis requested
				//
				if(datend-toffset>ds) {
					tstart=datend-toffset-ds;
					if(tstart>datend) tstart=datend;
					switch (HOR.trig_mode) {
						case ASC_TRIG:
							for (k=tstart-1;k>=0;k--) {
								if(tbuf[chan[0]][k]<=HOR.trig_level && tbuf[chan[0]][k+1]>HOR.trig_level) {
									trig_pos=k;
									update=1;
									delta=(HOR.trig_level-tbuf[chan[0]][k])/
										(tbuf[chan[0]][k+1]-tbuf[chan[0]][k]);
									if(armed==2) break;
								}
								if(tbuf[chan[0]][k]<=HOR.trig_level-HOR.hyst) {
									armed=1;						
									if(update) { armed=2; break; }
								}
							}
							break;
						case DESC_TRIG:
							for (k=tstart-1;k>=0;k--) {
								if(tbuf[chan[0]][k]>=HOR.trig_level && tbuf[chan[0]][k+1]<HOR.trig_level) {
									trig_pos=k;
									update=1;
									delta=(HOR.trig_level-tbuf[chan[0]][k])/
										(tbuf[chan[0]][k+1]-tbuf[chan[0]][k]);
									if(armed==2) break;
								}
								if(tbuf[chan[0]][k]>=HOR.trig_level+HOR.hyst) {
									armed=1;						
									if(update) { armed=2; break; }
								}
							}
							break;
					}
				}
				if(armed<2) update=0;
				//
				// in this second part, executed only if the first part was not successful in finding
				// a trigger point, we search ascending in time, right from the beginning of the 
				// data which actually needs to be displayed; this is where the first part search began.
				// If trigger point is found here, we end up with a reduced amount of data in the display.
				// If in the first part a hysteresis point was found, it is inherited here.
				if(update==0) { 
					tstop=datend;
					if(toffset>0) tstop=datend-toffset;
					// allow for some overlap
					tstart=datend-toffset-ds-10;
					if(tstart<0) tstart=0;
					if(tstart>datend) tstart=datend;
					switch (HOR.trig_mode) {
						case ASC_TRIG:
							for (k=tstart;k<tstop;k++) {
								if(armed) {
									if(tbuf[chan[0]][k]<=
											HOR.trig_level && tbuf[chan[0]][k+1]>HOR.trig_level) {
										trig_pos=k;
										update=1;
										delta=(HOR.trig_level-tbuf[chan[0]][k])/
											(tbuf[chan[0]][k+1]-tbuf[chan[0]][k]);
										break;
									}
								}
								else {
									if(tbuf[chan[0]][k]<=HOR.trig_level-HOR.hyst) armed=1;
								}
							}
							break;
						case DESC_TRIG:
							for (k=tstart;k<tstop;k++) {
								if(armed) {
									if(tbuf[chan[0]][k]>=
											HOR.trig_level && tbuf[chan[0]][k+1]<HOR.trig_level) {
										trig_pos=k;
										update=1;
										delta=(HOR.trig_level-tbuf[chan[0]][k])/
											(tbuf[chan[0]][k+1]-tbuf[chan[0]][k]);
										break;
									}
								}
								else {
									if(tbuf[chan[0]][k]>=HOR.trig_level+HOR.hyst) armed=1;
								}
							}
							break;
					}
				} // if(update==0)
				//
				if(HOR.hyst>0.001) {
					if(armed==0) update=0;
				}
				//
				if(update==0) {
					LX.trig=0;
					fl_delete_xyplot_overlay(xyplot,2);
					fl_delete_xyplot_overlay(xyplot,3);
					for (ch=0; ch<2;ch++) {
						fl_set_object_label(lxa->text_min_chan[ch],"*");
						fl_set_object_label(lxa->text_max_chan[ch],"*");
						fl_set_object_label(lxa->text_pp_chan[ch],"*");
						fl_set_object_label(lxa->text_avg_chan[ch],"*");
						fl_set_object_label(lxa->text_rms_chan[ch],"*");
					}
					disp_axes(xyplot,disp_size);
					return 1;
				}
				else fl_set_object_color(lxa->indicator_triggered,FL_GREEN,FL_GREEN);

				if(datend-trig_pos-toffset<ds) ds=datend-trig_pos-toffset;
				LX.trig=trig_pos;
				toffset=HOR.T_pos*sampling_rate/1000.0;
				LX.delta=delta*1000.0/sampling_rate;
				if(HOR.trig_chan) {
					for (cont=0;cont<disp_size;cont++) {
						datx[0][cont]=(cont+toffset-0.5)*1000.0/sampling_rate-LX.delta;
						datx[1][cont]=(cont+toffset)*1000.0/sampling_rate-LX.delta;
					}
				}
				else {
					for (cont=0;cont<disp_size;cont++) {
						datx[0][cont]=(cont+toffset)*1000.0/sampling_rate-LX.delta;
						datx[1][cont]=(cont+toffset+0.5)*1000.0/sampling_rate-LX.delta;
					}
				}
				if(trig_pos+toffset>0) {
					for (ch=0;ch<2;ch++) {
						for (cont=0;cont<ds;cont++)
							daty[ch][cont]=tbuf[ch][cont+trig_pos+toffset];
					}
				}
				else {
					shift=-trig_pos-toffset;	
					for (ch=0;ch<2;ch++) {
						for (cont=0;cont<shift;cont++)
							daty[ch][cont]=0;
					}		
					for (ch=0;ch<2;ch++) {
						for (cont=shift;cont<ds;cont++)
							daty[ch][cont]=tbuf[ch][cont+trig_pos+toffset];
					}
				}		
				LX.disp_start=trig_pos;
		} //switch (HOR.trig_enb)
		LX.datend=datend;
	} // if (mode==0) (normal mode)
	/////
	else {			// review mode
		// toffset,sampling rate and disp_size can change in review mode; however the 
		// data is from previous sweep 
		toffset=HOR.T_pos*sampling_rate/1000.0;
		ds=disp_size;
		switch (HOR.trig_enb)  {
			case TRIG_OFF:   
				for (cont=0;cont<disp_size;cont++) {
					datx[0][cont]=cont*1000.0/sampling_rate;
					datx[1][cont]=(cont+0.5)*1000.0/sampling_rate;
				}
				for (ch=0;ch<2;ch++) {
					for (cont=0;cont<ds;cont++)
						daty[ch][cont]=tbuf[ch][cont+LX.disp_start];
				}
				break;	
			case TRIG_ON:
				if(LX.disp_start+toffset<0) toffset= -LX.disp_start;
				if(LX.disp_start+toffset+ds>LX.datend) ds=LX.datend-LX.disp_start-toffset;
				if(HOR.trig_chan) {
					for (cont=0;cont<disp_size;cont++) {
						datx[0][cont]=(cont+toffset-0.5)*1000.0/sampling_rate-LX.delta;
						datx[1][cont]=(cont+toffset)*1000.0/sampling_rate-LX.delta;
					}
				}
				else {
					for (cont=0;cont<disp_size;cont++) {
						datx[0][cont]=(cont+toffset)*1000.0/sampling_rate-LX.delta;
						datx[1][cont]=(cont+toffset+0.5)*1000.0/sampling_rate-LX.delta;
					}
				}
				for (ch=0;ch<2;ch++) {
					for (cont=0;cont<ds;cont++)
						daty[ch][cont]=tbuf[ch][cont+LX.disp_start+toffset];
				}
		}
	}
	///////
	LX.disp_actual=ds; // save; needed for pointer in cb.c
	// calculate and update channel information
	float s;
	int nbch=2;
	if(LX.op_mode==1) {
		nbch=1;
		fl_set_object_label(lxa->text_min_chan[1],"*");
		fl_set_object_label(lxa->text_max_chan[1],"*");
		fl_set_object_label(lxa->text_pp_chan[1],"*");
		fl_set_object_label(lxa->text_avg_chan[1],"*");
		fl_set_object_label(lxa->text_rms_chan[1],"*");	
	}

	for (ch=0; ch<nbch;ch++) {
		CHAN.max[ch]=-1000000;
		CHAN.min[ch]=1000000;
		CHAN.avg[ch]=0;
		for (cont=0; cont<ds; cont++) {
			s=daty[ch][cont];
			if (s<CHAN.min[ch]) CHAN.min[ch]=s;
			if (s>CHAN.max[ch]) CHAN.max[ch]=s;
			CHAN.avg[ch]=CHAN.avg[ch]+s;
		}
		CHAN.pp[ch]=CHAN.max[ch]-CHAN.min[ch];
		CHAN.avg[ch]=CHAN.avg[ch]/ds;

		double rms=rmsq(0,daty[ch][0],daty[ch][1],daty[ch][2]);
		//printf("%d %e    %e %e %e %d %d\n",ch,rms,daty[ch][0],daty[ch][1],daty[ch][2],rch,nch);
		for (cont=2; cont<ds; cont++) {
			rms=rms+rmsq(1,daty[ch][cont-2],daty[ch][cont-1],daty[ch][cont]);
		}	
		//printf("%d %e\n",ch,rms);
		rms=rms+rmsq(0,daty[ch][ds-1],daty[ch][ds-2],daty[ch][ds-3]);
		//printf("%d %e    %e %e %e \n",ch,rms,daty[ch][ds-1],daty[ch][ds-2],daty[ch][ds-3]);
		if(rms>0) CHAN.rms[ch]=sqrt(0.5*rms/(double)ds);
		else CHAN.rms[ch]=0;	

		sprintf(cad,"%.3f",CHAN.min[ch]);
		fl_set_object_label(lxa->text_min_chan[ch],cad);
		sprintf(cad,"%.3f",CHAN.max[ch]);
		fl_set_object_label(lxa->text_max_chan[ch],cad);
		sprintf(cad,"%.3f",CHAN.pp[ch]);
		fl_set_object_label(lxa->text_pp_chan[ch],cad);
		sprintf(cad,"%.3f",CHAN.avg[ch]);
		fl_set_object_label(lxa->text_avg_chan[ch],cad);
		sprintf(cad,"%.3f",CHAN.rms[ch]);
		fl_set_object_label(lxa->text_rms_chan[ch],cad);
	}
	// indicate over- and under-range in the input
	// note that gndcal shifts the values getting into this routine
	// thus a displayed value of zero does not indicate GND after gndcal procedure.
	for (ch=0; ch<2;ch++) {
		if(CHAN.minvalue[ch]>5) {
			fl_set_object_lcolor(lxa->text_min_chan[ch],FL_BLACK);
			CHAN.minvalue[ch]=0;
		}		
		else if(CHAN.minvalue[ch]>0) {
			fl_set_object_lcolor(lxa->text_min_chan[ch],FL_RED);
			CHAN.minvalue[ch]++;
		}
		if(CHAN.maxvalue[ch]>5) {
			fl_set_object_lcolor(lxa->text_max_chan[ch],FL_BLACK);
			CHAN.maxvalue[ch]=0;
		}							
		else if(CHAN.maxvalue[ch]>0) {
			fl_set_object_lcolor(lxa->text_max_chan[ch],FL_RED);
			CHAN.maxvalue[ch]++;
		}
	}


	switch (LX.op_mode) {
		case OPERATION_MODE_NORMAL: {
																	disp_axes(xyplot,disp_size);
																	for (ch=0; ch<2;ch++) {
																		for (cont=0; cont<ds; cont++) {
																			daty[ch][cont]=CHAN.V_pos[ch]+daty[ch][cont]/CHAN.V_scale[ch];
																		}
																		if (CHAN.on[ch]) {
																			fl_add_xyplot_overlay(xyplot,ch+2,datx[ch],daty[ch],ds,CHAN.color[ch]);
																		}
																		else fl_delete_xyplot_overlay(xyplot,ch+2);
																	}
																	break;
																}
		case OPERATION_MODE_ADD: {
															 disp_axes(xyplot,disp_size);
															 for (cont=0;cont<ds;cont++) {
																 daty[0][cont]=CHAN.V_pos[0]+daty[0][cont]/CHAN.V_scale[0];
															 }
															 fl_add_xyplot_overlay(xyplot,2,datx[0],daty[0],ds,CHAN.color[0]);
															 fl_delete_xyplot_overlay(xyplot,3);
															 break;
														 }
		case OPERATION_MODE_XY: {
															disp_axes(xyplot,disp_size);       
															for (ch=0; ch<2;ch++) {
																for (cont=0; cont<ds; cont++) {
																	daty[chan[ch]][cont]=CHAN.V_pos[chan[ch]]+daty[chan[ch]][cont]/CHAN.V_scale[chan[ch]];
																}
															}      
															fl_add_xyplot_overlay(xyplot,2,daty[chan[1]],daty[chan[0]],ds,CHAN.color[chan[0]]);
															fl_delete_xyplot_overlay(xyplot,3);
															break;
														}
	} 
	return 1;
}



