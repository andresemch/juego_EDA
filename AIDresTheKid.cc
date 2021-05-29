#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME DresTheKid

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  const vector<NutrientType> nutrients = { Carbohydrate, Protein, Lipid };
  vector<int> my_workers_ids = workers(me());
  vector<int> my_soldiers_ids = soldiers(me());
  vector<int> my_queens_ids = queens(me());


  // Default direction to be used during all match.
  Dir default_dir = Up;
  

  // Returns true if winning.
  // bool winning() {
  //   for (int pl = 0; pl < num_players(); ++pl)
  //     if (pl != me() and score(me()) <= score(pl))
  //       return false;
  //   return true;
  // }

  bool bfs_check(Ant& ant_bfs, Cell cell_bfs) {
    if (ant_bfs.type == Worker) {

      if (cell_bfs.id == -1 and cell_bfs.bonus != None) return true;

    } else if (ant_bfs.type == Soldier) {

      if (cell_bfs.id != -1 and ant(cell_bfs.id).player != me() and ant(cell_bfs.id).type == Worker) return true;

    } else if (ant_bfs.type == Queen) {

      if ((cell_bfs.id == -1 and cell_bfs.bonus != None) or (cell_bfs.id != -1 and ant(cell_bfs.id).player != me() and ant(cell_bfs.id).type != Queen)) return true;

    }
    return false;
  }

  void movements_bfs(vector<int>& my_ants_ids, vector<vector<bool>>& available_pos, int iteration) {
    Ant ant_bfs = ant(my_ants_ids[iteration]);
      
    queue <pair <Pos,Dir> > queue_bonus;
    bool trobat = false;
    vector<vector<bool>> route(board_rows(),vector<bool>(board_cols(),false));
    for (int j = 0; j < 4 and not trobat; ++j) {
      int xx = ant_bfs.pos.i + x[j];
      int yy = ant_bfs.pos.j + y[j];
      if (pos_ok(xx,yy)) {
        Cell cell_act = cell(xx,yy);
        if (bfs_check(ant_bfs,cell_act) and available_pos[xx][yy]) {
          trobat = true;
          Dir direc = Up;
          if (j == 0) direc = Up;
          else if (j == 1) direc = Right;
          else if (j == 2) direc = Down;
          else if (j == 3) direc = Left;
          available_pos[xx][yy] = false;
          move(my_ants_ids[iteration], direc);
        }
        else if (cell_act.id == -1 and cell_act.type != Water and !route[xx][yy] and available_pos[xx][yy]) {
          route[xx][yy] = true;
          Pos pos_xx_yy(xx,yy);
          Dir direc = Up;
          if (j == 0) direc = Up;
          else if (j == 1) direc = Right;
          else if (j == 2) direc = Down;
          else if (j == 3) direc = Left;
          queue_bonus.push(make_pair(pos_xx_yy, direc));
        }  
      }
    }

    while (not queue_bonus.empty() and not trobat) {
      Pos pos_act = queue_bonus.front().first;
      Dir dir_act = queue_bonus.front().second;
      queue_bonus.pop();
      for (int j=0; j < 4 and not trobat; ++j) {
        int xx = pos_act.i + x[j];
        int yy = pos_act.j + y[j];
        if (pos_ok(xx,yy) and !route[xx][yy]) {
          Cell cell_act = cell(xx,yy);
          if (bfs_check(ant_bfs,cell_act)) {
            trobat = true;
            //Check current position of ant
            Pos real_pos = ant_bfs.pos + dir_act;
            available_pos[real_pos.i][real_pos.j] = false;
            move(my_ants_ids[iteration],dir_act);
          } 
          else if (cell_act.type != Water) {
            route[xx][yy] = true;
            Pos pos_xx_yy(xx,yy);
            queue_bonus.push(make_pair(pos_xx_yy,dir_act));
          }
        }
      }
    }
  }

  void movement_bfs_worker(vector<int>& my_ants_ids, vector<vector<bool>>& available_pos, vector<vector<bool>>& queen_near, int iteration) {
    Ant ant_bfs = ant(my_ants_ids[iteration]);
    queue <pair <Pos,Dir> > queue_bonus;
    bool trobat = false;
    vector<vector<bool>> route(board_rows(),vector<bool>(board_cols(),false));
    for (int j = 0; j < 4 and not trobat; ++j) {
      int xx = ant_bfs.pos.i + x[j];
      int yy = ant_bfs.pos.j + y[j];
      if (pos_ok(xx,yy)) {
        Cell cell_act = cell(xx,yy);
        if (bfs_check(ant_bfs,cell_act) and available_pos[xx][yy] and !queen_near[xx][yy]) {
          trobat = true;
          Dir direc = Up;
          if (j == 0) direc = Up;
          else if (j == 1) direc = Right;
          else if (j == 2) direc = Down;
          else if (j == 3) direc = Left;
          available_pos[xx][yy] = false;
          move(my_ants_ids[iteration], direc);
        }
        else if (cell_act.id == -1 and cell_act.type != Water and !route[xx][yy] and available_pos[xx][yy]) {
          route[xx][yy] = true;
          Pos pos_xx_yy(xx,yy);
          Dir direc = Up;
          if (j == 0) direc = Up;
          else if (j == 1) direc = Right;
          else if (j == 2) direc = Down;
          else if (j == 3) direc = Left;
          queue_bonus.push(make_pair(pos_xx_yy, direc));
        }  
      }
    }
    while (not queue_bonus.empty() and not trobat) {
      Pos pos_act = queue_bonus.front().first;
      Dir dir_act = queue_bonus.front().second;
      queue_bonus.pop();
      for (int j=0; j < 4 and not trobat; ++j) {
        int xx = pos_act.i + x[j];
        int yy = pos_act.j + y[j];
        if (pos_ok(xx,yy) and !route[xx][yy]) {
          Cell cell_act = cell(xx,yy);
          if (bfs_check(ant_bfs,cell_act) and !queen_near[xx][yy]) {
            trobat = true;
            //Check current position of ant
            Pos real_pos = ant_bfs.pos + dir_act;
            available_pos[real_pos.i][real_pos.j] = false;
            move(my_ants_ids[iteration],dir_act);
          } 
          else if (cell_act.type != Water) {
            route[xx][yy] = true;
            Pos pos_xx_yy(xx,yy);
            queue_bonus.push(make_pair(pos_xx_yy,dir_act));
          }
        }
      }
    }
  }

  void movement_bfs_leave(vector<int>& my_ants_ids, vector<vector<bool>>& available_pos, vector<vector<bool>>& queen_adjacent, int iteration) {
    Ant ant_bfs = ant(my_ants_ids[iteration]);
    queue <pair <Pos,Dir> > queue_bonus;
    bool trobat = false;
    vector<vector<bool>> route(board_rows(),vector<bool>(board_cols(),false));
    for (int j = 0; j < 4 and not trobat; ++j) {
      int xx = ant_bfs.pos.i + x[j];
      int yy = ant_bfs.pos.j + y[j];
      if (pos_ok(xx,yy)) {
        Cell cell_act = cell(xx,yy);
        if (cell_act.id == -1 and queen_adjacent[xx][yy] and available_pos[xx][yy]) {
          trobat = true;
          Dir direc = Up;
          if (j == 0) direc = Up;
          else if (j == 1) direc = Right;
          else if (j == 2) direc = Down;
          else if (j == 3) direc = Left;
          available_pos[xx][yy] = false;
          move(my_ants_ids[iteration], direc);
        }
        else if (cell_act.id == -1 and cell_act.type != Water and !route[xx][yy] and available_pos[xx][yy]) {
          route[xx][yy] = true;
          Pos pos_xx_yy(xx,yy);
          Dir direc = Up;
          if (j == 0) direc = Up;
          else if (j == 1) direc = Right;
          else if (j == 2) direc = Down;
          else if (j == 3) direc = Left;
          queue_bonus.push(make_pair(pos_xx_yy, direc));
        }  
      }
    }

    while (not queue_bonus.empty() and not trobat) {
      Pos pos_act = queue_bonus.front().first;
      Dir dir_act = queue_bonus.front().second;
      queue_bonus.pop();
      for (int j=0; j < 4 and not trobat; ++j) {
        int xx = pos_act.i + x[j];
        int yy = pos_act.j + y[j];
        if (pos_ok(xx,yy) and !route[xx][yy]) {
          Cell cell_act = cell(xx,yy);
          if (cell_act.id == -1 and queen_adjacent[xx][yy]) {
            trobat = true;
            //Check current position of ant
            Pos real_pos = ant_bfs.pos + dir_act;
            available_pos[real_pos.i][real_pos.j] = false;
            move(my_ants_ids[iteration],dir_act);
          } 
          else if (cell_act.type != Water) {
            route[xx][yy] = true;
            Pos pos_xx_yy(xx,yy);
            queue_bonus.push(make_pair(pos_xx_yy,dir_act));
          }
        }
      }
    }
  }


  


  int x[4] = {-1,0,1,0};
  int y[4] = {0,1,0,-1};

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

