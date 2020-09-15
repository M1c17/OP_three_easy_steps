# Homework (Simulation)
This homework uses disk.py to familiarize you with how a modern
hard drive works. It has a lot of different options, and unlike most of
the other simulations, has a graphical animator to show you exactly what
happens when the disk is in action. See the README for details.

## Questions:

1. Compute the seek, rotation, and transfer times for the following sets of requests: -a 0, -a 6, -a 30, -a 7,30,8, and finally -a 10,11,12,13.
```
./disk.py -a 0 -c
REQUESTS ['0']

Block:   0  Seek:  0  Rotate:165  Transfer: 30  Total: 195

TOTALS      Seek:  0  Rotate:165  Transfer: 30  Total: 195
```
```
./disk.py -a 6 -c
REQUESTS ['6']

Block:   6  Seek:  0  Rotate:345  Transfer: 30  Total: 375

TOTALS      Seek:  0  Rotate:345  Transfer: 30  Total: 375
```
```
./disk.py -a 30 -c
REQUESTS ['30']

Block:  30  Seek: 80  Rotate:265  Transfer: 30  Total: 375

TOTALS      Seek: 80  Rotate:265  Transfer: 30  Total: 375
```
```
./disk.py -a 7,30,8 -c
REQUESTS ['7', '30', '8']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  30  Seek: 80  Rotate:220  Transfer: 30  Total: 330
Block:   8  Seek: 80  Rotate:310  Transfer: 30  Total: 420

TOTALS      Seek:160  Rotate:545  Transfer: 90  Total: 795
```
```
./disk.py -a 10,11,12,13 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:425  Transfer:120  Total: 585
```
2. Do the same requests above, but change the seek rate to different values: -S 2, -S 4, -S 8, -S 10, -S 40, -S 0.1. How do the times change?

```
./disk.py -a 0 -S 2 -c
REQUESTS ['0']

Block:   0  Seek:  0  Rotate:165  Transfer: 30  Total: 195

TOTALS      Seek:  0  Rotate:165  Transfer: 30  Total: 195


```
```
./disk.py -a 6 -S 4 -c
Block:   6  Seek:  0  Rotate:345  Transfer: 30  Total: 375

TOTALS      Seek:  0  Rotate:345  Transfer: 30  Total: 375

```
```
./disk.py -a 6 -S 0.1 -c
REQUESTS ['6']

Block:   6  Seek:  0  Rotate:345  Transfer: 30  Total: 375

TOTALS      Seek:  0  Rotate:345  Transfer: 30  Total: 375

```
```
./disk.py -a 30 -S 8 -c
Block:  30  Seek: 10  Rotate:335  Transfer: 30  Total: 375

TOTALS      Seek: 10  Rotate:335  Transfer: 30  Total: 375
```
```
./disk.py -a 7,30,8 -S 10 -c
REQUESTS ['7', '30', '8']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  30  Seek:  8  Rotate:292  Transfer: 30  Total: 330
Block:   8  Seek:  8  Rotate: 22  Transfer: 30  Total:  60

TOTALS      Seek: 16  Rotate:329  Transfer: 90  Total: 435

```
```
./disk.py -a 10,11,12,13  -S 40 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek:  1  Rotate:359  Transfer: 30  Total: 390
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek:  1  Rotate:464  Transfer:120  Total: 585

```

In this case we can observe the Seek time is shorter and the default value is 1.

