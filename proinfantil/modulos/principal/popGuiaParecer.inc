<?php

if($_REQUEST['parid']){
	$sql = "SELECT tipid, pardescricao  
			FROM proinfantil.parecer
			WHERE parstatus = 'A'
			and parid = ".$_REQUEST['parid'];
	$dados = $db->pegaLinha($sql);
	
	$dados['pardescricao'] = str_ireplace('"', "", $dados['pardescricao']);
	$dados['pardescricao'] = str_ireplace("'", "", $dados['pardescricao']);
	
	$dados['pardescricao'] = simec_json_encode(utf8_encode($dados['pardescricao']) );
	
	?>
	<script type="text/javascript">
		window.opener.carregarParcerAnalise(<?=$dados['pardescricao']; ?>, <?=$dados['tipid']?>, '<?=$_REQUEST['campoTipo']?>', '<?=$_REQUEST['campoParecer']?>');
		/*window.opener.jQuery('[name="<?=$_REQUEST['campoTipo']?>"]').val("<?=$dados['tipid']?>");
		window.opener.jQuery('[name="<?=$_REQUEST['campoParecer']?>"]').val('<?=utf8_encode($dados['pardescricao'])?>');*/
		window.close();
		window.opener.document.focus();
	</script>
	<?
	die;	
}

monta_titulo( 'Guia de Pareceres', 'Clique no botão "+" para adicionar um Parecer' );

?>
<html>
 <head>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script type="text/javascript">
	
	function addParecer(parid){
		location.href="proinfantil.php?modulo=principal/popGuiaParecer&acao=A&campoTipo=<?=$_REQUEST['campoTipo']?>&campoParecer=<?=$_REQUEST['campoParecer']?>&parid="+parid;
	}

  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formDemanda" action="" method="post" onsubmit="return validaForm();">

	<? 
	if($_REQUEST['modid']){
		
		if($_REQUEST['filtraTipo']) $andTipo = " AND tipid = ".$_REQUEST['filtraTipo'];
		
		$sql = "SELECT '<center><img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"addParecer('||parid||')\"></center>' as acao,
						pardescricao as descricao --parid, modid, tipid, pardescricao, parstatus  
				FROM proinfantil.parecer
				WHERE
				parstatus = 'A'
				and modid = ".$_REQUEST['modid']."
				$andTipo
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
	
</form>
</body>
</html>



