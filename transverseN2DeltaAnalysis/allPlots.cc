/**************************************************************************/
/*                              Nuruzzaman                                */
/*                              03/10/2011                                */
/*                                                                        */
/*                        Last Edited:08/22/2012                          */
/*                                                                        */
/* PLEASE CONSULT WITH ME BEFORE CHANGING THE SCRIPT. IF NEEDED YOU CAN   */
/* HAVE YOUR COPY AND EDIT THERE.TRY NOT TO USE qwanalysis DIRECTORY.     */
/* PLEASE MAINTAIN THIS TO GET OVERWRITTEN BY OLD COPIES.                 */
/*                                                                        */
/* This macro create all the necessary post plots using rootfiles.        */
/* Manual at the bottom of this macro for its use                         */
/**************************************************************************/
#include "NurClass2.h"
#include <stdlib.h>

//void prepareHCLog();
void allPlots(UInt_t run_number=0, Int_t hclog_switch=-1);
void allPlots(TString rootfile, UInt_t run_number=0, Int_t hclog_switch=-1);
void PlotBPM_effectiveq_asyms(TTree* tree, TPad * pad);



void allPlots(UInt_t run_number, Int_t hclog_switch)
{
  gRunNumber = run_number;
  if( gRunNumber == 0 ) {
    /* Ask to input the run condition to user */
    printf("%sEnter Run Number%s\n",blue,normal); 
    std::cin >> gRunNumber;
  }


  // gRootfileDir = getenv("QW_ROOTFILES");
  // TString rootname(Form("/home/nur/scratch/rootfiles/first100k_%d.root",gRunNumber));
  // TString rootname(Form("$QW_ROOTFILES/first100k_%d.root",gRunNumber));
  // TString rootname(Form("$QW_ROOTFILES/inj_parity_%d.000.trees.root",gRunNumber));
  // TString rootname(Form("$QW_ROOTFILES/QwPass1_%d.000.root",gRunNumber));

  // TString currentUser =  getenv("USER");

  TString currentDir  =  gSystem->pwd();

  // TString currentPass = "1";
  TString currentPass = "4b";

  // TString rootFileName = Form("QwPass%s",currentPass.Data());
  TString rootFileName = "Qweak";

  gRootfileDir  = Form("$QW_ROOTFILES");
  gRootfileDir2 = Form("/net/cdaqfs/home/cdaq/qweak/QwScratch/rootfiles");
  gRootfileDir3 = Form("/volatile/hallc/qweak/QwAnalysis/run1/rootfiles");
  gRootfileDir4 = Form("/cache/mss/hallc/qweak/rootfiles/pass%s",currentPass.Data());

  gPlotDir = Form("$QWSCRATCH");
  gPlotDir2 = Form("/net/cdaqfs/home/cdaq/users/nur/transverseN2Delta");
  gPlotDir3 = Form("/w/hallc/qweak/nur/transverseN2Delta");
  gPlotDir4 = Form("/w/hallc/qweak/nur/transverseN2Delta");

  if(currentDir==gPlotDir2){TString rootname(Form("%s/%s_%d.000.trees.root",gRootfileDir2.Data(),rootFileName.Data(),gRunNumber));}
  if(currentDir==gPlotDir3){TString rootname(Form("%s/%s_%d.000.trees.root",gRootfileDir3.Data(),rootFileName.Data(),gRunNumber));}
  if(currentDir==gPlotDir4){TString rootname(Form("%s/%s_%d.000.trees.root",gRootfileDir4.Data(),rootFileName.Data(),gRunNumber));}
  else{TString rootname(Form("%s/%s_%d.000.trees.root",gRootfileDir.Data(),rootFileName.Data(),gRunNumber));}

  allPlots(rootname,run_number,hclog_switch);
}

