#pragma once



namespace MemoryTracking
{
    class Heap
    {
    public:
        void* Allocate(int bytes, int alignment);
        void Deallocate(void* pMemory);
    };


    class Tracker
    {
    public:
        static void Initialize();

        static void Terminate();
    };
}



