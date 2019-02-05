//*****************************************************************************************************//
// Author      : Nuruzzaman on 07/16/2013
// Last Update : 03/28/2014
// 
//*****************************************************************************************************//
/*
  This macro is to extract final physics asymmetry for Transverse N-to-Delta.

******************************************************************************************************/

using namespace std;
#include "NurClass.h"


int main(Int_t argc,Char_t* argv[]){

  Bool_t SUMMARY_PLOT = 1;
  Bool_t SUMMARY_PLOT2 = 0;
  Bool_t ERROR_PLOT = 1;
  Bool_t PHYSERROR_PLOT = 0;

  Double_t figSize = 2.0;

  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //   printf ( "%sThe current date/time is: %s%s%s",blue,red,asctime(timeinfo),normal);

  TString database="qw_run2_pass5";
  TString database_stem="run2_pass5";
  //   TString deviceTitle = "Barsum"; TString deviceName = "Barsum";
  TString deviceTitle = "PMTavg"; TString deviceName = "PMTavg";

  Bool_t FIGURE = 0;

  std::ofstream MyfileFinal,MyfileFinalAsymmetry,MyfileRegAsymmetry;


  TString target, polar,targ, goodfor, reg_set, reg_calc;

  Int_t opt =3;
  Int_t ropt = 2;
  Int_t qtor_opt = 2;

  TString good_for;
  TString qtor_current;
  TString qtor_stem;
  TString good;
  TString interaction,interaction2;

  /* Canvas and Pad and Scale parameters */
  Int_t canvasSize[6] ={1600*figSize,1000*figSize,1200*figSize,1000*figSize,1200*figSize,650*figSize};
  Double_t pad1x[2] = {0.005,0.995};
  Double_t pad1y[2] = {0.935,0.995};
  Double_t pad2x[2] = {0.005,0.995};
  Double_t pad2y[2] = {0.005,0.945};


//   std::cout<<Form("###############################################")<<std::endl;
//   std::cout<<Form(" \nSlug averages of Main Detector Asymmetries \n")<<std::endl;
//   std::cout<<Form("###############################################")<<std::endl;
//   std::cout<<Form("Enter target type (%sJust Hit ENTER to choose default%s):",blue,normal)<<std::endl;
//   std::cout<<Form("1. %sLiquid Hydrogen (deafult)%s",blue,normal)<<std::endl;
//   std::cout<<Form("2. 4% DS Al ")<<std::endl;
//   std::cout<<Form("3. 1.6% DS Carbon ")<<std::endl;
//   //   std::cin>>opt;
//   std::string input_opt;
//   std::getline( std::cin, input_opt );
//   if ( !input_opt.empty() ) {
//     std::istringstream stream( input_opt );
//     stream >> opt;
//   }


//   //ropt = 2;
//   // select the QTor current
//   //    if(ropt==2){
//       std::cout<<Form("Enter QTOR current (%sJust Hit ENTER to choose default%s):",blue,normal)<<std::endl;
//       std::cout<<Form("1.%s6000 A (deafult)%s",blue,normal)<<std::endl;
//       std::cout<<Form("2.6700 A ")<<std::endl;
//       std::cout<<Form("3.7300 A ")<<std::endl;
//       //std::cin>>qtor_opt;
//       std::string input_qtor_opt;
//       std::getline( std::cin, input_qtor_opt );
//       if ( !input_qtor_opt.empty() ) {
//         std::istringstream stream( input_qtor_opt );
//         stream >> qtor_opt;
//       }
//       //    }


  if(argc>1) database = argv[1];

  // select the target
  if(opt == 1){
    target = "HYDROGEN-CELL";
    targ = "HYDROGEN-CELL";
  }
  else if(opt == 2){
    target = "DS-4-Aluminum";
    targ = "DS 4\% Al";
  }
  else if(opt == 3){
    target = "DS-1.6-C";
    targ = "DS 1.6\% Carbon";
  }
  else{
    std::cout<<Form("Unknown target type!")<<std::endl;
    exit(1);
  }

  
  // Select the interaction
  if(ropt == 1){
    good_for = "(md_data_view.good_for_id = '3' or md_data_view.good_for_id = '1,"+good+"')";
    interaction = "elastic";
    qtor_current=" (slow_controls_settings.qtor_current>8800 && slow_controls_settings.qtor_current<9000) ";
    qtor_stem = "8901";
  }
  else if(ropt == 2){
    good_for = "(md_data_view.good_for_id = '"+good+",18')";
    interaction  = "n-to-delta"; 
    interaction2 = "N-to-#Delta"; 
  }
  else if(ropt == 3){
    good_for = "(md_data_view.good_for_id = '1,3,21')";
    qtor_current=" (slow_controls_settings.qtor_current>8990 && slow_controls_settings.qtor_current<9010) ";
    qtor_stem = "9000";
    interaction = "dis"; 
  }
  else{
    std::cout<<Form("Unknown interaction type!")<<std::endl;
    exit(1);
  }

  // QTOR current cut
  if(qtor_opt == 1){
    qtor_current=" (slow_controls_settings.qtor_current>5900 && slow_controls_settings.qtor_current<6100) ";
    qtor_stem = "6000";
  }
  else if(qtor_opt == 2){
    qtor_current=" (slow_controls_settings.qtor_current>6600 && slow_controls_settings.qtor_current<6800) ";
    qtor_stem = "6700";
  }
  else if(qtor_opt == 3){
    qtor_current=" (slow_controls_settings.qtor_current>7200 && slow_controls_settings.qtor_current<7400) ";
    qtor_stem = "7300";
  }



  TApplication theApp("App",&argc,argv);

  Char_t  textfileFinal[400], textfileFinalAsymmetry[400],textfileRegAsymmetry[400];
  sprintf(textfileFinal,"dirPlot/resultText/%s_%s_%s_MD_Final_%s.txt"
	  ,interaction.Data(),qtor_stem.Data(),target.Data(),database_stem.Data());
  sprintf(textfileFinalAsymmetry,"dirPlot/resultText/%s_%s_%s_MD_Final_Asymmetry_%s.txt"
	  ,interaction.Data(),qtor_stem.Data(),target.Data(),database_stem.Data());
  sprintf(textfileRegAsymmetry,"dirPlot/resultText/%s_%s_%s_MD_Reg_Asymmetry_%s.txt"
	  ,interaction.Data(),qtor_stem.Data(),target.Data(),database_stem.Data());


  /********************************************/
  /*            Initilize variables           */
  /********************************************/

  Double_t A_msr_v,dAmsr_v,dAmsr_stat_v,dAmsr_sys_v;
  Double_t A_msr_h,dAmsr_h,dAmsr_stat_h,dAmsr_sys_h;
  Double_t A_uslumi_h,dAuslumi_h,dAuslumi_stat_h,dAuslumi_sys_h;
  Double_t A_uslumi_v,dAuslumi_v,dAuslumi_stat_v,dAuslumi_sys_v;
  Double_t A_uslumi,dAuslumi,MarkDfactor,dMarkDfactor,BBfactor,dBBfactor;
  Double_t A_T_Moller,dA_T_Moller;
  Double_t A_DSal_h,dADSal_h,dADSal_stat_h,dADSal_sys_h;
  Double_t A_DSal_v,dADSal_v,dADSal_stat_v,dADSal_sys_v;
  Double_t A_uslumi_h_3pos,dAuslumi_h_3pos,A_uslumi_h_7pos,dAuslumi_h_7pos;
  Double_t A_DSal,dADSal,A_USal,dAUSal,A_al,dAal;
  Double_t A_DSal_DetAcpt,dADSalDetAcpt,dAal_nonlin;
  Double_t Al_nonlin_h,Al_nonlin_v,calc_Al_nonlin;
  Double_t BlindingFactor,BlindingCorrection;
  Double_t A_msr,dAmsr_stat,dAmsr_sys,dAmsr;
  Double_t A_msr_DetAcpt,dADetAcpt,dADetAcpt_h,dADetAcpt_v,DetAcpt;
  Double_t dAmsr_P,dAmsr_P_h,dAmsr_P_v;
  Double_t dAmsr_regscheme,dAmsr_regscheme_h,dAmsr_regscheme_v;
  Double_t dAmsr_reg_time,dAmsr_reg_time_h,dAmsr_reg_time_v;
  Double_t dAmsr_nonlin,dAmsr_nonlin_h,dAmsr_nonlin_v;
  Double_t dAmsr_cuts,dAmsr_cuts_h,dAmsr_cuts_v;
  Double_t dAmsr_fit,dAmsr_fit_h,dAmsr_fit_v;
  Double_t dAmsr_q2_acceptance,dAmsr_q2_acceptance_h,dAmsr_q2_acceptance_v;
  Double_t dAmsr_trans_h,dAmsr_trans_v,dAmsr_trans;
  Double_t P,dP_stat,dP_sys,dP;
  Double_t Q2,dQ2,Q2_el,dQ2_el;
  Double_t Theta,dTheta;
  Double_t E,dE;
  Double_t A_b1,dA_b1,f_b1,df_b1,c_b1;
  Double_t A_b2,dA_b2,f_b2,df_b2,c_b2;
  Double_t A_b3,dA_b3,f_b3,df_b3,c_b3;
  Double_t A_b4,dA_b4,f_b4,df_b4,c_b4,R_b4,dR_b4,A_T_el,dA_T_el,dA_T_el_stat,dA_T_el_sys;
  Double_t df_b1_stat,df_b1_sys,df_b1_model;
  Double_t f_allneutral,df_allneutral,df_allneutral_stat,df_allneutral_sys;
  Double_t DetCorr,dDetCorr;
  Double_t RadCorr,dRadCorr;
  Double_t BinCenterCorr,dBinCenterCorr;
  Double_t Q2Corr,dQ2Corr;
  Double_t PhiCorr,dPhiCorr;
  Double_t TotalCorr,f_Total;
  Double_t Kappa;
  Double_t A_msr_unreg,RegCorr,A_msr_unreg_v,RegCorr_v,A_msr_unreg_h,RegCorr_h;
  Double_t dA_msr_unreg,dA_msr_unreg_v,dA_msr_unreg_h;
//   Double_t dAmsr_dblpeak_h,dAmsr_dblpeak_v,dAmsr_dblpeak;
  Double_t A_phys,dAphys,dAphys_stat,dAphys_sys;
  Double_t dAphys_Amsr,dAphys_P,dAphys_RC,dAphys_Det,dAphys_Bin,dAphys_Phi,dAphys_Q2;
  Double_t dAphys_Ab1,dAphys_Ab2,dAphys_Ab3,dAphys_Ab4;
  Double_t dAphys_fb1,dAphys_fb2,dAphys_fb3,dAphys_fb4;

  TString line;
  line  = Form("*********************************************************************");




  /*****************************************************************************/
  /*                                                                           */
  /*                                    Input                                  */
  /*                                                                           */
  /*****************************************************************************/

  // ************************************************************************
  // Horizontal Regressed Measured Asymmetry
  // ************************************************************************
  // 

  A_msr_h          = 9.86888; // std. reg., no fit to phi_0
  dAmsr_stat_h     = 1.87015;
//   A_msr_h          = 10.190;
//   dAmsr_stat_h     = 1.863;

  // *****************************************************************************
  // Information only: Unregressed Asymmetry and Size of Std Regression Correction
  // ******************************************************************************

  A_msr_unreg_h    = 10.240;
  dA_msr_unreg_h   = 1.868;


  A_msr_unreg      = A_msr_unreg_h;
  dA_msr_unreg     = dA_msr_unreg_h;

  RegCorr_h        = TMath::Abs(A_msr_unreg_h - A_msr_h); // note A_msr = A_msr_unreg + RegCorr

  // ****************************************************************************
  // Combined MEASURED ASYMMETRY for horizontal and vertical transverse
  // ****************************************************************************
  A_msr      = A_msr_h;
  dAmsr_stat = dAmsr_stat_h;
  RegCorr    = TMath::Abs(A_msr_unreg - A_msr);

  // *****************************************************************************
  // Detector acceptance correction of measured asymmetry
  // *****************************************************************************
  DetAcpt       = 0.9938;
  A_msr_DetAcpt = A_msr/DetAcpt;
  dADetAcpt     = 0.50*TMath::Abs(A_msr - A_msr_DetAcpt);
  dADetAcpt_h   = 0.50*TMath::Abs(A_msr_h - A_msr_h/DetAcpt);


  // *****************************************************************************
  // BEAM POLARIZATION
  // *****************************************************************************

//   P = 0.8796; // from DocDB 1655, only consedered run2
//   dP = 0.018; // Global error of 2% is added
  P       = 0.87497; // private communication with Josh. ELOG Anc 91
  dP_stat = 0.00277; // private communication with Josh. ELOG Anc 91
//   dP_sys  = 0.0084; // DocDB 1955.  ELOG Anc 91
  dP_sys  = 0.00735; // DocDB 1955.  ELOG Anc 91 and 109
  dP      = TMath::Sqrt(  pow(dP_stat,2) + pow(dP_sys,2) ); // Global error of 2% is added




  // *****************************************************************************
  // US Lumi Asymmetry
  // *****************************************************************************
  // asymmetries are in ppm

  A_uslumi_h      = 0.440;
  dAuslumi_stat_h = 0.130;
  dAuslumi_sys_h  = 0.000;
  dAuslumi_h      = TMath::Sqrt(  pow(dAuslumi_stat_h,2) + pow(dAuslumi_sys_h,2) );


  A_uslumi_h_3pos = 5.77;
  dAuslumi_h_3pos = 0.21;

  A_uslumi_h_7pos = -5.08;
  dAuslumi_h_7pos = 0.16;

//   A_uslumi        = A_uslumi_h_3pos - A_uslumi_h_7pos; // Max. variation between USLumi octant asym.
//   dAuslumi        = 1/TMath::Sqrt( (1/pow(dAuslumi_h_3pos,2)) + (1/pow(dAuslumi_h_7pos,2)) );

  A_uslumi        = 3.534;
  dAuslumi        = 0.1399;



  MarkDfactor     = 0.0046; // Elog 787:Mark Dalton
  dMarkDfactor    = 0.0014;

//   BBfactor        = 0.0085; //Manolis, https://qweak.jlab.org/elog/Analysis+%26+Simulation/140710_125210/14-07-10_BeamlineBackgrounds.pdf
//   dBBfactor       = 0.0016; // Manolis, Elog 

  BBfactor        = 0.085; //Manolis, Added factor of 10 for inelastic signal drop
  dBBfactor       = 0.016; // Manolis, 

//   cout<<red<<"USLumi A_h "<<A_uslumi_h<<" +- "<<dAuslumi_h<<normal<<endl;
//   cout<<blue<<"USLumi A_v "<<A_uslumi_v<<" +- "<<dAuslumi_v<<normal<<endl;
  cout<<red<<"USLumi A  "<<A_uslumi<<" +- "<<dAuslumi<<normal<<endl;



  // *****************************************************************************
  // Systematic uncertinity calculation
  // *****************************************************************************

  // Regression Scheme Uncertainty
  // ibid. see table of regression corrections for slugs 1013-1019

  dAmsr_regscheme_h = 0.011;
  dAmsr_regscheme   = dAmsr_regscheme_h;


  // Regression Time Dependence Uncertainty
  dAmsr_reg_time_h = 0.008;
  dAmsr_reg_time   = dAmsr_reg_time_h;

  // Fit Scheme Dependence Uncertainty
  dAmsr_fit_h = 0.355;
  dAmsr_fit   = dAmsr_fit_h;


  // Nonlinearity Correction Uncertainty 
  // In https://qweak.jlab.org/elog/Analysis+%26+Simulation/743 , 
  // Katherine gives an average value for the nonlinearity of -1.1% for slugs 1013-1019. 
  // reminder: the following calculation for Al is not contingent upon any blinding factor 
  dAmsr_nonlin_h = TMath::Abs(-0.04*A_msr_h);
  dAmsr_nonlin   = dAmsr_nonlin_h;

  // Cuts Dependence Ambiguity
  // Katherine argues for 3.6 ppb in https://qweak.jlab.org/elog/Analysis+%26+Simulation/743 .
  // This seems appropriately smaller than the pass4 value in her thesis p. 150 of 14.9 ppb.
  dAmsr_cuts_h = 0.140;
  dAmsr_cuts   = dAmsr_cuts_h;




  // Systematic calculation 

  dAmsr_sys_h = TMath::Sqrt(  pow(dAmsr_regscheme_h,2) + pow(dAmsr_reg_time_h,2) + pow(dAmsr_nonlin_h,2) + pow(dAmsr_cuts_h,2) + pow(dAmsr_fit_h,2) );
  dAmsr_sys = dAmsr_sys_h;

  dAmsr_h = TMath::Sqrt(  pow(dAmsr_stat_h,2) + pow(dAmsr_sys_h,2) );
  dAmsr   = dAmsr_h;


  // *****************************************************************************
  // MISC Inputs
  // *****************************************************************************

  // ** Q2 from geant3 by Nur. Units are (GeV/c)^2. J. Leacock had 0.02078+- 0.0005. 
  // Error bar set to be 2.4% of Q2 using 
  // Siyuan's Col talk DocDB 1646/ Mark Pitt ELOG 774.
  Q2  = 0.02088;
  dQ2 = 0.024*Q2;

  Q2_el  = 0.0250;
  dQ2_el = 0.0006;

  // ** Theta from geant3 by Nur. Units are (degree) 
  // Error bar set to RMS of the distribution for now. 
  Theta  = 8.348;
  dTheta = 1.337;

  // Energy is from Q-weak PhysRevLett.111.141803 paper.
  E  = 1.155;
  dE = 0.003;


  // *****************************************************************************
  // BACKGROUNDS
  // *****************************************************************************
  // **  In accordance with Eqn 4 in the version 2 Qweak tech note at 
  // https://qweak.jlab.org/doc-private/ShowDocument?docid=965
  // background asymmetries are implicitly understood to have been corrected by
  // the appropriate factor of 1/P. **

  // Bkg 1 Al windows
  // ** Set asymmetry and dilution to zero. Does not exist for 4% DS Al target.**
//   A_b1  = 8.43084185469048;
//   dA_b1 = 0.985267452607854;
  A_b1  = A_al/P;
  dA_b1 = dAal/P;
  // Al window bkg dilution 
  f_b1 = 0.03299;           // DocDB 2042, page 2
//   df_b1 = 0.00221585198062;           

  df_b1_stat  = 0.0001;
  df_b1_sys   = 0.0007;
  df_b1_model = 0.0021;

  df_b1 = TMath::Sqrt( pow(df_b1_stat,2) + pow(df_b1_sys,2) + pow(df_b1_model,2) );


  // The neutral backgrounds will be partitioned as per Mark Pitt's suggestion:
  // i.e., that the majority of the neutral background will carry an elastic-like asymmetry
  // while only the shutter-closed background will carry the large asymmetry implied by MD9, etc.


  // Bkg 2 beamline background neutrals
  // Kamyers's elog https://qweak.jlab.org/elog/Analysis+%26+Simulation/793
  // proposes we need to be more conservative knowledge of the beamline asymmetry
  // during Al running for Wien0. Constraints from Al data taking in Run I
  // are much weaker than they are for Hydrogen elastic. Run II data will help. 
  // (Hey, we agreed we wanted to go beyond the 0.5+-1ppm assumption from p. 154 and Fig 5.19
  // of Katherine's thesis. We implemented that for Hydrogen recently. This is the other shoe 
  // dropping just as we were about to drift off to sleep.) 
//   A_b2 = -5.500;
//   dA_b2 = 11.500;

  // obsolete      A_b2 = 0.5;
  // obsolete      dA_b2 = 1.5;
  // ** The following lead wall plug dilutions should be superseded with more precise 
  // W shutter measurements if they exist for Al targets. **
  // Impossible! That would be like asking for sharks with laser beams on their heads. 
  // So we're stuck using the value from Kamyers's thesis p. 104
  // For Inelastic the beamline background is a factor larger compare to elastic.
  // From J. Leacock p 169.
  f_b2 = 0.0179;
//   df_b2 = 0.0008;
//   df_b2 = 1.0*f_b2; // for now asummed to be 100% error
  df_b2 = 0.5*f_b2; // for now asummed to be 50% error

//   A_b2 = (1/P)*(A_uslumi*MarkDfactor)/f_b2;
//   dA_b2 = TMath::Sqrt( pow( ((dAuslumi*MarkDfactor)/f_b2) ,2) + pow( ((A_uslumi*dMarkDfactor)/f_b2) ,2) + pow( ((A_uslumi*MarkDfactor*df_b2)/pow(f_b2,2)) ,2) );
//   A_b2 = (1/P)*(A_uslumi*BBfactor)/f_b2;
//   dA_b2 = TMath::Sqrt( pow( ((dAuslumi*BBfactor)/f_b2) ,2) + pow( ((A_uslumi*dBBfactor)/f_b2) ,2) + pow( ((A_uslumi*BBfactor*df_b2)/pow(f_b2,2)) ,2) );
//   A_b2 = A_uslumi*BBfactor;
//   dA_b2 = TMath::Sqrt( pow( (dAuslumi*BBfactor) ,2) + pow( (A_uslumi*dBBfactor) ,2) );
  A_b2 = A_uslumi*BBfactor;
  dA_b2 = TMath::Sqrt( pow( (dAuslumi*BBfactor) ,2) + pow( (A_uslumi*dBBfactor) ,2) );
//   A_b2 = (1/P)*(A_uslumi*BBfactor)/f_b2;
//   dA_b2 = TMath::Sqrt( pow( ((dAuslumi*BBfactor)/(P*f_b2)) ,2) + pow( ((A_uslumi*dBBfactor)/(P*f_b2)) ,2) + pow( ((A_uslumi*BBfactor*df_b2)/(P*pow(f_b2,2))) ,2) + pow( ((A_uslumi*BBfactor*dP)/(f_b2*pow(P,2))) ,2) );


  // Bkg 3 QTOR transport channel neutrals               
  // ** Simulation is needed to estimate the asymmetry of the trivial (scraping) component. **
  // Given the lack of supporting simulations, Kent pointed out that assuming Moeller dominance 
  // would yield a less conservative assumption than elastic dominance. We'll keep the error
  // bar from the elastic dominance assumption however.
  //
  //We have measured transverse asymmetry at Moller peak. 
  //Moller peak is assumed to be around QTorc urrent 600 A. The measured asymmetry is from 
  //run 17830. 9.33+-0.54 ppm (sign corrected).
  A_T_Moller   = 9.33; //ELOG Analysis 553, Buddhini 
  dA_T_Moller  = 0.54; 


//   A_b3  = TMath::Abs(A_msr - A_T_Moller);
//   dA_b3 = TMath::Sqrt( pow(dAmsr,2) + pow(dA_T_Moller,2) );
  A_b3  = 0.000;
  dA_b3 = 10.0;

  //      A_b3 =  -5.0;dA_b3 = 1.0;
  // dilution = Rakitha-style all neutral bkg minus the shutter-based beamline bkg  
  // Unfortunately, no Rakitha-style result exists for Al from which we can subtract the 
  // shutter-based beamline bkg. Until it does, I'll use the Hydrogen elastic result below
  // as a guide and assign a 100% relative uncertainty. (WAG. No reference.) 
  //   f_b2 = 0.052;      
  //   df_b2 = 0.004;
//   f_allneutral = 0.04205; //from Rakitha Elog 813
//   df_allneutral = 0.00954594;
//   df_allneutral = 0.01103;
  f_allneutral       = 0.052;  //from Rakitha,DocDB 1549
  df_allneutral_stat = 0.004;  //from Rakitha,DocDB 1549
  df_allneutral_sys  = 0.0014; //from Rakitha, Elog 813. Elastic systematic study is used to assign for inelastic case.
  df_allneutral      = TMath::Sqrt( pow(df_allneutral_stat,2) + pow(df_allneutral_sys,2) );

  f_b3 = f_allneutral - f_b2;
  df_b3 = TMath::Sqrt( pow(df_allneutral,2) + pow(df_b2,2) );
//   df_b3 = 1.0*f_b3;


  // Bkg 4 Non-signal Electrons in QTOR transport channel (N to Delta in this case)          
  // ** Set to zero for now. ** Here, inelastics as part of the measured Al so don't want 
  // to subtract them. If for some reason there are significant differences between the 
  // inelastic bkg fractions in the 4% DS Al and actual thin window targets, then that 
  // should probably be handled separately as a "radiative" correction. 
  A_T_el       = -5.35; //Doc-DB 2019, Buddhini PAVI-14 talk.
  dA_T_el_stat = 0.07;
  dA_T_el_sys  = 0.15;
//   A_T_el       = -5.35; //Doc-DB 1961, Buddhini Hall-C Collaboration Meeting
//   dA_T_el_stat = 0.07;
//   dA_T_el_sys  = 0.15;

  dA_T_el = TMath::Sqrt( pow(dA_T_el_stat,2) + pow(dA_T_el_sys,2) );

  Double_t A_b4_2 = A_T_el*(Q2/Q2_el); 
  Double_t dA_b4_2   = TMath::Sqrt( pow((dA_T_el*(Q2/Q2_el)),2) + pow((A_T_el*(dQ2/Q2_el)),2) + pow((A_T_el*dQ2_el*(Q2/(pow(Q2_el,2)))),2) );

  A_b4 = A_T_el*TMath::Sqrt(Q2/Q2_el); 
  dA_b4   = TMath::Sqrt( pow((dA_T_el*TMath::Sqrt(Q2/Q2_el)),2) + pow((A_T_el*(1/2)*(dQ2/TMath::Sqrt(Q2*Q2_el))),2) + pow((A_T_el*(-1/2)*dQ2_el*TMath::Sqrt(Q2/(pow(Q2_el,3)))),2) );
//   dA_b4 = 0.166;
//   A_b4 = -4.754; 
//   dA_b4 = 0.149;

  f_b4 = 0.701007;
//   df_b4 = 0.05*f_b4;
  df_b4 = 0.1*f_b4;
//   f_b4 = 0.701007;
//   df_b4 = 0.0130575673504083;

  R_b4 = 0.896; 
  dR_b4 = 0.002;


//      A_b3 = 5.92; dA_b3 = 2.0;
//   A_b4 = -5.305*0.90; dA_b4 = 0.166*1.1; 


  f_Total = f_b1 + f_b2 + f_b3 + f_b4;

  // *****************************************************************************
  // Multiplicative (radiative and other) corrections
  // *****************************************************************************

  // ** No radiative correction for Al data as a background. Only need
  // that for physics interpretation. **
  RadCorr = 1.010; // Buddhini Thesis DocDB 1886
  dRadCorr = 0.004; // Buddhini Thesis DocDB 1886
//   RadCorr = 1.000;                       
//   dRadCorr = 0.0*TMath::Abs(1.-RadCorr); //  no uncertainty on the correction


  // ** No such correction is needed for purposes of bkg subtraction. **
  // This can be revisited later when precision improves or if we
  // try to interpret the Al physics asymmetry. 
  DetCorr  = 0.998; // Buddhini Thesis DocDB 1886
  dDetCorr = 0.001; // Buddhini Thesis DocDB 1886
//   DetCorr  = 1.0000;
//   dDetCorr = 0.0000;

 
  // ** No bin centering correction for Al data as a background. Only need
  // that for physics interpretation. **
  BinCenterCorr  = 1.000;
  dBinCenterCorr = 0.0*TMath::Abs(1.-BinCenterCorr);

  // ** No Q2 for precision calibration of Q2 yet.
  dAmsr_q2_acceptance_h = 0.064;
  dAmsr_q2_acceptance   = dAmsr_q2_acceptance_h;

  Q2Corr  = 1.000;
//   dQ2Corr = 0.030; // From Q-weak PRL.111.141803 paper
//   dQ2Corr = 0.0*TMath::Abs(1.-Q2Corr);
  //   dQ2Corr = TMath::Abs((dQ2)/Q2);
  dQ2Corr = dAmsr_q2_acceptance/A_msr;

  //Azimuthal acceptance correction
  PhiCorr   = 1.006;
  dPhiCorr  = 0.004;

  TotalCorr = RadCorr*DetCorr*Q2Corr;

  // *****************************************************************************
  // Constant
  // *****************************************************************************
  Kappa = (TotalCorr/P)/(1-f_Total);
  cout<<"Kappa"<<Kappa<<endl;
  // *****************************************************************************
  // Individual background corrections
  // *****************************************************************************

  c_b1 = A_b1*f_b1*P*Kappa;
  c_b2 = A_b2*f_b2*P*Kappa;
  c_b3 = A_b3*f_b3*P*Kappa;
//   c_b4 = A_b4*f_b4*R_b4*P*Kappa;
  c_b4 = A_b4*f_b4*P*Kappa;
 Double_t c_b4_2 = A_b4_2*f_b4*R_b4*P*Kappa;
  
  // ******************************************
  // Interpretational Error from the Acceptance
  // *******************************************
  // ** No such correction is needed for the purposes of bkg subtraction. **


  // *********************************************
  // Interpretational Error from 2-photon Exchange
  // **********************************************
  // ** No such correction is needed for the purposes of bkg subtraction. **




  // *****************************************************************************
  // Extraction of physics asymmetry
  // ******************************************************************************

  A_phys       = TotalCorr*( ((A_msr_DetAcpt/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )/(1 - f_Total) );

//   dAphys_Amsr  = TMath::Abs(TotalCorr*( ((dAmsr_stat/P))/(1 - f_Total) ));
  dAphys_Amsr  = TMath::Abs(TotalCorr*( ((dAmsr/P))/(1 - f_Total) ));
  dAphys_P     = TMath::Abs(TotalCorr*( ((A_msr/P)*(dP/P))/(1 - f_Total) ));
  dAphys_Ab1   = TMath::Abs(TotalCorr*( ( - dA_b1*f_b1 )/(1 - f_Total) ));
  dAphys_Ab2   = TMath::Abs(TotalCorr*( ( - dA_b2*f_b2 )/(1 - f_Total) ));
  dAphys_Ab3   = TMath::Abs(TotalCorr*( ( - dA_b3*f_b3 )/(1 - f_Total) ));
  dAphys_Ab4   = TMath::Abs(TotalCorr*( ( - dA_b4*f_b4 )/(1 - f_Total) ));

  dAphys_fb1   = TMath::Abs(TotalCorr*( ((A_msr/P) - A_b1*(1-f_b2-f_b3-f_b4) - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )*df_b1 /pow((1 - f_Total),2) ));
  dAphys_fb2   = TMath::Abs(TotalCorr*( ((A_msr/P) - A_b1*f_b1 - A_b2*(1-f_b1-f_b3-f_b4) - A_b3*f_b3 - A_b4*f_b4 )*df_b2 /pow((1 - f_Total),2) ));
  dAphys_fb3   = TMath::Abs(TotalCorr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*(1-f_b1-f_b2-f_b4) - A_b4*f_b4 )*df_b3 /pow((1 - f_Total),2) ));
  dAphys_fb4   = TMath::Abs(TotalCorr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*(1-f_b1-f_b2-f_b3) )*df_b4 /pow((1 - f_Total),2) ));

  dAphys_RC    = TMath::Abs( dRadCorr*DetCorr*PhiCorr*Q2Corr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )/(1 - f_Total) ));
  dAphys_Det   = TMath::Abs( RadCorr*dDetCorr*PhiCorr*Q2Corr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )/(1 - f_Total) ));
  dAphys_Bin   = TMath::Abs( RadCorr*DetCorr*dBinCenterCorr*Q2Corr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )/(1 - f_Total) ));
  dAphys_Q2    = TMath::Abs( RadCorr*DetCorr*PhiCorr*dQ2Corr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )/(1 - f_Total) ));
  dAphys_Phi   = TMath::Abs( RadCorr*DetCorr*dPhiCorr*Q2Corr*( ((A_msr/P) - A_b1*f_b1 - A_b2*f_b2 - A_b3*f_b3 - A_b4*f_b4 )/(1 - f_Total) ));

  dAphys_stat  =  TMath::Abs(dAphys_Amsr);

  dAphys_sys   =  TMath::Sqrt(pow(dAphys_P,2) + pow(dAphys_Ab1,2) + pow(dAphys_Ab2,2) + pow(dAphys_Ab3,2) + pow(dAphys_Ab4,2) + pow(dAphys_fb1,2) + pow(dAphys_fb2,2) + pow(dAphys_fb3,2) + pow(dAphys_fb4,2) + pow(dAphys_RC,2) + pow(dAphys_Det,2) + pow(dAphys_Q2,2) );

  dAphys       =  TMath::Sqrt( pow(dAphys_Amsr,2) + pow(dAphys_P,2) + pow(dAphys_Ab1,2) + pow(dAphys_Ab2,2) + pow(dAphys_Ab3,2) + pow(dAphys_Ab4,2) + pow(dAphys_fb1,2) + pow(dAphys_fb2,2) + pow(dAphys_fb3,2) + pow(dAphys_fb4,2) + pow(dAphys_RC,2) + pow(dAphys_Det,2) + pow(dAphys_Q2,2) );


  /*****************************************************************************/
  /*                                                                           */
  /*             Calculation for transverse analysis ends here                 */
  /*                                                                           */
  /*****************************************************************************/




  /*****************************************************************************/
  /*                                                                           */
  /*                                    Output                                 */
  /*                                                                           */
  /*****************************************************************************/



  cout <<"\n"<<line<<endl;

  TString unitPPM = "ppm";

  /*********************************************/

  TString measuredAsymmetry_h, measuredAsymmetry_h1, measuredAsymmetry_h2, measuredAsymmetry_h3;
  TString measuredAsymmetry_h4, measuredAsymmetry_h5, measuredAsymmetry_h6, measuredAsymmetry_h7;
  TString measuredAsymmetry_h8, measuredAsymmetry_h9, measuredAsymmetry_h10, measuredAsymmetry_h11;
  TString measuredAsymmetry_h12, measuredAsymmetry_h13, measuredAsymmetry_h14, measuredAsymmetry_h15;

  measuredAsymmetry_h1  = Form("Measured Asymmetry and Errors for Horizontal Transverse:");
  measuredAsymmetry_h2  = Form("Amsr_h(unregressed) \t = %2.3f ppm \tRegression Correction \t = %2.3f ppm ", A_msr_unreg_h,RegCorr_h);
  measuredAsymmetry_h3  = Form("Amsr_h \t\t\t = %2.3f ppm ", A_msr_h);
  measuredAsymmetry_h4  = Form("dAmsr_stat_h \t\t = %2.3f ppm ", dAmsr_stat_h);
  measuredAsymmetry_h5  = Form("dAmsr_regscheme_h \t = %2.3f ppm ", dAmsr_regscheme_h);
  measuredAsymmetry_h6  = Form("dAmsr_reg_time_h \t = %2.3f ppm ", dAmsr_reg_time_h);
  measuredAsymmetry_h7  = Form("dAmsr_cuts_h \t\t = %2.3f ppm ", dAmsr_cuts_h);
