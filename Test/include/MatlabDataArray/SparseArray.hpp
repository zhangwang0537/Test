/* Copyright 2016-2017 The MathWorks, Inc. */

#ifndef SPARSE_ARRAY_HPP_
#define SPARSE_ARRAY_HPP_

#include "matlab_data_array_defs.hpp"
#include "matlab_extdata_defs.hpp"
#include "TypedArray.hpp"
#include "Exception.hpp"

#include "detail/HelperFunctions.hpp"
#include "detail/sparse_array_interface.hpp"

#include <stdint.h>
#include <memory>
#include <utility>

namespace matlab {
    namespace data {

        namespace impl {
            class ArrayImpl;
        }

        namespace detail {
            class Access;
        }

        typedef std::pair<size_t, size_t> SparseIndex;

        /**
         * SparseArray class provides an API for accessing sparse TypedArray data.
         * It contains functionality to access only the non-zero elements
         * of the array.
         */

        template<typename T>
        class SparseArray : public Array {
          public:

            using iterator = TypedIterator<T>;
            using const_iterator = TypedIterator<typename std::add_const<T>::type>;

            static const ArrayType type = GetSparseArrayType<T>::type;
            /**
             * Copy constructor
             * @param rhs SparseArray to be copied to the current one
             * @throw none
             */
            SparseArray(const SparseArray<T> &rhs) MW_NOEXCEPT
                : Array(rhs)
            {}

            /**
             * Operator=
             * @param rhs SparseArray to be assigned to the current one
             * @throw none
             */
            SparseArray& operator=(const SparseArray<T> &rhs) MW_NOEXCEPT {
                Array::operator=(rhs);
                return *this;
            }

            /**
             * Move constructor
             * @param rhs SparseArray to be "moved" into the current one
             * @throw none
             */
            SparseArray(SparseArray&& rhs) MW_NOEXCEPT
                : Array(std::move(rhs)) {}

            /**
             * Move operator==
             * @param rhs SparseArray to be "move-assigned" into the current one
             * @throw none
             */
            SparseArray& operator=(SparseArray<T>&& rhs) MW_NOEXCEPT {
                Array::operator=(std::move(rhs));
                return *this;
            }

            /**
             * Copy construct a SparseArray from an Array
             * @param rhs Array to copy to the new Sparse array
             * @throw InvalidArrayException if type of rhs type is not sparse
             *
             */
            SparseArray(const Array& rhs) : Array(rhs) {
                bool result;
                detail::throwIfError(typed_array_is_valid_conversion(static_cast<int>(type), static_cast<int>(getType()), &result));
                if (!result) {
                    throw InvalidArrayTypeException("Can't convert the Array to this SparseArray");
                }
            }

            /**
             * Copy Operator=
             * @param rhs Array to be assigned to the current one
             * @throw InvalidArrayTypeException if type of rhs doesn't match or is not recognized
             */
            SparseArray& operator=(const Array& rhs) {
                bool result;
                detail::throwIfError(typed_array_is_valid_conversion(static_cast<int>(type), static_cast<int>(rhs.getType()), &result));
                if (!result) {
                    throw InvalidArrayTypeException("Can't convert the Array to this SparseArray");
                }
                Array::operator=(rhs);
                return *this;
            }

            /**
             * Move construct a SparseArray from an Array
             * @param rhs Array to be moved to the new Sparse array
             * @throw InvalidArrayTypeException if type of rhs is not sparse
             */
            SparseArray(Array&& rhs)
                : Array(std::move(rhs)) {
                bool result;
                detail::throwIfError(typed_array_is_valid_conversion(static_cast<int>(type), static_cast<int>(getType()), &result));
                if (!result) {
                    rhs = std::move(*this);
                    throw InvalidArrayTypeException("Can't convert the Array to this SparseArray");
                }
            }

            /**
             * Move Operator=
             * @param rhs Array to be move-assigned to the current one
             * @throw InvalidArrayTypeException if type of rhs doesn't match or is not recognized
             */
            SparseArray& operator=(Array&& rhs) {
                bool result;
                detail::throwIfError(typed_array_is_valid_conversion(static_cast<int>(type), static_cast<int>(rhs.getType()), &result));
                if (!result) {
                    throw InvalidArrayTypeException("Can't convert the Array to this SparseArray");
                }
                Array::operator=(std::move(rhs));
                return *this;
            }

