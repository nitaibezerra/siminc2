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

monta_titulo('Lista de Escolas', 'Filtro de Pesquisa');

$where = "";
if ($_POST){
	if(!empty($_REQUEST['entid'])){
		$_SESSION['pse']['entid'] = $_REQUEST['entid'];
		$_SESSION['pse']['tipo'] = 1;
		echo "<script>window.location.href='pse.php?modulo=principal/cadastroEstadoMunicipioArvore&acao=A';</script>";
	}
	$where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%".$_POST["entnome"]."%')" : "";
	$where.=($_POST["estuf"]) ? " AND ende.estuf = '".$_POST["estuf"]."'" : "";
	$where.=($_POST["muncod"]) ? " AND ende.muncod = '".$_POST["muncod"]."'" : "";
	$where.=($_POST["entcodent"]) ? " AND ent.entcodent = '".$_POST["entcodent"]."'" : "";
	if($pflcod <> ESCOLA_MUNICIPAL && $pflcod <> ESCOLA_ESTADUAL){ 
		$where.=($_POST["pse2009"]) ? " AND ep.espparticipapse2009 = '".$_POST["pse2009"]."'" : "";
		$pse = $_POST["pse2009"];
	}

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
	</tr><!--
	<?php if($pflcod <> ESCOLA_MUNICIPAL && $pflcod <> ESCOLA_ESTADUAL){?> 
	<tr>
		<td class="SubTituloDireita" valign="top"><b>PSE 2009?:</b></td>
			<td>
				<?php
					$checkNao = "";
					$checkSim = "";
					$check = "";
					$pse = ($pse==''?'':$pse);
					if($pse!="")
						$pse != "t" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
					else
						$check = "checked=checked";
				?>
				<input type="radio" name="pse2009" id="pse2009" value="" <?=$check?>>Todos</input>
				<input type="radio" name="pse2009" id="pse2009" value="t" <?=$checkSim?>>Sim</input>
				<input type="radio" name="pse2009" id="pse2009" value="f" <?=$checkNao?>>Não</input>
			</td>
	</tr>
	<?php }?>
	--><tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_pesquisar" <?=$btHab;?> value="Pesquisar" onclick="pesquisar()" />
		</td>
	</tr>
</table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Escolas</b></td>
	</tr>
</table>
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
			 ent.entnome, m.estuf, m.mundescricao";
if($pflcod <> ESCOLA_MUNICIPAL && $pflcod <> ESCOLA_ESTADUAL){ 
/*	$sql .=" ,case
		      when ep.espparticipapse2009 = 'f' then '<font color=\'FF0000\'>Não</font>'
		      when ep.espparticipapse2009 = 't' then '<font color=\'0000FF\'>SIM</font>'
		      when ep.espparticipapse2009 is null then '<font color=\'228B22\'>Sem cadastro</font>'
		end as PSE2009";*/
$sql .=", case when pp.porano = 2009 then 2010 else pp.porano end as porano";	
} 	 
$sql .=" FROM entidade.entidade ent				
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.funcaoentidade fe ON fe.entid = ent.entid
		INNER JOIN
			territorios.municipio m ON m.muncod = ende.muncod
		INNER JOIN
			pse.portariapse pp ON pp.pormunicipio = m.muncod";

if($pflcod <> ESCOLA_MUNICIPAL && $pflcod <> ESCOLA_ESTADUAL){			
	$sql .= "			
		LEFT JOIN
			pse.escolapse ep ON ep.entid=ent.entid";
}
$sql .= "			
		WHERE	ent.entstatus='A' AND 
				fe.funid = 3 AND
				ent.tpcid in (1, 3)
				AND pp.porano <= ".$_SESSION['pse']['portaria']."
				AND ent.entid IN ( select entid from pse.usuarioresponsabilidade where pflcod in (320,321) AND rpustatus = 'A')
				$where
		ORDER BY m.estuf,m.mundescricao,ent.entnome";

if($pflcod <> ESCOLA_MUNICIPAL && $pflcod <> ESCOLA_ESTADUAL){				
	$cabecalho 		= array( "Ação", "Escola", "UF", "Município", "Ano");
	$tamanho		= array( '10%', '50%', '10%', '20%','10%' );
	$alinhamento	= array( 'center', 'left', 'center', 'center','center');
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
}
else {
	$cabecalho 		= array( "Ação", "Escola", "UF", "Município");
	$tamanho		= array( '10%', '50%', '10%', '30%');
	$alinhamento	= array( 'center', 'left', 'center', 'center');
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinhamento);	
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
	btPesquisa.disabled = true;
	document.formPesquisaEscola.submit();
}

function EditarCadastro(entid) {
	document.getElementById("entid").value=entid;
	document.formPesquisaEscola.submit();
}

</script>