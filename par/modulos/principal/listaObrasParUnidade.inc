<?php
/*** INCLUDES ***/
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';

/*** INSTANCIA AS DE CLASSES ***/
$obPreObraControle 		= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();

/*** DECLARAÇÃO DE VARIAVEIS E FUNCOES ***/
if( !$_SESSION['par']['inuid'] ){
	echo "<script>
                    alert('Faltam dados na sessão!');
                    history.back(-1);
              </script>";
	die;
}
//criando variável que fará parte do nome do arquivo gerado em excel pelo dataTable
$dataAtual 			= date('d-m-Y_His'); 

$muncod 			= $_SESSION['par']['muncod']; 
$booQtdObrasEstado 	= true;
if($_REQUEST['tipo']){
	$booQtdObrasEstado = $obPreObraControle->verificaQtdObrasEstado(Array('estuf'=>$_SESSION['par']['estuf'],'tipo'=>$_REQUEST['tipo']));
}

$nrAdesao	= $obPreObraControle->verificaPrazoExpiraAdesao($muncod);
$boAdesao 	= $nrAdesao > 0 ? true : false;
$perfil 	= pegaArrayPerfil($_SESSION['usucpf']);
$escrita 	= verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

unset($_SESSION['par']['preid']);

$arrWhere = Array("inu.inuid = {$_SESSION['par']['inuid']}","pre.prestatus = 'A'","pre.preidpai IS NULL");

/* Os perfis de Equipe Municipal, EM Aprovação, Prefeito, Equipe Estadual e Estadual Aprovação não devem ver obras Arquivadas
 * */
$arrNotArquivados = Array(	PAR_PERFIL_EQUIPE_MUNICIPAL,
							PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,
							PAR_PERFIL_PREFEITO,
							PAR_PERFIL_EQUIPE_ESTADUAL,
							PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO);

$arrComparacao = array_intersect( $perfil, $arrNotArquivados );
if( $arrComparacao[0] != '' ){
	$arrWhere[] = "esd.esdid NOT IN (".WF_PAR_OBRA_ARQUIVADA.")"; 
}
/* FIM - Os perfis de Equipe Municipal, EM Aprovação, Prefeito, Equipe Estadual e Estadual Aprovação não devem ver obras Arquivadas
 * */

$podeProrrogar = false;
if( in_array( PAR_PERFIL_PREFEITO,$perfil ) ||
	in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil ) ||
	in_array( PAR_PERFIL_ADMINISTRADOR,$perfil ) ||
	in_array( PAR_PERFIL_SUPER_USUARIO,$perfil )
){
	$podeProrrogar = true;
}

// RECUPERA OBRAS DO PAR
$sql = "
    SELECT DISTINCT
		pre.preid, 
		(SELECT obrid FROM obras2.obras WHERE obrid = pre.obrid AND obridpai IS NULL LIMIT 1 ) as obrid,
		pre.docid, 
		pre.presistema,
		pre.estuf,
		pre.muncod,
		pre.predtinclusao,
		pre.predescricao,
		pre.preano,
		pre.pretipofundacao,
		esd.esdid,
		esd.esddsc as esddsc,
		pto.ptodescricao,
		pre.preprioridade, 
		sub.sbadsc,
		sub.sbaid,
		pre.preblockreformulacao
	FROM 
		obras.preobra pre
		--LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid AND obridpai IS NULL
		LEFT  JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
		INNER JOIN workflow.documento doc ON doc.docid = pre.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		INNER JOIN par.subacaoobra sbo ON sbo.preid = pre.preid
        INNER JOIN par.subacao sub on sub.sbaid = sbo.sbaid AND sub.sbastatus = 'A' 
        INNER JOIN par.acao aca ON aca.aciid = sub.aciid AND aca.acistatus = 'A'
        INNER JOIN par.pontuacao pon ON pon.ptoid = aca.ptoid AND pon.ptostatus = 'A'
        INNER JOIN par.instrumentounidade inu ON inu.inuid = pon.inuid AND ( pre.estuf = inu.estuf OR pre.muncod = inu.muncod )
	WHERE 
		".implode(' AND ',$arrWhere)."
	ORDER BY 
	    pre.predescricao, 
	    ptodescricao;
";
//ver($sql, d);
$arPreObrasSemCobertura = $db->carregar($sql);
$arPreObrasSemCobertura = $arPreObrasSemCobertura ? $arPreObrasSemCobertura : array();

// verifica se é um estado ou um municipio
if(empty($_SESSION['par']['muncod']) && !empty($_SESSION['par']['estuf'])) {
    $descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['estuf'], 1);
} else {
    $descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);
}

$arrResultado = array();

