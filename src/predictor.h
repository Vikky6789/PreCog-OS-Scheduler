#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "process.h"

class CPUPredictor {
private:
    float alpha; // The "Learning Rate" (0.0 to 1.0)
                 // 0.5 = Balanced
                 // 0.8 = React fast to changes (Impulsive)
                 // 0.2 = Rely on long-term average (Stubborn)

public:
    CPUPredictor(float a = 0.5) : alpha(a) {}

    // The "Pre-Cog" Algorithm
    void update_prediction(Process* p, int actual_burst_last_time) {
        // 1. Log the history
        p->burst_history.push_back(actual_burst_last_time);

        // 2. First run? Prediction is just the actual time.
        if (p->predicted_next_burst == 0) {
            p->predicted_next_burst = actual_burst_last_time;
        } 
        else {
            // 3. EMA Formula:
            // New_Prediction = (alpha * Actual) + ((1 - alpha) * Old_Prediction)
            p->predicted_next_burst = (alpha * actual_burst_last_time) + 
                                      ((1.0 - alpha) * p->predicted_next_burst);
        }
    }
};

#endif