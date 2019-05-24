/*
    Name         -->  VINAYAK G BHAT - 15CSE1028
    CLASS        -->  BTECH 2ND YEAR
    SUBJECT      -->  SYSTEMS PRROGRAMMING
    PROJECT      -->  QUESTION NUMBER - 4 - ASSEMBLER THAT SUPPORTS PROGRAM REALLOCATION AND LITERALS
    COLLEGE      -->  NATIONAL INSTITUTE OF TECHNOLOGY GOA

 */


#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<map>
#include<stdlib.h>
#include<climits>
#include<limits>

using namespace std;

    void create_table();
    void pass1();
    void pass2();
    int error_flag = 0;

    int main()
    {
        create_table();
        pass1();
        if(error_flag == 1)
            exit(0);
        cout<<"\nIntermdiate file successfully generated !!! \n";
        pass2();
        cout<<"\nObjecct file successfully generated !!\n";
        return 0;
    }

    int to_base_10(char);
    char to_Base_16(int);
    string to_Base16(int);
    int to_Base10(string);


    struct op_Table_Element
    {
        string opcode;
        int format;
        char exist;
        op_Table_Element()
        {
            exist='n';
        }
    };

    struct Register_info
    {
        int num;
        char exist;
        Register_info()
        {
            exist='n';
        }
    };

     map<string,op_Table_Element>   OPTAB;
     map<string,Register_info>  REGISTER;

    void create_table()
   {
    ifstream optb("OPTABLE.txt");
    string opkey;
    optb>>opkey;
    string x;

    while(opkey != "END")
    {   optb>>x;
        OPTAB[opkey].opcode = x;
        optb>>x;
        OPTAB[opkey].format = to_Base10(x);
        OPTAB[opkey].exist = 'y';
        optb>>opkey;
    }


    REGISTER["A"].num=0;
    REGISTER["A"].exist='y';

    REGISTER["X"].num=1;
    REGISTER["X"].exist='y';

    REGISTER["L"].num=2;
    REGISTER["L"].exist='y';

    REGISTER["B"].num=3;
    REGISTER["B"].exist='y';

    REGISTER["S"].num=4;
    REGISTER["S"].exist='y';

    REGISTER["T"].num=5;
    REGISTER["T"].exist='y';

    REGISTER["F"].num=6;
    REGISTER["F"].exist='y';

    REGISTER["PC"].num=8;
    REGISTER["PC"].exist='y';

    REGISTER["SW"].num=9;
    REGISTER["SW"].exist='y';


  }

    struct literal_Table_Element
    {
        string value;
        char exist;
    };

    struct info_exist_literal
    {
        char exist;
        int address;
        info_exist_literal(){
        exist = 'n';
        }
    };

    struct symtab_Element
    {
     int address;
     string value;
     string block;
     char exist;
     symtab_Element()
     {
         exist='n';
     }
    };

    int size = 0;
    int literals_Defined = 0;
    int literals_Encountered = 0;
    map<string,info_exist_literal> LIT;
    map<int,literal_Table_Element>   LITERAL;
    map<string,symtab_Element>   SYMTAB;


string fill_Str(int,string);

