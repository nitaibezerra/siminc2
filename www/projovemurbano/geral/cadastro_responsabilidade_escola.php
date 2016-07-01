<?
/*
 Sistema Simec
 Setor responsável: SPO-MEC
 Desenvolvedor: Equipe Consultores Simec
 Analista: Cristiano Cabral
 Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
 Módulo:seleciona_unid_perfilresp.php

 */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

$perfis = pegaPerfilGeral();

if(isset($_REQUEST['enviar'])) {
	
	$sql = "UPDATE
			 	projovemurbano.usuarioresponsabilidade 
			SET
			 	rpustatus = 'I' 
			WHERE
			 	usucpf = '$usucpf'  
			 	AND pflcod = $pflcod 
			 	AND entid is not null";
	
	$db->executar($sql);
	
	if($_POST['usuunidresp'][0]){
		foreach($_POST['usuunidresp'] as $entid){
			$sql = "INSERT INTO projovemurbano.usuarioresponsabilidade (entid, usucpf, rpustatus, rpudata_inc, pflcod) 
																   VALUES ('$entid', '$usucpf', 'A',  now(), '$pflcod')";
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
<title>Estados e Municípios</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css'
	href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0"
	MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif"
	border="0" align="absmiddle"> <font color=blue size="2">Aguarde!
Carregando Dados...</font></div>
<?/*flush();*/?>
<DIV
	style="OVERFLOW: AUTO; WIDTH: 496px; HEIGHT: 350px; BORDER: 2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem" id="tabela">
	<script language="JavaScript">
		document.getElementById('tabela').style.visibility = "hidden";
		document.getElementById('tabela').style.display  = "none";
	</script>
	<form name="formulario" method="post" action="">
	
	
	<thead>
		<tr>
			<td valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				colspan="3"><strong>Selecione a escola</strong></td>

		</tr>
		<tr bgcolor="<?=$cor?>">
			<td width="20" align="right">
			</td>
			<td align="left" style="color: blue;">
				<input type="radio"
					   name="polid" id="nenhum"
					   value=""
					   onclick="retorna( this, '', 'Nenhuma' );" />
				Nenhum
			</td>
		</tr>
		<tr>
		<?
		if(in_array(648, $perfis)){
			$coordenador = "INNER JOIN projovemurbano.usuarioresponsabilidade usu on usu.muncod = mu.muncod and usu.usucpf = '{$_SESSION['usucpf']}'";
		}elseif(in_array(647, $perfis)){
			$coordenador = "INNER JOIN projovemurbano.usuarioresponsabilidade usu on usu.estuf = mu.estuf and usu.usucpf = '{$_SESSION['usucpf']}'";
		}
		$cabecalho = 'Selecione a escola';
		
		
		$sql = "SELECT DISTINCT 
					m.munid as codigo, 
					mu.estuf||' - '||mu.mundescricao as descricao 
				FROM projovemurbano.municipio m 
				INNER JOIN projovemurbano.nucleo nuc on nuc.munid = m.munid  
				INNER JOIN territorios.municipio mu on mu.muncod = m.muncod
				$coordenador
				WHERE munstatus='A' AND nuc.nucstatus='A' 
				ORDER BY descricao";
		$RS = @$db->carregar($sql);
		$nlinhas = count($RS)-1;
		$j = 0 ;
		
		for ($i=0; $i<=$nlinhas;$i++)
		{
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			?>
		
		
		<tr bgcolor="<?=$cor?>">
			<td width="20" align="right"><img src="/imagens/mais.gif"
				id="<?=$codigo."_img" ?>" onclick="mostraEsconde('<?=$codigo?>')">&nbsp;</td>
			<td align="left" style="color: blue;"><?=$descricao?></td>
		</tr>
		<tr>
			<td style="height: 0"></td>
			<td style="height: 0">
			<div id="<?=$codigo?>_div" style="display: none;">
			<table width="100%" cellpadding="0" cellspacing="0" border="0">
			<?
			$sql = "SELECT DISTINCT
						ent.entid as codigo, 
						entnome  as descricao
					FROM 
						projovemurbano.nucleo nuc
					INNER JOIN projovemurbano.municipio    mun ON mun.munid = nuc.munid 
					INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid AND nuestatus = 'A'
					INNER JOIN entidade.entidade 	       ent ON nes.entid = ent.entid 
					WHERE 
						munstatus='A' 
						AND mun.munid=$codigo 
						AND nuc.nucstatus='A'
						AND ent.entid NOT IN (SELECT ur.entid 
										      FROM projovemurbano.usuarioresponsabilidade ur 
										      WHERE rpustatus = 'A' AND ur.entid IS NOT NULL  )";
			$dados = $db->carregar($sql);
			if(is_array($dados)){
				foreach ($dados as $dado) {
					if ($cor2 == '#e0e0e0') $cor2 = '#f4f4f4' ; else $cor2='#e0e0e0';
					?>
					<tr bgcolor="<?=$cor2?>">
						<td align="left" style="border: 0">
							<input type="radio"
							name="polid" id="<?=$dado['codigo']?>"
							value="<?=$dado['codigo']?>"
							onclick="retorna( this, '<?= $dado['codigo'] ?>', '<?= addslashes( $dado['descricao'] ) ?>' );" />
							<font size=1><?=$dado['descricao']?></font><br>
							<div>
			<?php 
						$sql = "SELECT 
									'Nucleo '||nuc.nucid||CASE WHEN nuetipo = 'S' THEN ' - SEDE' ELSE ' - ANEXO' END as nucleo
								FROM 
									projovemurbano.nucleo nuc
								INNER JOIN projovemurbano.municipio    mun ON mun.munid = nuc.munid 
								INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid AND nuestatus = 'A'
								INNER JOIN entidade.entidade 	       ent ON nes.entid = ent.entid 
								WHERE 
									munstatus='A' 
									AND mun.munid=$codigo 
									AND nuc.nucstatus='A'
									AND ent.entid = ".$dado['codigo'];
						$nucleos = $db->carregarColuna($sql);
						foreach( $nucleos as $nucleo ){
							echo $nucleo."<br>";
						}
			?>
							</div>
						</td>
					</tr>
				<?
				}
			}else{
			?>
				<tr bgcolor="#f4f4f4">
					<td align="left" style="border: 0">
						Não possui escolas sem diretor
					</td>
				</tr>
			<? 
			}
			?>
			</table>
			</div>
			</td>
		</tr>
		<?}
		?>
</table>
</form>
</div>
<form name="formassocia" style="margin: 0px;" method="POST"><input
	type="hidden" name="usucpf" value="<?=$usucpf?>"> <input type="hidden"
	name="pflcod" value="<?=$pflcod?>"> <input type="hidden" name="enviar"
	value=""> <select multiple size="8" onclick="mostraPolo(this);"
	name="usuunidresp[]" id="usuunidresp" style="width: 500px;"
	class="CampoEstilo">
	<?
	$sql = "SELECT DISTINCT
				ent.entid as codigo, 
				entnome as descricao
			FROM 
				projovemurbano.usuarioresponsabilidade ur 
			INNER JOIN entidade.entidade 	       ent ON ur.entid = ent.entid
			INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid
			WHERE 
				ur.rpustatus='A' 
				AND ur.usucpf = '$usucpf' 
				AND ur.pflcod = $pflcod";
	
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
</select></form>
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding: 3px;" colspan="3"><input
			type="Button" name="ok" value="OK"
			onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok"></td>
	</tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


function mostraEsconde(estado){
	var estadoAtual = document.getElementById(estado+'_div').style.display;
	var objImagem = document.getElementById(estado+'_img');
	if(estadoAtual == 'none'){
		document.getElementById(estado+'_div').style.display = 'block';
		
		objImagem.src = '/imagens/menos.gif';
		
	}else{
		document.getElementById(estado+'_div').style.display = 'none';
		objImagem.src = '/imagens/mais.gif';
	}
	
}


var campoSelect = document.getElementById("usuunidresp");


if (campoSelect.options[0] && campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}


function enviarFormulario(){
	document.formassocia.enviar.value=1;
	document.formassocia.submit();

}


function mostraPolo(objSelect){
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


function retorna( check, polid, poldescricao )
{
	// tira tudo
	for( var i = 0; i < campoSelect.options.length; i++ )
	{
		campoSelect.options[i] = null;
	}
	// tira tudo
	for( var i = 0; i < campoSelect.options.length; i++ )
	{
		campoSelect.options[i] = null;
	}
	if ( check.checked )
	{
		// põe
		campoSelect.options[campoSelect.options.length] = new Option( poldescricao, polid, false, false );
	}
	else
	{
		// tira
		for( var i = 0; i < campoSelect.options.length; i++ )
		{
			if ( campoSelect.options[i].value == polid )
			{
				campoSelect.options[i] = null;
			}
		}
	}
	sortSelect( campoSelect );
}

</script>