multilibtool_DATA += \
		  %D%/sim.specs \
		  %D%/crt0.o

multilibtool_LIBRARIES += %D%/libgloss.a
%C%_libgloss_a_CPPFLAGS = -I$(srcdir)/%D%
%C%_libgloss_a_SOURCES = \
		       %D%/sys_exit.c \
		       %D%/sys_sbrk.c \
		       %D%/sys_close.c \
		       %D%/sys_lseek.c \
		       %D%/sys_read.c \
		       %D%/sys_write.c

includemachinetooldir = $(tooldir)/include/machine
includemachinetool_DATA = %D%/machine/syscall.h
