Compression through sorting

This was designed for .bmp files but can work on others (but not those that have incorporated some compression technique).

First we take the header and read in which bytes have information. 
We assume a max header size of 64 bytes so we create a 64 bit array (zeroed) where 1s indicate a non zero value.
We write this array out as 8 (8 bit) numbers.
We then write out the bytes in order from this reference array.
Header done.

We now read in the bitmap data 3 bytes at a time and in lots of 8.
We sort the numbers and record the original order.
E.G. Numbers: 1, 5, 7, 2, 5, 3, 1, 6 are read
Sorted they are 1, 1, 2, 3, 5  5, 6, 7
The order they are sorted is: 0, 6, 3, 5, 1, 4, 7, 2
This sort order is 1 of 40,320 permutations of 8 factorial.
We'll get back to this later but note that 40,320 is a 16 bit number.

With our sorted numbers we can eliminate duplicates.
We are left with: 1, 2, 3, 5, 6, 7
This is our NOD (number of data points).
We have a NOD of 6
Counts are associated with the NOD and our counts here are: 1, 0, 0, 1, 0, 0
(this is the way they are stored - add 1 for the actual counts).
Counts for each NOD have a specific bit size and are stored in variables defined at the beginning of the program. 
For NOD 6 we have 21 different possible count definitions.
For NOD 6 the count reference adress size is 5 bits.
Since our numbers are in order we can shrink them.
We do this by finding the difference between each 2 consecutive numbers starting at 0.
In our case we get: 1, 1, 1, 2, 1, 1
Before we needed 3 bits to define our numbers, now we need only 2.
This size is our mxnumb 