//   measuredAsymmetry_h8  = Form("dAmsr_dblpeak_h \t = %2.3f ppm ", dAmsr_dblpeak_h);
//   measuredAsymmetry_h9  = Form("dAmsr_trans_h \t\t = %2.3f ppm ", dAmsr_trans_h);
  measuredAsymmetry_h10  = Form("dAmsr_nonlin_h \t\t = %2.3f ppm ", dAmsr_nonlin_h);
  measuredAsymmetry_h11 = Form("dAmsr_sys_h \t\t = %2.3f ppm (total reg.+nonlin.+transverse)", dAmsr_sys_h);
  measuredAsymmetry_h12  = Form("Amsr_h = %2.3f +- %2.3f (stat) +- %2.3f (sys) ppm ", A_msr_h, dAmsr_stat_h, dAmsr_sys_h);

  cout <<"\n"<<measuredAsymmetry_h1<<"\n"<<measuredAsymmetry_h2<<"\n"<<measuredAsymmetry_h3 
       <<"\n"<<measuredAsymmetry_h4<<"\n"<<measuredAsymmetry_h5<<"\n"<<measuredAsymmetry_h6 
       <<"\n"<<measuredAsymmetry_h7<<"\n"<<measuredAsymmetry_h8<<"\n"<<measuredAsymmetry_h9 
       <<"\n"<<measuredAsymmetry_h10<<"\n\n"<<measuredAsymmetry_h11<<"\n"<<measuredAsymmetry_h12<<endl;


  TString measuredAsymmetry_v, measuredAsymmetry_v1, measuredAsymmetry_v2, measuredAsymmetry_v3;
  TString measuredAsymmetry_v4, measuredAsymmetry_v5, measuredAsymmetry_v6, measuredAsymmetry_v7;
  TString measuredAsymmetry_v8, measuredAsymmetry_v9, measuredAsymmetry_v10, measuredAsymmetry_v11;
  TString measuredAsymmetry_v12, measuredAsymmetry_v13, measuredAsymmetry_v14, measuredAsymmetry_v15;

  measuredAsymmetry_v1  = Form("Measured Asymmetry and Errors for Vertical Transverse:");
  measuredAsymmetry_v2  = Form("Amsr_v(unregressed) \t = %2.3f ppm \tRegression Correction \t = %2.3f ppm ", A_msr_unreg_v,RegCorr_v);
  measuredAsymmetry_v3  = Form("Amsr_v \t\t\t = %2.3f ppm ", A_msr_v);
  measuredAsymmetry_v4  = Form("dAmsr_stat_v \t\t = %2.3f ppm ", dAmsr_stat_v);
  measuredAsymmetry_v5  = Form("dAmsr_regscheme_v \t = %2.3f ppm ", dAmsr_regscheme_v);
  measuredAsymmetry_v6  = Form("dAmsr_reg_time_v \t = %2.3f ppm ", dAmsr_reg_time_v);
  measuredAsymmetry_v7  = Form("dAmsr_cuts_v \t\t = %2.3f ppm ", dAmsr_cuts_v);
