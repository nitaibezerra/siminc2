<?php

if($_REQUEST['prgid'] == 0){
	echo "	<script>
			alert('Selecione um programa para poder inserir o Plano Interno.');
			window.close();
		</script>";
	die();
}

if( $_REQUEST['filtraPTRES'] && $_REQUEST['plicod'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "select 
				ptres as codigo,
				ptres as descricao
			from monitora.pi_planointerno pli 
			inner join monitora.pi_planointernoptres plp on plp.pliid = pli.pliid
			inner join monitora.ptres ptr on ptr.ptrid = plp.ptrid
			where pli.plicod = '{$_REQUEST['plicod']}'";
	
	echo $db->monta_combo( "ptres", $sql, 'S', 'Selecione...', '',"","","","N","","");
	exit;
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
$ptres				= $_REQUEST['ptres'];




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
	}else if($ano == "2013"){
		$selected13 = "selected";
	}else if($ano == "2014"){
		$selected14 = "selected";
	}else if($ano == "2015"){
		$selected15 = "selected";
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
				$sql = "INSERT INTO par.planointerno (
							pliano, 
							plinumplanointerno, 
							prgid, 
							pliidmonitora,
							pliptres) 
						VALUES (
							".$ano.", 
							'".$plicod."', 
							".$prgid.", 
							".$pliidMonitora.",
							'".$ptres."')";

				if($db->executar($sql)){
					$db->commit();
					echo "	<script>
								alert('Operação realizada com sucesso.');
								location.href='par.php?modulo=sistema/tabelasapoio/cadPlanoInterno&acao=A&prgid='+".$prgid.";
								//window.close();
							</script>";
				}
			//}else{
				//alert("Já existe um Plano interno cadastrado para este ano.");
			//}
		}else if($modo == "editar"){
			$sql = "UPDATE par.planointerno 
					SET 
						plinumplanointerno = '".$plicod."', 
						pliano = ".$ano.", 
						pliptres = '".$ptres."' 
					WHERE 
						prgid =".$prgid."  
						AND pliid = ".$pliid;
			if($db->executar($sql)){
				$db->commit();
				$_REQUEST['modo'] = NULL;
				echo "	<script>
							alert('Operação realizada com sucesso.');
							location.href='par.php?modulo=sistema/tabelasapoio/cadPlanoInterno&acao=A&prgid='+".$prgid.";
							//window.close();
						</script>";
				}
		}
	}else{
		alert("Este Plano Interno não está cadastrado na tabela de referência do SIMEC.");
	}
}


/************************* RECUPERA DESCRIÇÃO DO PROGRAMA ****************/
$sql   	= "SELECT prgdsc FROM par.programa WHERE prgid = ".$prgid;
$prgdsc = $db->pegaUm($sql);

$sqlPlanos = "  SELECT DISTINCT 
					plicod as codigo, 
					plicod as descricao
				FROM 
					monitora.pi_planointerno pi
				INNER JOIN monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid
				INNER JOIN monitora.ptres pt ON pt.ptrid = pip.ptrid
				WHERE 
					pi.plistatus = 'A' 
					AND prgcod::integer IN (1061, 1060, 1062, 1448, 1374, 1377, 2030, 2031)  
				ORDER BY 
					plicod ";
//inserir ano no Sql de planos
// and pliano = '2009'

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
  				<option <?= $selected13; ?> value="2013">2013</option>
  				<option <?= $selected14; ?> value="2014">2014</option>
  				<option <?= $selected15; ?> value="2015">2015</option>
			</select> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
		</td>
      </tr>
		<tr>
        	<td align='right' class="SubTituloDireita" >Plano Interno: </td>
        	<td ><?php
       			$db->monta_combo( "plicod", $sqlPlanos , 'S', 'Selecione', 'filtraPTRES', '', '','','S', 'plicod', false, $plicod, 'Plano Interno' ); 
       			?>
       		</td>
		</tr>
		<tr>
	        <td class="SubTituloDireita">PTRES:</td>
	        <td id="td_ptres">
	        	<?php $sql = "select 
								ptres as codigo,
								ptres as descricao
							from monitora.pi_planointerno pli 
							inner join monitora.pi_planointernoptres plp on plp.pliid = pli.pliid
							inner join monitora.ptres ptr on ptr.ptrid = plp.ptrid
							where pli.plicod = '{$plicod}'";
	        	
	        	$db->monta_combo("ptres",$sql,"S","Selecione...",'',"","","","N","","",$ptres,"PTRES"); ?>
	        </td>
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
			$cabecalho 			= array("Ação","Ano", "Plano Interno","PTRES");
			$sqlPlanoInternos 	= " SELECT 									
										'<a style=\"margin: 0 -20px 0 20px;\" href=\"par.php?modulo=sistema/tabelasapoio/cadPlanoInterno&acao=A&modo=editar&plicod='|| plinumplanointerno ||'&anoreferencia='|| pliano  ||'&prgid='|| prgid  ||'&id='|| pliid  ||'\"><img src=\"../imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
										pliano, 
										plinumplanointerno,
										pliptres 
									FROM par.planointerno 
									WHERE prgid =".$prgid." ORDER BY pliano";
			$db->monta_lista_simples($sqlPlanoInternos,$cabecalho,20, 10, 'N', '100%', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="3"><input id="btnfechar" type="button" name="btnfechar" value="Fechar" onclick="window.close();" /></td>
	</tr>
</table>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function salvarDados(){
	var nomeform 		= 'formulario';
	campos 				= new Array("anoreferencia","plicod","ptres" );
	tiposDeCampos 		= new Array("select","select","select");

	if(validaForm(nomeform, campos, tiposDeCampos, false )){
		document.getElementById("salvar").value = 1;
		document.formulario.submit();
	}
}

function filtraPTRES(plicod) {
	if(plicod!=''){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "filtraPTRES=true&" + "plicod=" + plicod,
		   success: function(resp){
				document.getElementById("td_ptres").innerHTML = resp;
			}
		});
	}
}
</script>
</body>
</html>