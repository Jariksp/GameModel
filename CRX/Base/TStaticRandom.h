#ifndef CRX_TSTATICRANDOM_H_
#define CRX_TSTATICRANDOM_H_

#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstddef>
#include <random>

namespace crx {

class TStaticRandom {
	public:
	TStaticRandom();

	public:
	double RandDouble();//[0;1]
	double RandDoubleRange(double min, double max); // [min, max]

	uint32_t Rand();
	uint32_t RandRange(uint32_t min, uint32_t max); // [min, max]

	bool RandBool();

	uint32_t RandMax();

	public:
	// выбирает ячейку, считает, что в каждой ячейке лежит её вес, а суммарный вес все ячеек равен 1
	int SelectOne(const double* array, int size);
	int SelectOne(const std::vector<double>& array);
	unsigned SelectOneForVector(size_t vectorSize);

	template<typename RandomAccessIterator>
	void RandomShuffle(RandomAccessIterator first, RandomAccessIterator last) {
		if (first == last) return;

		for (RandomAccessIterator i = first + 1; i != last; ++i) {
			RandomAccessIterator j = first +  (Rand()%((i - first) + 1));
			if (i != j) std::iter_swap(i, j);
		}
	}

	public:
	void UpdateSeed();
	void LockSeedUpdate(bool lock=true);

	public:
	uint32_t Seed = 0;

	protected:
	uint32_t m_uPrevSeed = 0;

	protected:
	std::mt19937 m_xRng = std::mt19937(Seed);

	protected:
	using UniIntDist = std::uniform_int_distribution<std::mt19937::result_type>;
	UniIntDist m_xUniformDist = UniIntDist(0, RandMax());

	protected:
	virtual uint32_t DoRand();

	protected:
	void UpdateRandomDeviceToMatchSeed();

	protected:
	int m_iLockSeedUpdate = 0;
};

} /* namespace crx */

#endif /* CRX_TSTATICRANDOM_H_ */
