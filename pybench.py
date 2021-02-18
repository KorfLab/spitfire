#!/usr/bin/env python3

import random
import time
from korflib import randomseq

def kmers(k, seq):
	count = {}
	for i in range(len(seq) -k + 1):
		kmer = seq[i:i+k]
		if kmer not in count: count[kmer] = 1
		else: count[kmer] += 1
	return count

def countNT(seq):
	a = seq.count('A')
	c = seq.count('C')
	g = seq.count('G')
	t = seq.count('T')
	return a, c, g, t

def burn(seq1, seq2):
	x = 0
	for i in range(len(seq1)):
		for j in range(len(seq2)):
				if seq1[i] == seq2[j]: x += 1				
	return x

## Test 1 ##

t0 = time.time()
for i in range(100):
	print(f'{i}%', end='\r')
	seq = randomseq(1000)
	for j in range(20000):
		countNT(seq)
print()
t1 = time.time()
v = int(1000 / (t1 - t0))
print(f'Test #1: Speed: {v}')

## Test 2 ##

t0 = time.time()
for i in range(100):
	print(f'{i}%', end='\r')
	seq = randomseq(100000)
	ks = kmers(9, seq)
print()
t1 = time.time()
v = int(1000 / (t1 - t0))
print(f'Test #2: Speed: {v}')

## Test 3 ##

t0 = time.time()
for i in range(100):
	print(f'{i}%', end='\r')
	s1 = randomseq(1000)
	s2 = randomseq(1000)
	a = burn(s1, s2)
print()
t1 = time.time()
v = int(1000 / (t1 - t0))
print(f'Test #3: Speed: {v}')