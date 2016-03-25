

# Wireless Networking RFID Assignment ET4394
Name : 
1. Pranav Sailesh Mani (4508467)
2. Rizky Dharmawan (4501225)


## Description 

<p align="center">
<img src="https://cloud.githubusercontent.com/assets/17114181/14053359/fe9468d6-f2d0-11e5-8b24-3182c353abcb.jpg">
</p>

To decode the information from the RFID , we separate the data into four parts (figure above), we use Matlab to decode the RFID data. There are 4 parts of the graph that have to be decoded. This is shown in the figure above. 

##Details of the code
Reader_to_Tag.m is one file that can be run once and can give two bitstreams as output. This output contains the bitstreams of the two signals from the reader to the Tag. The variables 'bitstream' and 'bitstream1' contain the value of the two bitstreams. 'Tag_To_Reader_1.m' contains the code for the first tag signal. The bitstream is stored in the variable 'streamm'.'Tag_To_Reader_2.m' contains the code for the second tag signal. The bitstream is stored in the variable 'streamm'. This file is same as 'Tag_To_Reader_1' except that the starting point for the second Tag signal has been changed. The 'signal' file is the same file that was given in class to test our program.

## The Result 
1. READER  to TAG (ACK = 01)	:  0	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1


2. TAG to READER	 : 1	0	1	0	v	1	0	0	0	0	0	0	0	0	0	0	1	0	1	0	1	0	0	0	0	0	1	1	0	0	0	1	0	1	0	1	1	0	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1	1	0	0	1	1	0	0	1

3.  READER to TAG (Req_RN= 1 1 0 0 0 0 0 1) : 1	1	0	0	0	0	0	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	0	0	1	1	1	1	1	1	1	0	1	0	1	0	1	1

	
4. TAG to READER	 :1	0	1	1	v	1	1	1	1	1	1	1	1	1	1	1	1	1	1
