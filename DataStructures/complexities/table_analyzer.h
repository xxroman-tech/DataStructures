#pragma once

#include <complexities/complexity_analyzer.h>
#include <random>
#include <libds/adt/table.h>
#include <libds/amt/implicit_sequence.h>

namespace ds::utils
{
	template<class HashTable>
	class TableAnalyzer : public ComplexityAnalyzer<HashTable>
	{
	public:
		explicit TableAnalyzer(const std::string& name);

	protected:
		void beforeOperation(HashTable& table) override;
		void afterOperation(HashTable& table) override;

	private:
		void insertNElements(HashTable& table, size_t n);

	protected:
		ds::amt::ImplicitSequence<int>* isForKeys_;

		std::default_random_engine randomIndex_;
		std::default_random_engine randomKey_;
		int key_;
	};

	/**
	 * @brief Analyzes complexity of the insert operation.
	 */
	template<class HashTable>
	class TableInsertAnalyzer : public TableAnalyzer<HashTable>
	{
	public:
		explicit TableInsertAnalyzer(const std::string& name);

	protected:
		void executeOperation(HashTable& table) override;
	};

	/**
	 * @brief Analyzes complexity of the find operation.
	 */
	template<class HashTable>
	class TableFindAnalyzer : public TableAnalyzer<HashTable>
	{
	public:
		explicit TableFindAnalyzer(const std::string& name);

	protected:
		void executeOperation(HashTable& table) override;
	};

	/**
	 * @brief Container for all table analyzers.
	 */
	class TablesAnalyzer : public CompositeAnalyzer
	{
	public:
		TablesAnalyzer() :
			CompositeAnalyzer("Tables")
		{
			this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::HashTable<int, int>>>("HashTable-insert"));
			this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::HashTable<int, int>>>("HashTable-find"));
		}
	};

	template<class HashTable>
	inline TableAnalyzer<HashTable>::TableAnalyzer(const std::string& name) :
	ComplexityAnalyzer<HashTable>(name, [&](HashTable& table, size_t n) {
				this->insertNElements(table, n);
			}),
			randomIndex_(std::random_device()()),
			randomKey_(std::random_device()()),
			key_(0)
	{
		this->isForKeys_ = new ds::amt::ImplicitSequence<int>();
	}

	template<class HashTable>
	inline void TableAnalyzer<HashTable>::beforeOperation(HashTable& table)
	{
		std::uniform_int_distribution<size_t> indexDist(0, this->isForKeys_->size() - 1);

		this->key_ = this->isForKeys_->access(indexDist(randomIndex_))->data_;
	}

	template<class HashTable>
	inline void TableAnalyzer<HashTable>::afterOperation(HashTable& table)
	{
		this->isForKeys_->clear();
	}

	template<class HashTable>
	inline void TableAnalyzer<HashTable>::insertNElements(HashTable& table, size_t n) {
		std::uniform_int_distribution<int> keyDist(INT_MIN, INT_MAX); // vygenerujem si kluc

		for (size_t i = 0; i < n; i++) {
			int key = keyDist(this->randomKey_);

			int data = key; // data mozu byt rovnake

			try {
				table.insert(key, data);
				this->isForKeys_->insertLast().data_ = key;
			}
			catch (...)
			{
				i--; // ak sa nepodari vlozit tak skusim vlozit znovu iny prvok s inym klucom
			}  
		}
	}

	template<class HashTable>
	TableInsertAnalyzer<HashTable>::TableInsertAnalyzer(const std::string& name) :
		TableAnalyzer<HashTable>(name)
	{
	}

	template<class HashTable>
	void TableInsertAnalyzer<HashTable>::executeOperation(HashTable& table)
	{
		std::uniform_int_distribution<int> keyDist(INT_MIN, INT_MAX);
		int key = keyDist(this->randomKey_);
		int data = key; // data mozu byt rovnake

		table.insert(key, data);
	}

	template<class HashTable>
	TableFindAnalyzer<HashTable>::TableFindAnalyzer(const std::string& name) :
		TableAnalyzer<HashTable>(name)
	{
	}

	template<class HashTable>
	void TableFindAnalyzer<HashTable>::executeOperation(HashTable& table)
	{
		table.find(this->key_);
	}
}