3. Do the same requests above, but change the rotation rate: -R 0.1, -R 0.5,-R 0.01. How do the times change?
```
./disk.py -a 0 -R 0.1 -c
REQUESTS ['0']

Block:   0  Seek:  0  Rotate:1650  Transfer:300  Total:1950

TOTALS      Seek:  0  Rotate:1650  Transfer:300  Total:1950
```
```
./disk.py -a 6 -R 0.5 -c
REQUESTS ['6']

Block:   6  Seek:  0  Rotate:690  Transfer: 60  Total: 750

TOTALS      Seek:  0  Rotate:690  Transfer: 60  Total: 750
```
```
./disk.py -a 30 -R 0.01 -c
REQUESTS ['30']

Block:  30  Seek: 80  Rotate:34420  Transfer:3001  Total:37501

TOTALS      Seek: 80  Rotate:34420  Transfer:3001  Total:37501
```
```
./disk.py -a 7,30,8 -R 0.1 -c
Block:   7  Seek:  0  Rotate:150  Transfer:299  Total: 449
Block:  30  Seek: 80  Rotate:2920  Transfer:301  Total:3301
Block:   8  Seek: 80  Rotate:219  Transfer:300  Total: 599

TOTALS      Seek:160  Rotate:3289  Transfer:900  Total:4349
```
```
./disk.py -a 10,11,12,13 -R 0.5 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:210  Transfer: 60  Total: 270
Block:  11  Seek:  0  Rotate:  0  Transfer: 60  Total:  60
Block:  12  Seek: 40  Rotate:680  Transfer: 60  Total: 780
Block:  13  Seek:  0  Rotate:  0  Transfer: 60  Total:  60

TOTALS      Seek: 40  Rotate:890  Transfer:240  Total:1170
```
The bigger -R the faster is rotation gonna take and viceversa.
The default -> rotateSpeed 1.
In this case Rotation time and transfer are more longer because -R < 1.

4. FIFO is not always best, e.g., with the request stream -a 7,30,8, what order should the requests be processed in? Run the shortest seek-time first (SSTF) scheduler (-p SSTF) on this workload; how long should it take (seek, rotation, transfer) for each request to be served?
```
./disk.py -a 7,30,8 -p SSTF -c
REQUESTS ['7', '30', '8']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  30  Seek: 80  Rotate:190  Transfer: 30  Total: 300

TOTALS      Seek: 80  Rotate:205  Transfer: 90  Total: 375
```
FIFO Order -> 7, 30, 8
SSTF Order -> 7, 8, 30

5. Now use the shortest access-time first (SATF) scheduler (-p SATF). Does it make any difference for -a 7,30,8 workload? Find a set of requests where SATF outperforms SSTF; more generally, when is SATF better than SSTF?
```
./disk.py -a 7,30,8 -p SATF -c
REQUESTS ['7', '30', '8']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  30  Seek: 80  Rotate:190  Transfer: 30  Total: 300

TOTALS      Seek: 80  Rotate:205  Transfer: 90  Total: 375
```
```
./disk.py -a 7,22,5 -p SATF -S 40 -c
REQUESTS ['7', '22', '5']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  22  Seek:  1  Rotate: 59  Transfer: 30  Total:  90
Block:   5  Seek:  1  Rotate:179  Transfer: 30  Total: 210

TOTALS      Seek:  2  Rotate:253  Transfer: 90  Total: 345
```
```
./disk.py -a 7,22,5 -p SSTF -S 40 -c
REQUESTS ['7', '22', '5']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   5  Seek:  0  Rotate:270  Transfer: 30  Total: 300
Block:  22  Seek:  1  Rotate:119  Transfer: 30  Total: 150

TOTALS      Seek:  1  Rotate:404  Transfer: 90  Total: 495
```
SATF is better when the rotation cost is significantly higher than the Seek time


6. Here is a request stream to try: -a 10,11,12,13. What goes poorly when it runs? Try adding track skew to address this problem (-o skew). Given the default seek rate, what should the skew be to maximize performance? What about for different seek rates (e.g., -S 2, -S 4)? In general, could you write a formula to figure out the skew?
```
./disk.py -a 10,11,12,13 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:425  Transfer:120  Total: 585
```
```
./disk.py -a 10,11,12,13 -o 2 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:125  Transfer:120  Total: 285
```
```
./disk.py -a 10,11,12,13 -o 2 -S 2 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 20  Rotate: 40  Transfer: 30  Total:  90
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 20  Rotate:145  Transfer:120  Total: 285
```
```
./disk.py -a 10,11,12,13 -o 2 -S 40 -c
REQUESTS ['10', '11', '12', '13']

Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek:  1  Rotate: 59  Transfer: 30  Total:  90
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek:  1  Rotate:164  Transfer:120  Total: 285
```
skew = track-distance(40) / seek-speed / (rotational-space-degrees(360 / 12) * rotation-speed) = 40 / 1 / (30 * 1) ≈ 2

