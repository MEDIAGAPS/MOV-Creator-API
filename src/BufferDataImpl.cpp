#include "BufferDataImpl.h"

using namespace MediaGaps::Memory;
using namespace MediaGaps;
using namespace MOVCreator;

BufferDataImpl::BufferDataImpl(UInt64 size)
    : AlignedData(size)
{
}

const unsigned char* BufferDataImpl::GetBuffer() const
{
    return this->GetAlignedBufferConst();
}

UInt64 BufferDataImpl::GetSize() const
{
    return this->GetAlignedAllocatedSize();
}

UInt64 BufferDataImpl::GetLinesize() const
{
    //Since this doesn't have a fixed line size we return 0;
    return 0;
}

unsigned char* BufferDataImpl::MemoryPointer()
{
    return this->GetAlignedBuffer();
}
