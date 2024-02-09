/**
 * file ipushwheel.c
 *
 * this file derived from scrollbar.c (hierarchical widget) and
 * counter.c
 *
 * features:
 * input field for entering value either
 *     - manually, with number of characters set by fl_set_ipushwheel_maxchars
 *     - programmatically by fl_set_ipushwheel_value
 * 3 or 4 pushbuttons to adjust value shown in the input field,
 * with step size of 1,10, or 100 (or 1000) * sstep.
 * text is aligned right (fl_set_ipushwheel_maxchars)
 * text format chosen by fl_set_ipushwheel_precision
 * value is limited by fl_set_ipushwheel_bounds
 * color for compontents chosen by fl_set_ipushwheel_color
 * Note that unlike for the standard components, the sequence
 * of setting the properties matters.
 *
 * text alignment is not as nice as for built in components; maybe later ...
 *
 * types: 	1: 3 buttons, FL_INT_INPUT
 * 			2: 4 buttons, FL_INT_INPUT
 *	 		3: 3 buttons, FL_FLOAT_INPUT
 *			4: 4 buttons, FL_FLOAT_INPUT
 */

#include "xforms-1.0.93sp1_ext/forms.h"
#include "xforms-1.0.93sp1_ext/config.h"
#include "xforms-1.0.93sp1_ext/flinternal.h"
#include "xforms-1.0.93sp1_ext/ipushwheel.h"

#include <string.h>
#include <stdlib.h>
#include <float.h>

static void
get_geom( FL_OBJECT * ob )
{
    FLI_IPUSHWHEEL_SPEC *sp = ob->spec;
    FL_OBJECT *button1   = sp->button1,
              *button2   = sp->button2,
              *button3   = sp->button3,
              *button4   = sp->button4,
              *button5   = sp->button5,
              *button6   = sp->button6,
              *button7   = sp->button7,
              *button8   = sp->button8,
              *input 	 = sp->input;
//
	if((sp->draw_type==1) | (sp->draw_type ==3)) {

//		button arrangement:		1 2 3
//								5 6 7		
		sp->ww[1] = (int)(0.33f * ob->w);
		sp->ww[2] = 3 * sp->ww[1];
		sp->hh[1] = sp->hh[3] = (int)(0.33f * ob->h); 
		sp->hh[2] = ob->h - 2 * sp->hh[1];        
		sp->xx[0] = ob->x;
		sp->xx[1] = sp->xx[0] + sp->ww[1];
		sp->xx[2] = sp->xx[1] + sp->ww[1];
		sp->yy[0] = ob->y;
		sp->yy[1] = sp->yy[0] + sp->hh[1];
		sp->yy[2] = sp->yy[1] + sp->hh[2];


	}
	else {

//		button arrangement:		1 2 3 4
//								5 6 7 8	
        sp->ww[1] = (int)(0.25f * ob->w);
        sp->ww[2] = 4 * sp->ww[1];
		sp->hh[1] = sp->hh[3] = (int)(0.33f*ob->h);  
		sp->hh[2] = ob->h - 2 * sp->hh[1];        
		sp->xx[0] = ob->x;
		sp->xx[1] = sp->xx[0] + sp->ww[1];
		sp->xx[2] = sp->xx[1] + sp->ww[1];
		sp->xx[3] = sp->xx[2] + sp->ww[1];
		sp->yy[0] = ob->y;
    	sp->yy[1] = sp->yy[0] + sp->hh[1];
		sp->yy[2] = sp->yy[1] + sp->hh[2];
		button4->x = sp->xx[3]; button4->y = sp->yy[0];
		button4->w = sp->ww[1]; button4->h = sp->hh[1];
		button8->x = sp->xx[3]; button8->y = sp->yy[2];
		button8->w = sp->ww[1]; button8->h = sp->hh[3];	
	}
		button1->x = sp->xx[0]; button1->y = sp->yy[0];
		button1->w = sp->ww[1]; button1->h = sp->hh[1];
		button2->x = sp->xx[1]; button2->y = sp->yy[0];
		button2->w = sp->ww[1]; button2->h = sp->hh[1];
		button3->x = sp->xx[2]; button3->y = sp->yy[0];
		button3->w = sp->ww[1]; button3->h = sp->hh[1];
		input->x   = sp->xx[0]; input->y   = sp->yy[1];
		input->w   = sp->ww[2]; input->h   = sp->hh[2];
		button5->x = sp->xx[0]; button5->y = sp->yy[2];
		button5->w = sp->ww[1]; button5->h = sp->hh[3];
		button6->x = sp->xx[1]; button6->y = sp->yy[2];
		button6->w = sp->ww[1]; button6->h = sp->hh[3];
		button7->x = sp->xx[2]; button7->y = sp->yy[2];
		button7->w = sp->ww[1]; button7->h = sp->hh[3];	
}