void allPlots(TString rootfile, UInt_t run_number, Int_t hclog_switch)
{
  Bool_t BPM_EFAQ=0;Bool_t HCLOGPOST=0;Bool_t CREATETEXT=1;

  // Bool_t MDPMT=1;Bool_t CHARGE=1;Bool_t CHARGEDD=1;Bool_t BMODCYCLE=1;Bool_t BPMS=1;Bool_t MDYIELDVAR=1; Bool_t MDBKG=1; 
  // Bool_t MDALLASYM=1;Bool_t SENSITIVITY=1;Bool_t MDLUMI=1;Bool_t USLUMI=1;Bool_t USLUMISEN=1;Bool_t BMODSEN=1;
  Bool_t MDPMT=0;Bool_t CHARGE=1;Bool_t CHARGEDD=0;Bool_t BMODCYCLE=0;Bool_t BPMS=0;Bool_t MDYIELDVAR=0; Bool_t MDBKG=0; 
  Bool_t MDALLASYM=0;Bool_t SENSITIVITY=0;Bool_t MDLUMI=0;Bool_t USLUMI=0;Bool_t USLUMISEN=0;Bool_t BMODSEN=0;
  

  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  // printf ( "%sThe current date/time is: %s%s\n%s",blue,red,asctime(timeinfo),normal);
  
  if( run_number == 0 ) {
    //  Extract the run number from the filename
    //  The form is assumed to be:  stem_###.(###.)root
    //  where the segment number is optional.
    //  The run number is assumed to be between the 
    //  last "_" character and the first "." character.
    TString filename, stem;
    TObjArray*  all_strings = rootfile.Tokenize("/");
    filename = ((TObjString*)(all_strings->Last()))->GetString();
    all_strings->Delete();
    all_strings = filename.Tokenize(".");
    stem = ((TObjString*)(all_strings->First()))->GetString();
    all_strings->Delete();
    all_strings = stem.Tokenize("_");
    gRunNumber = std::atol(((TObjString*)(all_strings->Last()))->GetString());
    all_strings->Delete();
  } else {
    gRunNumber = run_number;
  }


  TFile *f = new TFile(rootfile);
  if (!f->IsOpen()) {printf("%sFile not found. Exiting the program!\n%s",red,normal); exit(1); 
  }

  /* Load different Trees. */
  TChain * th = new TChain("Hel_Tree");
  if(th == NULL) {printf("%sUnable to find %sHel_Tree%s. Exiting the program!%s\n",blue,red,blue,normal); exit(1);}
  TChain * tm = new TChain("Mps_Tree");
  if(tm == NULL) {printf("%sUnable to find %sMps_Tree%s. Exiting the program!%s\n",blue,red,blue,normal); exit(1);}
  TChain * ts = new TChain("Slow_Tree");
  if(ts == NULL) {printf("%sUnable to find %sSlow_Tree%s. Exiting the program!%s\n",blue,red,blue,normal); exit(1);}
  Char_t runCC[255];
//   for(Int_t t=0; t<=15; t++){
//     sprintf(runCC,"%s/%s_%d.00%d.trees.root",gRootfileDir2.Data(),rootFileName.Data(),gRunNumber,t);
    sprintf(runCC,"%s/Qweak_%d.0*.trees.root",gRootfileDir2.Data(),gRunNumber);
    th->Add(runCC);
    tm->Add(runCC);
    ts->Add(runCC);
//   }

  //Make plots directory
  gSystem->mkdir("plots");

  std::ofstream Myfile;
  Myfile.open(Form("plots/%drunSummary.txt",gRunNumber));
  //  UInt_t gRunNumber = 0;
  char run0[255],run[255],run100k[255],sline[255],dline[255],ssline[255],sslinen[255];

  sprintf(dline,"%s#*************************************************************************#%s\n",red,normal);
  sprintf(ssline,"%s---------------------------------------------------------%s\n",green,normal);
  sprintf(sslinen,"---------------------------------------------------------\n");

  // printf("%sPlease Insert target Information. 1= LH2, 2= 4% US Al, 3= 2% US Al, 4= 1% US Al,\n5= 4% DS Al, 6= 2% DS Al, 7= 1% DS Al, 8= US Pure Al and hit ENTER\n %s",blue,normal); 
  // UInt_t target = 0;
  // std::cin >> target;
  // if (target> 8) {printf("%sPlease insert a correct No. Exiting the program!%s\n",blue,normal); exit(1);}

  char *tar[8];
  tar[1] = "Qweak Target: LH2";
  tar[2] = "Qweak Target: 4% US Al";
  tar[3] = "Qweak Target: 2% US Al";
  tar[4] = "Qweak Target: 1% US Al";
  tar[5] = "Qweak Target: 4% DS Al";
  tar[6] = "Qweak Target: 2% DS Al";
  tar[7] = "Qweak Target: 1% DS Al";
  tar[8] = "Qweak Target: US Pure Al";
  gTarget = tar[1];
  // gTarget = "Qweak Target: ";

  // TString caget_target_name = gSystem->GetFromPipe("caget -c -t QWtgt_name");
  // if(gTarget.IsNull()) caget_target_name = "undefined";

  // gTarget += caget_target_name;

  TString s1 = "ErrorFlag==0 && yield_ramp<0";
  TString s2 = "Device_Error_Code==0";

  Double_t fit_range[2] = { -0.02, 0.02};
  TF1* fitfunx = new TF1("fitfunx","pol1",fit_range[0],fit_range[1]);
  TF1* fitfuny = new TF1("fitfuny","pol1",fit_range[0],fit_range[1]);

  Double_t current,current_check,raster,cal_mdalla,cal_bcmdd,cal_abcm,cal_abcmm,cal_mdxsen,cal_mdysen;
  Double_t cal_emdxsen,cal_emdysen,mean_charge,charge_scale,cal_uslumi_sum_mean,cal_uslumi_sum_rms;
  TString cut_charge;
  TCanvas *c0 = new TCanvas("c0","c0",340,340,100,100);
  c0->cd();
  //   ts->Draw("ibcm1>>bcm1","","goff");bcm1->Draw("goff");


  th->Draw("yield_qwk_charge>>cur",Form("%s && yield_qwk_charge.%s",s1.Data(),s2.Data()),"goff");
  current_check = GetHist("cur")->GetEntries();
  if(!current_check){
    printf("%sRun %d: No data for this run. Exiting program %s\n",blue,gRunNumber,normal);
    //    exit(1);
  }

  //  Extract raster settings.
  ts->Draw("EHCFR_LIXWidth>>rasterx","","goff");
  ts->Draw("EHCFR_LIYWidth>>rastery","","goff");
  //  Extract beam energy.
  ts->Draw("EHCFR_ENERGY>>beamEnergy","","goff");
  //  Extract QTOR info.
  ts->Draw("qw_qt_mps_i_dcct>>qtorDCCT","","goff");
  //  Extract Beam Current from EPICS.
  ts->Draw("ibcm1>>currentEPICS","","goff");


  if(gRunNumber>13000){
    th->Draw("(asym_qwk_bcm7-asym_qwk_bcm8)*1e6>>bcmdd",Form("%s && asym_qwk_bcm7.%s && asym_qwk_bcm8.%s",s1.Data(),s2.Data(),s2.Data()),"goff");
  }
  else{
    th->Draw("(asym_qwk_bcm1-asym_qwk_bcm2)*1e6>>bcmdd",Form("%s && asym_qwk_bcm1.%s && asym_qwk_bcm2.%s",s1.Data(),s2.Data(),s2.Data()),"goff");
  }
  th->Draw("asym_qwk_charge*1e6>>abcm",Form("%s && asym_qwk_charge.%s",s1.Data(),s2.Data()),"goff");
  th->Draw("asym_qwk_mdallbars*1e6>>mdalla",Form("%s && asym_qwk_mdallbars.%s",s1.Data(),s2.Data()),"goff");
  th->Draw("asym_uslumi_sum*1e6>>uslumi_sum_a",Form("%s && asym_uslumi_sum.%s",s1.Data(),s2.Data()),"goff");
  th->Draw("asym_qwk_mdallbars*1e6:diff_qwk_targetX>>mdxsen",
	   Form("%s && asym_qwk_mdallbars.%s && diff_qwk_targetX.%s",s1.Data(),s2.Data(),s2.Data()),"prof");
  GetHist("mdxsen")->Fit("fitfunx","E M R F Q","",-0.02,0.02);
  th->Draw("asym_qwk_mdallbars*1e6:diff_qwk_targetY>>mdysen",
	   Form("%s && asym_qwk_mdallbars.%s && diff_qwk_targetY.%s",s1.Data(),s2.Data(),s2.Data()),"prof");
  GetHist("mdysen")->Fit("fitfuny","E M R F Q","",-0.02,0.02);
  tm->Draw("qwk_charge:event_number>>histocrg", "ErrorFlag == 0 && qwk_charge.Device_Error_Code==0");
  mean_charge = GetHist("histocrg")->GetMean(2);
  charge_scale = 0.95*mean_charge;
  cut_charge = Form("qwk_charge > %f",charge_scale);
  //  printf("cut_charge = %s\n",cut_charge.Data());
  //   cut_charge = Form("qwk_charge > %f", 0.99*mean_charge);
  //   yield_charge = Form("qwk_charge >= %f", 0.98*mean_charge);

  gCurrent    = Form("I (EPICS): %3.1f uA", GetHist("currentEPICS")->GetMean());
  gRasterString = Form("%2.1fx%2.1f mm^2", GetHist("rasterx")->GetMean(), GetHist("rastery")->GetMean());
  gEnergy = Form("Beam Energy: %4.3f GeV", GetHist("beamEnergy")->GetMean());
  gQTORDCCT = Form("QTOR current: %4.1f A", GetHist("qtorDCCT")->GetMean());

  cal_mdalla  = GetHist("mdalla")->GetRMS();
  cal_bcmdd   = GetHist("bcmdd")->GetRMS();
  cal_abcm    = GetHist("abcm")->GetRMS();
  cal_abcmm   = GetHist("abcm")->GetMean();
  cal_mdxsen  = fitfunx->GetParameter(1);
  cal_mdysen  = fitfuny->GetParameter(1);
  cal_emdxsen = fitfunx->GetParError(1);
  cal_emdysen = fitfuny->GetParError(1);
  cal_uslumi_sum_mean = GetHist("uslumi_sum_a")->GetMean();
  cal_uslumi_sum_rms  = GetHist("uslumi_sum_a")->GetRMS();

  c0->Update();

  const Int_t NUM = 8, NUM1 = 5,NUM2 = 6, NUM3 = 3, NUM4 = 4, NUM7 = 7, NUM15 = 15;
  if(gRunNumber>13000){
    TString bcms[NUM7] = {"charge","bcm1","bcm2","bcm5","bcm6","bcm7","bcm8"};
    TString bcms2[NUM2] = {"bcm1","bcm1","bcm5","bcm5","bcm7","bcm8"};
    TString bcms3[NUM2] = {"bcm2","bcm5","bcm6","bcm7","bcm8","bcm1"};
    TString bcms4[NUM15] = {"bcm1","bcm1","bcm1","bcm1","bcm1","bcm2","bcm2","bcm2","bcm2","bcm5","bcm5","bcm5","bcm6","bcm6","bcm7"};
    TString bcms5[NUM15] = {"bcm2","bcm5","bcm6","bcm7","bcm8","bcm5","bcm6","bcm7","bcm8","bcm6","bcm7","bcm8","bcm7","bcm8","bcm8"};
  }
  else{
    TString bcms[NUM7] = {"charge","bcm1","bcm2","bcm5","bcm6","bcm1","bcm2"};
    TString bcms2[NUM2] = {"bcm1","bcm1","bcm5","bcm5","bcm1","bcm2"};
    TString bcms3[NUM2] = {"bcm2","bcm5","bcm6","bcm1","bcm2","bcm1"};
    TString bcms4[NUM15] = {"bcm1","bcm1","bcm1","bcm1","bcm1","bcm2","bcm2","bcm2","bcm2","bcm5","bcm5","bcm5","bcm6","bcm6","bcm1"};
    TString bcms5[NUM15] = {"bcm2","bcm5","bcm6","bcm1","bcm2","bcm5","bcm6","bcm1","bcm2","bcm6","bcm1","bcm2","bcm1","bcm2","bcm2"};
  }
  TString bpms[NUM4] = {"target","bpm3h09","bpm3h07c","bpm3c12"};
  TString mdasym[NUM] = {"md1barsum","md2barsum","md3barsum","md4barsum","md5barsum","md6barsum","md7barsum","md8barsum"};
  TString mdbkg[NUM2] = {"md9pos","md9neg","pmtonl","pmtltg","pmtled","preamp"};
  TString mdbkg2[2] = {"pmtled","preamp"};
  TString allmd[NUM3] = {"all","even","odd"};
  TString lumi[NUM3] = {"sum","even","odd"};
  TString uslumi[NUM1] = {"uslumi1","uslumi3","uslumi5","uslumi7","uslumi"};
  //  TString uslumi[NUM4] = {"uslumi1","uslumi3","uslumi7","uslumi"};
  TString bmod[NUM2] = {"ramp","fgx1","fgx2","fge","fgy1","fgy2"};

  char pcharge[255],pchargeasym[255],pchargeddasym[255],pbpmd[255],pmdpmtyield[255],pmdpmtasym[255],pmdasym[255],pmdallasym[255],pmdallsenx[255],pmdallseny[255],pdslumiyield[255],pdslumiasym[255],plumisenx[255],plumiseny[255],pmodulation[255],puslumi[255],puslumisen[255],pmdyieldvar[255],pmdbkg[255],psenbmod[255];
  char pbpmefcharge[255];

  sprintf(pcharge,"plots/%dCharge.png",gRunNumber);
  sprintf(pchargeasym,"plots/%dChargeAsym.png",gRunNumber);
  sprintf(pchargeddasym,"plots/%dChargeDDAsym.png",gRunNumber);
  sprintf(pbpmd,"plots/%dBPMDiff.png",gRunNumber);
  sprintf(pmdpmtyield,"plots/%dMDPMTYield.png",gRunNumber);
  sprintf(pmdpmtasym,"plots/%dMDPMTAsym.png",gRunNumber);
  sprintf(pmdyieldvar,"plots/%dMDYieldVariation.png",gRunNumber);
  sprintf(pmdasym,"plots/%dMDAsym.png",gRunNumber);
  sprintf(pmdallasym,"plots/%dMDBCMAsym.png",gRunNumber);
  sprintf(pmdbkg,"plots/%dMDBackground.png",gRunNumber);
  sprintf(pmodulation,"plots/%dModulationCycle.png",gRunNumber);
  sprintf(psenbmod,"plots/%dModulationSensitivity.png",gRunNumber);
  sprintf(pmdallsenx,"plots/%dMDXSensitivity.png",gRunNumber);
  sprintf(pmdallseny,"plots/%dMDYSensitivity.png",gRunNumber);
  sprintf(pdslumiyield,"plots/%dDSLumiYield.png",gRunNumber);
  sprintf(pdslumiasym,"plots/%dDSLumiAsym.png",gRunNumber);
  sprintf(plumisenx,"plots/%dLumiXSensitivity.png",gRunNumber);
  sprintf(plumiseny,"plots/%dLumiYSensitivity.png",gRunNumber);
  sprintf(puslumi,"plots/%dUSLumiYieldAsym.png",gRunNumber);
  sprintf(puslumisen,"plots/%dUSLumiSensitivity.png",gRunNumber);
  sprintf(pbpmefcharge,"plots/%dBPMEffectiveCharge.png",gRunNumber);


  //   char txtcharge[255],txtchargeasym[255];
  //   char comontext[255],txtcharge1[255],txtchargeasym1[255];
  //   sprintf(txtcharge,"Run %d: Charge (uA)",gRunNumber);
  //   sprintf(txtchargeasym,"Run %d: Charge Asymmetry (ppm)",gRunNumber);
  //   sprintf(txtchargeddasym,"Run %d: Charge Double Difference Asymmetry (ppm)",gRunNumber);
  //   sprintf(txtbpmd,"Run %d: BPM Difference (mm)",gRunNumber);
  //   sprintf(txtmdyield,"%s/%dMDPMTYield",gRunNumber);
  //   sprintf(txtmdasym,"Run %d: Main Detector Barsum Asymmetries (ppm)",gRunNumber);
  //   sprintf(txtmdallasym,"%dMDAllbarsumAsym",gRunNumber);
  //   sprintf(txtmdallsen,"%dMDAllbarsumSensitivity",gRunNumber);
  //   sprintf(txtdslumiyield,"Run %d: Downstream Lumi Yields (Volts/uA)",gRunNumber);
  //   sprintf(txtdslumiasym,"Run %d: Downstream Lumi Asymmetries (ppm)",gRunNumber);

  //   sprintf(comontext,"for %s, %2.1f uA, %2.1fx%2.1f mm",tar[1],gCurrent,gRasterString);
  //   sprintf(txtcharge1,"%s %s",txtcharge,comontext);
  //   sprintf(txtchargeasym1,"%s %s",txtchargeasym,comontext);
  /************************************************************************/
  //   TString nodatax = "NO Modulation for X";
  //   TString nodataxp = "NO Modulation for X Angle";
  //   TString nodatae = "NO Modulation for E";
  //   TString nodatay = "NO Modulation for Y";
  //   TString nodatayp = "NO Modulation for Y Angle";
  TText *tnodatax = new TText(0.40,0.40,"NO Modulation for X");
  TText *tnodataxp = new TText(0.40,0.40,"NO Modulation for X Angle");
  TText *tnodatae = new TText(0.40,0.40,"NO Modulation for E");
  TText *tnodatay = new TText(0.40,0.40,"NO Modulation for Y");
  TText *tnodatayp = new TText(0.40,0.40,"NO Modulation for Y Angle");
  tnodatax->SetTextSize(0.15);
  tnodataxp->SetTextSize(0.15);
  tnodatae->SetTextSize(0.15);
  tnodatay->SetTextSize(0.15);
  tnodatayp->SetTextSize(0.15);
  /************************************************************************/
  Int_t csizx,csizy,csiz3x,csiz3y;
  // Double_t cx1,cy1,cx2,cx3;
  // Double_t tsiz,tsiz3,tll,tlr;
  // Double_t ps1,ps2,ps3,ps4;
  csizx=1200;csizy=1000;csiz3x=1100;csiz3y=780;
  // tsiz=0.40;tsiz3=0.45;tll=0.012;tlr=0.4;
  // cx1=0;cy1=0;cx2=300;cx3=600;
  // ps1=0.01;ps2=0.93;ps3=0.94;ps4=0.99;
  /************************************************************************/
  int position1[5] = { 1, 3, 4, 5, 6 };
  int position2[6] = { 1, 2, 3, 4, 5, 6 };
  int position3[4] = { 1, 3, 5, 7 };
  int position4[4] = { 2, 4, 6, 8 };
  int position5[9] = { 0, 4, 1, 2, 3, 6, 9, 8, 7 };
  int position6[9] = { 0, 5, 1, 2, 3, 7, 11, 10, 9 };
  int position7[3] = { 4, 8, 12 };
  int position8[9] = { 0, 7, 1, 2, 3, 9, 15, 14, 13 };
  int position9[9] = { 0, 10, 4, 5, 6, 12, 18, 17, 16 };
  int position10[5] = { 7, 2, 9, 14, 8 };
  int position11[5] = { 10, 5, 12, 17, 11 };
  int position12[5] = { 4, 2, 6, 8, 5 };
  int position13[6] = { 1, 2, 5, 6, 9, 10 };
  int position14[6] = { 3, 4, 7, 8, 11, 12 };
  int position15[7] = { 1, 3, 4, 5, 6, 7, 8 };
  int position16[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  int position17[4] = { 7, 2, 14, 8 };
  int position18[4] = { 10, 5, 17, 11 };

  /****************************************************************************/
  char ycharge[255],cutycharge[255],acharge[255],cutacharge[255],ddcharge[255],cutddcharge[255],
    bpmx[255],cutbpmx[255],bpmy[255],cutbpmy[255],
    adet[255],cutadet[255],ydetvar[255],cutydetvar[255],
    ylumi[255],cutylumi[255],ylumis[255],cutylumis[255],
    alumi[255],cutalumi[255],alumis[255],cutalumis[255],
    ypmtpos[255],ypmtneg[255],cutypmtpos[255],cutypmtneg[255],
    apmtpos[255],apmtneg[255],cutapmtpos[255],cutapmtneg[255],
    adetsenx[255],adetseny[255],cutadetsenx[255],cutadetseny[255],
    //REM    alldetsenx[255],alldetseny[255],adetsenxp[255],adetsenyp[255],
    //REM    cutalldetsenx[255],cutalldetseny[255],cutadetsenxp[255],cutadetsenyp[255],
    histox[255],histoy[255],
    //REM    histoxp[255],histoyp[255],hyaxis[255],
    //REM    modulation[255],cutmodulation[255],
    //REM    hbmod[255],ybmod[255],xbmod[255],tbmod[255],
    yuslumis[255],cutyuslumis[255],auslumis[255],cutauslumis[255];  
  /****************************************************************************/
  char yaxis[255],xaxis[255],
    //REM adetsenxf[255],histoxf[255],cutadetsenxf[255],
    //REM    adetasenxf[255],
    adetasenx[255],
    //REM    histoxfa[255],
    histoxa[255],
    //REM    cutadetasenxf[255],
    cutadetasenx[255],
    yaxisa[255],xaxisa[255],
    //REM    adetsenyf[255],
    //REM    histoyf[255],
    //REM    cutadetsenyf[255],
    //REM    yaxis1[255],
    xaxis1[255],
    //REM    adetasenyf[255],
    adetaseny[255],
    //REM    histoyfa[255],
    histoya[255],
    //REM    cutadetasenyf[255],
    cutadetaseny[255],
    //REM    yaxisa1[255],
    xaxisa1[255];

  char alumisenx[255],
    //REM    alumisenxf[255],
    //REM    histolxf[255],
    histolx[255],
    //REM    cutalumisenxf[255],
    cutalumisenx[255],
    yaxisl[255],
    //REM    xaxisl[255],
    //REM    alumiasenxf[255],
    alumiasenx[255],
    //REM    histolxfa[255],
    histolxa[255],
    //REM    cutalumiasenxf[255],
    cutalumiasenx[255],
    yaxisla[255],
    //REM    xaxisla[255],
    //REM    alumisenyf[255],
    alumiseny[255],
    //REM    histolyf[255],
    histoly[255],
    //REM    cutalumisenyf[255],
    cutalumiseny[255],
    //    yaxisl1[255],xaxisl1[255],
    //REM    alumiasenyf[255],
    alumiaseny[255],
    //REM    histolyfa[255],
    histolya[255],
    //REM    cutalumiasenyf[255],
    cutalumiaseny[255],
    //REM    yaxisla1[255],xaxisla1[255],
    auslumisenx[255],cutauslumisenx[255],
    histouslx[255],auslumiseny[255],cutauslumiseny[255],
    histously[255],yaxisusl[255],
    hapmtpos[255],hapmtneg[255],
    hpmtpos[255],hpmtneg[255];

  char tarbitrary[255],txdetvar[255];
  sprintf(tarbitrary,"Counts [Arbitrary Units]");
  sprintf(txdetvar,"TIME [s]");



  TH1D *senbmodx = NULL;
  TH1D *senbmode = NULL;
  TH1D *senbmody = NULL;
  //    TH1D *senbmodxp = NULL;
  //    TH1D *senbmodyp = NULL;
  TF1* fitbmodx = new TF1("fitbmodx","pol1",0.7,1.1);
  TF1* fitbmode = new TF1("fitbmode","pol1",0.7,1.1);
  TF1* fitbmody = new TF1("fitbmody","pol1",0.7,1.1);
  //    TF1* fitbmodxp = new TF1("fitbmodxp","pol1",0.7,1.1);
  //    TF1* fitbmodyp = new TF1("fitbmodyp","pol1",0.7,1.1);
  // char *s3 = Form("qwk_bcm1.Device_Error_Code==0 && (ramp.Device_Error_Code&0x80)==0x80 && ramp>0 && abs((ramp.block3+ramp.block0)-(ramp.block2+ramp.block1))<50 && %s",cut_charge.Data());
  TString s3 = Form("qwk_bcm1.Device_Error_Code==0 && (ramp.Device_Error_Code&0x80)==0x80 && ramp>0 && abs((ramp.block3+ramp.block0)-(ramp.block2+ramp.block1))<50 && %s",cut_charge.Data());

  Double_t amplitudex = 0.092,  amplitudey  = 0.130;
  //    Double_t amplitudexp = 0.092, amplitudeyp = 0.130;
  Double_t amplitudee = 0.400;
  Double_t xbmodslope1,xbmodslope2,exbmodslope1,exbmodslope2,xbmodslope,exbmodslope;
  //   Double_t xpbmodslope1,xpbmodslope2,expbmodslope1,expbmodslope2,xpbmodslope,expbmodslope;
  Double_t ybmodslope1,ybmodslope2,eybmodslope1,eybmodslope2,ybmodslope,eybmodslope;
  //    Double_t ypbmodslope1,ypbmodslope2,eypbmodslope1,eypbmodslope2,ypbmodslope,eypbmodslope;
  Double_t ebmodslope1,ebmodslope2,eebmodslope1,eebmodslope2,ebmodslope,eebmodslope;
  Double_t exbmodslope11,exbmodslope12,
    //      expbmodslope11,expbmodslope12,
    eebmodslope11,eebmodslope12,
    eybmodslope11,eybmodslope12,
    //      eypbmodslope11,eypbmodslope12,
    exbmodslope13,
    //      expbmodslope13,
    eebmodslope13,
    eybmodslope13;
  // , eypbmodslope13;

  //   TString in;
  //   std::cout <<"Insert q to exit"<<endl;
  //   std::cin >> in;
  //   if(in == "q") exit(-1);
  /************************************************************************/
  gStyle->Reset();
  gStyle->SetTitleYOffset(1.3);
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetStatFontSize(0.12);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetPadColor(kWhite);
  gPad->SetFillColor(kWhite);
  //   gStyle->SetOptStat(1);
  //   gStyle->SetOptFit(1111);
  //    gStyle->SetOptStat(1);
  gStyle->SetStatY(0.99);
  gStyle->SetStatX(0.42);
  gStyle->SetStatW(0.42);
  gStyle->SetStatH(0.36);
  gStyle->SetNdivisions(-404);
  gStyle->SetCanvasColor(kRed-10);
  /************************************************************************/
  if (MDPMT){
    TCanvas *c15 = new TCanvas("c15",CanvasTitle("Main Detector PMT Yields (Volts/uA)"),0,0,1580,1000);
    TPad* pad151 = NewFramedPad(kRed-10, kWhite, "Main Detector PMT Yields (Volts/uA)");

    TString mdpos = "MD POSITIVE";
    TString mdneg = "MD NEGATIVE";
    TText* ctp = new TText(0.30,0.45,mdpos);
    TText* ctn = new TText(0.30,0.45,mdneg);
    ctp->SetTextSize(0.075);
    ctn->SetTextSize(0.075);
    pad151->cd();
    pad151->Divide(6,3);
    printf("%sPlotting %sMain Detector PMT Yields (Volts/uA)%s\n",blue,red,normal);


    TCanvas *c25 = new TCanvas("c25",CanvasTitle("Main Detector PMT Asymmetries (ppm)"),0,0,1580,1000);
    TPad* pad251 = NewFramedPad(kRed-10, kWhite, "Main Detector PMT Asymmetries (ppm)");
    pad251->cd();
    pad251->Divide(6,3);
    printf("%sPlotting %sMain Detector PMT Asymmetries (Volts/uA)%s\n",blue,red,normal);

    /************************************************************************/

    const Int_t pmtNUM = 8;
    Double_t yMDPMTpos[pmtNUM],yMDPMTposError[pmtNUM],yMDPMTposRMS[pmtNUM];
    Double_t yMDPMTneg[pmtNUM],yMDPMTnegError[pmtNUM],yMDPMTnegRMS[pmtNUM];
    Double_t aMDPMTpos[pmtNUM],aMDPMTposError[pmtNUM],aMDPMTposRMS[pmtNUM];
    Double_t aMDPMTneg[pmtNUM],aMDPMTnegError[pmtNUM],aMDPMTnegRMS[pmtNUM];

    pad151->cd(8);
    ctp->Draw();
    pad151->cd(11);
    ctn->Draw();

    pad251->cd(8);
    ctp->Draw();
    pad251->cd(11);
    ctn->Draw();

    int pd = 0;
    TH1* histpmtp;
    TH1* histpmtn;
    TH1* hapmtp;
    TH1* hapmtn;

    for ( int pt=0; pt<8; pt++) {
      pd = pt+1;

      sprintf(ypmtpos,"yield_qwk_md%dpos>>hpmtp%d",pd,pd);
      sprintf(ypmtneg,"yield_qwk_md%dneg>>hpmtn%d",pd,pd);
      sprintf(cutypmtpos,"%s && yield_qwk_md%dpos.%s",s1.Data(),pd,s2.Data());
      sprintf(cutypmtneg,"%s && yield_qwk_md%dneg.%s",s1.Data(),pd,s2.Data());
      sprintf(hpmtpos,"hpmtp%d",pd);
      sprintf(hpmtneg,"hpmtn%d",pd);

      sprintf(apmtpos,"asym_qwk_md%dpos*1e6>>hapmtp%d",pd,pd);
      sprintf(apmtneg,"asym_qwk_md%dneg*1e6>>hapmtn%d",pd,pd);
      sprintf(cutapmtpos,"%s && asym_qwk_md%dpos.%s",s1.Data(),pd,s2.Data());
      sprintf(cutapmtneg,"%s && asym_qwk_md%dneg.%s",s1.Data(),pd,s2.Data());
      sprintf(hapmtpos,"hapmtp%d",pd);
      sprintf(hapmtneg,"hapmtn%d",pd);


      pad151->cd(position8[pd]);
      th->Draw(ypmtpos,cutypmtpos,"goff");
      histpmtp = (TH1F *)gDirectory->Get(hpmtpos);
      histpmtp->SetLineColor(kRed);
      histpmtp->SetYTitle(tarbitrary);
      histpmtp->SetXTitle(Form("MD PMT %d POS Yield [V/uA]",pd));
      histpmtp->SetTitle(Form("MD PMT %d POS Yield ",pd));
      histpmtp->Draw("");
      gPad->Update();

      pad151->cd(position9[pd]);
      th->Draw(ypmtneg,cutypmtneg,"goff");
      histpmtn = (TH1F *)gDirectory->Get(hpmtneg);
      histpmtn->SetLineColor(kBlack);
      histpmtn->SetYTitle(tarbitrary);
      histpmtn->SetXTitle(Form("MD PMT %d NEG Yield [V/uA]",pd));
      histpmtn->SetTitle(Form("MD PMT %d NEG Yield ",pd));
      histpmtn->Draw("");
      gPad->Update();

      pad251->cd(position8[pd]);
      th->Draw(apmtpos,cutapmtpos,"goff");
      hapmtp = (TH1F *)gDirectory->Get(hapmtpos);
      hapmtp->SetFillStyle(3013);
      hapmtp->SetLineColor(kRed);
      hapmtp->SetFillColor(kRed);
      hapmtp->SetYTitle(tarbitrary);
      hapmtp->SetXTitle(Form("MD PMT %d POS Asym [ppm]",pd));
      hapmtp->SetTitle(Form("MD PMT %d POS Asym ",pd));
      hapmtp->Draw("");
      gPad->Update();

      pad251->cd(position9[pd]);
      th->Draw(apmtneg,cutapmtneg,"goff");
      hapmtn = (TH1F *)gDirectory->Get(hapmtneg);
      hapmtn->SetFillStyle(3013);
      hapmtn->SetFillColor(kBlack);
      hapmtn->SetLineColor(kBlack);
      hapmtn->SetYTitle(tarbitrary);
      hapmtn->SetXTitle(Form("MD PMT %d NEG Asym [ppm]",pd));
      hapmtn->SetTitle(Form("MD PMT %d NEG Asym ",pd));
      hapmtn->Draw("");
      gPad->Update();


      yMDPMTpos[pt]      =  histpmtp->GetMean();
      yMDPMTposError[pt] =  histpmtp->GetMeanError();
      yMDPMTposRMS[pt]   =  histpmtp->GetRMS();

      yMDPMTneg[pt]      =  histpmtn->GetMean();
      yMDPMTnegError[pt] =  histpmtn->GetMeanError();
      yMDPMTnegRMS[pt]   =  histpmtn->GetRMS();

      aMDPMTpos[pt]      =  hapmtp->GetMean();
      aMDPMTposError[pt] =  hapmtp->GetMeanError();
      aMDPMTposRMS[pt]   =  hapmtp->GetRMS();

      aMDPMTneg[pt]      =  hapmtn->GetMean();
      aMDPMTnegError[pt] =  hapmtn->GetMeanError();
      aMDPMTnegRMS[pt]   =  hapmtn->GetRMS();

      printf("A_MD%d PMTs\t %4.2f+-%4.2f %4.2f\t %4.2f+-%4.2f %4.2f\n",
	     pd,aMDPMTpos[pt],aMDPMTposError[pt],aMDPMTposRMS[pt],aMDPMTneg[pt],aMDPMTnegError[pt],aMDPMTnegRMS[pt]);
      printf("Y_MD%d PMTs\t %4.2g+-%4.2g %4.2g\t %4.2g+-%4.2g %4.2g\n",
	     pd,yMDPMTpos[pt],yMDPMTposError[pt],yMDPMTposRMS[pt],yMDPMTneg[pt],yMDPMTnegError[pt],yMDPMTnegRMS[pt]);

    }
    c15->Update(); c15->SaveAs(pmdpmtyield);
    c25->Update(); c25->SaveAs(pmdpmtasym);
  }

  /****************************************************************************/
  gStyle->SetStatFontSize(0.060);
  gStyle->SetStatY(1.0);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.25);
  gStyle->SetStatH(0.25); 
  gStyle->SetCanvasColor(kCyan-10);
  if (CHARGE){
    /****************************************************************************/
    TCanvas *c1 = new TCanvas("c1",CanvasTitle("Charge (uA)"),0,0,csizx,csizy);
    TPad* pad11 = NewFramedPad(kCyan-10, kWhite, "Charge (uA)");
    pad11->cd();
    pad11->Divide(2,4);
    printf("%sPlotting %sCharge (uA)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    TCanvas *c2 = new TCanvas("c2",CanvasTitle("Charge Asymmetry (ppm)"),20,20,csizx,csizy);
    TPad* pad21 = NewFramedPad(kCyan-10, kWhite, "Charge Asymmetry (ppm)");
    pad21->cd();
    pad21->Divide(2,4);
    printf("%sPlotting %sCharge Asymmetry (ppm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    char hycharge[255],hacharge[255],tycharge[255], tacharge[255];//, hddcharge[255];

    const Int_t chargeNUM=8,chargeNUM2=7;
    Double_t aCharge[chargeNUM],aChargeError[chargeNUM],aChargeRMS[chargeNUM],yCharge[chargeNUM],yChargeError[chargeNUM],yChargeRMS[chargeNUM];
    Double_t ddCharge[chargeNUM2],ddChargeError[chargeNUM2],ddChargeRMS[chargeNUM2];

    TH1* histycrg;
    TH1* histacrg;
    TH1* hycharge2;
    TH1* hacharge2;
    for ( int i=0; i<7; i++) {

      sprintf(ycharge,"yield_qwk_%s>>hycrg%d",bcms[i].Data(),i);
      sprintf(acharge,"asym_qwk_%s*1e6>>hacrg%d",bcms[i].Data(),i);
      sprintf(cutycharge,"%s && yield_qwk_%s.%s",s1.Data(),bcms[i].Data(),s2.Data());
      sprintf(cutacharge,"%s && asym_qwk_%s.%s",s1.Data(),bcms[i].Data(),s2.Data());
      sprintf(hycharge,"hycrg%d",i);
      sprintf(hacharge,"hacrg%d",i);
      sprintf(tycharge,"%s YIELD [uA]",bcms[i].Data());
      sprintf(tacharge,"%s ASYM [ppm]",bcms[i].Data());

      pad11->cd(position15[i]);
      th->Draw(ycharge,cutycharge);
      histycrg = (TH1F *)gDirectory->Get(hycharge);
      histycrg->SetYTitle(tarbitrary);
      histycrg->SetXTitle(tycharge);
      histycrg->SetTitle(tycharge);
      histycrg->Draw();
      gPad->Update();
   
      pad21->cd(position15[i]);
      th->Draw(acharge,cutacharge);
      histacrg = (TH1F *)gDirectory->Get(hacharge);
      histacrg->SetYTitle(tarbitrary);
      histacrg->SetXTitle(tacharge);
      histacrg->SetTitle(tacharge);
      histacrg->Draw();
      gPad->Update();

      yCharge[i]       =  histycrg->GetMean();
      yChargeError[i]  =  histycrg->GetMeanError();
      yChargeRMS[i]    =  histycrg->GetRMS();
      aCharge[i]       =  histacrg->GetMean();
      aChargeError[i]  =  histacrg->GetMeanError();
      aChargeRMS[i]    =  histacrg->GetRMS();

      printf("%s\t %4.2f+-%4.2f %4.2f\t %4.2f+-%4.2f %4.2f\n", 
	     bcms[i].Data(),aCharge[i],aChargeError[i],aChargeRMS[i],yCharge[i],yChargeError[i],yChargeRMS[i]);
    }

    pad11->cd(2);
    th->Draw("yield_qwk_charge:mps_counter*0.001>>hycharge2",Form("%s && yield_qwk_charge.%s",s1.Data(),s2.Data()));
    //     TH1D* hycharge2 = (TH1D*)(gPad->FindObject("htemp"));
    hycharge2 = GetHist("hycharge2");
    hycharge2->SetYTitle("Charge YIELD [uA]");
    hycharge2->SetXTitle(txdetvar);
    hycharge2->SetTitle("Charge YIELD with TIME");
    hycharge2->Draw();
    pad21->cd(2);
    th->Draw("asym_qwk_charge*1e6:pat_counter>>hacharge2",Form("%s && asym_qwk_charge.%s",s1.Data(),s2.Data()));
    //     TH1F* hacharge2 = (TH1F*)(gPad->FindObject("hacharge2"));
    hacharge2 = GetHist("hacharge2");
    hacharge2->SetYTitle("Charge ASYM [ppm]");
    hacharge2->SetXTitle("PATTERN COUNTER");
    hacharge2->SetTitle("Charge ASYM with PATTERN");
    hacharge2->Draw();


    c1->Update(); c1->SaveAs(pcharge);
    c2->Update(); c2->SaveAs(pchargeasym);}

  /****************************************************************************/
  if (CHARGEDD){
    TCanvas *c3 = new TCanvas("c3", CanvasTitle("Charge Double Difference (ppm)"),40,40,csizx,csizy);
    TPad* pad31 = NewFramedPad(kCyan-10, kWhite, "Charge Double Difference Asymmetry (ppm)");
    //   pad30 = new TPad("pad30","pad30",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad31 = new TPad("pad31","pad31",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad30->SetF20illColor(kCyan-10);
    //   pad31->SetFillColor(kWhite);
    //   pad30->Draw();
    //   pad31->Draw();
    //   pad30->cd();
    //   DrawCanvasHeader("Charge Double Difference Asymmetry (ppm)");
    pad31->cd();
    pad31->Divide(2,3);
    printf("%sPlotting %sCharge Double Difference (ppm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    gStyle->SetOptStat(1);
    gStyle->SetOptStat("emr");

    TString hddcharge;
    TH1* hddcrg;
    for ( int j=0; j<6; j++) {

      sprintf(ddcharge,"asym_qwk_%s*1e6-asym_qwk_%s*1e6>>hdd%d",bcms2[j].Data(),bcms3[j].Data(),j);
      sprintf(cutddcharge,"%s && asym_qwk_%s.%s  && asym_qwk_%s.%s",s1.Data(),bcms2[j].Data(),s2.Data(),bcms3[j].Data(),s2.Data());
      hddcharge = Form("hdd%d",j);
    
      pad31->cd(position2[j]);
      th->Draw(ddcharge,cutddcharge);
      hddcrg = (TH1F *)gDirectory->Get(hddcharge);
      hddcrg->SetYTitle(tarbitrary);
      hddcrg->SetXTitle(Form("%s - %s ASYM [ppm]",bcms2[j].Data(),bcms3[j].Data()));
      hddcrg->SetTitle(Form("%s - %s ASYM",bcms2[j].Data(),bcms3[j].Data()));
      hddcrg->Draw();
      gPad->Update();

      ddCharge[j]       =  hddcrg->GetMean();
      ddChargeError[j]  =  hddcrg->GetMeanError();
      ddChargeRMS[j]    =  hddcrg->GetRMS();

      printf("%s\t %4.2f+-%4.2f %4.2f\n", 
	     bcms2[j].Data(),ddCharge[j],ddChargeError[j],ddChargeRMS[j]);

    }
    c3->Update(); c3->SaveAs(pchargeddasym);
  }

  gStyle->SetTitleSize(0.09);
  gStyle->SetTitleYSize(0.09);
  gStyle->SetTitleXSize(0.075);
  gStyle->SetTitleYOffset(0.3);
  gStyle->SetTitleXOffset(0.6);
  gStyle->SetLabelSize(0.085);
  gStyle->SetLabelSize(0.085,"y");
  gStyle->SetLabelSize(0.085,"x");
  gStyle->SetCanvasColor(kOrange-9);
  if (BMODCYCLE){
    /****************************************************************************/
    TCanvas *c13 = new TCanvas("c13", CanvasTitle("Modulation Cycle"),120,120,csizx,csizy);
    TPad* pad131 = NewFramedPad(kOrange-9, kWhite, "Modulation Cycle");
    //     pad130 = new TPad("pad130","pad130",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //     pad131 = new TPad("pad131","pad131",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //     pad130->SetFillColor(kOrange-9);
    //     pad131->SetFillColor(kWhite);
    //     pad130->Draw();
    //     pad131->Draw();
    //     pad130->cd();
    //     DrawCanvasHeader("Modulation Cycle");
    pad131->cd();
    pad131->Divide(1,6);
    printf("%sPlotting %sModulation Cycle%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    gStyle->SetOptStat(0);
    TString subblock0 = "((ramp.block3+ramp.block0)-(ramp.block2+ramp.block1)) <  0.5e2";
    TString subblock1 = "((ramp.block3+ramp.block0)-(ramp.block2+ramp.block1)) > -0.5e2";
    TString error = "qwk_bcm1.Device_Error_Code==0 && (ramp.Device_Error_Code&0x80)==0x80";
    TString cut = Form("%s && %s && %s", subblock0.Data(), subblock1.Data(), error.Data());
    Double_t bmod_r[2] = { -0.4, 0.4};
    Int_t    bmod_bin[1] = {250};
    Double_t bmod_view[2] = {0,2000};
    TH2D * histe  = new TH2D("histe", "histe", bmod_bin[0], bmod_view[0], bmod_view[1], bmod_bin[0],bmod_r[0],bmod_r[1]);
    TH2D * histx1 = new TH2D("histx1", "histx1", bmod_bin[0], bmod_view[0], bmod_view[1], bmod_bin[0],bmod_r[0],bmod_r[1]);
    TH2D * histx2 = new TH2D("histx2", "histx2", bmod_bin[0], bmod_view[0], bmod_view[1], bmod_bin[0],bmod_r[0],bmod_r[1]);
    TH2D * histy1 = new TH2D("histy1", "histy1", bmod_bin[0], bmod_view[0], bmod_view[1], bmod_bin[0],bmod_r[0],bmod_r[1]);
    TH2D * histy2 = new TH2D("histy2", "histy2", bmod_bin[0], bmod_view[0], bmod_view[1], bmod_bin[0],bmod_r[0],bmod_r[1]);
    TH2D * histramp = new TH2D("histramp", "histramp", bmod_bin[0], bmod_view[0], bmod_view[1], bmod_bin[0],bmod_r[0],bmod_r[1]);

    pad131->cd(1);
    tm->Draw("(76.29e-6)*ramp:event_number*0.001>>histramp", cut);
    histramp = (TH2D *)gDirectory->Get("histramp");
    histramp->SetXTitle("TIME [s]");
    histramp->SetYTitle("Ramp [V]");
    histramp->GetYaxis()->SetRangeUser(-0.4, 0.4);
    histramp->Draw("");
    gPad->Update();

    pad131->cd(2);
    tm->Draw("(76.29e-6)*fgx1:event_number*0.001>>histx1", cut);
    histx1 = (TH2D *)gDirectory->Get("histx1");
    histx1->SetXTitle("TIME [s]");
    histx1->SetYTitle("FGX1 [V]");
    histx1->GetYaxis()->SetRangeUser(-0.4, 0.4);
    histx1->SetMarkerColor(kRed-4);
    histx1->Draw("");
    gPad->Update();

    pad131->cd(3);
    tm->Draw("(76.29e-6)*fgx2:event_number*0.001>>histx2", cut);
    histx2 = (TH2D *)gDirectory->Get("histx2");
    histx2->SetXTitle("TIME [s]");
    histx2->SetYTitle("FGX2 [V]");
    histx2->GetYaxis()->SetRangeUser(-0.4, 0.4);
    histx2->SetMarkerColor(kRed-4);
    histx2->Draw("");
    gPad->Update();

    pad131->cd(4);
    tm->Draw("(76.29e-6)*fge:event_number*0.001>>histe", cut);
    histe = (TH2D *)gDirectory->Get("histe");
    histe->SetXTitle("TIME [s]");
    histe->SetYTitle("FGE [V]");
    histe->GetYaxis()->SetRangeUser(-0.4, 0.4);
    histe->SetMarkerColor(kBlue);
    histe->Draw("");
    gPad->Update();

    pad131->cd(5);
    tm->Draw("(76.29e-6)*fgy1:event_number*0.001>>histy1", cut);
    histy1 = (TH2D *)gDirectory->Get("histy1");
    histy1->SetXTitle("TIME [s]");
    histy1->SetYTitle("FGY1 [V]");
    histy1->GetYaxis()->SetRangeUser(-0.4, 0.4);
    histy1->SetMarkerColor(kGreen-3);
    histy1->Draw("");
    gPad->Update();

    pad131->cd(6);
    tm->Draw("(76.29e-6)*fgy2:event_number*0.001>>histy2", cut);
    histy2 = (TH2D *)gDirectory->Get("histy2");
    histy2->SetXTitle("TIME [s]");
    histy2->SetYTitle("FGY2 [V]");
    histy2->GetYaxis()->SetRangeUser(-0.4, 0.4);
    //     histy2->GetXaxis()->SetTitleColor(2);
    //     histy2->GetYaxis()->SetTitleColor(2);
    histy2->SetMarkerColor(kGreen-3);
    histy2->Draw("");
    gPad->Update();

    c13->Update(); c13->SaveAs(pmodulation);}

  gStyle->SetTitleSize(0.055);
  gStyle->SetTitleYSize(0.055);
  gStyle->SetTitleXSize(0.055);
  gStyle->SetTitleYOffset(0.8);
  gStyle->SetTitleXOffset(0.8);
  gStyle->SetLabelSize(0.045);
  gStyle->SetLabelSize(0.045,"y");
  gStyle->SetLabelSize(0.052,"x");
  gStyle->SetOptStat(1);
  gStyle->SetOptStat("emr");
  /****************************************************************************/
  if (BPMS){
    TCanvas *c4 = new TCanvas("c4", CanvasTitle("BPM Difference (mm)"),60,60,csizx,csizy);
    TPad* pad41 = NewFramedPad(kOrange-9, kWhite, "BPM Difference (mm)");
    //     pad40 = new TPad("pad40","pad40",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad41 = new TPad("pad41","pad41",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad40->SetFillColor(kOrange-9);
    //   pad41->SetFillColor(kWhite);
    //   pad40->Draw();
    //   pad41->Draw();
    //   pad40->cd();
    //   DrawCanvasHeader("BPM Difference (mm)");
    pad41->cd();
    pad41->Divide(2,4);
    printf("%sPlotting %sBPM Difference (mm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    char hdiffbpmx[255],hdiffbpmy[255]; //,tdiffbpmx[255];

    const Int_t bpmNUM=5,bpmNUM2=6;
    Double_t difX[bpmNUM],difXError[bpmNUM],difXRMS[bpmNUM],difY[bpmNUM],difYError[bpmNUM],difYRMS[bpmNUM];

    TH1* hdiffx;
    TH1* hdiffy;
    for ( int k=0; k<4; k++) {

      sprintf(bpmx,"diff_qwk_%sX>>difx%d",bpms[k].Data(),k);
      sprintf(bpmy,"diff_qwk_%sY>>dify%d",bpms[k].Data(),k);
      sprintf(cutbpmx,"%s && diff_qwk_%sX.%s",s1.Data(),bpms[k].Data(),s2.Data());
      sprintf(cutbpmy,"%s && diff_qwk_%sY.%s",s1.Data(),bpms[k].Data(),s2.Data());
      sprintf(hdiffbpmx,"difx%d",k);
      sprintf(hdiffbpmy,"dify%d",k);

      pad41->cd(position3[k]);
      th->Draw(bpmx,cutbpmx,"goff");
      hdiffx = (TH1F *)gDirectory->Get(hdiffbpmx);
      hdiffx->SetYTitle(tarbitrary);
      hdiffx->SetXTitle(Form("%s-X [mm]",bpms[k].Data()));
      hdiffx->SetTitle(Form("%s-X",bpms[k].Data()));
      hdiffx->SetLineColor(kRed);
      hdiffx->Draw();

      pad41->cd(position4[k]);
      th->Draw(bpmy,cutbpmy,"goff");
      hdiffy = (TH1F *)gDirectory->Get(hdiffbpmy);
      hdiffy->SetYTitle(tarbitrary);
      hdiffy->SetXTitle(Form("%s-Y [mm]",bpms[k].Data()));
      hdiffy->SetTitle(Form("%s-Y",bpms[k].Data()));
      hdiffy->SetLineColor(kGreen);
      hdiffy->Draw();

      difX[k]       =  hdiffx->GetMean();
      difXError[k]  =  hdiffx->GetMeanError();
      difXRMS[k]    =  hdiffx->GetRMS();
      difY[k]       =  hdiffy->GetMean();
      difYError[k]  =  hdiffy->GetMeanError();
      difYRMS[k]    =  hdiffy->GetRMS();

      printf("%s\t %4.2g+-%4.2g %4.2g\t %4.2g+-%4.2g %4.2g\n", 
	     bpms[k].Data(),difX[k],difXError[k],difXRMS[k],difY[k],difYError[k],difYRMS[k]);

      gPad->Update();
    }
    c4->Update(); c4->SaveAs(pbpmd);
  }

  /****************************************************************************/
  /*Plot bpm effective charge*/
  if(BPM_EFAQ){
    TCanvas *c20 = new TCanvas("c20", CanvasTitle("BPM Effective Charge Asymmetry (ppm)"),40,40,csizx,csizy);
    TPad* pad20 = NewFramedPad(kOrange-9, kWhite, "BPM Effective Charge Asymmetry (ppm)");
    printf("%sPlotting %sBPM effective charge asymmetry (ppm)%s\n",blue,red,normal);
    PlotBPM_effectiveq_asyms(th, pad20);
    c20->Update(); 
    c20->SaveAs(pbpmefcharge);
  }


  gStyle->SetTitleSize(0.04);
  gStyle->SetTitleYSize(0.04);
  gStyle->SetTitleXSize(0.04);
  gStyle->SetTitleYOffset(1.1);
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetLabelSize(0.050);
  gStyle->SetLabelSize(0.050,"y");
  gStyle->SetLabelSize(0.050,"x");
  gStyle->SetCanvasColor(kRed-10);
  /****************************************************************************/
  if (MDYIELDVAR){
    TCanvas *c14 = new TCanvas("c14", CanvasTitle("Main Detector Barsum Yield Variation"),90,90,csizx,csizy);
    TPad* pad141 = NewFramedPad(kRed-10, kWhite, "Main Detector Barsum Yield Variation");
    pad141->cd();
    pad141->Divide(3,3);
    printf("%sPlotting %sMain Detector Barsum Yield Variation%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    char  hdetvar[255], tydetvar[255], tyalldetvar[255];
    //  char hadet[255], hadetall[255], txadet[255], tyadet[255], txalladet[255];
    //  Double_t mdt_varr[2] = { -0.0300, 0.0450};
 
    sprintf(tyalldetvar,"MD All YIELD");
    int ld=0;
    TH1* hyvar;
    TH1* hvarall;
    for ( int l=0; l<8; l++) {
      ld = l+1;

      sprintf(ydetvar,"yield_qwk_md%dbarsum:mps_counter*0.001>>hyvar%d",ld,ld);
      sprintf(cutydetvar,"%s && yield_qwk_md%dbarsum.%s",s1.Data(),ld,s2.Data());
      sprintf(hdetvar,"hyvar%d",ld);
      sprintf(tydetvar,"MD %d YIELD",ld);

      pad141->cd(position5[ld]);
      pad141->cd(position5[ld])->SetGrid();
      th->Draw(ydetvar,cutydetvar,"goff");
      hyvar = (TH2F *)gDirectory->Get(hdetvar);
      hyvar->SetYTitle(tydetvar);
      hyvar->SetXTitle(txdetvar);
      hyvar->SetTitle(tydetvar);
      hyvar->Draw();
      gPad->Update();
    }

    pad141->cd(5);
    pad141->cd(5)->SetGrid();
    th->Draw("yield_qwk_mdallbars:mps_counter*0.001>>hvarall",Form("%s && yield_qwk_mdallbars.%s",s1.Data(),s2.Data()));
    hvarall = GetHist("hvarall");
    hvarall->SetFillColor(kRed-2);
    hvarall->SetMarkerColor(kRed-2);
    //     hvarall->SetFillStyle(3013);
    hvarall->SetYTitle(tyalldetvar);
    hvarall->SetXTitle(txdetvar);
    hvarall->SetTitle(tyalldetvar);
    hvarall->Draw();

    gPad->Update();
    c14->Update(); c14->SaveAs(pmdyieldvar);}
  /****************************************************************************/
  if (MDBKG){
    TCanvas *c18 = new TCanvas("c18", CanvasTitle("Main Detector Background Yields[V/uA] & Asymmetries [ppm]"),100,100,csizx,csizy);
    TPad* pad181 = NewFramedPad(kRed-10, kWhite, "Main Detector Background Yields[V/uA] & Asymmetries [ppm]");
    pad181->cd();
    pad181->Divide(4,3);
    printf("%sPlotting %sMain Detector Background Yields[V/uA] & Asymmetries [ppm]%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    char amdbkg[255],cutamdbkg[255],ymdbkg[255],cutymdbkg[255],hamdbkg[255],hymdbkg[255];
    int xd=0;
    TH1* hybkg;
    TH1* habkg;

    const Int_t bkgNUM=7;
    Double_t abkg[bkgNUM],abkgError[bkgNUM],abkgRMS[bkgNUM],ybkg[bkgNUM],ybkgError[bkgNUM],ybkgRMS[bkgNUM];

    for ( int x=0; x<6; x++) {
      xd = x+1;

      sprintf(amdbkg,"asym_qwk_%s*1e6>>hab%d",mdbkg[x].Data(),xd);
      sprintf(cutamdbkg,"%s && asym_qwk_%s.%s",s1.Data(),mdbkg[x].Data(),s2.Data());
      sprintf(ymdbkg,"yield_qwk_%s>>hyb%d",mdbkg[x].Data(),xd);
      sprintf(cutymdbkg,"%s && yield_qwk_%s.%s",s1.Data(),mdbkg[x].Data(),s2.Data());
      sprintf(hamdbkg,"hab%d",xd);
      sprintf(hymdbkg,"hyb%d",xd);

      pad181->cd(position13[x]);
      th->Draw(ymdbkg,cutymdbkg,"goff");
      hybkg = (TH1F *)gDirectory->Get(hymdbkg);
      hybkg->SetYTitle(tarbitrary);
      if(xd==5||xd==6){
	hybkg->SetXTitle(Form("MD %s YIELD [V]",mdbkg[x].Data()));
	hybkg->SetTitle(Form("MD %s YIELD [V]",mdbkg[x].Data()));
      }
      else{
	hybkg->SetXTitle(Form("MD %s YIELD [V/uA]",mdbkg[x].Data()));
	hybkg->SetTitle(Form("MD %s YIELD [V/uA]",mdbkg[x].Data()));
      }
      hybkg->Draw();
      gPad->Update();

      pad181->cd(position14[x]);
      th->Draw(amdbkg,cutamdbkg,"goff");
      habkg = (TH1F *)gDirectory->Get(hamdbkg);
      habkg->SetYTitle(tarbitrary);
      habkg->SetXTitle(Form("MD %s ASYM [ppm]",mdbkg[x].Data()));
      habkg->SetTitle(Form("MD %s ASYM [ppm]",mdbkg[x].Data()));

      habkg->SetFillColor(kBlack);
      habkg->SetFillStyle(3013);
      habkg->Draw();
      gPad->Update();

      abkg[x]       =  habkg->GetMean();
      abkgError[x]  =  habkg->GetMeanError();
      abkgRMS[x]    =  habkg->GetRMS();
      ybkg[x]       =  hybkg->GetMean();
      ybkgError[x]  =  hybkg->GetMeanError();
      ybkgRMS[x]    =  hybkg->GetRMS();

      printf("%s\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n", 
	     mdbkg[x].Data(),abkg[x],abkgError[x],abkgRMS[x],ybkg[x],ybkgError[x],ybkgRMS[x]);


    }

    c18->Update(); c18->SaveAs(pmdbkg);}
  /****************************************************************************/
  if (MDALLASYM){

    Double_t amdAll1,amdAll1Error,amdAll1RMS;
    Double_t amdAll2,amdAll2Error,amdAll2RMS;
    Double_t amdAll5,amdAll5Error,amdAll5RMS;
    Double_t amdAll6,amdAll6Error,amdAll6RMS;
    Double_t amdAll7,amdAll7Error,amdAll7RMS;
    Double_t amdAll8,amdAll8Error,amdAll8RMS;


    TCanvas *c6 = new TCanvas("c6", CanvasTitle("Main Detector All Barsum Asymmetry w.r.t BCMs (ppm)"),110,110,csizx,csizy);
    TPad* pad61 = NewFramedPad(kRed-10, kWhite, "Main Detector All Barsum Asymmetry (ppm) Normalized to Different BCMs");
    //   pad60 = new TPad("pad60","pad60",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad61 = new TPad("pad61","pad61",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad60->SetFillColor(kRed-10);
    //   pad61->SetFillColor(kWhite);
    //   pad60->Draw();
    //   pad61->Draw();
    //   pad60->cd();
    //   DrawCanvasHeader("Main Detector All Barsum Asymmetry (ppm) Normalized to Different BCMs");
    pad61->cd();
    pad61->Divide(2,3);
    printf("%sPlotting %sMain Detector All Barsum Asymmetries (ppm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    Double_t mdt_r[2] = { -1700, 1700};
    Int_t    mdt_bin[1] = {100};
    TH1F * hmdbcm1  = new TH1F("hmdbcm1", "hmdbcm1", mdt_bin[0],mdt_r[0],mdt_r[1]);
    TH1F * hmdbcm2  = new TH1F("hmdbcm2", "hmdbcm2", mdt_bin[0],mdt_r[0],mdt_r[1]);
    TH1F * hmdbcm5  = new TH1F("hmdbcm5", "hmdbcm5", mdt_bin[0],mdt_r[0],mdt_r[1]);
    TH1F * hmdbcm6  = new TH1F("hmdbcm6", "hmdbcm6", mdt_bin[0],mdt_r[0],mdt_r[1]);
    TH1F * hmdbcm7  = new TH1F("hmdbcm7", "hmdbcm7", mdt_bin[0],mdt_r[0],mdt_r[1]);
    TH1F * hmdbcm8  = new TH1F("hmdbcm8", "hmdbcm8", mdt_bin[0],mdt_r[0],mdt_r[1]);
    //   TH1F * hmdeven  = new TH1F("hmdeven", "hmdeven", mdt_bin[0],mdt_r[0],mdt_r[1]);
    //   TH1F * hmdodd  = new TH1F("hmdodd", "hmdodd", mdt_bin[0],mdt_r[0],mdt_r[1]);

    pad61->cd(1);
    gPad->SetLogy();
    th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm1)*1e6>>hmdbcm1",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_bcm1.%s && asym_qwk_charge.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
    hmdbcm1 = (TH1F *)gDirectory->Get("hmdbcm1");
    hmdbcm1->SetYTitle(tarbitrary);
    hmdbcm1->SetXTitle("MD BARSUM ASYM [ppm]");
    hmdbcm1->SetTitle("MD BARSUM ASYM normalized to BCM1");
    hmdbcm1->Draw();

    amdAll1      = hmdbcm1->GetMean();
    amdAll1Error = hmdbcm1->GetMeanError();
    amdAll1RMS   = hmdbcm1->GetRMS();

    gPad->Modified();
    gPad->Update();

    pad61->cd(2);
    gPad->SetLogy();
    th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm2)*1e6>>hmdbcm2",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm2.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
    hmdbcm2 = (TH1F *)gDirectory->Get("hmdbcm2");
    hmdbcm2->SetYTitle(tarbitrary);
    hmdbcm2->SetXTitle("MD BARSUM ASYM [ppm]");
    hmdbcm2->SetTitle("MD BARSUM ASYM normalized to BCM2");
    hmdbcm2->Draw();

    amdAll2      = hmdbcm2->GetMean();
    amdAll2Error = hmdbcm2->GetMeanError();
    amdAll2RMS   = hmdbcm2->GetRMS();

    gPad->Update();

    pad61->cd(3);
    gPad->SetLogy();
    th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm5)*1e6>>hmdbcm5",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm5.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
    hmdbcm5 = (TH1F *)gDirectory->Get("hmdbcm5");
    hmdbcm5->SetYTitle(tarbitrary);
    hmdbcm5->SetXTitle("MD BARSUM ASYM [ppm]");
    hmdbcm5->SetTitle("MD BARSUM ASYM normalized to BCM5");
    hmdbcm5->Draw();

    amdAll5      = hmdbcm5->GetMean();
    amdAll5Error = hmdbcm5->GetMeanError();
    amdAll5RMS   = hmdbcm5->GetRMS();

    gPad->Update();

    pad61->cd(4);
    gPad->SetLogy();
    th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm6)*1e6>>hmdbcm6",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm6.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
    hmdbcm6 = (TH1F *)gDirectory->Get("hmdbcm6");
    hmdbcm6->SetYTitle(tarbitrary);
    hmdbcm6->SetXTitle("MD BARSUM ASYM [ppm]");
    hmdbcm6->SetTitle("MD BARSUM ASYM normalized to BCM6");
    hmdbcm6->Draw();

    amdAll6      = hmdbcm6->GetMean();
    amdAll6Error = hmdbcm6->GetMeanError();
    amdAll6RMS   = hmdbcm6->GetRMS();

    gPad->Update();

    if(gRunNumber>13000){
      pad61->cd(5);
      gPad->SetLogy();
      th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm7)*1e6>>hmdbcm7",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm7.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
      hmdbcm7 = (TH1F *)gDirectory->Get("hmdbcm7");
      hmdbcm7->SetYTitle(tarbitrary);
      hmdbcm7->SetXTitle("MD BARSUM ASYM [ppm]");
      hmdbcm7->SetTitle("MD BARSUM ASYM normalized to BCM7");
      hmdbcm7->Draw();

      amdAll7      = hmdbcm7->GetMean();
      amdAll7Error = hmdbcm7->GetMeanError();
      amdAll7RMS   = hmdbcm7->GetRMS();

      gPad->Update();

      pad61->cd(6);
      gPad->SetLogy();
      th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm8)*1e6>>hmdbcm8",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm8.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
      hmdbcm8 = (TH1F *)gDirectory->Get("hmdbcm8");
      hmdbcm8->SetYTitle(tarbitrary);
      hmdbcm8->SetXTitle("MD BARSUM ASYM [ppm]");
      hmdbcm8->SetTitle("MD BARSUM ASYM normalized to BCM8");
      hmdbcm8->Draw();

      amdAll8      = hmdbcm8->GetMean();
      amdAll8Error = hmdbcm8->GetMeanError();
      amdAll8RMS   = hmdbcm8->GetRMS();

      gPad->Update();
    }
    else{
      pad61->cd(5);
      gPad->SetLogy();
      th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm1)*1e6>>hmdbcm7",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm1.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
      hmdbcm7 = (TH1F *)gDirectory->Get("hmdbcm7");
      hmdbcm7->SetYTitle(tarbitrary);
      hmdbcm7->SetXTitle("MD BARSUM ASYM [ppm]");
      hmdbcm7->SetTitle("MD BARSUM ASYM normalized to BCM1");
      hmdbcm7->Draw();

      amdAll7      = hmdbcm7->GetMean();
      amdAll7Error = hmdbcm7->GetMeanError();
      amdAll7RMS   = hmdbcm7->GetRMS();

      gPad->Update();

      pad61->cd(6);
      gPad->SetLogy();
      th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-asym_qwk_bcm2)*1e6>>hmdbcm8",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm2.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
      hmdbcm8 = (TH1F *)gDirectory->Get("hmdbcm8");
      hmdbcm8->SetYTitle(tarbitrary);
      hmdbcm8->SetXTitle("MD BARSUM ASYM [ppm]");
      hmdbcm8->SetTitle("MD BARSUM ASYM normalized to BCM2");
      hmdbcm8->Draw();

      amdAll8      = hmdbcm8->GetMean();
      amdAll8Error = hmdbcm8->GetMeanError();
      amdAll8RMS   = hmdbcm8->GetRMS();

      gPad->Update();
    }

    //   pad61->cd(7);
    //   gPad->SetLogy();
    //   th->Draw("asym_qwk_mdevenbars*1e6>>hmdeven",Form("%s && asym_qwk_mdevenbars.%s",s1.Data(),s2.Data()),"goff");
    //   hmdeven = (TH1F *)gDirectory->Get("hmdeven");
    //   hmdeven->SetYTitle(tarbitrary);
    //   hmdeven->SetXTitle("MD EVEN BAR ASYM [ppm]");
    //   hmdeven->SetTitle("MD EVEN BAR ASYM");
    //   hmdeven->Draw();
    //   gPad->Update();

    //   pad61->cd(8);
    //   gPad->SetLogy();
    //   th->Draw("asym_qwk_mdoddbars*1e6>>hmdodd",Form("%s && asym_qwk_mdoddbars.%s",s1.Data(),s2.Data()),"goff");
    //   hmdodd = (TH1F *)gDirectory->Get("hmdodd");
    //   hmdodd->SetYTitle(tarbitrary);
    //   hmdodd->SetXTitle("MD ODD BAR ASYM [ppm]");
    //   hmdodd->SetTitle("MD ODD BAR ASYM");
    //   hmdodd->Draw();

    printf("bcm1\t %4.2f+-%4.2f %4.2f\n",amdAll1,amdAll1Error,amdAll1RMS);
    printf("bcm2\t %4.2f+-%4.2f %4.2f\n",amdAll2,amdAll2Error,amdAll2RMS);
    printf("bcm5\t %4.2f+-%4.2f %4.2f\n",amdAll5,amdAll5Error,amdAll5RMS);
    printf("bcm6\t %4.2f+-%4.2f %4.2f\n",amdAll6,amdAll6Error,amdAll6RMS);
    printf("bcm7\t %4.2f+-%4.2f %4.2f\n",amdAll7,amdAll7Error,amdAll7RMS);
    printf("bcm8\t %4.2f+-%4.2f %4.2f\n",amdAll8,amdAll8Error,amdAll8RMS);

    gPad->Update();
    c6->Update(); c6->SaveAs(pmdallasym);}
  /************************************************************************/
  gStyle->SetStatFontSize(0.070);
  gStyle->SetStatY(1.0);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.30);
  gStyle->SetStatH(0.30); 
  /************************************************************************/
  if (SENSITIVITY){
    TCanvas *c7 = new TCanvas("c7", CanvasTitle("Main Detector All Barsum X-Sensitivities (ppm/mm)"),120,120,1400,1000);
    TPad* pad71 = NewFramedPad(kRed-10, kWhite, "Main Detector Barsum X-Sensitivities (ppm/mm)");
    //   pad70 = new TPad("pad70","pad70",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad71 = new TPad("pad71","pad71",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad70->SetFillColor(kRed-10);
    //   pad71->SetFillColor(kWhite);
    //   pad70->Draw();
    //   pad71->Draw();
    //   pad70->cd();
    //   DrawCanvasHeader("Main Detector Barsum X-Sensitivities (ppm/mm)");
    pad71->cd();
    pad71->Divide(4,3);
    printf("%sPlotting %sMain Detector Barsum X-Sensitivities (ppm/mm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    TCanvas *c8 = new TCanvas("c8", CanvasTitle("Main Detector All Barsum Y-Sensitivities (ppm/mm)"),140,140,1400,1000);
    TPad* pad81 = NewFramedPad(kRed-10, kWhite, "Main Detector Barsum Y-Sensitivities (ppm/mm)");
    //   pad80 = new TPad("pad80","pad80",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad81 = new TPad("pad81","pad81",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad80->SetFillColor(kRed-10);
    //   pad81->SetFillColor(kWhite);
    //   pad80->Draw();
    //   pad81->Draw();
    //   pad80->cd();
    //   DrawCanvasHeader("Main Detector Barsum Y-Sensitivities (ppm/mm)");
    pad81->cd();
    pad81->Divide(4,3);
    printf("%sPlotting %sMain Detector Barsum Y-Sensitivities (ppm/mm)%s\n",blue,red,normal);
    gStyle->SetCanvasColor(kBlue-10);
    /* ----------------------------------------------------------------------- */
    TCanvas *c11 = new TCanvas("c11", CanvasTitle("Downstream Lumi X-Sensitivities (ppm/mm)"),200,200,1400,1000);
    TPad* pad111 = NewFramedPad(kBlue-10, kWhite, "Downstream Lumi X-Sensitivities (ppm/mm)");
    //   pad110 = new TPad("pad110","pad110",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad111 = new TPad("pad111","pad111",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad110->SetFillColor(kBlue-10);
    //   pad111->SetFillColor(kWhite);
    //   pad110->Draw();
    //   pad111->Draw();
    //   pad110->cd();
    //   DrawCanvasHeader("Downstream Lumi X-Sensitivities (ppm/mm)");
    pad111->cd();
    pad111->Divide(4,3);
    printf("%sPlotting %sDownstream Lumi X-Sensitivities (ppm/mm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    TCanvas *c12 = new TCanvas("c12", CanvasTitle("Downstream Lumi Y-Sensitivities (ppm/mm)"),220,220,1400,1000);
    TPad* pad121 = NewFramedPad(kBlue-10, kWhite, "Downstream Lumi Y-Sensitivities (ppm/mm)");
    //   pad120 = new TPad("pad120","pad120",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad121 = new TPad("pad121","pad121",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad120->SetFillColor(kBlue-10);
    //   pad121->SetFillColor(kWhite);
    //   pad120->Draw();
    //   pad121->Draw();
    //   pad120->cd();
    //   DrawCanvasHeader("Downstream Lumi Y-Sensitivities (ppm/mm)");
    pad121->cd();
    pad121->Divide(4,3);
    printf("%sPlotting %sDownstream Lumi Y-Sensitivities (ppm/mm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    gStyle -> SetOptFit(1111);
    gStyle -> SetOptStat("e");

    TF1* fitMDX        =  new TF1("fitMDX","pol1",fit_range[0], fit_range[1]);
    TF1* fitMDY        =  new TF1("fitMDY","pol1",fit_range[0], fit_range[1]);
    TF1* fitDsLumiX    =  new TF1("fitDsLumiX","pol1",fit_range[0], fit_range[1]);
    TF1* fitDsLumiY    =  new TF1("fitDsLumiY","pol1",fit_range[0], fit_range[1]);
    TF1* fitMDAllX     =  new TF1("fitMDAllX","pol1",fit_range[0], fit_range[1]);
    TF1* fitMDAllY     =  new TF1("fitMDAllY","pol1",fit_range[0], fit_range[1]);
    TF1* fitDsLumiSumX =  new TF1("fitDsLumiSumX","pol1",fit_range[0], fit_range[1]);
    TF1* fitDsLumiSumY =  new TF1("fitDsLumiSumY","pol1",fit_range[0], fit_range[1]);

    const Int_t senNUM = 8,senNUM2 = 4;
    Double_t senMDXp0[senNUM],senMDXp0Error[senNUM],senMDXp1[senNUM],senMDXp1Error[senNUM],senMDXChi[senNUM],senMDXNDF[senNUM],senMDXProb[senNUM];
    Double_t senMDYp0[senNUM],senMDYp0Error[senNUM],senMDYp1[senNUM],senMDYp1Error[senNUM],senMDYChi[senNUM],senMDYNDF[senNUM],senMDYProb[senNUM];
    Double_t senDsLumiXp0[senNUM],senDsLumiXp0Error[senNUM],senDsLumiXp1[senNUM],senDsLumiXp1Error[senNUM],senDsLumiXChi[senNUM],senDsLumiXNDF[senNUM],senDsLumiXProb[senNUM];
    Double_t senDsLumiYp0[senNUM],senDsLumiYp0Error[senNUM],senDsLumiYp1[senNUM],senDsLumiYp1Error[senNUM],senDsLumiYChi[senNUM],senDsLumiYNDF[senNUM],senDsLumiYProb[senNUM];

    Double_t senMDAllXp0[senNUM2],senMDAllXp0Error[senNUM2],senMDAllXp1[senNUM2],senMDAllXp1Error[senNUM2],senMDAllXChi[senNUM2],senMDAllXNDF[senNUM2],senMDAllXProb[senNUM2];
    Double_t senMDAllYp0[senNUM2],senMDAllYp0Error[senNUM2],senMDAllYp1[senNUM2],senMDAllYp1Error[senNUM2],senMDAllYChi[senNUM2],senMDAllYNDF[senNUM2],senMDAllYProb[senNUM2];
    Double_t senDsLumiSumXp0[senNUM2],senDsLumiSumXp0Error[senNUM2],senDsLumiSumXp1[senNUM2],senDsLumiSumXp1Error[senNUM2],senDsLumiSumXChi[senNUM2],senDsLumiSumXNDF[senNUM2],senDsLumiSumXProb[senNUM2];
    Double_t senDsLumiSumYp0[senNUM2],senDsLumiSumYp0Error[senNUM2],senDsLumiSumYp1[senNUM2],senDsLumiSumYp1Error[senNUM2],senDsLumiSumYChi[senNUM2],senDsLumiSumYNDF[senNUM2],senDsLumiSumYProb[senNUM2];

    int dt=0;
    for ( int t=0; t<8; t++) {
      dt = t+1;

      sprintf(adetsenx,"asym_qwk_md%dbarsum*1e6:diff_qwk_targetX>>htx%d",dt,dt);
      sprintf(cutadetsenx,"%s && asym_qwk_md%dbarsum.%s && diff_qwk_targetX.%s",s1.Data(),dt,s2.Data(),s2.Data());
      sprintf(histox,"htx%d",dt);
      sprintf(yaxis,"MD%d ASYM [ppm]",dt);
      sprintf(xaxis,"TARGET X [mm]");

      pad71->cd(position6[dt]);
      pad71->cd(position6[dt])->SetGrid();

      th->Draw(adetsenx,cutadetsenx,"prof");
      TH1D *htxf = (TH1D*)gDirectory->Get(histox);
      htxf->GetYaxis()->SetTitle(yaxis);
      htxf->GetXaxis()->SetTitle(xaxis);
      htxf->SetTitle(yaxis);
      htxf->Fit("fitMDX","E M R F Q","",fit_range[0], fit_range[1]);
      htxf->SetLineColor(kRed);
      htxf->Draw("");
      gPad->Update();

      TPaveStats *st = (TPaveStats*)htxf->FindObject("stats");
      st->SetX1NDC(0.52); //new x start position
      st->SetX2NDC(0.99);    //new x end position
      st->SetY1NDC(0.72); //new y start position
      st->SetY2NDC(1);    //new y end position
      st->SetBorderSize(2);
      st->SetFillColor(19);
      st->SetTextAlign(12);
      st->Draw("same");

      gPad->Modified();
      gPad->Update();

      sprintf(adetseny,"asym_qwk_md%dbarsum*1e6:diff_qwk_targetY>>hty%d",dt,dt);
      sprintf(cutadetseny,"%s && asym_qwk_md%dbarsum.%s && diff_qwk_targetY.%s",s1.Data(),dt,s2.Data(),s2.Data());
      sprintf(histoy,"hty%d",dt);
      sprintf(xaxis1,"TARGET Y [mm]");

      pad81->cd(position6[dt]);
      pad81->cd(position6[dt])->SetGrid();

      th->Draw(adetseny,cutadetseny,"prof");
      TH1D *htyf = (TH1D*)gDirectory->Get(histoy);
      htyf->GetYaxis()->SetTitle(yaxis);
      htyf->GetXaxis()->SetTitle(xaxis1);
      htyf->SetTitle(yaxis);
      htyf->Fit("fitMDY","E M R F Q","",fit_range[0], fit_range[1]);
      htyf->SetLineColor(kGreen);
      htyf->Draw("");

      gPad->Modified();
      gPad->Update();

      pad111->cd(position6[dt]);
      pad111->cd(position6[dt])->SetGrid();

      sprintf(alumisenx,"asym_qwk_dslumi%d*1e6:diff_qwk_targetX>>hlx%d",dt,dt);
      sprintf(cutalumisenx,"%s && asym_qwk_dslumi%d.%s && diff_qwk_targetX.%s",s1.Data(),dt,s2.Data(),s2.Data());
      sprintf(histolx,"hlx%d",dt);
      sprintf(yaxisl,"DSLUMI%d ASYM [ppm]",dt);

      th->Draw(alumisenx,cutalumisenx,"prof");
      TH1D *hlxf = (TH1D*)gDirectory->Get(histolx);
      hlxf->GetYaxis()->SetTitle(yaxisl);
      hlxf->GetXaxis()->SetTitle(xaxis);
      hlxf->SetTitle(yaxisl);
      hlxf->Fit("fitDsLumiX","E M R F Q","",fit_range[0], fit_range[1]);
      hlxf->SetLineColor(kRed);
      hlxf->Draw("");

      gPad->Modified();
      gPad->Update();

      pad121->cd(position6[dt]);
      pad121->cd(position6[dt])->SetGrid();

      sprintf(alumiseny,"asym_qwk_dslumi%d*1e6:diff_qwk_targetY>>hly%d",dt,dt);
      sprintf(cutalumiseny,"%s && asym_qwk_dslumi%d.%s && diff_qwk_targetY.%s",s1.Data(),dt,s2.Data(),s2.Data());
      sprintf(histoly,"hly%d",dt);

      th->Draw(alumiseny,cutalumiseny,"prof");
      TH1D *hlyf = (TH1D*)gDirectory->Get(histoly);
      hlyf->GetYaxis()->SetTitle(yaxisl);
      hlyf->GetXaxis()->SetTitle(xaxis1);
      hlyf->SetTitle(yaxisl);
      hlyf->Fit("fitDsLumiY","E M R F Q","",fit_range[0], fit_range[1]);
      hlyf->SetLineColor(kGreen);
      hlyf->Draw("");

      gPad->Modified();
      gPad->Update();


      senMDXp0[t]          =  fitMDX->GetParameter(0);
      senMDXp0Error[t]     =  fitMDX->GetParError(0);
      senMDXp1[t]          =  fitMDX->GetParameter(1);
      senMDXp1Error[t]     =  fitMDX->GetParError(1);
      senMDXChi[t]         =  fitMDX->GetChisquare();
      senMDXNDF[t]         =  fitMDX->GetNDF();
      senMDXProb[t]        =  fitMDX->GetProb();

      senMDYp0[t]          =  fitMDY->GetParameter(0);
      senMDYp0Error[t]     =  fitMDY->GetParError(0);
      senMDYp1[t]          =  fitMDY->GetParameter(1);
      senMDYp1Error[t]     =  fitMDY->GetParError(1);
      senMDYChi[t]         =  fitMDY->GetChisquare();
      senMDYNDF[t]         =  fitMDY->GetNDF();
      senMDYProb[t]        =  fitMDY->GetProb();

      senDsLumiXp0[t]      =  fitDsLumiX->GetParameter(0);
      senDsLumiXp0Error[t] =  fitDsLumiX->GetParError(0);
      senDsLumiXp1[t]      =  fitDsLumiX->GetParameter(1);
      senDsLumiXp1Error[t] =  fitDsLumiX->GetParError(1);
      senDsLumiXChi[t]     =  fitDsLumiX->GetChisquare();
      senDsLumiXNDF[t]     =  fitDsLumiX->GetNDF();
      senDsLumiXProb[t]    =  fitDsLumiX->GetProb();

      senDsLumiYp0[t]      =  fitDsLumiY->GetParameter(0);
      senDsLumiYp0Error[t] =  fitDsLumiY->GetParError(0);
      senDsLumiYp1[t]      =  fitDsLumiY->GetParameter(1);
      senDsLumiYp1Error[t] =  fitDsLumiY->GetParError(1);
      senDsLumiYChi[t]     =  fitDsLumiY->GetChisquare();
      senDsLumiYNDF[t]     =  fitDsLumiY->GetNDF();
      senDsLumiYProb[t]    =  fitDsLumiY->GetProb();


      printf("MD%d SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     dt,senMDXp0[t],senMDXp0Error[t],senMDXp1[t],senMDXp1Error[t],senMDXChi[t],senMDXNDF[t],senMDXProb[t]);
      printf("MD%d SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     dt,senMDYp0[t],senMDYp0Error[t],senMDYp1[t],senMDYp1Error[t],senMDYChi[t],senMDYNDF[t],senMDYProb[t]);
      printf("DsLumi%d SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     dt,senDsLumiXp0[t],senDsLumiXp0Error[t],senDsLumiXp1[t],senDsLumiXp1Error[t],senDsLumiXChi[t],senDsLumiXNDF[t],senDsLumiXProb[t]);
      printf("DsLumi%d SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     dt,senDsLumiYp0[t],senDsLumiYp0Error[t],senDsLumiYp1[t],senDsLumiYp1Error[t],senDsLumiYChi[t],senDsLumiYNDF[t],senDsLumiYProb[t]);


    }
    /* ----------------------------------------------------------------------- */
    int dv=0;
    for ( int v=0; v<3; v++) {
      dv = v+1;

      pad71->cd(position7[v]);
      pad71->cd(position7[v])->SetGrid();
      // pad71->cd(position7[v])->SetFillColor(kGreen-10);

      sprintf(adetasenx,"asym_qwk_md%sbars*1e6:diff_qwk_targetX>>h1x%d",allmd[v].Data(),v);
      sprintf(cutadetasenx,"%s && asym_qwk_md%sbars.%s && diff_qwk_targetX.%s",s1.Data(),allmd[v].Data(),s2.Data(),s2.Data());
      sprintf(histoxa,"h1x%d",v);
      sprintf(yaxisa,"MD %s BARS ASYM [ppm]",allmd[v].Data());
      sprintf(xaxisa,"TARGET X [mm]");

      th->Draw(adetasenx,cutadetasenx,"prof");
      TH1D *hxf = (TH1D*)gDirectory->Get(histoxa);
      hxf->GetYaxis()->SetTitle(yaxisa);
      hxf->GetXaxis()->SetTitle(xaxisa);
      hxf->SetTitle(yaxisa);
      hxf->Fit("fitMDAllX","E M R F Q","",fit_range[0], fit_range[1]);
      hxf->SetLineColor(kRed-2);
      hxf->Draw("");

      gPad->Modified();
      gPad->Update();

      //     Float_t calculation;
      //     if(v == 1){calculation = hxf->Fit("fitfunx","E M R F","",fit_range[0], fit_range[1]);
      //       fitfunx->GetParameter(1);
      //       printf("Result %f\n",calculation);}


      pad81->cd(position7[v]);
      pad81->cd(position7[v])->SetGrid();

      sprintf(adetaseny,"asym_qwk_md%sbars*1e6:diff_qwk_targetY>>h1y%d",allmd[v].Data(),v);
      sprintf(cutadetaseny,"%s && asym_qwk_md%sbars.%s && diff_qwk_targetY.%s",s1.Data(),allmd[v].Data(),s2.Data(),s2.Data());
      sprintf(histoya,"h1y%d",v);
      sprintf(xaxisa1,"TARGET Y [mm]");

      th->Draw(adetaseny,cutadetaseny,"prof");
      TH1D *hyf = (TH1D*)gDirectory->Get(histoya);
      hyf->GetYaxis()->SetTitle(yaxisa);
      hyf->GetXaxis()->SetTitle(xaxisa1);
      hyf->SetTitle(yaxisa);
      hyf->Fit("fitMDAllY","E M R F Q","",fit_range[0], fit_range[1]);
      hyf->SetLineColor(kGreen+2);
      hyf->Draw("");

      pad111->cd(position7[v]);
      pad111->cd(position7[v])->SetGrid();

      sprintf(alumiasenx,"asym_dslumi_%s*1e6:diff_qwk_targetX>>hl1x%d",lumi[v].Data(),v);
      sprintf(cutalumiasenx,"%s && asym_dslumi_%s.%s && diff_qwk_targetX.%s",s1.Data(),lumi[v].Data(),s2.Data(),s2.Data());
      sprintf(histolxa,"hl1x%d",v);
      sprintf(yaxisla,"DSLUMI %s ASYM [ppm]",lumi[v].Data());

      th->Draw(alumiasenx,cutalumiasenx,"prof");
      TH1D *hxl = (TH1D*)gDirectory->Get(histolxa);
      hxl->GetYaxis()->SetTitle(yaxisla);
      hxl->GetXaxis()->SetTitle(xaxisa);
      hxl->SetTitle(yaxisla);
      hxl->Fit("fitDsLumiSumX","E M R F Q","",fit_range[0], fit_range[1]);
      hxl->SetLineColor(kRed-2);
      hxl->Draw("");

      gPad->Modified();
      gPad->Update();

      pad121->cd(position7[v]);
      pad121->cd(position7[v])->SetGrid();

      sprintf(alumiaseny,"asym_dslumi_%s*1e6:diff_qwk_targetY>>hl1y%d",lumi[v].Data(),v);
      sprintf(cutalumiaseny,"%s && asym_dslumi_%s.%s && diff_qwk_targetY.%s",s1.Data(),lumi[v].Data(),s2.Data(),s2.Data());
      sprintf(histolya,"hl1y%d",v);

      th->Draw(alumiaseny,cutalumiaseny,"prof");
      TH1D *hyl = (TH1D*)gDirectory->Get(histolya);
      hyl->GetYaxis()->SetTitle(yaxisla);
      hyl->GetXaxis()->SetTitle(xaxisa1);
      hyl->SetTitle(yaxisla);
      hyl->Fit("fitDsLumiSumY","E M R F Q","",fit_range[0], fit_range[1]);
      hyl->SetLineColor(kGreen+2);
      hyl->Draw("");

      gPad->Modified();
      gPad->Update();


      senMDAllXp0[v]          =  fitMDAllX->GetParameter(0);
      senMDAllXp0Error[v]     =  fitMDAllX->GetParError(0);
      senMDAllXp1[v]          =  fitMDAllX->GetParameter(1);
      senMDAllXp1Error[v]     =  fitMDAllX->GetParError(1);
      senMDAllXChi[v]         =  fitMDAllX->GetChisquare();
      senMDAllXNDF[v]         =  fitMDAllX->GetNDF();
      senMDAllXProb[v]        =  fitMDAllX->GetProb();

      senMDAllYp0[v]          =  fitMDAllY->GetParameter(0);
      senMDAllYp0Error[v]     =  fitMDAllY->GetParError(0);
      senMDAllYp1[v]          =  fitMDAllY->GetParameter(1);
      senMDAllYp1Error[v]     =  fitMDAllY->GetParError(1);
      senMDAllYChi[v]         =  fitMDAllY->GetChisquare();
      senMDAllYNDF[v]         =  fitMDAllY->GetNDF();
      senMDAllYProb[v]        =  fitMDAllY->GetProb();

      senDsLumiSumXp0[v]      =  fitDsLumiSumX->GetParameter(0);
      senDsLumiSumXp0Error[v] =  fitDsLumiSumX->GetParError(0);
      senDsLumiSumXp1[v]      =  fitDsLumiSumX->GetParameter(1);
      senDsLumiSumXp1Error[v] =  fitDsLumiSumX->GetParError(1);
      senDsLumiSumXChi[v]     =  fitDsLumiSumX->GetChisquare();
      senDsLumiSumXNDF[v]     =  fitDsLumiSumX->GetNDF();
      senDsLumiSumXProb[v]    =  fitDsLumiSumX->GetProb();

      senDsLumiSumYp0[v]      =  fitDsLumiSumY->GetParameter(0);
      senDsLumiSumYp0Error[v] =  fitDsLumiSumY->GetParError(0);
      senDsLumiSumYp1[v]      =  fitDsLumiSumY->GetParameter(1);
      senDsLumiSumYp1Error[v] =  fitDsLumiSumY->GetParError(1);
      senDsLumiSumYChi[v]     =  fitDsLumiSumY->GetChisquare();
      senDsLumiSumYNDF[v]     =  fitDsLumiSumY->GetNDF();
      senDsLumiSumYProb[v]    =  fitDsLumiSumY->GetProb();


      printf("MD-%s SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     allmd[v].Data(),senMDAllXp0[v],senMDAllXp0Error[v],senMDAllXp1[v],senMDAllXp1Error[v],senMDAllXChi[v],senMDAllXNDF[v],senMDAllXProb[v]);
      printf("MD-%s SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     allmd[v].Data(),senMDAllYp0[v],senMDAllYp0Error[v],senMDAllYp1[v],senMDAllYp1Error[v],senMDAllYChi[v],senMDAllYNDF[v],senMDAllYProb[v]);
      printf("DsLumi-%s SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     lumi[v].Data(),senDsLumiSumXp0[v],senDsLumiSumXp0Error[v],senDsLumiSumXp1[v],senDsLumiSumXp1Error[v],senDsLumiSumXChi[v],senDsLumiSumXNDF[v],senDsLumiSumXProb[v]);
      printf("DsLumi-%s SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     lumi[v].Data(),senDsLumiSumYp0[v],senDsLumiSumYp0Error[v],senDsLumiSumYp1[v],senDsLumiSumYp1Error[v],senDsLumiSumYChi[v],senDsLumiSumYNDF[v],senDsLumiSumYProb[v]);



    }
    c7->Update(); c7->SaveAs(pmdallsenx);
    c8->Update(); c8->SaveAs(pmdallseny);
    c11->Update(); c11->SaveAs(plumisenx);
    c12->Update(); c12->SaveAs(plumiseny);}

  gStyle->SetTitleYSize(0.07);
  gStyle->SetTitleXSize(0.07);
  gStyle->SetTitleSize(0.07);
  gStyle->SetTitleYOffset(0.6);
  gStyle->SetTitleXOffset(0.7);
  gStyle->SetLabelSize(0.060);
  gStyle->SetLabelSize(0.060,"y");
  gStyle->SetLabelSize(0.060,"x");
  gStyle->SetCanvasColor(kOrange-9);
  gStyle->SetStatFontSize(0.070);
  gStyle->SetStatY(1.0);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.10);
  gStyle->SetStatH(0.20); 
  /****************************************************************************/
  if (BMODSEN){
    TCanvas *c19 = new TCanvas("c19", CanvasTitle("Sensitivities from Modulation [ppm/mm]"),120,120,csizx,csizy);
    TPad* pad191 = NewFramedPad(kOrange-9, kWhite, "Sensitivities from Modulation [ppm/mm]");
    //     pad190 = new TPad("pad190","pad190",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //     pad191 = new TPad("pad191","pad191",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //     pad190->SetFillColor(kOrange-9);
    //     pad191->SetFillColor(kWhite);
    //     pad190->Draw();
    //     pad191->Draw();
    //     pad190->cd();
    //     DrawCanvasHeader("Sensitivities from Modulation [ppm/mm]");
    pad191->cd();
    pad191->Divide(1,3);
    printf("%sPlotting %sSensitivities from Modulation [ppm/mm]%s\n",blue,red,normal);
    gStyle -> SetOptFit(1111);
    gStyle -> SetOptStat("e");

    /* ----------------------------------------------------------------------- */

    Double_t senbmodXp0,senbmodXp0Error,senbmodXp1,senbmodXp1Error,senbmodXChi,senbmodXNDF,senbmodXProb;
    Double_t senbmodYp0,senbmodYp0Error,senbmodYp1,senbmodYp1Error,senbmodYChi,senbmodYNDF,senbmodYProb;
    Double_t senbmodEp0,senbmodEp0Error,senbmodEp1,senbmodEp1Error,senbmodEChi,senbmodENDF,senbmodEProb;
    Double_t senbmodXpp0,senbmodXpp0Error,senbmodXpp1,senbmodXpp1Error,senbmodXpChi,senbmodXpNDF,senbmodXpProb;
    Double_t senbmodYpp0,senbmodYpp0Error,senbmodYpp1,senbmodYpp1Error,senbmodYpChi,senbmodYpNDF,senbmodYpProb;

    pad191->cd(1);
    if (tm->Draw("qwk_mdallbars*1e6:qwk_targetX>>senbmodx",Form("%s && qwk_mdallbars.%s && qwk_targetX.%s && bm_pattern_number==11",s3.Data(),s2.Data(),s2.Data()),"prof goff")>0){
      senbmodx = (TH1D *)gDirectory->Get("senbmodx");
      senbmodx->SetLineColor(kRed);
      senbmodx->Draw("goff");
      fitbmodx->SetRange(senbmodx->GetMean(1) - amplitudex, senbmodx->GetMean(1) + amplitudex );
      fitbmodx->SetParameters(senbmodx->GetMean(2), 6000);
      senbmodx->Fit("fitbmodx","E M R F Q 0");
      xbmodslope1 =fitbmodx->GetParameter(1);
      exbmodslope1 =fitbmodx->GetParError(1);
      xbmodslope2 =fitbmodx->GetParameter(0);
      exbmodslope2 =fitbmodx->GetParError(0);
      exbmodslope11 = TMath::Power((exbmodslope1/xbmodslope1),2);
      exbmodslope12 = TMath::Power((exbmodslope2/xbmodslope2),2);
      xbmodslope = xbmodslope1/xbmodslope2;
      exbmodslope13 = TMath::Sqrt(exbmodslope11 + exbmodslope12);
      exbmodslope =  exbmodslope13*xbmodslope;
      Double_t limitx = senbmodx->GetMean(2);
      senbmodx->SetYTitle("MD ALL YIELD [V/A]");
      senbmodx->SetXTitle("TARGETX [mm]");
      senbmodx->SetTitle(Form("MD ALL vs TARGETX for X-Sensitivities:%2.2f+-%2.2f [ppm/mm]",1e6*xbmodslope,1e6*exbmodslope));
      senbmodx->Draw("goff");
      fitbmodx->Draw("same");
      senbmodx->GetYaxis()->SetRangeUser(limitx*0.995,limitx*1.005);

      senbmodXp0          =  fitbmodx->GetParameter(0);
      senbmodXp0Error     =  fitbmodx->GetParError(0);
      senbmodXp1          =  fitbmodx->GetParameter(1);
      senbmodXp1Error     =  fitbmodx->GetParError(1);
      senbmodXChi         =  fitbmodx->GetChisquare();
      senbmodXNDF         =  fitbmodx->GetNDF();
      senbmodXProb        =  fitbmodx->GetProb();

      printf("BMod SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodXp0,senbmodXp0Error,senbmodXp1,senbmodXp1Error,senbmodXChi,senbmodXNDF,senbmodXProb);

    } else {
      pad191->cd(1);
      tnodatax->Draw();
    }
    gPad->Update();
   

    //     pad191->cd(2);
    //     if (tm->Draw("qwk_mdallbars*1e6:qwk_targetX>>senbmodxp",Form("%s && qwk_mdallbars.%s && qwk_targetX.%s && bm_pattern_number==14",s3.Data(),s2.Data(),s2.Data()),"prof goff")>0){
    //       senbmodxp = (TH1D *)gDirectory->Get("senbmodxp");
    //       senbmodxp->SetLineColor(kRed);
    //       senbmodxp->Draw("goff");
    //       fitbmodxp->SetRange(senbmodxp->GetMean(1) - amplitudexp, senbmodxp->GetMean(1) + amplitudexp );
    //       fitbmodxp->SetParameters(senbmodxp->GetMean(2), 6000);
    //       senbmodxp->Fit("fitbmodxp","E M R F Q 0");
    //       xpbmodslope1 =fitbmodxp->GetParameter(1); expbmodslope1 =fitbmodxp->GetParError(1);
    //       xpbmodslope2 =fitbmodxp->GetParameter(0); expbmodslope2 =fitbmodxp->GetParError(0);
    //       expbmodslope11 = TMath::Power((expbmodslope1/xpbmodslope1),2);expbmodslope12 = TMath::Power((expbmodslope2/xpbmodslope2),2);
    //       xpbmodslope = xpbmodslope1/xpbmodslope2;  expbmodslope13 = TMath::Sqrt(expbmodslope11 + expbmodslope12);
    //       expbmodslope =  expbmodslope13*xpbmodslope;
    //       Double_t limitxp = senbmodxp->GetMean(2);
    //       senbmodxp->SetYTitle("MD ALL YIELD [V/A]");
    //       senbmodxp->SetXTitle("TARGETX [mm]");
    //       senbmodxp->SetTitle(Form("MD ALL vs TARGETX for Xp-Sensitivities:%2.2f+-%2.2f [ppm/mm]",1e6*xpbmodslope,1e6*expbmodslope));
    //       senbmodxp->Draw("goff");
    //       fitbmodxp->Draw("same");
    //       senbmodxp->GetYaxis()->SetRangeUser(limitxp*0.995,limitxp*1.005);

    //       senbmodXpp0          =  fitbmodxp->GetParameter(0);
    //       senbmodXpp0Error     =  fitbmodxp->GetParError(0);
    //       senbmodXpp1          =  fitbmodxp->GetParameter(1);
    //       senbmodXpp1Error     =  fitbmodxp->GetParError(1);
    //       senbmodXpChi         =  fitbmodxp->GetChisquare();
    //       senbmodXpNDF         =  fitbmodxp->GetNDF();
    //       senbmodXpProb        =  fitbmodxp->GetProb();
    //       printf("BMod SenXp\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodXpp0,senbmodXpp0Error,senbmodXpp1,senbmodXpp1Error,senbmodXpChi,senbmodXpNDF,senbmodXpProb);
    //     }
    //     gPad->Update();
    //     else{pad191->cd(2);tnodataxp->Draw();}

    pad191->cd(3);
    if (tm->Draw("qwk_mdallbars*1e6:qwk_bpm3c12X>>senbmode",Form("%s && qwk_mdallbars.%s && qwk_bpm3c12X.%s && bm_pattern_number==13",s3.Data(),s2.Data(),s2.Data()),"prof goff")>0){
      senbmode = (TH1D *)gDirectory->Get("senbmode");
      senbmode->SetLineColor(kBlue);
      senbmode->Draw("goff");
      fitbmode->SetRange(senbmode->GetMean(1) - amplitudee, senbmode->GetMean(1) + amplitudee );
      fitbmode->SetParameters(senbmode->GetMean(2), 6000);
      senbmode->Fit("fitbmode","E M R F Q 0");
      ebmodslope1 =fitbmode->GetParameter(1); eebmodslope1 =fitbmode->GetParError(1);
      ebmodslope2 =fitbmode->GetParameter(0); eebmodslope2 =fitbmode->GetParError(0);
      eebmodslope11 = TMath::Power((eebmodslope1/ebmodslope1),2);eebmodslope12 = TMath::Power((eebmodslope2/ebmodslope2),2);
      ebmodslope = ebmodslope1/ebmodslope2;  eebmodslope13 = TMath::Sqrt(eebmodslope11 + eebmodslope12);
      eebmodslope =  eebmodslope13*ebmodslope;
      Double_t limite = senbmode->GetMean(2);
      senbmode->SetYTitle("MD ALL YIELD [V/A]");
      senbmode->SetXTitle("BPM 3C12X [mm]");
      senbmode->SetTitle(Form("MD ALL vs BPM 3C12X for E-Sensitivities:%2.2f+-%2.2f [ppm/mm]",1e6*ebmodslope,1e6*eebmodslope));
      senbmode->Draw("goff");
      fitbmode->Draw("same");
      senbmode->GetYaxis()->SetRangeUser(limite*0.995,limite*1.005);

      senbmodEp0          =  fitbmode->GetParameter(0);
      senbmodEp0Error     =  fitbmode->GetParError(0);
      senbmodEp1          =  fitbmode->GetParameter(1);
      senbmodEp1Error     =  fitbmode->GetParError(1);
      senbmodEChi         =  fitbmode->GetChisquare();
      senbmodENDF         =  fitbmode->GetNDF();
      senbmodEProb        =  fitbmode->GetProb();

      printf("BMod SenE\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodEp0,senbmodEp0Error,senbmodEp1,senbmodEp1Error,senbmodEChi,senbmodENDF,senbmodEProb);

    } else {
      pad191->cd(3);
      tnodatae->Draw();
    }
    gPad->Update();
    

    pad191->cd(2);
    if (tm->Draw("qwk_mdallbars*1e6:qwk_targetY>>senbmody",Form("%s && qwk_mdallbars.%s && qwk_targetY.%s && bm_pattern_number==12",s3.Data(),s2.Data(),s2.Data()),"prof goff")>0){
      senbmody = (TH1D *)gDirectory->Get("senbmody");
      senbmody->SetLineColor(kGreen);
      senbmody->Draw("goff");
      fitbmody->SetRange(senbmody->GetMean(1) - amplitudey, senbmody->GetMean(1) + amplitudey );
      fitbmody->SetParameters(senbmody->GetMean(2), 6000);
      senbmody->Fit("fitbmody","E M R F Q 0");
      ybmodslope1 =fitbmody->GetParameter(1); eybmodslope1 =fitbmody->GetParError(1);
      ybmodslope2 =fitbmody->GetParameter(0); eybmodslope2 =fitbmody->GetParError(0);
      eybmodslope11 = TMath::Power((eybmodslope1/ybmodslope1),2);eybmodslope12 = TMath::Power((eybmodslope2/ybmodslope2),2);
      ybmodslope = ybmodslope1/ybmodslope2;  eybmodslope13 = TMath::Sqrt(eybmodslope11 + eybmodslope12);
      eybmodslope =  eybmodslope13*ybmodslope;
      Double_t limity = senbmody->GetMean(2);
      senbmody->SetYTitle("MD ALL YIELD [V/A]");
      senbmody->SetXTitle("TARGETY [mm]");
      senbmody->SetTitle(Form("MD ALL vs TARGETY for Y-Sensitivities:%2.2f+-%2.2f [ppm/mm]",1e6*ybmodslope,1e6*eybmodslope));
      senbmody->Draw("goff");
      fitbmody->Draw("same");
      senbmody->GetYaxis()->SetRangeUser(limity*0.995,limity*1.005);

      senbmodYp0          =  fitbmody->GetParameter(0);
      senbmodYp0Error     =  fitbmody->GetParError(0);
      senbmodYp1          =  fitbmody->GetParameter(1);
      senbmodYp1Error     =  fitbmody->GetParError(1);
      senbmodYChi         =  fitbmody->GetChisquare();
      senbmodYNDF         =  fitbmody->GetNDF();
      senbmodYProb        =  fitbmody->GetProb();

      printf("BMod SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodYp0,senbmodYp0Error,senbmodYp1,senbmodYp1Error,senbmodYChi,senbmodYNDF,senbmodYProb);

    } else {
      pad191->cd(2);
      tnodatay->Draw();
    }
    gPad->Update();
    

    //     pad191->cd(5);
    //     if (tm->Draw("qwk_mdallbars*1e6:qwk_targetY>>senbmodyp",Form("%s && qwk_mdallbars.%s && qwk_targetY.%s && bm_pattern_number==15",s3.Data(),s2.Data(),s2.Data()),"prof goff")>0){
    //       senbmodyp = (TH1D *)gDirectory->Get("senbmodyp");
    //       senbmodyp->SetLineColor(kGreen);
    //       senbmodyp->Draw("goff");
    //       fitbmodyp->SetRange(senbmodyp->GetMean(1) - amplitudeyp, senbmodyp->GetMean(1) + amplitudeyp );
    //       fitbmodyp->SetParameters(senbmodyp->GetMean(2), 6000);
    //       senbmodyp->Fit("fitbmodyp","E M R F Q 0");
    //       ypbmodslope1 =fitbmodyp->GetParameter(1); eypbmodslope1 =fitbmodyp->GetParError(1);
    //       ypbmodslope2 =fitbmodyp->GetParameter(0); eypbmodslope2 =fitbmodyp->GetParError(0);
    //       eypbmodslope11 = TMath::Power((eypbmodslope1/ypbmodslope1),2);eypbmodslope12 = TMath::Power((eypbmodslope2/ypbmodslope2),2);
    //       ypbmodslope = ypbmodslope1/ypbmodslope2;  eypbmodslope13 = TMath::Sqrt(eypbmodslope11 + eypbmodslope12);
    //       eypbmodslope =  eypbmodslope13*ypbmodslope;
    //       Double_t limityp = senbmodyp->GetMean(2);
    //       senbmodyp->SetYTitle("MD ALL YIELD [V/A]");
    //       senbmodyp->SetXTitle("TARGETY [mm]");
    //       senbmodyp->SetTitle(Form("MD ALL vs TARGETY for Yp-Sensitivities:%2.2f+-%2.2f [ppm/mm]",1e6*ypbmodslope,1e6*eypbmodslope));
    //       senbmodyp->Draw("goff");
    //       fitbmodyp->Draw("same");
    //       senbmodyp->GetYaxis()->SetRangeUser(limityp*0.995,limityp*1.005);

    //       senbmodYpp0          =  fitbmodyp->GetParameter(0);
    //       senbmodYpp0Error     =  fitbmodyp->GetParError(0);
    //       senbmodYpp1          =  fitbmodyp->GetParameter(1);
    //       senbmodYpp1Error     =  fitbmodyp->GetParError(1);
    //       senbmodYpChi         =  fitbmodyp->GetChisquare();
    //       senbmodYpNDF         =  fitbmodyp->GetNDF();
    //       senbmodYpProb        =  fitbmodyp->GetProb();
    //       printf("BMod SenYp\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodYpp0,senbmodYpp0Error,senbmodYpp1,senbmodYpp1Error,senbmodYpChi,senbmodYpNDF,senbmodYpProb);
    //     }
    //     gPad->Update();
    //     else{pad191->cd(5);tnodatayp->Draw();}

    c19->Update(); c19->SaveAs(psenbmod);}

  gStyle->SetTitleSize(0.04);
  gStyle->SetTitleYSize(0.04);
  gStyle->SetTitleXSize(0.04);
  gStyle->SetTitleYOffset(1.1);
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetLabelSize(0.050);
  gStyle->SetLabelSize(0.050,"y");
  gStyle->SetLabelSize(0.050,"x");
  gStyle->SetCanvasColor(kRed-10);
  gStyle->SetStatFontSize(0.070);
  gStyle->SetStatY(1.0);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.30);
  gStyle->SetStatH(0.30); 
  /****************************************************************************/
  if (MDLUMI){
    TCanvas *c5 = new TCanvas("c5", CanvasTitle("Main Detector Barsum Asymmetries (ppm)"),80,80,1400,1000);
    TPad* pad51 = NewFramedPad(kRed-10, kWhite, "Main Detector Barsum Asymmetries (ppm)");
    //   pad50 = new TPad("pad50","pad50",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad51 = new TPad("pad51","pad51",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad50->SetFillColor(kRed-10);
    //   pad51->SetFillColor(kWhite);
    //   pad50->Draw();
    //   pad51->Draw();
    //   pad50->cd();
    //   DrawCanvasHeader("Main Detector Barsum Asymmetries (ppm)");
    pad51->cd();
    pad51->Divide(4,3);
    printf("%sPlotting %sMain Detector Barsum Asymmetries (ppm)%s\n",blue,red,normal);
    gStyle->SetCanvasColor(kBlue-10);
    /* ----------------------------------------------------------------------- */
    TCanvas *c9 = new TCanvas("c9", CanvasTitle("Downstream Lumi Yields (Volts/uA)"),160,160,1400,1000);
    TPad* pad91 = NewFramedPad(kBlue-10, kWhite, "Downstream Lumi Yields (Volts/uA)");
    //   pad90 = new TPad("pad90","pad90",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad91 = new TPad("pad91","pad91",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad90->SetFillColor(kBlue-10);
    //   pad91->SetFillColor(kWhite);
    //   pad90->Draw();
    //   pad91->Draw();
    //   pad90->cd();
    //   DrawCanvasHeader("Downstream Lumi Yields (Volts/uA)");
    pad91->cd();
    pad91->Divide(4,3);
    printf("%sPlotting %sDownstream Lumi Yields (Volts/uA)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    TCanvas *c10 = new TCanvas("c10", CanvasTitle("Downstream Lumi Asymmetries (ppm)"),180,180,1400,1000);
    TPad* pad101 = NewFramedPad(kBlue-10, kWhite, "Downstream Lumi Asymmetries (ppm)");
    //   pad100 = new TPad("pad100","pad100",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad101 = new TPad("pad101","pad101",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad100->SetFillColor(kBlue-10);
    //   pad101->SetFillColor(kWhite);
    //   pad100->Draw();
    //   pad101->Draw();
    //   pad100->cd();
    //   DrawCanvasHeader("Downstream Lumi Asymmetries (ppm)");
    pad101->cd();
    pad101->Divide(4,3);
    printf("%sPlotting %sDownstream Lumi Asymmetries (ppm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    gStyle->SetOptStat(1);
    gStyle->SetOptStat("emr");

    char hadslumi[255],hadslumiall[255], txadslumi[255]; //, tyadslumi[255];
    char txadslumiall[255],hydslumi[255],hydslumiall[255], txydslumi[255], txydslumiall[255];

    const Int_t detNUM=9, detNUM2=4;
    Double_t aMD[detNUM],aMDError[detNUM],aMDRMS[detNUM],yMD[detNUM],yMDError[detNUM],yMDRMS[detNUM];
    Double_t adsLumi[detNUM],adsLumiError[detNUM],adsLumiRMS[detNUM],ydsLumi[detNUM],ydsLumiError[detNUM],ydsLumiRMS[detNUM];

    Double_t aMDAll,aMDAllError,aMDAllRMS;
    Double_t aMDAll56,aMDAll56Error,aMDAll56RMS;
    Double_t aMDAll78,aMDAll78Error,aMDAll78RMS;
    Double_t aMDAll15,aMDAll15Error,aMDAll15RMS;
    Double_t aMDAll26,aMDAll26Error,aMDAll26RMS;
    Double_t adsLumiAll[detNUM2],adsLumiAllError[detNUM2],adsLumiAllRMS[detNUM2],ydsLumiAll[detNUM2],ydsLumiAllError[detNUM2],ydsLumiAllRMS[detNUM2];

  
    TString hadet;
    TString txalladet("MD All ASYM [ppm]");
    TString txadet;

    int dm=0;
    TH1*  hadt;
    TH1*  hylumi;
    TH1*  halumi;
    for ( int m=0; m<8; m++) {
      dm = m+1;

      sprintf(adet,"asym_qwk_md%dbarsum*1e6>>ha%d",dm,dm);
      sprintf(cutadet,"%s && asym_qwk_md%dbarsum.%s",s1.Data(),dm,s2.Data());
      hadet = Form("ha%d",dm);
      txadet = Form("MD %d ASYM [ppm]",dm);

      sprintf(ylumi,"yield_qwk_dslumi%d>>hydsl%d",dm,dm);
      sprintf(alumi,"asym_qwk_dslumi%d*1e6>>hadsl%d",dm,dm);
      sprintf(cutylumi,"%s && yield_qwk_dslumi%d.%s",s1.Data(),dm,s2.Data());
      sprintf(cutalumi,"%s && asym_qwk_dslumi%d.%s",s1.Data(),dm,s2.Data());
      sprintf(hydslumi,"hydsl%d",dm);
      sprintf(hadslumi,"hadsl%d",dm);
      sprintf(txydslumi,"DS LUMI %d YIELD [V/uA]",dm);
      sprintf(txadslumi,"DS LUMI %d ASYM [ppm]",dm);

      pad51->cd(position6[dm]);
      th->Draw(adet,cutadet,"goff");
      hadt = (TH1F *)gDirectory->Get(hadet);
      hadt->SetYTitle(tarbitrary);
      hadt->SetXTitle(txadet);
      hadt->SetTitle(txadet);
      hadt->Draw();
      gPad->Update();

      pad91->cd(position6[dm]);
      th->Draw(ylumi,cutylumi,"goff");
      hylumi = (TH1F *)gDirectory->Get(hydslumi);
      hylumi->SetYTitle(tarbitrary);
      hylumi->SetXTitle(txydslumi);
      hylumi->SetTitle(txydslumi);
      hylumi->Draw();
      gPad->Update();

      pad101->cd(position6[dm]);
      th->Draw(alumi,cutalumi,"goff");
      halumi = (TH1F *)gDirectory->Get(hadslumi);
      halumi->SetYTitle(tarbitrary);
      halumi->SetXTitle(txadslumi);
      halumi->SetTitle(txadslumi);
      halumi->Draw();
      gPad->Update();


      aMD[m]      =  hadt->GetMean();
      aMDError[m] =  hadt->GetMeanError();
      aMDRMS[m]   =  hadt->GetRMS();

      adsLumi[m]      =  halumi->GetMean();
      adsLumiError[m] =  halumi->GetMeanError();
      adsLumiRMS[m]   =  halumi->GetRMS();
      ydsLumi[m]      =  hylumi->GetMean();
      ydsLumiError[m] =  hylumi->GetMeanError();
      ydsLumiRMS[m]   =  hylumi->GetRMS();

      printf("MD %d\t %4.2f+-%4.2f %4.2f\n", 
	     dm,aMD[m],aMDError[m],aMDRMS[m]);
      printf("DSLumi %d\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n", 
	     dm,adsLumi[m],adsLumiError[m],adsLumiRMS[m],ydsLumi[m],ydsLumiError[m],ydsLumiRMS[m]);

    }
    int dn=0;
    TH1*  hylumiall;
    TH1*  halumiall;
    for ( int n=0; n<3; n++) {
      dn = n+1;

      sprintf(ylumis,"yield_dslumi_%s>>hydslall%d",lumi[n].Data(),dn);
      sprintf(alumis,"asym_dslumi_%s*1e6>>hadslall%d",lumi[n].Data(),dn);
      sprintf(cutylumis,"%s && yield_dslumi_%s.%s",s1.Data(),lumi[n].Data(),s2.Data());
      sprintf(cutalumis,"%s && asym_dslumi_%s.%s",s1.Data(),lumi[n].Data(),s2.Data());
      sprintf(hydslumiall,"hydslall%d",dn);
      sprintf(hadslumiall,"hadslall%d",dn);
      sprintf(txydslumiall,"DS LUMI %s YIELD [V/uA]",lumi[n].Data());
      sprintf(txadslumiall,"DS LUMI %s ASYM [ppm]",lumi[n].Data());

      pad91->cd(position7[n]);
      th->Draw(ylumis,cutylumis,"goff");
      hylumiall = (TH1F *)gDirectory->Get(hydslumiall);
      hylumiall->SetFillStyle(3013);
      hylumiall->SetFillColor(kBlue-2);
      hylumiall->SetYTitle(tarbitrary);
      hylumiall->SetXTitle(txydslumiall);
      hylumiall->SetTitle(txydslumiall);
      hylumiall->Draw();

      gPad->Update();

      pad101->cd(position7[n]);
      th->Draw(alumis,cutalumis,"goff");
      halumiall = (TH1F *)gDirectory->Get(hadslumiall);
      halumiall->SetFillStyle(3013);
      halumiall->SetFillColor(kBlue-2);
      halumiall->SetYTitle(tarbitrary);
      halumiall->SetXTitle(txadslumiall);
      halumiall->SetTitle(txadslumiall);
      halumiall->Draw();
      gPad->Update();

      adsLumiAll[n]      =  halumiall->GetMean();
      adsLumiAllError[n] =  halumiall->GetMeanError();
      adsLumiAllRMS[n]   =  halumiall->GetRMS();
      ydsLumiAll[n]      =  hylumiall->GetMean();
      ydsLumiAllError[n] =  hylumiall->GetMeanError();
      ydsLumiAllRMS[n]   =  hylumiall->GetRMS();

      printf("DSLumi %s\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n", 
	     lumi[n].Data(),adsLumiAll[n],adsLumiAllError[n],adsLumiAllRMS[n],ydsLumiAll[n],ydsLumiAllError[n],ydsLumiAllRMS[n]);

    }

    TH1*  hall;
    TH1*  hall56;
    TH1*  hall78;
    TH1*  hall15;
    TH1*  hall26;
    pad51->cd(6);
    th->Draw("asym_qwk_mdallbars*1e6>>hall",Form("%s && asym_qwk_mdallbars.%s",s1.Data(),s2.Data()),"goff");
    hall = GetHist("hall");
    hall->SetFillColor(kRed-2);
    // hall->SetLineColor(kRed-2);
    hall->SetFillStyle(3013);
    hall->SetYTitle(tarbitrary);
    hall->SetXTitle(txalladet);
    hall->SetTitle(txalladet);
    hall->Draw();

    aMDAll        =  hall->GetMean();
    aMDAllError   =  hall->GetMeanError();
    aMDAllRMS     =  hall->GetRMS();
    printf("MDAll\t %4.2f+-%4.2f %4.2f\n",aMDAll,aMDAllError,aMDAllRMS);

    pad51->cd(4);
    th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-(asym_qwk_bcm5+asym_qwk_bcm6)/2)*1e6>>hall56",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm5.%s && asym_qwk_bcm6.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
    hall56 = GetHist("hall56");
    hall56->SetFillColor(kRed-2);
    hall56->SetFillStyle(3004);
    hall56->SetYTitle(tarbitrary);
    hall56->SetXTitle(txalladet);
    hall56->SetTitle("Normalized to BCM 5 & 6");
    hall56->Draw();

    aMDAll56        =  hall56->GetMean();
    aMDAll56Error   =  hall56->GetMeanError();
    aMDAll56RMS     =  hall56->GetRMS();
    printf("MDAll56\t %4.2f+-%4.2f %4.2f\n",aMDAll56,aMDAll56Error,aMDAll56RMS);

    pad51->cd(8);
    th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-(asym_qwk_bcm1+asym_qwk_bcm5)/2)*1e6>>hall15",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm1.%s && asym_qwk_bcm5.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
    hall15 = GetHist("hall15");
    hall15->SetFillColor(kRed-2);
    hall15->SetFillStyle(3004);
    hall15->SetYTitle(tarbitrary);
    hall15->SetXTitle(txalladet);
    hall15->SetTitle("Normalized to BCM 1 & 5");
    hall15->Draw();

    aMDAll15        =  hall15->GetMean();
    aMDAll15Error   =  hall15->GetMeanError();
    aMDAll15RMS     =  hall15->GetRMS();
    printf("MDAll15\t %4.2f+-%4.2f %4.2f\n",aMDAll15,aMDAll15Error,aMDAll15RMS);

    if(gRunNumber>13000){
      pad51->cd(12);
      th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-(asym_qwk_bcm7+asym_qwk_bcm8)/2)*1e6>>hall78",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm7.%s && asym_qwk_bcm8.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
      hall78 = GetHist("hall78");
      hall78->SetFillColor(kRed-2);
      hall78->SetFillStyle(3004);
      hall78->SetYTitle(tarbitrary);
      hall78->SetXTitle(txalladet);
      hall78->SetTitle("Normalized to BCM 7 & 8");
      hall78->Draw();

      aMDAll78        =  hall78->GetMean();
      aMDAll78Error   =  hall78->GetMeanError();
      aMDAll78RMS     =  hall78->GetRMS();
      printf("MDAll78\t %4.2f+-%4.2f %4.2f\n",aMDAll78,aMDAll78Error,aMDAll78RMS);

    }
    else{
      pad51->cd(12);
      th->Draw("(asym_qwk_mdallbars+asym_qwk_charge-(asym_qwk_bcm1+asym_qwk_bcm2)/2)*1e6>>hall78",Form("%s && asym_qwk_mdallbars.%s && asym_qwk_charge.%s && asym_qwk_bcm1.%s && asym_qwk_bcm2.%s",s1.Data(),s2.Data(),s2.Data(),s2.Data(),s2.Data()),"goff");
      hall78 = GetHist("hall78");
      hall78->SetFillColor(kRed-2);
      hall78->SetFillStyle(3004);
      hall78->SetYTitle(tarbitrary);
      hall78->SetXTitle(txalladet);
      hall78->SetTitle("Normalized to BCM 1 & 2");
      hall78->Draw();
    }

    c5->Update(); c5->SaveAs(pmdasym);
    c9->Update(); c9->SaveAs(pdslumiyield);
    c10->Update(); c10->SaveAs(pdslumiasym);}
  /****************************************************************************/
  if (USLUMI){
    TCanvas *c16 = new TCanvas("c16", CanvasTitle("Upstream Lumi Yields (Volts/uA) & Asymmetry (ppm)"),200,200,1580,1000);
    TPad* pad161 = NewFramedPad(kBlue-10, kWhite, "Upstream Lumi Yields (Volts/uA) & Asymmetry (ppm)");
    //   pad160 = new TPad("pad160","pad160",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad161 = new TPad("pad161","pad161",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad160->SetFillColor(kBlue-10);
    //   pad161->SetFillColor(kWhite);
    //   pad160->Draw();
    //   pad161->Draw();
    //   pad160->cd();
    //   DrawCanvasHeader("Upstream Lumi Yields (Volts/uA) & Asymmetry (ppm)");
    pad161->cd();
    pad161->Divide(6,3);
    printf("%sPlotting %sUpstream Lumi Yields (Volts/uA) & Asymmetry (ppm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    char hauslumi[255],txauslumi[255], hyuslumi[255], txyuslumi[255];

    const Int_t usNUM = 6;
    Double_t ausLumi[usNUM],ausLumiError[usNUM],ausLumiRMS[usNUM],yusLumi[usNUM],yusLumiError[usNUM],yusLumiRMS[usNUM];


    TH1* hyuslumis;
    TH1* hauslumis;
    for ( int q=0; q<5; q++) {

      sprintf(yuslumis,"yield_%s_sum>>hyusl%d",uslumi[q].Data(),q);
      sprintf(auslumis,"asym_%s_sum*1e6>>hausl%d",uslumi[q].Data(),q);
      sprintf(cutyuslumis,"%s && yield_%s_sum.%s",s1.Data(),uslumi[q].Data(),s2.Data());
      sprintf(cutauslumis,"%s && asym_%s_sum.%s",s1.Data(),uslumi[q].Data(),s2.Data());
      sprintf(hyuslumi,"hyusl%d",q);
      sprintf(hauslumi,"hausl%d",q);
      sprintf(txyuslumi,"%s YIELD [V/uA]",uslumi[q].Data());
      sprintf(txauslumi,"%s ASYM [ppm]",uslumi[q].Data());

      pad161->cd(position10[q]);
      th->Draw(yuslumis,cutyuslumis,"goff");
      hyuslumis = (TH1F *)gDirectory->Get(hyuslumi);
      hyuslumis->SetYTitle(tarbitrary);
      hyuslumis->SetXTitle(txyuslumi);
      hyuslumis->SetTitle(txyuslumi);
      hyuslumis->Draw();
      gPad->Update();

      pad161->cd(position11[q]);
      th->Draw(auslumis,cutauslumis,"goff");
      hauslumis = (TH1F *)gDirectory->Get(hauslumi);
      hauslumis->SetFillColor(kBlue-2);
      hauslumis->SetFillStyle(3013);
      hauslumis->SetYTitle(tarbitrary);
      hauslumis->SetXTitle(txauslumi);
      hauslumis->SetTitle(txauslumi);
      hauslumis->Draw();
      gPad->Update();

      ausLumi[q]      =  hauslumis->GetMean();
      ausLumiError[q] =  hauslumis->GetMeanError();
      ausLumiRMS[q]   =  hauslumis->GetRMS();
      yusLumi[q]      =  hyuslumis->GetMean();
      yusLumiError[q] =  hyuslumis->GetMeanError();
      yusLumiRMS[q]   =  hyuslumis->GetRMS();

      printf("%s\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n", 
	     uslumi[q].Data(),ausLumi[q],ausLumiError[q],ausLumiRMS[q],yusLumi[q],yusLumiError[q],yusLumiRMS[q]);


    }
    c16->Update(); c16->SaveAs(puslumi);}

  /****************************************************************************/
  if (USLUMISEN){
    TCanvas *c17 = new TCanvas("c17", CanvasTitle("Upstream Lumi Sensitivity (ppm/mm)"),220,220,1580,1000);
    TPad* pad171 = NewFramedPad(kBlue-10, kWhite, "Upstream Lumi Sensitivity (ppm/mm)");
    //   pad170 = new TPad("pad170","pad170",gPadCoord1,gPadCoord2,gPadCoord4,gPadCoord4);
    //   pad171 = new TPad("pad171","pad171",gPadCoord1,gPadCoord1,gPadCoord4,gPadCoord3);
    //   pad170->SetFillColor(kBlue-10);
    //   pad171->SetFillColor(kWhite);
    //   pad170->Draw();
    //   pad171->Draw();
    //   pad170->cd();
    //   DrawCanvasHeader("Upstream Lumi Sensitivity (ppm/mm)");
    pad171->cd();
    pad171->Divide(6,3);
    printf("%sPlotting %sUpstream Lumi Sensitivity (ppm/mm)%s\n",blue,red,normal);
    /* ----------------------------------------------------------------------- */
    gStyle -> SetOptFit(1111);
    gStyle -> SetOptStat("e");

    const Int_t usNUM = 6;
    Double_t senUsLumiXp0[usNUM],senUsLumiXp0Error[usNUM],senUsLumiXp1[usNUM],senUsLumiXp1Error[usNUM],senUsLumiXChi[usNUM],senUsLumiXNDF[usNUM],senUsLumiXProb[usNUM];
    Double_t senUsLumiYp0[usNUM],senUsLumiYp0Error[usNUM],senUsLumiYp1[usNUM],senUsLumiYp1Error[usNUM],senUsLumiYChi[usNUM],senUsLumiYNDF[usNUM],senUsLumiYProb[usNUM];

    TF1* fitUsLumiX = new TF1("fitUsLumiX","pol1",fit_range[0], fit_range[1]);
    TF1* fitUsLumiY = new TF1("fitUsLumiY","pol1",fit_range[0], fit_range[1]);

    int xn=0;
    for ( int x=0; x<5; x++) {
      xn = x +1;

      sprintf(auslumisenx,"asym_%s_sum*1e6:diff_qwk_targetX>>huslx%d",uslumi[x].Data(),x);
      sprintf(cutauslumisenx,"%s && asym_%s_sum.%s && diff_qwk_targetX.%s",s1.Data(),uslumi[x].Data(),s2.Data(),s2.Data());
      sprintf(histouslx,"huslx%d",x);
      sprintf(yaxisusl,"%s ASYM [ppm]",uslumi[x].Data());
      sprintf(auslumiseny,"asym_%s_sum*1e6:diff_qwk_targetY>>husly%d",uslumi[x].Data(),x);
      sprintf(cutauslumiseny,"%s && asym_%s_sum.%s && diff_qwk_targetY.%s",s1.Data(),uslumi[x].Data(),s2.Data(),s2.Data());
      sprintf(histously,"husly%d",x);

      pad171->cd(position10[x]);
      pad171->cd(position10[x])->SetGrid();

      th->Draw(auslumisenx,cutauslumisenx,"prof");
      TH1D *huslxf = (TH1D*)gDirectory->Get(histouslx);
      huslxf->GetYaxis()->SetTitle(yaxisusl);
      huslxf->GetXaxis()->SetTitle(xaxis);
      huslxf->SetTitle(yaxisusl);
      huslxf->Fit("fitUsLumiX","E M R F Q","",fit_range[0], fit_range[1]);
      huslxf->SetLineColor(kRed);
      huslxf->Draw("");
      gPad->Modified();
      gPad->Update();

      pad171->cd(position11[x]);
      pad171->cd(position11[x])->SetGrid();
      // pad171->cd(position11[x])->SetFillColor(kGreen-10);

      th->Draw(auslumiseny,cutauslumiseny,"prof");
      TH1D *huslyf = (TH1D*)gDirectory->Get(histously);
      huslyf->GetYaxis()->SetTitle(yaxisusl);
      huslyf->GetXaxis()->SetTitle(xaxis1);
      huslyf->SetTitle(yaxisusl);
      huslyf->Fit("fitUsLumiY","E M R F Q","",fit_range[0], fit_range[1]);
      huslyf->SetLineColor(kGreen);
      huslyf->Draw("");
      gPad->Modified();
      gPad->Update();

      senUsLumiXp0[x]      =  fitUsLumiX->GetParameter(0);
      senUsLumiXp0Error[x] =  fitUsLumiX->GetParError(0);
      senUsLumiXp1[x]      =  fitUsLumiX->GetParameter(1);
      senUsLumiXp1Error[x] =  fitUsLumiX->GetParError(1);
      senUsLumiXChi[x]     =  fitUsLumiX->GetChisquare();
      senUsLumiXNDF[x]     =  fitUsLumiX->GetNDF();
      senUsLumiXProb[x]    =  fitUsLumiX->GetProb();

      senUsLumiYp0[x]      =  fitUsLumiY->GetParameter(0);
      senUsLumiYp0Error[x] =  fitUsLumiY->GetParError(0);
      senUsLumiYp1[x]      =  fitUsLumiY->GetParameter(1);
      senUsLumiYp1Error[x] =  fitUsLumiY->GetParError(1);
      senUsLumiYChi[x]     =  fitUsLumiY->GetChisquare();
      senUsLumiYNDF[x]     =  fitUsLumiY->GetNDF();
      senUsLumiYProb[x]    =  fitUsLumiY->GetProb();


      printf("%s SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     uslumi[x].Data(),senUsLumiXp0[x],senUsLumiXp0Error[x],senUsLumiXp1[x],senUsLumiXp1Error[x],senUsLumiXChi[x],senUsLumiXNDF[x],senUsLumiXProb[x]);
      printf("%s SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
	     uslumi[x].Data(),senUsLumiYp0[x],senUsLumiYp0Error[x],senUsLumiYp1[x],senUsLumiYp1Error[x],senUsLumiYChi[x],senUsLumiYNDF[x],senUsLumiYProb[x]);

    }
    c17->Update(); c17->SaveAs(puslumisen);}


  /****************************************************************************/
  printf("%sSummary Table.%s\n",blue,normal);  
  printf(ssline);printf("%s|\t%sRun Number: %6d%s\t\t\t\t|%s\n",green,blue,gRunNumber,green,normal);
  printf("%s|  \t%s%s, %s, %s%s  \t|%s\n",green,blue,gTarget.Data(),gCurrent.Data(),gRasterString.Data(),green,normal);
  printf(ssline);
  printf("%s|%sI                        \t%s|%suA    \t%s|%s%s%s      \t|%s\n",green,blue,green,blue,green,red,gCurrent.Data(),green,normal);
  printf("%s|%sMDALLBARS width          \t%s|%sppm   \t%s|%s%2.1f%s        \t|%s\n",green,blue,green,blue,green,red,cal_mdalla,green,normal);
  printf("%s|%sUS Lumi Sum mean         \t%s|%sppm   \t%s|%s%2.1f%s        \t|%s\n",green,blue,green,blue,green,red,cal_uslumi_sum_mean,green,normal);
  printf("%s|%sUS Lumi Sum width        \t%s|%sppm   \t%s|%s%2.1f%s        \t|%s\n",green,blue,green,blue,green,red,cal_uslumi_sum_rms,green,normal);
  printf("%s|%sBCM78-ddif width         \t%s|%sppm   \t%s|%s%2.1f%s        \t|%s\n",green,blue,green,blue,green,red,cal_bcmdd,green,normal);
  printf("%s|%sA_q mean                 \t%s|%sppm   \t%s|%s%2.2f%s        \t|%s\n",green,blue,green,blue,green,red,cal_abcmm,green,normal);
  printf("%s|%sA_q width                \t%s|%sppm   \t%s|%s%2.1f%s        \t|%s\n",green,blue,green,blue,green,red,cal_abcm,green,normal);
  printf("%s|%sMDALLBARS X-sens.        \t%s|%sppm/mm\t%s|%s%2.1f+-%1.1f%s \t|%s\n",green,blue,green,blue,green,red,cal_mdxsen,cal_emdxsen,green,normal);
  printf("%s|%sMDALLBARS Y-sens.        \t%s|%sppm/mm\t%s|%s%2.1f+-%1.1f%s \t|%s\n",green,blue,green,blue,green,red,cal_mdysen,cal_emdysen,green,normal);
  printf("%s|%sBMOD MDALLBARS X-sens.   \t%s|%sppm/mm\t%s|%s%2.1f+-%1.1f%s \t|%s\n",green,blue,green,blue,green,red,1e6*xbmodslope,1e6*exbmodslope,green,normal);
  printf("%s|%sBMOD MDALLBARS Y-sens.   \t%s|%sppm/mm\t%s|%s%2.1f+-%1.1f%s \t|%s\n",green,blue,green,blue,green,red,1e6*ybmodslope,1e6*eybmodslope,green,normal);
  printf("%s|%sBMOD MDALLBARS E-sens.   \t%s|%sppm/mm\t%s|%s%2.1f+-%1.1f%s \t|%s\n",green,blue,green,blue,green,red,1e6*ebmodslope,1e6*eebmodslope,green,normal);
  printf(ssline);
  /****************************************************************************/
  printf("%sDone with all the plots.%s\n",blue,normal);
  /****************************************************************************/
  /****************************************************************************/
  if(HCLOGPOST){

    if (hclog_switch<=-1) {
      printf("%sPlease Insert %s1%s for submiting to HCLOG, otherwise %s0%s and hit ENTER\n%s",blue,red,blue,red,blue,normal);
      std::cin >> hclog_switch;
    }
    if (hclog_switch<=-1 || hclog_switch> 2) {
      printf("%sPlease insert a correct No. Exiting the program!%s\n",blue,normal);
      exit(1);
    }
 
    /****************************************************************************/
    TString textfile;
    TString comments_hclog;
    TString contact(Form("%sPlease contact Nuruzzaman (nur@jlab.org) for problems and comments%s\n",blue,normal));

    if (hclog_switch>=1) {
      textfile = Form("%s/%drunsummary.txt",gPlotDir.Data(),gRunNumber);
      FILE *check;
      check = fopen(textfile,"r");
      if (check != NULL) {
	printf("%sPlots for this run are already submitted to HCLOG !!!. Exiting Program%s\n",red,normal);
	printf(contact);
	exit(1);
      }
      if (hclog_switch==1) {
	std::cout << "Please insert comments for the run to post in HCLOG" << std::endl;
	std::cin  >> comments_hclog;
      } else {
	comments_hclog = "Automatic entry from the first 100k analysis";
      }
    } else {
      textfile = Form("%s/%druninfo.txt",gPlotDir.Data(),gRunNumber);
    }
    ofstream outfile(textfile);
    if (comments_hclog.Length()!=0){
      outfile << comments_hclog << std::endl << std::endl;
    }
    outfile << "<pre>\n" 
	    << sslinen
	    << Form("|\tRun Number: %6d\t\t\t\t|\n", gRunNumber)
	    << Form("|%30s,%9s,%13s |\n",gTarget.Data(),gCurrent.Data(),gRasterString.Data())
	    << sslinen
	    << Form("|I                        \t|uA    \t|%s      \t|\n",gCurrent.Data())
	    << Form("|MDALLBARS width          \t|ppm   \t|%2.1f        \t|\n",cal_mdalla)
	    << Form("|US Lumi Sum mean         \t|ppm   \t|%2.1f        \t|\n",cal_uslumi_sum_mean)
	    << Form("|US Lumi Sum width        \t|ppm   \t|%2.1f        \t|\n",cal_uslumi_sum_rms)
	    << Form("|BCM78-ddif width         \t|ppm   \t|%2.1f        \t|\n",cal_bcmdd)
	    << Form("|A_q mean                 \t|ppm   \t|%2.2f        \t|\n",cal_abcmm)
	    << Form("|A_q width                \t|ppm   \t|%2.1f        \t|\n",cal_abcm)
	    << Form("|MDALLBARS X-sens.        \t|ppm/mm\t|%2.1f+-%1.1f \t|\n",cal_mdxsen,cal_emdxsen)
	    << Form("|MDALLBARS Y-sens.        \t|ppm/mm\t|%2.1f+-%1.1f \t|\n",cal_mdysen,cal_emdysen)
	    << Form("|BMOD MDALLBARS X-sens.   \t|ppm/mm\t|%2.1f+-%1.1f \t|\n",1e6*xbmodslope,1e6*exbmodslope)
	    << Form("|BMOD MDALLBARS Y-sens.   \t|ppm/mm\t|%2.1f+-%1.1f \t|\n",1e6*ybmodslope,1e6*eybmodslope)
	    << Form("|BMOD MDALLBARS E-sens.   \t|ppm/mm\t|%2.1f+-%1.1f \t|\n",1e6*ebmodslope,1e6*eebmodslope)
	    << sslinen
	    << "</pre>"
	    <<endl;
    outfile.close();
  
    if (hclog_switch==0) {
      std::cout << dline
		<< "#          Plots are not submitted to HCLOG and saved in directory        #"
		<< std::endl
		<< "#              " << gPlotDir  << "              #"
		<< std::endl
		<< dline;
    } else {
      TString hclog_post_string;

      std::cout << "User Name : " << user_name_hclog << std::endl;
      std::cout << "Subject :   " << MakeSubject() << std::endl;
      std::cout << "Comments :  " << comments_hclog << std::endl;
      std::cout << "Email :     " << email_list << std::endl;
    
      hclog_post_string = "/home/cdaq/bin/Linux/hclog_post ";
      hclog_post_string += WrapParameter("author", user_name_hclog);
      hclog_post_string += WrapParameter("emailto",email_list);
      hclog_post_string += "--tag=\"This is logged by hclog_post and allPlots\" ";
      // hclog_post_string += "  --test ";
      hclog_post_string += "  --cleanup ";

      hclog_post_string += WrapParameter("subject", MakeSubject());
      hclog_post_string += WrapParameter("textfile", textfile);
      hclog_post_string += WrapAttachment(pcharge);
      hclog_post_string += WrapAttachment(pchargeasym);
      hclog_post_string += WrapAttachment(pchargeddasym);
      hclog_post_string += WrapAttachment(pbpmd);
      hclog_post_string += WrapAttachment(pbpmefcharge);
      hclog_post_string += WrapAttachment(pmdallasym);
      hclog_post_string += WrapAttachment(pmdasym);
      hclog_post_string += WrapAttachment(pmdallsenx);
      hclog_post_string += WrapAttachment(pmdallseny);
      hclog_post_string += WrapAttachment(pmdpmtyield);
      hclog_post_string += WrapAttachment(pdslumiasym);
      hclog_post_string += WrapAttachment(plumisenx);
      hclog_post_string += WrapAttachment(plumiseny);
      hclog_post_string += WrapAttachment(pdslumiyield);
      hclog_post_string += WrapAttachment(puslumi);
      hclog_post_string += WrapAttachment(puslumisen);
      hclog_post_string += WrapAttachment(pmodulation);
      hclog_post_string += WrapAttachment(psenbmod);
      hclog_post_string += WrapAttachment(pmdbkg);
      hclog_post_string += WrapAttachment(pmdyieldvar);

      //       printf("%s",hclog_post_string.Data());
      gSystem->Exec(hclog_post_string.Data());

      printf("%s%s#%s    Congratulations !!!!!   All plots are successfully posted to HCLOG     %s#\n%s",dline,red,blue,red,dline);
    }

  }


  if(CREATETEXT){
    Myfile << Form(" \n#================================================================")<<std::endl;
    Myfile << Form("# User = %s, Host = %s, Home Directory = %s \n# Current Directory = %s",
	  getenv("USER"), getenv("HOST"), getenv("HOME"),getenv("PWD"))<<std::endl;
    Myfile << Form("# Date and time of analysis: %s#",asctime(timeinfo))<<std::endl;
    Myfile << Form("# +++++++++++++++:: Analysis conditions ::+++++++++++++++ ")<<std::endl;
    Myfile << Form("# \tRun Number: %d\t %s",gRunNumber,gEnergy.Data())<<std::endl;
    Myfile << Form("# \t%s\t Raster: %s",gTarget.Data(),gRasterString.Data())<<std::endl;
    Myfile << Form("# \t%s\t %s",gCurrent.Data(),gQTORDCCT.Data())<<std::endl;
    Myfile << Form("# +++++++++++++++++++++++++++++++++++++++++++++++++++++++ ")<<std::endl;
    Myfile << Form("# ----------------------:: Charge ::--------------------- ")<<std::endl;
    if(CHARGE){
      Myfile << Form("# Device\t A+-AError ARMS\t Y+-YError YRMS")<<std::endl;
      for ( int i=0; i<7; i++) {
	Myfile << Form("%s\t %4.2f+-%4.2f %4.2f\t %4.2f+-%4.2f %4.2f",
		       bcms[i].Data(),aCharge[i],aChargeError[i],aChargeRMS[i],yCharge[i],yChargeError[i],yChargeRMS[i])<<std::endl;}}
    if(CHARGEDD){
      Myfile << Form("# Device\t DD+-DDError DDRMS")<<std::endl;
      for ( int i=0; i<6; i++) {
	Myfile << Form("%s\t %4.2f+-%4.2f %4.2f",
		       bcms2[i].Data(),ddCharge[i],ddChargeError[i],ddChargeRMS[i])<<std::endl;}}
    else{  Myfile << Form("# No Charge data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# ------------------:: Beam Parameter ::----------------- ")<<std::endl;
    if(BPMS){
      for ( int i=0; i<4; i++) {
	Myfile << Form("%s\t %4.2g+-%4.2g %4.2g\t %4.2g+-%4.2g %4.2g",
		       bpms[i].Data(),difX[i],difXError[i],difXRMS[i],difY[i],difYError[i],difYRMS[i])<<std::endl;}}
    else{  Myfile << Form("# No Beam Parameter data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# -------------------:: Main Detector ::----------------- ")<<std::endl;
    if(MDLUMI){
      Myfile << Form("All\t %4.2f+-%4.2f %4.2f",aMDAll,aMDAllError,aMDAllRMS)<<std::endl;
      for ( int i=0; i<8; i++) {
	Myfile << Form("A_MD%d\t %4.2f+-%4.2f %4.2f\n",i+1,aMD[i],aMDError[i],aMDRMS[i]);}}
    if(MDALLASYM){
      Myfile << Form("bcm1\t %4.2f+-%4.2f %4.2f",amdAll1,amdAll1Error,amdAll1RMS)<<std::endl;
      Myfile << Form("bcm2\t %4.2f+-%4.2f %4.2f",amdAll2,amdAll2Error,amdAll2RMS)<<std::endl;
      Myfile << Form("bcm5\t %4.2f+-%4.2f %4.2f",amdAll5,amdAll5Error,amdAll5RMS)<<std::endl;
      Myfile << Form("bcm6\t %4.2f+-%4.2f %4.2f",amdAll6,amdAll6Error,amdAll6RMS)<<std::endl;
      Myfile << Form("bcm7\t %4.2f+-%4.2f %4.2f",amdAll7,amdAll7Error,amdAll7RMS)<<std::endl;
      Myfile << Form("bcm8\t %4.2f+-%4.2f %4.2f",amdAll8,amdAll8Error,amdAll8RMS)<<std::endl;}
    if(MDBKG){
      Myfile << Form("# Device\t A+-AError ARMS\t Y+-YError YRMS")<<std::endl;
      for ( int i=0; i<6; i++) {
	Myfile << Form("%s\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g",
		       mdbkg[i].Data(),abkg[i],abkgError[i],abkgRMS[i],ybkg[i],ybkgError[i],ybkgRMS[i])<<std::endl;}}
    if(MDPMT){
      for ( int i=0; i<8; i++) {
	Myfile << Form("A_MD%d PMTs\t %4.2f+-%4.2f %4.2f\t %4.2f+-%4.2f %4.2f\n",
	     i+1,aMDPMTpos[i],aMDPMTposError[i],aMDPMTposRMS[i],aMDPMTneg[i],aMDPMTnegError[i],aMDPMTnegRMS[i]);}
      Myfile << Form("# ")<<std::endl;}
    if(MDPMT){
      for ( int i=0; i<8; i++) {
	Myfile << Form("Y_MD%d PMTs\t %4.2g+-%4.2g %4.2g\t %4.2g+-%4.2g %4.2g\n",
		       i+1,yMDPMTpos[i],yMDPMTposError[i],yMDPMTposRMS[i],yMDPMTneg[i],yMDPMTnegError[i],yMDPMTnegRMS[i]);}}
    else{  Myfile << Form("# No Main Detector data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# -----------------:: Downstream LUMIs ::---------------- ")<<std::endl;
    if(MDLUMI){
      for ( int i=0; i<3; i++) {
	Myfile << Form("DS Lumi %s\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n",
		       lumi[i].Data(),adsLumiAll[i],adsLumiAllError[i],adsLumiAllRMS[i],ydsLumiAll[i],ydsLumiAllError[i],ydsLumiAllRMS[i]);}
      for ( int i=0; i<8; i++) {
	Myfile << Form("A_DSLumi%d\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n",
		       i+1,adsLumi[i],adsLumiError[i],adsLumiRMS[i],ydsLumi[i],ydsLumiError[i],ydsLumiRMS[i]);}}
    else{  Myfile << Form("# No DS Lumi data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# ------------------:: Upstream LUMIs ::----------------- ")<<std::endl;
    if(USLUMI){
      for ( int i=0; i<5; i++) {
	Myfile << Form("A_%s\t %4.2f+-%4.2f %4.2f\t %4.2g+-%4.2g %4.2g\n",
		       uslumi[i].Data(),ausLumi[i],ausLumiError[i],ausLumiRMS[i],yusLumi[i],yusLumiError[i],yusLumiRMS[i]);}}
    else{  Myfile << Form("# No US Lumi data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# -------------------:: Sensitivities ::----------------- ")<<std::endl;
    if(SENSITIVITY){
      for ( int i=0; i<3; i++){
	Myfile << Form("MD-%s SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       allmd[i].Data(),senMDAllXp0[i],senMDAllXp0Error[i],senMDAllXp1[i],senMDAllXp1Error[i],senMDAllXChi[i],senMDAllXNDF[i],senMDAllXProb[i]);}}
    if(SENSITIVITY){
      for ( int i=0; i<8; i++){
	Myfile << Form("MD%d SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       i+1,senMDXp0[i],senMDXp0Error[i],senMDXp1[i],senMDXp1Error[i],senMDXChi[i],senMDXNDF[i],senMDXProb[i]);}}
    if(SENSITIVITY){
      for ( int i=0; i<3; i++){
	Myfile << Form("MD-%s SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       allmd[i].Data(),senMDAllYp0[i],senMDAllYp0Error[i],senMDAllYp1[i],senMDAllYp1Error[i],senMDAllYChi[i],senMDAllYNDF[i],senMDAllYProb[i]);}}
    if(SENSITIVITY){
      for ( int i=0; i<8; i++){
	Myfile << Form("MD%d SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       i+1,senMDYp0[i],senMDYp0Error[i],senMDYp1[i],senMDYp1Error[i],senMDYChi[i],senMDYNDF[i],senMDYProb[i]);}
      Myfile << Form("# ")<<std::endl;}

    if(SENSITIVITY){
      for ( int i=0; i<3; i++){
	Myfile << Form("DsLumi-%s SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       lumi[i].Data(),senDsLumiSumXp0[i],senDsLumiSumXp0Error[i],senDsLumiSumXp1[i],senDsLumiSumXp1Error[i],senDsLumiSumXChi[i],senDsLumiSumXNDF[i],senDsLumiSumXProb[i]);}}
    if(SENSITIVITY){
      for ( int i=0; i<8; i++){
	Myfile << Form("DsLumi%d SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       i+1,senDsLumiXp0[i],senDsLumiXp0Error[i],senDsLumiXp1[i],senDsLumiXp1Error[i],senDsLumiXChi[i],senDsLumiXNDF[i],senDsLumiXProb[i]);}}
    if(SENSITIVITY){
      for ( int i=0; i<3; i++){
	Myfile << Form("DsLumi-%s SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       lumi[i].Data(),senDsLumiSumYp0[i],senDsLumiSumYp0Error[i],senDsLumiSumYp1[i],senDsLumiSumYp1Error[i],senDsLumiSumYChi[i],senDsLumiSumYNDF[i],senDsLumiSumYProb[i]);}}
    if(SENSITIVITY){
      for ( int i=0; i<8; i++){
	Myfile << Form("DsLumi%d SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       i+1,senDsLumiYp0[i],senDsLumiYp0Error[i],senDsLumiYp1[i],senDsLumiYp1Error[i],senDsLumiYChi[i],senDsLumiYNDF[i],senDsLumiYProb[i]);}
      Myfile << Form("# ")<<std::endl;}

    if(USLUMISEN){
      for ( int i=0; i<5; i++) {
	Myfile << Form("%s SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       uslumi[i].Data(),senUsLumiXp0[i],senUsLumiXp0Error[i],senUsLumiXp1[i],senUsLumiXp1Error[i],senUsLumiXChi[i],senUsLumiXNDF[i],senUsLumiXProb[i]);}
      for ( int i=0; i<5; i++) {
	Myfile << Form("%s SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n", 
		       uslumi[i].Data(),senUsLumiYp0[i],senUsLumiYp0Error[i],senUsLumiYp1[i],senUsLumiYp1Error[i],senUsLumiYChi[i],senUsLumiYNDF[i],senUsLumiYProb[i]);}}
    else{  Myfile << Form("# No Sensitivity data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# ------------------:: Beam Modulation ::---------------- ")<<std::endl;
    if(BMODSEN){
    Myfile << Form("BMod SenX\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodXp0,senbmodXp0Error,senbmodXp1,senbmodXp1Error,senbmodXChi,senbmodXNDF,senbmodXProb);
    Myfile << Form("BMod SenY\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodYp0,senbmodYp0Error,senbmodYp1,senbmodYp1Error,senbmodYChi,senbmodYNDF,senbmodYProb);
    Myfile << Form("BMod SenE\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodEp0,senbmodEp0Error,senbmodEp1,senbmodEp1Error,senbmodEChi,senbmodENDF,senbmodEProb);
    Myfile << Form("BMod SenXp\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodXpp0,senbmodXpp0Error,senbmodXpp1,senbmodXpp1Error,senbmodXpChi,senbmodXpNDF,senbmodXpProb);
    Myfile << Form("BMod SenYp\t %4.2f+-%4.2f %4.2f+-%4.2f %4.2f %4.0f %4.2f\n",senbmodYpp0,senbmodYpp0Error,senbmodYpp1,senbmodYpp1Error,senbmodYpChi,senbmodYpNDF,senbmodYpProb);
    }
    else{  Myfile << Form("# No BModulation data OR didn't analysed")<<std::endl;}
    Myfile << Form("# ")<<std::endl;
    Myfile << Form("# -----------------------:: MISC ::---------------------- ")<<std::endl;
    Myfile << Form("# ")<<std::endl;

  }
  
  /****************************************************************************/
  std::cout << contact << std::endl;
  std::cout << "Done with everything. Exiting the program ......." << std::endl;
  // exit(1);
  //  gDirectory->Delete("*");
  // return(0);
  /****************************************************************************/
}

/*A function to plot bpm effective charge across hallc beamline*/
void PlotBPM_effectiveq_asyms(TTree* tree, TPad * pad)
{
    
  const Int_t ndevices = 45;
  
  TString devicelist[ndevices]=
    {
      "1i02","1i04","1i06","0i02","0i02a","0i05","0i07","0l01","0l02","0l03",
      "0l04","0l05","0l06","0l07","0l08","0l09","0l10","0r03","0r04",
      "0r05","0r06",
      "bpm3c07","bpm3c07a","bpm3c08","bpm3c11","bpm3c12","bpm3c14","bpm3c16","bpm3c17",
      "bpm3c18","bpm3c19","bpm3p02a","bpm3p02b","bpm3p03a","bpm3c20","bpm3c21","bpm3h02",
      "bpm3h04","bpm3h07a","bpm3h07b","bpm3h07c","bpm3h08","bpm3h09","bpm3h09b","target"
    };


  gStyle->Reset();
  gStyle->SetOptTitle(1);
  gStyle->SetStatColor(10);  
  gStyle->SetStatH(0.4);
  gStyle->SetStatW(0.3);     
  gStyle->SetOptStat(1110); 
  gStyle->SetOptStat(1); 


  // canvas parameters
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetFrameFillColor(10);

  // pads parameters
  gStyle->SetPadColor(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetPadLeftMargin(0.07);  
  gStyle->SetPadRightMargin(0.05);  
  gStyle->SetLabelSize(0.07,"x");
  gStyle->SetLabelSize(0.04,"y");
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);


  pad->Divide(1,2);

  TString command;
  TString cut;
  std::vector<TString>  names;
  TVectorD  mean(0);
  TVectorD  width(0);	
  TVectorD  meanerr(0);
  TVectorD  fakeerr(0);
  TVectorD  fakename(0);
  names.clear();

  Int_t k = 0;

  for(Int_t i=0; i<ndevices; i++) {  
    command = Form("asym_qwk_%s_EffectiveCharge.hw_sum*1e6>>htemp",devicelist[i].Data());
    cut =  Form("asym_qwk_%s_EffectiveCharge.Device_Error_Code == 0 && ErrorFlag == 0",
		devicelist[i].Data());
    tree->Draw(command, cut, "goff");
    
    TH1 * h1 = (TH1D*)gDirectory->Get("htemp");
    if(!h1) exit(1);
    mean.ResizeTo(k+1);
    width.ResizeTo(k+1);	
    meanerr.ResizeTo(k+1);
    fakeerr.ResizeTo(k+1);
    fakename.ResizeTo(k+1);
    
    Double_t meanl = h1->GetMean();
    
    //    std::cout<<"Getting data from : "<<devicelist[i]<<std::endl;
    mean.operator()(k) = meanl;
    width.operator()(k) = h1->GetRMS();
    meanerr.operator()(k) = (h1->GetMeanError());
    fakeerr.operator()(k) = (0.0);
    fakename.operator()(k) =(k+1);
    names.push_back(devicelist[i]);
    k++;
  }

  std::cout<<"here\n";
  Int_t size = mean.GetNoElements();


  TGraphErrors* grp = new TGraphErrors(fakename, mean, fakeerr,meanerr);
  grp->SetMarkerColor(2);
  grp->SetMarkerStyle(21);
  grp->SetMarkerSize(1);
  grp->SetTitle("");
  grp->GetYaxis()->SetTitle("Effective Charge Asymmetry (ppm)");
  grp->GetYaxis()->SetTitleOffset(0.7);

  TGraph* grw = new TGraph(fakename, width);
  grw->SetMarkerColor(2);
  grw->SetMarkerStyle(21);
  grw->SetMarkerSize(1);
  grw->SetTitle("");
  grw->GetYaxis()->SetTitle("Width (ppm)");
  grw->GetYaxis()->SetTitleOffset(0.7);
  
  pad->cd(1);
  TAxis *axis = grp->GetHistogram()->GetXaxis();
  axis->SetLabelOffset(999);
  Double_t ypos =  grp->GetHistogram()->GetMaximum()-grp->GetHistogram()->GetMinimum();
  Double_t ylabel = grp->GetHistogram()->GetMinimum() - 0.01*ypos;
  grp->Draw("AEP");

  TText t;
  t.SetTextSize(0.04);
  t.SetTextAlign(31);
  t.SetTextAngle(90);
  for (Int_t i=0;i<size;i++){
    t.DrawText(fakename[i],ylabel,names[i]);
  }

  pad->cd(2);
  axis = grw->GetHistogram()->GetXaxis();
  axis->SetLabelOffset(999);
  ypos =  grw->GetHistogram()->GetMaximum()-grw->GetHistogram()->GetMinimum();
  ylabel = grw->GetHistogram()->GetMinimum() - 0.01*ypos;
  grw->Draw("APB");

  t.SetTextSize(0.04);
  t.SetTextAlign(31);
  t.SetTextAngle(90);
  for (Int_t i=0;i<size;i++){
    t.DrawText(fakename[i],ylabel,names[i]);
  }

  gPad->Update();
  //  std::cout<<"Done plotting!\n";
};




/******************************************************************************************************/
/*                                User Manual                                                         */
/*                                                                                                    */
/*Step1: Open a terminal and run 'allPlots'                                                         */
/*Step2: It will ask for a 'run #'. Type 'run '# and press 'ENTER'.                                   */
/*Step3: Next it will ask for 'target' information. Type 1=LH2, 2=4% US Al, 3=2% US Al, 4=1% US Al,   */
/*       5=4% DS Al, 6=2% DS Al, 7=1% DS Al, 8=US Pure Al and hit ENTER.                              */
/*Step4: If all the plots are done, it will ask wheather you want to submit the plots to HCLOG or not.*/
/*       Insert '1' for to submit the plots to HCLOG otherwise '0' and press 'ENTER'                  */
/*Step5: If you choose to submit all the plots to HCLOG, it will ask for comments                     */
/*       Please insert comments and hit 'ENTER'.                                                      */
/*       e.g; Beam currents was ramping up during the run.                                            */
/*                                                                                                    */
/*       If you didn't submitted plots in HCLOG (otherwise deleted), will be saved in the directory:  */
/*       'users/qwanalysis/plots/'                                                                    */
/*       Type 'display filename.png' & 'emacs filename.txt' to view the figures & run information.    */
/******************************************************************************************************/
