//---------------------------------------------------------------------------------
//divplotのdivide設定
#define ROW 4
#define  COLUMN 4

//---------------------------------------------------------------------------------
bool wait(){
	printf("[1]:continue, [0]:quit >");
	string s;
	cin >> s;
	if(s=='0') return true;
	return false;
}

//---------------------------------------------------------------------------------
//FirstHistNameからHistNumberSumの数だけヒストグラムを一つずつ表示する
//.lsの順番に表示される
void multiplot(string FirstHistName, int HistNumberSum, string option = ""){
	TFile *file = _file0;
	TCanvas *c1 = new TCanvas("c1","c1");
	TKey *key = 0;
	const char* str = "mulplot.pdf";
 
	TIter next(file->GetListOfKeys());

	while((key = (TKey*)next())) if(key->GetName() == FirstHistName) break;

	c1->Print(Form("%s(",str));

	for(int i = 0; i < HistNumberSum; i++){
  		key->ReadObj()->Draw("colz");
		gPad->GetCanvas()->Update();
		gPad->Draw("colz");
		if(option!="nstop")
			if(wait()) break;
		key = (TKey*)next();
  		c1->Print(str);
	}
	c1->Print(Form("%s)",str));
	c1->Close(); 
}

//---------------------------------------------------------------------------------
//multiplotのdivideバージョン
void divplot(string FirstHistName, int HistNumberSum, 
			 string option="", string option2="", string option3=""){
	//option "nstop"だと表示するヒストを自動で更新
	//option2 "pdf"だとヒストをpdfに保存
	//option3 "X"だとX射影　"Y"だとY射影
	TFile *file = _file0;
	TCanvas *c1 = new TCanvas("c1","c1",900,900);
	TKey *key = 0;
	const char* str = "divplot.pdf";
 
	TIter next(file->GetListOfKeys());

	while((key = (TKey*)next())) if(key->GetName() == FirstHistName) break;

	if(option2=="pdf") c1->Print(Form("%s(",str));

	c1->Divide(ROW,COLUMN);
	for(int i = 0; i < HistNumberSum; i++){
		if(i>=(COLUMN*ROW) && i%(COLUMN*ROW) == 0){
			if(option!="nstop") 
				if(wait()) break;
			if(option2=="pdf") c1->Print(str);
		}

		c1->cd(i%(COLUMN*ROW)+1);
		if(option3=="X"){
			TH2F* h = (TH2F*)file->Get(key->GetName());
			h->GetXaxis()->SetTitle("RF[ns]");
			h->ProjectionX()->Draw();
		}else if(option3=="Y"){
			TH2F* h = (TH2F*)file->Get(key->GetName());
			h->ProjectionY()->Draw();
		}else{
			key->ReadObj()->Draw("colz");
		}
		cout << key->GetName() << endl;
		key = (TKey*)next();

		if(i == HistNumberSum-1){
			string end;
			cout << "quit[q]" << endl;
			cin >> end;
		}
		
		gPad->GetCanvas()->Update();
	}
	if(option2=="pdf") c1->Print(Form("%s)",str));
	c1->Close();
}
//---------------------------------------------------------------------------------
void DSSDplot(string FirstHistName, int HistNumberSum, string option="", string option2=""){

	TFile *file = _file0;
	TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
	TKey *key = 0;
	const char* str = "dssdplot.pdf";
 
	TIter next(file->GetListOfKeys());

	while((key = (TKey*)next())) if(key->GetName() == FirstHistName) break;

	if(option2=="pdf") c1->Print(Form("%s(",str));

	c1->Divide(4,4);
	for(int i = 0; i < HistNumberSum; i++){
		if(i>=16 && i%16 == 0){
			if(option!="nstop") 
				if(wait()) break;
			if(option2=="pdf") c1->Print(str);
		}

		c1->cd(i%16+1);
  		key->ReadObj()->Draw("colz");
		cout << key->GetName() << endl;
		key = (TKey*)next();

		if(i == HistNumberSum-1){
			string end;
			cout << "quit[q]" << endl;
			cin >> end;
		}
		
		gPad->GetCanvas()->Update();
	}
	if(option2=="pdf") c1->Print(Form("%s)",str));
	c1->Close();
}
//---------------------------------------------------------------------------------

