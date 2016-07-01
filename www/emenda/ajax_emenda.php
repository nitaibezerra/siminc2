<?php
header('content-type: text/html; charset=ISO-8859-1');
include_once '_funcoes.php';
include_once 'fndeWebservice.php';
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "emenda/classes/ExecucaoFinanceira.class.inc";
include_once APPRAIZ . "emenda/classes/ExecFinanceiraHistorico.class.inc";
//require_once APPRAIZ . "emenda/classes/AssocArray2xml.class.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
require_once APPRAIZ . "emenda/classes/PlanoTrabalho.class.inc";
require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
include_once APPRAIZ . "emenda/classes/ContaCorrente.class.inc";
require_once APPRAIZ . "emenda/classes/ContaCorrenteHistorico.class.inc";
include_once APPRAIZ . "emenda/classes/WSContaCorrente.class.inc";
require_once APPRAIZ . "emenda/classes/Habilita.class.inc";
require_once APPRAIZ . "emenda/classes/LogErroWS.class.inc";
require_once APPRAIZ . "emenda/classes/Ordembanchistorico.class.inc";
require_once APPRAIZ . "emenda/classes/Ordembancaria.class.inc";
include_once APPRAIZ.'emenda/classes/PagamentoFNDE.class.inc';
include_once APPRAIZ."emenda/classes/WSIntegracaoSiconv.class.inc";
//include_once APPRAIZ."emenda/classes/WSSiconvConvenio.class.inc";

$db = new cls_banco();

/*
 * Autores
*/

if(isset($_REQUEST['autor'])) {
	switch($_REQUEST['autor']) {
		case 'pesquisa': 
			pesquisaAutor($_REQUEST);
			break;
		case 'insere' :
			echo insereAutor($_REQUEST);
			break;		
		case 'altera' :
			echo alteraAutor($_REQUEST);
			break;		
		case 'excluir' :
			echo excluirAutor($_REQUEST['autid']);
			break;		
	}
}

/*
 * Partidos
*/

if(isset($_REQUEST['partido'])) {
	switch($_REQUEST['partido']) {
		case 'pesquisa': 
			pesquisaPartido($_REQUEST);
			break;	
		case 'excluir' :
			echo excluirPartido($_REQUEST['parid']);
			break;		
	}
}

/*
 * Tipo de Autores 
*/

if(isset($_REQUEST['tipoautor'])) {
	switch($_REQUEST['tipoautor']) {
		case 'pesquisa': 
			pesquisaTipoAutor($_REQUEST);
			break;
		case 'insere' :
			echo insereTipoAutor($_REQUEST);
			break;		
		case 'altera' :
			echo alteraTipoAutor($_REQUEST);
			break;	
		case 'excluir' :
			echo excluirTipoAutor($_REQUEST['tpaid']);
			break;		
	}
}

/*
 * Grupo de Autores 
*/

if(isset($_REQUEST['grupoautor'])) {
	switch($_REQUEST['grupoautor']) {
		case 'pesquisa': 
			pesquisaGrupoAutor($_REQUEST);
			break;
		case 'insere' :
			echo insereGrupoAutor($_REQUEST);
			break;		
		case 'altera' :
			echo alteraGrupoAutor($_REQUEST);
			break;	
		case 'excluir' :
			echo excluirGrupoAutor($_REQUEST['gpaid']);
			break;		
	}
}


if(isset($_REQUEST['ExecucaoOrcamentaria'])) {
	switch($_REQUEST['ExecucaoOrcamentaria']) {
		case 'PTRES': 
			montaComboPTRES($_REQUEST['arValor'], $_REQUEST['id'], $_REQUEST['emeid'], $_REQUEST['tipoemenda']);
			break;	
		case 'salvar':
			salvarExecucaoOrcamentaria($_REQUEST);
			break;	
	}
}

