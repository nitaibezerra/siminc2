<?php
cte_verificaSessao();
include_once( "par_subacao_funcoes.inc" );

$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$editavel           = true;
ob_clean();

define('CTE_NOVA_SUBACAO', trim($_REQUEST['sbaid']) == '');

if (!$_REQUEST['anoExercicio'])
    $_REQUEST['anoExercicio'] = $_REQUEST['parsubacaoanocrt'] ? $_REQUEST['parsubacaoanocrt'] : date('Y');
     
$cosano = $_REQUEST['cosano'] ? $_REQUEST['cosano'] : $_REQUEST['anoExercicio'];

if($_REQUEST['sbaidpai'] != 0){
	$_REQUEST['parsubacaoanocrt']= $_REQUEST['anoconvenio'];
}

$anoAtualTrava = 0;
$anoParecer2007 = 0;
$anoParecer2008 = 0;
$anoParecer2009 = 0;
$anoParecer2010 = 0;
$anoParecer2011 = 0;
$anoConvenio = 0000;

if (!CTE_NOVA_SUBACAO) {
	$sql="select sbaid from cte.subacaoindicador WHERE sbaidpai = ". $_REQUEST['sbaid'];
	$subacoesFilhos = $db->carregarColuna($sql);
	$strSubacoesFilhos = "";
	if(is_array($subacoesFilhos)){
		$strSubacoesFilhos .= implode(",", $subacoesFilhos);
	}
	
	$sql = "select ss.ssudescricao from cte.statussubacao ss
																		inner join cte.subacaoparecertecnico spt on spt.ssuid = ss.ssuid
																		where sbaid in ( " . $_REQUEST['sbaid']  . $strSubacoesFilhos. ")";
																//dbg($sql,1);	
	

	/**************** Verifica se a subção foi conveniada com o FNDE Se foi bloqueia o ano que foi conveniada ***************/
	$subConveniada 	= subacaoConveniada();
	$anoConvenio 	= ($subConveniada == NULL) ? 0000 : $subConveniada;
	/**************** FIM Verifica se a subção foi conveniada com o FNDE ***************/
	
	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  TRAVA ABA ANO SE O ANO FOR MENOR QUE O ANO CORRENTE.
	* 	
	* 	CHAMADA JAVASCRIPT	: function travaAnosAnteriores(cosano); (no arquivo: www/includes/par_subacao.js)
	* 	DESCRIÇÃO:
	*	Se estiver em uma aba onde o ano e menor que o ano atual, e a forma de execução da subação
	* 	for assistencia financeira, transferencia voluntaria ou assistencia tecnica com complementação financeira,
	*   a aba dos anos anteriores são travadas.
	*******************************/
	 
	$sql = "SELECT sbaid FROM cte.subacaoindicador si  WHERE  si.sbaid = ".$_REQUEST['sbaid']." AND si.frmid in(11,3,2)";
	$frmFinanceira = $db->pegaUm($sql);
	if($frmFinanceira){
		$anoAtualTrava = getdate();
		$anoAtualTrava = $anoAtualTrava["year"]; 	
	}

	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  TRAVA ABA ANO EM ELABORAÇÃO DO PAR E VALIDAÇÃO DO MUNICÍPIO.
	* 
	* 	DESCRIÇÃO:
	*	Se estiver em Elaboração do PAR ou em  Validação do Município e se o usuário estiver em alguma aba onde no ano tenha 
	*   o status da subação diferente de vazio Bloqueia a aba do ano da subação.
	*******************************/
	$sql =		"SELECT sptano 
				FROM cte.subacaoparecertecnico 
				WHERE sbaid = '".$_REQUEST['sbaid']."' 
				AND ssuid is not null";
	$anosParecerDados 	= $db->carregar($sql);
	$docid 				= cte_pegarDocid( $_SESSION['inuid'] );
	$documento 			= wf_pegarEstadoAtual( $docid );
	$total 				= count($anosParecerDados); // Anos que tem o parecer Técnico dado.

	for($i = 0; $i < $total; $i++){
		switch($anosParecerDados[$i]["sptano"]) {
			case '2007':
				$anoParecer2007 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2008':
				$anoParecer2008 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2009':
				$anoParecer2009 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2010':
				$anoParecer2010 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2011':
				$anoParecer2011 = $anosParecerDados[$i]["sptano"];
				break; 
		}
	}
	/****** FIM TRAVA ABA ANO EM ELABORAÇÃO DO PAR E VALIDAÇÃO DO MUNICÍPIO. ******/
	
	$parecer = existeParecerPorSubacao( $_REQUEST['sbaid'] );
    if($estadoDocumento['esdid'] == CTE_ESTADO_PAR){
    	$editavel = !$parecer;
    }
}

define( 'CTE_PRM_EDITAR_SUBACAO' , cte_podeEditarParecer( $_SESSION['inuid'] ) );
define( 'CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);
define( 'CTE_PRM_EMITIR_PARECER' , CTE_PRM_EDITAR_SUBACAO && ($db->testa_superuser() ||
                                                            cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                   CTE_PERFIL_EQUIPE_TECNICA,
                                                                                   CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN))));
                                                                                   
define('CTE_PRM_VISUALIZAR_PARECER', CTE_PRM_EMITIR_PARECER || cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_EQUIPE_TECNICA,
                                                                                      CTE_PERFIL_ALTA_GESTAO,
                                                                                      CTE_PERFIL_EQUIPE_LOCAL,
																					  CTE_PERFIL_EQUIPE_LOCAL_APROVACAO)));
header('content-type: text/html; charset=iso-8859-1;');

/*********************************************************************************************
*
*  	REGRA DE NEGÓCIO DO PAR:  ADITIVOS.
*
**********************************************************************************************/

/************************************************************	
* 	CARREGAR ADITIVOS
* 	(FUNÇÃO AJAX)
* 	REQUEST				: carregaAditivos
*   CHAMADA JAVASCRIPT	: function carregarAditivos(cosano)
* 	DESCRIÇÃO: 
*   Carrega lista de Aditivos da subação por aba.
*************************************************************/
if($_REQUEST['req'] == 'carregaAditivos'){//lista de Aditivos
	$sql=	"SELECT si.sbaid, to_char(si.sbadata, 'dd/mm/yyyy') AS data, si.sbaanoaditivo AS anoaditivo , cvrnumprocesso
			FROM cte.subacaoindicador si
			LEFT JOIN cte.subacaoconvenio sc on sc.sbaid = si.sbaid
			LEFT JOIN cte.convenio c on sc.cnvid = c.cnvid
			LEFT JOIN cte.convenioretorno cr on cr.cnvid = c.cnvid 
			WHERE sbaidpai = ".$_REQUEST['sbaid']."  
			AND sbaanoaditivo = ".$cosano;

	$aditivos = $db->carregar($sql);
	if($anoConvenio != 0000){//Se existe aditivo 0000 quer dizer que não tem aditivo.
		if(is_array($aditivos)){
			$cont = 1;
			$listaAditivos = "";
			foreach($aditivos as $aditivo){		
				$numeroDeProcesso = "";						
				if($aditivo["cvrnumprocesso"]){
					$numeroDeProcesso = ' referente ao número de processo: '.$aditivo["cvrnumprocesso"];
				}
				if($aditivo['sbaid']){
					$listaAditivos .= '
						<div>
				 			<a  style="color:#000000;" onclick="irAditivo('.$aditivo['sbaid'].', '.$aditivo['anoaditivo'].');" title="Aditivo" >Aditivo '.$cont.' criado em '.$aditivo["data"].$numeroDeProcesso.' </a>
						</div>
					';								
					$cont++;
				}else{
					$listaAditivos .= '
						<div>
				 			<a  style="color:#000000;" title="Aditivo" >Não foram encontrados Registros.</a>
						</div>
					';	
				}
				
			}
		}
	}						
	echo $listaAditivos; // Retorna para o F 
	die();
}

/************************************************************	
* 	VOLTA PARA SUAÇÃO PAI
* 	(FUNÇÃO AJAX)
* 	REQUEST				: voltarsubacaooriginal
*   CHAMADA JAVASCRIPT	: function voltarSubacaoPai(sbaidPai)
* 	DESCRIÇÃO: 
*   Volta para subação pai (Estando na subação aditivo)
*************************************************************/
if($_REQUEST['req'] == 'voltarsubacaooriginal'){
	$sbaidOrigianl = $_REQUEST['sbaidpai'];
	$_REQUEST['sbaidpai'] = NULL;
	$_REQUEST['anoconvenio'] = NULL;
	echo $sbaidOrigianl;
	die();
}

