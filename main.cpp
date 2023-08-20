#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <limits.h>
#include <queue>
using namespace std;

///////////// a world contains diffrent galaxies with diffrent planets
class World
{
private:

    int adj[11][200][200]; // stores relation of planets, 11th galaxy is a galaxy between BG planets
    unsigned int number_of_node[11] = {0}; // number of planets in each galaxy
    string property[11][200][3]; // stores name, galaxyne, type
    string NameOfGalaxy[11]; // name of galaxies
    int path[11][200][200]; // stores smallest path planets
    string field[8]; // stores input after cleaning
    int n_planet, galaxy_count = 0; // galaxy counter
    int galaxy_id = 0; // determine galaxy id
    int temp_1, temp_2; // previos galaxy id and planet id
    int col; // number of planets to dijkstra algorithm
    string input;// stores input sentence
    bool find_galaxy = false, find_planet = false, check_galaxy = false;; // check galaxy or planet is fined or not
    bool prevent = false, last_on = false; // prevent: prevent from saving planet again
    queue<string> myQueue; // stores planets in order

public:
    ///////////// cleaning step
    void Analyse()
    {
        getline(cin, input);

        input.erase(remove(input.begin(), input.end(), ','), input.end());
        input.erase(remove(input.begin(), input.end(), '('), input.end());
        input.erase(remove(input.begin(), input.end(), ')'), input.end());
        input.erase(remove(input.begin(), input.end(), ':'), input.end());
        input.erase(remove(input.begin(), input.end(), '{'), input.end());
        input.erase(remove(input.begin(), input.end(), '}'), input.end());
        input.erase(remove(input.begin(), input.end(), '['), input.end());
        input.erase(remove(input.begin(), input.end(), ']'), input.end());
        input.erase(remove(input.begin(), input.end(), '-'), input.end());
        input.erase(remove(input.begin(), input.end(), '\''), input.end());
        input.erase(remove(input.begin(), input.end(), '>'), input.end());

        int lcs = 0; //letter counter , use for stemp
        int lcw = 0; //letter counter , use for wtemp
        int wrd = 0; //word counter

        string stemp; //sentences temp
        stemp = input;
        char wtemp[10]; //word temp
        while (true)
        {
            if ((stemp.size() + 1) == lcs || (stemp.size()+2) == lcs) // check current line is finished
            {

                break;

            }
            if (stemp[lcs] == ' ' || stemp[lcs] == '.' || stemp[lcs] == ':') // next word
            {
                lcs += 1;
                if (stemp[lcs] == ' ')
                {
                    lcs += 1;
                }
                
                wtemp[lcw] = '\0';

                lcw = 0;

                field[wrd] = wtemp;

                for (size_t i = 0; i < 10; i++) // make the char array empty
                {
                    wtemp[i] = '\0';
                }
                wrd ++;
            }

            wtemp[lcw] = stemp[lcs];
            if (stemp.size() == lcs)
            {
                field[wrd] = wtemp;
                break;
            }
            lcs ++;
            lcw ++;
        }
}
    ///////////// checking galaxy
    void galaxy_check(string galaxy)
    {
        if(galaxy_count == 0) // first time
        {
            NameOfGalaxy[0] = galaxy;
            galaxy_id = 0;
            find_galaxy = true;
            galaxy_count++;
        }
        for (size_t i = 0; i < galaxy_count; i++) // galaxy is available
        {
            if(NameOfGalaxy[i] == galaxy)
            {
                galaxy_id = i;
                find_galaxy = true;
                break;
            }
        }
        if(find_galaxy == false) // new galaxy after first time
        {
            NameOfGalaxy[galaxy_count] = galaxy;
            galaxy_count++;
        }        
    }
    ///////////// cheacking planet
    void planet_check(string planet)
    {
        int find;
        if(find_galaxy) 
        {
            find =  galaxy_id;
        }
        else 
        {
            find = galaxy_count - 1;
        }
        for (size_t i = 0; i < number_of_node[find]; i++) // galaxy is available
        {
            if(property[find][i][0] == planet)
            {
                find_planet = true;
                n_planet = i;
                break;
            }
        }
        if(!find_planet && !prevent) // stores new planet informations
        {
            property[find][number_of_node[find]][0] = planet; // set name
            property[find][number_of_node[find]][2] = field[6]; // set type
            property[find][number_of_node[find]][1] = field[7]; // set galaxy name
            number_of_node[find]++;
            
            if(field[6] == "BG") // stores BG planets
            {
                property[10][number_of_node[10]][0] = planet;
                property[10][number_of_node[10]][2] = field[6];
                property[10][number_of_node[10]][1] = field[7]; // set galaxy name
                number_of_node[10]++;
            }
        }
    }
    ///////////// cheacking planet id
    bool id_check(string id)
    {
        for (size_t i = 0; i < galaxy_count; i++)
        {
            for (size_t j = 0; j < number_of_node[i]; j++)
            {
                if(property[i][j][2] == id)
                {
                    return false;
                }
            }
        }
        return true;
    }
    ///////////// stores relations cost in adj
    void PlanetCost(int id_galaxy, int id_planet)
    {   
        long int cost = stoi(field[4].substr(4, field[4].size() - 4));

        if(property[temp_1][temp_2][2] == "BG" && property[id_galaxy][id_planet][2] == "BG" && id_galaxy != temp_1) // BG to BG in diffrent galaxy
        {
            adj[10][temp_1][temp_2] = cost;
            adj[10][temp_2][temp_1] = cost;
            last_on = true; // condition of dijkstra on BG planets
        }
        else if(property[temp_1][temp_2][2] == "BG" && property[id_galaxy][id_planet][2] == "BG" && id_galaxy == temp_1) // BG to BG in same galaxy
        {
            adj[id_galaxy][id_planet][temp_2] = cost;
            adj[id_galaxy][temp_2][id_planet] = cost;
            adj[10][id_planet][temp_2] = cost;
            adj[10][temp_2][id_planet] = cost;
        }
        else if(property[temp_1][temp_2][2] == "BG" && property[id_galaxy][id_planet][2] == "N_BG" && id_galaxy == temp_1) // BG to N_BG in same galaxy
        {
            adj[id_galaxy][id_planet][temp_2] = cost;
            adj[id_galaxy][temp_2][id_planet] = cost;
        }
        else if(property[temp_1][temp_2][2] == "N_BG" && property[id_galaxy][id_planet][2] == "N_BG" && id_galaxy == temp_1) // N_BG to N_BG in same galaxy
        {
            adj[id_galaxy][id_planet][temp_2] = cost;
            adj[id_galaxy][temp_2][id_planet] = cost;
        }
        else if(property[temp_1][temp_2][2] == "N_BG" && property[id_galaxy][id_planet][2] == "BG" && id_galaxy == temp_1) // N_BG to BG in same galaxy
        {
            adj[id_galaxy][id_planet][temp_2] = cost;
            adj[id_galaxy][temp_2][id_planet] = cost;
        }
    }
    ///////////// shows smallest path between planets of each galaxy
    void print()
    {
        for (size_t i = 0; i < 11; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                for (size_t k = 0; k < 3; k++)
                {
                    cout << path[i][j][k] << ' ';
                }
                cout << endl;
            }
            cout << "\n--------------------------------\n";
        }
    }
    ///////////// initial min distance
    int minDist(int dist[], bool visited[])
    {
        int min = INT_MAX, min_index;

        for (int v = 0; v < col; v++)
        {
            if(visited[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;
        }
        return min_index;
    } 
    ///////////// dijkstra algorithm
    void dijkstra(int graph[200][200], int src, int page) // graph of planets in a galaxy, source planet, id of galaxy
    {
        int dist[col]; // stores smallest path planets from source planet
    
        bool visited[col]; // stores planets that they have smallest distance from a src planet to them in a boolean

        for (int i = 0; i < col; i++) // set inf to distance of planets and set false to visited 
            dist[i] = INT_MAX, visited[i] = false;
    
        dist[src] = 0; // source set 0 distance from itself
    
        for (int count = 0; count < col - 1; count++) // find smallest path for all planets
        {
            int u = minDist(dist, visited); // chooses the smallest distance planet
    
            
            visited[u] = true; // set true to visited planet

            for (int v = 0; v < col; v++) // update values of adj array
    
                if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                {
                    dist[v] = dist[u] + graph[u][v]; // choose smaller path
                }
        }
        for (size_t j = 0; j < number_of_node[page]; j++) // set smallest path into path array
        {
            path[page][src][j]  = dist[j];
        }
    }
    ///////////// updating smallest path
    void  updateCost()
    {
        int temp_array[200][200] = {0};
        for (size_t i = 0; i < galaxy_count; i++) // put elements of adj into temp_araay to dijkstra operation
        {
            for (size_t j = 0; j < number_of_node[i]; j++)
            {
                for (size_t k = 0; k < number_of_node[i]; k++)
                {
                    temp_array[j][k] = adj[i][j][k];
                }
            }
            col = number_of_node[i];
            for (size_t r = 0; r < number_of_node[i]; r++)
            {
                dijkstra(temp_array, r, i); // dijkstra on all planets of all galaxies
            }
        }
        if(last_on)
        {
            for (size_t j = 0; j < number_of_node[10]; j++) // dijkstra on BG planets
            {
                for (size_t k = 0; k < number_of_node[10]; k++)
                {
                    temp_array[j][k] = adj[10][j][k];
                }
            }
            col = number_of_node[10];
            for (size_t r = 0; r < number_of_node[10]; r++)
            {
                dijkstra(temp_array, r, 10);
            }
        }
    }
    ///////////// find smallest path from BG planet to N_BG planet with diffrent galaxies
    void BG2NBG_DIF(string start_name, string end_name, int end_id, int end_id_galaxy)
    {
        int min_cost = INT16_MAX, n_planet_last[2], sum_of_cost = 0, n_planet_galaxy;

        myQueue.push(start_name); // push planets name in order start-end into queue

        for (size_t i = 0; i < number_of_node[10]; i++) // find BG id in last page or BG galaxies
        {
            if(property[10][i][0] == start_name)
            {
                n_planet_last[0] = i;
            }
        }
        for (size_t i = 0; i < number_of_node[10]; i++)
        {
            if(property[10][i][1] == field[3]) // find end planet's id and smallest path in BG planets
            {
                if(path[10][i][n_planet_last[0]] < min_cost && path[10][i][n_planet_last[0]] != 0)
                {
                    min_cost = path[10][i][n_planet_last[0]];
                    n_planet_last[1] = i;
                }
            }
        }
        myQueue.push(property[10][n_planet_last[1]][0]);
        sum_of_cost += path[10][n_planet_last[0]][n_planet_last[1]]; // sum costs

        for (size_t i = 0; i < number_of_node[end_id_galaxy]; i++) // find end planet's id
        {
            if(myQueue.back() == property[end_id_galaxy][i][0])
            {
                n_planet_galaxy = i;
            }
        }
        myQueue.push(end_name);
        sum_of_cost += path[end_id_galaxy][n_planet_galaxy][end_id];

        while(!myQueue.empty()) // start-end path
        {
            cout << myQueue.front() << "->";
            myQueue.pop();
        }
        cout << ":" << sum_of_cost << endl;        
    }
    ///////////// find smallest path from N_BG planet to BG planet with diffrent galaxies
    void NBG2BG_DIF(int start_id, int start_id_galaxy)
    {
        int min_cost = INT16_MAX, n_planet_galaxy[2], sum_of_cost = 0;
        string name;
        
        myQueue.push(property[start_id_galaxy][start_id][0]); // push start planet's name
        for (size_t i = 0; i < number_of_node[start_id_galaxy]; i++)
        {
            if(property[start_id_galaxy][i][2] == "BG" && path[start_id_galaxy][start_id][i] < min_cost) // choose BG planet that has smallest path with start planet
            {
                min_cost = path[start_id_galaxy][start_id][i];
                name = property[start_id_galaxy][i][0];
            }
        }
        sum_of_cost += min_cost;
        myQueue.push(name);

        for (size_t i = 0; i < number_of_node[10]; i++)
        {
            if(property[10][i][0] == name) // find id of BG planet that has same galaxy with start planet
            {
                n_planet_galaxy[0] = i;
            }
            if(property[10][i][0] == field[4]) // find id of end planet that is BG planet
            {
                n_planet_galaxy[1] = i;
            }
        }
        
        sum_of_cost += path[10][n_planet_galaxy[0]][n_planet_galaxy[1]];
        myQueue.push(field[4]);

        while(!myQueue.empty()) // start-end path
        {
            cout << myQueue.front() << "->";
            myQueue.pop();
        }
        cout << ":" << sum_of_cost << endl;
    }
    ///////////// find smallest path from N_BG planet to N_BG planet with diffrent galaxies
    void NBG2NBG_DIF(int start_id_galaxy, int end_id_galaxy, int start_id)
    {
        int min_cost = INT16_MAX, n_planet_galaxy[2], sum_of_cost = 0;
        string name;
        
        myQueue.push(property[start_id_galaxy][start_id][0]); // push start planet's name
        for (size_t i = 0; i < number_of_node[start_id_galaxy]; i++) // find smallest path between start planet and a BG planet with same galaxy
        {
            if(property[start_id_galaxy][i][2] == "BG" && path[start_id_galaxy][start_id][i] < min_cost)
            {
                min_cost = path[start_id_galaxy][start_id][i];
                name = property[start_id_galaxy][i][0];
            }
        }

        sum_of_cost += min_cost;
        myQueue.push(name);
        min_cost = INT16_MAX;

        for (size_t i = 0; i < number_of_node[10]; i++)
        {
            if(property[10][i][0] == name) // find id of BG planet that has same galaxy with start planet
            {
                n_planet_galaxy[0] = i;
            }
        }
        for (size_t i = 0; i < number_of_node[10]; i++)
        {
            if(property[10][i][1] == field[3] && path[10][i][n_planet_galaxy[0]] < min_cost) // find id of end planet and smallest path
            {
                n_planet_galaxy[1] = i;
                min_cost = path[10][i][n_planet_galaxy[0]];
            }
        }

        sum_of_cost += path[10][n_planet_galaxy[0]][n_planet_galaxy[1]]; // path BG start galaxy to BG end galaxy
        myQueue.push(property[10][n_planet_galaxy[1]][0]);

        for (size_t i = 0; i < number_of_node[end_id_galaxy]; i++) // find end planet's id and galaxy's id
        {
            if(property[end_id_galaxy][i][0] == property[10][n_planet_galaxy[1]][0])
            {
                n_planet_galaxy[1] = i;
            }
            if(property[end_id_galaxy][i][0] == field[4])
            {
                n_planet_galaxy[0] = i;
            }
        }

        myQueue.push(field[4]);
        sum_of_cost += path[end_id_galaxy][n_planet_galaxy[1]][n_planet_galaxy[0]];
        
        while(!myQueue.empty()) // start-end path
        {
            cout << myQueue.front() << "->";
            myQueue.pop();
        }
        cout << ":" << sum_of_cost << endl;
    }
    ///////////// find smallest path from BG planet to BG planet with diffrent galaxies
    void BG2BG_DIF(int start_id_galaxy, int end_id_galaxy, int start_id, int end_id)
    {
        int n_planet_galaxy;
        for (size_t i = 0; i < number_of_node[10]; i++) // find end planet's id 
        {
            if(property[10][i][0] == field[4])
            {
                n_planet_galaxy = i;
            }
        }
        cout << property[start_id_galaxy][start_id][0] << "->" << property[end_id_galaxy][end_id][0]<< ":"
                                                    << path[10][start_id_galaxy][n_planet_galaxy] << endl; // smallest path in order to visited planets
    }
    ///////////// find smallest in same galaxy's planets
    void SAME_GALAXY(int start_id_galaxy, int start_id, int end_id)
    {
        cout << property[start_id_galaxy][start_id][0] << "->" << property[start_id_galaxy][end_id][0] << ":" 
                                                         << path[start_id_galaxy][start_id][end_id] << endl; // smallest path in order to visited planets
    }
    ///////////// run all functions and algorithms
    void run()
    {
        while (field[0] != "exit") // enter "end" to exit form program
        {
            field[6] = "";
            Analyse();

            if(field[0] == "update") // enter "update" to update costs
                updateCost();

            if(field[0] == "CREATE") // create planets and their relation
            {
                if(field[2] == "Node")
                {
                    if(id_check(field[4])) // ckeck to have unique id
                    {
                        galaxy_check(field[7]); // check planet's galaxy
                        planet_check(field[1]); // check planet
                        find_galaxy = false;
                        find_planet =false;
                    }
                }
                else if(field[3] == "ROAD") // making roads between planets
                {
                    for (size_t i = 0; i < galaxy_count; i++) // check start galaxy
                    {
                        if(NameOfGalaxy[i] == field[2]) 
                        {
                            galaxy_id = i;
                            check_galaxy = true;
                            find_galaxy = true;
                            break;
                        }
                    }
                    if(check_galaxy)
                    {
                        planet_check(field[1]); // check start planet
                        if(find_planet)
                        {
                            find_planet = false;

                            if(field[6] == "") // end galaxy does not exist
                            {
                                temp_1 = galaxy_id;
                                temp_2 = n_planet;
                                prevent = true;
                                planet_check(field[5]); // check end planet
                                prevent = false;
                                
                                if(find_planet) // if end planet exists, store relations cost
                                {
                                    /////////// BG to BG same
                                    if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        PlanetCost(galaxy_id, n_planet);
                                    }
                                    /////////// BG to N_BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        PlanetCost(galaxy_id, n_planet);
                                    }
                                    /////////// N_BG to N_BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        PlanetCost(galaxy_id, n_planet);
                                    }
                                    /////////// N_BG to BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        PlanetCost(galaxy_id, n_planet);
                                    }
                                    //------------------------------------ invalid relation
                                    /////////// BG to BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        cout << "invalild relation !!!" << endl; // exception
                                    }
                                    /////////// N_BG to N_BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        cout << "invalid relation !!!" << endl;
                                    }
                                    /////////// N_BG to BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        cout << "invalid relation !!!" << endl;
                                    }
                                    /////////// BG to N_BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        cout << "invalid relation !!!" << endl;
                                    }
                                    //------------------------------------ invalid relation
                                }
                                if(!find_planet) // not find
                                {
                                   if(field[6] == "")
                                   {
                                        cout << "invalid galaxy name or not found !!!" << endl; // exception
                                   }
                                }
                            }
                            else if(field[6] != "") // end galaxy exists
                            {
                                find_galaxy = false;
                                temp_1 = galaxy_id; // store galaxy id before change 
                                temp_2 = n_planet; // store planet id before change 
                                galaxy_check(field[6]); // check end galaxy
                                
                                if(find_galaxy)
                                {
                                    find_planet = false;
                                    planet_check(field[5]); // check end planet
                                    find_galaxy = false;

                                    if(find_planet) // if end planet exists, store relations cost
                                    {
                                        /////////// BG to BG dif
                                        if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "BG")
                                        {
                                            PlanetCost(galaxy_id, n_planet);
                                        }
                                        /////////// BG to BG same
                                        else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "BG")
                                        {
                                            PlanetCost(galaxy_id, n_planet);
                                        }
                                        /////////// N_BG to N_BG same
                                        else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                        {
                                            PlanetCost(galaxy_id, n_planet);
                                        }
                                        /////////// N_BG to BG same
                                        else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "BG")
                                        {
                                            PlanetCost(galaxy_id, n_planet);
                                        }
                                        /////////// BG to N_BG same
                                        else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                        {
                                            PlanetCost(galaxy_id, n_planet);
                                        }
                                        //------------------------------------ invalid
                                        /////////// N_BG to N_BG dif
                                        else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                        {
                                            cout << "invalid relation !!!" << endl; // exceptions
                                        }
                                        /////////// N_BG to BG dif
                                        else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "BG")
                                        {
                                            cout << "invalid relation !!!" << endl;
                                        }
                                        /////////// BG to N_BG dif
                                        else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                        {
                                            cout << "invalid relation !!!" << endl;
                                        }
                                        //------------------------------------ invalid
                                    }
                                }
                            }
                        }
                    
                    }
                }
                else // wrong input
                    cout << "instruction is wrong ..." << endl; //excepion
            }
            else if(field[0] == "FIND") // find smallest path between planets
            {
                for (size_t i = 0; i < galaxy_count; i++) // check start galaxy
                {
                    if(NameOfGalaxy[i] == field[1]) 
                    {
                        galaxy_id = i;
                        check_galaxy = true;
                        find_galaxy = true;
                        break;
                    }
                }
                if(check_galaxy)
                {
                    planet_check(field[2]); // check start planet
                    if(find_planet)
                    {
                        find_planet = false;
                        
                        if(field[4] == "") // end planet does not exist
                        {
                            cout << "invalid start planet !!!" << endl;
                        }
                        else if(field[4] != "")
                        {
                            find_galaxy = false;
                            temp_1 = galaxy_id;
                            temp_2 = n_planet;
                            galaxy_check(field[3]); // check end galaxy

                            if(find_galaxy)
                            {
                                find_planet = false;
                                planet_check(field[4]); // check end planet
                                find_galaxy = false;
                                if(find_planet) // if end planet exists, find smallest path
                                {
                                    /////////// BG to BG dif
                                    if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        BG2BG_DIF(temp_1, galaxy_id, temp_2, n_planet);
                                    }
                                    /////////// BG to BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        SAME_GALAXY(temp_1, temp_2, n_planet);
                                    }
                                    /////////// N_BG to N_BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        SAME_GALAXY(temp_1, temp_2, n_planet);
                                    }
                                    /////////// N_BG to N_BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        NBG2NBG_DIF(temp_1, galaxy_id, temp_2);
                                    }
                                    //////////// N_BG to BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        NBG2BG_DIF(temp_2, temp_1);
                                    }
                                    //////////// BG to N_BG dif
                                    else if(galaxy_id != temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        BG2NBG_DIF(property[temp_1][temp_2][0], property[galaxy_id][n_planet][0], n_planet, galaxy_id);
                                    }
                                    /////////// N_BG to BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "N_BG" && property[galaxy_id][n_planet][2] == "BG")
                                    {
                                        SAME_GALAXY(temp_1, temp_2, n_planet);
                                    }
                                    /////////// BG to N_BG same
                                    else if(galaxy_id == temp_1 && property[temp_1][temp_2][2] == "BG" && property[galaxy_id][n_planet][2] == "N_BG")
                                    {
                                        SAME_GALAXY(temp_1, temp_2, n_planet);
                                    }
                                }
                                if(!find_planet)
                                {

                                }
                            }
                        }
                    }
                }          
            }
        }
    }
};


int main()
{

    World object;

    object.run();

    // you can uncomment bellow lines to see smallest path between planets of each galaxy
    // cout << endl;
    // object.print();

    return 0;
}