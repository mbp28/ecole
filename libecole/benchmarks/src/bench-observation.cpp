#include <benchmark/benchmark.h>
#include <scip/scip.h>

#include "ecole/observation/khalil-2016.hpp"
#include "ecole/observation/nodebipartite.hpp"

#include "utils.hpp"

using namespace ecole;

template <typename ObsFunc> auto benchmark_observation(benchmark::State& state, ObsFunc&& func_to_bench) {
	auto n_nodes = ecole::scip::long_int{0};

	for (auto _ : state) {
		state.PauseTiming();
		auto model = get_model();

		model.solve_iter();

		state.ResumeTiming();
		func_to_bench.before_reset(model);
		state.PauseTiming();

		while (!model.solve_iter_is_done()) {
			model.solve_iter_branch(model.lp_branch_cands()[0]);

			state.ResumeTiming();
			benchmark::DoNotOptimize(func_to_bench.extract(model, false));
			state.PauseTiming();
		}

		n_nodes += SCIPgetNTotalNodes(model.get_scip_ptr());
		state.ResumeTiming();
	}

	using benchmark::Counter;
	state.counters["Nodes"] = Counter{static_cast<double>(n_nodes), Counter::kAvgIterations};
}

auto obs_NodeBipartite(benchmark::State& state) {
	benchmark_observation(state, observation::NodeBipartite{});
}
BENCHMARK(obs_NodeBipartite)  // NOLINT(cert-err58-cpp)
	->MeasureProcessCPUTime()
	->UseRealTime()
	->Unit(benchmark::kMillisecond);

auto obs_Khalil2016(benchmark::State& state) {
	benchmark_observation(state, observation::Khalil2016{});
}
BENCHMARK(obs_Khalil2016)  // NOLINT(cert-err58-cpp)
	->MeasureProcessCPUTime()
	->UseRealTime()
	->Unit(benchmark::kMillisecond);
