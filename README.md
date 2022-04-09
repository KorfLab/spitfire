spitfire
========

This document is for users in the Korf Lab to set up their computer environments
on the Genome Center cluster.

## Accounts ##

You'll need an account to connect to spitfire and the rest of the cluster.

* https://computing.genomecenter.ucdavis.edu
* Follow the "request account" link
* Select Ian Korf from the list of sponsors
* You will get an email with a link where you can set your password
* For help, email hpc-help@ucdavis.edu

## Genome Center cluster ##

The Genome Center cluster is composed of _head_ nodes that submit jobs,
_cluster_ nodes that work on jobs, and _file servers_ that store all of the
data.

![Cluster Topology](cluster.png)

While spitfire can submit jobs to the cluster nodes, most of the time we use it
as a loosely managed compute node. What exactly does _loosely_ mean? We don't
need to submit jobs via `slurm`. Instead, we run jobs directly via the shell.
There is 1 main advantage: jobs start immediately. There is also 1 main problem:
other people are also using the computer. As a result, sharing must be
cooperative. There are no hard rules, but rather social guidelines.

## ssh and scp ##

To log in to spitfire, use `ssh`. In the following examples, the user's name is
`username`. Switch this to whatever user name you have.

	ssh username@spitfire.genomecenter.ucdavis.edu

To copy files to spitfire, use `scp`.

	scp my_file username@spitfire.genomecenter.ucdavis.edu:/share/korflab/project/whatever

You can also copy whole directories with the `-r` option.

	scp -r my_dir username@spitfire.genomecenter.ucdavis.edu:/share/korflab/project/whatever

Of course, you can also `scp` stuff from spitfire back to your personal
computer.

	scp -r username@spitfire.genomecenter.ucdavis.edu:/share/korflab/project/whatever .

## Best practices ##

### RAM ###

RAM is the hardest resource to share. A good rule of thumb is for each user to
never use more than half of the total RAM. Since there is currently 256G RAM,
never run jobs that take more than 128G in total. So don't set up 50 jobs, each
taking 10G RAM. If you need to use more than 128G, discuss with Ian first so we
can warn other users. If you have no idea how much RAM your process is using,
run `top` or `htop` and examine the memory usage.

### CPU ###

CPU is easily shared but you should still be cognizant of how much you are
using. There are 64 CPUs and you shouldn't use more than half (32) at a time.
However, if you have some kind of rush job, you can use most of them if you
`nice` your jobs to reduce their priority. In fact, if you want to be a good lab
citizen, you will `nice` all of your jobs. To `nice` your job, simply put
precede your command with the word `nice`.

### Storage ###

Looking at the cluster topology, it should be clear that spitfire doesn't store
any of your files directly. It has access to `/share/korflab` and other mount
points via a network.

Most Korflab members and spitfire users will be using the `/share/korflab` mount
point to store code, data, and experiments. This is your main hub on the file
system, not your home directory.

+ /share/korflab/data - Shared data files like genomes
+ /share/korflab/home - Your personal home away from home (see below)
+ /share/korflab/project - Group projects

To determine how much space you have available, use `df`.

	cd /share/korflab
	df -h .

This will report the size of the partition and how much is in use. If you want
to know exactly how much is in each project directory (for example), use `du`.

	du -h -d 1 /share/korflab/project

## $HOME away from $HOME ##

The authentication system may drop the connection to your home directory after
a long time. This means the programs that are running for hours will suddenly
lose their connection to your `$HOME` directory. This could very well break
whatever you're trying to do. Fortunately, `/share/korflab` does not have this
problems. So the workaround is to reset your `$HOME` to
`/share/korflab/home/$USER` and then place all of your configurations in there.
Hopefully the sysadmins fix this someday.

Are you using Conda? The answer should be _yes_. Is the Conda directory in your
home directory? The answer should be _no_. Reset your $HOME to
`/share/korflab/home/$USER` and do **all** of your work from there.

## Super-advanced shit for I/O intensive tasks ##

In the cluster topology diagram, you may have noticed that every computer is
attached to its own `/tmp` directory. If you have I/O intensive operations, it's
a good idea to use `/tmp` as a local cache. This gives you a much higher speed
to data and keeps traffic off the network. Less traffic means more speed. This
strategy also prevents you from network outages as `/tmp` is attached directly
to spitfire and not the network.

Provisioning `/tmp` on spitfire is not a big deal, just copy stuff there. But
provisioning lots of tmp directories on the cluster isn't trivial. You have to
know which cluster nodes your jobs are going to land on and set those up with
data ahead of time. If you have these kinds of needs, we need to discuss.
