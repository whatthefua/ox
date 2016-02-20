var db = require('./db.js');
function initBoard(){
  var board = [];
  for(var mi=0;mi<3;mi++){
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
  for(var i=0;i<s;i++){
    if(ans[i][s-1]||ans[s-1][i])return ans[i][s-1] <16 ? 1:2;
  }
}
function getBlock(board,target){
  return board[target.mi][target.mj][target.i][target.j];
}
function setBlock(board,target){
  board[target.mi][target.mj][target.i][target.j] = parseInt(target.player);
}
function validateMove(boardState,move){
  if(move.player!=='1'&&move.player!=='2') return "Who?!?";
  if(boardState.ended) return "Game ended.";
  if(getBlock(boardState.board,move)) return "Block occupied.";
  if(getDominator(boardState.board[move.mi][move.mj])) return "This macro is fucking owned";
  if(boardState.lastmove){

  }
}
function setBoard(boardName,boardState,callback){
  db.set(boardName+ ":state",JSON.stringify(boardState),(err,resp)=>{
    if(err)return callback(err);
    callback(null,boardState);
  });
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
    callback(null,db_res);
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