bool is_Blank_Space(char);
void get_arr_of_words(string,string[],int&);
void check_optb_symtb_pc(string[] , int) ;
void assembler_Directives(string[],int);
string int_string(int);
void modify_object_file();
int string_To_Int(string);
void assemble_pass_2(string []);
string generate_Object_Code(string []);
void input(string []);
int reg_num(char );
void check_optb_symtb_pc(string [] , int) ;
int count;
void assembler_Directives(string a[],int);
string line;
int counter = 0;
int program_Counter = 0;
int strt = 0;
int nol = 0;
int line_no = 1;

  void pass1()
  {
    ofstream imt("INTERMEDIATE.txt");
    int count;                              //count = number of codes in a particular input line
    string code[5];
    ifstream in_file;
    in_file.open("INPUT.txt");
    string x,line;

    getline(in_file,line);                  //Input a line from the input file
    get_arr_of_words(line,code,count);      //store different words in the taken line in an array(code)


    while(count==0)
        getline(in_file,line);

    if(code[0]=="START")
    {
        stringstream convert(code[1]);
        convert>>hex>>program_Counter;
        strt = program_Counter;
        imt<<fill_Str(4,code[1])<<"\t"<<line<<"\n";
    }

    else if(code[1]=="START")
    {
        stringstream convert(code[2]);
        convert>>hex>>program_Counter;
        imt<<fill_Str(4,code[2])<<"\t"<<line<<"\n";
        strt = program_Counter;
    }

    else check_optb_symtb_pc(code,count);


    while(true)
    {
        getline(in_file,line);
        get_arr_of_words(line,code,count);

        if(count==0)
            continue;
        if((code[2][0] == '=') || (code[1][0]=='='))
        {
            int j;
            if(count == 3)
               j = 2;
            else
               j = 1;

            if(LIT[code[j]].exist != 'y')
               LITERAL[literals_Encountered++].value = code[j];

            if(LIT[code[j]].exist == 'n')
               LIT[code[j]].exist = 'y';
        }

        if(code[0] == "LTORG" || code[1] == "LTORG")
        {
            imt<<"\t \t"<<"LTORG"<<"\n";
            for(int i = literals_Defined;i < literals_Encountered;i++)
            {
                string x = LITERAL[i].value;
                int length= x.length()-4;
                if(x[1] == 'X')
                    length = (x.length()-4)/2;
                LIT[x].address = program_Counter;
                imt<<fill_Str(4,to_Base16(program_Counter))<<"\t\t"<<"*"<<"\t"<<x<<"\n";
                program_Counter=program_Counter+length;
            }

            literals_Defined = literals_Encountered;
            continue;
        }

        if(code[0]=="END")
        {
            imt<<"\t"<<line<<"\n";
            for(int i = literals_Defined;i < literals_Encountered;i++)
            {
                string x = LITERAL[i].value;
                int length=x.length()-4;

                if(x[1] == 'X')
                    length = (x.length()-4)/2;

                LIT[x].address = program_Counter;
                imt<<fill_Str(4,to_Base16(program_Counter))<<"\t\t"<<"*"<<"\t"<<x<<"\n";
                program_Counter=program_Counter+length;
            }
            size = program_Counter;
            literals_Defined = literals_Encountered;
            break;
        }

        if(code[0]!="END")
            imt<<fill_Str(4,to_Base16(program_Counter))<<"\t"<<line<<"\n";

        check_optb_symtb_pc(code,count);
     }
     program_Counter = strt;
   }



    void check_optb_symtb_pc(string code[],int count)
    {
        if(code[0]=="END")
           return ;

        if(count==3 && OPTAB[code[0]].exist == 'n')
        {
            if(SYMTAB[code[0]].exist == 'y')
            {
                cout<<"\nERROR  "<<code[0]<<" is being used multiple times . LOOK AT LINE NO. : "<<nol;
                error_flag = 1;
                return;
            }

            else
            {
                SYMTAB[code[0]].address = program_Counter;
                SYMTAB[code[0]].exist = 'y';

                if(OPTAB[code[1]].exist == 'y')
                    program_Counter += OPTAB[code[1]].format;

                else if(code[1][0] == '+')
                    program_Counter += 4;

                else assembler_Directives(code,count);
            }
        }

        else if(count == 1 || count == 2)
        {
            if(OPTAB[code[0]].exist == 'y')
                program_Counter+=OPTAB[code[0]].format;

            else if(code[0][0] == '+')
                program_Counter += 4;

            return;
        }
    }



    void assembler_Directives(string a[],int count)
    {

        int i=0;
        if(count == 3)
            i = 1;
        else
            i = 0;
        if(a[i]=="WORD")
              program_Counter+=3;
        else if(a[i]=="RESW")
        {
            stringstream num(a[i+1]);int n = 0;
            num>>n;
            program_Counter=program_Counter + n * 3;
        }
        else if(a[i]=="RESB")
        {
            stringstream num(a[i+1]);int n = 0;
            num>>n;
            program_Counter=program_Counter + n;
        }
        else if(a[i]=="BYTE")
            {
                 int length=a[i+1].length()-3;
                 if(a[i+1][0]=='X') length/=2;
                 program_Counter=program_Counter+length;
            }
        else
            {
                cout<<"\nOpcode not found : " << a[i]<<"  .LOOK AT LINE NO. : "<<nol;
                error_flag = 1;
            }

    }


ofstream obj,lst,mod;
ifstream intm;
int curr_block_num;
string pco;
char flag = 'n';

string int_string(int a)
{
    stringstream s;
    s<<hex<<a;
    string x = s.str();
    return x;
}


