#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <algorithm>
#include <iostream>

namespace ds::mm {

	template<typename BlockType>
	class CompactMemoryManager : public MemoryManager<BlockType> {
	public:
		CompactMemoryManager();
		CompactMemoryManager(size_t size);
		CompactMemoryManager(const CompactMemoryManager<BlockType>& other);
		~CompactMemoryManager() override;

		BlockType* allocateMemory() override;
		BlockType* allocateMemoryAt(size_t index);
		void releaseMemory(BlockType* pointer) override;
		void releaseMemoryAt(size_t index);
		void releaseMemory();

		size_t getCapacity() const;

		CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
		void changeCapacity(size_t newCapacity);
		void shrinkMemory();
		void clear();
		bool equals(const CompactMemoryManager<BlockType>& other) const;
		void* calculateAddress(const BlockType& data);
		size_t calculateIndex(const BlockType& data);
		BlockType& getBlockAt(size_t index);
		void swap(size_t index1, size_t index2);

		void print(std::ostream& os);

	private:
		size_t getAllocatedBlocksSize() const;
		size_t getAllocatedCapacitySize() const;

	private:
		BlockType* base_;
		BlockType* end_;
		BlockType* limit_;

		static const size_t INIT_SIZE = 4;
	};

	template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
		CompactMemoryManager(INIT_SIZE)
	{
	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
		base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
		end_(base_),
		limit_(base_ + size) // posunie sa o velkost size pretoze uz tam je smernik na BlockType
	{
	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other):
		CompactMemoryManager(other.getAllocatedBlockCount())
	{
		// Cvicenie 02, kopirovaci konstruktor
		// chcem vytvorit kopiu prichodzej pamate
		// robim to iste ako ked davam cmm1 = cmm2; teda assign();

		this->assign(other);
	}

	template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
	{
		// Cvicenie 02
		releaseMemory(base_); // zavola destruktory na danych objektoch, ak su tam objekty nie hodnoty
		std::free(base_);

		base_ = nullptr;
		end_ = nullptr;
		limit_ = nullptr;
	}

	template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
	{
		// Cvicenie 02 doma
		return allocateMemoryAt(end_ - base_);
	}

	template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
	{
		// Cvicenie 02 doma
		if (end_ == limit_)
		{	
			changeCapacity(2 * allocatedBlockCount_);
		}

		if (end_ - base_ > index)
		{
			std::memmove(base_ + index + 1, base_ + index, ((end_ - base_) - index) * sizeof(BlockType));
		}

		allocatedBlockCount_++;
		end_++;

		BlockType* pointerOnBlock = base_ + index;
		BlockType& newBlock = *pointerOnBlock;
		return &newBlock;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
	{
		// Cvicenie 02
		// vstup pointer, cielom metody je uprace vsetko co je na tom pointry a dalej
		// musim skontrolovat ci vstupny ukazovatel ukaze na nasu pamat
		if (pointer < base_ || pointer > end_)
		{
			throw std::bad_exception();
		}

		if (std::is_class<BlockType>::value)
		{
			BlockType* p = pointer;

			while (p != end_)
			{
				p->~BlockType();
				p++;
			}
		}

		end_ = pointer;

		allocatedBlockCount_ = end_ - base_;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
	{
		// Cvicenie 02 doma
		std::memmove(base_ + index, base_ + index + 1, ((end_ - base_) - index - 1) * sizeof(BlockType));
		releaseMemory();
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
	{
		// Cvicenie 02 doma
		releaseMemory(end_ - 1);
	}

	template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
	{
		// Cvicenie 02, capacita je maximalny pocet prvkov ktory mozem zmestit
		return limit_ - base_;
	}

	template<typename BlockType>
	CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign(const CompactMemoryManager<BlockType>& other) {
		// Cvicenie 02, operacia priradenia

		if (this != &other)
		{
			this->releaseMemory(base_);
			allocatedBlockCount_ = other.allocatedBlockCount_;
			void* newBase = std::realloc(base_, other.getAllocatedCapacitySize());
			if (newBase == nullptr)
			{
				throw std::bad_alloc();
			}
			base_ = static_cast<BlockType*>(newBase);
			end_ = base_ + allocatedBlockCount_;
			limit_ = base_ + (other.limit_ - other.base_);

			for (size_t i = 0; i < other.getAllocatedBlockCount(); ++i)
			{
				placement_copy(base_ + i, *(other.base_ + i));
			}
		}
		return *this;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
	{
		size_t newCapacity = end_ - base_;

		if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
		{
			newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
		}

		changeCapacity(newCapacity);
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
	{
		// Cvicenie 02
		if (newCapacity == this->getCapacity())
		{
			return;
		}

		if (newCapacity < this->getAllocatedBlockCount())
		{
			this->releaseMemory(base_ + newCapacity);
		}

		void* newFirst = std::realloc(base_, newCapacity * sizeof(BlockType));

		if (newFirst == nullptr)
		{
			throw std::bad_alloc();
		}

		base_ = static_cast<BlockType*>(newFirst);
		end_ = base_ + allocatedBlockCount_;
		limit_ = base_ + newCapacity;
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
	{
		// Cvicenie 02 doma
		releaseMemory(base_);
	}

	template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
	{
		// Cvicenie 02
		// porovnanie dvoch memory
		return this == &other ||
			allocatedBlockCount_ == other.allocatedBlockCount_ && 
			std::memcmp(base_, other.base_, allocatedBlockCount_ * sizeof(BlockType)) == 0;
	}

	template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
	{
		// Cvicenie 02, prechadzam pamatovimi miestami od zaciatku do konca

		BlockType* p = base_;

		while (p != end_ && p != &data)
		{
			p++;
		}

		return p == end_ ? nullptr : p;
	}

	template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
	{
		// Cvicenie 02
		return &data < end_ && &data >= base_ ? &data - base_ : INVALID_INDEX;
	}

	template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
	{
		// Cvicenie 02, vrati data na danom indexe
		return *(base_ + index);
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
	{
		std::swap(getBlockAt(index1), getBlockAt(index2));
	}

	template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
	{
		// Cvicenie 02, velkost ktoru mam obsadenu
		return (end_ - base_) * sizeof(BlockType);
	}

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
	{
		// Cvicenie 02, velkost celeho priestoru
		return (limit_ - base_) * sizeof(BlockType);
	}

	template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
	{
		os << "first = " << base_ << std::endl;
		os << "last = " << end_ << std::endl;
		os << "limit = " << limit_ << std::endl;
		os << "block size = " << sizeof(BlockType) << "B" << std::endl;

		BlockType* ptr = base_;
		while (ptr != limit_) {
			std::cout << ptr;
			os << PtrPrintBin<BlockType>(ptr);

			if (ptr == base_) {
				os << "<- first";
			}
			else if (ptr == end_) {
				os << "<- last";
			}
			os << std::endl;
			ptr++;
		}

		os << limit_ << "|<- limit" << std::endl;
	}

}