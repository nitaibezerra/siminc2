<?php
include_once APPRAIZ . "includes/workflow.php";

if (!$_SESSION['par']) {
    echo "<script>alert('Erro de sessão!');window.close();</script>";
    exit;
}



if ($_POST['requisicaoAjax']) {
    ob_clean();
    header('content-type: text/html; charset=ISO-8859-1');
    $_POST['requisicaoAjax']();
    exit;
}

/*CArrega Item*/
function carregarItemPorSesid(){
    global $db;
    $sql = "SELECT 
                ico.icodescricao, 
                ssi.seiqtd, 
                ssi.seiqtdtecnico
            FROM par.subescolas_subitenscomposicao ssi
            INNER JOIN par.subacaoitenscomposicao ico ON ico.icoid = ssi.icoid and ico.icostatus = 'A'
            WHERE ssi.sesid = {$_POST['sesid']}";
    $arrItem = $db->carregar($sql);
    $strRetorno = "<table width=100%>";
    if ($arrItem) {
        $strRetorno .= "
            <tr>
                <th>Item</th>
                <th>Quantidade</th>
                <th>Quantidade Técnico</th>
            </tr>";
        foreach($arrItem as $item){
            $seiqtd = (!empty($item['seiqtd'])) ? $item['seiqtd'] : '0';
            $seiqtdtecnico = (!empty($item['seiqtdtecnico'])) ? $item['seiqtdtecnico'] : '0';
            $strRetorno .= "<tr>
                    <td>{$item['icodescricao']}</td>
                    <td align=center>{$seiqtd}</td>
                    <td align=center>{$seiqtdtecnico}</td>
                </tr>";
        }   
    }else {
        $strRetorno .= "<tr>
                    <td>Nenhum resultado encontrado.</td>
                </tr>";
    }
            
    $strRetorno .= "</table>";
    echo $strRetorno;
}

function desabilitarAnoSubacaoReprogramacao($sbaid, $ano){
	
    global $db;
    
    $sql = "SELECT sbdid FROM par.subacaodetalhe WHERE sbaid = $sbaid AND sbdano = $ano";
    
    $sbdid = $db->pegaUm( $sql );
    
    if( !$sbdid ){
    	return 't';
    }
    
    $sql = "SELECT TRUE FROM par.termocomposicao WHERE sbdid = $sbdid";
    
    $temTermo = $db->pegaUm( $sql );
    
    if( $temTermo != 't' ){
    	return 't';
    }
    
    #Bloquear Ano da Subacao quando não estiver em reprogramacao e estiver vinculado a um documento
    $sql = "SELECT DISTINCT
                TRUE
            FROM par.documentopar dop
            LEFT  JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
            LEFT  JOIN par.reprogramacao rep ON rep.dopidreprogramado = dop.dopid
            LEFT  JOIN par.documentoparreprogramacaosubacao	dprs ON rep.repid = dprs.repid
            WHERE (
                (dop.dopreformulacao IS TRUE AND dpr.dpsstatus = 'A')
                OR (dpr.dpsstatus = 'P')
                OR rep.repstatus = 'A'
            )
            AND dop.dopstatus = 'A'
            AND dop.dopid = (
                SELECT DISTINCT
                    max(dp.dopid)
                FROM par.termocomposicao  tc
                INNER JOIN par.subacaodetalhe sd ON tc.sbdid = sd.sbdid
                INNER JOIN par.documentopar dp ON dp.dopid = tc.dopid AND dp.dopstatus = 'A'
                INNER JOIN par.processopar prp ON prp.prpid = dp.prpid AND prp.prpfinalizado IS NOT TRUE
                INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
                WHERE sd.sbaid = {$sbaid} and sd.sbdano = {$ano} AND md.tpdcod IN (21, 102)
        )";
    $booResultado = $db->pegaUm($sql);

    return ($booResultado) ? 't' : 'f';
}

 if ($_REQUEST['inuid']) {
     $_SESSION['par']['inuid'] = $_REQUEST['inuid'];
 }
montarAvisoCabecalho(null, null, null, $_SESSION['par']['inuid']);

// varifica se a subação é de ônibus acessível
function isSubacaoOnibusAcessivel($dados) {
    if ($dados['sbaid']) {
        global $db;

        if (is_array($dados['sbaid'])) {
            $anoatual = $dados['anoatual'];
            $sbaIdPesquisa = $dados['sbaid'][$anoatual];
        } else {
            $sbaIdPesquisa = $dados['sbaid'];
        }
        $perfil = pegaArrayPerfil($_SESSION['usucpf']);

        $sql = "select *  from par.subacao  where sbaid = $sbaIdPesquisa AND prgid = 153";
        $result = $db->pegaLinha($sql);
        if (is_array($result) && count($result)) {
            $isTransporteAcessivel = true;

            $muncod = $_SESSION['par']['muncod'];
            $inuId = $_SESSION['par']['inuid'];
            if (($muncod) && ($inuId)) {

                $sqlCota = "
					SELECT ma.mprqtd as cota_inicial ,
						CASE WHEN /*CASO*/
						(
							       (
									      select sum( icoquantidade  )::integer
									      from  par.subacao s 
									      inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
									      inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
									      inner join par.acao                     a on a.aciid = s.aciid
									      inner join par.pontuacao                p on p.ptoid = a.ptoid
									      WHERE  prgid in ( 153) and p.inuid = {$inuId}
									      and s.sbastatus = 'A'
							       ) /*SOLICITADO*/
							       
							       > /*MAIOR QUE */
							       
							       ma.mprqtd::integer /* COTA INICIAL */
						)
						THEN /*ENTÃO*/
						
						(
							       ma.mprqtd::integer  /*COTA INICIAL*/
							       
							       - /* MENOS */
							      (
								   select  sum( icoquantidadetecnico  )::integer
									from par.subacao s  
									inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
									inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
									
									inner join par.acao                     a on a.aciid = s.aciid
									inner join par.pontuacao                p on p.ptoid = a.ptoid
									WHERE p.inuid =  {$inuId}
									and prgid in ( 153)
									and sd.sbdid in ( select sd.sbdid 
										       from par.empenhosubacao es 
										       inner join  par.subacaodetalhe sd on es.sbaid = sd.sbaid and sd.sbdano = es.eobano 
										       inner join par.empenho e on e.empid = es.empid and empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' 
										       inner join par.processopar pp on pp.prpnumeroprocesso = e.empnumeroprocesso 
										       where inuid = {$inuId} and es.eobstatus = 'A'
										       )
									and s.sbastatus = 'A'
								       
								) /*ENPENHADO*/
						)
						
						ELSE  /*SENÃO */
						
						(
							       (
								      select sum( icoquantidade  )::integer
							       from  par.subacao s 
							       inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
							       inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
							       inner join par.acao                     a on a.aciid = s.aciid
							       inner join par.pontuacao                p on p.ptoid = a.ptoid
							       WHERE  prgid in ( 153) and p.inuid =  {$inuId}
							       and s.sbastatus = 'A'
						
							       ) /* SOLICITADO */
						
							       - /*MENOS*/
						
							      (
								   select  sum( icoquantidadetecnico  )::integer
									from par.subacao s  
									inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
									inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
									
									inner join par.acao                     a on a.aciid = s.aciid
									inner join par.pontuacao                p on p.ptoid = a.ptoid
									WHERE p.inuid =   {$inuId}
									and prgid in ( 153)
									and sd.sbdid in ( select sd.sbdid 
										       from par.empenhosubacao es 
										       inner join  par.subacaodetalhe sd on es.sbaid = sd.sbaid and sd.sbdano = es.eobano 
										       inner join par.empenho e on e.empid = es.empid and empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' 
										       inner join par.processopar pp on pp.prpnumeroprocesso = e.empnumeroprocesso 
										       where inuid = {$inuId} and es.eobstatus = 'A'
										       )
									and s.sbastatus = 'A'
								       
								)  /*EMPENHADO*/
						) 
						END
						
						as cota_atual
						
						
						from par.municipioadesaoprograma ma 
						
						where muncod = '{$muncod}' and pfaid = 11
				
				
				";

                $resultCota = $db->pegaLinha($sqlCota);

                $cotaInicial = $resultCota['cota_inicial'];
                $cotaAtual = $resultCota['cota_atual'];
                if (is_array($resultCota) && count($resultCota)) {
                    return array(
                        'temCota' => true,
                        'cotaInicial' => $cotaInicial,
                        'cotaAtual' => $cotaAtual
                    );
                } else {
                    return array(
                        'temCota' => false
                    );
                }
            } else {
                return array(
                    'temCota' => false
                );
            }
        } else {
            return array(
                'temCota' => false
            );
        }
    } else {
        return array(
            'temCota' => false
        );
    }
}

function bloqueiaEquipeLocalOnibusAcessivel($dados) {

    if ($dados['sbaid']) {
        global $db;

        if (is_array($dados['sbaid'])) {
            $anoatual = $dados['anoatual'];
            $sbaIdPesquisa = $dados['sbaid'][$anoatual];
        } else {
            $sbaIdPesquisa = $dados['sbaid'];
        }
        $perfil = pegaArrayPerfil($_SESSION['usucpf']);

        $sql = "select *  from par.subacao  where sbaid = $sbaIdPesquisa AND prgid = 153";
        $result = $db->pegaLinha($sql);
        if (is_array($result) && count($result)) {
            $isTransporteAcessivel = true;
        } else {
            $isTransporteAcessivel = false;
        }

        if (
                ($isTransporteAcessivel) &&
                (
                in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
                in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
                in_array(PAR_PERFIL_PREFEITO, $perfil) ||
                in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
                in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
                in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil)
                )
        ) {

            return true;
        } else {
            return false;
        }
    } else {
        echo "<script>alert('Erro de variável, sbaid ausente!');window.close();</script>";
        exit;
    }
}

if ($_REQUEST['enviaAnaliseBP'] == 1) {

    $sql = "SELECT 
				DISTINCT sea.ppsid 
			FROM 
				par.subacaobpenvioanalise sea
			INNER JOIN par.subacao s ON s.ppsid = sea.ppsid
			WHERE 
				s.sbaid = " . $_REQUEST['sbaid'];

    $testaBP = $db->pegaUm($sql);

    $testaEnvio = false;

    if ($testaBP) {
        $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
        if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $arrayPerfil)) {
            $testaEnvio = true;
        }
    }

    if ($testaEnvio == true) {
        global $db;

        //altera o estado do WF
        $sql = "SELECT docid FROM par.subacao WHERE sbaid = " . $_REQUEST['sbaid'];
        $docidS = $db->pegaUm($sql);

        $sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, htddata)
						VALUES (" . WF_SUBACAO_BTN_ENVIAR_PARA_ANALISE . ", $docidS, '{$_SESSION['usucpf']}', now()) returning hstid";
        $hstid = $db->pegaUm($sql);

        $sql = "update workflow.documento set esdid = " . ANALISE . ", docdsc = 'Análise' where docid = $docidS";
        $db->executar($sql);

        $cmddsc = trim('Subações criadas visando evitar que as unidades escolares prontas em 2013 e 2014 e na iminência de serem concluídas fiquem impedidas de funcionarem por falta de aquisição dos equipamentos, mobiliários e laboratórios indispensáveis ao funcionamento das mesmas.
						O critério estabelecido para possibilitar o pedido dos bens no sistema SIMEC foi o percentual de execução das obras financiadas pelo Programa Brasil Profissionalizado de 65% acima.
						Ressalta-se que  o empenho das despesas será condicionado à aprovação da área técnica da SETEC, podendo em caso de inobservância do Termo de compromisso, serem os recursos bloqueados à época da disponibilidade orçamentária.');
        $cmddsc = str_replace("'", "\\'", $cmddsc);
        $sql = "insert into workflow.comentariodocumento(docid, hstid, cmddsc, cmddata, cmdstatus)
				values(" . $docidS . "," . $hstid . ", '" . addslashes($cmddsc) . "', now(), 'A' )";
        $db->executar($sql);
        $db->commit();


        echo "<script>
				alert('Estado alterado com sucesso!');
				window.location.href = '/par/par.php?modulo=principal/subacao&acao=A&sbaid=" . $_REQUEST['sbaid'] . "'
			</script>";

        exit;
    }
}

function verificaComplementoDB() {

    global $db;

    if ($_POST['ssuid'] == 20 || $_POST['ssuid'] == 23) {
        $campos = "	coalesce(sbdrepassevlrcomplementar,0)::numeric(20,2) as sbdrepassevlrcomplementar,
					coalesce(sbdrepassevlrempenho,0)::numeric(20,2) as sbdrepassevlrempenho,
					coalesce(sbdrepassevlrraf,0)::numeric(20,2) as sbdrepassevlrraf";
    }
    if ($_POST['ssuid'] == 21) {
        $campos = "	coalesce(sbdrepassevlrcomplementaraprovado,0)::numeric(20,2) as sbdrepassevlrcomplementaraprovado,
					coalesce(sbdrepassevlrempenhoaprovado,0)::numeric(20,2) as sbdrepassevlrempenhoaprovado,
					coalesce(sbdrepassevlrrafaprovado,0)::numeric(20,2) as sbdrepassevlrrafaprovado";
    }


    $sql = "SELECT 
				$campos
			FROM par.subacaodetalhe 
			WHERE 
				sbaid = {$_POST['sbaid']} 
				AND sbdano = '{$_POST['ano']}'";

    $dados = $db->pegaLinha($sql);

    echo implode('}{', $dados);
}

function carregaEmendaPTA() {
    global $db;

    $sql = "select vede.emdid as codigo, vede.emecod as descricao from emenda.ptemendadetalheentidade pte
				inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
			where
				pte.ptrid = {$_POST['ptrid']}
				and vede.edestatus = 'A'";
    $emeid = $db->carregar($sql);

    if (sizeof($emeid) == '1') {
        $html = '<select maximo="0" tipo="combo_popup" multiple="multiple" size="5" name="emenda[' . $_POST['ano'] . '][]" id="emenda[' . $_POST['ano'] . ']" onclick="javascript:combo_popup_alterar_campo_busca( this );"
						ondblclick="javascript:combo_popup_abre_janela( \'emenda[' . $_POST['ano'] . ']\', 400, 400, false );" onkeydown="javascript:combo_popup_remove_selecionados( event, \'emenda[' . $_POST['ano'] . ']\' );"
						class="CampoEstilo" style="width:400px;">
					<option value="' . $emeid[0]['codigo'] . '">' . $emeid[0]['descricao'] . '</option>
				</select>';
        echo $html;
    } else {
        combo_popup('emenda[' . $_POST['ano'] . ']', $sql, '', '400x400', 0, array(), '', true, false, false, 5, 400);
    }
    die();
}

function preencherParecer() {
    global $db;

    $sbaid = $_POST['sbaid'];

    $sql = "SELECT
				pp.pspdescricao
			FROM
				par.propostasubacaoparecer pp
			INNER JOIN par.subacao s ON s.ppsid = pp.ppsid
			WHERE
				s.sbaid = " . $sbaid;

    $parecer = $db->pegaUm($sql);

    if ($parecer) {
        echo $parecer;
    } else {
        echo 1;
    }
    die;
}

function filtraPTRES() {
    global $db;

    $sql = "select
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$_POST['sbdplanointerno']}'";

    echo $db->monta_combo("sbdptres[" . $_POST['ano'] . "]", $sql, 'S', 'Selecione...', '', "", "", "", "N", "", "");
    exit;
}

function excluirEscola() {

    $sesid = $_POST['sesid'];
    if (!$sesid) {
        echo "erro";
    } else {
        global $db;
        $sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE sesid = " . $sesid;
        $db->executar($sql);
        $oSes = new SubacaoEscola();
        $oSes->excluirEscola($sesid);
    }
}

function recarregarEscola() {
    global $db;

    $oSes = new SubacaoEscola();

    echo $oSes->montaListaAlunos($_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['frmid'], $_REQUEST['ptsid']);
    die;
}

function excluirItem() {
    global $db;

    $id = $_POST['icoid'];

// 	$sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid = ".$id;
// 	$db->executar( $sql );
// 	$sql = "DELETE FROM par.subacaoitenscomposicao WHERE icoid = ".$id;
    $sql = "UPDATE par.subacaoitenscomposicao SET icostatus = 'I' WHERE icoid = " . $id;
    $db->executar($sql);
    $db->commit();
    die;
}

function retiraQuantidadeEscola() {
    global $db;

    $sesid = $_POST['sesid'];
    $sbaid = $_POST['sbaid'];
    $sbdano = $_POST['sbdano'];
    
    $sql = "DELETE FROM par.subescolas_subitenscomposicao where sesid = {$sesid}";
    $db->executar($sql);
    
    $oSes = new SubacaoEscola();

    $oSes->excluirEscola($sesid);    
    atualizaValorSubacaoDetalhe($sbaid, $sbdano);
            
    echo ($db->commit()) ? 1 : 0 ;
    die;
}

function validarItem() {
    global $db;

    $sbaid = $_POST['sbaid'];
    $ano = $_POST['ano'];
    $icoid = $_POST['icoid'];
    $tipo = $_POST['tipo'];
    $cronograma = $_POST['cronograma'];

    $sql = "SELECT * FROM par.subacaoitenscomposicao WHERE icoid = " . $icoid . " AND icoano = " . $ano . " AND sbaid = " . $sbaid;
    $dados = $db->pegaLinha($sql);
    if ($cronograma == 'esc') {
    	
    	$sql = "SELECT iu.itrid FROM par.subacao s
                    INNER JOIN par.acao a ON a.aciid = s.aciid
                    INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
                    INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
                    WHERE s.sbaid IN (".$sbaid.")";
        $itrid = $db->pegaUm( $sql );

        $sql = "SELECT iu.inuid FROM par.subacao s
                    INNER JOIN par.acao a ON a.aciid = s.aciid
                    INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
                    INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
                    WHERE s.sbaid IN (".$sbaid.")";

        $inuid = $db->pegaUm( $sql );

        $sqlInu = "SELECT * FROM par.instrumentounidade WHERE inuid = $inuid";
        $muncodInu = $db->pegaLinha($sqlInu);
    	
    	if( $itrid == '1' ){
    		$tcpid = 1;
    	}else{
    		$tcpid = 3;
    		if ($muncodInu['muncod'] == '5300108' || $muncodInu['estuf'] == 'DF') {
    			$tcpid = 1;
    		}
    	}
    	
		$sql = "SELECT
					SUM( seiqtd )
				FROM
					par.subacaoitenscomposicao sic
				INNER JOIN par.subacaoescolas 					ses ON ses.sbaid = sic.sbaid 	AND ses.sesstatus = 'A'
				INNER JOIN par.escolas 							esc ON esc.escid = ses.escid
				INNER JOIN entidade.entidade 					t   ON t.entid = esc.entid 		AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and t.tpcid = $tcpid
				INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid 		AND f.funid = 3
				INNER JOIN par.subescolas_subitenscomposicao 	ssc ON ssc.icoid = sic.icoid 	AND ssc.sesid = ses.sesid
				WHERE
					sic.sbaid = $sbaid
					AND sic.icoid = $icoid
					AND sic.icoano = $ano";
        $quantidade = $db->pegaUm($sql);
    } else {
        $quantidade = $dados['icoquantidade'];
    }

    if ($dados['icoid']) {
        if ($tipo == 'marca') {
            $sql = "UPDATE par.subacaoitenscomposicao SET icovalidatecnico = 'S' WHERE icoid = " . $icoid . " AND icoano = " . $ano . " AND sbaid = " . $sbaid;
        } else {
            $sql = "UPDATE par.subacaoitenscomposicao SET icovalidatecnico = 'N' WHERE icoid = " . $icoid . " AND icoano = " . $ano . " AND sbaid = " . $sbaid;
        }
        $db->executar($sql);
        $db->commit();
        $erro = 0;
    } else {
        $erro = 1;
    }

    if ($cronograma == 'esc') { //por escola
        $sql = "UPDATE par.subescolas_subitenscomposicao SET seiqtdtecnico = seiqtd WHERE sesid IN ( select sesid from par.subacaoescolas where sbaid = " . $sbaid . " AND sesano = " . $ano . " ) AND icoid = " . $icoid;
        $db->executar($sql);
        $db->commit();
    }

    $sbdano = $ano;
    if($sbaid && $sbdano)
    {	 
    	atualizaValorSubacaoDetalhe($sbaid, $sbdano);
    }
    if ($erro == 0) {
        echo $quantidade;
    } else {
        echo 'Erro';
    }
    die;
}

function validarEscola() {
    global $db;

    $sbaid = $_POST['sbaid'];
    $ano = $_POST['ano'];
    $sesid = $_POST['sesid'];
    $tipo = $_POST['tipo'];

    $sql = "SELECT * FROM par.subacaoescolas WHERE sesid = " . $sesid . " AND sesano = " . $ano . " AND sbaid = " . $sbaid;
    $dados = $db->pegaLinha($sql);

    $quantidade = $dados['sesquantidade'];

    if ($dados['sesid']) {
        if ($tipo == 'marca') {
            $sql = "UPDATE par.subacaoescolas SET sesvalidatecnico = 'S' WHERE sesid = " . $sesid . " AND sesano = " . $ano . " AND sbaid = " . $sbaid;
        } else {
            $sql = "UPDATE par.subacaoescolas SET sesvalidatecnico = 'N' WHERE sesid = " . $sesid . " AND sesano = " . $ano . " AND sbaid = " . $sbaid;
        }
        $db->executar($sql);
        $db->commit();
        $erro = 0;
    } else {
        $erro = 1;
    }

    if ($erro == 0) {
        echo $quantidade;
    } else {
        echo 'Erro';
    }
    die;
}

function excluirBeneficiario() {
    global $db;

    $id = $_POST['sabid'];

    $sql = "DELETE FROM par.subacaobeneficiario WHERE sabid = " . $id;
    $db->executar($sql);
    $db->commit();

    die;
}

function excluirObra() {
    $preid = $_POST['preid'];
    if (!$preid) {
        echo "erro";
    } else {
        $oPre = new PreObra();
        $oPre->excluirPreObra($preid);
        $oPre->commit();
    }
}

function verificaPendenciasComplementoSubacaoRef( $sbaid ){

	global $db;

	$sql = "SELECT DISTINCT true
			FROM par.subacaodetalhe sb
			WHERE
				sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbaidpai = $sbaid AND sbastatus = 'R' )
				AND sbaid NOT IN (
								SELECT sbaid
								FROM
									(
									SELECT min(sba.sbaid) as sbaid, sbdano
									FROM par.subacao sba
									INNER JOIN par.subacaodetalhe sbd ON sbd.sbaid = sba.sbaid
									WHERE sbaidpai = $sbaid AND sbastatus = 'R'
									GROUP BY sbdano
									) as foo)
				AND ( SELECT count(sbaid) FROM par.subacao WHERE sbaidpai = $sbaid AND sbastatus = 'R' ) > 1
				AND ( (
						par.recuperavalorvalidadossubacaoporano(sbaid, sbdano)-
						( 	SELECT par.recuperavalorvalidadossubacaoporano(sbaid, sbdano)
							FROM par.subacaodetalhe WHERE sbdid = ( SELECT sbdid FROM (
							SELECT sbdid, sba.sbaid
							FROM par.subacao sba
							INNER JOIN par.subacaodetalhe sbd ON sbd.sbaid = sba.sbaid
							WHERE sbaidpai = $sbaid AND sbd.sbaid < sb.sbaid
							ORDER BY sbaid
							LIMIT 1 ) as foo )
						) ) > 0 )
				AND ( sbdrepassevlrcomplementaraprovado IS NULL AND sbdrepassevlrempenhoaprovado IS NULL AND sbdrepassevlrrafaprovado IS NULL )";

	return $db->pegaUm($sql)=='t';
}

function salvar_pendenciasComplementoReformulacao(){

	global $db;

	$sql = "";
	$erro = "";
	if( is_array($_POST['vlr_aprovado']) ){
		foreach( $_POST['vlr_aprovado'] as $sbdid => $vlr_aprovado ){
			$sbdrepassevlrcomplementaraprovado 	= str_replace(Array('.', ','), Array('', '.'), $_POST['sbdrepassevlrcomplementaraprovado'][$sbdid]);
			$sbdrepassevlrempenhoaprovado 		= str_replace(Array('.', ','), Array('', '.'), $_POST['sbdrepassevlrempenhoaprovado'][$sbdid] );
			$sbdrepassevlrrafaprovado 			= str_replace(Array('.', ','), Array('', '.'), $_POST['sbdrepassevlrrafaprovado'][$sbdid] );
			if( $vlr_aprovado == ( $sbdrepassevlrcomplementaraprovado + $sbdrepassevlrempenhoaprovado + $sbdrepassevlrrafaprovado ) ){
				$sql .= "UPDATE par.subacaodetalhe
						SET sbdrepassevlrcomplementaraprovado = $sbdrepassevlrrafaprovado, sbdrepassevlrempenhoaprovado = $sbdrepassevlrempenhoaprovado, sbdrepassevlrrafaprovado = $sbdrepassevlrrafaprovado
						WHERE sbdid = $sbdid;";
			}else{
				$erro .= "<b>{$_POST['nome'][$sbdid]}:</b> <label style=\"color: red\">A soma dos valores complementares deve ser igual ao \"Valor Complementar Total\". </label>\n";
			}
		}
	}
		
	if( $erro != '' ){
		echo utf8_decode($erro);
	}else{
		$db->executar($sql);
		$db->commit();
		echo 'sucesso';
	}
}

function form_pendenciasComplementoReformulacao(){

	global $db;

	if($_SESSION['par']['itrid'] == 2 ){

		$sql = "SELECT mundescricao || '-' || estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
	
		$local = $db->pegaUm( $sql );
	} else {

		$local = $_SESSION['par']['estuf'];
	}

	$sql = "SELECT DISTINCT sbdano
			FROM par.subacaodetalhe
			WHERE
				sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbaidpai = {$_POST['sbaid']} AND sbastatus = 'R' )
				AND sbaid NOT IN (
								SELECT sbaid
								FROM
								(
										SELECT min(sba.sbaid) as sbaid, sbdano
										FROM par.subacao sba
										INNER JOIN par.subacaodetalhe sbd ON sbd.sbaid = sba.sbaid
										WHERE sbaidpai = {$_POST['sbaid']} AND sbastatus = 'R'
										GROUP BY sbdano
								) as foo)
				AND ( SELECT count(DISTINCT sbaid) FROM par.subacao WHERE sbaidpai = {$_POST['sbaid']} AND sbastatus = 'R' ) > 1
				AND ( sbdrepassevlrcomplementaraprovado IS NULL AND sbdrepassevlrempenhoaprovado IS NULL AND sbdrepassevlrrafaprovado IS NULL )";

	$arrAnos = $db->carregarColuna( $sql );
	$arrAnos = is_array($arrAnos) ? $arrAnos : Array();
	?>
<div id="div_alert_dialog"></div>
<form name="form_complemento" id="form_complemento" method="post" action="" >
	<label style="color: red; font-size:11px;"><b>Por favor, para continuar, preencha os valores de complemento para todas as reformulações dos anos solicitados abaixo.</b></label>
	<br><br>
	<table align="center" border="0" cellpadding="3" cellspacing="1" >
<?php 
	foreach( $arrAnos as $ano ){
		$sql = "SELECT DISTINCT sbaid, sbdid, sbdrepassevlrcomplementaraprovado, sbdrepassevlrempenhoaprovado, sbdrepassevlrrafaprovado, par.recuperavalorvalidadossubacaoporano(sbaid, sbdano) as vlr_aprovado 
				FROM par.subacaodetalhe 
				WHERE 
					sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbaidpai = {$_POST['sbaid']} AND sbastatus = 'R' ) 
					AND sbdano = $ano
				ORDER BY 1";
		
		$arrComplementos = $db->carregar( $sql );
		if( is_array($arrComplementos) ){
?>
		<tr style="background-color: 0451c5; color: white;"><td style="font-size:12px;text-align: center;" colspan="5"><b><?=$ano ?></b></td></tr>
		<tr>
			<td><b>Ordem</b></td>
			<td><b>Valor Complementar<br>Total</b></td>
			<td><b><?=$local ?></b></td>
			<td><b>Empenho Complementar (MEC/FNDE)</b></td>
			<td><b>RAF (MEC/FNDE)</b></td>
		</tr>
<?php 
			foreach( $arrComplementos as $ordem => $complemento ){
?>
		<tr>
			<td><?=( $ordem == 0 ? "Original" : "{$ordem}° Reformulação <input type=hidden name=nome[{$complemento['sbdid']}] value=\"{$ordem}° Reformulação de $ano\" />") ?></td>
			<td style="text-align: center">
				<?=($ordem == 0 ? "N possui" : 
						( ( $complemento['vlr_aprovado'] - $arrComplementos[$ordem-1]['vlr_aprovado'] ) > 0 ? 
							number_format( ($complemento['vlr_aprovado']-$arrComplementos[$ordem-1]['vlr_aprovado']), 2, ',', '.').
							"<input type=hidden name=vlr_aprovado[{$complemento['sbdid']}] value=".($complemento['vlr_aprovado'] - $arrComplementos[$ordem-1]['vlr_aprovado'])." />" : 
							'0,00' 
						) 
					) 
				?>
			</td>
			<td style="text-align: center">
				<?=( $ordem == 0 || ( $complemento['vlr_aprovado'] - $arrComplementos[$ordem-1]['vlr_aprovado'] ) <= 0 ? 
						"N possui" : 
						( ($complemento['sbdrepassevlrcomplementaraprovado'] > 0 && $complemento['sbdrepassevlrempenhoaprovado'] > 0 && $complemento['sbdrepassevlrrafaprovado'] > 0 ) ?
							number_format( $complemento['sbdrepassevlrcomplementaraprovado'], 2, ',', '.') :
							campo_texto("sbdrepassevlrcomplementaraprovado[".$complemento['sbdid']."]","N",$habilita,"Valor","10","20","[.###],##","","","","",'id="sbdrepassevlrcomplementaraprovado_'.$complemento['sbdid'].'"', '', number_format($complemento['sbdrepassevlrcomplementaraprovado'], 2, ',', '.') )
						) 
					) 
				?>
			</td>
			<td style="text-align: center">
				<?=( $ordem == 0 || ( $complemento['vlr_aprovado'] - $arrComplementos[$ordem-1]['vlr_aprovado'] ) <= 0 ? 
						"N possui" : 
						( ($complemento['sbdrepassevlrcomplementaraprovado'] > 0 && $complemento['sbdrepassevlrempenhoaprovado'] > 0 && $complemento['sbdrepassevlrrafaprovado'] > 0 ) ?
							number_format( $complemento['sbdrepassevlrempenhoaprovado'], 2, ',', '.') :
							campo_texto("sbdrepassevlrempenhoaprovado[".$complemento['sbdid']."]","N",$habilita,"Valor","10","20","[.###],##","","","","",'id="sbdrepassevlrempenhoaprovado_'.$complemento['sbdid'].'"', '', number_format($complemento['sbdrepassevlrempenhoaprovado'], 2, ',', '.') )
						) 
					) 
				?>
			</td>
			<td style="text-align: center">
				<?=( $ordem == 0 || ( $complemento['vlr_aprovado'] - $arrComplementos[$ordem-1]['vlr_aprovado'] ) <= 0 ? 
						"N possui" : 
						( ($complemento['sbdrepassevlrcomplementaraprovado'] > 0 && $complemento['sbdrepassevlrempenhoaprovado'] > 0 && $complemento['sbdrepassevlrrafaprovado'] > 0 ) ?
							number_format( $complemento['sbdrepassevlrrafaprovado'], 2, ',', '.') :
							campo_texto("sbdrepassevlrrafaprovado[".$complemento['sbdid']."]","N",$habilita,"Valor","10","20","[.###],##","","","","",'id="sbdrepassevlrrafaprovado_'.$complemento['sbdid'].'"', '', number_format($complemento['sbdrepassevlrrafaprovado'], 2, ',', '.') )
						) 
					) 
				?>
			</td>
		</tr>
<?php 
			}
		}
	}
