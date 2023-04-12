#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>

namespace ds::adt {

    class Dimension
    {
    public:
        Dimension();
        Dimension(int base, size_t size);
        Dimension(const Dimension& other);

        int getBase() const;
        size_t getSize() const;

        bool operator==(const Dimension& other) const;
        bool operator!=(const Dimension& other) const;
        Dimension& operator=(const Dimension& other);

    private:
        int base_;
        size_t size_;
    };

    //----------

    template <typename T>
    class Array :
        public ADS<T>
    {
    public:
        using IteratorType = typename amt::IS<T>::IteratorType;

    public:
        Array(size_t size);
        Array(Dimension dimension);
        Array(const Array<T>& other);
        ~Array();

        ADT& assign(const ADT& other) override;
        void clear() override;
        bool isEmpty() const override;
        bool equals(const ADT& other) override;

        size_t size() const override;
        int getBase() const;
        T access(size_t index) const;
        void set(T element, size_t index);

        IteratorType begin();
        IteratorType end();

    private:
        bool validateIndex(size_t index) const;
        size_t mapIndex(size_t index) const;
        amt::IS<T>* getSequence() const;

    private:
        int base_;
    };

    //----------

    template <typename T>
    class Matrix :
        public ADS<T>
    {
    public:
        Matrix(size_t size1, size_t size2);
        Matrix(Dimension dimension1, Dimension dimension2);
        Matrix(const Matrix& other);
        ~Matrix();

        ADT& assign(const ADT& other) override;
        void clear() override;
        size_t size() const override;
        bool isEmpty() const override;
        bool equals(const ADT& other) override;

        T access(size_t index1, size_t index2) const;
        void set(T element, size_t index1, size_t index2);

    private:
        bool validateIndices(size_t index1, size_t index2) const;
        size_t mapIndices(size_t index1, size_t index2) const;
        amt::IS<T>* getSequence() const;

    private:
        Dimension dimension1_;
        Dimension dimension2_;
    };

    //----------

    inline Dimension::Dimension() :
        Dimension(0, 0)
    {
    }

    inline Dimension::Dimension(int base, size_t size) :
        base_(base),
        size_(size)
    {
    }

    inline Dimension::Dimension(const Dimension& other) :
        base_(other.base_),
        size_(other.size_)
    {
    }

    inline int Dimension::getBase() const
    {
        return base_;
    }

    inline size_t Dimension::getSize() const
    {
        return size_;
    }

    inline bool Dimension::operator==(const Dimension& other) const
    {
        return base_ == other.base_ && size_ == other.size_;
    }

    inline bool Dimension::operator!=(const Dimension& other) const
    {
        return !(*this == other);
    }

    inline Dimension& Dimension::operator=(const Dimension& other)
    {
        base_ = other.base_;
        size_ = other.size_;
        return *this;
    }

    //----------

    template<typename T>
    Array<T>::Array(size_t size) :
        Array(Dimension(0, size))
    {
    }

    template<typename T>
    Array<T>::Array(Dimension dimension) :
        ADS<T>(new amt::IS<T>(dimension.getSize(), true)),
        base_(dimension.getBase())
    {
    }

    template<typename T>
    Array<T>::Array(const Array<T>& other) :
        ADS<T>(new amt::IS<T>(), other),
        base_(other.base_)
    {
    }

    template<typename T>
    Array<T>::~Array()
    {
    }

    template<typename T>
    ADT& Array<T>::assign(const ADT& other)
    {
        // Cvicenie 08
        const Array<T>& otherArray = dynamic_cast<const Array<T>&>(other);
        if (base_ != otherArray.base_ || this->size() != otherArray.size()) {
            this->error("Array dimensions are different!");
        }
        ADS<T>::assign(other);

        return *this;
    }

    template<typename T>
    void Array<T>::clear()
    {
        this->error("Array can't be cleared!");
    }

    template<typename T>
    size_t Array<T>::size() const
    {
        return this->getSequence()->size();
    }

    template<typename T>
    int Array<T>::getBase() const
    {
        return base_;
    }

    template<typename T>
    bool Array<T>::isEmpty() const
    {
        // Cvicenie 08
        return false;
    }

    template<typename T>
    bool Array<T>::equals(const ADT& other)
    {
        const Array<T>& otherArray = dynamic_cast<const Array<T>&>(other);
        return base_ == otherArray.base_ && ADS<T>::equals(other);
    }

    template<typename T>
    T Array<T>::access(size_t index) const
    {
        // Cvicenie 08
        if (!this->validateIndex(index)) {
            this->error("Invalid index!");
        }

        return this->getSequence()->access(this->mapIndex(index))->data_;
    }

