
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <bits/stdc++.h>
#include <vector>
char verzio[20]="69";
unsigned int alaprouter;
enum class Direction : char
{
    LEFT = 'l',
    RIGHT = 'r'
};

int fogadott=0;
  int bevaras=0;
struct Data
{
    unsigned int currRouter; ///
    unsigned int currStoreId;
    unsigned int dataIndex;
    unsigned int messageId;
    unsigned int fromRouter;
    unsigned int toRouter;
    Direction dir;
};

struct MessagePiece
{
    int index;
    std::string message;

};
std::vector<MessagePiece>azenyem;
void swap(MessagePiece *xp, MessagePiece *yp)
{
    MessagePiece temp = *xp;
    *xp = *yp;
    *yp = temp;
}
struct Reader
{
    std::array<unsigned int, 3> data;
    std::string previous;
    std::array<std::array<bool, 10>, 14> routerBits;
    std::vector<Data> dataArray;
    std::vector<MessagePiece> receivedPieces; ///letarolando, rendezendo
    bool hasEnd;
};
struct Rendezendo
{
    float ertek;
    int indexem;
};
std::vector<Rendezendo>anyukad;

void readData(Reader& to) // void barmi(int a)
{
    std::string line;
    to.dataArray.clear();
    //  to.receivedPieces.clear();

    while (std::getline(std::cin, line))
    {
        if (!line.rfind(".", 0))
            return;

        if (!line.rfind("WRONG", 0) ||
                !line.rfind("SCORE", 0) ||
                !line.rfind("TICK", 0))
        {
            to.hasEnd = true;
            to.previous = std::move(line);
        }
        else if (!line.rfind("REQUEST", 0))
        {
            std::stringstream(std::move(line).substr(8)) >> to.data[0] >> to.data[1] >> to.data[2];
            alaprouter=to.data[2];
        }
        else if (!line.rfind("PREVIOUS", 0))
        {
            to.previous = std::move(line).substr(9);
        }
        else if (!line.rfind("ROUTER", 0))
        {
            unsigned int routerIndex;
            std::istringstream(line.substr(7)) >> routerIndex >> line;
            auto it = line.begin();
            for (bool& routers : to.routerBits[routerIndex])
                routers = *it++ == '1';
        }
        else if (!line.rfind("DATA", 0))
        {
            Data& curr = to.dataArray.emplace_back();
            std::istringstream(std::move(line).substr(5))
                    >> curr.currRouter
                    >> curr.currStoreId
                    >> curr.dataIndex
                    >> curr.messageId
                    >> curr.fromRouter
                    >> curr.toRouter
                    >> reinterpret_cast<char&>(curr.dir);
        }
        else if (!line.rfind("MESSAGE"))
        {
           // std::cerr<<"MESSAGE"<<std::endl;


            MessagePiece & msg = to.receivedPieces.emplace_back();

            std::istringstream(std::move(line).substr(8)) >> msg.index >> msg.message;


            if(msg.message.size()==0)
            {
                bevaras++;

            }
            if(beveras>=3)
            {
                 to.hasEnd = true;
            }





           //

         //
            azenyem.push_back(msg);


        }
        else
        {
            std::cerr << "READER ERROR HAPPENED: unrecognized command line: " << line << std::endl;
            to.hasEnd = true;
            return;
        }
    }
    std::cerr << "Unexpected input end." << std::endl;
    to.hasEnd = true;
}

