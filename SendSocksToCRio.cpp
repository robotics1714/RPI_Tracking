#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <cmath>

using namespace std;

int main()
{
    int status;
    int roboSock;
    struct addrinfo hints;
    struct addrinfo* roboInfo;
    float lastSent = (float)clock()/CLOCKS_PER_SEC;

    memset(&hints, 0, sizeof(hints));

    //set the values of the hint s struct
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    //Get the information of the crio
    status = getaddrinfo("10.17.14.2", "1714", &hints, &roboInfo);
    //Check for an error
    if(status != 0)
    {
        cout<<gai_strerror(status);
        cin.get();
        return -1;
    }

    //Create the socket
    roboSock = socket(roboInfo->ai_family, roboInfo->ai_socktype, roboInfo->ai_protocol);
    //Check to see if there is an error
    if(roboSock == -1)
    {
        cout<<"Error creating socket";
        cin.get();
        return -1;
    }

    int count = 0;
    while(true)
    {
        float currTime = (float)clock()/CLOCKS_PER_SEC;
        if(abs(currTime - lastSent) > 0.1)
        {
            char msg[20];
            memset(&msg, 0, sizeof(msg));
            sprintf(msg, "%d", count);
            count++;
            int j = sendto(roboSock, msg, 20, 0, roboInfo->ai_addr, roboInfo->ai_addrlen);
            lastSent = currTime;
            cout<<"Sent\n";
            if(j == -1)
                cout<<"Bad send to\n";
        }
    }

    return 1;
}
