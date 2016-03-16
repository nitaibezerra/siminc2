<?php

if($_REQUEST['requisicao'] == 'carregarParecer' ){
	
	if( $_REQUEST['parid'] ){
		$sql = "SELECT pardescricao FROM proinfantil.parecer WHERE parid in (".$_REQUEST['parid'].") order by pardescricao";
		$dados = $db->carregarColuna($sql);
		$dados = $dados ? $dados : array();
		if( $dados[0] ){
			echo implode("\n\r", $dados);
		} else {
			echo '';
		}
	} else {
		echo '';
	}
	exit();	
}


monta_titulo( 'Guia de Pareceres', 'Clique no botão "+" para adicionar um Parecer' );

?>
<html>
 <head>
  <script type="text/javascript" src="../estrutura/js/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
  <script type="text/javascript">
	
	function addParecer(){
		var campoRequest = $('[name="campoRequest"]').val();
		var arParid = new Array();
		$('[name="chk[]"]:checked').each(function(i, v){
			arParid[i] = $(v).val();
		});
		var strparid = implode(',', arParid);
		
		$.ajax({
			type: "POST",
			url: 'proinfantil.php?modulo=novasturmas/guiaParecerNovasTurmas&acao=A',
			data: "requisicao=carregarParecer&parid="+strparid,
			async: false,
			success: function(msg){
				//$('#debug').html( msg );
				window.opener.$('[name="'+campoRequest+'"]').val(msg);
			}
		});
	}
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
	<div id="debug"></div>
	<input type="hidden" name="campoRequest" id="campoRequest" value="<?php echo $_REQUEST['campo'];?>">
	<? 
	if($_REQUEST['modid']){
		
		$sql = "select
					'<center><input type=\"checkbox\" name=\"chk[]\" id=\"chk\" value=\"'||parid||'\" onclick=\"addParecer();\"></center>' as acao, 
					p.pardescricao 
				from 
					proinfantil.parecer p 
				where 
					p.tipid = {$_REQUEST['tipid']}
				    and p.parstatus = 'A' 
				    and p.modid = {$_REQUEST['modid']}
				order by pardescricao";
		$arrCab = array("<center>Ação</center>","<center>Parecer</center>");
		$db->monta_lista($sql,$arrCab,1000,10,"N","","N");
		
	}
	
	?>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td align="center">
		    	<input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();' />	    	
			</td>			
		</tr>				
	</table>
</body>
</html>



