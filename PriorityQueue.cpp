// Implementações relacionadas com a lista de prioridade (OPEN)

#include"AStarHeader.h"

// !todo! Criar um nó especial para a lista de prioridades
// Ideia -> o construtor desse nó deve:
//    - Receber um nó convencional como argumento
//    - Ter como atributos um valor "f" e o "node_index" apenas
// Se feito:
//    - Nó normal não precisa mais armazenar o "f"

// Estrututura de propósito geral, que guarda apenas o necessário
//  - Chave de referência: diz a qual objeto faz referência
//  - f: calculado e utilizado para fazer o ordenamento da lista, mostra qual é o "melhor" nó
/*
PriorityQueueContainer::PriorityQueueContainer(long f, std::vector<long> reference_key)
{
    this->f = f;
    this->reference_key = reference_key;
}

// essa classe é a que organiza a lista de prioridades
// baseado no valor f de cada nó
bool SortPriorityQueue::operator() (PriorityQueueContainer n1, PriorityQueueContainer n2)
{
    if (n1.f > n2.f)
    {
        return true;
    }
    else
    {
        return false;
    }
}
*/

// usa uma cópia da minha lista de prioridade
// imprime a lista de prioridades a partir de uma cópia
void ShowPriorityQueue (std::priority_queue < PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long> > priority_queue)
{
    while (!priority_queue.empty())
    {
        std::cout << "priority_queue_copy : " 
        << " x " << Node::GetX(priority_queue.top().reference_key) 
        << "  y " << Node::GetY(priority_queue.top().reference_key) 
        << "  |  f " << priority_queue.top().f 
        << "  |  node_index " << priority_queue.top().reference_key 
        << "\n";
        priority_queue.pop();
    }
}


// !todo! Talvez seja melhor apenas colocar o valor novo duplicado ao invés de atualizar
std::priority_queue< PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long> >CopyPriorityQueueExcept (std::priority_queue <PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long>>priority_queue, long except_key)
{
    std::priority_queue < PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long> > new_priority_queue;
    while (!priority_queue.empty())
    {
        if(priority_queue.top().reference_key != except_key)
        {
            new_priority_queue.push(priority_queue.top());
        }
        priority_queue.pop();
    }
    return new_priority_queue;
}