multilibtool_DATA += \
		  %D%/de2.specs \
		  %D%/max10.specs \
		  %D%/mister.specs \
		  %D%/crt0.o

multilibtool_LIBRARIES += %D%/libgloss.a
%C%_libgloss_a_CPPFLAGS = -I$(srcdir)/%D%
%C%_libgloss_a_SOURCES = \
		       %D%/fileio.c \
		       %D%/getpid.c \
		       %D%/isatty.c \
		       %D%/kill.c \
		       %D%/sbrk.c \
		       %D%/exit.c \
		       %D%/gettimeofday.c \
		       %D%/usleep.c \
		       %D%/syscalls.c \
		       %D%/rtc.c

multilibtool_LIBRARIES += %D%/libmister.a
%C%_libmister_a_CPPFLAGS = -I$(srcdir)/%D%
%C%_libmister_a_SOURCES = \
		       %D%/fileio.c \
		       %D%/getpid.c \
		       %D%/isatty.c \
		       %D%/kill.c \
		       %D%/sbrk.c \
		       %D%/exit.c \
		       %D%/gettimeofday.c \
		       %D%/usleep.c \
		       %D%/syscalls.c \
		       %D%/rtc.c \
		       %D%/misterstack.c

includemachinetooldir = $(tooldir)/include/machine
includemachinetool_DATA = %D%/machine/syscall.h
