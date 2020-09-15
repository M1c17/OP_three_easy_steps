#! /usr/bin/env python

from Tkinter import *
from types import *
import math, random, time, sys, os
from optparse import OptionParser

class Coords:
    def __init__(self, block_width, block_height, y_max, x_offset, y_offset):
        self.block_width = block_width
        self.block_height = block_height
        self.y_max = y_max
        self.x_offset = x_offset
        self.y_offset = y_offset
        return

    def TranslateX(self, value):
        return self.x_offset + (value * self.block_width)
        
    def TranslateY(self, value):
        return self.y_max - (self.y_offset + (value * self.block_height))

class Block:
    def __init__(self, x, y, text):
        self.x = x
        self.y = y
        self.text = text.split(',')
        self.block_id = -1
        self.move_ongoing = False
        self.font = ('Helvetica', 10)
        return

    def Draw(self, canvas, coords):
        self.tx = coords.TranslateX(self.x)
        self.ty = coords.TranslateY(self.y)
        self.block_id = canvas.create_rectangle(self.tx, self.ty, self.tx+80, self.ty-80, fill='orange', outline='black')
        self.text_ids = []
        cnt = 0
        for line in self.text:
            self.text_ids.append(canvas.create_text(self.tx+40,self.ty-68+(cnt*8),text=line,font=self.font,anchor='c'))
            cnt += 1
        return

    def MovePlanned(self):
        return self.move_ongoing

    def PlanMove(self, coords, final_x, final_y, time_steps):
        self.move_ongoing = True
        fx, fy = coords.TranslateX(final_x), coords.TranslateX(final_y)
        dx, dy = fx - self.tx, fy - self.ty
        # dist = math.hypot(dx - self.tx, dy - self.ty)
        self.move_dx = dx / float(time_steps)
        self.move_dy = dy / float(time_steps)
        return

    def Move(self, canvas):
        self.tx += self.move_dx
        self.ty += self.move_dy
        canvas.coords(self.block_id, self.tx, self.ty, self.tx+80, self.ty-80)
        for cnt in range(len(self.text)):
            canvas.coords(self.text_ids[cnt], self.tx+40, self.ty-68+(cnt*8))
        return
        
        
        
        
class Animate:
    def __init__(self, width, height, delay):
        self.width, self.height = width, height
        self.delay = delay

        # BASIC WINDOW and CANVAS
        self.root = Tk()
        self.canvas = Canvas(self.root, width=self.width, height=self.height)
        self.canvas.pack()

        # KEY BINDINGS
        self.root.bind('s', self.Start)
        self.root.bind('p', self.Pause)
        self.root.bind('q', self.Exit)

        self.coords = Coords(80, 80, int(height), 20, 20)

        # keep track of all blocks, index by name
        self.blocks = {}

        # current things to animate
        self.current_blocks = []

        # animation list
        self.animation_list = {}
        self.animation_timer = 0

        self.do_animate = False
        return

    def ReadAnimationFile(self, afile):
        fd = open(afile)
        for line in fd:
            # print line,
            tmp = line.split()
            if len(tmp) == 0:
                continue
            cmd = tmp[0]
            if cmd == 'Block':
                # Block name time x y text
                assert(len(tmp) == 6)
                name, time, x, y, text = tmp[1], int(tmp[2]), int(tmp[3]), int(tmp[4]), tmp[5]
                self.blocks[name] = Block(x, y, text)
                self.blocks[name].Draw(self.canvas, self.coords)
            elif cmd == 'CopyMove':
                # CopyMove name time final_x final_y how_long
                old_name, new_name, time, final_x, final_y, how_long = tmp[1], tmp[2], int(tmp[3]), int(tmp[4]), int(tmp[5]), int(tmp[6])
                self.blocks[new_name] = Block(self.blocks[old_name])
                # print old_name, new_name, time, final_x, final_y, how_long
                # self.InsertAtTime(time, {'cmd':'CopyMove', 'name':name, 'final_x':final_x, 'final_y':final_y, 'end_time':time + how_long})
            elif cmd == 'Move':
                # CopyMove name time final_x final_y how_long
                name, time, final_x, final_y, final_time = tmp[1], int(tmp[2]), int(tmp[3]), int(tmp[4]), int(tmp[5])
                # print old_name, new_name, time, final_x, final_y, how_long
                self.InsertAtTime(time, {'cmd':'Move', 'name':name, 'final_x':final_x, 'final_y':final_y, 'final_time':final_time})
        fd.close()
        return

    def InsertAtTime(self, time, entry):
        if time not in self.animation_list:
            self.animation_list[time] = [entry]
        else:
            self.animation_list[time].append(entry)
        return

    # take a step from list and do it
    def Execute(self, cmd_list):
        for entry in cmd_list:
            print 'Entry', entry
            command = entry['cmd']
            
            if command == 'CopyMove':
                print 'CopyMove'
                b = self.blocks[entry['name']]
                if b.CopyExists() == False:
                    b.CopyCreate(self.canvas)
                    b.CopyPlanMove(entry['final_x'], entry['final_y'], entry['end_time'] - self.animation_timer) 
                b.CopyMove(self.canvas)
                if self.animation_timer < entry['end_time']:
                    self.InsertAtTime(self.animation_timer + 1, entry)
                else:
                    # animation over: delete copy?
                    print 'done'
            elif command == 'Move':
                print 'Move'
                b = self.blocks[entry['name']]
                if b.MovePlanned() == False:
                    b.PlanMove(self.coords, entry['final_x'], entry['final_y'], entry['final_time'])
                b.Move(self.canvas)
                if self.animation_timer < entry['final_time']:
                    self.InsertAtTime(self.animation_timer + 1, entry)
                else:
                    # animation over: delete copy?
                    print 'done'
        return

    # call this to start simulation
    def Go(self):
        self.root.mainloop()
        return

    def Animate(self):
        # print 'animate', self.animation_timer
        t = self.animation_timer
        if t in self.animation_list:
            self.Execute(self.animation_list[t])
        self.animation_timer += 1
        self.root.after(self.delay, self.Animate)

    #
    # BUTTONS
    #
    def Start(self, event):
        self.Animate()
        return

    def Pause(self, event):
        if self.do_animate == False:
            self.do_animate = True
        else:
            self.do_animate = False
        return

    def Exit(self, event):
        sys.exit(0)
        return

        