-S2 => 40 / 2 / 30 ≈ 1
-S4 => 40 / 4 / 30 ≈ 1

7. Specify a disk with different density per zone, e.g., -z 10,20,30, which specifies the angular difference between blocks on the outer, middle, and inner tracks. Run some random requests (e.g., -a -1 -A 5,-1,0, which specifies that random requests should be used via the -a -1 flag and that five requests ranging from 0 to the max be generated), and compute the seek, rotation, and transfer times. Use different random seeds. What is the bandwidth (in sectors per unit time) on the outer, middle, and inner tracks?
```
 ./disk.py -z 10,20,30 -a -1 -A 5,-1,0 -c
 REQUESTS [45, 40, 22, 13, 27]

 Block:  45  Seek: 40  Rotate:310  Transfer: 20  Total: 370
 Block:  40  Seek:  0  Rotate:240  Transfer: 20  Total: 260
 Block:  22  Seek: 40  Rotate: 85  Transfer: 10  Total: 135
 Block:  13  Seek:  0  Rotate:260  Transfer: 10  Total: 270
 Block:  27  Seek:  0  Rotate:130  Transfer: 10  Total: 140

 TOTALS      Seek: 80  Rotate:1025  Transfer: 70  Total:1175
```
Outer  = 3 / (140 + 270 + 135) = 0.0055
Middle = 2 / (260 + 370) = 0.0032
```
 ./disk.py -z 10,20,30 -a -1 -A 5,-1,0 -s 1 -c
 REQUESTS [7, 45, 41, 13, 26]

 Block:   7  Seek:  0  Rotate:245  Transfer: 10  Total: 255
 Block:  45  Seek: 40  Rotate: 55  Transfer: 20  Total: 115
 Block:  41  Seek:  0  Rotate:260  Transfer: 20  Total: 280
 Block:  13  Seek: 40  Rotate:335  Transfer: 10  Total: 385
 Block:  26  Seek:  0  Rotate:120  Transfer: 10  Total: 130

 TOTALS      Seek: 80  Rotate:1015  Transfer: 70  Total:1165
```
Outer  = 3 / (255 + 385 + 130) = 0.0038
Middle = 2 / (115 + 280) = 0.0051
```
 ./disk.py -z 10,20,30 -a -1 -A 5,-1,0 -s 2 -c
 REQUESTS [51, 51, 3, 4, 45]

 Block:  51  Seek: 40  Rotate: 70  Transfer: 20  Total: 130
 Block:  51  Seek:  0  Rotate:340  Transfer: 20  Total: 360
 Block:   3  Seek: 40  Rotate: 35  Transfer: 10  Total:  85
 Block:   4  Seek:  0  Rotate:  0  Transfer: 10  Total:  10
 Block:  45  Seek: 40  Rotate: 85  Transfer: 20  Total: 145

 TOTALS      Seek:120  Rotate:530  Transfer: 80  Total: 730
```
Outer = 2 / (85 + 10) = 0.0210
Middle = 3 / (130 + 360 + 145) = 0.0047
```
 ./disk.py -z 10,20,30 -a -1 -A 5,-1,0 -s 3 -c
 REQUESTS [12, 29, 19, 32, 33]

 Block:  12  Seek:  0  Rotate:295  Transfer: 10  Total: 305
 Block:  29  Seek:  0  Rotate:160  Transfer: 10  Total: 170
 Block:  19  Seek:  0  Rotate:250  Transfer: 10  Total: 260
 Block:  32  Seek:  0  Rotate:120  Transfer: 10  Total: 130
 Block:  33  Seek:  0  Rotate:  0  Transfer: 10  Total:  10

 TOTALS      Seek:  0  Rotate:825  Transfer: 50  Total: 875
```
Outer = 5 / (305 + 170 + 260) = 0.0057

