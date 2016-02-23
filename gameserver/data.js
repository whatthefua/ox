var db = require('./db.js');
var zip = require('adm-zip');
var async = require('async');
function fetchAllEnded(callback){
  db.keys("*:log:*",(err,resp)=>{
    if(err)return callback(err);
    var q = async.queue((task,qcallback)=>{
      console.log(task,"fuck");
      qcallback();
    },20);
    q.drain = ()=>{

    }
    q.push(resp,(err)=>{
      if(err)
        callback(err);
    });
  })
}
exports.getAllEnded = fetchAllEnded;