?>
	</table>
</form>
<?php 
}

if ($_REQUEST['sbaid']) {
    if (is_array($_REQUEST['sbaid'])) {
        if ($_REQUEST['anoatual']) {
            $anoT = $_REQUEST['anoatual'];
        } else {
            $anoT = date('Y');
        }
        $sbaid = $_REQUEST["sbaid"][$anoT];
    } else {
        $sbaid = $_REQUEST["sbaid"];
    }
}

if (!$inuid) {
    $inuid = $_REQUEST['inuid'] ? $_REQUEST['inuid'] : $_SESSION['par']['inuid'];
    if (!$inuid) {
        $inuid = pegaInuidDeSbaid($sbaid);
    }
    $_SESSION['par']['inuid'] = $inuid;
    if (!$inuid) {
        echo "<script>alert('Falta o INUID!');window.close();</script>";
        exit;
    }
}

if ($_SESSION['par']['itrid'] == 2) {
    //$_SESSION['par']['muncod'] = $db->pegaUm( "SELECT muncod FROM par.instrumentounidade WHERE inuid = ".$inuid );
    if (!$_SESSION['par']['muncod']) {
        if (is_array($_REQUEST['sbaid'])) {
            $sbaid = implode(', ', $_REQUEST['sbaid']);
        } else {
            $sbaid = $_REQUEST['sbaid'];
        }
        $_SESSION['par']['muncod'] = $db->pegaUm("SELECT iu.muncod FROM par.subacao s
													INNER JOIN par.acao a ON a.aciid = s.aciid
													INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
													INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
													WHERE s.sbaid IN (" . $sbaid . ")");
    }
} elseif ($_SESSION['par']['itrid'] == 1) {
    if (is_array($_REQUEST['sbaid'])) {
        $sbaid = implode(', ', $_REQUEST['sbaid']);
    } else {
        $sbaid = $_REQUEST['sbaid'];
    }
    if (!$_SESSION['par']['estuf']) {
        $_SESSION['par']['estuf'] = $db->pegaUm("SELECT iu.estuf FROM par.subacao s
													INNER JOIN par.acao a ON a.aciid = s.aciid
													INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
													INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
													WHERE s.sbaid IN (" . $sbaid . ")");
    }
}

$docid = parPegarDocidParaEntidade($inuid);
$estadoAtual = wf_pegarEstadoAtual($docid);

function limpar() {
    if ($_GET['sbaid'] && $_POST['anoatual']) {

        $ano = $_POST['anoatual'];
        $anos = array(0 => $ano);
        $sbaid = $_GET['sbaid'];

        if (limparSubacao($sbaid, $anos)) {

            echo "<script>
					alert('Dados excluídos com sucesso!');
					location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$sbaid}';
		 		</script>";
        } else {
            echo "<script>
				alert('Erro!');
				history.back(-1);
	 		</script>";
        }
    } else {
        echo "<script>
				alert('Falta o sbaid!');
				history.back(-1);
	 		</script>";
    }
}

function gravarHistoricoSubacaoDetalhe($post) {
    global $db;

//	if(count($post['sbaid'])){
    if ($post['anoatual']) {
        $k = $post['anoatual'];
        //foreach($post['sbaid'] as $k => $v){
        if ($post['bo_alterar_parecer'][$k] || $post['bo_alterar_merito'][$k] || empty($post['ssuids'][$k])) {

            $post['sbdinicio'][$k] = $post['sbdinicio'][$k] ? $post['sbdinicio'][$k] : 'null';
            $post['sbdfim'][$k] = $post['sbdfim'][$k] ? $post['sbdfim'][$k] : 'null';
            $post['sbdquantidade'][$k] = $post['sbdquantidade'][$k] ? $post['sbdquantidade'][$k] : 'null';
            $post['ssuid'][$k] = $post['ssuid'][$k] ? $post['ssuid'][$k] : 'null';
            $post['prpid'][$k] = $post['prpid'][$k] ? $post['prpid'][$k] : 'null';
            $post['sdhanotermino'][$k] = $post['sdhanotermino'][$k] ? $post['sdhanotermino'][$k] : 'null';

            if (empty($post['ssuids'][$k])) {
                $sdhtipoparecer = 'N';
            } else {
                $sdhtipoparecer = !empty($post['bo_alterar_parecer'][$k]) ? 'P' : ($post['bo_alterar_merito'][$k] ? 'M' : '');
            }

            if ($post['sbdparecer'][$k] || $post['sbdparecerdemerito'][$k]) {

                $sql = "insert into par.subacaodetalhehistorico
								(sbdid, sdhparecer, sdhquantidade, sdhinicio, sdhfim, ssuid, sdhanotermino, sdhnaturezadespesa, sdhdetalhamento,
				  	 			prpid, sdhplanointerno, sdhparecerdemerito, sdhptres, sdhtipoparecer, sdhstatus)
				  	 		values
				  	 			({$post['sbdid'][$k]}, '{$post['sbdparecer'][$k]}', {$post['sbdquantidade'][$k]}, {$post['sbdinicio'][$k]}, {$post['sbdfim'][$k]},
				  	 			{$post['ssuid'][$k]}, {$post['sdhanotermino'][$k]}, '{$post['sdhnaturezadespesa'][$k]}', '{$post['sdhdetalhamento'][$k]}',
				  	 			{$post['prpid'][$k]}, '{$post['sbdplanointerno'][$k]}', '{$post['sbdparecerdemerito'][$k]}','{$post['sbdptres'][$k]}',
				  	 			'{$sdhtipoparecer}', 'A')";

                $db->executar($sql);
                $db->commit();
            }
        }
        //	}
        //}
    }
}

function salvarSubacao() {

    global $db;

    $ano = $_POST['anoatual'];
	$success = false;

    $anoSbd = current($_POST['sbdano']);

    $sql = "SELECT true FROM par.subacao WHERE sbaid = {$_POST['sbaid'][$anoSbd]} AND sbaidpai IS NOT NULL";

    $teste = $db->pegaUm($sql);

    if ($teste == 't') {
        echo "<script>alert('Não é possível alterar dados de histórico de subação.');window.location.href = window.location.href;</script>";
        die();
    }
	
    if (
// 		($_POST['sbdrepassecomplementar'][$ano] || $_POST['sbdrepasseempenho'][$ano] || $_POST['sbdrepasseraf'][$ano]) &&
            ( $_POST['ssuids'][$ano] == 20 || $_POST['ssuids'][$ano] == 21 || $_POST['ssuids'][$ano] == 22 || $_POST['ssuids'][$ano] == 23)) { // Está reformulando e dizendo como será o repasse
        global $db;
        $w = "";
        $arr = array();
     
        if ($_POST['ssuids'][$ano] == 20 || $_POST['ssuids'][$ano] == 23) {
            if ($_POST['sbdrepassecomplementar'][$ano]) {
                $arr[] = "sbdrepassecomplementar = '" . $_POST['sbdrepassecomplementar'][$ano] . "'";
                
            }
            if ($_POST['sbdrepasseempenho'][$ano]) {
                $arr[] = "sbdrepasseempenho = '" . $_POST['sbdrepasseempenho'][$ano] . "'";
                
            }
            if ($_POST['sbdrepasseraf'][$ano]) {
                $arr[] = "sbdrepasseraf = '" . $_POST['sbdrepasseraf'][$ano] . "'";
               
            }
            if ($_POST['sbdrepassevlrcomplementar'][$ano]) {
                if ($_POST['sbdrepassecomplementar'][$ano] == 't') {
                    $arr[] = "sbdrepassevlrcomplementar = " . str_replace(',', '.', str_replace('.', '', $_POST['sbdrepassevlrcomplementar'][$ano]));
                } else {
                    $arr[] = "sbdrepassevlrcomplementar = 0";
                }
               
            }
            else{                
                $arr[] = "sbdrepassevlrcomplementar = 0";
              
            }
            if ($_POST['sbdrepassevlrempenho'][$ano]) {
                if ($_POST['sbdrepasseempenho'][$ano] == 't') {
                    $arr[] = "sbdrepassevlrempenho = " . str_replace(',', '.', str_replace('.', '', $_POST['sbdrepassevlrempenho'][$ano]));
                } else {
                    $arr[] = "sbdrepassevlrempenho = 0";
                }
               
            }
             else{                
                $arr[] = "sbdrepassevlrempenho = 0";
                
            }
            if ($_POST['sbdrepassevlrraf'][$ano]) {
                if ($_POST['sbdrepasseraf'][$ano] == 't') {
                    $arr[] = "sbdrepassevlrraf = " . str_replace(',', '.', str_replace('.', '', $_POST['sbdrepassevlrraf'][$ano]));
                } else {
                    $arr[] = "sbdrepassevlrraf = 0";
                }
               
            }
             else{                
                $arr[] = "sbdrepassevlrraf = 0";
               
            }
        }
        if ($_POST['ssuids'][$ano] == 21 || $_POST['ssuids'][$ano] == 22) {
//             ver(str_replace(',','.',str_replace('.','',$_POST['sbdrepassevlrrafaprovado'][$ano])), $_POST, d);
            if ($_POST['sbdrepassevlrcomplementaraprovado'][$ano]) {
                $arr[] = "sbdrepassevlrcomplementaraprovado = " . str_replace(',', '.', str_replace('.', '', $_POST['sbdrepassevlrcomplementaraprovado'][$ano]));
               
            }
             else{                
                $arr[] = "sbdrepassevlrcomplementaraprovado = 0";
                
            }
            
            
            if ($_POST['sbdrepassevlrempenhoaprovado'][$ano]) {
                $arr[] = "sbdrepassevlrempenhoaprovado = " . str_replace(',', '.', str_replace('.', '', $_POST['sbdrepassevlrempenhoaprovado'][$ano]));
              
            }
            
              else{                
                $arr[] = "sbdrepassevlrempenhoaprovado = 0";
               
            }
            if ($_POST['sbdrepassevlrrafaprovado'][$ano]) {
                $arr[] = "sbdrepassevlrrafaprovado = " . str_replace(',', '.', str_replace('.', '', $_POST['sbdrepassevlrrafaprovado'][$ano]));
               
            }
            
              else{                
                $arr[] = "sbdrepassevlrrafaprovado = 0";
                
            }
        }
       
            $w = implode(",", $arr);
            $sql = "UPDATE par.subacaodetalhe SET " . $w . " WHERE sbaid = " . $_POST['sbaid'][$ano] . " AND sbdano = " . $ano;
            $db->executar($sql);
            $db->commit();
           

        if (($_POST['ssuids'][$ano] == 20 || $_POST['ssuids'][$ano] == 21 || $_POST['ssuids'][$ano] == 23 )) {
            $success = true;
        	
        }
    }
// ver($_POST,d);
    if ($_POST['subacaoAnalise'] == 'analise' || $_POST['reformulacao'] == 't' || $_POST['reforco'][$_POST['anoatual']] == 't') {
        if ($_POST['sbdano']) {
            if (is_array($_POST['sbdano'])) {
                $ano = $_POST['anoatual'];
//                if ($_POST['sbdanotermino'][$ano]) {
//                    if ($_POST['sbdanotermino'][$ano] < date("Y")) {
//                        $success = false;
//                    	echo "<script>
//									alert('Verifique o ano de término!');
//									window.location.href = window.location.href;
//								 </script>";
//                        exit();
//                    }
//                }

                //salvando quantidade dos itens do tecnico
                if ($_POST['quant'] == 1) {
                    if (is_array($_POST['icoqtdtecnico'][$ano])) {
                        foreach ($_POST['icoqtdtecnico'][$ano] as $icoid => $qtd) {

                            $qtdMonitorada = 0;
                            $qtdMonitorada = pegaQtdMonitorada($icoid);

                            if (!($qtdMonitorada > $qtd)) {
                                $oSic = new SubacaoItensComposicao();

                                $arCamposNulo = array("icoquantidadetecnico");
                                $oSic->icoid = $icoid;
                                $oSic->icoquantidadetecnico = $qtd ? $qtd : 0;
                                $oSic->salvar(true, true, $arCamposNulo);
                                $oSic->commit();
                            }
                        }
                    }
                }

                //salvando quantidade dos itens
                if ($_POST['quant'] == 1) {
                    if (is_array($_POST['icoqtd'][$ano])) {
                        foreach ($_POST['icoqtd'][$ano] as $icoid => $qtd) {

                            $qtdMonitorada = 0;
                            $qtdMonitorada = pegaQtdMonitorada($icoid);

                            if (!($qtdMonitorada > $qtd)) {

                                $oSic = new SubacaoItensComposicao();

                                $oSic->icoid = $icoid;
                                $oSic->icoquantidade = $qtd ? $qtd : 0;
                                $oSic->salvar();
                                $oSic->commit();
                            }
                        }
                    }
                }

                //salvando as escolas do tecnico
                if ($_POST['quant'] == 2) {
                    if (is_array($_POST['sesquantidadetecnico'][$ano])) {
                        foreach ($_POST['sesquantidadetecnico'][$ano] as $sesid => $qtd) {
                            $oSe = new SubacaoEscola();

                            $oSe->sesid = $sesid;
                            $oSe->sesquantidadetecnico = $qtd ? $qtd : 0;
                            $oSe->salvar();
                            $oSe->commit();
                        }
                    }
                }

                //salvando as escolas da diligencia
                if ($_POST['quant'] == 2) {
                    if (is_array($_POST['sesquantidade'][$ano])) {
                        foreach ($_POST['sesquantidade'][$ano] as $sesid => $qtd) {
                            $oSe = new SubacaoEscola();

                            $oSe->sesid = $sesid;
                            $oSe->sesquantidade = $qtd ? $qtd : 0;
                            $oSe->salvar();
                            $oSe->commit();
                        }
                    }
                }

                //Se for diligencia entao manda pra diligencia
                /*
                  if( $_POST['ssuid'][$ano] == 7 ){
                  $arDados = array('sbaid' => $_POST['sbaid'][$ano]);
                  $estadoAtualSubacao = wf_pegarEstadoAtual( $_POST['docid'] );
                  if( $estadoAtualSubacao["esdid"] != WF_SUBACAO_DILIGENCIA ){
                  wf_alterarEstado( $_POST['docid'], WF_SUBACAO_BTN_ENVIAR_PARA_DILIGENCIA, '', $arDados );
                  }
                  }
                 */
                $oSbd = new SubacaoDetalhe();
                if ($_POST['sbdid'][$ano]) {
                    $oSbd->sbdid = $_POST['sbdid'][$ano];
                } else {
                    $sbdid = $oSbd->retornaDetalheSubacao($ano, $_POST['sbaid'][$ano]);
                    $oSbd->sbdid = $sbdid;
                }

                $oSbd->sbaid = $_POST['sbaid'][$ano];
                $oSbd->sbdano = $ano;
                $oSbd->sbdparecer = $_POST['sbdparecer'][$ano];
                $oSbd->sbddetalhamento = $_POST['sbddetalhamento'][$ano];
                $oSbd->sbdreformulacaoempenho = $_POST['sbdreformulacaoempenho'][$ano];

                if ($_POST['sbdparecerdemerito'][$ano] !== '') {
                    $oSbd->sbdparecerdemerito = $_POST['sbdparecerdemerito'][$ano];
                }

                if ($_POST['ssuid'][$ano] !== '') {
                    $oSbd->ssuid = $_POST['ssuid'][$ano];
                }

                if ($_POST['sbdfim'][$ano]) {
                    $oSbd->sbdfim = $_POST['sbdfim'][$ano];
                }

                if ($_POST['sbdinicio'][$ano]) {
                    $oSbd->sbdinicio = $_POST['sbdinicio'][$ano];
                }

                if ($_POST['sbdplanointerno'][$ano]) {
                    $oSbd->sbdplanointerno = $_POST['sbdplanointerno'][$ano];
                }

                if ($_POST['sbdptres'][$ano]) {
                    $oSbd->sbdptres = $_POST['sbdptres'][$ano];
                }
                if ($_POST['sbdquantidade'][$ano]) {
                    $oSbd->sbdquantidade = $_POST['sbdquantidade'][$ano];
                }

                /*
                 * REGRA QUE LIBERA EDIÇÃO PARA OS USUÁRIOS
                 * RENILDA PERES DE LIMA
                 * JULIO CEZAR DA CAMARA RIBEIRO VIANA
                 * CRIADA EM 07/08/2012 POR WESCLEY
                 * DEMANDANTE: THIAGO TASCA
                 */
                if (in_array($_SESSION['usucpf'], array('', ''))) {
                    $oSbd->sbdanotermino = $_POST['sbdanotermino'][$ano] ? $_POST['sbdanotermino'][$ano] : null; //esse
                }

                $sbdid = $oSbd->salvar(true, true, array('sbdanotermino'));

// 					if( is_array($_POST['emenda'][$ano]) && $_POST['emenda'][$ano][0] && $_POST['pta'][$ano][0] ){
// 						global $db;
// 						$sbdid = ($_POST['sbdid'][$ano] ? $_POST['sbdid'][$ano] : $sbdid);
// 						if( $sbdid ){
// 							$db->executar("DELETE FROM par.subacaoemendapta WHERE sbdid = {$sbdid}");
// 							foreach( $_POST['emenda'][$ano] as $emdid ){
// 								$sql = "INSERT INTO par.subacaoemendapta(sbdid, emdid, ptrid)
// 										VALUES ({$sbdid}, $emdid, {$_POST['pta'][$ano]})";
// 								$db->executar( $sql );
// 							}
// 						}
// 					}


                $sql = "
						UPDATE par.subacao
						  SET usucpf='{$_SESSION['usucpf']}', sbadataalteracao='NOW()'
						WHERE sbaid = " . $_POST['sbaid'][$ano];

                $oSbd->executar($sql);

                $oSbd->commit();

                $_POST['sbdid'][$ano] = $oSbd->sbdid;

                gravarHistoricoSubacaoDetalhe($_POST, $oSbd->sbdid);

                $sbaidRequest = ($_POST['carregarSbaid'] ? $_POST['carregarSbaid'] : $_POST['sbaid'][$_POST['anoatual']]);
                 $success = true;
            }
        }
    } else {
        if ($_POST['sbdano']) {
            if (is_array($_POST['sbdano'])) {
                $ano = $_POST['anoatual'];
                if ($_POST['sbdinicio'][$ano] && $_POST['sbdfim'][$ano]) {
                    if ($_POST['sbdanotermino'][$ano]) {
                        if ($_POST['sbdanotermino'][$ano] < date("Y")) {
                        	 $success = false;
                            echo "<script>
										alert('Verifique o ano de término!');
									 </script>";
                            exit();
                        }
                    }

                    //salvando quantidade de escolas
                    if (is_array($_POST['sesquantidade'][$ano])) {
                        foreach ($_POST['sesquantidade'][$ano] as $sesid => $qtd) {
                            $oSe = new SubacaoEscola();

                            $qtd = str_replace(".", "", $qtd);
                            $qtd = str_replace(",", ".", $qtd);
                            $qtd = str_replace(" ", "", $qtd);

                            if ($sesid != '') {
                                $oSe->sesid = $sesid;
                                $oSe->sesquantidade = $qtd ? $qtd : 0;
                                $oSe->salvar(null, null, array('sesquantidade'));
                                $oSe->commit();
                            }
                        }
                    }

                    /* $oSbd = new SubacaoDetalhe();
                      $sbdid = $oSbd->retornaDetalheSubacao($ano,$_POST['sbaid'][$ano]);
                      if($sbdid){
                      $oSbd->sbdid 				= $sbdid;
                      }else{
                      $oSbd->sbdid 				= $_POST['sbdid'][$ano];
                      } */

                    $oSbd = new SubacaoDetalhe();
                    if ($_POST['sbdid'][$ano]) {
                        $oSbd->sbdid = $_POST['sbdid'][$ano];
                    } else {
                        $sbdid = $oSbd->retornaDetalheSubacao($ano, $_POST['sbaid'][$ano]);
                        $oSbd->sbdid = $sbdid;
                    }

                    $oSbd->sbaid = $_POST['sbaid'][$ano];
                    $oSbd->sbdparecer = $_POST['sbdparecer'][$ano];
                    $oSbd->sbdparecerdemerito = $_POST['sbdparecerdemerito'][$ano];
                    $oSbd->sbdquantidade = $_POST['sbdquantidade'][$ano] ? str_replace(".", "", $_POST['sbdquantidade'][$ano]) : null;
                    $oSbd->sbdano = $_POST['sbdano'][$ano];
                    $oSbd->sbdinicio = $_POST['sbdinicio'][$ano];
                    $oSbd->sbdfim = $_POST['sbdfim'][$ano];
                    $oSbd->sbddetalhamento = $_POST['sbddetalhamento'][$ano];

                    if ($_POST['sbdplanointerno'][$ano]) {
                        $oSbd->sbdplanointerno = $_POST['sbdplanointerno'][$ano];
                    }
                    if ($_POST['sbdptres'][$ano]) {
                        $oSbd->sbdptres = $_POST['sbdptres'][$ano];
                    }
                    if ($_POST['ssuid'][$ano] !== '') {
                        $oSbd->ssuid = $_POST['ssuid'][$ano];
                    }

                    $oSbd->sbdanotermino = $_POST['sbdanotermino'][$ano] ? $_POST['sbdanotermino'][$ano] : null; //esse
                    $oSbd->sbdnaturezadespesa = $_POST['sbdnaturezadespesa'][$ano];
                    $sbdid = $oSbd->salvar(true, true, array('sbdanotermino'));

// 						if( is_array($_POST['emenda'][$ano]) && $_POST['emenda'][$ano][0] && $_POST['pta'][$ano][0] ){
// 							global $db;
// 							$sbdid = ($_POST['sbdid'][$ano] ? $_POST['sbdid'][$ano] : $sbdid);
// 							$db->executar("DELETE FROM par.subacaoemendapta WHERE sbdid = ".$sbdid);
// 							foreach( $_POST['emenda'][$ano] as $emdid ){
// 								$sql = "INSERT INTO par.subacaoemendapta(sbdid, emdid, ptrid)
// 										VALUES ({$sbdid}, $emdid, {$_POST['pta'][$ano]})";
// 								$db->executar( $sql );
// 							}
// 						}


                    $sql = "UPDATE par.subacao
								   SET usucpf='{$_SESSION['usucpf']}', sbadataalteracao='NOW()'
								 WHERE sbaid = " . $_POST['sbaid'][$ano];

                    $oSbd->executar($sql);

                    $oSbd->commit();
                }

                gravarHistoricoSubacaoDetalhe($_POST);
                $sbaidRequest = ($_POST['carregarSbaid'] ? $_POST['carregarSbaid'] : $_POST['sbaid'][$_POST['anoatual']]);
               $success = true;
            }
        }
    }

    if($success)
    {
    	
    	if($_POST['sbaid'][$ano] && $ano)
    	{
	    	atualizaValorSubacaoDetalhe($_POST['sbaid'][$ano], $ano);
		}
    	echo "<script>
    	alert('Dados salvos com sucesso!');
    	location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$_POST['sbaid'][$ano]}&anoatual={$ano}';
    	</script>";
    }
    
    if ($_POST['carregarSbaid']) {
        //header("Location: par.php?modulo=principal/subacao&acao=A&sbaid={$_POST['carregarSbaid']}");
        echo "<script>
					location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$_POST['carregarSbaid']}';
			 </script>";
    }
    
}

$sbaid = $_GET['sbaid'];
if (empty($sbaid)) {
    echo "<script>alert('Subação não encontrada!');window.close();</script>";
    exit;
}

function cancelarReformulacao() {

    global $db;

    $ano = $_POST['anoatual'];
    $sbaid = $_POST['sbaid'][$ano];
    $motivo = $_POST['cancela_' . $ano];

    if (cancelaReformulacao($sbaid, $ano, $motivo)) {	

		$resposta = enviaEmailDiligCancelReprogramacao($motivo,$sbaid, $ano,'C');

		if( ! $resposta )
		{
			$email = "Erro ao enviar o email para o responsável pela unidade.";
		}
		else
		{
			$email = "Email enviado ao responsável da unidade";
		}
		
		
        echo "	<script>
					alert('Reprogramação cancelada com sucesso! {$email}');
					location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$sbaid}';
		 		</script>";
    } else {

        echo "	<script>
					alert('Reprogramação não pôde ser cancelada!');
					location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$sbaid}';
		 		</script>";
    }
}

function enviaDiligencia() {
    global $db;
	
    $ano = $_POST['anoatual'];
    $sbaid = $_POST['sbaid'][$ano];
    $motivo = $_REQUEST["diligencia_$ano"];
    
    $sbdid = $db->pegaUm("SELECT sbdid FROM par.subacaodetalhe WHERE sbaid = " . $sbaid . " AND sbdano = " . $ano);

    $sql = "INSERT INTO par.reprogramacaosubacaodiligencia (sbdid, usucpf, rsddata, rsdmotivo, rsdstatus, rsdfluxo) VALUES 
															( " . $sbdid . ", '" . $_SESSION['usucpf'] . "', 'NOW()', '" . substr($_POST['diligencia_' . $ano], 0, 1200) . "', 'A', 'E' )";

    $db->executar($sql);

    $sql = "UPDATE par.subacaodetalhe SET ssuid = 23 WHERE sbdid = " . $sbdid;
    $db->executar($sql);

    $db->commit();
    
    $resposta = enviaEmailDiligCancelReprogramacao($motivo,$sbaid, $ano,'D');
    
    if( ! $resposta )
    {
    	$email = "Erro ao enviar o email para o responsável pela unidade.";
    }
    else
    {
    	$email = "Email enviado ao responsável da unidade";
    }
    
    echo "<script>
				alert('Reprogramação encaminhada para diligência com sucesso! {$email}');
				location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$sbaid}&ano={$ano}';
	 		</script>";
}

