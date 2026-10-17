#include "common.h"

bool SPYWAIT(QSignalSpy& spy, int timeout, qsizetype minSignals) noexcept
{
	// For more details, see:
	// http://stackoverflow.com/questions/22390208/google-test-mock-with-qt-signals
	return spy.count() >= minSignals || spy.wait(timeout);
}
