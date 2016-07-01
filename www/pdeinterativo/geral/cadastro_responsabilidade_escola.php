<?

ini_set( "memory_limit", "512M" );
set_time_limit(0);

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."www/pdeinterativo/_constantes.php";

$db 	= new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

if(isset($_REQUEST['enviar'])) {



	$sql = "update
			 pdeinterativo.usuarioresponsabilidade 
			set
			 rpustatus = 'I' 
			where
			 usucpf = '$usucpf' and 
			 pflcod = $pflcod and
			 entid is not null";
	$db->executar($sql);


	if(isset($_POST['usuunidresp'])){
		foreach($_POST['usuunidresp'] as $entid) {
				
			$sql = "";

			$sql = "update 
				   		pdeinterativo.usuarioresponsabilidade 
				   set  
				   		rpustatus 	= 'A',
				   		rpudata_inc = now()
				   	where
				   		usucpf  	= '$usucpf' and
				   		pflcod 	    =  $pflcod and
				   		entid    	= '$entid'
				   ";
				
			$sqlQtd = "
				select count(*) as qtd from pdeinterativo.usuarioresponsabilidade 
				where
				   		usucpf  	= '$usucpf' and
				   		pflcod 	    =  $pflcod and
				   		entid  	= '$entid'
				";

			$qtd = $db->pegaUm($sqlQtd);
				
			if($qtd == 0)
			{
				$sql = "";

				$sql = "insert into
				   		pdeinterativo.usuarioresponsabilidade 
					   (
					        rpustatus 	,
					   		rpudata_inc ,					   		
					   		entid  	,
					   		usucpf  	,
					   		pflcod 	
					   	)values(
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
		<tr>
			<td class="SubTituloDireita" align="right">
				Estado
			</td>
			<td>
			<?php
				$estuf = $_REQUEST['estuf'];
				$sql = "select
						 e.estuf as codigo, e.estdescricao as descricao 
						from
						 territorios.estado e 
						order by
						 e.estdescricao asc";
				$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', 'filtro', '' );
			?>				
			</td>
		</tr>
		<?php if ($_POST['estuf']):  ?>
		<tr>
			<td class="SubTituloDireita" align="right">
				Município
			</td>
			<td>
			<?php
				$muncod = $_POST['muncod'];
				$sql = sprintf("SELECT
								 DISTINCT
								 m.muncod AS codigo,
								 m.mundescricao AS descricao
								FROM
								 entidade.entidade e
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
								 INNER JOIN entidade.endereco endi ON endi.entid = e.entid
								 INNER JOIN territorios.municipio m ON m.muncod = endi.muncod
								WHERE
								 fe.funid = 3 AND
								 tpcid IN (%d) AND		 	
								 m.estuf = '%s'
								ORDER BY m.mundescricao",
							$pflcod == 223  ? 3 : 1,
							$_POST['estuf']);
				
				$db->monta_combo( "muncod", $sql, 'S', 'Selecione um município', 'filtro', '' );
			?>				
			</td>
		</tr>
		<?php endif; 

		if ( $pflcod == PDEESC_PERFIL_CAD_MAIS_EDUCACAO || $pflcod == PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL || $pflcod == PDEESC_PERFIL_CAD_ESCOLA_ABERTA ){
			if ($_POST['muncod']):
				?>
					<tr>
						<td class="SubTituloDireita" align="right">
							Tipo de Escola
						</td>
						<td>
							<?php
								
								$tpcid = $_POST["tpcid"];
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
		}
		if ( $pflcod == PDEESC_PERFIL_DIRETOR ){
			if ($_POST['muncod']):
				?>
					<tr>
						<td class="SubTituloDireita" align="right">
							Tipo de Escola
						</td>
						<td>
							<?php
								
								$tpcid = $_POST["tpcid"];
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
		}

		if ($_POST['muncod']): ?>
		<tr>
			<td colspan="2">
			<?php
			if ( $pflcod == PDEESC_PERFIL_CAD_MAIS_EDUCACAO )
			{
				if ( $_POST["tpcid"] ) {
					$sql = sprintf("SELECT DISTINCT
										'<input type=\"checkbox\" name=\"entid\" id=\"\" value=\"'|| et.entid ||'\" onclick=\'retorna( this, \"' || et.entid || '\", \"' || replace(mun.mundescricao || ' - ' || et.entnome,'''','') || '\");\'/>' AS input,
									 	mun.estuf || ' - ' || mun.mundescricao AS local,
									 	CASE WHEN et.tpcid = 1 THEN 'Estadual'
									 	ELSE 'Municipal'
									 END AS tipo,
									 	et.entnome
									FROM
										pdeescola.memaiseducacao me
									INNER JOIN 
										entidade.entidade et ON et.entid = me.entid
									INNER JOIN 
										entidade.endereco ed ON et.entid = ed.entid
									INNER JOIN 
										territorios.municipio mun ON mun.muncod = ed.muncod
									LEFT JOIN pdeescola.usuarioresponsabilidade ur ON ur.entid = et.entid AND
									 	rpustatus = 'A'
									WHERE
										mun.muncod = '%s' AND
										et.tpcid = %d AND
										me.memanoreferencia = ".$_SESSION["exercicio"]." AND
										me.memstatus = 'A'
									ORDER BY
									 et.entnome", $_POST['muncod'], $_POST["tpcid"]);
					
					$cabecalho = array( "Ação", "Localidade", "Tipo", "Escola" );
					$db->monta_lista_simples( $sql, $cabecalho, 500, 10, 'N', '100%', 'N' );
					
				}
				
			}
			else if( $pflcod == PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL)
			{
				if ( $_POST["tpcid"] ) {
					$sql = sprintf("SELECT DISTINCT
										'<input type=\"checkbox\" name=\"entid\" id=\"\" value=\"'|| et.entid ||'\" onclick=\'retorna( this, \"' || et.entid || '\", \"' || replace(mun.mundescricao || ' - ' || et.entnome,'''','') || '\");\'/>' AS input,
									 	mun.estuf || ' - ' || mun.mundescricao AS local,
									 	CASE WHEN et.tpcid = 1 THEN 'Estadual'
									 	ELSE 'Municipal'
									 END AS tipo,
									 	et.entnome
									FROM
										pdeescola.eacescolaacessivel me
									INNER JOIN 
										entidade.entidade et ON et.entid = me.entid
									INNER JOIN 
										entidade.endereco ed ON et.entid = ed.entid
									INNER JOIN 
										territorios.municipio mun ON mun.muncod = ed.muncod
									LEFT JOIN pdeescola.usuarioresponsabilidade ur ON ur.entid = et.entid AND
									 	rpustatus = 'A'
									WHERE
										mun.muncod = '%s' AND
										et.tpcid = %d AND
										me.eacanoreferencia = ".$_SESSION["exercicio"]." AND
										me.eacstatus = 'A'
									ORDER BY
									 et.entnome", $_POST['muncod'], $_POST["tpcid"]);
					
					$cabecalho = array( "Ação", "Localidade", "Tipo", "Escola" );
					$db->monta_lista_simples( $sql, $cabecalho, 500, 10, 'N', '100%', 'N' );
					
				}
			}
			else if( $pflcod == PDEESC_PERFIL_CAD_ESCOLA_ABERTA)
			{
				if ( $_POST["tpcid"] ) {
					$sql = sprintf("SELECT DISTINCT
										'<input type=\"checkbox\" name=\"entid\" id=\"\" value=\"'|| et.entid ||'\" onclick=\'retorna( this, \"' || et.entid || '\", \"' || replace(mun.mundescricao || ' - ' || et.entnome,'''','') || '\");\'/>' AS input,
									 	mun.estuf || ' - ' || mun.mundescricao AS local,
									 	CASE WHEN et.tpcid = 1 THEN 'Estadual'
									 	ELSE 'Municipal'
									 END AS tipo,
									 	et.entnome
									FROM
										pdeescola.eabescolaaberta me
									INNER JOIN 
										entidade.entidade et ON et.entid = me.entid
									INNER JOIN 
										entidade.endereco ed ON et.entid = ed.entid
									INNER JOIN 
										territorios.municipio mun ON mun.muncod = ed.muncod
									LEFT JOIN pdeescola.usuarioresponsabilidade ur ON ur.entid = et.entid AND
									 	rpustatus = 'A'
									WHERE
										mun.muncod = '%s' AND
										et.tpcid = %d AND
										me.eabanoreferencia = ".($_SESSION["exercicio"] = 2010)." AND
										me.eabstatus = 'A'
									ORDER BY
									 et.entnome", $_POST['muncod'], $_POST["tpcid"]);
					
					$cabecalho = array( "Ação", "Localidade", "Tipo", "Escola" );
					$db->monta_lista_simples( $sql, $cabecalho, 500, 10, 'N', '100%', 'N' );
			
				}
			}
			else if( $pflcod == PDEESC_PERFIL_ESCOLA_QUEST_SEESP)
			{
					$sql = sprintf("SELECT DISTINCT
										'<input type=\"checkbox\" name=\"entid\" id=\"\" value=\"'|| et.entid ||'\" onclick=\'retorna( this, \"' || et.entid || '\", \"' || replace(mun.mundescricao || ' - ' || et.entnome,'''','') || '\");\'/>' AS input,
									 	mun.estuf || ' - ' || mun.mundescricao AS local,
									 	CASE WHEN et.tpcid = 1 THEN 'Estadual'
									 	ELSE 'Municipal'
									 END AS tipo,
									 	et.entnome
									FROM
										entidade.entidade et
									INNER JOIN 
										entidade.endereco ed ON et.entid = ed.entid
									INNER JOIN 
										territorios.municipio mun ON mun.muncod = ed.muncod
									INNER JOIN
										pdeescola.eacescolaquestionario eq ON eq.entcodent = et.entcodent
									WHERE
										mun.muncod = '%s' -- AND
									--	et.tpcid = %d
									ORDER BY
									 et.entnome", $_POST['muncod'], $_POST["tpcid"]);
					$cabecalho = array( "Ação", "Localidade", "Tipo", "Escola" );
					$db->monta_lista_simples( $sql, $cabecalho, 500, 10, 'N', '100%', 'N' );
					
			}
			else {
					
				$sql = sprintf("SELECT
								 DISTINCT
								 '<input type=\"checkbox\" name=\"entid\" id=\"\" value=\"'|| ent.entid ||'\" onclick=\'retorna( this, \"' || ent.entid || '\", \"' || replace(mun.mundescricao || ' - ' || ent.entnome,'''','') || '\");\'/>' AS input,
								 est.estuf || ' - ' || mun.mundescricao AS local,
								 CASE
								 	WHEN ent.tpcid = 1 THEN 'Estadual'
								 	ELSE 'Municipal'
								 END AS tipo,
								 ent.entnome
								FROM
								 entidade.entidade ent
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid AND
								 										   ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN pdeinterativo.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								 fe.funid = 3 AND
								 ent.tpcid IN (%s) AND
								 mun.muncod = '%s'
								ORDER BY
								 ent.entnome",
							$pflcod == 223  ? 3 : 1,
							$_POST['muncod']);
							
				$cabecalho = array( "Ação", "Localidade", "Tipo", "Escola" );
				$db->monta_lista_simples( $sql, $cabecalho, 500, 10, 'N', '100%', 'N' );
					
			}

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
	$sql = "select 
				distinct 
				e.entid as codigo, 
				mun.estuf ||'-'|| mun.mundescricao || ' - ' || e.entnome as descricao 
			from 
				pdeinterativo.usuarioresponsabilidade ur 
				INNER JOIN entidade.entidade e on ur.entid = e.entid
				INNER JOIN entidade.endereco ende ON ende.entid = e.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
	 		where 
	 			ur.rpustatus='A' and 
	 			ur.usucpf = '$usucpf' and 
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
/*
function mostraEsconde(estado){
	var estadoAtual = document.getElementById(estado).style.display;
	var objImagem = document.getElementById(estado+'_img');
	if(estadoAtual == 'none'){
		document.getElementById(estado).style.display = 'block';
		
		objImagem.src = '/imagens/menos.gif';
		
	}else{
		document.getElementById(estado).style.display = 'none';
		objImagem.src = '/imagens/mais.gif';
	}
	
}
*/

var campoSelect = document.getElementById("usuunidresp");

/*
if (campoSelect.options[0] && campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}
*/

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