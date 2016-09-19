#ifndef SCATERTABLE_H
#define SCATERTABLE_H


class ScaterTable
{
public:
    int cnt ;
    ScaterTable()
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
    struct index
    {
        char Name[51];
        int offset ;
        int next = -1 ;
        friend ostream& operator << (ostream &out , index &b)
        {
            out << "Name : " << b.Name << '\n';
            out << "offset : " << b.offset << '\n' ;
            out << "Next : " << b.next << '\n';
        }
    };
//=====================================================

    int getCounter()const
    {
        return cnt ;
    }
    void initializeFile(string path)  // done
    {
        fstream file;
        file.open(path,ios::trunc|ios::out);
        int address = -1 ;
        //=====================================================
        file.seekp(0);
        //=====================================================
        for(int i = 1; i<=101; i++)
        {
            cout << "Record " << i << " has been Written" << '\n';
            file.write((char*)&address,sizeof(int));
        }
        file.close();
        //=====================================================
    }
//=====================================================

    void WriteOnIndexFile(string Primarypath,string Secondarypath,index &ht)
    {
        index t ;

        int offset_s ;

        fstream pfile , sfile ;
//======================================================
        pfile.open(Primarypath,ios::in|ios::out);
        sfile.open(Secondarypath);
//======================================================
        int RRN = H1(ht.Name);

        //    sfile.clear();

        sfile.seekp(0,ios::end);

        offset_s = sfile.tellp();


//======================================================

        int arr[102] , b ;

        if(pfile.good())
        {
            pfile.seekg(0);
            for(int i = 1; i<=101; i++)
            {
                pfile.read((char*)&b,sizeof(int));
                arr[i] = b ;
            }
        }

        sfile.seekg(0,ios::beg);


        int newRRN = RRN ;

        if(arr[RRN]==-1)
        {
            arr[RRN] = offset_s ;
        }
        else
        {
            sfile.seekg(arr[newRRN]);
            while(true)
            {
                newRRN = sfile.tellg() ;
                sfile.read((char*)&t,sizeof(index));
                if(t.Name[0]=='#')
                {
                    sfile.seekp(newRRN,ios::beg);
                    ht.next = t.next ;
                    sfile.write((char*)&ht,sizeof(ht));
                    return ;
                }
                else if(t.next==-1)
                {
                    t.next = offset_s ;
                    sfile.seekp(newRRN,ios::beg);
                    sfile.write((char*)&t,sizeof(t));
                    break ;
                }
            }
        }

        sfile.clear();
        pfile.clear();
        sfile.seekp(0,ios::end);
        sfile.write((char*)&ht,sizeof(index));

//======================================================
        pfile.seekp(0,ios::beg);
        for(int i = 1; i<=101; i++)
        {
            pfile.write((char*)&arr[i],sizeof(int));
        }
        pfile.close();
        sfile.close();
    }
//=====================================================

    void WriteOnDataFile(string path,Student &s)
    {
        fstream file ;
        file.open(path);
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
        int b ;
        file.seekg(0,ios::beg);

        int i = 0 ;
        while(true)
        {

            file.read((char*)&b,sizeof(int));
            if(file.fail())
                break ;
            cout << (++i) << '\t' <<  b << '\n' ;
        }
        file.close();
    }
    void PrintIndexFile(string indexpath)
    {
        fstream sfile;
        sfile.open(indexpath);
        sfile.seekg(0);
        struct index t ;
        while(true)
        {
            sfile.read((char*)&t,sizeof(index));

            if(sfile.fail())
            {
                break ;
            }
            cout << t << '\n';
        }
        sfile.close();
    }
    void addStudent(Student s, string datapath,string hashpath,string indexpath)// done
    {
        fstream datafile;

        datafile.open(datapath);
//======================================================
        int RRN = H1(s.Name);
//======================================================
        datafile.seekp(0,ios::end);
//======================================================
        int Offset = datafile.tellp();
        datafile.close();
//======================================================
        index h ;
//======================================================
        strcpy(h.Name,s.Name) ;
//======================================================
        h.offset = Offset ;
//======================================================
        h.next = -1 ;
//======================================================
        WriteOnDataFile(datapath,s);
//======================================================
        WriteOnIndexFile(hashpath,indexpath,h);
//======================================================
    }
//======================================================


