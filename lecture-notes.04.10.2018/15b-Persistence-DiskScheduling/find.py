#! /usr/bin/env python

import subprocess
import itertools
import random
import sys

def do_sim(block_list, sched_policy):
    # assumes a proper python list, turns it into something that sim can handle
    b = str(block_list).strip('[').strip(']').replace(' ','')
    # exec simulation with given block list
    exec_list = ['./disk.py', '-c', '-a', b, '-p', sched_policy, '-w', '10']
    result = subprocess.check_output(exec_list)
    # find "Total:" and then return the thing after it 
    tmp = result.split()
    next = False
    total_time = -1
    for word in tmp:
        if next:
            total_time = word
        if word == 'Total:':
            next = True
    return int(total_time)

max_block = 36

if len(sys.argv) != 4:
    print 'usage: find.py <request_list_length> <exit_when_found> <iterations:-1 for forever>'
    exit(1)

block_list_len = int(sys.argv[1])
exit_when_found = int(sys.argv[2])
iterations = int(sys.argv[3])

while iterations == -1 or iterations > 0:
    # make random list
    r_list = []
    while len(r_list) < block_list_len:
        block = int(random.random() * max_block)
        r_list.append(block)

    print 'TRYING', r_list

    # first do SATF
    satf_time = do_sim(r_list, 'SATF')

    # now do each possible one in FIFO order
    for schedule in itertools.permutations(r_list):
        fifo_time = do_sim(list(schedule), 'FIFO')
        # print '  ', list(schedule), fifo_time, satf_time
        print '  ', list(schedule), fifo_time, satf_time,
        if fifo_time < satf_time:
            print '  FOUND ONE!', satf_time - fifo_time
            if exit_when_found > 0:
                print ''
                exit(0)
        else:
            print ''

    # sadly, satf is best
    # try again?


