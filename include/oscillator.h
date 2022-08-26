#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

#ifndef M_TAUl
#define M_TAUl \
6.28318530717958647692\
5286766559005768394338\
7987502116419498891846\
1563281257241799725606\
9650684234
#endif

template <typename T = double>
class DiscreetTimeBase
{
public:
    static constexpr T tau = M_TAUl;

    virtual void _trim_phase();

public:
    uint_fast32_t sampleRate;
    T radians;

    DiscreetTimeBase();
    DiscreetTimeBase(const DiscreetTimeBase& obj);
    ~DiscreetTimeBase();

    virtual void set_sample_rate(uint_fast32_t samplerate);

    virtual T get_phase();
};

template <typename T = double>
class Sinusoid : virtual public DiscreetTimeBase<T>
{
protected:
    T _step;

    virtual T _get_sample();

public:
    size_t numHarmonics;

    Sinusoid();
    Sinusoid(const Sinusoid& obj);

    virtual void get(T* buff, const size_t numSamples);
};

template <typename T = double>
class MultiHarmonicWave : virtual public Sinusoid<T>
{
protected:
    size_t _endLoop;

    T _get_sample() override;

public:
    MultiHarmonicWave();
    MultiHarmonicWave(const MultiHarmonicWave& obj);

    virtual void set_num_harmonics(size_t num);
};

template <typename T = double>
class OscillatorBase : virtual public Sinusoid<T>
{
protected:
    T _frequency, _samplesPerCycle;

    virtual void _set();

public:
    OscillatorBase();
    OscillatorBase(const OscillatorBase& obj);
    
    void set_sample_rate(uint_fast32_t samplerate) override;
    virtual void set_frequency(T freq);

    virtual bool is_set();

    void get(T* buff, const size_t numSamples) override;
    virtual void get(std::vector<T>* buff);
};

template <typename T = double>
class MultiHarmonicOscillator :
virtual public MultiHarmonicWave<T>,
virtual public OscillatorBase<T>
{
protected:
    T _get_sample() override;

public:
    MultiHarmonicOscillator();
    MultiHarmonicOscillator(const MultiHarmonicOscillator& obj);
};

#endif
