#include "Init.h"
#include "Task_Scheduler.h"


int main (void)
{
    All_Init();
    while (1)
    {
        System_Task_Runner();
    }
}
