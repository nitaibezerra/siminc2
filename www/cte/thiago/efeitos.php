<html>
<head>
<style type="text/css">
body {color: white;background: #52616F;}
a { color: white; }
</style>
<link href="css/default.css" rel="stylesheet" type="text/css"/>
<link href="css/alphacube.css" rel="stylesheet" type="text/css"/>

<script type="text/javascript" src="javascripts/prototype.js"> </script>
<script type="text/javascript" src="javascripts/effects.js"> </script>
<script type="text/javascript" src="javascripts/window.js"> </script>
<script type="text/javascript" src="javascripts/window_effects.js"> </script>
<script type="text/javascript" src="javascripts/debug.js"> </script>

<style>
body {
background: #ffffff;
}
#border {
position:absolute;
top:10px;
left:10px;
width:500px;
height:500px;
border: 1px solid #000;
color:#999999;
overflow:hidden;
}
#container {
position:absolute;
top:0px;
left:0px;
width:500px;
height:500px;
overflow:hidden;
background:#ffffff;
border:1px solid #999999;
color:#999999;
z-index:0;
}
</style>
</head>
<body>

<script type="text/javascript">
function win1()
{
var win = new Window({id: "win1", className: "alphacube", title: "Caixa1", width:250, height:150, top:0, left: 1, parent:$('container')});
win.getContent().innerHTML = "<h1>Teste1</h1><br><a href='#' onclick='Windows.getWindow(\"win1\").maximize()'>Maximize me</a>";

win.setDestroyOnClose();
win.show();
win.setConstraint(true, {left:10, right:20})
win.toFront();
}

function win2()
{
var win = new Window({id: "win2", className: "alphacube", title: "Caixa2", width:200, height:150});
win.getContent().innerHTML = "<h1>Teste2</h1><br><a href='#' onclick='Windows.getWindow(\"win2\").maximize()'>Maximize me</a>";

win.setDestroyOnClose();
win.showCenter();
win.setConstraint(true, {left:0, right:0, top: 30, bottom:10})
win.toFront();
}

function win3()
{
var win = new Window({id: "win3", className: "alphacube", title: "Caixa3", width:250, height:150, wiredDrag: true});
win.getContent().innerHTML = "<h1>Teste3</h1><br><a href='#' onclick='Windows.getWindow(\"win3\").maximize()'>Maximize me</a>";
win.setDestroyOnClose();
win.setLocation(10, 500);
win.show();
win.toFront();
}

</script>

<div id="border">
<div id="container"></div>
</div>

<script>
win1();
win2();
win3();
</script>
</body></html> 