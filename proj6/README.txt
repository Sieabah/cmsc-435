Usage:
./seamcarving <input> <output> <new_width> <new_height>

You'll be given the following output on the console

RowsLeft ColumnsLeft %Complete

Notes:
I apologize for just making a huge main function, with time it'd be better off split into an image class that can do this all in it's own implementation but for the sake of time this should suffice.

I've provided numerous comments throughout my code, I believe the downfall of my implementation is the energy function I've chosen. Since I've separated it out of my code though so it's only calculated at read and seam deletion it can be modified in energyMap()

I also chose to not attempt to modify the image array size as that slowed the algorithm to an almost unsable state.

I made the EC website here: http://userpages.umbc.edu/~csidell1/cmsc435.html

Resources:
- Piazza
- Book
- Cplusplus.org
- Lots of notes on paper of data structures
- Found a proper dual gradient explanation: http://www.cs.princeton.edu/courses/archive/spr13/cos226/assignments/seamCarving.html, made my images look better after not using magnitude but squaring it.