// PERCORRE OBRAS DO PAR
if(is_array($arPreObrasSemCobertura)) {
    foreach($arPreObrasSemCobertura as $k => $preobra) 
    {
        if($esdid == WF_PAR_EM_CADASTRAMENTO) {
    		$acao = "<img src=\"../imagens/alterar.gif\" class=\"alterar\" style=\"cursor:pointer\" id=\"" . $preobra['preid'] . "-" . $preobra['preano'] . "-" . $preobra['sbaid'] . "\" />";
        } else {
    		$acao = "<img src=\"../imagens/alterar.gif\" class=\"alterar\" style=\"cursor:pointer\" id=\"" . $preobra['preid'] . "-" . $preobra['preano'] . "-" . $preobra['sbaid'] . "\" />";
        }
        
        $acao .= "<img src=\"../imagens/consultar.gif\" title=\"abrir\" style=\"cursor:pointer;\" onclick=\"window.open('par.php?modulo=principal/subacao&acao=A&sbaid=" . $preobra['sbaid'] . "', 'modelo', 'height=600,width=800,scrollbars=yes,top=0,left=0' )\">";
        
        #Adiciona Icone de historico
        $acao.="<img src=\"../imagens/schedule.png\" title=\"Histórico de Prorrogação de Prazo\" style=\"cursor:pointer\" class=\"historicoProrrogacao\" id=\"{$preobra['preid']}\" />";
        
        // INICIO prorrogação de prazo
        $sql = "SELECT
					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
				FROM
					par.pagamentoobrapar po
				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				WHERE
					po.preid = {$preobra['preid']}";
        
        $data_primeiro_pagamento = $db->pegaUm( $sql );
        
        $sql = "SELECT
					coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
				FROM
					par.pagamentoobrapar po
				INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
        		WHERE
        			po.preid = {$preobra['preid']}";
        
        $prazo = $db->pegaUm($sql);
        
        /* Trecho que preenche a coluna Fim da vigência */
        $sql = "
            SELECT
    			MIN(pag.pagdatapagamentosiafi) AS data_primeiro_pagamento,
    			MIN(pag.pagdatapagamentosiafi) + 720 as prazo
    		FROM
    			par.pagamentoobrapar po
    		INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
    		WHERE
    			po.preid = " . $preobra['preid'];
         
        $dataPrimeiroPagamento = $db->carregar($sql);
        
        $fimVigenciaObra = '';
        $prazoProrrogar = '';
        if( !empty($dataPrimeiroPagamento[0]['data_primeiro_pagamento']) ) {
        	$sql = "SELECT
                      to_char(popdataprazoaprovado, 'YYYY-MM-DD') as popdataprazoaprovado,
                      arqid,
                      popstatus
                    FROM obras.preobraprorrogacao
                    WHERE preid = ".$preobra['preid']."
                    ORDER BY popid DESC";

        	$arrPreObraProrrogacao = $db->carregar($sql);
            $prorrogacaoAtiva = 0;
            $popdataprazoaprovadoAtivo = '';
            $prorrogacaoPendente = 0;

            if(!empty($arrPreObraProrrogacao)) {
                foreach ($arrPreObraProrrogacao as $preObraProrrogacao) {
                    if ($preObraProrrogacao['popstatus'] == 'A') { # VErifica se existe prorrogacao ativa
                        $prorrogacaoAtiva++;
                        $popdataprazoaprovadoAtivo = $preObraProrrogacao['popdataprazoaprovado'];
                    }
                    if ($preObraProrrogacao['popstatus'] == 'P') { # VErifica se existe prorrogacao Pendente
                        $prorrogacaoPendente++;
                    }
                }
            }

            if ($prorrogacaoAtiva > 0) {
                $fimVigenciaObra = formata_data($popdataprazoaprovadoAtivo);
            }elseif ($prorrogacaoPendente){
                $sql = "SELECT
                      to_char(popdataprazoaprovado, 'YYYY-MM-DD') as popdataprazoaprovado,
                      arqid,
                      popstatus
                    FROM obras.preobraprorrogacao
                    WHERE preid = ".$preobra['preid']." AND popstatus = 'I'
                    ORDER BY popid DESC LIMIT 1";

                $arrPreObraProrrogacaoInativo = $db->pegaLinha($sql);
                if ($arrPreObraProrrogacaoInativo) {
                    $fimVigenciaObra = formata_data($arrPreObraProrrogacaoInativo['popdataprazoaprovado']);
                } else {
                    $fimVigenciaObra = formata_data($dataPrimeiroPagamento[0]['prazo']);
                    if( $dataPrimeiroPagamento[0]['prazo'] ) $prazoProrrogar = $db->pegaUm("SELECT CAST('{$dataPrimeiroPagamento[0]['prazo']}' AS date) - CAST(NOW() AS date) ");
                }
            }else{
                $fimVigenciaObra = formata_data($dataPrimeiroPagamento[0]['prazo']);
                if( $dataPrimeiroPagamento[0]['prazo'] ) $prazoProrrogar = $db->pegaUm("SELECT CAST('{$dataPrimeiroPagamento[0]['prazo']}' AS date) - CAST(NOW() AS date) ");
            }
        } else {
            $fimVigenciaObra = '-';
        }
        
        /* FIM - Trecho que preenche a coluna Fim da vigência */
     
        $btnProrrogaDiligencia = '';
        if( ( $podeProrrogar && $data_primeiro_pagamento && $preobra['esdid'] != WF_PAR_OBRA_AGUARDANDO_PRORROGACAO && $prazo < 91 ) || 
        	( $preobra['esdid'] == WF_PAR_OBRA_APROVADA && ( in_array( PAR_PERFIL_ADM_OBRAS,$perfil ) || in_array( PAR_PERFIL_SUPER_USUARIO,$perfil ) ) ) 
		){
        	if( $preobra['preblockreformulacao'] != 'S' ){
        		if($prazoProrrogar < 0 && ( in_array( PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil ) || in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil ) || in_array( PAR_PERFIL_PREFEITO,$perfil ) ) ){
        			$btnProrrogaDiligencia = "";
        		} else {
                            $dataAtualTime = strtotime(date('Y-m-d'));
                            $fimVigencia = strtotime(formata_data_sql($fimVigenciaObra));
                            if ($fimVigencia < $dataAtualTime) {
        			$btnProrrogaDiligencia = "<br/><button style=\"background-color: grey; border: solid #a3a3a3 1px; font-size:9px\" onclick=\"alert('Solicitação de prorrogação bloqueada pois a vigência já está expirada.')\" type=button id={$preobra['preid']}; >Solicitar Prorrogação<br/>da Vigência</button>";
                            } else {
        			$btnProrrogaDiligencia = "<br/><button type=button class=prorrogar id={$preobra['preid']} style=font-size:9px; >Solicitar Prorrogação<br/>da Vigência</button>";
                            }
        		}
        	}
        }
        // FIM prorrogação de prazo
        
        $nomeObra = "<span class=\"alterar span_link\" id=\"" . $preobra['preid'] . "-" . $preobra['preano'] . "-" . $preobra['sbaid'] . "\">" . $preobra['predescricao'] . "</span>$btnProrrogaDiligencia";
      
        if(	$preobra['esdid'] == WF_PAR_EM_CADASTRAMENTO || 
	        $preobra['esdid'] == WF_PAR_EM_DILIGENCIA || 
	        $preobra['esdid'] == WF_PAR_OBRA_EM_REFORMULACAO || 
	        $preobra['esdid'] == WF_PAR_OBRA_APROVACAO_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL || 
	        $preobra['esdid'] == WF_PAR_OBRA_EM_REFORMULACAO_MI_CONVENCIONAL || 
	        $preobra['esdid'] == WF_PAR_OBRA_AGUARDANDO_PRORROGACAO || 
	        $preobra['esdid'] == WF_PAR_OBRA_APROVADA || 
	        $preobra['esdid'] == WF_PAR_OBRA_CANCELADA || 
	        $preobra['esdid'] == WF_PAR_OBRA_INDEFERIDA || 
	        $preobra['esdid'] == WF_PAR_OBRA_ARQUIVADA){
            
            $situacaoObra = $preobra['esddsc'];
            
            if($preobra['esdid'] == WF_PAR_EM_DILIGENCIA) {
                $docid = prePegarDocid($preobra['preid']);
                $sql = "
                    SELECT 
						TO_CHAR(MAX(hst.htddata),'DD/MM/YYYY')
					FROM 
						workflow.historicodocumento hst
					INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
					WHERE 
						docid = {$docid} 
						AND aed.esdiddestino = 328
                ";
                
                $dataInicial = $db->pegaUm($sql);
        
                $diligencia = $db->pegaUm("SELECT dioqtddia FROM par.diligenciaobra WHERE dioativo = true and preid = ".$preobra['preid']);
        
                if($diligencia) {
                    $dias = $diligencia . ' DAYS';
                    $sql = "
                        SELECT
                            TO_CHAR(CAST(MAX(hst.htddata) AS date) + INTERVAL '" . $dias . "','DD/MM/YYYY') AS data
    					FROM 
    					   workflow.historicodocumento hst
					INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
					WHERE 
						docid = {$docid} 
						AND aed.esdiddestino = 328;
    				";
                } else {
                    $sql = "
                        SELECT
                            TO_CHAR(CAST(MAX(hst.htddata) AS date) + INTERVAL '30 DAYS','DD/MM/YYYY') AS data
                        FROM 
                            workflow.historicodocumento hst
					INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
					WHERE 
						docid = {$docid} 
						AND aed.esdiddestino = 328;
                    ";
                }
                
                $dataFinal = $db->pegaUm($sql);
        
                $situacaoObra .= "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: ".$dataFinal.")";
            }
        } else {
                $situacaoObra = 'Em Análise';
        }
        

        $empenhoobra = empenhosPorObras($db, $preobra['preid']);

        if($empenhoobra) {
            if(is_array($empenhoobra)) {
                $empenhoProcessoObra = '';
                foreach ($empenhoobra as $value) {
                    $empenhoProcessoObra .= "{$value['ne']}<br />(R$".number_format($value['saldo'],2,",",".").")<br /> <br />";
                }
            } else {
                $empenhoProcessoObra = "Não gerado";
            }
        } else {
            $empenhoProcessoObra = "Não gerado";
        }
        
        $sql = "
            SELECT 
                pro.proagencia, 
                pro.probanco, 
                pago.popvalorpagamento, 
                pag.pagnumeroob, 
                TO_CHAR(pag.pagdatapagamento,'dd/mm/YYYY') AS pagdatapagamento
    		FROM par.empenhoobrapar emo
                INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A'
                INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                INNER JOIN par.pagamento pag ON pag.empid = emo.empid
                INNER JOIN par.pagamentoobrapar pago ON pago.pagid = pag.pagid AND pago.preid = emo.preid
    		WHERE 
                emo.preid = '" . $preobra['preid'] . "' 
                AND pag.pagstatus='A';
        ";
        	
        $pagamentoobra = $db->pegaLinha($sql);
        	
        if($pagamentoobra) {
            $pagamentoObra = "Pago.&nbsp;Valor pagamento(R$): ".number_format($pagamentoobra['popvalorpagamento'],2,",",".").".&nbsp; Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob'].".&nbsp; Data do pagamento: ".$pagamentoobra['pagdatapagamento'].". &nbsp; Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia'].".";
        } else {
            $pagamentoObra = "Não pago";
        }
            
        $sql = "SELECT
                   ( CASE WHEN ( po.dfisaldoconta + po.dfisaldofundo + po.dfisaldopoupanca + po.dfisaldordbcdb ) > 0 THEN
                        '<span class=\"processoDetalhes processo_detalhe\" >'
                    ELSE
                        '<span class=\"processo_detalhe\" >'
                    END
                ) ||
    			to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
    		FROM
    			par.processoobrasparcomposicao ppc
    		INNER JOIN par.processoobraspar 			poc ON poc.proid = ppc.proid and poc.prostatus = 'A'
    		LEFT  JOIN painel.dadosfinanceirosconvenios po 	ON poc.pronumeroprocesso = po.dfiprocesso
    		WHERE
    			preid = ".$preobra['preid'];
        
        $processo = $db->pegaUm($sql);
        $processoObra = ($processo ? $processo : '<center>--</center>');
        
       // dbg($empenhoProcessoObra);
        $arrResultado[$k]['acao'] = $acao;
        $arrResultado[$k]['preid'] = $preobra['preid'];
        $arrResultado[$k]['obrid'] = $preobra['obrid'];
        $arrResultado[$k]['nomeObra'] = $nomeObra;
        $arrResultado[$k]['tipoObra'] = $preobra['ptodescricao'];
        $arrResultado[$k]['situacaoObra'] = $situacaoObra;
        $arrResultado[$k]['empenhoProcessoObra'] = $empenhoProcessoObra;
        $arrResultado[$k]['pagamentoObra'] = $pagamentoObra;
        $arrResultado[$k]['fimVigenciaObra'] = $fimVigenciaObra;
        $arrResultado[$k]['prazoProrrogar'] = $prazoProrrogar;
        $arrResultado[$k]['subarcaoObra'] = $preobra['sbadsc'];
        $arrResultado[$k]['processoObra'] = $processoObra;
        
    }
}
$cabecalho = array(
    "Ação",
	"Preid",
	"Obrid",
    "Nome da obra",
    "Tipo de obra",
    "Situação",
    "Empenhos",
    "Pagamento",
    "Fim da vigência",
    "Subação",
    "Processo",
);



