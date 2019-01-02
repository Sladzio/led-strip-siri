#include <cstdlib>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <sys/file.h>

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
    int fd = open("locker", O_RDWR | O_CREAT, 0666); // open or create lockfile
    //check open success...
    int rc = flock(fd , LOCK_EX | LOCK_NB); // grab exclusive lock, fail if can't obtain.
    while (rc!=0)
    {
        delay(50);
        rc = flock(fd , LOCK_EX | LOCK_NB); 
    }
    if (argv[1])
    {
        // Setup and configure rf radio
        radio.begin();
        radio.setPALevel(RF24_PA_LOW);
        radio.setPayloadSize(32);
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]);
        while(true)
        {
            radio.stopListening();
            bool ok = radio.write( &actionResponse, sizeof(actionResponse) );
            if (!ok)
            {
               flock(fd , LOCK_UN | LOCK_NB);
               return -1;
            }
            else{
                if(actionResponse.action==0 && actionResponse.mode != 2)
                {
                    radio.startListening(); 
                                    
                     // Wait here until we get a response, or timeout (250ms)
                    unsigned long started_waiting_at = millis();
                    bool timeout = false;
                    while ( ! radio.available() && ! timeout )
                      if (millis() - started_waiting_at > 200 )
                        timeout = true;
                
                    // Describe the results
                    if ( !timeout )
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
                        flock(fd , LOCK_UN | LOCK_NB);
                        return 0;
                    }
                    delay(1000);
                }
                else{
                    flock(fd , LOCK_UN | LOCK_NB);
                    return 0;
                }
            }
        }
      
    }
    flock(fd , LOCK_UN | LOCK_NB);
    return 0;
}
