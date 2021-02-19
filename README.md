spitfire
========

## Genome Center cluster ##

The Genome Center cluster is composed of _head_ nodes that submit jobs,
_cluster_ nodes that perform computations, and _file servers_ that store
all of the data.

![Cluster Topology](cluster.png)

While spitfire can submit jobs to the cluster nodes, most of the time we
use it as a loosely managed compute node. What exactly does _loosely_
mean? We don't need to submit jobs via `slurm`. Instead, we run jobs
directly via the shell. There is 1 main advantage: jobs start
immediately. There is also 1 main problem: other people are also using
the computer.

## ssh ##

To log in to spitfire, you use `ssh`. In the following examples, the
user is `username`. Switch this to whatever username you have.

	ssh username@spitfire.genomecenter.ucdavis.edu

To copy files to spitfire, you use `scp`. Note the colon at the end.
This is critical!

	scp my_file username@spitfire.genomecenter.ucdavis.edu:/share/korflab/project/whatever

You can also copy whole directories with the `-r` option.

	scp -r my_dir username@spitfire.genomecenter.ucdavis.edu:/share/korflab/project/whatever

Of course, you can also `scp` stuff from spitfire back to your personal
computer.

	scp -r username@spitfire.genomecenter.ucdavis.edu:/share/korflab/project/whatever .

## Best practices for sharing spitfire ##

RAM is the hardest resource to share. A good rule of thumb is never use
more than half of the total RAM. Since there is 256G RAM, never run jobs
that take more than 128G. If you need to use more than 128G, discuss
with Ian first so we can warn other users.

CPU is easily shared but you should still be cognizant of how much you
are using. There are 64 CPUs and you shouldn't use more than half (32)
at a time. However, if you have some kind of rush job, you can use all
of them if you nice your jobs to reduce their priority.

## /share/korflab ##

Looking at the cluster topology, it should be clear that spitfire
doesn't store any of your files directly. It has access to
`/share/korflab` and other mount points via the local network.

Most Korflab members and spitfire users will be using the
`/share/korflab` mount point to store code, data, and experiments. This
is your main hub on the file system, not your home directory.

+ /share/korflab/data - Shared data files like genomes
+ /share/korflab/home - Your personal home away from home (see below)
+ /share/korflab/project - Group projects

For reasons that aren't obvious, your home directory at `/home/username`
is sometimes unstable. Most days it works great, but every once in a
while the fileserver has issues. If you're running a long compute that
requires access to your home directory, the job may fail. For this
reason **never use your home directory**. Instead, put _stuff_ in your
`/share/korflab/home/username` directory.

## Setting up your home ##

The first thing you need to do is to create a home directory.

	cd /share/korflab/home
	mkdir username

Next, create a few directories in your new home away from home.

	cd username
	mkdir bin lib

Your `/share/korflab/home/username` directory is the place to put all of
your GitHub repositories. Let's add one now.

	git clone https://github.com/KorfLab/spitfire.git

Let's enter that directory and check it out.

	cd spitfire
	ls

You will see this `README.md`,  an executable python progam
`pybench.py`, a python library `korflib.py`, and a bit more. Try running
the program.

	./pybench.py

If this doesn't work, don't go any further. Something is broken and
needs to be fixed. If everything is fine, our next step is to be able to
run `pybench.py` from anywhere on spitfire without using the explicit
path, just like _real_ programs like `ls`. In order to do that, you will
need to make 3 important changes.

1. Change your `PATH` environment variable
2. Change your `PYTHONPATH` environment variable
3. Make symbolic links in `/share/korflab/home/username/bin

For steps 1 and 2 above, you will need to edit your `.profile` or
`.bashrc` in your `/home/username` directory. 

	nano ~/.profile

Edit the file to contain the following lines, substituting `username`
for your actual user name (of course).

	export KORFHOME=/share/korflab/home/username
	export PATH=$PATH:$KORFHOME/bin
	export PYTHONPATH=$PYTHONPATH:$KORFHOME/lib
	alias ls="ls -F"
	alias rm="rm -i"
	alias cp="cp -i"
	alias mv="mv -i"

Now change directory to your bin and create an alias for the
`pybench.py` so that it looks like it's in your bin directory. Anything
you put in your bin directory will now be part of the system executable
path and can be used just like `ls`.

	cd /share/korflab/home/username/bin
	ln -s ../spitfire/pybench.py .

Let's also make a similar kind of change to your `PYTHONPATH` so that
python can find your libraries.

	cd /share/korflab/home/username/lib
	ln -s ../spitfire/korflib.py .

Now, no matter where your script resides, it will be able to do an
`import korflib`.

Every time you write a program that you want to be executable anywhere,
make a symbolic link in bin, just as you did above. Every time you write
a library you want your python code to import, make a symbolic link in
lib, just as you did above.
