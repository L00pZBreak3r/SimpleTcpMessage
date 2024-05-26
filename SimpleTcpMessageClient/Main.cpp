// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "SimpleClient.h"

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc > 3)
    {
        const _TCHAR* aName = argv[1];
        int aPort = _wtoi(argv[2]);
        short aIntervalSeconds = (short)_wtoi(argv[3]);
        if (aIntervalSeconds > 0)
        {
            if ((aPort < 0) || (aPort > USHRT_MAX))
                aPort = 0;
            StartSimpleClient((unsigned short)aPort, aName, aIntervalSeconds);
        }
    }
    return 0;
}
