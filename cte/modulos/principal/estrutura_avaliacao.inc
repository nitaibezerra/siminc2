<?php
//dbg($_SESSION['inuid']);
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0");
header("Pragma: no-cache");
header('Content-type: text/html; charset="iso-8859-1"',true);

/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(300);
/* FIM configurações */

if($_SESSION['inuid'] != '' || isset($_SESSION['inuid'])){

	$sql = "select docid from cte.instrumentounidade where inuid = ".$_SESSION['inuid']."";
	$pegaDocid = $db->pegaUm($sql);
	$nrEstadoAtual = parPegarEstadoAtual($pegaDocid);
	$arEstadoAtualBloqueia = array("1", "2", "12");
	$boEstadoAtual = in_array($nrEstadoAtual, $arEstadoAtualBloqueia) ? false : true;
} else {

	alert('A variável de sessão não existe.');
	echo "<script>document.location.href = \"cte.php?modulo=lista&acao=M\"</script>";
	return false;
}


if( cte_possuiPerfilExclusivo( CTE_PERFIL_EQUIPE_ESCOLA_ATIVA ) ){
	if( $_REQUEST["csFiltroArvore"] != 2 ){
		$stLinkPagina = "?modulo=principal/estrutura_avaliacao&acao=A&csFiltroArvore=2";
		echo '	<script type="text/javascript">window.location = "'.$stLinkPagina.'";</script>';
		die();
	}
}

// Henrique
if($_SESSION['inuid']){
	$dadosDesist = verificaDesist( $_SESSION['inuid'] );
}

if( $_REQUEST['param'] ){
	$param = atualizaDesist( $_SESSION['inuid'], $_REQUEST['param'] );
	echo 'Dados salvos com sucesso';
	die();
}

include_once( APPRAIZ. "cte/classes/InstrumentoUnidade.class.inc" );
include_once( APPRAIZ. "cte/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "cte/classes/EscolaAtiva.class.inc" );
include_once( APPRAIZ. "cte/classes/EscolaAtivaEscolas.class.inc" );
include_once( APPRAIZ. "cte/classes/EscolaAtivaPessoas.class.inc" );
include_once( APPRAIZ . "includes/classes/questionario/Tela.class.inc" );
include_once( APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc" );

cte_verificaSessao();

if($_POST['resetaEscolaAtiva'] == 'true'){
	
	$sql = "select esaid from cte.escolaativa where inuid = '{$_SESSION['inuid']}' and esaano = '2012'";
	$esaid = $db->pegaUm($sql);
	if($esaid){	
		$sql = "delete from cte.escolaativaescolas where esaid = '{$esaid}'";
		$db->executar($sql);
		$sql = "delete from cte.escolaativa where esaid = '{$esaid}'";
		$db->executar($sql);
		$db->commit();
	}	
}

$obEscolaAtiva = new EscolaAtiva( "", $_SESSION['inuid'] );

if($_REQUEST['req'] == 'removeescolas'){

	if( $obEscolaAtiva->esaid && $_REQUEST["entid"] ){
		$sql = " delete from cte.escolaativaescolas where esaid = {$obEscolaAtiva->esaid} and entid = {$_REQUEST["entid"]} ";

		$obEscolaAtiva->executar( $sql );
		$obEscolaAtiva->commit();
	}
	die();
}

$csFiltroArvore = $_REQUEST['csFiltroArvore'] ? $_REQUEST['csFiltroArvore'] : 1;

$itrid = cte_pegarItrid( $_SESSION['inuid'] );

$obInstrumentoUnidade = new InstrumentoUnidade( $_SESSION["inuid"] );

if( $_REQUEST["csTratarAdesao"] ){

	$boAdesaoEscolaAtiva   = false;
	$boAdesaoProLetramento = false;

	$arAdesaoEscolaAtiva = array( CTE_ADESAO_ESCOLA_ATIVA_ACEITA, CTE_ADESAO_ESCOLA_ATIVA_NAO_ACEITA, CTE_ADESAO_ESCOLA_ATIVA_SEM_CLASSES, CTE_ADESAO_ESCOLA_ATIVA_MULTISERIADAS );
	//if( !$obInstrumentoUnidade->usucpfescolaativa && in_array( $_REQUEST["csTratarAdesao"], $arAdesaoEscolaAtiva ) ){
	if( !$obEscolaAtiva->usucpfescolaativa && in_array( $_REQUEST["csTratarAdesao"], $arAdesaoEscolaAtiva ) ){

		$boAdesaoEscolaAtiva = true;
		$csFiltroAtual = 2;

		if( $_REQUEST["csTratarAdesao"] == CTE_ADESAO_ESCOLA_ATIVA_ACEITA || $_REQUEST["csTratarAdesao"] == CTE_ADESAO_ESCOLA_ATIVA_MULTISERIADAS){
			if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
				$obInstrumentoUnidade->criarArvoreEscolaAtivaEstadual();
			}
			elseif( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){
				$obInstrumentoUnidade->criarArvoreEscolaAtiva();
			}
		}

//		$sql = "update cte.instrumentounidade set
//						usucpfescolaativa = '{$_SESSION["usucpf"]}',
//						inusituacaoadesao = '{$_REQUEST["csTratarAdesao"]}' 
//				where inuid = '{$_SESSION["inuid"]}' ";		

		$obEscolaAtiva->inuid 				= $_SESSION['inuid'];
		$obEscolaAtiva->esaano 				= '2012';
		$obEscolaAtiva->docid 				= null;
		$obEscolaAtiva->usucpfescolaativa 	= $_SESSION['usucpf'];
		$obEscolaAtiva->esasituacaoadesao 	= $_REQUEST['csTratarAdesao'];
		$obEscolaAtiva->salvar();
		$obEscolaAtiva->commit();

	}
	elseif( !$obInstrumentoUnidade->usucpfproletramento && $_REQUEST["csTratarAdesao"] == CTE_ADESAO_PRO_LETRAMENTO ){

		$boAdesaoProLetramento = true;
		$csFiltroAtual = 3;

		if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
			$obInstrumentoUnidade->criarArvoreProLetramentoEstadual();
		}
		elseif( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){
			$obInstrumentoUnidade->criarArvoreProLetramento();
		}

		$sql = "update cte.instrumentounidade set
						usucpfproletramento = '{$_SESSION["usucpf"]}'
				where inuid = '{$_SESSION["inuid"]}' ";

	}
	elseif( !$obInstrumentoUnidade->usucpfprofuncionario && $_REQUEST["csTratarAdesao"] == CTE_ADESAO_PRO_FUNCIONARIO ){

		$boAdesaoProFuncionario = true;
		$csFiltroAtual = 5;

		if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
			$obInstrumentoUnidade->criarArvoreProFuncionarioEstadual();
		}
		elseif( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){
			$obInstrumentoUnidade->criarArvoreProFuncionario();
		}

		$sql = "update cte.instrumentounidade set
						usucpfprofuncionario = '{$_SESSION["usucpf"]}'
				where inuid = '{$_SESSION["inuid"]}' ";

	}
	elseif( !$obInstrumentoUnidade->usucpfformacaoescola && $_REQUEST["csTratarAdesao"] == CTE_ADESAO_FORMACAO_ESCOLA ){

		$boAdesaoFormacaoEscola = true;
		$csFiltroAtual = 6;

		if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
			$obInstrumentoUnidade->criarArvoreFormacaoEscolaEstadual();
		}
		elseif( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){
			$obInstrumentoUnidade->criarArvoreFormacaoEscola();
		}

		$sql = "update cte.instrumentounidade set
						usucpfformacaoescola = '{$_SESSION["usucpf"]}'
				where inuid = '{$_SESSION["inuid"]}' ";

	}

	$obInstrumentoUnidade->executar( $sql );
	$obInstrumentoUnidade->commit();

	$stLinkPagina = $boAdesaoEscolaAtiva ? "?modulo=principal/estrutura_avaliacao&acao=A&csFiltroArvore=2" : "?modulo=principal/estrutura_avaliacao&acao=A&csFiltroArvore=".$csFiltroAtual;

	echo '	<script type="text/javascript">
				//alert( "Atenção! É preciso preencher as subações referentes ao Programa." );
				window.location = "'.$stLinkPagina.'";
			</script>';
	die();
}

