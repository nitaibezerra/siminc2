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

unset($_SESSION['pse']['entid']);
unset($_SESSION['pse']['uf']);
unset($_SESSION['pse']['mundescricao']);
unset($_SESSION['pse']['muncod']);
unset($_SESSION['pse']['tipo']);

$_SESSION['pse']['portaria'] = $_SESSION['exercicio'] - 1;

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo, '' );
echo '<br />';
//$db->cria_aba( $abacod_tela, $url, '' );

$menu = carregarMenuAbasPse();
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Unidade Local Integrada', 'Filtro de Pesquisa');

$where = "";
if ($_POST){
	if(!empty($_REQUEST['entid'])){
		$_SESSION['pse']['entid'] = $_REQUEST['entid'];
		$_SESSION['pse']['tipo'] = 3;
		echo "<script>window.location.href='pse.php?modulo=principal/cadastroEstadoMunicipioArvore&acao=A';</script>";
	}
	$where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%".$_POST["entnome"]."%')" : "";
	$where.=($_POST["estuf"]) ? " AND ende.estuf = '".$_POST["estuf"]."'" : "";
	$where.=($_POST["muncod"]) ? " AND ende.muncod = '".$_POST["muncod"]."'" : "";
}

include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
?>

<form id="formPesquisaEscola" name="formPesquisaEscola" method="post" action="">
<input type="hidden" name="entid" id="entid">
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
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Unidades</b></td>
	</tr>
</table>
<?
/*
$sql = "SELECT DISTINCT
			'<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			' as acao,
			 ent.entnome, m.estuf, m.mundescricao  
		FROM entidade.entidade ent				
		LEFT JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.entidade2 ent2 ON ent2.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod
		INNER JOIN
			pse.escolapse ep ON ep.entid=ent.entid
		WHERE	ent.entstatus='A' AND
				ep.espparticipapse2009 = 't' AND 
				ent2.funid = 3 
				$where
		ORDER BY m.estuf,m.mundescricao,ent.entnome";
	*/			
$sql = "SELECT DISTINCT
			'<a href=\"#\" onclick=\"EditarCadastro(\'' || ent.entid || '\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			' as acao,
			 ent.entnome, m.estuf, m.mundescricao, case when pp.porano = 2009 then 2010 else pp.porano end as porano
		FROM entidade.entidade ent				
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.funcaoentidade fe ON fe.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod
		INNER JOIN
			pse.escolapse ep ON ep.entid=ent.entid
		INNER JOIN
			pse.portariapse pp ON pp.pormunicipio = m.muncod
		WHERE	ent.entstatus='A' AND 
			--	ep.espparticipapse2009 = 't' AND 
				fe.funid = 3 AND
				ent.tpcid IN (1, 3)
				AND pp.porano <= ".$_SESSION['pse']['portaria']."
				$where
				AND ent.entid in (
							SELECT
								entid
							FROM
								(SELECT ent.entid,
												(select count(entid) from pse.equipesaudefamilia eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano = ".$_SESSION['exercicio'].") as p2,
												(select count(entid) from pse.educadoreferencia eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano = ".$_SESSION['exercicio'].") as p3,
												(select count(pp.nieid) from pse.escolapse ep inner join pse.equipesaudefamilia ef on ef.espid = ep.espid inner join pse.pseanoescola pp on ef.esfid = pp.esfid where ep.entid = ent.entid
										--			AND ep.espano = ".$_SESSION['exercicio']."
												) as p4,
												(select count(entid) from pse.atorequipe eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano = ".$_SESSION['exercicio'].") AS p5								
											FROM
												entidade.entidade ent
											INNER JOIN
												entidade.funcaoentidade fe ON fe.entid = ent.entid
											WHERE
												ent.entstatus='A' 
												AND fe.funid = 3									
												AND (ent.tpcid = 1 or ent.tpcid = 3)
								) as f
							WHERE
								((f.p2 <> 0) AND
								(f.p3 <> 0) AND
								(f.p4 <> 0) AND
								(f.p5 <> 0))
							)
		ORDER BY m.estuf,m.mundescricao,ent.entnome";			

$cabecalho 		= array( "Ação", "Escola", "UF", "Município", "Ano");
$tamanho		= array( '10%', '60%', '10%', '15%', '5%' );
$alinhamento	= array( 'center', 'left', 'center', 'center', 'center');
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento );

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
	document.formPesquisaEscola.submit();
}

function EditarCadastro(entid) {
	document.getElementById("entid").value=entid;
	document.formPesquisaEscola.submit();
}

</script>