function finalizarReformulacao() {
    global $db;

    $ano = $_POST['anoatual'];
    $sbaid = $_POST['sbaid'][$ano];
    $ssuid = $_POST['ssuids'][$ano];

    if ($_POST['tipoReform'] == 'elaboracao') {

        //verifico se o valor mudou!!
        // se mudou, não atualizo e mando ele preencher o quadro. Se mudou, atualizo!!
        $sbaidPai = $db->pegaUm("SELECT MAX(sbaid) as sbaid FROM par.subacao WHERE sbaidpai = " . $sbaid);
        $dadosEmpenho = $db->pegaLinha("select eobpercentualemp, vve.vrlempenhocancelado as eobvalorempenho from par.empenhosubacao es
											inner join par.v_vrlempenhocancelado vve on vve.empid = es.empid
										where sbaid = " . $sbaid . " AND eobano = " . $ano." and eobstatus = 'A'");

        // Julio mandou tirar essa condição no dia 10/09/2013
//		if( $dadosEmpenho['eobpercentualemp'] == 100 ){ //Se ele foi 100% empenhado então tenho que verificar os valores.
// 			$valorPai = $dadosEmpenho['eobvalorempenho'];
// 			$valorFilho = retornaValorSubacao( $sbaid, $ano );

        $sql = "SELECT
					( par.recuperavalorplanejadossubacaoporano($sbaid, $ano) )
					-
					sum(valorempenho) from 
					(
					SELECT DISTINCT
						CASE WHEN emr.vrlreforco IS NOT NULL 
							THEN SUM(vve.vrlempenhocancelado+emr.vrlreforco)
							ELSE  SUM(vve.vrlempenhocancelado)
						END as valorempenho
					FROM
						par.empenhosubacao ems
					INNER JOIN par.empenho 			emp ON emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
					INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp.empid
					INNER JOIN par.processopar 		prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
					LEFT  JOIN par.subacaodetalhe 	sbd ON ems.sbaid = sbd.sbaid AND sbdano = $ano
					LEFT  JOIN (
							SELECT ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid
							FROM par.empenho ep
							INNER JOIN par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A'
							INNER JOIN par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
							WHERE ep.empcodigoespecie in ('02')
							GROUP BY ep.empnumeroprocesso, ep.empcodigoespecie, ep.empidpai, sd1.sbdid
							) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
					WHERE
					ems.sbaid = $sbaid  AND ems.eobano = $ano AND ems.eobstatus = 'A'
					GROUP BY
					emr.vrlreforco
					) as foo";

        $testaValores = $db->pegaUm($sql);

// 			if( $valorPai < $valorFilho ){ //Se estou tentando alterar o valor para um valor maior, então tenho que informar de onde virá a diferença.
        if ($testaValores > 0) {
            //verifico se já foi informado de onde virá o repasse
            $dadosRepasse = $db->pegaLinha("SELECT sbdrepassecomplementar, sbdrepasseempenho FROM par.subacaodetalhe WHERE sbaid = {$sbaid} AND sbdano = " . $ano);

            if ($dadosRepasse['sbdrepassecomplementar'] == '' && $dadosRepasse['sbdrepasseempenho'] == '') {

                echo "<script>
								alert('Informe como será feita a complementação da diferença do Empenho!');
								location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$sbaid}&ano={$ano}&repasse=1';
					 		</script>";
                exit;
            }
        }
//		}

        $sql = "UPDATE par.subacaodetalhe SET ssuid = 21 WHERE sbaid = " . $sbaid . " AND sbdano = " . $ano;
        $db->executar($sql);

        if ($ssuid == 23) {
            $sql = "UPDATE par.reprogramacaosubacaodiligencia SET rsdstatus = 'I' WHERE sbdid = " . $_POST['sbdid'][$ano] . "; ";
            $sql .= "INSERT INTO par.reprogramacaosubacaodiligencia (sbdid, usucpf, rsddata, rsdstatus, rsdfluxo) VALUES 
															( " . $_POST['sbdid'][$ano] . ", '" . $_SESSION['usucpf'] . "', 'NOW()', 'A', 'R' ); ";
            $db->executar($sql);
        }

        $db->commit();

        enviaEmailFimReformulacao($sbaid);
    } else {
        

        $sql = "UPDATE par.subacaodetalhe SET ssuid = 2, sbdrepitemvencido = false WHERE sbaid = " . $sbaid . " AND sbdano = " . $ano;
        $db->executar($sql);
        
        
         //IMPLEMENTAÇÃO SOLICITADA DIA 07/08/2014
         //CÓPIA DO ULTIMO COMPLEMENTO E ELIMINAÇÃO DOS VALORES DA ORIGINAL
        
        /*
        
        $sql2 = "SELECT max(s.sbaid)  FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid WHERE s.sbastatus <> 'I' AND (s.sbaidpai = ".$sbaid." OR s.sbaid = ".$sbaid.") AND sd.sbdano = " . $ano;
        $id_complemento_atual = $db->pegaUm($sql2);
        
        
        $sql3 = "SELECT s.sbaid, sbdrepassevlrcomplementar, sbdrepassevlrempenho, sbdrepassevlrraf, sbdrepassevlrcomplementaraprovado, sbdrepassevlrempenhoaprovado, sbdrepassevlrrafaprovado FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid WHERE s.sbastatus <> 'I' AND (s.sbaidpai = ".$sbaid." OR s.sbaid = ".$sbaid.") AND sd.sbdano = " . $ano . "order by s.sbaid asc limit 1" ;
        $dados_complemento_original = $db->carregar($sql3);
        
        
        if(empty($dados_complemento_original[0][sbdrepassevlrcomplementar]))
            $dados_complemento_original[0][sbdrepassevlrcomplementar] = 0;
        
         if(empty($dados_complemento_original[0][sbdrepassevlrempenho]))
            $dados_complemento_original[0][sbdrepassevlrempenho] = 0;
         
         if(empty($dados_complemento_original[0][sbdrepassevlrraf]))
            $dados_complemento_original[0][sbdrepassevlrraf] = 0;
         
         if(empty($dados_complemento_original[0][sbdrepassevlrcomplementaraprovado]))
            $dados_complemento_original[0][sbdrepassevlrcomplementaraprovado] = 0;
         
         if(empty($dados_complemento_original[0][sbdrepassevlrempenhoaprovado]))
            $dados_complemento_original[0][sbdrepassevlrempenhoaprovado] = 0;
         
         if(empty($dados_complemento_original[0][sbdrepassevlrrafaprovado]))
            $dados_complemento_original[0][sbdrepassevlrrafaprovado] = 0;
               
        $sql4 = "UPDATE par.subacaodetalhe SET  "
                . "sbdrepassevlrcomplementar = '".$dados_complemento_original[0][sbdrepassevlrcomplementar]."',  "
                . "sbdrepassevlrempenho = '".$dados_complemento_original[0][sbdrepassevlrempenho]."',  "
                . "sbdrepassevlrraf = '".$dados_complemento_original[0][sbdrepassevlrraf]."',  "
                . "sbdrepassevlrcomplementaraprovado = '".$dados_complemento_original[0][sbdrepassevlrcomplementaraprovado]."',  "
                . "sbdrepassevlrempenhoaprovado = '".$dados_complemento_original[0][sbdrepassevlrempenhoaprovado]."',  "
                . "sbdrepassevlrrafaprovado = '".$dados_complemento_original[0][sbdrepassevlrrafaprovado]."' "
              
               
                . "WHERE sbaid = " . $id_complemento_atual . " AND sbdano = " . $ano;
        $db->executar($sql4);
        
    
        $sql5 = "UPDATE par.subacaodetalhe SET  "
                . "sbdrepassevlrcomplementar = '0',  "
                . "sbdrepassevlrempenho = '0',  "
                . "sbdrepassevlrraf = '0',  "
                . "sbdrepassevlrcomplementaraprovado = '0',  "
                . "sbdrepassevlrempenhoaprovado = '0',  "
                . "sbdrepassevlrrafaprovado = '0' "              
                . "WHERE sbaid = " . $dados_complemento_original[0][sbaid] . " AND sbdano = " . $ano;
        $db->executar($sql5); */
        $db->commit(); 
        
        
    }
    echo "<script>
				alert('Reprogramação finalizada com sucesso!');
				location.href='par.php?modulo=principal/subacao&acao=A&sbaid={$sbaid}';
	 		</script>";
}
function montaListaEscolaSubacaoControle(){
    global $db;
    
    $arrParam = array(
        'inuid' => $_POST['inuid'],
        'sbaid' => $_POST['sbaid'],
        'ano' => $_POST['ano']
    );
    
    $arrEscola = montaListaEscolaSubacao($arrParam);    
    $cabecalhoL = array('Ação', 'Município', 'Escola','Código INEP', 'Tipo da Escola' );
    $db->monta_lista_array($arrEscola, $cabecalhoL,500,5,"N","center");
    die();    
}

if ($_POST['requisicao']) {
    ob_clean();
    $_POST['requisicao']();
}

/** Declaração de Variaveis * */
global $db;


/** Carrega subações próxima e anterior * */
if ($inuid) {
    $sql = "SELECT  s.sbaid
			FROM par.subacao 	 	 s
			INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
			INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
			INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
			INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
			INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
			INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
			WHERE s.sbastatus = 'A' AND
				p.inuid = {$inuid}
			ORDER BY d.dimcod,
					 ar.arecod,
					 i.indcod,
					 s.sbaordem,
					 s.sbadsc";

    $subacoes = (array) $db->carregar($sql);

    foreach ($subacoes as $key => $subacao) {
        if ($subacao['sbaid'] == (integer) $_GET['sbaid']) {
            $sbaidAnterior = $subacoes[$key - 1]['sbaid'];
            $sbaidProximo = $subacoes[$key + 1]['sbaid'];
            break;
        }
    }
}

/** Carrega informações de localização e dados da subação  * */
$sql = "SELECT  s.sbaid,
				s.sbadsc,
				s.foaid,
				d.dimid,
				d.dimcod,
				d.dimdsc,
				d.itrid,
				ar.areid,
				ar.arecod,
				ar.aredsc,
				i.indcod,
				i.indid,
				i.inddsc,
				a.acidsc,
				p.ptoid,
				s.sbaordem,
				frm.frmid,
				s.sbacronograma,
				s.ppsid,
				s.ptsid,
				s.docid,
				s.sbaextraordinaria,
				s.sbareformulacao,
				pps.ppscarga,
				COALESCE(s.sbaestrategiaimplementacao,'N/A') as sbaestrategiaimplementacao,
				COALESCE(u.unddsc,'N/A') as unddsc,
				COALESCE(f.foadescricao,'N/A') as foadescricao,
				s.prgid,
				COALESCE(prg.prgdsc,'N/A') as prgdsc,
				prg.prglink,
				COALESCE(frm.frmdsc,'N/A') as frmdsc,
				COALESCE(tps.tpsdescricao, COALESCE(pts.ptsdescricao,'N/A')) as ptsdescricao,
				s.sbaextraordinariatramita,
				CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf
		FROM par.subacao 	 	 s
		INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus IN ('A','E')
		INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus IN ('A','E')
		INNER JOIN par.instrumentounidade iu  ON iu.inuid = p.inuid
		INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus IN ('A','E')
		INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
		INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
		INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
		LEFT JOIN par.formaatendimento f ON f.foaid = s.foaid
		LEFT JOIN par.propostatiposubacao pts ON pts.ptsid = s.ptsid
		LEFT JOIN par.tiposubacao tps on tps.tpsid = pts.tpsid
		LEFT JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
		LEFT JOIN par.unidademedida u ON u.undid = s.undid
		LEFT JOIN par.programa prg ON prg.prgid = s.prgid
		LEFT JOIN par.formaexecucao frm ON frm.frmid = s.frmid
		WHERE s.sbastatus IN ('A','R') AND s.sbaid = $sbaid";

$subacao = $db->pegaLinha($sql);
$subacaoPrograma = $subacao['prgid'] ? $subacao['prgid'] : '0';
$estadoAtualSubacao = wf_pegarEstadoAtual($subacao['docid']);

if ($estadoAtualSubacao['esdid'] == WF_SUBACAO_DILIGENCIA) {

    $sql = "SELECT
				true
			FROM
				workflow.historicodocumento hst
			INNER JOIN workflow.documento doc ON doc.docid = hst.docid AND doc.tpdid = " . WF_TPDID_SUBACOES_PAR . "
			INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem AND esd.esdid = " . WF_SUBACAO_DILIGENCIA_CONDICIONAL . "
			WHERE doc.docid = {$subacao['docid']}";

    $testaDiligenciaCondicional = $db->pegaUm($sql);
}

$arrPerfil = pegaPerfilGeral();

$valida = "true";
if (!in_array(PAR_PERFIL_ORGAO_DE_CONTROLE, $arrPerfil))
    $habil = "S";
else
    $habil = "N";
$merito = "N";
$ssuid = array();

/*
  if( $subacao['ppscarga'] == 'f' ){
  $valida = "false";
  $habil = "N";
  }
 */

$docid = parPegarDocidParaEntidade($inuid);
$estadoAtual = wf_pegarEstadoAtual($docid);

$sbaextraordinariatramita = $subacao['sbaextraordinariatramita'];

if ($estadoAtual['esdid'] == WF_ELABORACAO && !$subacao['docid']) { // Caso a subação não tenha docid e o par esteja em elaboração entao a subação estará em elaboração
    $estadoAtualSubacao["esdid"] = WF_SUBACAO_ELABORACAO;
}

if ($subacao['ppsid']) {
    $sql = "SELECT DISTINCT
				pra.praanos
			FROM par.propostasubacaoanos psa
			INNER JOIN par.propostaanos pra ON pra.praid = psa.praid
			WHERE
				pra.prastatus = 'A' AND
				psa.ppsid = " . $subacao['ppsid'] . "
			ORDER BY
				pra.praanos";

    $anos = $db->carregarColuna($sql);
}

if (empty($anos))
    $anos = array(2011, 2012, 2013, 2014, 2015);

// Solicitado pelo Julio no dia 06/12 ( código 23 adicionado no dia 11/12 a pedido do Julio)
if ($subacao['sbacronograma'] == 2) {
    $tipos = array(14, 5, 7, 19, 20);
} else {
    $tipos = array(14, 5, 7, 19, 20, 8, 9, 52, 55, 56, 23);
}

if (in_array($subacao['ptsid'], $tipos)) {
    $detalha = true;
} else {
    $detalha = false;
}

//pega os perfis do usuario
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

//recupera docid e o estado atual do documento - esdid
$docid = parPegarDocidParaEntidade($inuid);
$esdid = pgPegarEstadoAtual($docid);

if (!$db->testa_superuser()) {
    if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE &&
            ( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
            (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO || $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES) && $permitido == 'S') ||
            (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) && $permitido == 'S') ) ||
            (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) )
            )
    ) {
        $habilParecer = "S";
    } else {
        $habilParecer = "N";
    }

    if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO &&
            ( in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
            in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
            in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) ||
            in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
            in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil)
            )
    ) {
        $validaSalvar = "true";
        $habilSalvar = "S";
        $habilStatus = "N";
    } else if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE &&
            ( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
            (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO || $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES) && $permitido == 'S') ||
            (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) && $permitido == 'S') ) ||
            (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) )
            )
    ) {
        $validaSalvar = "true";
        $habilSalvar = "S";
        $habilStatus = "S";
        $habil = "S";
        $valida = "true";
        $permitido = "S";
    } else {
        $validaSalvar = "false";
        $habilSalvar = "N";
        $habilStatus = "N";
    }

    /* if(in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)){
      $valida = "true";
      $habil = "S";
      $validaSalvar = "true";
      $habilSalvar = "S";
      $habilStatus = "S";
      } */

    if (in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil)) {
        $merito = "S";
    }

    if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $arrayPerfil)) {
        $arqid = $db->pegaUm("SELECT arqid FROM par.protocolo WHERE terid = 1 AND inuid = " . $_SESSION['par']['inuid']);
        if (!$arqid) {
            $vinculaObra = "S";
            $permitido = "S";
            $valida = "true";
            $habil = "S";
            $validaSalvar = "true";
            $habilParecer = "S";
            $habilSalvar = "S";
            $habilStatus = "S";
            $validaBP = true;
        }
    }

    if (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil)) {
        $permitido = verifcaMunEstUsusario(PAR_PERFIL_EQUIPE_FINANCEIRA);
    }
    if (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil)) {
        $permitido = $permitido == 'S' ? 'S' : verifcaMunEstUsusario(PAR_PERFIL_EQUIPE_TECNICA);
    }
} else {
    $vinculaObra = "S";
    $permitido = "S";
    $valida = "true";
    $habil = "S";
    $validaSalvar = "true";
    $habilParecer = "S";
    $habilSalvar = "S";
    $habilStatus = "S";
    $habilita_combo_Plano_PTR = 'N';
    $habilita_combo_PTA = 'N';

    if (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) {
        $habilita_combo_Plano_PTR = 'S';
        $habilita_combo_PTA = 'S';
    }
}

if ($_REQUEST['anoatual']) {
    $sbaid = isset($_REQUEST['sbaid'][$_REQUEST['anoatual']]) ? $_REQUEST['sbaid'][$_REQUEST['anoatual']] : $_REQUEST['sbaid'];
} else {
    $sbaid = $_REQUEST['sbaid'];
}


$sql = "SELECT true FROM par.subacao WHERE sbaid = $sbaid AND sbaidpai IS NOT NULL";

$testaCopia = $db->pegaUm($sql);

if ($testaCopia == 't') {
    $vinculaObra = "N";
    $permitido = "N";
    $valida = "false";
    $habil = "N";
    $validaSalvar = "false";
    $habilParecer = "N";
    $habilSalvar = "N";
    $habilStatus = "N";
    $habilita_combo_Plano_PTR = 'N';
    $habilita_combo_PTA = 'N';
}

?>
<html>
    <head>
        <meta http-equiv="Cache-Control" content="no-cache">
        <meta http-equiv="Pragma" content="no-cache">
        <meta http-equiv="Connection" content="Keep-Alive">
        <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
        <title>PAR 2010 - Plano de Metas - Subação</title>

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>

        <script type="text/javascript" src="../includes/funcoes.js"></script>
	    
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

        <style>
            .disabled_textarea{
                border:none;
                border-left:#888888 3px solid;
                color:#404040;
                width:100ex;
            }
        </style>

        <script>
            $(function() {

<? //if( $testaCopia == 't' && !in_array($_SESSION['usucpf'],Array('','','','')) ){    ?>
<? if ($testaCopia == 't') { ?>
                    $('input').attr('disabled', 'disabled');
                    $('[name*="btn_salvar"],[name*="btn_importar"],[name*="btn_limpar"]').remove();
                    $('select').attr('disabled', 'disabled');
                    $('[src*="excluir"]').remove();
<? } ?>

                $.each($('textarea[name^="sbdparecer["]'), function(i, v) {
                    if (v.value != '') {
                        var ano = v.id.replace('sbdparecer[', '').replace(']', '');
                        $(v).attr('disabled', true).addClass('disabled_textarea').removeClass('txareanormal');
                        $('#div_adiciona_parecer_' + ano).show();
                    }
                });

                $.each($('textarea[name^="sbdparecerdemerito["]'), function(i, v) {
                    if (v.value != '') {
                        var ano = v.id.replace('sbdparecerdemerito[', '').replace(']', '');
                        $(v).attr('disabled', true).addClass('disabled_textarea').removeClass('txareanormal');
                        $('#div_adiciona_merito_' + ano).show();
                    }
                });

//				$('textarea[name^="sbdparecer"]').attr('disabled', true).addClass('disabled_textarea').removeClass('txareanormal');

                $('.btnAddParecer').click(function() {

                    var ano = this.id.split('_')[1];
                    $('[name=sbdparecer[' + ano + ']]').attr('disabled', false).removeClass('disabled_textarea').addClass('txareanormal');
                    $('[name=bo_alterar_parecer[' + ano + ']]').val(ano);

                });

                $('.btnAddMerito').click(function() {

                    var ano = this.id.split('_')[1];
                    $('[name=sbdparecerdemerito[' + ano + ']]').attr('disabled', false).removeClass('disabled_textarea').addClass('txareanormal');
                    $('[name=bo_alterar_merito[' + ano + ']]').val(ano);

                });

                $('.mostrar_ocultar_historico').click(function() {
                    $('.img_mais_menos, #div_historico_subacao').toggle();
                });

            });
        </script>
