// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "SimpleServer.h"

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc > 1)
    {
        int aPort = _wtoi(argv[1]);
        if ((aPort < 0) || (aPort > USHRT_MAX))
            aPort = 0;
        StartSimpleServer((unsigned short)aPort);
    }
    return 0;
}
