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

#ifndef FD_lxa_h_
#define FD_lxa_h_

/** Callbacks, globals and object handlers **/
extern void CB_xyplot_display(FL_OBJECT *, long);
extern void CB_button_color0(FL_OBJECT *, long);
extern void CB_check_chan0(FL_OBJECT *, long);
extern void CB_droplist_dev(FL_OBJECT *, long);
extern void CB_droplist_trigmode(FL_OBJECT *, long);
extern void CB_droplist_trigchan(FL_OBJECT *, long);
extern void CB_droplist_opmode(FL_OBJECT *, long);
extern void CB_button_close(FL_OBJECT *, long);
extern void CB_button_spectrum(FL_OBJECT *, long);
extern void CB_button_display_setup(FL_OBJECT *, long);
extern void CB_logotipo(FL_OBJECT *, long);
extern void CB_counter_Vpos1(FL_OBJECT *, long);
extern void CB_counter_Vscale1(FL_OBJECT *, long);
extern void CB_counter_Tscale(FL_OBJECT *, long);
extern void CB_counter_Tpos(FL_OBJECT *, long);
extern void CB_button_color1(FL_OBJECT *, long);
extern void CB_check_chan1(FL_OBJECT *, long);
extern void CB_counter_Vpos2(FL_OBJECT *, long);
extern void CB_counter_Vscale2(FL_OBJECT *, long);
extern void CB_timer_refresh(FL_OBJECT *, long);
extern void CB_button_inv_chan1(FL_OBJECT *, long);
extern void CB_button_inv_chan2(FL_OBJECT *, long);
extern void CB_button_run(FL_OBJECT *, long);
extern void CB_check_trigger(FL_OBJECT *, long);
extern void CB_button_about_OK(FL_OBJECT *, long);
extern void CB_trig_level(FL_OBJECT *, long);
extern void CB_reference(FL_OBJECT *, long);
extern void CB_verbose(FL_OBJECT *, long);
extern void CB_trig_channel(FL_OBJECT *, long);
extern void CB_trig_mode(FL_OBJECT *, long);
extern void CB_record(FL_OBJECT *, long);
extern void CB_f_position(FL_OBJECT *, long);
extern void CB_button_pulse(FL_OBJECT *, long);
extern void CB_counter_hyst(FL_OBJECT *, long);
extern void CB_f_rate(FL_OBJECT *, long);
extern void CB_input_Pos(FL_OBJECT *, long);
extern int CB_file(const char *, void *);
extern int CB_at_close(FL_FORM *,void *);
extern void CB_option(FL_OBJECT *, long);
extern void CB_input_gain1(FL_OBJECT *, long);
extern void CB_input_gain2(FL_OBJECT *, long);
extern void CB_option_OK(FL_OBJECT *, long);
extern void CB_droplist_inputmode1(FL_OBJECT *, long);
extern void CB_droplist_inputmode2(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *lxa;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *xyplot_display;
	FL_OBJECT *button_color[2];
	FL_OBJECT *check_chan[2];
	FL_OBJECT *droplist_dev;
	FL_OBJECT *droplist_trigmode;
	FL_OBJECT *droplist_trigchan;
	FL_OBJECT *droplist_opmode;
	FL_OBJECT *button_close;
	FL_OBJECT *button_spectrum;
	FL_OBJECT *button_display_setup;
	FL_OBJECT *text_mouse_V[2];
	FL_OBJECT *text_mouse_T;
	FL_OBJECT *text_avg_chan[2];
	FL_OBJECT *text_pp_chan[2];
	FL_OBJECT *text_rms_chan[2];
	FL_OBJECT *logotipo;
	FL_OBJECT *option;
	FL_OBJECT *counter_Vpos[2];
	FL_OBJECT *counter_Vscale[2];
	FL_OBJECT *text_min_chan[2];
	FL_OBJECT *text_max_chan[2];
	FL_OBJECT *counter_Tscale;
	FL_OBJECT *trig_level;
	FL_OBJECT *counter_Tpos;
	FL_OBJECT *timer_refresh;
	FL_OBJECT *button_inv_chan[2];
	FL_OBJECT *button_run;
	FL_OBJECT *indicator_triggered;
	FL_OBJECT *record;
    FL_OBJECT *check_trigger;
    FL_OBJECT *reference;
    FL_OBJECT *verbose;
    FL_OBJECT *trig_channel;
    FL_OBJECT *trig_mode;
	FL_OBJECT *arrow;
	FL_OBJECT *input;
	FL_OBJECT *f_counter_Pos;
	FL_OBJECT *button_pulse;
	FL_OBJECT *counter_Hyst;
	FL_OBJECT *f_counter_Tscale;
	FL_OBJECT *f_Rate;
	FL_OBJECT *f_trig_channel;
	FL_OBJECT *trigger_Group;
	FL_OBJECT *filehandling_Group;
	FL_OBJECT *input_Pos;
	FL_OBJECT *droplist_inputmode1;
	FL_OBJECT *droplist_inputmode2;
} FD_lxa;

extern FD_lxa * create_form_lxa(char *);
typedef struct {
	FL_FORM *about;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *button_about_OK;
} FD_about;
extern FD_about * create_form_about(void);

typedef struct {
	FL_FORM *option;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *option_OK;
	FL_OBJECT *input_gain1;
	FL_OBJECT *input_gain2;
} FD_option;

extern FD_option * create_form_option(void);

#endif /* FD_lxa_h_ */

