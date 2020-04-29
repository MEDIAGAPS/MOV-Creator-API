#ifndef _BUFFERDATAIMPL_H_
#define _BUFFERDATAIMPL_H_

#include "MediaGaps/Memory/IBufferData.h"
#include "MediaGaps/Memory/AlignedData.h"

namespace MOVCreator
{
    /** This class represents a stream of data.
         *   In this case we're going to copy data inside the buffer.
         *   In your implementation if you already have data in memory you can implement your own IBufferData instance in order to avoid memory copies
         */
    class BufferDataImpl : public MediaGaps::Memory::IBufferData, public MediaGaps::Memory::AlignedData
    {
    public:
        BufferDataImpl(MediaGaps::UInt64 size);

        const unsigned char* GetBuffer() const override;

        MediaGaps::UInt64 GetSize() const override;

        MediaGaps::UInt64 GetLinesize() const override;

        unsigned char* MemoryPointer();
    };

}

#endif _BUFFERDATAIMPL_H_
