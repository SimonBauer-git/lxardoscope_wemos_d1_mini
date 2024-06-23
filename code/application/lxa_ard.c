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
#include <termios.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#define RUNBUFSIZE 8292
#define BAUDRATE B2000000

// from http://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
int set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		//                error_message ("error %d from tcgetattr", errno);
		printf("error1\n");
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // ignore break signal
	tty.c_lflag = 0;                // no signaling chars, no echo,
	// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	//tty.c_cc[VMIN]  = 200;            // read doesn't block
	//tty.c_cc[VTIME] = 1;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
	// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		//                error_message ("error %d from tcsetattr", errno);
		printf("error2\n");
		return -1;
	}
	return 0;
}

int atmega_start() {
	unsigned char inString[3];
	int n=0;
	int count=0;
	sprintf(msg,"opening port %s",LX.device_name);
	dispmsg(msg);
	sleep(1);
	while(1) {
		unsigned char sum;
		n=read(LX.dev_h,inString,3);
		sum = (inString[0] >> 7) + (inString[1] >> 7) + (inString[2] >> 7);
		if (sum == 1 || sum == 2)
			break;
		else {
			usleep(200000);
			if(LX.verbose) printf("%d %d %u %u %u\n",n,count,inString[0],inString[1],inString[2]);
			if(count>20) { 
				sprintf(msg,"no response from %s, please select different port",
						LX.device_name);
				dispmsg(msg);				
				close(LX.dev_h);
				return 0;
			}
			count++;
		}
	}
	if(LX.verbose) printf("last response at count=%d) %u %u %u\n",count,inString[0],inString[1],inString[2]);	
	sprintf(msg,"%s ready, press START",LX.device_name);
	dispmsg(msg);
	return 1;
}

int open_dev1() {
	LX.dev_h = open (LX.device_name, O_RDWR | O_NOCTTY | O_SYNC);
	if (LX.dev_h <= 0)
	{
		printf("error3\n");
		return 0;
	}


	set_interface_attribs (LX.dev_h, BAUDRATE, 0);  // set speed to 250000 bps, 8n1 (no parity)
	int flag=atmega_start();
	return flag;
}

// version from 0.9:
struct termios oldtio,newtio;

int open_dev()
{
	int BAUD;
#if 0
	if(LX.baud_rate==115200) BAUD = B115200;
	else {
		//		printf("only baud rate of 115200 supported\n");
		dispmsg("only baud rate of 115200 supported\n");
		return 0;
	}
#endif
	BAUD = BAUDRATE;
	if(LX.verbose) printf("trying to open device <%s>\n",LX.device_name);
	LX.dev_h = open(LX.device_name, O_RDWR | O_NOCTTY ); 
	if (LX.dev_h<0) {perror(LX.device_name); exit(-1); }
	if(LX.verbose) printf("success: device open\n");      
	tcgetattr(LX.dev_h,&oldtio); /* save current port settings */

	bzero(&newtio, sizeof(newtio));


	// this is OK
	newtio.c_cflag = BAUD | CS8 | CLOCAL | CREAD;
	//  stty -F /dev/ttyUSB0 -g
	//	4:0:18b2:0:0:0:0:0:0:1:c8:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
	//	stty -F /dev/ttyUSB0 -a
	//	speed 115200 baud; rows 0; columns 0; line = 0;
	//	intr = <undef>; quit = <undef>; erase = <undef>; kill = <undef>; eof = <undef>;
	//	eol = <undef>; eol2 = <undef>; swtch = <undef>; start = <undef>; stop = <undef>;
	//	susp = <undef>; rprnt = <undef>; werase = <undef>; lnext = <undef>; flush = <undef>;
	//  min = 200; time = 1; 
	//	-parenb -parodd cs8 -hupcl -cstopb cread clocal -crtscts -ignbrk -brkint ignpar -parmrk
	//	-inpck -istrip -inlcr -igncr -icrnl -ixon -ixoff -iuclc -ixany -imaxbel -iutf8
	//	-opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0
	//	-isig -icanon -iexten -echo -echoe -echok -echonl -noflsh -xcase -tostop -echoprt -echoctl -echoke
	// this is not OK
	//		newtio.c_cflag = BAUD | CRTSCTS | CS8 | CLOCAL | CREAD;
	//	stty -F /dev/ttyUSB0 -g
	//	4:0:800018b2:0:0:0:0:0:0:1:c8:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
	//  stty -F /dev/ttyUSB0 -a
	//	speed 115200 baud; rows 0; columns 0; line = 0;
	//	intr = <undef>; quit = <undef>; erase = <undef>; kill = <undef>; eof = <undef>; 
	//	eol = <undef>; //	eol2 = <undef>; swtch = <undef>; start = <undef>; stop = <undef>;
	//	susp = <undef>; rprnt = <undef>; werase = <undef>; lnext = <undef>; flush = <undef>;
	//	min = 200; time = 1; 
	//	-parenb -parodd cs8 -hupcl -cstopb cread clocal crtscts -ignbrk -brkint ignpar -parmrk
	//	-inpck -istrip -inlcr -igncr -icrnl -ixon -ixoff -iuclc -ixany -imaxbel -iutf8
	//	-opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0
	//	-isig -icanon -iexten -echo -echoe -echok -echonl -noflsh -xcase -tostop -echoprt
	//	 -echoctl -echoke
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 1;   	
	newtio.c_cc[VMIN]     = 200;   	/* blocking read until 200 chars received */
	/* however this is ignored; see read command below */
	tcflush(LX.dev_h, TCIFLUSH);
	tcsetattr(LX.dev_h,TCSANOW,&newtio);
	int flag=atmega_start();
	return flag;
}