if(isset($_REQUEST['execucaoPTA'])) {
	switch($_REQUEST['execucaoPTA']) {	
		case 'salvar':
			salvarExecucaoPTA($_REQUEST);
			break;	
		case 'solicitarEmpenho':
			solicitarEmpenhoPTA($_POST);
			break;	
		case 'enviarPropostaSiconv':
			enviarPropostaSiconv($_POST);
			break;	
		case 'solicitarEmpenhoSiconv':
			solicitarEmpenhoSiconv($_POST);
			break;	
		case 'exportaConvenioSiconv':
			exportaConvenioSiconv($_POST);
			break;	
		case 'alteraValorEmpenho':
			alteraValorEmpenho($_POST);
			break;	
		case 'solicitarPagamento':
			solicitarPagamentoPTA($_POST);
			break;	
		case 'salvarParcela':
			salvarParcelaOrdemBancariaPTA($_POST);
			break;	
		case 'salvarCancelamento':
			salvarCancelamentoConvenio( $_POST );
			break;	
		case 'ConsultarAndamentoContaCorrente':
			$obConta = new WSContaCorrente();
			$obConta->atualizaDadosContaCorrente( $_POST );
			break;	
		case 'SolicitarContaCorrente':
			$obConta = new WSContaCorrente();
			$obConta->solicitarContaCorrente( $_POST );
			break;	
	}
}