if( $_REQUEST["gravarEscolaAtiva"] && $obEscolaAtiva->podeEditarEscolaAtiva() ){

	$arCampos = array( "esaid", "entid", "inuid", "esaano", "esaqtdprofessores" );
	$obEscolaAtiva->popularObjeto( $arCampos );

	$obEscolaAtiva->entid = $_REQUEST["entid"] ? $_REQUEST["entid"] : null;

	if( is_array( $_REQUEST["entid_escolas"] ) ){
		$obEscolaAtiva->esaqtdtecnicos =  ceil( count( $_REQUEST["entid_escolas"] )/25 );
	}

	if( $obEscolaAtiva->salvar() ){
		$obEscolaAtiva->commit();
	}
	else{
		$obEscolaAtiva->rollback();
	}

	if( is_array( $_REQUEST["eaeid"] ) ){
		
//		$sql = "DELETE FROM cte.escolaativaescolas WHERE esaid = {$obEscolaAtiva->esaid}";
//		$db->executar($sql);
//		$db->commit();
		
		foreach( $_REQUEST["eaeid"] as $count => $eaeid ){
				
			( $eaeid ? $eaeid : $eaeid = null );
			
			$obEscolaAtivaEscolas = new EscolaAtivaEscolas( $eaeid );
				
			$obEscolaAtivaEscolas->eaeid = $eaeid;
			$obEscolaAtivaEscolas->esaid = $obEscolaAtiva->esaid;
			$obEscolaAtivaEscolas->entid = $_REQUEST["entid_escolas"][$count];
			$obEscolaAtivaEscolas->eaeqtdpredios = (int) $_REQUEST["eaeqtdpredios"][$count];
			$obEscolaAtivaEscolas->eaeqtdturmas  = (int) $_REQUEST["eaeqtdturmas" ][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos1 = (int) $_REQUEST["eaeqtdalunos1"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos2 = (int) $_REQUEST["eaeqtdalunos2"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos3 = (int) $_REQUEST["eaeqtdalunos3"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos4 = (int) $_REQUEST["eaeqtdalunos4"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos5 = (int) $_REQUEST["eaeqtdalunos5"][$count];
				
			$obEscolaAtivaEscolas->salvar();
			$obEscolaAtivaEscolas->commit();
		}
	}

	$obEscolaAtiva->sucesso( "principal/estrutura_avaliacao&acao=A&csFiltroArvore=2" );

}

//dbg($_SESSION['inuid']);
/*!@
 * Carrega os detalhes da subação para exibição no dialog
 */
if ($_REQUEST['titleFor']) {
	$sql = '
    SELECT
        CASE WHEN
            sa.sbaporescola <> false THEN \'Por escola\'
        ELSE
            \'Global\'
        END as sbaporescola,
        usu.usunome,
        to_char(sa.sbadata,\'dd/mm/YYYY HH24:MI:SS\') as data,
        coalesce(ss.ssudescricao, \'Nao informado\') as ssudescricao,
        coalesce(fat.foadsc, \'Nao informado\')      as fatendimento,
        coalesce(fex.frmdsc, \'Nao informado\')      as frmexecucao
    FROM
        cte.subacaoindicador sa
    LEFT JOIN
        cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
    LEFT JOIN
        cte.statussubacao ss on ss.ssuid = spt.ssuid
    LEFT JOIN
        cte.formaatendimento fat on fat.foaid = sa.foaid 
    LEFT JOIN
        cte.formaexecucao fex on fex.frmid = sa.frmid 
    LEFT JOIN
        seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE
        sa.sbaid = ' . (integer) $_REQUEST['titleFor'];

	if (($dados = $db->pegaLinha($sql)) !== false) {
		 
		if($_REQUEST['titleFor']){
			$sql = 'SELECT sptano,
	    	   			   coalesce(ss.ssudescricao, \'Não informado\') as ssudescricao FROM cte.subacaoparecertecnico spt
					LEFT JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
					WHERE spt.sbaid = ' . (integer) $_REQUEST['titleFor'] .' ORDER BY sptano ';
			$arStatus = $db->carregar($sql);
		}
		$arStatus = (is_array($arStatus)) ? $arStatus : array();
		$stStatus = "";
		foreach ($arStatus as $status){
			$stStatus .= "&nbsp;&nbsp;&nbsp;".$status['sptano'] .': '. $status['ssudescricao']."<br />";
		}
		 
		$stStatus = ($stStatus) ? $stStatus : "&nbsp;&nbsp;&nbsp;Não Informado";
		header("Content-Type: text/html; charset=ISO-8859-1");
		echo ''
		,'Última atualização feita por '        , str_replace( "'", "\'", $dados['usunome'] ), ' em ' , $dados['data'] , '<br /><br />'
		,'<strong>Status: </strong>'            , $dados['ssudescricao'] , '<br />'
		,'<strong>Status: </strong>',  '<br />'
		,$stStatus,  '<br />'
		,'<strong>Cronograma: </strong>'        , $dados['sbaporescola'] , '<br />'
		,'<strong>Forma de execução: </strong>' , $dados['frmexecucao'];
	} else {
		echo '<em>Nenhuma atualização realizada.</em>';
	}

	die();
}

$msg = '';

if ($_REQUEST['action']) {
	if ($_REQUEST['action'] == 'remov' && $_REQUEST['sbaid'] != '') {
		$sql = '
        SELECT
            COUNT(sbaid)
        FROM
            cte.subacaoparecertecnico spt
        WHERE
            sbaid = ' . $_REQUEST['sbaid'] . ' AND
            (
                ssuid IS NOT NULL or
                trim(sptparecer) <> \'\'
            )';

		$parec = $db->pegaUm($sql);

		$sql = '
        SELECT
            COUNT(sac.sbaid)
        FROM
            cte.subacaoconvenio sac
        INNER JOIN
            cte.subacaoindicador sba ON sba.sbaid = sac.sbaid
        WHERE
            sac.sbaid = ' . $_REQUEST['sbaid'];

		$conv  = $db->pegaUm($sql);

		$sql = '
		        SELECT COUNT(sbaid)
		        FROM cte.monitoramentosubacao ms
		        WHERE sbaid = ' . $_REQUEST['sbaid'];

		$boMonitoramento = $db->pegaUm($sql);

		if ($parec >= 1 || $conv >= 1 || $boMonitoramento >=1 ) {
			$msg = '<script type="text/javascript">'
			.'alert("Operação não realizada!\nFoi dado o parecer técnico, gerado convênio ou a subação está sendo monitorada.");'
			.'window.location.href = "/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A"'
			.'</script>';
		} else {
			$db->executar('
	        			   DELETE FROM
	                        cte.subacaoparecertecnico
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';

	        			   DELETE FROM
	                        cte.subacaobeneficiario
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';

	                       DELETE FROM
	                        cte.composicaosubacao
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';

	                       DELETE FROM
	                        cte.qtdfisicoano
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';

	                       DELETE FROM
	                        cte.termosubacaoindicador
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	                        
	                       DELETE FROM 
	                       	cte.composicaopessoa  
	                       WHERE 
	                       	sbaid = ' . (integer) $_REQUEST['sbaid'] .';

	                       DELETE FROM
	                        cte.subacaoindicador
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid']);

			$db->commit();

			$msg = '<script type="text/javascript">'
			.'alert("Subação excluída com sucesso.");'
			.'window.location.href = "/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A"'
			.'</script>';
		}

		echo $msg;
		die();
	}
}

/*******************************
 * 	FUNCTION: exibeErrosSubAcao();
 * 	26/06/2008
 *   DESCRIÇÃO:
 *	Ao gerar convênio com FNDE monta uma tela com
 *   os erros de validações de envio.
 *   (Dados que faltam ser preencher na tela Subaçãoes.)
 *   @PARAM  $erros - array (array de erros)
 *
 *******************************/
function exibeErrosSubAcao($erros) {

	echo " <script>
				function alterarSubacao( sbaid ){
					var janela = window.open( \"../cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=\" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
					janela.focus();
				}
		  	  </script>

			  <div style=\"width : 100%;\">
				<table class=\"tabela\">
				<tr>
				<td colspan='2'>O sistema verificou que alguns dados das seguintes subações não foram preenchidos :</td>
				</tr>";

	foreach($erros as $idsubacao => $dados) {
		echo "<tr style=\"background-color: #d9d9d9;\">";
		echo "<td>";
		if($dados['parecerGeral']) {
			echo "<b> - ".$dados['parecerGeral']."</b><br />";
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Para poder criar o parecer volte para fase de 'preenchimento de parecer Técnico'</br>";
		}
		echo "</td></tr>";
		if($dados['nome']){
			echo "<tr style=\"background-color: #d9d9d9;\">";
			echo "<td><img src='/imagens/consultar.gif' onclick='alterarSubacao(". $idsubacao .")'></td>";
			echo "<td><b>". $dados['nome'] ."</b><br />";
			if($dados['parecer']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['parecer']."<br />";
			}
			if($dados['PI']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['PI']."<br />";
			}
			if($dados['escola']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['escola']."<br />";
			}
			if($dados['quantidadesubacao']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['quantidadesubacao']."<br />";
			}
			if($dados['descricaosubacao']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['descricaosubacao']."<br />";
			}
			if(($dados['descricaoitemcomposicao']) ||($dados['itemcomposicao']) ) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>Erros nos itens de composição:</i><br />";
				if($dados['descricaoitemcomposicao']) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Existem itens de composição sem identificação. (Verifique em  Editar / Inserir Itens de Composição)</br>";
				}
				if($dados['itemcomposicaovalor']) {
					if(!is_array($dados['itemcomposicaovalor'])) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['itemcomposicaovalor'] ." ";
					} else {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O valor unitario dos itens abaixo não podem ser Zero:</br>";
						foreach($dados['itemcomposicaovalor'] as $item) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$item."<br />";
						}
					}
					echo"<br />";
				}
				if($dados['itemcomposicao']) {
					if(!is_array($dados['itemcomposicao'])) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['itemcomposicao'] ."<br />";
					} else {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A quantidade dos itens abaixo não podem ser Zero:</br>";
						foreach($dados['itemcomposicao'] as $item) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$item."<br />";
						}
					}
					echo"<br />";
				}
			}
		}
		if($dados['beneficiarios']) {
			echo "<br />";
			if(!is_array($dados['beneficiarios'])) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['beneficiarios'] ."<br />";
			} else {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>Erros nos beneficiários:</i><br />";
				foreach($dados['beneficiarios'] as $benef) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$benef."<br />";
				}
			}
		}
		echo "</td></tr>";
	}
	echo "</table>";
}


//cte_atualizarPlanosPequenoMunicipio( $_SESSION['inuid'] );
//$db->commit();
$estado_documento = wf_pegarEstadoAtual( cte_pegarDocid( $_SESSION['inuid'] ) );
/*
 echo "Estado:" ;
 print_r($estado_documento);
 echo "<br><br><br>";
 */
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
	$db->cria_aba( $abacod_tela, $url, '' );
	cte_montaTitulo(
	$titulo_modulo, '
		<img src="/imagens/atencao.png" align="middle"/> Itens com pendências
			&nbsp;&nbsp;&nbsp;
			<img src="/imagens/check_p.gif" align="middle"/> Itens ok'
			);
} else {
	$db->cria_aba( $abacod_tela, $url, '' );
	cte_montaTitulo( $titulo_modulo, '' );
}
//print_r( $_SESSION);

$itrid = cte_pegarItrid( $_SESSION['inuid'] );




$sql_dimensao = sprintf(
	"select d.dimid, d.dimcod, d.dimdsc
	from cte.dimensao d
	where d.dimstatus = 'A' and d.itrid = %d
	order by d.dimcod",
$itrid
);
$dimensoes = $db->carregar( $sql_dimensao );
if ( !$dimensoes )
{
	$dimensoes = array();
}

$sql_area = sprintf(
	"select a.ardid, a.ardcod, a.arddsc, a.dimid
	from cte.areadimensao a
	inner join cte.dimensao d on d.dimid = a.dimid
	where a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
	order by a.ardcod",
$itrid
);
$areas = $db->carregar( $sql_area );
if ( !$areas ) {

	$areas = array();
}

$sql_indicador_VerDados = sprintf("
	SELECT 
		i.indid,
		aie.aciid as acaoestadual, 
		aim.aciid as acaomunicipal, 
		saie.aciid as subacaoestadual, 
		saim.aciid as subacaomunicipal
	FROM cte.indicador i 
	INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
	INNER JOIN cte.dimensao d on d.dimid = a.dimid 
	LEFT  JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A' 
	LEFT  JOIN cte.criterio c on c.crtid = p.crtid
	LEFT  JOIN cte.acaoindicador aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E' 
	LEFT  JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 
	LEFT  JOIN cte.subacaoindicador saie on saie.aciid = aie.aciid 
	LEFT  JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid 
	LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
	WHERE
		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d 
	GROUP BY i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, p.ptodemandaestadual, 
		 p.ptodemandamunicipal, c.ctrpontuacao, saie.aciid, saim.aciid, aie.aciid, aim.aciid, usu.usunome, p.ptodata
	ORDER BY
		i.indcod",
$_SESSION['inuid'],
$itrid
);

$indicadores_Validacoes = $db->carregar( $sql_indicador_VerDados );
if ( !$indicadores_Validacoes )
{
	$indicadores_Validacoes = array();
}

$sql_indicador = sprintf("
	SELECT
		i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, length(p.ptodemandaestadual) as demandaestadual, 
		length(p.ptodemandamunicipal) as demandamunicipal, 
		c.ctrpontuacao,
		usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
	FROM cte.indicador i 
	INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
	INNER JOIN cte.dimensao d on d.dimid = a.dimid 
	LEFT  JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A' 
	LEFT  JOIN cte.criterio c on c.crtid = p.crtid
	LEFT  JOIN cte.acaoindicador aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E' 
	LEFT  JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 
	LEFT  JOIN cte.subacaoindicador saie on saie.aciid = aie.aciid 
	LEFT  JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid 
	LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
	WHERE
		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d 
	GROUP BY i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, p.ptodemandaestadual, 
		 p.ptodemandamunicipal, c.ctrpontuacao, usu.usunome, p.ptodata
	ORDER BY
		i.indcod",
$_SESSION['inuid'],
$itrid
);


$indicadores = $db->carregar( $sql_indicador );
if ( !$indicadores )
{
	$indicadores = array();
}

$sql_dimensao_q = sprintf(
	"select d.dimid, d.dimcod, d.dimdsc
	from cte.dimensao d
	where d.dimstatus = 'A' and d.itrid = %d
	and exists ( select p.prgid from cte.pergunta p where p.dimid = d.dimid )
	order by d.dimcod",
$itrid
);
$dimensoes_q = $db->carregar( $sql_dimensao_q );
if ( !$dimensoes_q )
{
	$dimensoes_q = array();
}
/*
 $sql_pergunta = sprintf(
 "select p.prgid, p.prgdsc, p.prgcod, d.dimid, r.rspid
 from cte.pergunta p
 inner join cte.dimensao d on d.dimid = p.dimid
 left join cte.resposta r on r.prgid = p.prgid and r.inuid = %d
 where p.prgstatus = 'A' and d.dimstatus = 'A'
 order by p.prgcod",
 $_SESSION['inuid']
 );
 */
$sql_pergunta = sprintf(
	"SELECT p.prgid, p.prgdsc, p.prgcod, d.dimid, r.rspid,
		usu.usunome, to_char( r.rspdata,'dd/mm/YYYY') as data 
	from cte.pergunta p
	inner join cte.dimensao d on d.dimid = p.dimid
	left join cte.resposta r on r.prgid = p.prgid and r.inuid = %d
	left JOIN seguranca.usuario AS usu ON (r.usucpf = usu.usucpf)
	where p.prgstatus = 'A' and d.dimstatus = 'A'
	order by p.prgcod",
$_SESSION['inuid']
);




$perguntas = $db->carregar( $sql_pergunta );
if ( !$perguntas )
{
	$perguntas = array();
}

$sql_arquivo = sprintf(
	"select t.*, a.*
	from cte.tabelas t
	inner join public.arquivo a on a.arqid = t.arqid
	where inuid = %d
	order by a.arqnome",
$_SESSION['inuid']
);
$arquivos = $db->carregar( $sql_arquivo );
if ( !$arquivos )
{
	$arquivos = array();
}

$stInner = "";
$stWhere = "";

if($_REQUEST["csFiltroArvore"] == CTE_FILTRO_ARVORE_CONVENIO && $_REQUEST['pssano']){

	$stInner = 	"
				INNER JOIN cte.projetosapesubacao psp ON psp.sbaid = sai.sbaid  
				INNER JOIN cte.projetosape ps ON ps.prsid = psp.prsid
				";

	$stWhere = "AND psp.pssano = {$_REQUEST['pssano']}";
}

$sql_acao_estadual = sprintf(
	"SELECT 
		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, count( sai.sbaid ) as subacoes,
		usu.usunome, to_char( ai.acidata,'dd/mm/YYYY') as data 
	FROM 
		cte.indicador i
	INNER JOIN cte.areadimensao a on a.ardid = i.ardid
	INNER JOIN cte.dimensao d on d.dimid = a.dimid
	INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A'
	INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'E'	
	LEFT  JOIN cte.subacaoindicador sai on sai.aciid = ai.aciid
	$stInner
		--descobre quem fez a ultima atualização
		LEFT JOIN seguranca.usuario AS usu ON (ai.usucpf = usu.usucpf)
	WHERE		
		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
		$stWhere
	GROUP BY
		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, usu.usunome, ai.acidata",
		$_SESSION['inuid'],
		$itrid
		);

		$acoes_estaduais = $db->carregar( $sql_acao_estadual );
		if ( !$acoes_estaduais )
		{
			$acoes_estaduais = array();
		}

		$sql_acao_municipal = sprintf(
	"SELECT 
		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, count( sai.sbaid ) as subacoes,
		usu.usunome, to_char( ai.acidata,'dd/mm/YYYY') as data 
	FROM 
		cte.indicador i
	INNER JOIN cte.areadimensao a on a.ardid = i.ardid
	INNER JOIN cte.dimensao d on d.dimid = a.dimid
	INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A'
	INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'M'
	LEFT  JOIN cte.subacaoindicador sai on sai.aciid = ai.aciid
	$stInner
		--descobre quem fez a ultima atualização
		LEFT JOIN seguranca.usuario AS usu ON (ai.usucpf = usu.usucpf)
	WHERE
		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
		$stWhere
	GROUP BY
		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, usu.usunome, ai.acidata",
		$_SESSION['inuid'],
		$itrid
		);

		$acoes_municipais = $db->carregar( $sql_acao_municipal );
		if ( !$acoes_municipais ) {
			$acoes_municipais = array();
		}

		if($itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
			$stPpsid = "and s.ppsid in ( 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291 )";
		} else {
			$stPpsid = "and s.ppsid in ( 1232, 1237, 1242, 1247, 1252, 1257, 1262, 1267, 1272, 1277 )";
		}

		$stInner = "";
		$stWhere = "";

		if($_REQUEST["csFiltroArvore"] == CTE_FILTRO_ARVORE_CONVENIO && $_REQUEST['pssano']){

			$stInner = 	"
				INNER JOIN cte.projetosapesubacao psp ON psp.sbaid = sa.sbaid  
				INNER JOIN cte.projetosape ps ON ps.prsid = psp.prsid
				";

			$stWhere = "AND psp.pssano = {$_REQUEST['pssano']}";
		}

		if ($estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO) {
			$subacoes = array();
		} else {
			$sql = '
    SELECT DISTINCT sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem, sa.sbaporescola, sa.frmid,
        usu.usunome, to_char(sa.sbadata,\'dd/mm/YYYY\') as data, spt.ssuid, 
        ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus
    FROM cte.subacaoindicador sa
	    INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 
	    INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
	    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
	    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
	    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
	    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
	    '.$stInner.'
    WHERE sbaidpai IS NULL
    '.$stWhere.'
    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5, 7, 9 )
    AND sa.sbaid not in (    
		select s.sbaid from cte.subacaoindicador s
			left join cte.subacaoparecertecnico p on s.sbaid = p.sbaid
		where s.sbaid =  sa.sbaid
			and sptid is null
			'.$stPpsid.'			
			and s.sbasituacaoarvore in (8, 9)
    )
    ORDER BY sa.sbaordem, sa.sbadsc';

			$subacoes = (array) $db->carregar($sql);
			$subacoesIds = $subacoes ? $subacoes : array();

			foreach($subacoesIds as $subacoesId){
				$sbaids[] = $subacoesId['sbaid'];
			}

			if($sbaids[0] >= 1){
				 
				$sql = "select
					 d.dimid,
					 a.ardid,
					 i.indid,
					 c.crtid,
					 p.ptoid,
					ai.aciid,
					si.sbaid
				from cte.dimensao d
				inner join cte.areadimensao 	a  on  d.dimid =  a.dimid 
				inner join cte.indicador 	i  on  a.ardid =  i.ardid
				inner join cte.criterio 	c  on  i.indid =  c.indid
				inner join cte.pontuacao 	p  on  c.crtid =  p.crtid
				inner join cte.acaoindicador	ai on  p.ptoid = ai.ptoid
				inner join cte.subacaoindicador	si on si.aciid = ai.aciid
				where si.sbaid IN ('".implode("', '", $sbaids)."')";
				 
				$subacoesHerancas = (array) $db->carregar($sql);

			} else {
				 
				$subacoesHerancas = array();
			}
		}

		function delimitador($texto){
			if(strlen($texto) > 120){
				$texto = substr($texto,0,120).'...';
			}
			return $texto;
		}


		$docid = cte_pegarDocid($_SESSION['inuid']);
		$estado_documento = wf_pegarEstadoAtual($docid);
		?>
<script
	language="javascript" type="text/javascript"
	src="/includes/dtree/dtree.js"></script>
<link
	rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script
	type="text/javascript" src="/includes/remedial.js"></script>
<script
	type="text/javascript" src="/includes/superTitle.js"></script>
<link
	rel="stylesheet" type="text/css" href="/includes/dtree/dtree.css">
<script type="text/javascript">
	function pesquisar(){
		formularioFiltro.submit();
	}
	function responde( opcao ){
		divCarregando();
		var param;
		if( opcao == 0 ){
			document.getElementById('divQuestionario').style.display = 'none';
			param = 'I';
		} else {
			document.getElementById('divQuestionario').style.display = '';
			param = 'A';
		}
		
		jQuery.ajax({
			type: "POST",
			url: "cte.php?modulo=principal/estrutura_avaliacao&acao=A",
			data: "param="+param,
		   	async: false,
		   	success: function(msg){
		    	alert( msg );
		   }
		});
		divCarregado();	
	}
	function load( opcao ){
		if( opcao == 'A' ){
			document.getElementById('divQuestionario').style.display = '';
		}
	}	
</script>


<style type="text/css">
#divTextoTermo {
	margin-bottom: 10px;
	padding: 20px;
	background: #fff;
	overflow: auto;
	height: 300px;
	border: 1px black solid;
}

#cabecalho {
	text-align: center;
	margin-bottom: 20px;
}

p {
	text-indent: 30px;
	text-align: justify;
}

#cabecalho p {
	line-height: 5px;
	font-weight: bold;
	text-indent: 0;
	text-align: center;
}

#divTermoAdesao {
	text-align: center;
	margin: 20px;
	width: 700px;
}
</style>
<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tbody>
		<tr>
			<td
				style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
				colspan="4">
			<form action="cte.php?modulo=principal/estrutura_avaliacao&acao=A"
				method="post" name="formularioFiltro"><input type="hidden"
				name="modulo" value="<?= $_REQUEST['modulo'] ?>" /> <input
				type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>" />
			<div style="float: left;">

			<table border="0" cellpadding="3" cellspacing="0">
				<tr>
					<td colspan="2">Exibir Árvore: <input onclick="pesquisar();"
						type="radio" name="csFiltroArvore" id="principal" value="1"
						<? if( $csFiltroArvore == CTE_FILTRO_ARVORE_PRINCIPAL ) 	   echo 'checked=checked'; ?>>Principal
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <input onclick="pesquisar();"
						type="radio" name="csFiltroArvore" id="escolaAtiva" value="2"
						<? if( $csFiltroArvore == CTE_FILTRO_ARVORE_ESCOLA_ATIVA ) 	   echo 'checked=checked'; ?>>Escola da Terra 2012 
						&nbsp;&nbsp;&nbsp; <input onclick="pesquisar();" type="radio"
						name="csFiltroArvore" id="proLetramento" value="3"
						<? if( $csFiltroArvore == CTE_FILTRO_ARVORE_PRO_LETRAMENTO )   echo 'checked=checked'; ?>>Pró-Letramento
					&nbsp;&nbsp;&nbsp; <?php if($itrid != INSTRUMENTO_DIAGNOSTICO_ESTADUAL){ ?>
					<!-- input onclick="pesquisar();" type="radio" name="csFiltroArvore" id="proFuncionario"  value="5" <? if( $csFiltroArvore == CTE_FILTRO_ARVORE_PRO_FUNCIONARIO )  echo 'checked=checked'; ?> >Pró-Funcionário &nbsp;&nbsp;&nbsp; -->
					<?php } ?> <?php if(($itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL && $_SESSION['inuid'] != 51) || ($itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL && $boEstadoAtual == true)){ ?>
					<input onclick="pesquisar();" type="radio" name="csFiltroArvore"
						id="formacaoEscola" value="6"
						<? if( $csFiltroArvore == CTE_FILTRO_ARVORE_FORMACAO_ESCOLA )   echo 'checked=checked'; ?>>Formação
					pela Escola &nbsp;&nbsp;&nbsp; <?php } ?> <input
						onclick="pesquisar();" type="radio" name="csFiltroArvore"
						id="projetoconvenio" value="7"
						<? if( $csFiltroArvore == CTE_FILTRO_ARVORE_CONVENIO )   echo 'checked=checked'; ?>>Projeto/Convênio
					&nbsp;&nbsp;&nbsp; <? if( $dadosDesist['fleid'] ){ ?><input onclick="pesquisar();" type="radio"
						name="csFiltroArvore" id="questionario" value="<? echo $dadosDesist['queid']; ?>"
						<? if( $csFiltroArvore == $dadosDesist['queid'] )               echo 'checked=checked'; ?>>Correção
					de Fluxo &nbsp;&nbsp;&nbsp; <? } ?><input onclick="pesquisar();"
						type="radio" name="csFiltroArvore" id="todas" value="4"
						<? if( $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) 		   echo 'checked=checked'; ?>>Todas

					</td>

				</tr>
			</table>
			</div>
			</form>
			</td>
		</tr>
	</tbody>
