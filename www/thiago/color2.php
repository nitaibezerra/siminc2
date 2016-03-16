<html>
	<head>
		<script type="text/javascript" src="jquery.js"></script>
		<script type="text/javascript" src="xcolor.js"></script>
	</head>
	<body>
		
	<p>
	
	</p>
	
	
	<script type="text/javascript">
	
		function retornarCor(corInicio, corFim, escala, posicao ){
			var cores = 0;
			for(i=0;i < escala; i++){
				cores = $.xcolor.gradientlevel(corInicio, corFim, i, escala);
				if(i+1 == posicao ){
					return cores;
					//$("p").append('<div style="width: 60px; height: 60px; background-color:'+cores+'; float:left; margin-left: 5px;"></div>');
				}else{
					//$("p").append('<div style="width: 30px; height: 30px; background-color:'+cores+'; float:left; margin-left: 5px;"></div>');
				}
			}
		}
		
		var corfinal = retornarCor('#DDD3C6', '#007725', 10, 8 );
		alert(corfinal);

		
		
	</script>
	</body>
</html>