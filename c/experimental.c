
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sequence {
	int	   len;
	char * def;
	char * seq;
};
typedef struct sequence * Sequence;

struct fasta_reader {
	char *path;     // path to file
	bool  gzipped;  // compressed?
	bool  seekable; // on filesystem?
	FILE *pipe;     // file pointer
	int  *index;    // future home of indexed reading
};
typedef struct fasta_reader *FastaReader;

FastaReader new_fasta_reader(const char *path) {
	FastaReader fs = malloc(sizeof(struct fasta_reader));
	
	// copy path for error reporting later
	fs->path = malloc(strlen(path) + 1);
	strcpy(fs->path, path);
	
	// check file extension for .gz
	const char* ext = strrchr(path, '.');
	if (ext && strcmp(ext, ".gz") == 0) fs->gzipped = true;
	else                                fs->gzipped = false;
	
	// open file pointer and set seekable
	if (fs->gzipped) {
		char command[256];
		sprintf(command, "gunzip -c %s", path);
		fs->pipe = popen(command, "r");
		fs->seekable = false;
	} else {
		if (strcmp(path, "-") == 0) {
			fs->pipe = stdin;
			fs->seekable = false;
		} else {
			fs->pipe = fopen(path, "r");
			fs->seekable = true;
		}
	}
	
	// indexed reading, only if seekable
	fs->index = NULL; // set up later, on first attempted read
	
	return fs;
}

Sequence next_record(FastaReader fr) {
	// reads the next record given the current file head
}

Sequence read_record(FastaReader fr, const char *id) {
	// attempts to find record id
	/*
		on first attempt:
			create indexes
			look for identifier errors
	*/
}


int main(int argc, char **argv) {
	FastaReader fr = new_fasta_reader(argv[1]);
	printf("%s\n", fr->path);
	if (fr->gzipped) printf("is compressed\n");
	if (fr->seekable) printf("is seekable\n");
}