/************************************************************************
*		   LISTAGEM, EXCULSÃO DE ITENS-INDICADOR E TOTALIZADORES		*
************************************************************************/

if( array_key_exists('req', $_REQUEST ) ) {

    if (trim($_REQUEST['sbaid']) == '') {
        exit;
    }
	
    // Limpar SubAção
    if( $_REQUEST['req'] == 'limparSubacao' ){
        
    	if( limparSubAcao() ){
			echo 'alert("Dados excluídos com sucesso.");'
                ,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
                ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
        } 
		else{
			echo 'alert("Já foi dados parecer para esta subação. Os dados NÃO foram apagados.");'
				,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
				,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
		}	
    	
    }
    // Plano Interno
    elseif( $_REQUEST['req'] == 'prgplanointerno' ){
        echo $db->pegaUm('SELECT prgplanointerno
                          FROM cte.programa
                          WHERE prgid = ' . (integer) $_REQUEST['prgplanointerno']);
    }
    // Verificar Inserção Dados SubAção
    elseif( $_REQUEST['req'] == 'verificarInsercaoDadosSubacao' ){
        
            $sql = ' 
            		SELECT count(sba.sbaid)
            		FROM cte.subacaoindicador sba
	            		INNER JOIN cte.subacaobeneficiario ben ON sba.sbaid = ben.sbaid AND (vlrurbano > 0 OR vlrrural > 0)
	            		INNER JOIN cte.composicaosubacao cos ON sba.sbaid = cos.sbaid   AND (cosqtd    > 0) ';
            $sql .= $_REQUEST['sbaporescola'] == 't' ? '   INNER JOIN cte.qtdfisicoano qfa ON sba.sbaid = qfa.sbaid AND (qfaqtd    > 0) ' : " ";
			$sql .= ' WHERE sba.sbaid = ' . $_REQUEST['sbaid'];

        echo '<script>_totalizar(' , $db->pegaUm($sql) , ');</script>';

    }
    // Carregar Itens de Composição
    elseif( $_REQUEST['req'] == 'carregarItensComposicao' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

        $sql = montarSQLItensComposicaoRelatorioCompleto($strSubacoesFilhos);
        $db->monta_lista_simples( $sql, array( 'Tipo','Identificação do Item', 'Un. de Medida', 'Qtd', 'Valor Unitário',
                                              'Total' ), 1000, 1000, 'N', '100%','N' );
        
    }
    // Carregar Beneficiários
    elseif ($_REQUEST['req'] == 'carregarBeneficiarios' && $_REQUEST['sbaid'] != '' && $_REQUEST['sabano'] != '' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

        $sql = montarSQLBeneficiariosRelatorioCompleto($strSubacoesFilhos);

        $db->monta_lista_simples($sql, array( 'Tipo','Beneficiário', 'Zona Urbana', 'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%','N');

    } 
    // Carregar Escolas
    elseif( $_REQUEST['req'] == 'carregarEscolas' && $_REQUEST['sbaid']  != '' && $_REQUEST['qfaano'] != '' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

		$arEscolas = montarSQLEscolasRelatorioCompleto($strSubacoesFilhos);		
        $resp = $db->monta_lista_simples($arEscolas, array('Tipo', 'Código INEP', 'Escola', 'Quantidade'), 
        									 	   5000, 5000, 'N', '100%','N');
        									 	   
    }
    // Carregar dados do Parecer
    elseif( $_REQUEST['req'] == 'carregarDadosParecer' ){
	    if($strSubacoesFilhos){
			$strSubacoesFilhos = ",".$strSubacoesFilhos;
		}
        $sql = '
		        SELECT
		            coalesce(spt.sptparecer, \'\')      as sptparecer,
		            coalesce(spt.sptunt, 0)             as sptunt,
		            coalesce(spt.sptuntdsc, \'\')       as sptuntdsc,
		            coalesce(spt.sptano, 0)             as sptano,
		            coalesce(spt.sptinicio, 0)          as sptinicio,
		            coalesce(spt.sptfim, 0)             as sptfim,
		            coalesce(spt.sptanoterminocurso, 0) as sptanoterminocurso,
		            coalesce(spt.ssuid, 0)              as ssuid,
		            CASE WHEN spt.sbaid = ' . $_REQUEST['sbaid']   . ' THEN \'Original\' ELSE \'Aditivo\' END as tipo,
		            sba.sbaseqaditivo
		        FROM cte.subacaoparecertecnico spt
		        	INNER JOIN cte.subacaoindicador sba on spt.sbaid = sba.sbaid
		        WHERE spt.sbaid in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
				AND spt.sptano = ' . $_REQUEST['sbtano'] . 'ORDER BY spt.sbaid';
		
	    $todosParecer = $db->carregar($sql);
	    if(is_array($todosParecer)){
	    	$a = '<table width="100%" border="0"> ';
	    	foreach ($todosParecer as $oParecer){
				if($oParecer['sptparecer']){
	    		$a .= '<tr>
							<td width="10%" class="SubTituloDireita">'.$oParecer['tipo'].' : </td>
							<td width="90%" style="border-left:none;" >'.$oParecer['sptparecer'].'</td>
						</tr>';	
				}
			}
			$a .= '</table> ';
	    }
	   
	    /*
	    $tipoParecer = $db->carregarColuna($sql,7);
		$strTodosParecer = "";
		if(is_array($todosParecer)){
			$strTipoParecer .= implode("<br>", $todosParecer);
			$strTodosParecer .= implode("<br>", $todosParecer);
		}
		*/
        $res = $db->pegaLinha($sql);

        header('content-type: text/xml; charset=ISO-8859-1');
        echo '<?xml version="1.0" encoding="ISO-8859-1"?>' , "\n"
            ,'<response>'
            ,'<sptparecer><![CDATA[', $a         , ']]></sptparecer>'
            ,'<sptunt>'             , $res['sptunt']             , '</sptunt>'
            ,'<sptuntdsc><![CDATA[' , $res['sptuntdsc']          , ']]></sptuntdsc>'
            ,'<sptano>'             , $res['sptano']             , '</sptano>'
            ,'<sptinicio>'          , $res['sptinicio']          , '</sptinicio>'
            ,'<sptfim>'             , $res['sptfim']             , '</sptfim>'
            ,'<sptanoterminocurso>' , $res['sptanoterminocurso'] == 0 ? '' : $res['sptanoterminocurso'] , '</sptanoterminocurso>'
            ,'<ssuid>'              , $res['ssuid']              , '</ssuid>'
            ,'</response>';

    }
    // Carregar Totalizadores
    elseif( $_REQUEST['req'] == 'carregarTotalizadores' ){
        echo '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">'
            ,'<tr>'
            ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td>';

        if ($_REQUEST['sbaporescola'] != 't') {
            $sql = '
            SELECT
                coalesce(sum(ano2007),0) AS a2007,
                coalesce(sum(ano2008),0) AS a2008,
                coalesce(sum(ano2009),0) AS a2009,
                coalesce(sum(ano2010),0) AS a2010,
                coalesce(sum(ano2011),0) AS a2011,
                coalesce(sum(sptunt) ,0) AS total
            FROM (
                SELECT
                CASE WHEN sptano = 2007 THEN coalesce(sptunt,0) END AS ano2007,
                CASE WHEN sptano = 2008 THEN coalesce(sptunt,0) END AS ano2008,
                CASE WHEN sptano = 2009 THEN coalesce(sptunt,0) END AS ano2009,
                CASE WHEN sptano = 2010 THEN coalesce(sptunt,0) END AS ano2010,
                CASE WHEN sptano = 2011 THEN coalesce(sptunt,0) END AS ano2011,
            sptunt
            FROM cte.subacaoparecertecnico where sbaid = ' . $_REQUEST['sbaid'] . ') AS valores';

            $res = (array) $db->carregar($sql);

            $db->monta_lista_simples($sql, array('2007', '2008', '2009', '2010', '2011',
                                                 'Total'), 1000, 1000, 'N', '100%','N');

            echo '</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';
        }

        if ($_REQUEST['sbaporescola'] == 't') {
            // TOTALIZACAO POR ESCOLA
            $sql = '
		            SELECT DISTINCT ent.entcodent, ent.entnome, sum(qfa.qfaqtd) as qtdtotal, sum(tot.cosvlruni * ecs.ecsqtd) as vlrtotal
		            FROM cte.qtdfisicoano qfa
		            	LEFT JOIN cte.escolacomposicaosubacao ecs on qfa.qfaid = ecs.qfaid
			            LEFT JOIN cte.composicaosubacao cos on cos.cosid = ecs.cosid
			            INNER JOIN entidade.entidade ent on qfa.entid = ent.entid
			            LEFT JOIN (
			            			SELECT DISTINCT cos.cosid, cos.cosvlruni
		                 			FROM cte.escolacomposicaosubacao ecs
		                 				INNER JOIN cte.composicaosubacao cos on cos.cosid = ecs.cosid
		                 			GROUP BY cos.cosid, cos.cosvlruni
		                 		   ) as tot on tot.cosid = ecs.cosid
		            WHERE qfa.sbaid = ' . $_REQUEST['sbaid'] . ' and  qfa.qfaano not in (0) 
		            GROUP BY ent.entnome, ent.entcodent
		            ORDER BY ent.entnome, ent.entcodent';

            $db->monta_lista_simples($sql, array( 'Código INEP', 'Escola', 'Quantidade', 'Total (R$)'), 1000, 1000, 'S', '100%','N');

            echo '</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';

        }

        $sql = '
        SELECT
            cos.cosdsc
            ,coalesce(cos.cosqtd, 0) as cosqtd
            ,coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0) as cosvlrtotal
        FROM
            cte.composicaosubacao cos
        WHERE
            cos.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Identificação do Item',
                                             'Qtd',
                                             //'Valor Unitário',
                                             'Total'), 1000, 1000, 'S', '100%');

        echo '</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td style="font-weight:bold;text-align:center">Totalizador Por Beneficiários</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td>';

        $sql = '
        SELECT
            be.bendsc,
            sb.vlrurbano as vlrurbano,
            sb.vlrrural  as vlrrural,
            sb.vlrurbano + sb.vlrrural as sabvlrtotal
        FROM
            cte.subacaobeneficiario sb
        INNER JOIN
            cte.beneficiario be ON sb.benid = be.benid
        WHERE
            sb.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Beneficiário', 'Zona Urbana', 'Zona Rural', 'Total'), 1000, 1000, 'S', '100%');
        echo '</td>'
            ,'</tr>'
            ,'</table>';
    } // Fim de elseif( $_REQUEST['req'] == 'carregarTotalizadores' )

    die();
} // Fim de if( array_key_exists('req', $_REQUEST ) )

if( $_REQUEST["parecerPadrao"] && $_REQUEST["sbaid"] != ''){
	
	$sql = "SELECT ppsparecerpadrao FROM cte.proposicaosubacao ps
				INNER JOIN cte.subacaoindicador sba on sba.ppsid = ps.ppsid 
			WHERE sbaid = ". $_REQUEST["sbaid"];
	
  	echo $db->pegaUm( $sql );
	
	die();
}

function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}

