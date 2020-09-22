# protocol_MARK1

        How it works
      ______first 2 bit show what type of packet is it ( 11 - header 
      | |                                                10 - packets in the middle of sending message 
      | |                                                00 - end of message)
      | |
      | | ___________________ The other 6 bit in header packet show how many packets will be send (There are will be sended 8 packets with 16 bit payload  110 111 101 101 101 000)
    _/__//___________/
    |1|1|0|0|0|1|0|0|

          _____________payload
          |    |
          |    |__________ xor with payload with payload from other packer (but here will be xor with 000)
    _____/____//____/
     0 1 2 3 4 5 6 7
    |1|0|1|1|0|1|1|0|
    |1|0|1|1|1|0|1|0|
    |1|0|1|0|1|0|0|0|
    |1|0|1|0|1|0|0|0|
    |1|0|1|0|1|0|0|0|
    |0|0|0|0|0|1|0|1| - last packet (coz 00)
    -----------------