//// --------------------------------------- PAC 2 --------------------------------------- ////
// Lista de obras PAC


if ($_SESSION['par']['muncod'] == '5300108' || $_SESSION['par']['estuf']=='DF'){
	$ptoclassificacaoobra = array("'Q'", "'C'", "'P'");
	$muncodpar = '5300108';
	$esfera = "AND pre.preesfera in ('E' , 'M')";
	$filtroMuncodOREstuf = "(pre.estufpar = 'DF' OR pre.muncodpar  = '5300108')";
}
else if($_SESSION['par']['muncod']) {
    $muncodpar = " = '" . $_SESSION['par']['muncod'] . "'";
    $esfera = "AND pre.preesfera = 'M' ";
    $ptoclassificacaoobra = array("'Q'", "'C'", "'P'");
    $filtroMuncodOREstuf = "pre.muncodpar  = '{$_SESSION['par']['muncod']}'";
} else {
    $muncodpar = "IS NULL";
    $esfera = "AND pre.preesfera = 'E' ";
    $ptoclassificacaoobra = array("'Q'", "'C'");
    $filtroMuncodOREstuf = "pre.estufpar = '{$_SESSION['par']['estuf']}'";
}

$sqlObrasPac = "
    SELECT
        pre.preid as preid,
		(SELECT obrid FROM obras2.obras WHERE obrid = pre.obrid AND obridpai IS NULL LIMIT 1) as obrid,
        pre.docid,
        pre.presistema,
        pre.estuf,
        pre.muncod,
        pre.predtinclusao,
        pre.predescricao,
        pre.preano,
        pre.pretipofundacao,
        esd.esdid,
        esd.esddsc,
        pto.ptodescricao,
        pre.preprioridade,
        pto.ptoclassificacaoobra,
		preblockreformulacao,
        esd.esddsc || 
		CASE WHEN esd.esdid IN (" . WF_TIPO_EM_CORRECAO . ") THEN
				'<label style=\"color: red\"> (Inicio: ' || 
				( SELECT to_char(max(hd.htddata),'DD/MM/YYYY')
				FROM workflow.historicodocumento hd 
				WHERE docid = pre.docid AND aedid IN (2022,516,617,1815) )
				 || ' Prazo Final: ' || 
				( 
				SELECT
					to_char(CAST(max(hd.htddata) as date) + COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = pre.preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo ), 30) ,'DD/MM/YYYY') AS data
				FROM workflow.historicodocumento hd
				WHERE
					docid = pre.docid
					AND aedid IN (2022,516,617,1815) )
				 || ' )'
			ELSE ''
		END as estado1
    FROM 
        obras.preobra pre
	--LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid AND obridpai IS NULL
	LEFT  JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
	INNER JOIN workflow.documento doc ON doc.docid = pre.docid
	INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    WHERE
        $filtroMuncodOREstuf
        --AND pto.ptoesfera IN ('" . $esfera . "','T') 
        {$esfera} 
        --AND pto.ptoclassificacaoobra IN (" . implode(',', $ptoclassificacaoobra) . ") 
        AND pre.presistema = '23'
        AND pre.prestatus = 'A'
        AND pre.tooid = 1
        AND pre.preidpai IS NULL
    ORDER BY 
        pre.preprioridade    