include APPRAIZ . "includes/registraracesso.php";

$podeExcluirSubacao = cte_podeEditarSubacao($_SESSION["inuid"]);
$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$capturaTipo        = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

//-------------------------------------------------- SUBAÇÕES - CARGA DE DADOS
$sql = '
select
    s.sbaid,
    s.aciid,
    s.undid,
    s.frmid,
    s.sbadsc,
    s.sbastgmpl,
    s.sbaprm,
    s.sbapcr,
    s.sbadata,
    s.usucpf,
    --s.sbaparecer,
    s.psuid,
    s.ssuid,
    s.foaid,
    s.prgid,
    s.sbaporescola,
    s.ppsid,
    s.sbaordem,
    s.sbaobjetivo,
    s.sbatexto,
    s.sbacategoria
from
    cte.subacaoindicador s
left join
    cte.programa p on p.prgid = s.prgid
where
    s.sbaid = ' . (integer) $_REQUEST['sbaid'];

$sai = array_map('sql2html', (array) $db->pegaLinha($sql));
extract($sai);

$sbaporescola            = $sbaporescola == 't';
$existemEscolasAtendidas = $db->pegaUm('SELECT count(*) FROM cte.qtdfisicoano WHERE sbaid = ' . (integer) $_REQUEST['sbaid']) > 0;

$sql = '
select
    dim.dimid,
    dim.dimcod,
    dim.dimdsc,
    are.ardid,
    are.ardcod,
    are.arddsc,
    ind.indid,
    ind.indcod,
    ind.inddsc,
    aci.acidsc,
    aci.acilocalizador,
    pto.ptoid
from
    cte.acaoindicador aci
inner join
    cte.pontuacao pto on pto.ptoid = aci.ptoid and pto.ptostatus = \'A\'
inner join
    cte.indicador ind on ind.indid = pto.indid
inner join
    cte.areadimensao are on are.ardid = ind.ardid
inner join
    cte.dimensao dim on dim.dimid = are.dimid
where
    aci.aciid = ' . (trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid']);

$aci = (array) $db->pegaLinha($sql);
extract($aci);

$_SESSION['indid'] = $indid;
$aciid             = trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid'];

if($_REQUEST['sbaidpai']){
	$anoAditivo =  $_REQUEST['anoconvenio'];
	$_cosano = cte_recuperArArAno($anoAditivo);
}else{
$_cosano = cte_recuperArArAno();
}

?>

