//--------------積分------------------------------------------------------------------
double integral(int first, int last, int id, TH2* hist){
    TH1D *hist_py = hist->ProjectionY("",id,id);
    hist_py->Draw();
    cout << "integral value is " << hist_py->Integral(first, last, "width") << endl;
    return hist_py->Integral(first, last, "width"); 
}

//-------------2つの積分値の比--------------------------------------------------------
void integral_all(int x1, int x2, int x3, int x4, int id_first, int id_last){
    ofstream ofs1;
    ofs1.open("integral.csv");

    for(int i = id_first; i <= id_last; i++){
	double integral_first = integral(x1, x2, i, ssd_ida);
	double integral_second = integral(x3, x4, i, ssd_ida);
	ofs1 << i << ", " << integral_first/integral_second << endl;
	cout << "integral ratio is " << integral_first/integral_second << endl;
    }

    ofs1.close();
}
