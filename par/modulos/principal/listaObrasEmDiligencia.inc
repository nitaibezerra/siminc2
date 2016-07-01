<?php

$obPreObraControle 	  	= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();

$muncod = $_SESSION['par']['muncod']; 
$nrAdesao = $obPreObraControle->verificaPrazoExpiraAdesao($muncod);
$booQtdObrasEstado = true;
if($_REQUEST['tipo']){
	$booQtdObrasEstado = $obPreObraControle->verificaQtdObrasEstado(Array('estuf'=>$_SESSION['par']['estuf'],'tipo'=>$_REQUEST['tipo']));
}
$boAdesao = $nrAdesao > 0 ? true : false;

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';

if( !$_SESSION['par']['inuid'] ){
	echo "<script>
			alert('Faltam dados na sessão!');
			history.back(-1)';
		  </script>";
	die;
}

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

$sql = "
    SELECT DISTINCT
		pre.preid, 
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
		sub.sbaid
	FROM 
		obras.preobra pre
		INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
		LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
		LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		INNER JOIN par.subacaoobra sbo ON sbo.preid = pre.preid
        INNER JOIN par.subacao sub on sub.sbaid = sbo.sbaid AND sub.sbastatus = 'A' 
        INNER JOIN par.acao aca ON aca.aciid = sub.aciid AND aca.acistatus = 'A'
        INNER JOIN par.pontuacao pon ON pon.ptoid = aca.ptoid AND pon.ptostatus = 'A'
        INNER JOIN par.instrumentounidade inu ON inu.inuid = pon.inuid AND ( pre.estuf = inu.estuf OR pre.muncod = inu.muncod )
	WHERE 
		".implode(' AND ',$arrWhere)."
        AND (esd.esdid = " . WF_PAR_EM_DILIGENCIA . " OR esd.esdid = " . WF_TIPO_EM_CORRECAO . ")
	ORDER BY 
	    pre.predescricao, 
	    ptodescricao;
";

$arPreObrasSemCobertura = $db->carregar($sql);
$arPreObrasSemCobertura = $arPreObrasSemCobertura ? $arPreObrasSemCobertura : array();

// verifica se é um estado ou um municipio
if(empty($_SESSION['par']['muncod']) && !empty($_SESSION['par']['estuf'])) {
    $descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['estuf'], 1);
} else {
    $descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);
}

$arrResultado = array();