<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR - Plano de Metas - Subação<?php echo " - " , $titulo; ?></title>
	
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <link rel="stylesheet" type="text/css" href="../cte/includes/par_subacao.css"/>
	    
	
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	   
	  
	    <script type="text/javascript">
	    function getElementText(tag, parentElem, index)
	    {
	        if (!index) {
	            index=0;
	        }
	
	        var result = parentElem.responseXML.getElementsByTagName(tag)[index];
	
	        if (result) {
	            if (result.childNodes.length == 0) {
	                return '';
	            } else if (result.childNodes.length > 1) {
	                return result.childNodes[1].nodeValue;
	            } else {
	                return result.firstChild.nodeValue;
	            }
	        } else {
	            return '';
	        }
	    }
	    </script>
	</head>
	<body>
		<div id="loader-container">
	    	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
	    </div>
	
		<div id="container">
			<?php cte_montaTitulo( $titulo_modulo, '' ); ?>
	
			<!-- LANÇAMENTO DE DADOS -->
	
			<?php if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO) { ?>
				<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<colgroup><col/>
					</colgroup>
					<tbody>
						<tr>
							<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
								<img align=top" src="/imagens/atencao.png" /> <p>Para elaborar o Plano de Ações Articuladas é necessário finalizar o <a href="?modulo=principal/estrutura_avaliacao&acao=A" title="Exibir diagnóstico" onclick="return redirecionarFaseDiagnostico();">diagnóstico</a>.</p>
							</td>
						</tr>
					</tbody>
				</table>
	
				<script type="text/javascript">
				<!--
					function redirecionarFaseDiagnostico(){
						window.opener.location.href = '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A';
						window.opener.focus();
						window.close();
					}
							
					document.getElementById('loader-container').style.display = 'none';
				-->
				</script>
			<?php die();
			} // Fim de if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO) ?>
	
			<form method="post" name="frmParSubacao" id="frmParSubacao" action="<?php echo $_SERVER['REQUEST_URI']; ?>">
				<input type="hidden" name="formularioSumit" value="1"/>
				<input type="hidden" name="ppsid" id="ppsid" value="<?php echo $ppsid ?>"/>
				<input type="hidden" name="aciid" id="aciid" value="<?php echo $aciid ?>"/>
				<input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>"/>
				<input type="hidden" name="sbaordem" id="sbaordem" value="<?php echo $sbaordem ?>"/>
				<input type="hidden" name="anoExercicio" id="anoExercicio" value="0"/>
				<input type="hidden" name="sbaobjetivo" id="sbaobjetivo" value="<?php echo $sbaobjetivo ?>"/>
				<input type="hidden" name="sbaporescola" id="sbaporescola" value="<?php echo $sbaporescola ? 'true' : 'false' ?>"/>
	
				<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
					<tbody>
						<tr>
							<td class="SubTituloDireita">Dimensão:</td>
							<td><?php echo $aci['dimcod'] , '. ' , $aci['dimdsc'] ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Área:</td>
							<td><?php echo $aci['dimcod'] , '.' , $aci['ardcod'] , '. ' , $aci['arddsc']; ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Indicador:</td>
							<td>
								<?php echo $aci['dimcod'] , '.' , $aci['ardcod'] , '.' , $aci['indcod']   , '. ' , $aci['inddsc']; ?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Demanda:</td>
							<td><?php echo $aci['acilocalizador'] == "E" ? "Rede Estadual" : "Rede Municipal"; ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Ação:</td>
							<td><?php echo $aci['acidsc']; ?></td>
						</tr>
	
						<?php

						if( !CTE_NOVA_SUBACAO ){
	
							$select_formaAtendimento = $db->pegaUm('SELECT foadsc FROM cte.formaatendimento WHERE foaid = ' . (integer) $foaid);
							$select_programa         = $db->pegaUm('SELECT prgdsc FROM cte.programa WHERE prgid = '         . (integer) $prgid);
							$select_unidadeMedida    = $db->pegaUm('SELECT unddsc FROM cte.unidademedida WHERE undid = '    . (integer) $undid);
								
							$select_formaExecucao    = $db->pegaUm('SELECT frmdsc FROM cte.formaexecucao WHERE frmbrasilpro = false and  frmid = '    . (integer) $frmid);
								
							$prgplanointerno         = $db->pegaUm('SELECT prgplanointerno
																	FROM cte.subacaoindicador sba
																		INNER JOIN cte.programa prg ON prg.prgid = sba.prgid
																	WHERE sbaid = '. (integer) $sbaid); ?>
																	
							<tr style="background-color: #cccccc;">
								<td align="left" colspan="2">
									<strong>Dados da Subação</strong>
								</td>
							</tr>
							<tr id="tr_categoriaDespesa">
								<td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
								<td><?php echo $sbacategoria == 3 ? 'Custeio' : 'Capital'; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Forma de atendimento:</td>
								<td><?php echo $select_formaAtendimento; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Descrição da Subação:</td>
								<td><?php echo $sbadsc; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
								<td><?php echo $sbastgmpl; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Programa:</td>
								<td><?php echo $select_programa; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Plano Interno:</td>
								<td><?php echo $prgplanointerno; ?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">Unidade de Medida:</td>
								<td><?php echo $select_unidadeMedida; ?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">Forma de Execução:</td>
								<td><?php echo $select_formaExecucao ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
								<td><?php echo $sbapcr; ?></td>
								</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Cronograma:</td>
								<td><?php echo !$sbaporescola ? 'Global' : 'Por Escola'; ?></td>
							</tr>
							
					
						<?php
						} // Fim de if( !CTE_NOVA_SUBACAO ){
						else {
	
	
							//------------------------------------------------------- CONSTRUÇÃO DE COMBOS
							$select_formaAtendimento = $db->monta_combo('foaid',
																		'select foaid as codigo, foadsc as descricao from cte.formaatendimento order by foadsc',
																		'S', 'Escolha...', '', '', '', '', 'N', 'foaid', true);
								
							$select_programa         = $db->monta_combo('prgid',
																		'select prgid as codigo, substr(prgdsc, 1, 100) as descricao from cte.programa order by descricao',
																		'S', 'Selecione', 'exibirPlanoInterno', '', '', '', 'S', 'prgid', true);
								
							$select_unidadeMedida    = $db->monta_combo('undid',
																		'SELECT DISTINCT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni INNER JOIN cte.indicadorunidademedida iun ON uni.undid = iun.undid WHERE undtipo = \'' . $capturaTipo . '\' ORDER BY descricao',
																		'S', 'Escolha a unidade de medida', '', '','','','S', 'undid', true);
								
							$sql = '
								    SELECT frmid as codigo, frmdsc as descricao
								    FROM cte.formaexecucao
								    WHERE frmtipo = \'' . $capturaTipo . '\'
									AND frmbrasilpro = false
								    ORDER BY descricao';
								
							$select_formaExecucao = $db->monta_combo('frmid', $sql, 'S', 'Selecione', 'regraFormaExec', '','','','S', 'frmid', true); ?>

							<tr style="background-color: #cccccc;">
								<td align="left" colspan="2">
									<strong>Dados da Subação</strong>
								</td>
							</tr>
							<tr id="tr_categoriaDespesa">
								<td colspan="2">
									<a href="#" onclick="windowOpen('cte.php?modulo=principal/proposta/proposta_subacao&acao=A&aciid=<?php echo $aciid ?>', 'janelaProposta', 'width=400,height=300,scrollbars=yes'); return false;">
										Clique neste link para abrir opções de proposta de Subação
									</a>
								</td>
							</tr>
							<tr id="tr_categoriaDespesa">
								<td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
								<td>
									<select name="sbacategoria" class="CampoEstilo" id="sbacategoria">
										<option value="">Escolha...</option>
										<option value="3" <?php echo $sbacategoria == 3 ? 'selected="selected"' : '' ?>>Custeio</option>
										<option value="4" <?php echo $sbacategoria == 4 ? 'selected="selected"' : '' ?>>Capital</option>
									</select>
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Forma de atendimento:</td>
								<td><?php echo $select_formaAtendimento; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Descrição da Subação:</td>
								<td><?php echo campo_textarea('sbadsc', "S", 'S', '', 70, 3, 2000 ); ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
								<td><?php echo campo_textarea('sbastgmpl', 'S', 'S', '', 70, 3, 2000 ); ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Programa:</td>
								<td><?php echo $select_programa; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Plano Interno:</td>
								<td><?php echo campo_texto('prgplanointerno', "N", "N", "Plano Interno", 51, 100, "", "", 'left', '', 0, 'id="prgplanointerno"' );?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">Unidade de Medida:</td>
								<td><?php echo $select_unidadeMedida; ?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">Forma de Execução:</td>
								<td><?php echo $select_formaExecucao ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
								<td><?php echo campo_texto('sbapcr', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="sbapcr"');?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Cronograma:</td>
								<td>
									<input id="sbaporescola0" type="radio" name="sbaporescola" value="false" checked="checked" /><label for="sbaporescola0">Global</label>
									<input id="sbaporescola1" type="radio" name="sbaporescola" value="true"  /><label for="sbaporescola1">Por Escola</label>
								</td>
							</tr>
	
						<?php
						} // Fim de else
	
						$colspan       = sizeof($_cosano) + 1;
						$tamanhoCelula = round(100 / $colspan);
						if(count($_cosano) == 1){
							$colspan = 4;
						}
						?>
	          		</tbody>
	        	</table>
	
				<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" style="display:none" id="alertaPermitirCadastroItensSubacao">
					<tbody>
						<tr style="background-color: #cccccc;">
							<td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
								<strong>Finalize o cadastro da subação para que escolas, itens, beneficiários e parecer possam ser registrados.</strong>
							</td>
						</tr>
						<tr>
							<td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
								<input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar" value="Gravar" id="btnSalvar" <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
								<input onclick="return fecharJanela();" type="button" name="btnCancelar" value="Fechar" id="btnCancelar" />
							</td>
						</tr>
					</tbody>
				</table>
	
				<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
					<tbody>
						<tr style="background-color: #cccccc;">
							<td align="left" colspan="<?php echo $colspan; ?>" style="padding: 5px; font-size: 110%">
								<strong>Detalhamento dos Itens que Compõem a Especificação - <span id="spanAno"></span></strong>
							</td>
						</tr>
				
						<tr id="abasSelecaoCosano">
							<?php 
							
							foreach( $_cosano as $ano ){
						        $i = $ano;

						        echo '<td id="aba_' , $i , '" style="width: ' , $tamanhoCelula , '%;"><a onclick="return selecionarAba(' , $i , ', true);" href="' , $_SERVER['REQUEST_URI'] , '">' , $i , '</a></td>' , "\n";
					 		}// foreach ($_cosano as $ano)
					 		if(count($_cosano) != 1){
					 		?>
							<td id="aba_0000" style="width: <?php echo $tamanhoCelula; ?>%;"><a onclick="return selecionarAba('0000', true)" href="<?php echo $_SERVER['REQUEST_URI']; ?>">Totalizadores</a></td>
							<?php }else{ ?>
							<td width="90%" style="background: none; border:none;"></td>
							<td width="90%" style="background: none; border:none;"></td>
							<td width="90%" style="background: none; border:none;"></td>
							<?php } ?>
						</tr>
	
						<?php foreach( $_cosano as $ano ){
							$i = $ano; 
							?>
								
							<tr class="container" style="display: none; height: auto; vertical-align: top" id="abaCosano<?php echo $i; ?>">
								<td align="center" colspan="<?php echo $colspan; ?>">
								
									<table cellpadding="0" cellspacing="0" width="100%">
										<tr>
											<td style="background-color: #ddd"><div class="SubtituloEsquerda" style="padding: 5px; text-align: center"> Lista de Aditivos</div></td>
										</tr>
										<tr id="areaListaAditivo<?=$i; ?>" >
											<td id="divListaAditivo<?php echo $i; ?>" colspan="3" style="padding:5 5 5 5;"></td>
										</tr>
										<tr>
											<td style="background-color: #ddd">
												<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Quantidades e Cronograma Físico</div>
												<table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
													<?php if (!$sbaporescola) { ?>
														<tr id="cronogramaGlobal<?php echo $i; ?>">
															<td class="SubTituloDireita">Quantidades:</td>
															<td  style="padding:5px 5px 5px 5px; background-color: #fff" id="sptunt_<?=$i;?>" ></td>
														</tr>
													<?php } // Fim de if (!$sbaporescola) ?>
													<tr>
														<td class="SubTituloDireita">Cronograma Físico:</td>
														<td style="background-color: #fff; padding-left: 2px">
														<table border="0">
														  <tr>
														    <td name="sptinicio_<?php echo $i; ?>" id="sptinicio_<?php echo $i; ?>"></td>
														    <td>a</td>
														    <td name="sptfim_<?php echo $i; ?>" id="sptfim_<?php echo $i; ?>" ></td>
														    <td><label for="<?php echo 'sptanoterminocurso_' . $i?>">Ano término:</label>
														    <label id="sptanoterminocurso_<?=$i; ?>" name="sptanoterminocurso_<?=$i; ?>"></label>
														    </td>
														  </tr>
														</table>
													</td>
													</tr>
												</table>
												<br />
											</td>
										</tr>
		
										<?php if ($sbaporescola) { ?>
											<tr id="cronogramaPorEscola<?php echo $i; ?>">
												<td>
													<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Escolas</div>
													<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="escolasSubAcao<?php echo $i; ?>"></div>
													
												</td>
											</tr>
										<?php } ?>
										
										<tr>
											<td>
												<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Itens</div>
												<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao<?php echo $i; ?>"></div>
											</td>
										</tr>
										
										<tr id="beneficiarioSubAcao<?php echo $i; ?>">
											<td>
												<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Beneficiários</div>
												<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="beneficiariosSubAcao<?php echo $i; ?>"></div>
											</td>
										</tr>
		
										<tr>
											<td>
												<table id="parecerEquipeTecnica<?php echo $i; ?>" border="0" align="center" cellpadding="0" cellspacing="0" width="100%">
													<colgroup>
														<col width="25%" />
														<col width="75%" />
													</colgroup>
													<tr>
														<td align="right" class="SubTituloDireita">Detalhamento/Comentário:</td>
														<td style="padding:5px 5px 5px 5px;" id="sptuntdsc_<?=$i; ?>"></td>
													</tr>
		
													<?php if (CTE_PRM_VISUALIZAR_PARECER) { ?>
														<tr>
															<td colspan="2" style="text-align: center;font-weight:bold">Parecer Equipe Técnica</td>
														</tr>
														
														<?php if($_REQUEST["sbaid"] != ''){
															$sql = "SELECT ppsparecerpadrao 
																	FROM cte.proposicaosubacao ps
																		INNER JOIN cte.subacaoindicador sba on sba.ppsid = ps.ppsid 
																	WHERE sbaid = ". $_REQUEST["sbaid"];
															$res = $db->pegaUm( $sql );
																				  	
															if( $res ){ ?>
																<tr>
																	<td align="right" class="SubTituloDireita">&nbsp;</td>
																	<td>							  	
																		<input type="checkbox" onclick="carregarParecerPadrao();" name="parecerPadrao" id="parecerPadrao_<?php echo $i ?>" />
																		<label for="parecerPadrao_<?php echo $i ?>">Adicionar Parecer Padrão</label>
																	</td>
																</tr>
															<?php }
														} // Fim de if($_REQUEST["sbaid"] != '') ?>
														
														<tr>
															<td align="right" class="SubTituloDireita">Parecer:</td>
															<td  style="padding:5px 5px 5px 5px;" id="sptparecer_<?=$i;?>" ></td>
														</tr>
														
														<tr>
															<td align="right" class="SubTituloDireita">Status Subação:</td>
															<td  style="padding:5px 5px 5px 5px;">
																<?php
																if($strSubacoesFilhos){
																	$subacoes = $_REQUEST['sbaid'].",".$strSubacoesFilhos;
																}else{
																	$subacoes = $_REQUEST['sbaid'];
																}
																
																$sql="SELECT CASE WHEN spt.sbaid = '".$_REQUEST['sbaid']."' THEN 'Original' ELSE 'Aditivo' END as tipo,
																		ss.ssudescricao,
																		si.sbaseqaditivo
																		from cte.subacaoindicador si
																		left join cte.subacaoparecertecnico spt on spt.sbaid = si.sbaid
																		left join cte.statussubacao ss on spt.ssuid = ss.ssuid
																		where si.sbaid in ( " . $subacoes. ") and spt.sptano = '".$ano."' ORDER BY spt.sbaid";

																$situacaoParecer = $db->carregar($sql);
																
																	if($situacaoParecer != NULL){
																		if(is_array($situacaoParecer)){
																			$tabela = '<table width="100%" border="0"> ';
																			foreach($situacaoParecer as $pareceres ){
																				$tabela .= '<tr>
																								<td width="10%" class="SubTituloDireita">'.$pareceres['tipo']." ".$pareceres['sbaseqaditivo'].' : </td>
																								<td width="90%" style="border-left:none;" >'.$pareceres['ssudescricao'].'</td>
																							</tr>';
																							
																			}
																			echo $tabela .= '</table> ';
																		}
																	}else{
																		echo "Parecer não foi dado.";
																	}
																													
																?>
															</td>
														</tr>
		
													<?php } ?>
												</table><br />
											</td>
										</tr>
										
										<?php if (CTE_PRM_EMITIR_PARECER) {
											//echo '<script type="text/javascript">$("sptparecer_' , $i , '","ssuid_' , $i , '").each(function(e){if(e)e.disable()});</script>';
										} ?>
										
									</table>
								</td>
							</tr>
						<?php } // foreach ($_cosano as $ano) ?>
	
						<tr class="container" style="display: none; height: auto" id="abaCosano0000">
							<td align="left" colspan="<?php echo $colspan; ?>">
								<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="totalizadoresSubacao"></div>
							</td>
						</tr>
						<tr>
							<td align="center" colspan="<?php echo $colspan; ?>">
								<table width="100%">
									<tr>
										<td colspan="3" style="font-weight:bold;color:red;text-align : center; display:none" id="errorMessageContainer">
											Documentos em análise financeira devem conter Escolas, Itens de Composição e Beneficiários cadastrados.
										</td>
									</tr>
								</table>
							</td>
						</tr>
					</tbody>
				</table>
			</form>
		</div>
	</body>

	<?php
	if (function_exists('uniqid'))
    	$reqt = md5(rand() . time() . uniqid());
	else
    	$reqt = md5(rand() . time() . rand());

	if ($undid) {
		$sql = 'SELECT il.labid
	    		FROM cte.laboratorio l
	    			INNER JOIN cte.itenslaboratorio il ON l.labid = il.labid
	    		WHERE l.undid = ' . $undid;

	    if( ( $labid = $db->pegaUm( $sql ) ) === false )
	        $labid = 'null';
	} 
	else {
	    $labid = 'null';
	}
	?>
	
	<script type="text/javascript">
  		var IExplorer               = !!document.all;
	    var safari                  = navigator.userAgent.indexOf( 'Safari' ) > 0;
	    var gecko                   = navigator.product == 'Gecko';
	    var _displayBlock           = IExplorer ? 'block' : 'table-row';
	
	    var TAB_TOTALIZADORES       = '0000';
	    var existemEscolasAtendidas = <?php echo $sbaporescola && $existemEscolasAtendidas ? 'true' : 'false' ?>;
	    var anoExercicio            = '<?php echo ($_REQUEST['parsubacaoanocrt'])?$_REQUEST['parsubacaoanocrt']:date('Y');  ?>';
	    var permitirCadastroSubacao = <?php echo trim($_REQUEST['sbaid']) != '' ? 'true' : 'false'; ?>;
	
	    var itemModificado          = false;
	
	    var totalizadorItensComposicao = 0;
	    var totalizadorBeneficiarios   = 0;
	    var totalizadorEscolas         = 0;
	    var labid                      = <?php echo $labid; ?>;
	    var total                      = 0;
	    
	    var anoAtualTrava 	= <?php echo $anoAtualTrava;?>;
	    var anoParecer2007 	= <?php echo $anoParecer2007;?>;
	    var anoParecer2008 	= <?php echo $anoParecer2008;?>;
	    var anoParecer2009 	= <?php echo $anoParecer2009;?>;
	    var anoParecer2010 	= <?php echo $anoParecer2010;?>;
	    var anoParecer2011 	= <?php echo $anoParecer2011;?>;
	    var anoConvenio 	= <?=$anoConvenio;?>;
	    var novaSub 		= <?php echo $novaSub = CTE_NOVA_SUBACAO ? 'true' : 'false'; ?>;
	    
	    var reqt 		= '<?php echo $reqt; ?>';
	    var porEscola 	= '<?php echo $sbaporescola ? 't' : 'f'; ?>';
	    var subacao		= <?php echo $_REQUEST['sbaid'] ? $_REQUEST['sbaid'] : 0;  ?>;
	    var eAditivo	= <?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>;

	    /**
	     * @public
	     * @global
	     */
	     
	     function carregarDadosParecerRelatorio() // Parametro utilizado apenas na função travaAbaAno(cosano)
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&reqt='+ reqt +'&req=carregarDadosParecer&sbtano=' + anoExercicio,
                                asynchronous: false,
                                onComplete: function(res)
                                {
                                    var sptparecer              = $('sptparecer_'         + anoExercicio);
                                    var sptunt                  = $('sptunt_'             + anoExercicio);
                                    var sptuntdsc               = $('sptuntdsc_'          + anoExercicio);
                                    var sptinicio               = $('sptinicio_'          + anoExercicio);
                                    var sptfim                  = $('sptfim_'             + anoExercicio);
                                    var sptanoterminocurso      = $('sptanoterminocurso_' + anoExercicio);
                                    var ssuid                   = $('ssuid_'              + anoExercicio);

                                    var ssuidValue              = getElementText('ssuid'             , res);
                                    var sptinicioValue          = getElementText('sptinicio'         , res);
                                    var sptfimValue             = getElementText('sptfim'            , res);
                                    var sptanoterminocursoValue = getElementText('sptanoterminocurso', res);
                                    var valorInicio = "";
                                    var valorFim = "";

                                    sptanoterminocurso.innerHTML = sptanoterminocursoValue;

                                    if (sptunt)
                                        sptunt.innerHTML = getElementText('sptunt', res);
									
                                    if(sptinicioValue == 1){
                                        valorInicio = "Janeiro";
                                    }else if(sptinicioValue == 2){
                                        valorInicio = "Fevereiro";
                                    }else if(sptinicioValue == 3){
                                        valorInicio = "Março";
                                    }else if(sptinicioValue == 4){
                                        valorInicio = "Abril";
                                    }else if(sptinicioValue == 5){
                                        valorInicio = "Maio";
                                    }else if(sptinicioValue == 6){
                                        valorInicio = "Junho";
                                    }else if(sptinicioValue == 7){
                                        valorInicio = "Julho";
                                    }else if(sptinicioValue == 8){
                                        valorInicio = "Agosto";
                                    }else if(sptinicioValue == 9){
                                        valorInicio = "Setembro";
                                    }else if(sptinicioValue == 10){
                                        valorInicio = "Outubro";
                                    }else if(sptinicioValue == 11){
                                        valorInicio = "Novembro";
                                    }else if(sptinicioValue == 12){
                                        valorInicio = "Dezembro";
                                    }
                                        sptinicio.innerHTML = valorInicio;
                                    
                                    if(sptfimValue == 1){
                                        valorFim = "Janeiro";
                                    }else if(sptfimValue == 2){
                                        valorFim = "Fevereiro";
                                    }else if(sptfimValue == 3){
                                        valorFim = "Março";
                                    }else if(sptfimValue == 4){
                                        valorFim = "Abril";
                                    }else if(sptfimValue == 5){
                                        valorFim = "Maio";
                                    }else if(sptfimValue == 6){
                                        valorFim = "Junho";
                                    }else if(sptfimValue == 7){
                                        valorFim = "Julho";
                                    }else if(sptfimValue == 8){
                                        valorFim = "Agosto";
                                    }else if(sptfimValue == 9){
                                        valorFim = "Setembro";
                                    }else if(sptfimValue == 10){
                                        valorFim = "Outubro";
                                    }else if(sptfimValue == 11){
                                        valorFim = "Novembro";
                                    }else if(sptfimValue == 12){
                                        valorFim = "Dezembro";
                                    }
                                        sptfim.innerHTML = valorFim;

                                    if (ssuid) {
                                        for (var i = 0; i < ssuid.options.length; i++) {
                                            if (ssuidValue == ssuid.options[i].value)
                                                ssuid.selectedIndex = ssuid.options[i].index;
                                        }
                                    }

                                    if (sptparecer)
                                        sptparecer.innerHTML = getElementText('sptparecer', res);

                                    if (sptuntdsc)
                                        sptuntdsc.innerHTML  = getElementText('sptuntdsc',  res);
                                        sptuntdsc.disabled="disabled";

                                    $('loader-container').hide();
                                }
                            });
}
	    
