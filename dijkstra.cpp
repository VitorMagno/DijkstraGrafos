#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<queue>
#include<algorithm>
#include<climits>
#include<array>

// algoritmo de dijkstra com otimizacao de caminho minimo
// usando priority queue


using namespace std;

int dijkstra(int verticeInicial, int verticeFinal, int n, int** G, int *dist, int *pais)
{
  // inicializa d e p
  for (int i = verticeInicial; i <= verticeFinal; i++)
  {
    dist[i] = 100000;
    pais[i]= -1;
    
  }
  
  // cria uma priority queue heapmin
  priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> fila;
  fila.push({0,verticeInicial});
  // enquanto a priority queue nao estiver vazia
  while (!fila.empty())
  {
    // pega o primeiro elemento da priority queue
    pair<int,int> topoDaFila = fila.top();
    fila.pop();
    int verticeAtual = topoDaFila.second;
    int pesoDoVerticeAtual = topoDaFila.first;
    // se o elemento for o destino, termina
    if (verticeAtual == verticeFinal)
    	break;
    // para cada vizinho do elemento atual
    for (int vizinho = 1; vizinho <= verticeFinal ; vizinho++){  
      if (dist[vizinho] > pesoDoVerticeAtual + G[verticeAtual][vizinho] && G[verticeAtual][vizinho] != INT_MAX){ 
        if(dist[vizinho] == 100000) dist[vizinho] = 0;
        dist[verticeAtual] = pesoDoVerticeAtual; 
        dist[vizinho] = dist[verticeAtual] + G[verticeAtual][vizinho];
        pais[vizinho] = verticeAtual; 
        //cout << "atualizando a distancia de "<< vizinho <<" para " << dist[vizinho] << endl;
        fila.push({dist[vizinho], vizinho});
      }
    }
  }
  return dist[verticeFinal];
}


void solutions(bool saida, string nomeSaida, bool solucao, int verticeInicial, int verticeFinal, int** G, int n){
    int dist[n];
    int pais[n];
    int custoFinal;
    custoFinal = dijkstra(verticeInicial, verticeFinal, n, G, &dist[0], &pais[0]);
    if (saida && solucao){
    // escreve a saida das distancias ordenadas num arquivo txt
        ofstream out;
        out.open(nomeSaida, ofstream::out);
        for (int i = verticeInicial; i <= verticeFinal; i++)
        {
          if(i == verticeInicial){
            out << "Exibindo todos os vertices e distancias atingiveis por " << i << endl;
          }else if (pais[i] == -1){
            out << "Nao existe caminho partindo de " << verticeInicial << " que atinja " << i << endl;
          }else{
            out << "De " << verticeInicial <<" -> "<< i <<" = "<< dist[i]  << " | pai = " << pais[i] << endl;
          }
        }
        out.close();
        return;
    }
    if(saida){
    // escreve o peso do menor caminho num arquivo txt
        ofstream out;
        out.open(nomeSaida, ofstream::out);
        int sumCaminho=0;
        for(int i = verticeInicial; i <= verticeFinal; i++){
          if(dist[i] == 100000){
            continue;
          }else{
            sumCaminho = dist[i];
          }
        }
        out << "Peso do menor caminho do vertice " <<verticeInicial << " ate o " << verticeFinal << " = "<< sumCaminho << " | Pai do vertice final = " << pais[verticeFinal] <<  endl;
        out.close();
        return;
    }
    if(solucao){
    // exibe no terminal as distancias ordenadas de cada aresta
      for(int i = verticeInicial; i <= verticeFinal; i++){
        if(i == verticeInicial){
          cout << "Exibindo todos os vertices e distancias atingiveis por " << i << endl;
        }else if (pais[i] == -1){
          cout << "Nao existe caminho partindo de " << verticeInicial << " que atinja " << i << endl;
        }else{
          cout << "De " << verticeInicial <<" -> "<< i <<" = "<< dist[i]  << " | pai = " << pais[i] << endl;
        }
      }
    return;
    }
    // exibe no terminal o peso do menor caminho
    int sumCaminho=0;
    for(int i = verticeInicial; i <= verticeFinal; i++){
      if(dist[i] == 100000){
        continue;
      }else{
        //cout<< "v: " << i << " distancia " << dist[i]<<endl;
        sumCaminho = dist[i];
      }
    }
    
    cout << "Peso do menor caminho do vertice " <<verticeInicial << " ate o " << verticeFinal << " = "<< sumCaminho << " | Pai do vertice final = " << pais[verticeFinal] <<  endl;
    return;
}