/***************************************
 ***************************************/

static int
handle_ipushwheel( FL_OBJECT * obj,
                int         event,
                FL_Coord    mx,
                FL_Coord    my,
                int         key  FL_UNUSED_ARG,
                void *      ev   FL_UNUSED_ARG )
{
//printf("col1=%d col2=%d\n",(int)obj->col1,(int)obj->col2);
    switch ( event )
    {
        case FL_ATTRIB :
        case FL_RESIZED :
            get_geom( obj );
            break;

        case FL_DRAW :
        case FL_DRAWLABEL :
            if ( obj->label && *obj->label )
                fl_draw_object_label_outside( obj );
            break;

        case FL_FREEMEM :
            /* children will take care of themselves */
            fl_free( obj->spec );
            break;
    }

    return FL_RETURN_NONE;
}



/***************************************
 * Callback for the input in the ipushwheel
 ***************************************/

static void 
update_input(FL_OBJECT * ob) {
	char str[ 64 ];
    FLI_IPUSHWHEEL_SPEC *sp = ob->spec;
	sprintf( str, "%.*f", sp->prec, sp->val );
	if(sp->maxchars-strlen(str)>5)        sprintf( str, "      %.*f", sp->prec, sp->val );
	else if(sp->maxchars-strlen(str)==5)  sprintf( str, "     %.*f", sp->prec, sp->val );
	else if (sp->maxchars-strlen(str)==4) sprintf( str, "    %.*f", sp->prec, sp->val );
	else if (sp->maxchars-strlen(str)==3) sprintf( str, "   %.*f", sp->prec, sp->val );
	else if (sp->maxchars-strlen(str)==2) sprintf( str, "  %.*f", sp->prec, sp->val );
	else if (sp->maxchars-strlen(str)==1) sprintf( str, " %.*f", sp->prec, sp->val );
	fl_set_input(sp->input,str);		
}


static void
input_cb( FL_OBJECT * obj,
           long        data  FL_UNUSED_ARG )
{
	double val;    
    FLI_IPUSHWHEEL_SPEC *sp = obj->parent->spec;    
	val=atof(fl_get_input(sp->input));
	sp->val=sp->sstep*rint(val/sp->sstep); 	
	if (sp->val > sp->max) sp->val=sp->max;
	if (sp->val < sp->min) sp->val=sp->min;
//printf("%f\n",sp->val);
	update_input(obj->parent);
//printf("%f\n",sp->val);
//	sprintf( str, "%.*f", sp->prec, sp->val );	
//	fl_set_input(sp->input,str);
	sp->button=0;
	obj->parent->returned |= FL_RETURN_CHANGED;
}


/***************************************
 * Callback for the buttons of the ipushwheel
 ***************************************/

static void
button_cb( FL_OBJECT * obj,long button )
{
    FLI_IPUSHWHEEL_SPEC *sp = obj->parent->spec;
	sp->button=button;
	if((sp->draw_type==1) | (sp->draw_type ==3)) {
		if (button==1) sp->val += 100*sp->sstep;
		if (button==2) sp->val += 10*sp->sstep;
		if (button==3) sp->val += sp->sstep;
		if (button==5) sp->val -= 100*sp->sstep;
		if (button==6) sp->val -= 10*sp->sstep;
		if (button==7) sp->val -= sp->sstep;
	}
	else {
		if (button==1) sp->val += 1000*sp->sstep;
		if (button==2) sp->val += 100*sp->sstep;
		if (button==3) sp->val += 10*sp->sstep;
		if (button==4) sp->val += sp->sstep;
		if (button==5) sp->val -= 1000*sp->sstep;
		if (button==6) sp->val -= 100*sp->sstep;
		if (button==7) sp->val -= 10*sp->sstep;
		if (button==8) sp->val -= sp->sstep;
	}
 	if (sp->val > sp->max) sp->val=sp->max;
	if (sp->val < sp->min) sp->val=sp->min;
	update_input(obj->parent);
//    	I am not going to figure out all these combinations; 
//		see fl_set_ipushwheel_return below
//		this is just a combination which works for me right now
//
//    if ( obj->returned & FL_RETURN_END )
//        obj->parent->returned |= FL_RETURN_END;
//    if (    obj->parent->how_return & FL_RETURN_END_CHANGED
//         && obj->returned & FL_RETURN_END )
//            obj->parent->returned |= FL_RETURN_CHANGED;
//    else if ( obj->returned & FL_RETURN_CHANGED )
//        obj->parent->returned |= FL_RETURN_CHANGED;
obj->parent->returned |= FL_RETURN_CHANGED;
}


