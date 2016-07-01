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
unset($_SESSION['pse']['flagmun']);
unset($_SESSION['pse']['empid']);

$_SESSION['pse']['portaria'] = $_SESSION['exercicio'] - 1;

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PSE - Programa Saúde na Escola";
if( $_SESSION['exercicio'] == 2009 ){
	$portaria = "<b>Portaria 2.931 de 4 de dezembro de 2008</b>";
} elseif( $_SESSION['exercicio'] == 2010 ){
	$portaria = "<b>Portaria 1.537 de 15 de junho de 2010</b>";
}
monta_titulo( $titulo, $portaria );
echo '<br />';
//$db->cria_aba( $abacod_tela, $url, '' );

$menu = carregarMenuAbasPse();
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


monta_titulo('Lista de Municípios', 'Filtro de Pesquisa');

$where = "";
if ($_POST){
	if(!empty($_REQUEST['municipio']) && !empty($_REQUEST['estmun'])){
		$_SESSION['pse']['muncod'] = $_REQUEST['municipio'];
		$_SESSION['pse']['tipo'] = 2;
		echo "<script>window.location.href='pse.php?modulo=principal/cadastroEstadoMunicipioArvore&acao=A';</script>";
	}
	$where.= ($_POST["estuf"]) ? " AND m.estuf = '".$_POST["estuf"]."'" : "";
	$where.= ($_POST["muncod"]) ? " AND m.muncod = '".$_POST["muncod"]."'":"";	
	$pse = ($_POST["pse2009"]==''?'':$_POST["pse2009"]);
	$where.= ($pse!='' ? " AND p.empparticipapse2009 = '".$pse."'" : "");

}

include  APPRAIZ . 'pse/modulos/principal/permissao.inc';

if($pflcod == SECRETARIA_MUNICIPAL){
	$empflagestmun = 'm';
} else {
	$empflagestmun = 'e';
}
?>

<form id="formPesquisaMun" name="formPesquisaMun" method="post" action="">
<input type="hidden" name="municipio" id="municipio">
<input type="hidden" name="estmun" id="estmun">
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
				$db->monta_combo( "estuf", $sql, $comboUFhab, 'Selecione...', 'filtraMunicipio', '', '', '215','','estuf' );
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
			$db->monta_combo( "muncod", $sql, $comboMUNhab, 'Selecione...', '', '', '', '215', 'N','muncod');
		?>
		</td>
	</tr>
	<!--<tr>
		<td class="SubTituloDireita" valign="top"><b>PSE 2009?:</b></td>
			<td>
				<?php
					$checkNao = "";
					$checkSim = "";
					$check = "";
					$pse = ($pse==''?'':$pse);
					if($pse!="")
						$pse == 0 ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
					else
						$check = "checked=checked";
				?>
				<input type="radio" name="pse2009" id="pse2009" value="" <?=$check?>>.Todos</input>
				<input type="radio" name="pse2009" id="pse2009" value="1" <?=$checkSim?>>Sim</input>
				<input type="radio" name="pse2009" id="pse2009" value="0" <?=$checkNao?>>Não</input>
			</td>
	</tr>
	--><tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_pesquisar" value="Pesquisar" onclick="pesquisar()" />
		</td>
	</tr>
</table>
</form>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Secretarias de Educação</b></td>
	</tr>
</table>
<?

$sql = "select distinct
		case when portaria.pormunicipio = m.muncod then 
		'<a href=\"#\" onclick=\"EditarCadastro(\''||m.muncod||'\',\'mu\');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
		' else '-' end as acao,   
		m.estuf, m.mundescricao
	 ,
	-- case
	-- 		  when (select count(x.empparticipapse2009) from pse.estadomunicipiopse x WHERE x.muncod = p.muncod)>1 then '<font color=\'0000FF\'>Duas Respostas</font>'
	--	      when p.empparticipapse2009 = '0' then '<font color=\'FF0000\'>Não</font>'
	--	      when p.empparticipapse2009 = '1' then '<font color=\'0000FF\'>SIM</font>'
	--	      when p.empparticipapse2009 is null then 
	--	      	( case when portaria.pormunicipio = m.muncod then '<font color=\'228B22\'>Sem cadastro</font>'
	--	      	else '<font color=\'228B22\'>Sem acesso ao PSE</font>' end )
	--	end as PSE2009
		case when portaria.porano = 2009 then 2010 else portaria.porano end as porano
		from territorios.municipio m
		LEFT JOIN
			pse.estadomunicipiopse p ON p.muncod=m.muncod
		LEFT JOIN
			pse.portariapse portaria ON portaria.pormunicipio = m.muncod
		where m.muncod<>'' 
		$where
		--AND (p.empflagestmun = '$empflagestmun' OR p.empflagestmun is null) 
		AND portaria.porano <= ".$_SESSION['pse']['portaria']."
		order by m.estuf, m.mundescricao";

//$db->monta_lista( $sql, array( "Ação", "UF", "Município" ), 30, 10, 'N', '', '' );
$alinha = array("center","center","left","center","center");
$tamanho = array("10%","10%","50%","20%","10%");
$db->monta_lista( $sql, array( "Ação", "UF", "Município","Portaria de Adesão" ), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
//$db->monta_lista( $sql, array( "Ação", "UF", "Município","PSE 2009?" ), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
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

function EditarCadastro(muncod,tipo) {
	document.getElementById("municipio").value=muncod;
	document.getElementById("estmun").value=tipo;
	document.formPesquisaMun.submit();
}
</script>