<?php
global $db;

//Pesquisar
if($_REQUEST['pesquisaINEP']) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaINEP($_POST);
	exit;
}

//Filtra municípios
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}

$proid = $_REQUEST['proid'];

monta_titulo( 'Pesquisar Códigos INEP', '' );

?>
<style>
<!--
#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 97%;
    text-align: center;
    z-index: 8000;
    height: 90%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 250px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
-->
</style>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="formulario" name="formulario" action="" method="post"	enctype="multipart/form-data">
<div id="erro"></div>
<input type="hidden" value="<?=$proid; ?>" name="proid" id="proid">
<input type="hidden" name="tplid" id="tplid" value="<?=$_REQUEST['tplid'];?>">
<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita"><b>Nome da Escola:</b></td>
		<td>
		<?	$entnome = $_REQUEST['entnome']; 
		echo campo_texto( 'entnome', 'N', 'S', 'Nome da Escola', 40, 500, '', '','','','','id="entnome"', ''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>UF:</b></td>
		<td>
			<!--  <input type="text" value="" name="estcod" id="estcod">-->
			<?
				$estuf = $_REQUEST['estuf']; 
				$sql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM
							territorios.estado
						ORDER BY
							estdescricao";
				$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraMunicipio', '', '', '215','','id="estuf"' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td id="td_municipio">
			<?
				$muncod = $_REQUEST['muncod'];
				$sql = "SELECT
							ter.muncod AS codigo,
							ter.mundescricao AS descricao
						FROM
							territorios.municipio ter
						WHERE
							ter.estuf = '$_REQUEST[estuf]' 
						ORDER BY ter.mundescricao"; 
				$db->monta_combo( "muncod", $sql, ($_REQUEST["estuf"]) ? 'S' : 'N', 'Selecione...', '', '', '', '215', 'N','id="muncod"');
			?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_pesquisar" value="Pesquisar" onclick="submeter()" />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="window.close()" />
		</td>
	</tr>
</table>
</form>


<br />
<? 
	if ($_POST){
		$where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%".$_POST["entnome"]."%')" : "";
		//$where.=($_POST["estcod"]) ? " AND ende.estuf = '".$_POST["estcod"]."'" : "";
		$where.=($_POST["estuf"]) ? " AND ende.estuf = '".$_POST["estuf"]."'" : "";
		$where.=($_POST["muncod"]) ? " AND ende.muncod = '".$_POST["muncod"]."'" : "";
		$where.=($_POST["tplid"]) ? " AND tpl.tplid = '".$_POST["tplid"]."'" : "";
		
		if(!empty($where)){
			
		echo "<table class='tabela' align='center' bgcolor='#f5f5f5' cellspacing='1' cellpadding='4' style='border-bottom:none;'>
				<tr>
					<td align='center' bgcolor='#dedfde'><b>Escolas</b></td>
				</tr>
			</table>";

			$sql = "SELECT	'<center>
								<a style=\"cursor:pointer;\" onclick=\"selecionaINEP(\''||ent.entcodent||'\');\">
									<img src=\"/imagens/alterar.gif \" border=0 title=\"Seleciona Escola\">
								</a>
							</center>' as acao,
						ent.entcodent,
						ent.entnome, 
						ende.estuf, 
						ter.mundescricao
					FROM 
						entidade.entidade ent
						INNER JOIN entidade.funcaoentidade f 
					    	ON f.entid = ent.entid
						INNER JOIN entidade.endereco ende 
							ON ent.entid=ende.entid
						INNER JOIN territorios.municipio ter 
							ON ende.muncod=ter.muncod
						inner join entidade.tipolocalizacao tpl ON tpl.tplid = ent.tplid
					WHERE 
						f.funid in (3,4,11,12,16,18,56)
						AND ent.entstatus='A'
						$where 
					ORDER BY 
						ter.mundescricao,
						ende.estuf,
						ent.entnome";

			$cabecalho 		= array("Ação", "INEP","Escola","UF","Município");
			$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');
		}
		else
			echo "<script>alert('utilize algum filtro para a pesquisa!');</script>";
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

function filtraMunicipio(estuf) {
	//document.getElementById("estcod").value = estuf;
	var destino = document.getElementById("td_municipio");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraMunicipio=true&" + "estuf=" + estuf,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
}

function submeter() {
	document.formulario.submit();
}

function selecionaINEP(inep){
	window.opener.document.getElementById('inepID').value=inep;
	window.opener.passa();
	window.close();
}
</script>
</body>