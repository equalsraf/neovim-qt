#ifndef NEOVIM_QT_TEST_COMMON
#define NEOVIM_QT_TEST_COMMON

// This is just a fix for QSignalSpy::wait
// http://stackoverflow.com/questions/22390208/google-test-mock-with-qt-signals
bool SPYWAIT(QSignalSpy &spy, int timeout=10000)
{
	return spy.count()>0||spy.wait(timeout);
}

bool SPYWAIT2(QSignalSpy &spy, int timeout=5000)
{
	return spy.count()>0||spy.wait(timeout);
}

#endif
