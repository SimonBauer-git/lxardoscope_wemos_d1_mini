/*
 *  This file is part of the XForms library package.
 *
 *  XForms is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1, or
 *  (at your option) any later version.
 *
 *  XForms is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with XForms. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *  \file flinternal.h
 *
 *  This file is part of the XForms library package.
 *  Copyright (c) 1996-1998  T.C. Zhao and Mark Overmars
 *  All rights reserved.
 *
 * Internal routines used by the Forms Library. An application should
 * not use any of this since there is no guarantee that exactly some
 * thing will exist in future versions of XForms.
 */

#ifndef FLINTERNAL_H
#define FLINTERNAL_H

#include <stdlib.h>
#include <signal.h>
#include "local.h"
#include "ulib.h"


/*  macros  and constants */

/* FL_DEBUG controls some conditional compilations. Even if the code
 * is compiled in, it still needs -debug level on the command line
 * to activate the message output. FL_DEBUG less than ML_ERR probably
 * is not a good idea. Only when making real production executables
 * should FL_DEBUG be less than ML_ERR (say ML_ERR-1)
 *
 *   ML_ERR    -- print some error messages
 *   ML_WARN    -- print some error messages
 *   ML_INFO1  -- some messages
 *   ML_INFO2  -- more messages
 *   ML_DEBUG  -- debugging code compiled in
 *   ML_TRACE  -- can be unbearable
 */

#ifndef FL_DEBUG
#define FL_DEBUG   ML_WARN
#endif


/* Mask for all possible events */

#define AllEventsMask   ( ( OwnerGrabButtonMask << 1 ) - 1 )


/* There are two ways to handle interaction. One is to process object
 * Q only if there are no more event pending. This can result in
 * an event entered into object Q more than once if the connection
 * is slow and clicking is rapid. If subsequently the object is
 * hidden or deleted/freed, problem occurs as the callback will
 * still be processed. To fix this, need to flush the object Q
 * when hiding/deleting objects. Also if an object is entered
 * multiple times, and its status changed each time, when the
 * Q is handled, we get wrong status.
 *
 * The other way to handle the interaction is to process object
 * Q as soon as the status of the object is changed. This is
 * the correct behavior. However, a change in status is typically
 * preceeded by a redraw (say mouse release on button), and due to
 * the X buffering mechnism, the redraw probably not get shown yet
 * when the callback is evoked. To fix this, an explicit flush
 * is needed. This can be time consuming. Also objects that do
 * not have callbacks may be handled out of order (after the one's
 * having callbacks). */

#if 1
#define DELAYED_ACTION
#endif


/* If we want to have gamma correction as a built-in feature.
 * Probably useless */

#define DO_GAMMA_CORRECTION


/* XForms internal colormap */

typedef struct {
    const char     * name;
    FL_COLOR         index;
    unsigned short   r;
    unsigned short   g;
    unsigned short   b;
    unsigned short   a;
    int              grayval;
} FLI_IMAP;

#define BadPixel  FL_NoColor

#define Clamp( v, vmin, vmax ) ( ( v ) < ( vmin ) ? \
                                 ( vmin ) : ( ( v ) > ( vmax ) ? \
                                              ( vmax ) :( v ) ) )

#define IsValidClass( o, c ) ( ( o ) && ( o )->objclass == ( c ) )

#define fl_safe_free( p )    \
    do { if ( p ) {          \
             fl_free( p );   \
             p = NULL;       \
          }                  \
       } while( 0 )

#define fli_class( i )     fl_state[ i ].vclass
#define fli_depth( i )     fl_state[ i ].depth
#define fli_visual( i )    fl_state[ i ].xvinfo->visual
#define fli_colormap( i )  fl_state[ i ].colormap
#define fli_map( i )       fl_state[ i ].colormap
#define fli_dithered( i )  fl_state[ i ].dithered

