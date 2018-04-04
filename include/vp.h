
#include <memory>
#include <vector>
#include <string>
#include <iostream>

template <typename ReturnValue, typename... Args>
class naive_function {};

template <typename ReturnValue, typename... Args>
class naive_function<ReturnValue(Args...)> {
public:
	template <typename T>
	naive_function& operator=(T t) {
		callable_ = std::unique_ptr<CallableT<T>>(new CallableT<T>(t));
		return *this;
	}

	ReturnValue operator()(Args... args) const {
		assert(callable_);
		return callable_->Invoke(args...);
	}

private:
  class ICallable {
  public:
    virtual ~ICallable() = default;
    virtual ReturnValue Invoke(Args...) = 0;
  };
  template <typename T>
  class CallableT : public ICallable {
  public:
    CallableT(const T& t)
      : t_(t) {
    }

    ~CallableT() override = default;

    ReturnValue Invoke(Args... args) override {
      return t_(args...);
    }

  private:
    T t_;
  };

	std::unique_ptr<ICallable> callable_;
};

class object_t
{
public:
  template <typename T>
  object_t(T t)
    : m(std::make_shared<model<T>>(std::move(t)))
  {}

  void behaviour() const
  {
    m->behaviour();
  }

private:
  struct concept
  {
    virtual ~concept() = default;
    virtual void behaviour() const = 0;
  };

  template <typename T>
  struct model final : concept
  {
    model(T t) : data(std::move(t))
    {}

    void behaviour() const override
    {
      std::cout << data;
    }

    T data;
  };

  std::shared_ptr<const concept> m;
};




