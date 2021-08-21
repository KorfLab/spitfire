#!/usr/bin/env python3

import argparse
import os
import random
import statistics
import time
from korflib import randomseq

def kmers(k, seq):
	count = {}
	for i in range(len(seq) -k + 1):
		kmer = seq[i:i+k]
		if kmer not in count: count[kmer] = 1
		else: count[kmer] += 1
	return count

def compare(seq1, seq2):
	x = 0
	for i in range(len(seq1)):
		for j in range(len(seq2)):
				if seq1[i] == seq2[j]: x += 1				
	return x

if __name__ == '__main__':
	
	parser = argparse.ArgumentParser(description='pybench.py')
	parser.add_argument('--faster', required=False, type=float, default=1.0,
		metavar='<float>', help='go faster [%(default)f]')
	arg = parser.parse_args()

	speed = []
	
	## SEQ test
	
	t0 = time.time()
	seqs = []
	for i in range(100):
		print(f'SEQ: {i}%', end='\r')
		seqs = []
		for j in range((int(1310 / arg.faster))):
			seq = randomseq(100)
			seqs.append(seq)
	t1 = time.time()
	v = 100 / (t1 - t0) / arg.faster
	speed.append(v)
	print(f'\rSEQ: {v}')
	
	## I/O test

	t0 = time.time()
	for i in range(100):
		print(f'I/O: {i}%', end='\r')
		for j in range(int(75 / arg.faster)):
			seq = randomseq(100)
			with open('tmp.pybench', 'w') as fp:
				fp.write(seq)
			os.system('cat tmp.pybench > /dev/null');
	t1 = time.time()
	v = 100 / (t1 - t0) / arg.faster
	print(f'\rI/O: {v}')
	speed.append(v)
	
	## MER test

	t0 = time.time()
	seq = randomseq(1000)
	for i in range(100):
		print(f'MER: {i}%', end='\r')
		for j in range(int(480 / arg.faster)):
			ks = kmers(15, seq)
	t1 = time.time()
	v = 100 / (t1 - t0) / arg.faster
	print(f'\rMER: {v}')
	speed.append(v)
	
	## CMP test

	t0 = time.time()
	for i in range(100):
		print(f'CMP {i}%', end='\r')
		s1 = randomseq(100)
		s2 = randomseq(100)
		for j in range(int(120 / arg.faster)):
			a = compare(s1, s2)
	t1 = time.time()
	v = 100 / (t1 - t0) / arg.faster
	print(f'\rCMP: {v}')
	speed.append(v)	

	## Summary ## should average them right?
	for v in speed:
		print(f'| {v:.1f}', end='')	
	print(f' | {statistics.mean(speed):.1f}')

