#define NUM_STATES 2 // Numero de estados da maquina.
#define NUM_EVENTS 2 // Numero de eventos da maquina.

// definicao dos possiveis eventos gerados pelos estados da FSM
typedef enum event_ {btn_pressed, repeat} event; // Typedef: criando e definindo um novo tipo. Enum: btn_pressed: valor 0 e repeat: valor 1. (Eventos dos estados da maquina)  
// definicao das funcoes que implementam o comportamento de cada estado
event off_state(void); 
event on_state(void);
event end_state(void); // Termino da maquina de estado. Nao vai ser utilizado (maquina em loop).
// array de ponteiros para as funcoes dos estados
event (* state_functions[])(void) = {off_state, on_state, end_state}; // Estrutura ninja. Ponteiro para funcao. Armazenamento da funcao em endereco de memoria. Array para buscar funcoes na memoria.
// definicao dos nomes dos estados
typedef enum state_ {off, on, end} state; // off: valor 0, on: valor 1 e end: valor 2.
// estrutura que define as transicoes dos estados
state state_transitions[NUM_STATES][NUM_EVENTS] = {{on, off}, // Implementar transicoes entre estados. Matriz Estados X Eventos. 
                                                   {off, on}};
// definicao dos estados inicial e final
#define EXIT_STATE end
#define ENTRY_STATE off

// funcao que implementa a transicao de estados.
state lookup_transitions(state cur_state, event cur_evt) { // Mapeamento da Matriz Estados X Eventos.
  return state_transitions[cur_state][cur_evt];
}

