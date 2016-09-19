#include<iostream>
#include <cstdlib>
using namespace std ;
#ifndef PROGRESSIVEOVERFLOW_H
#define PROGRESSIVEOVERFLOW_H


class ProgressiveOverflow
{
public:
    int cnt ;
    ProgressiveOverflow()
    {
        cnt = 0 ;
    }
    struct Student
    {
        char Name[51] = "" ; //max len = 50
        char GPA[4] = "" ; //max len =3
        char Address[51] = "" ; //max len=50
        char Mobile[12]  = "" ;//max len=11
        char Department[3] = "" ; //max len=2

        friend ostream &operator << (ostream &out,Student &s)
        {
            out << "Name : " << s.Name << '\n';
            out << "GPA : " << s.GPA << '\n';
            out << "Address : " << s.Address << '\n';
            out << "Mobile : " << s.Mobile << '\n';
            out << "Department : " << s.Department << '\n';
            return out ;
        }
        friend istream &operator >> (istream &in,Student &s)
        {
            if(in==cin)
            {
                cout << "Enter Name : ";
                in.getline(s.Name,51,'\n');
                cout << "Enter GPA : ";
                in.getline(s.GPA,4,'\n');
                cout << "Enter Address : ";
                in.getline(s.Address,51,'\n') ;
                cout << "Enter Mobile : ";
                in.getline(s.Mobile,12,'\n') ;
                cout << "Enter Department : ";
                in.getline(s.Department,3,'\n') ;
            }
            else
            {
                in.getline(s.Name,51,'\\');
                in.getline(s.GPA,4,'\\');
                in.getline(s.Address,51,'\\') ;
                in.getline(s.Mobile,12,'\\') ;
                in.getline(s.Department,3,'\n') ;
                return in ;
            }
        }
    };


    int H1(char Name[51])
    {
        int h1 = 1 ;
        h1 *= Name[0]*Name[1] ;
        return h1%303 ;
    }
//=====================================================
    int H2(char Name[51])
    {
        int h2  = 1 ;
        h2 *= Name[2]*Name[3];
        return h2%23 ;
    }
//=====================================================
    struct bucket
    {
        char Name[51];
        int offset ;
        friend ostream& operator << (ostream &out , bucket &b)
        {
            out << "Name : " << b.Name << '\n';
            out << "offset : " << b.offset << '\n' ;
        }
    };
//=====================================================

    void initializeFile(fstream &file)  // done
    {
        struct bucket s ;
        //=====================================================
        strcpy(s.Name,"*") ;
        s.offset = -1 ;
        //=====================================================
        for(int i = 0; i<101; i++)
        {
            for(int j = 0 ; j<3; j++)
            {
                cout << "Write Record " << i << " " << j << '\n';
                file.write((char*)&s,sizeof(s));
            }
        }
        //=====================================================
    }
//=====================================================

    void WriteOnIndexFile(fstream &f,bucket &ht)
    {
        int Hash = H1(ht.Name);
//=====================================================
        f.seekg(Hash*sizeof(bucket),ios::beg) ;
//=====================================================
        int RRN = f.tellg();
//=====================1================================
        bucket b ;
        f.read((char*)&b,sizeof(b));
//=====================================================
        if(b.Name=="*")
        {
            f.seekp(Hash*sizeof(bucket),ios::beg);
//=====================================================
            f.write((char*)&ht,sizeof(ht)) ;
//=====================================================
        }
        else if(b.Name[0]=='#')
        {
            cout << b.Name[0] << '\n';
            f.seekp(RRN,ios::beg);
//=====================================================
            f.write((char*)&ht,sizeof(ht)) ;
//=====================================================
        }
//=====================================================
        else
        {
            int cnt = Hash+1 ;

            while(true)
            {
                if(cnt==Hash)
                {
                    cout << "The file is full\n";
                    break ;
                }
                f.seekg(cnt*sizeof(bucket),ios::beg);
                f.read((char*)&b,sizeof(b));
                if(strcmp(b.Name,"*")==0)
                {
                    f.seekp(cnt*sizeof(bucket),ios::beg);
                    f.write((char*)&ht,sizeof(ht));
                    break ;
                }
                else if(b.Name[0]=='#')
                {
                    f.seekp(cnt*sizeof(bucket),ios::beg);
//=====================================================
                    f.write((char*)&ht,sizeof(ht)) ;
//=====================================================
                    break ;
                }
                cnt++ ;
                cnt %= 303 ;
            }
//=====================================================
        }
//=====================================================

    }
//=====================================================

    void WriteOnDataFile(fstream &file,Student &s)
    {
        char *buffer = new char[200];
        strcpy(buffer,s.Name);
        strcat(buffer,"\\");
        strcat(buffer,s.GPA);
        strcat(buffer,"\\");
        strcat(buffer,s.Address);
        strcat(buffer,"\\");
        strcat(buffer,s.Mobile);
        strcat(buffer,"\\");
        strcat(buffer,s.Department);
        strcat(buffer,"\n");
//    file.clear();
        if(file.fail())
            cout << "Error in the file " << '\n';
        file.seekp(0,ios::end);
        cout << "tellp = " << file.tellp()<<'\n' ;
        file.write((char*)buffer , strlen(buffer));
        file.close();
        cout << buffer << '\n';
        delete buffer ;
    }

