#pragma once

struct AllocationMetrics
{
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t CurrentMemoryUsage() const { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics; //Allocaion info

void* operator new(size_t size) {
    std::cout << "Allocated " << size << "bytes.\n";
    s_AllocationMetrics.TotalAllocated += size;
    return malloc(size);
}
void operator delete(void* memory, size_t size) {
    std::cout << "Freed " << size << "bytes.\n";
    s_AllocationMetrics.TotalFreed += size;
    free(memory);
}