<?php 
//include_once APPRAIZ."par/modulos/principal/empenho/redistribuicaoEmpenhoProcessoSubacao.inc";
?>
    </head>
    <body>
		<!--<div id="div_dialog" style="display:none"></div>-->
        <form name="form_subacao" id="form_subacao" method="post" action="" >
            <input type="hidden" name="requisicao" value="salvarSubacao"  />
            <input type="hidden" name="anoatual" id="anoatual" value="<?php echo $_GET['anoatual']; ?>"  />
            <input type="hidden" name="tipoReform" id="tipoReform" value=""  />
            <input type="hidden" name="quant" id="quant" value="<?php echo $subacao['sbacronograma']; ?>"  />
            <input type="hidden" name="frmid" id="frmid" value="<?php echo $subacao['frmid']; ?>"  />
            <input type="hidden" name="ptsid" id="ptsid" value="<?php echo $subacao['ptsid']; ?>"  />
            <input type="hidden" name="docid" id="docid" value="<?php echo $subacao['docid']; ?>"  />
            <input type="hidden" name="esdid" id="esdid" value="<?php echo $estadoAtualSubacao["esdid"] ?>"  />
            <input type="hidden" name="reformulacao" id="reformulacao" value="<?php echo $subacao['sbareformulacao']; ?>"  />
            <input type="hidden" name="detalha" value="<?php echo $detalha; ?>"  />
            <input type="hidden" name="carregarSbaid" value=""  />
            <!--
            /*
             * REGRA QUE LIBERA EDIÇÃO PARA OS USUÁRIOS
             * RENILDA PERES DE LIMA
             * JULIO CEZAR DA CAMARA RIBEIRO VIANA
             * CRIADA EM 07/08/2012 POR WESCLEY
             * DEMANDANTE: THIAGO TASCA
             */
            -->
            <input type="hidden" name="subacaoAnalise" value="<?php echo in_array($_SESSION['usucpf'], array('', '')) ? 'analise' : ''; ?>"  />
            <?php
            if ($_REQUEST['inuid']) {
                $verificaInstrumento = $db->pegaLinha("SELECT itrid, estuf, muncod FROM par.instrumentounidade WHERE inuid = " . $_REQUEST['inuid']);
                if ($verificaInstrumento['itrid'] == 1) {
                    $_SESSION['par']['estuf'] = $verificaInstrumento['estuf'];
                } else {
                    $_SESSION['par']['muncod'] = $verificaInstrumento['muncod'];
                }
            }
            ?>
            <style>
                .carimbo{
                    width:80px;
                }
                .div_carimbo_ref{
                    position:		absolute;
                    float:			right;
                    margin-left:	50%;
                }
            </style>
            <?php
            $sql = "SELECT
						sbdano
					FROM 
						par.subacaodetalhe 
					WHERE
						sbaid = $sbaid
						AND ( ssuid in (20,21,23) OR sbdreformulacao IS TRUE)";

            $sbdano = $db->carregarColuna($sql);

            if ($sbdano[0] != '') {
                ?>
                <div class="div_carimbo_ref">
                    <?php foreach ($sbdano as $anoR) { ?>
                        <img border="0" class="carimbo" align="absmiddle" src="../imagens/par/carimbo-reformulacao-<?= $anoR ?>.png">
                    <?php } ?>
                </div>
                <?php
            }
            ?>
            <table width="100%">
                <tr>
                    <td valign="top">
                        <table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela tabelasubacao">
                            <colgroup>
                                <col width="25%" />
                                <col width="75%" />
                            </colgroup>
                            <tbody>
                                <tr>
                                    <td colspan="2" align="center" style="background-color: #cccccc;" >
                                        <b><font size="3px">Subação</font></b>
                                    </td>
                                </tr>
                                <tr>
                                    <td colspan="2" style="background-color:#F5F5F5;color: blue; font-size: 22px">
                                        <a href="javascript:exibeArvore()">
                                            <?php //$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
                                            <?php $entidadePar = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN estuf ELSE muncod END as entidade FROM par.instrumentounidade WHERE inuid = " . $inuid); ?>
                                            <?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
                                        </a>
                                    </td>
                                </tr>
                                <tr id="buttonsContainer">
                                    <td class="tdbtncontainerleft">
                                        <input onclick="controleproxAntSubacao('<?php echo $sbaidAnterior; ?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo!$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
                                    </td>
                                    <td align="right" class="tdbtncontainerRight" >
                                        <input onclick="controleproxAntSubacao('<?php echo $sbaidProximo; ?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo" <?php echo!$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita">Dimensão:</td>
                                    <td><?php echo $subacao['dimcod'], '. ', $subacao['dimdsc'] ?></td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita">Área:</td>
                                    <td><?php echo $subacao['dimcod'], '.', $subacao['arecod'], '. ', $subacao['aredsc']; ?></td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita">Indicador:</td>
                                    <td>
                                        <?php echo $subacao['dimcod'], '.', $subacao['arecod'], '.', $subacao['indcod'], '. ', $subacao['inddsc']; ?>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita">Ação:</td>
                                    <td><?php echo $subacao['acidsc']; ?></td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita">Tipo da subação:</td>
                                    <td><?php echo $subacao['ptsdescricao']; ?></td>
                                </tr>
                                <tr style="background-color: #cccccc;">
                                    <td align="left" colspan="2">
                                        <strong>Dados da Subação</strong>
                                    </td>
                                </tr>
                                <!--
                                <tr>
                                        <td align='right' class="SubTituloDireita">Forma de atendimento:</td>
                                        <td><? // echo $subacao['foadescricao'];    ?></td>
                                </tr>
                                -->
                                <tr>
                                    <td align='right' class="SubTituloDireita">Descrição da Subação:</td>
                                    <td><?php echo $subacao['dimcod'] . "." . $subacao['arecod'] . "." . $subacao['indcod'] . "." . $subacao['sbaordem'] . " - " . $subacao['sbadsc']; ?></td>
                                </tr>
                                <tr>
                                    <td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
                                    <td><?php echo $subacao['sbaestrategiaimplementacao'] ?></td>
                                </tr>
                                <tr>
                                    <td align='right' class="SubTituloDireita">Programa:</td>
                                    <td><?php echo $subacao['prgdsc'] ?></td>
                                </tr>
                                <?php
                                if ($subacao['prglink']) {
                                    if (strpos($subacao['prglink'], 'http://') === false) {
                                        $link = 'http://' . $subacao['prglink'];
                                    } else {
                                        $link = $subacao['prglink'];
                                    }
                                    ?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">Mais informações sobre o Programa:</td>
                                        <td><a target="_blank" href="<?php echo $link ?>"><?php echo $link ?></a></td>
                                    </tr>
                                <?php } ?>
                                <?php if ($subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $subacao['frmid'] != ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) { ?>
                                    <tr>
                                        <td align="right" class="SubTituloDireita">Unidade de Medida:</td>
                                        <td><?php echo $subacao['unddsc'] ?></td>
                                    </tr>
                                <?php } ?>
                                <tr>
                                    <td align="right" class="SubTituloDireita">Forma de Execução:</td>
                                    <td><?php echo $subacao['frmdsc'] ?></td>
                                </tr>
                                <tr>
                                    <td align='right' class="SubTituloDireita">Cronograma:</td>
                                    <td><?php echo $subacao['sbacronograma'] == 1 ? "Global" : ( $subacao['sbacronograma'] == 2 ? "Escola" : "N/A" ) ?></td>
                                </tr>

                                <?php
                                $onibusAcessivel = isSubacaoOnibusAcessivel($_REQUEST);

                                if ($onibusAcessivel["temCota"]) {
                                    echo "
										<tr style='background-color: #cccccc;'>
											<td align='left' colspan='2'>
											<strong>Dados transporte escolar acessível</strong>
											</td> 
										</tr>
										<tr>
											<td align='right' class='SubTituloDireita'>Cota Inicial:</td>
											<td>{$onibusAcessivel["cotaInicial"]}</td>
										</tr>
										<tr>
											<td align='right' class='SubTituloDireita'>Cota Atualizada( Cota Inicial - Qtd Empenhada):</td>
											<td>{$onibusAcessivel["cotaAtual"]}</td>
										</tr>";
                                }

                                $sqlVerEmp = "select
														count(emp.empid)
													from
														par.empenhosubacao ems
													inner join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A' and empstatus = 'A'
													inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
													left  join par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
													left  join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid -- and sbdano = {$ano}
													left  join (
														select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
														from par.empenho ep
															inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A'
															inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
														where ep.empcodigoespecie in ('02')
														group by 
															ep.empnumeroprocesso,
															ep.empcodigoespecie,
															ep.empidpai,
															sd1.sbdid
													) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
													where
														ems.sbaid = " . $sbaid;
                                $verifica = $db->pegaUm($sqlVerEmp);

                                if ($verifica == 0 && $testaCopia != 't') {
                                    if (
                                            in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                            in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                            (
                                            (
                                            in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                            in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
                                            ) && $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO )
                                    ) {

                                        if ($subacao['itrid'] != 3 || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)) {
                                            ?>
                                            <tr>
                                                <td align="right" class="SubTituloDireita">&nbsp;</td>
                                                <td id="TdEditarSubacao" bgcolor="#dddddd"><input type="button" name="BtnEditarSubacao" id="BtnEditarSubacao" value="Editar" onclick="popupEditarSubacao('<?php echo $sbaid ?>');" /></td>
                                            </tr>
                                            <?php
                                        }
                                    }
                                }
                                ?>
                            </tbody>
                        </table>

                        <table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
                            <tr id="abasSelecaoCosano">
                                <?php
                                $colspan = 5;
                                $tamanhoCelula = round(100 / $colspan);
                                foreach ($anos as $ano) {
                                    $bloquearAnoSubacaoReprogramacao = desabilitarAnoSubacaoReprogramacao($sbaid, $ano);
                                    echo "<td id=\"aba_$ano\" style=\"width: 50px;\" ><a href=\"javascript:selecionarAba('$ano', '$bloquearAnoSubacaoReprogramacao')\">$ano</a></td>";
                                }
                                ?>
                                <td id="aba_totalizadores" style="width: 80px;"><a href="javascript:selecionarAba('totalizadores', '<?php echo $bloquearAnoSubacaoReprogramacao ?>')" >Totalizadores<br></a></td>
                            </tr>
                            <?php
                            $arrMeses = array(
                                0 => array("codigo" => 1, "descricao" => "Janeiro"),
                                1 => array("codigo" => 2, "descricao" => "Fevereiro"),
                                2 => array("codigo" => 3, "descricao" => "Março"),
                                3 => array("codigo" => 4, "descricao" => "Abril"),
                                4 => array("codigo" => 5, "descricao" => "Maio"),
                                5 => array("codigo" => 6, "descricao" => "Junho"),
                                6 => array("codigo" => 7, "descricao" => "Julho"),
                                7 => array("codigo" => 8, "descricao" => "Agosto"),
                                8 => array("codigo" => 9, "descricao" => "Setembro"),
                                9 => array("codigo" => 10, "descricao" => "Outubro"),
                                10 => array("codigo" => 11, "descricao" => "Novembro"),
                                11 => array("codigo" => 12, "descricao" => "Dezembro")
                            );
                            ?>
                            <?php foreach ($anos as $ano) {

                                $bloquearAnoSubacaoReprogramacao = desabilitarAnoSubacaoReprogramacao($sbaid, $ano);
                            	$finalizado = false;                            	
                            	$sql = "SELECT
											prp.prpfinalizado
										FROM
											par.processoparcomposicao ppc
										INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
										INNER JOIN par.processopar prp ON prp.prpid = ppc.prpid
										WHERE
											sd.sbaid = ".$sbaid." and sd.sbdano = $ano";
                            	$finalizado = $db->pegaUm($sql);
                            	?>
                                <tr id="abaAnos<?php echo $ano; ?>" class="abaAnos">
                                    <td colspan="<?php echo $colspan; ?>" >
                                        <?php
                                        $valida = "false";
                                        $habil = "N";
                                        $validaSalvar = "false";
                                        $habilSalvar = "N";
                                        $habilStatus = "N";

                                        $oSbd = new SubacaoDetalhe();
                                        $sbdid = $oSbd->retornaDetalheSubacao($ano, $sbaid);
                                        $sbdid ? $oSbd->carregarPorId($sbdid) : "";
                                        $ssuid[$ano] = $oSbd->ssuid;

                                        if (!$db->testa_superuser()) {
                                            if ($subacao['ppscarga'] == 'f') {
                                                $valida = "false";
                                                $habil = "N";
                                                $validaSalvar = "false";
                                                $habilSalvar = "N";
                                                $habilStatus = "N";
                                            }
                                            if ($subacao['sbaextraordinaria'] !== '' && $subacao['sbaextraordinaria']) {
                                                $valida = "false";
                                                $habil = "N";
                                                $validaSalvar = "false";
                                                $habilSalvar = "N";
                                                $habilStatus = "N";
                                            }

                                            if (
                                                    (
                                                    $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO ||
                                                    (
                                                    $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA &&
                                                    ($ssuid[$ano] == 7 || $ssuid[$ano] == 10)
                                                    ) ||
                                                    (
                                                    $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL &&
                                                    ($ssuid[$ano] == 22)
                                                    ) ||
                                                    ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ATUALIZACAO && $_SESSION['par']['itrid'] == 1)
                                                    ) &&
                                                    ( in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil)
                                                    )
                                            ) {
                                                $valida = "true";
                                                $habil = "S";
                                                $validaSalvar = "true";
                                                $habilSalvar = "S";
                                                $habilStatus = "N";
                                            } else if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE &&
                                                    (
                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $arrayPerfil) ||
                                                    (
                                                    ( in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO ) ||
                                                    ( $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES ) && $permitido == 'S'
                                                    ) ||
                                                    ( in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && ( ($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) && $permitido == 'S' ) ) ||
                                                    ( in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ( $subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS ) )
                                                    )
                                            ) {
                                                $validaSalvar = "true";
                                                $habilSalvar = "S";
                                                $habilStatus = "S";
                                                $habil = "S";
                                                $valida = "true";
                                                $habilParecer = "S";
                                            } else if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA && ($ssuid[$ano] != 7 || $ssuid[$ano] != 10)) {
                                                $validaSalvar = "false";
                                                $habilSalvar = "N";
                                                $habilStatus = "N";
                                                $habil = "N";
                                                $valida = "false";
                                            }

                                            if (in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) {
                                                $vinculaObra = "S";
                                            }

                                            // Se for de Emendas Parlamentares trava
                                            // A pedido do Julio, Em diligencia pode mexer.
                                            // A pedido do Julio, Em Elaboração pode mexer. (07/12/12)
                                            $emendas = array(14, 15);
                                            if (in_array($subacao['frmid'], $emendas)) {
                                                if (in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $arrayPerfil) ||
                                                        $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA ||
                                                        $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO) {
                                                    $validaSalvar = "true";
                                                    $habilSalvar = "S";
                                                    $habilStatus = "S";
                                                    $habilParecer = "S";
                                                    $habil = "S";
                                                    $valida = "true";
                                                } else {
                                                    $validaSalvar = "false";
                                                    $habilSalvar = "N";
                                                    $habilStatus = "N";
                                                    $habilParecer = "N";
                                                    $habil = "N";
                                                    $valida = "false";
                                                }
                                            }
                                            if ($ano < date("Y") &&
                                                    (!($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA ||
                                                    $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL ||
                                                    $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO) &&
                                                    (comparaValorArray(unserialize(GRUPO_DE_PERFIL_MUNICIPAL_EDICAO), $arrayPerfil, 'boolean') ||
                                                    comparaValorArray(unserialize(GRUPO_DE_PERFIL_ESTADUAL_EDICAO), $arrayPerfil, 'boolean')
                                                    )
                                                    )
                                            ) {
                                                $validaSalvar = "false";
                                                $habilSalvar = "N";
                                                $habilStatus = "N";
                                                $habil = "N";
                                                $valida = "false";
                                                $habilParecer = "S";
                                            }
                                        } else {
                                            $valida = "true";
                                            $habil = "S";
                                            $validaSalvar = "true";
                                            $habilSalvar = "S";
                                            $habilStatus = "S";
                                        }

                                        $SQLsubacaoFoiEmpenhada = "select eobano from par.empenhosubacao where sbaid = " . $sbaid." and eobstatus = 'A'";
                                        $anosEmpenhados = $db->carregar($SQLsubacaoFoiEmpenhada);

                                        if (!$db->testa_superuser() && !in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) && !in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil)) {
                                            if (is_array($anosEmpenhados) && $anosEmpenhados[0]) {
                                                foreach ($anosEmpenhados as $anos1) {
                                                    if ($anos1["eobano"] == $ano) {
                                                        $valida = "false";
                                                        $habil = "N";
                                                        $validaSalvar = "false";
                                                        $habilSalvar = "N";
                                                        $habilStatus = "N";
                                                    }
                                                }
                                            }
                                        }

                                        // SE EM REFORMULAÇÃO
                                        $reformulacao = "false";
                                        $habilItemPregaoVencido = false;
                                        if ($subacao['sbareformulacao'] == 't') {
                                            $sbaidpai = $db->pegaUm("SELECT MAX(sbaid) as sbaid FROM par.subacao WHERE sbaidpai = " . $sbaid);
//												$ssuid = $db->pegaUm("SELECT ssuid FROM par.subacaodetalhe WHERE sbaid = ".$sbaid." AND sbdano = ".$ano);
                                            //if(in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil)){
                                            if (
                                                    (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil)
                                                    ) && ($ssuid[$ano] == 20 || $ssuid[$ano] == 23)) {
                                                $reformulacao = "true";
                                                $valida = "true";
                                                $habil = "S";
                                                $validaSalvar = "true";
                                                $habilParecer = "N";
                                                $habilSalvar = "N";
                                                $habilItemPregaoVencido = true;
                                                //$habilStatus 	= "N";
                                            } elseif ((in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) && $ssuid[$ano] == 21) {
                                                $reformulacao = "true";
                                                $valida = "true";
                                                $habil = "S";
                                                $validaSalvar = "true";
                                                $habilParecer = "S";
                                                $habilSalvar = "S";
                                                $habilItemPregaoVencido = true;
                                                //$habilStatus 	= "S";
                                            }
                                        }
                                        // Verifico se tem reforço
                                        $reforco = $db->pegaUm("SELECT sbdreforco FROM par.subacaodetalhe WHERE sbaid = " . $sbaid . " AND sbdano = " . $ano);

                                        if ($reforco && (in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil))) {
                                            $validaSalvar = "true";
                                            $habilSalvar = "S";
                                            $habilStatus = "S";
                                            $habilParecer = "S";
                                            $habil = "S";
                                            $valida = "true";
                                        }

                                        if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $arrayPerfil)) {
                                            $arqid = $db->pegaUm("SELECT arqid FROM par.protocolo WHERE terid = 1 AND inuid = " . $_SESSION['par']['inuid']);
                                            if (!$arqid) {
                                                $validaSalvar = "true";
                                                $habilSalvar = "S";
                                                $habilStatus = "S";
                                                $habilParecer = "S";
                                                $habil = "S";
                                                $valida = "true";
                                            }
                                        }

                                        $sbaidsJulio = array(4622979, 4622601, 4622625, 4622318, 4622953, 4622606, 4622600, 4622616, 4622952, 4622318, 4622318, 4622306, 4622319, 4621983, 4622594, 4623526, 4623776, 4623776);

                                        if (in_array($sbaid, $sbaidsJulio)) {
                                            $vinculaObra = "S";
                                            $permitido = "S";
                                            $valida = "true";
                                            $habil = "S";
                                            $validaSalvar = "true";
                                            $habilParecer = "S";
                                            $habilSalvar = "S";
                                            $habilStatus = "S";
                                            $validaBP = true;
                                        }

                                        /*
                                         * REGRA QUE LIBERA EDIÇÃO PARA OS USUÁRIOS
                                         * RENILDA PERES DE LIMA
                                         * JULIO CEZAR DA CAMARA RIBEIRO VIANA
                                         * CRIADA EM 07/08/2012 POR WESCLEY
                                         * DEMANDANTE: THIAGO TASCA
                                         */
                                        if (in_array($_SESSION['usucpf'], array('', ''))) {
                                            $validaSalvar = "true";
                                            $habilSalvar = "S";
                                            $habilStatus = "S";
                                            $habil = "S";
                                            $valida = "true";
                                        }

                                        if (in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)) {
                                            if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL || $estadoAtualSubacao["esdid"] == WF_SUBACAO_APROVACAO_CONDICIONAL) {
                                                $vinculaObra = "S";
                                                $permitido = "S";
                                                $valida = "true";
                                                $habil = "S";
                                                $validaSalvar = "true";
                                                $habilParecer = "S";
                                                $habilSalvar = "S";
                                                $habilStatus = "S";
                                                $validaBP = true;
                                            }
                                        }

                                        if (in_array(PAR_PERFIL_PREFEITO, $arrayPerfil)) {
                                            $reprogramacao = $db->pegaUm("SELECT sbdid FROM par.documentoparreprogramacaosubacao WHERE dpsstatus = 'A' AND sbdid IN ( SELECT sbdid FROM par.subacaodetalhe WHERE sbaid = " . $sbaid . " AND sbdano = " . $ano . ")");

                                            if ( ( $reprogramacao && in_array( $oSbd->ssuid, Array(20,23) ) ) || (($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) &&
                                                    ($oSbd->ssuid == 7 || $oSbd->ssuid == 10) )) {

                                                $vinculaObra = "S";
                                                $permitido = "S";
                                                $valida = "true";
                                                $habil = "S";
                                                $validaSalvar = "true";
                                                $habilParecer = "S";
                                                $habilSalvar = "S";
                                                $habilStatus = "N";
                                                $validaBP = true;
                                                $reprogr = true;
                                                $reformulacao = "true";
                                            }
                                        }

                                        $travReformulacao = false;
                                        $habilita_combo_Plano_PTR_ano = $habilita_combo_Plano_PTR;

                                        if ($sbdid) {
                                            $sql = "SELECT 
															sbdid 
														FROM 
															par.documentoparreprogramacaosubacao 
														WHERE 
															dpsstatus = 'P' 
															AND sbdid = $sbdid";

                                            $reprogramacao = $db->pegaUm($sql);

                                            if ($reprogramacao) {
                                                $vinculaObra = "N";
                                                $permitido = "N";
                                                $valida = "false";
                                                $habil = "N";
                                                $validaSalvar = "false";
                                                $habilParecer = "N";
                                                $habilSalvar = "N";
                                                $habilStatus = "N";
                                                $validaBP = false;
                                                $reprogr = false;
                                                $reformulacao = "false";
                                                $habilita_combo_Plano_PTR_ano = 'N';
                                                $habilita_combo_PTA = 'N';
                                                $travReformulacao = true;
                                            }
                                        }
                                        
                                        if( $finalizado == true ){
                                        	$vinculaObra = "N";
                                            $permitido = "N";
                                            $valida = "false";
                                            $habil = "N";
                                            $validaSalvar = "false";
                                            $habilParecer = "N";
                                            $habilSalvar = "N";
                                            $habilStatus = "N";
                                            $validaBP = false;
                                            $reprogr = false;
                                            $reformulacao = "false";
                                            $habilita_combo_Plano_PTR_ano = 'N';
                                            $habilita_combo_PTA = 'N';
                                            $travReformulacao = true;
                                        }
                                        ?>
                                        <input type="hidden" name="sbdid[<?php echo $ano ?>]" value="<?php echo $sbdid ?>" />
                                        <input type="hidden" name="sbaid[<?php echo $ano ?>]" value="<?php echo $sbaid ?>" />
                                        <input type="hidden" name="sbdano[<?php echo $ano ?>]" value="<?php echo $ano ?>" />
                                        <input type="hidden" name="ssuids[<?php echo $ano ?>]" value="<?php echo $oSbd->ssuid ?>" />
                                        <input type="hidden" name="merito[<?php echo $ano ?>]" value="<?php echo $merito ?>" />
                                        <input type="hidden" name="reforco[<?php echo $ano ?>]" value="<?php echo $reforco ?>" />
                                        <style>
                                            .botao_tr:hover{
                                                background-color: #F8F8F8;
                                            }
                                            .botao_tr{
                                                width: 100%;
                                                background-color: #E0E0E0;
                                                margin-top: 1px;
                                                padding-top : 6px;
                                                padding-bottom : 6px;
                                                border-radius: 5px; /*5px;*/
                                                cursor:pointer;
                                            }
                                            .cancelar:hover{
                                                background-color: #F8F8F8;
                                            }
                                            .cancelar{
                                                background-color: #E0E0E0;
                                            }
                                        </style>
                                        <table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
                                            <tr>
                                                <td colspan="2" class="SubTituloTabela">
                                                    <?php
                                                    if ( $reformulacao == 'true' && $testaCopia != 't' ) {

                                                        if ($ssuid[$ano] == 20 || $ssuid[$ano] == 23) {
                                                            if (
                                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)) {
                                                                ?>
                                                                <div onclick="finalizaReformulacao(<?= $sbaid ?>, 'elaboracao');" class="botao_tr">
                                                                    <a style="cursor: pointer" title="Finalizar Reprogramação">Finalizar Reprogramação</a>
                                                                </div>
                                                                <?php
                                                            }
                                                        } elseif ($ssuid[$ano] == 21) {
                                                            if (
                                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil)) {
                                                                ?>
                                                                <div onclick="finalizaReformulacao(<?= $sbaid ?>, 'analise');" class="botao_tr">
                                                                    <a style="cursor: pointer" title="Finalizar Análise da Reprogramação">Finalizar Análise da Reprogramação</a>
                                                                </div>
                                                                <div onclick="diligenciaReformulacao(<?= $sbaid ?>, <?= $ano ?>);" class="botao_tr">
                                                                    <a style="cursor: pointer" title="Enviar para Diligência de Reprogramação">Enviar para Diligência de Reprogramação</a>
                                                                </div>
                                                                <div id="diligencia_<?= $sbaid ?>_<?= $ano ?>" style="display: none;" >
                                                                    <table align="center" border="0">
                                                                        <tr>
                                                                            <td>Motivo da diligência:</td>
                                                                            <td>
                                                                                <?php echo campo_textarea("diligencia_" . $ano, 'N', 'S', '', 80, 7, 0, '', '', '', '', ''); ?>
                                                                            </td>
                                                                        </tr>
                                                                        <tr>
                                                                            <td colspan="2" align="center">
                                                                                <input type="button" value="Solicitar Diligência de Reprogramação" onclick="enviaDiligencia(<?= $sbaid ?>, <?= $ano ?>)">
                                                                            </td>
                                                                        </tr>
                                                                    </table>
                                                                </div>
<!--                                                            <div class="botao_tr"> 
                                                                    <label style="color:#003366;">Reprogramação do tipo remanejamento de empenho?</label>
                                                                    <input type="radio" name="sbdreformulacaoempenho[<?= $ano ?>]" id="sbdreformulacaoempenho_s_<?= $ano ?>" value="TRUE" <?= ($oSbd->sbdreformulacaoempenho == 't' ? 'checked="checked"' : '') ?> /> Sim
                                                                    <input type="radio" name="sbdreformulacaoempenho[<?= $ano ?>]" id="sbdreformulacaoempenho_n_<?= $ano ?>" value="FALSE" <?= ($oSbd->sbdreformulacaoempenho != 't' ? 'checked="checked"' : '') ?> /> Não
                                                                </div> -->
                                                                <?php
                                                            }
                                                        }
                                                    }
                                                    if ($ssuid[$ano] == 20 || $ssuid[$ano] == 21 || $ssuid[$ano] == 23) {
                                                        $testa = $db->pegaUm("SELECT sbaid FROM par.subacao WHERE sbaidpai = " . $sbaid);
                                                        if ((in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) && $testa) {
                                                            ?>
                                                            <div onclick="cancelaReformulacaoSubacao(<?= $sbaid ?>, <?= $ano ?>);" class="botao_tr cancelar">
                                                                <a style="cursor: pointer" title="Cancelar Reprogramação">Cancelar Reprogramação</a>
                                                            </div>
                                                            <div id="divcancela_<?= $sbaid ?>_<?= $ano ?>" style="display: none;" >
                                                                <table align="center" border="0">
                                                                    <tr>
                                                                        <td>Motivo do Cancelamento:</td>
                                                                        <td>
                                                                            <?php echo campo_textarea("cancela_" . $ano, 'N', 'S', '', 80, 7, 0, '', '', '', '', ''); ?>
                                                                        </td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td colspan="2" align="center">
                                                                            <input type="button" value="Solicitar Cancelamento da Reprogramação" onclick="cancelaReformulacao(<?= $sbaid ?>, <?= $ano ?>)">
                                                                        </td>
                                                                    </tr>
                                                                </table>
                                                            </div>
                                                            <?
                                                        }
                                                    }
                                                    ?>
                                                </td>
                                            </tr>
                                            <tr class="hidden" >
                                                <td colspan="2" class="SubTituloTabela">Execução Financeira</td>
                                            </tr>
                                            <tr class="hidden" id="planoInterno_<?php echo $ano; ?>">
                                                <td class="SubTituloDireita" width="40%">Plano Interno:</td>
                                                <td></td>
                                            </tr>
                                            <tr class="hidden" id="numeroDoProcesso_<?php echo $ano; ?>">
                                                <td class="SubTituloDireita">Número do Processo:</td>
                                                <td></td>
                                            </tr>
                                            <?php
                                            #Para esse objeto "Parecer da equipe técnica", não é adcionado nos perfis, clausulas de forma de execução, as que foram definidas pelas regras,
                                            #pois nesse momento oucutaria o objeto, não sendo necessário porque esse objeto é disabilitado. Dessa forma esse objeto obdece as regras definidas para os perfis.
                                            #Perfis esses:  PAR_PERFIL_EQUIPE_TECNICA, PAR_PERFIL_EQUIPE_FINANCEIRA
                                            if ((($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE_COMISSAO) &&
                                                    ( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil)
                                                    ) ) ||
                                                    ( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL || $estadoAtualSubacao["esdid"] == WF_SUBACAO_APROVACAO_CONDICIONAL) &&
                                                    ( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ) )
                                            ) {
                                                ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">Dados Orçamentários</td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita" width="40%">Plano Interno:</td>
                                                    <td>
                                                        <?php
                                                        $sqlPlanos = "
														SELECT 	DISTINCT
																plinumplanointerno as codigo,
																plinumplanointerno || ' - ' || pliano as descricao
														FROM
															par.planointerno
														WHERE pliano >= " . $ano . "  AND prgid = " . $subacaoPrograma;
                                                        $db->monta_combo("sbdplanointerno[$ano]", $sqlPlanos, $habilita_combo_Plano_PTR_ano, 'Selecione', 'filtraPTRES', '', '', '', 'N', 'sbdplanointerno', false, $oSbd->sbdplanointerno, 'Plano Interno');
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita">PTRES:</td>
                                                    <td id="td_ptres_<?php echo $ano; ?>">
                                                        <?php
                                                        $sql = "select
																	pliptres as codigo,
																	pliptres as descricao
																from par.planointerno
																where plinumplanointerno = '{$oSbd->sbdplanointerno}'";

                                                        $db->monta_combo("sbdptres[$ano]", $sql, $habilita_combo_Plano_PTR_ano, "Selecione...", '', "", "", "", "N", "", "", $oSbd->sbdptres, "PTRES");
                                                        ?>
                                                    </td>
                                                </tr>
                                            <?php } ?>
                                            <? if ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS) { ?>
                                                <tr>
                                                    <td class="SubTituloDireita" width="40%">PTA:</td>
                                                    <td id="td_pta_<?php echo $ano; ?>">
                                                        <?php
                                                        //$filtropta = ( $_SESSION['par']['itrid'] == 2 ? " and enb.muncod = '{$_SESSION['par']['muncod']}'" : " and enb.estuf = '{$_SESSION['par']['estuf']}'" );
                                                        $pta = '';
                                                        if ($oSbd->sbdid) {
                                                            $sql = "SELECT ptrid FROM par.subacaoemendapta WHERE sbdid = " . $oSbd->sbdid . " and sepstatus = 'A'";
                                                            $pta = $db->pegaUm($sql);
                                                        }

                                                        if (!$_SESSION['par']['inuid']) {
                                                            $inuid = $db->pegaUm("SELECT iu.inuid FROM par.instrumentounidade iu
										        								INNER JOIN par.pontuacao p on p.inuid = iu.inuid
										        								INNER JOIN par.acao a ON a.ptoid = p.ptoid
										        								INNER JOIN par.subacao s on s.aciid = a.aciid
										        								WHERE
										        									s.sbaid = " . $sbaid);
                                                        } else {
                                                            $inuid = $_SESSION['par']['inuid'];
                                                        }

                                                        $iuecnpj = pegaCnpjInuid($inuid);
//										        	$iuecnpj = pegaCnpjInuid($_SESSION['par']['inuid']);

                                                        if ($pta) {
                                                            $sql = "SELECT
										        					ptr.ptrid as codigo,
										        					ptr.ptrcod as descricao
																FROM emenda.planotrabalho ptr
																	inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
																WHERE
																	ptr.ptrexercicio = '$ano'
																    and ptr.ptrstatus = 'A'
																    and enb.enbcnpj = '$iuecnpj'
																    AND ptr.ptrid = $pta
																ORDER BY ptr.ptrcod";
                                                            $p = $db->pegaLinha($sql);
                                                            echo $p['descricao'];
                                                        }
// 										        	$db->monta_combo("pta[$ano]",$sql, 'S',"Selecione...",'carregaEmendaPTA',"","","","N","","",$pta,"PTRES"); 
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita">Código da Emenda:</td>
                                                    <td id="td_emenda_<?php echo $ano; ?>">
                                                        <?php
                                                        $arrEmenda = '';
                                                        if ($oSbd->sbdid) {
                                                            $sql = "SELECT distinct
																	ed.emdid as codigo,
																	eme.emecod as descricao
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emenda eme on eme.emeid = ed.emeid
																WHERE sep.sbdid = " . $oSbd->sbdid . " and sep.sepstatus = 'A'";

                                                            $arrEmenda = $db->carregar($sql);
                                                        }
// 										        	ver($arrEmenda);
                                                        if ($arrEmenda) {
// 										        		$html = '<select maximo="0" tipo="combo_popup" multiple="multiple" size="5" name="emenda['.$ano.'][]" id="emenda['.$ano.']" onclick="javascript:combo_popup_alterar_campo_busca( this );"
// 																	ondblclick="javascript:combo_popup_abre_janela( \'emenda['.$ano.']\', 400, 400, false );" onkeydown="javascript:combo_popup_remove_selecionados( event, \'emenda['.$ano.']\' );"
// 																		class="CampoEstilo" style="width:400px;">';
                                                            foreach ($arrEmenda as $v) {
// 										        			$html .= '<option value="'.$v['codigo'].'">'.$v['descricao'].'</option>';
                                                                echo $v['descricao'] . '<br>';
                                                            }
// 										        		$html .= '</select>';
// 										        		echo $html;
                                                        } else {
// 											        	$sqlEme = "select vede.emdid as codigo, vede.emecod as descricao
// 											        				from emenda.ptemendadetalheentidade pte
// 																		inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
// 																	where
// 																		pte.ptrid = ".($_POST['ptrid'] ? $_POST['ptrid'] : '0')."
// 																		and vede.edestatus = 'A'";
// 														combo_popup( "emenda[$ano]", $sqlEme, '', '400x400', 0, array(), '', false, false, false, 5, 400 );
                                                        }
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita">Redução da Emenda com base em 1,2% da RCL de 2013 (25,363%):</td>
                                                    <td id="td_reducao_<?php echo $ano; ?>">
                                                        <?php
                                                        $edevalorreducao = 0;
                                                        if ($oSbd->sbdid) {
                                                            $sql = "SELECT distinct
																	sum(ede.edevalorreducao)
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
																WHERE sep.sbdid = " . $oSbd->sbdid . " and sep.sepstatus = 'A'";

                                                            $edevalorreducao = $db->pegaUm($sql);
                                                        }
                                                        echo 'R$ ' . number_format($edevalorreducao, 2, ',', '.');
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita">Valor indicado pela Entidade:</td>
                                                    <td id="td_indicado_entidade_<?php echo $ano; ?>">
                                                        <?php
                                                        $edevalor = 0;
                                                        if ($oSbd->sbdid) {
                                                            $sql = "SELECT distinct
																	sum(ede.edevalor)
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
																    inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
																    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrid = sep.ptrid
																WHERE sep.sbdid = " . $oSbd->sbdid . " and sep.sepstatus = 'A'";

                                                            $edevalor = $db->pegaUm($sql);
                                                        }
                                                        echo 'R$ ' . number_format($edevalor, 2, ',', '.');
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita">Valor Reduzido pelo Parlamentar:</td>
                                                    <td id="td_reducao_parlamentar_<?php echo $ano; ?>">
                                                        <?php
                                                        $edevalor = 0;
                                                        if ($oSbd->sbdid) {
                                                            $sql = "SELECT distinct
																	sum(ede.edevalorreducao)
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
																    inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
																    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrid = sep.ptrid
																WHERE sep.sbdid = " . $oSbd->sbdid . " and sep.sepstatus = 'A'";

                                                            $edevalor = $db->pegaUm($sql);
                                                        }
                                                        echo 'R$ ' . number_format($edevalor, 2, ',', '.');
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td class="SubTituloDireita">Valor aceito pela Entidade:</td>
                                                    <td id="td_aceito_entidade_<?php echo $ano; ?>">
                                                        <?php
                                                        $sepvalor = 0;
                                                        if ($oSbd->sbdid) {
                                                            $sql = "SELECT sepvalor FROM par.subacaoemendapta sep WHERE sep.sbdid = " . $oSbd->sbdid . " and sep.sepstatus = 'A'";
                                                            $sepvalor = $db->pegaUm($sql);
                                                        }
                                                        echo 'R$ ' . number_format($sepvalor, 2, ',', '.');
                                                        ?>
                                                    </td>
                                                </tr>
                                                <?php
                                            }

                                            $sql = "SELECT DISTINCT
                                                        emp.empid,
                                                        emp.empdata as data,
                                                        empnumero as notaempenho,
                                                        emp.empnumeroprocesso as numeroprocesso,
                                                        sbdplanointerno,
                                                        sbdptres,
                                                        CASE WHEN emr.vrlreforco IS NOT NULL THEN
                                                            sum(vve.vrlempenhocancelado)
                                                        ELSE
                                                            sum(vve.vrlempenhocancelado)
                                                        END as valorempenho,
                                                        ems.eobpercentualemp as percentualempenho,
                                                        prp.prpid	
                                            		FROM
                                                            par.empenhosubacao ems
                                                        INNER JOIN par.empenho emp ON emp.empid = ems.empid AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                                                        INNER JOIN par.v_vrlempenhocancelado vve ON vve.empid = emp.empid
                                                        INNER JOIN par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
                                                        LEFT  JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
                                                        LEFT  JOIN par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = {$ano}
                                                        LEFT JOIN (
                                                            SELECT ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
                                                            FROM par.empenho ep
                                                                INNER JOIN par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A'
                                                                INNER JOIN par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
                                                            WHERE ep.empcodigoespecie in ('02')
                                                            GROUP BY 
                                                                ep.empnumeroprocesso,
                                                                ep.empcodigoespecie,
                                                                ep.empidpai,
                                                                sd1.sbdid
                                                        ) as emr ON emr.empidpai = emp.empid AND emr.sbdid = sbd.sbdid
                                                        WHERE
                                                            ems.sbaid = " . $_GET['sbaid'] . "  AND ems.eobano = " . $ano . " AND ems.eobstatus = 'A'
                                                        GROUP BY
                                                            emp.empid,
                                                            emp.empdata,
                                                            empnumero,
                                                            emp.empnumeroprocesso,
                                                            sbdplanointerno,
                                                            sbdptres,
                                                            emr.vrlreforco,
                                                            ems.eobpercentualemp,
                                                            prp.prpid
                                                        HAVING (
                                                            CASE 
                                                                WHEN emr.vrlreforco IS NOT NULL THEN sum(vve.vrlempenhocancelado)
                                                                ELSE sum(vve.vrlempenhocancelado)
                                                            END
                                                        ) > 0
                                                        ORDER BY
                                                            emp.empid";
                                            $numerosEmpenhos = $db->carregar($sql);
                                            if ($numerosEmpenhos[0]['empid']) {
                                            ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">Empenhos</td>
                                         	</tr>
                                            <?php
                                            if (is_array($numerosEmpenhos)) {
                                            	foreach ($numerosEmpenhos as $i => $numerosEmpenho) {
                                            ?>
                                            <tr>
                                            	<td colspan="2">Dados do Empenho <?= $i + 1 ?> (<?= formata_data($numerosEmpenho['data']) ?>) &nbsp;
                                                	<img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'empenho_<?= $i ?>')">
                                                    &nbsp;
                                                    <img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'empenho_<?= $i ?>')"></td>
                                            </tr>
                                            <tr>
                                            	<td colspan="2">
                                            		<div id="empenho_<?= $i ?>_<?= $ano ?>" style="display: none;">
                                                    	<table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" width="100%">
                                                        	<colgroup>
                                                            	<col width="25%" />
                                                                <col width="35%" />
                                                                <col width="20%" />
                                                                <col width="20%" />
															</colgroup>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Nota de Empenho:</td>
	                                                            <td colspan="3">
	                                                            	<?php echo $numerosEmpenho['notaempenho']; ?>
	                                                            </td>
                                                            </tr>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Nº do Processo:</td>
	                                                            <td colspan="3">
	                                                            	<?php echo $numerosEmpenho['numeroprocesso']; ?>
	                                                            </td>
                                                            </tr>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Nº do Termo:</td>
	                                                            <td colspan="3">
	                                                            	<?php echo $db->pegaUm( "SELECT dopnumerodocumento || '/' || date_part('year',dopdatainclusao) as numerotermo FROM par.vm_documentopar_ativos WHERE prpid = {$numerosEmpenho['prpid']} ORDER BY 1 DESC LIMIT 1" ); ?>
	                                                            </td>
                                                            </tr>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Nº do PI:</td>
	                                                            <td colspan="3">
	                                                            	<?php echo $numerosEmpenho['sbdplanointerno']; ?>
	                                                            </td>
                                                            </tr>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Nº do PTRES:</td>
	                                                            <td colspan="3">
	                                                            	<?php echo $numerosEmpenho['sbdptres']; ?>
	                                                            </td>
                                                            </tr>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Valor do Empenho:</td>
	                                                            <td colspan="3">
	                                                            	<?php echo number_format($numerosEmpenho['valorempenho'], 2, ',', '.') . ' (' . $numerosEmpenho['percentualempenho'] . ' %)'; ?>
	                                                            </td>
                                                            </tr>
                                                            
                                                          <?php
                                                                if($numerosEmpenho['empid'] && $_GET['sbaid'])
                                                                {
                                                                        $empenhoSubacao = $db->pegaUm("select eobvalorempenho from par.empenhosubacao  where empid =  {$numerosEmpenho['empid']} and sbaid = {$_GET['sbaid']} and eobstatus = 'A'");
                                                                }
                                                                else
                                                                {
                                                                        $empenhoSubacao = false;
                                                                }
                                                          	
                                                          	if($empenhoSubacao){
                                                          ?>
                                                            
	                                                            <tr>
		                                                            <td class="SubTituloDireita">Valor empenhado da subação:</td>
		                                                            <td colspan="3">
		                                                            	<?php echo number_format($empenhoSubacao, 2, ',', '.'); ?>
		                                                            </td>
	                                                            </tr>
                                                            <?php
                                                          }
														$sqlPagamento = "SELECT distinct
					                                    					pag.pagdatapagamento as data,
					                                            			pob.pobvalorpagamento as valorpagamento,
					                                            			COALESCE(pob.pobpercentualpag,0) as percentualpagamento,
					                                            			coalesce(pagnumeroob,'-') as pagnumeroob
																		FROM
                                            								par.pagamentosubacao pob
                                            							INNER JOIN par.pagamento pag on pag.pagid = pob.pagid AND pag.pagstatus = 'A'
                                                                                                INNER JOIN par.empenhosubacao emps ON emps.sbaid = pob.sbaid AND emps.eobano = pob.pobano AND emps.eobstatus = 'A'
                                            							WHERE
                                            								pob.pobano = {$ano} and pob.sbaid = {$sbaid} and pobstatus = 'A' AND pag.empid = " . $numerosEmpenho['empid'];
                                                        $dadosPagamento = $db->carregar($sqlPagamento);
                                                        if (is_array($dadosPagamento)) {
                                                            ?>
                                                            <tr>
                                                            	<td colspan="4" class="SubTituloTabela">Dados do Pagamento</td>
                                                            </tr>
                                                            <?php
                                                            foreach ($dadosPagamento as $p => $dadosPag) {
                                                            ?>
                                                            <tr>
	                                                            <td class="SubTituloDireita">Valor do Pagamento <?= $p + 1 ?> <br>(<?= formata_data($dadosPag['data']) ?>):</td>
	                                                            <td>
	                                                            	<?php echo number_format($dadosPag['valorpagamento'], 2, ',', '.') . ' (' . $dadosPag['percentualpagamento'] . ' %)'; ?>
	                                                            </td>
	                                                            <td class="SubTituloDireita">Número da OB :</td>
	                                                            <td>
	                                                            	<?= $dadosPag['pagnumeroob'] ?>
	                                                            </td>
                                                            </tr>
                                                            <?php
                                                        	}
                                                        }
                                                            ?>
														</table>
													</div>
												</td>
											</tr>
                                            <?php
		                                            }
												} 
											}
                                            $sql = "SELECT 
														sbdrepassecomplementar as sbdrepassecomplementar,
														sbdrepasseempenho as sbdrepasseempenho,
														sbdrepasseraf as sbdrepasseraf,
														coalesce(sbdrepassevlrcomplementar::numeric			,0::numeric) as sbdrepassevlrcomplementar,
														coalesce(sbdrepassevlrempenho::numeric				,0::numeric) as sbdrepassevlrempenho,
														coalesce(sbdrepassevlrraf::numeric					,0::numeric) as sbdrepassevlrraf,
														coalesce(sbdrepassevlrcomplementaraprovado::numeric	,0::numeric) as sbdrepassevlrcomplementaraprovado,
														coalesce(sbdrepassevlrempenhoaprovado::numeric		,0::numeric) as sbdrepassevlrempenhoaprovado,
														coalesce(sbdrepassevlrrafaprovado::numeric			,0::numeric) as sbdrepassevlrrafaprovado,
														sbdreformulacao
													FROM
														par.subacaodetalhe WHERE sbaid = {$sbaid} AND sbdano = " . $ano;
