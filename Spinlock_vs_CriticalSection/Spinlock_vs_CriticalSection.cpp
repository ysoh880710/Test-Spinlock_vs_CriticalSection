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

unsigned long __stdcall Thread(void* const _pParameter)
{
    const unsigned long long ullThreadIndex = reinterpret_cast<unsigned long long>(_pParameter);

    while (true)
    {
        // Spinlock
        //while (true)
        //{
        //    if (_InterlockedCompareExchange64(&g_llLock, 1ll, 0ll) == 0ll)
        //    {
        //        break;
        //    }
        //
        //    Sleep(0ul);
        //}
        //
        //++g_llValue;
        //
        //_InterlockedExchange64(&g_llLock, 0ll);

        // Critical section
        EnterCriticalSection(&g_stCriticalSection);
        
        ++g_llValue;
        
        LeaveCriticalSection(&g_stCriticalSection);
    }

    return 0ul;
}

int main()
{
    // Create event
    g_pThreadEvent = CreateEventW(nullptr, 0, 0, nullptr);
    if (g_pThreadEvent == nullptr)
    {
        // TODO : Rollback
    }

    InitializeCriticalSection(&g_stCriticalSection);

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

    return 0;
}