void modify_object_file()
{
   ifstream fin;
   fin.open("modification.txt");
   string s;
   while(true)
   {
       getline(fin,s);
       if(s=="") break;
       obj<<s<<endl;
   }
}

    bool imm,ind;
    string l;
    void input(string a[]);


    void assemble_pass_2(string[]);
    string generate_Object_Code(string[]);
    string en="";
    string t_Record_Start="",t_Record_Entries="";
    int text_length=0,base=INT_MAX;
    void assemble_pass_2(string a[]);
    string generate_Object_Code(string a[]);
    ifstream n;
    string ll;
    string nn[6];
    ifstream imt;
    int nnn;
    ofstream objc;
    int ln = 0;


    void pass2()
    {

    objc.open("INTERMEDIATE_AND_OBJECT_CODE.txt");
    imt.open("INTERMEDIATE.txt");
    n.open("INTERMEDIATE.txt");
    string a[6];
    char ch;
    string start;
    obj.open("OBJECT_PROGRAM.txt");
    intm.open("INPUT.txt");
    mod.open("MODIFICATION.txt");

    input(a);
    counter++;

    for(int i = 0 ; i < 2 ;i++)
        getline(n,ll);

    get_arr_of_words(ll,nn,nnn);
    if(a[1]=="START" || a[2] == "START")
    {
        int j;
        j = 1;
        obj<<"H^";
        int i = 0;
        for(i=0;i<a[j].length();++i)
            obj<<a[j][i];
        for(;i<6;++i)
            obj<<" ";
        string x = to_Base16(size - strt);
        obj<<"^"<<fill_Str(6,a[j+2])<<"^"<<fill_Str(6,to_Base16(size - to_Base10(a[j+2])))<<endl;
        objc <<l<<endl;

    }

    while(true)
    {
        input(a);
        while(a[1]=="BASE" || a[2] == "BASE" )
        {
        input(a);
        line_no++;
        }

        while(a[0] == "LTORG")
        {
            input(a);
            line_no++;
        }

        getline(n,ll);
        get_arr_of_words(ll,nn,nnn);

        while(nn[1] == "BASE" || nn[2]== "BASE" || nn[0] == "LTORG")
        {
         getline(n,ll);
         get_arr_of_words(ll,nn,nnn);
        }

        if(a[0]=="END" )
        {
            objc<<l<<"\n";
            en = "END";
            continue;
        }

        if(en == "END" && ln == literals_Encountered)
        {
            if(text_length>0 )
            {
                obj<<t_Record_Start<<"^"<<fill_Str(2,to_Base16(text_length/2))<<t_Record_Entries<<endl;
                cout<<"!!"<<endl;
            }

            modify_object_file();
            text_length=0;
            t_Record_Start="";
            t_Record_Entries="";
            obj<<"E^"<<fill_Str(6,start)<<endl;
            break;
        }
      try
       {
        assemble_pass_2(a);
       }
        catch(...)
       {
            ;
       }
        }
}


    int string_To_Int(string a)
    {
        int b = 0;
        stringstream c(a);
        c>>b;
        return b;
    }

  void assemble_pass_2(string a[])
  {

    string object_code;
    string loc_ctr;
    int format;

    if(a[2]=="RESB"||a[2]=="RESW")
    {
        if(text_length>0)
        obj<<t_Record_Start<<"^"<<fill_Str(2,to_Base16(text_length/2))<<t_Record_Entries<<endl;
        t_Record_Start="";
        t_Record_Entries="";
        text_length=0;
        return;
    }

    imm=ind=false;
    object_code=generate_Object_Code(a);
    line_no++;
    cout<<"\nline : "<<line_no<<" done"<<"\t object code  "<<object_code;

    objc<<l<<"\t\t"<<object_code<<"\n";

    if(t_Record_Start=="")
    {
        loc_ctr=a[0];
        t_Record_Start="T^"+fill_Str(6,loc_ctr);
        t_Record_Entries="^"+object_code;
        text_length=object_code.length();
    }

    else if(text_length+object_code.length()>60)
    {
        stringstream x;
        x<<hex<<(text_length/2);
        string leng = x.str();
        string d = leng;
        obj<<t_Record_Start<<"^"<<d<<t_Record_Entries<<endl;
        loc_ctr=a[0];
        t_Record_Start="T^"+fill_Str(6,loc_ctr);
        t_Record_Entries="^"+object_code;
        text_length=object_code.length();
    }

    else
    {
        t_Record_Entries+="^"+object_code;
        text_length+=object_code.length();
    }

    if(a[1]=="LDB")
    {
        int j;
        if(count == 4)
            j = 3;
        else
            j = 2;
        base=to_Base10(int_string(SYMTAB[a[j]].address));
    }

}