// 											ver($sql);
                                            $dadosRepassePr = $db->pegaLinha($sql);
                                            $sbdrepassevlrcomplementar 			= number_format($dadosRepassePr['sbdrepassevlrcomplementar'], 2, ',', '.');
                                            $sbdrepassevlrempenho 				= number_format($dadosRepassePr['sbdrepassevlrempenho'], 2, ',', '.');
                                            $sbdrepassevlrraf			 		= number_format($dadosRepassePr['sbdrepassevlrraf'], 2, ',', '.');
                                            $sbdrepassevlrcomplementaraprovado 	= number_format($dadosRepassePr['sbdrepassevlrcomplementaraprovado'], 2, ',', '.');
                                            $sbdrepassevlrempenhoaprovado 		= number_format($dadosRepassePr['sbdrepassevlrempenhoaprovado'], 2, ',', '.');
                                            $sbdrepassevlrrafaprovado 			= number_format($dadosRepassePr['sbdrepassevlrrafaprovado'], 2, ',', '.');

                                            // Se for reformulação
                                            if ($ssuid[$ano] == 20 || $ssuid[$ano] == 23) {
                                                if ($dadosRepassePr['sbdreformulacao'] == 't') {
                                                	$sql = "SELECT 
                                                				(
																	par.recuperavalorplanejadossubacaoporano($sbaid, $ano)
																)
																-
																( 	SELECT DISTINCT SUM(saldo)  as valorempenho
																	FROM par.v_dadosempenhosubacao 
																	WHERE sbaid = $sbaid AND sbdano = $ano )";
//                                                 	$sql = "SELECT 
//                                                 				(
// 																	par.recuperavalorplanejadossubacaoporano($sbaid, $ano)
// 																)
// 																-
// 																(
// 																	SELECT DISTINCT
// 																		SUM(vve.vrlempenhocancelado)  as valorempenho
// 																	FROM
// 																		par.empenhosubacao ems
// 																	INNER JOIN par.empenho 			emp ON emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
// 																	inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
// 																	INNER JOIN par.processopar 		prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
// 																	LEFT  JOIN par.subacaodetalhe 	sbd ON ems.sbaid = sbd.sbaid AND sbdano = $ano
// 																	left join (
// 																		select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
// 																		from par.empenho ep
// 																			inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A'
// 																			inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
// 																		where ep.empcodigoespecie in ('02')
// 																		group by 
// 																			ep.empnumeroprocesso,
// 																			ep.empcodigoespecie,
// 																			ep.empidpai,
// 																			sd1.sbdid
// 																	) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
// 																	WHERE
// 																		ems.sbaid = $sbaid  AND ems.eobano = $ano AND ems.eobstatus = 'A'
// 																)";

                                                    $testaValores = $db->pegaUm($sql);
                                                } else {
                                                    $sql = "SELECT
																par.recuperavalorplanejadossubacaoporano($sbaid, '$ano') - coalesce(sep.sepvalor,0)  as dif
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid and sep.sepstatus = 'A' AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid";

                                                    $testaValores = $db->pegaUm($sql);

                                                    $sql = "SELECT
																coalesce(sep.sepvalor,0)  as vlrempenho
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";

                                                    $vlrEmpenho = $db->pegaUm($sql);
                                                }
// 											} elseif( $ssuid[$ano] == 21 ) { // se for a analise da reformulação
                                            } else { // se for a analise da reformulação e outros estados pega o valor validado se existir. Senão pega valor planejado.
                                                if ($dadosRepassePr['sbdreformulacao'] == 't') {
                                                    $sql = "SELECT
																(
																	par.recuperavalorvalidadossubacaoporano($sbaid, $ano)
																)
																-
																( 	SELECT DISTINCT SUM(saldo)  as valorempenho
																	FROM par.v_dadosempenhosubacao 
																	WHERE sbaid = $sbaid AND sbdano = $ano )";
//                                                     $sql = "SELECT
// 																(
// 																	par.recuperavalorvalidadossubacaoporano($sbaid, $ano)
// 																)
// 																-
// 																(
// 																	SELECT DISTINCT
// 																		SUM(vve.vrlempenhocancelado)  as valorempenho
// 																	FROM
// 																		par.empenhosubacao ems
// 																	INNER JOIN par.empenho 			emp ON emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
// 																	inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
// 																	INNER JOIN par.processopar 		prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
// 																	LEFT  JOIN par.subacaodetalhe 	sbd ON ems.sbaid = sbd.sbaid AND sbdano = $ano
// 																	left join (
// 																		select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
// 																		from par.empenho ep
// 																			inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A'
// 																			inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
// 																		where ep.empcodigoespecie in ('02')
// 																		group by 
// 																			ep.empnumeroprocesso,
// 																			ep.empcodigoespecie,
// 																			ep.empidpai,
// 																			sd1.sbdid
// 																	) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
// 																	WHERE
// 																		ems.sbaid = $sbaid  AND ems.eobano = $ano AND ems.eobstatus = 'A'
// 																)";
// ver($sql);
                                                	$testaValores = $db->pegaUm($sql);
                                                } else {
                                                    $sql = "SELECT
																par.recuperavalorvalidadossubacaoporano($sbaid, '$ano') - coalesce(sep.sepvalor,0)  as dif
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";

                                                    $testaValores = $db->pegaUm($sql);

                                                    $sql = "SELECT
																coalesce(sep.sepvalor,0)  as vlrempenho
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";

                                                    $vlrEmpenho = $db->pegaUm($sql);
                                                }
                                            }
                                            if ($_SESSION['par']['itrid'] == 2) {
                                                $local = $db->pegaUm("SELECT mundescricao || '-' || estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'");
                                            } else {
                                                $local = $_SESSION['par']['estuf'];
                                            }
                                            if (
                                                    (
	                                                    (
		                                                    (
			                                                    ( $testaValores > 0 ) &&
			                                                    $ssuid[$ano] == 21 &&
			                                                    (
				                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil)
			                                                    )
		                                                    ) ||
		                                                    (
			                                                    ( $testaValores > 0 ) &&
			                                                    ($ssuid[$ano] == 20 || $ssuid[$ano] == 23) &&
			                                                    (
				                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
				                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
			                                                    )
		                                                    )
	                                                    ) &&
	                                                    $dadosRepassePr['sbdreformulacao'] == 't'
                                                    ) ||
                                                    (
	                                                    (
		                                                    ( $testaValores > 0 ) ||
		                                                    (
		                                                    $ssuid[$ano] == 22 &&
		                                                    (
			                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
			                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
			                                                    in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) ||
			                                                    in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) ||
			                                                    in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil)
		                                                    )
		                                                    )
	                                                    ) &&
	                                                    $testaDiligenciaCondicional == 't'
                                                    )
                                            ) {
												?>
                                                 <tr>
                                                    <td colspan="2" class="SubTituloTabela">
                                                        <font color="red">Complementação</font>
                                                    </td>
                                                </tr>  
<!-- 												<tr > -->
<!-- 													<td colspan="4" class="SubTituloTabela"> -->
														<div class="botao_tr cancelar redistribuir_empenho" sbdano="<?=$ano ?>" >
                                                        	<a style="cursor: pointer" title="Redistribuir valores dos empenhos deste processo para completar o saldo desta subação">
<!--                                                         	Redistribuir Empenho? -->
<!--                                                         	</a> -->
<!-- 														</div> -->
<!-- 													</td> -->
<!-- 												</tr> -->
                                                <?php
                                                $disabled = 'disabled="disabled"';
                                                $habilita = "N";
                                                if ($_REQUEST['repasse'] == 1) {
                                                    $disabled = '';
                                                    $habilita = "S";
                                                }

                                                if (possui_perfil(PAR_PERFIL_ADMINISTRADOR)) {
                                                    $disabled = '';
                                                    $habilita = "S";
                                                }
                                                if (($ssuid[$ano] == 20 || $ssuid[$ano] == 23) && ( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) )
                                                ) {
                                                    $disabled = '';
                                                    $habilita = "S";
                                                }
                                                if ($ssuid[$ano] == 21 && ( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) )
                                                ) {
                                                    $disabled = '';
                                                    $habilita = "S";
                                                }
                                                if (($ssuid[$ano] == 22 || $ssuid[$ano] == 7) && (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) )
                                                ) {
                                                    $disabled = '';
                                                    $habilita = "S";
                                                }

                                                if( $finalizado == true ){
                                                    $disabled = 'disabled="disabled"';
                                                    $habilita = "N";
                                                    $habil = "N";
                                                }
                                                
                                                if ($ssuid[$ano] == 21) {
                                                    $sql = "SELECT
																sbdrepassevlrcomplementar,
																sbdrepassevlrempenho,
																sbdrepassevlrraf
															FROM
																par.subacaodetalhe 
															WHERE sbaid = {$sbaid} AND sbdano = " . $ano;

                                                    $dadosRepassePl = $db->pegaLinha($sql);
                                                }
                                                
                                                $titulo_complementacao_exibido = "true";
                                                
                                                    $sql = "SELECT true FROM par.subacao WHERE sbaidpai = $sbaid";

                                            $testaReformulacao = $db->pegaUm($sql);

// 											ver($testaReformulacao == 't', $dadosRepassePr['sbdreformulacao']);
// 											if( $subacao['sbareformulacao'] == 't' && ( $ssuid[$ano] == 20 || $ssuid[$ano] == 21 ) ){
                                            ?>
												<tr ><td colspan="4" class="SubTituloTabela">Complemento atual</td></tr>
                                                <tr>
                                                    <td colspan="2">
                                                        <?php if ($ssuid[$ano] == 20 || $ssuid[$ano] == 23) { ?>
                                                            <table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
                                                                <tr>
                                                                    <td width="20%">
                                                                        <?php
                                                                        if (
                                                                                $dadosRepassePr['sbdrepassecomplementar'] != 't' &&
                                                                                $dadosRepassePr['sbdrepasseempenho'] != 't' &&
                                                                                $dadosRepassePr['sbdrepasseraf'] != 't' &&
                                                                                ( $testaValores > 0 ) &&
                                                                                !($disabled)
                                                                        ) {
                                                                            ?>
                                                                            <script>
                                                                                alert('Valor planejado ultrapassa o valor empenhado para esta subação.\n Favor distribui-lo no campo de complementação.');
                                                                            </script>
                                                                            <?php
                                                                        }
                                                                        $sbdrepassevlrcomplementar 	= str_replace(Array('.',','), Array('', '.'),$sbdrepassevlrcomplementar) > $testaValores ? number_format($testaValores, 2, ',', '.') : $sbdrepassevlrcomplementar;
                                                                        $sbdrepassevlrempenho 		= str_replace(Array('.',','), Array('', '.'),$sbdrepassevlrempenho) > $testaValores ? number_format($testaValores, 2, ',', '.') : $sbdrepassevlrempenho;
                                                                        $sbdrepassevlrraf 			= str_replace(Array('.',','), Array('', '.'),$sbdrepassevlrraf) > $testaValores ? number_format($testaValores, 2, ',', '.') : $sbdrepassevlrraf;
                                                                        ?>
                                                                        <b>Complemento</b>
                                                                    </td>
                                                                    <td><b>Valor Planejado</b></td>
                                                                </tr>
                                                                <tr>
                                                                    <td class="SubTituloDireita">Valor Complementar Total:</td>
                                                                    <td>
                                                                        <input type="hidden" id="diferenca" name="diferenca[<?= $ano ?>]" value="<?= $testaValores ?>"/>
                                                                        <input type="hidden" id="vlrEmpenho" value="<?= $vlrEmpenho ?>"/>
                                                                        <span id="span_diferenca">
                                                                            R$ <?= number_format($testaValores, 2, ',', '.'); ?>
                                                                        </span>
                                                                        <span id="span_msg_diferenca">
                                                                        </span>
                                                                    </td>
                                                                </tr>
                                                                <tr>
                                                                    <td class="SubTituloDireita"><?= $local ?>:</td>
                                                                    <td>
                                                                        <input type="radio" <?= $disabled ?> name="sbdrepassecomplementar[<?= $ano ?>]" <?php
                                                                        if ($dadosRepassePr['sbdrepassecomplementar'] == 't') {
                                                                            echo 'checked';
                                                                        }
                                                                        ?> value="t">Sim &nbsp;
                                                                        <input type="radio" <?= $disabled ?> name="sbdrepassecomplementar[<?= $ano ?>]" <?php
                                                                        if ($dadosRepassePr['sbdrepassecomplementar'] != 't') {
                                                                            echo 'checked';
                                                                        }
                                                                        ?> value="f">Não&nbsp;&nbsp;&nbsp;
                                                                        <span id="sbdrepassecomplementarvalor" style="display:<?php
                                                                        if ($dadosRepassePr['sbdrepassecomplementar'] != 't') {
                                                                            echo 'none';
                                                                        }
                                                                        ?>">
                                                                                  <?php echo campo_texto("sbdrepassevlrcomplementar[" . $ano . "]", "N", $habilita, "Valor", "20", "20", "[.###],##", "", "", "", "", 'id="sbdrepassevlrcomplementar"', '', $sbdrepassevlrcomplementar) ?>
                                                                        </span>
                                                                    </td>
                                                                </tr>
                                                                <!--<tr>
                                                                        <td class="SubTituloDireita">Empenho Complementar (MEC/FNDE):</td>
                                                                        <td>
                                                                                <input type="radio" <?= $disabled ?> name="sbdrepasseempenho[<?= $ano ?>]" <?php
                                                                if ($dadosRepassePr['sbdrepasseempenho'] == 't') {
                                                                    echo 'checked';
                                                                }
                                                                ?> value="t">Sim &nbsp;
                                                                                <input type="radio" <?= $disabled ?> name="sbdrepasseempenho[<?= $ano ?>]" <?php
                                                                if ($dadosRepassePr['sbdrepasseempenho'] != 't') {
                                                                    echo 'checked';
                                                                }
                                                                ?> value="f">Não&nbsp;&nbsp;&nbsp;
                                                                                <span id="sbdrepasseempenhovalor" style="display: <?php
                                                                if ($dadosRepassePr['sbdrepasseempenho'] != 't') {
                                                                    echo 'none';
                                                                }
                                                                ?>;">
                                                                <?php echo campo_texto("sbdrepassevlrempenho[" . $ano . "]", "N", $habilita, "Valor", "20", "20", "[.###],##", "", "", "", "", 'id="sbdrepassevlrempenho"', '', $sbdrepassevlrempenho) ?>
                                                                                </span>
                                                                        </td>
                                                                </tr>
                                                                --><tr>
                                                                    <td class="SubTituloDireita">RAF (MEC/FNDE):</td>
                                                                    <td>
                                                                        <input type="radio" <?= $disabled ?> name="sbdrepasseraf[<?= $ano ?>]" <?php
                                                                        if ($dadosRepassePr['sbdrepasseraf'] == 't') {
                                                                            echo 'checked';
                                                                        }
                                                                        ?> value="t">Sim &nbsp;
                                                                        <input type="radio" <?= $disabled ?> name="sbdrepasseraf[<?= $ano ?>]" <?php
                                                                        if ($dadosRepassePr['sbdrepasseraf'] != 't') {
                                                                            echo 'checked';
                                                                        }
                                                                        ?> value="f">Não&nbsp;&nbsp;&nbsp;
                                                                        <span id="sbdrepasserafvalor" style="display: <?php
                                                                        if ($dadosRepassePr['sbdrepasseraf'] != 't') {
                                                                            echo 'none';
                                                                        }
                                                                        ?>;">
                                                                              <?php echo campo_texto("sbdrepassevlrraf[" . $ano . "]", "N", $habilita, "Valor", "20", "20", "[.###],##", "", "", "", "", 'id="sbdrepassevlrraf"', "javascript:verificaValorReformulacao(" . $valorRestante . ")", $sbdrepassevlrraf) ?>
                                                                        </span>
                                                                    </td>
                                                                </tr>
                                                            </table>
                                                        <?php } ?>
                                                        <?php
                                                        if ($ssuid[$ano] == 21 || $ssuid[$ano] == 22) {

                                                            if ($dadosRepassePr['sbdreformulacao'] == 't') {
                                                            	$sql = "SELECT
																			(
																				par.recuperavalorvalidadossubacaoporano($sbaid, $ano)
																			)
																			-
																			(
																				SELECT DISTINCT
																					sum(saldo)
																				FROM
																					par.v_dadosempenhosubacao
																				WHERE
																					sbaid = $sbaid  AND sbdano = $ano 
																			)";
                                                              	$testaValores = $db->pegaUm($sql);
                                                            } 
                                                            
                                                                $sql = "SELECT
																			par.recuperavalorplanejadossubacaoporano($sbaid, '$ano') 
																			-
																			(
																				SELECT DISTINCT
																					sum(saldo)
																				FROM
																					par.v_dadosempenhosubacao
																				WHERE
																					sbaid = $sbaid  AND sbdano = $ano 
																			)";
														
                                                                $difPlanejado = $db->pegaUm($sql);
                                                            ?>
                                                            <table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
                                                                <tr>
                                                                    <td width="20%">
                                                                        <?php
                                                                        if (
                                                                                $sbdrepassevlrcomplementaraprovado <= 0 &&
                                                                                $sbdrepassevlrempenhoaprovado <= 0 &&
                                                                                $sbdrepassevlrrafaprovado <= 0 &&
                                                                                ( $testaValores > 0 ) &&
                                                                                !($disabled)
                                                                        ) {
                                                                            ?>
                                                                            <script>
                                                                                alert('Valor planejado ultrapassa o valor empenhado para esta subação.\n Favor distribui-lo no campo de complementação.');
                                                                            </script>
                                                                            <?php
                                                                        }
                                                                        $sbdrepassevlrcomplementaraprovado 	= str_replace(Array('.',','), Array('', '.'),$sbdrepassevlrcomplementaraprovado) >= $testaValores 	? number_format($testaValores, 2, ',', '.') : $sbdrepassevlrcomplementaraprovado;
                                                                        $sbdrepassevlrempenhoaprovado 		= str_replace(Array('.',','), Array('', '.'),$sbdrepassevlrempenhoaprovado) >= $testaValores 		? number_format($testaValores, 2, ',', '.') : $sbdrepassevlrempenhoaprovado;
                                                                        $sbdrepassevlrrafaprovado 			= str_replace(Array('.',','), Array('', '.'),$sbdrepassevlrrafaprovado) >= $testaValores 			? number_format($testaValores, 2, ',', '.') : $sbdrepassevlrrafaprovado;
                                                                        ?>
                                                                        <b>Complemento</b>
                                                                    </td>
                                                                    <td><b>Valor Planejado</b></td>
                                                                    <td><b>Valor Aprovado</b></td>
                                                                </tr>
                                                                <tr>
                                                                    <td class="SubTituloDireita">Valor Complementar Total:</td>
                                                                    <td>
                                                                        <span id="span_diferenca">
                                                                            R$ <?= ( $difPlanejado < 0 ? '0,00' : number_format($difPlanejado, 2, ',', '.') ); ?>
                                                                        </span>
                                                                    </td>
                                                                    <td>
                                                                        <input type="hidden" id="diferenca" name="diferenca[<?= $ano ?>]" value="<?= $testaValores ?>"/>
                                                                        <input type="hidden" id="vlrEmpenho" value="<?= $vlrEmpenho ?>"/>
                                                                        <span id="span_diferenca">
                                                                            R$ <?= number_format($testaValores, 2, ',', '.'); ?>
                                                                        </span>
                                                                        <span id="span_msg_diferenca">
                                                                        </span>
                                                                    </td>
                                                                </tr>
                                                                <tr>
                                                                    <td class="SubTituloDireita"><?= $local ?>:</td>
                                                                    <td>R$ <?= $sbdrepassevlrcomplementar ?></td>
                                                                    <td>
                                                                        <?= campo_texto("sbdrepassevlrcomplementaraprovado[" . $ano . "]", "N", $habilita, "Valor", "20", "20", "[.###],##", "", "", "", "", 'id="sbdrepassevlrcomplementaraprovado"', '', $sbdrepassevlrcomplementaraprovado) ?>
                                                                    </td>
                                                                </tr>
                                                                <tr>
                                                                    <td class="SubTituloDireita">Empenho Complementar (MEC/FNDE):</td>
                                                                    <td>R$ <?= $sbdrepassevlrempenho ?></td>
                                                                    <td>
                                                                        <?= campo_texto("sbdrepassevlrempenhoaprovado[" . $ano . "]", "N", $habilita, "Valor", "20", "20", "[.###],##", "", "", "", "", 'id="sbdrepassevlrempenhoaprovado"', '', $sbdrepassevlrempenhoaprovado) ?>
                                                                    </td>
                                                                </tr>
                                                                <tr>
                                                                    <td class="SubTituloDireita">RAF (MEC/FNDE):</td>
                                                                    <td>R$ <?= $sbdrepassevlrraf ?></td>
                                                                    <td>
                                                                        <?= campo_texto("sbdrepassevlrrafaprovado[" . $ano . "]", "N", $habilita, "Valor", "20", "20", "[.###],##", "", "", "", "", 'id="sbdrepassevlrrafaprovado"', "javascript:verificaValorReformulacao(" . $valorRestante . ")", $sbdrepassevlrrafaprovado) ?>
                                                                    </td>
                                                                </tr>
                                                            </table>
                                                        <?php } ?>
                                                    </td>
                                                </tr>
                                                <?php
                                            } elseif (
//                                                     $testaValores > 0 &&
                                                    (
                                                    (float) $sbdrepassevlrcomplementaraprovado > 0 ||
                                                    (float) $sbdrepassevlrempenhoaprovado > 0 ||
                                                    (float) $sbdrepassevlrrafaprovado > 0
                                                    )
                                            ) {
                                                ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">
                                                        <font color="red">Complementação</font>
                                                    </td>
                                                </tr>



                                                <tr>
                                                    <td colspan="2">
                                                        <table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
                                                            <tr>
                                                                <td width="20%"><b>Complemento</b></td>
                                                                <td><b>Valor Planejado</b></td>
                                                                <td><b>Valor Aprovado</b></td>
                                                            </tr>
                                                            <tr>
                                                                <td class="SubTituloDireita">Valor Complementar Total:</td>
                                                                <td>
                                                                    R$ <?=number_format( ($dadosRepassePr['sbdrepassevlrcomplementar']+$dadosRepassePr['sbdrepassevlrempenho']+$dadosRepassePr['sbdrepassevlrraf']) , 2, ',', '.') ; ?>
                                                                </td>
                                                                <td>
                                                                    R$ <?= number_format( ($dadosRepassePr['sbdrepassevlrcomplementaraprovado']+$dadosRepassePr['sbdrepassevlrempenhoaprovado']+$dadosRepassePr['sbdrepassevlrrafaprovado']) , 2, ',', '.') ?>
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="SubTituloDireita"><?= $local ?>:</td>
                                                                <td>
                                                                    R$ <?= $sbdrepassevlrcomplementar; ?>
                                                                </td>
                                                                <td>
                                                                    R$ <?= $sbdrepassevlrcomplementaraprovado; ?>
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="SubTituloDireita">Empenho Complementar (MEC/FNDE):</td>
                                                                <td>
                                                                    R$ <?= $sbdrepassevlrempenho; ?>
                                                                </td>
                                                                <td>
                                                                    R$ <?= $sbdrepassevlrempenhoaprovado; ?>
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="SubTituloDireita">RAF (MEC/FNDE):</td>
                                                                <td>
                                                                    R$ <?= $sbdrepassevlrraf; ?>
                                                                </td>
                                                                <td>
                                                                    R$ <?= $sbdrepassevlrrafaprovado; ?>
                                                                </td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                                <?php
                                            }

                                            $sql = "SELECT true FROM par.subacao WHERE sbaidpai = $sbaid";

                                            $testaReformulacao = $db->pegaUm($sql);

                                            if ($testaReformulacao == 't' && $dadosRepassePr['sbdreformulacao'] == 't') {
                                                ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">
                                                            <font color="red">Histórico de Reformulações</font> <!--  <a style="cursor: pointer;" onclick="listarSubacao('<?= $sbaidpai ?>')"><br>(Visualizar versão anterior)</a>-->
                                                        <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'div_reformular')">
                                                        &nbsp;
                                                        <img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'div_reformular')">
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td colspan="2">
                                                        <div id="div_reformular_<?= $ano ?>" style="display: none;">
                                                            <table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" width="100%">
                                                                <tr>
                                                                    <td>
                                                                        <?
                                                                        
                                                                        $sql = "SELECT sbaidpai FROM par.subacao WHERE sbaid = {$subacao['sbaid']}";
                                                                        
                                                                        $sbaidpai = $db->pegaUm( $sql );
                                                                        $sbaidpai = $sbaidpai != '' ? $sbaidpai : $subacao['sbaid'];
                                                                        
                                                                        $sql = "SELECT DISTINCT
																					sbdid as ordem,
																					'<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"listarSubacao(\'' || sd.sbaid || '\');\" border=\"0\" />' as acao,
																					s.sbadsc,
																					sd.sbdano,
																					'<center>'||to_char(s.sbadatareformulacao, 'DD/MM/YYYY')||'</center>' as data
																				FROM
																					par.subacao s
																				INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid
																				WHERE
																					s.sbastatus <> 'I' AND
																					s.sbaidpai = $sbaidpai AND
																					sd.sbdano = $ano 
																					AND (
																						CASE WHEN $sbaidpai = {$subacao['sbaid']}
																							THEN TRUE
																							ELSE s.sbaid < {$subacao['sbaid']}
																						END
																					)
																				ORDER BY
																					sbdid";

                                                                        $arrDados = $db->carregar($sql);
                                                                        $arrDados = is_array($arrDados) ? $arrDados : Array();

                                                                        foreach ($arrDados as $k => $dados) {
                                                                            if ($k == 0) {
                                                                                $arrDados[$k]['ordem'] = 'Original';
                                                                            } else {
                                                                                $arrDados[$k]['ordem'] = $k . '° Reformulação';
                                                                            }
                                                                        }

                                                                        $cabecalho = array("&nbsp;", "&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Data da Reprogramação");
                                                                        $db->monta_lista_array($arrDados, $cabecalho, 5000, 5, 'N', 'center', $html);
