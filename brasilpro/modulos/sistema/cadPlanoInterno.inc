<?php

if($_REQUEST['prgid'] == 0){
	echo "	<script>
			alert('Selecione um programa para poder inserir o Plano Interno.');
			window.close();
		</script>";
	die();
}

/************************* DECLARAÇÃO DE VARIAVEIS *************************/
if($_REQUEST['modo'] == NULL){
	$_REQUEST['modo'] = "inserir";
}

$prgid 				= $_REQUEST['prgid'];
$ano 				= $_REQUEST['anoreferencia'];
$plicod 			= $_REQUEST['plicod'];
$modo 				= $_REQUEST['modo'];
$pliid				= $_REQUEST['id'];




/************************* MOSTRAR AO SELECIONAR ITEM NA LISTA *************************/
if($modo == "editar"){
	// SELECIONA O ANO //
	if($ano == "2007"){
		$selected07 = "selected";
	}else if($ano == "2008"){
		$selected08 = "selected";
	}else if($ano == "2009"){
		$selected09 = "selected";
	}else if($ano == "2010"){
		$selected10 = "selected";
	}else if($ano == "2011"){
		$selected11 = "selected";
	}else if($ano == "2012"){
		$selected12 = "selected";
	}
}
/************************* SALVAR DADOS ***********************************/
if($_REQUEST['salvar']){
	$sql = "SELECT pliid FROM monitora.pi_planointerno WHERE plicod = '".$plicod."'";
	$pliidMonitora = $db->pegaUm($sql);
	if($pliidMonitora){
		if($modo == "inserir"){
			//$sql = "SELECT pliano FROM cte.planointerno WHERE prgid =".$prgid." AND pliano = ".$ano;
			//$pliAnoExiste = $db->pegaUm($sql);
			//if(!$pliAnoExiste){
				$sql = "INSERT INTO cte.planointerno 
						(pliano, plinumplanointerno, prgid, pliidmonitora) 
						VALUES (".$ano.", '".$plicod."', ".$prgid.", ".$pliidMonitora.")";
				if($db->executar($sql)){
					$db->commit();
					echo "	<script>
								alert('Operação realizada com sucesso.');
								//window.close();
							</script>";
				}
			//}else{
				//alert("Já existe um Plano interno cadastrado para este ano.");
			//}
		}else if($modo == "editar"){
			$sql = "UPDATE cte.planointerno 
					SET plinumplanointerno = '".$plicod."', 
						pliano = ".$ano." 
					WHERE prgid =".$prgid."  AND pliid = ".$pliid;
			if($db->executar($sql)){
				$db->commit();
				$_REQUEST['modo'] = NULL;
				echo "	<script>
							alert('Operação realizada com sucesso.');
							location.href='brasilpro.php?modulo=sistema/cadPlanoInterno&acao=A&prgid='+".$prgid.";
							//window.close();
						</script>";
				}
		}
	}else{
		alert("Este Plano Interno não está cadastrado na tabela de referência do SIMEC.");
	}
}


/************************* RECUPERA DESCRIÇÃO DO PROGRAMA ****************/
$sql   	= "SELECT prgdsc FROM cte.programa WHERE prgid = ".$prgid;
$prgdsc = $db->pegaUm($sql);

$sqlPlanos = "SELECT distinct plicod as codigo, plicod as descricao
			FROM monitora.pi_planointerno pi
			INNER JOIN monitora.pi_planointernoptres pip on pip.pliid = pi.pliid
			INNER JOIN monitora.ptres pt ON pt.ptrid = pip.ptrid
			WHERE pt.prgcod::integer in (1061, 1060, 1062, 1448, 1374, 2031)
			ORDER BY plicod";
/************************* MONTA TÍTULO *************************/
monta_titulo($titulo_modulo,'(Plano Interno)');
?>
<html>
<head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<body>
<form method="POST"  name="formulario">
<input type=hidden name="salvar" id="salvar" value="0" />
<input type=hidden name="modo" id="modo" value="<?php echo($modo); ?>" />
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita" >Descrição: </td>
        <td ><?=$prgdsc; ?></td>
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita" >Ano:</td>
        <td >
			<select style="width:110px;" name="anoreferencia" title="ano" id="anoreferencia">
				<option value="">selecione</option>
			<?php if($modo == "editar"){ ?>
				<option <?= $selected07; ?> value="2007">2007</option>
				<option <?= $selected08; ?> value="2008">2008</option>
			<?php } ?>
  				<option <?= $selected09; ?> value="2009">2009</option>
  				<option <?= $selected10; ?> value="2010">2010</option>
  				<option <?= $selected11; ?> value="2011">2011</option>
				<option <?= $selected12; ?> value="2012">2012</option>
			</select> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
		</td>
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita" >Plano Interno: </td>
        <td ><?php
       $db->monta_combo( "plicod", $sqlPlanos , 'S', 'Selecione', '', '', '','','S', 'plicod', false, $plicod, 'Plano Interno' ); 

        ?></td>
      </tr>
      <tr>
      	<td class="SubTituloDireita"></td>
      	<td class="SubTituloEsquerda" align="center">
      		<input type="button" name="salvar" value="salvar"  onclick="salvarDados();" />
      	</td>
      </tr>
</table>
</form>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th  colspan="3" >Lista de Planos Internos</th>
	</tr>
	<tr>
		<td>
		<?php 
			$cabecalho 			= array("Ação","Ano", "Plano Interno");
			$sqlPlanoInternos 	= " SELECT 
										'<a style=\"margin: 0 -20px 0 20px;\" href=\"brasilpro.php?modulo=sistema/cadPlanoInterno&acao=A&modo=editar&plicod='|| plinumplanointerno ||'&anoreferencia='|| pliano  ||'&prgid='|| prgid  ||'&id='|| pliid  ||'\"><img src=\"../imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
										pliano, 
										plinumplanointerno 
									FROM cte.planointerno 
									WHERE prgid =".$prgid." ORDER BY pliano";
			$db->monta_lista_simples($sqlPlanoInternos,$cabecalho,20, 10, 'N', '100%', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="3"><input id="btnfechar" type="button" name="btnfechar" value="Fechar" onclick="window.close();" /></td>
	</tr>
</table>
<script type="text/javascript">

function salvarDados(){
	var nomeform 		= 'formulario';
	campos 				= new Array("anoreferencia","plicod" );
	tiposDeCampos 		= new Array("select","select");

	if(validaForm(nomeform, campos, tiposDeCampos, false )){
		document.getElementById("salvar").value = 1;
		document.formulario.submit();
	}
}
</script>
</body>
</html>