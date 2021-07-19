import argparse
import math
import korflib

def entropy(a, c, g, t):
	total = a + c + g + t
	if total == 0: return 0
	h = 0
	if a > 0: h -= a/total * math.log2(a/total)
	if c > 0: h -= c/total * math.log2(c/total)
	if g > 0: h -= g/total * math.log2(g/total)
	if t > 0: h -= t/total * math.log2(t/total)
	return h

if __name__ == '__main__':

	## Command Line Interface ##

	parser = argparse.ArgumentParser(description='dust.py')
	parser.add_argument('fasta', type=str, metavar='<file>', help='fasta file')                
	parser.add_argument('--window', required=False, type=int, default=11,
		metavar='<str>', help='window size, must be odd [%(default)i]')
	parser.add_argument('--entropy', required=False, type=float, default=1.1,
		metavar='<int>', help='entropy threshold [%(default)f]')
	parser.add_argument('--lcmask', action='store_true',
		help='use lowercase instead of N')
	arg = parser.parse_args()


	assert(arg.window % 2 == 1)
	w1 = arg.window // 2
	w2 = arg.window

	## Main loop ##

	for id, seq in korflib.read_fasta(arg.fasta):
		print(f'>{id}')
	
		# first half window is unmasked
		masked = []
		masked.append(seq[0:w1])
	
		# count first window
		iw = seq[0:w2]
		a = iw.count('A')
		c = iw.count('C')
		g = iw.count('G')
		t = iw.count('T')
		if entropy(a, c, g, t) < arg.entropy:
			if arg.lcmask: masked.append(seq[w1].lower())
			else:          masked.append('N')
		else: masked.append(seq[w1])
	
		# count all other windows
		for i in range(0, len(seq) - w2):
			off = seq[i]
			on = seq[i+w2]
			nt = seq[i+w1+1]
		
			if   off == 'A': a -= 1
			elif off == 'C': c -= 1
			elif off == 'G': g -= 1
			elif off == 'T': t -= 1
		
			if   on == 'A': a += 1
			elif on == 'C': c += 1
			elif on == 'G': g += 1
			elif on == 'T': t += 1
		
			if entropy(a, c, g, t) < arg.entropy:
				if arg.lcmask: nt = nt.lower()
				else:          nt = 'N'
			masked.append(nt)

		# final half window is unmasked
		masked.append(seq[-1:])
	
		# output
		masked = ''.join(masked)
		for i in range(0, len(masked), 50):
			print(masked[i:i+50])



