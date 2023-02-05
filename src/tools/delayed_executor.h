#ifndef DELAYED_EXECUTOR_H
#define DELAYED_EXECUTOR_H
#include <Arduino.h>

/**
 * @brief This class allows us to defer the execution of 
 * something in the caller's context
 * 
 * - Instanciate an object of this (e.g.: myDE)
 * - To trigger it, call myDE.executeAfter(duration_microseconds);
 * - Call myDE.update() in the loop()
 * - After the update, check myDE.isTimeElapsed(), it tells the caller
 *   if we now have to run the delayed stuff and gets idle.
 * 
 */
class DelayedExecutor
{
private:
    bool _isTimeElapsed = false;
    bool _isExecutionPending = false;
public:
    uint64_t nextExecutionTime = 0;
        
    inline void executeAfter(uint32_t us){
        this->nextExecutionTime = micros() + us;
        this->_isExecutionPending = true;
    }

    /**
     * @brief indicates if the execution has been done.
     * 
     * @return true 
     * @return false 
     */
    inline bool isWaitingForExecution()
    {
        return _isExecutionPending;
    }

    /**
     * @brief Check the time, and raise the flag to execute the stuff.
     * 
     */
    inline void update(){
        if(!_isExecutionPending)
            return;
        
        if(micros() >= nextExecutionTime)
        {
            _isTimeElapsed = true;
            _isExecutionPending = false;
        }
    }

    /**
     * @brief Indicates if it's time to run the stuff.
     * If it's time, it will return true.
     * Flags are disabled before it returns true, so another call right after will return false!
     * 
     * @return true if it's time !
     * 
     */
    inline bool isTimeElapsed()
    {
        if(_isTimeElapsed)
        {
            _isTimeElapsed = false;
            return true;
        }
        return false;
    }
    
    inline void cancel()
    {
        this->_isExecutionPending = false;
    }
};

#endif // DELAYED_EXECUTOR_H