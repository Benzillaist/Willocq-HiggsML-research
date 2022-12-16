void makeGraphs()
{
    /////////////////////////////
    //GENERAL ANALYSIS SETTINGS//
    /////////////////////////////

    auto fileName1 = "mc16e_signal.root";
    auto treeName1 = "MyLCTuple";
    TString filePrefix1 = "Signal";

    auto fileName2 = "mc16e_ttbar.root";
    auto treeName2 = "MyLCTuple";
    TString filePrefix2 = "tt background";

    TString saveDir = "mc16e_Graphs";

    /////////////////////////////

    TFile *myFile1 = new TFile(fileName1);
    TTree *myTree1 = (TTree*)myFile1->Get(treeName1);

    TFile *myFile2 = new TFile(fileName2);
    TTree *myTree2 = (TTree*)myFile2->Get(treeName2);

    TCanvas *c = new TCanvas();

    //opens the file to be read
    auto openFile1 = TFile::Open(fileName1);

    auto openFile2 = TFile::Open(fileName2);

    TH1F *higgs_pt1 = new TH1F("hpt1", filePrefix1 + ":Higgs pt", 25, 0, 1000000);
    TH1F *higgs_pt2 = new TH1F("hpt2", filePrefix2 + ":Higgs pt", 25, 0, 1000000);

    TH1F *dR1 = new TH1F("dR1", filePrefix1 + ":R", 25, 0, 1.2);
    TH1F *dR2 = new TH1F("dR2", filePrefix2 + ":R", 25, 0, 1.2);

    THStack *hs = new THStack("hs", "Transverse momenta of Higgs bosons ;p_{T} (MeV);Count");

    TTreeReader myReader1("nominal", openFile1);
    TTreeReader myReader2("nominal", openFile2);

    TTreeReaderArray<Float_t> pt_higgs_RA1(myReader1, "pt_higgs");
    TTreeReaderArray<Float_t> pt_higgs_RA2(myReader2, "pt_higgs");

    TTreeReaderArray<Float_t> dR_values_cand_RA1(myReader1, "dR_values_cand");
    TTreeReaderArray<Float_t> dR_values_cand_RA2(myReader2, "dR_values_cand");

    while(myReader1.Next()) {
        for(int i = 0; i < pt_higgs_RA1.GetSize(); i++) {
            higgs_pt1->Fill(pt_higgs_RA1.At(i));
            dR1->Fill(dR_values_cand_RA1.At(i));
        }
    }

    while(myReader2.Next()) {
        for(int i = 0; i < pt_higgs_RA2.GetSize(); i++) {
            higgs_pt2->Fill(pt_higgs_RA2.At(i));
            dR2->Fill(dR_values_cand_RA2.At(i));
        }
    }

    higgs_pt1->SetLineColor(kBlue);
    higgs_pt2->SetLineColor(kRed);

    dR1->SetLineColor(kBlue);
    dR2->SetLineColor(kRed);

    higgs_pt1->SetLineWidth(2);
    higgs_pt2->SetLineWidth(2);

    dR1->SetLineWidth(2);
    dR2->SetLineWidth(2);

    hs->Add(higgs_pt1);
    hs->Add(higgs_pt2);

    hs->Draw("nostack");

    gPad->SetGrid(1, 0);
    gPad->BuildLegend(0.65, 0.7, 0.9, 0.9, "");

    c->SaveAs(saveDir + "/pt_higgs.png");
    c->Close();
    c = new TCanvas();

    hs = new THStack("hs", "R candidate values ;Angle between muon and W jet (rads);Count");

    hs->Add(dR1);
    hs->Add(dR2);

    hs->Draw("nostack");

    gPad->SetGrid(1, 0);
    gPad->BuildLegend(0.65, 0.7, 0.9, 0.9, "");

    c->SaveAs(saveDir + "/dR_values_cand.png");
    // c->Close();

}