// 																$db->monta_lista($arrDados,$cabecalho,50000,5,'N','90%','N');
                                                                        ?>
                                                                    </td>
                                                                </tr>
                                                            </table>
                                                        </div>
                                                    </td>
                                                </tr>
                                                <?
                                            }
                                           	if(!empty($_GET['historico'])){  ?>
                                        <tr>
                                                <td colspan="2" class="SubTituloTabela">Histórico</td>
                                        </tr>
                                      <?php  
                                        
                                        
                                     } ?>

                                            <tr>
                                                <td colspan="2" class="SubTituloTabela">Quantidades e Cronograma de Execução</td>
                                            </tr>
    <?php if ($subacao['sbacronograma'] != 2 && ($subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $subacao['frmid'] != ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) && $subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS): ?>
                                                <tr id="cronogramaGlobal_<?php echo $ano; ?>">
                                                    <td class="SubTituloDireita">Quantidade:</td>
                                                    <td>
        <?php echo campo_texto("sbdquantidade[$ano]", "N", $habil, "Quantidade", "20", "20", "[.###]", "", "", "", "", "sbdquantidade_$ano", "", $oSbd->sbdquantidade ? number_format($oSbd->sbdquantidade, 0, 2, '.') : "", "this.value=mascaraglobal('[.###]',this.value);") ?>
                                                    </td>
                                                </tr>
    <? endif; ?>
                                            <tr>
                                                <td class="SubTituloDireita">Cronograma Físico:</td>
                                                <td>
                                                    <?php
                                                    if (bloqueiaEquipeLocalOnibusAcessivel($_REQUEST)) {
                                                        $habil = 'N';
                                                    }

                                                    $db->monta_combo("sbdinicio[$ano]", $arrMeses, $habil, "Selecione...", "", "", "", "", "N", "sbdinicio_$ano", "", $oSbd->sbdinicio)
                                                    ?>
                                                    a
                                                    <?php $db->monta_combo("sbdfim[$ano]", $arrMeses, $habil, "Selecione...", "", "", "", "", "N", "sbdfim_$ano", "", $oSbd->sbdfim) ?>
                                                    <span style="margin-left:50px;font-weight:bold">Ano de Término:</span>
    <?php echo campo_texto("sbdanotermino[$ano]", "N", $habil, "Ano de Término", "5", "4", "[#]", "", "", "", "", "sbdanotermino_$ano", "", $oSbd->sbdanotermino) ?>
                                                </td>
                                            </tr>
                                            <?php
                                            if ($subacao['frmid'] != FORMA_EXECUCAO_FINANCIAMENTO_BNDES) {
                                                if ($subacao['sbacronograma'] == 2 && $subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS):
                                                // Caso seja uma reformulação do tipo escolas é necessário fazer uma validação para casos antigos
                                                if ($testaReformulacao == 't') {
                                                    $arrParam = array(
                                                        'inuid' => $inuid,
                                                        'sbaid' => $_GET['sbaid'],
                                                        'ano' => $ano
                                                    );
                                                    
                                                    #Cabecalho das escolas
                                                    $cabecalhoL =   array('Ação', 'Município', 'Escola','Código INEP', 'Tipo da Escola' );
                                                    
                                                    #Escolas que estao vinculadas  indevidamente
                                                    $arrEscolaIndevida = montaListaEscolaSubacao($arrParam, false);
                                                    if ($arrEscolaIndevida) {
                                                    ?>
                                                        <tr>
                                                            <td colspan="2" class="SubTituloTabela"><span style="color:red;" >Escolas Indevidas</span>
                                                                <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'escolasindevidas')">
                                                                &nbsp;
                                                                <img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'escolasindevidas')">
                                                            </td>
                                                        </tr>
                                                        <tr id="escolasindevidas_<?php echo $ano ?>">
                                                            <td colspan="2" class="">
                                                                <?php $db->monta_lista_array($arrEscolaIndevida, $cabecalhoL,500,5,"N","center");?>
                                                            </td>
                                                        </tr>  
                                                    <?php
                                                    }
                                                    ?>
                                                     <tr>
                                                        <td colspan="2" class="SubTituloTabela">Escolas
                                                            <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'escolas'); montaListaEscolaSubacaoControle(<?php echo $inuid?>,<?php echo $_GET['sbaid']?>,<?php echo $ano?>)">
                                                            &nbsp;
                                                            <img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'escolas')">
                                                        </td>
                                                    </tr>
                                                    <tr id="escolas_<?php echo $ano ?>" style="display: none">
                                                        <td colspan="2" class="" id="tdescolas_<?php echo $ano ?>"></td>
                                                    </tr>  
                                                <?php
                                                }
                                                else 
                                                {
                                                	$arrParam = array(
                                                			'inuid' => $inuid,
                                                			'sbaid' => $_GET['sbaid'],
                                                			'ano' => $ano
                                                	);
                                                	
                                                	#Cabecalho das escolas
                                                	$cabecalhoL =   array('Ação', 'Município', 'Escola','Código INEP', 'Tipo da Escola' );
                                                	
                                                	#Escolas que estao vinculadas  indevidamente
                                                	$arrEscolaIndevida = montaListaEscolaSubacao($arrParam, false, false);
                                                	if ($arrEscolaIndevida) {
                                                	?>
                                                		<tr>
                                                	    	<td colspan="2" class="SubTituloTabela" > <span style="color:red;" > Escolas Indevidas 
                                                	        	<img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'escolasindevidas')">
                                                	        	&nbsp;
                                                	            <img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'escolasindevidas')">
                                                	            <br>
                                                	    																		Solicite a reprogramação para remover esta(s) escola(s) e corrigir a subação. </span>
                                                	            
                                                	        </td>
                                                	   </tr>
                                                	      	<tr id="escolasindevidas_<?php echo $ano ?>">
                                                	        	<td colspan="2" class="">
                                                	            	<?php $db->monta_lista_array($arrEscolaIndevida, $cabecalhoL,500,5,"N","center");?>
                                                	            </td>
                                                	        </tr>  
	                                               	<?php
	                                                }	
                                                }
                                                ?>
                                                    <tr>
                                                        <td id="escolas_<?php echo $ano; ?>" colspan="2" style="display: none;">
                                                            <?php $oSes = new SubacaoEscola(); 
                                                            $oSes->montaLista($_GET['sbaid'], $ano, $subacao['frmid'], $subacao['ptsid']); ?>
                                                            <input type="hidden" name="escolas[<?php echo $ano ?>]" value="<?php echo $oSes->conta($_GET['sbaid'], $ano); ?>" />
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td class="SubTituloEsquerda" colspan="2" >
                                                            <?php
                                                            #Para esse objeto "Editar / Inserir Escolas", não é adcionado nos perfis, clausulas de forma de execução, as que foram definidas pelas regras,
                                                            #pois nesse momento ocultaria o objeto, não sendo necessário porque esse objeto é disabilitado. Dessa forma esse objeto obdece as regras definidas para os perfis.
                                                            #Perfis esses:  PAR_PERFIL_EQUIPE_TECNICA, PAR_PERFIL_EQUIPE_FINANCEIRA.
                                                            /* if( (	($estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) && $valida == 'true' ) || $reformulacao	){
                                                              if( !( (
                                                              in_array(PAR_PERFIL_CONSULTA,$arrayPerfil) ||
                                                              in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrayPerfil)
                                                              ) && $_SESSION['par']['itrid'] == 2
                                                              ) ||
                                                              (	(
                                                              in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
                                                              in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrayPerfil)
                                                              ) && $_SESSION['par']['itrid'] == 1
                                                              ) ||
                                                              (	(
                                                              in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
                                                              in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrayPerfil)
                                                              ) && $_SESSION['par']['itrid'] == 2
                                                              ) ||
                                                              (
                                                              in_array(PAR_PERFIL_PREFEITO,$arrayPerfil) ||
                                                              in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) ||
                                                              in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil) ||
                                                              in_array(PAR_PERFIL_COORDENADOR_GERAL ,$arrayPerfil)
                                                              ) //|| $habil
                                                              ){ */
                                                        if ($bloquearAnoSubacaoReprogramacao == 't') {
                                                            if ((
                                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || $validaBP
                                                                ) ||
                                                                (
                                                                    $permitido == 'S' && ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE) && (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil))
                                                                ) ||
                                                                (
                                                                    (
                                                                        $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO || ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA && ($ssuid[$ano] == 7 || $ssuid[$ano] == 10))
                                                                    ) &&
                                                                    (
                                                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
                                                                    )
                                                                ) ||
                                                                (
                                                                    (
                                                                        ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ATUALIZACAO && $_SESSION['par']['itrid'] == 1) || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO || ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA && ($ssuid[$ano] == 7 || $ssuid[$ano] == 10)) || ($reformulacao == 'true' && $ssuid[$ano] == 20)
                                                                    ) &&
                                                                    (
                                                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) ||
                                                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $arrayPerfil) ||
                                                                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) ||
                                                                        in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) ||
                                                                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil)
                                                                    )
                                                                ) ||
                                                                (
                                                                    (
                                                                        $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO || ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA && ($ssuid[$ano] == 7 || $ssuid[$ano] == 10))
                                                                    ) &&
                                                                    (
                                                                    in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil)
                                                                    )
                                                                )
                                                            ) {
                                                                if ($validaSalvar == 'true' && $valida == 'true' && $testaCopia != 't') {
                                                                    ?>
                                                                    <img class="middle link"
                                                                         onclick="inserirEscolas('<?php echo $ano ?>', '<?php echo $_GET['sbaid'] ?>', '<?php echo $subacao['frmid'] ?>', '<?php echo $subacao['ptsid'] ?>')"
                                                                         src="../imagens/gif_inclui.gif"/> <a
                                                                        href="javascript:inserirEscolas('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>','<?php echo $subacao['frmid'] ?>','<?php echo $subacao['ptsid'] ?>')">Editar
                                                                        / Inserir Escolas</a>
                                                                <?php
                                                                }
                                                            }
                                                        }
                                                            ?>
                                                        </td>
                                                    </tr>
                                                    <?php
                                                endif;
                                                if ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_BRASIL_PRO ||
                                                        $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA ||
                                                        ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS) ||
                                                        ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS)
                                                ) {
                                                    ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Itens de Composição</td>
                                                    </tr>
                                                    <tr>
                                                        <td id="itens_<?php echo $ano; ?>" colspan="2" >
                                                            <?php 
                                                                $oIC = new SubacaoItensComposicao();
                                                                $oIC->montaLista($_GET['sbaid'], $ano, $subacao['sbacronograma'], $valida, $reformulacao, $finalizado) 
                                                            ?>
                                                            <input type="hidden" name="itens[<?php echo $ano ?>]" value="<?php echo $oIC->conta($_GET['sbaid'], $ano); ?>" />
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td class="SubTituloEsquerda" colspan="2" >
                                                            <?php

                                                            if ($bloquearAnoSubacaoReprogramacao == 't') {
                                                                if (
                                                                    $db->testa_superuser() ||
                                                                    $validaBP ||
                                                                    $reformulacao == 'true' ||
                                                                    (
                                                                        $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO ||
                                                                        ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ATUALIZACAO && $_SESSION['par']['itrid'] == 1) ||
                                                                        ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA && ($ssuid[$ano] == 7 || $ssuid[$ano] == 10))
                                                                    ) &&
                                                                    (
                                                                        (
                                                                            (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)) &&
                                                                            $_SESSION['par']['itrid'] == 1) ||
                                                                        ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil)) && $_SESSION['par']['itrid'] == 2) ||
                                                                        (in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil))
                                                                    ) ||
                                                                    (
                                                                        ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE) &&
                                                                        (in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                            in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                                            (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO || $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES) && $permitido == 'S') ||
                                                                            (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $permitido == 'S') || ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $permitido == 'S'))) ||
                                                                            (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS))
                                                                        )
                                                                    ) ||
                                                                    (
                                                                        $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA &&
                                                                        (
                                                                            in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                            in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                                            in_array(PAR_PERFIL_PREFEITO, $arrayPerfil)
                                                                        )
                                                                    ) ||
                                                                    (in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) && $reprogr)
                                                                ) {
                                                                    if ($valida == 'true' && $testaCopia != 't') {
                                                                        $varPregVenc = $habilItemPregaoVencido ? '2' : '1';
                                                                        ?>
                                                                        <img class="middle link"
                                                                             onclick="inserirItemCompsicao('<?php echo $ano ?>', '<?php echo $_GET['sbaid'] ?>', '<?php echo $subacao['sbacronograma'] ?>',<?= $varPregVenc ?>)"
                                                                             src="../imagens/gif_inclui.gif"/>
                                                                        <a href="javascript:inserirItemCompsicao('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>','<?php echo $subacao['sbacronograma'] ?>',<?= $varPregVenc ?>)">Editar
                                                                            / Inserir Itens de Composição</a>
                                                                    <?php
                                                                    }
                                                                }
                                                            }
                                                            ?>
                                                        </td>
                                                    </tr>
                                                    <?
                                                }
                                                if ($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_BRASIL_PRO_OBRAS) {
                                                    ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Obras
                                                            <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'obras')">
                                                            &nbsp;
                                                            <img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'obras')">
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td id="obras_<?php echo $ano; ?>" colspan="2" style="display: none;">
                                                            <?php $oPre = new PreObra(); ?>
            <?php $oPre->montaLista($_GET['sbaid'], $ano); ?>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td class="SubTituloEsquerda" colspan="2" >
                                                            <?php
                                                            $variavel = 'baixo';
                                                            /* $dias = verificaQuantidadeDiasObraPendencias( $_SESSION['par']['itrid'], $inuid );

                                                              // Se for de emendas pode
                                                              $emendas = array(14, 15);
                                                              if( in_array( $subacao['frmid'], $emendas ) ){
                                                              $dias = 1;
                                                              }
                                                              if( $_SESSION['par']['itrid'] == 1 || $_SESSION['par']['estuf'] == 'DF' ){
                                                              if( $dias['qtdbloqueio'] > 0 ){
                                                              $variavel = 'alto';
                                                              }else{
                                                              $sql = "SELECT estado,stoid, count(1) as qtdbloqueio, sum(qtdmedio) as qtdmedio, sum(qtdbaixo) as qtdbaixo FROM (
                                                              SELECT
                                                              o.obrid,
                                                              o.estuf AS estado,
                                                              o.situacaoobra as stoid,
                                                              case when o.situacaoobra in (763, 689) and desterminodeferido is not null and desterminodeferido <= now() then 0
                                                              when o.situacaoobra in (763, 689) and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 then 1
                                                              else 0 end as bloqueiolicitacao,
                                                              case when o.situacaoobra in (690, 691) and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada,
                                                              case when o.situacaoobra in (690, 691) and o.diasultimaalteracao between 30 and 60 then 1 else 0 end as qtdmedio,
                                                              case when o.situacaoobra in (690, 691) and o.diasultimaalteracao < 30 then 1 else 0 end as qtdbaixo,
                                                              o.diasultimaalteracao
                                                              --count(o.obrid) as qtdobras

                                                              FROM
                                                              obras2.v_obras_situacao_estadual o
                                                              WHERE
                                                              o.inuid = {$inuid}
                                                              --o.estuf = 'AL'
                                                              AND o.situacaoobra in (690, 691, 763, 689)
                                                              ) t where bloqueiolicitacao = 0 and bloqueioexecucaoparalisada = 0 and (qtdmedio > 0 or qtdbaixo > 0)
                                                              group by estado, stoid";
                                                              $arrVariavel = $db->pegaLinha($sql);

                                                              if($arrVariavel['qtdmedio'] > 0 ){
                                                              $variavel = 'medio';
                                                              } else {
                                                              $variavel = 'baixo';
                                                              }
                                                              }
                                                              } else {
                                                              if( ($dias['dias'] > 60 && ($dias['stoid'] == 1 || $dias['stoid'] == 690)) || //Em execução
                                                              ($dias['dias'] > 180 && ($dias['stoid'] == 2 || $dias['stoid'] == 691)) || //Paralisada
                                                              ($dias['dias'] > 365 && ($dias['stoid'] == 5 || $dias['stoid'] == 763)) || //Em Licitação
                                                              ($dias['dias'] > 209 && ($dias['stoid'] == 99 || $dias['stoid'] == 689)) //Em Planejamento pelo Proponente
                                                              ){
                                                              //não pode cadastrar a obra
                                                              $variavel = 'alto';
                                                              } elseif( $dias['dias'] < 30 ){
                                                              //pode cadastrar sem problemas
                                                              $variavel = 'baixo';
                                                              } elseif( $dias['dias'] <= 60 ){
                                                              //pode cadastrar mas recebe o alerta!
                                                              $variavel = 'medio';
                                                              }
                                                              } */

                                                            /* if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO) || ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE) ){
                                                              if( $valida == 'true' ){
                                                              if(
                                                              (	in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                              in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                              in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $arrayPerfil) ||
                                                              in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil)
                                                              )
                                                              ){ */

                                                            if (( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ) ||
                                                                    ( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE) && in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $arrayPerfil) ) ||
                                                                    ( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE) && in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) ) ||
                                                                    (
                                                                    (
                                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_PREFEITO, $arrayPerfil)
                                                                    ) && (($estadoAtualSubacao["esdid"] == WF_SUBACAO_ATUALIZACAO && $_SESSION['par']['itrid'] == 1) || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO)
                                                                    ) ||
                                                                    (
                                                                    (
                                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
                                                                    ) && (($estadoAtualSubacao["esdid"] == WF_SUBACAO_ATUALIZACAO && $_SESSION['par']['itrid'] == 1) || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO)
                                                                    ) ||
                                                                    ( $estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE && $valida == 'true' ) ||
                                                                    ( ( $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL || $estadoAtualSubacao["esdid"] == WF_SUBACAO_APROVACAO_CONDICIONAL) &&
                                                                    ( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ) ) ||
                                                                    ( in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) && $reprogr ) || ( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA && ($ssuid[$ano] == 7 || $ssuid[$ano] == 10) ) && (
                                                                    in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil)
                                                                    )
                                                                    )
                                                            ) {
                                                                if ($validaSalvar == 'true') {
                                                                    $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);

                                                                    if (!empty($possuiPendenciaObras) && !(in_array($subacao['frmid'], array(14, 15)))) {
                                                                        ?><a href="#" onclick="javascript: alert('Você não pode executar essa operação pois existe a necessidade de atualização dos dados no sistema de monitoramento de obras.');"><img class="middle link" src="../imagens/gif_inclui.gif"/>Inserir Obras</a><?php
                                                                        } else {
                                                                            ?><img class="middle link" onclick="inserirObras('<?php echo $ano ?>', '<?php echo $_GET['sbaid'] ?>', '<?php echo $variavel ?>')"  src="../imagens/gif_inclui.gif" /> <a href="javascript:inserirObras('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>','<?php echo $variavel ?>')">Inserir Obras</a><?php
                                                                        }
                                                                    }
                                                                }
                                                                //}
                                                                //}
                                                                ?>
                                                        </td>
                                                    </tr>
                                                    <?php
                                                }
                                                if ( (
                                                        ($subacao['sbacronograma'] == 1 && $subacao['itrid'] == 3) ||
                                                        ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $vinculaObra == 'S') || 
														( 
															( in_array(PAR_PERFIL_ADM_OBRAS, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ) 
															&& ( $subacao['ppsid'] == 1171 || $subacao['ppsid'] == 1256 || $subacao['ppsid'] == 563 || $subacao['ppsid'] == 924 || $subacao['ppsid'] == 906 || 
																$subacao['ppsid'] == 913 || $subacao['ppsid'] == 925 || $subacao['ppsid'] == 914 || $subacao['ppsid'] == 904 ))
														)
													) {
                                                    ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Vincular Obras</td>
                                                    </tr>
                                                    <tr>
                                                        <td id="itens_<?php echo $ano; ?>" colspan="2" >
                                                            <?php $oPre = new PreObra(); ?>
           													<?php $oPre->montaListaVinculadas($_GET['sbaid'], $ano); ?>
                                                        </td>
                                                    </tr>
                                               <?php if( $finalizado <> true ){?>
                                                    <tr>
                                                        <td class="SubTituloEsquerda" colspan="2" >
                                                            <img class="middle link" onclick="vincularObras('<?php echo $ano ?>', '<?php echo $_GET['sbaid'] ?>')"  src="../imagens/gif_inclui.gif" /> <a href="javascript:vincularObras('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>')">Vincular / Desvincular Obras</a>
                                                        </td>
                                                    </tr>
                                                <?php } ?>
                                                <?php } ?>
                                                <?php //if( !($subacao['frmid'] == 6 || $subacao['frmid'] == 11) ){ ?>
        <?php if (1 == 2) { ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Beneficiários</td>
                                                    </tr>
                                                    <tr>
                                                        <td id="beneficiarios_<?php echo $ano; ?>" colspan="2" >
                                                            <?php $oBen = new SubacaoBeneficiario(); ?>
            <?php $oBen->montaLista($_GET['sbaid'], $ano) ?>
                                                            <input type="hidden" name="beneficiarios[<?php echo $ano ?>]" value="<?php echo $oBen->conta($_GET['sbaid'], $ano); ?>" />
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td class="SubTituloEsquerda" colspan="2" >
                                                            <?php
                                                            /* if($estadoAtual['esdid'] == WF_ELABORACAO){ if( $valida == 'true' ){ if( !( in_array(PAR_PERFIL_CONSULTA,$arrayPerfil) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrayPerfil) && $_SESSION['par']['itrid'] == 2 ) || ( (in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrayPerfil)) && $_SESSION['par']['itrid'] == 1 || (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrayPerfil)) && $_SESSION['par']['itrid'] == 2 || in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil) || in_array(PAR_PERFIL_PREFEITO,$arrayPerfil) && $_SESSION['par']['itrid'] == 2 ) ){ */
                                                            if ($estadoAtual['esdid'] == WF_ELABORACAO) {
                                                                if ($valida == 'true' && $testaCopia != 't') {
                                                                    if (!(
                                                                            (in_array(PAR_PERFIL_CONSULTA, $arrayPerfil) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $arrayPerfil)) && $_SESSION['par']['itrid'] == 2 ) ||
                                                                            (
                                                                            ((in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)) && $_SESSION['par']['itrid'] == 1) ||
                                                                            ((in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil)) && $_SESSION['par']['itrid'] == 2) ||
                                                                            (in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) && $_SESSION['par']['itrid'] == 2) ||
                                                                            (
                                                                            (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO || $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES) && $permitido == 'S') ||
                                                                            (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && ( ($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $permitido == 'S') || ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $permitido == 'S') ) ) ||
                                                                            (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) )
                                                                            )
                                                                            )
                                                                    ) {
                                                                        ?>
                                                                        <img class="middle link" onclick="inserirBeneficiarios('<?php echo $ano ?>', '<?php echo $_GET['sbaid'] ?>')"  src="../imagens/gif_inclui.gif" /> <a href="javascript:inserirBeneficiarios('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>')">Editar / Inserir Beneficiários</a>
                                                                        <?php
                                                                    }
                                                                }
                                                            }
                                                            ?>
                                                        </td>
                                                    </tr>
                                                <? } ?>
        <?php 
        		
        		$liberaJulioDetalhamento = false;
        		if( in_array( $sbaid, array(4621983,4622306,4622318,4622319,4622594,4622606,4622607,4622608,4622616,4622625,4622952,4622953,4622954,4622955,4622965,4622600,4622601,4622602,4622603,4622604,4622605) ) ){
        			$liberaJulioDetalhamento = true;
        		}
        
        		if (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) && $detalha || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA && $detalha || $liberaJulioDetalhamento ) { ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Detalhamento</td>
                                                    </tr>
                                                    <tr id="parecer_<?php echo $ano; ?>">
                                                        <td class="SubTituloDireita">Detalhamento</td>
                                                        <td><?php echo campo_textarea("sbddetalhamento[$ano]", 'S', $habil, '', 100, 12, 0, '', '', '', '', '', $oSbd->sbddetalhamento); ?></td>
                                                    </tr>
                                                <?php } ?>
                                                <?php
                                                #Para esse objeto "Parecer da equipe técnica", não é adcionado nos perfis, clausulas de forma de execução, as que foram definidas pelas regras,
                                                #pois nesse momento oucutaria o objeto, não sendo necessário porque esse objeto é disabilitado. Dessa forma esse objeto obdece as regras definidas para os perfis.
                                                #Perfis esses:  PAR_PERFIL_EQUIPE_TECNICA, PAR_PERFIL_EQUIPE_FINANCEIRA
                                                if (!$travReformulacao && (($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO || $ssuid[$ano] == 21) &&
                                                        (
                                                        in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil)
                                                        )) ||
                                                        ( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL || $estadoAtualSubacao["esdid"] == WF_SUBACAO_APROVACAO_CONDICIONAL || $ssuid[$ano] == 21) &&
                                                        ( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                        in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ) )
                                                ) {
                                                    ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Parecer da Equipe Técnica</td>
                                                    </tr>
                                                    <tr id="parecer_<?php echo $ano; ?>">
                                                        <td class="SubTituloDireita">Parecer:</td>
                                                        <td>
                                                            <?php //ver($habilParecer);   ?>
            <?php echo campo_textarea("sbdparecer[$ano]", 'N', $habilParecer, '', 100, 12, 0, '', '', '', '', '', $oSbd->sbdparecer); ?>
                                                            <div style="display:none;" id="div_adiciona_parecer_<?php echo $ano; ?>">
                                                                <input type="hidden" value="" name="bo_alterar_parecer[<?php echo $ano; ?>]" />
                                                                <?php
                                                                $mostra = $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO ? "disabled=disabled" : "";
                                                                if ($ssuid[$ano] == 21)
                                                                    $mostra = "";
                                                                ?>
                                                                <input type="button" <?= $mostra ?> value="Adicionar novo parecer" class="btnAddParecer" id="addParecer_<?php echo $ano ?>" />
                                                            </div>
                                                        </td>
                                                    </tr>
                                                    <?php
                                                    //$habilParecerM = (in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil)) ? 'S' : 'N';
                                                    if ($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA) {
                                                        if (in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) {
                                                            $habilParecerM = 'S';
                                                        } else {
                                                            $habilParecerM = 'N';
                                                        }
                                                        ?>
                                                        <tr id="parecerdemerito_<?php echo $ano; ?>">
                                                            <td class="SubTituloDireita">Parecer de Mérito:</td>
                                                            <td>
                <?php echo campo_textarea("sbdparecerdemerito[$ano]", 'N', $habilParecerM, '', 100, 12, 0, '', '', '', '', '', $oSbd->sbdparecerdemerito); ?>
                                                                <div style="display:none;" id="div_adiciona_merito_<?php echo $ano; ?>">
                                                                    <input type="hidden" value="" name="bo_alterar_merito[<?php echo $ano; ?>]" />
                                                                    <input type="button" value="Adicionar novo parecer de mérito" class="btnAddMerito" id="addMerito_<?php echo $ano ?>" />
                                                                </div>
                                                            </td>
                                                        </tr>
                                                    <?php } ?>
                                                    <?php
                                                    if (in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                            in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $arrayPerfil) ||
                                                            (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO || $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES) && $permitido == 'S') ||
                                                            (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $permitido == 'S') || ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $permitido == 'S')) ) ||
                                                            (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) )
                                                    ) {
                                                        ?>
                                                        <tr id="parecerpadrao_<?php echo $ano; ?>">
                                                            <td class="SubTituloDireita">Parecer Padrão:</td>
                                                            <td><input type="checkbox" id="pp[<?php echo $ano ?>]" onclick="preencheParecer('<?php echo $ano; ?>', '<?php echo $_GET['sbaid']; ?>')"></td>
                                                        </tr>
                                                        <?php
                                                    }
                                                    $arrEsdidTravaStatusSub = Array(WF_SUBACAO_ELABORACAO,
							                                                        WF_SUBACAO_APROVACAO_CONDICIONAL,
							                                                        WF_SUBACAO_DILIGENCIA_CONDICIONAL
							                                                    	);
                                                    $arrSsuidReprogramacao = Array( 20, 21, 22, 23 ); // Se está em reformulação não pode alterar o status da subação. Esta açãoa é feita pelo sistema.
                                                    if (!in_array($estadoAtualSubacao["esdid"], $arrEsdidTravaStatusSub) && !in_array( $oSbd->ssuid, $arrSsuidReprogramacao ) ) {
                                                        ?>
                                                        <tr id="status_<?php echo $ano; ?>">
                                                            <td class="SubTituloDireita">Status da Subação:</td>
                                                            <td>
                                                                <?php
                                                                if (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) {
                                                                    $st = "'M', 'E' ";
                                                                } elseif (in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil)) {
                                                                    $st = "'M'";
                                                                } else {
                                                                    $st = "'E'";
                                                                }

                                                                /*                                                                 * * Lista de Status da Subação ** */
                                                                $sqlStatusSubacao = "
																SELECT 	ssuid AS codigo,
																		ssudescricao AS descricao
																FROM par.statussubacao
																WHERE 
																	ssutipostatus in ( {$st} )
																	AND ssuid NOT IN (20, 21, 22, 23)
																ORDER BY
																	2
															";

                                                                $db->monta_combo("ssuid[$ano]", $sqlStatusSubacao, $habilStatus, 'Selecione', '', '', '', '', '', 'ssuid_' . $ano, '', $oSbd->ssuid);
                                                                ?>
                                                            </td>
                                                        </tr>
                                                        <?
                                                    }else{
														?>
	                                                    <tr id="status_<?php echo $ano; ?>">
	                                                        <td class="SubTituloDireita">Status da Subação:</td>
	                                                        <td>
	                                                            <?php
	                                                            /*                                                             * * Lista de Status da Subação ** */
	                                                            if ($oSbd->ssuid) {
	                                                                $sqlStatusSubacao = "
																			SELECT	ssudescricao
																			FROM par.statussubacao
																			WHERE ssuid =  {$oSbd->ssuid}
																	";
	                                                                echo $db->pegaUm($sqlStatusSubacao);
	                                                            }
	                                                            ?>
	                                                        </td>
	                                                    </tr>
														<?php 
                                                    }
                                                } elseif ( !$travReformulacao && $estadoAtualSubacao["esdid"] != WF_SUBACAO_ELABORACAO ) { //ELABORAÇÃO
                                                    ?>
                                                    <tr>
                                                        <td colspan="2" class="SubTituloTabela">Parecer da Equipe Técnica</td>
                                                    </tr>
                                                    <tr id="parecer_<?php echo $ano; ?>">
                                                        <td class="SubTituloDireita">Parecer:</td>
                                                        <td><?php echo $oSbd->sbdparecer; ?></td>
                                                    </tr>
                                                    <tr id="parecerdemerito_<?php echo $ano; ?>">
                                                        <td class="SubTituloDireita">Parecer de Mérito:</td>
                                                        <td><?php echo $oSbd->sbdparecerdemerito; ?></td>
                                                    </tr>
                                                    <tr id="status_<?php echo $ano; ?>">
                                                        <td class="SubTituloDireita">Status da Subação:</td>
                                                        <td>
                                                            <?php
                                                            /*                                                             * * Lista de Status da Subação ** */
                                                            if ($oSbd->ssuid) {
                                                                $sqlStatusSubacao = "
																		SELECT	ssudescricao
																		FROM par.statussubacao
																		WHERE ssuid =  {$oSbd->ssuid}
																";
                                                                echo $db->pegaUm($sqlStatusSubacao);
                                                            }
                                                            ?>
                                                        </td>
                                                    </tr>
                                                    <?php
                                                    if ($oSbd->ssuid == 23) {
                                                        $motivoDiligencia = $db->pegaUm("select rsdmotivo from par.reprogramacaosubacaodiligencia WHERE sbdid = " . $sbdid . " AND rsdstatus = 'A' AND rsdfluxo = 'E'");
                                                        ?>
                                                        <tr id="status_<?php echo $ano; ?>">
                                                            <td class="SubTituloDireita">Motivo da Diligência:</td>
                                                            <td>
                                                                <?php
                                                                echo $motivoDiligencia;
                                                                ?>
                                                            </td>
                                                        </tr>
                                                        <?php
                                                    }
                                                }
                                            }
                                            if (!bloqueiaEquipeLocalOnibusAcessivel($_REQUEST)) {
                                                $boImpositivo = verificaEmendaImpositiva($inuid, $ano);
                                                if ($boImpositivo > 0 && ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS)) {
                                                    ?>
                                                    <tr>
                                                        <td class="SubTituloDireita">Orçamento Impositivo:</td>
                                                        <td><input type="button" name="btnOrcamento" value="Orçamento Impositivo" onclick="orcamentoImpositivo('<?= $ano ?>', '<?= $sbaid ?>')"></td>
                                                    </tr>
                                                    <?
                                                }
                                            }
                                            ?>
                                        </table>
                                        <?php
                                        if ($sbdid) {
                                            $sql = "select
													sdhid,
													sdhparecer,
													case when sdhtipoparecer = 'T' then 'Parecer Técnico'
														 when sdhtipoparecer = 'M' then 'Parecer de Mérito'
													else sdhtipoparecer end as sdhtipoparecer
												from
													par.subacaodetalhehistorico sbh
												where
													sbh.sbdid = {$sbdid}";

                                            $cabecalho = array('N.º', 'Parecer', 'Tipo de Parecer');

                                            echo '<div class="mostrar_ocultar_historico" style="cursor:pointer;background:#f2f2f2;padding:5px;">
												<img src="../imagens/mais.gif" class="img_mais_menos" />
												<img src="../imagens/menos.gif" class="img_mais_menos" style="display:none;"/>
												Histórico de Parecer
											  </div>';

                                            echo '<div id="div_historico_subacao" style="display:none;">';
                                            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', array(60, null, 100));
                                            echo '</div>';
                                        }
                                        ?>
                                    </td>
                                </tr>
                                <tr id="linhaAno_<?php echo $ano ?>" style="display: none">
                                    <td bgcolor="#F5F5F5" colspan="5" align="center" >
                                        <table width="100%" border="0" cellpadding="0" cellspacing="1" >
                                            <tr>
                                                <td width="15%" align="left" >
                                                    <input onclick="controleproxAntSubacao('<?php echo $sbaidAnterior; ?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo!$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
                                                </td>
                                                <td width="60%" align="center" >
                                                    <?php
                                                    if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE) {
                                                        if (in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil) ||
                                                                in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                                in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                (in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO || $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES) && $permitido == 'S') ||
                                                                (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $permitido == 'S') || ($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $permitido == 'S') ) ) ||
                                                                (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) )
                                                        ) {
                                                            $validaSalvar = 'true';
                                                        }
                                                    } else if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) {
                                                        if ($ssuid[$ano] == 7 || $ssuid[$ano] == 10 || $ssuid[$ano] == 22) {
                                                            $validaSalvar = 'true';
                                                            $disable = '';
                                                        } else {
                                                            $validaSalvar = 'false';
                                                            $disable = 'disabled="disabled"';
                                                        }
                                                    } else if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL) {
                                                        if ($ssuid[$ano] == 22) {
                                                            $validaSalvar = 'true';
                                                            $disable = '';
                                                        } else {
                                                            $validaSalvar = 'false';
                                                            $disable = 'disabled="disabled"';
                                                        }
                                                    } else if ($reformulacao == 'true') {
                                                        if (( in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil)) && ($ssuid[$ano] == 20 || $ssuid[$ano] == 23) || (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) && $ssuid[$ano] == 21) {
                                                            $validaSalvar = 'true';
                                                            $disable = 'disabled="disabled"';
                                                        } else {
                                                            $validaSalvar = 'false';
                                                            $disable = 'disabled="disabled"';
                                                        }
                                                    }
                                                    if ($travReformulacao) {
                                                        $validaSalvar = 'false';
                                                        $disable = 'disabled="disabled"';
                                                    }

                                                    #Equipe Estadual e Municipal quando em situação de analise não pode atualizar subação.
                                               //     if (!bloqueiaEquipeLocalOnibusAcessivel($_REQUEST)) {
                                                                    //ver($reformulacao, d);
                                                    if ($bloquearAnoSubacaoReprogramacao == 't') { #Disponibiliza botao apenas quando subacao estiver em reprogramacao
                                                        if ($validaSalvar == 'true' || $reformulacao == 'true') {
                                                            if (
                                                                    (
                                                                    in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
                                                                    $validaBP
                                                                    ) ||
                                                                    (
                                                                    (
                                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
                                                                    ) &&
                                                                    $_SESSION['par']['itrid'] == 1
                                                                    ) &&
                                                                    ( $estadoAtualSubacao["esdid"] != WF_SUBACAO_ANALISE || $reformulacao == 'true') ||
                                                                    (
                                                                    (
                                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) ||
                                                                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil)
                                                                    ) && $_SESSION['par']['itrid'] == 2 && $estadoAtualSubacao["esdid"] != WF_SUBACAO_ANALISE || $reformulacao == 'true'
                                                                    ) ||
                                                                    (in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) && $_SESSION['par']['itrid'] == 2 ) ||
                                                                    in_array(PAR_PERFIL_ANALISTA_MERITOS, $arrayPerfil) ||
                                                                    (
                                                                    in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) &&
                                                                    (
                                                                    $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO ||
                                                                    $subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO ||
                                                                    $subacao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA ||
                                                                    $subacao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES
                                                                    ) &&
                                                                    $permitido == 'S'
                                                                    ) ||
                                                                    (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) && (($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA ) && $permitido == 'S') ) ||
                                                                    (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && ($subacao['frmid'] == FORMA_EXECUCAO_ACAO_PAC || $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) ) ||
                                                                    (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arrayPerfil) && in_array($subacao['frmid'], $emendas) ) ||
                                                                    (in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $arrayPerfil) && in_array($subacao['frmid'], $emendas) )
                                                            ) {
                                                                ?>
                                                                <input type="button" value="Salvar e Anterior" name="btn_salvar_anterior_<?php echo $ano ?>" <?php echo!$sbaidAnterior ? 'disabled="disabled"' : ''; ?> onclick="salvar('<?php echo $sbaidAnterior; ?>', '<?= $estadoAtualSubacao["esdid"] ?>', '<?= $oSbd->ssuid ?>')"  />
                                                                <input type="button" value="Salvar" name="btn_salvar_<?php echo $ano ?>" onclick="salvar(<?= $sbaid ?>, '<?= $estadoAtualSubacao["esdid"] ?>', '<?= $oSbd->ssuid ?>')"  />
                                                                <input type="button" value="Salvar e Próxima" name="btn_salvar_proximo_<?php echo $ano ?>" <?php echo!$sbaidProximo ? 'disabled="disabled"' : ''; ?> onclick="salvar('<?php echo $sbaidProximo; ?>', '<?= $estadoAtualSubacao["esdid"] ?>', '<?= $oSbd->ssuid ?>')"  />
                                                                <?php
                                                            }
                                                        }
                                                    }
                                                    $sql = "select count(pp.sbdid)
															from par.empenhosubacao es
															inner join par.empenho e on e.empid = es.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
															inner join par.subacaodetalhe sd on sd.sbaid = es.sbaid
															inner join par.processoparcomposicao pp on pp.sbdid = sd.sbdid  and pp.ppcstatus = 'A'
															where e.empsituacao <> 'CANCELADO' AND sd.sbaid = {$sbaid} and sd.sbdano = '$ano' ";
                                                    $totEmpenho = $db->pegaUm($sql);

                                                    if ($bloquearAnoSubacaoReprogramacao == 't') { #Disponibiliza botao apenas quando subacao estiver em reprogramacao
                                                        if (!bloqueiaEquipeLocalOnibusAcessivel($_REQUEST) && (int)$totEmpenho < 1) {
                                                            if (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE && (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil))) || (($estadoAtual["esdid"] == WF_ELABORACAO || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) && (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)))) {
                                                                ?>
                                                                <input type="button" value="Limpar Subação"
                                                                       name="btn_limpar_<?php echo $ano ?>" <?php echo $disable; ?>
                                                                       onclick="limpar('<?php echo $sbaid; ?>')"/>
                                                            <?php
                                                            }
                                                        }
                                                    }
                                                    if(!bloqueiaEquipeLocalOnibusAcessivel($_REQUEST)) {
														$sql = "SELECT sbdid FROM par.subacaodetalhe WHERE sbaid = $sbaid AND sbdano = $ano";
														$sbdid = $db->pegaUm($sql);
														if( $sbdid ){
															$arrEmpenho = getEmpenhoAgrupadoSubacao( Array( $sbdid ) );
														}
                                                        if( $validaSalvar == 'true' && !$arrEmpenho[0] ) {
                                                            if ( 	
																in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || 
																in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || 
																(
																	$estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE && 
																	( in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) )
																) || 
																( 
																	( 
																		$estadoAtual["esdid"] == WF_ELABORACAO || 
																		($estadoAtualSubacao["esdid"] == WF_SUBACAO_ATUALIZACAO && $_SESSION['par']['itrid'] == 1) || 
																		$estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA
																	) && 
																	(
																		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) || 
																		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) || 
																		in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || 
																		in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) || 
																		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
																	)
																) && ( $ano >= date('Y') )
															) {
                                                                ?>
                                                                <!--  
                                                                <input type="button" value="Importar Dados" name="btn_importar_<?php echo $ano ?>" onclick="return importarDados('<?php echo $sbaid; ?>')"  />
                                                                -->
                                                                <?
                                                            }
                                                        }
                                                    }
                                                    ?>

                                                    <input type="button" value="Fechar" name="btn_fechar" onclick="fechar()"  />
                                                </td>
                                                <td width="15%" align="right" >
                                                    <input onclick="controleproxAntSubacao('<?php echo $sbaidProximo; ?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo" <?php echo!$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                </tr>
                            <? }// Fim do foreach anos ?>
