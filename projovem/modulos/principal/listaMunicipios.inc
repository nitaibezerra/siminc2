<?

//Filtra municípios
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


function filtraMunicipio($estuf){
	global $db;
	$sql = "SELECT
				ter.muncod AS codigo,
				ter.mundescricao AS descricao
			FROM
				territorios.municipio ter
			WHERE
				ter.estuf = '$estuf'
			ORDER BY ter.mundescricao";
		
	echo $db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','id="muncod"');

}




include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PROJOVEM";
monta_titulo( $titulo, '' );
echo '<br />';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo('Lista de Municípios', 'Filtro de Pesquisa');


$where = "";
if ($_POST){
	if(!empty($_REQUEST['municipio'])){
		$_SESSION['projovem']['muncod'] = $_REQUEST['municipio'];
		
		if($_REQUEST['termoCompromisso']){
			echo "<script>window.location.href='projovem.php?modulo=principal/termoAdesao&acao=A';</script>";					
		}
	}
	$where.= ($_POST["estuf"]) ? " AND m.estuf = '".$_POST["estuf"]."'" : "";
	$where.= ($_POST["muncod"]) ? " AND m.muncod = '".$_POST["muncod"]."'":"";	


}


?>

<form id="formPesquisaMun" name="formPesquisaMun" method="post" action="">
<input type="hidden" name="municipio" id="municipio">
<input type="hidden" name="termoCompromisso" id="termoCompromisso">
<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>UF:</b></td>
		<td>
			<?
				$estuf = ($uf==''?$_REQUEST['estuf']:$uf); 
				$sql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM
							territorios.estado
						ORDER BY
							estdescricao";
				$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraMunicipio', '', '', '215','','estuf' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td id="td_municipio">
		<? 
			$muncod = ($muncod==''?$_REQUEST['muncod']:$muncod); 
			$sql = "SELECT
						ter.muncod AS codigo,
						ter.mundescricao AS descricao
					FROM
						territorios.municipio ter
					WHERE
						ter.estuf = '$estuf' 
					ORDER BY ter.estuf, ter.mundescricao"; 
			$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','muncod');
		?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_pesquisar" value="Pesquisar" onclick="pesquisar()" />
		</td>
	</tr>
</table>
</form>

<?

$sql = "select distinct
		'<a href=\"#\" onclick=\"cadTermo(\''||m.muncod||'\');\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>' as acao,   
		m.estuf, m.mundescricao
		from territorios.municipio m
		where m.muncod<>'' 
		$where
		order by m.estuf, m.mundescricao";

$alinha = array("center","center","left","center","center");
$tamanho = array("10%","10%","50%","20%","10%");
$db->monta_lista( $sql, array( "Ação", "UF", "Município"), 30, 10, 'N', 'center', '','', $tamanho,$alinha);

?>


<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function filtraMunicipio(estuf) {
	if(estuf!=''){
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
}


var btPesquisa	= document.getElementById("bt_pesquisar");

function pesquisar() {
	btPesquisa.disabled = true;
	document.formPesquisaMun.submit();
}


function cadTermo(muncod) {
	document.getElementById("municipio").value=muncod;
	document.getElementById("termoCompromisso").value=1;
	document.formPesquisaMun.submit();
}
</script>