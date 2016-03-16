<?php
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
ini_set("memory_limit","512M");

$db 	= new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

if($_REQUEST['enviar']) {

	$sql = "update
			 eja.usuarioresponsabilidade 
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
				   		eja.usuarioresponsabilidade 
				   set  
				   		rpustatus 	= 'A',
				   		rpudata_inc = now()
				   	where
				   		usucpf  	= '$usucpf' and
				   		pflcod 	    =  $pflcod and
				   		entid    	= '$entid'
				   ";
				
			$sqlQtd = "
				select count(*) as qtd from eja.usuarioresponsabilidade 
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
				   		eja.usuarioresponsabilidade 
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
<?flush();?>
<form name="formlista" id="formlista" method="post" action="cadastro_responsabilidade_escola.php?pflcod=<? echo $_REQUEST['pflcod']; ?>&usucpf=<? echo $_REQUEST['usucpf']; ?>">	
<DIV id="rolagem">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<tr>
			<td class="SubTituloDireita" align="right">
				Estado
			</td>
			<td>
			<?php
				$tpcid = (($pflcod == SECRETARIA_ESTADUAL || $pflcod == ESCOLA_ESTADUAL) ? 1 : 3);
			
				$estuf = $_REQUEST['estuf'];
				$sql = "select
						 e.estuf as codigo, e.estdescricao as descricao 
						from
						 territorios.estado e 
						order by
						 e.estdescricao asc";
				$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades da Federação', 'filtro', '' );
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
								 tpcid IN (1,3) AND
								 m.estuf = '%s'
								ORDER BY m.mundescricao",
							$_POST['estuf']);
							
				$db->monta_combo( "muncod", $sql, 'S', 'Selecione um município', 'filtro', '' );
			?>				
			</td>
		</tr>
		<?php endif; 

		if ($_POST['muncod']): ?>
		<tr>
			<td colspan="2">
			<?php
				$input = <<<EOT
					 '\n\n<input type="checkbox" name="entid" id="" value="'|| ent.entid ||'" onclick="retorna( this, \'' || ent.entid || '\', \'' || replace(mun.mundescricao || ' - ' || ent.entnome,'''','') || '\');"/>\n\n' AS input
EOT;
				

				$sql = sprintf("SELECT
								 DISTINCT
								{$input},
								 est.estuf || ' - ' || mun.mundescricao AS local,
								 CASE
								 	WHEN ent.tpcid = 1 THEN 'Estadual'
								 	ELSE 'Municipal'
								 END AS tipo,
								 ent.entnome
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								 ent.entstatus = 'A' AND
								 fe.funid in (3,4) AND
								 ent.tpcid IN (1,3,5) AND
								 mun.muncod = '%s'	 
								ORDER BY
								 ent.entnome",
							$_POST['muncod']);
				//dbg($sql,1);
				$cabecalho = array( "Ação", "Localidade", "Tipo", "Escola" );
				$db->monta_lista_simples( $sql, $cabecalho, 500000, 10, 'N', '100%', 'N' );
			?>	
			</td>		
		</tr>
		<?php endif; ?>
	</thead>
</table>
</DIV>
</form>

<form name="formassocia" id="formassocia" style="margin: 0px;" method="post" action="cadastro_responsabilidade_escola.php">
	<input type="hidden" name="usucpf" value="<?=$usucpf?>"> 
	<input type="hidden" name="pflcod" value="<?=$pflcod?>"> 
	<input type="hidden" name="enviar" value=""> 
	<select maximo="0" tipo="combo_popup" multiple="multiple" 
			name="usuunidresp[]" 
			id="usuunidresp" 
			style="width: 500px;" 
			class="CampoEstilo">
	<?php
	$sql = "select 
				distinct 
				e.entid as codigo, 
				mun.estuf ||'-'|| mun.mundescricao || ' - ' || e.entnome as descricao 
			from 
				eja.usuarioresponsabilidade ur 
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
			<input type="Button" name="ok" value="OK" onclick="enviarFormulario();" id="ok">
		</td>
	</tr>
</table>
<script language="JavaScript" type="text/javascript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";

function filtro(){
	document.getElementById('formlista').submit();
}

function enviarFormulario(){
	var campoSelect = document.getElementById("usuunidresp");
	selectAllOptions(campoSelect);
	document.formassocia.enviar.value=1;
	document.getElementById('formassocia').submit();
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
	var campoSelect = document.getElementById("usuunidresp");
	if ( check.checked ) {
		/*
		if (campoSelect.options.length >= 1){
			//alert('Não é permitido marcar mais de uma escola por usuário!');
			//check.checked = false;
			//return;
			if(confirm('Já existe um escola marcada para o usuário!\nDeseja realmente alterar a escola?')){
				for( var i = 0; i < campoSelect.options.length; i++ )
				{
					campoSelect.options[i] = null;
				}
			}
			else {
				check.checked = false;
				return;
			}
			
		}	
		*/

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
<style type="text/css">

#rolagem{
	OVERFLOW: AUTO;
	WIDTH: 496px;
	HEIGHT: 350px; 
	BORDER: 2px SOLID #ECECEC; 
	background-color: White;
}

</style>