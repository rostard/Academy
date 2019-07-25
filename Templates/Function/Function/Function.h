#pragma once

template<typename R>
class function;

template<typename R, typename... Args>
class function<R(Args...)>
{
public:

	function() : func(nullptr) {}

	template<typename F>
	function(const F& arg) : func(new FuncModel<F>(arg)) {}
	~function() { if (func)delete func; }

	R operator()(Args...args) {
		return func->call(args...);
	}

	operator bool() {
		return func != nullptr;
	}
private:

	class FuncConcept {
	public:
		virtual R call(Args... args) = 0;
	};

	template<typename F>
	class FuncModel : public FuncConcept {
	public:
		FuncModel(const F& arg) : func(arg) {}
		R call(Args... args) override {
			return func(args...);
		}

	private:
		F func;
	};

	FuncConcept* func;
};