#include "WaveSampleProvider.h"

#include "BufferDataImpl.h"
#include "MediaSampleImpl.h"

#include "MediaGaps/Exception.h"

#include <iostream>

using namespace MediaGaps::IO;
using namespace MediaGaps::Media;
using namespace MediaGaps::Memory;
using namespace MediaGaps;

using namespace MOVCreator;

namespace
{
    struct WaveBaseChunk
    {
        UInt32 chunkId = 0;
        UInt32 chunkSize = 0;
    };

    struct WaveHeader : WaveBaseChunk
    {
        WaveHeader(WaveBaseChunk baseChunk)
            :WaveBaseChunk(baseChunk)
        {
        }

        UInt32 format = 0;
    };

    struct WaveFMTChunck : WaveBaseChunk
    {
        WaveFMTChunck(WaveBaseChunk baseChunk)
            :WaveBaseChunk(baseChunk)
        {
        }

        UInt16 audioFormat = 0;
        UInt16 numChannels = 0;
        UInt32 sampleRate = 0;
        UInt32 byteRate = 0;
        UInt16 blockAlign = 0;
        UInt16 bitsPerSample = 0;
    };
}


WaveSampleProvider::WaveSampleProvider(const wchar_t* source_name)
    : _source(source_name, 4 * 1024 * 1024)
{
    this->LoadCache(true);
}

void WaveSampleProvider::LoadCache(bool loadTrackDescriptor)
{
    if (_cache.Get()) return;

    if (_finished || _source.HasFinished())
    {
        _cache.Reset();
        return;
    }

    try
    {
        if (loadTrackDescriptor)
        {
            bool dataChunkLoaded = false;
            while (!dataChunkLoaded)
            {
                WaveBaseChunk baseChunk;
                _source.Read(reinterpret_cast<unsigned char*> (&baseChunk.chunkId), sizeof(baseChunk.chunkId));
                _source.Read(reinterpret_cast<unsigned char*> (&baseChunk.chunkSize), sizeof(baseChunk.chunkSize));

                switch (baseChunk.chunkId)
                {
                case 'FFIR':
                {
                    WaveHeader waveHeader(baseChunk);
                    _source.Read(reinterpret_cast<unsigned char*> (&waveHeader.format), sizeof(waveHeader.format));
                }
                break;
                case ' tmf':
                {
                    WaveFMTChunck fmtChunk(baseChunk);
                    _source.Read(reinterpret_cast<unsigned char*> (&fmtChunk.audioFormat), sizeof(fmtChunk.audioFormat));
                    _source.Read(reinterpret_cast<unsigned char*> (&fmtChunk.numChannels), sizeof(fmtChunk.numChannels));
                    _source.Read(reinterpret_cast<unsigned char*> (&fmtChunk.sampleRate), sizeof(fmtChunk.sampleRate));
                    _source.Read(reinterpret_cast<unsigned char*> (&fmtChunk.byteRate), sizeof(fmtChunk.byteRate));
                    _source.Read(reinterpret_cast<unsigned char*> (&fmtChunk.blockAlign), sizeof(fmtChunk.blockAlign));
                    _source.Read(reinterpret_cast<unsigned char*> (&fmtChunk.bitsPerSample), sizeof(fmtChunk.bitsPerSample));

                    _soundTrackDescriptor.SetTrackId(2);
                    _soundTrackDescriptor.SetEssenceKind(TrackDescriptorEssenceKind_PCM);
                    _soundTrackDescriptor.SetSampleRate(Rational32(fmtChunk.sampleRate, 1));
                    _soundTrackDescriptor.SetSamplingRate(fmtChunk.sampleRate);
                    _soundTrackDescriptor.SetChannelCount(fmtChunk.numChannels);
                    _soundTrackDescriptor.SetSamplingBits(fmtChunk.bitsPerSample);

                    _maxReadSize = fmtChunk.bitsPerSample * fmtChunk.sampleRate * fmtChunk.numChannels / 8;

                    if (fmtChunk.chunkSize > sizeof(WaveFMTChunck))
                    {
                        _source.SkipData(sizeof(WaveFMTChunck) - fmtChunk.chunkSize);
                    }
                }
                break;
                case 'atad':
                {
                    dataChunkLoaded = true;
                }
                break;
                default:
                {
                    _source.SkipData(baseChunk.chunkSize - sizeof(WaveBaseChunk));
                }
                }
            }
        }


        UInt64 dataToRead = _source.GetRemainingData() > _maxReadSize ? _maxReadSize : _source.GetRemainingData();

        BufferDataSharedPtr dataBuffer(new BufferDataImpl(dataToRead));
        auto dataBufferImpl = reinterpret_cast<BufferDataImpl*> (dataBuffer.Get());

        UInt64 readSize = _source.Read(dataBufferImpl->MemoryPointer(), dataToRead);
        ExceptionHandler::EnforceMediaGapsException(readSize == dataToRead, ExceptionCode_IO_Error, "Unable to read audio block from file");

        _cache.Reset(new MediaSampleImpl);
        _cache->Add(dataBuffer);
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


MediaSampleSharedPtr WaveSampleProvider::GetNextSample()
{
    this->LoadCache();

    MediaSampleSharedPtr toReturn = _cache;
    _cache.Reset();

    return toReturn;
}

bool WaveSampleProvider::IsNextSampleAvailable() const
{
    return true;
}

void WaveSampleProvider::Abort()
{
}

const TrackDescriptor& WaveSampleProvider::Descriptor() const
{
    return _soundTrackDescriptor;
}