//   measuredAsymmetry_v8  = Form("dAmsr_dblpeak_v \t = %2.3f ppm ", dAmsr_dblpeak_v);
//   measuredAsymmetry_v9  = Form("dAmsr_trans_v \t\t = %2.3f ppm ", dAmsr_trans_v);
  measuredAsymmetry_v10 = Form("dAmsr_nonlin_v \t\t = %2.3f ppm ", dAmsr_nonlin_v);
  measuredAsymmetry_v11 = Form("dAmsr_sys_v \t\t = %2.3f ppm (total reg.+nonlin.+transverse)", dAmsr_sys_v);
  measuredAsymmetry_v12 = Form("Amsr_v = %2.3f +- %2.3f (stat) +- %2.3f (sys) ppm ", A_msr_v, dAmsr_stat_v, dAmsr_sys_v);

  cout <<"\n"<<measuredAsymmetry_v1<<"\n"<<measuredAsymmetry_v2<<"\n"<<measuredAsymmetry_v3 
       <<"\n"<<measuredAsymmetry_v4<<"\n"<<measuredAsymmetry_v5<<"\n"<<measuredAsymmetry_v6 
       <<"\n"<<measuredAsymmetry_v7<<"\n"<<measuredAsymmetry_v8<<"\n"<<measuredAsymmetry_v9 
       <<"\n"<<measuredAsymmetry_v10<<"\n\n"<<measuredAsymmetry_v11<<"\n"<<measuredAsymmetry_v12<<endl;


  TString measuredAsymmetry, measuredAsymmetry1, measuredAsymmetry2, measuredAsymmetry3;
  TString measuredAsymmetry4, measuredAsymmetry5, measuredAsymmetry6, measuredAsymmetry7;
  TString measuredAsymmetry8, measuredAsymmetry9, measuredAsymmetry10, measuredAsymmetry11;
  TString measuredAsymmetry12, measuredAsymmetry13, measuredAsymmetry14, measuredAsymmetry15, measuredAsymmetry23;

  measuredAsymmetry1  = Form("Measured Asymmetry and Errors (Combined Horizontal and Vertical):");
  measuredAsymmetry2  = Form("Amsr (unregressed) \t = %2.3f ppm, \tRegression Correction \t = %2.3f ppm ", A_msr_unreg,RegCorr);
  measuredAsymmetry3  = Form("Amsr \t\t\t = %2.3f ppm ", A_msr);
  measuredAsymmetry4  = Form("dAmsr_{stat} \t\t = %2.3f ppm (%2.1f\%) [stat]", dAmsr_stat,100*dAmsr_stat/A_msr);
  measuredAsymmetry5  = Form("dAmsr_{P} \t = %2.3f ppm ", dAmsr_P);
  measuredAsymmetry6  = Form("dAmsr_{regscheme} \t = %2.3f ppm (%2.1f\%)", dAmsr_regscheme, 100*dAmsr_regscheme/A_msr);
  measuredAsymmetry7  = Form("dAmsr_{reg_time} \t = %2.3f ppm (%2.1f\%)", dAmsr_reg_time,100*dAmsr_reg_time/A_msr);
  measuredAsymmetry8  = Form("dAmsr_{nonlin} \t\t = %2.3f ppm (%2.1f\%)", dAmsr_nonlin,100*dAmsr_nonlin/A_msr);
  measuredAsymmetry9  = Form("dAmsr_{cuts} \t\t = %2.3f ppm (%2.1f\%)", dAmsr_cuts,100*dAmsr_cuts/A_msr);
  measuredAsymmetry10 = Form("dAmsr_{q2_acceptance} \t = %2.3f ppm (%2.1f\%)", dAmsr_q2_acceptance,100*dAmsr_q2_acceptance/A_msr);
  measuredAsymmetry11 = Form("dAmsr_{fit} \t\t = %2.3f ppm (%2.1f\%)", dAmsr_fit,100*dAmsr_fit/A_msr);
  measuredAsymmetry15 = Form("dAmsr_{det_acceptance} \t = %2.3f ppm (%2.1f\%)", dADetAcpt,100*dADetAcpt/A_msr);
  measuredAsymmetry12 = Form("dAmsr_{sys} \t\t = %2.3f ppm (%2.1f\%) [quadrature sum of all sys.]", dAmsr_sys,100*dAmsr_sys/A_msr);
  measuredAsymmetry13 = Form("dAmsr \t\t= %2.3f ppm ", dAmsr);
  measuredAsymmetry23 = Form("Amsr_{Det_acceptance} \t = %2.3f ppm ", A_msr_DetAcpt);
