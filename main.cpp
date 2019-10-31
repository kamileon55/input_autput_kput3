#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <bits/stdc++.h>
#include <vector>
enum class Direction : char
{
    LEFT = 'l',
    RIGHT = 'r'
};

 

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
std::vector<MessagePiece> azenyem;
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
            std::cerr<<"MESSAGE"<<std::endl;
            
            MessagePiece & msg = to.receivedPieces.emplace_back();
            
            std::istringstream(std::move(line).substr(8)) >> msg.index >> msg.message;
          //  std::cerr<<msg.message<<"   "<<msg.index<<std::endl;
            if(msg.message.size()==0)
            {
                to.hasEnd = true;
            }
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
     int seed = 0;
    int n=0;
    int befutott=0;
    std::string solution;
    std::cout << "START " << teamToken
               << " " << seed
              << std::endl;

    Reader reader = {};
    
   std::string command;
    
    while(true)
    {
        readData(reader);
        
        if (reader.hasEnd)
            break;

        // TODO logika jobb mint a semmitteves
        if(reader.dataArray.size()<5)
        {
            
        
                for(int i=0; i<9; i++)
                {
                    if(reader.routerBits[3][i]==1)
                    {
                        command= "CREATE ";
                        command=command+std::to_string(i)+" "+std::to_string(n);
                        n++;
                        break;
                    }
                    
                }
        }
        else
        {
            long unsigned int min=1000;
            for(int i=0; i<5; i++)
            {
                if(reader.dataArray[i].dataIndex<min)
                {
                    min=reader.dataArray[i].dataIndex;
                    command= "MOVE ";
                        command=command+std::to_string(reader.dataArray[i].currRouter)+" "+"^";
                        break;
                }
                
            }
            
            
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
            if(reader.receivedPieces.size()>100)
        {
            break;
        }
            
        
        
        
    }
  
    for(long unsigned int i=0; i<reader.receivedPieces.size()-1; i++)
    {
        for (long unsigned int j=0; j<reader.receivedPieces.size()-i-1; j++)
        {
            if(reader.receivedPieces[j].index>reader.receivedPieces[j+1].index)
            {
                swap(&reader.receivedPieces[j], &reader.receivedPieces[j+1]);
            }
        }
    }
     for( long unsigned int i=0; i<reader.receivedPieces.size(); i++)
        {
           // solution=solution+reader.receivedPieces[i].message;
           std::cerr<<reader.receivedPieces[i].message<<"   "<<reader.receivedPieces[i].index<<std::endl;
           solution=solution+reader.receivedPieces[i].message;// std::cerr<< solution;
        }
    std::cerr<<"Megtortent"<<std::endl;/*
        for(int i=0; i<reader.receivedPieces.size(); i++)
            {
                for(int j=i+1; j<reader.receivedPieces.size(); j++)
                {
                    if (reader.receivedPieces[j].index < reader.receivedPieces[i].index)
                    {
                        MessagePiece cser=reader.receivedPieces[j];
                        reader.receivedPieces[j]=reader.receivedPieces[i];
                        reader.receivedPieces[i]=cser;
                    }
                }
        }
        */
        
      /* 
      std::cerr<<"Meret: "<<reader.receivedPieces.size()<<std::endl;
        for( int i=0; i<reader.receivedPieces.size(); i++)
        {
            solution=solution+reader.receivedPieces[i].message;
           // std::cerr<< reader.receivedPieces[i].index<<"       -----     "<<reader.receivedPieces[i].message;
            std::cerr<< solution;
        }
        */
          command="SOLUTION ";
    std::cout << reader.data[0] << " " << reader.data[1] << " " << reader.data[2] << " " << command <<solution<<" "<<std::endl;
    std::cerr << "END (latest message): " << reader.previous << std::endl;
}
