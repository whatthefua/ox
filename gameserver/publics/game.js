var player;
var microEle = [];
var macroEle = [];
var boardName = "eiei";
function msg(str) {
  $('#msg').text(str);
}
var macroColor = {
  '-1':"rgb(122,122,122)",
  '0':"",
  "1":"rgb(134, 78, 78)",
  "2":"rgb(66, 66, 117)"
}
var microColor = [
  "rgb(186, 215, 187)",
  "rgb(249, 94, 94)",
  "rgb(176, 160, 223)"
]
var turnEle = $('#turn');
var turnStr = {
  '1':"red",
  '2':"blue"
}
function redraw(state) {
  var a = [0, 1, 2];
  for (var mi in a) {
    for (var mj in a) {
      macroEle[mi][mj].css("background",macroColor[state.macro[mi][mj]]);
      for (var i in a) {
        for (var j in a) {
          //SO TIGHT ,UGHH such nest
          var owner = state.board[mi][mj][i][j];
          microEle[mi][mj][i][j].css("background", microColor[owner]);
        }
      }
    }
  }
  turnEle.text(turnStr[state.turn] + " turn.");
  turnEle.css("background",state.ended?"rgba(0,0,0)":turnStr[state.turn]);
}

function getState(cb) {
  $.get('/state',{
    boardName:boardName
  }, function(ret) {
    ret = JSON.parse(ret);
    boardState = ret;
    redraw(ret);
    if (cb)
      cb(ret);
  })
}

function move(i, j, mi, mj) {
  $.post('/move', {
    boardName:boardName ,
    player: player,
    i: i,
    j: j,
    mi: mi,
    mj: mj
  }, function(ret) {
    getState();
  });
}

function handleClick(ele, i, j, mi, mj) {
  ele.click(function() {
    player = $("input:radio:checked").val();
    move(i, j, mi, mj);
  });
}
function generateTable() {
  var a = [0, 1, 2];
  for (var mi in a) {
    microEle[mi] = [];
    macroEle[mi] = [];
    for (var mj in a) {
      var macroBlock = $("<div>");
      macroBlock.css("display", "inline-block");
      macroBlock.css("margin", "0.5rem");
      microEle[mi][mj] = [];
      macroEle[mi][mj] = macroBlock;
      for (var i in a) {
        microEle[mi][mj][i] = [];
        var microBlock;
        for (var j in a) {
          microBlock = $("<div>");
          microEle[mi][mj][i][j] = microBlock;
          microBlock.css("margin", "0.3rem");
          microBlock.css("background", "#555");
          microBlock.css("width", "4rem");
          microBlock.css("height", "4rem");
          microBlock.css("float", "left");
          // microBlock.css("display", "inline-block");
          handleClick(microBlock, i, j, mi, mj);
          macroBlock.append(microBlock);

        }
        macroBlock.append("<br>")
      }
      $('#game').append(macroBlock);
    }
    $('#game').append("<br>")
  }
}
$(function() {
  $('#boardName').keyup(function(){
    boardName = $(this).val();
  })
  generateTable();
  getState();
});
setInterval(getState, 500);
