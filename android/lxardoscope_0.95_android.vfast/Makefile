# *
# * This file is part of the LXARDOSCOPE package.
# *
# * LXARDOSCOPE is an Arduino based oscilloscope for Linux, using the Xforms library.
# *
# * Copyright (C) 2013 Oskar Leuthold
# * 
# * LXARDOSCOPE is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * LXARDOSCOPE is distributed in the hope that it will be useful, but
# * WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# * Lesser General Public License for more details.
# *
# * You should have received a copy of the GNU Lesser General Public
# * License along with LXARDOSCOPE; see the file COPYING.  If not, write to
# * the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
# * MA 02111-1307, USA.
# *
# */


INSTALL_LX_PATH=/usr/local

# Library paths
LIB_X11_PATH=/usr/X11R6/lib

# Compilation options
CC=gcc
CC_OPTIONS= -O2 -Wall -L$(LIB_X11_PATH) -D_REENTRANT

# Linking libraries
#LIBS=./xforms-1.0.93sp1_ext/libforms.a -lX11 -lm -lpthread
LIBS=-lforms -lX11 -lm -lpthread

#*************************************************************************
#**************** Nothing to change after this line **********************
#*************************************************************************


FILE_OUT=lxa
FILES_O=$(FILE_OUT).o $(FILE_OUT)_cb.o $(FILE_OUT)_gui.o $(FILE_OUT)_ard.o $(FILE_OUT)_plt.o $(FILE_OUT)_file.o iscaler.o ipushwheel.o
FILE_EXEC=lxardoscope

#*************************************************************************

all: $(FILES_O)
	$(CC) $(CC_OPTIONS) $(FILES_O) $(LIBS) -o $(FILE_EXEC)


#*************************************************************************

$(FILE_OUT).o: $(FILE_OUT).c
	$(CC) $(CC_OPTIONS) -c $(FILE_OUT).c -o $(FILE_OUT).o

$(FILE_OUT)_cb.o: $(FILE_OUT)_cb.c
	$(CC) $(CC_OPTIONS) -c $(FILE_OUT)_cb.c -o $(FILE_OUT)_cb.o

$(FILE_OUT)_gui.o: $(FILE_OUT)_gui.c
	$(CC) $(CC_OPTIONS) -c $(FILE_OUT)_gui.c -o $(FILE_OUT)_gui.o

$(FILE_OUT)_ard.o: $(FILE_OUT)_ard.c
	$(CC) $(CC_OPTIONS) -c $(FILE_OUT)_ard.c -o $(FILE_OUT)_ard.o

$(FILE_OUT)_plt.o: $(FILE_OUT)_plt.c
	$(CC) $(CC_OPTIONS) -c $(FILE_OUT)_plt.c -o $(FILE_OUT)_plt.o

$(FILE_OUT)_file.o: $(FILE_OUT)_file.c
	$(CC) $(CC_OPTIONS) -c $(FILE_OUT)_file.c -o $(FILE_OUT)_file.o

iscaler.o: iscaler.c
	$(CC) $(CC_OPTIONS) -c iscaler.c -o iscaler.o

ipushwheel.o: ipushwheel.c
	$(CC) $(CC_OPTIONS) -c ipushwheel.c -o ipushwheel.o

#*************************************************************************

install:
	echo "May be you must be root to install the software !"
	cp -f lxardoscope $(INSTALL_LX_PATH)/bin

uninstall:
	echo "May be you must be root to remove the software !"
	rm -f $(INSTALL_LX_PATH)/bin/lxardoscope 

clean:
	rm -f $(FILES_O)

mrproper: clean
	rm -f $(FILE_EXEC)


#*************************************************************************
#*************************************************************************