    int searchStudent(char name[], string ppath,string spath) // done
    {
        index t ;
        int temp ;
        fstream pfile , sfile ;

        pfile.open(ppath);
        sfile.open(spath);

        int RRN = H1(name);

        int newRRN = RRN ;

        int arr[102] ;

        pfile.seekg(0);

        for(int i = 1; i<=101; i++)
        {
            pfile.read((char*)&temp,sizeof(int));
            arr[i] = temp ;
        }

        newRRN = arr[RRN] ;

        while(true)
        {
            cnt++ ;
            sfile.seekg(newRRN);

            sfile.read((char*)&t,sizeof(index));

            if(sfile.fail())
                break ;

            if(strcmp(t.Name,name)==0)
            {
                return t.offset ;
            }
            else if(newRRN==-1)
            {
                return -1 ;
            }
            newRRN = t.next ;
        }
        pfile.close();
        sfile.close();
    }
//======================================================
    void deleteStudent(char name[],string ppath,string spath)
    {
        index t ;
        int temp ;
        fstream pfile , sfile ;

        pfile.open(ppath);
        sfile.open(spath);

        int RRN = H1(name);

        int newRRN = RRN ;

        int arr[102] ;

        pfile.seekg(0);

        for(int i = 1; i<=101; i++)
        {
            pfile.read((char*)&temp,sizeof(int));
            arr[i] = temp ;
        }

        newRRN = arr[RRN] ;

        while(true)
        {
            sfile.seekg(newRRN);

            sfile.read((char*)&t,sizeof(index));

            if(sfile.fail())
                break ;
            if(strcmp(t.Name,name)==0)
            {
                t.Name[0]='#' ;

                sfile.seekp(newRRN);

                sfile.write((char*)&t,sizeof(index));

                return ;
            }
            else if(newRRN==-1)
            {
                return  ;
            }
            newRRN = t.next ;
        }
        pfile.close();
        sfile.close();
    }


    void Start()
    {
        char ans ;
        string datapath = "scaterData.txt" ;
        string hashpath = "primary.txt" ;
        string indexpath = "Secondary.txt";

        fstream datafile;
        fstream hashfile;
        ifstream ifile ;

        datafile.open(datapath);
        hashfile.open(hashpath);
        ifile.open("datainput.txt");

        do
        {
            Student t ;
Ask:
            short x ;
            cout << "======================================================\n" ;

            cout << "Scater Table Technique\n\n";
            cout << "Enter your choice\n";

            cout << "======================================================\n" ;

//======================================================
            puts("\n0-Initialize hash file\n");
            puts("1-Insert\n");
            puts("2-search\n");
            puts("3-delete\n");
            puts("4-load from Data file\n");
            puts("5-show hash file\n");
            puts("6-show index file\n");
            puts("7-Compute average\n");
            puts("8-Exit\n");
//======================================================

            cin >> x ;

            cin.ignore();

            if(x==0)
            {
                ofstream data;
                ofstream sfile ;
                sfile.open(indexpath,ios::out|ios::trunc);
                data.open(datapath,ios::out|ios::trunc);
                initializeFile(hashpath);
                data.close();
            }
            else if(x==1)
            {
                //cin >> t ;
                ifile >> t ;
                cout << t ;
                addStudent(t,datapath,hashpath,indexpath);
            }
            else if(x==2)
            {
                cout << "Enter name the do want to search about : ";
                char name[52];
                cin.getline(name,52,'\n');
                int offset = searchStudent(name,hashpath,indexpath);
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
                deleteStudent(name,hashpath,indexpath);
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
                PrintIndexFile(indexpath);
            }
            else if(x==7)
            {
                cout << "Average = " << getCounter()/27.0 << '\n';
            }
            else if(x==8)
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

#endif // SCATERTABLE_H