</table>

<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tbody>
		<tr>
			<td
				style="padding: 15px; background-color: #fafafa; color: #404040; vertical-align: top;"
				colspan="4"><?php if( cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_MUNICIPAL, CTE_PERFIL_CONSULTA_MUNICIPAL ) ) && $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){ ?>
			<!-- coloquei o if para o Minitoramento PAR não aparecer na escola ativa -->
				<?php if ($csFiltroArvore != 2 && $csFiltroArvore != $dadosDesist['queid']) { ?> <a
				href="?modulo=principal/monitora_estrutura&acao=A">Monitoramento PAR</a>
			<br />
			<br />
			<?php } }

			//--------------|| Questionario ||-------------------

			if($_SESSION['inuid']){
					
				if ($csFiltroArvore == $dadosDesist['queid']){

					$inuid = $_SESSION['inuid'];
					ini_set( "memory_limit", "1024M" );
					$qrpid = pegaQrpid( $inuid, $dadosDesist['queid'] );
					if( $dadosDesist['flestatus'] == 'A' ){
						$valueA = 'checked="checked"';
					} else {
						$valueI = 'checked="checked"';
					}
					?>
					<font size='3'>Desistente?</font>
					<input onclick="responde( this.value );" id="desistente" type="radio" <?php echo $valueI; ?> name="desistente" value="0">Sim
					<input onclick="responde( this.value );" id="desistenteA" type="radio" <?php echo $valueA; ?> name="desistente" value="1">Não
					<br><br>
					<div id='divQuestionario' style="display: none;">
						<?php
							$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25  ) );
						?>
					</div>
					<script>load( '<?php echo $dadosDesist['flestatus']; ?>' );</script> 
					<?php
				}
			} else{
				?><!-- Não possui o Inuid  --><?php
}

//---------------------------------------------------
?> <?php if( $csFiltroArvore == CTE_FILTRO_ARVORE_PRINCIPAL || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ){ ?>
			<div id="bloco" style="overflow: hidden;"><?php
			if($_REQUEST['exibirerros'] && $_SESSION['validaErro']) {
				if(!$_SESSION['validaErroAtualizado']) {
					echo "<script>
										//location.href = '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A';
										window.open('http://".$_SERVER['SERVER_NAME']."/cte/cte.php?modulo=principal/enviar&acao=A','Envio de projeto para o SAPE','width=1024,height=600,scrollbars=1');
										//location.href = '/cte/cte.php?modulo=principal/enviar&acao=A';
					  				  </script>";
					exit;
				}
				exibeErrosSubAcao($_SESSION['validaErro']);
				$_SESSION['validaErroAtualizado'] = false;
			} else {
				?>
			<div id="bloco" style="overflow: hidden;">
			<p><a href="javascript: arvore.openAll();">Abrir Todos</a>
			&nbsp;|&nbsp; <a href="javascript: arvore.closeAll();">Fechar Todos</a>
			</p>
			<div id="_arvore"></div>
			</div>
			<br />
			<br />
			<?php } ?></div>


			<?php 
			}

/****************************************************************
 * INÍCIO ÁRVORE PROJETO/CONVÊNIO
 */

if( $csFiltroArvore == CTE_FILTRO_ARVORE_CONVENIO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ){ ?>
			<div id="bloco" style="overflow: hidden;"><?php
			if($_REQUEST['exibirerros'] && $_SESSION['validaErro']) {
				if(!$_SESSION['validaErroAtualizado']) {
					echo "<script>
										//location.href = '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A';
										window.open('http://".$_SERVER['SERVER_NAME']."/cte/cte.php?modulo=principal/enviar&acao=A','Envio de projeto para o SAPE','width=1024,height=600,scrollbars=1');
										//location.href = '/cte/cte.php?modulo=principal/enviar&acao=A';
					  				  </script>";
					exit;
				}
				exibeErrosSubAcao($_SESSION['validaErro']);
				$_SESSION['validaErroAtualizado'] = false;
			} else {
				?> <?php if(($csFiltroArvore == CTE_FILTRO_ARVORE_CONVENIO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS) && $_REQUEST['pssano']){ ?>
			Ano de referência: <select id="pssano" name="pssano">
				<option>Selecione um ano...</option>
				<option onclick="enviaAnoConvenioProjeto(2007, 7)"
				<?=$_REQUEST['pssano'] == 2007 ? "selected" : ""?>>2007</option>
				<option onclick="enviaAnoConvenioProjeto(2008, 7)"
				<?=$_REQUEST['pssano'] == 2008 ? "selected" : ""?>>2008</option>
				<option onclick="enviaAnoConvenioProjeto(2009, 7)"
				<?=$_REQUEST['pssano'] == 2009 ? "selected" : ""?>>2009</option>
				<option onclick="enviaAnoConvenioProjeto(2010, 7)"
				<?=$_REQUEST['pssano'] == 2010 ? "selected" : ""?>>2010</option>
				<option onclick="enviaAnoConvenioProjeto(2011, 7)"
				<?=$_REQUEST['pssano'] == 2011 ? "selected" : ""?>>2011</option>
				<option onclick="enviaAnoConvenioProjeto(2012, 7)"
				<?=$_REQUEST['pssano'] == 2012 ? "selected" : ""?>>2012</option>

			</select> <br />
			<br />
			<h3>Projeto/Convênio</h3>
			<p><a href="javascript: arvore.openAll();">Abrir Todos</a>
			&nbsp;|&nbsp; <a href="javascript: arvore.closeAll();">Fechar Todos</a>
			</p>
			<?php } ?>
			<div id="_arvorePC"></div>

			<br />
			<br />
			<?php } ?></div>
			<?php
} // FINAL ÁRVORE PROJETO/CONVÊNIO

// Isto é um gato necessário!!!!
// Deve ficar aqui até o dia 25/09/2009
// Thiago, Favor não questionar.
$arMunicipiosExcecao = array(
					'1100098', '1100338', '1100403', '1100700', '1101500', '1200252', '1200435', '1200609', '1300086', '1300201', '1300706', '1301100', '1301803', '1302009', '1302207', '1302553', 
					'1303106', '1304401', '1400233', '1400506', '1501204', '1501402', '1501725', '1506583', '1506609', '1507607', '1508100', '1508407', '1600204', '1600212', '1600253', '1600279', 
					'1600402', '1600501', '1600535', '1600709', '1703073', '1708304', '1712454', '1715705', '1717800', '1717909', '1721307', '2100709', '2101103', '2101806', '2102150', '2102374', 
					'2102507', '2103158', '2103174', '2105450', '2105609', '2107704', '2109700', '2110278', '2110302', '2110500', '2111003', '2111631', '2114007', '2201572', '2203859', '2205532', 
					'2206100', '2206308', '2206951', '2207751', '2210359', '2210631', '2210706', '2211605', '2300507', '2301505', '2301851', '2304905', '2306108', '2310258', '2310407', '2311702', 
					'2312007', '2312601', '2312908', '2313351', '2313559', '2402402', '2414209', '2500700', '2504033', '2504355', '2504702', '2504801', '2505808', '2508901', '2509057', '2510808', 
					'2511608', '2512200', '2512903', '2513000', '2516102', '2601201', '2603108', '2603207', '2604502', '2606309', '2606705', '2607000', '2607406', '2608503', '2609154', '2610400', 
					'2611309', '2612455', '2613206', '2613404', '2613701', '2614303', '2614709', '2615409', '2615607', '2615706', '2616001', '2701001', '2702306', '2702405', '2702603', '2702702', 
					'2703205', '2703304', '2703601', '2703700', '2703759', '2704104', '2704302', '2704609', '2705408', '2706109', '2706802', '2707008', '2707800', '2707909', '2708501', '2708709', 
					'2708956', '2709004', '2800605', '2800670', '2800704', '2801108', '2801603', '2802403', '2802601', '2802809', '2803807', '2804409', '2804458', '2805109', '2805604', '2806305', 
					'2806404', '2807600', '2902609', '2903607', '2903953', '2904506', '2904852', '2905156', '2907400', '2908309', '2909406', '2910107', '2911907', '2912400', '2917201', '2917409', 
					'2917805', '2918308', '2918506', '2918753', '2918803', '2919009', '2919108', '2919801', '2921104', '2921302', '2923050', '2923357', '2923902', '2924702', '2925709', '2925758', 
					'2925808', '2926657', '2927408', '2927507', '2928059', '2928307', '2929370', '2929404', '2929750', '2930105', '2930204', '2930501', '2930758', '2930808', '2932457', '2932507', 
					'2933000', '2933158', '2933208', '2933604', '3101805', '3102001', '3105608', '3106606', '3108107', '3113701', '3114204', '3116159', '3120870', '3124708', '3126703', '3127602', 
					'3130051', '3130556', '3132701', '3137106', '3138658', '3140308', '3140704', '3144656', '3145604', '3146206', '3146750', '3147402', '3148103', '3149309', '3149804', '3153004', 
					'3154101', '3154457', '3156809', '3158607', '3159803', '3160405', '3170404', '3170651', '3200508', '3201407', '3201704', '3201803', '3202256', '3203163', '3205150', '3205176', 
					'3300936', '3301702', '3301801', '3302304', '3303955', '3304151', '3304557', '3305505', '3305554', '3305752', '3305901', '3528403', '3537800', '3543253', '3544251', '3557006', 
					'4103156', '4106555', '4107009', '4107256', '4108650', '4111001', '4112900', '4113254', '4113452', '4115853', '4120358', '4121208', '4122800', '4123105', '4128633', '4200051', 
					'4202438', '4203154', '4204459', '4205555', '4206405', '4207700', '4209177', '4210704', '4211801', '4300034', '4301651', '4304853', '4305975', '4307815', '4309506', '4310876', 
					'4311205', '4311239', '4311627', '4311643', '4313201', '4313359', '4313375', '4314068', '4314456', '4315503', '4319703', '4320008', '4320453', '4322608', '4323457', '5000203', 
					'5002159', '5007703', '5100300', '5100607', '5102637', '5103361', '5103437', '5103601', '5104203', '5105002', '5105309', '5105903', '5106000', '5106232', '5106273', '5107156', 
					'5107305', '5107354', '5107750', '5107792', '5107883', '5107909', '5107941', '5107958', '5108808', '5200829', '5200902', '5201108', '5201306', '5202155', '5202502', '5203575', 
					'5203807', '5203962', '5204003', '5204102', '5204300', '5204557', '5204706', '5205406', '5205455', '5205471', '5205802', '5205901', '5206206', '5206404', '5206503', '5206701', 
					'5207402', '5207535', '5208608', '5209291', '5209457', '5209804', '5210158', '5210208', '5210802', '5212006', '5213400', '5213772', '5213806', '5214051', '5214101', '5214507', 
					'5214879', '5215207', '5215231', '5215306', '5216908', '5217708', '5218904', '5219001', '5220686', '5221197', '5300108' 
					);
					$boMunicipiosExcecao = in_array( cte_pegarMuncod( $_SESSION['inuid'] ), $arMunicipiosExcecao );

					$arFases = array( CTE_ESTADO_PAR, CTE_ESTADO_ANALISE, CTE_ESTADO_ANALISE_FIN, CTE_ESTADO_PARECER, CTE_ESTADO_FNDE );

					$sql = "select estuf from cte.instrumentounidade where inuid = {$_SESSION['']}";
					$arInuidsEstados = array(29, 26, 33, 40, 39, 48, 30, 31);
					
					// Regra solicitada pelo Fernando em 17/11/2010 para liberar a adesao para os municipios abaixo