enum {
    FLI_FIND_INPUT,
    FLI_FIND_AUTOMATIC,
    FLI_FIND_MOUSE,
    FLI_FIND_CANVAS,
    FLI_FIND_KEYSPECIAL,
    FLI_FIND_RETURN
};

/* events.c or event related */

extern FL_OBJECT *fli_handled_obj;
extern FL_OBJECT *fli_handled_parent;

extern void fli_obj_queue_delete( void );

extern void fli_event_queue_delete( void );

extern void fli_object_qenter( FL_OBJECT * );

extern void fli_filter_returns( FL_OBJECT * );

extern FL_OBJECT * fli_object_qread( void );

extern void fli_object_qflush( FL_FORM * );

extern void fli_object_qflush_object( FL_OBJECT * );

extern FL_OBJECT *fli_object_qtest( void );

extern void fli_treat_user_events( void );

extern void fli_treat_interaction_events( int );

extern void fli_compress_event( XEvent *,
                                unsigned long );

extern const char *fli_event_name( int );

extern XEvent * fli_xevent_name( const char *,
                                 const XEvent * );

extern void fli_handle_idling( XEvent * xev,
                               long     msec,
                               int      do_idle_cb );

/* Variables defined in handling.c */

extern FL_FORM * mouseform;         /* the current form under mouse */
extern FL_FORM * keyform;           /* keyboard focus form */

extern FL_OBJECT * fli_pushobj;
extern FL_OBJECT * fli_mouseobj;

extern FL_Coord fli_mousex,
                fli_mousey;
extern unsigned int fli_keymask;

extern unsigned int fli_query_age;


/* Misc. utilitnes */

extern FL_FORM * fli_find_event_form( XEvent * );

extern void fli_print_version( int );

/* from forms.c and object.c */

extern void fli_scale_form( FL_FORM *,
                            double,
                            double );

extern void fli_handle_form( FL_FORM *,
                             int, int,
                             XEvent * );

extern FL_OBJECT *fli_end_group( void );

extern void fli_handle_object( FL_OBJECT *,
                               int,
                               FL_Coord,
                               FL_Coord,
                               int,
                               XEvent *,
                               int );

extern FL_OBJECT *fli_find_first( FL_FORM *,
                                  int,
                                  FL_Coord,
                                  FL_Coord );

extern void fli_redraw_form_using_xevent( FL_FORM *,
                                          int,
                                          XEvent * );

extern void fli_show_object( FL_OBJECT * );

extern void fli_recalc_intersections( FL_FORM * );

extern FL_OBJECT *fli_find_last( FL_FORM *,
                                 int,
                                 FL_Coord,
                                 FL_Coord );

extern FL_OBJECT *fli_find_object( FL_OBJECT *,
                                   int,
                                   FL_Coord,
                                   FL_Coord );

extern FL_OBJECT *fli_find_object_backwards( FL_OBJECT *,
                                             int,
                                             FL_Coord,
                                             FL_Coord );

extern void fli_insert_object( FL_OBJECT *,
                               FL_OBJECT * );


extern void fli_set_object_visibility( FL_OBJECT * obj,
                                       int         vis );

/* double buffering etc. */

extern void fli_free_flpixmap( FL_pixmap * );

extern void fli_create_object_pixmap( FL_OBJECT * );

extern void fli_show_object_pixmap( FL_OBJECT * );

extern void fli_create_form_pixmap( FL_FORM * );

extern void fli_show_form_pixmap( FL_FORM * );

/* windowing support */

extern void fli_default_xswa( void );

extern Window fli_cmap_winopen( Window,
                                Colormap,
                                const char * );

extern Window fli_create_window( Window,
                                 Colormap,
                                 const char * );

extern void fli_create_gc( Window );

enum {
    FLI_COMMAND_PROP = 1,
    FLI_PROP_SET     = ( 1 << 10 )  /* really set */
};

extern void fli_set_winproperty( Window,
                                 unsigned int );

/* graphics related */

extern void fli_init_colormap( int );

extern void fli_free_colormap( int );