<? // Inicio do totalizadores   ?>
                            <tr id="abaAnostotalizadores" class="abaAnos">
                                <td colspan="<?php echo $colspan; ?>" >
                                    <?php $oSbd = new SubacaoDetalhe(); ?>
                                    <?php
                                    $arrSbdid = $oSbd->retornaDetalheSubacaoTotal($sbaid);

                                    foreach ($anos as $ano) {
                                        $select .= "coalesce(sum(ano" . $ano . "),0) AS a" . $ano . ",";
                                        $case .= "CASE WHEN sbdano = " . $ano . " THEN coalesce(sbdquantidade,0) END AS ano" . $ano . ",";
                                    }

                                    $sql = "SELECT	{$select}
								                	coalesce(sum(stotal) ,0) AS total
								            FROM (
								                SELECT
												{$case}
								            	sum(sbdquantidade) as stotal
								            FROM par.subacaodetalhe where sbaid = {$sbaid} AND sbdano IN (" . implode(', ', $anos) . ") GROUP BY sbdano, sbdquantidade) AS valores";

                                    $anos[] = 'Total';
                                    ?>
                                    <table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
                                     
                                        
<?php if ($subacao['sbacronograma'] == 1) { ?>
                                            <tr>
                                                <td colspan="2" class="SubTituloTabela">Quantidades e Cronograma de Execução</td>
                                            </tr>
                                            <tr>
                                                <td colspan="2"><?php $db->monta_lista_simples($sql, $anos, 1000, 1000, 'N', '100%', 'N'); ?></td>
                                            </tr>
                                        <?php } ?>
<?php if ($subacao['sbacronograma'] == 2): ?>
                                            <tr>
                                                <td colspan="2" class="SubTituloTabela">Escolas</td>
                                            </tr>
                                            <tr>
                                                <td id="escolas_<?php echo $ano; ?>" colspan="2" >
                                                    <?php $oSes = new SubacaoEscola(); ?>
    <?php $oSes->montaLista($_GET['sbaid']) ?>
                                                </td>
                                            </tr>
                                            <?php
                                        endif;
                                        if ($subacao['frmid'] != FORMA_EXECUCAO_FINANCIAMENTO_BNDES) {
                                            if ($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA ||
                                                    ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS) ||
                                                    ($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS)
                                            ) {
                                                ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">Itens de Composição</td>
                                                </tr>
                                                <tr>
                                                    <td id="itens_<?php echo $ano; ?>" colspan="2" >
                                                        <?php $oIC = new SubacaoItensComposicao(); ?>
        <?php $oIC->montaLista($_GET['sbaid'], "totalizadores", $subacao['sbacronograma'], $valida) ?>
                                                    </td>
                                                </tr>
    <? } else if ($subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS) { ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">Obras</td>
                                                </tr>
                                                <tr>
                                                    <td id="obras_<?php echo $ano; ?>" colspan="2" >
                                                        <?php $oPre = new PreObra(); ?>
        <?php $oPre->montaLista($_GET['sbaid'], $ano) ?>
                                                    </td>
                                                </tr>
                                            <? } ?>
    <?php if (1 == 2) { ?>
                                                <tr>
                                                    <td colspan="2" class="SubTituloTabela">Beneficiários</td>
                                                </tr>
                                                <tr>
                                                    <td id="beneficiarios_<?php echo $ano; ?>" colspan="2" >
                                                        <?php $oBen = new SubacaoBeneficiario(); ?>
        <?php $oBen->montaLista($_GET['sbaid']) ?>
                                                    </td>
                                                </tr>
                                            <?php
                                            }
                                        }
                                        ?>
                                    </table>
                                </td>
                            </tr>