8. A scheduling window determines how many requests the disk can examine at once. Generate random workloads (e.g., -A 1000,-1,0, with different seeds) and see how long the SATF scheduler takes when the scheduling window is changed from 1 up to the number of requests. How big of a window is needed to maximize performance? Hint: use the -c flag and don’t turn on graphics (-G) to run these quickly. When the scheduling window is set to 1, does it matter which policy you are using?
```
./disk.py -A 1000,-1,0 -p SATF -w 1 -c
TOTALS      Seek:20960  Rotate:169165  Transfer:30000  Total:220125

./disk.py -A 1000,-1,0 -p FIFO -w 1 -c
TOTALS      Seek:20960  Rotate:169165  Transfer:30000  Total:220125

./disk.py -A 1000,-1,0 -p SSTF -w 1 -c
TOTALS      Seek:20960  Rotate:169165  Transfer:30000  Total:220125

./disk.py -A 1000,-1,0 -p BSATF -w 1 -c
TOTALS      Seek:20960  Rotate:169165  Transfer:30000  Total:220125

./disk.py -A 1000,-1,0 -p SATF -w 1 -c
TOTALS      Seek:20960  Rotate:169165  Transfer:30000  Total:220125

./disk.py -A 1000,-1,0 -p SATF -w 1000 -c
TOTALS      Seek:1520  Rotate:3955  Transfer:30000  Total:35475
```
To maximize performance needs size of the disk (-w 1 ).
setting window = 1 is FIFO

9. Create a series of requests to starve a particular request, assuming an SATF policy. Given that sequence, how does it perform if you use a bounded SATF (BSATF) scheduling approach? In this approach, you specify the scheduling window (e.g., -w 4); the scheduler only moves onto the next window of requests when all requests in the current window have been serviced. Does this solve starvation? How does it perform, as compared to SATF? In general, how should a disk make this trade-off between performance and starvation avoidance?
```
./disk.py -a 12,7,8,9,10,11 -p SATF -c
REQUESTS ['12', '7', '8', '9', '10', '11']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   9  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  10  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390

TOTALS      Seek: 40  Rotate:335  Transfer:180  Total: 555
```
```
./disk.py -a 12,7,8,9,10,11 -p BSATF -w 4 -c
REQUESTS ['12', '7', '8', '9', '10', '11']

Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   9  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
Block:  10  Seek: 40  Rotate:230  Transfer: 30  Total: 300
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 80  Rotate:265  Transfer:180  Total: 525
```

10. All the scheduling policies we have looked at thus far are greedy; they pick the next best option instead of looking for an optimal schedule. Can you find a set of requests in which greedy is not optimal?
```
./disk.py -a 9,20 -c
REQUESTS ['9', '20']

Block:   9  Seek:  0  Rotate: 75  Transfer: 30  Total: 105
Block:  20  Seek: 40  Rotate:260  Transfer: 30  Total: 330

TOTALS      Seek: 40  Rotate:335  Transfer: 60  Total: 435
```
```
./disk.py -a 9,20 -c -p SATF
REQUESTS ['9', '20']

Block:  20  Seek: 40  Rotate:  5  Transfer: 30  Total:  75
Block:   9  Seek: 40  Rotate:320  Transfer: 30  Total: 390

TOTALS      Seek: 80  Rotate:325  Transfer: 60  Total: 465
```
```
./disk.py -a 4,11 -c -p SATF
REQUESTS ['4', '11']

Block:  11  Seek:  0  Rotate:135  Transfer: 30  Total: 165
Block:   4  Seek:  0  Rotate:120  Transfer: 30  Total: 150

TOTALS      Seek:  0  Rotate:255  Transfer: 60  Total: 315
```