extern void fli_dump_state_info( int,
                                 const char * );

extern void fli_init_stipples( void );

extern void fli_draw_button( FL_OBJECT * );

/* for fdesign */

const char *fli_query_colorname( FL_COLOR );

extern long fli_query_namedcolor( const char *s );

void fli_free_xtext_workmem( void );

extern int fli_get_pos_in_string( int,
                                  int,
                                  FL_Coord,
                                  FL_Coord,
                                  FL_Coord,
                                  FL_Coord,
                                  int,
                                  int,
                                  FL_Coord,
                                  FL_Coord,
                                  const char *,
                                  int *,
                                  int * );

extern int fli_drw_stringTAB( Window,
                              GC,
                              int,
                              int,
                              int,
                              int,
                              const char *,
                              int,
                              int );

extern int fli_drw_string( int,
                           int,
                           FL_Coord,
                           FL_Coord,
                           FL_Coord,
                           FL_Coord,
                           int,
                           FL_COLOR,
                           FL_COLOR,
                           FL_COLOR,
                           int,
                           int,
                           int,
                           int,
                           int,
                           const char *,
                           int,
                           int,
                           int,
                           FL_COLOR );

extern int fli_get_maxpixel_line( void );

extern int fli_get_string_widthTABfs( XFontStruct *,
                                      const char *,
                                      int );

extern void fli_init_font( void );

extern void fli_canonicalize_rect( FL_Coord *,
                                   FL_Coord *,
                                   FL_Coord *,
                                   FL_Coord * );

extern void fli_get_goodie_title( FL_FORM *,
                                  const char * );

extern void fli_add_q_icon( FL_Coord,
                            FL_Coord,
                            FL_Coord,
                            FL_Coord );

extern void fli_add_warn_icon( FL_Coord,
                               FL_Coord,
                               FL_Coord,
                               FL_Coord );

extern void fli_check_key_focus( const char *,
                                 Window );


extern void fli_free_cmdline_args( void );


extern XRectangle *fli_get_underline_rect( XFontStruct *,
                                           FL_Coord,
                                           FL_Coord,
                                           const char *,
                                           int );


/* Group some WM stuff into a structure for easy maintainance */

typedef struct {
    unsigned int pos_request;   /* USPOSITION or PPOSITION            */
} FLI_WM_STUFF;


/* Routines in sldraw.c. */

typedef struct {
    FL_Coord x;
    FL_Coord y;
    FL_Coord w;
    FL_Coord h;
} FLI_SCROLLBAR_KNOB;

enum {
    FLI_SLIDER_NONE = 0,
    FLI_SLIDER_BOX  = 1,
    FLI_SLIDER_KNOB = 2,
    FLI_SLIDER_ALL  = 3
};


extern void fli_calc_slider_size( FL_OBJECT *,
                                  FLI_SCROLLBAR_KNOB * );

extern void fli_drw_slider( FL_OBJECT *,
                            FL_COLOR,
                            FL_COLOR,
                            const char *,
                            int );

extern void fli_set_perm_clipping( FL_Coord,
                                   FL_Coord,
                                   FL_Coord,
                                   FL_Coord );

extern void fli_unset_perm_clipping( void );


extern int fli_perm_clip;

extern XRectangle fli_perm_xcr;


/* Application windows */

typedef struct fli_win_ {
    struct fli_win_ * next;
    Window            win;
    FL_APPEVENT_CB    pre_emptive;      /* always gets called first if set */
    FL_APPEVENT_CB    callback[ LASTEvent ];
    void            * pre_emptive_data;
    void            * user_data[ LASTEvent ];
    FL_APPEVENT_CB    default_callback;
    unsigned long     mask;
} FLI_WIN;

extern FLI_WIN * fl_app_win;

extern void fli_set_form_window( FL_FORM * );

extern void fli_unmap_canvas_window( FL_OBJECT * );

extern FL_APPEVENT_CB fli_set_preemptive_callback( Window,
                                                   FL_APPEVENT_CB,
                                                   void * );

