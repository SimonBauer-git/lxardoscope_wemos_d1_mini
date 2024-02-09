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

#ifndef FL_IPUSHWHEEL_H
#define FL_IPUSHWHEEL_H

typedef enum {
    FL_FLOAT_IPUSHWHEEL,
    FL_INT_IPUSHWHEEL
} FL_IPUSHWHEEL_TYPE;

#include "pvaluator.h"
typedef FLI_VALUATOR_SPEC FLI_IPUSHWHEEL_SPEC;

/***** Defaults *****/

#define FL_IPUSHWHEEL_BOXTYPE  FL_UP_BOX
#define FL_IPUSHWHEEL_COL1     FL_COL1
#define FL_IPUSHWHEEL_COL2     FL_BLUE     /* ct label     */
#define FL_IPUSHWHEEL_LCOL     FL_LCOL     /* ct reporting */
#define FL_IPUSHWHEEL_ALIGN    FL_ALIGN_BOTTOM

/***** Others *****/

#define FL_IPUSHWHEEL_BW       ( FL_BOUND_WIDTH - 1 )

/***** Routines *****/

FL_EXPORT FL_OBJECT * fl_create_ipushwheel( int          type,
                                         FL_Coord     x,
                                         FL_Coord     y,
                                         FL_Coord     w,
                                         FL_Coord     h,
                                         const char * label );

FL_EXPORT FL_OBJECT * fl_add_ipushwheel( int          type,
                                      FL_Coord     x,
                                      FL_Coord     y,
                                      FL_Coord     w,
                                      FL_Coord     h,
                                      const char * label );


FL_EXPORT void fl_set_ipushwheel_return( FL_OBJECT    * ob,
                                      unsigned int   how );

/* Functions to set and get the timeout value used by the
 * ipushwheel code to control modification of the ipushwheel value. */

FL_EXPORT int fl_get_ipushwheel_repeat( FL_OBJECT * ob );

FL_EXPORT void fl_set_ipushwheel_repeat( FL_OBJECT * ob,
                                      int         millisec );

FL_EXPORT int fl_get_ipushwheel_min_repeat( FL_OBJECT * ob );

FL_EXPORT void fl_set_ipushwheel_min_repeat( FL_OBJECT * ob,
                                          int         millisec );

FL_EXPORT int fl_get_ipushwheel_speedjump( FL_OBJECT * ob );

FL_EXPORT void fl_set_ipushwheel_speedjump( FL_OBJECT * ob,
                                         int         yes_no );

FL_EXPORT int fl_get_ipushwheel_button( FL_OBJECT *ob );

FL_EXPORT void fl_set_ipushwheel_maxchars( FL_OBJECT * ob, int chars );

FL_EXPORT void fl_set_ipushwheel_nbchars( FL_OBJECT * ob, int chars );

FL_EXPORT void fl_set_ipushwheel_value( FL_OBJECT * ob,
                                     double      val );

FL_EXPORT double fl_get_ipushwheel_value( FL_OBJECT * ob);

FL_EXPORT void fl_set_ipushwheel_bounds( FL_OBJECT * ob,
                                      double      min,
                                      double      max );

FL_EXPORT void fl_set_ipushwheel_step( FL_OBJECT * ob,
                                    double      s );

FL_EXPORT void fl_set_ipushwheel_precision( FL_OBJECT * ob,
                                         int         prec );

FL_EXPORT void fl_set_ipushwheel_color(FL_OBJECT * ob,FL_COLOR col1,FL_COLOR col2);

#endif /* ! defined FL_IPUSHWHEEL_H */