";

$arPreObrasComCobertura = $db->carregar($sqlObrasPac);

$arrResultadoPAC = array();

if(is_array($arPreObrasComCobertura)) {
    foreach ($arPreObrasComCobertura as $key => $preobra) 
    {
        $docid = prePegarDocid($preobra['preid']);
        $esdid = prePegarEstadoAtual($docid);
        
        // ação
        if($esdid == WF_TIPO_EM_CADASTRAMENTO) {
            $acao = "<img src=\"../imagens/alterar.gif\" class=\"alterarpac\" style=\"cursor:pointer\" id=\"" . $preobra['preid'] . "\" />";
        } else {
            $acao = "<img src=\"../imagens/alterar.gif\" class=\"alterarpac\" style=\"cursor:pointer\" id=\"" . $preobra['preid'] . "\" />";
        }
        
        #Adiciona Icone de historico
        $acao.="<img src=\"../imagens/schedule.png\" title=\"Histórico de Prorrogação de Prazo\" style=\"cursor:pointer\" class=\"historicoProrrogacao\" id=\"{$preobra['preid']}\" />";
        
        // INICIO prorrogação de prazo
        $sql = "SELECT
					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
				FROM
					par.pagamentoobra po
				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				WHERE
					po.preid = {$preobra['preid']}";
        
        $data_primeiro_pagamento = $db->pegaUm( $sql );
        
        $sql = "SELECT
					coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
				FROM
					par.pagamentoobra po
				INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
        		WHERE
        			po.preid = {$preobra['preid']}";
        
        $prazo = $db->pegaUm($sql);
        
        /* Trecho que preenche a coluna Fim da vigência */
        $sql = "
            SELECT
                MIN(pag.pagdatapagamentosiafi) AS data_primeiro_pagamento,
                MIN(pag.pagdatapagamentosiafi) + 720 as prazo
            FROM
                par.pagamentoobra po
            INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
            WHERE
                po.preid = " . $preobra['preid'];
         
        $dataPrimeiroPagamento = $db->carregar($sql);

        $fimVigenciaObra = '';
        $prazoProrrogar = '';
        if( !empty($dataPrimeiroPagamento[0]['data_primeiro_pagamento']) ) {
            $sql = "SELECT
                      to_char(popdataprazoaprovado, 'YYYY-MM-DD') as popdataprazoaprovado,
                      arqid,
                      popstatus
                    FROM obras.preobraprorrogacao
                    WHERE preid = ".$preobra['preid']."
                    ORDER BY popid DESC";

            $arrPreObraProrrogacao = $db->carregar($sql);
            $prorrogacaoAtiva = 0;
            $popdataprazoaprovadoAtivo = '';
            $prorrogacaoPendente = 0;

            if(!empty($arrPreObraProrrogacao)) {
                foreach ($arrPreObraProrrogacao as $preObraProrrogacao) {
                    if ($preObraProrrogacao['popstatus'] == 'A') { # VErifica se existe prorrogacao ativa
                        $prorrogacaoAtiva++;
                        $popdataprazoaprovadoAtivo = $preObraProrrogacao['popdataprazoaprovado'];
                    }
                    if ($preObraProrrogacao['popstatus'] == 'P') { # VErifica se existe prorrogacao Pendente
                        $prorrogacaoPendente++;
                    }
                }
            }

            if ($prorrogacaoAtiva > 0) {
                $fimVigenciaObra = formata_data($popdataprazoaprovadoAtivo);
            }elseif ($prorrogacaoPendente){
                $sql = "SELECT
                      to_char(popdataprazoaprovado, 'YYYY-MM-DD') as popdataprazoaprovado,
                      arqid,
                      popstatus
                    FROM obras.preobraprorrogacao
                    WHERE preid = ".$preobra['preid']." AND popstatus = 'I'
                    ORDER BY popid DESC LIMIT 1";

                $arrPreObraProrrogacaoInativo = $db->pegaLinha($sql);
                if ($arrPreObraProrrogacaoInativo) {
                    $fimVigenciaObra = formata_data($arrPreObraProrrogacaoInativo['popdataprazoaprovado']);
                } else {
                    $fimVigenciaObra = formata_data($dataPrimeiroPagamento[0]['prazo']);
                    if( $dataPrimeiroPagamento[0]['prazo'] ) $prazoProrrogar = $db->pegaUm("SELECT CAST('{$dataPrimeiroPagamento[0]['prazo']}' AS date) - CAST(NOW() AS date) ");
                }
            }else{
                $fimVigenciaObra = formata_data($dataPrimeiroPagamento[0]['prazo']);
                if( $dataPrimeiroPagamento[0]['prazo'] ) $prazoProrrogar = $db->pegaUm("SELECT CAST('{$dataPrimeiroPagamento[0]['prazo']}' AS date) - CAST(NOW() AS date) ");
            }
        } else {
            $fimVigenciaObra = '-';
        }
       
        /* FIM - Trecho que preenche a coluna Fim da vigência */
        /* Trecho que preenche a coluna Prorrogar (dias) */
    	/* $sqlPrazo= "
    	    SELECT
    	       coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
    	   FROM
    	       par.pagamentoobra po
    	       INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
    	       LEFT JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
    	   WHERE
    	       po.preid = {$preobra['preid']};
        ";
    		
    	$prazoProrrogar = $db->pegaUm($sqlPrazo); */
        /* FIM - Trecho que preenche a coluna Prorrogar (dias) */
    	         
        $btnProrrogaDiligencia = '';
        $prazoProrrogar = '';
        if( ( $podeProrrogar && $data_primeiro_pagamento && $preobra['esdid'] != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO && $prazo < 91 ) || 
        	( $preobra['esdid'] == OBR_ESDID_CONCLUIDA && ( in_array( PAR_PERFIL_ADM_OBRAS,$perfil ) || in_array( PAR_PERFIL_SUPER_USUARIO,$perfil ) ) ) ){
        	if( $preobra['preblockreformulacao'] != 'S' ){
        		if($prazoProrrogar < 0 && ( in_array( PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil ) || in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil ) || in_array( PAR_PERFIL_PREFEITO,$perfil ) ) ){
        			$btnProrrogaDiligencia = "";
        		} else {
                            $dataAtualTime = strtotime(date('Y-m-d'));
                            $fimVigencia = strtotime(formata_data_sql($fimVigenciaObra));
                            if ($fimVigencia < $dataAtualTime) {
        			$btnProrrogaDiligencia = "<br/><button style=\"background-color: grey; border: solid #a3a3a3 1px; font-size:9px\" onclick=\"alert('Solicitação de prorrogação bloqueada pois a vigência já está expirada.')\" type=button id={$preobra['preid']}; >Solicitar Prorrogação<br/>da Vigência</button>";
                            } else {
        			$btnProrrogaDiligencia = "<br/><button type=button class=prorrogar id={$preobra['preid']} style=font-size:9px; >Solicitar Prorrogação<br/>da Vigência</button>";
                            }
        		}
        	}
        }
        // FIM prorrogação de prazo
        
        // nome da obra
        $nomeObra = "<span class=\"alterarpac span_link\" id=\"" . $preobra['preid'] . "\">" . $preobra['predescricao'] . "</span>$btnProrrogaDiligencia";
        
        $situacaoObra = $preobra['estado1'];
        /* FIM - Trecho que preenche a coluna Situação */
        /* Trecho que preenche a coluna termo */
        $sql = "
            SELECT 
                terassinado, 
                TO_CHAR(terdatainclusao,'dd/mm/YYYY') AS terdatainclusao, 
                tcp.arqid 
            FROM 
                par.termoobra teo
    			INNER JOIN par.termocompromissopac tcp ON teo.terid = tcp.terid
    		WHERE 
                teo.preid = '" . $preobra['preid'] . "';
        ";
        	
        $termoobra = $db->pegaLinha($sql);
        	
        if($termoobra) {
            if($termoobra['terassinado']=="f") {
                $termoObra =  "Gerado (".($termoobra['terdatainclusao']).")";
            } elseif($termoobra['terassinado']=="t") {
                $termoObra =  "Assinado";
            }
            
        } else {
            $termoObra = "Não gerado";
        }
        	
        /* FIM - Trecho que preenche a coluna termo */
        
        /* Trecho que preenche a coluna empenho */
        $empenhoobra = empenhosPorObras($db, $preobra['preid']);

    	if($empenhoobra) {
            if(is_array($empenhoobra)) {
                $empenhoProcessoObra = '';
                foreach ($empenhoobra as $value) {
                    $empenhoProcessoObra .= "{$value['ne']}<br />(R$".number_format($value['saldo'],2,",",".").")<br /> <br />";
                }
            } else {
                $empenhoProcessoObra = "Não gerado";
            }
        } else {
            $empenhoProcessoObra = "Não gerado";
        }

        /* FIM - Trecho que preenche a coluna empenho */
        /* Trecho que preenche a coluna pagamento */
        $sql = "
            SELECT 
                pro.proagencia, 
                pro.probanco, 
                pago.pobvalorpagamento, 
                pag.pagnumeroob, 
                TO_CHAR(pag.pagdatapagamento,'dd/mm/YYYY') AS pagdatapagamento
            FROM par.empenhoobra emo
            INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A'
            INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
            INNER JOIN par.pagamento pag ON pag.empid = emo.empid AND pag.pagstatus = 'A'
            INNER JOIN par.pagamentoobra pago ON pago.pagid = pag.pagid AND pago.preid = emo.preid
            WHERE emo.preid = '" . $preobra['preid'] . "' 
                AND pag.pagstatus='A'";
        	
        $pagamentoobra = $db->pegaLinha($sql);
        	
        if($pagamentoobra) {
            $pagamentoObra = "Pago<br>
    			  Valor pagamento(R$): ".number_format($pagamentoobra['pobvalorpagamento'],2,",",".")."<br>
    			  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br>
    			  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
    			  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
            ";
        } else {
            $pagamentoObra = "Não pago";
        }
        /* FIM - Trecho que preenche a coluna pagamento */
        
        /* Trecho que preenche a coluna Processo */
    	$sql = "SELECT DISTINCT
    			(
    			    CASE WHEN ( po.dfisaldoconta + po.dfisaldofundo + po.dfisaldopoupanca + po.dfisaldordbcdb ) > 0 THEN
    				'<span class=\"processoDetalhes processo_detalhe\" >'
    			    ELSE
    				'<span class=\"processo_detalhe\" >'
    			    END
    			) || 
    			to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
    		FROM
    			par.processoobraspaccomposicao ppc
    		INNER JOIN par.processoobra 				poc ON poc.proid = ppc.proid AND poc.prostatus = 'A'
    		LEFT  JOIN painel.dadosfinanceirosconvenios po 	ON poc.pronumeroprocesso = po.dfiprocesso
    		WHERE
    			ppc.pocstatus = 'A' AND
    			preid = ".$preobra['preid'];
    		
    	$processo = $db->pegaUm($sql);
    	
    	$processoObra = ($processo ? $processo : '<center>--</center>');
        /* FIM - Trecho que preenche a coluna processo*/
    	
    	if($preobra['ptoclassificacaoobra'] == 'C') {
    	    $tipoPrograma = "Cobertura";
    	} else if($preobra['ptoclassificacaoobra'] == 'Q') {
    	    $tipoPrograma = "Quadra";
    	} else {
    	    $tipoPrograma = 'Pró Infância';
    	}
        
        $arrResultadoPAC[$key]['acao'] = $acao;
        $arrResultadoPAC[$key]['preid'] = $preobra['preid'];
        $arrResultadoPAC[$key]['obrid'] = $preobra['obrid'];
        $arrResultadoPAC[$key]['nomeObra'] = $nomeObra;
        $arrResultadoPAC[$key]['tipoObra'] = $preobra['ptodescricao'];
        $arrResultadoPAC[$key]['situacaoObra'] = $situacaoObra;
        $arrResultadoPAC[$key]['termoObra'] = $termoObra;
        $arrResultadoPAC[$key]['empenhoProcessoObra'] = $empenhoProcessoObra;
        $arrResultadoPAC[$key]['pagamentoObra'] = $pagamentoObra;
        $arrResultadoPAC[$key]['fimVigenciaObra'] = $fimVigenciaObra;
        $arrResultadoPAC[$key]['prazoProrrogar'] = $prazoProrrogar;
        $arrResultadoPAC[$key]['processoObra'] = $processoObra;
        $arrResultadoPAC[$key]['tipoPrograma'] = $tipoPrograma;
    }
}