extern unsigned long fli_xevent_to_mask( int );

extern int fli_initialize_program_visual( void );


#define FLI_TIMER_RES           50  /* resolution of FL_STEP event */


/* currently only one idle procedure is permitted, so the next
 * field is of no much use */

typedef struct fli_idle_cb_ {
    struct fli_idle_cb_ * next;
    FL_APPEVENT_CB        callback;
    void                * data;
} FLI_IDLE_REC;

typedef struct fli_io_event_ {
    struct fli_io_event_ * next;
    FL_IO_CALLBACK         callback;
    void                 * data;
    unsigned int           mask;
    int                    source;
} FLI_IO_REC;



/* signals */

#if ! defined HAVE_SIGACTION
typedef RETSIGTYPE ( * FLI_OSSIG_HANDLER )( int );
#endif

typedef struct fli_signallist_ {
    struct fli_signallist_ * next;
    FL_SIGNAL_HANDLER        callback;
#if defined HAVE_SIGACTION
    struct sigaction         old_sigact;
#else
    FLI_OSSIG_HANDLER        ocallback; /* default OS signal handler */
#endif
    void                   * data;
    int                      signum;
    int                      caught;
} FLI_SIGNAL_REC;

extern void fl_remove_all_signal_callbacks( void );

/* timeouts */

typedef struct fli_timeout_ {
    int                    id;
    struct fli_timeout_  * next;
    struct fli_timeout_  * prev;
    long                   start_sec,
                           start_usec;
    long                   ms_to_wait;
    FL_TIMEOUT_CALLBACK    callback;
    void                 * data;
} FLI_TIMEOUT_REC;

extern void fl_remove_all_timeouts( void );

/*
 *  Intenal controls.
 */

typedef struct fli_context_ {
    FL_FORM_ATCLOSE      atclose;           /* what to do if WM_DELETE_WINDOW */
    void               * close_data;
    FLI_IDLE_REC       * idle_rec;          /* idle callback record   */
    FLI_IO_REC         * io_rec;            /* async IO      record   */
    FLI_SIGNAL_REC     * signal_rec;        /* list of app signals    */
    FLI_TIMEOUT_REC    * timeout_rec;       /* timeout callbacks      */
    int                  idle_delta;        /* timer resolution       */
    long                 mouse_button;      /* push/release record    */
    int                  pup_id;            /* current active pup id  */
    FL_FORM            * modal;             /* current modal form     */
    long                 max_request_size;  /* max protocol size      */
    int                  num_io;
    int                  hscb,
                         vscb;              /* default scrollbar      */
    long                 ext_request_size;  /* extended request size  */
    int                  tooltip_time;
#ifdef XlibSpecificationRelease
    XIM                  xim;               /* input method           */
    XIC                  xic;               /* input context          */
#else
    void               * xim;
    void               * xic;
#endif
    unsigned int         navigate_mask;     /* input field            */
    long                 reserverd[ 6 ];
} FLI_CONTEXT;

/* some X info that helps to make the windowing system independent
 * API work (fl_color() etc. */

typedef struct {
    Display       * display;
    Window          win;
    GC              gc,
                    textgc;
    GC              miscgc;
    int             isRGBColor;
    int             isMBFont;       /* multi-byte font       */
    unsigned long   bktextcolor;
    int             newpix;
    int             fdesc;          /* font descent          */
    int             fasc;           /* font ascent           */
    int             fheight;        /* font height           */
    Colormap        colormap;
    XFontStruct   * fs;
    unsigned long   color;          /* last color. cache     */
    unsigned long   textcolor;      /* last textcolor. cache */
    unsigned long   bkcolor;
    int             screen;
} FLI_TARGET;


