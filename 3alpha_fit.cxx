//___________________________________________________________________________
pair<double, double> LeastSqureMethod(double SourceEnergy[],
                                      double SpectreCh[]){
    double a00=0,a01=0,a02=0,a11=0,a12=0; 
    for(int i=0; i<3; i++){
        if(SpectreCh[i]==0) continue;
        a00 += 1.0;
        a01 += SpectreCh[i]; 
        a02 += SourceEnergy[i]; 
        a11 += SpectreCh[i]*SpectreCh[i]; 
        a12 += SpectreCh[i]*SourceEnergy[i]; 
    }
    //{切片、傾き}
    return {(a02*a11-a01*a12) / (a00*a11-a01*a01),
             (a00*a12-a01*a02) / (a00*a11-a01*a01)};
}
//___________________________________________________________________________
TH1* MakeBanY(TH2* hist, Int_t id){
    TString str=hist->GetName();
    ostringstream oss;
    oss<<id;
    str+="_py"+oss.str();
    return hist->ProjectionY(str.Data(),id,id);
}
//___________________________________________________________________________
void ZoomXAxis(TH1 *h){
    pair<double, double> xaxis;

    while(1){
        cout << "x軸zoom" << endl;
        cout << "  xmin:" ;
        cin >> xaxis.first;
        cout << "  xmax:" ;
        cin >> xaxis.second;
        if(xaxis.first > xaxis.second){
            cout << "xmin < xmax にしてください" << endl;
            continue;
        }
        if( !cin.fail() ) break;//整数値
        // 整数値以外
        cout << "数字を入力してください" << endl;
        cin.clear();
        cin.ignore( 1024, '\n' );// 入力文字列破棄
    }

    gPad->GetCanvas()->Clear();
    h->GetXaxis()->SetRangeUser(xaxis.first, xaxis.second);
    h->Draw();
    gPad->GetCanvas()->Update();
}
//___________________________________________________________________________
bool FitPeak(TH1 *h, double SourceEnergy[], int ID, 
             ofstream& ofs1,
             ofstream& ofs2,
             ofstream& ofs3){

    bool NextSpectre = true, quit = false;
    double SpectreCh[3] = {0,0,0};
    double resolution[3] = {0,0,0};
    double pedestal = 0; 
    string s;

    while(NextSpectre){
        double para[3];
        double xleft, xright;
        string calib;

        cout << "Fitするx軸の範囲" << endl;
        cout << "  xleft:" ;
        cin >> xleft;
        cout << "  xright:" ;
        cin >> xright;

        TF1 *func = new TF1("myfunc","gaus");
        func->SetParameters(h->GetMaximum(),
                            h->GetMean(),
                            h->GetRMS() );
        h->Fit(func,"Q+","",xleft,xright);
        gPad->GetCanvas()->Update();
        func->GetParameters(para);
        cout << "mean is "<< para[1] << endl;
        cout << "sigma is "<< para[2] << endl << endl;
        cout << "[1]:Gd, [2]:Am, [3]:Cf" << endl;
        cout << "[p]:pedestal, [n]:calibに使用しない" << endl;
        cout << "command >";
        cin >> calib;
        cout << endl;

        if(calib == "1"){
            SpectreCh[0] = para[1];
            resolution[0] = 2.35*para[2]/para[1];
            cout << "mean:" << para[1] << ", resolusion:" << resolution[0]*100 << "%" << endl << endl;
        }else if(calib == "2"){
            SpectreCh[1] = para[1];
            resolution[1] = 2.35*para[2]/para[1];
            cout << "mean:" << para[1] << ", resolusion:" << resolution[1]*100 << "%" << endl << endl;
        }else if(calib == "3"){
            SpectreCh[2] = para[1];
            resolution[2] = 2.35*para[2]/para[1];
            cout << "mean:" << para[1] << ". resolusion:" << resolution[2]*100 << "%" << endl << endl;
        }else if(calib == "p"){
            pedestal = para[1];
            cout << "Qped mean:" << para[1] << endl << endl;
        }else if(calib == "n"){
            //何もしない
        }else{
            //何もしない
        }
        
        while(1){
            cout << "[f]:fitする, [c]:次のスペクトルへ, [q]:やめる" << endl;
            cout << "[x]:x軸のzoom範囲を変える" << endl;
            cout << "command >" ;
            cin >> s;
            if(s=="f"){
                NextSpectre = true;
                break;
            }else if(s=="c"){
                NextSpectre = false;
                break;
            }else if(s=="x"){
                cout << endl;
                ZoomXAxis(h);
                NextSpectre = true;
                break;
            }else if(s=="q"){
                NextSpectre = false;
                break;
            }else{
                //何もしない
            }
        }
        
        //最小二乗法fit
        if(s=="c" || s=="q"){
            pair<double, double> CalibVal = LeastSqureMethod(SourceEnergy, SpectreCh);
            cout << "切片:" << CalibVal.first << "  " << "傾き:" << CalibVal.second << endl << endl;
            ofs1 << ID+1 << "," << CalibVal.second << "," << CalibVal.first << endl;
            ofs2 << ID+1 << "," << pedestal << endl;
            pedestal = 0;

            for(int i=0; i<3; i++){
                if(i==0) ofs3 << ID+1 << ", ";
                if(i==2){
                    ofs3  << resolution[i]*100 << "%";
                    cout << "分解能3；" << resolution[i]*100 << "%" << endl;
                    continue;
                }
                ofs3  << resolution[i]*100 << "%" << ", ";
                cout << "分解能" << i+1 << "；" << resolution[i]*100 << "%" << endl;
            }
            for(int i=0; i<3; i++) resolution[i] = 0;
        }
        quit = false;   
    }
    if(s=="q") quit = true;
    return quit;
}
//___________________________________________________________________________
void FitAllPeak(TH2* hist, int HistID=1){
    ofstream ofs1;
    ofs1.open("qcal.csv");
    ofstream ofs2;
    ofs2.open("qped.csv");
    ofstream ofs3;
    ofs3.open("resolution.csv");

    double SourceEnergy[3] = {3.182,5.486,5.81};//  (MeV)

    Int_t nbin=hist->GetXaxis()->GetNbins();
    hist->Draw();
    gPad->GetCanvas()->Clear();

    for(int i=HistID-1; i<nbin; i++){
        cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%calibration%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        TH1 *h = MakeBanY(hist,i+1);
        h->Draw();
        gPad->GetCanvas()->Update();
        ZoomXAxis(h);
        if(FitPeak(h, SourceEnergy, i, ofs1, ofs2, ofs3)) break;
        gPad->GetCanvas()->Clear();
    }
    ofs1.close();
    ofs2.close();
    ofs3.close();
}
