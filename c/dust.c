

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "korflib.h"

static char *usage = "\
usage: dust <fasta file>\n\
  -w <int>   window size [11]\n\
  -t <float> threshold [1.1]\n\
  -n         mask with Ns (lowercase default)\n\
  -h         this message";


int main(int argc, char *argv[]) {
	int      opt;
	int      window    = 11;
	double   threshold = 1.1;
	int      lowercase = 1;
	char    *filename;
	kl_seqio io;
	kl_seq   seq;
	
	// named parameters
	while ((opt = getopt(argc, argv, "w:t:nh")) != -1) {
		switch (opt) {
			case 'w':
				window = atoi(optarg);
				break;
			case 't':
				threshold = atof(optarg);
				break;
			case 'n':
				lowercase = 0;
				break;
			case 'h':
				fprintf(stderr, "%s\n", usage);
				exit(1);
		}
	}
	printf("window %d, threshold %f, lowercase %s\n", window, threshold,
		lowercase ? "yes" : "no");
	
	// positional arguments
	filename = argv[optind];
	
	// reader object
	io = kl_new_seqio(filename);
	
	// main loop
	while ((seq = kl_seqio_next(io)) != NULL) {
		printf("%s %s\n", seq->id, seq->seq);
	}
	
	kl_seqio_find(io, "foo");
	
	
}