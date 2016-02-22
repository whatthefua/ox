var db = require('./db.js');
function initBoard(){
  var board = [];
  var macro = [];
  for(var mi=0;mi<3;mi++){
    macro[mi]=[0,0,0];
    board[mi]=[];
    for(var mj=0;mj<3;mj++){
      board[mi][mj]=[];
      for(var i=0;i<3;i++){
        board[mi][mj][i]=[0,0,0];
      }
    }
  }
  var boardData = {};
  boardData.board = board;
  boardData.macro = macro;
  boardData.turn = 1;
  boardData.turnCount = 0;
  return boardData;
}
function getDominator(arr){
  var ans = [];
  var s = arr.length;
  var vertFlag = 0x1,horiFlag = 0x2,downFlag = 0x4,upFlag = 0x8;
  var map = [0,1,16];
  //Fuck this OX Logic
  for(var i=0;i<s;i++){
    ans[i] = [];
    for(var j=0;j<s;j++){
      ans[i][j]=0;
      if(arr[i][j]==1){
        if(i>0)ans[i][j] += ans[i-1][j]&vertFlag;
        else ans[i][j] += map[arr[i][j]]<<0;
        if(j>0)ans[i][j] += ans[i][j-1]&horiFlag;
        else ans[i][j] += map[arr[i][j]]<<1;
        if(i>0&&j>0)ans[i][j] += ans[i-1][j-1]&downFlag;
        else if(i==0 && j ==0) ans[i][j] += map[arr[i][j]]<<2;
        if(i>0&&j<s-1)ans[i][j] += ans[i-1][j+1]&upFlag;
        else if(i==0 && j == s-1) ans[i][j] += map[arr[i][j]]<<3;
      }
      else if(arr[i][j]==2){
        if(i>0)ans[i][j] += ans[i-1][j]&(vertFlag<<4);
        else ans[i][j] += map[arr[i][j]]<<0;
        if(j>0)ans[i][j] += ans[i][j-1]&(horiFlag<<4);
        else ans[i][j] += map[arr[i][j]]<<1;
        if(i>0&&j>0)ans[i][j] += ans[i-1][j-1]&(downFlag<<4);
        else if(i==0 && j ==0) ans[i][j] += map[arr[i][j]]<<2;
        if(i>0&&j<s-1)ans[i][j] += ans[i-1][j+1]&(upFlag<<4);
        else if(i==0 && j == s-1) ans[i][j] += map[arr[i][j]]<<3;
      }
    }
  }
  if((ans[s-1][0]&upFlag) || (ans[s-1][s-1]&downFlag))return 1;
  if((ans[s-1][0]&(upFlag<<4)) || (ans[s-1][s-1]&(downFlag<<4)))return 2;
  for(var i=0;i<s;i++){
    if((ans[i][s-1]&horiFlag) || (ans[s-1][i]&vertFlag))return 1;
    if((ans[i][s-1]&(horiFlag<<4)) || (ans[s-1][i]&(vertFlag<<4)))return 2;
    if((ans[s-1][0]&upFlag) || (ans[0][s-1]&downFlag))return 1;
    if((ans[s-1][0]&(upFlag<<4)) || (ans[0][s-1]&(downFlag<<4)))return 1;
  }
  return 0;
}
function findMacroState(boardState){
  var m = boardState.lastmove;
  var targetIsOwned = getDominator(boardState.board[m.i][m.j]);
  for(var i=0;i<3;i++){
    for(var j=0;j<3;j++){
      var dom = getDominator(boardState.board[i][j]);
      if(!dom){
        if(targetIsOwned)boardState.macro[i][j] = -1;
        else boardState.macro[i][j] = 0;
      }
      else boardState.macro[i][j]=dom;
    }
  }
  if(!targetIsOwned)boardState.macro[m.i][m.j] = -1;
}
function getBlock(board,target){
  return board[target.mi][target.mj][target.i][target.j];
}
function setBlock(board,target){
  board[target.mi][target.mj][target.i][target.j] = parseInt(target.player);
}
function validateMove(boardState,move){
  var lastmove = boardState.lastmove;
  var macroMapi = ["top","middle","bottom"];
  var macroMapj = ["left","center","right"];
  if(boardState.ended) return "Game ended.";
  if(move.player!=='1'&&move.player!=='2') return "Who?!?";
  if(move.player!=boardState.turn) return "Not your turn.";
  if(getBlock(boardState.board,move)) return "Block occupied.";
  if(getDominator(boardState.board[move.mi][move.mj])) return "This macro is fucking owned";
  if(boardState.lastmove){
    if(move.mi != lastmove.i || move.mj != lastmove.j){
      if(!getDominator(boardState.board[lastmove.i][lastmove.j]))
      return "Invalid move! must be in : " + macroMapi[lastmove.i] + "-" + macroMapj[lastmove.j];
    }
  }
  return null;
}
function setBoard(boardName,boardState,callback){
  db.set(boardName+ ":state",JSON.stringify(boardState),(err,resp)=>{
    if(err)return callback(err);
    callback(null,boardState);
  });
}
function recordMove(move,turn){

}
exports.getBoard = (boardName,callback)=>{
  db.get(boardName+":state",function(err,db_res){
    if(err)return console.log(err);
    if(!db_res){
      var newBoard = initBoard();
      setBoard(boardName,newBoard,(err,resp)=>{
        if(err)return callback(err);
        console.log("New board :" + boardName + " created");
        callback(null,JSON.stringify(newBoard));
      });
    }
    else callback(null,db_res);
  });
}
exports.move = (move,callback)=>{
  db.get(move.boardName+":state",(err,db_res)=>{
    if(err)return console.log(err);
    if(!db_res)
    return callback("Board not exists! Unusual activities.");
    var boardState = JSON.parse(db_res);
    var moveError = validateMove(boardState,move);
    if(!moveError){
      setBlock(boardState.board,move);
      // boardState.macro[move.mi][move.mj] = getDominator(boardState.board[move.mi][move.mj]);
      boardState.lastmove = move;
      findMacroState(boardState);
      var winner = getDominator(boardState.macro);
      if(winner)boardState.ended=true
      recordMove(move,boardState.turnCount);
      boardState.turnCount++;
      boardState.turn = boardState.turn == '1' ? 2 : 1;
      setBoard(move.boardName,boardState,(err,resp)=>{
        if(err){
          console.log("Database error:" + err);
          return callback(new Error("Internal server error"));
        }
        callback(null,JSON.stringify(resp));
      });
    }
    else callback(new Error(moveError));
  })
}
