<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

if( $_REQUEST["requisicao"] == 'listamunicipio' ){
	if ( !empty($_REQUEST["estuf"]) ){
		listaMunicipioIes($_REQUEST["estuf"]);
	}else{
		echo 'Selecione uma UF...';
	}
	die;
}

if( $_REQUEST["requisicao"] == 'listainstituicao' ){
	if ( !empty($_REQUEST["muncod"]) ){
		listaInstituicoesIes($_REQUEST["muncod"]);
	}else{
		echo 'Selecione os filtros acima para listar as instituições...';
	}
	die;
}

if( $acao == 'A' ){
	atribuiInstituicaoIes($_REQUEST);
}

function listaInstituicoesIes( $muncod ){
	
	global $db;
	
	$sql = "SELECT 
				case
					when
						( SELECT count(iesid) FROM ies.usuarioresponsabilidade i WHERE ies.iesid = i.iesid AND i.rpustatus = 'A' ) > 0
					then
						'<center>
							<input type=\"Checkbox\" name=\"unicod[]\" checked id=\"' || ies.iesid || '\" value=\"' || ies.iesid || '\">
						</center>'
					else
						'<center>
							<input type=\"Checkbox\" name=\"unicod[]\" id=\"' || ies.iesid || '\" value=\"' || ies.iesid || '\">
						</center>'
				end as acao, 
				ies.iescodigo,
				ies.iesnome 
			FROM
				ies.ies ies
			INNER JOIN
				territorios.municipio mun ON mun.muncod = '{$muncod}'
			WHERE
				UPPER(trim(ies.iesmunicipio)) like UPPER(trim(mun.mundescricao))
			ORDER BY
				ies.iesnome";
	
	$cabecalho = array( "Ação", "Código", "Nome da Instituição" );
		
	$db->monta_lista( $sql, $cabecalho, 10000, 10, 'N','center', '', '', '', '' );
	
}

function atribuiInstituicaoIes( $dados ){
	
	global $db;

	//dbg($dados, 1);
	
	$db->executar( "UPDATE ies.usuarioresponsabilidade SET rpustatus = 'I' 
					WHERE usucpf = '{$dados["usucpf"]}' AND pflcod = {$dados["pflcod"]}" );

	foreach ($dados['unicod'] as $uni){
		$db->executar( "INSERT INTO ies.usuarioresponsabilidade ( pflcod, usucpf, rpustatus, rpudata_inc, iesid ) 
						VALUES ( {$dados["pflcod"]}, '{$dados["usucpf"]}', 'A', 'now', {$uni} )" );
	}
	
	$db->commit();
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.reload();
			self.close();
		</script>";
	
}

function listaMunicipioIes( $estuf ){
	
	global $db;
	
	$sql = "SELECT
				tm.muncod as codigo,
				tm.mundescricao as descricao
			FROM
				territorios.municipio tm
			WHERE
				estuf = '{$estuf}'
			ORDER BY
				descricao";
	
	$db->monta_combo( "muncod", $sql, "S", "Todas", "iesListaInstituicao(this.value);", "", "", "", "N", "muncod" );
	
}

print "<br/>";
monta_titulo( "Lista de Instituições", "" );

?>

<script type="text/javascript" src="../../includes/prototype.js"></script>
<script>
	function insereInstituicao( iesid ){
		document.getElementById('iesid').value = iesid;
		document.formassocia.submit();
	}
	
	function iesListaMunicipio( uf ){
	
		var url = '/ies/geral/cadastro_responsabilidade_instituicao.php?requisicao=listamunicipio&estuf=' + uf;
	
		var myAjax = new Ajax.Updater(
			"municipio",
			url,
			{
				method: 'post',
				asynchronous: false
		});
		
	}
	
	function iesListaInstituicao( muncod ){
	
		var url = '/ies/geral/cadastro_responsabilidade_instituicao.php?requisicao=listainstituicao&muncod=' + muncod;
	
		var myAjax = new Ajax.Updater(
			"div_rolagem",
			url,
			{
				method: 'post',
				asynchronous: false
		});
		
	}
</script>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Instituições</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
		<style>
			#div_rolagem table {
				width: 100%;                
			}
       </style>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<form name="formassocia" action="?acao=A" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<input type="hidden" name="iesid" id="iesid" value="">
			<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<td class="subtitulocentro" colspan="2">Filtros de Pesquisa</td>
				</tr>
				<tr>
					<td class="subtitulodireita">UF</td>
					<td>
						<?php
						 
							$estuf = $_REQUEST["estuf"];
				
							$sql = "SELECT DISTINCT
										iesuf as codigo,
										iesuf as descricao
									FROM
										ies.ies
									WHERE
										iesuf <> ''
									ORDER BY
										descricao";
							
							$db->monta_combo( "estuf", $sql, "S", "Todas", "iesListaMunicipio(this.value);", "", "", "", "N", "estuf" );
							
						?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Municípío</td>
					<td id="municipio"  style="color: #C0C0C0;">Selecione uma UF...</td>
				</tr>
				<tr>
					<td colspan="2">
						<center>
							<div id="div_rolagem" style=" color: #C0C0C0; background-color: #FFFFFF; overflow-x: auto; overflow-y: auto; width:100%; height:300px;">
								Selecione os filtros acima para listar as instituições...
							</div>
						</center>
					</td>
				</tr>
				<tr bgcolor="#D0D0D0">
					<td colspan="2">
						<input type="button" value="OK" onclick="insereInstituicao('')" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>	
		</form>
	</body>
</html>