    void PrintFile(string path)
    {
        fstream file ;
        file.open(path);
        file.seekg(0);
        Student s ;
        while(true)
        {
            //file.read((char*)&s,sizeof(s));
            file >> s ;
            if(file.fail())
                break ;
            cout << s <<'\n';
        }
        file.close();
    }
    void PrintHashFile(string path)
    {
        fstream file ;
        file.open(path);
        bucket b ;
        file.seekg(0,ios::beg);
        while(true)
        {
            file.read((char*)&b,sizeof(bucket));
            if(file.fail())
                break ;
            cout << b ;
        }
        file.close();
    }
    int getCounter()const
    {
        return cnt ;
    }
    void addStudent(Student s, string datapath,string hashpath)
    {
        fstream hashfile;
        fstream datafile;
        hashfile.open(hashpath);
        datafile.open(datapath);
//======================================================
        int RRN = H1(s.Name);
//======================================================
        datafile.seekp(0,ios::end);
        hashfile.seekg(RRN*sizeof(bucket));
//======================================================
        int Offset = datafile.tellp();
        // cout << "offset = " << Offset << '\n';
//======================================================
        bucket h ;
//======================================================
        strcpy(h.Name,s.Name) ;
//======================================================
        h.offset = Offset ;
//======================================================
        WriteOnIndexFile(hashfile,h);
//======================================================
        WriteOnDataFile(datafile,s);
//======================================================
        hashfile.close();
        datafile.close();
    }
//======================================================


    int searchStudent(char Name[], fstream& ifile)
    {
        int RRN = H1(Name);
//======================================================

        ifile.seekg(RRN*sizeof(bucket),ios::beg);
//======================================================

        bucket b ;
//======================================================

        ifile.read((char*)&b,sizeof(bucket));
//======================================================
        cnt++ ;

        if(strcmp(Name,b.Name)==0)
        {
            return b.offset ;
        }
//======================================================
        else
        {
            int cnt = RRN+1;
            while(true)
            {
                cnt++ ;
                // cout << cnt << '\n';
                if(cnt==RRN)
                {
                    break ;
                }
                ifile.seekg(cnt*sizeof(bucket));
                ifile.read((char*)&b,sizeof(bucket));
                if(strcmp(Name,b.Name)==0)
                {
                    return b.offset ;
                }
                cnt = cnt%303 ;
            }
        }
        return -1;
    }
//======================================================
    void deleteStudent(char name[],fstream &file)
    {
        int RRN = H1(name);

        struct bucket b ;

        file.seekg(RRN*sizeof(bucket));

        file.read((char*)&b,sizeof(bucket));

        if(strcmp(name,b.Name)==0)
        {
            file.seekp(RRN*sizeof(bucket));
            char Deleted = '#';
            file.write((char*)&Deleted,1);
        }
        else
        {
            int cnt = RRN+1;
            while(true)
            {
                cout << cnt << '\n';
                if(cnt==RRN)
                {
                    cout << "Not Found\n" ;
                    break ;
                }
                file.seekg(cnt*sizeof(bucket));
                file.read((char*)&b,sizeof(bucket));
                if(strcmp(name,b.Name)==0)
                {
                    char Deleted = '#';
                    file.seekp(cnt*sizeof(bucket));
                    file.write((char*)&Deleted,1);
                    break ;
                }
                cnt++ ;
                cnt = cnt%303 ;
            }
        }
    }
    void Start()
    {
        char ans ;
        string datapath = "data.txt" ;
        string hashpath = "HashedIndexFile.txt" ;
        fstream datafile;
        fstream hashfile;
        ifstream ifile ;
        ifstream sfile ;
        sfile.open("Search.txt",ios::in);


        datafile.open(datapath);
        hashfile.open(hashpath);
        ifile.open("datainput.txt");

        do
        {
            Student t ;

Ask:
            short x ;
            cout << "======================================================\n" ;

            cout << "Progressive Overflow Technique\n\n";
            cout << "Enter your choice\n";

            cout << "======================================================\n" ;

//======================================================
            puts("\n0-Initialize hash file\n");
            puts("1-Insert\n");
            puts("2-search\n");
            puts("3-delete\n");
            puts("4-load from Data file\n");
            puts("5-show hash file\n");
            puts("6-Compute average\n");
            puts("7-Exit\n");
//======================================================
            cin >> x ;

            cin.ignore();

            if(x==0)
            {
                ofstream hashf ;
                hashf.open(hashpath,ios::out|ios::trunc);
                ofstream data;
                data.open(datapath,ios::out|    ios::trunc);
                initializeFile(hashfile);
                hashf.close();
                data.close();
            }
            else if(x==1)
            {
                //cin >> t ;
                ifile >> t ;
                cout << t ;
                addStudent(t,datapath,hashpath);
                // WriteOnDataFile(datapath,t);
            }
            else if(x==2)
            {
                cout << "Enter name the do want to search about : ";
                char name[52];
                sfile.getline(name,52,'\n');
                cout << name <<'\n';
                int offset = searchStudent(name,hashfile);
                Student s ;
                datafile.seekg(offset);
                datafile >> s ;
                if(strcmp(s.Name,name)==0)
                    cout << s <<'\n';
                else
                    cout << "Not Found\n";
            }
            else if(x==3)
            {
                cout << "Enter name that do want to delete it : ";
                char name[52];
                cin.getline(name,52,'\n');
                deleteStudent(name,hashfile);
            }
            else if(x==4)
            {
                PrintFile(datapath);
            }
            else if(x==5)
            {
                PrintHashFile(hashpath);
            }
            else if(x==6)
            {
                cout << "Average = " << getCounter()/27.0 << '\n';
            }
            else if(x==7)
            {
                break ;
            }
            else
            {
                goto Ask ;
            }


            cout << "Enter 'y' to continue : ";
            cin >> ans ;
            cin.ignore();
        }
        while(ans=='y' || ans=='Y');

        datafile.close();
        hashfile.close();
        sfile.close();

    }
};

#endif // PROGRESSIVEOVERFLOW_H