//					$arInuidsMunicipios = array(4562, 335, 2652, 299, 5767, 2124, 2152, 1339);
					$arInuidsMunicipios = array();
					
					//if( !$obInstrumentoUnidade->usucpfescolaativa && in_array( $estado_documento['esdid'], $arFases ) && $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){					
					if( !$obEscolaAtiva->usucpfescolaativa && ((in_array( $estado_documento['esdid'], $arFases ) && $_REQUEST['csFiltroArvore'] != 32)) ){
//					if(!$obEscolaAtiva->esaano){
					
					?>

			<div id="divTermoAdesao">
			
			<? if(true) : ?><!--
			
		  <p>Senhores/as  Gestores/as  informamos que este Sistema ainda permanecerá em teste por alguns dias, e oportunamente apresentaremos e disponibilizaremos as condições oficiais para a adesão ao Programa Escola da Terra.</p>
		  <p>Agradecemos a compreensão de todos/as.</p>
		  <p>Coordenação Geral de Políticas de Educação do Campo/SECADI/MEC</p>
			
			--><? else : ?>
			<div id="divTextoTermo">

			<div id="cabecalho"><img width="80" height="80" src="/imagens/brasao.gif" />
			
			

<?php if(1 == 2){	  ?>
			

			<p>MINISTÉRIO DA EDUCAÇÃO</p>
			<p>SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO  DIVERSIDADE E INCLUSÃO</p>
			<p>DIRETORIA DE POLÍTICAS DE EDUCAÇÃO DO CAMPO, INDÍGENA, E PARA AS RELAÇÕES ÉTNICO-RACIAIS</p>
			</div>
			<?php if(in_array($_SESSION['inuid'], $arInuidsEstados)): ?>
				<div>
				<p>Senhor (a) Secretário (a) de Educação,</p>
				
				<p>O Ministério da Educação, no âmbito das ações do Plano de Desenvolvimento da Educação, lança o Programa Nacional de Educação do Campo, que estabelece dentre seus atos o Programa Escola da Terra, para beneficiar estados e municípios de todas as regiões do País. O Programa destina-se às escolas do campo que oferecem os anos iniciais do ensino fundamental, nos sistemas municipal e estadual, em turmas organizadas sob a forma de multisseriação. Para o ano em curso, será feita a adesão dos estados e municípios que não foram atendidos pelo Programa “Escola Ativa”.</p> 
				<p>Para fazer a adesão, o (a) secretário (a) de educação deverá acessar o Plano de Ações Articuladas do estado/município no Simec, pelo endereço eletrônico http://simec.mec.gov.br, onde está disponível o Termo de Adesão. Para orientar o processo de adesão, encaminha-se um arquivo com os procedimentos a serem adotados (Passo a Passo da Adesão 2012 – Programa Escola da Terra).</p> 
				<p>Ressalta-se que a adesão pode ser realizada pelos dirigentes que têm escolas com classes multisseriadas na sua rede de ensino e tem interesse em implementar o Programa Escola da Terra; para aqueles que já aderiram ao Programa Escola Ativa em anos anteriores e desejam expandir para outras escolas multisseriadas da rede, que porventura ainda não foram atendidas, o processo poderá ser realizado a partir de 2013.</p>
				<p>Os Dirigentes podem fazer a adesão, no período compreendido entre......</p>
				
				<p>Colocamo-nos à disposição para quaisquer esclarecimentos que se fizerem necessários.</p>
				
				<p>Atenciosamente,</p>
				<p>Coordenação Geral de Políticas em Educação do Campo/SECADI/MEC.</p>
				<p>Coordenacaoeducampo@mec.gov.br</p>
				</div>
				
				</div>
				<input type="button" onclick="tratarAdesao( 7 );"
					name="concordoTermo" id="concordoTermo"
					value="Confirmo parceria" /> 
				<input type="button"
					onclick="tratarAdesao( 2 );" name="naoConcordoTermo"
					id="naoConcordoTermo" value="Não confirmo parceria" /> 
				</div>
			<?php else: ?>
				<div>
				<p>Senhor (a) Secretário (a) de Educação,</p>
				
				<p>O Ministério da Educação, no âmbito das ações do Plano de Desenvolvimento da Educação, lança o Programa Nacional de Educação do Campo, que estabelece dentre seus atos o Programa Escola da Terra, para beneficiar estados e municípios de todas as regiões do País. O Programa destina-se às escolas do campo que oferecem os anos iniciais do ensino fundamental, nos sistemas municipal e estadual, em turmas organizadas sob a forma de multisseriação. Para o ano em curso, será feita a adesão dos estados e municípios que não foram atendidos pelo Programa “Escola Ativa”.</p> 
				<p>Para fazer a adesão, o (a) secretário (a) de educação deverá acessar o Plano de Ações Articuladas do estado/município no Simec, pelo endereço eletrônico http://simec.mec.gov.br, onde está disponível o Termo de Adesão. Para orientar o processo de adesão, encaminha-se um arquivo com os procedimentos a serem adotados (Passo a Passo da Adesão 2012 – Programa Escola da Terra).</p> 
				<p>Ressalta-se que a adesão pode ser realizada pelos dirigentes que têm escolas com classes multisseriadas na sua rede de ensino e tem interesse em implementar o Programa Escola da Terra; para aqueles que já aderiram ao Programa Escola Ativa em anos anteriores e desejam expandir para outras escolas multisseriadas da rede, que porventura ainda não foram atendidas, o processo poderá ser realizado a partir de 2013.</p>
				<p>Os Dirigentes podem fazer a adesão, no período compreendido entre......</p>
				
				<p>Colocamo-nos à disposição para quaisquer esclarecimentos que se fizerem necessários.</p>
				
				<p>Atenciosamente,<br>
				Coordenação Geral de Políticas em Educação do Campo/SECADI/MEC.<br>
				Coordenacaoeducampo@mec.gov.br</p>
				</div>
				
				</div>
				<input type="button" onclick="tratarAdesao( 1 );"
					name="concordoTermo" id="concordoTermo"
					value="Iniciar Processo de Adesão" /> <input type="button"
					onclick="tratarAdesao( 2 );" name="naoConcordoTermo"
					id="naoConcordoTermo" value="Não Confirmo a Adesão" /> <input
					type="button" onclick="tratarAdesao( 3 );"
					name="naoPossuiMultisseriadas" id="naoPossuiMultisseriadas"
					value="Não há Classes Multisseriadas" /></div>
			<?php endif; ?>
<?php } ?>

			<? endif; ?>
				<?php 
				} elseif( ($obEscolaAtiva->esasituacaoadesao == 1 || $obEscolaAtiva->esasituacaoadesao == 7) && ( $csFiltroArvore == CTE_FILTRO_ARVORE_ESCOLA_ATIVA || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ) { 
				?>
					<div id="bloco2" style="overflow: hidden;"><!--
						<h3>Escola Ativa</h3>
						<p>
							<a href="javascript: arvoreA.openAll();">Abrir Todos</a>
							&nbsp;|&nbsp;
							<a href="javascript: arvoreA.closeAll();">Fechar Todos</a>
						</p>
						<div id="_arvoreA"></div>
						--> 
					<? require_once('escolaAtivaNova.inc'); ?>
					</div>
				<?php 
				} elseif ( ($obEscolaAtiva->esasituacaoadesao != 1 && $obEscolaAtiva->esasituacaoadesao != 7) && ( $csFiltroArvore == CTE_FILTRO_ARVORE_ESCOLA_ATIVA || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ) {
							//alert("O município não possui escola ativa! Adesão expirou no dia 31/08/2009");
//							echo "O município não possui escola ativa.";
							
							if($obEscolaAtiva->esasituacaoadesao == 2){
								echo "Não confirmou a adesão.";	
							} elseif ($obEscolaAtiva->esasituacaoadesao == 3){
								echo "Não há classes multisseriadas.";
							} else {
								echo "O município não possui escola ativa.";
							}
				}
							
				$perfis = cte_arrayPerfil();
				if( ( count($perfis == 1)) && ( in_array(CTE_PERFIL_CONSULTA_GERAL, $perfis ) || in_array(CTE_PERFIL_CONSULTA_ESTADUAL, $perfis ) || in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
					//não altera
					$habil = 'N';
				}else{
					$habil = 'S';
				}


				$arFases = array( CTE_ESTADO_PAR, CTE_ESTADO_ANALISE, CTE_ESTADO_ANALISE_FIN, CTE_ESTADO_PARECER, CTE_ESTADO_FNDE );
				$arInuids = array(5043, 3759, 3177, 2905, 5465, 4647);
				//if( !$obInstrumentoUnidade->usucpfproletramento && in_array( $estado_documento['esdid'], $arFases ) && $habil == 'S' && $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
				if( (!$obInstrumentoUnidade->usucpfproletramento && in_array( $estado_documento['esdid'], $arFases ) && $habil == 'S' &&
				$itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL)  ||
				(!$obInstrumentoUnidade->usucpfproletramento && in_array($_SESSION['inuid'], $arInuids))){
				?> 
				<!-- coloquei o if para não aparecer na escola ativa --> 
				<? if($csFiltroArvore != 2 && $csFiltroArvore != 5 && $csFiltroArvore != 6) { ?>
					<br />
					<br />
					<h3><a href="javascript: tratarAdesao( 4 );">Aderir ao Pró-Letramento</a></h3>
					<a href="javascript: tratarAdesao( 4 );"><img style="border: none;"
						src="/imagens/pro_letramento.jpg	" /></a> <? } ?> <?php } 
						elseif( $obInstrumentoUnidade->usucpfproletramento && ( $csFiltroArvore == CTE_FILTRO_ARVORE_PRO_LETRAMENTO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ){ ?>
					<div id="bloco2" style="overflow: hidden;"><br />
					<br />
					<h3>Pró-Letramento</h3>
					<p><a href="javascript: arvoreP.openAll();">Abrir Todos</a>
					&nbsp;|&nbsp; <a href="javascript: arvoreP.closeAll();">Fechar Todos</a>
					</p>
					<div id="_arvoreP"></div>
					</div>
				<?php 
				}

				/***************************
				 * VERIFICA PRÓ-FUNCIONÁRIO
				 */
	
				if(!$obInstrumentoUnidade->usucpfprofuncionario) {
					if($csFiltroArvore == 5) {
				?> 
				<br />
				<br />
				<h3><a href="javascript: tratarAdesao( 5 );">Aderir ao
				Pró-Funcionário</a></h3>
				<a href="javascript: tratarAdesao( 5 );"><img style="border: none;" src="/imagens/pro_letramento.jpg" /></a> 
				<?php 
					}
				} elseif( $obInstrumentoUnidade->usucpfprofuncionario && ( $csFiltroArvore == CTE_FILTRO_ARVORE_PRO_FUNCIONARIO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ){ 
				?>
					<div id="bloco2" style="overflow: hidden;">
					<br />
					<br />
					<h3>Pró-Funcionário</h3>
					<p><a href="javascript: arvoreP.openAll();">Abrir Todos</a>
					&nbsp;|&nbsp; <a href="javascript: arvoreP.closeAll();">Fechar Todos</a>
					</p>
					<div id="_arvoreP"></div>
					</div>
				<?php 
				}
	
				/********************************
				 * VERIFICA FORMAÇÃO PELA ESCOLA
				 */
				if(!$obInstrumentoUnidade->usucpfformacaoescola && date('Ymd') <= '20100416' && $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
					if($csFiltroArvore != 2 && $csFiltroArvore != 3 && $csFiltroArvore != 5 ) {
				?> 
						<br />
						<br />
						<div style="float: left; width: 200px;">
							<center>
								<h3><a href="javascript: tratarAdesao( 6 );">Aderir ao Formação pela Escola</a></h3>
								<a href="javascript: tratarAdesao( 6 );"><img style="border: none;" src="/imagens/formacao_pela_escola_par.jpg" /></a>
							</center>
						</div>
				<?php
					}
				} elseif( $obInstrumentoUnidade->usucpfformacaoescola && ( $csFiltroArvore == CTE_FILTRO_ARVORE_FORMACAO_ESCOLA || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ) { 
				?>
					<div id="bloco2" style="overflow: hidden;">
						<br />
						<br />
						<h3>Formação pela Escola</h3>
						<p>
							<a href="javascript: arvoreP.openAll();">Abrir Todos</a>
							&nbsp;|&nbsp; 
							<a href="javascript: arvoreP.closeAll();">Fechar Todos</a>
						</p>
						<div id="_arvoreFE"></div>
					</div>
				<?php } ?>
				<?php if(in_array(CTE_PERFIL_EQUIPE_ESCOLA_ATIVA, $perfis)): ?>
					<?php if($obEscolaAtiva->esasituacaoadesao != '' || $obEscolaAtiva->esasituacaoadesao != null): ?>
						<script>
						function reiniciaescolaativa(){
							if(!confirm("Deseja reiniciar a adesão do Escola Ativa?")){
								return false;
							}
	
							document.rsescolaativa.submit();
						}	
						</script>
						<form action="" method="post" id="rsescolaativa" name="rsescolaativa">
							<input type="hidden" id="resetaEscolaAtiva" name="resetaEscolaAtiva" value="true" />
							<input type="button" onclick="reiniciaescolaativa()" value="Reiniciar escola ativa" />
						</form>
					<?php endif; ?>
				<?php endif; ?>
			</td>
			<td style="background-color: #fafafa; color: #404040; width: 1px;" valign="top">
				<?php
				//esconde a barra lateral, final da linha = 1262
				if ($csFiltroArvore != 2) {
					
					/*
					 * Barra de estado atual e ações e Historico
					 * quando for diferente do perfil estrategico e equipe 
					 * tecnica mec municipal, exibe a barra
					 */
					$muncod_barra = cte_pegarMuncod( $_SESSION['inuid'] );

					wf_desenhaBarraNavegacao( $docid, array( 'inuid' => $_SESSION['inuid'] ) );

				?> 
				<!-- AÇÕES --> 
				<script>
					var verificaPendencias=true;
					function verificaPendencia(){
					
						return windowOpen('cte.php?modulo=principal/verificapendencias&acao=C', 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
					}
					
					function abrirsistema( sisid ) {
						location.href = "../muda_sistema.php?sisid=" + sisid;
					} 
				</script>
					<?php
					if(($estado_documento["esdid"] == CTE_ESTADO_PAR) ){
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
							<tr style="background-color: #c9c9c9; text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="Cadastramento de parecer"> <strong>Preenchimento</strong> </span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><a
									id="verificaPendencia" title="Verificar pendencias"
									onclick="verificaPendencia();">Verificar pendências</a></td>
							</tr>
						</table>
						<br />
						<br />
					<?php
					}
					
					$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA);
					$possuiPerfil = cte_possuiPerfil($perfis);
					
					if(($estado_documento["esdid"] == CTE_ESTADO_ANALISE_FIN || $estado_documento["esdid"] == CTE_ESTADO_ANALISE) && $possuiPerfil  ) {
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom: 20px;">
							<tr style="background-color: #c9c9c9; text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><span
									title="Cadastramento de parecer"> <strong>Preenchimento</strong> </span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><script
									type="text/javascript">function abrePendencias(){window.open('cte.php?modulo=principal/pendencia&acao=C','gera','width=600,height=750,scrollbars=1');}</script>
								<a style="cursor: pointer" onclick="abrePendencias();">Verificar pendências</a></td>
							</tr>
						</table>
						<br />
					<?
					}
					// Btn Gerar Termo de Cooperação e Parecer
					$sql = "
					        select
					            pe.pflcod,
					            pe.pfldsc
					        from
					            seguranca.perfilusuario pu
					        inner join
					            seguranca.perfil pe
					        on
					            pu.pflcod = pe.pflcod
					        where
					            usucpf = '" . $_SESSION['usucpf'] . "' and
					            pe.pfldsc like 'Super%'";
					
					$res          = $db->carregar($sql);
					$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR);
					$possuiPerfil = cte_possuiPerfil($perfis);

					if($possuiPerfil && ($estado_documento["esdid"] ==  CTE_ESTADO_ANALISE  or  $estado_documento["esdid"] == CTE_ESTADO_ANALISE_FIN) ){
					?>

						<table border="0" cellpadding="3" cellspacing="0"
							style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom: 20px;">
							<tr style="background-color: #c9c9c9; text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><span
									title="Cadastramento de parecer"> <strong>Gerar</strong> </span></td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><script
									type="text/javascript">function abreGeracaoDeDocumentos(){window.open('/cte/geratermoparecer.php','gera','width=50,height=50,scrollbars=0');}</script>
								<a style="cursor: pointer" onclick="abreGeracaoDeDocumentos();">Termo
								de Cooperação e Parecer</a></td>
							</tr>
						</table>
					<?php
					}
					
					$sql = "select usucpf from seguranca.usuario_sistema where usucpf = '".$_SESSION['usucpf']."' AND sisid = 23 AND susstatus = 'A'";
					if( $db->pegaUm( $sql ) ){
					
					?>
					<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
						<tr style="background-color: #c9c9c9; text-align: center;">
							<td style="font-size: 7pt; text-align: center;">
								<span title="Lista de Documentos para assinatura."><strong>PAR 2010</strong></span> 
		                    </td>
						</tr>
						
						<tr style="text-align: center;">
							<td style="font-size: 7pt; text-align: center;">
								<a style="cursor: pointer" onclick="abrirsistema(23);" title="Abrir o PAR 2010">Abrir o PAR 2010</a>
							</td>
						</tr>
					</table><br><br>
								
					
					<?php  }
					
					$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR,  CTE_PERFIL_EQUIPE_TECNICA);
					$possuiPerfil = cte_possuiPerfil($perfis);
					
					if($possuiPerfil && $estado_documento["esdid"] ==  CTE_ESTADO_ANALISE  or  $estado_documento["esdid"] == CTE_ESTADO_ANALISE_FIN){
					?>

						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom: 20px;">
							<?php
							
							if($possuiPerfil && $estado_documento["esdid"] ==  CTE_ESTADO_ANALISE ){
								
							?>
							<tr style="background-color: #c9c9c9; text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="Cadastramento de parecer"> <strong>Visualizar</strong> </span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<script type="text/javascript">
										function abreVisualizacaoDeDocumentos(documento){
											window.open('/cte/visualizatermoouparecer.php?documento='+documento,'gera','width=800,height=600,scrollbars=1');
										}
									</script> 
									<a style="cursor: pointer" onclick="abreVisualizacaoDeDocumentos('termo');">
										Termo de Cooperação Atual
									</a>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a style="cursor: pointer" onclick="abreVisualizacaoDeDocumentos('parecer');">Parecer Atual</a>
								</td>
							</tr>
							<?php
							}
							
							$arPerfisVisualizarHistorico = array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN, CTE_PERFIL_ADMINISTRADOR );
							
							if( cte_possuiPerfil( $arPerfisVisualizarHistorico ) ){ 
							?>
							<tr style="background-color: #c9c9c9; text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><span
									title="lista de parecer e Termos"> <strong>Histórico</strong> </span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;"><script
									type="text/javascript">
											function abreListaDeDocumentos(){
												window.open('/cte/listatermoparecer.php','gera','width=800,height=600,scrollbars=0');
											}
										</script> <a style="cursor: pointer"
									onclick="abreListaDeDocumentos();">Termo de Cooperação e Parecer</a>
								</td>
							</tr>
							<?php } ?>
						</table>
					<?php
					}
					$perfisHistoricoFinanJuridico 	= array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_TECNICA_FNDE);
					$possuiPerfilHistoricoFJ 		= cte_possuiPerfil($perfisHistoricoFinanJuridico);
					
					if($possuiPerfilHistoricoFJ && !($estado_documento["esdid"] == CTE_ESTADO_PARECER) ){
					?>
						<table border="0" cellpadding="3" cellspacing="0"
							style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
							<tr style="background-color: #c9c9c9; text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<script type="text/javascript">
									 	function abrirHistoricoPareceresEngFin(){
									 		windowOpen('cte.php?modulo=principal/historicoFinanceiroEngenharia&acao=A', 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
									 	}
									 </script> 
								 	<span title="Histórico de parecer de Engenharia e Financeiro"> 
								 		<strong>Histórico de parecer</strong> 
								 	</span>
						 		</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a href="javascript:void(0);" onclick="abrirHistoricoPareceresEngFin();" title="Histórico de parecer de Engenharia e Financeiro">
										Financeiro e Engenharia
									</a>
								</td>
							</tr>
						</table>
						<br>
						</br>
					<?php
					}
					
					if ( defined('CTE_ESTADO_PARECER') && $estado_documento["esdid"] == CTE_ESTADO_PARECER ) {

						$sql = "
					        select
					            pe.pflcod,
					            pe.pfldsc
					        from
					            seguranca.perfilusuario pu
					        inner join
					            seguranca.perfil pe
					        on
					            pu.pflcod = pe.pflcod
					        where
					            usucpf = '" . $_SESSION['usucpf'] . "' and
					            pe.pfldsc like '%Parecerista %' OR pe.pfldsc like 'Super%'";
					
						$res = $db->carregar($sql);
					
						$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA);
						$possuiPerfil = cte_possuiPerfil($perfis);
					
						if (is_array($res)) {
							$fisico     = false;
							$engenharia = false;
							$juridico   = false;
					
							foreach ($res as $linha) {
								$fisico     = $fisico     || trim($linha['pfldsc']) == 'Parecerista Fisico/Financeiro';
								$engenharia = $engenharia || trim($linha['pfldsc']) == 'Parecerista da Engenharia';
								$juridico   = $juridico   || trim($linha['pfldsc']) == 'Parecerista Jurídico PAR';
							}
						}
					
						if ($possuiPerfil || $fisico || $engenharia || $juridico) {
					?>
							<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
								<tr style="background-color: #c9c9c9; text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<span title="Cadastramento de parecer"> 
											<strong>parecer</strong> 
										</span>
									</td>
								</tr>
								<?php
								if ($possuiPerfil || $fisico) {
								?>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<a href="cte.php?modulo=principal/cadastrarparecer&acao=A" title="Cadastramento de parecer Físico Financeiro">
											Físico Financeiro
										</a>
									</td>
								</tr>
								<?php
								} if ($possuiPerfil || $engenharia) {
								?>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<a href="cte.php?modulo=principal/cadastrarparecer&acao=B" title="Cadastramento de parecer de Engenharia">Engenharia</a>
									</td>
								</tr>
								<?php
								}
						}
					}

					$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA);
					$possuiPerfil = cte_possuiPerfil($perfis);

					if ($possuiPerfil && $estado_documento["esdid"] == CTE_ESTADO_FNDE) {
					?>
				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="Cadastramento de parecer"><strong>FNDE</strong></span> <script
							type="text/javascript">
                            	function enviarPar(){
                            		//window.location.href = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=principal/enviar&acao=A';return false;
                            		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/enviar&acao=A','Envio de projeto para o SAPE','width=1024,height=600,scrollbars=1');
                            	}
                            </script></td>
					</tr>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							href="javascript:void(0);" onclick="return enviarPar();"
							title="Enviar para o SAPE">Enviar para o SAPE</a></td>
					</tr>
				</table>
				<br />

				<?php
				$perfis       = array(CTE_PERFIL_SUPER_USUARIO);
				$possuiPerfilSuper = cte_possuiPerfil($perfis);
				if($possuiPerfilSuper){ ?>
				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="Cadastramento de parecer"><strong>FNDE</strong></span> <script
							type="text/javascript">
	                            	function visualizarPAR(){
	                            		//window.location.href = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=principal/enviar&acao=A';return false;
	                            		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/visualizarXML&acao=A','Envio de projeto para o SAPE','width=1024,height=600,scrollbars=1');
	                            	}
	                            </script></td>
					</tr>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							href="javascript:void(0);" onclick="return visualizarPAR();"
							title="Gerar Convênio no FNDE">Visualizar XML</a></td>
					</tr>
				</table>
				<br />
				<?php
}
//}else
//{
$envia_docid = cte_pegarDocid( $_SESSION['inuid'] );
$envia_documento = wf_pegarEstadoAtual( $envia_docid );

$envia_esdid = (integer) $envia_documento['esdid'];
$perfis = array( CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_ALTA_GESTAO, CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA );
$possuiPerfil = cte_possuiPerfil( $perfis );
?>

<?php if ( $estado_documento["esdid"] == CTE_ESTADO_FNDE ){ ?>
				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="Cadastramento de parecer"><strong>PTA</strong></span> <script
							type="text/javascript">
						function visualizarPTA(){
							window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/pta/visualizarPTA&acao=A','Visualizar PTA','width=500,height=300,scrollbars=1');
						}
					</script></td>
					</tr>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							href="javascript:void(0);" onclick="return visualizarPTA();"
							title="Visualizar PTA">Visualizar PTA</a></td>
					</tr>
				</table>
				<br />
				<?php } ?>


				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="Cadastramento de parecer"><strong>FNDE</strong></span> <script
							type="text/javascript">function verRelatorioEnvioFnde(){window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/enviar_relatorio&acao=A','RelatoriodeConvenio','width=1024,height=600,scrollbars=1');}</script>
						</td>
					</tr>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							style="cursor: pointer" onclick="verRelatorioEnvioFnde();">Relatório
						de Envio de Convênio</a></td>
					</tr>
				</table>
				<br />
				<?php
				// }
}
if ($possuiPerfil && $estado_documento["esdid"] == CTE_ESTADO_FINALIZADO) {


	?>

				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="Cadastramento de parecer"><strong>FNDE</strong></span> <script
							type="text/javascript">function verRelatorioEnvioFnde(){window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/enviar_relatorio&acao=A','RelatoriodeConvenio','width=1024,height=600,scrollbars=1');}</script>
						</td>
					</tr>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							style="cursor: pointer" onclick="verRelatorioEnvioFnde();">Relatório
						de Envio de Convênio</a></td>
					</tr>
				</table>
				<br />

				<?
}

$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR);
$possuiPerfil = cte_possuiPerfil($perfis);

if ($possuiPerfil && $estado_documento["esdid"] == CTE_ESTADO_FNDE) {
	?>
				<!--  
	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="Deletar convênio e números de processos."><strong>FNDE</strong></span>
               		<script type="text/javascript">function 
                     	deletarNumProcessos(){
                     		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/deletarNumProcessos.php?acao=A','blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
                    	 }
					</script>
			</td>
		</tr>
		<tr style="text-align:center;">
			<td style="font-size:7pt; text-align:center;">
	        	<a href="javascript:void(0);" onclick="return deletarNumProcessos();" title="Deletar Convênio e números de processos.">Deletar Convênio e números de processos no SIMEC.</a>
			</td>
		</tr>
	
    </table><br />
    	-->


				<!-- Menu Gerencia Projetos-->

    	<?
}
$sql = sprintf("SELECT
						esdordem
					FROM 
						workflow.documento d
						inner join workflow.estadodocumento ed on ed.esdid = d.esdid
					WHERE
						d.docid = %d;", $docid);

$esdOrdem = $db->pegaUm($sql);

$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_TECNICA );
$possuiPerfil = cte_possuiPerfil($perfis);

if($possuiPerfil ){
	?>
				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom: 20px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="Cadastramento de parecer"> <strong>Projetos</strong> </span>
						</td>
					</tr>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><script
							type="text/javascript">
						function abreGerenciaProjetos(){
							window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/par_listaprojetos&acao=A','gera','width=700,height=250,scrollbars=1');
						}
						</script> <a style="cursor: pointer"
							onclick="abreGerenciaProjetos();"> <!-- Termo de Cooperação e  -->Gerênciar<br>
						Projetos </a>
						</td>
					</tr>
				</table>
				<?php
}

$muncod_barra = cte_pegarMuncod( $_SESSION['inuid'] );
/*
 if (cte_possuiPermissaoUfEstrategico($_SESSION['inuid']) &&
 cte_possuiPermissaoMunEstrategico($_SESSION['inuid']) &&
 cte_possuiPerfilMunicipio(CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN, $muncod_barra) )
 */
?>
				<table border="0" cellpadding="3" cellspacing="0"
					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><span
							title="estado atual"> <b>ferramentas</b> </span></td>
					</tr>
					<tr style="text-align: center;">
						<td
							style="font-size: 7pt; text-align: center; border-bottom: 1px solid #c9c9c9;">
						<a style="cursor: pointer" onclick="return false;"> <img
							src="/imagens/print.png" alt="Versão para impressão"
							onclick="relatorioImpressao( 'A' )"> </a></td>
					</tr>
					<br>

					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							style="cursor: pointer"
							onclick="relatorioImpressaoAssFinaceira( 'A' )"> <img
							src="/imagens/print.png" alt="Versão para impressão"> <br>
						Relatório de Assistência Financeira. </a></td>
					</tr>

					<?php
					if (!IS_PRODUCAO && $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) :
					?>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							onclick="relatorioImpressaojan2008( 'A' )"
							style="cursor: pointer" onclick="return false;"> <img
							src="/imagens/print.png" alt="Versão para impressão"><br>
						Relatório Jan 2008 </a></td>
					</tr>
					<?php
					endif;
					if ( cte_podeAnalisar( $_SESSION['inuid'] ) ) : ?>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><!-- <a style="cursor: pointer" onclick="relatorioImpressao2( 'A' )"> -->
						<a style="cursor: pointer"
							onclick="alert( 'Clique na subação que deseja analisar.' );">
						Analisar </a></td>
					</tr>
					<?php
					endif;
					if ( cte_podeVerRelatorioParCopia( $_SESSION['inuid'] ) ) : ?>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							style="cursor: pointer" onclick="relatorioCompleto('C');">
						Completo (Original) </a></td>
					</tr>
					<?php
					endif;
					if ( cte_podeVerRelatorioParAtual( $_SESSION['inuid'] ) ) : ?>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							style="cursor: pointer" onclick="relatorioCompleto('A');">Completo
						(Atual)</a></td>
					</tr>
					<?php
					endif;

					$arEstadoPar = array( CTE_ESTADO_ANALISE_FIN, CTE_ESTADO_PARECER, CTE_ESTADO_FINALIZADO, CTE_ESTADO_FNDE );
					
					$perfis = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_EQUIPE_TECNICA_FNDE, CTE_PERFIL_EQUIPE_MUNICIPAL );
					$possuiPerfil = cte_possuiPerfil($perfis);

					if ( $possuiPerfil && in_array( $estado_documento["esdid"], $arEstadoPar ) && ($itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL) && cte_verificaTermo($_SESSION['inuid'])){
					?>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><script
							type="text/javascript">function emitirTermo(){var janela = window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/documento/termo&acao=A','termo','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');janela.focus();}</script>
						<a style="cursor: pointer" onclick="emitirTermo();">Emitir<br />
						Termo de Cooperação</a></td>
					</tr>
					<?php } ?>
					
					<?php
						$perfis = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_EQUIPE_TECNICA_FNDE );
						$possuiPerfil = cte_possuiPerfil($perfis);
						
						if (($possuiPerfil || $envia_esdid == CTE_ESTADO_FINALIZADO) && ($itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL) && cte_verificaTermo($_SESSION['inuid'])){
					?>
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><script
							type="text/javascript">function emitirParecer(){var janela = window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/documento/parecer&acao=A','parecer','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');janela.focus();}</script>
						<a style="cursor: pointer" onclick="emitirParecer();"> Emitir<br />
						Parecer </a></td>
					</tr>
					<?php } ?>
				</table>

			</table>
	
	</tbody>
</table>
					<? }
					else{
						//	$obEscolaAtiva = new EscolaAtiva( "", $_SESSION['inuid'] );

						if( $obEscolaAtiva->esaid ){
							$obEscolaAtiva->verificarDocumentoEscolaAtiva();
							wf_desenhaBarraNavegacao( $obEscolaAtiva->docid, array( 'esaid' => $obEscolaAtiva->esaid ) );
						}
					}

					/**************************************************************************
					 * MOSTRA DIMENSÃO, AREA, INDICADOR QUE CONTEM SUBAÇÃO DO PROJETO/CONVÊNIO
					 */

					if($_REQUEST["csFiltroArvore"] == CTE_FILTRO_ARVORE_CONVENIO && $_REQUEST['pssano']){

						if($subacoesHerancas[0]['sbaid'] > 0){

							$indiceArvore = 0;
							foreach($dimensoes as $dimDados){
									
								foreach($subacoesHerancas as $subDados){
										
									if($dimDados['dimid'] != $subDados['dimid']){
										unset($dimensoes[$indiceArvore]);
									}
								}
									
								$indiceArvore++;
							}

							$indiceArvore = 0;
							foreach($areas as $areDados){
									
								foreach($subacoesHerancas as $subDados){
										
									if($areDados['ardid'] != $subDados['ardid']){
										unset($areas[$indiceArvore]);
									}
								}
									
								$indiceArvore++;
							}

							$indiceArvore = 0;
							foreach($indicadores as $indDados){
									
								foreach($subacoesHerancas as $subDados){
										
									if($indDados['indid'] != $subDados['indid']){
										unset($indicadores[$indiceArvore]);
									}
								}
									
								$indiceArvore++;
							}

						} else {

							$dimensoes = array();
						}
					}



					//ver($indicadores);
					?>


<script type="text/javascript"><!--
	
	function enviaAnoConvenioProjeto(ano, csFiltroArvore){
	
		document.location.href = 'cte.php?modulo=principal/estrutura_avaliacao&acao=A&csFiltroArvore='+csFiltroArvore+'&pssano='+ano;		
	}

	//impressao
	function relatorioImpressaojan2008(ptostatus){
	<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/impressaosalva2008.php?acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php } ?>
	}
	function relatorioImpressao( ptostatus ){
		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressao&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php else : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressao_mun&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php endif; ?>
	}
	
	function relatorioImpressaoAssFinaceira( ptostatus ){
		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressaoAssFinanceira&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php else : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressaoAssFinanceiraMun&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php endif; ?>
	}
	
	//impressao
	function relatorioImpressao2( ptostatus ){
		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressao2&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php else : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressao2_mun&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php endif; ?>
	}
	//impressao
	function relatorioCompleto( ptostatus ){
		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/par_relatorio&acao=R&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php else : ?>
			var janela = window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/par_relatorio_mun&acao=R&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		<?php endif; ?>
	}
	function responder( dimensao, pergunta ){
		window.location = '?modulo=principal/questoes/questoespontuais&acao=C&dimid='+ dimensao +'&prgid='+ pergunta;
	}
	function responderEscolaAtiva( nrQuestao ){
		window.location = '?modulo=principal/questoes/questoesPontuaisEscolaAtiva&acao=C&nrQuestao'+ nrQuestao;
	}
	function avaliar( indicador ){
		window.location = '?modulo=principal/formulario_avaliacao&acao=A&indid=' + indicador;
	}
	function download( arquivo ){
		window.location = '../mostra_arquivo.php?id=' + arquivo;
	}
	function cadastrarAcao( ptoid, acilocalizador ){
		window.location = "?modulo=principal/par_acao&acao=A&ptoid=" + ptoid + "&acilocalizador=" + acilocalizador;
	}
	function alterarAcao( aciid ){
		window.location = "http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=principal/par_acao&acao=A&aciid=" + aciid;
	}
	function excluirAcao( aciid ){
		if ( confirm( "Deseja excluir a ação?" ) ) {
			window.location = "?modulo=principal/par_acao&acao=A&aciid="+ aciid +"&action=remov&origem=<?= urlencode( $_SERVER['QUERY_STRING'] ) ?>";
		}
	}
	function cadastrarSubacao( aciid ){
		var janela = window.open( '?modulo=principal/par_subacao&acao=A&aciid=' + aciid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
		janela.focus();
	}
	function alterarSubacao( sbaid ){
		var janela = window.open( "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
		janela.focus();
	}
	function excluirSubacao(sbaid){if(confirm( "Deseja excluir a subação?")){window.location = "/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A&sbaid=" + sbaid + "&action=remov";}	}
	function cadastrarDadosReferencia(){
		window.location = "cte.php?modulo=principal/orgao_municipal_educacao&acao=A";
	}
	function cadastrarDadosReferenciaEscolaAtiva(){
		window.location = "cte.php?modulo=principal/dadosProgramaEscolaAtiva&acao=A";
	}
	function dadosDemograficosUF( estuf )
	{
		var janela = window.open( 'http://portal.mec.gov.br/ide/2008/gerarTabela.php?estado=' + estuf + '&submit=Gerar+Dados', 'blank', 'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
		janela.focus();
	}
	function dadosDemograficosMun( estuf, muncod )
	{
		var janela = window.open( 'http://portal.mec.gov.br/ide/2008/gerarTabela.php?municipio=' + muncod + '&submit=Gerar+Dados', 'blank', 'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
		janela.focus();
	}
	function gerenciarEscolas()
	{
		window.location.href = '?modulo=principal/escolas&acao=A';
	}
    function gerenciarPlanejamentoEstrategico() {
    	window.location = '/cte/cte.php?modulo=principal/planejamentoestrategico&acao=C';
    }
    var u='/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A&titleFor=';
	arvore = new dTree( "arvore" );
	arvore.config.folderLinks = true;
	arvore.config.useIcons = true;
	arvore.config.useCookies = true; 
	<?php
	$sql = sprintf( "select itrdsc from cte.instrumento where itrid = %d", $itrid );
	$instrumento = $db->pegaUm( $sql );
	?>
	arvore.add( 0, -1, '<?= $instrumento ?>' );
<?php
// inicio perfil estrategico estadual e municipal
if(cte_possuiPermissaoUfEstrategico($_SESSION['inuid']) || cte_possuiPermissaoMunEstrategico($_SESSION['inuid'])){?>
arvore.add( 1, 0, '<b>Planejamento Estratégico</b>', 'javascript:gerenciarPlanejamentoEstrategico();' );
<?}{?>
	arvore.add( 1, 0, '<b>Dados da Unidade</b>' , 'javascript:cadastrarDadosReferencia();' );
	<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>
		<?php $estuf = cte_pegarEstuf( $_SESSION['inuid'] ); ?> 
arvore.add( 2, 0, '<b>Dados Demográficos e Educacionais Quantitativos</b>', 'javascript:dadosDemograficosUF( \'<?= $estuf ?>\' )', '', '', '../includes/dtree/img/imgfolder.gif', '../includes/dtree/img/imgfolder.gif' );
	<?php else : ?>
		<?php
			$muncod = cte_pegarMuncod( $_SESSION['inuid'] );
			$sql = "select estuf from territorios.municipio where muncod ='" . $muncod . "'";
			$estuf_municipio = $db->pegaUm( $sql );
		?>
arvore.add( 2, 0, '<b>Dados Demográficos e Educacionais Quantitativos</b>', 'javascript:dadosDemograficosMun( \'<?= $estuf_municipio ?>\', \'<?= $muncod ?>\' )', '', '', '../includes/dtree/img/imgfolder.gif', '../includes/dtree/img/imgfolder.gif' );
	<?php endif; ?>
arvore.add( 3, 0, '<b>Planejamento Estratégico</b>', 'javascript:gerenciarPlanejamentoEstrategico();' );
arvore.add( 4, 0, '<b>Escolas Atendidas</b>', 'javascript:gerenciarEscolas();' );
	<?php if ( cte_podeVerQuestaoPontual( $_SESSION["inuid"] ) ): ?>
arvore.add( 5, 0, '<b>Questões Pontuais</b>', '', '', '', '../includes/dtree/img/question.gif', '../includes/dtree/img/question.gif' );
		<?php foreach( $dimensoes_q as $dimensao ): 
			$dimensao['dimdsc'] = delimitador($dimensao['dimdsc']);
		?>
			<?php $texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; ?>
arvore.add( 'dq_<?= $dimensao['dimid'] ?>', 5, '<?= $texto ?>' );
		<?php endforeach; 
		      foreach( $perguntas as $pergunta ): 
				$pergunta['prgdsc'] = delimitador($pergunta['prgdsc']);
				$icone= $pergunta['rspid'] ? '../imagens/check_p.gif' : '';
				$cor = $icone ? '#959595' : '#133368';

				$title = "Última Atualização feita por ".str_replace( "'", "\'", $pergunta['usunome'] )." em ".$pergunta['data'];
				if(strlen(trim($pergunta['usunome']))<2 || $pergunta['usunome'] )
					$title = "Nenhuma atualização realizada.";					
				$texto = '<span title="'.$title.'" style="color: '. $cor .';">'. $pergunta['prgcod'] .'. '. $pergunta['prgdsc'] .'</span>';
			?>
arvore.add( 'i_<?= $pergunta['prgid'] ?>', 'dq_<?= $pergunta['dimid'] ?>', '<?= $texto ?>', 'javascript: responder(<?= $pergunta['dimid'] ?>,<?= $pergunta['prgid'] ?>); ', '', '', '<?= $icone ?>' );
		<?php endforeach;
		
		if( $obInstrumentoUnidade->inusituacaoadesao == CTE_ADESAO_ESCOLA_ATIVA_ACEITA ){ 
			$obPergunta = new Pergunta();
			$coPergunta = $obPergunta->recuperarTodos( "prgid, dimid, prgcod, prgdsc", array( "dimid is null" ), "prgcod" ); ?>
					
			arvore.add( 'qpea', 5, 'Escola Ativa' );
			
			<?php foreach( $coPergunta as $obPergunta ){
				$stQuestao = $obPergunta["prgcod"].". ".$obPergunta["prgdsc"]; ?>
						
				arvore.add( 'qp_<?php echo $obPergunta["prgid"] ?> ?>', 'qpea', '<?php echo delimitador( $stQuestao ) ?>', 'javascript: responder( \'<?php echo $obPergunta["dimid"] ?>\', <?php echo $obPergunta["prgid"] ?> ); ' );
			<?php }
		}		
		
	endif;
	$_SESSION ['uf'] = $estuf;
if ( cte_podeVerIndicador( $_SESSION["inuid"] ) ): ?>arvore.add( 6, 0, '<b>Indicadores Qualitativos</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );
		
		<?php foreach( $dimensoes as $dimensao ):		 
		$dimensao['dimdsc'] = delimitador($dimensao['dimdsc']);
		?>
			<?php $texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; ?>
arvore.add( 'd_<?= $dimensao['dimid'] ?>', 6, '<?= $texto ?>' );
		<?php endforeach; ?><?php foreach( $areas as $area ): 
		$area['arddsc'] = delimitador($area['arddsc']);
		?>
			<?php $texto = '<span style="color: #000000;">'. $area['ardcod'] .'. '. $area['arddsc'] .'</span>'; ?>
arvore.add( 'a_<?= $area['ardid'] ?>', 'd_<?= $area['dimid'] ?>', '<?= $texto ?>' );
		<?php endforeach; ?>
		<?php foreach( $indicadores as $indicador ): 
		 $indicador['inddsc'] = delimitador($indicador['inddsc']);
		 		
		 		foreach( $indicadores_Validacoes as $indicadorV ){
		 			if($indicadorV['indid'] == $indicador['indid'] ){
		 				$indicador['acaoestadual'] = $indicadorV['acaoestadual'];
		 				$indicador['acaomunicipal'] = $indicadorV['acaomunicipal'];
		 				$indicador['subacaoestadual'] = $indicadorV['subacaoestadual'];
		 				$indicador['subacaomunicipal'] = $indicadorV['subacaomunicipal'];
		 			}
		 		}
		 
		?>
			<?php
				$icone = '';
				$cor = '';
				$mensagens = array();
				if ( $indicador['ptoid'] ) {
					$icone = '../imagens/check_p.gif';
					$cor = '#959595';
				}
				if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
					if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
						if ( !$indicador['subacaoestadual'] && in_array( $indicador['ctrpontuacao'], array(1,2) ) ) {
							array_push( $mensagens, "A pontuação está baixa." );
						}
						if ( !$indicador['subacaoestadual'] && $indicador['demandaestadual'] > 0 ) {
							array_push( $mensagens, "Há demanda estadual." );
						}
						if ( !$indicador['subacaomunicipal'] && $indicador['demandamunicipal'] > 0 ) {
							array_push( $mensagens, "Há demanda municipal." );
						}
						if ( count( $mensagens ) > 0 ) {
							array_push( $mensagens, "É necessário descrever um plano de ação." );
							$icone = '../imagens/atencao.png';
							$cor = '#202020';
						}
					}
				}
				
				if ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
					if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
						if ( !$indicador['subacaomunicipal'] && in_array( $indicador['ctrpontuacao'], array(1,2) ) ) {
							array_push( $mensagens, "A pontuação está baixa. É necessário descrever um plano de ação." );
						}
						if ( $indicador['acaomunicipal'] && in_array( $indicador['ctrpontuacao'], array(0,3,4) ) ) {
							array_push( $mensagens, "A pontuação não permite que exista ação para este indicador." );
						}
						if ( count( $mensagens ) > 0 ) {
							$icone = '../imagens/atencao.png';
							$cor = '#202020';
						}
					}
				}
				
				//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTE ÍTEM.				
				$title = "Última Atualização feita por ".str_replace( "'", "\'", $indicador['usunome'] )." em ".$indicador['data'];				
				if(strlen(trim($indicador['usunome']))<2)
					$title = "Nenhuma atualização realizada.";	
				
				array_push( $mensagens, $title) ;
				$texto = '<span style="color: '. $cor .';" title="'. implode(' - ', $mensagens ) .'">'. $indicador['indcod'] .'. '. $indicador['inddsc'] .'</span>';
				
			?>
			arvore.add( 'i_<?= $indicador['indid'] ?>', 'a_<?= $indicador['ardid'] ?>', '<?= $texto ?>', 'javascript: avaliar(<?= $indicador['indid'] ?>);', '', '', '<?= $icone ?>', '<?= $icone ?>' );
			
			<?php if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ){?>

				<?php if( cte_podeElaborarPlanoDeAcoes( $_SESSION['inuid'] ) ): ?>
					
					<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ): ?>
						<?php if ( $indicador['demandaestadual'] > 0 || in_array( $indicador['ctrpontuacao'], array(1,2) ) ): ?>
							<?php
							$evento_plano = "";
							$icone_plano = "";
							if ( !$indicador['acaoestadual'] ) {
								$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'E\');";
								$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';
							}
							?>
							arvore.add( 'ipe_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano de Ação da Rede Estadual', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
						<?php endif; ?>
						<?php if ( $indicador['demandamunicipal'] > 0 ): ?>
							<?php
							$evento_plano = "";
							$icone_plano = "";
							if ( !$indicador['acaomunicipal'] ) {
								$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'M\');";
								$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';
							}
							?>
							arvore.add( 'ipm_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano de Ação das Redes Municipais', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
						<?php endif; ?>
					<?php endif; ?>
					
					<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ): ?>
						<?php if ( in_array( $indicador['ctrpontuacao'], array(1,2) ) || $indicador['acaomunicipal'] ): ?>
							<?php
	                      	   // verifica se é município e pequeno município
	                      	   $pequenoMunicipio =
	                      	   cte_pegarMuncod( $_SESSION['inuid'] ) &&
	                      	   !cte_verificaGrandeMunicipio( $_SESSION['inuid'] );
							   $equipeTecnicaMec = cte_possuiPerfil(
	                              array( CTE_PERFIL_EQUIPE_TECNICA )
	                           );
	                           $superUser = $db->testa_superuser();
							?>
							<?php
							if (
								!$pequenoMunicipio || // pode quando não município OU
								(
									$pequenoMunicipio &&
									( $equipeTecnicaMec || $superUser )
								)
							):
							?>
								<?php
								$evento_plano = "";
								$icone_plano = "";
								if ( !$indicador['acaoestadual'] ) {
									$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'M\');";
									$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';
								}
								?>
								arvore.add( 'ipm_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano do Município', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
							<?php endif; ?>
							<?php
							$evento_plano = "";
							$icone_plano = "";
							if ( !$indicador['acaomunicipal'] ) {
								$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'M\');";
								$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';
							}
							?>
							arvore.add( 'ipm_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano de Ação do Município', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
						<?php endif; ?>
					<?php endif; ?>
					
				<?php endif; ?>

			<?php } ?>
			
			
		<?php endforeach; ?>
		
		<?php foreach( $acoes_estaduais as $acao ): 
				$acao['acidsc'] = delimitador($acao['acidsc']);
		?>
			<?php
				$prefixo = '';
//				$prefixo = '<img align="absmiddle" onclick="javascript:alterarAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/alterar.gif" title="alterar ação"/> ';
				if ( $acao['subacoes'] == 0 ) {
					$prefixo .= '<img align="absmiddle" onclick="javascript:excluirAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir ação"/> ';
				}
				
				//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTA AÇÃO
				$title = "Última Atualização feita por ".str_replace( "'", "\'", $acao['usunome'] )." em ".$acao['data'];
				if(strlen(trim($acao['usunome']))<2)
					$title = "Nenhuma atualização realizada.";
				$texto = '<a href="javascript:alterarAcao('. $acao['aciid'] .');"  title="'. $title .'">'. str_replace( "'", "", str_replace( array( "\n", "\r", chr(10), chr(13) ), " ", trim( $acao['acidsc'] ) ) ) .'</a>';
			?>
			arvore.add( 'ai_<?= $acao['aciid'] ?>', 'ipe_<?= $acao['indid'] ?>', '<?= $prefixo ?><?= $texto ?>', 'javascript:void(0);', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
			arvore.add( 'ais_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="cadastrar subação"/> Subações (<?=  $acao['subacoes'] ?>)', 'javascript:cadastrarSubacao(<?= $acao['aciid'] ?>);', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
		<?php endforeach; ?>
		
		<?php  foreach( $acoes_municipais as $acao ): 
					$acao['acidsc'] = delimitador($acao['acidsc']);
		?>
			<?php
				$prefixo = '';
//				$prefixo = '<img align="absmiddle" onclick="javascript:alterarAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/alterar.gif" title="alterar ação"/> ';

                //$verificacao =  ;
				if ( $acao['subacoes'] == 0 && ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL || cte_verificaGrandeMunicipio( $_SESSION['inuid'] ) ) ) {
					$prefixo .= '<img align="absmiddle" onclick="javascript:excluirAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir ação"/> ';
				}
				
				//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTA AÇÃO
				$title = "Última Atualização feita por ".str_replace( "'", "\'", $acao['usunome'] )." em ".$acao['data'];
				if(strlen(trim($acao['usunome']))<2)
					$title = "Nenhuma atualização realizada.";	
				$texto = '<a href="javascript:alterarAcao('. $acao['aciid'] .');" title="'. $title .'">'. str_replace( "'", "", str_replace( array( "\n", "\r", chr(10), chr(13) ), " ", trim( $acao['acidsc'] ) ) ) .'</a>';
			?>
			arvore.add( 'ai_<?= $acao['aciid'] ?>', 'ipm_<?= $acao['indid'] ?>', '<?= $prefixo ?><?= $texto ?>', 'javascript:void(0);', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
			<?php
				$verificacao = ( $estado_documento['esdid'] == CTE_ESTADO_ANALISE ) && ( cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_TECNICA ) ) );
			?>
			<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL || cte_verificaGrandeMunicipio( $_SESSION['inuid'] ) || $verificacao || cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) )  ): ?>
				arvore.add( 'ais_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="cadastrar subação"/> Subações (<?=  $acao['subacoes'] ?>)', 'javascript:cadastrarSubacao(<?= $acao['aciid'] ?>);', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
			<?php else: ?>
				arvore.add( 'ais_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', 'Subações (<?=  $acao['subacoes'] ?>)', '', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );
			<?php endif; ?>
		<?php endforeach; ?>
		
		<?php
		foreach( $subacoes as $subacao ): 
			$subacao['sbadsc'] = delimitador($subacao['sbadsc']);

            if ($subacao['ptostatus'] != 'A')
            	continue;

			if ( $estado_documento['esdid'] == CTE_ESTADO_PAR ) {
            	$icone = "";
			} 
			else {
				$subacao['ssudescricao'] = $subacao['ssudescricao'] ? $subacao['ssudescricao'] : "Não analisado";
				
				if( cte_validarSubAcaoFaseAnalise( $subacao ) ){
					$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
				}
				else {
					$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
				}
			}

                $prefixo = '';
//				$prefixo = '<img align="absmiddle" onclick="javascript:alterarSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/alterar.gif" title="alterar subação"/> ';
                //if ( cte_podeEditarSubacao( $_SESSION['inuid'] ) ) {
                $sql="select sptano from cte.subacaoparecertecnico where sbaid = '".$subacao['sbaid']."' and ssuid is not null";
				$anosParecerDados = $db->carregar($sql);
				$statusSub = $anosParecerDados;
                if( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || 
                      $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO  
                     ) && 
                      cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_MUNICIPAL ) )
                        && $statusSub == NULL
                   )
                {
                    $prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação"/> ';
                }else if(( $estado_documento['esdid'] == CTE_ESTADO_PAR || 
                      $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO  
                     ) && 
                      cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, 
                                               CTE_PERFIL_ADMINISTRADOR) 
                                        ))
                {
                    $prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação 2"/> ';
                }
                
               	$iconeConvenio = "";
                if( $stAnos = cte_recuperarAnosSubacaoConveniada( $subacao["sbaid"] ) ){
                	$iconeConvenio = '<img align="absmiddle" src="/imagens/conveniada.png" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados: '.$stAnos.'."/>';
	                
                	if( cte_possuiAditivo( $subacao["sbaid"] ) ){
	                	$iconeConvenio .= '<img align="absmiddle" src="/imagens/aditivo.png" alt="Sub-ação possui Aditivo" title="Sub-ação possui Aditivo"/>';
                	}	
                }
					

                $ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
                $texto = $icone . $iconeConvenio . $ordem . '<a onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['sbadsc']).'</a>';
            ?>
            arvore.add( 'sa_<?= $subacao['sbaid'] ?>', 'ais_<?= $subacao['aciid'] ?>', '<?= $prefixo ?><?= $texto  ?>', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
		<?php

        endforeach;
    endif;
}// fim perfil estrategico estadual ?>
	
	<?php if( $csFiltroArvore == CTE_FILTRO_ARVORE_PRINCIPAL || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ){ ?>
		elemento = document.getElementById( '_arvore' );
		elemento.innerHTML = arvore;
	<?php } elseif( ($csFiltroArvore == CTE_FILTRO_ARVORE_CONVENIO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS) && $_REQUEST['pssano']) { ?>
		elemento = document.getElementById( '_arvorePC' );
		elemento.innerHTML = arvore;
	<?php } elseif( $csFiltroArvore == CTE_FILTRO_ARVORE_CONVENIO && !isset($_REQUEST['pssano'])) {
		 
		$pssano = 	"
					<select id=\"pssano\" name=\"pssano\">
						<option>Selecione um ano...</option>
						<option onclick=\"enviaAnoConvenioProjeto(2007, 7)\">2007</option>
						<option onclick=\"enviaAnoConvenioProjeto(2008, 7)\">2008</option>
						<option onclick=\"enviaAnoConvenioProjeto(2009, 7)\">2009</option>
						<option onclick=\"enviaAnoConvenioProjeto(2010, 7)\">2010</option>
						<option onclick=\"enviaAnoConvenioProjeto(2011, 7)\">2011</option>
						<option onclick=\"enviaAnoConvenioProjeto(2012, 7)\">2012</option>
					</select>
					";
		?>
		elemento = document.getElementById( '_arvorePC' );				
		elemento.innerHTML = '<br><h3>Projeto/Convênio</h3>Ano de referência: '+<?=$pssano?>;
	<?php } ?>

	/*
	 * adaptação de largura e controle de overflow de conteúdo para o internet explorer
	 */
	var dav = navigator.appVersion;
	IE = document.all ? true : false;
	IE6 = dav.indexOf( "MSIE 6.0" ) >= 0;
	IE7 = dav.indexOf( "MSIE 7.0" ) >= 0;
	if ( IE ) {
		width = document.body.offsetWidth;
		height = document.body.offsetHeight;
		document.getElementById( 'bloco' ).style.width = ( width * 0.80 ) + 'px';
	}
	this._closeWindows = false;
	
	function tratarAdesao( csTratarAdesao ){
	
		if( csTratarAdesao == 4 ){
			if( !confirm( "Deseja realmente aderir ao Programa Pró-Letramento?" ) ) return;
		}
		window.location.href = window.location.href + '&csTratarAdesao=' + csTratarAdesao;
	}
