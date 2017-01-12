#include "ReconstructionEngine.h"

namespace Neuland
{
    void ReconstructionEngine::Init()
    {
        for (auto& op : fOps)
        {
            op->Init();
        }
    }

    std::vector<R3BNeulandNeutron> ReconstructionEngine::GetNeutrons(std::vector<R3BNeulandCluster*> clusters) const
    {
        UInt_t multiplicity(0);
        std::vector<R3BNeulandNeutron> neutrons;

        for (const auto& op : fOps)
        {
            op->Exec(clusters, multiplicity, neutrons);
        }

        return neutrons;
    }
}