<? //fim do totalizadores  ?>
                        </table>
                    </td>

                    <td valign="top">
                        <?php
                        // WORKFLOW

                        $enviobpanalise = false;

                        if ($_SESSION['par']['itrid'] == 1) {

                            if ($subacao['ppsid']) {
                                $sql = "SELECT ppsid FROM par.subacaobpenvioanalise WHERE ppsid = " . $subacao['ppsid'] . " AND uf = '" . $subacao['estuf'] . "'";
                                $testaBP = $db->pegaUm($sql);

                                if ($testaBP == $subacao['ppsid']) {

                                    if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $arrayPerfil)) {
                                        if ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO) {
                                            $enviobpanalise = true;
                                        }
                                    }
                                }
                            }

                            $estadosWF = Array(WF_SUBACAO_ANALISE,
                                WF_SUBACAO_DILIGENCIA,
                                WF_SUBACAO_DILIGENCIA_CONDICIONAL,
                                WF_SUBACAO_APROVADA,
                                WF_SUBACAO_ARQUIVADA,
                                WF_SUBACAO_ANALISE_COMISSAO,
                                WF_SUBACAO_INDEFERIDA,
                                WF_SUBACAO_EMPENHO,
                                WF_SUBACAO_PAGAMENTO,
                                WF_SUBACAO_ATUALIZACAO,
                                WF_SUBACAO_AGUARDANDO_ANALISE_GESTOR);
                        } else {
                            $estadosWF = Array(WF_SUBACAO_ANALISE,
                                WF_SUBACAO_DILIGENCIA,
                                WF_SUBACAO_DILIGENCIA_CONDICIONAL,
                                WF_SUBACAO_APROVADA,
                                WF_SUBACAO_ARQUIVADA,
                                WF_SUBACAO_ANALISE_COMISSAO,
                                WF_SUBACAO_INDEFERIDA,
                                WF_SUBACAO_EMPENHO,
                                WF_SUBACAO_PAGAMENTO,
                                //	   WF_SUBACAO_ATUALIZACAO,
                                WF_SUBACAO_AGUARDANDO_ANALISE_GESTOR);
                        }
                        if (
                                ((
                                in_array($estadoAtualSubacao["esdid"], $estadosWF) ||
                                (
                                $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA &&
                                (
                                in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
                                )
                                ) ||
                                (
                                $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS &&
                                (
                                in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
                                )
                                ) ||
                                $sbaextraordinariatramita &&
                                (
                                in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
                                in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
                                )
                                ) && $testaCopia != 't')
                        ) {
                            wf_desenhaBarraNavegacao($subacao['docid'], array('sbaid' => $sbaid, 'inuid' => $inuid, 'itrid' => $_SESSION['par']['itrid']));
                        } elseif ($enviobpanalise) {
                            ?> 
                            <br>
                            <table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                                <tr style="background-color: #c9c9c9; text-align: center;">
                                    <td style="font-size: 7pt; text-align: center;">
                                        <span title="Pendências de Preenchimento do Plano de Trabalho"><strong>Brasil Pró</strong></span> 
                                    </td>
                                </tr>

                                <tr style="text-align: center;">
                                    <td style="font-size: 7pt; text-align: center;">
                                        <a onclick="encaminhaAnaliseBP()" title="Encaminhar para Análise do MEC">Encaminhar para Análise do MEC</a>
                                    </td>
                                </tr>
                            </table><br><br>
                            <?php
                        }
                        ?>
                    </td>
                </tr>
            </table>
        </form>
        <script type="text/javascript"><!--

            function encaminhaAnaliseBP() {
                if (confirm("Tem certeza que deseja enviar para análise?")) {
                    window.location.href = window.location + "&enviaAnaliseBP=1";
                }
            }

            function orcamentoImpositivo(ano, sbaid) {
                window.open('par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano=' + ano + '&sbaid=' + sbaid, 'Orcamento', 'scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600');
                w.focus();
            }

            function number_format(number, decimals, dec_point, thousands_sep) {
                // %     nota 1: Para 1000.55 retorna com precisão 1 no FF/Opera é 1,000.5, mas no IE é 1,000.6
                // *     exemplo 1: number_format(1234.56);
                // *     retorno 1: '1,235'
                // *     exemplo 2: number_format(1234.56, 2, ',', ' ');
                // *     retorno 2: '1 234,56'
                // *     exemplo 3: number_format(1234.5678, 2, '.', '');
                // *     retorno 3: '1234.57'
                // *     exemplo 4: number_format(67, 2, ',', '.');
                // *     retorno 4: '67,00'
                // *     exemplo 5: number_format(1000);
                // *     retorno 5: '1,000'
                // *     exemplo 6: number_format(67.311, 2);
                // *     retorno 6: '67.31'

                var n = number, prec = decimals;
                n = !isFinite(+n) ? 0 : +n;
                prec = !isFinite(+prec) ? 0 : Math.abs(prec);
                var sep = (typeof thousands_sep == "undefined") ? ',' : thousands_sep;
                var dec = (typeof dec_point == "undefined") ? '.' : dec_point;

                var s = (prec > 0) ? n.toFixed(prec) : Math.round(n).toFixed(prec); //fix for IE parseFloat(0.55).toFixed(0) = 0;

                var abs = Math.abs(n).toFixed(prec);
                var _, i;

                if (abs >= 1000) {
                    _ = abs.split(/\D/);
                    i = _[0].length % 3 || 3;

                    _[0] = s.slice(0, i + (n < 0)) +
                            _[0].slice(i).replace(/(\d{3})/g, sep + '$1');

                    s = _.join(dec);
                } else {
                    s = s.replace('.', dec);
                }

                return s;
            }


            function importarDados(sbaid) {
                /*
                window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/par/par.php?modulo=principal/importarDadosSubacao&acao=A&sbaid=' + sbaid + '&ano=' + $('#anoatual').val(), 'Importar Dados', 'width=500,height=300,scrollbars=1');
                */
            }

            $(function() {
                if ($('#anoatual').val()) {
                    selecionarAba($('#anoatual').val());
                } else {
                    selecionarAba('<?php echo date("Y"); ?>');
                    $('#anoatual').val('<?php echo date("Y"); ?>', '');
                }
            });

			$(document).ready(function() {
                $('input').keyup();

                <?php if( 	verificaPendenciasComplementoSubacaoRef( $sbaid )
						&& in_array(21, $ssuid)
						&& (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || 
							in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || 
							in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) || 
							in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) ) ){ ?>
				$.ajax({
			   		type: 	"POST",
			   		url: 	window.location.href,
			   		data: 	'requisicaoAjax=form_pendenciasComplementoReformulacao&sbaid='+<?=$sbaid ?>,
			   		async: 	false,
			   		success: function(msg){
			   			$( "#div_dialog" ).attr('title', 'Histórico de Complementos de Subação');
			   			$( "#div_dialog" ).html(msg);		
			   			$( "#div_dialog" ).show();		
			   			$( "#div_dialog" ).dialog({
							resizable: true,
							width: 750,
							modal: true,
							closeOnEscape: false,
							show: { effect: 'drop', direction: "up" },
							open: function(event, ui) { $(".ui-dialog-titlebar-close", ui.dialog || ui).hide(); },
							buttons: {
								"Salvar": function() {
									$.ajax({
								   		type: 	"POST",
								   		url: 	window.location.href,
								   		data: 	'requisicaoAjax=salvar_pendenciasComplementoReformulacao&'+$('#form_complemento').serialize(),
								   		async: 	false,
								   		success: function(msg){
									   		if( msg == 'sucesso' ){
										   		$( "#div_alert_dialog" ).attr('title', 'Sucesso');
									   			$( "#div_alert_dialog" ).css('font-size', '12px');
									   			$( "#div_alert_dialog" ).html('<center><b>Complento(s) salvo(s) com sucesso!</b></center>');		
									   			$( "#div_alert_dialog" ).show();		
									   			$( "#div_alert_dialog" ).dialog({
													resizable: true,
													width: 600,
													modal: true,
													show: { effect: 'drop', direction: "up" },
													buttons: {
														"OK": function() {
															$( this ).dialog( "close" );
												   			$( '#div_dialog' ).dialog( "close" );
														}
													}
												});
										   	}else{
									   			$( "#div_alert_dialog" ).attr('title', 'Erro ao salvar complementos');
									   			$( "#div_alert_dialog" ).css('font-size', '11px');
									   			$( "#div_alert_dialog" ).html(msg);		
									   			$( "#div_alert_dialog" ).show();		
									   			$( "#div_alert_dialog" ).dialog({
													resizable: true,
													width: 700,
													modal: true,
													show: { effect: 'drop', direction: "up" },
													buttons: {
														"Close": function() {
															$( this ).dialog( "close" );
														}
													}
												});
											}
								   		}
									});
								}
							}
						});
			   		}
				});
			<?php } ?>
			});

            //sbdrepassecomplementar
            $('[name^="sbdrepasse"][type="radio"]').click(function() {
                $('.abaAnos').each(function(i, obj) {
                    if ($(obj).css('display') != 'none') {
                        abaid = $(obj).attr('id');
                    }
                });
                var ano = abaid.substring(7, 11);
                var qtd = $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name^="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name^="sbdrepasseraf[' + ano + ']"][value="t"]:checked').length;
                if (qtd > 1) {
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="f"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="f"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="f"]:checked').parent().find('span').hide();
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="f"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="f"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="f"]:checked').parent().find('span').find('input').val('0');
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').show();
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').find('input').val('0,00');
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').find('input').removeClass('disabled');
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').find('input').attr('readonly', false);
                } else {
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="f"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="f"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="f"]:checked').parent().find('span').hide();
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="f"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="f"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="f"]:checked').parent().find('span').find('input').val('0');
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').show();
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').find('input').val(number_format($('[name=diferenca[' + ano + ']]').val(), 2, ',', '.'));
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').find('input').addClass('disabled');
                    $('[name="sbdrepassecomplementar[' + ano + ']"][value="t"]:checked,[name="sbdrepasseempenho[' + ano + ']"][value="t"]:checked,[name="sbdrepasseraf[' + ano + ']"][value="t"]:checked').parent().find('span').find('input').attr('readonly', true);
                }
            });



            $('input[name^="sbdrepassevlr"]').blur(function() {
                var soma = 0;
                ano = $('#anoatual').val();
                var max = parseFloat($('[name=diferenca[' + ano + ']]').val()).toFixed(2);
                var vlr = 0;
                $('[name^="sbdrepassevlr"]').each(function() {
                    vlr = replaceAll($(this).val(), '.', '');
                    vlr = replaceAll(vlr, ',', '.');
                    if (parseFloat(vlr) > 0) {
                        soma = parseFloat(soma) + parseFloat(vlr);
                    }
                });
                vlr = replaceAll($(this).val(), '.', '');
                vlr = replaceAll(vlr, ',', '.');
                if (soma > max) {
                    $(this).val(number_format(parseFloat(max - (soma - parseFloat(vlr))), 2, ',', '.'));
                }
            });

            function verificaValorReformulacao(valorComp) {
                var total = new Number();
                var valorComp = new Number(valorComp);
                $('[name^="sbdrepassevlr"]').each(function() {
                    vlr = this.value.replace('.', '');
                    vlr = vlr.replace(',', '.');
                    valor = new Number(vlr);
                    total = new Number(total + valor);
                });

                if (total > valorComp) {
                    alert('O valor é maior que o permitido!');
                    $('[name^="btn_salvar_"]').attr('disabled', 'disabled');
                } else {
                    $('[name^="btn_salvar_"]').attr('disabled', '');
                }
            }

            function finalizaReformulacao(sbaid, tipo) {
                ano = $('#anoatual').val();

                var complementoLocal = null;
                var complementoEmpenho = null;
                var complementoRAF = null;
                var vlrDiferenca = null;
                var valorComplemento = null;
                var valorJustificado = null;
                var dados = null;
                var erro = false;

                if (($('[name="ssuids[' + ano + ']"]').val() == 20) && parseFloat($('[name=diferenca[' + ano + ']]').val()) > 0) {
                    vlrDiferenca = parseFloat($('[name=diferenca[' + ano + ']]').val()).toFixed(2);
                    complementoLocal = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrcomplementar[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoEmpenho = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrempenho[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoRAF = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrraf[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    valorComplemento = vlrDiferenca - (parseFloat(complementoLocal) + parseFloat(complementoEmpenho) + parseFloat(complementoRAF));
                    if (valorComplemento > 0) {
                        alert('Favor justificar o valor excedido nos campos de complementação.\n R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' ainda não foi justificado.');
                        return false;
                    }
                    if (valorComplemento < 0) {
                        alert('Foi justificado R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' a mais. Favor ajustar.');
                        return false;
                    }

                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=verificaComplementoDB&ssuid=" + $('[name="ssuids[' + ano + ']"]').val() + "&ano=" + ano + "&sbaid=" + sbaid,
                        async: false,
                        success: function(resp) {
                            dados = resp.split('}{');
                            if (complementoLocal != dados[0] || complementoRAF != dados[2]) {
                                alert('Favor salvar os dados de complementação antes de finalizar a Reprogramação.');
                                erro = true;
                                return false;
                            }
                        }
                    });
                    if (erro) {
                        return false;
                    }
                }

                if (($('[name="ssuids[' + ano + ']"]').val() == 23) && parseFloat($('[name=diferenca[' + ano + ']]').val()) > 0) {
                    vlrDiferenca = parseFloat($('[name=diferenca[' + ano + ']]').val()).toFixed(2);
                    complementoLocal = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrcomplementar[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoEmpenho = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrempenho[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoRAF = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrraf[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    valorComplemento = vlrDiferenca - (parseFloat(complementoLocal) + parseFloat(complementoEmpenho) + parseFloat(complementoRAF));
                    if (valorComplemento > 0) {
                        alert('Favor justificar o valor excedido nos campos de complementação.\n R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' ainda não foi justificado.');
                        return false;
                    }
                    if (valorComplemento < 0) {
                        alert('Foi justificado R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' a mais. Favor ajustar.');
                        return false;
                    }

                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=verificaComplementoDB&ssuid=" + $('[name="ssuids[' + ano + ']"]').val() + "&ano=" + ano + "&sbaid=" + sbaid,
                        async: false,
                        success: function(resp) {
                            dados = resp.split('}{');
                            if (complementoLocal != dados[0] || complementoRAF != dados[2]) {
                                alert('Favor salvar os dados de complementação antes de finalizar a Reprogramação.');
                                erro = true;
                                return false;
                            }
                        }
                    });
                    if (erro) {
                        return false;
                    }
                }

                if (($('[name="ssuids[' + ano + ']"]').val() == 21 || $('[name="ssuids[' + ano + ']"]').val() == 22) && parseFloat($('[name=diferenca[' + ano + ']]').val()) > 0) {
                    vlrDiferenca = parseFloat($('[name=diferenca[' + ano + ']]').val()).toFixed(2);
                    complementoLocal = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrcomplementaraprovado[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoEmpenho = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrempenhoaprovado[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoRAF = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrrafaprovado[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    valorComplemento = vlrDiferenca - (parseFloat(complementoLocal) + parseFloat(complementoEmpenho) + parseFloat(complementoRAF));

                    if (valorComplemento > 0) {
                        alert('Favor justificar o valor excedido nos campos de complementação.\n' + 'R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' ainda não foi justificado.');
                        return false;
                    }
                    if (valorComplemento < 0) {
                        alert('Foi justificado R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' a mais. Favor ajustar.');
                        return false;
                    }

                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=verificaComplementoDB&ssuid=" + $('[name="ssuids[' + ano + ']"]').val() + "&ano=" + ano + "&sbaid=" + sbaid,
                        async: false,
                        success: function(resp) {
                            dados = resp.split('}{');
                            if (complementoLocal != dados[0] || complementoEmpenho != dados[1] || complementoRAF != dados[2]) {
                                alert('Favor salvar os dados de complementação antes de finalizar a Reprogramação.');
                                erro = true;
                                return false;
                            }
                        }
                    });
                    if (erro) {
                        return false;
                    }
                }

                if (confirm("Tem certeza que deseja finalizar a Reprogramação?")) {
                    $("[name='requisicao']").val('finalizarReformulacao');
                    $("#tipoReform").val(tipo);
                    $("#anoatual").val(ano);
                    $('#form_subacao').submit();
                }
            }

            function diligenciaReformulacao(sbaid, ano) {
                $('#diligencia_' + sbaid + '_' + ano).show();
            }

            function enviaDiligencia(sbaid, ano) {
                if (confirm("Tem certeza que deseja solicitar diligência de reprogramação?")) {
                    if ($("#diligencia_" + ano).val() == '') {
                        alert('Preencha o motivo da diligência!');
                        return false;
                    } else {
                        $("[name='requisicao']").val('enviaDiligencia');
                        $("#anoatual").val(ano);
                        $('#form_subacao').submit();
                    }
                }
            }

            function cancelaReformulacaoSubacao(sbaid, ano) {
                $('#divcancela_' + sbaid + '_' + ano).show();
            }

            function cancelaReformulacao(sbaid, ano) {
                if (confirm("Tem certeza que deseja cancelar a Reprogramação?")) {
                    if ($("#cancela_" + ano).val() == '') {
                        alert('Preencha o motivo do cancelamento!');
                        return false;
                    } else {
                        $("[name='requisicao']").val('cancelarReformulacao');
                        $("#anoatual").val(ano);
                        $('#form_subacao').submit();
                    }
                }
            }

            function abrefecha(funcao, ano, tipo) {
                if (funcao == 'abre') {
                    $("#" + tipo + "_" + ano).show();
                } else {
                    $("#" + tipo + "_" + ano).hide();
                }
            }

            function controleproxAntSubacao(idSubacao) {
                if (idSubacao && idSubacao != '') {
                    window.location.href = '/par/par.php?modulo=principal/subacao&acao=A&sbaid=' + idSubacao;
                }
            }

            function filtraPTRES(plicod) {
                ano = $('#anoatual').val();
                if (plicod != '') {
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=filtraPTRES&" + "sbdplanointerno=" + plicod + "&ano=" + ano,
                        success: function(resp) {
                            document.getElementById("td_ptres_" + ano).innerHTML = resp;
                        }
                    });
                }
            }

            function carregaEmendaPTA(ptrid) {
                ano = $('#anoatual').val();
                if (ptrid != '') {
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=carregaEmendaPTA&" + "ptrid=" + ptrid + "&ano=" + ano,
                        success: function(resp) {
                            document.getElementById("td_emenda_" + ano).innerHTML = resp;
                        }
                    });
                }
            }

            function popupSelecionarEscolasItemComposicao(sbaid, ano, icoid, tipo) {
                if(tipo == 'visu')
                {
                	url = "par.php?modulo=principal/quantidadeEscola&acao=A&ano=" + ano + "&sbaid=" + sbaid + "&icoid=" + icoid + "&tipo=tec" +"&visualizacao=visu";
	                janela(url, 900, 500, "Quantidade por escolas " + tipo);
                }
                else
                {
                	url = "par.php?modulo=principal/quantidadeEscola&acao=A&ano=" + ano + "&sbaid=" + sbaid + "&icoid=" + icoid + "&tipo=" + tipo;
	                janela(url, 900, 500, "Quantidade por escolas " + tipo);
                }
            }

            function validaItem(ele, sbaid, ano, icoid, cronograma) {
                //if( document.getElementById( 'valida['+ano+']['+icoid+']' ).checked == true ){ //está marcando
                if (ele.value == 'S') { //está marcando
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=validarItem&sbaid=" + sbaid + "&ano=" + ano + "&icoid=" + icoid + "&tipo=marca" + "&cronograma=" + cronograma,
                        success: function(msg) {
                            if (msg == 'Erro') {
                                alert('Problema na execução!');
                            } else {
                                //alert('Dados salvos com sucesso!');
                                if (cronograma == 'esc') {
                                    document.getElementById('icoqtd' + icoid).disabled = false;
                                }
                                document.getElementById('icoqtdtecnico' + icoid).disabled = false;
                                document.getElementById('icoqtdtecnico' + icoid).value = msg;
                            }
                        }
                    });
                } else { // está desmarcando
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=validarItem&sbaid=" + sbaid + "&ano=" + ano + "&icoid=" + icoid + "&tipo=desmarca" + "&cronograma=" + cronograma,
                        success: function(msg) {
                            if (msg == 'Erro') {
                                alert('Problema na execução!');
                            } else {
                                //alert('Dados salvos com sucesso!');
                                if (cronograma == 'esc') {
                                    document.getElementById('icoqtd' + icoid).disabled = true;
                                }
                                document.getElementById('icoqtdtecnico' + icoid).disabled = true;
                                document.getElementById('icoqtdtecnico' + icoid).value = 0;
                            }
                        }
                    });
                }
            }

            function validaEscola(ele, sbaid, ano, sesid, cronograma) {
                if (ele.value == 'S') { //está marcando sim
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=validarEscola&sbaid=" + sbaid + "&ano=" + ano + "&sesid=" + sesid + "&tipo=marca",
                        success: function(msg) {
                            if (msg == 'Erro') {
                                alert('Problema na execução!');
                            } else {
                                //alert('Dados salvos com sucesso!');
                                //document.getElementById( 'sesquantidade_'+sesid ).disabled = false;
                                document.getElementById('sesquantidadetecnico_' + sesid).disabled = false;
                                document.getElementById('sesquantidadetecnico_' + sesid).value = msg;
                            }
                        }
                    });
                } else { // está marcando não
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=validarEscola&sbaid=" + sbaid + "&ano=" + ano + "&sesid=" + sesid + "&tipo=desmarca",
                        success: function(msg) {
                            if (msg == 'Erro') {
                                alert('Problema na execução!');
                            } else {
                                //alert('Dados salvos com sucesso!');
                                document.getElementById('sesquantidade_' + sesid).disabled = true;
                                document.getElementById('sesquantidadetecnico_' + sesid).disabled = true;
                                document.getElementById('sesquantidadetecnico_' + sesid).value = 0;
                            }
                        }
                    });
                }
            }

            function recarregaEscolas(sbaid, ano, frmid, ptsid) {
                divCarregando();
                $.ajax({
                    type: "POST",
                    url: window.location,
                    data: "requisicaoAjax=recarregarEscola&sbaid=" + sbaid + "&ano=" + ano + "&frmid=" + frmid + "&ptsid=" + ptsid,
                    success: function(res) {
                        $('#escolas_' + ano).html(res);
                        divCarregado();
                    }
                });
            }

            function selecionarAba(cosano, booReprogramacao)
            {
                $('[id^="abaAnos"]').attr("style", "display:none");
                $('[id^="aba_"]').attr("style", "width:50px;margin: 0;padding: 0 0 0 5px;background: url(\"../../imagens/left_both.gif\") no-repeat left top;border-bottom: 1px solid #765;background-position: 0% 0%");
                $('[id^="aba_"] a').attr("style", "display: block;background: url(\"../../imagens/right_both.gif\") no-repeat right top;padding: 5px 0 4px 0;text-decoration: none;font-weight: bold;color: #765;width: auto;font-size:12px");
                $('#spanAno').innerHTML = cosano;
                $('#abaAnos' + cosano).attr("style", "display:table-row");
                $('#aba_' + cosano).attr("style", "width:50px;background-position:0% -150px;color: #333;");
                $('#aba_' + cosano + ' a:first').attr("style", "background-position:100% -150px;color: #333;");
                $('#anoatual').val(cosano);
                if (cosano == 'totalizadores') {
                    $('[id^="linhaAno_"]').attr("style", "display:none");
                    //	$('[id^="linhaAno_totalizadores"]').attr("style","");
                    $('input[name=btn_salvar_anterior]').attr("disabled", "disabled");
                    $('input[name=btn_salvar]').attr("disabled", "disabled");
                    $('input[name=btn_salvar_proximo]').attr("disabled", "disabled");
                    $('input[name=btn_limpar]').attr("disabled", "disabled");
                    $('input[name=btn_importar]').attr("disabled", "disabled");
                } else {
                    $('[id^="linhaAno_"]').attr("style", "display:none");
                    $('[id^="linhaAno_' + cosano + '"]').attr("style", "");
                    $('input[name=btn_salvar_anterior]').attr("disabled", "");
                    $('input[name=btn_salvar]').attr("disabled", "");
                    $('input[name=btn_importar]').attr("disabled", "");
                    $('input[name=btn_salvar_proximo]').attr("disabled", "");
                }
                $('[type="text"]').keyup();
                desabilitarCampos(cosano, booReprogramacao);
            }

            function exibeArvore()
            {
                window.opener.location.href = "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
                window.close();
            }

            function popupEditarSubacao(sbaid)
            {
                url = "par.php?modulo=principal/editarSubacao&acao=A&sbaid=" + sbaid;
                janela(url, 700, 550, "Edição deSubação")
            }

            function limpar(sbaid) {
                if (confirm('Tem certeza que deseja limpar a Subação?\nTodos os dados serão perdidos!')) {
                    $('[name=carregarSbaid]').val(sbaid);
                    $("[name='requisicao']").val('limpar');
                    $('#form_subacao').submit();
                }
            }

            function verificaItensMonitorados() {
                var qtd_atual = 0;
                var qtd_monitorada = 0;
                var msg_monitorada = 0;
                if (<?= $subacao['sbacronograma'] ?> == '1') {
                    $('[name="icoid_monitorado"]').each(function() {
                        qtd_monitorada = $('#vlrmin_' + $(this).val()).val();
                        qtd_atual = $('#icoqtdtecnico' + $(this).val()).val();
                        $('#icoqtdtecnico' + $(this).val()).css('color', 'black');
                        if (parseFloat(qtd_monitorada) > parseFloat(qtd_atual)) {
                            msg_monitorada = qtd_monitorada;
                            $('#icoqtdtecnico' + $(this).val()).css('color', 'red');
                        }
                    });
                }
                return msg_monitorada;
            }

            function salvar(sbaid, tipo, statusSubacao)
            {

                var qtd_monitorada = verificaItensMonitorados();

                if (qtd_monitorada > 0) {
                    alert('Aquantidade deve ser maior ou igual a monitorada.');
                    return false;
                }

                ano = $('#anoatual').val();

        //    			if( tipo == <?= WF_SUBACAO_DILIGENCIA ?> && '<?= $testaDiligenciaCondicional ?>' == 't'  ){
        //         			alert('aqui');
        //         			var valorItens 			= parseFloat(replaceAll(replaceAll($('#itens_'+ano).children(':first').children(':last').children(':last').children('[title="Valor Planejado"]').html(),'.',''),',','.')).toFixed(2);
        //         			var vlrEmpenho			= parseFloat($('#vlrEmpenho').val()).toFixed(2);
        //         			var complementoLocal 	= parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrcomplementar['+ano+']]').val(),'.',''),',','.')).toFixed(2);
        //         			var complementoEmpenho 	= parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrempenho['+ano+']]').val(),'.',''),',','.')).toFixed(2);
        //         			var complementoRAF 		= parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrraf['+ano+']]').val(),'.',''),',','.')).toFixed(2);
        //         			var valorSub			= parseFloat(valorItens) - (parseFloat(complementoLocal)+parseFloat(complementoEmpenho)+parseFloat(complementoRAF));
        //         			if( valorSub > vlrEmpenho ){
        //             			var valorUltrapassado = valorSub - vlrEmpenho;
        //             			alert('O valor dos Itens de composição ultrapassou o valor planejado para esta subação em '+mascaraglobal('[.###],##',valorUltrapassado.toFixed(2))+'.\nFavor justificar o valor excedido nos campos de complementação.');
        //             			return false;
        //             		}
        //     			}

                if (tipo == <?= WF_SUBACAO_DILIGENCIA ?> && '<?= $testaDiligenciaCondicional ?>' == 't') {
                    vlrDiferenca = parseFloat($('[name=diferenca[' + ano + ']]').val()).toFixed(2);
                    complementoLocal = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrcomplementaraprovado[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoEmpenho = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrempenhoaprovado[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    complementoRAF = parseFloat(replaceAll(replaceAll($('[name=sbdrepassevlrrafaprovado[' + ano + ']]').val(), '.', ''), ',', '.')).toFixed(2);
                    valorComplemento = vlrDiferenca - (parseFloat(complementoLocal) + parseFloat(complementoEmpenho) + parseFloat(complementoRAF));

                    if (valorComplemento > 0) {
                        alert('Favor justificar o valor excedido nos campos de complementação.\n' + 'R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' ainda não foi justificado.');
                        return false;
                    }
                    if (valorComplemento < 0) {
                        alert('Foi justificado R$ ' + mascaraglobal('[.###],##', valorComplemento.toFixed(2)) + ' a mais. Favor ajustar.');
                        return false;
                    }
                }

                if (!(statusSubacao == <?= STATUS_SUBACAO_EM_REFORMULACAO ?> || statusSubacao == <?= STATUS_SUBACAO_EM_DILIGENCIA_REPROGRAMACAO ?>)) {
                    if (tipo == 452) { //analise
                        var erroQtd = 0;
                        var todos = $('[name*=valida[' + ano + ']]').length;
                        var check = $('[name*=valida[' + ano + ']]:checked').length;
                        var todosE = $('[name*=validaE[' + ano + ']]').length;
                        var checkE = $('[name*=validaE[' + ano + ']]:checked').length;
                        var merito = $('[name*=merito[' + ano + ']]').val();


                        if (merito == 'N') {
                            if (parseInt(todos) - parseInt(check) != check) {
                                alert('Favor validar o item!');
                                return false;
                            }
                            if (parseInt(todosE) - parseInt(checkE) != checkE) {
                                alert('Favor validar as escolas!');
                                return false;
                            }
                            if ($('[name=sbdparecer[' + ano + ']]').val() == '') {
                                alert("É necessário preencher o Parecer!");
                                this.focus();
                                return false;
                            }
                        }

                        if ($('[name^=ssuid[' + ano + ']]').val() == '') {
                            alert("É necessário preencher o Status da Subação!");
                            this.focus();
                            return false;
                        }

                        if ($('#frmid').val() == 14 || $('#frmid').val() == 15) {
                            selectAllOptions(document.getElementById('emenda[' + ano + ']'));
                        }

                        $('[name=subacaoAnalise]').val('analise');
                        $('[name=carregarSbaid]').val(sbaid);
                        $('#form_subacao').submit();
                        return false;
                    }
                }

                quant = $('#quant').val();
                ptsid = $('#ptsid').val();
                esdid = $('#esdid').val();
                frmid = $('#frmid').val();
                inicio = new Number($('[name=sbdinicio[' + ano + ']]').val());
                fim = new Number($('[name=sbdfim[' + ano + ']]').val());

                if (inicio > fim) {
                    if ($('[name=sbdanotermino[' + ano + ']]').val() == '') {
                        alert('O Mês inicial só pode ser maior que o final se inserir um ano!');
                        return false;
                    }
                }

                if (quant != 2 && frmid != 6 && frmid != 11) {
                    quantidade = $('[name=sbdquantidade[' + ano + ']]').val();
                    if (!quantidade) {
                        alert('Por favor preencha a quantidade.');
                        return false;
                    }
                } else {
                    escolas = $('[name=escolas[' + ano + ']]').val();
                    if (escolas == 0) {
                        alert('Por favor, cadastre pelo menos uma escola!');
                        return false;
                    }
                }

                if (frmid == 6 || (frmid == 12 && ptsid == 45) || (frmid == 4 && ptsid == 43)) {
                    itens = $('[name=itens[' + ano + ']]').val();
                    if (itens == 0 || itens == '') {
                        alert('Por favor, cadastre pelo menos um Item de Composição!');
                        return false;
                    }
                    var erroQtd = 0;
                    $('[name*=icoqtd[' + ano + ']]').each(function() {
                        if (this.value == 0) {
                            erroQtd = 1;
                            alert('Favor preencher a quantidade do item!');
                            this.focus();
                            //return false;
                        }
                    });

                    if (erroQtd == 1) {
                        return false;
                    }
                }

                if (frmid == 6 || frmid == 11) {
                    beneficiarios = $('[name=beneficiarios[' + ano + ']]').val();
                    if (beneficiarios == 0 || beneficiarios == '') {
                        alert('Por favor, cadastre pelo menos um Beneficiário!');
                        return false;
                    }
                }

                detalha = $('[name=detalha]').val();
                if (detalha == 1) {
                    detalhamento = $('[name=sbddetalhamento[' + ano + ']]').val();
                    if (detalhamento == '') {
                        alert('Por favor, preencha o campo Detalhamento!');
                        return false;
                    }
                }

<?php if ($reformulacao == 'true') { ?>
                    var diferenca = parseFloat($('[name=diferenca[' + ano + ']]').val());
                    diferenca = diferenca.toFixed(2);
                    if (diferenca > 0) {
                        var valor = 0;
                        var vlr = 0;
                        $('[name^="sbdrepasse"][value="t"]:checked').each(function() {
                            vlr = replaceAll($(this).parent().find('span').find('input').val(), '.', '');
                            vlr = parseFloat(vlr.replace(',', '.'));
                            if (vlr > 0) {
                                valor = valor + vlr;
                            }
                            vlr = 0;
                        });
                        if (valor != diferenca) {
    <?php if ($_REQUEST['repasse'] == 1) { ?>
                                alert('Falta distribuir o valor a ser complementado.');
                                return false;
    <?php } ?>
                        }
                    }
<?php } ?>

                if ($('[id^="sbdfim_' + ano + '"]').val() && $('[id^="sbdinicio_' + ano + '"]').val()) {
                    if (sbaid) {
                        $('[name=carregarSbaid]').val(sbaid);
                    }

                    $('#form_subacao').submit();
                } else {
                    alert('Por favor preencha o cronograma.');
                    return false;
                }
            }

            function fechar()
            {
                window.opener.document.location.reload();
            }

            function verificaAnoValido(obj, ano)
            {
                if (obj.value < ano) {
                    alert('O ano de término deve ser maior que ' + ano + '!');
                    $('[name=' + obj.name + ']').val("");
                }
            }
            function inserirItemCompsicao(ano, sbaid, sbacronograma, pregvenc)
            {
                varpreg = '';
                if (pregvenc == 2) {
                    varpreg = '&pregoesvencidos=1';
                }
                if (sbacronograma == 2) {
                    escolas = $('[name=escolas[' + ano + ']]').val();
                    if (escolas == 0) {
                        alert('Por favor, cadastre pelo menos uma escola!');
                        //return false;
                    } else {
                        url = "par.php?modulo=principal/subacaoItemComposicao&acao=A&sbaid=" + sbaid + "&ano=" + ano + "&sbacronograma=" + sbacronograma + varpreg;
                        janela(url, 700, 550, "Item de Composição");
                    }
                } else {
                    url = "par.php?modulo=principal/subacaoItemComposicao&acao=A&sbaid=" + sbaid + "&ano=" + ano + "&sbacronograma=" + sbacronograma + varpreg;
                    janela(url, 700, 550, "Item de Composição");
                }
            }
            function inserirEscolas(ano, sbaid, frmid, ptsid)
            {
                url = "par.php?modulo=principal/subacaoEscolas&acao=A&sbaid=" + sbaid + "&ano=" + ano + "&frmid=" + frmid + "&ptsid=" + ptsid;
                janela(url, 700, 550, "Escolas da Subação");
            }
            function inserirObras(ano, sbaid, variavel)
            {
                if (variavel == 'baixo') {
                    url = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=" + sbaid + "&ano=" + ano;
                    janela(url, 700, 550, "Obras da Subação");
                } else if (variavel == 'medio') {
                    if (confirm('Existe a necessidade de atualização dos\ndados no sistema de monitoramento de obras.')) {
                        url = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=" + sbaid + "&ano=" + ano;
                        janela(url, 700, 550, "Obras da Subação");
                    } else {
                        url = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=" + sbaid + "&ano=" + ano;
                        janela(url, 700, 550, "Obras da Subação");
                    }
                } else {
                    alert('Você não pode efetuar essa operação pois existe a necessidade\nde atualização dos dados no sistema de monitoramento de obras.');
                }
            }
            function vincularObras(ano, sbaid)
            {
                url = "par.php?modulo=principal/subacaoObrasVinculacao&acao=A&sbaid=" + sbaid + "&ano=" + ano;
                janela(url, 700, 550, "Obras da Subação");
            }
            function inserirBeneficiarios(ano, sbaid)
            {
                url = "par.php?modulo=principal/subacaoBeneficiarios&acao=A&sbaid=" + sbaid + "&ano=" + ano;
                janela(url, 700, 550, "Beneficiários da Subação")
            }
            function excluirEscola(obj, sesid)
            {

                if (confirm("Deseja realmente excluir esta escola permanentemente?")) {
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=excluirEscola&sesid=" + sesid,
                        success: function(msg) {
                            if (!msg) {
                                $(obj.parentNode.parentNode.parentNode).remove();
                            } else {
                                alert("Não foi possível excluir a escola!");
                            }
                        }
                    });
                }
            }

            function deletarItem(obj, icoid, ano) {
                itens = $('[name=itens[' + ano + ']]').val();

                if (confirm("Deseja realmente excluir este item permanentemente?")) {
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=excluirItem&icoid=" + icoid,
                        success: function(msg) {
                            if (!msg) {
                                alert('Registro excluído com sucesso!');
                                itens = new Number(itens - 1);
                                $('[name=itens[' + ano + ']]').val(itens);
                                $(obj.parentNode.parentNode.parentNode).remove();
                            } else {
                                alert("Não foi possível excluir o item!");
                            }
                        }
                    });
                }
            }

            function retiraQuantidadeEscola(sesid, sbaid, sbdano) {
               
                if (confirm("Tem certeza que deseja excluir a escola da subação?")) {
                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: "requisicaoAjax=retiraQuantidadeEscola&sesid=" + sesid+"&sbaid="+sbaid+"&sbdano="+sbdano,
                        success: function(msg) {
                            if (msg == 1) {
                                alert('Registro excluído com sucesso!');
                                window.location.href = window.location.href
                            } else {
                                alert("Não foi possível excluir a escola!");
                            }
                        }
                    });
                }
            }
            
            function deletarBeneficiario(obj, sabid, ano)
            {
                beneficiarios = $('[name=beneficiarios[' + ano + ']]').val();

                if (confirm("Deseja realmente excluir este beneficiário permanentemente?")) {
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=excluirBeneficiario&sabid=" + sabid,
                        success: function(msg) {
                            if (!msg) {
                                alert('Registro excluído com sucesso!');
                                beneficiarios = new Number(beneficiarios - 1);
                                $('[name=beneficiarios[' + ano + ']]').val(beneficiarios);
                                $(obj.parentNode.parentNode.parentNode).remove();
                            } else {
                                alert("Não foi possível excluir o beneficiário!");
                            }
                        }
                    });
                }
            }

            function consultarObra(preid, sbaid, ano)
            {
                url = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=" + sbaid + "&ano=" + ano + "&preid=" + preid;
                janela(url, 800, 600, "Pré-Obra da Subação")
            }

            function excluirObra(obj, preid)
            {

                if (confirm("Deseja realmente excluir esta obra permanentemente?")) {
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=excluirObra&preid=" + preid,
                        success: function(msg) {
                            if (!msg) {
                                $(obj.parentNode.parentNode.parentNode).remove();
                            } else {
                                alert("Não foi possível excluir a obra!");
                            }
                        }
                    });
                }
            }

            function preencheParecer(ano, sbaid) {
                $.ajax({
                    type: "POST",
                    url: window.location,
                    data: "requisicaoAjax=preencherParecer&ano=" + ano + "&sbaid=" + sbaid,
                    success: function(msg) {
                        if (msg == 1) {
                            alert('A subação não tem parecer padrão!');
                            document.getElementById('pp[' + ano + ']').checked = false;
                        } else {
                            document.getElementById('sbdparecer[' + ano + ']').value = msg;
                        }
                    }
                });
            }
            
            function carregarItem(obj, sesid) {
                if (obj.attr('src') == '../imagens/mais.gif') {
                    obj.attr('src','../imagens/menos.gif');
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "requisicaoAjax=carregarItemPorSesid&sesid=" + sesid,
                        success: function(msg) {
                            $('.listaItem_'+sesid).show();
                            $('.listaItemTd_'+sesid).html(msg);
                        }
                    });
                } else {
                    obj.attr('src', '../imagens/mais.gif');
                    $('.listaItem_'+sesid).hide();
                    $('.listaItemTd_'+sesid).html('');
                }
            }

            function listarSubacao(sbaid) {
                var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&historico=true ";
                janela(local, 800, 600, "Subação2");
            }
            
            function montaListaEscolaSubacaoControle(inuid, sbaid, ano) {
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "requisicaoAjax=montaListaEscolaSubacaoControle&inuid=" + inuid+"&sbaid="+sbaid+"&ano="+ano,
                    success: function(msg) {
                        console.log(msg);
                        $('#tdescolas_'+ano).empty().html(msg);
                    }
                });
            }

            function desabilitarCampos (ano, booReprogramacao) {
                if (booReprogramacao == 'f') {
                    jQuery('#abaAnos'+ano+' input, #abaAnos'+ano+' select, #abaAnos'+ano+' textarea').attr('disabled', 'disabled');
                } else {
                    jQuery('#abaAnos'+ano+' input, #abaAnos'+ano+' select, #abaAnos'+ano+' textarea').removeAttr('disabled');
                }
            }
        --></script>
    </body>
</html>
<?php
montaLinkRegra();
?>