FL_OBJECT *
fl_create_ipushwheel( int          type,
                     FL_Coord     x,
                     FL_Coord     y,
                     FL_Coord     w,
                     FL_Coord     h,
                     const char * label )
{
    FLI_IPUSHWHEEL_SPEC *sp;
    FL_OBJECT *obj;

    obj = fl_make_object( FL_COUNTER, type, x, y, w, h, label, handle_ipushwheel );
    obj->boxtype     = FL_COUNTER_BOXTYPE;
    obj->col1        = FL_COUNTER_COL1;
    obj->col2        = FL_COUNTER_COL2;
    obj->align       = FL_COUNTER_ALIGN;
    obj->lcol        = FL_COUNTER_LCOL;
    obj->want_motion = 1;
    obj->want_update = 1;
    obj->set_return = fl_set_ipushwheel_return;


    if ( obj->bw == FL_BOUND_WIDTH && obj->bw == 3 )
        obj->bw = FL_COUNTER_BW;

    sp = obj->spec     = fl_calloc( 1, sizeof *sp );
    sp->filter        = NULL;
    sp->min_repeat_ms = 50;
    sp->repeat_ms     = 600;
    sp->do_speedjump  = 0;
    sp->timeout_id    = -1;
	sp->ipushwheel       = obj;
	sp->draw_type=type;

	sp->button1 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+38>" );
	sp->button2 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+38>" );
	sp->button3 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+38>" );
	sp->button5 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+32>" );
	sp->button6 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+32>" );
	sp->button7 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+32>" );

    fl_set_object_callback( sp->button1, button_cb, 1 );
    fl_set_object_resize( sp->button1, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button2, button_cb, 2 );
    fl_set_object_resize( sp->button2, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button3, button_cb, 3 );
    fl_set_object_resize( sp->button3, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button5, button_cb, 5 );
    fl_set_object_resize( sp->button5, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button6, button_cb, 6 );
    fl_set_object_resize( sp->button6, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button7, button_cb, 7 );
    fl_set_object_resize( sp->button7, FL_RESIZE_NONE );

    fl_add_child( obj, sp->button1 );
    fl_add_child( obj, sp->button2 );
    fl_add_child( obj, sp->button3 );
    fl_add_child( obj, sp->button5 );
    fl_add_child( obj, sp->button6 );
    fl_add_child( obj, sp->button7 );

	if((sp->draw_type==2) | (sp->draw_type ==4)) {
	sp->button4 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+38>" );
	sp->button8 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@+32>" );
    fl_set_object_callback( sp->button4, button_cb, 4 );
    fl_set_object_resize( sp->button4, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button8, button_cb, 8 );
    fl_set_object_resize( sp->button8, FL_RESIZE_NONE );
    fl_add_child( obj, sp->button4 );
    fl_add_child( obj, sp->button8 );
	}

	if((sp->draw_type==1) | (sp->draw_type==2)) sp->input=fl_create_input(FL_INT_INPUT,1,1,1,1,"");
	else sp->input=fl_create_input(FL_FLOAT_INPUT,1,1,1,1,"");
    fl_set_object_callback(sp->input,input_cb,7);
    fl_set_object_resize( sp->input, FL_RESIZE_NONE );    
	fl_set_object_lalign(sp->input,FL_ALIGN_TOP);
	fl_add_child(obj,sp->input);                                         
    fl_set_object_return( obj, FL_RETURN_CHANGED );
    return obj;
}


/***************************************
 ***************************************/

FL_OBJECT *
fl_add_ipushwheel( int          type,
                  FL_Coord     x,
                  FL_Coord     y,
                  FL_Coord     w,
                  FL_Coord     h,
                  const char * l ) {
    FL_OBJECT *obj = fl_create_ipushwheel( type, x, y, w, h, l );
    get_geom( obj );
    fl_add_object( fl_current_form, obj );
    return obj;
}


/***************************************
 ***************************************/


void fl_set_ipushwheel_maxchars(FL_OBJECT *ob, int maxchars) {
    FLI_IPUSHWHEEL_SPEC *sp = ob->spec;
	sp->maxchars=maxchars;
	fl_set_input_maxchars(sp->input,maxchars);
}

//void fl_set_ipushwheel_nbchars(FL_OBJECT *ob, int nbchars) {
//    FLI_IPUSHWHEEL_SPEC *sp = ob->spec;
//	sp->nbchars=nbchars;
//}


/***************************************
 ***************************************/


double fl_get_ipushwheel_value( FL_OBJECT * obj ) {
    FLI_IPUSHWHEEL_SPEC *sp = obj->spec;
    return sp->val;
}

