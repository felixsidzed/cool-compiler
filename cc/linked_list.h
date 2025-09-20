#pragma once

#include <new>
#include <cstdint>
#include <type_traits>

namespace cc {
	template<typename T>
	class linked_list {
	public:
		struct element {
			T* data;
			element* next;

			element(T* ptr) : data(ptr), next(nullptr) {}
			~element() { delete data; }
		};

		struct iterator {
			element* current;

			iterator(element* ptr) : current(ptr) {}

			T& operator*() { return *current->data; }
			T* operator->() { return current->data; }

			iterator& operator++() { 
				if (current)
					current = current->next;
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return current != other.current;
			}
		};

		element* head;
		uint32_t size;

		linked_list() : head(nullptr), size(0) {}
		~linked_list() { clear(); }

		template<typename U = T, typename... Args>
		U* push(Args&&... args) {
			element* node = new element(new U(std::forward<Args>(args)...));
			node->next = head;
			head = node;
			size++;
			return static_cast<U*>(node->data);
		}

		void pop() {
			if (head) {
				element* tmp = head;
				head = head->next;
				delete tmp;
				size--;
			}
		}

		void clear() {
			while (head)
				pop();
		}

		template<typename U = T>
		U* get(uint32_t index) {
			static_assert(std::is_base_of<T, U>::value, "U must inherit from T");

			uint32_t i = 0;
			element* cur = head;
			while (cur && i < index) {
				cur = cur->next;
				i++;
			}
			if (!cur)
				return nullptr;

			return static_cast<U*>(cur->data);
		}

		iterator begin() { return iterator(head); }
		iterator end()   { return iterator(nullptr); }
	};
}
