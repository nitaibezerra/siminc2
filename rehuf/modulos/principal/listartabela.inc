<?php
/*
 * TELA LISTA DE TABELAS (lista de tabelas para serem preenchidas pelos hospitais)
 * As tabelas são criadas pelo administrador. Nesta tela é criado o documento
 * referente ao workflow. Esta passa por validação das permissões.
 * Parâmetro obrigatório : $_REQUEST['entid']
 */

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('buscargrupoitem'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

include APPRAIZ ."includes/workflow.php";
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_REQUEST['entid']);
/* FIM - Tratamento de segurança */
/* Tratamento dos documentos */

$docid = pegarDocid($_REQUEST['entid']);
if(!$docid) {
	$docid = criarDocumento($_REQUEST['entid']);
}
$estado_documento = wf_pegarEstadoAtual( $docid );
$dadosestruturaunidadeid = $db->pegaUm("SELECT esuid FROM rehuf.estruturaunidade WHERE entid = '".$_REQUEST['entid']."'");

$_SESSION['rehuf_var'] = array("esuid" => $dadosestruturaunidadeid,
							   "entid" => $_REQUEST['entid']);

/* FIM - Tratamento dos documentos */

// tratamento especial para os hospitais federais (ver somente plantões)
if($permissoes['hospfederal']) {
	echo "<script>
			window.location='rehuf.php?modulo=pregao/listaPregaoPreenchimento&acao=A';
		  </script>";
	exit;
}




if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */
/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
/* fim montando cabeçalho */
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0"	align="center">
<tr>
<td width="95%" valign="top">
<?
$sql = "SELECT * FROM rehuf.dimensao ORDER BY dimdsc";
$dimensoes = $db->carregar($sql);
if($dimensoes[0]) {
	foreach($dimensoes as $dime) {
		echo "<strong>".$dime['dimdsc']."</strong><br />";
		$sql = "SELECT '<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px;cursor:pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" onclick=\"abregrupoitem(this,'|| tab.tabtid ||');\"></center>' as opcoes, 
					   '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editartabela&acao=A&tabtid='|| tab.tabtid ||'\'\">'|| tab.tabtdsc ||'</a>' as tabelas,
					   CASE WHEN (SELECT ltadata FROM rehuf.logtabelasgrupos lta WHERE lta.tabtid = tab.tabtid AND lta.esuid='".$_SESSION['rehuf_var']['esuid']."' ORDER BY ltadata DESC LIMIT 1) IS NULL THEN 'Não existem registros' ELSE CAST((SELECT to_char(lta.ltadata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome FROM rehuf.logtabelasgrupos lta LEFT JOIN seguranca.usuario usu ON usu.usucpf = lta.usucpf WHERE lta.tabtid = tab.tabtid AND lta.esuid='".$_SESSION['rehuf_var']['esuid']."' ORDER BY ltadata DESC LIMIT 1) as varchar) END AS logtg,
					   (SELECT COUNT(*) FROM rehuf.grupoitem gpr WHERE gpr.tabtid = tab.tabtid ) AS numgrupoitm
					   FROM rehuf.tabela tab WHERE tab.dimid = '".$dime['dimid']."' AND tab.tabvisivel=TRUE ORDER BY tab.tabtdsc";
		$cabecalho = array("", "Tabelas", "Última atualização", "Número de grupos");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	}
} else {
	echo "Não existem dimensões cadastradas.";
}
?>	
</td>
<td width="5%" valign="top">
<? 
	/* Barra de estado atual e ações e Historico */
	wf_desenhaBarraNavegacao( $docid, array( 'entid' => $_SESSION['rehuf_var']['entid'] ) );			
?>		
</td>
</tr>
</table>