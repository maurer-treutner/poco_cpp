/*
 * AnySample.h
 *
 *  Created on: Aug 25, 2017
 *      Author: peter
 */

#ifndef TYPEDEMO_INCLUDE_TYPEDEMO_ANYSAMPLE_H_
#define TYPEDEMO_INCLUDE_TYPEDEMO_ANYSAMPLE_H_

#include "SampleSubsystem/Sample.h"

namespace TypeDemo
{
	using SampleSubsystem::Sample;

	class AnySample  : public Sample
	{
	public:
		AnySample():
			Sample("AnySample")
		{}

		void run(std::ostream &os, std::istream &is);
	};
}



#endif /* TYPEDEMO_INCLUDE_TYPEDEMO_ANYSAMPLE_H_ */
