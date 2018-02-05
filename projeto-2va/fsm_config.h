#define NUM_STATES 5 // Numero de estados da maquina.
#define NUM_EVENTS 6 // Numero de eventos da maquina.

// definicao dos possiveis eventos gerados pelos estados da FSM
typedef enum event_ {btn_pressed, time10, wifiIsConnected, serverIsConnected, reconnectWifi, reconnectServer} event; // Typedef: criando e definindo um novo tipo. Enum: btn_pressed: valor 0 e repeat: valor 1. (Eventos dos estados da maquina)  

// definicao das funcoes que implementam o comportamento de cada estado
event connectWifi_state(void); 
event connectServer_state(void);
event waitEvent_state(void); // Termino da maquina de estado. Nao vai ser utilizado (maquina em loop).
event sendTempHumiTime_state(void);
event sendTempHumiBtn_state(void);

// array de ponteiros para as funcoes dos estados
event (* state_functions[])(void) = {connectWifi_state, connectServer_state, waitEvent_state, sendTempHumiTime_state, sendTempHumiBtn_state}; // Estrutura ninja. Ponteiro para funcao. Armazenamento da funcao em endereco de memoria. Array para buscar funcoes na memoria.
// definicao dos nomes dos estados
typedef enum state_ {connectWifi, connectServer, waitEvent, sendTempHumiTime, sendTempHumiBtn} state; // off: valor 0, on: valor 1 e end: valor 2.

// estrutura que define as transicoes dos estados
state state_transitions[NUM_STATES][NUM_EVENTS] = {{connectWifi, connectWifi, connectServer, connectWifi, connectWifi, connectWifi}, // Implementar transicoes entre estados. Matriz Estados X Eventos. 
                                                   {connectServer, connectServer, connectServer, waitEvent, connectServer, connectServer},//
                                                   {sendTempHumiBtn, sendTempHumiTime, waitEvent, waitEvent, waitEvent, waitEvent},
                                                   {sendTempHumiTime, sendTempHumiTime, sendTempHumiTime, sendTempHumiTime, connectWifi, connectServer},
                                                   {sendTempHumiBtn,sendTempHumiBtn, sendTempHumiBtn, sendTempHumiBtn, connectWifi, connectServer}
                                                   };
// definicao dos estados inicial e final
#define EXIT_STATE connectWifi //trocar caso for ter um estado de saida
#define ENTRY_STATE connectWifi

// funcao que implementa a transicao de estados.
state lookup_transitions(state cur_state, event cur_evt) { // Mapeamento da Matriz Estados X Eventos.
  return state_transitions[cur_state][cur_evt];
}

