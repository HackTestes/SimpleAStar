#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H


    // Implementações relacionadas com a lista de prioridade (OPEN)

    #include"AStarHeader.h"

    // Estrututura de propósito geral, que guarda apenas o necessário
    //  - Chave de referência: diz a qual objeto faz referência
    //  - f: calculado e utilizado para fazer o ordenamento da lista, mostra qual é o "melhor" nó
    // PriorityQueueTemplates
    template<typename KeyType, typename IntType>
    struct PriorityQueueContainer
    {
        IntType f; // f(n) = cost_g(n) + heuristic_h(n)
        KeyType reference_key; //chave única que faz referência a um item: nó, Sliding Puzzle...

        PriorityQueueContainer(long f, KeyType reference_key)
        {
            this->f = f;
            this->reference_key = reference_key;
        }
    };

    // essa classe é a que organiza a lista de prioridades
    // baseado no valor f de cada nó
    template<typename KeyType, typename IntType>
    class SortPriorityQueue
    {
        public:
            bool operator() (PriorityQueueContainer<KeyType, IntType> n1, PriorityQueueContainer<KeyType, IntType> n2)
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
    };

    // usa uma cópia da minha lista de prioridade
    // imprime a lista de prioridades a partir de uma cópia
    template<typename IntType>
    void ShowPriorityQueue
            (std::priority_queue < PriorityQueueContainer<IntType, IntType>, std::vector<PriorityQueueContainer<IntType, IntType>>, SortPriorityQueue<IntType, IntType> > priority_queue)
    {
        while (!priority_queue.empty())
        {
            std::cout << "priority_queue_copy : " 
            << " x " << Node<IntType>::GetX(priority_queue.top().reference_key) 
            << "  y " << Node<IntType>::GetY(priority_queue.top().reference_key) 
            << "  |  f " << priority_queue.top().f 
            << "  |  node_index " << priority_queue.top().reference_key 
            << "\n";
            priority_queue.pop();
        }
    }


    // !todo! Talvez seja melhor apenas colocar o valor novo duplicado ao invés de atualizar
    template<typename IntType>
    std::priority_queue< PriorityQueueContainer<IntType, IntType>, std::vector<PriorityQueueContainer<IntType, IntType>>, SortPriorityQueue<IntType, IntType> >
            CopyPriorityQueueExcept
                (std::priority_queue <PriorityQueueContainer<IntType, IntType>, std::vector<PriorityQueueContainer<IntType, IntType>>, SortPriorityQueue<IntType, IntType>>priority_queue, IntType except_key)
    {
        std::priority_queue < PriorityQueueContainer<IntType, IntType>, std::vector<PriorityQueueContainer<IntType, IntType>>, SortPriorityQueue<IntType, IntType> > new_priority_queue;
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

#endif