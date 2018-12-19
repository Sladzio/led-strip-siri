#include <cstdlib>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(25,0);

struct actionStruct{
  unsigned long  mode;
  unsigned long action;
}actionResponse;
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
int main(int argc, char** argv)
{
    actionResponse.mode = atoi(argv[1]);
    actionResponse.action = atoi(argv[2]);
    unsigned long  powerStatus=0;
    if (argv[1])
    {
        // Setup and configure rf radio
        radio.begin();
        radio.setPALevel(RF24_PA_LOW);
        radio.stopListening();
        radio.setPayloadSize(32);
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]);
        bool ok = radio.write( &actionResponse, sizeof(actionResponse) );
        if (!ok)
        {
           return -1;
        }
        else{
            if(actionResponse.action==0)
            {
                radio.startListening(); 
                while(true)
                {
                        if(radio.available())
                        {
                                radio.read(&powerStatus,sizeof(unsigned long));
                                if(actionResponse.mode == 1 || actionResponse.mode == 3)
                                {
                                    std::cout<<powerStatus;
                                }
                                else if (actionResponse.mode == 4)
                                {
                                    std::cout<<std::hex<<powerStatus;
                                }
                                
                                return 0;
                        }
                }
            }
        }
    }
    return 0;
}
