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

void dijkstra(int s, int t, int n, int** G, int *dist, int *pais)
{
  // inicializa d e p
  for (int i = 0; i < t; i++)
  {
    dist[i] = 100000;
    pais[i]= -1;
    //cout << dist.at(i) << pais.at(i) <<endl;
  }
  //cout<< dist.size();
  // cria uma priority queue heapmin
  priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
  pq.push({0,s});
  // enquanto a priority queue nao estiver vazia
  while (!pq.empty())
  {
    // pega o primeiro elemento da priority queue
    pair<int,int> u = pq.top();
    //cout << "vizinho mais proximo foi " << u.second <<" com: " << u.first << endl;
    pq.pop();
    // se o elemento for o destino, termina
    if (u.second == t)
    	break;
    // para cada vizinho do elemento atual
    for (int j = 1; j < n; j++){
        if (G[u.second][j] != INT_MAX){
            if (dist[j] > u.first + G[u.second][j]){ // peso do vizinho j > peso do v atual + o caminho para j pelo vertice atual
                //cout<< "distancia atual segundo o grafo: "<<u.first <<". distancia do "<<u.second << " para "<<j<<": "<<G[u.second][j]<<endl;
                if(dist[j] == 100000) dist[j] = 0;
                //cout << dist[u.second]<< endl;
                dist[u.second] = u.first; // atualiza o peso do v atual 
                //cout << dist[u.second]<< endl;
                dist[j] = dist[u.second] + G[u.second][j]; // atualiza o peso do vizinho j 
                pais[j] = u.second; // atualiza pai de j
                //cout<< "vertice atual: "<<u.second <<". vizinho: "<< j << ". distancia pro vizinho: "<< dist[j] << ". distancia ate o momento: " << dist[u.second] << ". distancia no grafo[atual][vizinho]: " << G[u.second][j] << endl;
                //cout<< "de "<<pais[j] <<" a "<< j<<" peso: " << dist[j] <<endl;
                pq.push({dist[j], j});
                //cout << "vizinho de " << u.second << ": " << j << ", com a dist na heap: " << dist.at(j) << endl;
            }
        }
    }
  }
  return;
}


void solutions(bool saida, string nomeSaida, bool solucao, int s, int t, int** G, int n){
    int dist[t];
    int pais[t];
    //vector<int> *ptrDist = &dist;
    //vector<int> *ptrPais = &pais;
    dijkstra(s, t, n, G, &dist[0], &pais[0]);
    if (saida && solucao){
    // escreve a saida das distancias ordenadas num arquivo txt
        ofstream out;
        out.open(nomeSaida, ofstream::out);
        for (int i = s; i < t; i++)
        {
          if(i == s){
            out << "raiz " << i << endl;
          }else if (pais[i] == -1){
            out << "nao existe caminho" << endl;
          }else{
            out << "menor peso de " << pais[i] <<" a "<< i <<": "<< dist[i] << endl;
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
        for(int i = s; i < t; i++){
          if(dist[i] == 100000){
            continue;
          }else{
            sumCaminho += dist[i];
          }
        }
        out << "peso do menor caminho: "<< sumCaminho << endl;
        out.close();
        return;
    }
    if(solucao){
    // exibe no terminal as distancias ordenadas de cada aresta
      for(int i = s; i < t; i++){
        if(i == s){
          cout << "raiz " << i << endl;
        }else if (pais[i] == -1){
          cout << "nao existe caminho" << endl;
        }else{
          cout << "menor peso de " << pais[i] <<" a "<< i <<": "<< dist[i] << endl;
        }
      }
    return;
    }
    // exibe no terminal o peso do menor caminho
    int sumCaminho=0;
    for(int i = s; i < t; i++){
      if(dist[i] == 100000){
        continue;
      }else{
        sumCaminho += dist[i];
      }
    }
    
    cout << "peso do menor caminho: "<< sumCaminho << endl;
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
  int s = 1;
  int t = 999;
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
      n = qtd_ares+1;
      t = qtd_vert+1;
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
      s = stoi(argv[i+1]);
    }

    if (argv[i] == opFinal){
      final = true;
      t = stoi(argv[i+1]);
    }

    if (argv[i] == opSaida){
    	saida = true;
        nomeSaida = argv[i+1];
    }
    if(argv[i]== opSolucao){
        solucao = true;
    }
  }

  solutions(saida, nomeSaida, solucao, s, t, G, n);
  free(G);
  return 0;
}