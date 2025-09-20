#pragma once

#include <cstring>

namespace cc {
	class string {
	public:
		char* data;
	
		string() : data(new char[1]) {
			data[0] = '\0';
		}

		string(const char* s) {
			if (!s) {
				data = new char[1];
				data[0] = '\0';
			} else {
				size_t len = strlen(s) + 1;
				data = new char[len];
				strcpy_s(data, len, s);
			}
		}

		string(const string& other) {
			size_t len = strlen(other.data) + 1;
			data = new char[len];
			strcpy_s(data, len, other.data);
		}

		string(string&& other) noexcept : data(other.data) {
			other.data = nullptr;
		}

		string& operator=(const string& other) {
			if (this != &other) {
				delete[] data;
				size_t len = strlen(other.data) + 1;
				data = new char[len];
				strcpy_s(data, len, other.data);
			}
			return *this;
		}

		string& operator=(string&& other) noexcept {
			if (this != &other) {
				delete[] data;
				data = other.data;
				other.data = nullptr;
			}
			return *this;
		}

		~string() {
			delete[] data;
		}

		operator const char*() const {
			return data;
		}

		size_t size() const {
			return strlen(data);
		}

		bool empty() const { return size() == 0; };

		char& operator[](size_t index) {
			if (index >= size())
				return data[0];
			return data[index];
		}

		const char& operator[](size_t index) const {
			if (index >= size())
				return data[0];
			return data[index];
		}

		string operator+(const string& rhs) const {
			size_t len = size() + rhs.size() + 1;
			char* newData = new char[len];
			strcpy_s(newData, len, data);
			strcat_s(newData, len, rhs.data);

			string result;
			delete[] result.data;
			result.data = newData;
			return result;
		}

		string& operator+=(const string& rhs) {
			size_t len = size() + rhs.size() + 1;
			char* newData = new char[len];
			strcpy_s(newData, len, data);
			strcat_s(newData, len, rhs.data);

			delete[] data;
			data = newData;
			return *this;
		}

		friend string operator+(const char* lhs, const string& rhs) {
			if (!lhs) lhs = "";
			size_t len = strlen(lhs) + rhs.size() + 1;
			char* newData = new char[len];
			strcpy_s(newData, len, lhs);
			strcat_s(newData, len, rhs.data);

			string result;
			delete[] result.data;
			result.data = newData;
			return result;
		}

		bool operator==(const string& other) {
			if (data == nullptr || other.data == nullptr)
				return data == other.data;
			return !strcmp(data, other.data);
		}

		bool operator==(const char* other) {
			if (data == nullptr || other == nullptr)
				return data == other;
			return !strcmp(data, other);
		}
	};
}