    template<typename T>
    void Array<T>::set(T element, size_t index)
    {
        // Cvicenie 08
        if (!this->validateIndex(index)) {
            this->error("Invalid index!");
        }

        this->getSequence()->access(this->mapIndex(index))->data_ = element;
    }

    template <typename T>
    auto Array<T>::begin() -> IteratorType
    {
        // Cvicenie 08
        return this->getSequence()->begin();
    }

    template <typename T>
    auto Array<T>::end() -> IteratorType
    {
        // Cvicenie 08
        return this->getSequence()->end();
    }

    template<typename T>
    amt::IS<T>* Array<T>::getSequence() const
    {
        return static_cast<amt::IS<T>*>(this->memoryStructure_);
    }

    template<typename T>
    bool Array<T>::validateIndex(size_t index) const
    {
        // Cvicenie 08
        return index >= this->base_ && index < this->base_ + this->size();
    }

    template<typename T>
    size_t Array<T>::mapIndex(size_t index) const
    {
        // Cvicenie 08
        return index - this->base_;
    }

    //----------

    template<typename T>
    Matrix<T>::Matrix(size_t size1, size_t size2) :
        Matrix<T>(Dimension(0, size1), Dimension(0, size2))
    {
    }

    template<typename T>
    Matrix<T>::Matrix(Dimension dimension1, Dimension dimension2) :
        ADS<T>(new amt::IS<T>(dimension1.getSize() * dimension2.getSize(), true)),
        dimension1_(dimension1),
        dimension2_(dimension2)
    {
    }

    template<typename T>
    Matrix<T>::Matrix(const Matrix<T>& other) :
        ADS<T>(new amt::IS<T>(), other)
    {
    }

    template<typename T>
    Matrix<T>::~Matrix()
    {
    }

    template<typename T>
    ADT& Matrix<T>::assign(const ADT& other)
    {
        const Matrix<T>& otherMatrix = dynamic_cast<const Matrix<T>&>(other);
        if (dimension1_ != otherMatrix.dimension1_ || dimension2_ != otherMatrix.dimension2_)
        {
            this->error("Matrix dimensions are different!");
        }
        ADS<T>::assign(otherMatrix);
        return *this;
    }

    template<typename T>
    void Matrix<T>::clear()
    {
        this->error("Matrix can't be cleared!");
    }

    template<typename T>
    size_t Matrix<T>::size() const
    {
        return dimension1_.getSize() * dimension2_.getSize();
    }

    template<typename T>
    bool Matrix<T>::isEmpty() const
    {
        return false;
    }

    template<typename T>
    bool Matrix<T>::equals(const ADT& other)
    {
        const Matrix<T>& otherMatrix = dynamic_cast<const Matrix<T>&>(other);
        return dimension1_ == otherMatrix.dimension1_ &&
               dimension2_ == otherMatrix.dimension2_ &&
               ADS<T>::equals(other);
    }

    template<typename T>
    T Matrix<T>::access(size_t index1, size_t index2) const
    {
        // Cvicenie 08
        if (this->validateIndices(index1, index2)) {
            this->error("Invalid index!");
        }
        size_t mappedIndex = this->mapIndices(index1, index2);

        return this->getSequence()->access(mappedIndex)->data_;
    }
    template<typename T>
    void Matrix<T>::set(T element, size_t index1, size_t index2)
    {
        // Cvicenie 08
        if (this->validateIndices(index1, index2)) {
            this->error("Invalid index!");
        }
        size_t mappedIndex = this->mapIndices(index1, index2);

        this->getSequence()->access(mappedIndex)->data_ = element;
    }

    template<typename T>
    bool Matrix<T>::validateIndices(size_t index1, size_t index2) const
    {
        // Cvicenie 08
        return index1 >= this->dimension1_.getBase() &&
            index1 < this->dimension1_.getBase() + this->dimension1_.getSize() &&
            index2 >= this->dimension2_.getBase() &&
            index2 < this->dimension2_.getBase() + this->dimension2_.getSize();
    }

    template<typename T>
    size_t Matrix<T>::mapIndices(size_t index1, size_t index2) const
    {
        // Cvicenie 08
        return (index1 - this->dimension1_.getBase()) * this->dimension2_.getSize() +
            (index2 - this->dimension2_.getBase());
    }

    template<typename T>
    amt::IS<T>* Matrix<T>::getSequence() const
    {
        return static_cast<amt::IS<T>*>(this->memoryStructure_);
    }
}