$cabecalhoPAC = array(
    "Ação",
    "Preid",
    "Obrid",
    "Nome da obra",
    "Tipo de obra",
    "Situação",
    "Termo",
    "Empenhos",
    "Pagamento",
    "Fim da vigência",
    "Prorrogar (dias)",
    "Processo",
    "Programa",
);

?>
<br />
<style>
.span_link{
	color: blue;
	text-decoration: underline;
	cursor:pointer;
}

</style>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){

    jQuery('body').on('click', '.prorrogar',function(){

            return window.open('par.php?modulo=principal/popupProrrogacaoPAC&acao=A&preid='+this.id, 
                                               'Prorrogação', 
                                               "height=600,width=800,scrollbars=yes,top=50,left=200" ).focus();		

    });

    jQuery('body').on('click','.consultaprorrogacao',function(){

            return window.open('par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&arqidProrrogacao='+this.id, 
                                               'Consulta prorrogação', 
                                               "height=200,width=200,scrollbars=yes,top=50,left=200" ).focus();		

    });
	
    jQuery('body').on('click','.anexos',function(){
    	var arDados = this.id.split("_");		
    	return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+arDados[0]+'&preid='+arDados[1]+'', 
		   'ProInfancia',
		   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
    });
    
    jQuery('body').on('click','.consultaprorrogacao',function(){
    	return window.open('par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&arqidProrrogacao='+this.id, 
    	   'Consulta prorrogação', 
    	   "height=200,width=200,scrollbars=yes,top=50,left=200" ).focus();		
    });
    
    jQuery('body').on('click','.alterarpac',function(){
    	return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+this.id, 
    	   'ProInfancia', 
    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
    });
    
    jQuery('body').on('click','.voltar',function(){	
    	document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';	
    });
    
    jQuery('body').on('click','.alterar',function(){
    	var dados = this.id.split("-");
    	return window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+dados[0]+'&ano='+dados[1]+'&sbaid='+dados[2], 
    	   'ProInfancia', 
    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
    });
    
    jQuery('body').on('click','.historicoProrrogacao',function(){
    	var preid = this.id;
    	return window.open('par.php?modulo=principal/popupHistoricoProrrogacaoPrazo&acao=A&preid='+preid, 
    	   'Prorrogação de Prazo', 
    	   "height=400,width=900,scrollbars=yes,top=50,left=200" ).focus();	
    });
	
});

