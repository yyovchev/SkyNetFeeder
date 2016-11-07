#include "task.h"

Task::Task()
{

}

Task::~Task()
{
    qDebug("finished");
    emit workFinished();
}

void Task::CreateReader()
{
    m = new localreader(this);
}

