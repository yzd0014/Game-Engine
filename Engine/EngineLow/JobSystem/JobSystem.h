#ifndef __JOB_SYSTEM_H
#define __JOB_SYSTEM_H
#include <map>
#include "Core\HashedString\HashedString.h"

namespace Engine
{
namespace JobSystem
{
	class IJob;
	struct JobQueueData;
void CreateQueue( const char * i_pName, unsigned int i_numRunners, std::map<HashedString, JobQueueData *> &Queues);
void AddRunner( const HashedString & i_QueueName, std::map<HashedString, JobQueueData *> &Queues);

void RunJob( IJob * i_pJob, const HashedString & i_QueueName, std::map<HashedString, JobQueueData *> &Queues);
void Shutdown(std::map<HashedString, JobQueueData *> &Queues);

} // namespace JobSystem
} // namespace Engine

#endif // __JOB_SYSTEM_H