//   measuredAsymmetry14 = Form("A_{M}^{in} = %2.3f #pm %2.3f (stat) #pm %2.3f (sys) ppm ", A_msr, dAmsr_stat, dAmsr_sys);
  measuredAsymmetry14 = Form("A_{M}^{in} \t\t = %2.3f #pm %2.3f (stat) #pm %2.3f (sys) ppm ", A_msr_DetAcpt, dAmsr_stat, dAmsr_sys);



  cout <<"\n"<<measuredAsymmetry1<<"\n"<<measuredAsymmetry2<<"\n"<<measuredAsymmetry3 
       <<"\n"<<measuredAsymmetry4<<"\n"<<measuredAsymmetry5<<"\n"<<measuredAsymmetry6 
       <<"\n"<<measuredAsymmetry7<<"\n"<<measuredAsymmetry8<<"\n"<<measuredAsymmetry9 
       <<"\n"<<measuredAsymmetry10<<"\n"<<measuredAsymmetry11<<"\n"<<measuredAsymmetry13
       <<"\n"<<measuredAsymmetry14<<endl;


  TString msrAsym, msrAsym1, msrAsym2, msrAsym3;
  TString msrAsym4, msrAsym5, msrAsym6, msrAsym7;
  TString msrAsym8, msrAsym9, msrAsym10, msrAsym11;
  TString msrAsym12, msrAsym13, msrAsym14, msrAsym15,msrAsym23,msrAsym24;

  msrAsym1  = Form("Measured Asymmetry and Errors (Combined Horizontal and Vertical):");
  msrAsym2  = Form("Amsr (unregressed)");
  msrAsym3  = Form("Amsr");
  msrAsym4  = Form("dAmsr_{stat}");
  msrAsym5  = Form("dAmsr_{P}");
  msrAsym6  = Form("dAmsr_{regscheme}");
  msrAsym7  = Form("dAmsr_{reg_time}");
  msrAsym8  = Form("dAmsr_{nonlin}");
  msrAsym9  = Form("dAmsr_{cuts}");
  msrAsym10 = Form("dAmsr_{q2_acceptance}");
  msrAsym11 = Form("dAmsr_{fit}");
  msrAsym12 = Form("dAmsr_{sys}");
  msrAsym13 = Form("dAmsr");
  msrAsym14 = Form("A_{M}^{in}");
  msrAsym24 = Form("Det. acpt. corrected asym. A_{DetAcpt}");
  msrAsym23 = Form("Amsr_{DetAcpt}");


//   cout <<"\n"<<msrAsym1<<"\n"<<msrAsym2<<"\n"<<msrAsym3 
//        <<"\n"<<msrAsym4<<"\n"<<msrAsym5<<"\n"<<msrAsym6 
//        <<"\n"<<msrAsym7<<"\n"<<msrAsym8<<"\n"<<msrAsym9 
//        <<"\n"<<msrAsym10<<"\n\n"<<msrAsym11<<"\n"<<msrAsym13
//        <<"\n"<<msrAsym14<<endl;



  /*********************************************/

  TString polarization1,polarization0,polar2;
  TString kin0,kin1,kin2,kin3;

  polarization0  = Form("Beam polarization P");
  polarization1  = Form("Beam polarization P \t = %2.3f +- %2.3f",P,dP);
//   polarization2  = Form("P \t\t\t = %2.3f +- %2.3f ",P,dP);
//   polar2         = Form("P");

  kin1  = Form("Four momentum transfer squared Q^{2} \t =  %2.3f +- %2.3f (GeV/c)^2",Q2,dQ2);
  kin2  = Form("Scattering angle Theta \t =  %2.3f +- %2.3f",Theta,dTheta);
  kin3  = Form("Energy E \t\t =  %2.3f +- %2.3f",E,dE);


  cout <<"\n"<<polarization0<<"\n"<<polarization1<<endl;
  cout <<"\n"<<kin1<<"\n"<<kin2<<"\n"<<kin3<<endl;

  /*********************************************/

  TString bkgTitleMain,bkgTitle1,bkgTitle2,bkgTitle3,bkgTitle4;
  TString bkgAsymmetry1,bkgAsymmetry2,bkgAsymmetry3,bkgAsymmetry4;
  TString bkgDilution1,bkgDilution2,bkgDilution3,bkgDilution4;

  bkgTitleMain   = Form("Background Asymmetries, Dilutions and Corrections:");

  bkgTitle1      = Form("Aluminum target windows:");
  bkgAsymmetry1  = Form("A_{b1} \t = %2.3f #pm %2.3f",A_b1,dA_b1);
  bkgDilution1   = Form("f_{b1} = %2.3f #pm %2.3f",f_b1,df_b1);

  bkgTitle2      = Form("Beamline scattering:");
  bkgAsymmetry2  = Form("A_{b2} \t = %2.3f #pm %2.3f",A_b2,dA_b2);
  bkgDilution2   = Form("f_{b2} = %2.3f #pm %2.3f",f_b2,df_b2);

  //   bkgTitle3      = Form("Beamline bkg neutrals with W shutters installed:");
  bkgTitle3      = Form("Other neutral bkg:");
  bkgAsymmetry3  = Form("A_{b3} \t = %2.3f #pm %2.3f",A_b3,dA_b3);
  bkgDilution3   = Form("f_{b3} = %2.3f #pm %2.3f",f_b3,df_b3);

  bkgTitle4      = Form("Elastic dilution:");
  bkgAsymmetry4  = Form("A_{b4} \t = %2.3f #pm %2.3f",A_b4,dA_b4);
  bkgDilution4   = Form("f_{b4} = %2.3f #pm %2.3f",f_b4,df_b4);

 TString bkgAsymmetry4_2  = Form("A_{b4} \t = %2.3f #pm %2.3f",A_b4_2,dA_b4_2);

  cout <<"\n"<<bkgTitleMain
       <<"\n"<<bkgTitle1<<"\n"<<bkgAsymmetry1<<"\t"<<bkgDilution1
       <<"\n"<<bkgTitle2<<"\n"<<bkgAsymmetry2<<"\t"<<bkgDilution2
       <<"\n"<<bkgTitle3<<"\n"<<bkgAsymmetry3<<"\t"<<bkgDilution3
       <<"\n"<<bkgTitle4<<"\n"<<bkgAsymmetry4<<"\t"<<bkgDilution4
       <<endl;


  /*********************************************/

  TString multiplicative_title,multiplicative_RC_title,multiplicative_RC,multiplicative_dRC;
  TString multiplicative_Det_title,multiplicative_Det,multiplicative_dDet;
  TString multiplicative_Bin_title,multiplicative_Bin,multiplicative_dBin;
  TString multiplicative_Phi_title,multiplicative_Phi,multiplicative_dPhi;
  TString multiplicative_Q2_title,multiplicative_Q2,multiplicative_dQ2;

  multiplicative_title      = Form("Multiplicative Corrections:");

  multiplicative_RC_title   = Form("Radiative correction");
  multiplicative_RC         = Form("%2.3f",RadCorr);
  multiplicative_dRC        = Form("%2.3f",dRadCorr);
  multiplicative_Det_title  = Form("Detector bias");
  multiplicative_Det        = Form("%2.3f",DetCorr);
  multiplicative_dDet       = Form("%2.3f ",dDetCorr);
  multiplicative_Bin_title  = Form("Eective kinematics correction");
  multiplicative_Bin        = Form("%2.3f",BinCenterCorr);
  multiplicative_dBin       = Form("%2.3f",dBinCenterCorr);
  multiplicative_Phi_title  = Form("Azimuthal acceptance correction");
  multiplicative_Phi        = Form("%2.3f",PhiCorr);
  multiplicative_dPhi       = Form("%2.3f",dPhiCorr);
  multiplicative_Q2_title   = Form("Q^{2} calibration");
  multiplicative_Q2         = Form("%2.3f",Q2Corr);
  multiplicative_dQ2        = Form("%2.3f",dQ2Corr);

  cout <<"\n"<<multiplicative_title
       <<"\n"<<multiplicative_RC_title<<"\n"<<multiplicative_RC<<" +- "<<multiplicative_dRC
       <<"\n"<<multiplicative_Det_title<<"\n"<<multiplicative_Det<<" +- "<<multiplicative_dDet
       <<"\n"<<multiplicative_Bin_title<<"\n"<<multiplicative_Bin<<" +- "<<multiplicative_dBin
       <<"\n"<<multiplicative_Phi_title<<"\n"<<multiplicative_Phi<<" +- "<<multiplicative_dPhi
       <<"\n"<<multiplicative_Q2_title<<"\n"<<multiplicative_Q2<<" +- "<<multiplicative_dQ2
       <<endl;

  /*********************************************/
  
  TString correctedAsymmetry1,correctedAsymmetry2,correctedAsymmetry3;

  correctedAsymmetry1  = Form("*********************Final Corrected Asymmetry***********************");
  correctedAsymmetry2  = Form("A_PHYS \t\t = %2.3f #pm %2.3f (stat) #pm %2.3f (sys) ppm ",A_phys,dAphys_stat,dAphys_sys);
  correctedAsymmetry3  = Form("(Blinding correction has subtracted  %2.3f ppm.)",BlindingCorrection);

  cout <<"\n"
       <<correctedAsymmetry1
       <<"\n"<<correctedAsymmetry2
//        <<"\n"<<correctedAsymmetry3
       <<"\n"<<line.Data()<<endl;

  /*********************************************/

  TString correction1,correction2,correction3,correction4,correction5,correction6;
  TString correction7,correction8,correction9,correction10,correction11;

  correction1  = Form("%s",line.Data());
  correction2  = Form("Breakdown of Corrections (neglects compounding):");
  correction3  = Form("Effective Polarization Correction \t = %2.3f +- %2.3f ppm ");
  correction4  = Form("Window Bkg Correction \t\t\t = %2.3f +- %2.3f ppm ");
  correction5  = Form("Effective Regression Correction \t = %2.3f +- %2.3f ppm ");
  correction6  = Form("Effective Transverse Correction \t = %2.3f +- %2.3f ppm ");
  correction7  = Form("QTOR Channel Neutral Bkg Correction \t = %2.3f +- %2.3f ppm ");
  correction8  = Form("Beamline Bkg Neutrals Correction \t = %2.3f +- %2.3f ppm ");
  correction9  = Form("Non-signal Electron Bkg Correction \t = %2.3f +- %2.3f ppm ");
  correction10 = Form("Effective Detector Bias Correction \t = %2.3f +- %2.3f ppm ");
  correction11 = Form("Effective Radiative Correction \t\t = %2.3f +- %2.3f ppm ");

