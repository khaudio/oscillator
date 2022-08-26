﻿#include "oscillator.h"

template <typename T>
DiscreetTimeBase<T>::DiscreetTimeBase() :
sampleRate(0),
radians(0)
{
}

template <typename T>
DiscreetTimeBase<T>::~DiscreetTimeBase()
{
}

template <typename T>
inline void DiscreetTimeBase<T>::_trim_phase()
{
    /* Trims phase to avoid overflows */
    this->radians -= (
            (this->radians >= DiscreetTimeBase<T>::tau)
            ? DiscreetTimeBase<T>::tau : 0
        );
}

template <typename T>
void DiscreetTimeBase<T>::set_sample_rate(int_fast32_t samplerate)
{
    this->sampleRate = samplerate;
}

template <typename T>
inline T DiscreetTimeBase<T>::get_phase()
{
    /* Returns phase in range 0.0 - 1.0 */
    return this->radians / DiscreetTimeBase<T>::tau;
}

template <typename T>
Sinusoid<T>::Sinusoid() :
DiscreetTimeBase<T>(),
_step(0),
numHarmonics(1)
{
}

template <typename T>
inline T Sinusoid<T>::_get_sample()
{
    T sample = std::sin(this->radians);
    this->radians += this->_step;
    this->_trim_phase();
    return sample;
}

template <typename T>
inline void Sinusoid<T>::get(T* buff, const size_t numSamples)
{
    /* One sinusoidal cycle from 0 to numSamples
    frequency = (sampleRate / numSamples) */

    /* Phase change toward tau per sample */
    this->_step = DiscreetTimeBase<T>::tau / numSamples / this->numHarmonics;

    for (size_t i(0); i < numSamples; ++i)
    {
        buff[i] = this->_get_sample();
    }
}

template <typename T>
MultiHarmonicWave<T>::MultiHarmonicWave()
{
    set_num_harmonics(4);
}

template <typename T>
void MultiHarmonicWave<T>::set_num_harmonics(size_t num)
{
    this->numHarmonics = num;
    this->_endLoop = (this->numHarmonics * 2) + 1;
}

template <typename T>
inline T MultiHarmonicWave<T>::_get_sample()
{
    T sample(0);
    for (size_t i(1); i < this->_endLoop; i += 2)
    {
        sample += std::sin(this->radians * i) / i;
        this->radians += this->_step;
        this->_trim_phase();
    }
    return sample;
}

template <typename T>
OscillatorBase<T>::OscillatorBase() :
Sinusoid<T>(),
_frequency(1000),
_samplesPerCycle(0)
{
}

template <typename T>
inline void OscillatorBase<T>::_set()
{
    this->_samplesPerCycle = static_cast<T>(this->sampleRate) / this->_frequency;
    this->_step = DiscreetTimeBase<T>::tau / this->_samplesPerCycle / this->numHarmonics;
}

template <typename T>
void OscillatorBase<T>::set_sample_rate(int_fast32_t samplerate)
{
    this->sampleRate = samplerate;
    _set();
}

template <typename T>
void OscillatorBase<T>::set_frequency(T freq)
{
    this->_frequency = freq;
    _set();
}

template <typename T>
bool OscillatorBase<T>::is_set()
{
    return (
            this->_samplesPerCycle
            && this->_step
        );
}

template <typename T>
void OscillatorBase<T>::get(T* buff, size_t numSamples)
{
    for (size_t i(0); i < numSamples; ++i)
    {
        buff[i] = this->_get_sample();
    }
}

template <typename T>
void OscillatorBase<T>::get(std::vector<T>* buff)
{
    const size_t length(buff->size());
    for (size_t i(0); i < length; ++i)
    {
        buff->at(i) = this->_get_sample();
    }
}

template <typename T>
MultiHarmonicOscillator<T>::MultiHarmonicOscillator() :
MultiHarmonicWave<T>(),
OscillatorBase<T>()
{
    MultiHarmonicWave<T>::set_num_harmonics(4);
}

template <typename T>
inline T MultiHarmonicOscillator<T>::_get_sample()
{
    return MultiHarmonicWave<T>::_get_sample();
}

template class DiscreetTimeBase<float>;
template class DiscreetTimeBase<double>;
template class DiscreetTimeBase<long double>;
template class Sinusoid<float>;
template class Sinusoid<double>;
template class Sinusoid<long double>;
template class MultiHarmonicWave<float>;
template class MultiHarmonicWave<double>;
template class MultiHarmonicWave<long double>;
template class OscillatorBase<float>;
template class OscillatorBase<double>;
template class OscillatorBase<long double>;
template class MultiHarmonicOscillator<float>;
template class MultiHarmonicOscillator<double>;
template class MultiHarmonicOscillator<long double>;
