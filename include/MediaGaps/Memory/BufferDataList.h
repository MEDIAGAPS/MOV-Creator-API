#ifndef _MEDIAGAPS_MEMORY_IBUFFERDATALIST_H_
#define _MEDIAGAPS_MEMORY_IBUFFERDATALIST_H_

#include "MediaGaps/IList.h"
#include "MediaGaps/Memory/BufferDataSharedPtr.h"

namespace MediaGaps
{
	namespace Memory
	{
		class MEDIAGAPS_EXPORTS_KEYWORD BufferDataList : public IList<BufferDataSharedPtr>
		{
			struct Impl;
			Impl* _impl;
		public:
			BufferDataList();
			BufferDataList(const BufferDataList& other);
			BufferDataList& operator=(const BufferDataList& other);

			virtual ~BufferDataList();

			virtual void Add(BufferDataSharedPtr item);
			virtual void Remove(BufferDataSharedPtr item);
			virtual const BufferDataSharedPtr& operator[](size_t i) const;
			virtual BufferDataSharedPtr& operator[](size_t i);
			virtual size_t GetListSize() const;

			virtual IListConstIterator ConstBegin() const;
			virtual IListConstIterator ConstEnd() const;
			virtual IListIterator Begin();
			virtual IListIterator End();

			
			/** Pointer to the position where the sample is stored at first element.
			* @throw ExceptionCode_InvalidState when buffer data list is not single dimension 
		   */
			const unsigned char* GetFirstBuffer() const;

			/** Returns the size of the sample at first element
			* @throw ExceptionCode_InvalidState when buffer data list is not single dimension
			*/
			UInt64 GetFirstSize() const ;

			/** Returns the linesize of the sample at first element. If the value is not know returns 0.
			* @throw ExceptionCode_InvalidState when buffer data list is not single dimension
			*/
			UInt64 GetFirstLinesize() const ;

		};
	}
}

#endif //_MEDIAGAPS_MEMORY_IBUFFERDATALIST_H_