--></script>

	<?php

	/****************************************************************************************
	 *									ÁRVORE DE ESCOLA ATIVA								*
	 ****************************************************************************************/

	if( $obInstrumentoUnidade->inusituacaoadesao == CTE_ADESAO_ESCOLA_ATIVA_ACEITA && ( $csFiltroArvore == CTE_FILTRO_ARVORE_ESCOLA_ATIVA || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ){

		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

		$sql = "select distinct d.dimid, d.dimcod, d.dimdsc, i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid,
					length(p.ptodemandaestadual) as demandaestadual, length(p.ptodemandamunicipal) as demandamunicipal, 
					c.ctrpontuacao, ai.aciid, ai.acidsc, sa.sbaid, sa.sbadsc, a.ardid, a.ardcod, a.arddsc, a.dimid,
					sa.sbaordem, sa.sbaporescola, sa.frmid,  to_char(sa.sbadata, 'dd/mm/YYYY') as sbadata,
					spt.ssuid, ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus,
					usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
			from cte.dimensao d
				INNER JOIN cte.areadimensao a on a.dimid = d.dimid 
				INNER JOIN cte.indicador i on a.ardid = i.ardid 
				INNER JOIN cte.pontuacao p on p.indid = i.indid  
				INNER JOIN cte.criterio c on c.crtid = p.crtid
				INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador sa on sa.aciid = ai.aciid
		 	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid
			    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
			    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
			    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
				LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
			where d.dimstatus = 'A' 
			and p.inuid = ". $_SESSION["inuid"] ." 
			and p.ptostatus = 'A'
			and d.itrid = $itrid
			and ai.acilocalizador = '$acilocalizador'
			and sa.prgid = ".CTE_PROGRAMA_ESCOLA_ATIVA."
			and ( 
				sbasituacaoarvore = 2 or
				sbasituacaoarvore = 3 
			)
			order by dimcod, ardcod, indcod, aciid, sa.sbaordem, sa.sbadsc";			
			
		$arResultado = $db->carregar( $sql );
		$arResultado = $arResultado ? $arResultado : array();

		$arDados = array();
		foreach( $arResultado as $resultado ){
			$arDados[$resultado["dimid"]."_".$resultado["dimcod"].". ".$resultado["dimdsc"]]
			[$resultado["ardid"]."_".$resultado["ardcod"].". ".$resultado["arddsc"]]
			[$resultado["indid"]."_".$resultado["indcod"].". ".$resultado["inddsc"]]
			[$resultado["aciid"]."_".$resultado["acidsc"]][] = $resultado;
		}

		?>
<script type="text/javascript">
	
		<?php if( count( $arDados ) || true ){ ?>
	
			arvoreA = new dTree( "arvoreA" );
			arvoreA.config.folderLinks = true;
			arvoreA.config.useIcons = true;
			arvoreA.config.useCookies = true;
		
			arvoreA.add( 0, -1, '<?= $instrumento ?>' );
			
			arvoreA.add( 1, 0, '<b>Dados do Programa Escola Ativa</b>', 'javascript:cadastrarDadosReferenciaEscolaAtiva();' );
			//arvoreA.add( 2, 0, '<b>Questões Pontuais</b>', '', '', '', '../includes/dtree/img/question.gif', '../includes/dtree/img/question.gif' );
			arvoreA.add( 'qa', 2, '<b>Escola Ativa</b>' );
			
			<?php if ( cte_podeVerIndicador( $_SESSION["inuid"] ) || true ){
			
				$obPergunta = new Pergunta();
				$coPergunta = $obPergunta->recuperarTodos( "prgid, dimid, prgcod, prgdsc", array( "dimid is null" ), "prgcod" );
				
				foreach( $coPergunta as $obPergunta ){
					$stQuestao = $obPergunta["prgcod"].". ".$obPergunta["prgdsc"]; ?>
					
					arvoreA.add( 'qp_<?php echo $obPergunta["prgid"] ?> ?>', 'qa', '<?php echo delimitador( $stQuestao ) ?>', 'javascript: responder( \'<?php echo $obPergunta["dimid"] ?>\', <?php echo $obPergunta["prgid"] ?> ); ' );
				<?php } ?>
			
				arvoreA.add( 3, 0, '<b>Indicadores Qualitativos</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );
				
				
				<?php // Dimensão
				foreach( $arDados as $stDimensao => $arArea ){
					
					$dimcod = substr( $stDimensao, strpos( $stDimensao, "_" ) +1, 1 );
			
					// Área
					foreach( $arArea as $stArea => $arIndicador ){ 
						
						$ardcod = substr( $stArea, strpos( $stArea, "_" ) +1, 1 );
						
						// Indicador
						foreach( $arIndicador as $stIndicador => $arAcao ){ 
							
							$arSubacao = current( $arAcao );
							
							$indid  = substr( $stIndicador, 0, strpos( $stIndicador, "_" ) );
							$inddsc = delimitador( substr( $stIndicador, strpos( $stIndicador, "_" ) +1 ) );			
							
							$icone = '';
							$cor = '';
							$mensagens = array();
							if ( $arSubacao[0]["ptoid"] ) {
								$icone = '../imagens/check_p.gif';
								$cor = '#959595';
							}			
			
							
							if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandaestadual'] > 0 ) {
										array_push( $mensagens, "Há demanda estadual." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandamunicipal'] > 0 ) {
										array_push( $mensagens, "Há demanda municipal." );
									}
									if ( count( $mensagens ) > 0 ) {
										array_push( $mensagens, "É necessário descrever um plano de ação." );
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}	
							elseif ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa. É necessário descrever um plano de ação." );
									}
									if ( $arSubacao[0]['aciid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(0,3,4) ) ) {
										array_push( $mensagens, "A pontuação não permite que exista ação para este indicador." );
									}
									if ( count( $mensagens ) > 0 ) {
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}
							
							//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTE ÍTEM.				
							$title = "Última Atualização feita por ".str_replace( "'", "\'", $arSubacao[0]['usunome'] )." em ".$arSubacao[0]['data'];				
							if(strlen(trim($arSubacao[0]['usunome']))<2)
								$title = "Nenhuma atualização realizada.";	
							
							array_push( $mensagens, $title) ;
							$texto = '<span style="color: '. $cor .';" title="'. implode(' - ', $mensagens ) .'">'. $dimcod.".".$ardcod.".".$inddsc .'</span>';				
							
							?>
			
							arvoreA.add( 'i_<?= $indid ?>', '3', '<?= $texto ?>', 'javascript: avaliar(<?= $indid ?>);', '', '', '<?= $icone ?>', '<?= $icone ?>' );
							
							<?php foreach( $arAcao as $stAcao => $arSubacao ){
								
								foreach( $arSubacao as $indice => $subacao ){
									
									$subacao['sbadsc'] = delimitador( $subacao['sbadsc'] );
			
						            if ($subacao['ptostatus'] != 'A') continue;
						            
						            if( cte_subacaoProgramaPreenchida( $subacao["sbaid"] ) ){
						            	if( cte_subacaoProgramaAnalisada( $subacao["sbaid"] ) ){
											$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}	
						            	else{
											$icone = '<img src="../imagens/check_p_amarelo.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}
						            }
						            else{						            
										$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            }
	
					                $prefixo = '';
					                
					                $sql="select sptano from cte.subacaoparecertecnico where sbaid = '".$subacao['sbaid']."' and ssuid is not null";
									$anosParecerDados = $db->carregar($sql);
									$statusSub = $anosParecerDados;
					                if( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) &&  cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_MUNICIPAL ) ) && $statusSub == NULL ){
										//$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação"/> ';
					                }
					                elseif( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) && cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ){
					                    //$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação 2"/> ';
					                }
					                
					               	$iconeConvenio = "";
					                if( $stAnos = cte_recuperarAnosSubacaoConveniada( $subacao["sbaid"] ) ){
					                	$iconeConvenio = '<img align="absmiddle" src="/imagens/conveniada.png" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados: '.$stAnos.'."/>';
						                
					                	if( cte_possuiAditivo( $subacao["sbaid"] ) ){
						                	$iconeConvenio .= '<img align="absmiddle" src="/imagens/aditivo.png" alt="Sub-ação possui Aditivo" title="Sub-ação possui Aditivo"/>';
					                	}
					                }
										
					
					                $ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
					                $texto = $icone . $iconeConvenio . $ordem . '<a onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['sbadsc']).'</a>'; ?>
						            arvoreA.add( 'sa_<?= $subacao['sbaid'] ?>', 'i_<?= $indid ?>', '<?= $prefixo ?><?= $texto  ?>', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
								
					                <?php
								} // Foreach Subação
							} // Foreach Ação
						}  // Foreach Indicador
					 } // Foreach Área
				} // Foreach Dimensão
			} ?>
			//elemento = document.getElementById( '_arvoreA' );
			//elemento.innerHTML = arvoreA;
			//arvoreA.openAll()
		
		<?php }
		else{ ?>
				document.getElementById( 'bloco2' ).style.display = "none";
		<?php } ?>		
			
	</script>
		<?php } ?>



		<?php

		/****************************************************************************************
		 *								ÁRVORE DO PRÓ-LETRAMENTO								*
		 ****************************************************************************************/

		if( $obInstrumentoUnidade->usucpfproletramento && ( $csFiltroArvore == CTE_FILTRO_ARVORE_PRO_LETRAMENTO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ){

			$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

			$sql = "select distinct d.dimid, d.dimcod, d.dimdsc, i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid,
					length(p.ptodemandaestadual) as demandaestadual, length(p.ptodemandamunicipal) as demandamunicipal, 
					c.ctrpontuacao, ai.aciid, ai.acidsc, sa.sbaid, sa.sbadsc, a.ardid, a.ardcod, a.arddsc, a.dimid,
					sa.sbaordem, sa.sbaporescola, sa.frmid,  to_char(sa.sbadata, 'dd/mm/YYYY') as sbadata,
					spt.ssuid, ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus,
					usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
			from cte.dimensao d
				INNER JOIN cte.areadimensao a on a.dimid = d.dimid 
				INNER JOIN cte.indicador i on a.ardid = i.ardid 
				INNER JOIN cte.pontuacao p on p.indid = i.indid  
				INNER JOIN cte.criterio c on c.crtid = p.crtid
				INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador sa on sa.aciid = ai.aciid
		 	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid
			    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
			    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
			    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
				LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
			where d.dimstatus = 'A' 
			and p.inuid = ". $_SESSION["inuid"] ." 
			and p.ptostatus = 'A'
			and d.itrid = $itrid
			and ai.acilocalizador = '$acilocalizador'
			and ( 
				sbasituacaoarvore = 4 or
				sbasituacaoarvore = 5
			)
			order by dimcod, ardcod, indcod, aciid, sa.sbaordem, sa.sbadsc";			
				
			$arResultado = $db->carregar( $sql );
			$arResultado = $arResultado ? $arResultado : array();
				

			$arDados = array();
			foreach( $arResultado as $resultado ){
				$arDados[$resultado["dimid"]."_".$resultado["dimcod"].". ".$resultado["dimdsc"]]
				[$resultado["ardid"]."_".$resultado["ardcod"].". ".$resultado["arddsc"]]
				[$resultado["indid"]."_".$resultado["indcod"].". ".$resultado["inddsc"]]
				[$resultado["aciid"]."_".$resultado["acidsc"]][] = $resultado;
			}

			?>
<script type="text/javascript">
	
		<?php if( count( $arDados ) || true ){ ?>
	
			arvoreP = new dTree( "arvoreP" );
			arvoreP.config.folderLinks = true;
			arvoreP.config.useIcons = true;
			arvoreP.config.useCookies = true;
		
			arvoreP.add( 0, -1, '<?= $instrumento ?>' );
			
			
			<?php if ( cte_podeVerIndicador( $_SESSION["inuid"] ) || true ){ ?>
			
				arvoreP.add( 3, 0, '<b>Indicadores Qualitativos</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );
				
				<?php // Dimensão
				foreach( $arDados as $stDimensao => $arArea ){
					
					$dimcod = substr( $stDimensao, strpos( $stDimensao, "_" ) +1, 1 );

					// Área
					foreach( $arArea as $stArea => $arIndicador ){ 

						$ardcod = substr( $stArea, strpos( $stArea, "_" ) +1, 1 );
						
						// Indicador
						foreach( $arIndicador as $stIndicador => $arAcao ){ 
							
							$arSubacao = current( $arAcao );
							
							$indid  = substr( $stIndicador, 0, strpos( $stIndicador, "_" ) );
							$inddsc = delimitador( substr( $stIndicador, strpos( $stIndicador, "_" ) +1 ) );			
							
							$icone = '';
							$cor = '';
							$mensagens = array();
							if ( $arSubacao[0]["ptoid"] ) {
								$icone = '../imagens/check_p.gif';
								$cor = '#959595';
							}			
			
							
							if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandaestadual'] > 0 ) {
										array_push( $mensagens, "Há demanda estadual." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandamunicipal'] > 0 ) {
										array_push( $mensagens, "Há demanda municipal." );
									}
									if ( count( $mensagens ) > 0 ) {
										array_push( $mensagens, "É necessário descrever um plano de ação." );
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}	
							elseif ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa. É necessário descrever um plano de ação." );
									}
									if ( $arSubacao[0]['aciid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(0,3,4) ) ) {
										array_push( $mensagens, "A pontuação não permite que exista ação para este indicador." );
									}
									if ( count( $mensagens ) > 0 ) {
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}
							
							//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTE ÍTEM.				
							$title = "Última Atualização feita por ".str_replace( "'", "\'", $arSubacao[0]['usunome'] )." em ".$arSubacao[0]['data'];				
							if(strlen(trim($arSubacao[0]['usunome']))<2)
								$title = "Nenhuma atualização realizada.";	
							
							array_push( $mensagens, $title) ;
							
							$texto = '<span style="color: '. $cor .';" title="'. implode(' - ', $mensagens ) .'">'. $dimcod.".".$ardcod.".".$inddsc .'</span>';				
							
							?>
			
							arvoreP.add( 'i_<?= $indid ?>', '3', '<?= $texto ?>', 'javascript: avaliar(<?= $indid ?>);', '', '', '<?= $icone ?>', '<?= $icone ?>' );
							
							<?php //Ação
							foreach( $arAcao as $stAcao => $arSubacao ){

								// Subação
								foreach( $arSubacao as $indice => $subacao ){
									
									$subacao['sbadsc'] = delimitador( $subacao['sbadsc'] );
			
						            if ($subacao['ptostatus'] != 'A') continue;

									if( cte_subacaoProgramaPreenchida( $subacao["sbaid"] ) ){
						            	if( cte_subacaoProgramaAnalisada( $subacao["sbaid"] ) ){
											$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}	
						            	else{
											$icone = '<img src="../imagens/check_p_amarelo.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}
						            }
						            else{						            
										$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            }
						
					                $prefixo = '';
					                
					                $sql="select sptano from cte.subacaoparecertecnico where sbaid = '".$subacao['sbaid']."' and ssuid is not null";
									$anosParecerDados = $db->carregar($sql);
									$statusSub = $anosParecerDados;
					                if( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) &&  cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_MUNICIPAL ) ) && $statusSub == NULL ){
										//$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação"/> ';
					                }
					                elseif( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) && cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ){
					                    //$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação 2"/> ';
					                }
					                
					               	$iconeConvenio = "";
					                if( $stAnos = cte_recuperarAnosSubacaoConveniada( $subacao["sbaid"] ) ){
					                	$iconeConvenio = '<img align="absmiddle" src="/imagens/conveniada.png" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados: '.$stAnos.'."/>';
						                
					                	if( cte_possuiAditivo( $subacao["sbaid"] ) ){
						                	$iconeConvenio .= '<img align="absmiddle" src="/imagens/aditivo.png" alt="Sub-ação possui Aditivo" title="Sub-ação possui Aditivo"/>';
					                	}
					                }
										
					
					                $ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
					                $texto = $icone . $iconeConvenio . $ordem . '<a onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['sbadsc']).'</a>'; ?>
						            arvoreP.add( 'sa_<?= $subacao['sbaid'] ?>', 'i_<?= $indid ?>', '<?= $prefixo ?><?= $texto  ?>', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
								
					                <?php
								} // Foreach Subação
							} // Foreach Ação
						}  // Foreach Indicador
					 } // Foreach Área
				} // Foreach Dimensão
			} ?>
			elemento = document.getElementById( '_arvoreP' );
			elemento.innerHTML = arvoreP;
			arvoreP.openAll();
		
		<?php }
		else{ ?>
				document.getElementById( 'bloco2' ).style.display = "none";
		<?php } ?>		
			
	</script>
		<?php } ?>

		<?php

		/****************************************************************************************
		 *								ÁRVORE DO PRÓ-FUNCIONÁRIO								*
		 ****************************************************************************************/

		if( $obInstrumentoUnidade->usucpfprofuncionario && ( $csFiltroArvore == CTE_FILTRO_ARVORE_PRO_FUNCIONARIO || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ){

			$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

			$sql = "select distinct d.dimid, d.dimcod, d.dimdsc, i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid,
					length(p.ptodemandaestadual) as demandaestadual, length(p.ptodemandamunicipal) as demandamunicipal, 
					c.ctrpontuacao, ai.aciid, ai.acidsc, sa.sbaid, sa.sbadsc, a.ardid, a.ardcod, a.arddsc, a.dimid,
					sa.sbaordem, sa.sbaporescola, sa.frmid,  to_char(sa.sbadata, 'dd/mm/YYYY') as sbadata,
					spt.ssuid, ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus,
					usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
			from cte.dimensao d
				INNER JOIN cte.areadimensao a on a.dimid = d.dimid 
				INNER JOIN cte.indicador i on a.ardid = i.ardid 
				INNER JOIN cte.pontuacao p on p.indid = i.indid  
				INNER JOIN cte.criterio c on c.crtid = p.crtid
				INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador sa on sa.aciid = ai.aciid
		 	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid
			    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
			    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
			    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
				LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
			where d.dimstatus = 'A' 
			and p.inuid = ". $_SESSION["inuid"] ." 
			and p.ptostatus = 'A'
			and d.itrid = $itrid
			and ai.acilocalizador = '$acilocalizador'
			and ( 
				sbasituacaoarvore = 6 or
				sbasituacaoarvore = 7
			)
			order by dimcod, ardcod, indcod, aciid, sa.sbaordem, sa.sbadsc";			
			//			ver($sql);
			$arResultado = $db->carregar( $sql );
			$arResultado = $arResultado ? $arResultado : array();
				

			$arDados = array();
			foreach( $arResultado as $resultado ){
				$arDados[$resultado["dimid"]."_".$resultado["dimcod"].". ".$resultado["dimdsc"]]
				[$resultado["ardid"]."_".$resultado["ardcod"].". ".$resultado["arddsc"]]
				[$resultado["indid"]."_".$resultado["indcod"].". ".$resultado["inddsc"]]
				[$resultado["aciid"]."_".$resultado["acidsc"]][] = $resultado;
			}

			?>
<script type="text/javascript">
	
		<?php if( count( $arDados ) || true ){ ?>
	
			arvoreP = new dTree( "arvoreP" );
			arvoreP.config.folderLinks = true;
			arvoreP.config.useIcons = true;
			arvoreP.config.useCookies = true;
		
			arvoreP.add( 0, -1, '<?= $instrumento ?>' );
			
			
			<?php if ( cte_podeVerIndicador( $_SESSION["inuid"] ) || true ){ ?>
			
				arvoreP.add( 3, 0, '<b>Indicadores Qualitativos</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );
				
				<?php // Dimensão
				foreach( $arDados as $stDimensao => $arArea ){
					
					$dimcod = substr( $stDimensao, strpos( $stDimensao, "_" ) +1, 1 );

					// Área
					foreach( $arArea as $stArea => $arIndicador ){ 

						$ardcod = substr( $stArea, strpos( $stArea, "_" ) +1, 1 );
						
						// Indicador
						foreach( $arIndicador as $stIndicador => $arAcao ){ 
							
							$arSubacao = current( $arAcao );
							
							$indid  = substr( $stIndicador, 0, strpos( $stIndicador, "_" ) );
							$inddsc = delimitador( substr( $stIndicador, strpos( $stIndicador, "_" ) +1 ) );			
							
							$icone = '';
							$cor = '';
							$mensagens = array();
							if ( $arSubacao[0]["ptoid"] ) {
								$icone = '../imagens/check_p.gif';
								$cor = '#959595';
							}			
			
							
							if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandaestadual'] > 0 ) {
										array_push( $mensagens, "Há demanda estadual." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandamunicipal'] > 0 ) {
										array_push( $mensagens, "Há demanda municipal." );
									}
									if ( count( $mensagens ) > 0 ) {
										array_push( $mensagens, "É necessário descrever um plano de ação." );
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}	
							elseif ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa. É necessário descrever um plano de ação." );
									}
									if ( $arSubacao[0]['aciid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(0,3,4) ) ) {
										array_push( $mensagens, "A pontuação não permite que exista ação para este indicador." );
									}
									if ( count( $mensagens ) > 0 ) {
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}
							
							//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTE ÍTEM.				
							$title = "Última Atualização feita por ".str_replace( "'", "\'", $arSubacao[0]['usunome'] )." em ".$arSubacao[0]['data'];				
							if(strlen(trim($arSubacao[0]['usunome']))<2)
								$title = "Nenhuma atualização realizada.";	
							
							array_push( $mensagens, $title) ;
							
							$texto = '<span style="color: '. $cor .';" title="'. implode(' - ', $mensagens ) .'">'. $dimcod.".".$ardcod.".".$inddsc .'</span>';				
							
							?>
			
							arvoreP.add( 'i_<?= $indid ?>', '3', '<?= $texto ?>', 'javascript: avaliar(<?= $indid ?>);', '', '', '<?= $icone ?>', '<?= $icone ?>' );
							
							<?php //Ação
							foreach( $arAcao as $stAcao => $arSubacao ){

								// Subação
								foreach( $arSubacao as $indice => $subacao ){
									
									$subacao['sbadsc'] = delimitador( $subacao['sbadsc'] );
			
						            if ($subacao['ptostatus'] != 'A') continue;

									if( cte_subacaoProgramaPreenchida( $subacao["sbaid"] ) ){
						            	if( cte_subacaoProgramaAnalisada( $subacao["sbaid"] ) ){
											$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}	
						            	else{
											$icone = '<img src="../imagens/check_p_amarelo.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}
						            }
						            else{						            
										$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            }
						
					                $prefixo = '';
					                
					                $sql="select sptano from cte.subacaoparecertecnico where sbaid = '".$subacao['sbaid']."' and ssuid is not null";
									$anosParecerDados = $db->carregar($sql);
									$statusSub = $anosParecerDados;
					                if( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) &&  cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_MUNICIPAL ) ) && $statusSub == NULL ){
										//$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação"/> ';
					                }
					                elseif( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) && cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ){
					                    //$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação 2"/> ';
					                }
					                
					               	$iconeConvenio = "";
					                if( $stAnos = cte_recuperarAnosSubacaoConveniada( $subacao["sbaid"] ) ){
					                	$iconeConvenio = '<img align="absmiddle" src="/imagens/conveniada.png" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados: '.$stAnos.'."/>';
						                
					                	if( cte_possuiAditivo( $subacao["sbaid"] ) ){
						                	$iconeConvenio .= '<img align="absmiddle" src="/imagens/aditivo.png" alt="Sub-ação possui Aditivo" title="Sub-ação possui Aditivo"/>';
					                	}
					                }
										
					
					                $ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
					                $texto = $icone . $iconeConvenio . $ordem . '<a onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['sbadsc']).'</a>'; ?>
						            arvoreP.add( 'sa_<?= $subacao['sbaid'] ?>', 'i_<?= $indid ?>', '<?= $prefixo ?><?= $texto  ?>', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
								
					                <?php
								} // Foreach Subação
							} // Foreach Ação
						}  // Foreach Indicador
					 } // Foreach Área
				} // Foreach Dimensão
			} ?>
			elemento = document.getElementById( '_arvoreP' );
			elemento.innerHTML = arvoreP;
			arvoreP.openAll();
		
		<?php }
		else{ ?>
				document.getElementById( 'bloco2' ).style.display = "none";
		<?php } ?>		
			
	</script>
		<?php } ?>

		<?php
		/****************************************************************************************
		 *								ÁRVORE DO FORMAÇÃO PELA ESCOLA								*
		 ****************************************************************************************/

		if( $obInstrumentoUnidade->usucpfformacaoescola && ( $csFiltroArvore == CTE_FILTRO_ARVORE_FORMACAO_ESCOLA || $csFiltroArvore == CTE_FILTRO_ARVORE_TODAS ) ){

			$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

			$sql = "select distinct d.dimid, d.dimcod, d.dimdsc, i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid,
					length(p.ptodemandaestadual) as demandaestadual, length(p.ptodemandamunicipal) as demandamunicipal, 
					c.ctrpontuacao, ai.aciid, ai.acidsc, sa.sbaid, sa.sbadsc, a.ardid, a.ardcod, a.arddsc, a.dimid,
					sa.sbaordem, sa.sbaporescola, sa.frmid,  to_char(sa.sbadata, 'dd/mm/YYYY') as sbadata,
					spt.ssuid, ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus,
					usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
			from cte.dimensao d
				INNER JOIN cte.areadimensao a on a.dimid = d.dimid 
				INNER JOIN cte.indicador i on a.ardid = i.ardid 
				INNER JOIN cte.pontuacao p on p.indid = i.indid  
				LEFT JOIN cte.criterio c on c.crtid = p.crtid				
				INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador sa on sa.aciid = ai.aciid
		 	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid
			    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
			    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
			    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
				LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
			where d.dimstatus = 'A' 
			and p.inuid = ". $_SESSION["inuid"] ." 
			and p.ptostatus = 'A'
			and d.itrid = $itrid
			and ai.acilocalizador = '$acilocalizador'
			and ( 
				sbasituacaoarvore = 8 or
				sbasituacaoarvore = 9
			)
			order by dimcod, ardcod, indcod, aciid, sa.sbaordem, sa.sbadsc";			
			//			ver($sql);
			$arResultado = $db->carregar( $sql );
			$arResultado = $arResultado ? $arResultado : array();
				

			$arDados = array();
			foreach( $arResultado as $resultado ){
				$arDados[$resultado["dimid"]."_".$resultado["dimcod"].". ".$resultado["dimdsc"]]
				[$resultado["ardid"]."_".$resultado["ardcod"].". ".$resultado["arddsc"]]
				[$resultado["indid"]."_".$resultado["indcod"].". ".$resultado["inddsc"]]
				[$resultado["aciid"]."_".$resultado["acidsc"]][] = $resultado;
			}

			?>
<script type="text/javascript">
	
		<?php if( count( $arDados ) || true ){ ?>
	
			arvoreFE = new dTree( "arvoreFE" );
			arvoreFE.config.folderLinks = true;
			arvoreFE.config.useIcons = true;
			arvoreFE.config.useCookies = true;
		
			arvoreFE.add( 0, -1, '<?= $instrumento ?>' );
			
			
			<?php if ( cte_podeVerIndicador( $_SESSION["inuid"] ) || true ){ ?>
			
				arvoreFE.add( 3, 0, '<b>Indicadores Qualitativos</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );
				
				<?php // Dimensão
				foreach( $arDados as $stDimensao => $arArea ){
					
					$dimcod = substr( $stDimensao, strpos( $stDimensao, "_" ) +1, 1 );

					// Área
					foreach( $arArea as $stArea => $arIndicador ){ 

						$ardcod = substr( $stArea, strpos( $stArea, "_" ) +1, 1 );
						
						// Indicador
						foreach( $arIndicador as $stIndicador => $arAcao ){ 
							
							$arSubacao = current( $arAcao );
							
							$indid  = substr( $stIndicador, 0, strpos( $stIndicador, "_" ) );
							$inddsc = delimitador( substr( $stIndicador, strpos( $stIndicador, "_" ) +1 ) );			
							
							$icone = '';
							$cor = '';
							$mensagens = array();
							if ( $arSubacao[0]["ptoid"] ) {
								$icone = '../imagens/check_p.gif';
								$cor = '#959595';
							}			
			
							
							if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandaestadual'] > 0 ) {
										array_push( $mensagens, "Há demanda estadual." );
									}
									if ( !$arSubacao[0]['sbaid'] && $arSubacao[0]['demandamunicipal'] > 0 ) {
										array_push( $mensagens, "Há demanda municipal." );
									}
									if ( count( $mensagens ) > 0 ) {
										array_push( $mensagens, "É necessário descrever um plano de ação." );
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}	
							elseif ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
	//							if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
									if ( !$arSubacao[0]['sbaid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(1,2) ) ) {
										array_push( $mensagens, "A pontuação está baixa. É necessário descrever um plano de ação." );
									}
									if ( $arSubacao[0]['aciid'] && in_array( $arSubacao[0]['ctrpontuacao'], array(0,3,4) ) ) {
										array_push( $mensagens, "A pontuação não permite que exista ação para este indicador." );
									}
									if ( count( $mensagens ) > 0 ) {
										$icone = '../imagens/atencao.png';
										$cor = '#202020';
									}
	//							}
							}
							
							//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTE ÍTEM.				
							$title = "Última Atualização feita por ".str_replace( "'", "\'", $arSubacao[0]['usunome'] )." em ".$arSubacao[0]['data'];				
							if(strlen(trim($arSubacao[0]['usunome']))<2)
								$title = "Nenhuma atualização realizada.";	
							
							array_push( $mensagens, $title) ;
							
							$texto = '<span style="color: '. $cor .';" title="'. implode(' - ', $mensagens ) .'">'. $dimcod.".".$ardcod.".".$inddsc .'</span>';				
							
							?>
			
							arvoreFE.add( 'i_<?= $indid ?>', '3', '<?= $texto ?>', 'javascript: avaliar(<?= $indid ?>);', '', '', '<?= $icone ?>', '<?= $icone ?>' );
							
							<?php //Ação
							foreach( $arAcao as $stAcao => $arSubacao ){

								// Subação
								foreach( $arSubacao as $indice => $subacao ){
									
									$subacao['sbadsc'] = delimitador( $subacao['sbadsc'] );
			
						            if ($subacao['ptostatus'] != 'A') continue;

									if( cte_subacaoProgramaPreenchida( $subacao["sbaid"] ) ){
						            	if( cte_subacaoProgramaAnalisada( $subacao["sbaid"] ) ){
											$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}	
						            	else{
											$icone = '<img src="../imagens/check_p_amarelo.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            	}
						            }
						            else{						            
										$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';
						            }
						
					                $prefixo = '';
					                
					                $sql="select sptano from cte.subacaoparecertecnico where sbaid = '".$subacao['sbaid']."' and ssuid is not null";
									$anosParecerDados = $db->carregar($sql);
									$statusSub = $anosParecerDados;
					                if( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) &&  cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_MUNICIPAL ) ) && $statusSub == NULL ){
										//$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação"/> ';
					                }
					                elseif( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ) && cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ){
					                    //$prefixo .= '<img align="absmiddle" onclick="javascript:excluirSubacao('. $subacao['sbaid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir subação 2"/> ';
					                }
					                
					               	$iconeConvenio = "";
					                if( $stAnos = cte_recuperarAnosSubacaoConveniada( $subacao["sbaid"] ) ){
					                	$iconeConvenio = '<img align="absmiddle" src="/imagens/conveniada.png" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados: '.$stAnos.'."/>';
						                
					                	if( cte_possuiAditivo( $subacao["sbaid"] ) ){
						                	$iconeConvenio .= '<img align="absmiddle" src="/imagens/aditivo.png" alt="Sub-ação possui Aditivo" title="Sub-ação possui Aditivo"/>';
					                	}
					                }
										
					
					                $ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";
					                $texto = $icone . $iconeConvenio . $ordem . '<a onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",$subacao['sbadsc']).'</a>'; ?>
						            arvoreFE.add( 'sa_<?= $subacao['sbaid'] ?>', 'i_<?= $indid ?>', '<?= $prefixo ?><?= $texto  ?>', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );
								
					                <?php
								} // Foreach Subação
							} // Foreach Ação
						}  // Foreach Indicador
					 } // Foreach Área
				} // Foreach Dimensão
			} ?>
			elemento = document.getElementById( '_arvoreFE' );
			elemento.innerHTML = arvoreFE;
			arvoreFE.openAll();
		
		<?php }
		else{ ?>
				document.getElementById( 'bloco2' ).style.display = "none";
		<?php } ?>		
			
	</script>
		<?php } ?>
		
