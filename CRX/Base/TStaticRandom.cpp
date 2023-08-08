#include <CRX/Base/TStaticRandom.h>
#include <CRX/Base/TMiscUtils.h>
#include <chrono>
#include <CRX/Base/TDebug.h>
#include <math.h>

namespace crx {

TStaticRandom::TStaticRandom() {
	UpdateSeed();
}

double TStaticRandom::RandDouble() {
	return double(DoRand())/double(RandMax());
}

double TStaticRandom::RandDoubleRange(double min, double max) {
	return RandDouble()*(max -min)+min;
}

uint32_t TStaticRandom::Rand() { return DoRand(); }

uint32_t TStaticRandom::RandRange(uint32_t min, uint32_t max) {
	return Rand()%(max-min+1)+min;
}

bool TStaticRandom::RandBool() { return bool( Rand()%2 ); }

uint32_t TStaticRandom::RandMax() { return std::numeric_limits<int32_t>::max(); }

int TStaticRandom::SelectOne(const double *array, int size) {
	auto v = RandDouble();

	double sum = 0;
	for(int i=0;i<size;i++) {
		sum += array[i];
		if(sum>v) return i;
	}
	return size-1;
}

int TStaticRandom::SelectOne(const std::vector<double>& vv) {
	return SelectOne(vv.data(), (int)vv.size());
}

unsigned TStaticRandom::SelectOneForVector(size_t vs) {
	vs--;
	return crx::uround(RandDouble()*double(vs));
}

uint32_t TStaticRandom::DoRand() {
	if(m_uPrevSeed!=Seed) {
		UpdateRandomDeviceToMatchSeed();
	}
	return m_xUniformDist(m_xRng);
	// uint32_t hi, lo, x;
	// if(Seed == 0) Seed = 123459876;
	// hi = Seed / 127773;
	// lo = Seed % 127773;
	// x = 16807 * lo - 2836 * hi;
	// if (x < 0) x += 0x7fffffff;
	// return ((Seed = x) % (RandMax() + 1));
}

void TStaticRandom::UpdateSeed() {
	if(m_iLockSeedUpdate) return;
	Seed = rand();
	UpdateRandomDeviceToMatchSeed();
}

void TStaticRandom::UpdateRandomDeviceToMatchSeed() {
	std::seed_seq seed{Seed};
	m_xRng = std::mt19937(seed);
	m_uPrevSeed = Seed;
}

void TStaticRandom::LockSeedUpdate(bool lock) {
	m_iLockSeedUpdate += lock?+1:-1;
}

} /* namespace crx */