function focamensagem(){
    document.getElementById('msg').focus();
}

</script>
<div class="tabela" style="margin-left: 7px; margin-right: 5px; width: 97.5% !important;">
    <span class="TituloTela" style="text-align: center;width: 100%">OBRAS - <?= $descricao;?></span>
    <span style="float: right; color: blue; font-size: 12px;">
        <a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
        <a href="par.php?modulo=principal/orgaoEducacao&acao=A">Dados da Unidade |</a>
            <label style="color:black; text-align:center"><b>Lista de Obras</b> </label>
        <a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
    </span>
</div>
<div style="clear: both;"></div>
<?php 

include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';

$param['nome'] 				= 'listaPar';
$param['titulo'] 			= 'Listas de Obras PAR';
$param['descricaoBusca']	= 'PAR';
$param['instrucaoBusca']	= '(Nome da Obra, Tipo da Obra ou Situação)';
$param['nomeXLS']			= "Lista de Obras $descricao-$dataAtual";
$param['numeroColunasXLS']	= '1,2,3,4,5,6,7,8';
$param['arrDados']			= $arrResultado;		
$param['arrCabecalho']		= Array( 'Ação', 'Preid', 'Obrid', 'Nome da obra', 'Tipo de obra', 'Situação', 'Empenho processo', 'Pagamento', 'Fim da vigência', 'Prorrogar(dias)', 'Subação', 'Processo' );
$param['arrSemSpan']		= Array( 'N', '', '', 'N', '', '', '', '', 'N' );
$param['arrCampoNumerico']	= Array();

