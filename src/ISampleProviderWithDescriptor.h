#ifndef _ISAMPLEPROVIDERWITHDESCRIPTOR_H_
#define _ISAMPLEPROVIDERWITHDESCRIPTOR_H_

#include "MediaGaps/Media/ISampleProvider.h"
#include "MediaGaps/Media/TrackDescriptor.h"

namespace MOVCreator
{
    /** An interface class for holding a ISampleProvider and returning track information about it-
     *
     */
    class ISampleProviderWithDescriptor : public MediaGaps::Media::ISampleProvider
    {
    public:
        virtual const MediaGaps::Media::TrackDescriptor& Descriptor() const = 0;
    };
}

#endif //_ISAMPLEPROVIDERWITHDESCRIPTOR_H_
