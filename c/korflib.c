/*****************************************************************************\
korflib.c
Copyright (C) Ian Korf
\*****************************************************************************/

#ifndef KORFLIB_C
#define KORFLIB_C

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "korflib.h"

static int LineLength = 60;

static char *get_id(const char *def) {
	int end = 0;
	for (int i = 0; i < strlen(def); i++) {
		if (isspace((int)def[i])) {
			end = i;
			break;
		}
	}
	assert(end > 1); // empty identifier is terminal
	char *id = malloc(end);
	strncpy(id, def+1, end);
	return id;
}

void kl_set_linelength(int n) {
	assert(n > 0);
	LineLength = n;
}

kl_seqio kl_new_seqio(const char *path) {
	kl_seqio fs = malloc(sizeof(struct korflab_seqio));
	
	// copy path for use later?
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

kl_seq kl_seqio_next(kl_seqio io) {
	char c;
	char *def = NULL;
	size_t len;
	ssize_t bytes;

	// check for > at start of record	
	c = fgetc(io->pipe);
	if (c == EOF) return NULL;
	assert(c == '>');
	ungetc(c, io->pipe);
	
	// read definition line
	bytes = getline(&def, &len, io->pipe);
	assert(bytes != -1);
	
	// assign identifier
	char *id = get_id(def);

	
	// read sequence
	char *seq;
	size_t size = 1024;
	int i = 0;
	seq = malloc(size);
	while ((c = fgetc(io->pipe)) != EOF) {
		if (c == '>') {
			ungetc(c, io->pipe);
			break;
		}
		if (isspace((int)c)) continue;
		seq[i] = c;
		i++;
		if (i == size) {
			size *= 2;
			seq = realloc(seq, size);
		}
	}
	seq[i] = '\0';
	
	// create object
	kl_seq obj = malloc(sizeof(struct korflab_sequence));
	obj->len = strlen(seq);
	obj->id = id;
	obj->def = def;
	obj->seq = seq;

	return obj;
}

kl_seq kl_seqio_find(kl_seqio io, const char *id) {
	char c;
	char token[64];
	// can't do this if not seekable
	if (io->index == NULL) {
		rewind(io->pipe);
		// read lines, trap >, get id
	}

	kl_seq s = NULL;
	return s;
	
}

// delete seqio closes filehandle

#endif
