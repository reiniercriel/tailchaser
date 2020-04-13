#include "../motiondetector/motiondetector.h"

class TailChaser {
public:
    TailChaser();
    ~TailChaser();

    int start();
    void stop();

public:
    static void on_motionDetected(const Mat &frame);

private:
    void motionDetected(const Mat &frame);

private:
    bool m_isRunning;
};