</script>

<script type="text/javascript" src="../cte/includes/par_subacao.js"></script>

<script type="text/javascript">
	    //Recupera os aditivos da subação pela aba do ano.
	     function carregarAditivos(cosano)
	    {	
	        return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&reqt='+ reqt +'&req=carregaAditivos&cosano=' + cosano,
	                                    asynchronous: false,
	                                    onComplete: function(res)
	                                    {	
	                                      // Controle do que deve ser travado de acordo com a situação da subação.
	                                        <?php if (!CTE_NOVA_SUBACAO) { ?>   
           											if( ( <?= CTE_ESTADO_PAR?> == <?=$documento["esdid"]; ?> ) && <?php echo (integer) !cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ?>  ){
           												travaAbaAnoJaAnalisada(cosano);
           	 										}else if(<?=$anoConvenio; ?>){
               											travaAbaAnoSeAditivada(cosano);
               										}
               										if(<?=$anoConvenio; ?> != cosano ){
               											travaAnosAnteriores(cosano);
               										}
            									<? } ?>
											//$('areaListaAditivo').style.display="table-row";
	                                        $('divListaAditivo' + anoExercicio).innerHTML = res.responseText; // Mostra lista de Aditivos
	                                    }
	                                });
	    }
		<?php
		//------------------------- FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA

		if ($sbaporescola) {

    		if (CTE_PRM_EDITAR_SUBACAO) { ?>
			    /**
			     * 
			     */
			    function removerEscola(qfaid)
			    {
			        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
			            return false;
			        }
			
			        return new Ajax.Request(window.location.href,
			                                   {
			                                       method: 'post',
			                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerEscola&qfaid=' + qfaid,
			                                       onComplete: function(res)
			                                       {
			                                           carregarEscolas();
			                                       }
			                                   });
			
			        return false;
			    }
			
			<?php
			} // Fim de if (CTE_PRM_EDITAR_SUBACAO) 
    		else {
				echo 'function removerEscola() {return false;}';
			} ?>

    /**
     * 
     */
    function extratoEscolas(qfaid)
    {
        return windowOpen('/cte/cte.php?modulo=principal/extratoescolassubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaid=' + qfaid,
                          'extratoEscolas',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function carregarEscolas()
    {
		$('escolasSubAcao' + anoExercicio).style.height = "auto";
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&reqt=<?php echo $reqt; ?>&req=carregarEscolas&qfaano=' + anoExercicio,
                                    onComplete: function(res)
                                    {
                                        $('escolasSubAcao' + anoExercicio).innerHTML = res.responseText;
                                        
                                        
                                        var totalEscolas = $('somaTotalEscolas'+ anoExercicio ).innerHTML;
                                        
                                        if( totalEscolas > 20 ){
	                                        $('escolasSubAcao' + anoExercicio).style.height = "500px";
                                        }
                                        else{
	                                        $('escolasSubAcao' + anoExercicio).style.height = "auto";
                                        }
                                        $('loader-container').hide();
                                    }
                                });
    }


    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    {	
        if (itemModificado) {
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {validacao: 'false'},
                onComplete: function()
                {
                    $('loader-container').hide();
                }
            });
        } else {
            itemModificado = false;
        }

        $('loader-container').show();
        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';

        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
        } else {
<?php
// Desabilita o campo Detalhamento/Comentarios para formas de execução
// forem "EXECUTADA PELO MUNICIPIO" ou "ASSISTENCIA TECNICA DO MEC"
if ($frmid == 4 || $frmid == 10) {
    echo 'var sptuntdsc = $(\'sptuntdsc_\' + anoExercicio);'
        ,'if (sptuntdsc) sptuntdsc.up(2).hide();';
}
?>

            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarEscolas();
                carregarDadosParecerRelatorio();
                carregarAditivos(cosano);
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
//------------------------ ! Fim das FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA
<?
} else {

?>
    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    { 
        if (itemModificado) {
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {validacao: 'false'},
                onComplete: function()
                {
                    $('loader-container').hide();
                }
            });
        } else {
            itemModificado = false;
        }

        $('loader-container').show();
        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
       
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.color              = '#333';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';
        $('aba_'      + cosano)      .firstChild.style.color              = '#333';
		
        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
        } else {
<?php
// Desabilita o campo Detalhamento/Comentarios para formas de execução
// forem "EXECUTADA PELO MUNICIPIO" ou "ASSISTENCIA TECNICA DO MEC"
if ($frmid == 4 || $frmid == 10) {
    echo 'var sptuntdsc = $(\'sptuntdsc_\' + anoExercicio);'
        ,'if (sptuntdsc) sptuntdsc.up(2).hide();';
}
?>
            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarDadosParecerRelatorio();
                carregarAditivos(cosano);
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
<?php

} // if ($sbaporescola) {

