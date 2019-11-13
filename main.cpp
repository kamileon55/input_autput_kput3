#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>


int seed = 111;


char verzio[20]="69";
unsigned int alaprouter;
bool finishMode = 0;
int fogadott=0;
int bevaras=0;

bool createLeft = 1;
int lowestEmptyPacket = 9999;

enum class Direction : char
{
    LEFT = 'l',
    RIGHT = 'r'
};


struct Data
{
    unsigned int currRouter;
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

struct PossibleAction
{
	PossibleAction(char c, int i) {
		action = c;
		moveRouter = i;
		value = 0;
	}
	char action; //^, v, c
	union
	{
		int moveRouter;
		int createSlot;
	};

	int value;
};


void simulateAction(std::array < std::array<bool, 10>, 14> routerBits, std::vector<Data> originalPackets, PossibleAction &pa)
{
	//Copy vector
	std::vector<Data> packets(originalPackets);

	//Diversify. Each moving by 1 is better than 1 moving by 4.
	bool couldMove[8] = {0};

	//Copy data into an array
	char state[14][10];
	for (int i = 0; i<14; i++)
		for (int j = 0; j<10; j++)
			state[i][j] = (routerBits[i][j])?'-':'#'; //- open, #blocked

	//Generate a hypothetical packet?
	if (pa.action == 'c')
	{
		Data addition;
		addition.currRouter = alaprouter;
		addition.currStoreId = pa.createSlot;
		addition.dir = createLeft ? Direction::LEFT : Direction::RIGHT;
		addition.fromRouter = alaprouter;
		addition.toRouter = (alaprouter+7)%14;
		packets.push_back(addition);
	}

	//Store packets
	for (int i = 0; i < packets.size(); i++)
		state[packets[i].currRouter][packets[i].currStoreId] = '0'+i;

	//Shift packets?
	char temp;
	if (pa.action == '^')
	{
		temp = state[pa.moveRouter][0];
		for (int i = 0; i<9; i++)
			state[pa.moveRouter][i] = state[pa.moveRouter][i+1];
		state[pa.moveRouter][9] = temp;

		for (int i = 0; i<10; i++)
			if (state[pa.moveRouter][i] >= '0' && state[pa.moveRouter][i] <= '9')
				packets[state[pa.moveRouter][i]-'0'].currStoreId = (packets[state[pa.moveRouter][i]-'0'].currStoreId-1)%14;

	} else
	if (pa.action == 'v')
	{
		temp = state[pa.moveRouter][9];
		for (int i = 9; i>0; i--)
			state[pa.moveRouter][i] = state[pa.moveRouter][i-1];
		state[pa.moveRouter][0] = temp;

		for (int i = 0; i<10; i++)
			if (state[pa.moveRouter][i] >= '0' && state[pa.moveRouter][i] <= '9')
				packets[state[pa.moveRouter][i]-'0'].currStoreId = (packets[state[pa.moveRouter][i]-'0'].currStoreId+1)%14;
	}
	
	//Start simulating
	while (true)
	{
		togo:
		//Automatic upwards shift
		for(int i = 0; i<packets.size(); i++)
			if (packets[i].currStoreId > 0 && state[packets[i].currRouter][packets[i].currStoreId - 1] == '-')
			{

				state[packets[i].currRouter][packets[i].currStoreId - 1] = '0'+i;

				state[packets[i].currRouter][packets[i].currStoreId] = '-';

				packets[i].currStoreId--;

				goto togo;
			}

		//Automatic movement
		for (int i = 0; i < packets.size(); i++)
		{

			int direction = 1;
			if (packets[i].dir == Direction::LEFT) direction = -1;

			if (packets[i].currRouter != packets[i].toRouter && state[(int(packets[i].currRouter) + direction)%14][packets[i].currStoreId] == '-')
			{

				state[(int(packets[i].currRouter) + direction)%14][packets[i].currStoreId] = '0'+i;
				state[packets[i].currRouter][packets[i].currStoreId] = '-';
				packets[i].currRouter = (int(packets[i].currRouter) + direction)%14;
				pa.value++; //increment action's usefulness
				couldMove[i] = 1;

				goto togo;
				
			}
		}



		break;
	}

	for(int i = 0; i<8; i++)
		if (couldMove[i])
			pa.value++;
}

void readData(Reader& to)
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
			std::cerr<<"##end1\n"<<line<<"\n";
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