typedef struct {
    FL_FORM      ** forms;             /* all forms, visible and hidden */
    int             formnumb;          /* number of visible forms */
    int             hidden_formnumb;   /* number of hidden forms */
    size_t          auto_count;
    int             unmanaged_count;

    FL_Coord        mousex,            /* last recorded mouse position */
                    mousey;
    unsigned int    keymask;           /* state of buttons and modifier keys */
    unsigned int    query_age;         /* age of recorded information */

    FL_FORM       * mouseform;         /* the current form under the mouse */
    FL_FORM       * keyform;           /* keyboard focus form */

    FL_OBJECT * pushobj;               /* latest pushed object */
    FL_OBJECT * mouseobj;              /* object under the mouse */
} FLI_INTERNAL;

extern FLI_INTERNAL fli_int;


extern void fli_init_context( void );


#include "extern.h"


extern void fli_watch_io( FLI_IO_REC *,
                          long );

extern int fli_do_shortcut( FL_FORM *,
                            int,
                            FL_Coord,
                            FL_Coord,
                            XEvent * );

extern void fli_get_hv_align( int,
                              int *,
                              int *);

extern void fli_get_outside_align( int,
                                   int,
                                   int,
                                   int,
                                   int,
                                   int *,
                                   int *,
                                   int * );

extern void fli_init_symbols( void );

extern void fli_release_symbols( void );

extern int fli_handle_event_callbacks( XEvent * );



/* Some macros to test how an object can be moved or resized, depending on
   its gravity settings (ULC = uppler left hand cornner, LRC = lower right
   hand corner) */

#define ULC_POS_LEFT_FIXED( obj )                \
    (    ( obj )->nwgravity == FL_NorthWest      \
      || ( obj )->nwgravity == FL_West           \
      || ( obj )->nwgravity == FL_SouthWest )

#define ULC_POS_RIGHT_FIXED( obj )               \
    (    ( obj )->nwgravity == FL_NorthEast      \
      || ( obj )->nwgravity == FL_East           \
      || ( obj )->nwgravity == FL_SouthEast )

#define LRC_POS_LEFT_FIXED( obj )                \
    (    ( obj )->segravity == FL_NorthWest      \
      || ( obj )->segravity == FL_West           \
      || ( obj )->segravity == FL_SouthWest )

#define LRC_POS_RIGHT_FIXED( obj )               \
    (    ( obj )->segravity == FL_NorthEast      \
      || ( obj )->segravity == FL_East           \
      || ( obj )->segravity == FL_SouthEast )

#define HAS_FIXED_HORI_ULC_POS( obj )                             \
    ( ULC_POS_LEFT_FIXED( obj ) || ULC_POS_RIGHT_FIXED( obj ) )

#define HAS_FIXED_HORI_LRC_POS( obj )                             \
    ( LRC_POS_LEFT_FIXED( obj ) || LRC_POS_RIGHT_FIXED( obj ) )

#define HAS_FIXED_WIDTH( obj )                                          \
    ( HAS_FIXED_HORI_ULC_POS( obj ) && HAS_FIXED_HORI_LRC_POS( obj ) ) 


#define ULC_POS_TOP_FIXED( obj )                 \
    (    ( obj )->nwgravity == FL_NorthWest      \
      || ( obj )->nwgravity == FL_North          \
      || ( obj )->nwgravity == FL_NorthEast )

#define ULC_POS_BOTTOM_FIXED( obj )              \
    (    ( obj )->nwgravity == FL_SouthWest      \
      || ( obj )->nwgravity == FL_South          \
      || ( obj )->nwgravity == FL_SouthEast )

#define LRC_POS_TOP_FIXED( obj )                 \
    (    ( obj )->segravity == FL_NorthWest      \
      || ( obj )->segravity == FL_North          \
      || ( obj )->segravity == FL_NorthEast )

#define LRC_POS_BOTTOM_FIXED( obj )              \
    (    ( obj )->segravity == FL_SouthWest      \
      || ( obj )->segravity == FL_South          \
      || ( obj )->segravity == FL_SouthEast )