?>


    /**
     * 
     */
    function verificarModificaoItensComposicao(el)
    {
        if (!itemModificado)
            itemModificado = el.value != el.defaultValue;
    }


    /**
     * 
     */
    function regraFormaExec()
    {
        return true;
    }

    function exibirPlanoInterno(value)
    {
        new Ajax.Request(window.location.href,
                         {
                             parameters: '&reqt=<?php echo $reqt; ?>&req=prgplanointerno&prgplanointerno=' + value,
                             onComplete: function(e)
                             {
                                 $('prgplanointerno').value = e.responseText;
                             }
                         });
    }


    /**
     * 
     */
    function calcularValorTotal(cosid)
    {
        var vlrtotal = string2float($F('cosqtd'    + cosid)) *
                       string2float($F('cosvlruni' + cosid)) ;

        $('cosvlrtotal' + cosid).innerHTML = float2string(vlrtotal);
    }

    /**
     * 
     */
    function string2float(str)
    {
        if (str.indexOf(',') == -1)
            return parseFloat(str);
        else
            return parseFloat(str.replace(/\./g, '').replace(/,/g, '.'));
    }

    /**
     * 
     */
    function float2string(num)
    {
        return mascaraglobal('###.###.###.###.###,##', (parseFloat(num).toFixed(2)) + '');
    }


    /**
     * @private
     */
    //this._closeWindows = false;

