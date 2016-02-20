var express = require('express');
var url = require('url');
var querystring = require('querystring');
var bodyParser = require('body-parser');
var db = require('./db.js');
var game = require('./game.js');
var app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
}));
app.get('/state',(req,res)=>{
  var queryData = querystring.parse(url.parse(req.url).query);
  game.getBoard(queryData.boardName,function(err,data){
    if(err)return console.log(err);
    res.end(data);
  });
});
app.post('/move',(req,res)=>{
  game.move(req.body,(err,data)=>{
    if(err)return res.end(err.toString());
    res.end(data);
  });
});

app.use(express.static('./publics'));
app.listen(3000);