/**
 * Classes to use functions from:
 * State.hh -> board state
 * Action.hh -> commands
 * AIDemo.cc -> example of code
 * Structs.hh -> data structures
 * Random.hh -> random generation utilities
 * Settings.hh -> settings
 * Player.hh -> me() method
 */

    vector<int> my_workers_ids = workers(me());
    vector<int> my_soldiers_ids = soldiers(me());
    vector<int> my_queens_ids = queens(me());

    vector<vector<bool>> available_pos(board_rows(),vector<bool>(board_cols(),true));
    vector<vector<bool>> queen_adjacent(board_rows(),vector<bool>(board_cols(),false));
    vector<vector<bool>> queen_near(board_rows(),vector<bool>(board_cols(),false));

    //Queen
    for (int i=0; i < int(my_queens_ids.size()); ++i) {
      Ant queen = ant(my_queens_ids[i]);
      //cerr << "++++++++++++++++++++++++++REINA " << queen.id << endl;
      bool puedo_worker = (queen.reserve[0] >= worker_carbo() and queen.reserve[1] >= worker_prote() and queen.reserve[2] >= worker_lipid());
      bool puedo_soldier = (queen.reserve[0] >= soldier_carbo() and queen.reserve[1] >= soldier_prote() and queen.reserve[2] >= soldier_lipid());

      if (round()%queen_period() == 0) {

        movements_bfs(my_queens_ids, available_pos, i);
      } else if (int(my_soldiers_ids.size()) <= num_ini_soldiers() and puedo_soldier) {
        for (int j=0; j < 4; ++j) {
          int xx = queen.pos.i + x[j];
          int yy = queen.pos.j + y[j];
          if (pos_ok(xx,yy)) {
            Cell c = cell(xx,yy);
            Dir direc = Up;
            if (j == 0) direc = Up;
            else if (j == 1) direc = Right;
            else if (j == 2) direc = Down;
            else if (j == 3) direc = Left;
            if (c.id == -1) lay(my_queens_ids[i], direc, Soldier);
          }
        }
        //movements_bfs(my_queens_ids, available_pos, i);
      } else if (int(my_workers_ids.size()) <= (num_ini_workers()*3/4) and puedo_worker) {
        for (int j=0; j < 4; ++j) {
          int xx = queen.pos.i + x[j];
          int yy = queen.pos.j + y[j];
          if (pos_ok(xx,yy)) {
            Cell c = cell(xx,yy);
            Dir direc = Up;
            if (j == 0) direc = Up;
            else if (j == 1) direc = Right;
            else if (j == 2) direc = Down;
            else if (j == 3) direc = Left;
            if (c.id == -1) lay(my_queens_ids[i], direc, Worker);
          }
        }
        // movements_bfs(my_queens_ids, available_pos, i);
      }
      for (int j=0; j < 4; ++j) {
        int xx = queen.pos.i + x[j];
        int yy = queen.pos.j + y[j];
        if (pos_ok(xx,yy)) queen_adjacent[xx][yy] = true;
      }
      for (int j=-4; j <= 4; ++j) {
        for (int k=-4; k <= 4; ++k) {
          int xx = queen.pos.i + j;
          int yy = queen.pos.j + k;
          if (pos_ok(xx,yy)) queen_near[xx][yy] = true;
        }
      }
    }

    //Worker
    for (int i=0; i < int(my_workers_ids.size()); ++i) {

      Ant worker = ant(my_workers_ids[i]);
      //cerr << "++++++++++++++++++++++++++WORKER " << worker.id << endl;
      if (cell(worker.pos).bonus != None and worker.bonus == None and !queen_adjacent[worker.pos.i][worker.pos.j] and !queen_near[worker.pos.i][worker.pos.j]) take(worker.id); //Worker se encuentra en la cell Bonus
      else if(worker.bonus != None) { //Worker está cargando un bonus
        
        if (queen_adjacent[worker.pos.i][worker.pos.j] and cell(worker.pos).bonus == None) leave(worker.id);
        else movement_bfs_leave(my_workers_ids, available_pos, queen_adjacent, i);

      } else { //Worker no tiene bonus (va en busca de uno)
        
        movement_bfs_worker(my_workers_ids, available_pos, queen_near, i);

      }
    }

    //Soldier
    for (int i=0; i < int(my_soldiers_ids.size()); ++i) {
      //cerr << "++++++++++++++++++++++++++SOLDIER " << ant(my_soldiers_ids[i]).id << endl;
      movements_bfs(my_soldiers_ids, available_pos, i);
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
