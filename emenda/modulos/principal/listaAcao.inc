<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

unset($_SESSION["iniid"]);

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

if($_REQUEST["excluir_iniciativa"]) {
	$sql = "UPDATE emenda.iniciativa SET inistatus = 'I' WHERE iniid = " . $_REQUEST["excluir_iniciativa"];
	$db->executar($sql);
	$sql = "UPDATE emenda.iniciativaresponsavel SET irestatus = 'I' WHERE iniid = " . $_REQUEST["excluir_iniciativa"];
	$db->executar($sql);
	$db->commit();
	
	echo "<script type=\"text/javascript\">
			alert(\"Iniciativa excluída com sucesso.\");
		  </script>";
}

$db->cria_aba( $abacod_tela, $url, '' );
$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

//echo montarAbasArray(carregaAbasEmendas('listaAcao'), "/emenda/emenda.php?modulo=principal/listaAcao&acao=A");
monta_titulo('Listar Iniciativas', 'Filtro de Pesquisa');

?>

<form id="formPesquisaIniciativa" method="post" action="">
<input type="hidden" id="excluir_iniciativa" name="excluir_iniciativa" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Responsável:</b></td>
		<td>
		<?=comboResponsavel($_SESSION['usucpf'], $_REQUEST['resid']);?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Iniciativa:</b></td>
		<td>
		<? 
			$ininome = $_REQUEST["ininome"];
			echo campo_texto('ininome', 'N', 'S', 'Iniciativa a ser executada', 50, 500, '', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Grupo de Natureza de Despesa:</b></td>
		<td>
		<? 
			$gndcod = $_REQUEST["gndcod"];
			$sql = "SELECT gndcod as codigo,gndcod || ' - ' || gnddsc as descricao FROM public.gnd WHERE gndstatus = 'A'";
			$db->monta_combo("gndcod", $sql, 'S', "Selecione...", '', '', '', '265', 'N', '');
		?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_pesquisar" value="Pesquisar" onclick="javascript:pesquisarIniciativa();" />
			&nbsp;
			<?
			if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
				$retorno = '';
				/*if(!possuiPermissao($categoria)) {
					$retorno = 'disabled="disabled"';
				}*/
			} else {
				$retorno = 'disabled="disabled"';
			}
			?>
			<input type="button" id="bt_novainiciativa" value="Nova Iniciativa" onclick="javascript:novaIniciativa();" <?=$retorno;?> />
		</td>
	</tr>
</table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Iniciativas Cadastradas</b></td>
	</tr>
</table>

<?

$where = "";

// filtro: 'Responsável'
$responsaveis = recuperaResponsaveis($_SESSION["usucpf"]);
$perfil = pegaPerfil( $_SESSION['usucpf'] );

if($responsaveis) {
	$where .= ($_REQUEST["resid"]) ? " AND inr.resid = ".$_REQUEST["resid"]."" : " AND inr.resid in (".implode(",", $responsaveis).")";
} else {
	$where .= ($_REQUEST["resid"]) ? " AND inr.resid = ".$_REQUEST["resid"]."" : "";
}
// filtro: 'Iniciativa'
$where .= ($_REQUEST["ininome"]) ? " AND UPPER(ini.ininome) like '%".str_to_upper( trim($_REQUEST["ininome"]) )."%'" : "";
//filtro: 'Grupo de Natureza de Despesa'
$where .= ($_REQUEST["gndcod"]) ? " AND ini.gndcod = ".$_REQUEST["gndcod"]."" : "";

if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
	$acao = "'<a href=\"#\" onclick=\"alterarIniciativa(' || ini.iniid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
  			  <a href=\"#\" onclick=\"excluirIniciativa(' || ini.iniid || ');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
} else {
	$acao = "'<a href=\"#\" onclick=\"alterarIniciativa(' || ini.iniid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
  			  <a href=\"#\" title=\"Excluir\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
}

$sql = "SELECT distinct
			".$acao." as comando,
			ini.ininome as iniciativa,
			ini.gndcod as gnd,
			ini.iniid,
			CASE WHEN (SELECT idc.idoid FROM emenda.iniciativadocumentos idc 
            			WHERE idc.iniid = ini.iniid 
                        	and idc.idostatus = 'A' LIMIT 1 ) is not null THEN 'Sim' ELSE 'Não' END as minuta
		FROM
			emenda.iniciativa ini
			left join emenda.iniciativaresponsavel inr
			  	on inr.iniid = ini.iniid
			  	and inr.irestatus = 'A'
		WHERE
			ini.inistatus = 'A'			
			".$where."
		ORDER BY
			ini.ininome";

$arIniciativa = $db->carregar( $sql );
$arIniciativa = $arIniciativa ? $arIniciativa : array();

foreach ($arIniciativa as $chave => $val) {
	
	$sql = "SELECT 
				res.resdsc
			FROM 
			  emenda.responsavel res
			  inner join emenda.iniciativaresponsavel inr
			  	on res.resid = inr.resid
			WHERE
				inr.iniid = ".$val['iniid']."
				and inr.irestatus = 'A'";
	
	$arResp = $db->carregarColuna( $sql );
	
	$arDados[$chave] = array("comando" => $val['comando'],
		         		   "responsavel" => implode(', ', $arResp),
		         		   "iniciativa" => $val['iniid'].' - '.$val['iniciativa'],
						   "gnd" => $val['gnd'],
						   "minuta" => $val['minuta']);
}


$cabecalho 		= array( "Comando", "Responsável", "Iniciativa", "GND", "Existe Minuta cadastrada");
$tamanho		= array( '15%', '10%', '60%', '5%', '10%' );
$alinhamento	= array( 'center', 'left', 'left', 'center', 'center' );
$db->monta_lista_array($arDados, $cabecalho, 5000, 20, '', 'center', '');
//$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);

?>
<script type="text/javascript">

var btPesquisa			= document.getElementById("bt_pesquisar");
var btNovaIniciativa	= document.getElementById("bt_novainiciativa");
var hdExcluir			= document.getElementById("excluir_iniciativa");
var form				= document.getElementById("formPesquisaIniciativa");

function novaIniciativa() {
	btPesquisa.disabled 		= true;
	btNovaIniciativa.disabled 	= true;
	window.location 			= "emenda.php?modulo=principal/cadastroAcao&acao=A";
}

function pesquisarIniciativa() {
	btPesquisa.disabled 		= true;
	btNovaIniciativa.disabled 	= true;
	form.submit();
}

function alterarIniciativa(iniid) {
	window.location = "emenda.php?modulo=principal/cadastroAcao&acao=A&iniid=" + iniid + "";
}

function excluirIniciativa(iniid) {
	if(confirm("Deseja realmente excluir esta iniciativa ?")) {
		hdExcluir.value = iniid;
		form.submit();
	}
}

</script>