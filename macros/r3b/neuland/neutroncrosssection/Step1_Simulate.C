void Step1_Simulate(const Double_t energy /* MeV */,
                    const UInt_t nEvents,
                    const TString basePath,
                    const TString baseName)
{
    // Output files
    const TString outFile = basePath + "/" + baseName + ".sim.root";
    const TString parFile = basePath + "/" + baseName + ".par.root";
    const TString geoFile = basePath + "/" + baseName + ".geo.root";

    // Basic simulation setup
    FairRunSim* run = new FairRunSim();
    run->SetName("TGeant4");
    run->SetOutputFile(outFile);
    run->SetMaterials("media_r3b.geo");

    // Geometry
    FairModule* cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave_vacuum.geo");
    run->AddModule(cave);

    R3BNeuland* neuland = new R3BNeuland();
    neuland->SetGeometryFileName("neuland_test.geo.root");
    run->AddModule(neuland);

    // Primary particle generator
    FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

    FairBoxGenerator* boxGen = new FairBoxGenerator(2112, 1);
    boxGen->SetXYZ(0, 0, 0.);
    boxGen->SetThetaRange(0., 0.);
    boxGen->SetPhiRange(0., 360.);
    boxGen->SetEkinRange(energy / 1000., energy / 1000.);

    primGen->AddGenerator(boxGen);
    run->SetGenerator(primGen);

    // Further setup options and initialization
    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
    FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
    run->SetStoreTraj(kFALSE);
    run->Init();

    // Connect runtime parameter file
    FairParRootFileIo* parFileIO = new FairParRootFileIo(kTRUE);
    parFileIO->open(parFile);
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    rtdb->setOutput(parFileIO);
    rtdb->saveOutput();

    // Save geometry for later reference
    // run->CreateGeometryFile(geoFile);

    // Simulate
    run->Run(nEvents);
}
