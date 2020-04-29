#include "Helpers.h"

using namespace MediaGaps::Media;
using namespace MediaGaps;

using namespace MOVCreator;

bool Helpers::IsFinishedState(IStateProvider::State state)
{
    return state == IStateProvider::State_Finished || state == IStateProvider::State_Aborted || state == IStateProvider::State_Failed;
}
