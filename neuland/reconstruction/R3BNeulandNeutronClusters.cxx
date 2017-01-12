#include "R3BNeulandNeutronClusters.h"

#include <algorithm>

#include "TClonesArray.h"
#include "TH1D.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairRtdbRun.h"
#include "FairLogger.h"
#include "FairMCPoint.h"

#include "R3BNeulandCluster.h"

R3BNeulandNeutronClusters::R3BNeulandNeutronClusters(const TString input,
                                                     const TString outputPrimary,
                                                     const TString outputSecondary,
                                                     const Double_t maxDist)
    : FairTask("R3B Neuland Neutron Cluster Finder")
    , fInput(input)
    , fOutputPrimary(outputPrimary)
    , fOutputSecondary(outputSecondary)
    , fMaxDist(maxDist)
    , fPrimaryClusters(new TClonesArray("R3BNeulandCluster"))
    , fSecondaryClusters(new TClonesArray("R3BNeulandCluster"))
{
}

R3BNeulandNeutronClusters::~R3BNeulandNeutronClusters() {}

InitStatus R3BNeulandNeutronClusters::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
    {
        LOG(FATAL) << "R3BNeulandNeutronClusters::Init: No FairRootManager" << FairLogger::endl;
        return kFATAL;
    }

    // Set Input: TClonesArray of R3BNeulandClusters
    if ((TClonesArray*)ioman->GetObject(fInput) == nullptr)
    {
        LOG(FATAL) << "R3BNeulandNeutronClusters::Init No " << fInput << "!" << FairLogger::endl;
        return kFATAL;
    }
    if (!TString(((TClonesArray*)ioman->GetObject(fInput))->GetClass()->GetName()).EqualTo("R3BNeulandCluster"))
    {
        LOG(FATAL) << "R3BNeulandNeutronClusters::Init Branch " << fInput << " does not contain R3BNeulandCluster"
                   << FairLogger::endl;
        return kFATAL;
    }
    fClustersIn = (TClonesArray*)ioman->GetObject(fInput);

    // Set Input: TClonesArray of FairMCPoints
    if ((TClonesArray*)ioman->GetObject("NeulandPrimaryNeutronInteractionPoints") == nullptr)
    {
        LOG(FATAL) << "R3BNeulandNeutronClusters::Init No NeulandPrimaryNeutronInteractionPoints!" << FairLogger::endl;
        return kFATAL;
    }
    if (!TString(((TClonesArray*)ioman->GetObject("NeulandPrimaryNeutronInteractionPoints"))->GetClass()->GetName())
             .EqualTo("FairMCPoint"))
    {
        LOG(FATAL) << "R3BNeulandNeutronClusters::Init Branch NeulandPrimaryNeutronInteractionPoints does not contain "
                      "FairMCPoints!" << FairLogger::endl;
        return kFATAL;
    }
    fPrimaryNeutronInteractionPoints = (TClonesArray*)ioman->GetObject("NeulandPrimaryNeutronInteractionPoints");

    // Set Output: TClonesArray of R3BNeulandClusters
    ioman->Register(fOutputPrimary, "Cluster closest to NPNIPs", fPrimaryClusters, kTRUE);
    ioman->Register(fOutputSecondary, "Secondary Clusters", fSecondaryClusters, kTRUE);

    fhNeutronClusterDistance = new TH1D("fhNeutronClusterDistance", "fhNeutronClusterDistance", 100, 0, 1000);

    return kSUCCESS;
}

void R3BNeulandNeutronClusters::Exec(Option_t*)
{
    fPrimaryClusters->Clear();
    fSecondaryClusters->Clear();

    const Int_t nClustersIn = fClustersIn->GetEntries();
    std::vector<R3BNeulandCluster*> clustersIn;
    clustersIn.reserve(nClustersIn);
    for (Int_t i = 0; i < nClustersIn; i++)
    {
        clustersIn.push_back(((R3BNeulandCluster*)fClustersIn->At(i)));
    }

    const Int_t nNPNIPS = fPrimaryNeutronInteractionPoints->GetEntries();
    std::vector<FairMCPoint*> npnips;
    npnips.reserve(nNPNIPS);
    for (Int_t i = 0; i < nNPNIPS; i++)
    {
        npnips.push_back(((FairMCPoint*)fPrimaryNeutronInteractionPoints->At(i)));
    }

    TVector3 npnippos;
    for (const FairMCPoint* npnip : npnips)
    {
        npnip->Position(npnippos);
        auto min = std::min_element(clustersIn.begin(),
                                    clustersIn.end(),
                                    [&](const R3BNeulandCluster* a, const R3BNeulandCluster* b)
                                    {
                                        return (a->GetFirstDigi().GetPosition() - npnippos).Mag() <
                                               (b->GetFirstDigi().GetPosition() - npnippos).Mag();
                                    });
        if (min != clustersIn.end())
        {
            const Double_t minDist = ((*min)->GetFirstDigi().GetPosition() - npnippos).Mag();
            fhNeutronClusterDistance->Fill(minDist);
            if (minDist < fMaxDist)
            {
                new ((*fPrimaryClusters)[fPrimaryClusters->GetEntries()]) R3BNeulandCluster(**min);
                clustersIn.erase(min);
            }
        }
    }

    for (auto& cluster : clustersIn)
    {
        new ((*fSecondaryClusters)[fSecondaryClusters->GetEntries()]) R3BNeulandCluster(std::move(*cluster));
    }
}

void R3BNeulandNeutronClusters::Finish() { fhNeutronClusterDistance->Write(); }

ClassImp(R3BNeulandNeutronClusters)