if(is_array($arPreObrasSemCobertura)) {
    foreach($arPreObrasSemCobertura as $k => $preobra) 
    {
        
        if($esdid == WF_PAR_EM_CADASTRAMENTO) {
    		$acao = "<img src=\"../imagens/alterar.gif\" class=\"alterar\" style=\"cursor:pointer\" id=\"" . $preobra['preid'] . "-" . $preobra['preano'] . "-" . $preobra['sbaid'] . "\" />";
        } else {
    		$acao = "<img src=\"../imagens/alterar.gif\" class=\"alterar\" style=\"cursor:pointer\" id=\"" . $preobra['preid'] . "-" . $preobra['preano'] . "-" . $preobra['sbaid'] . "\" />";
        }
        
        $acao .= "<img src=\"../imagens/consultar.gif\" title=\"abrir\" style=\"cursor:pointer;\" onclick=\"window.open('par.php?modulo=principal/subacao&acao=A&sbaid=" . $preobra['sbaid'] . "', 'modelo', 'height=600,width=800,scrollbars=yes,top=0,left=0' )\">";
        
        $nomeObra = "<a class=\"alterar\" id=\"" . $preobra['preid'] . "-" . $preobra['preano'] . "-" . $preobra['sbaid'] . "\">" . $preobra['predescricao'] . "</a>";
        
        /* Trecho que preenche a coluna Situação */
        if($preobra['esdid'] == WF_PAR_EM_CADASTRAMENTO || $preobra['esdid'] == WF_PAR_EM_DILIGENCIA || $preobra['esdid'] == WF_PAR_OBRA_EM_REFORMULACAO || $preobra['esdid'] == WF_PAR_OBRA_APROVADA || $preobra['esdid'] == WF_PAR_OBRA_ARQUIVADA){
            
            $situacaoObra = $preobra['esddsc'];
            
            if($preobra['esdid'] == WF_PAR_EM_DILIGENCIA) {
                $docid = prePegarDocid($preobra['preid']);
                $sql = "
                    SELECT 
                        TO_CHAR(MAX(hd.htddata),'DD/MM/YYYY')
                    FROM 
                        workflow.historicodocumento hd
                    WHERE 
                        docid = {$docid} 
                        AND aedid = 872;
                ";
                
                $dataInicial = $db->pegaUm($sql);
        
                $diligencia = $db->pegaUm("SELECT dioqtddia FROM par.diligenciaobra WHERE dioativo = true and preid = ".$preobra['preid']);
        
                if($diligencia) {
                    $dias = $diligencia . ' DAYS';
                    $sql = "
                        SELECT
                            TO_CHAR(CAST(MAX(hd.htddata) AS date) + INTERVAL '" . $dias . "','DD/MM/YYYY') AS data
    					FROM 
    					   workflow.historicodocumento hd
    					WHERE
        					docid = {$docid}
    						AND aedid = 872;
    				";
                } else {
                    $sql = "
                        SELECT
                            TO_CHAR(CAST(MAX(hd.htddata) AS date) + INTERVAL '30 DAYS','DD/MM/YYYY') AS data
                        FROM 
                            workflow.historicodocumento hd
                        WHERE
                            docid = {$docid}
                            AND aedid = 872;
                    ";
                }
                
                $dataFinal = $db->pegaUm($sql);
        
                $situacaoObra .= "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: ".$dataFinal.")";
            }
        } else {
                $situacaoObra = 'Em Análise';
        }
        /* FIM - Trecho que preenche a coluna Situação */
        
        $sql = "
            SELECT 
                emp.empnumero, 
                vve.vrlempenhocancelado as empvalorempenho
    		FROM 
                par.empenhoobrapar emo
    			INNER JOIN par.empenho emp ON emo.empid = emp.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and eobstatus = 'A'
        		inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
    		WHERE 
                emo.preid = '" . $preobra['preid'] . "';
        ";
        	
        $empenhoobra = $db->pegaLinha($sql);
        	
        if($empenhoobra) {
            if($empenhoobra['empnumero']) {
                $empenhoProcessoObra = "Gerado (R$".number_format($empenhoobra['empvalorempenho'],2,",",".")." - ".$empenhoobra['empnumero'].")";
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
                pag.pagvalorparcela, 
                pag.pagnumeroob, 
                TO_CHAR(pag.pagdatapagamento,'dd/mm/YYYY') AS pagdatapagamento
    		FROM 
                par.empenhoobrapar emo
    			INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A'
    			INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
    			INNER JOIN par.pagamento pag ON pag.empid = emo.empid
    		WHERE 
                emo.preid = '" . $preobra['preid'] . "' 
                AND pag.pagstatus='A';
        ";
        	
        $pagamentoobra = $db->pegaLinha($sql);
        	
        if($pagamentoobra) {
            $pagamentoObra = "Pago<br>
    			  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br>
    			  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br>
    			  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
    			  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
            ";
        } else {
            $pagamentoObra = "Não pago";
        }
        
        $sql = "SELECT
                   ( CASE WHEN (
                        REPLACE( REPLACE( REPLACE(po.ppasaldoconta, ',', ''), '.', ''), '-', '0')::FLOAT +
                        REPLACE( REPLACE( REPLACE(po.ppasaldofundos, ',', ''), '.', ''), '-', '0')::FLOAT +
                        REPLACE( REPLACE( REPLACE(po.ppasaldopoupanca, ',', ''), '.', ''), '-', '0')::FLOAT +
                        REPLACE( REPLACE( REPLACE(po.ppasaldordbcdb, ',', ''), '.', ''), '-', '0')::FLOAT
                    ) > 0 THEN
                        '<span class=\"processoDetalhes processo_detalhe\" >'
                    ELSE
                        '<span class=\"processo_detalhe\" >'
                    END
                ) ||
    			to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
    		FROM
    			par.processoobrasparcomposicao ppc
    		INNER JOIN par.processoobraspar poc ON poc.proid = ppc.proid and poc.prostatus = 'A'
    		INNER JOIN obras2.pagamentopac po 	ON poc.pronumeroprocesso = po.ppaprocesso
    		WHERE
    			preid = ".$preobra['preid'];
        $processo = $db->pegaUm($sql);
        $processoObra = ($processo ? $processo : '<center>--</center>');
        
        
        $arrResultado[$k]['acao'] = $acao;
        $arrResultado[$k]['nomeObra'] = $nomeObra;
        $arrResultado[$k]['tipoObra'] = $preobra['ptodescricao'];
        $arrResultado[$k]['situacaoObra'] = $situacaoObra;
        $arrResultado[$k]['empenhoProcessoObra'] = $empenhoProcessoObra;
        $arrResultado[$k]['pagamentoObra'] = $pagamentoObra;
        $arrResultado[$k]['subarcaoObra'] = $preobra['sbadsc'];
        $arrResultado[$k]['processoObra'] = $processoObra;
    }
}

$cabecalho = array(
    "Ação",
    "Nome da obra",
    "Tipo de obra",
    "Situação",
    "Empenho processo",
    "Pagamento",
    "Subação",
    "Processo",
);


//// --------------------------------------- PAC 2 --------------------------------------- ////
// Lista de obras PAC
if($_SESSION['par']['muncod']) {
    $muncodpar = " = '" . $_SESSION['par']['muncod'] . "'";
    $esfera = "M";
    $ptoclassificacaoobra = array("'Q'", "'C'", "'P'");
} else {
    $muncodpar = "IS NULL";
    $esfera = "E";
    $ptoclassificacaoobra = array("'Q'", "'C'");
}

$sqlObrasPac = "
    SELECT
        pre.preid,
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
        INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
        LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
        LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    WHERE
        pre.estufpar = '" . $_SESSION['par']['estuf'] . "'
        AND pre.muncodpar " . $muncodpar . "
        AND pto.ptoesfera IN ('" . $esfera . "','T') 
        AND pre.preesfera = '" . $esfera . "' 
        AND pto.ptoclassificacaoobra IN (" . implode(',', $ptoclassificacaoobra) . ") 
        AND pre.presistema = '23'
        AND pre.prestatus = 'A'
        AND pre.tooid = 1
        AND pre.preidpai IS NULL
        AND (esd.esdid = " . WF_PAR_EM_DILIGENCIA . " OR esd.esdid = " . WF_TIPO_EM_CORRECAO . ")
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
        
        // nome da obra
        $nomeObra = "<a class=\"alterarpac\" id=\"" . $preobra['preid'] . "\">" . $preobra['predescricao'] . "</a>";
        
        /* Trecho que preenche a coluna Situação */
        /* Retirado a pedido do Tiago Radunz no dia 22/07/2014 */
//         if($preobra['esdid'] == WF_TIPO_EM_CADASTRAMENTO || $preobra['esdid'] == WF_TIPO_OBRA_APROVADA || $preobra['esdid'] == WF_TIPO_OBRA_ARQUIVADA || $preobra['esdid'] == WF_TIPO_EM_REFORMULACAO || $preobra['esdid'] == WF_TIPO_EM_CORRECAO){
//             $situacaoObra = $preobra['esddsc'];
//             if($preobra['esdid'] == WF_TIPO_EM_CORRECAO){
//                 $docid = prePegarDocid($preobra['preid']);
                	
//                 $sql = "
//                     SELECT
//                         max(hd.htddata)
//                     FROM 
//                         workflow.historicodocumento hd
//                     WHERE
//                         docid = {$docid}
//                         AND aedid = 516";
                	
//                 $data = $db->pegaUm($sql);
                	
//                 $arData = explode(" ", $data);
        
//                 $dataFinal = calculaPrazoDiligencia( $preobra['preid'] );
//                 $arDataInicial = explode("-",$arData[0]);
//                 $dataInicial = $arDataInicial[2]."/".$arDataInicial[1]."/".$arDataInicial[0];
//                 $arDataFinal = explode("-",$dataFinal);
//                 $dataFinal = $arDataFinal[2]."/".$arDataFinal[1]."/".$arDataFinal[0];
                	
//                 $sql = "
//                     SELECT 
//                         dioqtddia, 
//                         diodata 
//                     FROM 
//                         par.diligenciaobra 
//                     WHERE 
//                         dioativo = 't' 
//                         AND preid = " . $preobra['preid'];
//     			$dados = $db->pegaLinha($sql);
        
//                 if(is_array($dados) && $dados['diodata'] && $dados['dioqtddia']) {
//                     $arData = explode('-', $dados['diodata']);
            
//                     $ano = $arData[0];
//                     $mes = $arData[1];
//                     $dia = $arData[2];
//                     $dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
//                     $dataFormatada = date('d/m/Y',$dataFinal);
//                     $dataInicial = $dia . "/" . $mes . "/" . $ano;
//                     $dataFinal = $dataFormatada;
//                 }
                	
//                 $situacaoObra .= "<label style=\"color: red\"> (Inicio: " . $dataInicial . ". Prazo Final: " . $dataFinal . ")";
//             }
//         } else {
//             $situacaoObra = 'Em Análise';
//         }

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
        $sql = "
            SELECT 
                emp.empnumero, 
                vve.vrlempenhocancelado as empvalorempenho 
            FROM 
                par.empenhoobra emo
    			INNER JOIN par.empenho emp ON emo.empid = emp.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and eobstatus = 'A'
        		inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
    	   WHERE 
                emo.preid = '" . $preobra['preid'] . "';
        ";
        	
        $empenhoobra = $db->pegaLinha($sql);
        	
        if($empenhoobra) {
            if($empenhoobra['empnumero']) {
                $empenhoProcessoObra = "Gerado (R$".number_format($empenhoobra['empvalorempenho'],2,",",".")." - ".$empenhoobra['empnumero'].")";
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
                pag.pagvalorparcela, 
                pag.pagnumeroob, 
                TO_CHAR(pag.pagdatapagamento,'dd/mm/YYYY') AS pagdatapagamento
    		FROM 
                par.empenhoobra emo
    			INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A'
    			INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso
    			INNER JOIN par.pagamento pag ON pag.empid = emo.empid
    		WHERE 
                emo.preid = '" . $preobra['preid'] . "' 
                AND pag.pagstatus='A';
        ";
        	
        $pagamentoobra = $db->pegaLinha($sql);
        	
        if($pagamentoobra) {
            $pagamentoObra = "Pago<br>
    			  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br>
    			  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br>
    			  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
    			  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
            ";
        } else {
            $pagamentoObra = "Não pago";
        }
        /* FIM - Trecho que preenche a coluna pagamento */
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
        
        if($dataPrimeiroPagamento[0]['data_primeiro_pagamento']){
            $sql = "
                SELECT 
                    popdataprazoaprovado, 
                    arqid 
                FROM 
                    obras.preobraprorrogacao 
                WHERE 
                    popstatus = 'A' 
                    AND popvalidacao = 't' 
                    AND preid = " . $preobra['preid'];
        
            $prorrogado = $db->pegaLinha($sql);
        
            if($prorrogado['popdataprazoaprovado']) {
                $fimVigenciaObra = formata_data($prorrogado['popdataprazoaprovado']);
    		} else {
    			$fimVigenciaObra = formata_data($dataPrimeiroPagamento[0]['prazo']);
    		}
    	}
        /* FIM - Trecho que preenche a coluna Fim da vigência */
        /* Trecho que preenche a coluna Prorrogar (dias) */
    	$sqlPrazo= "
    	    SELECT
    	       coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
    	   	FROM
    	       par.pagamentoobra po
    	 	INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
    		LEFT JOIN obras.preobraprorrogacao 	pop ON pop.preid = po.preid AND popstatus = 'A'
    	   	WHERE
    	       po.preid = {$preobra['preid']};";
    		
    	$prazoProrrogar = $db->pegaUm($sqlPrazo);
        /* FIM - Trecho que preenche a coluna Prorrogar (dias) */
        /* Trecho que preenche a coluna Processo */
    	$sql = "select distinct
    			(
    			    CASE WHEN (
    				REPLACE( REPLACE( REPLACE(po.ppasaldoconta, ',', ''), '.', ''), '-', '0')::FLOAT +
    				REPLACE( REPLACE( REPLACE(po.ppasaldofundos, ',', ''), '.', ''), '-', '0')::FLOAT +
    				REPLACE( REPLACE( REPLACE(po.ppasaldopoupanca, ',', ''), '.', ''), '-', '0')::FLOAT +
    				REPLACE( REPLACE( REPLACE(po.ppasaldordbcdb, ',', ''), '.', ''), '-', '0')::FLOAT
    			    ) > 0 THEN
    				'<span class=\"processoDetalhes processo_detalhe\" >'
    			    ELSE
    				'<span class=\"processo_detalhe\" >'
    			    END
    			) ||
    			to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
    		from
    			par.processoobraspaccomposicao ppc
    		inner join par.processoobra poc on poc.proid = ppc.proid
    		inner join obras2.pagamentopac po on poc.pronumeroprocesso = po.ppaprocesso
    		where
    			ppc.pocstatus = 'A' and
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
    "Nome da obra",
    "Tipo de obra",
    "Situação",
    "Termo",
    "Empenho processo",
    "Pagamento",
    "Fim da vigência",
    "Prorrogar (dias)",
    "Processo",
    "Programa",
);

// ver($arrResultadoPAC, d);

?>
<br />
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
</script>
<script type="text/javascript"><!--
jQuery(document).ready(function(){
    jQuery('.anexos').live('click',function(){
    	var arDados = this.id.split("_");		
    	return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+arDados[0]+'&preid='+arDados[1]+'', 
		   'ProInfancia',
		   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
    });
    
    jQuery('.consultaprorrogacao').live('click',function(){
    	return window.open('par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&arqidProrrogacao='+this.id, 
    	   'Consulta prorrogação', 
    	   "height=200,width=200,scrollbars=yes,top=50,left=200" ).focus();		
    });
    
    jQuery('.alterarpac').live('click',function(){
    	return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+this.id, 
    	   'ProInfancia', 
    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
    });
    	
    jQuery('.voltar').live('click',function(){
    	document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';	
    });
    
    jQuery('.alterar').live('click',function(){
    	var dados = this.id.split("-");
    	return window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+dados[0]+'&ano='+dados[1]+'&sbaid='+dados[2], 
    	   'ProInfancia', 
    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
    });
    	
    	
	
});

function focamensagem(){
	document.getElementById('msg').focus();
}

--></script>
<!--  <div style="color: red; font-size: 14px; font-weight: bold; text-align: center; padding-bottom: 5px;" > Devido a inoperância do sistema no período de 20h25min as 20h45min do dia 30/11/2011, prorrogaremos o prazo de envio até a 01h00min do dia 01/12/2011. </div> -->

<?php 

require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");

$ajax = new MontaListaAjax($db);
echo montarAbasArray(criaAbaListaObrasPar(), "par.php?modulo=principal/listaObrasEmDiligencia&acao=A");

monta_titulo($descricao, 'Obras'); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
			<a href="par.php?modulo=principal/orgaoEducacao&acao=A">Dados da Unidade |</a>
				<label style="color:black; text-align:center">
					<b>Lista de Obras</b>
				</label>
			<a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
		</td>
	</tr>
</table>

<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="left" valign="top" width="90%">
            <h3 style="margin-bottom: 10px;"><center>Listas de Obras PAR</center></h3>
            <?php $db->monta_lista_array($arrResultado, $cabecalho, 20, 5, "N", "center"); ?>
		</td>
	</tr>
</table>	
<br />
<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="left" valign="top" width="90%">
            <h3 style="margin-bottom: 10px;"><center>Listas de Obras PAC</center></h3>
            <?php $ajax->montaLista($arrResultadoPAC, $cabecalhoPAC, 20, 5, "N", "center", 100); ?>
		</td>
	</tr>
</table>	
<br />
<div id="divDebug"></div>