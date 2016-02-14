// var board = [];
// var turn = 1;
// var turnCount = 0;
// var lastmove;
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
  return boardData;
}
exports.getBoard = (boardName,callback)=>{
  db.get(boardName+":state",function(err,db_res){
    if(err)return console.log(err);
    if(!db_res){
      var newBoard = initBoard();
      return db.set(boardName+":state",JSON.stringify(newBoard),function(err,resp){
        if(err)return callback(err);
        console.log("New board :" + boardName + " created");
        callback(null,JSON.stringify(newBoard));
      });
    }
    callback(null,db_res);
  })
}
exports.move = (boardName,player,i,j,mi,mj,res)=>{
  db.get(boardName+":state",function(err,db_res){
    if(err)return console.log(err);
    if(!res)
      return res.end("Board not exists! Unusual activities.");
    var board = JSON.parse(db_res);
    res.end(db_res);
  })
}