//   cout <<"\n"<<correction1<<"\n"<<correction2<<"\n"<<correction3 
//        <<"\n"<<correction4<<"\n"<<correction5<<"\n"<<correction6 
//        <<"\n"<<correction7<<"\n"<<correction8<<"\n"<<correction9 
//        <<"\n"<<correction10<<"\n"<<correction11<<endl;

  /*********************************************/

  TString errorContribution1,errorContribution2,errorContribution3,errorContribution4;
  TString errorContribution5,errorContribution6,errorContribution7,errorContribution8;
  TString errorContribution9,errorContribution10,errorContribution11,errorContribution12;
  TString errorContribution13,errorContribution14,errorContribution15,errorContribution16;
  TString errorContribution20,errorContribution0;
  TString errorContribution21,errorContribution22,errorContribution23,errorContribution24;

  errorContribution0  = Form("Error contributions to final corrected asymmetry:");
  errorContribution1  = Form("dAmsr \t\t: %2.3f ppm (%2.1f\%)",dAphys,100*dAphys/A_phys);
  errorContribution2  = Form("dAmsr_stat \t: %2.3f ppm (%2.1f\%)",dAphys_stat,100*dAphys_stat/A_phys);
  errorContribution3  = Form("dAmsr_sys \t: %2.3f ppm (%2.1f\%)",dAphys_sys,100*dAphys_sys/A_phys);
  errorContribution4  = Form("dP \t\t: %2.3f ppm (%2.1f\%)",dAphys_P,100*dAphys_P/A_phys);
  errorContribution5  = Form("Aluminum alloy windows:");
  errorContribution6  = Form("dA_b1 \t\t: %2.3f ppm (%2.1f\%)",dAphys_Ab1,100*dAphys_Ab1/A_phys);
  errorContribution7  = Form("df_b1 \t\t: %2.3f ppm (%2.1f\%)",dAphys_fb1,100*dAphys_fb1/A_phys);
  errorContribution8  = Form("QTOR transport channel neutrals:");
  errorContribution9  = Form("dA_b2 \t\t: %2.3f ppm (%2.1f\%)",dAphys_Ab2,100*dAphys_Ab2/A_phys);
  errorContribution10 = Form("df_b2 \t\t: %2.3f ppm (%2.1f\%)",dAphys_fb2,100*dAphys_fb2/A_phys);
  errorContribution11 = Form("Beamline bkg neutrals with W shutters installed:");
  errorContribution12 = Form("dA_b3 \t\t: %2.3f ppm (%2.1f\%)",dAphys_Ab3,100*dAphys_Ab3/A_phys);
  errorContribution13 = Form("df_b3 \t\t: %2.3f ppm (%2.1f\%)",dAphys_fb3,100*dAphys_fb3/A_phys);
  errorContribution14 = Form("Non-signal electrons on detector:");
  errorContribution15 = Form("dA_b4 \t\t: %2.3f ppm (%2.1f\%)",dAphys_Ab4,100*dAphys_Ab4/A_phys);
  errorContribution16 = Form("df_b4 \t\t: %2.3f ppm (%2.1f\%)",dAphys_fb4,100*dAphys_fb4/A_phys);
  errorContribution20 = Form("Multiplative Corrections:");
  errorContribution21 = Form("dR_RC \t\t: %2.3f ppm (%2.1f\%)",dAphys_RC,100*dAphys_RC/A_phys);
  errorContribution22 = Form("dR_Det \t\t: %2.3f ppm (%2.1f\%)",dAphys_Det,100*dAphys_Det/A_phys);
  errorContribution23 = Form("dR_Q2 \t\t: %2.3f ppm (%2.1f\%)",dAphys_Q2,100*dAphys_Q2/A_phys);
  errorContribution24 = Form("dR_Bin \t\t: %2.3f ppm (%2.1f\%)",dAphys_Bin,100*dAphys_Bin/A_phys);




  cout <<"\n"<<errorContribution0<<"\n"<<errorContribution1<<"\n"<<errorContribution2<<"\n"<<errorContribution3 
       <<"\n"<<errorContribution4<<"\n"<<errorContribution5<<"\n"<<errorContribution6 
       <<"\n"<<errorContribution7<<"\n"<<errorContribution8<<"\n"<<errorContribution9 
       <<"\n"<<errorContribution10<<"\n"<<errorContribution11<<"\n"<<errorContribution12 
       <<"\n"<<errorContribution13<<"\n"<<errorContribution14<<"\n"<<errorContribution15 
       <<"\n"<<errorContribution16
       <<"\n"<<errorContribution20<<"\n"<<errorContribution21<<"\n"<<errorContribution22 
       <<"\n"<<errorContribution23<<"\n"<<errorContribution24
       <<endl;

  cout <<"\n"<<line<<endl;

  /*********************************************/
  TString misc_title;
  TString Q2_title,Q2_value,Q2_error;
  TString Theta_title,Theta_value,Theta_error;
  TString E_title,E_value,E_error;

  misc_title      = Form("Miscellaneous:");

  Q2_title      = Form("Four momentum transferd square");
  Q2_value      = Form("%2.4f",Q2);
  Q2_error      = Form("%2.4f ",dQ2);

  Theta_title   = Form("Scattering Angle");
  Theta_value   = Form("%2.3f",Theta);
  Theta_error   = Form("%2.3f ",dTheta);

  E_title       = Form("Beam Energy");
  E_value       = Form("%2.3f",E);
  E_error       = Form("%2.3f ",dE);

  cout<<"Q2 = "<<Q2<<"+-"<<dQ2<<endl;
  cout<<"Theta = "<<Theta<<"+-"<<dTheta<<endl;
  cout<<"E = "<<E<<"+-"<<dE<<endl;

  cout <<"\n"<<line<<endl;

  /*********************************************/
  //  Error contributions to final corrected asymmetry:
  //  dAmsr_stat :  0.1948 ppm
  //  dAmsr_sys :  0.0211 ppm
  //          dP :  0.0388 ppm
  //  Aluminum alloy windows:
  //         dA1 :  0.0000 ppm
  //         df1 :  0.0000 ppm
  //  QTOR transport channel neutrals:
  //         dA2 :  0.0163 ppm
  //         df2 :  0.0192 ppm
  //  Beamline bkg neutrals with W shutters installed:
  //         dA3 :  0.1855 ppm
  //         df3 :  0.0021 ppm
  //  Non-signal electrons on detector:
  //         dA4 :  0.0000 ppm
  //         df4 :  0.0000 ppm

  //   Canvas2->Update();

  /*****************************************************************************/
  /*                                                                           */
  /*                              Text Summary                                 */
  /*                                                                           */
  /*****************************************************************************/

  MyfileFinal.open(textfileFinal);

  MyfileFinal <<"\n"<<line<<endl;

  MyfileFinal <<"\n"<<measuredAsymmetry1<<"\n"<<measuredAsymmetry2<<"\n"<<measuredAsymmetry3 
	      <<"\n"<<measuredAsymmetry4
	      <<"\n"<<measuredAsymmetry12
	      <<"\n"<<measuredAsymmetry6 
	      <<"\n"<<measuredAsymmetry7<<"\n"<<measuredAsymmetry8<<"\n"<<measuredAsymmetry9 
	      <<"\n"<<measuredAsymmetry10<<"\n"<<measuredAsymmetry11
 	      <<"\n"<<measuredAsymmetry15<<"\n"<<measuredAsymmetry23<<"\n\n"<<measuredAsymmetry14
	      <<endl;

  MyfileFinal <<"\n"<<line<<"\n"<<polarization1<<"\n"<<kin1<<"\n"<<kin2<<"\n"<<kin3<<"\n"<<line<<endl;

  MyfileFinal <<"\n"<<bkgTitleMain
	      <<"\n"<<bkgTitle1<<"\n"<<bkgAsymmetry1<<"\t"<<bkgDilution1
	      <<"\n"<<bkgTitle2<<"\n"<<bkgAsymmetry2<<"\t"<<bkgDilution2
	      <<"\n"<<bkgTitle3<<"\n"<<bkgAsymmetry3<<"\t"<<bkgDilution3
	      <<"\n"<<bkgTitle4<<"\n"<<bkgAsymmetry4<<"\t"<<bkgDilution4
	      <<endl;

  MyfileFinal <<"\n"<<multiplicative_title
	      <<"\n"<<multiplicative_RC_title<<"\n"<<multiplicative_RC<<" +- "<<multiplicative_dRC
	      <<"\n"<<multiplicative_Det_title<<"\n"<<multiplicative_Det<<" +- "<<multiplicative_dDet
	      <<"\n"<<multiplicative_Bin_title<<"\n"<<multiplicative_Bin<<" +- "<<multiplicative_dBin
	      <<endl;

  MyfileFinal <<"\n"<<correctedAsymmetry1<<"\n"<<correctedAsymmetry2<<"\n"<<correctedAsymmetry3<<"\n"<<line<<endl;

//   MyfileFinal <<"\n"<<correction1<<"\n"<<correction2<<"\n"<<correction3 
// 	      <<"\n"<<correction4<<"\n"<<correction5<<"\n"<<correction6 
// 	      <<"\n"<<correction7<<"\n"<<correction8<<"\n"<<correction9 
// 	      <<"\n"<<correction10<<"\n"<<correction11<<endl;

  MyfileFinal <<"\n"<<errorContribution0<<"\n"<<errorContribution1<<"\n"<<errorContribution2<<"\n"<<errorContribution3 
	      <<"\n"<<errorContribution4<<"\n"<<errorContribution5<<"\n"<<errorContribution6 
	      <<"\n"<<errorContribution7<<"\n"<<errorContribution8<<"\n"<<errorContribution9 
	      <<"\n"<<errorContribution10<<"\n"<<errorContribution11<<"\n"<<errorContribution12 
	      <<"\n"<<errorContribution13<<"\n"<<errorContribution14<<"\n"<<errorContribution15 
	      <<"\n"<<errorContribution16
	      <<"\n"<<errorContribution20<<"\n"<<errorContribution21<<"\n"<<errorContribution22 
	      <<"\n"<<errorContribution23
	      <<endl;

  MyfileFinal <<"\n"<<line<<endl;

  MyfileFinal.close();

  /*********************************************/

  MyfileRegAsymmetry.open(textfileRegAsymmetry);
  MyfileRegAsymmetry <<A_msr<<"\t"<<dAmsr_stat<<"\t"<<dAmsr_sys<<"\t"<<dAmsr<<endl;
  MyfileRegAsymmetry.close();

  /*********************************************/

  MyfileFinalAsymmetry.open(textfileFinalAsymmetry);
  MyfileFinalAsymmetry <<A_phys<<"\t"<<dAphys_stat<<"\t"<<dAphys_sys<<"\t"<<dAphys<<endl;
  MyfileFinalAsymmetry.close();

  /*****************************************************************************/
  /*                                                                           */
  /*                                    Plot                                   */
  /*                                                                           */
  /*****************************************************************************/


  // Fit and stat parameters
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0000000);
  gStyle->SetStatY(0.99);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.15);
  gStyle->SetStatH(0.5);
  
  //Pad parameters
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetPadTopMargin(0.18);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadRightMargin(0.22);
  gStyle->SetPadLeftMargin(0.08);

  // histo parameters
  gStyle->SetTitleYOffset(1.6);
  gStyle->SetTitleXOffset(1.8);
  gStyle->SetLabelSize(0.05,"x");
  gStyle->SetLabelSize(0.05,"y");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetTitleX(0.1);
  gStyle->SetTitleW(0.6);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleFontSize(0.09);

  //Set fonts
  gStyle->SetTextFont(42);
  gStyle->SetStatFont(42);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleFont(42,"y");
  gStyle->SetTitleFont(42,"x");
  gStyle->SetLabelFont(42);
  gStyle->SetLabelFont(42,"y");
  gStyle->SetLabelFont(42,"x");
  
  //Delete all the objects stored in the current directory memmory
  gDirectory->Delete("*");

  if(SUMMARY_PLOT){

  TString title1;
  TString titleSummary = Form("(%s,QTor %sA) Extraction of Physics Asym Using Regressed 5+1 MD PMTavg for Transverse %s"
			      ,targ.Data(),qtor_stem.Data(),interaction2.Data());

  title1= Form("%s",titleSummary.Data());


  TCanvas * canvas1 = new TCanvas("canvas1", title1,0,0,canvasSize[0],canvasSize[1]);
  canvas1->Draw();
  canvas1->SetBorderSize(0);
  canvas1->cd();

  TPad*pad1 = new TPad("pad1","pad1",pad1x[0],pad1y[0],pad1x[1],pad1y[1]);
  TPad*pad2 = new TPad("pad2","pad2",pad2x[0],pad2y[0],pad2x[1],pad2y[1]);
  pad1->SetFillColor(kWhite);
  pad1->Draw();
  pad2->Draw();
  pad1->cd();
  TString text = Form("%s",title1.Data());
  TLatex *t1 = new TLatex(0.02,0.3,text);
  t1->SetTextSize(0.5);
  t1->Draw();
  pad2->cd();

  gStyle->SetTextSize(0.02);


//   Double_t cor[2] = {0.25,1.25};
//   Double_t corGaph[3] = {0.03,0.35,0.25};



 // Draw the lower case letters
  TLatex TlA;
  TlA.SetTextAlign(12);
  float y, x1, x2, x3, x4, x5, y1, y2, y3, y4, y5, y_2;
  y = 0.96; x1 = 0.02; x2 = x1+0.18; x3 = x1+0.26;  x4 = x1+0.40;

  TlA.DrawLatex(x1, y, Form("%s", msrAsym1.Data()));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym2.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm, \t Regression Correction \t = %2.3f ppm",A_msr_unreg,RegCorr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym3.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm",A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym4.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_stat)); TlA.DrawLatex(x3, y, Form("(%2.1f%) [stat]", 100*dAmsr_stat/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym24.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm",A_msr_DetAcpt));

  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s %s", polarization0.Data(),polar2.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f #pm %2.3f", P,dP)); 
  y -= 0.0100; 
//   y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym5.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_P)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_P/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym6.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_regscheme)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_regscheme/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym7.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_reg_time)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_reg_time/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym9.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_cuts)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_cuts/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym8.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_nonlin)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_nonlin/A_msr));
//   y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym10.Data()));TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_q2_acceptance)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_q2_acceptance/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym11.Data()));TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_fit)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dAmsr_fit/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym23.Data())); TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dADetAcpt)); TlA.DrawLatex(x3, y, Form("(%2.1f%)", 100*dADetAcpt/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym12.Data()));TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr_sys));TlA.DrawLatex(x3, y, Form("(%2.1f%) [sys]", 100*dAmsr_sys/A_msr));
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym13.Data()));TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr));TlA.DrawLatex(x3, y, Form("(%2.1f%) [quadrature sum of stat and sys]", 100*dAmsr/A_msr));
//   y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym13.Data()));TlA.DrawLatex(x2, y, Form("= %2.3f ppm", dAmsr));TlA.DrawLatex(x3, y, Form("(%2.1f%) (Total = dAmsr_{stat} #oplus dAmsr_{sys})", 100*dAmsr/A_msr));
  y -= 0.0100; 
  y -= 0.0250; TlA.DrawLatex(x1, y, Form("%s", msrAsym14.Data()));TlA.DrawLatex(x2, y, Form("= %2.3f #pm %2.3f #pm %2.3f ppm", A_msr_DetAcpt,dAmsr_stat,dAmsr_sys));


  /*********************************************/

  // Draw the lower case letters
  TLatex Tl;
  Tl.SetTextAlign(12);