/*if( !empty( $_REQUEST['atualizaEntidadeFNDE'] ) ){
	switch($_REQUEST['atualizaEntidadeFNDE']) {	
		case 'atualiza':
			include_once APPRAIZ.'emenda/classes/Emenda.class.inc';
			$obEmenda = new Emenda();
			$obEmenda->verificaEntidadeBaseFNDE( $_REQUEST['ptrid'] );
			break;	
	}
}*/

 /**
 * Função Pesquisar Autor
 * Método usado para pesquisa de um registro do banco
 * @param array $post - Campos do Formulário
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function pesquisaAutor( $post = array() ){
	global $db;
	extract($post);
	$filtro = array();	
	
	if($autcod){
		$filtro[] = " a.autcod = '" . $autcod . "'";
	}
	if($tpaid){
		$filtro[] = " a.tpaid = " . $tpaid;
	}
	if($estuf){
		$filtro[] = " a.estuf = '" . $estuf . "'";
	}
	if($parid){
		$filtro[] = " a.parid = " . $parid;
	}
	if($autnome){
		$filtro[] = " lower(a.autnome) = lower('" . $autnome . "')";
	}
	
	
	$sql = "SELECT 
			  ( '<center><a href=\"emenda.php?modulo=principal/cadastroAutor&acao=A&autid='|| a.autid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    		'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiAutor('|| a.autid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  a.autcod,
			  a.autnome,
			  ta.tpanome,
			  ga.gpanome,
			  p.parsigla,
			  p.parnome,
			  a.autemail
			FROM 
			  emenda.autor a INNER JOIN emenda.tipoautor ta
			  ON (a.tpaid = ta.tpaid) INNER JOIN emenda.grupoautor ga
			  ON (ta.gpaid = ga.gpaid) INNER JOIN emenda.partido p
			  ON (a.parid = p.parid)
			WHERE
			  a.autstatus = 'A'
			  AND ta.tpastatus = 'A'
			  AND ga.gpastatus = 'A'
			  AND p.parstatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' )."
			ORDER BY a.autnome";
	
	$cabecalho = array("Ação", "Código", "Nome", "Tipo", "Grupo", "Sigla do Partido", "Nome do Partido", "e-mail");
	$db->monta_lista( iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 50, 10, 'N', 'center', '', '', '','');
}

 /**
 * Função Insere Autor
 * Método usado para inserção de um registro do banco
 * @param array $post - Campos do Formulário
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function insereAutor( $post = array() ){
	global $db;
	extract($post);
	
	$sql = "INSERT INTO 
			  emenda.autor(
			  autcod,
			  tpaid,
			  estuf,
			  parid,
			  autnome,
			  autnomeabreviado,
			  autemail
			) 
			VALUES (
			  $autcod,
			  $tpaid,
			  '$estuf',
			  $parid,
			  '$autnome',
			  '$autnomeabreviado',
			  '$autemail'
			)";
			
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	return $db->commit();
}

 /**
 * Função Altera Autor
 * Método usado para alteração de um registro do banco
 * @param array $post - Campos do Formulário
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function alteraAutor( $post = array() ){
	global $db;
	extract($post);
	
	$sql = "UPDATE 
			  emenda.autor  
			SET 
			  autcod = $autcod,
			  tpaid = $tpaid,
			  estuf = '$estuf',
			  parid = $parid,
			  autnome = '$autnome',
			  autnomeabreviado = '$autnomeabreviado',
			  autemail = '$autemail'
			 
			WHERE 
			  autid = $autid";
	
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	return $db->commit();
}

 /**
 * Função Excluir Autor
 * Método usado para exclução de um registro do banco
 * @param integer $autid - Código do Autor
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function excluirAutor($autid){
	global $db;
	
	$sql = "UPDATE 
			  emenda.autor  
			SET 
			  autstatus = 'I'
			 
			WHERE 
			  autid = $autid";
	
	$db->executar($sql);
	return $db->commit();
}

 /**
 * Função Pesquisa Partido
 * Método usado para pesquisa de um registro do banco
 * @param array $post - Campos do Formulário
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function pesquisaPartido( $post = array() ){
	global $db;
	extract($post);
	$filtro = array();	
	
	if($parcodigo){
		$filtro[] = " parcodigo = " . $parcodigo;
	}
	if($parnome){
		$filtro[] = " lower(parnome) = lower('" . $parnome . "')";
	}
	if($parsigla){
		$filtro[] = " lower(parsigla) = lower('" . $parsigla . "')";
	}	
	
	$sql = "SELECT 
			  ( '<center><a href=\"emenda.php?modulo=principal/cadastroPartido&acao=A&parid='|| parid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    		'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiPartido('|| parid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
		  parcodigo,
		  parnome,
		  parsigla
		FROM 
		  emenda.partido
		WHERE
		  parstatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' )."
		  ORDER BY parnome";
	
	$cabecalho = array("Ação", "Código", "Nome", "Sigla");
	$db->monta_lista( iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 10, 'N', 'center', '', '', '','');
}


 /**
 * Função Exclui Partido
 * Método usado para exclusão de um registro do banco
 * @param integer $parid - Código do partido
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function excluirPartido($parid){
	global $db;
	
	$sql = "UPDATE 
			  emenda.partido  
			SET 
			  parstatus = 'I'
			 
			WHERE 
			  parid = $parid";
	
	$db->executar($sql);
	return $db->commit();
}

/**
 * Função Pesquisa Tipo de Autor
 * Método usado para pesquisa de um registro do banco
 * @param array $post - Campos do Formulário
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function pesquisaTipoAutor( $post = array() ){
	global $db;
	extract($post);
	$filtro = array();	
	
	if($tpanome){
		$filtro[] = " lower(ta.tpanome) = lower('" . $tpanome . "')";
	}
	if($gpaid){
		$filtro[] = " ga.gpaid = " . $gpaid;
	}	
	
	$sql = "SELECT 
			  ( '<center><a href=\"emenda.php?modulo=principal/cadastroTipoAutor&acao=A&tpaid='|| ta.tpaid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    		'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiTipoAutor('|| ta.tpaid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  ta.tpanome,
			  ga.gpanome
			FROM 
			  emenda.tipoautor ta INNER JOIN emenda.grupoautor ga
			  ON (ta.gpaid = ga.gpaid)
			WHERE
			  ta.tpastatus = 'A'
			  AND ga.gpastatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' )."
			  ORDER BY tpanome";

	$cabecalho = array("Ação", "Nome", "Grupo");
	$db->monta_lista( iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 10, 'N', 'center', '', '', '','');
}

 /**
 * Função Exclui Tipo de Autor
 * Método usado para exclusão de um registro do banco
 * @param integer $tpaid - Código do  tipo de autor
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function excluirTipoAutor($tpaid){
	global $db;
	
	$sql = "UPDATE 
			  emenda.tipoautor  
			SET 
			  tpastatus = 'I'
			 
			WHERE 
			  tpaid = $tpaid";
	
	$db->executar($sql);
	return $db->commit();
}

/**
 * Função Insere Tipo de Autor
 * Método usado para inserção de um registro do banco
 * @param array $post - Campos do Formulário
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function insereTipoAutor( $post = array() ){
	global $db;
	extract($post);
	
	$sql = "INSERT INTO 
			  emenda.tipoautor(
			  tpanome,
			  gpaid,
			  tpastatus
			) 
			VALUES (
			  '$tpanome',
			  $gpaid,
			  'A'
			)";
			
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	return $db->commit();
}

 /**
 * Função Altera Tipo de Autor
 * Método usado para alteração de um registro do banco
 * @param array $post - Campos do Formulário
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function alteraTipoAutor( $post = array() ){
	global $db;
	extract($post);
	
	$sql = "UPDATE 
			  emenda.tipoautor  
			SET 
			  tpanome = trim('$tpanome'),
			  gpaid = $gpaid
			 
			WHERE 
			  tpaid = $tpaid";
	
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	return $db->commit();
}

/**
 * Função Pesquisa Grupo de Autor
 * Método usado para pesquisa de um registro do banco
 * @param array $post - Campos do Formulário
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function pesquisaGrupoAutor( $post = array() ){
	global $db;
	extract($post);
	$filtro = array();	
	
	if($gpanome){
		$filtro[] = " lower(gpanome) = lower('" . $gpanome . "')";
	}
	if($gpacategoria){
		$filtro[] = " lower(gpacategoria) = lower('" . $gpacategoria . "')";
	}	
	
	$sql = "SELECT 
			  ( '<center><a href=\"emenda.php?modulo=principal/cadastraGrupoAutor&acao=A&gpaid='|| gpaid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    		'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiGrupoAutor('|| gpaid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  gpanome,
			  gpacategoria
			FROM 
			  emenda.grupoautor
			WHERE
			  gpastatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' )."
			  ORDER BY gpanome";

	$cabecalho = array("Ação", "Nome", "Categoria");
	$db->monta_lista( iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 10, 'N', 'center', '', '', '','');
}

 /**
 * Função Exclui Grupo de Autor
 * Método usado para exclusão de um registro do banco
 * @param integer $tpaid - Código do  Grupo de autor
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function excluirGrupoAutor($gpaid){
	global $db;
	
	$sql = "UPDATE 
			  emenda.grupoautor  
			SET 
			  gpastatus = 'I'
			 
			WHERE 
			  gpaid = $gpaid";
	
	$db->executar($sql);
	return $db->commit();
}

/**
 * Função Insere Grupo de Autor
 * Método usado para inserção de um registro do banco
 * @param array $post - Campos do Formulário
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function insereGrupoAutor( $post = array() ){
	global $db;
	extract($post);
	
	$sql = "INSERT INTO 
			  emenda.grupoautor(
			  gpanome,
			  gpacategoria,
			  gpastatus
			) 
			VALUES (
			  trim('$gpanome'),
			  trim('$gpacategoria'),
			  'A'
			)";
			
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	return $db->commit();
}

 /**
 * Função Altera Grupo de Autor
 * Método usado para alteração de um registro do banco
 * @param array $post - Campos do Formulário
 * @return bool 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function alteraGrupoAutor( $post = array() ){
	global $db;
	extract($post);
	
	$sql = "UPDATE 
			  emenda.grupoautor  
			SET 
			  gpanome = trim('$gpanome'),
			  gpacategoria = trim('$gpacategoria')
			 
			WHERE 
			  gpaid = $gpaid";
	
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	return $db->commit();
}

?>