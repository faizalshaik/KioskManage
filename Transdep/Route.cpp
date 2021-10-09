#include "control.h"
#include "route.h"

using namespace transdep;

StopSequence::StopSequence(UINT stop, UINT seq)
{
	Stop = stop;
	Sequence = seq;
}

bool StopSequence :: operator < (const StopSequence s) const
{
	return (Sequence < s.Sequence);
}

bool StopSequence :: operator == (const UINT value) const
{
	return (Stop == value);
}

Route::Route()
{
}

void Route::Reset()
{
	sequence.clear();
}

bool Route::IsEmpty()
{
	return sequence.empty();
}

void Route::Extend(StopSequence s)
{
	if (std::find(sequence.begin(), sequence.end(), s.Stop) == sequence.end())
		sequence.push_back(s);
}

void Route::Sort()
{
	std::sort(sequence.begin(), sequence.end());
}