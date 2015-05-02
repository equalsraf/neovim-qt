#ifndef NEOVIM_QT_TEST_COMMON
#define NEOVIM_QT_TEST_COMMON

// This is just a fix for QSignalSpy::wait
// http://stackoverflow.com/questions/22390208/google-test-mock-with-qt-signals
#define SPYWAIT(spy) (spy.count()>0||spy.wait())

#endif
