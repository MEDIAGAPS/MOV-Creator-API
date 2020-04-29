#include "ProResSampleProvider.h"

#include "BufferDataImpl.h"
#include "Helpers.h"
#include "MediaSampleImpl.h"

#include "MediaGaps/ExceptionHandler.h"
#include "MediaGaps/Exception.h"

#include <iostream>
#include <memory>

using namespace MediaGaps::IO;
using namespace MediaGaps::Media;
using namespace MediaGaps::Memory;
using namespace MediaGaps;

using namespace MOVCreator;

namespace
{
    //Only using required information for extracting frame details to fill PictureTrackDescriptor
    struct ProResImage
    {
        UInt32 size = 0;
        UInt32 type = 0;

        UInt16 headerSize = 0;
        UInt16 version = 0;
        UInt32 creatorId = 0;

        UInt16 frameWidth = 0;
        UInt16 frameHeigth = 0;

        UInt8 chromaAndInterlaced = 0;
    };
}

ProResSampleProvider::ProResSampleProvider(const wchar_t* source_name, TrackDescriptorEssenceKind essencekind, Rational32 editRate)
    : _source(source_name, 4 * 1024 * 1024)
{
    _pictureTrackDescriptor.SetSampleRate(editRate);
    _pictureTrackDescriptor.SetEssenceKind(essencekind);
    this->LoadCache(true);
}

ProResSampleProvider::~ProResSampleProvider()
{
    std::cout << "Processed ProRes samples: " << _readSamples << std::endl;
}

void ProResSampleProvider::LoadCache(bool loadTrackDescriptor)
{
    if (_finished || _source.HasFinished())
    {
        _cache.Reset();
        return;
    }

    try
    {
        UInt32 imageSize = 0;
        UInt64 readSize = _source.Read(reinterpret_cast<unsigned char*> (&imageSize), sizeof(imageSize));

        ExceptionHandler::EnforceMediaGapsException(readSize == sizeof(imageSize), ExceptionCode_IO_Error, "Unable to read image size");

        UInt32 imageSizeRotated = Helpers::RotateByte(imageSize);
        BufferDataSharedPtr dataBuffer(new BufferDataImpl(imageSizeRotated));
        auto dataBufferImpl = reinterpret_cast<BufferDataImpl*> (dataBuffer.Get());

        memcpy(dataBufferImpl->MemoryPointer(), &imageSize, sizeof(imageSize));

        readSize = _source.Read(dataBufferImpl->MemoryPointer() + sizeof(imageSize), imageSizeRotated - sizeof(imageSize));
        ExceptionHandler::EnforceMediaGapsException(readSize == imageSizeRotated - sizeof(imageSize), ExceptionCode_IO_Error, "Unable to read full image");

        _cache.Reset(new MediaSampleImpl);
        _cache->Add(dataBuffer);

        if (loadTrackDescriptor)
        {
            const auto proResHeader = reinterpret_cast<ProResImage*> (dataBufferImpl->MemoryPointer());

            _pictureTrackDescriptor.SetTrackId(1);
            _pictureTrackDescriptor.SetComponentDepth(10);
            _pictureTrackDescriptor.SetInterlaced((proResHeader->chromaAndInterlaced >> 2 & 0x3) != 0);
            _pictureTrackDescriptor.SetWidth(Helpers::RotateByte(proResHeader->frameWidth));
            _pictureTrackDescriptor.SetHeight(Helpers::RotateByte(proResHeader->frameHeigth));
        }

        ++_readSamples;
    }
    catch (const Exception& ex)
    {
        std::cout << "Finishing reading from file because: " << ex.GetMGExceptionMessage() << std::endl;
        _finished = true;
        _cache.Reset();
    }
    catch (...)
    {
        std::cout << "Finishing reading from file because unexpected size";
        _finished = true;
        _cache.Reset();
    }
}

MediaSampleSharedPtr ProResSampleProvider::GetNextSample()
{
    this->LoadCache();

    MediaSampleSharedPtr toReturn = _cache;
    _cache.Reset();

    return toReturn;
}

bool ProResSampleProvider::IsNextSampleAvailable() const
{
    return true;
}

void ProResSampleProvider::Abort()
{
}

const TrackDescriptor& ProResSampleProvider::Descriptor() const
{
    return _pictureTrackDescriptor;
}