int main()
{
    char teamToken[] = "tqEzVLvbq6wz_uWr6HS1";
    int seed = 2;
    int befutott=0;
    std::string solution;
    std::cout << "START " << teamToken
              << " " << seed<<" "<<verzio
              << std::endl;

    Reader reader = {};

    std::string command;

    int faszpicsa = 0;

    while(true)
    {
        readData(reader);

        if (reader.hasEnd || reader.receivedPieces.size()>100)
        {
             break;
        }


        // TODO logika jobb mint a semmitteves
        ///

        int bitjeim=0;
        for (int i=0; i<reader.dataArray.size(); i++)
        {
            if( reader.dataArray[i].fromRouter==alaprouter)
            {
                bitjeim++;

            }
        }
        /*
        if(bitjeim>0)
        {
            bool vege=true;
            for (int j=0; j<bitjeim; j++)
            {
                if( reader.dataArray[i].fromRouter==alaprouter && reader.receivedPieces[i].message.size()!=0)
                {
                    vege=false;
                }
            }
            if(vege=true)
            {
                to.hasEnd = true;
            }
        }
        */


        int betesz=0;
        int beleptem=0;
        if(bitjeim<4)
        {
            beleptem=1;
            for(int i=0; i<9; i++)
            {
                if(reader.routerBits[alaprouter][i]==1 )
                {
                    int ures=1;
                    for(int j=0; j<reader.dataArray.size(); j++)
                    {
                        if(reader.dataArray[j].currStoreId==i && reader.dataArray[j].currRouter==alaprouter )
                        {
                            ures=0;
                            std::cerr<<"Utkozes a bitek kozott"<<"\n";
                            break;
                        }
                    }
                    if(ures==1)
                    {

                        command= "CREATE ";
                        command=command+std::to_string(i)+" "+std::to_string(faszpicsa++);
               //         std::cerr<<" a["<<i<<"]["<<alaprouter<<"]="<<reader.routerBits[i][alaprouter]<<"\n";
                        betesz=1;
                        break;



                    }




                }

            }
        }


        if(bitjeim==4 || (betesz==0 && beleptem==1))

        {
            long unsigned int min=1000;
            int minIndex=-1;
            for(int i=0; i<8; i++)
            {
                if(reader.dataArray[i].dataIndex<min && reader.dataArray[i].fromRouter==alaprouter)
                {
                    min=reader.dataArray[i].dataIndex;
                    minIndex = i;
                }

            }

            command= "MOVE ";
            command=command+std::to_string(reader.dataArray[minIndex].currRouter)+" "+"^";



        }
        if(reader.receivedPieces.size()!=0)
        {
            //  std::cerr << "Recived pieces number " << reader.receivedPieces[reader.receivedPieces.size()-1].index << std::endl;

        }



        // Ha szeretnetek debug uzenetet kuldeni, akkor megtehetitek.
        // Vigyazzatok, mert maximalisan csak 1024 * 1024 bajtot kaptok vissza
        //   std::cerr << "Send " << command << std::endl;

        // standard out-ra meg mehet ki a megoldas! Mas ne irodjon ide ki ;)



        std::cout << reader.data[0] << " " << reader.data[1] << " " << reader.data[2] << " " << command << std::endl;
     //   std::cerr << reader.data[0] << " " << reader.data[1] << " " << reader.data[2] << " " << command << std::endl;
        if(reader.receivedPieces.size()>100)
        {
            //  break;
        }
    }

    for(int i=0; i<azenyem.size(); i++)
    {
     //   std::cerr<<azenyem[i].message<<" "<<azenyem[i].index<<std::endl;
    }
///**************
    for(int i=0; i<azenyem.size(); i++)
    {
        std::cerr<<azenyem[i].message<<" "<<azenyem[i].index<<std::endl;
    }

    for(long unsigned int i=0; i<azenyem.size()-1; i++)
    {
        for (long unsigned int j=0; j<azenyem.size()-i-1; j++)
        {
            if(azenyem[j].index>azenyem[j+1].index)
            {
                swap(&azenyem[j], &azenyem[j+1]);
            }
        }
    }
    std::cerr<<"Betaprotokol "<<std::endl;
    for(int i=0; i<azenyem.size(); i++)
    {
        std::cerr<<azenyem[i].message<<" "<<azenyem[i].index<<std::endl;
        solution=solution+azenyem[i].message;
    }
    ///************************
    std::cerr<<"Betaprotokol "<<std::endl;



    command="SOLUTION ";
    std::cout << reader.data[0] << " " << reader.data[1] << " " << reader.data[2] << " " << command<<solution<<std::endl;
    std::cerr << reader.data[0] << " " << reader.data[1] << " " << reader.data[2] << " " << command<<solution<<std::endl;
    std::cout << "END (latest message): " << reader.previous << std::endl;
    std::cerr << "END (latest message): " << reader.previous << std::endl;
}