#define HAS_FIXED_VERT_ULC_POS( obj )                             \
    ( ULC_POS_TOP_FIXED( obj ) || ULC_POS_BOTTOM_FIXED( obj ) )

#define HAS_FIXED_VERT_LRC_POS( obj )                             \
    ( LRC_POS_TOP_FIXED( obj ) || LRC_POS_BOTTOM_FIXED( obj ) )

#define HAS_FIXED_HEIGHT( obj )                                         \
    ( HAS_FIXED_VERT_ULC_POS( obj ) && HAS_FIXED_VERT_LRC_POS( obj ) ) 



#define XK_PageUp    XK_Prior
#define XK_PageDn    XK_Next

#define XK_PageUp    XK_Prior
#define XK_PageDn    XK_Next


/* some header has XK_XP_Left etc */

#if XlibSpecificationRelease >= 6

#define IsHome( k )     ( k == XK_Home  || k == XK_Begin || k == XK_KP_Home )
#define IsLeft( k )     ( k == XK_Left  || k == XK_KP_Left )
#define IsRight( k )    ( k == XK_Right || k == XK_KP_Right )
#define IsUp( k )       ( k == XK_Up    || k == XK_KP_Up )
#define IsDown( k )     ( k == XK_Down  || k == XK_KP_Down )
#define IsEnd( k )      ( k == XK_End   || k == XK_KP_End )
#define IsPageDown( k ) ( k == XK_Next  || k == XK_Page_Down || k == XK_KP_Page_Down )
#define IsPageUp( k )   ( k == XK_Prior || k == XK_Page_Up || k==XK_KP_Page_Up)

#else

#define IsHome( k )     ( k == XK_Home  || k == XK_Begin )
#define IsLeft( k )     ( k == XK_Left )
#define IsRight( k )    ( k == XK_Right )
#define IsDown( k )     ( k == XK_Down )
#define IsUp( k )       ( k == XK_Up )
#define IsEnd( k )      ( k == XK_End )
#define IsPageDown( k ) ( k == XK_Next )
#define IsPageUp( k )   ( k == XK_Prior )

#endif

#define FLI_HALFPAGE_UP        0x10000000
#define FLI_HALFPAGE_DOWN      0x20000000
#define FLI_NLINES_UP          0x30000000
#define FLI_NLINES_DOWN        0x40000000
#define FLI_1LINE_UP           0x50000000
#define FLI_1LINE_DOWN         0x60000000
#define IsHalfPageUp( k )      ( ( k ) == FLI_HALFPAGE_UP )
#define IsHalfPageDown( k )    ( ( k ) == FLI_HALFPAGE_DOWN )
#define IsNLinesUp( k )        ( ( k ) == FLI_NLINES_UP )
#define IsNLinesDown( k )      ( ( k ) == FLI_NLINES_DOWN )
#define Is1LineUp( k )         ( ( k ) == FLI_1LINE_UP )
#define Is1LineDown( k )       ( ( k ) == FLI_1LINE_DOWN )


void fli_hide_and_get_region( FL_OBJECT *,
                              Region    * );

extern int fli_convert_shortcut( const char *,
                                long * );

extern int fli_get_underline_pos( const char *,
                                  const char * );

extern void fli_scale_length( FL_Coord *,
                              FL_Coord *,
                              double );

extern int fli_get_visible_forms_index( FL_FORM * );

extern void fli_recount_auto_objects( void );

extern int fli_get_tabpixels( XFontStruct * );

extern int fli_get_default_scrollbarsize( FL_OBJECT * );

extern void fli_set_app_name( const char *,
                              const char * );

void fli_hide_composite( FL_OBJECT *,
                         Region    * );

extern void fli_show_composite( FL_OBJECT * );

extern void fli_deactivate_composite( FL_OBJECT * );

extern void fli_activate_composite( FL_OBJECT * );

extern void fli_delete_composite( FL_OBJECT * ob );

extern void fli_free_composite( FL_OBJECT * ob );

