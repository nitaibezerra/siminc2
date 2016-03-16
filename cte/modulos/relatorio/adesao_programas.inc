<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório de Adesão aos Programas", "" );

?>

<script type="text/javascript" src="../includes/prototype.js"></script>

<style type="text/css">

	 .escolaAtiva{
	 	width: 25%;
	 	float: left;
	 	text-align: center;
	 }
	 
	 .proLetramento{
	 	width: 50%;
	 	float: left;
	 	text-align: center;
	 }
	 
	 .formacaoEscola{
	 	width: 50%;
	 	float: left;
	 	text-align: center;
	 }

	.tituloPrograma{
		text-align: center;
		margin-top: 20px; 
	}
	
	.tituloPrograma img{
		height: 20px;
		width: 20px;
		margin-right: 5px;
	}
	
	.labelColulaRelatorio{		
		height: 45px;
	}
	
	.colunasRelatorio{				 
		height: 300px; 
		overflow: auto;
	}

</style>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script>
	function mostraConteudo(pagina, id) {
	
		var mostraConteudo = $(id);
		
		mostraConteudo.innerHTML = "<div style=\"width:100%;text-align:center;margin-top:0%\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...</div>"; 
		
		new Ajax.Request(pagina, {
			method: 'post',
			parameters: '&mostraRelAdesao=true',
			onComplete: function(res){		

				mostraConteudo.innerHTML = res.responseText;
			}
			
		});
	
	}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			
			<div id="escolaAtiva" style="width: 100%;">
				<h2 class="tituloPrograma"><img src="../../imagens/grafico_pizza.png" />Escola ativa</h2>				
				<div id="mostraEscolaAtiva"><center><a onclick="mostraConteudo('cte.php?modulo=relatorio/adesao_escola_ativa&acao=A','mostraEscolaAtiva')" style="cursor:pointer;">Visualizar dados da Escola ativa</a></center></div>
			</div>
			
			<br style="clear: both;" />
			
			<div id="proLetramento" style="width: 100%;">
				<h2 class="tituloPrograma"><img src="../../imagens/grafico_pizza.png" />Pró-letramento</h2>				
				<div id="mostraProLetramento"><center><a onclick="mostraConteudo('cte.php?modulo=relatorio/adesao_pro_letramento&acao=A','mostraProLetramento')" style="cursor:pointer;">Visualizar dados do Pró-letramento</a></center></div>
			</div>
			
			<br style="clear: both;" />
			
			<div id="formacaoEscola" style="width: 100%;">
				<h2 class="tituloPrograma"><img src="../../imagens/grafico_pizza.png" />Formação pela escola</h2>				
				<div id="mostraFormcaoEscola"><center><a onclick="mostraConteudo('cte.php?modulo=relatorio/adesao_formacao_escola&acao=A','mostraFormcaoEscola')" style="cursor:pointer;">Visualizar dados do formação pela escola</a></center></div>
			</div>
			<br>

		</td>
	</tr>
</table>

<script type="text/javascript">
	
	function toggleDiv( idImg, idDiv ){

		var img = document.getElementById( idImg );	
		var div = document.getElementById( idDiv );
		
		if( div.style.display == 'none' ){
			div.style.display = '';
			img.src = '../imagens/menos.gif'
		}
		else{
			div.style.display = 'none';
			img.src = '../imagens/mais.gif'
		}
	}
	
	function toggleTodos( nameImg, nameDiv, acao ){

		var arImg   = getElementsByName_iefix( 'img', nameImg );	
		var arDiv = getElementsByName_iefix( 'div', nameDiv );
		
		for( var i = 0; i<arDiv.length; i++ ){
		
			if( acao == 'abrir' ){
				arDiv[i].style.display = '';
				arImg[i].src = '../imagens/menos.gif'
			}
			else{
				arDiv[i].style.display = 'none';
				arImg[i].src = '../imagens/mais.gif'
			}
		}
	}	
	
	function getElementsByName_iefix(tag, name) {  
	       
		var elem = document.getElementsByTagName(tag);  
		var arr = new Array();  
	    for(i = 0,iarr = 0; i < elem.length; i++) {  
			att = elem[i].getAttribute("name");  
			if(att == name) {  
				arr[iarr] = elem[i];  
				iarr++;  
			}  
		}  
		return arr;  
	}  	

</script>