string generate_Object_Code(string a[])
{
    string ob1,ob2,ob3;
    string operand_addr,prgm_ctr;
    int format;
    if(a[1][0] == '*')
    {
        int i;
        ob1="";
        if(a[2][1]=='X')
            for(i=3;i<a[2].length()-1;++i) ob1+=a[2][i];
        else
            for(i=3;i<a[2].length()-1;++i)
                ob1+=to_Base16((int)a[2][i]);
        ln++;
        return ob1;
    }
    if(a[2]=="BYTE")
    {
        int i;
        ob1="";
        if(a[3][0]=='X')
            for(i=2;i<a[3].length()-1;++i) ob1+=a[3][i];
        else
            for(i=2;i<a[3].length()-1;++i)
                ob1+=to_Base16((int)a[3][i]);
        return ob1;
    }

    if(a[2]=="WORD")
    {
        ob1=to_Base16(string_To_Int(a[3].c_str()));
        return ob1;
    }

    if(a[1]=="RSUB")
    {
        ob1="4F0000";
        return ob1;
    }

    if(a[1]=="+RSUB")
    {
        ob1="4F000000";
        return ob1;
    }

    else if(a[2][0]=='+')
    {
        format=4;
        a[2]=a[2].substr(1);
    }

    else if(a[1][0]=='+')
    {
        format=4;
        a[1]=a[1].substr(1);
    }

    else
    {
        if(count==3 || count == 2)
        format=OPTAB[a[1]].format;
        else if(count == 4)
        format=OPTAB[a[2]].format;
    }

    if(format==1)
    {
        ob1=OPTAB[a[1]].opcode;
        return ob1;
    }

    if(format==2)
    {
        int j;
        if(OPTAB[a[1]].exist=='y')
            j = 1;
        else
            j = 2;
        ob1=OPTAB[a[j]].opcode;
        if(a[j+1].length()==3)
        {
            ob2=to_Base16(reg_num(a[j+1][0]));
            if(isdigit(a[j+1][2]))  ob2=ob2+to_Base_16(a[j+1][2]-1);
            else
            {
                ob2=ob2+to_Base_16(reg_num(a[j+1][2]));
            }
        }
        else
        {
            if(isdigit(a[j+1][0]))
                ob2=to_Base16(string_To_Int(a[j+1].c_str()))+"0";
            else
                ob2=to_Base16(reg_num(a[j+1][0]))+"0";
        }
        return (ob1+fill_Str(2,ob2));
    }

    if(format==3)
    {   int j;
        if(count==4)
            j = 2;
        else
            j = 1;

        ob1=OPTAB[a[j]].opcode;

        if(a[j+1][0]=='#')
        {
            imm=true;
            ob1=to_Base16(to_Base10(ob1)+1);
            a[j+1]=a[j+1].substr(1);
            if(isdigit(a[j+1][0]))
            {
                ob2="0";
                ob3=to_Base16(string_To_Int(a[j+1].c_str()));
                return fill_Str(2,ob1)+ob2+fill_Str(3,ob3);
            }
        }

        else if(a[j+1][0]=='@')
        {
            ind=true;
            ob1=to_Base16(to_Base10(ob1)+2);
            a[j+1]=a[j+1].substr(1);
        }

        else
            ob1=to_Base16(to_Base10(ob1)+3);

        ob2="0";
        bool x=false;

        if(a[j+1][a[j+1].length()-2]==',')
        {
            x=true;
            ob2=to_Base16(to_Base10(ob2)+8);
            a[j+1]=a[j+1].substr(0,a[j+1].length()-2);
        }

        prgm_ctr=nn[0];
        if(a[j+1][0] == '=')
        operand_addr=to_Base16(LIT[a[j+1]].address);
        else
        operand_addr=to_Base16(SYMTAB[a[j+1]].address);
        if(x) a[j+1]+=",X";
        int disp=to_Base10(operand_addr)-to_Base10(prgm_ctr);
        if(disp>=-2048 && disp<2048)
        {
            ob2=to_Base16(to_Base10(ob2)+2);
            if(disp<0) disp+=4096;
            ob3=to_Base16(disp);
            return fill_Str(2,ob1)+fill_Str(1,ob2)+fill_Str(3,ob3);
        }
        disp=to_Base10(operand_addr)-base;
        if(disp>=-2048 && disp<2048)
        {
            ob2=to_Base16(to_Base10(ob2)+4);
            if(disp<0) disp+=4096;
            ob3=to_Base16(disp);
            return fill_Str(2,ob1)+fill_Str(1,ob2)+fill_Str(3,ob3);
        }
    }


    if(format==4)
    {
        int j;
        if(count == 4)
          j = 2;
        else
            j = 1;


        ob1=OPTAB[a[j]].opcode;
        if(a[j+1][0]=='#')
        {
            imm=true;
            ob1=to_Base16(to_Base10(ob1)+1);
            a[j+1]=a[j+1].substr(1);
            if(isdigit(a[j+1][0]))
            {
                ob2="0";
                ob3=to_Base16(string_To_Int(a[j+1].c_str()));
                a[2]="+"+a[2];
                return ob1+ob2+fill_Str(5,ob3);
            }
        }
        else if(a[j+1][0]=='@')
        {
            ind=true;
            ob1=to_Base16(to_Base10(ob1)+2);
            a[j+1]=a[j+1].substr(1);
        }
        else
            ob1=to_Base16(to_Base10(ob1)+3);
        bool x=false;
        ob2="1";
        if(a[j+1][a[j+1].length()-2]==',')
        {
            x=true;
            ob2=to_Base16(to_Base10(ob2)+8);
            a[j+1]=a[j+1].substr(0,a[j+1].length()-2);
        }
        operand_addr=to_Base16(SYMTAB[a[j+1]].address);
        if(x) a[3]+=",X";
        ob3=operand_addr;
        a[2]="+"+a[2];
        string loc_ctr=a[0];
        mod<<"M^"<<fill_Str(6,to_Base16(to_Base10(a[0]) + 1))<<"^05"<<endl;
        return fill_Str(2,ob1)+fill_Str(1,ob2)+fill_Str(5,ob3);
    }
}

    void input(string a[])
    {
        int i;
        getline(imt,l);
        get_arr_of_words(l,a,count);
    }

    int reg_num(char a)
    {
        if(a=='A') return 0;
        if(a=='X') return 1;
        if(a=='L') return 2;
        if(a=='B') return 3;
        if(a=='S') return 4;
    if(a=='T') return 5;
    if(a=='F') return 6;
    }


        string fill_Str(int dig,string a)
    {
        string temp="";
        for(int i=0;i<dig-a.length();++i)
        temp+='0';
        return temp+a;
    }