extern void fli_set_composite_gravity( FL_OBJECT *,
                                       unsigned int,
                                       unsigned int );

extern void fli_set_composite_resize( FL_OBJECT *,
                                     unsigned int );

extern void fli_composite_has_been_resized( FL_OBJECT * );

extern void fli_parse_goodies_label( FL_OBJECT *,
                                     const char * );

extern int fli_goodies_preemptive( FL_FORM *,
                                   void * );

extern void fli_get_goodies_font( int *,
                                  int * );

extern void fli_handle_goodie_font( FL_OBJECT *,
                                    FL_OBJECT * );

extern void fli_goodies_cleanup( void );

extern void fli_msg_cleanup( void );

extern void fli_alert_cleanup( void );

extern void fli_choice_cleanup( void );

extern void fli_question_cleanup( void );

extern void fli_input_cleanup( void );

extern void fli_sinput_cleanup( void );

extern void fli_handle_timeouts( long * );

#define FL_IS_NONSQRBOX( t ) (    t == FL_SHADOW_BOX          \
                               || t == FL_NO_BOX              \
                               || t == FL_RFLAT_BOX           \
                               || t == FL_ROUNDED_BOX         \
                               || t == FL_OVAL_BOX            \
                               || t == FL_ROUNDED3D_UPBOX     \
                               || t == FL_ROUNDED3D_DOWNBOX )

enum {
    FLI_TRIANGLE_UPBOX1,
    FLI_TRIANGLE_UPBOX2,
    FLI_TRIANGLE_UPBOX3,
    FLI_TRIANGLE_UPBOX4,
    FLI_TRIANGLE_UPBOX6,
    FLI_TRIANGLE_UPBOX7,
    FLI_TRIANGLE_UPBOX8,
    FLI_TRIANGLE_UPBOX9,
    FLI_TRIANGLE_DOWNBOX1,
    FLI_TRIANGLE_DOWNBOX2,
    FLI_TRIANGLE_DOWNBOX3,
    FLI_TRIANGLE_DOWNBOX4,
    FLI_TRIANGLE_DOWNBOX6,
    FLI_TRIANGLE_DOWNBOX7,
    FLI_TRIANGLE_DOWNBOX8,
    FLI_TRIANGLE_DOWNBOX9
};

extern void fli_set_additional_clipping( FL_Coord,
                                         FL_Coord,
                                        FL_Coord,
                                        FL_Coord );

extern FL_RECT *fli_union_rect( const FL_RECT *,
                                const FL_RECT * );

extern void fli_xyplot_nice_label( float,
                                   int,
                                   float,
                                   char * );

extern void fli_xyplot_compute_data_bounds( FL_OBJECT *,
                                            int *,
                                            int *,
                                            int );

extern int fli_xyplot_interpolate( FL_OBJECT *,
                                   int,
                                   int,
                                   int );

extern void fli_insert_composite_after( FL_OBJECT *,
                                        FL_OBJECT * );

extern void fli_add_composite( FL_OBJECT * );

extern void fli_insert_composite( FL_OBJECT *,
                                  FL_OBJECT * );

extern int fli_is_watched_io( int );

extern const char *fli_object_class_name( FL_OBJECT * );

extern char *fli_print_to_string( const char * fmt,
                                  ... );

extern char *fli_read_line( FILE * fp );

extern char *fli_sstrcpy( char       * dest,
                          const char * src,
                          size_t       n );

extern void fli_set_form_icon_data( FL_FORM *,
                                    char ** );

extern char *fli_getcwd( char *,
                         int );

extern void fli_get_clipping( FL_Coord *,
                              FL_Coord *,
                              FL_Coord *,
                              FL_Coord * );

extern void fli_replacepup_text( int,
                                 int,
                                 const char * );

extern int fli_handle_mouse_wheel( int *,
                                   int *,
                                   void * );

extern int fli_valuator_handle_drag( FL_OBJECT *,
                                     double );

extern int fli_valuator_handle_release( FL_OBJECT *,
                                        double );

