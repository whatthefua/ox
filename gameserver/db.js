var Redis = require('redis-fast-driver');
var r = new Redis({
	host: '127.0.0.1',
	port: 6379
});
exports.get = (key,callback)=>{
  r.rawCall(['GET',key],callback);
}
exports.set = (key,value,callback)=>{
  r.rawCall(['SET',key,value],callback);
}
exports.keys = (keyword,callback)=>{
	r.rawCall(['KEYS',keyword],callback)
}
