#include<bits/stdc++.h>
using namespace std;
string myText ;
ifstream MyReadFile("input.txt");
ofstream MyWriteFile("output.txt");

class OS{
private:
    char MAT[100][4]; //External Memory
    char IR[4]; //2 bits operation and next 2 bits memory. Instruction Register
    char R[4]; //GENERAL PURPOSE REGISTER
    string buffer;
    int IC,SI,m; //INSTRUCTION COUNTER REGISTER, SERVICE INTERRUPT, 
    bool C; //TOGGLE
public:
    void INIT();
    void LOAD();
    void START_EXECUTION();
    void EXECUTE_USER_PROGRAMME();
    void MOS();
    void READ();
    void WRITE();
    void TERMINATE();
    void PrintM();
    void os();
};

void OS::os(){
    PrintM();
    cout<<endl;
    for(int h=0;h<4;h++){
        cout<<IR[h];
    }
    cout<<endl;
    for(int h=0;h<4;h++){
        cout<<R[h];
    }
    cout<<endl;
    cin.get();
}

void OS::PrintM(){
    for(int y =0;y<100;y++){
        cout<<y<<":- ";
        for(int h=0;h<4;h++){
            cout<< MAT[y][h]<<"";
        }
        cout<<endl;
    }
}

//INIT method
void OS::INIT(){
    for(int y =0;y<100;y++){
        for(int h=0;h<4;h++){
            MAT[y][h]=' ';
        }
    }
    for(int h=0;h<4;h++){
        IR[h] =' ';
        R[h] = ' ';
    }
    IC = 0;
    SI = 0;
    C = false;
    buffer ="";
    m = 0;
}

//LOAD method
void OS::LOAD(){
    while(getline (MyReadFile, myText)){
        buffer=myText;
        string check = "";
        for(int r = 0;r<4;r++){
            check+=buffer[r];
        }

        if (check=="$AMJ"){
            INIT();
            int p = 0,L=0,k=0;
            while(1){
                p=0;
                getline (MyReadFile, myText);
                buffer = myText ;
                check="";

                for(int r = 0;r<4;r++){
                    check+=buffer[r];
                }

                if(check == "$DTA" ){
                    PrintM();
                    break;
                }
            
                while(p<buffer.length()){
                    for(int t=0;t<4;t++){
                        MAT[L][t] = buffer[p];
                        p++;
                    }
                    if(buffer[p]==' '){
                        cout<<"found"<<endl ;
                        PrintM();
                        L+=1;
                        break;
                    }
                    L+=1;
                }
            }
        }
        if(check == "$DTA" ){
            getline (MyReadFile, myText);
            buffer = myText;
            START_EXECUTION();
        }
        if(check=="$END"){
            PrintM();
            continue;
        }
    }
}

//START_EXECUTION method
void OS::START_EXECUTION(){
    IC = 0;
    EXECUTE_USER_PROGRAMME();
}

//EXECUTE_USER_PROGRAMME method
void OS::EXECUTE_USER_PROGRAMME(){
    while(1){
        for(int i = 0;i<4;i++){
            IR[i] = MAT[IC][i];
        }

        IC+=1 ;
        cout<<IC<<endl;

        if(IR[0]== 'G' && IR[1]== 'D'){   
            SI = 1;
            MOS();
        }else if(IR[0]=='P' && IR[1]=='D' ){
            SI=2;
            MOS();
        }else if(IR[0]== 'L' && IR[1]== 'R'){
            string kat = "";
            kat+=IR[2];
            kat+=IR[3];
            int k = stoi(kat);

            for(int i = 0;i<4;i++){
                R[i] = MAT[k][i];
            }
        }else if(IR[0]== 'S' && IR[1]== 'R'){
            string kat = "";
            kat+=IR[2];
            kat+=IR[3];
            int k = stoi(kat);

            for(int i = 0;i<4;i++){
                MAT[k][i] = R[i];
            }
        }else if(IR[0]=='C' && IR[1]=='R'){
            string kat = "";
            kat+=IR[2];
            kat+=IR[3];
            int k = stoi(kat);

            for(int t=0;t<4;t++){
                if(R[t]== MAT[k][t]){
                    C = true;
                }else{
                    C = false;
                    break;
                }
            }
        }else if(IR[0]=='B' && IR[1]=='T'){
            if(C==true){
                string kat = "";
                kat+=IR[2];
                kat+=IR[3];
                int k = stoi(kat);
                IC = k;
            }
        }else if(IR[0]=='H'){
            cout<<"job ended"<<endl;
            SI = 3;
            MOS();
            break;
        }
    }
}

//MOS method
void OS::MOS(){
    switch(SI){
        case 1:
            READ();
            break;
        case 2:
            WRITE();
            break;
        case 3:
            TERMINATE();
            break;
    }
}


//READ method
void OS::READ(){
    IR[3] = '0';
    string kat = "";
    kat+=IR[2];
    kat+=IR[3];
    int k = stoi(kat);
    int f = 0;
    for(int t = k;t<k+10;t++){
        for(int y = 0;y<4;y++){
            MAT[t][y] = buffer[f];
            f++;
        }
    }
    getline (MyReadFile, myText);
    buffer = myText;
}

//WRITE method
void OS::WRITE(){
    IR[3] = '0';
    string kat = "";
    kat+=IR[2];
    kat+=IR[3];
    int k = stoi(kat);
    string past = "";
    for(int t = k;t<k+10;t++){
        for(int u = 0;u<4;u++){
            past+=MAT[t][u] ;
        }
    }
    MyWriteFile<<past<<endl;
}


//TERMINATE method
void OS::TERMINATE(){
    MyWriteFile<<endl<<endl;
}

int main(){
    OS oss;
    oss.LOAD();
    oss.PrintM();
    MyWriteFile.close();
    return  0 ;
}