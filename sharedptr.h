#pragma once


template <typename T>
class sharedptr {
	class CenterBlock;
	CenterBlock* cb = nullptr;
private:
	class CenterBlock {
		int count;
		T* m_ptr = nullptr;
	public:
		CenterBlock(T* ptr) : m_ptr(ptr) {
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
		void set_ptr(T* ptr) {
			m_ptr = ptr;
		}
		T* get_ptr() {
			if (m_ptr == nullptr) {
				return nullptr;
			}
			return m_ptr;
		}
		void deleteptr() {
			if (--count == 0) {
				delete m_ptr;
			}
		}
		~CenterBlock() {
			deleteptr();
		}
	};
public:
	sharedptr() = default;
	explicit sharedptr(T* obj) {
		cb = new CenterBlock(obj);
	}
	sharedptr(const sharedptr& obj) {
		this->cb = obj.cb;
		this->cb->increment();
		this->cb->set_ptr(obj.cb->get_ptr());
	}
	sharedptr& operator=(const sharedptr& obj) {
		if (this != &obj) {
			if (cb != nullptr)
			{
				if (cb->get_ptr() != nullptr) {
					cb->deleteptr();
					delete this->cb;
				}
			}
			this->cb = obj.cb;
			this->cb->increment();
			this->cb->set_ptr(obj.cb->get_ptr());
		}
		return *this;
	}
	sharedptr(sharedptr&& obj) noexcept {
		this->cb = obj.cb;
		this->cb->set_ptr(obj.cb->get_ptr());
		obj.cb->set_ptr(nullptr);
		obj.cb = nullptr;
	}
	sharedptr& operator=(sharedptr&& obj) noexcept {
		if (this != &obj) {
			if (cb != nullptr)
			{
				if (cb->get_ptr() != nullptr) {
					cb->deleteptr();
				}
			}
			this->cb = obj.cb;
			this->cb->set_ptr(obj.cb->get_ptr());
			obj.cb->set_ptr(nullptr);
			obj.cb = nullptr;
		}
		return *this;
	}
	~sharedptr() {
		if (cb != nullptr)
		{
			if (cb->get_ptr() != nullptr) {
				cb->deleteptr();
				cb->set_ptr(nullptr);
				cb = nullptr;
			}
		}
	}
public:
	int use_count() {
		if (cb == nullptr) { return 0; }
		return cb->get_count();
	}
	T* get() {
		if (cb == nullptr) { return nullptr; }
		return cb->get_ptr();
	}
	T* operator ->() {
		if (cb == nullptr) { return nullptr; }
		return cb->get_ptr();
	}
	T operator *() {
		if (cb == nullptr) { return INT_MIN; }
		return *(cb->get_ptr());
	}
	void reset(T* obj = nullptr) {
		if (cb != nullptr) {
			if (cb->get_ptr() != nullptr) {
				cb->deleteptr();
				cb->set_ptr(nullptr);
				cb = nullptr;
			}
			else {
				cb->set_ptr(obj);
				cb = new CenterBlock();
			}
		}
	}

};