int main(int argc, char const *argv[]){

  const int BUFFER_LENGTH = 8;      // Tamanho do buffer de leitura das linhas dos arquivos de entrada.
  char buffer[BUFFER_LENGTH];       // Buffer de leitura mencionado.

  string opReadFile = "-f";
  bool read = false;
  string opHelp = "-h";
  string opInicial = "-i";
  string opSolucao = "-s";
  bool solucao = false;
  bool inicial = false;
  string opFinal = "-l";
  bool final = false;
  string opSaida = "-o";
  string nomeSaida;
  bool saida = false;
  int verticeInicial = 1;
  int verticeFinal = 999;
  int** G;
  int n;

  for (int i = 1; i < argc; i++){
    if (argv[i] == opReadFile){
      // Input File Stream pra abrir o arquivo de entrada.
      ifstream file_input;

      // Tenta abrir arquivo informado.
      file_input = ifstream(argv[i+1], ios::binary);

      // Encerra se houve erro ao abrir arquivo com a lista de vitórias.
      if (!file_input.is_open()) {
        cout << "Erro ao abrir arquivo " << argv[i+1] << '\n';
        return 1;
      }
      
      // Pega primeira linha para dentro do buffer.
      file_input.getline(buffer, BUFFER_LENGTH);
      
      int qtd_vert = atoi(&buffer[0]);  // Ordem do grafo
      int qtd_ares = atoi(&buffer[2]);  // Tamanho do grafo
      
      // Usamos +1 pra acessar os vértices pelos seus números. Deixaremos a primeira linha e primeira colunas sem usar.
      G = new int* [qtd_ares+1];
      n = qtd_vert + 1;
      verticeFinal = n - 1;
      for (int y = 0; y <qtd_ares+1; y++){
        G[y] = new int [qtd_ares+1];
      }
      
      // Inicializa pesos no máximo.
      for (int j=0; j < qtd_ares+1; j++)
      {
        for (int i=0; i < qtd_ares+1; i++)
        	G[i][j] = INT_MAX;
            //cout<<G[i][j]<<endl;
      }
      
      // Lê até atingir o final do arquivo.
      while(!file_input.fail())
      {
        // Puxa linha pro buffer
        file_input.getline(buffer, BUFFER_LENGTH);
        if(file_input.fail()) break;
        // Lê e converte os valores desejados.
        int vert_orig = atoi(&buffer[0]);
        int vert_dest = atoi(&buffer[2]);
        int weight = atoi(&buffer[4]);
        //cout<< vert_orig << " " << vert_dest << " " << weight << endl;
        
        // Atualiza o valor do peso da aresta saindo de vert_orig e indo para vert_dest.
        // cout<< "origem: " << vert_orig << "destino: "<< vert_dest << "peso: "<< G[vert_orig][vert_dest] << endl; == sem resposta
        G[vert_orig][vert_dest] = weight;
      }
    }

    if (argv[i] == opHelp){
      // descreve as entradas e o que cada parametro inicial pode fazer
      cout << "Algoritmo de Dijkstra utilizando heapmin" << endl;
      cout << "use -f <arquivo> para entrada de um grafo utilizando uma matriz de incidencia"<< endl;
      cout << "use -o <arquivo> para redirecionar saida para \"arquivo\""<< endl;
      cout << "use -s para exibir solucao ordenada"<< endl;
      cout << "use -i <vertInicial> apos -o ou apos <arquivo> para indicar vertice inicial"<< endl;
      cout << "use -l <vertFinal> apos -o ou apos <arquivo> para indicar vertice final"<< endl;
      return 0;
    }

    if (argv[i] == opInicial){
      inicial = true;
      verticeInicial = stoi(argv[i+1]);
    }

    if (argv[i] == opFinal){
      final = true;
      verticeFinal = stoi(argv[i+1]);
    }

    if (argv[i] == opSaida){
    	saida = true;
      nomeSaida = argv[i+1];
    }
    if(argv[i]== opSolucao){
      solucao = true;
    }
  }

  solutions(saida, nomeSaida, solucao, verticeInicial, verticeFinal, G, n);
  free(G);
  return 0;
}