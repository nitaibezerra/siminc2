<?
ini_set( "memory_limit", "512M" );
set_time_limit(0);

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."www/minc/_constantes.php";
include APPRAIZ."www/minc/_funcoes.php";

$db 	= new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];
$arPerfil = arrayPerfil();

$perfis = pegaPerfilGeral();

if( in_array( PERFIL_MINC_SEC_ESTADUAL, $perfis ) ){
	$innerRespUF = 'INNER JOIN minc.usuarioresponsabilidade ur ON ur.estuf = e.estuf';
}
if( in_array( PERFIL_MINC_SEC_MUNICIPAL, $perfis ) ){
	$innerRespMuncod = 'INNER JOIN minc.usuarioresponsabilidade ur2 ON ur2.muncod = m.muncod';
}

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

function listaEscolasResp(){
	ini_set('memory_limit', '256M');

	global $db;
	
	if($_POST["tpcid"]){
		$arWhere[] = "e.tpcid = '{$_POST["tpcid"]}'";
	}

	$sql = "SELECT DISTINCT
				e.entid,
				e.entcodent,
				e.entnome,
				endi.estuf,
				m.mundescricao,
				e.entcodent
			FROM
				entidade.entidade e
			INNER JOIN entidade.endereco 			endi  ON endi.entid = e.entid
			LEFT  JOIN territorios.municipio 		m 	  ON m.muncod = endi.muncod
			LEFT  JOIN minc.usuarioresponsabilidade ur1   ON ur1.entid = e.entid AND ur1.rpustatus = 'A' AND ur1.pflcod = 383
			INNER JOIN minc.mcemaiscultura 			maedu ON  maedu.entid = e.entid
			LEFT  JOIN workflow.documento 			d 	  ON d.docid = maedu.docid
			LEFT  JOIN workflow.estadodocumento 	est   ON est.esdid = d.esdid
			".($arWhere ? " WHERE ".implode(" AND ", $arWhere) : "")."
			ORDER BY
				e.entnome";

	$escolasExistentes = $db->carregar($sql);

	$count = count($escolasExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){

		$codigo      = $escolasExistentes[$i]["entid"];
		$inep        = $escolasExistentes[$i]["entcodent"];
		$descricao   = $escolasExistentes[$i]["entnome"];
		$localizacao = $escolasExistentes[$i]["mundescricao"]." - " . $escolasExistentes[$i]["estuf"];

		if (fmod($i,2) == 0){
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		//onclick=\"retorna('".$i."');\"
		echo "<tr bgcolor=\"".$cor."\">
		<td align=\"right\" width=\"10%\">
		<input type=\"checkbox\" name=\"entid\" id=\"".$codigo."\" value=\"".$codigo."\" class=\"valorOpcao\">
		<input type=\"hidden\" name=\"entnome\" value=\"".$codigo." - ".$descricao."\">
		</td>
		<td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
		<td>".$inep."</td>
		<td>".$descricao."</td>
		<td>".$localizacao."</td>
		</tr>";
	}
}

if(isset($_REQUEST['enviar'])) {

	$sql = "UPDATE
			 	minc.usuarioresponsabilidade 
			SET
			 	rpustatus = 'I' 
			WHERE
			 	usucpf = '$usucpf' AND 
			 	pflcod = $pflcod AND
			 	entid IS NOT NULL";
	
	$db->executar($sql);

	if(isset($_POST['usuunidresp'])){
		foreach($_POST['usuunidresp'] as $entid) {
				
			$sql = "";

			$sql = "UPDATE minc.usuarioresponsabilidade SET  
				   		rpustatus 	= 'A',
				   		rpudata_inc = now()
				   	WHERE
				   		usucpf  	= '$usucpf' AND
				   		pflcod 	    =  $pflcod AND
				   		entid    	= '$entid' ";
				
			$sqlQtd = "
				SELECT count(*) as qtd FROM minc.usuarioresponsabilidade 
				WHERE
				   		usucpf  	= '$usucpf' AND
				   		pflcod 	    =  $pflcod AND
				   		entid  	= '$entid' ";

			$qtd = $db->pegaUm($sqlQtd);
				
			if($qtd == 0)
			{
				$sql = "";

				$sql = "INSERT INTO minc.usuarioresponsabilidade 
					   (
					        rpustatus 	,
					   		rpudata_inc ,					   		
					   		entid  	,
					   		usucpf  	,
					   		pflcod 	
					   	)VALUES(
					   		'A'			,
					   		now()		,					   		
					   		'$entid'	,
					   		'$usucpf'	,
					   		$pflcod
						)					 			   			
				   ";					   						 
			}
			$db->executar($sql);
		}
	}
	$db->commit();

	?>
<script>
	
	window.parent.opener.location.reload();self.close();
</script>
	<?
	exit(0);
}

/*
 *** FIM REGISTRO RESPONSABILIDADES ***
 */
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Escolas</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde">
	<img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> 
	<font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>

<form name="formlista" method="post" action="">	
<DIV style="OVERFLOW: AUTO; WIDTH: 496px; HEIGHT: 350px; BORDER: 2px SOLID #ECECEC; background-color: White;">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<?php if( !in_array( PERFIL_MINC_SEC_MUNICIPAL, $perfis ) ){ ?>
		<tr>
			<td class="SubTituloDireita" align="right">
				Estado
			</td>
			<td>
			<?php
				$estuf = $_REQUEST['estuf'];
				$sql = "SELECT
						 	e.estuf as codigo, e.estdescricao as descricao 
						FROM
						 	territorios.estado e 
						$innerRespUF
						ORDER BY
						 	e.estdescricao asc";
		
				$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', 'filtro', '' );
			?>				
			</td>
		</tr>
		<?php } ?>
		<?php if ( $_POST['estuf'] || in_array( PERFIL_MINC_SEC_MUNICIPAL, $perfis ) ):  ?>
		<tr>
			<td class="SubTituloDireita" align="right">
				Município
			</td>
			<td>
			<?php
				$muncod = $_POST['muncod'];
				
				if($_REQUEST['muncod']){
					if(is_array($_REQUEST['muncod'])){
						$arWhere[] = "endi.muncod in ('".implode("','", $_REQUEST['muncod'])."')";
					}else{
						$arWhere[] = "endi.muncod = '{$_REQUEST['muncod']}'";		
					}
				}
				
				if($_REQUEST['estuf']){
					if(is_array($_REQUEST['estuf'])){
						$arWhere[] = "endi.estuf in ('".implode("','", $_REQUEST['estuf'])."')";
					}else{
						$arWhere[] = "endi.estuf = '{$_REQUEST['estuf']}'";
					}
				}
				
				$sql = "SELECT DISTINCT
							m.muncod AS codigo,
						 	m.mundescricao AS descricao
						FROM
						 	entidade.entidade e
						INNER JOIN entidade.funcaoentidade 	fe   ON fe.entid = e.entid
						INNER JOIN entidade.endereco 		endi ON endi.entid = e.entid
						INNER JOIN territorios.municipio 	m 	 ON m.muncod = endi.muncod
						WHERE
						 	fe.funid = 3						 			 	
						 	".($arWhere ? " AND ".implode(' AND ', $arWhere) : '')."
						ORDER BY m.mundescricao";

				$db->monta_combo( "muncod", $sql, 'S', 'Selecione um município', 'filtro', '' );
			?>				
			</td>
		</tr>
		<?php endif; 

		if ($_POST['muncod']):
			?>
				<tr>
					<td class="SubTituloDireita" align="right">
						Tipo de Escola
					</td>
					<td>
						<?php
					
							$tpcid = ($pflcod == PERFIL_MINC_SEC_MUNICIPAL)  ? 3 : 1;
// 							if( $db->testa_superuser() || in_array( PERFIL_MINC_ADMINISTRADOR, $perfis ) ){
								$tpcid = $_POST["tpcid"];
// 							}
							$tpcid = $tpcid != '' ? $tpcid : 1;
							$sql1 = "SELECT
										tpcid as codigo,
										tpcdesc as descricao
									FROM
										entidade.tipoclassificacao
									WHERE
										tpcid in (1,3)
									ORDER BY
										tpcdesc";

							$db->monta_combo( "tpcid", $sql1, 'S', 'Selecione um tipo de escola', 'filtro', '' );
							 
						?>
					</td>
				</tr>
			<?php
		
		endif;

		if ($_POST['muncod']): ?>
		<tr>
			<td colspan="2">
			<?php

				$sql = sprintf("SELECT
								 DISTINCT
								 '<input type=\"checkbox\" name=\"entid\" id=\"\" value=\"'|| ent.entid ||'\" onclick=\'retorna( this, \"' || ent.entid || '\", \"' || replace(mun.mundescricao || ' - ' || ent.entnome,'''','') || '\");\'/>' AS input,
								 ent.entcodent,								
								 ent.entnome,
 								 est.estuf || ' - ' || mun.mundescricao AS local,
								 CASE
								 	WHEN ent.tpcid = 1 THEN '<span title=\"Estadual\" alt=\"Estadual\">E</span>'
								 	ELSE '<span title=\"Municipal\" alt=\"Municipal\">M</span>'
								 END AS tipo
								from minc.mcemaiscultura mc
								inner join entidade.entidade ent using (entid)
								INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
								INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod 
								INNER JOIN territorios.estado est ON est.estuf = mun.estuf 
								WHERE
								 ent.tpcid IN (%s) AND
								 mun.muncod = '%s'
								ORDER BY
								 ent.entnome",
							$tpcid,
							$_POST['muncod']);
			
				$cabecalho = array( "Ação", "Cód. INEP", "Escola", "Localidade", "Tipo" );
				$db->monta_lista_simples( $sql, $cabecalho, 1500, 10, 'N', '100%', 'N' );
					
			?>	
			</td>		
		</tr>
		<?php endif; ?>
	</thead>
</table>
</DIV>
</form>

<form name="formassocia" style="margin: 0px;" method="POST">
	<input type="hidden" name="usucpf" value="<?=$usucpf?>"> 
	<input type="hidden" name="pflcod" value="<?=$pflcod?>"> 
	<input type="hidden" name="enviar" value=""> 
	<select multiple size="8" 
			name="usuunidresp[]" 
			id="usuunidresp" 
			style="width: 500px;" 
			class="CampoEstilo">
	<?
	$sql = "SELECT 
				distinct 
				e.entid as codigo, 
				mun.estuf ||'-'|| mun.mundescricao || ' - ' || e.entnome as descricao 
			FROM 
				minc.usuarioresponsabilidade ur 
			INNER JOIN entidade.entidade e on ur.entid = e.entid
			INNER JOIN entidade.endereco ende ON ende.entid = e.entid
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
	 		WHERE 
	 			ur.rpustatus='A' AND 
	 			ur.usucpf = '$usucpf' AND 
	 			ur.pflcod=$pflcod";
	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$descricao</option>";
			}
		}
	}

	?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding: 3px;" colspan="3">
			<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok">
		</td>
	</tr>
</table>
<script language="JavaScript" type="text/javascript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";

function filtro(){
	document.formlista.submit();
}

var campoSelect = document.getElementById("usuunidresp");

function enviarFormulario(){
	document.formassocia.enviar.value=1;
	document.formassocia.submit();

}


function mostraMunicipio(objSelect){
	for( var i = 0; i < objSelect.options.length; i++ )
	{
		if ( objSelect.options[i].value == objSelect.value )
		{
			var estado = objSelect.options[i].innerHTML.substring(0,2);
			break;
		}
	}
	var estadoAtual = document.getElementById(estado).style.display;
	if(estadoAtual != 'block'){
		 mostraEsconde(estado);
	}
	document.getElementById(objSelect.value).focus();
		
}


function retorna( check, muncod, mundescricao )
{
	//alert(campoSelect.options.length);
	if ( check.checked ) {
		if (campoSelect.options.length >= 1){
			alert('Não é permitido marcar mais de uma escola por usuário!');
			check.checked = false;
			return;
		}	
		// põe
		campoSelect.options[campoSelect.options.length] = new Option( mundescricao, muncod, false, false );
	
	}else{
		// tira
		for( var i = 0; i < campoSelect.options.length; i++ )
		{
			if ( campoSelect.options[i].value == muncod )
			{
				campoSelect.options[i] = null;
			}
		}
	}
	sortSelect( campoSelect );
}
</script>