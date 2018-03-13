#include "JobRunner.h"

#include <assert.h>

#include "Core\Utility\Utility.h"
#include "Core\MemoryManagement\HeapOperations.h"

namespace Engine
{
namespace JobSystem
{

DWORD WINAPI JobRunner( void * i_pThreadInput )
{
	assert( i_pThreadInput );
	assert( ( (JobRunnerInput *) i_pThreadInput )->i_pQueue );

	JobRunnerInput * pInput = reinterpret_cast<JobRunnerInput *>( i_pThreadInput );
 	SharedJobQueue * pQueue = reinterpret_cast<SharedJobQueue *>( pInput->i_pQueue );

	DEBUG_PRINT_INFO( "JobRunner \"%s\": Starting Queue \"%s\".\n", pInput->m_ThreadName.c_str(), pQueue->GetName() ? pQueue->GetName() : "Unknown" );

	bool bDone = false;

	do
	{
		DEBUG_PRINT_INFO( "JobRunner \"%s\": Acquiring Job.\n", pInput->m_ThreadName.c_str() );
		IJob * pJob = pQueue->GetWhenAvailable();
		if( pJob )
		{
			DEBUG_PRINT_INFO( "JobRunner \"%s\": Starting Job \"%s\" on Processor %d.\n", pInput->m_ThreadName.c_str(), pJob->GetName() ? pJob->GetName() : "Unknown", GetCurrentProcessorNumber() );

			pJob->Run();

			DEBUG_PRINT_INFO( "JobRunner \"%s\": Finished Job \"%s\".\n", pInput->m_ThreadName.c_str(), pJob->GetName() ? pJob->GetName() : "Unknown" );
			delete pJob;

			bDone = pQueue->ShutdownRequested();
		}
		else
			bDone = pQueue->ShutdownRequested();

	} while( bDone == false );

#ifdef _DEBUG
	DEBUG_PRINT_INFO( "JobRunner \"%s\": Shutting down.\n", pInput->m_ThreadName.c_str() );
#endif
	return 0;
}

} // namespace JobSystem
} // namespace Engine
