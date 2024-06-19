
/**
 * file iscaler.c
 *
 * this file derived from scrollbar.c (hierarchical widget) and
 * counter.c
 */






#include "xforms-1.0.93sp1_ext/forms.h"
#include "xforms-1.0.93sp1_ext/config.h"
#include "xforms-1.0.93sp1_ext/flinternal.h"
#include "xforms-1.0.93sp1_ext/iscaler.h"

#include <string.h>
#include <stdlib.h>
#include <float.h>

static void
get_geom( FL_OBJECT * ob )
{
    FLI_ISCALER_SPEC *sp = ob->spec;
    FL_OBJECT *button1   = sp->button1,
              *button2   = sp->button2,
              *button3   = sp->button3,
              *button4   = sp->button4,
              *input 	 = sp->input;
        sp->ww[ 0 ] = sp->ww[ 1 ] = sp->ww[ 2 ] = sp->ww[ 3 ] =
                                                 FL_min( 0.18 * ob->w, ob->h );
        sp->ww[ 4 ] = ob->w - 4 * sp->ww[ 0 ]; 
        sp->xx[ 0 ] = ob->x;
        sp->xx[ 1 ] = sp->xx[ 0 ] + sp->ww[ 0 ];
        sp->xx[ 4 ] = sp->xx[ 1 ] + sp->ww[ 1 ];
        sp->xx[ 2 ] = sp->xx[ 4 ] + sp->ww[ 4 ];
        sp->xx[ 3 ] = sp->xx[ 2 ] + sp->ww[ 2 ];

		button1->x = sp->xx[ 0 ];
		button1->y = ob->y;
		button1->w = sp->ww[ 0 ];
		button1->h = ob->h;
		button2->x = sp->xx[ 1 ];
		button2->y = ob->y;
		button2->w = sp->ww[ 1 ];
		button2->h = ob->h;
		input->x   = sp->xx[ 4 ];
		input->y   = ob->y;
		input->w   = sp->ww[ 4 ];
		input->h   = ob->h;
		button3->x = sp->xx[ 2 ];
		button3->y = ob->y;
		button3->w = sp->ww[ 2 ];
		button3->h = ob->h;
		button4->x = sp->xx[ 3 ];
		button4->y = ob->y;
		button4->w = sp->ww[ 3 ];
		button4->h = ob->h;
}



/***************************************
 ***************************************/