extern void *fli_init_valuator( FL_OBJECT * );

extern double fli_valuator_round_and_clamp( FL_OBJECT *,
                                            double );

extern double fli_clamp( double,
                         double,
                         double );

extern void fli_inherit_attributes( FL_OBJECT *,
                                    FL_OBJECT * );

extern int fli_boxtype2frametype( int );

extern void fli_xvisual2flstate( FL_State *,
                                 XVisualInfo * );

extern int fli_find_closest_color( int,
                                   int,
                                   int,
                                   XColor *,
                                   int,
                                   unsigned long * );

extern void fli_rgbmask_to_shifts( unsigned long,
                                   unsigned int *,
                                   unsigned int * );

extern void fli_show_tooltip( const char *,
                              int,
                              int );

extern void fli_hide_tooltip( void );

extern int fli_is_tooltip_form( FL_FORM * );

extern void fli_do_radio_push( FL_OBJECT *,
                               FL_Coord,
                               FL_Coord,
                               int,
                               void * );

extern long fli_getpid( void );

extern void fli_xlinestyle( Display *,
                            GC,
                            int );

#define FLI_BROKEN_BOX  ( 1 << 10 )

extern FLI_TARGET *fli_internal_init( void );

extern void fli_switch_target( FLI_TARGET * );

extern void fli_restore_target( void );

extern void fli_draw_text_inside( int align,
                                  FL_Coord,
                                  FL_Coord,
                                  FL_Coord,
                                  FL_Coord,
                                  const char *,
                                  int,
                                  int,
                                  FL_COLOR,
                                  FL_COLOR,
                                  int );

/* Misc. stuff */

extern void fli_add_vertex( FL_Coord x,
                           FL_Coord y );

extern void fli_add_float_vertex( float x,
                                  float y );

extern void fli_reset_vertex( void );

extern void fli_endpolygon( void );

extern void fli_endclosedline( void );

extern void fli_endline( void );

extern const char * fli_get_xevent_name( const XEvent * );

extern void fli_set_input_navigate( unsigned int mask );

extern void fli_adjust_browser_scrollbar( FL_OBJECT * );


extern FL_POPUP *fli_popup_add( Window,
                                const char *,
                                const char * );

extern FL_POPUP_ENTRY *fli_popup_add_entries( FL_POPUP *,
                                              const char *,
                                              va_list,
                                              const char *,
                                              int );

extern FL_POPUP_ENTRY *fli_popup_insert_entries( FL_POPUP *,
                                                 FL_POPUP_ENTRY *,
                                                 const char *,
                                                 va_list,
                                                 const char *,
                                                 int );

extern FL_POPUP_ENTRY *fli_popup_insert_items( FL_POPUP       *,
                                               FL_POPUP_ENTRY *,
                                               FL_POPUP_ITEM  *,
                                               const char * );

extern void fli_popup_init( void );

extern void fli_popup_finish( void );

extern int fli_check_popup_exists( FL_POPUP * );

extern int fli_check_popup_entry_exists( FL_POPUP_ENTRY * );

extern FL_POPUP_RETURN *fli_set_popup_return( FL_POPUP_ENTRY * );

extern void fli_popup_reset_counter( FL_POPUP * );

extern void fli_free_cursors( void );

extern void fli_free_fselectors( void );


/*  Some utility stuff */

typedef struct {
    int          val;
    const char * name;
} FLI_VN_PAIR;

extern int fli_get_vn_value( FLI_VN_PAIR * vn_pair,
                             const char  * name );

extern const char * fli_get_vn_name( FLI_VN_PAIR * vn_pair,
                                     int           val );

#if XlibSpecificationRelease == 6

#  define IsTab( ksym )  ( ksym == XK_ISO_Left_Tab || ksym== XK_Tab )

#else

#  define IsTab( ksym )  ( ksym == XK_Tab )

#endif

#endif /* ! defined FL_INTERNAL_H */


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