//   float y, x1, x2, x3, x4;
  x1 = 0.02; x2 = x1+0.16; x3 = x1+0.27;  x4 = x1+0.30; x5 = x1+0.47;
  y -= 0.0150; 
  y -= 0.0650;
  Tl.DrawLatex(x1, y, Form("%s",bkgTitleMain.Data()));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",bkgTitle1.Data())); Tl.SetTextColor(kGreen+2);
  Tl.DrawLatex(x2, y, Form("%s",bkgAsymmetry1.Data())); 
  Tl.DrawLatex(x3, y, Form("%s",unitPPM.Data())); Tl.SetTextColor(kOrange+7);
  Tl.DrawLatex(x4, y, Form("%s    c_{b1} = %2.3f",bkgDilution1.Data(),c_b1)); Tl.SetTextColor(kBlack);
  Tl.DrawLatex(x5, y, Form("DocDB 1819"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",bkgTitle2.Data()));  Tl.SetTextColor(kGreen+2);
  Tl.DrawLatex(x2, y, Form("%s",bkgAsymmetry2.Data())); 
  Tl.DrawLatex(x3, y, Form("%s",unitPPM.Data())); Tl.SetTextColor(kOrange+7);
  Tl.DrawLatex(x4, y, Form("%s    c_{b2} = %2.3f",bkgDilution2.Data(),c_b2)); Tl.SetTextColor(kBlack);
  Tl.DrawLatex(x5, y, Form("Elog 782, 784"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",bkgTitle3.Data())); Tl.SetTextColor(kGreen+2);
  Tl.DrawLatex(x2, y, Form("%s",bkgAsymmetry3.Data())); 
  Tl.DrawLatex(x3, y, Form("%s",unitPPM.Data()));  Tl.SetTextColor(kOrange+7);
  Tl.DrawLatex(x4, y, Form("%s    c_{b3} = %2.3f",bkgDilution3.Data(),c_b3)); Tl.SetTextColor(kBlack);
  Tl.DrawLatex(x5, y, Form("Elog 714, DocDB 1549"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",bkgTitle4.Data())); Tl.SetTextColor(kGreen+2);
  Tl.DrawLatex(x2, y, Form("%s",bkgAsymmetry4.Data()));
  Tl.DrawLatex(x3, y, Form("%s",unitPPM.Data())); Tl.SetTextColor(kOrange+7);
  Tl.DrawLatex(x4, y, Form("%s    c_{b4} = %2.3f",bkgDilution4.Data(),c_b4)); Tl.SetTextColor(kBlack);
  Tl.DrawLatex(x5, y, Form("DocDB 1961, Elog 837"));
  y -= 0.0150; Tl.SetTextColor(kGreen+2);Tl.SetTextSize(0.015);
  Tl.DrawLatex(x2+0.032, y, Form("(Q corrected)"));Tl.SetTextColor(kBlack); Tl.SetTextSize(0.02);
  y_2 = 0.375; 
  y -= 0.025; 
  Tl.DrawLatex(x1, y, Form("%s",bkgTitle4.Data())); 
  Tl.DrawLatex(x2, y, Form("%s",bkgAsymmetry4_2.Data()));
  Tl.DrawLatex(x3, y, Form("%s",unitPPM.Data())); 
  Tl.DrawLatex(x4, y, Form("%s    c_{b4} = %2.3f",bkgDilution4.Data(),c_b4_2)); 
  Tl.DrawLatex(x5, y, Form("DocDB 1961, Elog 837"));
  y_2 -= 0.0150; Tl.SetTextSize(0.015);
  y -= 0.0150; Tl.SetTextSize(0.015);
  Tl.DrawLatex(x2+0.032, y, Form("(Q^{2} corrected)"));Tl.SetTextColor(kBlack); Tl.SetTextSize(0.02);

  /*********************************************/
  x1 = 0.02; x2 = x1+0.16; x3 = x1+0.27;  x4 = x1+0.30; x5 = x1+0.47;
  y -= 0.0750; 
  Tl.DrawLatex(x1, y, Form("%s",multiplicative_title.Data()));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",multiplicative_RC_title.Data())); Tl.SetTextColor(kRed);
  Tl.DrawLatex(x2, y, Form("R_{RC} = %s #pm %s",multiplicative_RC.Data(),multiplicative_dRC.Data())); Tl.SetTextColor(kBlack); 
//   Tl.DrawLatex(x5, y, Form("Place holder"));
  Tl.DrawLatex(x5, y, Form("DocDB 1886"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",multiplicative_Det_title.Data())); Tl.SetTextColor(kRed);
  Tl.DrawLatex(x2, y, Form("R_{Det} = %s #pm %s",multiplicative_Det.Data(),multiplicative_dDet.Data())); Tl.SetTextColor(kBlack); 
//   Tl.DrawLatex(x5, y, Form("Place holder"));
  Tl.DrawLatex(x5, y, Form("DocDB 1886"));
//   y -= 0.0250;
//   Tl.DrawLatex(x1, y, Form("%s",multiplicative_Bin_title.Data())); Tl.SetTextColor(kRed);
//   Tl.DrawLatex(x2, y, Form("R_{Bin} = %s #pm %s",multiplicative_Bin.Data(),multiplicative_dBin.Data())); Tl.SetTextColor(kBlack); 
//   Tl.DrawLatex(x5, y, Form("Place holder"));
//   y -= 0.0250;
//   Tl.DrawLatex(x1, y, Form("%s",multiplicative_Phi_title.Data())); Tl.SetTextColor(kGreen+2);
//   Tl.DrawLatex(x2, y, Form("R_{#phi}  = %s #pm %s",multiplicative_Phi.Data(),multiplicative_dPhi.Data())); Tl.SetTextColor(kBlack); 
//   Tl.DrawLatex(x5, y, Form("DocDB 1886"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",multiplicative_Q2_title.Data())); Tl.SetTextColor(kGreen+2);
  Tl.DrawLatex(x2, y, Form("R_{Q^{2}}  = %s #pm %s",multiplicative_Q2.Data(),multiplicative_dQ2.Data())); Tl.SetTextColor(kBlack); 
  Tl.DrawLatex(x5, y, Form("Elog 44 (Anc.)"));

  /*********************************************/

  x1 = 0.02; x2 = x1+0.165; x3 = x1+0.26;  x4 = x1+0.29; x5 = x1+0.47;
  y -= 0.0750; 
  Tl.DrawLatex(x1, y, Form("%s",misc_title.Data()));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",Q2_title.Data())); Tl.SetTextColor(kGreen+2); //Tl.SetTextColor(kOrange+7);
  Tl.DrawLatex(x2, y, Form("Q^{2} = %s #pm %s (GeV/c)^{2}",Q2_value.Data(),Q2_error.Data())); Tl.SetTextColor(kBlack); 
  Tl.DrawLatex(x5, y, Form("Elog 44 (Anc.)"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",Theta_title.Data())); Tl.SetTextColor(kGreen+2);//Tl.SetTextColor(kOrange+7);
  Tl.DrawLatex(x2, y, Form("#theta   = %s #pm %s degree",Theta_value.Data(),Theta_error.Data())); Tl.SetTextColor(kBlack); 
  Tl.DrawLatex(x5, y, Form("Elog 44 (Anc.)"));
  y -= 0.0250; 
  Tl.DrawLatex(x1, y, Form("%s",E_title.Data())); Tl.SetTextColor(kGreen+2);
  Tl.DrawLatex(x2, y, Form("E   = %s #pm %s GeV",E_value.Data(),E_error.Data())); Tl.SetTextColor(kBlack); 
  Tl.DrawLatex(x5, y, Form("PRL.111.141803"));

  /*********************************************/

  y = 0.90; x1 = 0.65; x2 = x1+0.05; x3 = x1+0.12;  x4 = x1+0.28; x5 = x1+0.45;

  y += 0.0250; Tl.SetTextColor(kRed); 
  Tl.DrawLatex(x1, y, Form("A_{N} =  R_{RC}R_{Det}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{b1} - f_{b2} - f_{b3} - f_{b4}} #right]")); 
//   Tl.DrawLatex(x1, y, Form("A_{N} =  R_{RC}R_{Det}R_{#phi}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{b1} - f_{b2} - f_{b3} - f_{b4}} #right]"));
  y -= 0.0650;
//   Tl.DrawLatex(x1+0.017, y, Form("= %2.2f #pm %2.2f ppm",A_phys,dAphys)); Tl.SetTextColor(kBlack); 
  Tl.DrawLatex(x1+0.017, y, Form("= %2.2f #pm %2.2f (stat) #pm %2.2f (sys) ppm",A_phys,dAphys_stat,dAphys_sys)); Tl.SetTextColor(kBlack); 

//    TLatex * tPhysAsym = new TLatex(x1,y,Form("A_{N} =  R_{RC}R_{Det}R_{Bin}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{b1} - f_{b2} - f_{b3} - f_{b4}} #right] = %2.3f #pm %2.3f",A_phys,dAphys));
// //   TLatex * tPhysAsym = new TLatex(x1,y,Form("A_{N} =  R_{Total} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %2.3f #pm %2.3f",A_phys,dAphys));

//   tPhysAsym->Draw();
//   tPhysAsym->SetTextColor(kRed);

  gStyle->SetTextSize(0.018);

  y -= 0.0500;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{A_{M}^{in}}")); 
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Amsr));
  Tl.DrawLatex(x3, y, Form("(%2.1f%) [stat]",100*dAphys_Amsr/A_phys)); 
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{P}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_P));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_P/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{A_{b1}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Ab1));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Ab1/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{A_{b2}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Ab2));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Ab2/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{A_{b3}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Ab3));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Ab3/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{A_{b4}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Ab4));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Ab4/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{f_{b1}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_fb1));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_fb1/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{f_{b2}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_fb2));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_fb2/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{f_{b3}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_fb3));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_fb3/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{f_{b4}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_fb4));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_fb4/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{R_{RC}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_RC));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_RC/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{R_{Det}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Det));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Det/A_phys));
//   y -= 0.0350;
//   Tl.DrawLatex(x1, y, Form("(dA_{N})_{R_{Bin}}"));
//   Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Bin));
//   Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Bin/A_phys));
//   y -= 0.0350;
//   Tl.DrawLatex(x1, y, Form("(dA_{N})_{R_{#phi}}"));
//   Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Phi));
//   Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Phi/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{R_{Q^{2}}}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_Q2));
  Tl.DrawLatex(x3, y, Form("(%2.1f%)",100*dAphys_Q2/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})_{sys}"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys_sys));
  Tl.DrawLatex(x3, y, Form("(%2.1f%) [sys]",100*dAphys_sys/A_phys));
  y -= 0.0350;
  Tl.DrawLatex(x1, y, Form("(dA_{N})"));
  Tl.DrawLatex(x2, y, Form(" = %2.2f ppm",dAphys));
  Tl.DrawLatex(x3, y, Form("(%2.1f%) [quadrature sum of all uncertainties]",100*dAphys/A_phys));
