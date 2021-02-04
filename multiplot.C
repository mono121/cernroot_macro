//---------------------------------------------------------------------------------
bool wait(){
	printf("[1]:continue, [0]:quit >");
	string s;
	cin >> s;
	if(s=='0') return true;
	return false;
}

//---------------------------------------------------------------------------------
void multiplot(string FirstHistName, int HistNumberSum, string option = ""){
	TFile *file = new TFile("../build/cyric.root");
	TCanvas *c1 = new TCanvas("c1","c1");
	TKey *key = 0;
	const char* str = "mulplot.pdf";
 
	TIter next(file->GetListOfKeys());

	int FirstHistNumber = 0;
	while((key = (TKey*)next())){
		FirstHistNumber++;
		if(key->GetName() == FirstHistName) break;
	}

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
void divplot(string FirstHistName, int HistNumberSum, string option=""){
	TFile *file = new TFile("../build/cyric.root");
	TCanvas *c1 = new TCanvas("c1","c1",1100,800);
	TKey *key = 0;
	const char* str = "divplot.pdf";
 
	TIter next(file->GetListOfKeys());

	int FirstHistNumber = 0;
	while((key = (TKey*)next())){
		FirstHistNumber++;
		if(key->GetName() == FirstHistName) break;
	}

	c1->Print(Form("%s(",str));

	c1->Divide(3,2);
	for(int i = 0; i < HistNumberSum; i++){
		if(i>=6 && i%6 == 0){
			gPad->GetCanvas()->Update();
			gPad->Draw("colz");
			if(option!="nstop")
				if(wait()) break;
			c1->Print(str);
		}
		c1->cd(i%6+1);
  		key->ReadObj()->Draw("colz");
		key = (TKey*)next();
	}
	c1->Print(Form("%s)",str));
	c1->Close();
}
//---------------------------------------------------------------------------------
