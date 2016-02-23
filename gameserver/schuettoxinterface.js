exports.run = (boardState,callback)=>{
  const exec = require('child_process').exec;
  const botProc = exec("./schuettox");
  botProc.stderr.on('data',(data)=>{
    console.log("Bot interface error : " + data);
  });
  var hs = 0;
  botProc.stdout.on('data',(data)=>{
    data = data.split(" ");
    if(!hs)
    doInput();
    else callback([ parseInt(data[0]), parseInt(data[1]) ]);
    hs=1;
  });
  function doInput(){
    inp(boardState.turn);
    for(var i=0;i<3;i++)
      for(var j=0;j<3;j++)inp(boardState.macro[i][j]);
    for(var i=0;i<3;i++){
      for(var j=0;j<3;j++){
        for(var k=0;k<3;k++){
          for(var l=0;l<3;l++) inp(boardState.board[i][j][k][l]);
        }
      }
    }
  }

  function inp_p(val){
    botProc.stdin.write(val);
  }
  var i=0;
  function inp(val){
    process.stdout.write(val + " ");
    botProc.stdin.write(val + " ");
  }
}
