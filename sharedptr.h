#pragma once

template <typename T>
class sharedptr {
	class CenterBlock {
		int count;
	public:
		CenterBlock() {
			count = 1;
		}
		int get_count() {
			return count;
		}
		void increment() {
			++count;
		}
		int decrement() {
			return --count;
		}
	};
private:
	CenterBlock* counter = nullptr;
	T* s_ptr = nullptr;
public:
	sharedptr() = default;
	explicit sharedptr(T* obj) {
		counter = new CenterBlock();
		s_ptr = obj;
	}
	sharedptr(const sharedptr& obj) {
		this->counter = obj.counter;
		this->counter->increment();
		this->s_ptr = obj.s_ptr;
	}
	sharedptr& operator=(const sharedptr& obj) {
		if (this != &obj) {
			if (s_ptr != nullptr) {
				if (counter->decrement() == 0) {
					delete counter;
					delete s_ptr;
				}
			}
			counter = obj.counter;
			counter->increment();
			s_ptr = obj.s_ptr;
		}
		return *this;
	}
	sharedptr(sharedptr&& obj) noexcept {
		this->counter = obj.counter;
		this->s_ptr = obj.s_ptr;
		obj.counter = nullptr;
		obj.s_ptr = nullptr;
	}
	sharedptr& operator=(sharedptr&& obj) noexcept{
		if (this != &obj) {
			if (s_ptr != nullptr) {
				if (counter->decrement() == 0) {
					delete counter;
					delete s_ptr;
				}
			}
			counter = obj.counter;
			s_ptr = obj.s_ptr;
			obj.counter = nullptr;
			obj.s_ptr = nullptr;
		}
		return *this;
	}
	~sharedptr() {
		if (s_ptr != nullptr && counter->decrement() == 0) {
			delete counter;
			delete s_ptr;
			counter = nullptr;
			s_ptr = nullptr;
		}
	}
public:
	int use_count() {
		if (counter == nullptr) { return 0; }
		return counter->get_count();
	}
	T* get() {
		return s_ptr;
	}
	T* operator ->() {
		return s_ptr;
	}
	T operator *() {
		return * s_ptr;
	}
	void reset(T* obj = nullptr) {
		if (s_ptr != nullptr && counter->get_count() == 1) {
			delete counter;
			delete s_ptr;
			counter = nullptr;
			s_ptr = nullptr;
		}
		else {
			s_ptr = obj;
			counter = new CenterBlock();
		}
	}

};