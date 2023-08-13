#include <benchmark/benchmark.h>
#include <proxy.h>

#include <memory>

struct Base{
    virtual int Virtual() const = 0;
    virtual ~Base() = default;
};

struct Derived : Base{
    int Proxy() const { return val_;}
    int Virtual() const override { return val_; }

private:
    int val_  = 42;
};

struct CallViaProxy : pro::dispatch<int()>{
template <class T> int operator()(T const & self){ return self.Proxy(); }
};
struct Facade : pro::facade<CallViaProxy> {};

static void BM_ViaVirtual(benchmark::State& state) {
  std::unique_ptr<Base> ptr = std::make_unique<Derived>();
  for (auto _ : state) {
    benchmark::DoNotOptimize(ptr);
    auto val = ptr->Virtual();
    benchmark::DoNotOptimize(val);
  }
}
BENCHMARK(BM_ViaVirtual);

static void BM_ViaProxy(benchmark::State& state) {
  Derived d;
  auto proxy = pro::make_proxy<Facade>(d);
  for (auto _ : state) {
    benchmark::DoNotOptimize(proxy);
    auto val = proxy.invoke<CallViaProxy>();
    benchmark::DoNotOptimize(val);
  }
}
BENCHMARK(BM_ViaProxy);