            MessagePiece & msg = to.receivedPieces.emplace_back();

            std::istringstream(std::move(line).substr(8)) >> msg.index >> msg.message;


            std::cerr<<msg.message<<"__"<<msg.message.size()<<"\n";
            if(msg.message.size()==0)
            {
                finishMode=1;
				lowestEmptyPacket = msg.index;
                std::cerr<<"Finish Mode Started\n";


            }

			//If only enemy packets left, end the game
            bool vegem=true;
            bool belep2=true;
            for(int i=0; i<to.dataArray.size(); i++)
            {
                belep2=true;
                if(to.dataArray[i].fromRouter==alaprouter && to.dataArray[i].dataIndex < lowestEmptyPacket)
                {
                    vegem=false;

                }
            }
            if(vegem && belep2 && finishMode)
            {
                to.hasEnd=true;
				std::cerr<<"##end2\n";
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
	std::cerr<<"##end3\n";
}

int main()
{
    char teamToken[] = "tqEzVLvbq6wz_uWr6HS1";

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

		if (reader.hasEnd || reader.receivedPieces.size() > 100)
		{
			std::cerr<<"##end4\n";
			break;
		}
            


		//Count our active packets
        int bitjeim=0;
        for (int i=0; i<reader.dataArray.size(); i++)
            if(reader.dataArray[i].fromRouter==alaprouter)
                bitjeim++;

		std::vector<PossibleAction> posActs;

		//If we can create
		if (bitjeim < 4 && !finishMode)
		{
			for(int i=0; i<9; i++)
            {

                if(reader.routerBits[alaprouter][i]==1 )
                {
                    bool slotEmpty=1;
                    for(int j=0; j<reader.dataArray.size(); j++)
                    {
                        if(reader.dataArray[j].currStoreId==i && reader.dataArray[j].currRouter==alaprouter )
                        {
                            slotEmpty=0;
                            break;
                        }
                    }

                    if(slotEmpty)
                    {
                        PossibleAction pa('c', i);
						pa.value += 5*(4-bitjeim);
						simulateAction(reader.routerBits, reader.dataArray, pa);
						posActs.push_back(pa);

                    }


                }


            }
		}

		//All possible movements
		for(int i=0; i<reader.dataArray.size(); i++)
        {

            if(reader.dataArray[i].fromRouter==alaprouter)
            {

				PossibleAction pa1('^', reader.dataArray[i].currRouter);
	
				simulateAction(reader.routerBits, reader.dataArray, pa1);
			
				posActs.push_back(pa1);
	

				PossibleAction pa2('v', reader.dataArray[i].currRouter);
				simulateAction(reader.routerBits, reader.dataArray, pa2);
				posActs.push_back(pa2);

			
            }

        }



		//Pick best possible action
		int bestID = 0;
		for(int i = 1; i<posActs.size(); i++) //todo if size() = 0 pass
			if(posActs[i].value > posActs[bestID].value)
				bestID = i;

		//Run best possible action
		PossibleAction &pa = (posActs[bestID]);
		if (pa.action == 'c')
		{
            command="CREATE "+std::to_string(pa.createSlot)+" "+std::to_string(faszpicsa++);
			createLeft = !createLeft;
		} else
		if (pa.action == '^' || pa.action == 'v')
		{
			command="MOVE "+std::to_string(pa.moveRouter)+" "+pa.action;
		}

		std::cerr<<"RUNCOMMAND0";


        std::cout << reader.data[0] << " " << reader.data[1] << " " << reader.data[2] << " " << command << std::endl;
		std::cerr<<"."<<finishMode<<"Command: "<<command<<"\n";
    }

	/*
    for(int i=0; i<azenyem.size(); i++)
    {
        std::cerr<<azenyem[i].message<<" "<<azenyem[i].index<<std::endl;
    }*/

	//Sort
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