void close_dev() {
	if(LX.dev_h) {
		if(LX.verbose) printf("closing serial port <%s>\n",LX.device_name);
		tcsetattr(LX.dev_h,TCSANOW,&oldtio);
		close(LX.dev_h);
	}
	else {
		if(LX.verbose) printf("no serial port to close\n");
	}
}


//************************************************************************
/* serial data structure:
	 byte 1  A0 5 most significant bits + 224 (128+64+32)
	 byte 2  A0 5 least significant bits + 96 (64+32)
	 byte 3  A1 5 most significant bits +  96 (64+32)
	 byte 4  A1 5 least significant bits +  96 (64+32)
	 for byte 1, legitimate values are between 224 and 255
	 for bytes 2, 3 and 4 legitimate values are between 96 and 127
 */

void *lx_run(void *arg)
{
	if(LX.verbose) printf(
			"data acquisition thread running with pid= %d tid=%ld\n",
			getpid(),(long int)syscall(224));
	int res,slot,start;
	unsigned char bufn[RUNBUFSIZE];
	unsigned char bufj[3]={0,0,0};
	int n=0,k=0,j=0;  
	//		int rchar;   // characters read
	//		int inchunk; // counts number of input chunks
	int nbchar=0;
	short ch[2];
	int valid;
	int calcount1=0,calcount2=0;
	float calval1=0,calval2=0;
	int minval[2]={0,0}, maxval[2]={0,0};
	float mean1=0, mean2=0;
	float coeff=DEFAULT_COEFF;
	struct timeval tres;
	struct timeval sta;
	struct timeval fin;
	slot=LX.rchunk%RBUF_NB_CHUNKS;
	start=slot*RBUF_CHUNK_SIZE;
	LX.pulse=0;
	LX.orchunk=0;  //  old rchunk value, for pulse display
	//		if (open_dev()==0) {
	//			return 0;
	//		}
	// flushing helps for startup
	tcflush(LX.dev_h, TCIFLUSH);
	gettimeofday(&sta, NULL);
	res = read(LX.dev_h,bufn,RUNBUFSIZE);
	//		rchar=res;
	//		inchunk=1;
	while (LX.threadenb) {
		if(bufn[n] & 0x80) { 
			if (j == 2) bufj[2] = 0;
			if (bufj[0] & 0x80 && !(bufj[1] & 0x80) && !(bufj[1] & 0x80))
				valid = 1;
			else
				valid = 0;
			if(valid) {
				ch[0] = ((bufj[0] & 0x7F) << 3) | (bufj[1] >> 4);
				ch[1] = 0;
				if(ch[0]==0) minval[0]++;
				if(ch[1]==0) minval[1]++;
				if(ch[0]==1023) maxval[0]++;
				if(ch[1]==1023) maxval[1]++;
				if(CHAN.acdc1==0) rbuf[start+k]=(short)(ch[0]-CHAN.calvaldc1);
				else rbuf[start+k]=(short)(ch[0]-mean1);
				mean1=(1-coeff)*mean1+coeff*(float)ch[0];
				if(CHAN.acdc2==0) rbuf[start+k+1]=(short)(ch[1]-CHAN.calvaldc2);
				else rbuf[start+k+1]=(short)(ch[1]-mean2);
				mean2=(1-coeff)*mean2+coeff*(float)ch[1];
				if(CHAN.gndcal1>1) {
					calval1=calval1+(float)ch[0];
					calcount1++;
					if(calcount1==1000) {
						CHAN.calvaldc1=calval1/(float)calcount1;
						if(LX.verbose) {
							printf("DC calibration offset for channel 1 = %3.1f\n",CHAN.calvaldc1);
						}
						CHAN.gndcal1=1;
						CHAN.gndcalseq++;
						calval1=0; 
						calcount1=0;
					}
				}
				if(CHAN.gndcal2>1) {
					calval2=calval2+(float)ch[1];
					calcount2++;
					if(calcount2==1000) {
						CHAN.calvaldc2=calval2/(float)calcount2;
						if(LX.verbose) {
							printf("DC calibration offset for channel 2 = %3.1f\n",CHAN.calvaldc2);
						}
						CHAN.gndcal2=1;
						CHAN.gndcalseq++;
						calval2=0; 
						calcount2=0;
					} 
				}
				k=k+2;

				if(LX.record==1) {
					int e;
					if((e=write(LX.rhandle,&ch,4))<0) {
						if(LX.verbose) printf("write failed %s\n", strerror(errno));
						close(LX.rhandle);
						exit(1);
					}
				}
				if(k>=RBUF_CHUNK_SIZE) {
					k=0;
					LX.orchunk=LX.rchunk;
					LX.rchunk++;
					slot=LX.rchunk%RBUF_NB_CHUNKS;
					start=slot*RBUF_CHUNK_SIZE;

					if(minval[0]>0) CHAN.minvalue[0]=1;
					if(maxval[0]>0) CHAN.maxvalue[0]=1;
					if(minval[1]>0) CHAN.minvalue[1]=1;
					if(maxval[1]>0) CHAN.maxvalue[1]=1;
					minval[0]=0; minval[1]=0; maxval[0]=0; maxval[1]=0;

					//if(LX.rchunk%40==20) 
					if(LX.rchunk%8==4) 
					{
						gettimeofday(&fin, NULL);
						timeval_subtract(&tres,&fin,&sta);
						//double tspan=(double)tres.tv_sec+1e-6*(double)tres.tv_usec;
						// update the effective rate:
						double tspan = 0.05;
						nbchar = 32768;
						LX.charspersec=nbchar/tspan;
						// charspersec=3*sampling_rate
						// with some moving average:
						if (CHAN.on[1]) 
							LX.sampling_rate=(3.0*LX.sampling_rate+LX.charspersec)/6.0;
						else
							LX.sampling_rate=(4.0*LX.sampling_rate+LX.charspersec)/6.0;

						//						set_lxa_time_scale();
						if(LX.verbose) {
							printf("***** elapsed time=%2.3f, %d bytes received, %d bytes per second, averaged sampling rate= %d\n",
									tspan,nbchar,(int)rint(LX.charspersec),(int)rint(LX.sampling_rate));
						}
						gettimeofday(&sta, NULL);
						//nbchar=0;					uncommented because nbchar is fixed now
					}//
				}//	if(k>=RBUF_CHUNK_SIZE)	
			} //if(valid)
			j=0;
		}
		if(j<3) bufj[j]=bufn[n];
		j++;
		n++;
		if(n>=res) {
			res = read(LX.dev_h,bufn,RUNBUFSIZE);
			//nbchar=nbchar+res;	uncommented because nbchar is a fixed value now for stability
			n=0;
		}
		if(j>1000) {
			if(LX.verbose) printf("no valid data found in 1000 samples\n");
			LX.threadenb=0;
			j=10;
		}
	} //while (LX.threadenb)


	//tcsetattr(LX.dev_h,TCSANOW,&oldtio);
	return 0;
}