$lista = new listaDT();
echo $lista->lista($param);

$param['nome'] 				= 'listaPac';
$param['titulo'] 			= 'Listas de Obras PAC';
$param['descricaoBusca']	= 'PAC';
$param['instrucaoBusca']	= '(Nome da Obra, Tipo da Obra ou Situação)';
$param['nomeXLS']			= "Lista de Obras $descricao-$dataAtual";
$param['numeroColunasXLS']	= '1,2,3,4,5,6,7,8,9';
$param['arrDados']			= $arrResultadoPAC;
$param['arrCabecalho']		= Array( 'Ação','Preid', 'Obrid', 'Nome da obra', 'Tipo de obra', 'Situação', 'Termo', 'Empenho processo', 'Pagamento', 'Fim da vigência', 'Prorrogar(dias)', 'Processo', 'Programa' );
$param['arrSemSpan']		= Array( 'N', '', '', 'N', '', '', '', '', '', 'N' );
$param['arrCampoNumerico']	= Array();

echo '<br>'.$lista->lista($param);

// Função que retorna os empenhos por obras. 
function empenhosPorObras($db, $preid)
{
	$sql = "SELECT 	ne, 
					saldo 
			FROM par.v_saldo_obra_por_empenho    
			WHERE 
				preid = $preid
				AND saldo > 0";
	
	$dadosEmpenhoObras = $db->carregar($sql);
	
	return $dadosEmpenhoObras;
}
?>
<div id="divDebug"></div>