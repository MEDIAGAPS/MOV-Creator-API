#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "MediaGaps/Media/IStateProvider.h"

namespace MOVCreator
{
namespace Helpers
{
    template <typename TYPE>
    TYPE RotateByte(TYPE value)
    {
        TYPE reversed = 0;
        MediaGaps::UInt8* reversedPointer = reinterpret_cast<MediaGaps::UInt8*>(&reversed);
        MediaGaps::UInt8* valuePointer = reinterpret_cast<MediaGaps::UInt8*>(&value) + sizeof(TYPE) - 1;

        for (size_t counter = 0; counter < sizeof(TYPE); ++counter)
        {
            reversedPointer[counter] = *valuePointer;
            --valuePointer;
        }

        return reversed;
    }

    bool IsFinishedState(MediaGaps::Media::IStateProvider::State state);
}
}

#endif //_HELPERS_H_
