#include <iostream>
#include <Windows.h>
#include <chrono>

using namespace std;

alignas(64ull) void* g_pThreadEvent = nullptr;

alignas(64ull) void* g_pThreads[8]{};
alignas(64ull) unsigned long g_ulThreadIDs[8]{};

alignas(64ull) long long g_llValue = 0ll;

alignas(64ull) long long g_llLock = 0ll;
alignas(64ull) _RTL_CRITICAL_SECTION g_stCriticalSection{};

struct SValue
{
    //long long g_llLock;
    long long llValue;
    _RTL_CRITICAL_SECTION stCriticalSection;
    unsigned char pPadding[16];
};

alignas(64ull) SValue g_pValue2[8]{};

unsigned long __stdcall Thread(void* const _pParameter)
{
    const unsigned long long ullThreadIndex = reinterpret_cast<unsigned long long>(_pParameter);

    while (true)
    {
        // Spinlock
        //while (true)
        //{
        //    if (_InterlockedCompareExchange64(&g_pValue2[ullThreadIndex].g_llLock, 1ll, 0ll) == 0ll)
        //    {
        //        break;
        //    }
        //
        //    Sleep(0ul);
        //}
        //
        ////++g_llValue;
        //++g_pValue2[ullThreadIndex].llValue;
        //
        //_InterlockedExchange64(&g_pValue2[ullThreadIndex].g_llLock, 0ll);

        // Critical section
        //EnterCriticalSection(&g_pValue2[ullThreadIndex].stCriticalSection);
        //
        //++g_pValue2[ullThreadIndex].llValue;
        //
        //LeaveCriticalSection(&g_pValue2[ullThreadIndex].stCriticalSection);
    }

    return 0ul;
}

int main()
{
    // Create event
    //g_pThreadEvent = CreateEventW(nullptr, 0, 0, nullptr);
    //if (g_pThreadEvent == nullptr)
    //{
    //    // TODO : Rollback
    //}

    InitializeCriticalSection(&g_stCriticalSection);

    for (unsigned long long i = 0ull; i < 8ull; ++i)
    {
        InitializeCriticalSection(&g_pValue2[i].stCriticalSection);
    }

    // Create threads
    for (unsigned long long i = 0ull; i < 8ull; ++i)
    {
        g_pThreads[i] = CreateThread(nullptr,
                                     0ull,
                                     &Thread,
                                     reinterpret_cast<void*>(i),
                                     0ul,
                                     &g_ulThreadIDs[i]);
        if (g_pThreads[i] == nullptr)
        {
            // TODO : Rollback
        }
    }

    Sleep(3000ul);

    DeleteCriticalSection(&g_stCriticalSection);

    for (unsigned long long i = 0ull; i < 8ull; ++i)
    {
        DeleteCriticalSection(&g_pValue2[i].stCriticalSection);
    }

    return 0;
}