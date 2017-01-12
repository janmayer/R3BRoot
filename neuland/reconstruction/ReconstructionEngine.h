#ifndef NEULANDRECONSTRUCTIONENGINEH
#define NEULANDRECONSTRUCTIONENGINEH

#include "Filterable.h"
#include "R3BNeulandCluster.h"
#include "R3BNeulandNeutron.h"
#include "ReconstructionOperation.h"
#include <memory>
#include <vector>

namespace Neuland
{
    class ReconstructionEngine
    {
      public:
        using Op = std::shared_ptr<Neuland::ReconstructionOperation>;

        ReconstructionEngine() = default;
        ~ReconstructionEngine() = default;
        ReconstructionEngine(const ReconstructionEngine&) = default;            // copy constructor
        ReconstructionEngine(ReconstructionEngine&&) = default;                 // move constructor
        ReconstructionEngine& operator=(const ReconstructionEngine&) = default; // copy assignment
        ReconstructionEngine& operator=(ReconstructionEngine&&) = default;      // move assignment

        void Init();
        // Copy, will be modified inside
        std::vector<R3BNeulandNeutron> GetNeutrons(std::vector<R3BNeulandCluster*>) const;

        inline void AddOperation(const Op& op) { fOps.push_back(op); }

        inline void AddOperation(Neuland::ReconstructionOperation* op) { fOps.push_back(Op(op)); }

      private:
        std::vector<Op> fOps;
    };
}

#endif // NEULANDRECONSTRUCTIONENGINEH