static int
handle_iscaler( FL_OBJECT * obj,
                int         event,
                FL_Coord    mx,
                FL_Coord    my,
                int         key  FL_UNUSED_ARG,
                void *      ev   FL_UNUSED_ARG )
{
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
 * Callback for the input in the iscaler
 ***************************************/

static void
input_cb( FL_OBJECT * obj,
           long        data  FL_UNUSED_ARG )
{
    FLI_ISCALER_SPEC *sp = obj->parent->spec;
	sp->button=0;
	obj->parent->returned |= FL_RETURN_ALWAYS;
}


/***************************************
 * Callback for the buttons of the iscaler
 ***************************************/

static void
button_cb( FL_OBJECT * obj,long val )
{
    FLI_ISCALER_SPEC *sp = obj->parent->spec;
sp->button=val;
    if ( obj->returned & FL_RETURN_END )
        obj->parent->returned |= FL_RETURN_END;
    if (    obj->parent->how_return & FL_RETURN_END_CHANGED
         && obj->returned & FL_RETURN_END )
            obj->parent->returned |= FL_RETURN_CHANGED;
    else if ( obj->returned & FL_RETURN_CHANGED )
        obj->parent->returned |= FL_RETURN_CHANGED;
}


FL_OBJECT *
fl_create_iscaler( int          type,
                     FL_Coord     x,
                     FL_Coord     y,
                     FL_Coord     w,
                     FL_Coord     h,
                     const char * label )
{
    FLI_ISCALER_SPEC *sp;
    FL_OBJECT *obj;

    obj = fl_make_object( FL_COUNTER, type, x, y, w, h, label, handle_iscaler );
    obj->boxtype     = FL_COUNTER_BOXTYPE;
    obj->col1        = FL_COUNTER_COL1;
    obj->col2        = FL_COUNTER_COL2;
    obj->align       = FL_COUNTER_ALIGN;
    obj->lcol        = FL_COUNTER_LCOL;
    obj->want_motion = 1;
    obj->want_update = 1;
    obj->set_return = fl_set_iscaler_return;


    if ( obj->bw == FL_BOUND_WIDTH && obj->bw == 3 )
        obj->bw = FL_COUNTER_BW;

    sp = obj->spec     = fl_calloc( 1, sizeof *sp );
    sp->filter        = NULL;
    sp->min_repeat_ms = 50;
    sp->repeat_ms     = 600;
    sp->do_speedjump  = 0;
    sp->timeout_id    = -1;
	sp->iscaler       = obj;

	sp->button1 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@#<<" );
	sp->button2 = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@#<" );
	sp->button3   = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@#>" );
	sp->button4   = fl_create_button( FL_TOUCH_BUTTON, 1, 1, 1, 1, "@#>>" );

    fl_set_object_callback( sp->button3, button_cb, 1 );
    fl_set_object_resize( sp->button3, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button4, button_cb, 2 );
    fl_set_object_resize( sp->button4, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button2, button_cb, -1 );
    fl_set_object_resize( sp->button2, FL_RESIZE_NONE );
    fl_set_object_callback( sp->button1, button_cb, -2 );
    fl_set_object_resize( sp->button1, FL_RESIZE_NONE );

    fl_add_child( obj, sp->button1 );
    fl_add_child( obj, sp->button2 );
    fl_add_child( obj, sp->button3 );
    fl_add_child( obj, sp->button4 );

	sp->input=fl_create_input(type,1,1,1,1,"");
    fl_set_object_callback(sp->input,input_cb,4);
    fl_set_object_resize( sp->input, FL_RESIZE_NONE );    
	fl_set_object_lalign(sp->input,FL_ALIGN_TOP);
	fl_add_child(obj,sp->input);                                         

    fl_set_object_return( obj, FL_RETURN_CHANGED );
    return obj;
}






/***************************************
 ***************************************/

FL_OBJECT *
fl_add_iscaler( int          type,
                  FL_Coord     x,
                  FL_Coord     y,
                  FL_Coord     w,
                  FL_Coord     h,
                  const char * l )
{
    FL_OBJECT *obj = fl_create_iscaler( type, x, y, w, h, l );

    get_geom( obj );
    fl_add_object( fl_current_form, obj );

    return obj;
}


/***************************************
 ***************************************/


void fl_set_iscaler_maxchars(FL_OBJECT *ob, int maxchars) {
    FLI_ISCALER_SPEC *sp = ob->spec;
	fl_set_input_maxchars(sp->input,maxchars);
}

/***************************************
 ***************************************/

void
fl_set_iscaler_input( FL_OBJECT * ob, const char *str ) {
    FLI_ISCALER_SPEC *sp = ob->spec;
 fl_set_input(sp->input,str);
}


const char *
fl_get_iscaler_input( FL_OBJECT * obj )
{
    FLI_ISCALER_SPEC *sp = obj->spec;
    return fl_get_input(sp->input);
}

int
fl_get_iscaler_button(FL_OBJECT * ob)
{
    return ((FLI_ISCALER_SPEC *) (ob->spec))->button;
}
/***************************************
 ***************************************/


void
fl_set_iscaler_return( FL_OBJECT    * obj,
                         unsigned int   when )
{
    FLI_ISCALER_SPEC *sp = obj->spec;
    if ( when & FL_RETURN_END_CHANGED )
        when &= ~ ( FL_RETURN_NONE | FL_RETURN_CHANGED );

    obj->how_return = when;

    fl_set_object_return( sp->input, FL_RETURN_END_CHANGED );
    fl_set_object_return( sp->button1,     FL_RETURN_ALWAYS );
    fl_set_object_return( sp->button2,   FL_RETURN_ALWAYS );
    fl_set_object_return( sp->button3,     FL_RETURN_ALWAYS );
    fl_set_object_return( sp->button4,   FL_RETURN_ALWAYS );
}



/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