<?php
if (!CTE_PRM_EDITAR_SUBACAO) {

?>
    function submeterFrmParSubacao(submit, vlrProximaSubacao)
    {	
        if (vlrProximaSubacao != '') {
            window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' + vlrProximaSubacao + '&bloqueado=1';
        } else {
            window.opener.location.reload();
            window.opener.focus();
            window.close();
        }
    }
    // 2083
    Form.disable('frmParSubacao');

    var btnCancelar = $('btnCancelar');
    var btnProximo  = $('btnProximo');
    var btnAnterior = $('btnAnterior');

    if (btnCancelar)
        btnCancelar.removeAttribute('disabled');

    if (btnProximo)
        btnProximo .removeAttribute('disabled');

    if (btnAnterior)
        btnAnterior.removeAttribute('disabled');

    //---------------------------------------------------------------- POP-UPS
    function popupSelecionarEscolas(qfaano)
    {
        return false;
    }

    function popupEditarSubacao()
    {
        return false;
    }

    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return false;
    }

    function adicionarItensComposicao()
    {
        return false;
    }

    function adicionarBeneficiarios()
    {
        return false;
    }

   

    function removerBeneficiario(benid)
    {
        return false;
    }
<?php
} else {
?>

    var submeter = false;
    //---------------------------------------------------------------- POP-UPS
    /**
     * 
     */
    function adicionarItensComposicao()
    {
        if (itemModificado)
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {validacao: 'false'},
                onComplete: function()
                {
                    $('loader-container').hide();
                }
            });

        return windowOpen('/cte/cte.php?modulo=principal/cadastraritenssubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosano=' + anoExercicio +'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>+ '&ad='+<?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>,
                          'adicionarItensComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }


    /**
     * 
     */
    function adicionarBeneficiarios()
    {
        return windowOpen('/cte/cte.php?modulo=principal/cadastrarbeneficiariossubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&sabano=' + anoExercicio+'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>+ '&ad='+<?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>,
                          'adicionarBeneficiarios',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    

    /**
     * 
     */
    function removerBeneficiario(benid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        var req = new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerBeneficiario&benid=' + benid + '&sabano=' + anoExercicio,
                                       onComplete: function(res)
                                       {
                                           carregarBeneficiarios();
                                       }
                                   });

        return false;
    }

    /**
     * 
     */
    function popupSelecionarEscolas(qfaano)
    {
        return windowOpen( '/cte/cte.php?modulo=principal/cadastrarescolassubacao&acao=A&tipo=cadastrarEscolas&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaano=' + qfaano,
                           'popupSelecionarEscolas',
                           'height=400, width=800, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
        return false;
    }

    /**
     * 
     */
    function popupEditarSubacao()
    {
        return windowOpen('/cte/cte.php?modulo=principal/popupeditarsubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>',
                          'editarSubacao',
                          'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return windowOpen('/cte/cte.php?modulo=principal/escolasescolhasubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosid='+cosid+'&qfaano='+anoExercicio +'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>,
                          'escolherEscolasItemComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
    }

    function submeterFrmParSubacao(submit, vlrProximaSubacao)
    {
        $('anoExercicio').value = anoExercicio;

		// Exclusão de sub-ação
        if (vlrProximaSubacao == '-1' && permitirCadastroSubacao) {
            if (confirm('Deseja realmente excluir a subação?')) {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
                $('frmParSubacao').submit();
                return true;
            } else {
                return false;
            }
        }

		// Gravar sub-ação
        if (submit) {
            var sbapcr = $('sbapcr');

			<?php if (CTE_NOVA_SUBACAO) {
                echo 'while (sbapcr.disabled)';
            } ?>
            Form.enable("frmParSubacao");

            var sptunt             = $('sptunt_'             + anoExercicio);
            var sptinicio          = $('sptinicio_'          + anoExercicio);
            var sptfim             = $('sptfim_'             + anoExercicio);
            var sptanoterminocurso = $('sptanoterminocurso_' + anoExercicio);
			

			
			// Verificando se cronograma físico e quantidade estão preenchidos.			
			if( vlrProximaSubacao ){
				
				<?php
				if( $_REQUEST['sbaid'] ){
									
					$sql = "select sbaporescola from cte.subacaoindicador where sbaid = ". $_REQUEST['sbaid'];
					$sbaPorEscola = $db->pegaUm($sql); ?>
				
					sbaporescola = '<?php echo $sbaPorEscola ?>';
					
					<?php
					if( $estadoDocumento["esdid"] == CTE_ESTADO_PAR ){ ?> 
						
						var boErro = false;
						var complementoQtd = "";
						var complementoDatas = "";
						
						if( sbaporescola == 'f' ) {
							var bosptunt = sptunt.value == "" || sptunt.value == 0 ? false : true;
							
							if( !bosptunt ){
								boErro = true;
								var complementoQtd = "- Quantidades;\n"; 
							}
						}
							
						var bosptinicio = sptinicio.value == "" || sptinicio.value == 0 ? false : true;
						var bosptfim = sptfim.value == "" || sptfim.value == 0 ? false : true;
						
						if( !bosptinicio || !bosptfim ){
								boErro = true;
								var complementoDatas = "- Cronograma Físico (início e fim)."; 
						}
						
						if( boErro ){
							alert(  "Não foi possível gravar essa subação.\n"+
									"Favor verificar se todos os campos listados abaixo estão preenchidos:\n"+
									complementoQtd +
									complementoDatas );
							return false;						
						}
					
					<?php } // Fim de if( $estadoDocumento["esdid"] == CTE_ESTADO_PAR )
				} // Fim de if( $_REQUEST['sbaid'] ) ?>
			} // Fim de if( vlrProximaSubacao ){

            if (vlrProximaSubacao != '' && sptanoterminocurso) {
                if (sptanoterminocurso.value == '' && (Number(sptinicio.value) > Number(sptfim.value))) {
                    alert('O mês fim para o campo Cronograma Físico deve ser maior ou igual o mês inicial ou o ano de término deve ser preenchido.');
                    return false;
                } else if (sptanoterminocurso.value != '' && Number(sptanoterminocurso.value) <= anoExercicio) {
                    alert('O ano de término do cronograma deve ser maior do que o ano do lançamento do exercício.');
                    return false;
                }
            }

            if (vlrProximaSubacao != '') {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
            }

            var errors    = new Array();

            var sbadsc    = $('sbadsc');
            var sbastgmpl = $('sbastgmpl');
            var prgid     = $('prgid');
            var undid     = $('undid');
            var frmid     = $('frmid');

            if (sbadsc    && sbadsc.value == '')
                errors.push('Descrição');

            if (sbastgmpl && sbastgmpl.value == '')
                errors.push('Estratégia de Implementação');

            if (prgid     && (prgid.value == '0' || prgid.value == ''))
                errors.push('Programa');

            if (undid     && (undid.value == '0' || undid.value == ''))
                errors.push('Unidade de Medida');

            if (frmid     && (frmid.value == '0' || frmid.value == ''))
                errors.push('Forma de Execução');
			
            if (errors.length > 0) {
                if (errors.length == 1)
                    alert('O campo ' + errors.join('') + ' é obrigatório!');
                else
                    alert('Os campos abaixo são obrigatórios:\n - '
                         +errors.join('\n - ')
                         +'\n\nPor favor corrija para continuar!');

                return false;
            } 
            else {
				<?php if (in_array($estadoDocumento['esdid'], array(30,31,26))) { ?>
	                if (vlrProximaSubacao != '' && total <= 0) {
	                    alert('Em fase de análise financeira, a sub-ação deve conter Escolas, Itens de Composição e Beneficiários viculados.');
	                    return false;
	                }
				<?php 
				} // if ($estadoDocumento['esdid'] == 59)@1823 ?>

                var submeter = true;
                var inputs   = $('frmParSubacao').getInputs('text');

				<?php if ($labid == 'null' && ($frmid != 4 && $frmid != 10)) { ?>
	                for (var i = 0; i < inputs.length; i++) {
	                    if (/cosvlruni/.test(inputs[i].getAttribute('name')) ||
	                        /cosqtd/   .test(inputs[i].getAttribute('name'))) {
	                        if (string2float(inputs[i].value) <= 0) {
	                            inputs[i].setStyle({backgroundColor: '#ffd'});
	                            submeter = false;
	                        } else {
	                            inputs[i].setStyle({backgroundColor: '#fff'});
	                        }
	                    }
	                }
				<?php
				} ?>
                if (submeter) {
                    if (vlrProximaSubacao != '-1' && vlrProximaSubacao != '') {
                        if (confirm('Atenção!\nOs dados serão gravados somente para o ano ' + anoExercicio + '.\n\nDeseja continuar?')) {
                            $('loader-container').show();
                            $('frmParSubacao').request(
                            {
                                encoding: 'ISO-8859-1',
                                parameters: {
                                    anoExercicio: anoExercicio
                                },
                                onComplete: function(response)
                                {
                                    eval(response.responseText);
                                    $('loader-container').hide();
                                }
                            });
                        }
                    } 
                    else {
                        $('loader-container').show();
                        $('frmParSubacao').request(
                        {
                            encoding: 'ISO-8859-1',
                            parameters: {
                                anoExercicio: anoExercicio,
                                validacao: 'false'
                            },
                            onComplete: function(response)
                            {
                                eval(response.responseText);
                                $('loader-container').hide();
                            }
                        });
                    }

                    return false;
                } 
                else {
                    alert('O dados para Quantidade e Valor Unitário dos Itens de Composição devem ser preenchidos.');
                    return false;
                }
            }
        } 
        else {
            if (vlrProximaSubacao != '') {
                window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' + vlrProximaSubacao;
            } 
            else {
                window.opener.location.reload();
                window.opener.focus();
                window.close();
            }
        }
    }

<?php

} // if (!CTE_PRM_EDITAR_SUBACAO) {

if (trim($_REQUEST['sbaid']) != '')
	echo '        selecionarAba(anoExercicio, true);';
else {
    echo '        $("alertaPermitirCadastroItensSubacao").show();
        $("tabAbasSelecaoCosano").hide();
        $("btnExcluir").setAttribute("disabled", "disabled");';
}

?>

    //$('loader-container').hide();

<?php

if (CTE_NOVA_SUBACAO && $capturaTipo == 'M' && !cte_verificaGrandeMunicipio($_SESSION['inuid'])) {

?>
    Form.disable('frmParSubacao');
    var btnCancelar = $('btnCancelar');

    if (btnCancelar)
        btnCancelar.removeAttribute('disabled');

<?php

} // if (CTE_NOVA_SUBACAO && $capturaTipo == 'M')

?>
    function propostaSubAcao(arr)
    {
        $('ppsid').value        = arr['ppsid'];
        $('prgid').value        = arr['prgid'];
        $('frmid').value        = arr['frmid'];
        $('sbadsc').value       = arr['ppsdsc'];
        $('sbaordem').value     = arr['ppsordem'];
        $('sbastgmpl').value    = arr['ppsmetodologia'];
        $('sbacategoria').value = arr['sbacategoria'];

        for (var i = 0, length = $('undid').options.length; i < length; i++) {
            if ($('undid').options[i].value == arr['undid'])
                $('undid').selectedIndex = $('undid').options[i].index;
        }

        Form.disable('frmParSubacao');

        var btnSalvar   = $('btnSalvar');
        var btnCancelar = $('btnCancelar');

        if (btnSalvar)
            btnSalvar.removeAttribute('disabled');

        if (btnCancelar)
            btnCancelar.removeAttribute('disabled');

        if (arr['indqtdporescola'] == 'f') {
            $('sbaporescola0').checked = true;
            $('sbaporescola0').enable();
            $('sbaporescola1').enable();
        } else {
            $('sbaporescola1').checked = true;
        }
    }

<?php

if (CTE_NOVA_SUBACAO) {
    echo '$(\'loader-container\').hide();';

} else {
?>

    function limparSubacao(sbaid)
    {
        if (confirm('Atenção!\nTodo o preenchimento do ano ' + anoExercicio + ' da subação será excluído.\n'
                   +'Os dados NÃO poderão ser recuperados posteriormente.\n\nDeseja continuar?'))
        {
            $('loader-container').show();
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {
                    anoExercicio: anoExercicio,
                    req: 'limparSubacao',
                    formularioSumit: 0
                },
                onComplete: function(response)
                {
                    eval(response.responseText);
                    $('loader-container').hide();
                }
            });
        }
    }


<?php } ?> 

	</script>
</html>

<?php die(); ?> 
