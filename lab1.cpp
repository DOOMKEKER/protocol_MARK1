
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

/*
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
	|1|0|1|1|1|0|0|1|
	|1|0|1|0|1|0|1|0|
	|1|0|1|0|1|0|0|0|
	|1|0|1|0|1|0|0|0|
	|0|0|0|0|0|1|0|1| - last packet (coz 00)
	-----------------

*/

std::string BinStr = "";



int convertBinaryToDecimal(long long n)
{
	int decimalNumber = 0, i = 0, remainder;
	while (n != 0) {
		remainder = n % 10;
		n /= 10;
		decimalNumber += remainder * pow(2, i);
		++i;
	}
	return decimalNumber;
}

void outputHex(int num, std::ofstream &output)
{
	if (num < 10) {
		output << num;
	}
	else {
		switch (num) {
		case 10:
			output << 'A';
			break;
		case 11:
			output << 'B';
			break;
		case 12:
			output << 'C';
			break;
		case 13:
			output << 'D';
			break;
		case 14:
			output << 'E';
			break;
		case 15:
			output << 'F';
			break;

		default:
			break;
		}
	}

}

void print_bin_to_hex(std::ofstream &output)
{
	int count = BinStr.size();
	count = count - (count % 4);
	//std::string sliceBinStr = BinStr.substr(0,count);
	for (int i = 0; i < count; i += 4) {
		outputHex(convertBinaryToDecimal(stoi(BinStr.substr(i, 4))), output); //uffffffff some cake
	}
}


bool pckt_check(std::string pckt_buf, std::string pckt)
{
	if ((pckt[5] - '0') != (pckt_buf[2] - '0') ^ (pckt[2] - '0'))
		return false;
	if ((pckt[6] - '0') != (pckt_buf[3] - '0') ^ (pckt[3] - '0'))
		return false;
	if ((pckt[7] - '0') != (pckt_buf[4] - '0') ^ (pckt[4] - '0'))
		return false;
	return true;
}


int read_packets(int num_packet, std::ifstream &input, std::string pckt_buf)
{
	int count = num_packet;
	std::string pckt;
	std::string payload;


	while (count) {

		input >> pckt;

		if (input.eof())
			return -1;

		if ((count != 1) && (pckt[0] == '1') && (pckt[1] == '0')) {
			if (!pckt_check(pckt_buf, pckt))
				return -2;
			BinStr = BinStr + pckt.substr(2, 3);
			pckt_buf = pckt;
			count--;
			continue;
		}

		if ((count == 1) && (pckt[0] == '0') && (pckt[1] == '0')) {
			if (!pckt_check(pckt_buf, pckt))
				return -2;
			BinStr = BinStr + pckt.substr(2, 3);
			return 0;
		}

		return -2;
	}

	return -1;

}


void HexToBinTower(string hexdec, std::ofstream &output) //hexdec - the original hex number from input.txt. Outputs to output.txt (1 byte per column).
{
	long int i = 0;
	string temp; //A temporary string to which the original hex is translated into binary in line.

	while (hexdec[i]) {

		switch (hexdec[i]) {
		case '0':
			temp += "0000";
			break;
		case '1':
			temp += "0001";
			break;
		case '2':
			temp += "0010";
			break;
		case '3':
			temp += "0011";
			break;
		case '4':
			temp += "0100";
			break;
		case '5':
			temp += "0101";
			break;
		case '6':
			temp += "0110";
			break;
		case '7':
			temp += "0111";
			break;
		case '8':
			temp += "1000";
			break;
		case '9':
			temp += "1001";
			break;
		case 'A':
		case 'a':
			temp += "1010";
			break;
		case 'B':
		case 'b':
			temp += "1011";
			break;
		case 'C':
		case 'c':
			temp += "1100";
			break;
		case 'D':
		case 'd':
			temp += "1101";
			break;
		case 'E':
		case 'e':
			temp += "1110";
			break;
		case 'F':
		case 'f':
			temp += "1111";
			break;
		default:
			cout << "\nInvalid hexadecimal digit " << hexdec[i];
		}
		i++;
	}
	//The block of output in output.txt (line-like binary number in a column of 1 byte).
	int noBlock = temp.length() / 8; //checking the number of bytes.
	int len = 0;
	for (int i = 0; i < noBlock; i++)
	{
		output << temp.substr(len, 8) << std::endl; //sequential output of substrings from the main string, which begin at every eighth (counting from zero) bit and are 8 bit long.
		len += 8;
	}

}

int main()
{
	/*
	Ñ6 -> 11000110
	B6 -> 10110110
	B9 -> 10111001
	AA -> 10101010
	A8 -> 10101000
	A8 -> 10101000
    05 -> 00000101

	Wrote my own main function. Old one down below.
	*/
	std::ifstream input("input.txt");
	std::ofstream output("output.txt");
	
	string number;
	input >> number; //hex number from input.txt to string for further work.

	HexToBinTower(number, output); 

	system("pause");
	return 0;
}


/*
int main()
{

	int num_packet = 0;
	int operation;
	std::string msg, msg_buf;
	std::ifstream input("input.txt");
	std::ofstream output("output.txt");

	input >> msg;
	num_packet = stoi(msg.substr(2, 6));
	num_packet = convertBinaryToDecimal(num_packet);

	if (msg[0] == '1' && msg[1] == '1')
		read_packets(num_packet, input, msg);

	print_bin_to_hex(output);

	return 0;

}
*/