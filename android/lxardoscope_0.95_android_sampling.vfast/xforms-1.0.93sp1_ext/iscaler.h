/*
 *
 * This file is part of the LXARDOSCOPE package.
 *
 * LXARDOSCOPE is an Arduino based oscilloscope for Linux, using the Xforms library.
 *
 * Copyright (C) 2011 Oskar Leuthold
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

// this file derived from xforms-1.0.93sp1 counter.h 

#ifndef FL_ISCALER_H
#define FL_ISCALER_H

typedef enum {
    FL_FLOAT_ISCALER,
    FL_INT_ISCALER
} FL_ISCALER_TYPE;

#include "pvaluator.h"
typedef FLI_VALUATOR_SPEC FLI_ISCALER_SPEC;

/***** Defaults *****/

#define FL_ISCALER_BOXTYPE  FL_UP_BOX
#define FL_ISCALER_COL1     FL_COL1
#define FL_ISCALER_COL2     FL_BLUE     /* ct label     */
#define FL_ISCALER_LCOL     FL_LCOL     /* ct reporting */
#define FL_ISCALER_ALIGN    FL_ALIGN_BOTTOM

/***** Others *****/

#define FL_ISCALER_BW       ( FL_BOUND_WIDTH - 1 )

/***** Routines *****/

FL_EXPORT FL_OBJECT * fl_create_iscaler( int          type,
                                         FL_Coord     x,
                                         FL_Coord     y,
                                         FL_Coord     w,
                                         FL_Coord     h,
                                         const char * label );

FL_EXPORT FL_OBJECT * fl_add_iscaler( int          type,
                                      FL_Coord     x,
                                      FL_Coord     y,
                                      FL_Coord     w,
                                      FL_Coord     h,
                                      const char * label );


FL_EXPORT void fl_set_iscaler_return( FL_OBJECT    * ob,
                                      unsigned int   how );

/* Functions to set and get the timeout value used by the
 * iscaler code to control modification of the iscaler value. */

FL_EXPORT int fl_get_iscaler_repeat( FL_OBJECT * ob );

FL_EXPORT void fl_set_iscaler_repeat( FL_OBJECT * ob,
                                      int         millisec );

FL_EXPORT int fl_get_iscaler_min_repeat( FL_OBJECT * ob );

FL_EXPORT void fl_set_iscaler_min_repeat( FL_OBJECT * ob,
                                          int         millisec );

FL_EXPORT int fl_get_iscaler_speedjump( FL_OBJECT * ob );

FL_EXPORT void fl_set_iscaler_speedjump( FL_OBJECT * ob,
                                         int         yes_no );

FL_EXPORT int fl_get_iscaler_button( FL_OBJECT *ob );

FL_EXPORT void fl_set_iscaler_input( FL_OBJECT *ob, const char *str );

FL_EXPORT const char * fl_get_iscaler_input( FL_OBJECT *ob);

FL_EXPORT void fl_set_iscaler_maxchars( FL_OBJECT * ob, int chars );


#endif /* ! defined FL_ISCALER_H */