//   y -= 0.0450;
//   Tl.DrawLatex(x1, y, Form("(dA_{N})_{}"));
//   Tl.DrawLatex(x2, y, Form(" = %2.2f",dAphys_)); 

  y -= 0.1050;
  TLatex * tdPhysAsym_Amsr = new TLatex(x1,y,Form("(dA_{N})_{A_{M}^{in}} =  R_{Total} #frac{dA_{M}^{in}}{P} #left[ #frac{1}{1 - f_{Total}} #right] = %4.3f ",dAphys_Amsr));
  y -= 0.0450;
  TLatex * tdPhysAsym_P = new TLatex(x1,y,Form("(dA_{N})_{P} =  R_{Total} #frac{A_{M}^{in}}{P} #frac{dP}{P} #left[ #frac{1}{1 - f_{Total}} #right] = %4.3f ",dAphys_P));
  y -= 0.0450;
  TLatex * tdPhysAsym_Ab1 = new TLatex(x1,y,Form("(dA_{N})_{A_{b1}} =  R_{Total} #left[ #frac{ - dA_{b1}f_{b1} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab1));
  y -= 0.0450;
  TLatex * tdPhysAsym_Ab2 = new TLatex(x1,y,Form("(dA_{N})_{A_{b2}} =  R_{Total} #left[ #frac{ - dA_{b2}f_{b2} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab2));
  y -= 0.0450;
  TLatex * tdPhysAsym_Ab3 = new TLatex(x1,y,Form("(dA_{N})_{A_{b3}} =  R_{Total} #left[ #frac{ - dA_{b3}f_{b3} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab3));
  y -= 0.0450;
  TLatex * tdPhysAsym_Ab4 = new TLatex(x1,y,Form("(dA_{N})_{A_{b4}} =  R_{Total} #left[ #frac{ - dA_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab4));

  y -= 0.0750;
  TLatex * tdPhysAsym_fb1 = new TLatex(x1,y,Form("(dA_{N})_{f_{b1}} =  R_{Total} df_{b1} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}(1 - f_{b2} - f_{b3} - f_{b4}) - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb1));
  y -= 0.0750;
  TLatex * tdPhysAsym_fb2 = new TLatex(x1,y,Form("(dA_{N})_{f_{b2}} =  R_{Total} df_{b2} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}(1 - f_{b1} - f_{b3} - f_{b4}) - A_{b3}f_{b3} - A_{b4}f_{b4} }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb2));
  y -= 0.0750;
  TLatex * tdPhysAsym_fb3 = new TLatex(x1,y,Form("(dA_{N})_{f_{b3}} =  R_{Total} df_{b3} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}(1 - f_{b1} - f_{b2} - f_{b3}) - A_{b4}f_{b4} }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb3));
  y -= 0.0750;
  TLatex * tdPhysAsym_fb4 = new TLatex(x1,y,Form("(dA_{N})_{f_{b4}} =  R_{Total} df_{b4} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}(1 - f_{b1} - f_{b2} - f_{b3}) }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb4));

  y -= 0.0650;
  TLatex * tdPhysAsym_RC = new TLatex(x1,y,Form("(dA_{N})_{R_{RC}} =  dR_{RC}R_{Det}R_{#phi}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_RC));
  y -= 0.0650;
  TLatex * tdPhysAsym_Det = new TLatex(x1,y,Form("(dA_{N})_{R_{Det}} =  R_{RC}dR_{Det}R_{#phi}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Det));
  y -= 0.0650;
  TLatex * tdPhysAsym_Phi = new TLatex(x1,y,Form("(dA_{N})_{R_{#phi}} =  R_{RC}R_{Det}dR_{#phi}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Bin));
//   y -= 0.0650;
//   TLatex * tdPhysAsym_Bin = new TLatex(x1,y,Form("(dA_{N})_{R_{Bin}} =  R_{RC}R_{Det}dR_{Bin}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Bin));
  y -= 0.0650;
  TLatex * tdPhysAsym_Q2 = new TLatex(x1,y,Form("(dA_{N})_{R_{Q^{2}}} =  R_{RC}R_{Det}R_{#phi}dR_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Q2));

//   tdPhysAsym_Amsr->Draw();
//   tdPhysAsym_P->Draw();
//   tdPhysAsym_Ab1->Draw();
//   tdPhysAsym_Ab2->Draw();
//   tdPhysAsym_Ab3->Draw();
//   tdPhysAsym_Ab4->Draw();

//   tdPhysAsym_fb1->Draw();
//   tdPhysAsym_fb2->Draw();
//   tdPhysAsym_fb3->Draw();
//   tdPhysAsym_fb4->Draw();

//   tdPhysAsym_RC->Draw();
//   tdPhysAsym_Det->Draw();
//   tdPhysAsym_Bin->Draw();
//   tdPhysAsym_Q2->Draw();

  y += 0.8950;
//   TLatex * tPhysAsymRDefination = new TLatex(x2,y-0.1,Form("R_{Total} = R_{RC}R_{Det}R_{Bin}R_{Q^{2}}"));
  TLatex * tPhysAsymRDefination = new TLatex(x2,y-0.1,Form("R_{Total} = R_{RC}R_{Det}R_{Q^{2}}"));
  TLatex * tPhysAsymfDefination = new TLatex(x2,y-0.135,Form("f_{Total} = f_{b1} + f_{b2} + f_{b3} + f_{b4}"));
  TLatex * tPhysAsymCDefination = new TLatex(x2,y-0.170,Form("c_{bi} = #kappaPf_{bi}A_{bi} where   #kappa = (R_{Total}/P)/(1-f_{Total})"));
  tPhysAsymRDefination->Draw();
  tPhysAsymfDefination->Draw();
  tPhysAsymCDefination->Draw();


  y1 = 0.98; y2 = 0.57; y3 = 0.18; y4 = 0.83; x1 = 0.62; x2 = x1+0.35;


  TLine l1(x1,0.02,x1,y1); l1.SetLineColor(kBlue); l1.SetLineWidth(2);
  TLine l2(0.02,y2,x1,y2); l2.SetLineColor(kBlue); l2.SetLineWidth(2);
  TLine l3(0.02,y3,x1,y3); l3.SetLineColor(kBlue); l3.SetLineWidth(2);
  TLine l4(x1,y4,x2,y4); l4.SetLineColor(kBlue); l4.SetLineWidth(2);l4.SetLineStyle(2);


  l1.Draw(); l2.Draw(); l3.Draw(); l4.Draw();


  gPad->Update();

  TString saveSummaryPlot = Form("dirPlot/summaryPlot/%s_%s_%s_MD_Final_%s"
				 ,interaction.Data(),qtor_stem.Data(),target.Data(),database_stem.Data());

  canvas1->Update();
  canvas1->Print(saveSummaryPlot+".png");
  if(FIGURE){
    canvas1->Print(saveSummaryPlot+".svg");
    canvas1->Print(saveSummaryPlot+".C");
  }

  }

  /*****************************************************************************/
  /*****************************************************************************/

  if(SUMMARY_PLOT2){

  TString title1;
  TString titleSummary = Form("(%s, QTor %sA) Extraction of Physics Asym Using Regressed 5+1 MD PMTavg for Transverse %s"
			      ,targ.Data(),qtor_stem.Data(),interaction2.Data());

  title1= Form("%s",titleSummary.Data());


  TCanvas * canvas11 = new TCanvas("canvas11", title1,0,0,canvasSize[0],canvasSize[1]);
  canvas11->Draw();
  canvas11->SetBorderSize(0);
  canvas11->cd();

  TPad*pad11 = new TPad("pad11","pad11",pad1x[0],pad1y[0],pad1x[1],pad1y[1]);
  TPad*pad12 = new TPad("pad12","pad12",pad2x[0],pad2y[0],pad2x[1],pad2y[1]);
  pad11->SetFillColor(kWhite);
  pad11->Draw();
  pad12->Draw();
  pad11->cd();
  TString text11 = Form("%s",title1.Data());
  TLatex *t11 = new TLatex(0.02,0.3,text11);
  t11->SetTextSize(0.5);
  t11->Draw();
  pad12->cd();

  gStyle->SetTextSize(0.02);

  TLatex TlA;
  TlA.SetTextAlign(12);
  float y, x1, x2, x3, x4, x5;


  y = 0.90; x1 = 0.31; x2 = x1+0.23; x3 = x1+0.30;  x4 = x1+0.34; x5 = x1+0.45;
   TLatex * tPhys2Asym = new TLatex(x1,y,Form("A_{N} =  R_{RC}R_{Det}R_{Bin}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{b1} - f_{b2} - f_{b3} - f_{b4}} #right] = %2.3f #pm %2.3f",A_phys,dAphys));


  tPhys2Asym->Draw();
  tPhys2Asym->SetTextColor(kRed);

  y = 0.90; x1 = 0.02; x2 = x1+0.23; x3 = x1+0.30;  x4 = x1+0.34; x5 = x1+0.45;

  gStyle->SetTextSize(0.018);

  y -= 0.0650;
  TLatex * tdPhys2Asym_Amsr = new TLatex(x1,y,Form("(dA_{N})_{A_{M}^{in}} =  R_{Total} #frac{dA_{M}^{in}}{P} #left[ #frac{1}{1 - f_{Total}} #right] = %4.3f ",dAphys_Amsr));
  y -= 0.0450;
  TLatex * tdPhys2Asym_P = new TLatex(x1,y,Form("(dA_{N})_{P} =  R_{Total} #frac{A_{M}^{in}}{P} #frac{dP}{P} #left[ #frac{1}{1 - f_{Total}} #right] = %4.3f ",dAphys_P));
  y -= 0.0450;
  TLatex * tdPhys2Asym_Ab1 = new TLatex(x1,y,Form("(dA_{N})_{A_{b1}} =  R_{Total} #left[ #frac{ - dA_{b1}f_{b1} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab1));
  y -= 0.0450;
  TLatex * tdPhys2Asym_Ab2 = new TLatex(x1,y,Form("(dA_{N})_{A_{b2}} =  R_{Total} #left[ #frac{ - dA_{b2}f_{b2} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab2));
  y -= 0.0450;
  TLatex * tdPhys2Asym_Ab3 = new TLatex(x1,y,Form("(dA_{N})_{A_{b3}} =  R_{Total} #left[ #frac{ - dA_{b3}f_{b3} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab3));
  y -= 0.0450;
  TLatex * tdPhys2Asym_Ab4 = new TLatex(x1,y,Form("(dA_{N})_{A_{b4}} =  R_{Total} #left[ #frac{ - dA_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Ab4));

  y -= 0.0750;
  TLatex * tdPhys2Asym_fb1 = new TLatex(x1,y,Form("(dA_{N})_{f_{b1}} =  R_{Total} df_{b1} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}(1 - f_{b2} - f_{b3} - f_{b4}) - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb1));
  y -= 0.0750;
  TLatex * tdPhys2Asym_fb2 = new TLatex(x1,y,Form("(dA_{N})_{f_{b2}} =  R_{Total} df_{b2} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}(1 - f_{b1} - f_{b3} - f_{b4}) - A_{b3}f_{b3} - A_{b4}f_{b4} }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb2));
  y -= 0.0750;
  TLatex * tdPhys2Asym_fb3 = new TLatex(x1,y,Form("(dA_{N})_{f_{b3}} =  R_{Total} df_{b3} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}(1 - f_{b1} - f_{b2} - f_{b3}) - A_{b4}f_{b4} }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb3));
  y -= 0.0750;
  TLatex * tdPhys2Asym_fb4 = new TLatex(x1,y,Form("(dA_{N})_{f_{b4}} =  R_{Total} df_{b4} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}(1 - f_{b1} - f_{b2} - f_{b3}) }{ (1 - f_{Total})^{2} } #right] = %4.3f ",dAphys_fb4));


  y = 0.90; x1 = 0.41; x2 = x1+0.23; x3 = x1+0.30;  x4 = x1+0.34; x5 = x1+0.45;

  y -= 0.0650;
  TLatex * tdPhys2Asym_RC = new TLatex(x1,y,Form("(dA_{N})_{R_{RC}} =  dR_{RC}R_{Det}R_{Bin}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_RC));
  y -= 0.0650;
  TLatex * tdPhys2Asym_Det = new TLatex(x1,y,Form("(dA_{N})_{R_{Det}} =  R_{RC}dR_{Det}R_{Bin}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Det));
  y -= 0.0650;
  TLatex * tdPhys2Asym_Bin = new TLatex(x1,y,Form("(dA_{N})_{R_{Bin}} =  R_{RC}R_{Det}dR_{Bin}R_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Bin));
  y -= 0.0650;
  TLatex * tdPhys2Asym_Q2 = new TLatex(x1,y,Form("(dA_{N})_{R_{Q^{2}}} =  R_{RC}R_{Det}R_{Bin}dR_{Q^{2}} #left[ #frac{ #frac{A_{M}^{in}}{P} - A_{b1}f_{b1} - A_{b2}f_{b2} - A_{b3}f_{b3} - A_{b4}f_{b4} }{1 - f_{Total}} #right] = %4.3f ",dAphys_Q2));

  tdPhys2Asym_Amsr->Draw();
  tdPhys2Asym_P->Draw();
  tdPhys2Asym_Ab1->Draw();
  tdPhys2Asym_Ab2->Draw();
  tdPhys2Asym_Ab3->Draw();
  tdPhys2Asym_Ab4->Draw();

  tdPhys2Asym_fb1->Draw();
  tdPhys2Asym_fb2->Draw();
  tdPhys2Asym_fb3->Draw();
  tdPhys2Asym_fb4->Draw();

  tdPhys2Asym_RC->Draw();
  tdPhys2Asym_Det->Draw();
  tdPhys2Asym_Bin->Draw();
  tdPhys2Asym_Q2->Draw();

  y += 0.8950;
  TLatex * tPhys2AsymRDefination = new TLatex(x2,y-0.1,Form("R_{Total} = R_{RC}R_{Det}R_{Bin}R_{Q^{2}}"));
  TLatex * tPhys2AsymfDefination = new TLatex(x2,y-0.135,Form("f_{Total} = f_{b1} + f_{b2} + f_{b3} + f_{b4}"));
  tPhys2AsymRDefination->Draw();
  tPhys2AsymfDefination->Draw();


  gPad->Update();

  }

  /*****************************************************************************/
  /*****************************************************************************/
  /*****************************************************************************/

  if(ERROR_PLOT){

  const Int_t n = 8;

  Double_t x_n[n] = {1,2,3,4,5,6,7,8};
  Double_t x2_n[n] = {0.84,1.47,2.67,3.59,4.85,5.63,6.84,7.81};

  Double_t y_n[n] = {dAmsr_stat,dAmsr_regscheme,dAmsr_reg_time,dAmsr_nonlin,dAmsr_cuts,dAmsr_fit,dAmsr_sys,dAmsr};
  Double_t y_n_h[n] = {dAmsr_stat_h,dAmsr_regscheme_h,dAmsr_reg_time_h,dAmsr_nonlin_h,dAmsr_cuts_h,dAmsr_fit_h,dAmsr_sys_h,dAmsr_h};
  Double_t y_n_v[n] = {dAmsr_stat_v,dAmsr_regscheme_v,dAmsr_reg_time_v,dAmsr_nonlin_v,dAmsr_cuts_v,dAmsr_fit_v,dAmsr_sys_v,dAmsr_v};

  Double_t yPercent_n[n] = {100*TMath::Abs(dAmsr_stat)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr_regscheme)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr_reg_time)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr_nonlin)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr_cuts)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr_fit)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr_sys)/TMath::Abs(A_msr),100*TMath::Abs(dAmsr)/TMath::Abs(A_msr)};
  Double_t yPercent_n_h[n] = {100*TMath::Abs(dAmsr_stat_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_regscheme_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_reg_time_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_nonlin_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_cuts_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_fit_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_sys_h)/TMath::Abs(A_msr_h),100*TMath::Abs(dAmsr_h)/TMath::Abs(A_msr_h)};
  Double_t yPercent_n_v[n] = {100*TMath::Abs(dAmsr_stat_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_regscheme_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_reg_time_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_nonlin_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_cuts_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_fit_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_sys_v)/TMath::Abs(A_msr_v),100*TMath::Abs(dAmsr_v)/TMath::Abs(A_msr_v)};

  TString title_n[n] = {"stat","reg. scheme","reg. time","nonlinearity","cuts","fit scheme","sys","Total"};

//   const Int_t n = 9;

//   Double_t x_n[n] = {1,2,3,4,5,6,7,8,9};
//   Double_t x2_n[n] = {0.88,1.52,2.74,3.67,4.88,5.72,6.77,7.84,8.80};

//   Double_t y_n[n] = {dAmsr_stat,dAmsr_regscheme,dAmsr_reg_time,dAmsr_nonlin,dAmsr_cuts,dAmsr_fit,dADetAcpt,dAmsr_sys,dAmsr};
//   Double_t y_n_h[n] = {dAmsr_stat_h,dAmsr_regscheme_h,dAmsr_reg_time_h,dAmsr_nonlin_h,dAmsr_cuts_h,dAmsr_fit_h,dADetAcpt_h,dAmsr_sys_h,dAmsr_h};
//   Double_t y_n_v[n] = {dAmsr_stat_v,dAmsr_regscheme_v,dAmsr_reg_time_v,dAmsr_nonlin_v,dAmsr_cuts_v,dAmsr_fit_v,dADetAcpt_v,dAmsr_sys_v,dAmsr_v};

//   Double_t yPercent_n[n] = {100*dAmsr_stat/TMath::Abs(A_msr),100*dAmsr_regscheme/TMath::Abs(A_msr),100*dAmsr_reg_time/TMath::Abs(A_msr),100*dAmsr_nonlin/TMath::Abs(A_msr),100*dAmsr_cuts/TMath::Abs(A_msr),100*dAmsr_fit/TMath::Abs(A_msr),100*dADetAcpt/TMath::Abs(A_msr),100*dAmsr_sys/TMath::Abs(A_msr),100*dAmsr/TMath::Abs(A_msr)};
//   Double_t yPercent_n_h[n] = {100*dAmsr_stat_h/TMath::Abs(A_msr_h),100*dAmsr_regscheme_h/TMath::Abs(A_msr_h),100*dAmsr_reg_time_h/TMath::Abs(A_msr_h),100*dAmsr_nonlin_h/TMath::Abs(A_msr_h),100*dAmsr_cuts_h/TMath::Abs(A_msr_h),100*dAmsr_fit_h/TMath::Abs(A_msr_h),100*dADetAcpt_h/TMath::Abs(A_msr_h),100*dAmsr_sys_h/TMath::Abs(A_msr_h),100*dAmsr_h/TMath::Abs(A_msr_h)};
//   Double_t yPercent_n_v[n] = {100*dAmsr_stat_v/TMath::Abs(A_msr_v),100*dAmsr_regscheme_v/TMath::Abs(A_msr_v),100*dAmsr_reg_time_v/TMath::Abs(A_msr_v),100*dAmsr_nonlin_v/TMath::Abs(A_msr_v),100*dAmsr_cuts_v/TMath::Abs(A_msr_v),100*dAmsr_fit_v/TMath::Abs(A_msr_v),100*dADetAcpt_v/TMath::Abs(A_msr_v),100*dAmsr_sys_v/TMath::Abs(A_msr_v),100*dAmsr_v/TMath::Abs(A_msr_v)};

//   TString title_n[n] = {"stat","reg. scheme","reg. time","nonlinearity","cuts","fit scheme","det_acpt","sys","Total"};



  Double_t yScale[2] = {0.0,2.28};
  Double_t textSize[2] = {0.075,0.090};

  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0000000);
  gStyle->SetStatY(0.99);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.10);
  gStyle->SetStatH(0.3);

  //Pad parameters
  gStyle->SetPadColor(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetStatColor(0);
  //   gStyle->SetPadTopMargin(0.18);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadRightMargin(0.06);
  gStyle->SetPadLeftMargin(0.10);

  gStyle->SetNdivisions(507,"y");
  gStyle->SetNdivisions(000,"x");

  // histo parameters
  gStyle->SetTitleYOffset(0.45);
  gStyle->SetTitleXOffset(0.90);
  gStyle->SetLabelSize(textSize[0],"x");
  gStyle->SetLabelSize(textSize[0],"y");
  gStyle->SetTitleSize(textSize[0],"x");
  gStyle->SetTitleSize(textSize[0],"y");
  gStyle->SetTitleX(0.1);
  gStyle->SetTitleW(0.6);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleFontSize(0.09);

  gStyle->SetTextFont(42);
  gStyle->SetStatFont(42);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleFont(42,"y");
  gStyle->SetTitleFont(42,"x");
  gStyle->SetLabelFont(42);
  gStyle->SetLabelFont(42,"y");
  gStyle->SetLabelFont(42,"x");

  gStyle->SetBarWidth(0.5); // set the bar width of the bar plot
  //  gStyle->SetBarOffset(0.4);

  gDirectory->Delete("*");


  TString title2;
  TString titleError = Form("(%s, QTor %sA) Summary of Uncertainties in Measured Transverse %s Asymmetry",targ.Data(),qtor_stem.Data(),interaction2.Data());
  title2= Form("%s",titleError.Data());

  TCanvas * canvas2 = new TCanvas("canvas2", title2,0,0,canvasSize[2],canvasSize[3]);
  canvas2->Draw();
  canvas2->SetBorderSize(0);
  canvas2->cd();
  TPad*pad21 = new TPad("pad21","pad21",pad1x[0],pad1y[0],pad1x[1],pad1y[1]);
  TPad*pad22 = new TPad("pad22","pad22",pad2x[0],pad2y[0],pad2x[1],pad2y[1]);
  pad21->SetFillColor(kWhite);
  pad21->Draw();
  pad22->Draw();
  pad21->cd();
  TString text2 = Form("%s",title2.Data());
  TLatex *t2 = new TLatex(0.05,0.3,text2);
  t2->SetTextSize(0.5);
  t2->Draw();
  pad22->cd();
  pad22->Divide(1,3);

  //  pad22->cd(1)->SetGridy();
  pad22->cd(1);
  TGraph * gr_h = new TGraph(n,x_n,y_n_h);
  gr_h->SetFillColor(kRed-7);
  gr_h->SetFillStyle(3007);
  gr_h->SetTitle("");
  gr_h->GetXaxis()->CenterTitle();
  gr_h->GetYaxis()->SetTitle(Form("Uncertainty [ppm]"));
  gr_h->GetYaxis()->CenterTitle();
  gr_h->Draw("AB");
  gr_h->GetXaxis()->SetLabelColor(0);
  gr_h->GetYaxis()->SetRangeUser(yScale[0],yScale[1]);
  TLatex* tText_h = new TLatex(3.4,yScale[1]*0.85,Form("Horizontal Transverse"));
  tText_h->SetTextSize(textSize[1]);
  tText_h->SetTextColor(kRed-4);
  tText_h->Draw();
//   y -= 0.0450;
  for(Int_t i=0; i<n; i++){
  TLatex* tAxis = new TLatex(x2_n[i],-0.20,Form("%s",title_n[i].Data()));
  tAxis->SetTextSize(textSize[0]);
  tAxis->SetTextAngle(0);
  tAxis->Draw();
  TLatex* tValue_h = new TLatex(x_n[i]-0.20,y_n_h[i]+0.18,Form("%0.3f",y_n_h[i]));
  tValue_h->SetTextSize(textSize[0]);
  tValue_h->SetTextAngle(0);
  TLatex* tPercentValue_h = new TLatex(x_n[i]-0.25,y_n_h[i]+0.01,Form("(%0.1f%)",yPercent_n_h[i]));
  tPercentValue_h->SetTextSize(textSize[0]);
  tPercentValue_h->SetTextAngle(0);
  tValue_h->Draw();  tPercentValue_h->Draw();
  }



  TString errorPlot = Form("dirPlot/summaryPlot/%s_%s_%s_MD_Error_Chart_%s"
				 ,interaction.Data(),qtor_stem.Data(),target.Data(),database_stem.Data());
  
  canvas2->Update();
  canvas2->Print(errorPlot+".png");
  if(FIGURE){
    canvas2->Print(errorPlot+".svg");
    canvas2->Print(errorPlot+".C");
  }

  }

  /*****************************************************************************/
  /*****************************************************************************/
  /*****************************************************************************/
  if(PHYSERROR_PLOT){

  const Int_t nPhys = 15;

  Double_t xPhys_n[nPhys] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15.3};
  Double_t xPhys2_n[nPhys] = {0.60,1.85,2.8,3.8,4.8,5.8,6.8,7.8,8.8,9.8,10.7,11.7,12.7,13.7,14.8};

  Double_t yPhys_n[nPhys] = {dAphys_Amsr,dAphys_P,dAphys_Ab1,dAphys_Ab2,dAphys_Ab3,dAphys_Ab4,dAphys_fb1,dAphys_fb2,dAphys_fb3,dAphys_fb4,dAphys_RC,dAphys_Det,dAphys_Q2,dAphys_sys,dAphys};
  Double_t yPhysPercent_n[nPhys] = {100*(dAphys_Amsr/A_phys),100*(dAphys_P/A_phys),100*(dAphys_Ab1/A_phys),100*(dAphys_Ab2/A_phys),100*(dAphys_Ab3/A_phys),100*(dAphys_Ab4/A_phys),100*(dAphys_fb1/A_phys),100*(dAphys_fb2/A_phys),100*(dAphys_fb3/A_phys),100*(dAphys_fb4/A_phys),100*(dAphys_RC/A_phys),100*(dAphys_Det/A_phys),100*(dAphys_Q2/A_phys),100*(dAphys_sys/A_phys),100*(dAphys/A_phys)};
  TString titlePhys_n[nPhys] = {"stat","P","A_{b1}","A_{b2}","A_{b3}","A_{b4}","f_{b1}","f_{b2}","f_{b3}","f_{b4}","R_{RC}","R_{Det}","R_{Q^{2}}","sys","Total"};
  Double_t yPhysScale[2] = {0.0,17.5};

  Double_t yPhys_n_superimpose_1[nPhys] = {dAphys_Amsr,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,dAphys_sys,0.0};
  Double_t yPhys_n_superimpose[nPhys] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,dAphys};
//   Double_t yPhys_n_superimpose[nPhys] = {dAphys_Amsr,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,dAphys_sys,dAphys};

  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0000000);
  gStyle->SetStatY(0.99);
  gStyle->SetStatX(0.99);
  gStyle->SetStatW(0.10);
  gStyle->SetStatH(0.3);

  //Pad parameters
  gStyle->SetPadColor(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetStatColor(0);
  //   gStyle->SetPadTopMargin(0.18);
  gStyle->SetPadTopMargin(0.06);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadRightMargin(0.06);
  gStyle->SetPadLeftMargin(0.10);

  gStyle->SetNdivisions(507,"y");
  gStyle->SetNdivisions(000,"x");

  // histo parameters
  gStyle->SetTitleYOffset(0.75);
  gStyle->SetTitleXOffset(0.90);
  gStyle->SetLabelSize(0.05,"x");
  gStyle->SetLabelSize(0.05,"y");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetTitleX(0.1);
  gStyle->SetTitleW(0.6);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleFontSize(0.07);

  gStyle->SetTextFont(42);
  gStyle->SetStatFont(42);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleFont(42,"y");
  gStyle->SetTitleFont(42,"x");
  gStyle->SetLabelFont(42);
  gStyle->SetLabelFont(42,"y");
  gStyle->SetLabelFont(42,"x");

  gStyle->SetBarWidth(0.8); // set the bar width of the bar plot
  //  gStyle->SetBarOffset(0.4);

  gDirectory->Delete("*");


  TString title3;
  TString titlePhysError = Form("(QTor %sA)Summary of Uncertainties in Extracted Transverse %s Physics Asymmetry",qtor_stem.Data(),interaction2.Data());
  title3= Form("%s",titlePhysError.Data());

  TCanvas * canvas3 = new TCanvas("canvas3", title3,0,0,canvasSize[4],canvasSize[5]);
  canvas3->Draw();
  canvas3->SetBorderSize(0);
  canvas3->cd();
  TPad*pad31 = new TPad("pad31","pad31",pad1x[0],pad1y[0],pad1x[1],pad1y[1]);
  TPad*pad32 = new TPad("pad32","pad32",pad2x[0],pad2y[0],pad2x[1],pad2y[1]);
  pad31->SetFillColor(kWhite);
  pad31->Draw();
  pad32->Draw();
  pad31->cd();
  TString text3 = Form("%s",title3.Data());
  TLatex *t3 = new TLatex(0.10,0.3,text3);
  t3->SetTextSize(0.7);
  t3->Draw();
  pad32->cd();


  TGraph * grPhys = new TGraph(nPhys,xPhys_n,yPhys_n);
  grPhys->SetFillColor(kOrange+2);
//   grPhys->SetFillColor(kGray);
  grPhys->SetFillStyle(3144);
  grPhys->SetTitle("");
  grPhys->GetXaxis()->CenterTitle();
  grPhys->GetYaxis()->SetTitle(Form("Uncertainty [ppm]"));
  grPhys->GetYaxis()->CenterTitle();
  grPhys->Draw("AB");
  grPhys->GetXaxis()->SetLabelColor(0);
  grPhys->GetYaxis()->SetRangeUser(yPhysScale[0],yPhysScale[1]);
  TLatex* tPhysText = new TLatex(2.5,yPhysScale[1]*0.85,Form("Combined Horizontal & Vertical"));
  tPhysText->SetTextSize(0.050);
  tPhysText->SetTextColor(kOrange+2);
//   tPhysText->Draw();
  for(Int_t i=0; i<nPhys; i++){
  TLatex* tPhysAxis = new TLatex(xPhys2_n[i],-0.90,Form("%s",titlePhys_n[i].Data()));
  tPhysAxis->SetTextSize(0.050);
  tPhysAxis->SetTextAngle(0);
  tPhysAxis->Draw();
  TLatex* tPhysValue = new TLatex(xPhys_n[i]-0.38,yPhys_n[i]+0.80,Form("%0.2f",yPhys_n[i]));
//   TLatex* tPhysValue = new TLatex(xPhys_n[i]-0.38,yPhys_n[i]+0.10,Form("%0.2f (%0.1f%)",yPhys_n[i],yPhysPercent_n[i]));
  tPhysValue->SetTextSize(0.040);
  tPhysValue->SetTextAngle(0);
  TLatex* tPhysPercentValue = new TLatex(xPhys_n[i]-0.58,yPhys_n[i]+0.15,Form("(%0.1f%)",yPhysPercent_n[i]));
  tPhysPercentValue->SetTextSize(0.040);
  tPhysPercentValue->SetTextAngle(0);
  tPhysValue->Draw();  tPhysPercentValue->Draw();
  }

  TGraph * grPhys_superimpose_1 = new TGraph(nPhys,xPhys_n,yPhys_n_superimpose_1);
  grPhys_superimpose_1->SetFillColor(kOrange+2);
  grPhys_superimpose_1->SetFillStyle(3008);
  grPhys_superimpose_1->Draw("B");

  TGraph * grPhys_superimpose = new TGraph(nPhys,xPhys_n,yPhys_n_superimpose);
  grPhys_superimpose->SetFillColor(kOrange+2);
  grPhys_superimpose->Draw("B");

  TString errorPhysPlot = Form("dirPlot/summaryPlot/%s_%s_%s_MD_PhysError_Chart_%s"
				 ,interaction.Data(),qtor_stem.Data(),target.Data(),database_stem.Data());
  
  canvas3->Update();
  canvas3->Print(errorPhysPlot+".png");
  if(FIGURE){
    canvas3->Print(errorPhysPlot+".svg");
    canvas3->Print(errorPhysPlot+".C");
  }

  }

  /*****************************************************************************/
  /*****************************************************************************/
  /*****************************************************************************/

  printf("\n%sDone with everything. Do CTRL+C to exit%s\n",red,normal);

  theApp.Run();
  return(1);
}