int to_base_10(char a)
{
    if(a=='0') return 0;
    if(a=='1') return 1;
    if(a=='2') return 2;
    if(a=='3') return 3;
    if(a=='4') return 4;
    if(a=='5') return 5;
    if(a=='6') return 6;
    if(a=='7') return 7;
    if(a=='8') return 8;
    if(a=='9') return 9;
    if(a=='A') return 10;
    if(a=='B') return 11;
    if(a=='C') return 12;
    if(a=='D') return 13;
    if(a=='E') return 14;
    if(a=='F') return 15;
}


char to_Base_16(int a)
{
    if(a==0) return '0';
    if(a==1) return '1';
    if(a==2) return '2';
    if(a==3) return '3';
    if(a==4) return '4';
    if(a==5) return '5';
    if(a==6) return '6';
    if(a==7) return '7';
    if(a==8) return '8';
    if(a==9) return '9';
    if(a==10) return 'A';
    if(a==11) return 'B';
    if(a==12) return 'C';
    if(a==13) return 'D';
    if(a==14) return 'E';
    if(a==15) return 'F';
}

 string to_Base16(int a)
{
    string res="";
    while(a>0)
    {
        res=to_Base_16(a%16)+res;
        a/=16;
    }
    return res;
}


int to_Base10(string val)
{
    int l=val.length()-1,curr=1,res=0;
    while(l>=0)
    {
        res+=(to_base_10(val[l])*curr);
        curr*=16;
        --l;
    }
    return res;
}


bool is_Blank_Space(char a)
{
    if(a==' ')  return true;
    if(a=='\t') return true;
    return false;
}

void get_arr_of_words(string a,string code[],int& count)
{
    int i;
    for(i=0;i<5;++i) code[i]="";
    count=0;
    i=0;
    while(is_Blank_Space(a[i])&&i<a.length()) {++i;continue;}
    if(i==a.length()||a[i]=='.'){ nol++;  return;}
    for(;i<a.length();)
    {
        while(is_Blank_Space(a[i])&&i<a.length()) {++i;continue;}
        if(i==a.length()) break;
        for(;!is_Blank_Space(a[i])&&i<a.length();++i) code[count]+=a[i];
        ++count;
    }
    nol++;
}