            /**
             * Return an iterator to the beginning of the array
             *
             * @return iterator
             * @throw none
             */
            iterator begin() MW_NOEXCEPT {
                impl::ArrayImpl* newImpl = nullptr;
                if (array_unshare(pImpl.get(), pImpl.unique(), &newImpl)) {
                    pImpl.reset(newImpl, [](impl::ArrayImpl* ptr) {
                        array_destroy_impl(ptr);
                    });
                }
                return detail::Access::createObj<iterator>(typed_array_begin(pImpl.get(), !std::is_const<T>::value));
            }

            /**
             * Return an const_iterator to the beginning of the array
             *
             * @return const_iterator
             * @throw none
             */
            const_iterator begin() const MW_NOEXCEPT {
                return cbegin();
            }


            /**
             * Return an const_iterator to the beginning of the array
             *
             * @return const_iterator
             * @throw none
             */
            const_iterator cbegin() const MW_NOEXCEPT {
                return detail::Access::createObj<const_iterator>(typed_array_begin(pImpl.get(), false));
            }

            /**
             * Return an iterator to the end of the array
             *
             * @return iterator
             * @throw none
             */
            iterator end() MW_NOEXCEPT {
                impl::ArrayImpl* newImpl = nullptr;
                if (array_unshare(pImpl.get(), pImpl.unique(), &newImpl)) {
                    pImpl.reset(newImpl, [](impl::ArrayImpl* ptr) {
                        array_destroy_impl(ptr);
                    });
                }
                return detail::Access::createObj<iterator>(typed_array_end(pImpl.get(), !std::is_const<T>::value));
            }

            /**
             * Return an const_iterator to the end of the array
             *
             * @return const_iterator
             * @throw none
             */
            const_iterator end() const MW_NOEXCEPT {
                return cend();
            }

            /**
             * Return an const_iterator to the end of the array
             *
             * @return const_iterator
             * @throw none
             */
            const_iterator cend() const MW_NOEXCEPT {
                return detail::Access::createObj<const_iterator>(typed_array_end(pImpl.get(), false));
            }

            /**
             * This API returns the number of non-zero elements in the array. 
             * Since sparse arrays only store non-zero elements, this API effectively
             * returns the actual array size.  It is different from array dimensions
             * that specify the full array size.
             * @return Number of non-zero elements in the array
             * @throw none
             */
            size_t getNumberOfNonZeroElements() const MW_NOEXCEPT {
                size_t val;
                sparse_array_get_num_nonzero_elements(detail::Access::getImpl<impl::ArrayImpl>(*this), &val);
                return val;
            }

            /**
             * This API returns the row-column coordinates of the non-zero entry that the iterator is pointing to.
             * 
             * @param it - Iterator pointing to the current entry in the Sparse matrix
             * @return - {m,n} of the entry
             * @throw none
             */
            SparseIndex getIndex(const TypedIterator<T>& it) const MW_NOEXCEPT {
                size_t row, col;
    	    	sparse_array_get_index(detail::Access::getImpl<impl::ArrayImpl>(*this),
                                       detail::Access::getImpl<detail::IteratorImpl>(it),
                                       &row,
                                       &col);
                return SparseIndex(row, col);
    	    }

            /**
             * This API returns the row-column coordinates of the non-zero entry that the iterator is pointing to.
             *
             * @param it - Iterator pointing to the current entry in the Sparse matrix
             * @return - {m,n} of the entry
             * @throw none
             */
            SparseIndex getIndex(const TypedIterator<T const>& it)  const MW_NOEXCEPT {
                size_t row, col;
    	    	sparse_array_get_index(detail::Access::getImpl<impl::ArrayImpl>(*this),
                                       detail::Access::getImpl<detail::IteratorImpl>(it),
                                       &row,
                                       &col);
                return SparseIndex(row, col);
            }

          private:
            friend class detail::Access;

            SparseArray(matlab::data::impl::ArrayImpl *impl) MW_NOEXCEPT :
                Array(impl) {}
            
            SparseArray() = delete;
        };

    }
}

#endif
