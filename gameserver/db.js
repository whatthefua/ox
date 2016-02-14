var Redis = require('redis-fast-driver');
var r = new Redis({
	host: '127.0.0.1',
	port: 6379
});
exports.get = (key,callback)=>{
  r.rawCall(['GET',key],(e,resp)=>{
    if(e)return callback(e);
    callback(null,resp);
  });
}
exports.set = (key,value,callback)=>{
  r.rawCall(['SET',key,value],(e,resp)=>{
    if(e)return callback(e);
    callback(null,resp);
  });
}
