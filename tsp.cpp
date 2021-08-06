#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<sstream>
#include <limits>
#include <math.h>

using namespace std;


void print_vect2D(vector<vector<double>> V){
    int len1 = V.size();
    int len2 = V[0].size();
    for (int i=0; i<len1; i++){
        vector<double> &v = V[i];
        for (int j=0; j<len2; j++){
            cout<<v[j]<<',';
        }
        cout<<endl;
    }
}


class TSP{

    public:
    string file_address;
    int numbere_of_cities;
    double infty;
    vector<vector<double>> cities;
    vector<vector<double>> DP;
    vector<vector<double>> Distances;

    void index_to_binary(vector<int> one_indexes,int num_bits){
        string s={};
        int len = one_indexes.size();
        for (int i=0; i<num_bits; i++){
            s=s+'0';
        }

        for (int i=0; i<len; i++){
            int ind = one_indexes[i];
            s[ind] = '1';
        }
    }

    void index_generation_for_loop(vector<vector<int>>& Ans, vector<int>set_index, int high, int level){
        if (level<0){
            Ans.push_back(set_index);
            return;
        }
        int low = level;
        for (int i=low; i<=high; i++){
            vector<int> set_index_to_go = set_index;
            set_index_to_go.push_back(i);
            index_generation_for_loop(Ans, set_index_to_go, i-1, level-1);
        }
    }

    vector<vector<int>> set_generation_index(int num_bits, int num_bits_one){
        vector<vector<int>> Ans;
        vector<int> set_index;
        index_generation_for_loop(Ans, set_index,  num_bits-1,  num_bits_one-1);
        return(Ans);
    }

    void set_generation(int num_bits, int num_bits_one){
        vector<vector<int>> Ans;
        vector<int> set_index;
        index_generation_for_loop(Ans, set_index,  num_bits-1,  num_bits_one-1);

        int len = Ans.size();
        for (int i=0; i<len; i++){
            vector<int> ind  = Ans[i];
            index_to_binary(ind,num_bits); 
        }
    }

    void set_cities(){
        ifstream infile;
        string row;
        infile.open(file_address);
        infile>>numbere_of_cities;
        getline(infile,row);
        while(infile.good()){
            getline(infile,row);
            stringstream s(row);
            double x;
            double y;
            s>>x;
            s>>y;
            vector<double> loc = {x,y};
            cities.push_back(loc);
        }
    }

    void init_DP(){
        int number_of_sub_sets = pow(2,numbere_of_cities-1);
        vector<double> V(numbere_of_cities,infty);
        DP.resize(number_of_sub_sets, V);
        DP[0][0]=0;
    }

    int set_to_index(vector<int> ind_vector){
        int ans = 0;
        int len = ind_vector.size();
        for(int i=0; i<len; i++){
            int index = ind_vector[i];
            if (index>=0){
                ans = ans + pow(2,index);
            }
        }
        return ans;
    }

    double dist_cities(int i, int j){
        vector<double> city1 = cities[i];
        double x1 = city1[0];
        double y1 = city1[1];
        vector<double> city2 = cities[j];
        double x2 = city2[0];
        double y2 = city2[1];
        int ans = pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
        return ans;
        
    }

    void set_distances(){
        Distances.resize(numbere_of_cities,vector<double>(numbere_of_cities,0));
        for (int i=0; i<numbere_of_cities; i++){
            for (int j=0; j<numbere_of_cities; j++){
                Distances[i][j]=dist_cities(i,j);
            }
        }
    }

    void tsp_dp(){
        for (int m=1; m<numbere_of_cities; m++){
            vector<vector<int>> Sets_m;
            Sets_m = set_generation_index(numbere_of_cities-1,m);
            int len = Sets_m.size();
            cout<<m<<endl;

            for (int s=0; s<len; s++){
                vector<int> set= Sets_m[s];
                int index_set = set_to_index(set);

                for (int k=0; k<m; k++){
                    int j = set[k];
                    vector<int> setMj=set;
                    setMj[k]=-1;
                    int index_setMj = set_to_index(setMj);

                    double ans_set_j = DP[index_setMj][0] + dist_cities(0,j+1);
                    for (int l=0; l<m; l++){
                        int ending_index = setMj[l];
                        if (ending_index >=0){
                            double temp_ans_set_j = DP[index_setMj][ending_index+1] + Distances[ending_index+1][j+1];
                            ans_set_j = min (ans_set_j,temp_ans_set_j);
                        }

                    }
                    DP[index_set][j+1] = ans_set_j;

                }
            }
        }
        
    }

    void find_tour_len(){
        double Ans = infty;
        double last_set_ind = pow(2,numbere_of_cities-1)-1;
        for (int i=1; i<numbere_of_cities; i++){
            double temp_ans = DP[last_set_ind][i] + dist_cities(0,i);
            Ans = min (Ans, temp_ans);
        }
        cout<<Ans;
    }

    TSP(string f_adress){
        file_address = f_adress;
        infty = numeric_limits<double>::max();
        set_cities();
        set_distances();
        init_DP();
        tsp_dp();
        find_tour_len();

    }

};


int main(){
    string file_address_sets = "G:\\My Drive\\course\\Algorithm\\TSP\\tsp.txt";
    TSP tsp25(file_address_sets);
    return 0;
}