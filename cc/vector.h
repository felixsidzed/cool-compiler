#pragma once

#include <new>
#include <cstdint>
#include <initializer_list>

#define CC_VECTOR_CHUNK_SIZE 4

namespace cc {
	template<typename T>
	class vector {
	public:
		T* data;
		uint32_t size;
		uint32_t capacity;
		
		vector(vector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity) {
			other.data = nullptr;
			other.size = 0;
			other.capacity = 0;
		}
		vector(uint32_t size = CC_VECTOR_CHUNK_SIZE) : size(0), capacity(size), data((T*)::operator new(size * sizeof(T))) {}

		template<typename U = T, typename = typename std::enable_if<std::is_copy_constructible<U>::value>::type>
		vector(const std::initializer_list<T>& init) :
			size((uint32_t)init.size()), capacity((uint32_t)init.size()), data((T*)::operator new(init.size() * sizeof(T)))
		{
			uint32_t i = 0;
			for (const T& item : init) {
				new (&data[i]) T(item);
				i++;
			}
		}

		vector(const T* otherData, uint32_t otherSize)
			: size(otherSize), capacity(otherSize), data((T*)::operator new(otherSize * sizeof(T)))
		{
			for (uint32_t i = 0; i < otherSize; i++)
				new (&data[i]) T(otherData[i]);
		}

		~vector() {
			clear();
			::operator delete(data, capacity * sizeof(T));
		}

		template<typename U = T>
		typename std::enable_if<std::is_copy_constructible<U>::value>::type
		push(const T& value) {
			resize(size + 1);
			new (&data[size]) T(value);
			size++;
		}

		void push(T&& value) {
			resize(size + 1);
			new (&data[size]) T(std::move(value));
			size++;
		}

		template <typename... Args>
		T* emplace(Args&&... args) {
			resize(size + 1);
			T* place = &data[size];
			new (place) T(std::forward<Args>(args)...);
			size++;
			return place;
		}

		void pop() {
			if (size > 0) {
				size--;
				data[size].~T();
			}
		}

		void clear() {
			for (uint32_t i = 0; i < size; i++)
				data[i].~T();
			size = 0;
		}

		bool empty() const { return size == 0; }

		T* find(const T& value) {
			for (uint32_t i = 0; i < size; i++) {
				if (data[i] == value)
					return &data[i];
			}
			return nullptr;
		}

		const T* find(const T& value) const {
			for (uint32_t i = 0; i < size; i++) {
				if (data[i] == value)
					return &data[i];
			}
			return nullptr;
		}

		T* begin() { return data; }
		T* end() { return data + size; }
		const T* begin() const { return data; }
		const T* end() const { return data + size; }

		T& operator[](uint32_t i) { return data[i]; }
		const T& operator[](uint32_t i) const { return data[i]; }

		vector& operator=(vector&& other) noexcept {
			if (this != &other) {
				clear();
				::operator delete(data, capacity * sizeof(T));

				data = other.data;
				size = other.size;
				capacity = other.capacity;

				other.data = nullptr;
				other.size = 0;
				other.capacity = 0;
			}
			return *this;
		}

	private:
		void resize(uint32_t ns) {
			if (ns <= capacity)
				return;

			uint32_t nc = capacity + CC_VECTOR_CHUNK_SIZE;
			T* newData = (T*)::operator new(nc * sizeof(T));
			for (uint32_t i = 0; i < size; ++i) {
				new (&newData[i]) T(std::move(data[i]));
				data[i].~T();
			}
			::operator delete(data, capacity * sizeof(T));
			
			capacity = nc;
			data = newData;
		}
	};
}
