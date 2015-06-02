bin_PROGRAMS = dvbotc
dvbotc_SOURCES = channel.c convolution.c dump.c dvbotc.c dvbotc_opts.c \
	inner_interleaver.c insert_reference_signals.c mapper.c ofdm_encode.c outer_interleave.c \
	parameter.c puncturing.c rs_wrapper.c scrambler.c scrambler_prbs.c

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
dvbotc_LDFLAGS = $(all_libraries) -g -D_FILE_OFFSET_BITS=64 \
	-D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -lfftw3 -lm librs.a
noinst_HEADERS = channel.h convolution.h ds_ii_hq.h dump.h dvbotc.h \
	dvbotc_opts.h inner_interleaver.h insert_reference_signals.h mainpage.h mapper.h \
	ofdm_encode.h outer_interleave.h parameter.h puncturing.h qam_ref.h ref_sig.h \
	rs_wrapper.h scrambler.h
AM_CFLAGS = -g
