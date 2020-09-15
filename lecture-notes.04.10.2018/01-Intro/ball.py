#! /usr/bin/env python

from Tkinter import *
import sys, os

class Ball:
    def __init__(self, letter):
        self.root = Tk()
        self.canvas = Canvas(self.root, width=100, height=100)
        self.canvas.pack()

        self.root.bind('q', self.Exit)

        self.x, self.y = 50, 50
        self.vector    = 1
        self.sz        = 8

        self.ballID = self.canvas.create_oval(self.x-self.sz, self.y-self.sz,
                                              self.x+self.sz, self.y+self.sz,
                                              fill='orange',
                                              outline='orange')
        self.textID = self.canvas.create_text(self.x, self.y,
                                              anchor='c', text=letter)
    def Go(self):
        self.Animate()
        self.root.mainloop()
    def Exit(self, event):
        sys.exit(0)
    def Animate(self):
        self.x += self.vector
        if self.x > (100-self.sz) or self.x < self.sz:
            self.vector = -self.vector
        self.canvas.coords(self.ballID,
                           self.x-self.sz, self.y-self.sz,
                           self.x+self.sz, self.y+self.sz)
        self.canvas.coords(self.textID, self.x, self.y)
        self.root.after(20, self.Animate)

# main
b = Ball(sys.argv[1])
b.Go()