#
# MAIN SIMULATOR
#
parser = OptionParser()
parser.add_option('-s', '--seed',            default='0',         help='Random seed',                                             action='store', type='int',    dest='seed')
parser.add_option('-a', '--addr',            default='',          help='Request list (comma-separated) [-1 -> use addrDesc]',     action='store', type='string', dest='addr')
parser.add_option('-r', '--read_fraction',   default='0.5',       help='Fraction of requests that are reads',                     action='store', type='string', dest='read_fraction')
parser.add_option('-A', '--addr_desc',       default='5,-1,0',    help='Num requests, max request (-1->all), min request',        action='store', type='string', dest='addr_desc')
parser.add_option('-B', '--balanced',        default=True,        help='If generating random requests, balance across disks',     action='store_true',           dest='balance')
parser.add_option('-S', '--seek_speed',      default='4',         help='Speed of seek (1,2,4,5,10,20)',                           action='store', type='int',    dest='seek_speed')
parser.add_option('-p', '--policy',          default='FIFO',      help='Scheduling policy (FIFO, SSTF, SATF, BSATF)',             action='store', type='string', dest='policy')
parser.add_option('-i', '--num_inodes',      default=16,          help='Number of inodes',                                        action='store', type='int',    dest='num_inodes')
parser.add_option('-d', '--num_data',        default=32,          help='Number of data blocks',                                   action='store', type='int',    dest='num_data')
parser.add_option('-D', '--animate_delay',   default=10,          help='Animation delay',                                         action='store', type='int',    dest='animate_delay')
parser.add_option('-G', '--graphics',        default=True,        help='Turn on graphics',                                        action='store_true',           dest='graphics')
parser.add_option('-c', '--compute',         default=False,       help='Compute the answers',                                     action='store_true',           dest='compute')
parser.add_option('-P', '--print_options',   default=False,       help='Print the options',                                       action='store_true',           dest='print_options')
(options, args) = parser.parse_args()

if options.print_options:
    print 'OPTIONS seed', options.seed
    print 'OPTIONS addr', options.addr
    print 'OPTIONS addr_desc', options.addr_desc
    print 'OPTIONS seek_speed', options.seek_speed
    print 'OPTIONS window', options.window
    print 'OPTIONS policy', options.policy
    print 'OPTIONS compute', options.compute
    print 'OPTIONS read_fraction', options.read_fraction
    print 'OPTIONS graphics', options.graphics
    print 'OPTIONS animate_delay', options.animate_delay
    print ''

random.seed(options.seed)

a = Animate(1000, 500, options.animate_delay)

a.ReadAnimationFile('mkfs.anim')

a.Go()
