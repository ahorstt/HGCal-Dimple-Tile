#include "Analysis.hh"

#include "LYSimRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "LYSimDetectorConstruction.hh"
#include <iostream>
#include "g4root.hh"

LYSimRunAction::LYSimRunAction(LYSimDetectorConstruction* ipDetectorConstruction)
{
//     runMessenger = new LYSimRunActionMessenger(this);
    pDetectorConstruction = ipDetectorConstruction;
    G4AnalysisManager::Instance();
}

LYSimRunAction::~LYSimRunAction()
{
//     delete runMessenger;
    delete G4AnalysisManager::Instance();
    delete Analysis::GetInstance();
}

void LYSimRunAction::BeginOfRunAction(const G4Run* aRun)
{
    Analysis::GetInstance()->PrepareNewRun(aRun);
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    outFileName = Analysis::GetInstance()->GetROOTFileName();
    G4cout << "Output filename: " << outFileName << G4endl;
    man->OpenFile(outFileName.c_str());
    man->SetFirstHistoId(1);

    // Create histogram(s) (avoid non-integer bins)
    //1
    man->CreateH1("ogammaE","Optical photons Wavelength [nm]", //histoID,histo name 
                      100,200.,1200.); //bins' number, xmin, xmax
    //2
    man->CreateH1("Ndetect","Number of detected photons per event",
                      250,0.,250.); //bins' number, xmin, xmax
    //3
    man->CreateH1("totE","Total optical photons energy deposited per event [eV]",
                      100,0.,5.); //bins' number, xmin, xmax
    //4
    man->CreateH1("timeGamma","time of detected photon [ns]",100,0.,100.);
    //5
    man->CreateH1("npvs","number produced photons vs x [mm]",400,-20.,20.);
    //6
    man->CreateH1("ndvs","number detected photons vs x [mm]",400,-20.,20.);

    //2D hists
    man->CreateH2("xycr","xy creation point of optical photons",400,-20.,20.,400,-20.,20.);
    man->CreateH2("szcr","xz creation point of optical photons",400,-20.,20.,400,-20.,20.);

    man->CreateH2("xycrd","xy creation point of detected optical photons",400,-20.,20.,400,-20.,20.);
    man->CreateH2("szcrd","xz creation point of detected optical photons",400,-20.,20.,400,-20.,20.);

    //3D Hist
    man->CreateNtuple("Traj", "Trajectory of Detected Optical Photons");
    man->CreateNtupleDColumn("zPos");
    man->CreateNtupleDColumn("Phi");
    man->CreateNtupleDColumn("Theta");
    man->FinishNtuple();

}

void LYSimRunAction::EndOfRunAction(const G4Run* aRun)
{
    Analysis::GetInstance()->EndOfRun(aRun);
}