int
fl_get_ipushwheel_button(FL_OBJECT * ob) {
    return ((FLI_IPUSHWHEEL_SPEC *) (ob->spec))->button;
}
/***************************************
 ***************************************/

//   the following is called from fl_create_ipushwheel -> obj->set_return
void
fl_set_ipushwheel_return( FL_OBJECT    * obj,
                         unsigned int   when ) {
    FLI_IPUSHWHEEL_SPEC *sp = obj->spec;

//    	I am not going to figure out all these combinations; 
//		see button_cb above
//		this is just a combination which works for me right now
//		
//    if ( when & FL_RETURN_END_CHANGED )
//        when &= ~ ( FL_RETURN_NONE | FL_RETURN_CHANGED );
//    obj->how_return = when;
	obj->how_return=FL_RETURN_CHANGED;
    fl_set_object_return( sp->input, FL_RETURN_END_CHANGED );
    fl_set_object_return( sp->button1,   FL_RETURN_CHANGED );
    fl_set_object_return( sp->button2,   FL_RETURN_CHANGED );
    fl_set_object_return( sp->button3,   FL_RETURN_CHANGED );
    fl_set_object_return( sp->button5,   FL_RETURN_CHANGED );
    fl_set_object_return( sp->button6,   FL_RETURN_CHANGED );
    fl_set_object_return( sp->button7,   FL_RETURN_CHANGED );
	if((sp->draw_type==1) | (sp->draw_type ==3)) {
    	fl_set_object_return( sp->button4,   FL_RETURN_CHANGED );
    	fl_set_object_return( sp->button8,   FL_RETURN_CHANGED );
	}

}


void fl_set_ipushwheel_bounds( FL_OBJECT * ob,
                                      double      min,
                                      double      max ) {
//printf("min=%f max=%f\n",min,max);
	FLI_IPUSHWHEEL_SPEC *sp = ob->spec;	
    if ( sp->min != min || sp->max != max )    {
        sp->min = min;
        sp->max = max;
        sp->val = fli_clamp( sp->val, sp->min, sp->max );
        fl_redraw_object( ob );
    }
}


void fl_set_ipushwheel_step( FL_OBJECT * ob, double s ) {
//printf("step s=%f\n",s);
	FLI_IPUSHWHEEL_SPEC *sp = ob->spec;	
    if ( sp->sstep != s )    {
        sp->sstep = s;
        fl_redraw_object( ob );
    }
}



void fl_set_ipushwheel_precision( FL_OBJECT * ob,int prec ) {
//printf("precision prec=%d\n",prec);
	FLI_IPUSHWHEEL_SPEC *sp = ob->spec;	
    if ( prec < 0 ) prec = 0;
    if ( prec > DBL_DIG )  prec = DBL_DIG;
    if ( sp->prec != prec )    {
        sp->prec = prec;
        fl_redraw_object( ob );
    }
}	


void fl_set_ipushwheel_value( FL_OBJECT * ob, double      val ) {
//printf("value val=%f\n",val);
	FLI_IPUSHWHEEL_SPEC *sp = ob->spec;
	sp->val=val;
	update_input(ob);
}

void fl_set_ipushwheel_color(FL_OBJECT * ob,FL_COLOR col1,FL_COLOR col2) {
	FLI_IPUSHWHEEL_SPEC *sp = ob->spec;	
	sp->col1=col1;
	sp->col2=col2;
//printf("col1=%d col2=%d\n",(int)sp->col1,(int)sp->col2);
	fl_set_object_color(sp->button1,sp->col1,sp->col1);
	fl_set_object_lcolor(sp->button1,sp->col2);
	fl_set_object_color(sp->button2,sp->col1,sp->col1);
	fl_set_object_lcolor(sp->button2,sp->col2);
	fl_set_object_color(sp->button3,sp->col1,sp->col1);
	fl_set_object_lcolor(sp->button3,sp->col2);
	fl_set_object_color(sp->button5,sp->col1,sp->col1);
	fl_set_object_lcolor(sp->button5,sp->col2);
	fl_set_object_color(sp->button6,sp->col1,sp->col1);
	fl_set_object_lcolor(sp->button6,sp->col2);
	fl_set_object_color(sp->button7,sp->col1,sp->col1);
	fl_set_object_lcolor(sp->button7,sp->col2);	
	if((sp->draw_type==2) | (sp->draw_type ==4)) {
		fl_set_object_color(sp->button4,sp->col1,sp->col1);
		fl_set_object_lcolor(sp->button4,sp->col2);
		fl_set_object_color(sp->button8,sp->col1,sp->col1);
		fl_set_object_lcolor(sp->button8,sp->col2);	
	}
}


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
