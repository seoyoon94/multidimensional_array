#ifndef ARRAY_H
#define ARRAY_H

#include <tuple>
#include <stdexcept>
#include <string>

namespace cs540{

	template <typename U>
	constexpr int numDimensions(const U &t){
		return t;
	}

	template <typename U, typename... N>
	constexpr int numDimensions(const U &t, const N &... dims){
		return t + numDimensions(dims...);
	}

	class OutOfRange : public std::out_of_range{
		public:
			OutOfRange(const std::string &error)
				: std::out_of_range(error){
			}
	};

	template<typename T, size_t... Dims>
	class Array{};
	
	//Base case with only 1 dimension array
	template <typename T, size_t Dim>
	class Array<T, Dim>{
		public:
			typedef T ValueType;
			Array(){
				static_assert(Dim > 0, "The array cannot have a dimension of size 0.");
			}
			
			Array(const Array &rhs){
				static_assert(Dim > 0, "The array cannot have a dimension of size 0.");
				for(int i = 0; i < (int)Dim; i++){
					array[i] = rhs[i];
				}
			}
			
			template <typename U>
			Array(const Array<U, Dim> &rhs){
				static_assert(Dim > 0, "The array cannot have a dimension of size 0.");
				for(int i = 0; i < (int)Dim; i++){
					array[i] = rhs[i];
				}
				
			}

			Array &operator=(const Array &rhs){
				if(this != &rhs){
					for(int i = 0; i < (int)Dim; i++){
						array[i] = rhs[i];
					}
				}
				return *this;
			}

			template <typename U>
			Array &operator=(const Array<U, Dim> &rhs){
				for(int i = 0; i < (int)Dim; i++){
					array[i] = rhs[i];
				}
				return *this;
			}

			T &operator[](int n){
				if(n < 0 || n >= (int)Dim){
					throw OutOfRange("The index is not accessible. The index is out of range.");
				}
				return array[n];
			}
			
			const T &operator[](int n) const{
				if(n < 0 || n >= (int)Dim){
					throw OutOfRange("The index is not accessible. The index is out of range.");
				}
				return array[n];
			}

			template <size_t N>
			class Access{
				public:
					Access()
						:index(0){
					}
					
					const size_t getIndex() const{
						return index;
					}
					
					void setIndex(int index){
						this->index = index;
					}

					void setMax(){
						index = N - 1;
					}
					
					bool incrementFirstMajor(){
						if(++index == N){
							return true;
						}
						return false;
					}
				
					void incrementLastMajor(){
						++index;
					}

					friend bool operator==(const Access<N> &access1, const Access<N> &access2){
						return access1.index == access2.index;
					}
				private:
					size_t index;
			};

			class FirstDimensionMajorIterator{
				public:
					FirstDimensionMajorIterator()
						:ref(nullptr){
					}
					
					FirstDimensionMajorIterator(Array<T, Dim> *ref, Access<Dim> accessor)
						: ref(ref), accessor(accessor){
					}
					
					FirstDimensionMajorIterator &operator++(){
						accessor.incrementFirstMajor();
						return *this;
					}
					
					FirstDimensionMajorIterator operator++(int){
						FirstDimensionMajorIterator temp = *this;
						accessor.incrementFirstMajor();
						return temp;
					}

					T &operator*() const{
						int index = accessor.getIndex();
						return (*ref)[index];
					}

					friend bool operator==(const FirstDimensionMajorIterator &f1, const FirstDimensionMajorIterator &f2){
						return f1.accessor.getIndex() == f2.accessor.getIndex();
					}

					friend bool operator!=(const FirstDimensionMajorIterator &f1, const FirstDimensionMajorIterator &f2){
						return !(f1 == f2);
					}
				private:
					Array<T, Dim> *ref;
					Access<Dim> accessor;
			};	

			class LastDimensionMajorIterator{
				public:
					LastDimensionMajorIterator()
						:ref(nullptr){
					}
					
