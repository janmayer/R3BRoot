// Configuration macro for Geant3 VirtualMC

void Config()
{
    cout << "[g3Config]: Creating TGeant3 VirtualMC ..." << endl;

    TGeant3* geant3 = new TGeant3TGeo("C++ Interface to Geant3");

    R3BStack* stack = new R3BStack();
    stack->SetDebug(kFALSE);
    stack->StoreSecondaries(kTRUE);
    stack->SetMinPoints(1);
    geant3->SetStack(stack);

    // ******* GEANT3  specific configuration for simulated Runs  *******
    geant3->SetTRIG(1); // Number of events to be processed
    geant3->SetSWIT(4, 10);
    geant3->SetDEBU(0, 0, 1);

    //<DB> Command for debugging purpose
    //  geant3->SetSWIT(2, 2);
    //  geant3->SetDEBU(1, 100, 2);

    geant3->SetRAYL(1);
    geant3->SetSTRA(1);
    geant3->SetAUTO(1); // Select automatic STMIN etc... calc. (AUTO 1) or manual (AUTO 0)
    geant3->SetABAN(0); // Restore 3.16 behaviour for abandoned tracks
    geant3->SetOPTI(2); // Select optimisation level for GEANT geometry searches (0,1,2)
    geant3->SetERAN(5.e-7);
    geant3->SetCKOV(1); // cerenkov photons
    //    geant3->SetHADR(5);     // gcalor
}
