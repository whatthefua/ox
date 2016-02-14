var express = require('express');
var url = require('url');
var querystring = require('querystring');
var bodyParser = require('body-parser');
var db = require('./db.js');
var game = require('./game.js');
var app = express();
var board = [];
var turn = 1;
var lastmove;
function initBoard(){
  for(var mi=0;mi<3;mi++){
    board[mi]=[];
    for(var mj=0;mj<3;mj++){
      board[mi][mj]=[];
      for(var i=0;i<3;i++){
        board[mi][mj][i]=[0,0,0];
      }
    }
  }
}
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
}));
app.get('/state',(req,res)=>{
  var queryData = querystring.parse(url.parse(req.url).query);
  game.getBoard(queryData.boardName,function(err,data){
    if(err)return console.log(err);
    res.end(data);
  })
  // res.end(JSON.sboard)  ;
  // res.end(JSON.stringify({
  //   board:board,
  //   turn:turn,
  //   lastmove:lastmove
  // }));
});
// app.post('/move',(req,res)=>{
//   if(req.body.player!=turn){
//     res.end("Not your turn.");
//   }
//   else{
//     var move = req.body;
//     // res.end(JSON.stringify(move));
//     var owner = board[move.mi][move.mj][move.i][move.j];
//     if(!lastmove||(lastmove.i==move.mi && lastmove.j==move.mj)){
//       if(owner!=0)return res.end("Block owned by " + owner==1?"red":"blue");
//       board[move.mi][move.mj][move.i][move.j] = move.player;
//       if(!lastmove)lastmove={};
//       lastmove.i = move.i;
//       lastmove.j = move.j;
//       lastmove.mi = move.mi;
//       lastmove.mj = move.mj;
//       turn = turn==1?2:1;
//       res.end("Move ok");
//     }
//     else{
//       res.end("Invalid move");
//     }
//   }
//   // res.end(req.body.toString());
//
// });
app.post('/move',(req,res)=>{
  var move = req.body;
  // game.move(req)
});
// res.end(req.body.toString());

app.use(express.static('./publics'));
initBoard();
app.listen(3000);
