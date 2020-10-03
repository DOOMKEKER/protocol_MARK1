#include <iostream>
#include <fstream>
#include <cmath>

/*
16 bit of useful information
3 bit in one UART payload.

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
          |    |__________ xor payload with payload from other packer (but here will be xor with 000)
    _____/____//____/
     0 1 2 3 4 5 6 7
    |1|0|1|1|0|1|1|0|
    |1|0|1|1|1|0|0|1| 
    |1|0|1|0|1|0|1|0|
    |1|0|1|0|1|0|0|0|
    |1|0|1|0|1|0|0|0|
    |0|0|0|0|0|1|0|1| - last packet (coz 00)
    -----------------

*/

std::string BinStr = "";

int convertBinaryToDecimal(long long n) //Binary -> Decimal, 8 byte 
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0){
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

void outputHex(int num, std::ofstream &output) //HEX decoder, output to file
{
    if(num < 10){
             output<< num;
    }
    else{
         switch (num){
         case 10:
             output<< 'A';
             break;
         case 11:
             output<< 'B';
             break;
         case 12:
             output<< 'C';
             break;
         case 13:
             output<< 'D';
             break;
         case 14:
             output<< 'E';
             break;
         case 15:
             output<< 'F';
             break;
        
         default:
             break;
         }
    }
    
}

void print_bin_to_hex( std::ofstream &output ) //convert usefull binary info into hex by byte, output in file
{
    int count = BinStr.size();
    count = count - (count % 4);
    for (int i = 0; i < count; i+=4){
        outputHex(convertBinaryToDecimal(stoi(BinStr.substr(i,4))),output); /* substr(i,j)   - output substring, begining from i index and j chars long.
                                                                               stoi(str,int) - convert string to int.

                                                                            */
    }
}


bool pckt_check( std::string pckt_buf, std::string pckt )
{
            if ( (pckt[5] - '0') !=  (pckt_buf[2] - '0' ) ^ (pckt[2] - '0')   ) /*XOR check.
                                                                                  char to int by ASCII table. 
				                                                                  ASCII "0" = 48, "1" = 49. pckt[i] will be always 0 or 1. 
				                                                                  0(48 in ASCII) - 0(48 in ASCII) = 0; 1(49 ASCII) - 0(48 ASCII) = 1;
				                                                                  0=0 / 1=1*/
                return false;
            if ( (pckt[6] - '0') !=  (pckt_buf[3] - '0' ) ^ (pckt[3] - '0')   )
                return false;
            if ( (pckt[7] - '0') !=  (pckt_buf[4] - '0' ) ^ (pckt[4] - '0')   )
                return false;
    return true;
}


int read_packets(int num_packet, std::ifstream &input, std::string pckt_buf) //reading binary packets from input.txt 
{
    int count = num_packet; //read in desc (pckt[2]...pckt[7])
    std::string pckt;
    std::string payload;

    while (count){

        input >> pckt;//file->string
        
        if (input.eof()) //if file read error -> return -1
           return -1;

        if ( ( count != 1 ) && ( pckt[0] == '1' ) && ( pckt[1] == '0' )){ //if info in middle of payload 
            if(!pckt_check(pckt_buf,pckt)) //xor check
                return -2;
            BinStr = BinStr + pckt.substr(2,3); //usefull information -> BinStr
            pckt_buf = pckt;
            count--;
            continue;
        }

        if ( ( count == 1 ) && (pckt[0] == '0' ) && (pckt[1] == '0' ) ){ //if info in the end of payload
            if(!pckt_check(pckt_buf,pckt)) 
                return -2;
            BinStr = BinStr + pckt.substr(2,3); 
            return 0;
        }

        return -2;
    }

    return -1; //if there is no last packet
}

std::string fromHex2Bin(int num) //hex -> binary
{
    std::string msg = "";
    int k   = 1;
    int bin = 0;
    while (num)
    {
        bin += (num % 2) * k;
        k *= 10;
        num /= 2;
    }

    msg = std::to_string(bin); //int to string
    if (msg.size() < 4){      //Checking the count of bits at the begining. If count is less than 4 - adds zer0es at the begining.
        for(int i = 0; (4 - msg.size() ) > 0; i++){
            msg = "0" + msg;
        }
    }
    return msg;
}

void from_input_hex_2_input(std::string &msg_hex) //hex to binary tower
{
    std::ofstream input ("input.txt");
    for(int i = 0; i < msg_hex.size(); i+=2){
        int first_4  = msg_hex[i]   - '0'; // char to int by ASCII ( read about it from above (line 95) )
        int second_4 = msg_hex[i+1] - '0'; 
        std::string body = "";

        int ans1 = first_4  < 10 ? 1:0; //check variable if its number or letter.
        int ans2 = second_4 < 10 ? 1:0; 

        switch (ans1)
        {
        case 1:
            body = fromHex2Bin(first_4);
            input << body;
            break;
        case 0:
            first_4 -= 7 ;  
            body = fromHex2Bin(first_4);
            input << body;
            break;
        }

        switch (ans2)
        {
        case 1:
            body = fromHex2Bin(second_4);
            input << body << std::endl;
            break;
        case 0:
            second_4 -= 7 ;
            body = fromHex2Bin(second_4);
            input << body << std::endl;
            break;
        }
    }
}


int main()
{
    int num_packet = 0;
    int operation;
    std::string msg, msg_buf,msg_hex;
    std::ifstream input_hex  ("input_hex.txt"); //original hex number (OHN)
    std::ifstream input  ("input.txt");         // binary tower (converted OHN)
    std::ofstream output ("output.txt");        //output hex number

    input_hex >> msg_hex;
    from_input_hex_2_input(msg_hex);

    input >> msg;
    num_packet =  stoi(msg.substr(2,6)); //number of packets, that will be sent
    num_packet = convertBinaryToDecimal(num_packet); 

	int kek;
	if (msg[0] == '1' && msg[1] == '1') //if header
	{
		kek = read_packets(num_packet, input, msg);
		if (kek != 0)
		{
			std::cout << "CHECK YOUR input_hex, mate!" << std::endl;
			return -1;
		}
        else std::cout << "Alright, delo sdelano!" << std::endl;
	}
    
    print_bin_to_hex(output);

    return 0;

}