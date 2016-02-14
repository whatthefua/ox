var async = require('async');
var bl = require('bl');
var http = require('http');
var fs = require('fs');
var i =0;
var q = async.queue(function(task,callback){
  http.get(task.url,function(res){
    res.pipe(bl(function(err,dat){
      if(err)return callback(err);
      fs.writeFileSync(task.i + ".txt",dat);
      console.log(task.i);
      callback();
    }));
  }).on('error',function(err){
    console.log(err);
    console.log("Pushing back to queue");
    q.push(task);
  });
},20);

q.drain(function(){
  console.log("Queue emptied");
});

var list = fs.readFileSync("urllist.txt").toString().split("\n");
list.forEach(function(val,idx){
  q.push({
    i:idx,
    url:val
  })
});
