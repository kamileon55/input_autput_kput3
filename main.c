#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_PLAYERS_COUNT 1

#define strncmp_n(buff, arr) \
    strncmp(buff, arr, sizeof(arr) / sizeof(arr[0]) - 1)

typedef enum {LEFT='l', RIGHT='r'} Direction;

typedef struct {
    unsigned int currRouter;
    unsigned int currStoreId;
    unsigned int dataIndex;
    unsigned int messageId;
    unsigned int fromRouter;
    unsigned int toRouter;
    Direction direction;
} Data;

typedef struct {
    int index;
    char message[100];
} MessagePiece;

typedef struct {    
    unsigned int data[3];
    char previous[100];
    bool routerBits[14][10];
    Data dataArray[MAX_PLAYERS_COUNT*5];
    int dataArrayFilled;
    MessagePiece receivedPieces[5];
    int receivedPiecesFilled;
    bool hasEnd;
} Reader;

void readData(Reader* result) {
    char buffer[256];
    result->dataArrayFilled = 0;
    result->receivedPiecesFilled = 0;
    
    while (fgets(buffer, 255, stdin) != NULL) {
        if (!strncmp_n(buffer, "."))
            return;

        if (!strncmp_n(buffer, "WRONG") || 
            !strncmp_n(buffer, "SCORE") || 
            !strncmp_n(buffer, "TICK"))
        {
            result->hasEnd = true;
            strncpy(result->previous, buffer, 99);
        } else if (!strncmp_n(buffer, "REQUEST")) {
            sscanf(buffer + 8, "%u %u %u\n", result->data, result->data + 1, result->data + 2);
        } else if (!strncmp_n(buffer, "PREVIOUS")) {
            strncpy(result->previous, buffer + 9, 99);
        } else if (!strncmp_n(buffer, "ROUTER")) {
            unsigned int routerIndex;
            sscanf(buffer + 7, "%u", &routerIndex);
            unsigned int moveBuffer = 9 + (routerIndex >= 10);

            for (unsigned int storeId = 0; storeId < 10; ++storeId)
                result->routerBits[routerIndex][storeId] = buffer[moveBuffer + storeId] == '1';
        } else if (!strncmp_n(buffer, "DATA")) {
            char dir;
            Data* data = result->dataArray + result->dataArrayFilled++;
            sscanf(buffer + 5, "%u %u %u %u %u %u %c\n", 
                &data->currRouter,
                &data->currStoreId,
                &data->dataIndex,
                &data->messageId,
                &data->fromRouter,
                &data->toRouter,
                &dir);
            data->direction = dir;
        } else if (!strncmp_n(buffer, "MESSAGE")) {
            MessagePiece* msg = result->receivedPieces + result->receivedPiecesFilled++;
            sscanf(buffer + 8, "%u %99s\n", &msg->index, msg->message);
        } else {
            fprintf(stderr, "READER ERROR HAPPENED: unrecognized command line: %s", buffer);
            result->hasEnd = true;
            return;
        }
    }
    fputs("Unexpected input end.\n", stderr);
    result->hasEnd = true;
}

int main() {
    char teamToken[] = "tqEzVLvbq6wz_uWr6HS1";
    // int seed = 0;

    fprintf(stdout, "START %s", teamToken);
    // fprintf(stdout, " %d", seed);
    fputc('\n', stdout);

    Reader reader = {};

    while (1) {
        readData(&reader);
        
        if (reader.hasEnd)
            break;
        
        // TODO logika jobb mint a semmitteves
        const char* command = "PASS";
        
        // Ha szeretnetek debug uzenetet kuldeni, akkor megtehetitek.
        // Vigyazzatok, mert maximalisan csak 1024 * 1024 bajtot kaptok vissza
        fprintf(stderr, "Send %s\n", command);
       
        // standard out-ra meg mehet ki a megoldas! Mas ne irodjon ide ki ;)
        fprintf(stdout, "%u %u %u %s\n", reader.data[0], reader.data[1], reader.data[2], command);
    }
    fprintf(stderr, "END (latest message): %s\n", reader.previous);
}
