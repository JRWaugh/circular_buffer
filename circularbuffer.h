/*
 * circularbuffer.h
 *
 *  Created on: 9 Feb 2020
 *      Author: Joshua
 */
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <utility>

#define RETURN_POP 0
/* Usage example */
/* circularbuffer<uint8_t, 4> cb;  This constructs a circular buffer of type uint8_t with a length of 4.
 * for(unsigned int i = 0; i < 6; ++i)
 *     cb.push_back(i);
 * Array as the loop iterates:
 * { 0 }, { 0, 1 }, { 0, 1, 2 }, { 0, 1, 2, 3 }, { 1, 2, 3, 4 }, { 2, 3, 4, 5 }
 * cb.push_front(7); // { 7, 2, 3, 4 }
 */
template <typename T, unsigned int N>
class circularbuffer {

public:
	constexpr circularbuffer() : buffer{ 0 }, begin{ buffer }, size_{ 0 }  {}

	template <typename... Ts>
	circularbuffer(Ts const &... args) : buffer{ std::forward<T>(args)... }, begin{ buffer }, size_{ sizeof...(Ts) } {}

	constexpr unsigned int size() const { return size_; }

	/* Elements are indexed from oldest to newest starting from 0, going to (size - 1) */
	T& operator[](unsigned int const index) { return buffer[(begin - buffer + index) % size_]; }
	const T& operator[](unsigned int const index) const { return buffer[(begin - buffer + index) % size_]; }

	/* Returns a reference to the oldest element in the buffer */
	T& front() { return *begin; }
	const T& front() const { return *begin; }

	/* Returns a reference to the newest element in the buffer */
	T& back() { return operator[](size_ - 1); }
	const T& back() const { return operator[](size_ - 1); }

	/* Pushes new data into the back of the buffer and evicts the oldest element if the buffer is full */
	void push_back(T const t) {
		if (size_ < N)
			operator[](size_++) = t;
		else {
			*begin = t;

			if (++begin == (buffer + N))
				begin = buffer;
		}
	}

	/* Pushes new data into the front of the buffer and evicts what was the newest element if the buffer is full */
	void push_front(T const t) {
		if (--begin < buffer)
			begin = buffer + N - 1;

		*begin = t;

		if (size_ < N)
			++size_;
	}

	constexpr void pop_back() {
		if (size_ > 0)
			--size_;
	}

	constexpr void pop_front() {
		if (size_ > 0) {
			--size_;
			if (++begin == (buffer + N))
				begin = buffer;
		}
	}

private:
	T buffer[N];
	T* begin; /* Points to the oldest datum, although that gets muddied if you use push_front() */
	unsigned int size_;
};

#endif /* CIRCULARBUFFER_H_ */