					LastDimensionMajorIterator(Array<T, Dim> *ref, Access<Dim> accessor)
						: ref(ref), accessor(accessor){
					}
					
					LastDimensionMajorIterator &operator++(){
						accessor.incrementLastMajor();
						return *this;
					}
					
					LastDimensionMajorIterator operator++(int){
						LastDimensionMajorIterator temp = *this;
						accessor.incrementLastMajor();
						return temp;
					}

					T &operator*() const{
						int index = accessor.getIndex();
						return (*ref)[index];

					}

					friend bool operator==(const LastDimensionMajorIterator &f1, const LastDimensionMajorIterator &f2){
						return f1.accessor.getIndex() == f2.accessor.getIndex();
					}

					friend bool operator!=(const LastDimensionMajorIterator &f1, const LastDimensionMajorIterator &f2){
						return !(f1 == f2);
					}
				private:
					Array<T, Dim> *ref;
					Access<Dim> accessor;
			};	
			
			FirstDimensionMajorIterator fmbegin(){
				Access<Dim> accessor;
				return FirstDimensionMajorIterator(this, accessor);
			}

			FirstDimensionMajorIterator fmend(){
				Access<Dim> accessor;
				accessor.setMax();
				accessor.incrementFirstMajor();
				return FirstDimensionMajorIterator(this, accessor);
			}

			LastDimensionMajorIterator lmbegin(){
				Access<Dim> accessor;
				return LastDimensionMajorIterator(this, accessor);
			}

			LastDimensionMajorIterator lmend(){
				Access<Dim> accessor;
				accessor.setMax();
				accessor.incrementLastMajor();
				return LastDimensionMajorIterator(this, accessor);
			}
		private:
			T array[Dim];
	};

	template <typename T, size_t Dim, size_t... Dims>
	class Array<T, Dim, Dims...>{
		public:
			typedef T ValueType;
			Array(){
				static_assert(numDimensions(Dims...), "The array cannot have a dimension of size 0.");
			}
			
			Array(const Array &rhs){
				static_assert(numDimensions(Dims...), "The array cannot have a dimension of size 0.");
				for(int i = 0; i < (int)Dim; i++){
					array[i] = rhs[i];
				}
			}
			
			template <typename U>
			Array(const Array<U, Dim, Dims...> &rhs){
				static_assert(numDimensions(Dims...), "The array cannot have a dimension of size 0.");
				for(int i = 0; i < (int)Dim; i++){
					array[i] = rhs[i];
				}
				
			}

			Array &operator=(const Array &rhs){
				if(this != &rhs){
					for(int i = 0; i < (int)Dim; i++){
						array[i] = rhs[i];
					}
				}
				return *this;
			}

			template <typename U>
			Array &operator=(const Array<U, Dim, Dims...> &rhs){
				for(int i = 0; i < (int)Dim; i++){
					array[i] = rhs[i];
				}
				return *this;
			}

			Array<T, Dims...> &operator[](int n){
				if(n < 0 || n >= (int)Dim){
					throw OutOfRange("The index is not accessible. The index is out of range.");
				}
				return array[n];
			}
			
			const Array<T, Dims...> &operator[](int n) const{
				if(n < 0 || n >= (int)Dim){
					throw OutOfRange("The index is not accessible. The index is out of range.");
				}
				return array[n];
			}

			template <size_t N, size_t... M>
			class Access{
				public:
					Access()
						:index(0){
					}
					
					const auto getIndex() const{
						return index;
					}

					const auto getNested() const{
						return nestedIndex;
					}
					
					void setIndex(size_t index){
						this->index = index;
					}

					void setMax(){
						index = N - 1;
						nestedIndex.setMax();
					}
					
					bool incrementFirstMajor(){
						if(nestedIndex.incrementFirstMajor() == false){
							return false;
						}
						nestedIndex.setIndex(0);
						if(++index == N){
							return true;
						}
						return false;
					}

