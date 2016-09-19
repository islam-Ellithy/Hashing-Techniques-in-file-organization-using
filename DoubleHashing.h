#ifndef DOUBLEHASHING_H
#define DOUBLEHASHING_H


class DoubleHashing
{
public:
    int cnt ;
    DoubleHashing()
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
        return h1%101 ;
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

    void initializeFile(string path)  // done
    {
        fstream file;
        file.open(path,ios::trunc|ios::out);
        struct bucket s ;
        //=====================================================
        strcpy(s.Name,"*") ;
        s.offset = -1 ;
        file.seekp(0);
        //=====================================================
        for(int i = 1; i<=101; i++)
        {
            for(int j = 1 ; j<=2; j++)
            {
                cout << "Write Record " << i << " " << j << '\n';
                file.write((char*)&s,sizeof(s));
            }
        }
        file.close();
        //=====================================================
    }
//=====================================================

    void WriteOnIndexFile(fstream &ofile,bucket &ht)
    {
        bucket t ;

        int RRN = H1(ht.Name);

        int jump = H2(ht.Name);

        int newRRN = RRN;

        bucket arr[102][3];

        for(int i = 1; i<=101; i++)
        {
            for(int j = 1 ; j<=2; j++)
            {
                ofile.read((char*)&t,sizeof(bucket));
                arr[i][j] = t ;
            }
        }

        int i = 1 ;

        while(true)
        {
            cout << newRRN << '\n';
            if( (arr[newRRN][i].offset==-1) || (arr[newRRN][i].Name[0]=='#'))
            {
                arr[newRRN][i] = ht ;
                break ;
            }
            else if( (arr[newRRN][i+1].offset==-1) || (arr[newRRN][i+1].Name[0]=='#'))
            {
                arr[newRRN][i+1] = ht ;
                break ;
            }

            newRRN = (newRRN + jump)%101;

            //No Free slot in the file
            if(newRRN == RRN)
            {
                cout<<"No Free slots in the file";
                return;
            }
        }
        ofile.seekp(0);
        for(int i = 1; i<=101; i++)
        {
            for(int j = 1 ; j<=2; j++)
            {
                ofile.write((char*)&arr[i][j],sizeof(bucket));
            }
        }
        ofile.write((char*)&t,sizeof(bucket));

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
        file.write((char*)buffer , strlen(buffer));
        file.close();
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
            file.read((char*)&s,sizeof(s));
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
        int i = 0 ;
        while(true)
        {
            file.read((char*)&b,sizeof(bucket));
            if(file.fail())
                break ;
            cout << (i++) << '\n' <<  b << '\n' ;
        }
        file.close();
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

    int getCounter()const
    {
        return cnt ;
    }


    int searchStudent(char name[], string path) // done
    {
        bucket t ;
        fstream  ifile ;
        ifile.open(path);
        int RRN = H1(name);

        int jump = H2(name);

        int newRRN = RRN;

        ifile.seekg(0);
        ifile.seekp(0);

        bucket arr[102][3];

        for(int i = 1; i<=101; i++)
        {
            for(int j = 1 ; j<=2; j++)
            {
                ifile.read((char*)&t,sizeof(bucket));
                arr[i][j] = t ;
            }
        }


        ifile.close();

        int i = 1 ;

        while(true)
        {

            cnt++ ;

            if( (strcmp(arr[newRRN][i].Name,name)==0) )
            {
                return arr[newRRN][i].offset ;
            }

            else if(strcmp(arr[newRRN][i+1].Name,name)==0)
            {
                return arr[newRRN][i+1].offset ;
            }


            newRRN = (newRRN + jump)%101;

            //No Free slot in the file
            if(newRRN == RRN)
            {
                //  cout<<"Not Found\n";
                return -1 ;
            }
        }
    }
//======================================================
    void deleteStudent(char name[],string path)
    {
        bucket t ;

        fstream file ;

        file.open(path,ios::out|ios::in|ios::trunc);

        int RRN = H1(name);

        int jump = H2(name);

        int newRRN = RRN;

        file.seekg(0,ios::beg);

        file.seekp(0,ios::beg);

        bucket arr[102][3];

        for(int i = 1; i<=101; i++)
        {
            for(int j = 1 ; j<=2; j++)
            {
                file.read((char*)&t,sizeof(bucket));
                arr[i][j] = t ;
            }
        }

        int i = 1 ;

        while(true)
        {
            if(strcmp(arr[newRRN][i].Name,name)==0)
            {
                arr[newRRN][i].Name[0] = '#' ;
                cout << "Name : " << arr[newRRN][i].Name << '\n';
                break ;
            }
            else if(strcmp(arr[newRRN][i+1].Name,name)==0)
            {
                arr[newRRN][i+1].Name[0] = '#' ;
                cout << "Name : " << arr[newRRN][i+1].Name << '\n';
                break;
            }

            newRRN = (newRRN + jump)%101;

            //No Free slot in the file
            if(newRRN == RRN)
            {
                cout<<"Not Found\n";
                return  ;
            }
        }

        file.seekp(0);

        for(int i = 1; i<=101; i++)
        {
            for(int j = 1 ; j<=2; j++)
            {
                cout << arr[i][j] << '\n';
                file.write((char*)&arr[i][j],sizeof(bucket));
            }
        }
        file.close();

    }


    void Start()
    {
        char ans ;
        string datapath = "dataDouble.txt" ;
        string hashpath = "HashedIndexFileDouble.txt" ;
        fstream datafile;
        fstream hashfile;
        ifstream ifile ;

        datafile.open(datapath);
        hashfile.open(hashpath,ios::trunc| ios::out);
        ifile.open("datainput.txt");

        do
        {
            Student t ;
Ask:
            short x ;
            cout << "======================================================\n" ;

            cout << "Double Hashing Technique\n\n";
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
                ofstream data;
                data.open(datapath,ios::out|ios::trunc);
                initializeFile(hashpath);
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
                cin.getline(name,52,'\n');
                int offset = searchStudent(name,hashpath);
                Student s ;
                fstream dataf ;
                dataf.open(datapath);
                dataf.seekg(offset);
                dataf >> s ;

                if(strcmp(s.Name,name)==0)
                    cout << s <<'\n';
                else
                {
                    cout << "Not Found\n";
                }
                dataf.close();
            }
            else if(x==3)
            {
                cout << "Enter name that do want to delete it : ";
                char name[52];
                cin.getline(name,52,'\n');
                deleteStudent(name,hashpath);
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
                break  ;
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
    }


};

#endif // DOUBLEHASHING_H
