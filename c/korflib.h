/*****************************************************************************\
korflib.h
Copyright (C) Ian Korf
\*****************************************************************************/

#ifndef KORFLIB_H
#define KORFLIB_H

void kl_set_linelength (int);

struct korflab_sequence {
	int	   len; // length of sequence
	char * id;  // identifier (first token after >)
	char * def; // entire definition line including id
	char * seq; // sequence
};
typedef struct korflab_sequence * kl_seq;

struct korflab_seqio {
	char *path;     // path to file
	bool  gzipped;  // compressed?
	bool  seekable; // on filesystem?
	FILE *pipe;     // file pointer
	int  *index;    // future home of indexed reading
};
typedef struct korflab_seqio * kl_seqio;

kl_seqio kl_new_seqio(const char *);
kl_seq   kl_seqio_next(kl_seqio);
kl_seq   kl_seqio_find(kl_seqio, const char *);

#endif
