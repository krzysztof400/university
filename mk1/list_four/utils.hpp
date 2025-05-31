void assign(int &a, int &b){
	a = b;
    assignments++;
}

bool compare(int &a, int &b){
    return a > b;
    comparisons++;
}