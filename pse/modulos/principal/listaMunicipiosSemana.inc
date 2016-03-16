<?
 
//Filtra municípios
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	//header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}

//pega perfil do usuario
$pflcod = pegaPerfil($_SESSION['usucpf']);


include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo, '' );
echo '<br />';
//$db->cria_aba( $abacod_tela, $url, '' );

$menu = carregarMenuAbasPse();
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Lista de Escolas', 'Filtro de Pesquisa');

$where = "";
if ($_POST){
	if(!empty($_REQUEST['entid'])){
		$_SESSION['pse']['entid'] = $_REQUEST['entid'];
		$_SESSION['pse']['sse'] = "S";
		
		$sql = "SELECT muncod FROM entidade.endereco WHERE entid = '{$_SESSION['pse']['entid']}'";
		$_SESSION['pse']['muncod'] = $db->pegaUm($sql);
		 
		echo "<script>window.location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A';</script>";
		exit;
	}
	$where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%".$_POST["entnome"]."%')" : "";
	$where.=($_POST["estuf"]) ? " AND ende.estuf = '".$_POST["estuf"]."'" : "";
	$where.=($_POST["muncod"]) ? " AND ende.muncod = '".$_POST["muncod"]."'" : "";
	$where.=($_POST["entcodent"]) ? " AND ent.entcodent = '".$_POST["entcodent"]."'" : "";
}


//cria sessoes para o pse (nao retirar a linha abaixo)
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';

?>
<form id="formPesquisaEscola" name="formPesquisaEscola" method="post" action="">
	<input type="hidden" name="entid" id="entid">
<?
if ($pflcod != ESCOLA_MUNICIPAL && $pflcod != ESCOLA_ESTADUAL && $pflcod != DIRETOR_ESCOLA && $pflcod != EDUCADOR_ESCOLA){
	?>
	
	<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top"><b>INEP:</b></td>
			<td>
			<? 
				$entcodent = $_REQUEST["entcodent"];
				echo campo_texto('entcodent', 'N', 'S', 'Código INEP', 50, 500, '', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top"><b>Nome da Escola:</b></td>
			<td>
			<? 
				$entnome = $_REQUEST["entnome"];
				echo campo_texto('entnome', 'N', 'S', 'Nome da Escola', 50, 500, '', '');
			?>
			</td>
		</tr>
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
					$db->monta_combo( "estuf", $sql, $comboUFhab, 'Selecione...', 'filtraMunicipio', '', '', '215','','','',$estuf );
	
	 
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
				$db->monta_combo( "muncod", $sql, $comboMUNhab, 'Selecione...', '', '', '', '215', 'N','','','',$muncod);
			?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#c0c0c0"></td>
			<td align="left" bgcolor="#c0c0c0">
				<input type="button" id="bt_pesquisar" <?=$btHab;?> value="Pesquisar" onclick="pesquisar()" />
			</td>
		</tr>
	</table>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td align="center" colspan="2"><b>Lista de Escolas</b></td>
		</tr>
	</table>
	<?
}

?>
</form>
<?

/*
$sql = "SELECT DISTINCT
			'<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			' as acao,
			 ent.entnome, m.estuf, m.mundescricao  
		FROM entidade.entidade ent				
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.funcaoentidade fe ON fe.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod";
*/
$sql = "SELECT DISTINCT
			'<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			' as acao,
			 ent.entnome, m.estuf, m.mundescricao
		FROM entidade.entidade ent				
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.funcaoentidade fe ON fe.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod
		WHERE	ent.entstatus='A' AND 
				fe.funid = 3 AND
				ent.tpcid in (1, 3)
				$where
		ORDER BY m.estuf,m.mundescricao,ent.entnome";

$cabecalho 		= array( "Ação", "Escola", "UF", "Município");
$tamanho		= array( '10%', '50%', '10%', '30%');
$alinhamento	= array( 'center', 'left', 'center', 'center');

if($pflcod == MEC || $pflcod == SUPER_USUARIO){
	if($_POST){
		$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
	}
}
else{
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
}

	
//menssagem de aviso	
if(!$_POST){
	$texto = "<div style=\"font-size:12px\" >
					<center><b><font color=red>ATENÇÃO</font></b></center>
					<br>
					<font color=black>O PERÍODO PARA LANÇAMENTO DAS INFORMAÇÕES FINALIZOU NO DIA <b>30/11/2012</b> !</font>
					<br>
					<br>
					<font color=red>A ABA DE JUSTIFICATIVA DE NÃO CUMPRIMENTO DE META, ESTÁ DISPONÍVEL PARA PREENCHIMENTO PELO PERFIL SECRETARIA MUNICIPAL, DO SISTEMA SIMEC/PSE, ATÉ O DIA <b>10/12/2012</b> !</font>
					<br><br> Atenciosamente,
					<br> Equipe Saúde na Escola.
		 		  </div>";
	//popupAlertaGeral($texto,'550px',"220px");
}
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
	<?if($pflcod == MEC || $pflcod == SUPER_USUARIO){?>
		if( document.formPesquisaEscola.entcodent.value == '' && 
			document.formPesquisaEscola.entnome.value == '' &&
			document.formPesquisaEscola.estuf.value == '' &&
			document.formPesquisaEscola.muncod.value == ''){
			alert("Preencha pelo menos um item para pesquisar.");
			return false;
		}
	<?}?>
	
	btPesquisa.disabled = true;
	document.formPesquisaEscola.submit();
}

function EditarCadastro(entid) {
	document.getElementById("entid").value=entid;
	document.formPesquisaEscola.submit();
}

</script>