					void incrementLastMajor(){
						if(++index == N){
							index = 0;
							nestedIndex.incrementLastMajor();
						}
					}

					friend bool operator==(const Access<N, M...> &access1, const Access<N, M...> &access2){
						if(access1.index != access2.index){
							return false;
						}
						return access1.nestedIndex == access2.nestedIndex;
					}
				private:
					size_t index;
					typename Array<T, M...>::template Access<M...> nestedIndex;
			};

			class FirstDimensionMajorIterator{
				public:
					FirstDimensionMajorIterator()
						:ref(nullptr){
					}
					
					FirstDimensionMajorIterator(Array<T, Dim, Dims...> *ref, Access<Dim, Dims...> accessor)
						: ref(ref), accessor(accessor){
					}
					
					FirstDimensionMajorIterator &operator++(){
						accessor.incrementFirstMajor();
						return *this;
					}
					
					FirstDimensionMajorIterator operator++(int){
						FirstDimensionMajorIterator temp = *this;
						accessor.incrementFirstMajor();
						return temp;
					}

					T &operator*() const{
						Array<T, Dims...> *ar = &(ref->array[accessor.getIndex()]);
						typename Array<T, Dims...>::FirstDimensionMajorIterator retVal(ar, accessor.getNested());
						return *retVal;
					}

					friend bool operator==(const FirstDimensionMajorIterator &f1, const FirstDimensionMajorIterator &f2){
						return f1.accessor == f2.accessor;
					}

					friend bool operator!=(const FirstDimensionMajorIterator &f1, const FirstDimensionMajorIterator &f2){
						return !(f1 == f2);
					}
				private:
					Array<T, Dim, Dims...> *ref;
					Access<Dim, Dims...> accessor;
			};	

			class LastDimensionMajorIterator{
				public:
					LastDimensionMajorIterator()
						:ref(nullptr){
					}
					
					LastDimensionMajorIterator(Array<T, Dim, Dims...> *ref, Access<Dim, Dims...> accessor)
						: ref(ref), accessor(accessor){
					}
					
					LastDimensionMajorIterator &operator++(){
						accessor.incrementLastMajor();
						return *this;
					}
					
					LastDimensionMajorIterator operator++(int){
						LastDimensionMajorIterator temp = *this;
						accessor.incrementLastMajor();
						return temp;
					}

					T &operator*() const{
						Array<T, Dims...> *ar = &(ref->array[accessor.getIndex()]);
						typename Array<T, Dims...>::LastDimensionMajorIterator retVal(ar, accessor.getNested());
						return *retVal;
					}

					friend bool operator==(const LastDimensionMajorIterator &f1, const LastDimensionMajorIterator &f2){
						return f1.accessor == f2.accessor;
					}

					friend bool operator!=(const LastDimensionMajorIterator &f1, const LastDimensionMajorIterator &f2){
						return !(f1 == f2);
					}
				private:
					Array<T, Dim, Dims...> *ref;
					Access<Dim, Dims...> accessor;
			};	
			
			FirstDimensionMajorIterator fmbegin(){
				Access<Dim, Dims...> accessor;
				return FirstDimensionMajorIterator(this, accessor);
			}

			FirstDimensionMajorIterator fmend(){
				Access<Dim, Dims...> accessor;
				accessor.setMax();
				accessor.incrementFirstMajor();
				return FirstDimensionMajorIterator(this, accessor);
			}

			LastDimensionMajorIterator lmbegin(){
				Access<Dim, Dims...> accessor;
				return LastDimensionMajorIterator(this, accessor);
			}

			LastDimensionMajorIterator lmend(){
				Access<Dim, Dims...> accessor;
				accessor.setMax();
				accessor.incrementLastMajor();
				return LastDimensionMajorIterator(this, accessor);
			}
		private:
			Array<T, Dims...> array[Dim];
	};

}

#endif //ARRAY_H
