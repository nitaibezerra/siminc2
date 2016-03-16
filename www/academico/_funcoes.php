<?php

/**
 * Envia email quando o estado do documento vai para CONJUR.
 *
 * @name enviarEmailEstadoDocumentoConjur .
 * @return boolean
 *
 * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
 * @since 18/09/2014.
 *
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

function enviarEmailEstadoDocumento($docid)
{
    global $db;

    $sbsid = $_SESSION['academico']['sbsid'];
    $entid = $_SESSION['academico']['entid'];
    $estado = wf_pegarEstadoAnterior($docid);
    $acao = wf_pegarAcaoPorId($estado['aedid']);

    $array_ac_conjur = array(
        WF_CADASTRAMENTO_ENVIAR_ANALISE_CONJUR,
        WF_AGUARD_AUT_DIRIGEN_ENVIAR_ANALISE_CONJUR,
        WF_AGUARD_AUT_SECRETA_ENVIAR_ANALISE_CONJUR,
        WF_EM_ANALISE_SESU_ANALISE_CONJUR,
        WF_EM_ANALISE_SETEC_ANALISE_CONJUR,
        WF_AJUSTE_DEMANDANTE_ANALISE_CONJUR
    );

    $array_ac_sesu = array(
        WF_CADASTRAMENTO_ENVIAR_ANALISE_SESU,
        WF_AGUARD_AUT_DIRIGEN_ENVIAR_ANALISE_SESU
    );

    $array_ac_setec = array(
        WF_CADASTRAMENTO_ENVIAR_ANALISE_SETEC,
        WF_AGUARD_AUT_DIRIGEN_ENVIAR_ANALISE_SETEC
    );

    $array_ac_secretario = array(
        WF_AGUARD_AUT_DIRIGEN_ENVIAR_AUTORIZACAO_SECRETARIO,
        WF_EM_ANALISE_CONJUR_ENVIAR_AUTORIZACAO_SECRETARIO_EXEC
    );

    $array_ac_ministro = array(
        WF_AGUARD_AUT_DIRIGEN_ENVIAR_AUTORIZACAO_MINISTERIAL,
        WF_AUTORIZ_MINISTRO_RETORNA_AUTORIZACAO_MINISTERIAL,
        WF_EM_ANALISE_CONJUR_ENVIAR_AUTORIZACAO_MINISTERIAL
    );

    if (in_array($acao['aedid'], $array_ac_conjur)) {
        $perfil = PERFIL_RF_CONSUTORIA_JURIDICA;
    } else
        if (in_array($acao['aedid'], $array_ac_sesu)) {
            $perfil = PERFIL_RF_SESU_DEC_GOV;
        } else
            if (in_array($acao['aedid'], $array_ac_setec)) {
                $perfil = PERFIL_RF_SETEC_DEC_GOV;
            } else
                if (in_array($acao['aedid'], $array_ac_secretario)) {
                    $perfil = PERFIL_RF_SECRETARIO_EXECUTIVO_DEC_GOV;
                } else
                    if (in_array($acao['aedid'], $array_ac_ministro)) {
                        $perfil = PERFIL_RF_SECRETARIO_EXECUTIVO_DEC_GOV . "," . PERFIL_RF_MINISTRO_DEC_GOV;
                    }

    if ($sbsid) {
        $sql = "
            SELECT  tpc.tpcdsc,
                    epc.epcdsc,
                    mdl.mdldsc,
                    e.entnome,
                    sbs.sbsnumprocesso,
                    sbs.sbsvalor,
                    to_char(sbs.sbsdtiniciovigencia, 'DD/MM/YYYY') as sbsdtiniciovigencia,
                    to_char(sbs.sbsdtfimvigencia, 'DD/MM/YYYY') as sbsdtfimvigencia

            FROM academico.solicitacaobensservicos AS sbs

            JOIN entidade.entidade AS e on e.entid = sbs.entid
            LEFT JOIN academico.modalidadelicitacao AS mdl ON sbs.mdlid = mdl.mdlid
            LEFT JOIN academico.tipocontratomodalidade AS tcm ON mdl.mdlid = tcm.mdlid
            LEFT JOIN academico.tipocontrato AS tpc ON sbs.tpcid = tpc.tpcid
            LEFT JOIN academico.especiecontratacao AS epc ON sbs.epcid = epc.epcid

            WHERE sbs.entid = {$_SESSION['academico']['entid']} AND sbs.sbsstatus = 'A' AND sbs.sbsid = {$sbsid}

            ORDER BY sbs.sbsid
        ";
        $cabecalho = array("&nbsp", "", "", "", "", "", "", "");

        $resultado = $db->pegaLinha($sql);

        if ($perfil != '') {
            $sqlEmail = "
                SELECT usuemail FROM seguranca.usuario AS u
                JOIN seguranca.perfilusuario AS pu ON pu.usucpf = u.usucpf
                WHERE pu.pflcod IN ( {$perfil} )
            ";
            $arrEmail = $db->carregarColuna($sqlEmail);
        } else {
            $arrEmail = array();
        }

        $assunto = 'SOLICITAÇÃO DE DECRETO PARA ANÁLISE (' . date('d/m/Y') . ')';

        $conteudo = "";
        $conteudo .= "ENCAMINHADO SOLICITAÇÃO DE DECRETO PARA ANÁLISE";
        $conteudo .= "<br><br>";
        $conteudo .= "<b> Dados: </b>";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b> Instituição: </b> {$resultado['entnome']}";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b> Tipo de Contrato: </b> {$resultado['tpcdsc']}";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b> Espécie de Contratação: </b> {$resultado['epcdsc']}";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b> Modalidade da Licitação: </b> {$resultado['mdldsc']}";
        $conteudo .= '<br>';
        $conteudo .= "&nbsp;&nbsp; <b> Nº do Processo: </b> {$resultado['sbsnumprocesso']}";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b> Valor R$: </b> " . number_format($resultado['sbsvalor'], 2, ',', '.');
        $conteudo .= '<br>';
        $conteudo .= "&nbsp;&nbsp; <b> Início da Vigência: </b> {$resultado['sbsdtiniciovigencia']}";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b>Fim da Vigência:</b> {$resultado['sbsdtfimvigencia']}";
        $conteudo .= "<br>";
        $conteudo .= "&nbsp;&nbsp; <b>Tramitado por:</b> {$_SESSION['usunome']} em " . date('d/m/Y h:m:s');

        require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');

        #A REDUNDANCIA DO "TRUE" É NECESSARIO E JUTIFICADO, PARA QUE ACONTEÇA A TRAMITAÇÃO. CASO HAJA E-MAIL SERÁ TRAMITADO E OS RESPECTIVOS E-MAIL ENVIDOS, CASO CONTRARIO NÃO.
        if (array_key_exists(0, $arrEmail)) {
            $e = new EmailAgendado();
            $e->setTitle($assunto);
            $e->setText($conteudo);
            $e->setName("SIMEC");
            $e->setEmailOrigem("simec@mec.gov.br");
            $e->setEmailsDestino($arrEmail);
            $e->enviarEmails();

            return true;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

/**
 * Verifica e retorna o tipo de pesquisa.
 *
 * @name verificaTipoPesquisa
 * @param array $filtros - Array com os filtros do where.
 * @return string $tipo (A = Antigo , N = Novo e NULL = Novo)
 *
 * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
 * @since 29/08/2014
 */
function verificaTipoPesquisa($filtros = array())
{
    global $db;

    // Filtros academico.movprofequivalente
    if ($filtros['ano']) $arWhere[] = "ppe.ppeano = {$filtros['ano']}";
    if ($filtros['portaria']) $arWhere[] = "ppe.ppeid = {$filtros['portaria']}";
    if ($filtros['mes']) $arWhere[] = "mpe.mpemes = {$filtros['mes']}";

    $sql = "SELECT mpeprocesso FROM academico.portariaprofequival ppe
               LEFT JOIN academico.movprofequivalente mpe ON mpe.ppeid = ppe.ppeid and mpe.ppeid = ppe.ppeid AND ppe.ppeano = mpe.mpeano
               WHERE mpestatus = 'A'
               " . (is_array($arWhere) ? ' and ' . implode(' and ', $arWhere) : '') . "
               LIMIT 1;";
    $tipo = $db->pegaUm($sql);
    return $tipo;
}

function montaSqlProfEquivalenteNovo($filtros = array())
{
// Filtros academico.portariaprofequival
    if ($_GET['ano']) $arWherePpe[] = "ppe.ppeano = {$_GET['ano']}";
//if($_GET['semestre']) $arWherePpe[] = "ppe.ppesemestre = {$_GET['semestre']}";
    if ($_GET['portaria']) $arWherePpe[] = "ppe.ppeid = {$_GET['portaria']}";

// Filtros academico.movprofequivalente
    if ($_GET['ano']) $arWhereMpe[] = "mpe.mpeano = {$_GET['ano']}";
    if ($_GET['mes']) $arWhereMpe[] = "mpe.mpemes = {$_GET['mes']}";
    if ($_GET['portaria']) $arWhereMpe[] = "mpe.ppeid = {$_GET['portaria']}";

// Filtros academico.portariavalor
    if ($_GET['portaria']) $arWherePtv[] = "ptv.ppeid = {$_GET['portaria']}";

// Calculo saldo
    $stCalculo = "coalesce((coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ))";

// Banco Eqv
    $stBancoEqv = "coalesce( ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ),0)";

    $mpevlr20h_inici = "'<input type=\"text\" name=\"mpevlr20h[' || e.entid || ']\" id=\"mpevlr20h_' || e.entid || '\" value=\"' || ";
    $mpevlr20h_final = "|| '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>'";

    $sql = "
               SELECT  UPPER(entsig) as codigo,
                       UPPER(entnome) as orgao,
                       upper(mun.estuf) as uf,
                       ppe.ppenumero,
                       ppeano,
                       (select mesdsc from public.meses where mescod::integer='" . $_GET['mes'] . "') as mes,

                       {$mpevlr20h_inici}
                       CASE WHEN mpevlr20h > 0
                           THEN trim( to_char( coalesce(mpevlr20h,0 ), '999G999G999G999D99' ) )
                           ELSE '0,00'
                       END
                       {$mpevlr20h_final} AS mpevlr20h,

                       '<input type=\"text\" name=\"mpevlr40h[' || e.entid || ']\" id=\"mpevlr40h_' || e.entid || '\" value=\"' ||
                       case when mpevlr40h > 0
                           then trim(to_char(coalesce(mpevlr40h,0), '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlr40h,

                       '<input type=\"text\" name=\"mpevlrdedexclusiva[' || e.entid || ']\" id=\"mpevlrdedexclusiva_' || e.entid || '\" value=\"' ||
                       case when mpevlrdedexclusiva > 0
                           then trim(to_char(coalesce(mpevlrdedexclusiva,0), '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrdedexclusiva,



                       '<input type=\"text\" name=\"mpevlrsub20h[' || e.entid || ']\" id=\"mpevlrsub20h_' || e.entid || '\" value=\"' ||
                       case when coalesce(mpevlrsub20h,0) > 0
                           then trim(to_char(coalesce(mpevlrsub20h,0), '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrsub20h,

                       '<input type=\"text\" name=\"mpevlrsub40h[' || e.entid || ']\" id=\"mpevlrsub40h_' || e.entid || '\" value=\"' ||
                       case when coalesce(mpevlrsub40h,0) > 0
                           then trim(to_char(coalesce(mpevlrsub40h,0), '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrsub40h,




                       '<input type=\"text\" name=\"mpevlrvisitante[' || e.entid || ']\" id=\"mpevlrvisitante_' || e.entid || '\" value=\"' ||
                       case when coalesce(mpevlrvisitante,0) > 0
                           then trim(to_char(coalesce(mpevlrvisitante,0), '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrvisitante,

                       '<input type=\"text\" name=\"mpevlrvagos[' || e.entid || ']\" id=\"mpevlrvagos_' || e.entid || '\" value=\"' ||
                       case when coalesce(mpevlrvagos,0) > 0
                           then trim(to_char(coalesce(mpevlrvagos,0), '999999'))
                           else '0'
                       end
                       || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'######\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrvagos,


                       '<span id=\"total_' || e.entid || '\">' ||
                       case when coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsub20h + mpevlrsub40h+mpevlrvisitante),0) > 0
                           then trim(to_char(coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsub20h+mpevlrsub40h+mpevlrvisitante),0), '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '</span>' as total,

                       '<span id=\"mpevlrbcequiv_' || e.entid || '\">' ||
                       case when {$stBancoEqv} > 0
                           then trim(to_char({$stBancoEqv}, '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '</span>

                       <input type=\"hidden\" name=\"mpevlrbcequiv[' || e.entid || ']\" value=\"' ||
                       case when {$stBancoEqv} > 0
                           then trim(to_char({$stBancoEqv}, '999G999G999G999D99'))
                           else '0,00'
                       end
                       || '\" />

                       ' as mpevlrbcequiv,

                       '<span id=\"ptvvalor_' || e.entid || '\">' ||
                       case when ptvvalor > 0
                           then trim(to_char(coalesce(ptvvalor,0), '999G999G999G999D99'))
                           else '0,00'
                       end ||
                       '</span>' as ptvvalor,

                       '<span id=\"saldo_' || e.entid || '\">' ||
                       case when {$stCalculo} = 0
                           then '0,00'
                           else trim(to_char({$stCalculo}, '999G999G999G999D99'))
                       end ||

                       '</span> <input type=\"hidden\" name=\"entid[]\" value=\"' || e.entid || '\" /> <input type=\"hidden\" name=\"mpevlrsaldo[' || e.entid || ']\" value=\"' ||
                       case when {$stCalculo} = 0
                           then '0,00'
                           else trim(to_char({$stCalculo}, '999G999G999G999D99'))
                       end
                       || '\" />' as portmpmec

               FROM entidade.entidade e

               INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
               LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
               LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
               LEFT JOIN academico.portariavalor ptv ON ptv.entid = e.entid

               " . (is_array($arWherePtv) ? ' and ' . implode(' and ', $arWherePtv) : '') . "

               LEFT JOIN academico.portariaprofequival ppe ON 1=1 " . (is_array($arWherePpe) ? ' and ' . implode(' and ', $arWherePpe) : '') . "

               LEFT JOIN academico.movprofequivalente mpe ON mpe.ppeid = ppe.ppeid AND mpestatus = 'A' AND mpe.entid = e.entid " . (is_array($arWhereMpe) ? ' and ' . implode(' and ', $arWhereMpe) : '') . "

               WHERE e.entstatus = 'A' AND ef.funid  in ('12') " . (is_array($arWhere) ? ' and ' . implode(' and ', $arWhere) : '') . "

               ORDER BY e.entsig, e.entnome
           ";
    return $sql;
}

function tramitaUnidade($entid)
{

    global $db;

    $sql = "SELECT
				esdid
			FROM
				workflow.documento doc
			INNER JOIN academico.entidade_consolicadao_ifes eci ON eci.docid = doc.docid
			WHERE
				entid = " . $_SESSION['academico']['entid'];
    $esdid = $db->pegaUm($sql);
    $esdidcampus = $esdid == WF_CONSOLIDACAO_IFES_EM_PREENCHIMENTO ? WF_CONSOLIDACAO_IFES_EM_ANALISE_MEC . "," . WF_CONSOLIDACAO_IFES_APROVADO_MEC : WF_CONSOLIDACAO_IFES_APROVADO_MEC;
    $sql = "SELECT
				true
			FROM
				entidade.entidade e2
			INNER JOIN entidade.entidade 					e 	ON e2.entid = e.entid
			INNER JOIN entidade.funcaoentidade 				ef  ON ef.entid = e.entid
			INNER JOIN entidade.funentassoc 				ea  ON ea.fueid = ef.fueid
			INNER JOIN academico.entidade_consolicadao_ifes eci ON eci.entid = e.entid
			INNER JOIN workflow.documento 					doc ON doc.docid = eci.docid
			WHERE
				ea.entid = " . $_SESSION['academico']['entid'] . " AND e.entstatus = 'A' AND ef.funid = " . ACA_ID_CAMPUS . " AND doc.esdid IN ($esdidcampus)
			ORDER BY
				e.entnome";
    $pode = $db->pegaUm($sql);
    $pode = $pode == 't' ? false : true;
    if ($pode) {
        $sql = "SELECT
					docid
				FROM
					academico.entidade_consolicadao_ifes
				WHERE
					entid = " . $_SESSION['academico']['entid'];
        $docid = $db->pegaUm($sql);
        require_once APPRAIZ . 'includes/workflow.php';
        $esdidcampus = $esdid == WF_CONSOLIDACAO_IFES_EM_PREENCHIMENTO ? WF_AEDID_CONSOLIDACAO_IFES_ENVIAR_ANALISE : WF_AEDID_CONSOLIDACAO_IFES_APROVAR;
        wf_alterarEstado($docid,
            $dados['aedid'],
            'tramitado.',
            array('entid' => $_SESSION['academico']['entid'], 'entidcampus' => $_SESSION['academico']['entidcampus']));
        return true;
    }
    return true;
}

function verificaEstadoPreenchimentoEntidade($entid)
{

    global $db;

    $sql = "SELECT
				true
			FROM
				academico.entidade_consolicadao_ifes eci
			INNER JOIN workflow.documento doc ON doc.docid = eci.docid
			WHERE
				entid = $entid
				AND esdid = " . WF_CONSOLIDACAO_IFES_EM_PREENCHIMENTO;
    $retorno = $db->pegaUm($sql);
    $retorno = $retorno == 't' ? true : false;
    return $retorno;
}

function verificaPreenchimentoQuestionario($entidcampus)
{

    global $db;

    $pendencias = false;
    //Verifica Unidade
    $sql = "SELECT
				res.resid,
				resdescricao
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = " . $_SESSION['academico']['entid'] . "
			WHERE
				per.perid = 1
				AND ( resdescricao IS NULL OR resjustificativa = '' )
			ORDER BY
				1";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    //Verifica Página 1, 2 e 3
    $sql = "SELECT
				res.resid,
				resdescricao
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = $entidcampus
			WHERE
				(pernumero ILIKE '2.'
				OR pernumero ILIKE '2.1.%'
				OR pernumero ILIKE '2.2.%'
				OR pernumero ILIKE '2.3.%'

				OR pernumero ilike '2.4.%'
				OR pernumero ilike '2.5.%'
				OR pernumero ilike '2.6.%'

				OR pernumero ilike '2.7.%')
				AND pernumero NOT IN ('2.5.1.1.','2.5.1.2.')
				AND per.tppid = 2
				AND ( resdescricao IS NULL AND resjustificativa = '' )
			ORDER BY
				1";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    $sql = "SELECT DISTINCT
				resdescricao
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = $entidcampus
			WHERE
				pernumero = '2.5.1.'
				AND per.tppid = 2
			ORDER BY
				1";
    $test = $db->pegaUm($sql);
    if ($test == 'T') {
        $sql = "SELECT
					res.resid,
					resdescricao
				FROM
					academico.pergunta per
				LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = $entidcampus
				WHERE
					pernumero NOT IN ('2.5.1.1.','2.5.1.2.')
					AND per.tppid = 2
					AND ( resdescricao IS NULL AND resjustificativa = '' )
				ORDER BY
					1";
        $tipos = $db->carregar($sql);
        $tipos = $tipos ? $tipos : Array();
        $pendencias = count($tipos) > 0 ? true : $pendencias;
    }
    //Verifica Página 4
    $sql = "(SELECT
				per.perid,
				pernumero||' - '||pertitulo as pergunta
			FROM
				academico.pergunta per
			LEFT JOIN academico.anexosquestionario anx ON anx.perid = per.perid AND entid = $entidcampus
			WHERE
				(pernumero ilike '3.1.%')
				AND anx.arqid IS NULL
			ORDER BY 1)
			UNION ALL
			(SELECT
				per.perid,
				'3.2. - '||pertitulo as pergunta
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = $entidcampus
			WHERE
				per.perid = 33
				AND resjustificativa = ''
			ORDER BY 1)";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    //Verifica Página 5, 6, 7 e 8
    $sql = "SELECT
				per.perid,
				pernumero||' - '||pertitulo as pergunta,
				tbv2013.tbvcusteio,
				tbv2013.tbvcapital,
				tbv2014.tbvcusteio,
				tbv2014.tbvcapital,
				tbv2015.tbvcusteio,
				tbv2015.tbvcapital,
				CASE
					WHEN per.perid = 44 THEN 5
					WHEN per.perid = 45 THEN 6
					WHEN per.perid = 46 THEN 7
					ELSE 8
				END as pag
			FROM
				academico.pergunta per
			INNER JOIN academico.tabelaresposta tbr ON tbr.perid = per.perid AND tbr.entid = $entidcampus
			LEFT  JOIN academico.tabelavalor    tbv2013 ON tbv2013.tbrid = tbr.tbrid AND tbv2013.tbvano = 2013
			LEFT  JOIN academico.tabelavalor    tbv2014 ON tbv2014.tbrid = tbr.tbrid AND tbv2014.tbvano = 2014
			LEFT  JOIN academico.tabelavalor    tbv2015 ON tbv2015.tbrid = tbr.tbrid AND tbv2015.tbvano = 2015
			WHERE
				pernumero ilike '4.%'
				AND pernumero != '4.'
				AND ( tbrdemanda IS NULL
					OR tbrarea IS NULL
					OR tbraluno IS NULL
					OR tbrprojeto IS NULL
					OR tbv2013.tbvcusteio IS NULL
					OR tbv2013.tbvcapital IS NULL
					OR tbv2014.tbvcusteio IS NULL
					OR tbv2014.tbvcapital IS NULL
					OR tbv2015.tbvcusteio IS NULL
					OR tbv2015.tbvcapital IS NULL )";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    //Verifica Página 9, 10 e 11
    $sql = "SELECT
				per.perid,
				'5. Demanda Arquivos - '||pertitulo as pergunta,
				CASE
					WHEN per.perid = 44 THEN 5
					WHEN per.perid = 45 THEN 6
					WHEN per.perid = 46 THEN 7
					ELSE 8
				END as pag
			FROM
				academico.pergunta per
			LEFT JOIN academico.tabelaresposta tbr ON tbr.perid = per.perid AND tbr.entid = $entidcampus
			WHERE
				pernumero ilike '4.%'
				AND pernumero != '4.'
				AND pernumero != '4.4.'
				AND ( tbrprojeto IS TRUE
					AND tbr.arqid IS NULL )";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    return !$pendencias;
}

function verificaPreenchimentoQuestionario2($entidcampus)
{

    global $db;

    ?>
    <table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' style="width:100%;">
    <tr>
        <td>
            <b>Unidade</b>
        </td>
    </tr>
    <?php
    //Verifica Unidade
    $sql = "SELECT
				per.perid,
				pernumero||' - '||pertitulo as pergunta
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = " . $_SESSION['academico']['entid'] . "
			WHERE
				per.perid = 1
				AND ( resdescricao IS NULL OR resjustificativa = '' )
			ORDER BY
				1";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    $cond = true;
    foreach ($tipos as $tipo) {
        ?>
        <tr>
            <td bgcolor="white">
                <img border="0" title="Ir para questionário." class="irUni" style="cursor:pointer" width="40px"
                     id="<?= $tipo['pag'] ?>" src="../imagens/gadget_busca.png">&nbsp;
                <b><?= $tipo['pergunta'] ?></b>
            </td>
        </tr>
    <?php
    }
    if (!$pendencias) {
        ?>
        <tr>
            <td bgcolor="white">
                Não posui pendências.
            </td>
        </tr>
    <?php
    }
    ?>
    <tr>
        <td>
            <b>Campus</b>
        </td>
    </tr>
    <?php
    //Verifica Página 1, 2 e 3
    $sql = "SELECT
				per.perid,
				pernumero||' - '||pertitulo as pergunta,
				pernumero,
				resdescricao,
				CASE
					WHEN per.perid < 12 THEN 1
					WHEN per.perid < 20 THEN 2
					ELSE 3
				END as pag
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = " . $_SESSION['academico']['entidcampus'] . "
			WHERE
				(pernumero ILIKE '2.'
				OR pernumero ILIKE '2.1.%'
				OR pernumero ILIKE '2.2.%'
				OR pernumero ILIKE '2.3.%'

				OR pernumero ilike '2.4.%'
				OR pernumero ilike '2.5.%'
				OR pernumero ilike '2.6.%'

				OR pernumero ilike '2.7.%')
				AND per.tppid = 2
				AND ( resdescricao IS NULL AND resjustificativa = '' )
			ORDER BY
				2";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    $cond = true;
    foreach ($tipos as $tipo) {
        if ($tipo['pernumero'] == '2.5.1.1.' || $tipo['pernumero'] == '2.5.1.2.') {
            $sql = "SELECT DISTINCT
						resdescricao
					FROM
						academico.pergunta per
					LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = " . $_SESSION['academico']['entidcampus'] . "
					WHERE
						pernumero = '2.5.1.'
						AND per.tppid = 2
					ORDER BY
						1";
            $test = $db->pegaUm($sql);
            if ($test == 'FALSE') {
                $cond = false;
                $pendencias = !$pendencias ? false : true;
            }
        }
        if ($cond) {
            ?>
            <tr>
                <td bgcolor="white">
                    <img border="0" title="Ir para questionário." class="ir" style="cursor:pointer" width="40px"
                         id="<?= $tipo['pag'] ?>" src="../imagens/gadget_busca.png">&nbsp;
                    <b><?= $tipo['pergunta'] ?></b>
                </td>
            </tr>
        <?php
        }
        if ($tipo['pernumero'] == '2.5.1.2.') {
            $cond = true;
        }
    }
    //Verifica Página 4
    $sql = "(SELECT
				per.perid,
				pernumero||' - '||pertitulo as pergunta,
				4 as pag
			FROM
				academico.pergunta per
			LEFT JOIN academico.anexosquestionario anx ON anx.perid = per.perid AND entid = " . $_SESSION['academico']['entidcampus'] . "
			WHERE
				(pernumero ilike '3.1.%')
				AND anx.arqid IS NULL
			ORDER BY 1)
			UNION ALL
			(SELECT
				per.perid,
				'3.2. - '||pertitulo as pergunta,
				4 as pag
			FROM
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = " . $_SESSION['academico']['entidcampus'] . "
			WHERE
				per.perid = 33
				AND resjustificativa = ''
			ORDER BY 1)";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    $cond = true;
    foreach ($tipos as $tipo) {
        ?>
        <tr>
            <td bgcolor="white">
                <img border="0" title="Ir para questionário." class="ir" style="cursor:pointer" width="40px"
                     id="<?= $tipo['pag'] ?>" src="../imagens/gadget_busca.png">&nbsp;
                <b><?= $tipo['pergunta'] ?></b>
            </td>
        </tr>
    <?php
    }
    //Verifica Página 5, 6, 7 e 8
    $sql = "SELECT
				per.perid,
				pernumero||' - '||pertitulo as pergunta,
				tbv2013.tbvcusteio,
				tbv2013.tbvcapital,
				tbv2014.tbvcusteio,
				tbv2014.tbvcapital,
				tbv2015.tbvcusteio,
				tbv2015.tbvcapital,
				CASE
					WHEN per.perid = 44 THEN 5
					WHEN per.perid = 45 THEN 6
					WHEN per.perid = 46 THEN 7
					ELSE 8
				END as pag
			FROM
				academico.pergunta per
			INNER JOIN academico.tabelaresposta tbr ON tbr.perid = per.perid AND tbr.entid = " . $_SESSION['academico']['entidcampus'] . "
			LEFT  JOIN academico.tabelavalor    tbv2013 ON tbv2013.tbrid = tbr.tbrid AND tbv2013.tbvano = 2013
			LEFT  JOIN academico.tabelavalor    tbv2014 ON tbv2014.tbrid = tbr.tbrid AND tbv2014.tbvano = 2014
			LEFT  JOIN academico.tabelavalor    tbv2015 ON tbv2015.tbrid = tbr.tbrid AND tbv2015.tbvano = 2015
			WHERE
				pernumero ilike '4.%'
				AND pernumero != '4.'
				AND ( tbrdemanda IS NULL
					OR tbrarea IS NULL
					OR tbraluno IS NULL
					OR tbrprojeto IS NULL
					OR tbv2013.tbvcusteio IS NULL
					OR tbv2013.tbvcapital IS NULL
					OR tbv2014.tbvcusteio IS NULL
					OR tbv2014.tbvcapital IS NULL
					OR tbv2015.tbvcusteio IS NULL
					OR tbv2015.tbvcapital IS NULL )";
    //	ver($sql);
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    foreach ($tipos as $tipo) {
        ?>
        <tr>
            <td bgcolor="white">
                <img border="0" title="Ir para questionário." class="ir" style="cursor:pointer" width="40px"
                     id="<?= $tipo['pag'] ?>" src="../imagens/gadget_busca.png">&nbsp;
                <b><?= $tipo['pergunta'] ?></b>
            </td>
        </tr>
    <?php
    }
    //Verifica Página 9, 10 e 11
    $sql = "SELECT
				per.perid,
				'5. Demanda Arquivos - '||pertitulo as pergunta,
				CASE
					WHEN per.perid = 44 THEN 5
					WHEN per.perid = 45 THEN 6
					WHEN per.perid = 46 THEN 7
					ELSE 8
				END as pag
			FROM
				academico.pergunta per
			LEFT JOIN academico.tabelaresposta tbr ON tbr.perid = per.perid AND tbr.entid = " . $_SESSION['academico']['entidcampus'] . "
			WHERE
				pernumero ilike '4.%'
				AND pernumero != '4.'
				AND pernumero != '4.4.'
				AND ( tbrprojeto IS TRUE
					AND tbr.arqid IS NULL )";
    $tipos = $db->carregar($sql);
    $tipos = $tipos ? $tipos : Array();
    $pendencias = count($tipos) > 0 ? true : $pendencias;
    foreach ($tipos as $tipo) {
        ?>
        <tr>
            <td bgcolor="white">
                <img border="0" title="Ir para questionário." class="ir" style="cursor:pointer" width="40px"
                     id="<?= $tipo['pag'] ?>" src="../imagens/gadget_busca.png">&nbsp;
                <b><?= $tipo['pergunta'] ?></b>
            </td>
        </tr>
    <?php
    }
    if (!$pendencias) {
        ?>
        <tr>
            <td bgcolor="white">
                Não posui pendências.
            </td>
        </tr>
    <?php
    }
    ?>
    </table>
<?php
}

function criarDocidEntidadeSoncolidacao($entid)
{

    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo Consolidacao e Expansão das IFES - entid " . $entid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_CONSOLIDACAO_IFES, $docdsc);

    // atualiza pap do EMI
    $sql = "INSERT INTO academico.entidade_consolicadao_ifes(entid, docid)
			VALUES($entid, $docid)";

    $db->executar($sql);
    $db->commit();

    return $docid;
}

function pegaDocidConsolidacao()
{

    global $db;

    $sql = "SELECT DISTINCT
				docid
			FROM
				academico.entidade_consolicadao_ifes
			WHERE
				entid = " . $_SESSION['academico']['entid'];
    $docid_unidade = $db->pegaUm($sql);
    if ($_SESSION['academico']['entidcampus']) {
        $sql = "SELECT DISTINCT
					docid
				FROM
					academico.entidade_consolicadao_ifes
				WHERE
					entid = " . $_SESSION['academico']['entidcampus'];
        $docid = $db->pegaUm($sql);
        if (!$docid) {
            $docid = criarDocidEntidadeSoncolidacao($_SESSION['academico']['entidcampus']);
        }
    }
    if (!$docid_unidade) {
        criarDocidEntidadeSoncolidacao($_SESSION['academico']['entid']);
    }
    return $docid;
}

//condicaoTipoContrato( tpcid, ministro )
function condicaoTipoContrato($tcpid, $tipo)
{
    global $db;
    if ($tipo == 1) {
        if ($tcpid == 2 || $tcpid == 3) return true;
        else return false;
    } elseif ($tipo == 2) {
        if ($tcpid == 1) return true;
        else return false;
    } else {
        return false;
    }
}

function montarAbasConsolidacao($param)
{

    global $db;

    if ($param['link1']) {
        ?>
        <table cellspacing="0" cellpadding="0" width="95%" border="0" align="center" class="notprint"
               style="width:100%;">
            <tr>
                <td>
                    <?php
                    $lnkabas = $param['link1'];
                    $aba = Array();
                    $aba[] = Array("descricao" => "2. Situação da Infraestrutura:",
                        "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=1");
                    $aba[] = Array("descricao" => "3. Plano de Desenvolvimento da Instituição (PDI):",
                        "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=4");
                    $aba[] = Array("descricao" => "4. Demanda:",
                        "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=5");
                    $aba[] = Array("descricao" => "5. Demanda - Arquivos:",
                        "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=9");
                    $aba[] = Array("descricao" => "Pendências:",
                        "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=12");
                    echo montarAbasArrayAcademico($aba, $lnkabas, $win);
                    ?>
                </td>
            </tr>
        </table>
        <table cellspacing="0" cellpadding="0" bgcolor="#f5f5f5" align="center" class="tabela" style="width:100%;">
            <tbody>
            <tr>
                <td class="SubTituloDireita bold direita">
                    <table cellspacing="1" cellpadding="0" width="240" border="0" align="right">
                        <tbody>
                        <tr>
                            <td height="10px;"></td>
                        </tr>
                        </tbody>
                    </table>
                </td>
            </tr>
            </tbody>
        </table>
        <br>
    <?php
    }
    if ($param['link2']) {
        ?>
        <table cellspacing="0" cellpadding="0" width="95%" border="0" align="center" class="notprint"
               style="width:100%;">
            <tr>
                <td>
                    <?php
                    $lnkabas = $param['link2'];
                    $aba = Array();
                    switch ($param['link2tipo']) {
                        case 2:
                            $aba[] = Array("descricao" => "2.1. Salas de Aula:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=1");
                            $aba[] = Array("descricao" => "2.4. Restaurante Universitário:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=2");
                            $aba[] = Array("descricao" => "2.7. Infraestrutura: os itens a seguir precisam de melhorias/adequações?:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=3");
                            break;
                        case 4:
                            $aba[] = Array("descricao" => "4.1. Consolidação:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=5");
                            $aba[] = Array("descricao" => "4.2. Expansão:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=6");
                            $aba[] = Array("descricao" => "4.3. Expansão Medicina:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=7");
                            $aba[] = Array("descricao" => "4.4. Câmpus Pactuados:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=8");
                            break;
                        case 5:
                            $aba[] = Array("descricao" => "5.1. Consolidação:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=9");
                            $aba[] = Array("descricao" => "5.2. Expansão:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=10");
                            $aba[] = Array("descricao" => "5.3. Expansão Medicina:",
                                "link" => "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=11");
                            break;
                    }
                    echo montarAbasArrayAcademico($aba, $lnkabas, $win);
                    ?>
                </td>
            </tr>
        </table>
        <table cellspacing="0" cellpadding="0" bgcolor="#f5f5f5" align="center" class="tabela" style="width:100%;">
            <tbody>
            <tr>
                <td class="SubTituloDireita bold direita">
                    <table cellspacing="1" cellpadding="0" width="240" border="0" align="right">
                        <tbody>
                        <tr>
                            <td height="10px;"></td>
                        </tr>
                        </tbody>
                    </table>
                </td>
            </tr>
            </tbody>
        </table>
        <br>
    <?php
    }
}

function montarAbasArrayAcademico($itensMenu, $url = false, $boOpenWin = false)
{
    $url = $url ? $url : $_SERVER['REQUEST_URI'];

    if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $menu = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
        . '<tr>'
        . '<td>'
        . '<table cellpadding="0" cellspacing="0" align="left">'
        . '<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);

        if ($url != $link && $j == 0)
            $gifaba = 'aba_nosel_ini.gif';
        elseif ($url == $link && $j == 0)
            $gifaba = 'aba_esq_sel_ini.gif';
        elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
            $gifaba = 'aba_dir_sel.gif';
        elseif ($url != $link)
            $gifaba = 'aba_nosel.gif';
        elseif ($url == $link)
            $gifaba = 'aba_esq_sel.gif';

        if ($url == $link) {
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
        } else {
            $giffundo_aba = 'aba_fundo_nosel.gif';
            $cor_fonteaba = '#4488cc';
        }

        $menu .= '<td height="20" valign="top"><img src="../imagens/' . $gifaba . '" width="11" height="20" alt="" border="0"></td>'
            . '<td height="20" align="center" valign="middle" background="../imagens/' . $giffundo_aba . '" style="color:' . $cor_fonteaba . ';
               		  padding-left: 10px; padding-right: 10px;cursor:pointer;" class="btfAba" id="' . $link . '" >';

        if ($link != $url) {
            $menu .= $descricao;
        } else {
            $menu .= $descricao . '</td>';
        }
    }

    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="../imagens/' . $gifaba . '" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}


function pegaQrpid($entid, $queid)
{
    global $db;

    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    $sql = "SELECT
            	enq.qrpid
            FROM
            	academico.entidadequestionario enq
            INNER JOIN
            	questionario.questionarioresposta qr ON qr.qrpid = enq.qrpid
            WHERE
            	enq.entid = {$entid}
            	AND qr.queid = {$queid}";
    $qrpid = $db->pegaUm($sql);

    if (!$qrpid) {
        $sql = "SELECT DISTINCT
    				'true'
    			FROM
            		questionario.questionario
            	WHERE
            		queid = " . $queid;
        $testaQueid = $db->pegaUm($sql);

        if ($testaQueid) {
            $sql = "SELECT
	                   entnome
	                FROM
	                    entidade.entidade
	                WHERE
	                    entid = {$entid}";
            $titulo = $db->pegaUm($sql);

            $arParam = array("queid" => $queid, "titulo" => "Acadêmico (" . $titulo . ")");
            $qrpid = GerenciaQuestionario::insereQuestionario($arParam);

            $sql = "INSERT INTO academico.entidadequestionario (entid, qrpid) VALUES ({$entid}, {$qrpid})";
            $db->executar($sql);
            $db->commit();
        } else {
            echo "<script>alert('Ação impossivel. Questionario inexistente.')</script>";
            return false;
        }
    }

    return $qrpid;
}

function redir($url = null, $msg = null)
{
    $script .= '<script>';
    if (!empty($msg))
        $script .= '	alert(\'' . $msg . '\');';

    if (!empty($url))
        $script .= '	location.href=\'' . $url . '\';';
    else
        $script .= '	history.go(-1);';

    $script .= '</script>';
    die($script);
}

    function apagaDadosDirigentes($entid, $funid, $mcsid){
        global $db;

        #DELETA O REGISTRO DO DIRIGENTE.
        if ($mcsid != '') {
            $sql = "
                DELETE FROM academico.membroconselho WHERE mcsid = {$mcsid} RETURNING feaid
            ";
            $d_feaid = $db->pegaUm($sql);
        }
        
        #VERIFICA SE O DIRIGENTE DELETADO ESTA PRESENTE MAIS DE UMA VEZ COM A MESMA FUNÇÃO.
        if( $d_feaid > 0 ){
           $sql = "SELECT feaid FROM academico.membroconselho WHERE feaid = {$d_feaid}";
           $r_feaid = $db->pegaUm($sql);
        }

        #CASO O DIRIGENTE SEJA UNICO, ELE PODERA SER DELETADO NA TABELA DE ASSOCIAÇÃO DE FUNÇAO A ENTIDADE. SE NÃO É DELETADO APENAS NA TABELA "academico.membroconselho"
        if( $entid != '' && $funid != '' ){
            if( $r_feaid == '' ){
                $sql = "
                    DELETE FROM entidade.funentassoc
                    WHERE feaid IN (
                        SELECT fea.feaid
                        FROM entidade.funcaoentidade fe
                        INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
                        WHERE fe.entid = {$entid} AND fe.funid = {$funid}
                    ) RETURNING feaid
                ";
                $feaid = $db->pegaUm($sql);
            }
        }
        
        if( $feaid > 0 || $d_feaid > 0 ){
            $db->commit();
            $db->sucesso('principal/dadosdirigentes', '', 'Registro excluido com sucesso!');
        } else {
            $db->rollback();
            $db->insucesso('Operação não Realizada. Por favor tente novamente mais tarde!', '', 'principal/dadosdirigentes&acao=A');
        }
    }

function verificaEditalPortaria($edpid, $tpeid)
{
    global $db;

    if (!is_numeric($edpid) || empty($tpeid)) {
        die("<script>
				alert('Faltam parametros para abrir o edital!');
				location.href='?modulo=inicio&acao=C';
			 </script>");
    }
    $sql = "SELECT
				COUNT(1)
			FROM
				academico.editalportaria
			WHERE
				edpid = {$edpid}
				AND tpeid = {$tpeid}";
    $edpidExist = $db->pegaUm($sql);
    if (empty($edpidExist)) {
        die("<script>
				alert('Faltam parametros para abrir o edital!');
				location.href='?modulo=inicio&acao=C';
			 </script>");
    }
    return true;
}

# Setando as variáveis de sessão
function pegaSessoes($exigeprtid = 0, $exigeedpid = 0, $exigeentidcampus = 0)
{
    global $db;

    if (isset($_REQUEST["prtid"]) and $_REQUEST["prtid"] != 'undefined' and $_REQUEST["prtid"] != '') {
        $_SESSION['academico']['prtid'] = (integer)$_REQUEST["prtid"];

        $sql = "SELECT tprid, prtano, prgid
				FROM academico.portarias prt
				WHERE prt.prtid = " . $_REQUEST["prtid"];

        $dados = $db->pegaLinha($sql);

        $_SESSION['academico']['tprid'] = (integer)$dados["tprid"];
        $_SESSION['academico']['ano'] = (integer)$dados["prtano"];
        $_SESSION['academico']['prgid'] = (integer)$dados["prgid"];
    }

    if (isset($_REQUEST["edpid"]) and $_REQUEST["edpid"] != 'undefined' and $_REQUEST["edpid"] != '') {
        $_SESSION['academico']['edpid'] = (integer)$_REQUEST["edpid"];
        $sql = "SELECT edpidhomo, tpeid, edpideditalhomologacao
				FROM academico.editalportaria
				WHERE edpid = " . $_REQUEST["edpid"];

        $dados = $db->pegaLinha($sql);

        $_SESSION['academico']['edpideditalhomologacao'] = (integer)$dados["edpideditalhomologacao"];
        $_SESSION['academico']['edpidhomo'] = (integer)$dados["edpidhomo"];
        $_SESSION['academico']['tpetipo'] = (integer)$dados["tpeid"];

    }

    if (isset($_REQUEST["entidcampus"]) and $_REQUEST["entidcampus"] != 'undefined' and $_REQUEST["entidcampus"] != '') {
        $_SESSION['academico']['entidcampus'] = (integer)$_REQUEST["entidcampus"];

        $sql = "SELECT ea.entid
				FROM entidade.entidade e
				INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN entidade.funentassoc ea ON ef.fueid = ea.fueid
				WHERE e.entid = " . (integer)$_REQUEST["entidcampus"];

        $entid = $db->pegaUm($sql);
        $_SESSION['academico']['entid'] = $entid;
    }

    if (isset($_REQUEST["tprnivel"]) && $_REQUEST["tprnivel"] != '') {

        $_SESSION['academico']['entidcampus'] = (integer)$_REQUEST["entidcampus"];
        $_SESSION["academico"]["tprnivel"] = $_REQUEST["tprnivel"];
    }

    # Volta para a tela inicial caso a sessão seja perdida
    if ((!isset($_SESSION['academico']['prtid']) || ($_SESSION['academico']['prtid'] == '')) and $exigeprtid) {
        echo "<script>
				alert('Portaria não selecionada, voltando para o inicio.');
				window.location = '?modulo=inicio&acao=C';
			  </script>";
        exit;
    }
    # Volta para a tela inicial caso a sessão seja perdida
    if ((!isset($_SESSION['academico']['edpid']) || ($_SESSION['academico']['edpid'] == '')) and $exigeedpid) {

        if ($_SESSION['academico']['entidcampus'] && $_SESSION['academico']['prtid'] && $_SESSION['academico']['prgid']) {

            echo "<script>
				alert('Edital não selecionado, voltando para a lista de editais.');
				window.location = '?modulo=principal/listareditais&acao=C&evento=A&entidcampus=" . $_SESSION['academico']['entidcampus'] . "&prtid=" . $_SESSION['academico']['prtid'] . "&prgid=" . $_SESSION['academico']['prgid'] . "';
			  </script>";

        } else {

            echo "<script>
				alert('Edital não selecionado, voltando para o inicio.');
				window.location = '?modulo=inicio&acao=C';
			  </script>";
        }
        exit;
    }

    # Volta para a tela inicial caso a sessão seja perdida
    if ((!isset($_SESSION['academico']['entidcampus']) || ($_SESSION['academico']['entidcampus'] == '')) and $exigeentidcampus) {
        echo "<script>
				alert('Unidade não selecionada, voltando para o inicio.');
				window.location = '?modulo=inicio&acao=C';
			  </script>";
        exit;
    }

}

function EnviarArquivo($arquivo, $dados, $dir = 'cadedital')
{
    global $db;
    // obtém o arquivo
    $arquivo = $_FILES['arquivo'];
    if (!is_uploaded_file($arquivo['tmp_name'])) {
        redirecionar($_REQUEST['modulo'], $_REQUEST['acao'], $parametros);
    }
    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if ($arquivo["type"] == 'image/pjpeg') {
        $arquivo["type"] = 'image/jpeg';
    }
    //Insere o registro do arquivo na tabela public.arquivo
    $sql = "INSERT INTO public.arquivo (arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('" . current(explode(".", $arquivo["name"])) . "','" . end(explode(".", $arquivo["name"])) . "','" . $dados["arqdescricao"] . "','" . $arquivo["type"] . "','" . $arquivo["size"] . "','" . date('Y-m-d') . "','" . date('H:i:s') . "','" . $_SESSION["usucpf"] . "'," . $_SESSION["sisid"] . ") RETURNING arqid;";
    $arqid = $db->pegaUm($sql);

    //Insere o registro na tabela academico.anexos
    if (isset($dados["edpid"]) && ($dados["edpid"] != '')) {

        $sql = "INSERT INTO academico.anexos (edpid, prtid, arqid, tpaid, anxdesc, anxdtinclusao)
		values(" . $dados["edpid"] . "," . $dados["prtid"] . "," . $arqid . "," . $dados["tpaid"] . ",'" . $dados["arqdescricao"] . "','" . date("Y-m-d H:i:s") . "');";
    } else {
        $sql = "INSERT INTO academico.anexos (prtid, arqid, tpaid, anxdesc, anxdtinclusao)
		values(" . $dados["prtid"] . "," . $arqid . "," . $dados["tpaid"] . ",'" . $dados["arqdescricao"] . "','" . date("Y-m-d H:i:s") . "');";
    }
    $db->executar($sql);

    if (!is_dir('../../arquivos/academico/')) {
        mkdir(APPRAIZ . '/arquivos/academico/', 0777);
    }
    if (!is_dir('../../arquivos/academico/' . floor($arqid / 1000))) {
        mkdir(APPRAIZ . '/arquivos/academico/' . floor($arqid / 1000), 0777);
    }

    $caminho = APPRAIZ . 'arquivos/' . $_SESSION['sisdiretorio'] . '/' . floor($arqid / 1000) . '/' . $arqid;

    if (!move_uploaded_file($arquivo['tmp_name'], $caminho)) {
        $db->rollback();
        echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
        exit;
    }
    $db->commit();
}

function EnviarArquivoEdital($arquivo, $dados, $dir = 'cadedital')
{
    global $db;
    // obtém o arquivo
    $arquivo = $_FILES['arquivo'];
    if (!is_uploaded_file($arquivo['tmp_name'])) {
        redirecionar($_REQUEST['modulo'], $_REQUEST['acao'], $parametros);
    }
    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if ($arquivo["type"] == 'image/pjpeg') {
        $arquivo["type"] = 'image/jpeg';
    }
    //Insere o registro do arquivo na tabela public.arquivo
    $sql = "INSERT INTO public.arquivo (arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('" . current(explode(".", $arquivo["name"])) . "','" . end(explode(".", $arquivo["name"])) . "','" . $dados["arqdescricao"] . "','" . $arquivo["type"] . "','" . $arquivo["size"] . "','" . date('Y-m-d') . "','" . date('H:i:s') . "','" . $_SESSION["usucpf"] . "'," . $_SESSION["sisid"] . ") RETURNING arqid;";


    $arqid = $db->pegaUm($sql);

    //Insere o registro na tabela academico.anexos
    $sql = "INSERT INTO academico.anexos (prtid, edpid, arqid, tpaid, anxdesc, anxdtinclusao)
	values(" . $dados["prtid"] . "," . $dados["edpid"] . "," . $arqid . "," . $dados["tpaid"] . ",'" . $dados["arqdescricao"] . "','" . date("Y-m-d H:i:s") . "');";


    $db->executar($sql);
    if (!is_dir('../../arquivos/academico/')) {
        mkdir(APPRAIZ . '/arquivos/academico/', 0777);
    }

    if (!is_dir('../../arquivos/academico/' . floor($arqid / 1000))) {
        mkdir(APPRAIZ . '/arquivos/academico/' . floor($arqid / 1000), 0777);
    }
    //.floor($arqid/1000)
    //echo APPRAIZ.'/arquivos/academico'.floor($arqid/1000);
    $caminho = APPRAIZ . 'arquivos/' . $_SESSION['sisdiretorio'] . '/' . floor($arqid / 1000) . '/' . $arqid;
    //$caminho = APPRAIZ.'teste/'.$arqid;

    if (!move_uploaded_file($arquivo['tmp_name'], $caminho)) {
        $db->rollback();
        echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
        exit;
    }
    $db->commit();
}

function DownloadArquivo($param)
{
    global $db;
    ob_end_clean();
    $sql = "SELECT * FROM public.arquivo WHERE arqid = " . $param['arqid'];
    $arquivo = current($db->carregar($sql));
    $caminho = APPRAIZ . 'arquivos/' . $_SESSION['sisdiretorio'] . '/' . floor($arquivo['arqid'] / 1000) . '/' . $arquivo['arqid'];
    // $caminho=" C:/simec-desenvolvimento/simec/teste/".$arquivo['arqid'];
    if (!is_file($caminho)) {
        $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
    }
    $filename = str_replace(" ", "_", $arquivo['arqnome'] . '.' . $arquivo['arqextensao']);
    //echo($caminho);exit;
    header('Content-type: ' . $arquivo['arqtipo']);
    header('Content-Disposition: attachment; filename=' . $filename);
    readfile($caminho);
    exit();
}

function DeletarDocumento($documento)
{
    global $db;
    $sql = "UPDATE academico.anexos SET anxstatus = 'I' where anxid=" . $documento["anxid"];
    $db->executar($sql);

    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=" . $documento["arqid"];
    $db->executar($sql);

    $db->commit();
}

function pegaPublicado($edpid = null)
{
    global $db;

    $sql = "SELECT
				edpidhomo
			FROM
				academico.editalportaria
			WHERE
				edpid = {$edpid}";

    return $db->pegaUm($sql);
}

function pegaHomologado($edpid = null)
{
    global $db;

    $sql = "SELECT
				edpideditalhomologacao
			FROM
				academico.editalportaria
			WHERE
				edpid = {$edpid}";

    return $db->pegaUm($sql);
}

/**
 * Função que monta a aba da distribuição de cargos
 * @param void
 */
function montaTabelaPlanoDistribuicao()
{

    global $db, $tabela, $clsid, $habil, $habilitado, $disabled;

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $edpid = $_SESSION['academico']['edpid'];
    $edpidhomo = $_SESSION['academico']['edpidhomo'];
    $edpideditalhomologacao = $_SESSION['academico']['edpideditalhomologacao'];
    $tpetipo = $_SESSION['academico']['tpetipo'];
    $prtid = $_SESSION['academico']['prtid'];
    $prgid = $_SESSION['academico']['prgid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $orgid = $_SESSION["academico"]["orgid"];
    $ano = $_SESSION['academico']['ano'];

    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);

    //pre-confirgurando a exibição dos campos
    $editavel_homo = "";
    $display = "style=\"display:none\"  disabled=\"disabled\"";
    $editavel_efe = "";
    $display_efe = "style=\"display:none\"  disabled=\"disabled\"";
    $display_publicacao = "";
    $total_nomeado = "";
    $editavel_nomeado = "";
    $select = array();
    $from = array();

    $titulo = $tpetipo == ACA_TPEDITAL_NOMEACAO ? "Valor da nomeação maior do que de homologação!" : "Valor da homologação maior do que de publicação!";
    $img = "<img align='middle' src='/imagens/atencao.png'/ title='{$titulo}' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";

    // montando a query de acordo com o tipo de edital
    if ($tpetipo == ACA_TPEDITAL_NOMEACAO) {

        $edpid_homo = pegaHomologado($edpid);
        $edpid_pub = pegaPublicado($edpid_homo);
        //-------------recuperando o total de efetivado/nomeados e exibindo

        //total concurso
        $sql_concurso = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
							FROM academico.lancamentosportaria lp
							INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
							WHERE
							lp.prtid = " . $prtid . " AND
							lp.entidcampus = " . $entidcampus . " AND
							lp.entidentidade = " . $entidentidade . " AND
							lp.clsid =" . $clsid . " AND
							p.prtano = '" . $ano . "' AND
							p.tprid = " . ACA_TPORTARIA_CONCURSO . " AND
							lp.lnpstatus = 'A'";
        $concurso = $db->pegaUm($sql_concurso);

        //total provimento
        $sql_provimento = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
							FROM academico.lancamentosportaria lp
							INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
							WHERE
							p.prtidautprov = $prtid AND
							lp.entidcampus = " . $entidcampus . " AND
							lp.entidentidade = " . $entidentidade . " AND
							lp.clsid =" . $clsid . " AND
							p.prtano = '" . $ano . "' AND
							p.tprid = " . ACA_TPORTARIA_PROVIMENTO . " AND
							lp.lnpstatus = 'A'";
        $provimento = $db->pegaUm($sql_provimento);

        //total utilizado para efetivação

        $sql_nomeado = "
				    	SELECT
							COALESCE (sum(lp.lepvlrprovefetivados), 0) as lepvlrprovefetivados
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lp ON lp.edpid = ep.edpid
												 AND lp.lepstatus = 'A'
												 --AND lp.lepano = '$ano'
						INNER JOIN
							academico.cargos c on c.crgid = lp.crgid
						WHERE
							c.clsid = " . $clsid . "
							AND edpideditalhomologacao = $edpid_homo
							AND edpstatus = 'A' ";
        $nomeado = $db->pegaUm($sql_nomeado);

        //disponivel para efetivação
        $disponivel_nomeacao = $provimento - $nomeado;

        $total_nomeado = "<tr>
    					   <input type=\"hidden\" id=\"concurso_autorizado_" . $tabela . "\"  value=\"" . $concurso . "\">
    					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
						  <td>" . $concurso . "</td>
						  </tr>
						  <input type=\"hidden\" id=\"provimento_autorizado_" . $tabela . "\"  value=\"" . $provimento . "\">
    					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Autorizado:</b></td>
						  <td>" . $provimento . "</td>
						  </tr>
						  <tr>
    					  <td  width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Efetivado:</b></td>
						  <td id=\"td_provimento_efetivado_" . $tabela . "\">" . $nomeado . "</td>
						  </tr>
						  <tr>
    					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Efetivação:</b></td>
						 <td id=\"td_disponivel_efetivacao_" . $tabela . "\">" . $disponivel_nomeacao . "</td>
						 </tr>";
        //--------------------------------------------------------------------


        $sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					sum(efetivado) AS efetivado,
					cargo,
					crgid
				FROM (

						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							0 AS efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_pub
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid

					UNION ALL

						SELECT
							0 AS publicado,
							sum( lepvlrhomologado ) as homologado,
							0 as efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_homo
							AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid

					UNION ALL

						SELECT
							0 AS publicado,
							0 as homologado,
							sum(lepvlrprovefetivados) as efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_NOMEACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid


				) as f
				GROUP BY
					cargo,
					crgid";

        $display = "";
        $editavel_homo = "readonly=\"readonly\" style=\"color: #696969\"";

        $editavel_nomeado = "readonly=\"readonly\" style=\"color: 696969\"";
        $display_efe = "";
        $editavel_efe = "readonly=\"readonly\" style=\"color: #696969\"";
        $display_publicacao = "style=\"display:none\"";

    } elseif ($tpetipo == ACA_TPEDITAL_HOMOLOGACAO) {
        $edpid_pub = pegaPublicado($edpid);

        $sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					sum(efetivado) AS efetivado,
					cargo,
					crgid
				FROM (

						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							0 AS efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_pub
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid

					UNION ALL

						SELECT
							0 AS publicado,
							sum( lepvlrhomologado ) as homologado,
							0 as efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid

				) as f
				GROUP BY
					cargo,
					crgid";

        $display = "";
        $editavel_homo = "readonly=\"readonly\" style=\"color: #696969\"";
        $display_publicacao = "style=\"display:none\"";
    } else {

        $sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					sum(efetivado) AS efetivado,
					cargo,
					crgid
				FROM (

						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							0 AS efetivado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid

				) as f
				GROUP BY
					cargo,
					crgid";

        $display_publicacao = "";

    }


    // exibir ou não o projetado  por cargos
    if ($orgid == ACA_ORGAO_SUPERIOR) {
        $display_proj = "style=\"display:none\"";
        //$display_proj = "";
    } else {
        $display_proj = "style=\"display:none\"";
    }

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);

    $habil = verificaPerfil($perfis);
    if ($habil) {
        $editavel = '';
    } else {
        $editavel = "disabled=\"false\"";
    }

    //dump($sql);
    $dados = $db->carregar($sql);

    //total do autorizado para concurso
    $sql_proj = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
							FROM academico.lancamentosportaria lp
							INNER JOIN academico.portarias p ON p.prtid = lp.prtid
							WHERE
							lp.prtid = " . $prtid . " AND
							lp.entidcampus = " . $entidcampus . " AND
							lp.entidentidade = " . $entidentidade . " AND
							lp.clsid =" . $clsid . " AND
							p.prtano = '" . $ano . "' AND
							lp.lnpstatus = 'A'";
    $projetado = $db->pegaUm($sql_proj);

    //total utilizado autorizado para concurso
    $sql_aut_proj = "
							SELECT
							COALESCE (sum(lp.lepvlrpublicacao), 0) as lepvlrpublicacao

							FROM academico.lancamentoeditalportaria lp
							INNER JOIN
								academico.cargos AS ca ON (ca.crgid  = lp.crgid)
							INNER JOIN
								academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
							INNER JOIN
								academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_PUBLICACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus AND eppub.edpidhomo IS null)
							INNER JOIN
								academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
							WHERE
							lp.lepstatus = 'A' AND
							eppub.edpstatus = 'A'
							--AND lp.lepano = '$ano'";
    $utilizado_projetado = $db->pegaUm($sql_aut_proj);
    $disponivel_projetado = $projetado - $utilizado_projetado;

    if ($tpetipo == ACA_TPEDITAL_NOMEACAO) {
        echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
					<thead>
						<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
						<input type=\"hidden\" id=\"td_disponivel_projetado_" . $tabela . "\"  value=\"0\">
						<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $disponivel_projetado . "\">
						" . $total_nomeado . "
					</table>
				";
    } else {
        echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
					<thead>
						<tr>
						<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
						<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
						<td>" . $projetado . "</td>
						</tr>
						<tr>
						<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $disponivel_projetado . "\">
						<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Publicação:</b></td>
						<td id=\"td_disponivel_projetado_" . $tabela . "\">" . $disponivel_projetado . "</td>
						</tr>
					</table>
				";
    }
    echo "<table id=\"" . $tabela . "\" width=\"100%\" align=\"left\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
							<thead>
								<tr>
						            <td width=\"4%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Ação</strong></td>
									<td width=\"46%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Cargo</strong></td>
									<td " . $display_proj . " valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Autorizados</strong></td>
									<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Publicados</strong></td>
									<td " . $display . "  valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Homologados</strong></td>
									<td $display_efe valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Provimentos Efetivados</strong></td>
									<td style='display:none;'  valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Provimentos Não Efetivados</strong></td>
								</tr>
							</thead>
						    <tbody>";

    if ($dados) {
        $cont = 0;
        for ($i = 0; $i < count($dados); $i++) {
            $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
            $cont = $cont + 1;
            $nefeticado = ($dados[$i]["autorizado"] - $dados[$i]["efetivado"]) ? ($dados[$i]["autorizado"] - $dados[$i]["efetivado"]) : 0;
            $img_src = ($dados[$i]["lepobs"] != '') ? "src=\"/imagens/restricao.png \"" : "src=\"/imagens/pop_p.gif \"";

            $img_alert_homo = ($dados[$i]["homologado"] > $dados[$i]["publicado"]) ? $img : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';
            $img_alert_efe = ($dados[$i]["efetivado"] > $dados[$i]["homologado"]) ? $img : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';

            //recuperando o lepid para o teste de exclusão do lançamento
            $sql_lepid = "SELECT lepid
	    							FROM academico.lancamentoeditalportaria
 									WHERE edpid = $edpid
 									AND crgid = " . $dados[$i]["crgid"] . "
 									--AND lepano = '$ano'";
            $lepid = $db->pegaUm($sql_lepid);

            $btexcluir = $habilitado ? "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">"
                : "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir_01.gif\" border=\"0\" title=\"Excluir\">";

            echo "<tr bgcolor=\"" . $cor . "\" id=\"" . $tabela . "_" . $dados[$i]["crgid"] . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
							<td align=\"center\">
								" . $btexcluir . "
								<input type=\"hidden\" value=\"" . $dados[$i]["crgid"] . "\" name=\"crgid[]\">
							</td>
							<td>
								" . $dados[$i]["cargo"] . "
							</td>
							<td $display_proj align=\"center\">
								<input style=\"color: #696969\" " . $disabled . " id=\"projetado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
								 type=\"text\" size=\"15\" maxlength=\"15\" value=\"\">
							</td>
							<td align=\"center\">
								<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\"  value=\"" . $dados[$i]["publicado"] . "\">
								<input $editavel_homo id=\"publicado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
								onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
								onkeyup=\"calculaTotal(this, 'total_pub_" . $tabela . "');\"
								name=\"publicado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
								size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["publicado"] . "\">
							</td>
							<td " . $display . "  align=\"center\">
								<input type=\"hidden\" id=\"homologado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\" value=\"" . $dados[$i]["homologado"] . "\">
								<input $display $editavel_nomeado id=\"homologado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
								onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
								onkeyup=\"calculaTotal(this, 'total_hom_" . $tabela . "');\"
								name=\"homologado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
								size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["homologado"] . "\">
								" . $img_alert_homo . "
							</td>
							<td style='display:none;'  lign=\"left\">
								<input type=\"hidden\" id=\"autorizado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\"  value=\"" . $dados[$i]["autorizado"] . "\">
								<input " . $editavel . " id=\"autorizado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
								onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
								onkeyup=\"calculaTotal(this, 'total_aut_" . $tabela . "');\"
								name=\"autorizado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
								size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["autorizado"] . "\">
							</td>
							<td $display_efe  align=\"center\">
								<input type=\"hidden\" id=\"efetivado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\" value=\"" . $dados[$i]["efetivado"] . "\">
								<input $display_efe id=\"efetivado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
								onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
								onkeyup=\"calculaTotal(this, 'total_efe_" . $tabela . "');\"
								name=\"efetivado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
								size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["efetivado"] . "\">
								" . $img_alert_efe . "
							</td>
							<td style='display:none;'  align=\"left\" valing=\"center\">
								<input type=\"hidden\" id=\"nefetivado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\"  value=\"" . $nefeticado . "\">
								<input type=\"hidden\" id=\"nefetivado_obs_" . $tabela . "_" . $dados[$i]["crgid"] . "\" name=\"obs_nefetivado[]\" value=\"" . $dados[$i]["lepobs"] . "\">
								<input disabled=\"false\" id=\"nefetivado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
								name=\"nefetivado[]\" type=\"text\"	size=\"15\" maxlength=\"15\" value=\"" . $nefeticado . "\">
								<img style=\"cursor: pointer;\" " . $img_src . " border=0 onclick=\"cadastrarObs('nefetivado_obs_" . $tabela . "_" . $dados[$i]["crgid"] . "');\" title=\"Observação\">
							</td>
						</tr>";
        }
        echo("
			    		<tr  bgcolor=\"#FFFFFF\" >
			    			<td align=\"right\"></td>
							<td align=\"right\"><b>Totais</b></td>
							<td " . $display_proj . " align=\"center\">
								<input  style=\"color: #696969\" " . $disabled . " id='total_proj_" . $tabela . "' class=\"CampoEstilo\"
								 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_projetado"] . "\">
							</td>
							<td  align=\"center\">
								<input style=\"color: #696969\" " . $disabled . " id='total_pub_" . $tabela . "' class=\"CampoEstilo\"
								 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_publicado"] . "\">
							</td>
							<td $display align=\"center\">
								<input $display style=\"color: #696969\" " . $disabled . " id='total_hom_" . $tabela . "' class=\"CampoEstilo\"
								 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_homologado"] . "\">
								 <span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>
							</td>
							<td style='display:none;' >
								<input style=\"color: #696969\" " . $disabled . " id='total_aut_" . $tabela . "' class=\"CampoEstilo\"
								 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_autorizado"] . "\">
							</td>
							<td $display_efe align=\"center\">
								<input style=\"color: #696969\" " . $disabled . " id='total_efe_" . $tabela . "' class=\"CampoEstilo\"
								 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_efetivado"] . "\">
								 <span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>
							</td>
							<td style='display:none;' >
								<input style=\"color: #696969\" " . $disabled . " id ='total_nefe_" . $tabela . "' class=\"CampoEstilo\"
								 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_nefetivado"] . "\">
							</td>
						</tr>
						");
    }
    echo "</tbody>
					</table>";

    //-------------------- Calculando os totais
    if ($orgid == ACA_ORGAO_SUPERIOR) {
        echo("<script>calculaTotalInicial( 'projetado_" . $tabela . "','total_proj_" . $tabela . "');</script>");
    }

    echo("<script>calculaTotalInicial( 'publicado_" . $tabela . "','total_pub_" . $tabela . "');</script>");

    if ($tpetipo == ACA_TPEDITAL_HOMOLOGACAO || $tpetipo == ACA_TPEDITAL_NOMEACAO) {
        echo("<script>calculaTotalInicial( 'homologado_" . $tabela . "','total_hom_" . $tabela . "');</script>");
    }

    if ($tpetipo == ACA_TPEDITAL_NOMEACAO) {
        echo("<script>calculaTotalInicial( 'efetivado_" . $tabela . "','total_efe_" . $tabela . "');</script>");
    }

}

/*
 *************************************************************************************************
 */
function academico_possui_perfil_sem_vinculo()
{

    global $db;

    $sql = "SELECT
				count(*)
			FROM
				seguranca.perfil p
			INNER JOIN
				seguranca.perfilusuario u on
				u.pflcod = p.pflcod
			LEFT JOIN
				academico.tprperfil tp on
				tp.pflcod = p.pflcod
			LEFT JOIN
				academico.tiporesponsabilidade tr on
				tr.tprcod = tp.tprcod
			WHERE
				p.pflstatus = 'A' AND
				p.sisid = '56' AND
				u.usucpf = '" . $_SESSION['usucpf'] . "' AND
				tr.tprcod is null";

    return $db->pegaUm($sql) > 0;
}

function academico_possui_perfil_resp_tipo_ensino()
{

    global $db;

    $sql = "SELECT
				count(*)
			FROM
				seguranca.perfil p
			INNER JOIN
				seguranca.perfilusuario u on u.pflcod = p.pflcod
			INNER JOIN
				academico.tprperfil tp on tp.pflcod = p.pflcod
			INNER JOIN
				academico.tiporesponsabilidade tr on tr.tprcod = tp.tprcod
			WHERE
				p.pflstatus = 'A' AND
				p.sisid = '56' AND
				u.usucpf = '" . $_SESSION['usucpf'] . "' AND
				tr.tprcod = 1";
    return $db->pegaUm($sql) > 0;
}

/**
 * Função que verifica quais são os órgãos que o usuário possui permissão
 *
 * @author Fernando Araújo Bagno da Silva
 * @return array
 */
function academico_pega_orgao_permitido($tela = 'inicio')
{

    global $db;
    static $orgao = null;

    $link = $tela == 'inicio' ? '/academico/academico.php?modulo=inicio&acao=C&orgid=' : '/academico/academico.php?modulo=principal/painel&acao=A&orgid=';

    if ($tela == 'painel') {
        $where = " AND o.orgid IN (1,2)";
    }

    if ($orgao === null) {
        if ($db->testa_superuser() || academico_possui_perfil_sem_vinculo()) {

            // pega todos os orgãos
            $sql = "
				SELECT
					o.orgdesc                                               as descricao,
	                o.orgid                                                 as id,
                	'{$link}' || o.orgid as link
				FROM
					academico.orgao o
				WHERE 1 = 1
				$where
				ORDER BY
					o.orgid";

        } else {
            $sql = "
				SELECT DISTINCT
					coalesce(o.orgdesc, o2.orgdesc) as descricao,
					coalesce(o.orgid, o2.orgid) as id,
					'{$link}' || coalesce(o.orgid, o2.orgid) as link
				FROM
					academico.usuarioresponsabilidade ur
				LEFT JOIN
					academico.orgao o ON ur.orgid = o.orgid
				LEFT JOIN
					seguranca.perfil p ON ur.pflcod = p.pflcod
				LEFT JOIN
					seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod
												  AND pu.usucpf = ur.usucpf
				LEFT JOIN
					entidade.entidade en ON ur.entid = en.entid
				LEFT JOIN
					entidade.funcaoentidade ef ON ef.entid = en.entid
												  AND ef.fuestatus = 'A'
												  AND ef.funid IN (11,12,14,102)
				LEFT JOIN
					academico.orgaofuncao of ON ef.funid = of.funid
				LEFT JOIN
					academico.orgao o2 ON of.orgid = o2.orgid
				WHERE
					ur.usucpf = '{$_SESSION["usucpf"]}' AND
					ur.rpustatus = 'A' AND
					(ef.funid IN (11,12,14,102) OR ef.funid IS NULL)
					AND p.sisid = 56
					AND coalesce(o.orgdesc, o2.orgdesc) is not null
					$where
					";
        }
        $orgao = $db->carregar($sql);

    }

    return $orgao;
}


/**
 * Pega as unidades que o usuário possui responsabilidade
 *
 * @author Fernando Araújo Bagno da Silva
 * @return array
 *
 */
function academico_pegarUnidadesPermitidas()
{

    global $db;
    static $unidades = null;

    if ($unidades === null) {
        if ($db->testa_superuser() || academico_possui_perfil_sem_vinculo()) {

            // pega todas as unidades
            $sql = "
				SELECT
					e.entid
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON e.entid = ef.entid
				WHERE
					ef.funid IN (12,11,14,102)";

        } else {

            // pega as unidades do perfil do usuário
            $sql = "
				SELECT
					ur.entid
				FROM
					academico.usuarioresponsabilidade ur
				INNER JOIN
					entidade.entidade et ON
					et.entid = ur.entid
				INNER JOIN
					seguranca.perfil p ON
					p.pflcod = ur.pflcod
				INNER JOIN
					seguranca.perfilusuario pu ON
					pu.pflcod = ur.pflcod AND
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A' AND
					p.sisid = 56";
        }
        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();
        $unidades = array();

        foreach ($dados as $linha) {
            array_push($unidades, $linha['entid']);
        }
    }
    return $unidades;
}

/**
 * Função que verifica se o usuário possui perfil para acessar as páginas
 *
 * @author Fernando Araújo Bagno da Silva
 * @param array $pflcods
 * @return integer possui ou não perfil
 */
function academico_possui_perfil($pflcods)
{
    global $db;

    if ($db->testa_superuser()) {

        return true;

    } else {

        if (is_array($pflcods)) {
            $pflcods = array_map("intval", $pflcods);
            $pflcods = array_unique($pflcods);
        } else {
            $pflcods = array((integer)$pflcods);
        }
        if (count($pflcods) == 0) {
            return false;
        }
        $sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode(",", $pflcods) . " ) ";
        return $db->pegaUm($sql) > 0;

    }
}

function academico_array_perfil()
{
    global $db;

    $sql = sprintf("SELECT
						pu.pflcod
					FROM
						seguranca.perfilusuario pu
					INNER JOIN
						seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 56
					WHERE
						pu.usucpf = '%s'
					ORDER BY
						p.pflnivel",
        $_SESSION['usucpf']);

    return (array)$db->carregarColuna($sql, 'pflcod');
}

function academico_direcionaUser()
{
    global $db;

    if ($db->testa_superuser() || academico_possui_perfil_sem_vinculo()) {
        return;
    }

    $arrTipoMec = unserialize(ARRAY_PERFIL_MEC);
    $arrTipoIfes = unserialize(ARRAY_PERFIL_IFES);

    $arrOrgao = academico_pega_orgao_permitido();

    if (academico_possui_perfil($arrTipoMec) && !academico_possui_perfil($arrTipoIfes) && count($arrOrgao) == 1) {
        $_SESSION['academico']['orgid'] = $arrOrgao[0]['id'];
        die("<script>location.href='academico.php?modulo=principal/listarPortarias&acao=C';</script>");
    } elseif (academico_possui_perfil($arrTipoIfes) && !academico_possui_perfil($arrTipoMec) && count($arrOrgao) == 1) {
        $_SESSION['academico']['orgid'] = $arrOrgao[0]['id'];
        die("<script>location.href='academico.php?modulo=principal/listaUnidade&acao=A';</script>");
    }

    return;
}


/**
 * Enter description here...
 *
 * @return unknown
 */
function carregardadosmenuacademico($contexto = null)
{

    if ($contexto == 'obrasunidade') {
        $menu = array(0 => array("id" => 1, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
            1 => array("id" => 2, "descricao" => "Tabela da entidade", "link" => "/academico/academico.php?modulo=principal/editarentidade&acao=A&tpeid=" . $_SESSION["academico"]["orgid"] . "&entidunidade=" . $_SESSION["academico"]["entid"]),
            2 => array("id" => 3, "descricao" => "Dados da Unidade", "link" => "/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            3 => array("id" => 4, "descricao" => "Dados Específicos", "link" => "/academico/academico.php?modulo=principal/inserir_entidade&acao=A&page=esp&entidunidade=" . $_SESSION["academico"]["entid"] . "&iscampus=nao"),
            4 => array("id" => 5, "descricao" => "Lista de Campus", "link" => "/academico/academico.php?modulo=principal/listaCampus&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            5 => array("id" => 6, "descricao" => "Contatos", "link" => "/academico/academico.php?modulo=principal/dadosdirigentes&acao=C"),
            6 => array("id" => 7, "descricao" => "Rel. de Monit. de Concursos e Provimentos", "link" => "/academico/academico.php?modulo=principal/relUniDistribuicao&acao=A"),
            7 => array("id" => 8, "descricao" => "Obras", "link" => "/academico/academico.php?modulo=principal/lista_de_obras&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            8 => array("id" => 9, "descricao" => "Dados da Obra", "link" => "/academico/academico.php?modulo=principal/extrato_selecionar&acao=A&entidcampus={$_SESSION["academico"]["entidcampus"]}&obrid={$_REQUEST["obrid"]}"),
            9 => array("id" => 10, "descricao" => "Nota Técnica", "link" => "/academico/academico.php?modulo=principal/notatecnica&acao=A&entidunidade=" . $_SESSION["academico"]["entid"])
        );
        /*		$menu = array(0 => array("id" => 1, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados da Unidade", 	"link" => "/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=".$_SESSION["academico"]["entid"]),
					  2 => array("id" => 3, "descricao" => "Lista de Campus",   "link" => "/academico/academico.php?modulo=principal/listaCampus&acao=A&entidunidade=".$_SESSION["academico"]["entid"]),
					  3 => array("id" => 4, "descricao" => "Contatos",  "link" => "/academico/academico.php?modulo=principal/dadosdirigentes&acao=C"),
					  4 => array("id" => 5, "descricao" => "Rel. de Monit. de Concursos e Provimentos",  "link" => "/academico/academico.php?modulo=principal/relUniDistribuicao&acao=A"),
					  5 => array("id" => 6, "descricao" => "Obras",  "link" => "/academico/academico.php?modulo=principal/lista_de_obras&acao=A&entidunidade=".$_SESSION["academico"]["entid"]),
					  6 => array("id" => 7, "descricao" => "Dados da Obra",  "link" => "/academico/academico.php?modulo=principal/extrato_selecionar&acao=A&entidcampus={$_SESSION["academico"]["entidcampus"]}&obrid={$_REQUEST["obrid"]}"),
					  7 => array("id" => 8, "descricao" => "Nota Técnica",  "link" => "/academico/academico.php?modulo=principal/notatecnica&acao=A&entidcampus={$_SESSION["academico"]["entidcampus"]}")
					  );*/
    } else if ($contexto == 'obrascampus') {
        $menu = array(0 => array("id" => 1, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
            1 => array("id" => 2, "descricao" => "Dados da Unidade", "link" => "/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            2 => array("id" => 3, "descricao" => "Tabela do Campus", "link" => "/academico/academico.php?modulo=principal/editarcampus&acao=A&entid=" . $_SESSION["academico"]["entidcampus"]),
            3 => array("id" => 4, "descricao" => "Dados do Campus", "link" => "/academico/academico.php?modulo=principal/dadoscampus&acao=A&entid=" . $_SESSION["academico"]["entidcampus"]),
            4 => array("id" => 5, "descricao" => "Dados Específicos", "link" => "/academico/academico.php?modulo=principal/inserir_entidade&acao=A&page=esp&entidcampus=" . $_SESSION["academico"]["entidcampus"] . "&iscampus=sim"),
            5 => array("id" => 6, "descricao" => "Lista de Portarias", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"]),
            6 => array("id" => 7, "descricao" => "Contatos", "link" => "/academico/academico.php?modulo=principal/dadosdirigentes&acao=A"),
            7 => array("id" => 8, "descricao" => "Rel. de Monit. de Concursos e Provimentos", "link" => "/academico/academico.php?modulo=principal/relDistribuicao&acao=A"),
            8 => array("id" => 9, "descricao" => "Obras", "link" => "/academico/academico.php?modulo=principal/lista_de_obras&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"]),
            9 => array("id" => 10, "descricao" => "Dados da Obra", "link" => "/academico/academico.php?modulo=principal/extrato_selecionar&acao=A&entidcampus={$_SESSION["academico"]["entidcampus"]}&obrid={$_REQUEST["obrid"]}"),
            10 => array("id" => 11, "descricao" => "Nota Técnica", "link" => "/academico/academico.php?modulo=principal/notatecnica&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"])
        );
    } else if ($contexto == 'unidade') {
        $menu = array(0 => array("id" => 1, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
            1 => array("id" => 2, "descricao" => "Tabela da entidade", "link" => "/academico/academico.php?modulo=principal/editarentidade&acao=A&tpeid=" . $_SESSION["academico"]["orgid"] . "&entidunidade=" . $_SESSION["academico"]["entid"]),
            2 => array("id" => 3, "descricao" => "Dados da Unidade", "link" => "/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            3 => array("id" => 4, "descricao" => "Dados Específicos", "link" => "/academico/academico.php?modulo=principal/inserir_entidade&acao=A&page=esp&entidunidade=" . $_SESSION["academico"]["entid"] . "&iscampus=nao"),
            4 => array("id" => 5, "descricao" => "Lista de Campus", "link" => "/academico/academico.php?modulo=principal/listaCampus&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            5 => array("id" => 6, "descricao" => "Contatos", "link" => "/academico/academico.php?modulo=principal/dadosdirigentes&acao=C"),
            6 => array("id" => 7, "descricao" => "Rel. de Monit. de Concursos e Provimentos", "link" => "/academico/academico.php?modulo=principal/relUniDistribuicao&acao=A"),
            7 => array("id" => 8, "descricao" => "Obras", "link" => "/academico/academico.php?modulo=principal/lista_de_obras&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
            8 => array("id" => 9, "descricao" => "Nota Técnica", "link" => "/academico/academico.php?modulo=principal/notatecnica&acao=A&entidunidade=" . $_SESSION["academico"]["entid"])
        );
    } elseif ($contexto == 'listaportaria') {
        $menu = array(0 => array("id" => 1, "descricao" => "2008", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2008"),
            1 => array("id" => 2, "descricao" => "2009", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2009"),
            2 => array("id" => 3, "descricao" => "2010", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2010"),
            3 => array("id" => 4, "descricao" => "2011", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2011"),
            4 => array("id" => 5, "descricao" => "2012", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2012"),
            5 => array("id" => 6, "descricao" => "2013", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2013"),
            6 => array("id" => 7, "descricao" => "2014", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2014"),
            7 => array("id" => 8, "descricao" => "2015", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2015"),/* Inserção do ano de 2012 dia 18/11/2010*/
            8 => array("id" => 9, "descricao" => "2016", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2016"),/* Inserção do ano de 2012 dia 18/11/2010*/
            9 => array("id" => 10, "descricao" => "2017", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2017"),/* Inserção do ano de 2012 dia 18/11/2010*/
            9 => array("id" => 10, "descricao" => "2018", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus={$_SESSION['academico']['entidcampus']}&ano=2018"),/* Inserção do ano de 2012 dia 18/11/2010*/
        );
    } elseif ($contexto == 'dadosPortaria') {
        $title = $_SESSION["academico"]["orgid"] == 1 ? 'Educação Superior' : 'Educação Profissional';
        $acao = $_SESSION["academico"]["orgid"] == 1 ? 'C' : 'A';
        $menu = array(0 => array("id" => 1, "descricao" => $title, "link" => "/academico/academico.php?modulo=principal/listarPortarias&acao=" . $acao),
            1 => array("id" => 2, "descricao" => "Dados da Portaria", "link" => "/academico/academico.php?modulo=principal/cadportaria&acao=C"),
            2 => array("id" => 3, "descricao" => "Distribuição de Cargos", "link" => "/academico/academico.php?modulo=principal/planodistribuicaocargos&acao=C"),
            3 => array("id" => 4, "descricao" => "Documentos da Portaria", "link" => "/academico/academico.php?modulo=principal/documentos&acao=C"),
        );
    } else {
        if (academico_possui_perfil(PERFIL_ADMINISTRADOR) || academico_possui_perfil(PERFIL_SUPERUSUARIO)) {

            // monta menu padrão contendo informações sobre as entidades
            $menu = array(0 => array("id" => 1, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
                1 => array("id" => 2, "descricao" => "Dados da Unidade", "link" => "/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
                2 => array("id" => 3, "descricao" => "Tabela do Campus", "link" => "/academico/academico.php?modulo=principal/editarcampus&acao=A&entid=" . $_SESSION["academico"]["entidcampus"]),
                3 => array("id" => 4, "descricao" => "Dados do Campus", "link" => "/academico/academico.php?modulo=principal/dadoscampus&acao=A&entid=" . $_SESSION["academico"]["entidcampus"]),
                4 => array("id" => 5, "descricao" => "Dados Específicos", "link" => "/academico/academico.php?modulo=principal/inserir_entidade&acao=A&page=esp&entidcampus=" . $_SESSION["academico"]["entidcampus"] . "&iscampus=sim"),
                5 => array("id" => 6, "descricao" => "Lista de Portarias", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"]),
                6 => array("id" => 7, "descricao" => "Contatos", "link" => "/academico/academico.php?modulo=principal/dadosdirigentes&acao=A"),
                7 => array("id" => 8, "descricao" => "Rel. de Monit. de Concursos e Provimentos", "link" => "/academico/academico.php?modulo=principal/relDistribuicao&acao=A"),
                8 => array("id" => 9, "descricao" => "Obras", "link" => "/academico/academico.php?modulo=principal/lista_de_obras&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"]),
                9 => array("id" => 10, "descricao" => "Nota Técnica", "link" => "/academico/academico.php?modulo=principal/notatecnica&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"])
            );

        } else {
            /*			$menu = array(
							0 => array("id" => 1, "descricao" => "Lista de Unidades",  	"link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
						  1 => array("id" => 2, "descricao" => "Lista de Portarias", 	"link" => "/academico/academico.php?modulo=principal/listareditais&acao=C&evento=A&entidcampus=" . $_SESSION["academico"]["entidcampus"])
						  );*/

            // monta menu padrão contendo informações sobre as entidades
            $menu = array(0 => array("id" => 1, "descricao" => "Lista de Unidades", "link" => "/academico/academico.php?modulo=principal/listaUnidade&acao=A"),
                1 => array("id" => 2, "descricao" => "Dados da Unidade", "link" => "/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=" . $_SESSION["academico"]["entid"]),
                2 => array("id" => 3, "descricao" => "Tabela do Campus", "link" => "/academico/academico.php?modulo=principal/editarcampus&acao=A&entid=" . $_SESSION["academico"]["entidcampus"]),
                3 => array("id" => 4, "descricao" => "Dados do Campus", "link" => "/academico/academico.php?modulo=principal/dadoscampus&acao=A&entid=" . $_SESSION["academico"]["entidcampus"]),
                4 => array("id" => 5, "descricao" => "Dados Específicos", "link" => "/academico/academico.php?modulo=principal/inserir_entidade&acao=A&page=esp&entidcampus=" . $_SESSION["academico"]["entidcampus"] . "&iscampus=sim"),
                5 => array("id" => 6, "descricao" => "Lista de Portarias", "link" => "/academico/academico.php?modulo=principal/listaPortaria&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"]),
                6 => array("id" => 7, "descricao" => "Contatos", "link" => "/academico/academico.php?modulo=principal/dadosdirigentes&acao=A"),
                7 => array("id" => 8, "descricao" => "Rel. de Monit. de Concursos e Provimentos", "link" => "/academico/academico.php?modulo=principal/relDistribuicao&acao=A"),
                8 => array("id" => 9, "descricao" => "Obras", "link" => "/academico/academico.php?modulo=principal/lista_de_obras&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"]),
                9 => array("id" => 10, "descricao" => "Nota Técnica", "link" => "/academico/academico.php?modulo=principal/notatecnica&acao=A&entidcampus=" . $_SESSION["academico"]["entidcampus"])
            );

        }
    }
    return $menu;

}

/**
 *
 */
function academico_montacabecalhounidades($entid)
{

}

/**
 * Enter description here...
 *
 * @param unknown_type $dados
 */
function buscarCnpj($dados)
{
    global $db;
    ob_end_clean();
    $entidade = Entidade::carregarEntidadePorCnpjCpf(str_replace(array(".", "/"), array("", ""), $dados['entnumcpfcnpj']), $db->testa_superuser());
    if ($entidade->getPrimaryKey() !== null) {
        die($entidade->getPrimaryKey());
    } else {
        die('0');
    }
}


function buscarIdFuncaoAssoc($entid, $funid){
    global $db;
    
    if( $_SESSION["academico"]["entidadenivel"] == "unidade" ){
        $_entid_inst = $_SESSION['academico']['entid'];
    }else{
        $_entid_inst = $_SESSION['academico']['entidcampus'];
    }
    
    $sql = "
        SELECT fea.feaid
        FROM entidade.funcaoentidade fen

	LEFT JOIN entidade.funcao fun ON fun.funid = fen.funid
	LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
	LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid

        WHERE fen.entid = {$entid} AND fen.funid = {$funid} AND ent.entid = {$_entid_inst}
    ";
    return $db->pegaUm($sql);
}


function salvarRegistroDirigente($dados){
    global $db;

    $acao = $_SESSION["academico"]["entidadenivel"] == "unidade" ? 'A' : 'C';

    $entidade = new Entidades();
    $entidade->carregarEntidade($dados);
    $entidade->adicionarFuncoesEntidade($dados['funcoes']);
    $entidade->salvar();

    if ($dados['opc'] == 'NV') {
        $dados['entid'] = $entidade->getEntId();
    }
    
    $feaid = buscarIdFuncaoAssoc($dados['entid'], $dados['funid']);

    if ($_REQUEST['tela'] == "viagemExterior") {
        $tela = "?modulo=principal/solicitacaoViagemExterior&acao=A";
        $opc = 'V';
    } else {
        $tela = "?modulo=principal/dadosdirigentes&acao=$acao";
        $opc = 'D';
    }

    echo " <script type=\"text/javascript\"> ";

    if ($opc == 'V') {
        echo " alert(\"Operação realizada com sucesso!\"); ";
        echo " window.opener.location = '{$tela}'; ";
        echo " window.close(); ";
    } else {
        echo " alert(\"Operação realizada com sucesso. De continuidade preenchendo as Informações Adicionais!\"); ";
        echo " window.location = '?modulo=principal/dados_adicionais_dirigentes&acao=A&entid={$dados['entid']}&funid={$dados['funid']}&feaid={$feaid}&opc={$dados['opc']}&sit={$dados['sit']}'; ";
    }
    echo " </script> ";

    exit;
}

/**
 * Função que verifica se existe portaria com o prtid informado
 *
 */
function academico_verificaportaria($prtid)
{

    global $db;

    $portaria = $db->pegaLinha("SELECT
									prtid
								FROM
									academico.portarias
								WHERE
									prtid = {$prtid} AND
									prtstatus = 'A'");

    return $portaria;

}

/**
 * Função que verifica se o usuário possui permissão na obra informada
 *
 */
function academico_verificapermissao($prtid, $campus)
{

    global $db;

    $academico = new autoriazacaoconcursos();
    $entid = $academico->buscaentidade($campus);

    if ($db->testa_superuser() || (academico_possui_perfil(PERFIL_MECCADASTRO) ||
            academico_possui_perfil(PERFIL_MECCONSULTAGERAL) ||
            academico_possui_perfil(PERFIL_ADMINISTRADOR) ||
            academico_possui_perfil(PERFIL_CONSULTA_GERAL))
    ) {

        return true;

    } else {

        if (!$entid) {

            return false;

        } else {

            $portaria = $db->pegaUm("SELECT
										rpuid
									FROM
										academico.usuarioresponsabilidade ur
									WHERE
										ur.usucpf = '{$_SESSION["usucpf"]}' AND
										entid = {$entid} AND
										ur.rpustatus = 'A'");

        }
        return $portaria;

    }

}

/**
 * Verifica a Eexitência de um programa...
 *
 * @param unknown_type $prgid
 */
function academico_verificaprograma($prgid)
{

    global $db;
    $programa = "";

    if ($prgid) {
        $programa = $db->pegaUm("SELECT
						   		prgid
						   	FROM
						   		academico.programa
						   	WHERE
						   		prgid = {$prgid}");
    }

    return $programa;

}

/**
 * Verifica a exitência de um edital...
 *
 * @param unknown_type $edpid
 */
function academico_existeedital($edpid)
{

    global $db;
    $edital = "";

    if ($edpid) {
        $edital = $db->pegaUm("SELECT
						   		edpid
						   	FROM
						   		academico.editalportaria
						   	WHERE
						   		edpid = {$edpid}");
    }

    return $edital;

}

/**
 * Verifica a Eexitência de um orgao...
 *
 * @param unknown_type $orgid
 */
function academico_existeorgao($orgid)
{

    global $db;
    $orgao = "";

    if ($orgid) {
        $orgao = $db->pegaUm("SELECT
						   		orgid
						   	FROM
						   		academico.orgao
						   	WHERE
						   		orgid = {$orgid}");
    }


    return $orgao;

}

/**
 * Verifica a Existência de uma entidade...
 *
 * @param unknown_type $entid
 */
function academico_existeentidade($entid)
{

    global $db;
    $entidade = "";

    if ($entid) {
        $entidade = $db->pegaUm("SELECT
						   		entid
						   	FROM
						   		entidade.entidade
						   	WHERE
						   		entid = {$entid}");
    }

    return $entidade;

}

/**
 * Verifica a Eexitência de um campus...
 *
 * @param unknown_type $entid
 */
function academico_existecampus($entid)
{

    global $db;
    $campus = "";

    if ($entid) {
        $campus = $db->pegaUm("SELECT
						   		entid
						   	FROM
						   		entidade.entidade
						   	WHERE
						   		entid = {$entid}");
    }

    return $campus;

}

/**
 * Verifica a Eexitência de um ano...
 *
 * @param unknown_type $ano
 */
function academico_existeano($ano)
{

    $anos_validos = array('2008', '2009', '2010', '2011', '2012', '2013', '2014', '2015', '2016', '2017', '2018');

    $valido = in_array($ano, $anos_validos) ? true : false;

    return $valido;
}

function academico_existefuncao($entid, $funid)
{
    global $db;
    $exist = false;

    if ($entid && $funid) {
        $exist = $db->pegaUm("SELECT
						   	   	f.funid
							   FROM
							   	entidade.entidade e
							   	JOIN entidade.funcaoentidade fe USING(entid)
							   	JOIN entidade.funcao f USING(funid)
							   WHERE
							    funid = {$funid}
							   	AND entid = {$entid}");
    }

    return $exist ? true : false;
}

/**
 * FUNÇÕES PARA CRIAR AS TABELAS DE LANCAMENTOS DE EDITAIS DE PUBLICAÇÃO HOMOLOGAÇÃO E EFETIVAÇÃO
 *
 */
function criarTabelaPublicado($tabela, $clsid)
{

    global $db, $habilitado;

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $edpid = $_SESSION['academico']['edpid'];
    $tpetipo = $_SESSION['academico']['tpetipo'];
    $prtid = $_SESSION['academico']['prtid'];
    $prgid = $_SESSION['academico']['prgid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $orgid = $_SESSION["academico"]["orgid"];
    $ano = $_SESSION['academico']['ano'];

    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);

    $select = array();
    $from = array();

    $sql = "
			SELECT
				sum(lepvlrpublicacao) as publicado,
				ca.crgdsc as cargo,
				lep.crgid,
				lep.lepid
			FROM
				academico.editalportaria ep
			INNER JOIN
				academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
									  AND lep.lepstatus = 'A'
			INNER JOIN
				academico.cargos AS ca ON (ca.crgid  = lep.crgid)
			INNER JOIN
				academico.classes AS cls ON (cls.clsid = ca.clsid)
			WHERE
				ep.edpid = $edpid
				AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
				AND ep.edpstatus = 'A'
				--AND ep.edpano = '$ano'
				AND cls.clsid = $clsid
			GROUP BY
				ca.crgdsc,
				lep.crgid,
				lep.lepid
			";


    $dados = $db->carregar($sql);

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);

    $habil = verificaPerfil($perfis);
    if ($habil) {
        $editavel = '';
    } else {
        $editavel = "disabled=\"false\"";
    }

    //total do autorizado para concurso
    $sql_proj = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
						FROM academico.lancamentosportaria lp
						INNER JOIN academico.portarias p ON p.prtid = lp.prtid
						WHERE
						lp.prtid = " . $prtid . " AND
						lp.entidcampus = " . $entidcampus . " AND
						lp.entidentidade = " . $entidentidade . " AND
						lp.clsid =" . $clsid . " AND
						p.prtano = '" . $ano . "' AND
						lp.lnpstatus = 'A'";
    $projetado = $db->pegaUm($sql_proj);

    //total utilizado autorizado para concurso
    $sql_aut_proj = "
					SELECT
					COALESCE (sum(lp.lepvlrpublicacao), 0) as lepvlrpublicacao

					FROM academico.lancamentoeditalportaria lp
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lp.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
					INNER JOIN
						academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_PUBLICACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus AND eppub.edpidhomo IS null)
					INNER JOIN
						academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
					WHERE
					lp.lepstatus = 'A' AND
					eppub.edpstatus = 'A'
					--AND ep.edpano = '$ano'";
    $utilizado_projetado = $db->pegaUm($sql_aut_proj);
    $disponivel_projetado = $projetado - $utilizado_projetado;

    //montando totalizadores iniciais da tabela
    echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
		<thead>
			<tr>
			<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
			<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
			<td>" . $projetado . "</td>
			</tr>
			<tr>
			<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $disponivel_projetado . "\">
			<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Publicação:</b></td>
			<td id=\"td_disponivel_projetado_" . $tabela . "\">" . $disponivel_projetado . "</td>
			</tr>
		</table>
	";
    //montando cabeçalho da tabela
    echo "<table id=\"" . $tabela . "\" width=\"100%\" align=\"left\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
			<thead>
				<tr>
					<td width=\"4%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Ação</strong></td>
					<td width=\"46%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Cargo</strong></td>
					<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Publicados</strong></td>
				</tr>
			</thead>
			<tbody>";

    if ($dados) {

        if ($habilitado) {
            $btexcluir = "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">";
            $editavel = "";
        } else {
            $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
            $editavel = "disabled=\"disabled\"";
        }

        $cont = 0;
        for ($i = 0; $i < count($dados); $i++) {
            $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
            $cont = $cont + 1;

            echo "<tr bgcolor=\"" . $cor . "\" id=\"" . $tabela . "_" . $dados[$i]["crgid"] . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
					<td align=\"center\">
						$btexcluir
						<input type=\"hidden\" value=\"" . $dados[$i]["crgid"] . "\" name=\"crgid[]\">
					</td>
					<td>
						" . $dados[$i]["cargo"] . "
					</td>
					<td align=\"center\">
						<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\"  value=\"" . $dados[$i]["publicado"] . "\">
						<input $editavel id=\"publicado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
						onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
						onkeyup=\"calculaTotal(this, 'total_pub_" . $tabela . "');\"
						name=\"publicado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
						size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["publicado"] . "\">
					</td>
				</tr>";
        }
        echo("
				<tr  bgcolor=\"#FFFFFF\" >
					<td align=\"right\"></td>
					<td align=\"right\"><b>Totais</b></td>
					<td  align=\"center\">
						<input style=\"color: #696969\" disabled=\"false\" id='total_pub_" . $tabela . "' class=\"CampoEstilo\"
						 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_publicado"] . "\">
					</td>
				</tr>
				");
    }
    echo "</tbody>
			</table>";

    //-------------------- Calculando os totais
    if ($orgid == ACA_ORGAO_SUPERIOR) {
        echo("<script>calculaTotalInicial( 'projetado_" . $tabela . "','total_proj_" . $tabela . "');</script>");
    }

    echo("<script>calculaTotalInicial( 'publicado_" . $tabela . "','total_pub_" . $tabela . "');</script>");

    if ($habilitado) {
        echo("<br/>
				<div style=\"text-align:right;\">
					&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"listarCargos('$tabela',$clsid);\"><img src=\"/imagens/gif_inclui.gif\" style=\"cursor:pointer;\" border=\"0\" title=\"Inserir Cargos\">&nbsp;&nbsp;Inserir Cargos</a>
				</div>");
    } else {
        echo("<br/>
				<div style=\"text-align:right;\">
					&nbsp;&nbsp;<img src=\"/imagens/gif_inclui_d.gif\"  border=\"0\" title=\"Inserir Cargos\">&nbsp;&nbsp;Inserir Cargos
				</div>");
    }

}

function criarTabelaDocentesPublicado($tabela, $clsid)
{

    global $db, $habilitado;
//	$crgid = 323;

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);
    $habil = verificaPerfil($perfis);

    $prtid = $_SESSION['academico']['prtid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $edpid = $_SESSION['academico']['edpid'];
    $tpetipo = $_SESSION["academico"]["tpetipo"];
    $prgid = $_SESSION['academico']['prgid'];
    $ano = $_SESSION['academico']['ano'];
    $select = array();
    $from = array();

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);

    //obetendo o total projetado
    $sql_proj = "SELECT COALESCE (SUM(lnpvalor), 0) as lnpvalor
				FROM academico.lancamentosportaria
				WHERE
			    prtid = " . $prtid . " AND
			    entidcampus = " . $entidcampus . " AND
			    entidentidade = " . $entidentidade . " AND
			    clsid = " . $clsid . " AND
			    lnpstatus = 'A'";

    $projetado = $db->pegaUm($sql_proj);

    //total utilizado autorizado para concurso
    $sql_aut_proj = "
						SELECT
						COALESCE (sum(lp.lepvlrpublicacao), 0) as lepvlrpublicacao

						FROM academico.lancamentoeditalportaria lp
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lp.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
						INNER JOIN
							academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_PUBLICACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus AND eppub.edpidhomo IS null)
						INNER JOIN
							academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
						WHERE
						lp.lepstatus = 'A' AND
						eppub.edpstatus = 'A'
						--AND ep.edpano = '$ano'";

    $utilizado_projetado = $db->pegaUm($sql_aut_proj);
    $disponivel_projetado = $projetado - $utilizado_projetado;

    //montando totalizadores iniciais da tabela
    echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
					<thead>
						<tr>
						<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
						<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
						<td>" . $projetado . "</td>
						</tr>
						<tr>
						<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $disponivel_projetado . "\">
						<td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Publicação:</b></td>
						<td id=\"td_disponivel_projetado_" . $tabela . "\">" . $disponivel_projetado . "</td>
						</tr>
					</table>
				";
    //montando cabeçalho da tabela
    echo("<table id='" . $tabela . "' width='100%' align='center' border='0' cellspacing='2' cellpadding='2' class='listagem'>
				<thead>
					<tr>
						<td  width='4%' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Ação</strong></td>
						<td  width='46%' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Cargo</strong></td>
						<td style='display:none;' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Concursos Autorizados</strong></td>
						<td valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Concursos Publicados</strong></td>
					</tr>
				</thead>
			    <tbody>");
//	$sql = "SELECT
//				sum(lepvlrpublicacao) as publicado,
//				ca.crgdsc as cargo,
//				lep.crgid,
//				lep.lepid
//
//			FROM
//				academico.editalportaria ep
//			INNER JOIN
//				academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
//									  					  AND lep.lepstatus = 'A'
//			INNER JOIN
//				academico.cargos AS ca ON (ca.crgid  = lep.crgid)
//			INNER JOIN
//				academico.classes AS cls ON (cls.clsid = ca.clsid)
//			WHERE
//				ep.edpid = $edpid
//				AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
//				AND ep.edpstatus = 'A'
//				--AND ep.edpano = '$ano'
//				AND cls.clsid = $clsid
//			GROUP BY
//				ca.crgdsc,
//				lep.crgid,
//				lep.lepid";
    $sql = "SELECT
				COALESCE(sum(lepvlrpublicacao), 0) as publicado,
				ca.crgdsc as cargo,
				ca.crgid,
				lep.lepid
			FROM
				academico.cargos ca
			JOIN academico.classes cl ON cl.clsid = ca.clsid
			LEFT JOIN academico.lancamentoeditalportaria lep ON lep.edpid = {$edpid}
																AND lep.crgid = ca.crgid
																AND lep.lepstatus = 'A'
			WHERE ca.clsid = {$clsid}
			GROUP BY
				ca.crgdsc,
				ca.crgid,
				lep.lepid";
    $dados = $db->carregar($sql);


    $cont = 0;
    for ($i = 0; $i < count($dados); $i++):
        $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
        $cont++;


        $cargo = $dados[$i]["cargo"];
        $publicado = $dados[$i]["publicado"] ? $dados[$i]["publicado"] : 0;
        $lepid = $dados[$i]["lepid"];
        $crgid = $dados[$i]["crgid"];

        if ($habilitado) {
            $btexcluir = "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">";
            $editavel = "";
        } else {
            $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
            $editavel = "disabled=\"disabled\"";
        }

        //montado linha da tabela com os valores lançados para o cargo
        echo("<tr bgcolor=\"" . $cor . "\" id=\"" . $tabela . "_" . $crgid . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">

				<td align=\"center\">
					$btexcluir
					<input type=\"hidden\" value=\"" . $crgid . "\" name=\"crgid[]\">
				</td>

				<td align=\"left\"align=\"center\">
					{$cargo}
				</td>
				<td align=\"center\">
					<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $crgid . "\"  value=\"" . $publicado . "\">
					<input $editavel id=\"publicado_" . $tabela . "_" . $crgid . "\" class=\"CampoEstilo\"
					onchange=\"valida_lancamento_docentes('" . $tabela . "', '" . $crgid . "');\"
					name=\"publicado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
					onkeyup=\"calculaTotal(this, 'total_pub_" . $tabela . "');\"
					size=\"15\" maxlength=\"15\" value=\"" . $publicado . "\">
				</td>
			</tr>");
    endfor;

    echo("	</tbody>
		   </table>");
}

function criarTabelaHomologado($tabela, $clsid)
{

    global $db, $habilitado;

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $edpid = $_SESSION['academico']['edpid'];
    $edpid_pub = pegaPublicado($edpid);
    $tpetipo = $_SESSION['academico']['tpetipo'];
    $prtid = $_SESSION['academico']['prtid'];
    $prgid = $_SESSION['academico']['prgid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $orgid = $_SESSION["academico"]["orgid"];
    $ano = $_SESSION['academico']['ano'];

    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);
    $select = array();
    $from = array();

    $titulo = "Valor da homologação maior do que de publicação!";
    $img = "<img align='middle' src='/imagens/atencao.png'/ title='{$titulo}' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";

    $sql = "SELECT
				sum(publicado) AS publicado,
				sum(homologado) AS homologado,
				cargo,
				crgid
			FROM (
					SELECT
						sum(lepvlrpublicacao) as publicado,
						0 as homologado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid_pub
						AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid

				UNION ALL

					SELECT
						0 AS publicado,
						sum( lepvlrhomologado ) as homologado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid
						AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid
			) as f
			GROUP BY
				cargo,
				crgid";

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);

    $dados = $db->carregar($sql);

    //total do autorizado para concurso
    $sql_proj = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
					FROM academico.lancamentosportaria lp
					INNER JOIN academico.portarias p ON p.prtid = lp.prtid
					WHERE
					lp.prtid = " . $prtid . " AND
					lp.entidcampus = " . $entidcampus . " AND
					lp.entidentidade = " . $entidentidade . " AND
					lp.clsid =" . $clsid . " AND
					p.prtano = '" . $ano . "' AND
					lp.lnpstatus = 'A'";
    $projetado = $db->pegaUm($sql_proj);

    //total publicado
    $sql_aut_proj = "
					SELECT
					COALESCE (sum(lp.lepvlrpublicacao), 0) as lepvlrpublicacao

					FROM academico.lancamentoeditalportaria lp
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lp.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
					INNER JOIN
						academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_PUBLICACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus AND eppub.edpidhomo IS null)
					INNER JOIN
						academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
					WHERE
					lp.lepstatus = 'A' AND
					eppub.edpstatus = 'A' ";

    $utilizado_projetado = $db->pegaUm($sql_aut_proj);

    //total utilizado para homologação
    $sql_total_homo = "
					SELECT
					COALESCE (sum(lp.lepvlrhomologado), 0) as lepvlrhomologado

					FROM academico.lancamentoeditalportaria lp
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lp.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
					INNER JOIN
						academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus )
					INNER JOIN
						academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
					WHERE
					lp.lepstatus = 'A'
					AND	eppub.edpstatus = 'A'";

    $utilizado_homologacao = $db->pegaUm($sql_total_homo);
    $disponivel_homologacao = $utilizado_projetado - $utilizado_homologacao;

    //montando totalizadores iniciais da tabela
    echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
				<tr>
					<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
					<td>" . $projetado . "</td>
				</tr>
				<tr>
					<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $utilizado_projetado . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Concurso Publicado:</b></td>
					<td id=\"td_disponivel_projetado_" . $tabela . "\">" . $utilizado_projetado . "</td>
				</tr>
				<tr>
					<input type=\"hidden\" id=\"utilizado_homologacao_" . $tabela . "\"  value=\"" . $utilizado_homologacao . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Total Concursos Homologados:</b></td>
					<td id=\"td_utilizado_homologacao_" . $tabela . "\">" . $utilizado_homologacao . "</td>
				</tr>
				<tr>
					<input type=\"hidden\" id=\"disponivel_homologacao_" . $tabela . "\"  value=\"" . $disponivel_homologacao . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Homologação:</b></td>
					<td id=\"td_disponivel_homologacao_" . $tabela . "\">" . $disponivel_homologacao . "</td>
				</tr>
		   </table>
	";
    //montando cabeçalho da tabela de lançamentos
    echo "<table id=\"" . $tabela . "\" width=\"100%\" align=\"left\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
			<thead>
				<tr>
		            <td width=\"4%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Ação</strong></td>
					<td width=\"46%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Cargo</strong></td>
					<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Publicados</strong></td>
					<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Homologados</strong></td>
				</tr>
			</thead>
    		<tbody>";

    if ($dados) {

        if ($habilitado) {
            $btexcluir = "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">";
            $editavel = "";
        } else {
            $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
            $editavel = "disabled=\"disabled\"";
        }

        $cont = 0;
        for ($i = 0; $i < count($dados); $i++) {
            $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
            $cont = $cont + 1;

            $img_src = ($dados[$i]["lepobs"] != '') ? "src=\"/imagens/restricao.png \"" : "src=\"/imagens/pop_p.gif \"";
            $img_alert_homo = ($dados[$i]["homologado"] > $dados[$i]["publicado"]) ? $img : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';


            //recuperando o lepid para o teste de exclusão do lançamento
            $sql_lepid = "SELECT lepid
							FROM academico.lancamentoeditalportaria
								WHERE edpid = $edpid
								AND crgid = " . $dados[$i]["crgid"] . "
								--AND lepano = '$ano'";
            $lepid = $db->pegaUm($sql_lepid);

            echo "<tr bgcolor=\"" . $cor . "\" id=\"" . $tabela . "_" . $dados[$i]["crgid"] . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
						<td align=\"center\">
							$btexcluir
							<input type=\"hidden\" value=\"" . $dados[$i]["crgid"] . "\" name=\"crgid[]\">
						</td>
						<td>
							" . $dados[$i]["cargo"] . "
						</td>
						<td align=\"center\">
							<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\"  value=\"" . $dados[$i]["publicado"] . "\">
							<input style=\"color: #696969\" disabled=\"false\" id=\"publicado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
							name=\"publicado[]\" type=\"text\"
							size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["publicado"] . "\">
						</td>
						<td align=\"center\">
							<input type=\"hidden\" id=\"homologado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\" value=\"" . $dados[$i]["homologado"] . "\">
							<input $editavel id=\"homologado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
							onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
							onkeyup=\"calculaTotal(this, 'total_hom_" . $tabela . "');\"
							name=\"homologado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
							size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["homologado"] . "\">
							" . $img_alert_homo . "
						</td>
					</tr>";
        }
        echo("
				<tr  bgcolor=\"#FFFFFF\" >
					<td align=\"right\"></td>
					<td align=\"right\"><b>Totais</b></td>
					<td  align=\"center\">
							<input style=\"color: #696969\" disabled=\"false\" id='total_pub_" . $tabela . "' class=\"CampoEstilo\"
							 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_publicado"] . "\">
					</td>
					<td align=\"center\">
							<input style=\"color: #696969\" disabled=\"false\" id='total_hom_" . $tabela . "' class=\"CampoEstilo\"
							 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_homologado"] . "\">
							 <span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>
						</td>
				</tr>
			");
    }
    echo "</tbody>
			</table>";

    //-------------------- Calculando os totais
    if ($orgid == ACA_ORGAO_SUPERIOR) {
        echo("<script>calculaTotalInicial( 'projetado_" . $tabela . "','total_proj_" . $tabela . "');</script>");
    }

    echo("<script>calculaTotalInicial( 'publicado_" . $tabela . "','total_pub_" . $tabela . "');</script>");
    echo("<script>calculaTotalInicial( 'homologado_" . $tabela . "','total_hom_" . $tabela . "');</script>");

}


function criarTabelaDocentesHomologado($tabela, $clsid)
{

    global $db, $habilitado;

//	$crgid = 323;

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);

    $edpideditalhomologacao = $_SESSION['academico']['edpideditalhomologacao'];
    $prtid = $_SESSION['academico']['prtid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $edpid = $_SESSION['academico']['edpid'];
    $edpid_pub = pegaPublicado($edpid);
    $tpetipo = $_SESSION["academico"]["tpetipo"];
    $prgid = $_SESSION['academico']['prgid'];
    $ano = $_SESSION['academico']['ano'];
    $select = array();
    $from = array();

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);

    $titulo = "Valor da homologação maior do que de publicação!";
    $img = "<img align='middle' src='/imagens/atencao.png'/ title='{$titulo}' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";

    //obetendo o total projetado
    $sql_proj = "SELECT COALESCE (SUM(lnpvalor), 0) as lnpvalor
		FROM academico.lancamentosportaria
		WHERE
	    prtid = " . $prtid . " AND
	    entidcampus = " . $entidcampus . " AND
	    entidentidade = " . $entidentidade . " AND
	    clsid = " . $clsid . " AND
	    lnpstatus = 'A'";

    $projetado = $db->pegaUm($sql_proj);

    //total utilizado autorizado para concurso

    $sql_aut_proj = "
						SELECT
						COALESCE (sum(lp.lepvlrpublicacao), 0) as lepvlrpublicacao

						FROM academico.lancamentoeditalportaria lp
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lp.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
						INNER JOIN
							academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_PUBLICACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus AND eppub.edpidhomo IS null)
						INNER JOIN
							academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
						WHERE
						lp.lepstatus = 'A' AND
						eppub.edpstatus = 'A'
						--AND ep.edpano = '$ano'";

    $utilizado_projetado = $db->pegaUm($sql_aut_proj);
    $disponivel_projetado = $projetado - $utilizado_projetado;

    //total utilizado para homologação
    $sql_total_homo = "
					SELECT
					COALESCE (sum(lp.lepvlrhomologado), 0) as lepvlrhomologado

					FROM academico.lancamentoeditalportaria lp
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lp.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid and cls.clsid = $clsid)
					INNER JOIN
						academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND eppub.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . " AND eppub.prtid = $prtid AND eppub.entidcampus = $entidcampus )
					INNER JOIN
						academico.portarias AS p ON (p.prtid = eppub.prtid AND p.prgid = $prgid)
					WHERE
					lp.lepstatus = 'A'
					AND	eppub.edpstatus = 'A'";

    $utilizado_homologacao = $db->pegaUm($sql_total_homo);
    $disponivel_homologacao = $utilizado_projetado - $utilizado_homologacao;

    //montando totalizadores iniciais da tabela
    echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
				<tr>
					<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
					<td>" . $projetado . "</td>
				</tr>
				<tr>
					<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $utilizado_projetado . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Concurso Publicado:</b></td>
					<td id=\"td_disponivel_projetado_" . $tabela . "\">" . $utilizado_projetado . "</td>
				</tr>
				<tr>
					<input type=\"hidden\" id=\"utilizado_homologacao_" . $tabela . "\"  value=\"" . $utilizado_homologacao . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Total Concursos Homologados:</b></td>
					<td id=\"td_utilizado_homologacao_" . $tabela . "\">" . $utilizado_homologacao . "</td>
				</tr>
				<tr>
					<input type=\"hidden\" id=\"disponivel_homologacao_" . $tabela . "\"  value=\"" . $disponivel_homologacao . "\">
					<td width=\"20%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Homologação:</b></td>
					<td id=\"td_disponivel_homologacao_" . $tabela . "\">" . $disponivel_homologacao . "</td>
				</tr>
		   </table>
	";

    //montando cabeçalho da tabela
    echo("<table id='" . $tabela . "' width='100%' align='center' border='0' cellspacing='2' cellpadding='2' class='listagem'>
				<thead>
					<tr>
						<td  width='4%' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Ação</strong></td>
						<td  width='46%' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Cargo</strong></td>
						<td valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Concursos Publicados</strong></td>
						<td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Concursos Homologados</strong></td>
					</tr>
				</thead>
			    <tbody>");

    if ($edpid_pub) {

        $sql = "SELECT
					sum(publicado) AS publicado,
					sum(homologado) AS homologado,
					cargo,
					crgid
				FROM (
					SELECT
						COALESCE(sum(lepvlrpublicacao), 0) as publicado,
						0 as homologado,
						ca.crgdsc as cargo,
						ca.crgid
					FROM
						academico.cargos ca
					JOIN academico.classes cl ON cl.clsid = ca.clsid
					LEFT JOIN academico.lancamentoeditalportaria lep ON lep.edpid = {$edpid_pub}
																		AND lep.crgid = ca.crgid
																		AND lep.lepstatus = 'A'
					WHERE ca.clsid = {$clsid}
					GROUP BY
						ca.crgdsc,
						ca.crgid
				/*
						SELECT
							sum(lepvlrpublicacao) as publicado,
							0 as homologado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid_pub
							AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid
				*/
					UNION ALL

						SELECT
							0 AS publicado,
							sum( lepvlrhomologado ) as homologado,
							ca.crgdsc as cargo,
							lep.crgid
						FROM
							academico.editalportaria ep
						INNER JOIN
							academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
												  AND lep.lepstatus = 'A'
						INNER JOIN
							academico.cargos AS ca ON (ca.crgid  = lep.crgid)
						INNER JOIN
							academico.classes AS cls ON (cls.clsid = ca.clsid)
						WHERE
							ep.edpid = $edpid
							AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
							AND ep.edpstatus = 'A'
							--AND ep.edpano = '$ano'
							AND cls.clsid = $clsid
						GROUP BY
							ca.crgdsc,
							lep.crgid

				) as f
				GROUP BY
					cargo,
					crgid";
        $dados = $db->carregar($sql);
    }

    $cont = 0;
    for ($i = 0; $i < count($dados); $i++):
        $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
        $cont++;

        $cargo = $dados[$i]["cargo"];
        $publicado = $dados[$i]["publicado"] ? $dados[$i]["publicado"] : 0;
        $homologado = $dados[$i]["homologado"];
        $crgid = $dados[$i]["crgid"];

        $img_alert_homo = ($homologado > $publicado) ? $img : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';

        //recuperando o lepid para o teste de exclusão do lançamento
        $sql_lepid = "SELECT lepid
			    	FROM academico.lancamentoeditalportaria
		 			WHERE edpid = $edpid
		 			AND crgid = " . $crgid . "
		 			--AND lepano = '$ano'";
        $lepid = $db->pegaUm($sql_lepid);

        if ($habilitado) {
            $btexcluir = "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">";
            $editavel = "";
        } else {
            $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
            $editavel = "disabled=\"disabled\"";
        }

        echo("<tr id=\"" . $tabela . "_" . $crgid . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">

				<td align=\"center\">
					$btexcluir
					<input type=\"hidden\" value=\"" . $crgid . "\" name=\"crgid[]\">
				</td>

				<td align=\"left\"align=\"center\">
					{$cargo}
				</td>
				<td align=\"center\">
					<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $crgid . "\"  value=\"" . $publicado . "\">
					<input style=\"color: #696969\" disabled=\"false\" id=\"publicado_" . $tabela . "_" . $crgid . "\" class=\"CampoEstilo\"
					name=\"publicado[]\" type=\"text\"
					size=\"15\" maxlength=\"15\" value=\"" . $publicado . "\">
				</td>
				<td align=\"center\" >
					<input  type=\"hidden\" id=\"homologado_old_" . $tabela . "_" . $crgid . "\"  value=\"" . $homologado . "\">
					<input $editavel id=\"homologado_" . $tabela . "_" . $crgid . "\" class=\"CampoEstilo\"
					onchange=\"valida_lancamento_docentes('" . $tabela . "', '" . $crgid . "');\"
					name=\"homologado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
					size=\"15\" maxlength=\"15\" value=\"" . $homologado . "\">
					" . $img_alert_homo . "
				</td>
				</tr>");
    endfor;

    echo("</tbody>
			</table>");
}

function criarTabelaEfetivado($tabela, $clsid)
{

    global $db, $habilitado;

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $edpid = $_SESSION['academico']['edpid'];
    $edpid_homo = pegaHomologado($edpid);
    $edpid_pub = pegaPublicado($edpid_homo);
    $edpideditalhomologacao = $_SESSION['academico']['edpideditalhomologacao'];
    $tpetipo = $_SESSION['academico']['tpetipo'];
    $prtid = $_SESSION['academico']['prtid'];
    $prgid = $_SESSION['academico']['prgid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $orgid = $_SESSION["academico"]["orgid"];
    $ano = $_SESSION['academico']['ano'];

    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);

    $select = array();
    $from = array();

    $img_hom = "<img align='middle' src='/imagens/atencao.png'/ title='Valor da homologação maior do que de publicação!' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";
    $img_efe = "<img align='middle' src='/imagens/atencao.png'/ title='Valor da nomeação maior do que de homologação!' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";

    //-------------recuperando o total de efetivado/nomeados e exibindo
    //total concurso
    $sql_concurso = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
						FROM academico.lancamentosportaria lp
						INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
						WHERE
						lp.prtid = " . $prtid . " AND
						lp.entidcampus = " . $entidcampus . " AND
						lp.entidentidade = " . $entidentidade . " AND
						lp.clsid =" . $clsid . " AND
						p.prtano = '" . $ano . "' AND
						p.tprid = " . ACA_TPORTARIA_CONCURSO . " AND
						lp.lnpstatus = 'A'";
    $concurso = $db->pegaUm($sql_concurso);

    //total provimento
    $sql_provimento = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
						FROM academico.lancamentosportaria lp
						INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
						WHERE
						p.prtidautprov = $prtid AND
						lp.entidcampus = " . $entidcampus . " AND
						lp.entidentidade = " . $entidentidade . " AND
						lp.clsid =" . $clsid . " AND
						p.prtano = '" . $ano . "' AND
						p.tprid = " . ACA_TPORTARIA_PROVIMENTO . " AND
						lp.lnpstatus = 'A'";
    $provimento = $db->pegaUm($sql_provimento);

    //total utilizado para efetivação

    $sql_edpis_homo = "SELECT
							ep.edpid
						FROM
							academico.editalportaria ep
						WHERE

							ep.edpidhomo = $edpid_pub";
    $edpis_homo = implode(',', $db->carregarColuna($sql_edpis_homo));


    $sql_nomeado = "
					SELECT
						COALESCE (sum(lp.lepvlrprovefetivados), 0) as lepvlrprovefetivados
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lp ON lp.edpid = ep.edpid
											 AND lp.lepstatus = 'A'
											 --AND ep.edpano = '$ano'
					INNER JOIN
						academico.cargos c on c.crgid = lp.crgid
					WHERE
						c.clsid = " . $clsid . "
						AND edpideditalhomologacao IN ( $edpis_homo )
						AND edpstatus = 'A' ";
    $nomeado = $db->pegaUm($sql_nomeado);

    //disponivel para efetivação
    $disponivel_nomeacao = $provimento - $nomeado;

    $total_nomeado = "<tr>
					   <input type=\"hidden\" id=\"concurso_autorizado_" . $tabela . "\"  value=\"" . $concurso . "\">
					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
					  <td>" . $concurso . "</td>
					  </tr>
					  <input type=\"hidden\" id=\"provimento_autorizado_" . $tabela . "\"  value=\"" . $provimento . "\">
					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Autorizado:</b></td>
					  <td>" . $provimento . "</td>
					  </tr>
					  <tr>
					  <td  width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Efetivado:</b></td>
					  <td id=\"td_provimento_efetivado_" . $tabela . "\">" . $nomeado . "</td>
					  </tr>
					  <tr>
					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Efetivação:</b></td>
					 <td id=\"td_disponivel_efetivacao_" . $tabela . "\">" . $disponivel_nomeacao . "</td>
					 </tr>";
    //--------------------------------------------------------------------

    //lançamentos
    $sql = "SELECT
				sum(publicado) AS publicado,
				sum(homologado) AS homologado,
				sum(efetivado) AS efetivado,
				cargo,
				crgid
			FROM (

					SELECT
						sum(lepvlrpublicacao) as publicado,
						0 as homologado,
						0 AS efetivado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid_pub
						AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid

				UNION ALL

					SELECT
						0 AS publicado,
						sum( lepvlrhomologado ) as homologado,
						0 as efetivado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid_homo
						AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid

				UNION ALL

					SELECT
						0 AS publicado,
						0 as homologado,
						sum(lepvlrprovefetivados) as efetivado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid
						AND ep.tpeid = " . ACA_TPEDITAL_NOMEACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid


			) as f
			GROUP BY
				cargo,
				crgid";
    $dados = $db->carregar($sql);

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);

    $habil = verificaPerfil($perfis);

    //total do autorizado para concurso

    echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
		<thead>
			<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
			<input type=\"hidden\" id=\"td_disponivel_projetado_" . $tabela . "\"  value=\"0\">
			<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $disponivel_projetado . "\">
			" . $total_nomeado . "
		</table>
	";

    echo "<table id=\"" . $tabela . "\" width=\"100%\" align=\"left\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
					<thead>
						<tr>
							<td width=\"4%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Ação</strong></td>
							<td width=\"46%\" valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Cargo</strong></td>
							<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Publicados</strong></td>
							<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Concursos Homologados</strong></td>
							<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;\" ><strong>Provimentos Efetivados</strong></td>
						</tr>
					</thead>
					<tbody>
	";

    if ($dados) {

        if ($habilitado) {
            $btexcluir = "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">";
            $editavel = "";
        } else {
            $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
            $editavel = "disabled=\"disabled\"";
        }

        $cont = 0;
        for ($i = 0; $i < count($dados); $i++) {
            $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
            $cont = $cont + 1;

            $img_alert_homo = ($dados[$i]["homologado"] > $dados[$i]["publicado"]) ? $img_hom : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';
            $img_alert_efe = ($dados[$i]["efetivado"] > $dados[$i]["homologado"]) ? $img_efe : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';
            //recuperando o lepid para o teste de exclusão do lançamento
            $sql_lepid = "SELECT lepid
							FROM academico.lancamentoeditalportaria
							WHERE edpid = $edpid
							AND crgid = " . $dados[$i]["crgid"] . "
							--AND lepano = '$ano'";
            $lepid = $db->pegaUm($sql_lepid);

            echo "<tr bgcolor=\"" . $cor . "\" id=\"" . $tabela . "_" . $dados[$i]["crgid"] . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
					<td align=\"center\">
						$btexcluir
						<input type=\"hidden\" value=\"" . $dados[$i]["crgid"] . "\" name=\"crgid[]\">
					</td>
					<td>
						" . $dados[$i]["cargo"] . "
					</td>
					<td align=\"center\">
						<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\"  value=\"" . $dados[$i]["publicado"] . "\">
						<input  readonly=\"readonly\" style=\"color: #696969\" id=\"publicado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
						name=\"publicado[]\" type=\"text\"
						size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["publicado"] . "\">
					</td>
					<td align=\"center\">
						<input type=\"hidden\" id=\"homologado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\" value=\"" . $dados[$i]["homologado"] . "\">
						<input  readonly=\"readonly\" style=\"color: #696969\" id=\"homologado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
						name=\"homologado[]\" type=\"text\"
						size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["homologado"] . "\">
						" . $img_alert_homo . "
					</td>
					<td   align=\"center\">
						<input type=\"hidden\" id=\"efetivado_old_" . $tabela . "_" . $dados[$i]["crgid"] . "\" value=\"" . $dados[$i]["efetivado"] . "\">
						<input $editavel id=\"efetivado_" . $tabela . "_" . $dados[$i]["crgid"] . "\" class=\"CampoEstilo\"
						onblur=\"valida_lancamento('" . $tabela . "', '" . $dados[$i]["crgid"] . "');\"
						onkeyup=\"calculaTotal(this, 'total_efe_" . $tabela . "');\"
						name=\"efetivado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
						size=\"15\" maxlength=\"15\" value=\"" . $dados[$i]["efetivado"] . "\">
						" . $img_alert_efe . "
					</td>
				</tr>";
        }
        echo("
			<tr  bgcolor=\"#FFFFFF\" >
				<td align=\"right\"></td>
				<td align=\"right\"><b>Totais</b></td>
				<td  align=\"center\">
					<input style=\"color: #696969\" disabled=\"false\" id='total_pub_" . $tabela . "' class=\"CampoEstilo\"
					 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_publicado"] . "\">
				</td>
				<td align=\"center\">
					 <input style=\"color: #696969\" disabled=\"false\" id='total_hom_" . $tabela . "' class=\"CampoEstilo\"
					 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_homologado"] . "\">
					 <span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>
				</td>
				<td  align=\"center\">
					 <input style=\"color: #696969\" disabled=\"false\" id='total_efe_" . $tabela . "' class=\"CampoEstilo\"
					 type=\"text\" 	size=\"15\" maxlength=\"15\" value=\"" . $dados_totais["total_efetivado"] . "\">
					 <span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>
				</td>
			</tr>
			");
    }
    echo "</tbody>
			</table>";

    //-------------------- Calculando os totais
    if ($orgid == ACA_ORGAO_SUPERIOR) {
        echo("<script>calculaTotalInicial( 'projetado_" . $tabela . "','total_proj_" . $tabela . "');</script>");
    }

    echo("<script>calculaTotalInicial( 'publicado_" . $tabela . "','total_pub_" . $tabela . "');</script>");

    echo("<script>calculaTotalInicial( 'homologado_" . $tabela . "','total_hom_" . $tabela . "');</script>");

    echo("<script>calculaTotalInicial( 'efetivado_" . $tabela . "','total_efe_" . $tabela . "');</script>");

}

function criarTabelaDocentesEfetivado($tabela, $clsid)
{

    global $db, $habilitado;

    //$crgid = 323;

    $perfis = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_MECCADASTRO, PERFIL_MECCONSULTAGERAL);

    $habil = verificaPerfil($perfis);

    $edpideditalhomologacao = $_SESSION['academico']['edpideditalhomologacao'];
    $prtid = $_SESSION['academico']['prtid'];
    $entidcampus = $_SESSION['academico']['entidcampus'];
    $edpid = $_SESSION['academico']['edpid'];
    $edpid_homo = pegaHomologado($edpid);
    $edpid_pub = pegaPublicado($edpid_homo);
    $tpetipo = $_SESSION["academico"]["tpetipo"];
    $prgid = $_SESSION['academico']['prgid'];
    $ano = $_SESSION['academico']['ano'];
    $select = array();
    $from = array();

    $autoriazacaoconcursos = new autoriazacaoconcursos();
    $entidentidade = $autoriazacaoconcursos->buscaentidade($entidcampus);

    $img_hom = "<img align='middle' src='/imagens/atencao.png'/ title='Valor da homologação maior do que de publicação!' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";
    $img_efe = "<img align='middle' src='/imagens/atencao.png'/ title='Valor da nomeação maior do que de homologação!' width='18px' height='18px' style='display: float: left;' valign='MIDDLE '>";

    //total concurso
    $sql_concurso = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
						FROM academico.lancamentosportaria lp
						INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
						WHERE
						lp.prtid = " . $prtid . " AND
						lp.entidcampus = " . $entidcampus . " AND
						lp.entidentidade = " . $entidentidade . " AND
						lp.clsid =" . $clsid . " AND
						p.prtano = '" . $ano . "' AND
						p.tprid = " . ACA_TPORTARIA_CONCURSO . " AND
						lp.lnpstatus = 'A'";
    $concurso = $db->pegaUm($sql_concurso);

    //total provimento
    $sql_provimento = "SELECT COALESCE (sum(lp.lnpvalor), 0) as lnpvalor
						FROM academico.lancamentosportaria lp
						INNER JOIN academico.portarias p ON (p.prtid = lp.prtid AND p.prgid = $prgid)
						WHERE
						p.prtidautprov = $prtid AND
						lp.entidcampus = " . $entidcampus . " AND
						lp.entidentidade = " . $entidentidade . " AND
						lp.clsid =" . $clsid . " AND
						p.prtano = '" . $ano . "' AND
						p.tprid = " . ACA_TPORTARIA_PROVIMENTO . " AND
						lp.lnpstatus = 'A'";
    $provimento = $db->pegaUm($sql_provimento);

    //total utilizado para efetivação
    $sql_nomeado = "
					SELECT
						COALESCE (sum(lp.lepvlrprovefetivados), 0) as lepvlrprovefetivados
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lp ON lp.edpid = ep.edpid
											 AND lp.lepstatus = 'A'
											 --AND ep.edpano = '$ano'
					INNER JOIN
						academico.cargos c on c.crgid = lp.crgid
					WHERE
						c.clsid = " . $clsid . "
						AND edpideditalhomologacao = $edpid_homo
						AND edpstatus = 'A' ";

    $nomeado = $db->pegaUm($sql_nomeado);
    //disponivel para efetivação
    $disponivel_nomeacao = $provimento - $nomeado;

    $total_nomeado = "<tr>
					   <input type=\"hidden\" id=\"concurso_autorizado_" . $tabela . "\"  value=\"" . $concurso . "\">
					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Concurso Autorizado:</b></td>
					  <td>" . $concurso . "</td>
					  </tr>
					  <input type=\"hidden\" id=\"provimento_autorizado_" . $tabela . "\"  value=\"" . $provimento . "\">
					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Autorizado:</b></td>
					  <td>" . $provimento . "</td>
					  </tr>
					  <tr>
					  <td  width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Provimento Efetivado:</b></td>
					  <td id=\"td_provimento_efetivado_" . $tabela . "\">" . $nomeado . "</td>
					  </tr>
					  <tr>
					  <td width=\"18%\" align='right' class=\"SubTituloDireita\"><b>Disponível para Efetivação:</b></td>
					 <td id=\"td_disponivel_efetivacao_" . $tabela . "\">" . $disponivel_nomeacao . "</td>
					 </tr>";

    echo "<table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
		<thead>
			<input type=\"hidden\" id=\"td_disponivel_projetado_" . $tabela . "\"  value=\"0\">
			<input type=\"hidden\" id=\"projetado_" . $tabela . "\"  value=\"" . $projetado . "\">
			<input type=\"hidden\" id=\"disponivel_projetado_" . $tabela . "\"  value=\"" . $disponivel_projetado . "\">
			" . $total_nomeado . "
		</table>
	";


    echo("<table id='" . $tabela . "' width='100%' align='center' border='0' cellspacing='2' cellpadding='2' class='listagem'>
			<thead>
				<tr>
					<td  width='4%' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Ação</strong></td>
					<td  width='46%' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Cargo</strong></td>
					<td valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Concursos Publicados</strong></td>
					<td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Concursos Homologados</strong></td>
					<td valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;' ><strong>Provimentos Efetivados</strong></td>
				</tr>
			</thead>
			<tbody>");

    $edpid_homo = pegaHomologado($edpid);
    $edpid_pub = pegaPublicado($edpid_homo);

    $sql = "SELECT
				sum(publicado) AS publicado,
				sum(homologado) AS homologado,
				sum(efetivado) AS efetivado,
				cargo,
				crgid
			FROM (
				SELECT
						COALESCE(sum(lepvlrpublicacao), 0) as publicado,
						0 as homologado,
						0 AS efetivado,
						ca.crgdsc as cargo,
						ca.crgid
					FROM
						academico.cargos ca
					JOIN academico.classes cl ON cl.clsid = ca.clsid
					LEFT JOIN academico.lancamentoeditalportaria lep ON lep.edpid = {$edpid_pub}
																		AND lep.crgid = ca.crgid
																		AND lep.lepstatus = 'A'
					WHERE ca.clsid = {$clsid}
					GROUP BY
						ca.crgdsc,
						ca.crgid


			/*
					SELECT
						sum(lepvlrpublicacao) as publicado,
						0 as homologado,
						0 AS efetivado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid_pub
						AND ep.tpeid = " . ACA_TPEDITAL_PUBLICACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid
			*/
				UNION ALL

					SELECT
						0 AS publicado,
						sum( lepvlrhomologado ) as homologado,
						0 as efetivado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid_homo
						AND ep.tpeid = " . ACA_TPEDITAL_HOMOLOGACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid

				UNION ALL

					SELECT
						0 AS publicado,
						0 as homologado,
						sum(lepvlrprovefetivados) as efetivado,
						ca.crgdsc as cargo,
						lep.crgid
					FROM
						academico.editalportaria ep
					INNER JOIN
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
											  AND lep.lepstatus = 'A'
					INNER JOIN
						academico.cargos AS ca ON (ca.crgid  = lep.crgid)
					INNER JOIN
						academico.classes AS cls ON (cls.clsid = ca.clsid)
					WHERE
						ep.edpid = $edpid
						AND ep.tpeid = " . ACA_TPEDITAL_NOMEACAO . "
						AND ep.edpstatus = 'A'
						--AND ep.edpano = '$ano'
						AND cls.clsid = $clsid
					GROUP BY
						ca.crgdsc,
						lep.crgid
			) as f
			GROUP BY
				cargo,
				crgid";

    $dados = $db->carregar($sql);

    $cont = 0;
    for ($i = 0; $i < count($dados); $i++):
        $cor = ($cont % 2) ? "#e0e0e0" : "#f4f4f4";
        $cont++;

        $publicado = $dados[$i]["publicado"] ? $dados[$i]["publicado"] : 0;
        $homologado = $dados[$i]["homologado"];
        $efetivado = $dados[$i]["efetivado"];
        $crgid = $dados[$i]["crgid"];
        $cargo = $dados[$i]["cargo"];

        $img_alert_homo = ($homologado > $publicado) ? $img_hom : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';
        $img_alert_efe = ($efetivado > $homologado) ? $img_efe : '<span>&nbsp&nbsp&nbsp&nbsp&nbsp</span>';

        //recuperando o lepid para o teste de exclusão do lançamento
        $sql_lepid = "SELECT lepid
			    	FROM academico.lancamentoeditalportaria
		 			WHERE edpid = $edpid
		 			AND crgid = " . $crgid . "
		 			--AND lepano = '$ano'";
        $lepid = $db->pegaUm($sql_lepid);

        if ($habilitado) {
            $btexcluir = "<img  style=\"cursor:pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" onclick=\"excluirLinha(this.parentNode.parentNode.rowIndex, '" . $tabela . "', '" . $lepid . "', '" . $tpetipo . "');\">";
            $editavel = "";
        } else {
            $btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
            $editavel = "disabled=\"disabled\"";
        }

        echo "<tr id=\"" . $tabela . "_" . $crgid . "\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td align=\"center\">
					$btexcluir
					<input type=\"hidden\" value=\"" . $crgid . "\" name=\"crgid[]\">
				</td>

				<td align=\"left\"align=\"center\">
					{$cargo}
				</td>
				<td align=\"center\">
					<input type=\"hidden\" id=\"publicado_old_" . $tabela . "_" . $crgid . "\"  value=\"" . $publicado . "\">
					<input  readonly=\"readonly\" style=\"color: #696969\" id=\"publicado_" . $tabela . "_" . $crgid . "\" class=\"CampoEstilo\"
					name=\"publicado[]\" type=\"text\"
					size=\"15\" maxlength=\"15\" value=\"" . $publicado . "\">
				</td>
				<td align=\"center\" $display>
					<input  type=\"hidden\" id=\"homologado_old_" . $tabela . "_" . $crgid . "\"  value=\"" . $homologado . "\">
					<input  readonly=\"readonly\" style=\"color: #696969\" id=\"homologado_" . $tabela . "_" . $crgid . "\" class=\"CampoEstilo\"
					name=\"homologado[]\" type=\"text\"
					size=\"15\" maxlength=\"15\" value=\"" . $homologado . "\">
					" . $img_alert_homo . "
				</td>
				<td align=\"center\">
					<input type=\"hidden\" id=\"efetivado_old_" . $tabela . "_" . $crgid . "\"  value=\"" . $efetivado . "\">
					<input $editavel id=\"efetivado_" . $tabela . "_" . $crgid . "\" class=\"CampoEstilo\"
					onchange=\"valida_lancamento_docentes('" . $tabela . "', '" . $crgid . "');\"
					name=\"efetivado[]\" type=\"text\" onkeypress=\"return somenteNumeros(event);\"
					size=\"15\" maxlength=\"15\" value=\"" . $efetivado . "\">
					" . $img_alert_efe . "
				</td>
			</tr>";
    endfor;
    echo "	</tbody>
		  </table>";
}

/*
 *************************************************************************************************
 */


// --- REALIZA AS ROTINAS DO CADASTRO/ATUALIZAÇÂO DE EDITAIS ---


/**
 * Enter description here...
 *
 * @param unknown_type $edpid
 * @return unknown
 */
function aca_busca_editais($edpid)
{

    global $db;

    $sql = "SELECT
				*
			FROM
				academico.editalportaria
			WHERE
				edpid = {$edpid} AND edpstatus = 'A'";

    $dados = $db->pegaLinha($sql);
    return $dados;

}

/**
 * Enter description here...
 *
 * @param unknown_type $dados
 */
function aca_cadastra_editais($dados)
{

    global $db;

    //cria as sessões do edital de homologação e Efetivação
    switch ($dados["tpeid"]) {
        case ACA_TPEDITAL_HOMOLOGACAO:
            $_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_HOMOLOGACAO;
            break;
        case ACA_TPEDITAL_NOMEACAO:
            $_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_NOMEACAO;
            break;
        default:
            $_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_PUBLICACAO;
            break;
    }

    if (!empty($dados['edpidhomo'])) {
        $_SESSION["academico"]["edpidhomo"] = $dados['edpidhomo'];
    }

    # Referente a EFETIVAÇÃO
    if (!empty($dados['prtidautprovimento'])) {
        $_SESSION["academico"]["prtidautprovimento"] = $dados['prtidautprovimento'];
    }

    if (!empty($dados['edpideditalhomologacao'])) {
        $_SESSION["academico"]["edpideditalhomologacao"] = $dados['edpideditalhomologacao'];
    }

    # -- Referente a EFETIVAÇÃO

    $dados['edpdtcriacao'] = formata_data_sql($dados['edpdtcriacao']);
    $dados['edpdtpubldiario'] = formata_data_sql($dados['edpdtpubldiario']);

    // Atribui valores nulos aos campos em branco e coloca aspas
    foreach ($dados as $campo => $valor) {
        if (!is_array($dados[$campo])) {
            if ($valor == "") {
                $dados[$campo] = 'NULL';
            } else {
                $dados[$campo] = "'" . pg_escape_string(trim($valor)) . "'";
            }
        }
    }

    $dados['edpideditalhomologacao'] = !empty($dados['edpideditalhomologacao']) ? $dados['edpideditalhomologacao'] : 'NULL';
    $dados['prtidautprovimento'] = !empty($dados['prtidautprovimento']) ? $dados['prtidautprovimento'] : 'NULL';

    // query de inserção dos editais
    $sql = "INSERT INTO
				academico.editalportaria (prtid, entidcampus,
										  entidentidade, usucpf,
										  tpeid, edpnumero,
										  edpdtcriacao, edpdtpubldiario,
										  edpano, edpnumdiario, edpdtinclusao,
										  edpsecaodiario, edpdiariopagina,
										   edpideditalhomologacao, prtidautprovimento,edpidhomo)
			VALUES
				 ({$dados['prtid']}, {$dados['entidcampus']},
				  {$dados['entid']}, {$dados['usucpf']},
				  {$dados['tpeid']}, {$dados['edpnumero']},
				  {$dados['edpdtcriacao']}, {$dados['edpdtpubldiario']},
				  {$_SESSION["academico"]["ano"]}, {$dados['edpnumdiario']}, now(),
				  {$dados['edpsecaodiario']}, {$dados['edpdiariopagina']},
				  {$dados['edpideditalhomologacao']}, {$dados['prtidautprovimento']},
				  {$dados['edpidhomo']} )
				  RETURNING	edpid";


    // executa a query de insert e cria a sessao com o id do edital
    $edpid = $db->pegaUm($sql);

    $_SESSION["academico"]["edpid"] = $edpid;

    $db->commit();
    $db->sucesso('principal/cadedital', '');

}

/**
 * Enter description here...
 *
 * @param unknown_type $dados
 */
function aca_atualiza_editais($dados)
{

    global $db;

    // cria as sessões do edital de homologação e Efetivação
    switch ($dados["tpeid"]) {
        case ACA_TPEDITAL_HOMOLOGACAO:
            $_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_HOMOLOGACAO;
            break;
        case ACA_TPEDITAL_NOMEACAO:
            $_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_NOMEACAO;
            break;
        default:
            $_SESSION["academico"]["tpetipo"] = ACA_TPEDITAL_PUBLICACAO;
            break;
    }

    # Referente a EFETIVAÇÃO
    if (!empty($dados['prtidautprovimento'])) {
        $_SESSION["academico"]["prtidautprovimento"] = $dados['prtidautprovimento'];
    }

    if (!empty($dados['edpideditalhomologacao'])) {
        $_SESSION["academico"]["edpideditalhomologacao"] = $dados['edpideditalhomologacao'];
    }

    # -- Referente a EFETIVAÇÃO

    $dados['edpdtcriacao'] = formata_data_sql($dados['edpdtcriacao']);
    $dados['edpdtpubldiario'] = formata_data_sql($dados['edpdtpubldiario']);

    // Atribui valores nulos aos campos em branco e coloca aspas
    foreach ($dados as $campo => $valor) {
        if (!is_array($dados[$campo])) {
            if ($valor == "") {
                $dados[$campo] = 'NULL';
            } else {
                $dados[$campo] = "'" . pg_escape_string(trim($valor)) . "'";
            }
        }
    }

    $dados['edpideditalhomologacao'] = !empty($dados['edpideditalhomologacao']) ? $dados['edpideditalhomologacao'] : 'NULL';
    $dados['prtidautprovimento'] = !empty($dados['prtidautprovimento']) ? $dados['prtidautprovimento'] : 'NULL';

    $sql = "UPDATE
				academico.editalportaria
			SET
				 edpnumero 			= {$dados['edpnumero']},
				 prtidautprovimento	= {$dados['prtidautprovimento']},
				 edpdtcriacao  		= {$dados['edpdtcriacao']},
				 edpnumdiario 		= {$dados['edpnumdiario']},
				 edpdtpubldiario	= {$dados['edpdtpubldiario']},
				 edpsecaodiario 	= {$dados['edpsecaodiario']},
				 edpdiariopagina 	= {$dados['edpdiariopagina']}
			WHERE
				edpid = {$dados['edpid']}";


    $db->executar($sql);
    $db->commit();
    $db->sucesso('principal/cadedital', '');

}

/**
 * Enter description here...
 *
 * @param unknown_type $edpid
 */
function aca_exclui_editais_homo($edpid)
{

    global $db;
    $ano = $_SESSION["academico"]["ano"];

    $sql = "SELECT edpidhomo
			FROM academico.editalportaria WHERE edpid = {$edpid}";

    $_SESSION["academico"]["edpid"] = $db->pegaUm($sql);

    $sql = "";
    $sql = "SELECT tpeid
			FROM academico.editalportaria WHERE edpid = {$_SESSION["academico"]["edpid"]}";

    $_SESSION["academico"]["tpetipo"] = $db->pegaUm($sql);
    $sql = "SELECT
				tpeid
				FROM
					academico.editalportaria
				WHERE
					edpideditalhomologacao = {$edpid} AND
					edpstatus = 'A'";
    $tem_nomeacao = $db->pegaUm($sql);

    $sql_lancamento = "SELECT
						lepid
						FROM
							academico.lancamentoeditalportaria
						WHERE
							edpid = {$edpid} AND
							--lepano = '{$ano}' AND
							lepstatus = 'A' ";
    $tem_lancamento = $db->pegaUm($sql_lancamento);

    if (!$tem_nomeacao && !$tem_lancamento) {
        $sql = "";
        $sql = "UPDATE academico.editalportaria
				SET edpstatus = 'I'
				WHERE edpid = {$edpid}";

        $db->executar($sql);

        //deletando os lançamentos associados a este edital
        //$sql_lancamentos  = "DELETE FROM academico.lancamentoeditalportaria WHERE edpid = {$edpid}";
        //$db->executar($sql_lancamentos);

        $db->commit();
        $_REQUEST['acao'] = 'C';
        $db->sucesso('principal/cadedital', '');
    } else if ($tem_nomeacao) {

        echo "<script>
					alert('Este registro não pode ser excluido pois possui uma Nomeação vinculada.');
				  	history.go(-1);
			  </script>";
    } else if ($tem_lancamento) {
        echo "<script>
					alert('Este registro não pode ser excluido pois possui um ou mais lançamentos vinculados.');
				  	history.go(-1);
			  </script>";
    }
}

/**
 * Enter description here...
 *
 * @param unknown_type $edpid
 */
function aca_exclui_editais_nomeacao($edpid)
{

    global $db;
    $ano = $_SESSION["academico"]["ano"];
    # Pega os dados para voltar
    $sql = "SELECT edpideditalhomologacao
			FROM academico.editalportaria
			WHERE edpid = {$edpid}";
    $dados = $db->pegaLinha($sql);
    $edpideditalhomologacao = $dados["edpideditalhomologacao"];

    $sql = "SELECT tpeid
			FROM academico.editalportaria
			WHERE edpid = {$dados["edpideditalhomologacao"]}";
    $dados = $db->pegaLinha($sql);
    $tpeid = $dados["tpeid"];


    $sql_lancamento = "SELECT
						lepid
						FROM
							academico.lancamentoeditalportaria
						WHERE
							edpid = {$edpid} AND
							--lepano = '{$ano}' AND
							lepstatus = 'A' ";
    $tem_lancamento = $db->pegaUm($sql_lancamento);

    if (!$tem_lancamento) {
        $sql = "UPDATE academico.editalportaria
			SET edpstatus = 'I'
			WHERE edpid = {$edpid}";
        $db->executar($sql);
        $db->commit();
        $_REQUEST['acao'] = 'H';
        $db->sucesso('principal/cadedital', '&edpid=' . $edpideditalhomologacao . '&tpetipo=' . $tpeid);
    } else {
        echo "<script>
				alert('Este registro não pode ser excluido pois possui um ou mais lançamentos vinculados.');
			  	history.go(-1);
			  </script>";
    }
}

/**
 * Enter description here...
 *
 * @param unknown_type $edpid
 */
function montaListaPortariasProvimentos($prtid)
{
    global $db;
    $sql = "SELECT
			'<center>
			<a href=# onClick=\"vincularPortaria(\''|| p.prtid ||'_'|| p.prtnumero ||'\')\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Vincular\"></a>
			</center>',
			'<center>' ||TO_CHAR(p.prtdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
			'<center>' || tp.tprdsc  ||'</center>' as tipoportaria,
			'<center>' || p.prtid ||'</center>' AS cod,
			'<center>' || p.prtnumero ||'</center>' as numero,
			'<center>' ||  u.usunome ||'</center>' as nome
		FROM academico.portarias AS p
			INNER JOIN academico.orgao o on o.orgid = p.orgid
			INNER JOIN academico.tipoportaria tp ON tp.tprid = p.tprid
			INNER JOIN seguranca.usuario u ON u.usucpf = p.usucpf
		WHERE p.prtstatus = 'A' AND
			p.prtidautprov = " . $prtid;
    $cabecalho = array("Ação",
        "Data Inclusão",
        "Tipo de Portaria",
        "Nº de Controle",
        "Nº Portaria",
        "Nome");
    $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '');
}

/**
 * Enter description here...
 *
 * @param unknown_type $entid
 */
function academico_busca_campus_obras($entid, $tipo = 'naoinauguradas', $lista = 'lista_de_obras')
{

    global $db;

    if ($tipo == 'naoinauguradas') {
        $filtro = " AND obi.obcid IS NOT NULL";
    } else if ($tipo == 'inauguradas') {
        $filtro = " AND obi.obcid IS NULL";
    } else {
        $filtro = "";
    }

    $sql = "SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/{$lista}&acao=A&subAcao=gravarCarga&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
				CASE WHEN e.entnome is not null
				THEN e.entnome ELSE 'Não informado' END as campus,
				'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus cam
			LEFT JOIN
				obras.obrainfraestrutura oi ON oi.entidcampus = cam.entid
			LEFT JOIN
				academico.obrainauguradacampus obi ON oi.obrid = obi.obrid
			INNER JOIN
				entidade.entidade e ON oi.entidcampus = e.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ef.fueid = ea.fueid
			WHERE
				entidunidade = {$entid} AND (oi.obrstatusinauguracao ='N' OR oi.obrstatusinauguracao IS NULL)
				AND obsstatus = 'A' {$filtro}
			group by
				entnome, e.entid
			order by
				e.entnome";

    $cabecalho = array("Ação", "Campus", "");
    $db->monta_lista_simples($sql, $cabecalho, 100, 50, 'N', '100%', 'N');


}

function academico_busca_obras($campus, $tamanho = '100%', $tipo = 'naoinauguradas', $cadastro = false)
{

    global $db;

    if ($tipo == 'naoinauguradas') {
        $filtro = " AND obi.obcid IS NULL";
    } else if ($tipo == 'inauguradas') {
        $filtro = " AND obi.obcid IS NOT NULL";
    } else {
        $filtro = "";
    }

    $complemento = $cadastro ? "'<input type=\"checkbox\" onclick=\"crtobrasinauguradas(this,\''||cam.cmpid||';'||oi.obrid||'\');\" value=\"\" ' || CASE WHEN (SELECT obcid FROM academico.obrainauguradacampus WHERE obrid = oi.obrid AND cmpid = cam.cmpid) IS NULL THEN '' ELSE 'checked' END || '>' as complemento" :
        "case when ao.obrid is null then '' else '<center><img src=\"/imagens/cam_foto.gif\" border=0 title=\"Galeria de fotos\" style=\"cursor:pointer;\" onclick=\"window.open(\'../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid='||oi.obrid||'\',\'imagem\',\'width=850,height=600,resizable=yes\');\"></center>' end as foto";

    $link = $cadastro ? "obrdesc" :
        "'<a href=\"academico.php?modulo=principal/extrato_selecionar&acao=A&entidcampus={$campus}&obrid=' || oi.obrid || '\">' || obrdesc || '</a>' as nome";

    $sql = "SELECT
				{$complemento},
				{$link},
				to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
				to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
				sto.stodesc as situacao,
				CASE WHEN oi.obrdtvistoria is not null THEN to_char(oi.obrdtvistoria, 'DD/MM/YYYY') ELSE to_char(oi.obsdtinclusao, 'DD/MM/YYYY') END as ultimadata,
				(select replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total from obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual
			FROM
				academico.campus cam
			LEFT JOIN
				obras.obrainfraestrutura oi ON oi.entidcampus = cam.entid
			LEFT JOIN
				academico.obrainauguradacampus obi ON oi.obrid = obi.obrid
			INNER JOIN
				entidade.entidade et ON oi.entidunidade = et.entid
			LEFT JOIN
				obras.situacaoobra sto ON oi.stoid = sto.stoid
			LEFT JOIN
				(select obrid from obras.arquivosobra where tpaid = 21 group by obrid) ao ON ao.obrid = oi.obrid
			WHERE
				entidcampus = {$campus} AND
				obsstatus = 'A' {$filtro}
			GROUP BY
				ao.obrid, oi.obrdesc, oi.obrdtinicio,
				oi.obrdttermino, sto.stodesc, oi.obrdtvistoria,
				oi.obsdtinclusao, oi.obrid, cam.cmpid
			ORDER BY
				oi.obrdesc";

    $cabecalho = $cadastro ? array("Ação", "Nome da Obra", "Data de Início", "Data de Término", "Última Atualização", "Situação da Obra", "% Executado") :
        array("Fotos", "Nome da Obra", "Data de Início", "Data de Término", "Última Atualização", "Situação da Obra", "% Executado");
    $db->monta_lista_simples($sql, $cabecalho, 100, 50, 'N', $tamanho, 'N');


}

/**
 * Verifica se o sistema está bloqueado para um determinado tipo de
 * ensino
 *
 * @param integer $orgid
 * @return integer $dados
 *
 */
function academico_sistema_bloqueado($orgid)
{

    global $db;

    $sql = "SELECT
				blsid,
				trim(blsmotivo) as blsmotivo
			FROM
				academico.bloqueiosistema
			WHERE
				orgid = {$orgid} AND blsstatus = 'A'";

    $dados = $db->pegaLinha($sql);

    return $dados;

}

function academico_mensagem_bloqueio($orgid)
{

    global $db;

    $bloqueado = academico_sistema_bloqueado($orgid);
    $autorizado = academico_unidades_autorizadas();

    if (!$autorizado) {
        if ($bloqueado && (academico_possui_perfil(PERFIL_IFESCONSULTA) || academico_possui_perfil(PERFIL_IFESCADASTRO) ||
                academico_possui_perfil(PERFIL_IFESPERFIL_IFESAPROVACAO)) && !$db->testa_superuser()
        ) {

            ?>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
                <tr>
                    <td align="center" style="color:#FF0000">
                        Sistema Bloqueado Para Edição <br/>
                        <b>Motivo:</b> <?php echo $bloqueado['blsmotivo']; ?>
                    </td>
                </tr>
            </table>
            <?php
            return true;
        }
    } else {
        return false;
    }

}

function academico_unidades_autorizadas()
{

    global $db;
    static $unidades = null;

    if ($unidades === null) {
        if ($db->testa_superuser() || academico_possui_perfil_sem_vinculo()) {

            // pega todas as unidades
            $sql = "
				SELECT
					e.entid
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON e.entid = ef.entid
				WHERE
					ef.funid IN (12,11,14,102)";

        } else {

            // pega as unidades do perfil do usuário
            $sql = "
				SELECT
					ur.entid
				FROM
					academico.usuarioresponsabilidade ur
				INNER JOIN
					academico.autorizacaoentidade ae ON ae.entid = ur.entid
				INNER JOIN
					seguranca.perfil p ON
					p.pflcod = ur.pflcod
				INNER JOIN
					seguranca.perfilusuario pu ON
					pu.pflcod = ur.pflcod AND
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A' AND
					p.sisid = 56";
        }

        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();
        $unidades = array();

        foreach ($dados as $linha) {
            array_push($unidades, $linha['entid']);
        }
    }
    return $unidades;
}

function academico_retira_autorizacao()
{

    global $db;

    $hoje = date('d/m/Y');

    $sql = "SELECT autid, to_char(autdttermino, 'DD/MM/YYYY') as data
			FROM academico.autorizacaoespecial
			WHERE orgid = {$_SESSION["academico"]["orgid"]} AND
				  autstatus = 'A'";

    $data = $db->carregar($sql);

    for ($k = 0; $k < count($data); $k++) {

        if ($hoje == $data[$k]['data']) {

            $sql = "UPDATE academico.autorizacaoespecial
					SET autstatus = 'I'
					WHERE autid = {$data[$k]['autid']}";

            $db->executar($sql);

            $sql = "DELETE FROM academico.autorizacaoentidade
					WHERE autid = {$data[$k]['autid']}";

            $db->executar($sql);
            $db->commit();

        }
    }

}


/**
 * ******************* Funções que foram migradas do SIG ***********
 */
function filtrarcampus($dados)
{
    global $db;
    $normatiza = array('campus' => 'cam.cmpid = ',
        'orgao' => 'tpe.orgid = ',
        'uf' => 'en.estuf = ',
        'unidade' => 'ea.entid = ');

    foreach ($dados as $campo => $valor) {
        $where[] = $normatiza[$campo] . "'" . $valor . "'";
    }
    $sql = "
	 SELECT cam.cmpid FROM academico.campus cam
	 LEFT JOIN entidade.entidade e ON e.entid = cam.entid
	 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid
	 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
	 LEFT JOIN entidade.entidade ea ON fea.entid = ea.entid
	 LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = ea.entid
	 LEFT JOIN academico.orgaouo teo ON teo.funid = fen2.funid
	 LEFT JOIN academico.orgao tpe ON teo.orgid = tpe.orgid
	 LEFT JOIN entidade.entidadeendereco ee ON e.entid = ee.entid
	 INNER JOIN entidade.endereco en ON en.endid = ee.endid

	 WHERE " . implode(" AND ", $where);

    $cmpids = $db->carregar($sql);
    return $cmpids;
}


function academico_salvarRegistroEntidade($dados)
{
    global $db;

    $acao = $_SESSION["academico"]["entidadenivel"] == "unidade" ? 'A' : 'C';

    $entidade = new Entidades();
    $entidade->carregarEntidade($dados);
    $entidade->salvar();

    echo '<script type="text/javascript">
    		alert("Dados gravados com sucesso");
		    window.location = \'academico.php?modulo=principal/inserir_entidade&acao=' . $acao . '\';
	      </script>';
    exit;
}

function removernotatecnica($dados)
{
    global $db;

    $sql = "SELECT arqid FROM academico.notatecnica WHERE ntcid = '" . $dados['ntcid'] . "'";
    $arqid = $db->pegaUm($sql);
    //deletando nota técnica
    $sql = "DELETE FROM academico.notatecnica WHERE ntcid='" . $dados['ntcid'] . "'";
    $db->executar($sql);
    //deletando pdf em public.arquivo
    if ($arqid) {
        $sql = "DELETE FROM public.arquivo WHERE arqid = '$arqid'";
        $db->executar($sql);
    }
    $db->commit();
    //deletando o arquivo pdf físico do servidor
    if ($arqid) {
        $caminho = APPRAIZ . 'arquivos/' . $_SESSION['sisdiretorio'] . '/' . floor($arqid / 1000) . '/' . $arqid;

        if (file_exists($caminho)) {
            unlink($caminho);
        }
    }
    echo "<script>
			alert('Nota técnica removida com sucesso');
			window.location = '?modulo=principal/notatecnica&acao={$dados["acao"]}';
		  </script>";

}

function salvarProcessoSeletivo($dados)
{
    global $db;
    /*
	 * prsnrvagas - apesar do javascript esta com a mascara, estava chegando número com "."(ponto), isto ocasionava erros. Estou filtrando via PHP, para confirmar a inserção
	 */
    if ($dados['prsid']) {

        $vagas = $dados['prsnrvagas'] ? "'" . str_replace(array(".", ","), array("", ""), $dados['prsnrvagas']) . "'" : "NULL";

        $sql = "UPDATE academico.processoseletivo
				SET prsinscricaofim='" . formata_data_sql($dados['prsinscricaofim']) . "',
   				prsinscricaoini='" . formata_data_sql($dados['prsinscricaoini']) . "',
   				prsprovaini='" . formata_data_sql($dados['prsprovaini']) . "',
       			prsprovafim='" . formata_data_sql($dados['prsprovafim']) . "',
       			prsinicioaula='" . formata_data_sql($dados['prsinicioaula']) . "',
       			prsnrvagas=" . $vagas . ",
       			prsobservacao='" . $dados['prsobservacao'] . "'
       			WHERE prsid='" . $dados['prsid'] . "'";

        $db->executar($sql);
    } elseif ($dados['rmprsid']) {
        $db->executar("DELETE FROM academico.processoseletivo WHERE prsid='" . $dados['rmprsid'] . "'");
    } else {
        $cmpid = $db->pegaUm("SELECT cmpid FROM academico.campus WHERE entid = '" . $_SESSION['sig_var']['entid'] . "'");
        if( !empty($cmpid) ){
            $sql = "INSERT INTO academico.processoseletivo(
                    cmpid, prsinscricaofim, prsinscricaoini, prsprovaini,
                            prsprovafim, prsinicioaula, prsobservacao, prsnrvagas)
                              VALUES ('" . $cmpid . "',
                                      '" . formata_data_sql($dados['prsinscricaofim']) . "',
                                      '" . formata_data_sql($dados['prsinscricaoini']) . "',
                                      '" . formata_data_sql($dados['prsprovaini']) . "',
                              '" . formata_data_sql($dados['prsprovafim']) . "',
                              '" . formata_data_sql($dados['prsinicioaula']) . "',
                              '" . $dados['prsobservacao'] . "',
                              " . (($dados['prsnrvagas']) ? "'" . str_replace(array(".", ","), array("", ""), $dados['prsnrvagas']) . "'" : "NULL") . ");";
            $db->executar($sql);
        }else{
                echo '<script type="text/javascript">
                            alert("Recuperando dados da sessão");
                            window.location = \'?modulo=inicio&acao=C\';
                      </script>';
                exit;
        }
    }
    $db->commit();
    echo '<script type="text/javascript">
    		alert("Operação realizada com sucesso!");
	        window.location = \'?modulo=principal/processoSeletivo&acao=A&page=esp\';
	      </script>';
    exit;
}


function academico_ordenaritens($dados)
{
    global $db;
    $sql = "SELECT tei.teiordem FROM academico.orgaoitem tei WHERE tei.itmid = '" . $dados['itematual'] . "' AND tei.orgid = '" . $dados['orgid'] . "'";
    $ordematual = $db->pegaUm($sql);
    $sql = "SELECT tei.teiordem FROM academico.orgaoitem tei WHERE tei.itmid = '" . $dados['itemir'] . "' AND tei.orgid = '" . $dados['orgid'] . "'";
    $ordemir = $db->pegaUm($sql);
    if ($ordemir) {
        $sql = "UPDATE academico.orgaoitem SET teiordem = '" . $ordemir . "' WHERE itmid = '" . $dados['itematual'] . "' AND orgid = '" . $dados['orgid'] . "'";
        $db->executar($sql);
    }
    if ($ordematual) {
        $sql = "UPDATE academico.orgaoitem SET teiordem = '" . $ordematual . "' WHERE itmid = '" . $dados['itemir'] . "' AND orgid = '" . $dados['orgid'] . "'";
        $db->executar($sql);
    }
    $db->commit();
    exit;
}

function academico_listaitens($dados)
{
    global $db;
    $sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cadastraritens&acao=E&itmid=' || itm.itmid || '&orgid=' || tei.orgid || '\'\");\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"academico_Excluir(\'?modulo=principal/cadastraritens&acao=R&alterabd=R&orgid=' || tei.orgid || '&itmid=' || itm.itmid || '\',\'Deseja realmente excluir este item?\');\">' AS acao, itmdsc, itm.itmid, itm.itmobs, itm.itmglobal
			FROM academico.item itm
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
			WHERE tei.orgid = '" . (($dados['orgid']) ? $dados['orgid'] : TIPOENSINO_DEFAULT) . "' AND itm.itmglobal = false AND itm.itmtcu = false
			ORDER BY tei.teiordem";
    $dadositens = $db->carregar($sql);

    ?>
    <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
        <thead>
        <tr>
            <td colspan="4" align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                <strong>Itens por ano</strong></td>
        </tr>
        <tr>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Ações
            </td>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Item
            </td>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Ordem
            </td>
        </tr>
        </thead>
        <tbody>
        <?php
        if ($dadositens) {
            $i = 0;
            foreach ($dadositens as $item) {
                unset($setas);
                if ($i == 0) {
                    $setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i + 1)]['itmid'] . ");' border='0' title='Descer'>";
                } elseif ($i == (count($dadositens) - 1)) {
                    $setas = "<img src='/imagens/seta_cima.gif' border='0' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i - 1)]['itmid'] . ");' style='cursor:pointer;' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
                } elseif (count($dadositens) === 1) {
                    $setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
                } else {
                    $setas = "<img src='/imagens/seta_cima.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i - 1)]['itmid'] . ");' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i + 1)]['itmid'] . ");' border='0' title='Descer'>";
                }
                ?>
                <tr<? echo(($i % 2) ? 'bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';"' : 'bgcolor="#F7F7F7" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#F7F7F7\'"'); ?>>
                <td title="Ações"><? echo $item['acao']; ?></td>
                <td title="<?= $item['itmobs'] ?>"><? echo $item['itmdsc'] ?></td>
                <td title="Ordem" align="center"><? echo $setas; ?></td>
                </tr>
                <?php $i++;
            }
        } else {
            ?>
            <tr>
                <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
            </tr><?php
        }

        $sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cadastraritens&acao=E&itmid=' || itm.itmid || '&orgid=' || tei.orgid || '\'\");\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"academico_Excluir(\'?modulo=principal/cadastraritens&acao=R&alterabd=R&orgid=' || tei.orgid || '&itmid=' || itm.itmid || '\',\'Deseja realmente excluir este item?\');\">' AS acao, itmdsc, itm.itmid, itm.itmobs, itm.itmglobal
			FROM academico.item itm
			LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
			WHERE tei.orgid = '" . (($dados['orgid']) ? $dados['orgid'] : TIPOENSINO_DEFAULT) . "'AND itm.itmglobal = true
			ORDER BY tei.teiordem";
        $dadositens = $db->carregar($sql);

        ?>
        <thead>
        <tr>
            <td colspan="4" align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                <strong>Itens globais</strong></td>
        </tr>
        <tr>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Ações
            </td>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Item
            </td>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Ordem
            </td>
        </tr>
        </thead>
        <tbody>
        <?php
        if ($dadositens) {
            $i = 0;
            foreach ($dadositens as $item) {
                unset($setas);
                if ($i == 0) {
                    $setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i + 1)]['itmid'] . ");' border='0' title='Descer'>";
                } elseif ($i == (count($dadositens) - 1)) {
                    $setas = "<img src='/imagens/seta_cima.gif' border='0' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i - 1)]['itmid'] . ");' style='cursor:pointer;' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
                } elseif (count($dadositens) === 1) {
                    $setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
                } else {
                    $setas = "<img src='/imagens/seta_cima.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i - 1)]['itmid'] . ");' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i + 1)]['itmid'] . ");' border='0' title='Descer'>";
                }
                ?>
                <tr<?php echo(($i % 2) ? 'bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';"' : 'bgcolor="#F7F7F7" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#F7F7F7\'"'); ?>>
                <td title="Ações"><? echo $item['acao']; ?></td>
                <td title="<?= $item['itmobs'] ?>"><? echo $item['itmdsc'] ?></td>
                <td title="Ordem" align="center"><? echo $setas; ?></td>
                </tr>
                <?php $i++;
            }
        } else {
            ?>
            <tr>
                <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
            </tr><?php
        }

        if ($dados['orgid'] == 2){
        $sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cadastraritens&acao=E&itmid=' || itm.itmid || '&orgid=' || tei.orgid || '\'\" > <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"academico_Excluir(\'?modulo=principal/cadastraritens&acao=R&alterabd=R&orgid=' || tei.orgid || '&itmid=' || itm.itmid || '\',\'Deseja realmente excluir este item?\');\">' AS acao, itmdsc, itm.itmid, itm.itmobs, itm.itmglobal
				FROM academico.item itm
				LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
				WHERE tei.orgid = '" . (($dados['orgid']) ? $dados['orgid'] : TIPOENSINO_DEFAULT) . "' AND itm.itmtcu = true
				ORDER BY tei.teiordem";

        $dadositens = $db->carregar($sql);

        ?>
        <thead>
        <tr>
            <td colspan="4" align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                <strong>Itens TCU</strong></td>
        </tr>
        <tr>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Ações
            </td>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Item
            </td>
            <td align="center" valign="top" class="title"
                style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                Ordem
            </td>
        </tr>
        </thead>
        <tbody>
        <?php
        if ($dadositens) {
            $i = 0;
            foreach ($dadositens as $item) {
                unset($setas);
                if ($i == 0) {
                    $setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i + 1)]['itmid'] . ");' border='0' title='Descer'>";
                } elseif ($i == (count($dadositens) - 1)) {
                    $setas = "<img src='/imagens/seta_cima.gif' border='0' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i - 1)]['itmid'] . ");' style='cursor:pointer;' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
                } elseif (count($dadositens) === 1) {
                    $setas = "<img src='/imagens/seta_cimad.gif' border='0' title='Subir'> <img src='/imagens/seta_baixod.gif' border='0' title='Descer'>";
                } else {
                    $setas = "<img src='/imagens/seta_cima.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i - 1)]['itmid'] . ");' border='0' title='Subir'> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' onclick='ordenaritens(" . $item['itmid'] . "," . $dadositens[($i + 1)]['itmid'] . ");' border='0' title='Descer'>";
                }
                ?>
                <tr<?php echo(($i % 2) ? 'bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';"' : 'bgcolor="#F7F7F7" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#F7F7F7\'"'); ?>>
                <td title="Ações"><?php echo $item['acao']; ?></td>
                <td title="<?= $item['itmobs'] ?>"><?php echo $item['itmdsc'] ?></td>
                <td title="Ordem" align="center"><?php echo $setas; ?></td>
                </tr>
                <?php $i++;
            }
        } else {
            ?>
            <tr>
                <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
            </tr><?php
        }
        }
        ?>
    </table>
    <?php
    exit;
}

function academico_salvarObrasInauguradas($dados)
{
    global $db;
    $dadosobrinau = explode(";", $dados['param']);
    $db->executar("DELETE FROM academico.obrainauguradacampus WHERE cmpid='" . $dadosobrinau[0] . "' AND obrid='" . $dadosobrinau[1] . "'");
    $db->executar("INSERT INTO academico.obrainauguradacampus(cmpid, obrid) VALUES ('" . $dadosobrinau[0] . "', '" . $dadosobrinau[1] . "');");
    $db->commit();
    exit;
}

function academico_removerObrasInauguradas($dados)
{
    global $db;
    $dadosobrinau = explode(";", $dados['param']);
    $db->executar("DELETE FROM academico.obrainauguradacampus WHERE cmpid='" . $dadosobrinau[0] . "' AND obrid='" . $dadosobrinau[1] . "'");
    $db->commit();
    exit;
}

function academico_monta_cabecalho_sig($entid, $titulo_modulo = '')
{
    global $db;

    monta_titulo($titulo_modulo, '');


    // mensagem de bloqueio
    //$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);


    if ($_SESSION['sig_var']['iscampus'] == 'sim') {
        $sql = "SELECT ent.entnome as campus, ende.estuf, mundescricao, orgdesc, uo.entnome AS unidadeorc, uo.entid as unidadeorcid, tpe.orgid FROM entidade.entidade ent
				inner JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
				inner JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				inner JOIN entidade.entidade uo ON uo.entid = fea.entid
				inner JOIN entidade.funcaoentidade fen2 ON fen2.entid = uo.entid
				inner JOIN academico.orgaouo teu ON teu.funid = fen2.funid
				inner JOIN academico.orgao tpe ON tpe.orgid = teu.orgid
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
				WHERE ent.entid = '" . $entid . "' ORDER BY ent.entnome";
    } else {
        $sql = "SELECT ent.entid as unidadeorcid, ent.entnome as unidadeorc, ende.estuf, mundescricao, orgdesc, tpe.orgid FROM entidade.entidade ent
				inner JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
				inner JOIN academico.orgaouo teu ON teu.funid = fen.funid
				inner JOIN academico.orgao tpe ON tpe.orgid = teu.orgid
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
				WHERE ent.entid = '" . $entid . "' ORDER BY ent.entnome";
    }

    $dadosentidade = $db->pegaLinha($sql);

    #USADO PELO MONITORAMENTO DE PROGRAMAS E AÇÕES -> DEMANDA/INFRAESTRUTURA -> DEMANDA/INFRA ESTRUTURA SELEÇÃO DE OBRAS. PARA EXIBIÇÃO DO NOME.
    $_SESSION['MPA']['nome_univercidade'] = $dadosentidade['unidadeorc'];

    if ($dadosentidade && $dadosentidade['orgdesc']) {
        echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
        echo "<tr>";
        echo "<td class='SubTituloDireita'>Tipo Ensino :</td><td>" . $dadosentidade['orgdesc'] . "</td>";
        echo "</tr>";
        echo "<tr>";
        echo "<td class='SubTituloDireita'>Instituição :</td><td><a style=\"cursor:pointer;\" onclick=\"window.location='?modulo=principal/editarentidade&acao=A&iscampus=nao&orgid=" . $dadosentidade['orgid'] . "&entid=" . $dadosentidade['unidadeorcid'] . "';\"><img src=\"../imagens/consultar.gif\" border=\"0\"> " . $dadosentidade['unidadeorc'] . "</a></td>";
        echo "</tr>";
        if ($_SESSION['sig_var']['iscampus'] == 'sim') {
            echo "<tr>";
            echo "<td class='SubTituloDireita'>Campus / Uned :</td><td>" . $dadosentidade['campus'] . "</td>";
            echo "</tr>";
        }
        echo "<tr>";
        echo "<td class='SubTituloDireita'>UF / Munícipio :</td><td>" . $dadosentidade['estuf'] . " / " . $dadosentidade['mundescricao'] . "</td>";
        echo "</tr>";
        echo "</table>";
    } else {
        die("<script>
				alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
				window.location='?modulo=inicio&acao=C';
			 </script>");
    }
}


/************************
 *
 * FUNÇÕES VINDAS DO SIG
 *
 */

function inserircampuscurso($dados)
{
    global $db, $anosanalisados;
    if ($anosanalisados[$dados['orgid']]) {
        $anos = $anosanalisados[$dados['orgid']];
    } else {
        $anos = $anosanalisados['default'];
    }
    foreach ($anos as $ano) {
        // Validar para naão deixar o sistema inserir arquivo duplicados
        $cpcid = $db->pegaUm("SELECT cpcid FROM academico.campuscurso WHERE curid='" . $dados['curid'] . "' AND cmpid='" . $dados['cmpid'] . "' AND cpcano='" . $ano . "' AND cpcprevisto = {$dados['cpcprevisto']}");
        if (!$cpcid) {
            $sql = "INSERT INTO academico.campuscurso(
    	   			curid, cmpid, cpcano, cpcqtd, cpcprevisto)
	 				VALUES ('" . $dados['curid'] . "','" . $dados['cmpid'] . "','" . $ano . "',NULL,{$dados['cpcprevisto']});";
            echo $sql . "<br />";
            $db->executar($sql);
        }
    }
    $db->commit();
    exit;
}

function removercampuscurso($dados)
{
    global $db;
    $sql = "DELETE FROM academico.campuscurso WHERE cmpid='" . $dados['cmpid'] . "' AND curid='" . $dados['curid'] . "' AND cpcprevisto = {$dados['cpcprevisto']}";
    $db->executar($sql);
    $db->commit();
    exit;
}

function carregaCursoDescricao($dados)
{
    header('content-type: text/html; charset=UTF-8');
    global $db;
    echo campo_texto('curdsc', 'N', 'S', '', 30, 40, '', '', "", "", "", "id='curdsc_{$dados['curid']}'", "", $dados['curdsc']);
}

function salvarNovoCurso($dados)
{
    global $db;

    $sql = "insert into public.curso ( tpcid, curdsc, entid, turid,  curstatus  )
                    values ( {$dados['tpcid']} , '{$dados['curdsc']}' , '{$dados['entid']}' , {$dados['turid']} , 'A' )";

    if ($db->executar($sql)) {
        $db->commit($sql);
        echo "<script>alert('Operação realizada com sucesso!')</script>";
    } else {
        echo "<script>alert('Ocorreu um erro ao tentar gravar os dados, tente novamente mais tarde!')</script>";
    }
}


function buscarDadosEditarCurso($dados)
{
    global $db;

    $curid = $dados['curid'];

    $sql = "
        SELECT c.curid, trim(c.curdsc) as curdsc, c.tpcid, t.turid, turdsc
        FROM public.curso c
        LEFT JOIN public.turno t ON t.turid = c.turid
        WHERE curid = " . $curid . " AND c.curstatus='A'
        ORDER BY c.curdsc
    ";

    $dados = $db->pegaLinha($sql);

    $dados["curdsc"] = iconv("ISO-8859-1", "UTF-8", $dados["curdsc"]);

    echo simec_json_encode($dados);
    die;
}

function excluirCurso($dados)
{
    global $db;

    $sql = "update public.curso set curstatus = 'I' where curid = {$dados['curid']}";

    if ($db->executar($sql)) {
        $db->commit($sql);
        die ("ok");
    } else {
        echo "erro";
    }
}

function carregaCursoTurno($dados)
{
    global $db;
    echo "<div style=\"white-space: nowrap; \" >";
    $turnid = $db->pegaUm("select turid from public.turno where turdsc like '{$dados['turdsc']}'");
    $sql = "select turid as codigo, turdsc as descricao from public.turno";
    echo $db->monta_combo("turid", $sql, 'S', 'Selecione', '', '', '', '200', 'N', "turid_{$dados['curid']}", '', $turnid);
    echo " <input type=\"button\" value=\"ok\" onclick=\"salvarEdicaoCurso({$dados['curid']})\" >";
    echo "</div>";
}

function salvaCursoDescricao($dados = array())
{
    header('content-type: text/html; charset=UTF-8');
    global $db;
    if ($dados['curdsc'] && $dados['curid']) {

        $sql = "update public.curso set curdsc = '" . utf8_decode($dados['curdsc']) . "' where curid = {$dados['curid']}";
        $db->executar($sql);
        $db->commit($sql);
        echo $dados['curdsc'];
    } else
        carregaCursoDescricao($dados);

}

function salvaCursoTurno($dados = array())
{
    global $db;

    if ($dados['turid'] && $dados['curid']) {

        $sql = "update
					public.curso
				set
					turid = '{$dados['turid']}'
				where
					curid = {$dados['curid']};
				select
					turdsc
				from
					public.turno
				where
					turid = {$dados['turid']};";
        echo $db->pegaUm($sql);
        $db->commit($sql);
    } else
        carregaCursoTurno($dados);

}

function exibeInserirNovoCurso($dados = array())
{
    global $db;

    $sql = "SELECT tpcedicao FROM public.tipocurso WHERE tpcid = {$dados['tpcid']} ";
    $edicao = $db->pegaUm($sql);

    if ($edicao == "t") {
        echo "true";
    } else {
        echo "false";
    }
    exit;

}

function listarcursos($dados)
{
    global $db;

    $tpc = $db->pegaLinha("
            SELECT  tpcpossuientidade,
                    tpcedicao
            FROM public.tipocurso
            WHERE tpcid='" . $dados['tpcid'] . "'
        ");

    if ($db->testa_superuser() || academico_possui_perfil(PERFIL_ADMINISTRADOR))
        $habil = true;
    else
        $habil = false;

    if ($tpc['tpcedicao'] == "t" && $habil) {
        //$sqlEdicao = " <img src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" id=\"img_edit_' || curid || '\" onclick=\"editarCurso(this)\" /> <img id=\"img_delete_' || curid || '\" src=\"../imagens/excluir.gif\" style=\"cursor:pointer\"  onclick=\"excluirCurso(this)\" /> ";
    }

    if ($tpc['tpcpossuientidade'] == "t") {

        $entid = $db->pegaUm("SELECT entid FROM academico.campus WHERE cmpid='" . $dados['cmpid'] . "'");
        $sql = "
                SELECT  '<div style=\"white-space: nowrap; \" >
                            <input type=\"checkbox\" id=\"ckc_' || curid || '\" value=\"'|| curid ||'\" onclick=\"crtcursos(this);\" ' ||
                                CASE WHEN (SELECT cc1.curid
                                           FROM academico.campuscurso cc1
                                           WHERE cc1.curid = cc2.curid AND cc1.cmpid ='" . $dados['cmpid'] . "' AND cc1.cpcprevisto = {$dados['cpcprevisto']}
                                           GROUP BY cc1.curid) IS NULL
                                     THEN ''
                                     ELSE 'checked'
                                END || '> $sqlEdicao
                        </div>' as codigo,

                        curdsc as curso,
                        turdsc
                FROM public.curso cc2
                LEFT JOIN public.turno tur ON tur.turid = cc2.turid
                WHERE tpcid = '" . $dados['tpcid'] . "' AND entid='" . $entid . "' AND cc2.curstatus='A'
                ORDER BY curdsc
            ";
        $cabecalho = array("", "Cursos", "Turno");

    } else {

        $sql = "
                SELECT '<div style=\"white-space: nowrap; \" >
                            <input type=\"checkbox\" id=\"ckc_' || curid || '\" value=\"'|| curid ||'\" onclick=\"crtcursos(this);\" '||
                                CASE WHEN (SELECT cc1.curid FROM academico.campuscurso cc1 WHERE cc1.curid = cc2.curid AND cc1.cmpid ='" . $dados['cmpid'] . "' GROUP BY cc1.curid) IS NULL
                                        THEN ''
                                        ELSE 'checked'
                                END ||'> $sqlEdicao
                        </div>' as codigo,
                        curdsc as curso
                FROM public.curso cc2
                WHERE tpcid = '" . $dados['tpcid'] . "' AND cc2.curstatus='A'
                ORDER BY curdsc
            ";
        $cabecalho = array("", "Cursos");

        $sql = "SELECT '<div style=\"white-space: nowrap; \" ><input type=\"checkbox\" id=\"ckc_' || curid || '\" value=\"'|| curid ||'\" onclick=\"crtcursos(this);\" '|| CASE WHEN (SELECT cc1.curid FROM academico.campuscurso cc1 WHERE " . ($dados['curdsc'] != '' ? 'curdsc ilike\'%' . $dados['curdsc'] . '%\' AND ' : '') . " cc1.curid = cc2.curid AND cc1.cmpid ='" . $dados['cmpid'] . "' GROUP BY cc1.curid) IS NULL THEN '' ELSE 'checked' END ||'> $sqlEdicao </div>' as codigo, curdsc as curso FROM public.curso cc2 WHERE " . ($dados['curdsc'] != '' ? 'curdsc ilike\'%' . $dados['curdsc'] . '%\' AND ' : '') . " tpcid = '" . $dados['tpcid'] . "' AND cc2.curstatus='A' ORDER BY curdsc";
        $cabecalho = array("", "Cursos");
    }

    //ver($sql,d);
    $db->monta_lista_simples($sql, $cabecalho, 1000, 10, 'N', '100%', 'N');
    exit;
}

function processaInsercaoCampusItem($dados = array())
{
    /* Montando um array com os indices das variaveis
	 * para identificar qual o indice contem contem as váriaiveis para serem salvas
	 */
    $indicestodos = array_keys($_POST);
    // Verifica se existe itens no array
    if ($indicestodos) {
        // Varrendo os indices

        foreach ($indicestodos as $ind) {
            // Verifica se o indice tem o termo "gravacampo_"
            $iscampogravacao = strpos($ind, 'gravacaocampo_');
            // Se tiver, executar o procedimento de gravação
            if ($iscampogravacao !== false) {
                if ($_REQUEST[$ind]) {
                    // Campo na qual sera gravado os itens da tabela "campusitem"
                    $campogravacao = str_replace("gravacaocampo_", "", $ind);
                    $campogravacao = explode("_", $campogravacao);

                    foreach ($_REQUEST[$ind] as $itmid => $valor) {
                        if ($valor && (count($valor) > 1)) {
                            foreach ($valor as $ano => $campo) {
                                foreach ($campo as $cpitabnum => $camp) {
                                    if ($camp !== "" || $_POST['obs'][$itmid][$ano]) {
                                        switch ($campogravacao[1]) {
                                            case 'MONEY':
                                                $camp = str_replace(array(".", ","), array("", "."), $camp);
                                                break;
                                        }
                                        $sql[] = "INSERT INTO academico.campusitem(
		   		        						cmpid, itmid, " . $campogravacao[0] . ",cpidata, cpiano, cpiobs, cpitabnum)
						    					VALUES ('" . $_POST['cmpid'] . "', '" . $itmid . "', " . (($camp !== "") ? "'" . $camp . "'" : "NULL") . ", '" . date("Y-m-d") . "', '" . $ano . "',  '" . $_POST['obs'][$itmid][$ano] . "', '" . $cpitabnum . "');";
                                    }
                                }
                            }
                        }
                        // gravando os dados globais (não possui vinculo com ano)
                        if ($valor !== "" && (count($valor) == 1)) {
                            switch ($campogravacao[1]) {
                                case 'MONEY':
                                    $valor = str_replace(array(".", ","), array("", "."), $valor);
                                    break;
                            }
                            $sql[] = "INSERT INTO academico.campusitem(
	   		        						cmpid, itmid, " . $campogravacao[0] . ",cpidata, cpiobs, cpiano, cpitabnum)
					    					VALUES ('" . $_POST['cmpid'] . "', '" . $itmid . "', " . (($valor !== "") ? "'" . $valor . "'" : "NULL") . ", '" . date("Y-m-d") . "',  '" . $_POST['obs'][$itmid] . "', '" . date("Y") . "', '0');";

                        }
                    }
                }
            }
        }
    }
    return ($sql) ? $sql : false;
}


function atualizardadoscampus($dados)
{
    global $db;

    $sql_excluir = "DELETE FROM academico.campusitem WHERE cmpid = '" . $dados['cmpid'] . "' AND itmid in (SELECT itmid FROM academico.item WHERE itmtcu = false)";
    $db->executar($sql_excluir);
    $sqls = processaInsercaoCampusItem($_POST);
    if ($sqls) {
        foreach ($sqls as $sql) {
            $db->executar($sql);
        }
    }
    // Inserindo os dados do curso
    if ($dados['cursosP']) {
        foreach ($dados['cursosP'] as $curid => $val) {
            foreach ($val as $ano => $cpcqtd) {
                $cpcid = $db->pegaUm("SELECT
										cpcid
									  FROM
									  	academico.campuscurso
									  WHERE
									  	cpcano='" . $ano . "' AND
									  	curid='" . $curid . "' AND
									  	cmpid='" . $dados['cmpid'] . "' AND
									  	cpcprevisto = true");
                if ($cpcid) {
                    $sql_update = "UPDATE academico.campuscurso
									SET
										cpcqtd=" . (($cpcqtd !== "") ? "'" . $cpcqtd . "'" : "NULL") . "
									WHERE
										cpcid = {$cpcid}
										--cpcano='" . $ano . "' AND
										--curid='" . $curid . "' AND
										--cmpid='" . $dados['cmpid'] . "'";
//					ver($sql_update);
                    $db->executar($sql_update);
                } else {
                    $sql_insert = "INSERT INTO
										academico.campuscurso(curid, cmpid, cpcano, cpcqtd, cpcprevisto)
	    						   VALUES
	    						   		('" . $curid . "',
	    						   		 '" . $dados['cmpid'] . "',
	    						   		 '" . $ano . "',
	    						   		 " . (($cpcqtd !== "") ? "'" . $cpcqtd . "'" : "NULL") . ",
	    						   		 true);";
                    $db->executar($sql_insert);
                }
            }
        }
    }

    if ($dados['cursosR']) {
        foreach ($dados['cursosR'] as $curid => $val) {
            foreach ($val as $ano => $cpcqtd) {
                $cpcid = $db->pegaUm("SELECT
										cpcid
									  FROM
									  	academico.campuscurso
									  WHERE
									  	cpcano='" . $ano . "' AND
									  	curid='" . $curid . "' AND
									  	cmpid='" . $dados['cmpid'] . "'AND
									  	cpcprevisto = false");
                if ($cpcid) {
                    $sql_update = "UPDATE academico.campuscurso
								   SET
										cpcqtd=" . (($cpcqtd !== "") ? "'" . $cpcqtd . "'" : "NULL") . "
								   WHERE
								   		cpcid = {$cpcid}
								   		--cpcano='" . $ano . "' AND
								   		--curid='" . $curid . "' AND
								   		--cmpid='" . $dados['cmpid'] . "'";
                    $db->executar($sql_update);
                } else {
                    $sql_insert = "INSERT INTO
										academico.campuscurso(curid, cmpid, cpcano, cpcqtd, cpcprevisto)
	    						   VALUES
	    						   		('" . $curid . "',
	    						   		 '" . $dados['cmpid'] . "',
	    						   		 '" . $ano . "',
	    						   		 " . (($cpcqtd !== "") ? "'" . $cpcqtd . "'" : "NULL") . ",
	    						   		 false);";
                    $db->executar($sql_insert);
                }
            }
        }
    }

    $db->commit();
    echo "<script>
			alert('Os dados do campus foram atualizados com sucesso.');
			window.location = '?modulo=principal/editarcampus&acao=A&entid=" . $_SESSION['academico']['entidcampus'] . "';
		  </script>";
    exit;

}

function atualizardadosreitoriastcu($dados)
{
    global $db;

    if ($dados['itenstcu']) {
        foreach ($dados['itenstcu'] as $itmid => $val) {
            foreach ($val as $ano => $cpcqtd) {
                $sql = "SELECT
							count(cpiid)
						FROM
							academico.reitoriasitem
						WHERE
							itmid = {$itmid} AND
							retano = '{$ano}' AND
							retid = {$dados['retid']} AND
							rettabnum = 1 ";
                $existe = $db->pegaUm($sql);
                if ($cpcqtd[0] != '' || $existe > 0) {

                    $cpcqtd[0] = $cpcqtd[0] == '' ? 'null' : $cpcqtd[0];
                    $cpcqtd[0] = str_replace('.', '', $cpcqtd[0]);
                    $cpcqtd[0] = str_replace(',', '.', $cpcqtd[0]);

                    if ($existe > 0) {

                        $sql = "UPDATE academico.campusitem
							    SET
							   		cpivalor = {$cpcqtd[0]},
									cpidata = current_date
							    WHERE
									itmid = {$itmid} AND
									cpiano = '{$ano}' AND
									cpitabnum = 1 AND
									cmpid = {$dados['cmpid']}";
                    } else {
                        $sql = "INSERT INTO academico.campusitem
									(itmid, cpiano, cmpid, cpitabnum, cpivalor, cpidata)
								VALUES ({$itmid}, '{$ano}', {$dados['cmpid']}, 1, {$cpcqtd[0]}, current_date )";
                    }

                    $db->executar($sql);
                }
            }
        }
    }

    $db->commit();
    echo "<script>
			alert('Os dados da reitoria foram atualizados com sucesso.');
			window.location = '?modulo=principal/dadosindicadorestcu&acao=A&entid=" . $_SESSION['academico']['entidcampus'] . "';
		  </script>";
    exit;

}

function atualizardadoscampustcu_reitoria($dados)
{
    global $db;

    if ($dados['itenstcu']) {
        foreach ($dados['itenstcu'] as $itmid => $val) {
            foreach ($val as $ano => $cpcqtd) {
                $sql = "SELECT
							count(retid)
						FROM
							academico.reitoriasitem
						WHERE
							itmid = {$itmid} AND
							retano = '{$ano}' AND
							entid = {$dados['entid']} AND
							rettabnum = 1 ";
                $existe = $db->pegaUm($sql);
                if ($cpcqtd[0] != '' || $existe > 0) {

                    $cpcqtd[0] = $cpcqtd[0] == '' ? 'null' : $cpcqtd[0];
                    $cpcqtd[0] = str_replace('.', '', $cpcqtd[0]);
                    $cpcqtd[0] = str_replace(',', '.', $cpcqtd[0]);

                    if ($existe > 0) {

                        $sql = "UPDATE academico.reitoriasitem
							    SET
							   		retvalor = {$cpcqtd[0]},
									retdata = current_date
							    WHERE
									itmid = {$itmid} AND
									retano = '{$ano}' AND
									rettabnum = 1 AND
									entid = {$dados['entid']}";
                    } else {
                        $sql = "INSERT INTO academico.reitoriasitem
									(itmid, retano, entid, rettabnum, retvalor, retdata)
								VALUES ({$itmid}, '{$ano}', {$dados['entid']}, 1, {$cpcqtd[0]}, current_date )";
                    }

                    $db->executar($sql);
                }
            }
        }
    }

    $db->commit();
    echo "<script>
			alert('Os dados da reitoria foram atualizados com sucesso.');
			window.location = '?modulo=principal/dadosindicadorestcu_reitoria&acao=C&entid=" . $_SESSION['academico']['entidcampus'] . "';
		  </script>";
    exit;

}

function atualizardadoscampustcu($dados)
{
    global $db;

    if ($dados['itenstcu']) {
        foreach ($dados['itenstcu'] as $itmid => $val) {
            foreach ($val as $ano => $cpcqtd) {
                $sql = "SELECT
							count(cpiid)
						FROM
							academico.campusitem
						WHERE
							itmid = {$itmid} AND
							cpiano = '{$ano}' AND
							cmpid = {$dados['cmpid']} AND
							cpitabnum = 1 ";
                $existe = $db->pegaUm($sql);
                if ($cpcqtd[0] != '' || $existe > 0) {

                    $cpcqtd[0] = $cpcqtd[0] == '' ? 'null' : $cpcqtd[0];
                    $cpcqtd[0] = str_replace('.', '', $cpcqtd[0]);
                    $cpcqtd[0] = str_replace(',', '.', $cpcqtd[0]);

                    if ($existe > 0) {

                        $sql = "UPDATE academico.campusitem
							    SET
							   		cpivalor = {$cpcqtd[0]},
									cpidata = current_date
							    WHERE
									itmid = {$itmid} AND
									cpiano = '{$ano}' AND
									cpitabnum = 1 AND
									cmpid = {$dados['cmpid']}";
                    } else {
                        $sql = "INSERT INTO academico.campusitem
									(itmid, cpiano, cmpid, cpitabnum, cpivalor, cpidata)
								VALUES ({$itmid}, '{$ano}', {$dados['cmpid']}, 1, {$cpcqtd[0]}, current_date )";
                    }

                    $db->executar($sql);
                }
            }
        }
    }

    $db->commit();
    echo "<script>
			alert('Os dados do campus foram atualizados com sucesso.');
			window.location = '?modulo=principal/dadosindicadorestcu&acao=A&entid=" . $_SESSION['academico']['entidcampus'] . "';
		  </script>";
    exit;

}

function carregarvagasporcurso($dados)
{

    global $db, $anosanalisados;

    // pegando dados sobre o tipo de curso, verifica apenas o primeiro registro
    // se for por entidade (Ensino Superior) insere o turno
    // se por acaso colocarem vários tipos sendo com o campo tpc.tpcpossuientidade diferentes,
    // modificar a forma de distinção neste código. Solicitação feita pelo analista: Hugo Morais
    $sql = "
        SELECT tpc.tpcpossuientidade
        FROM academico.campuscurso cmc2
        LEFT JOIN public.curso cur ON cur.curid = cmc2.curid
        LEFT JOIN public.turno tur ON tur.turid = cur.turid
        LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid
        WHERE cmc2.cmpid='" . $dados['cmpid'] . "' AND cur.curstatus    = 'A'
        GROUP BY tpc.tpcpossuientidade LIMIT 1
    ";
    $tipocurso = $db->pegaUm($sql);

    $cabecalho[] = "";
    $cabecalho[] = "Tipo de curso";
    $cabecalho[] = "Cursos";

    // verifica se é por entidade, logo necessita do turno
    if ($tipocurso == "t") {
        $cabecalho[] = "Turno";
    }

    if ($anosanalisados[$dados['orgid']]) {
        $anos = $anosanalisados[$dados['orgid']];
    } else {
        $anos = $anosanalisados['default'];
    }

    // Pegando mascara definida para a quantidade de cursos (constantes.php)
    $mask = $db->carregar("SELECT tpimascara, tpitamanhomax FROM academico.tipoitem WHERE tpiid='" . TIPOITEM_QTD . "'");

    if ($mask) {
        $mask = current($mask);
    }

    //Inicio Listagem Previsto
    foreach ($anos as $ano) {
        $cabecalho[] = $ano;
        $inputs[] = "'<input " . (($mask['tpitamanhomax']) ? "maxlength=\"" . $mask['tpitamanhomax'] . "\"" : "") . " " . (($mask['tpimascara']) ? "
                                onKeyUp=\"this.value=mascaraglobal(\'" . $mask['tpimascara'] . "\',this.value);calculacoluna(this);\"" : "") . "
                                type=\"text\" name=\"cursosP['|| cmc2.curid ||'][" . $ano . "]\" size=\"12\" class=\"normal\"
                                value=\"'|| coalesce( ( SELECT  coalesce(cast(cpcqtd as varchar),'')
							FROM academico.campuscurso cmc1
							WHERE   cmc1.curid = cmc2.curid AND
                                                                cmc1.cpcano='" . $ano . "' AND
								cpcprevisto=true AND
								cmc1.cmpid='" . $dados['cmpid'] . "'),'') ||'\">' as ano" . $ano;
        $inputsSoma[] = "(SELECT sum(cpcqtd)
                          FROM academico.campuscurso cmc1
			  WHERE cmc1.cpcano='{$ano}' AND cpcprevisto=true AND cmc1.cmpid='{$dados['cmpid']}') as ano{$ano}";

        $totalizador[$ano] = "<input type='text' size='12' class='normal' name='totp" . $ano . "' value='' readonly>";
    }

    //verifica se é por entidade, logo necessita do turno. Faz analise na construção do SELECT
    $sql = "
        SELECT  '<img src=\"../imagens/excluir.gif\" onclick=\"removercurso(' || cmc2.curid || ',' || cmc2.cpcprevisto || ')\">' as acao,
		tpc.tpcdsc,
		cur.curdsc as curso,
		" . (($tipocurso == "t") ? "tur.turdsc," : "") . " " . implode(",", $inputs) . "
        FROM academico.campuscurso cmc2
        LEFT JOIN public.curso cur ON cur.curid = cmc2.curid
        " . (($tipocurso == "t") ? "LEFT JOIN public.turno tur ON tur.turid = cur.turid" : "") . "
        LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid
        WHERE cmc2.cmpid='" . $dados['cmpid'] . "' AND cur.curstatus='A' AND cpcprevisto=true
        GROUP BY cmc2.curid, cur.curdsc, tpc.tpcdsc, cmc2.cpcprevisto " . (($tipocurso == "t") ? ", tur.turdsc" : "") . "
        ORDER BY tpc.tpcdsc, curso " . (($tipocurso == "t") ? ",tur.turdsc" : "");

    $sqlSoma = "
        SELECT DISTINCT " . implode(",", $inputsSoma) . "
        FROM academico.campuscurso cmc2
        LEFT JOIN public.curso cur ON cur.curid = cmc2.curid
        WHERE cmc2.cmpid='" . $dados['cmpid'] . "' AND cur.curstatus='A' AND  cpcprevisto=true
    ";

    //Quase identica ao monta lista simples, porém adicionei uma última linha com os contadores em javascript
    $RS = $db->carregar($sql);
    $RSoma = $db->carregar($sqlSoma);
    $nlinhas = $RS ? count($RS) : 0;

    if (!$RS) {
        $nl = 0;
    } else {
        $nl = $nlinhas;
    }

    print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';

    if ($nlinhas > 0) {
        $cols = count($cabecalho);
        print '<tr><td colspan="' . $cols . '" align="center" valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Previstas</strong></label></tr>';

        //Monta Cabeçalho
        if (is_array($cabecalho)) {
            print '<thead><tr>';
            for ($i = 0; $i < count($cabecalho); $i++) {
                print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">' . $cabecalho[$i] . '</label>';
            }
            print '</tr> </thead>';
        }

        echo '<tbody>';
        //Monta Listagem
        $totais = array();
        $tipovl = array();
        for ($i = 0; $i < $nlinhas; $i++) {
            $c = 0;
            if (fmod($i, 2) == 0)
                $marcado = '';
            else
                $marcado = '#F7F7F7';
            print '<tr bgcolor="' . $marcado . '" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'' . $marcado . '\';">';
            foreach ($RS[$i] as $k => $v) {
                print '<td title="' . $cabecalho[$c] . '">' . $v;
                print '</td>';
                $c = $c + 1;
            }
            print '</tr>';
        }
        print '<tr>';
        if (is_array($cabecalho)) {
            print '<td class="title">&nbsp;</label>';
            print '<td class="title">&nbsp;</label>';
            // 	verifica se é por entidade, logo necessita do turno.
            if ($tipocurso == "t") {
                print '<td class="title">&nbsp;</label>';
            }
            print '<td class="title" align=\'right\'><strong>TOTAL:</strong></label>';
            foreach ($cabecalho as $campo) {
                if ($totalizador[$campo]) {
                    print '<td class="title">' . $totalizador[$campo] . '</label>';
                }
            }
//            foreach($RSoma[0] as $soma) {
//                    print '<td class="title">'.$soma.'</td>';
//
//            }
        }
        print '</tr>';
//      print '</tbody>';
    }
    print '</table>';

    //Fim Listagem Previsto
    //Inicio Listagem Realizado

    unset($cabecalho);
    foreach ($anos as $ano) {
        $cabecalho[] = $ano;
        $inputs[] = "'<input " . (($mask['tpitamanhomax']) ? "maxlength=\"" . $mask['tpitamanhomax'] . "\"" : "") . " " . (($mask['tpimascara']) ? "
                                onKeyUp=\"this.value=mascaraglobal(\'" . $mask['tpimascara'] . "\',this.value);calculacoluna(this);\"" : "") . "
                                type=\"text\" name=\"cursosR['|| cmc2.curid ||'][" . $ano . "]\" size=\"12\" class=\"normal\"
                                value=\"'|| coalesce((SELECT coalesce(cast(cpcqtd as varchar),'')
                                                      FROM academico.campuscurso cmc1
                                                      WHERE cmc1.curid = cmc2.curid AND
                                                      cmc1.cpcano='" . $ano . "' AND
						      cpcprevisto=false AND
                                                      cmc1.cmpid='" . $dados['cmpid'] . "'),'') ||'\">' as ano" . $ano;

        $totalizador[$ano] = "<input type='text' size='12' class='normal' name='totr" . $ano . "' value='' readonly>";
    }

    // verifica se é por entidade, logo necessita do turno. Faz analise na construção do SELECT
    $sql = "
        SELECT '<img src=\"../imagens/excluir.gif\" onclick=\"removercurso('|| cmc2.curid || ',' || cmc2.cpcprevisto || ')\">' as acao,
                tpc.tpcdsc,
                cur.curdsc as curso,
                " . (($tipocurso == "t") ? "tur.turdsc," : "") . " " . implode(",", $inputs) . "
        FROM academico.campuscurso cmc2
        LEFT JOIN public.curso cur ON cur.curid = cmc2.curid
        " . (($tipocurso == "t") ? "LEFT JOIN public.turno tur ON tur.turid = cur.turid" : "") . "
        LEFT JOIN public.tipocurso tpc ON tpc.tpcid = cur.tpcid
        WHERE cmc2.cmpid='" . $dados['cmpid'] . "' AND cur.curstatus='A' AND cpcprevisto=false
        GROUP BY cmc2.curid, cur.curdsc, tpc.tpcdsc, cmc2.cpcprevisto " . (($tipocurso == "t") ? ", tur.turdsc" : "") . "
        ORDER BY tpc.tpcdsc, curso " . (($tipocurso == "t") ? ",tur.turdsc" : "");

    // Quase identica ao monta lista simples, porém adicionei uma última linha com os contadores em javascript
    $RS = $db->carregar($sql);
    $nlinhas = $RS ? count($RS) : 0;
    if (!$RS)
        $nl = 0;
    else
        $nl = $nlinhas;

    print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
    if ($nlinhas > 0) {
        print '<tr><td colspan="' . $cols . '" align="center" valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Realizadas</strong></label></tr>';

//		//Monta Cabeçalho
//		if(is_array($cabecalho)) {
//			print '<thead><tr>';
//			for ($i=0;$i<count($cabecalho);$i++)
//			{
//				print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</label>';
//			}
//			print '</tr> </thead>';
//		}

        echo '<tbody>';
        //Monta Listagem
        $totais = array();
        $tipovl = array();
        for ($i = 0; $i < $nlinhas; $i++) {
            $c = 0;
            if (fmod($i, 2) == 0)
                $marcado = '';
            else
                $marcado = '#F7F7F7';
            print '<tr bgcolor="' . $marcado . '" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'' . $marcado . '\';">';
            foreach ($RS[$i] as $k => $v) {
                print '<td title="' . $cabecalho[$c] . '">' . $v;
                print '</td>';
                $c = $c + 1;
            }
            print '</tr>';
        }
        print '<tr>';
        if (is_array($cabecalho)) {
            print '<td class="title">&nbsp;</label>';
            print '<td class="title">&nbsp;</label>';
            // 	verifica se é por entidade, logo necessita do turno.
            if ($tipocurso == "t") {
                print '<td class="title">&nbsp;</label>';
            }
            print '<td class="title" align=\'right\'><strong>TOTAL:</strong></label>';
            foreach ($cabecalho as $campo) {
                if ($totalizador[$campo]) {
                    print '<td class="title">' . $totalizador[$campo] . '</label>';
                }
            }
        }
        print '</tr>';
        print '</tbody>';
    }
    print '</table>';

    //Fim Listagem Realizado
    exit;
}

function salvarRegistroDetalhes($dados, $files)
{
    global $db;

    if ($_SESSION['sig_var']['iscampus'] == 'sim') {

        $sig_var_entid = $_SESSION['sig_var']['entid'];
        $cmpid = $dados['cmpid'];
        $cmpobs = substr($dados['cmpobs'], 0, 1000);
        $usucpf = $_SESSION['usucpf'];
        $cmpdatainauguracao = $dados['cmpdatainauguracao'] ? "'" . formata_data_sql($dados['cmpdatainauguracao']) . "'" : "NULL";
        $cmpdtportaria = $dados['cmpdtportaria'] ? "'" . formata_data_sql($dados['cmpdtportaria']) . "'" : "NULL";
        $exiid = $dados['exiid'];
        $cmpsituacao = $dados['cmpsituacao'];
        $cmpinstalacao = $dados['cmpinstalacao'];
        $cmpsituacaoobra = $dados['cmpsituacaoobra'];
        $cptid = $dados['cptid'] ? $dados['cptid'] : 'NULL';
        $id_arqid = $dados['arqidportaria'] ? $dados['arqidportaria'] : 'NULL';

        #AJUSTANDO DATAS (MM/YYYY) - SOLICITADO PELO HUGO (DEVIDO ORDENAÇÃO) FORMATO DE SAÍDA : YYYYMM
        $datacriacao = substr($dados['datacriacao'], 3, 4) . substr($dados['datacriacao'], 0, 2);
        $cmpdataimplantacao = substr($dados['cmpdataimplantacao'], 3, 4) . substr($dados['cmpdataimplantacao'], 0, 2);

        #ANEXAR AQUIVOS - DOCUMENTO DA PORTARIA DE AUTORIZAÇÃO
        if ($files['arquivo']['name'] != '') {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $file = new FilesSimec("campus", NULL, "academico");

            if ($files) {
                $arquivoSalvo = $file->setUpload("Portaria de Autorização de Funcionamento", NULL, false);
                $id_arqid = $file->getIdArquivo();
            }
            if ($id_arqid == '' || $id_arqid == NULL) {
                $campo = ", arqidportaria = NULL ";
            } else {
                $campo = ", arqidportaria = {$id_arqid} ";
            }
        }


        #ATUALIZANDO/INSERINDO "CAMPUS"
        if ($cmpid != '') {
            $sql = "
                    UPDATE academico.campus SET
                        cmpobs              = '{$cmpobs}',
			cmpdataatualizacao  = NOW(),
			usucpf              = '{$usucpf}',
			cmpdataimplantacao  = '$cmpdataimplantacao',
                        cmpdatainauguracao  = {$cmpdatainauguracao},
	       		exiid               = '{$exiid}',
	       		cmpsituacao         = '{$cmpsituacao}',
                        cmpinstalacao       = '{$cmpinstalacao}',
                        cmpsituacaoobra     = '{$cmpsituacaoobra}',
                        datacriacao         = '{$datacriacao}',
                        cptid               = {$cptid},
                        cmpdtportaria       = {$cmpdtportaria}
                        {$campo}
                    WHERE cmpid = {$cmpid} RETURNING cmpid;
                ";
        } else {
            $sql = "
                    INSERT INTO academico.campus(
                            cmpobs, entid, cmpdataatualizacao, usucpf, cmpdataimplantacao, cmpdatainauguracao, exiid, cmpsituacao,
                            cmpinstalacao, cptid, datacriacao, cmpsituacaoobra, cmpdtportaria, arqidportaria
                        ) VALUES (
                            '{$cmpobs}', {$sig_var_entid}, NOW(), '$usucpf', '$cmpdataimplantacao', {$cmpdatainauguracao}, '{$exiid}', '$cmpsituacao',
                            '{$cmpinstalacao}', {$cptid}, '{$datacriacao}', '{$cmpsituacaoobra}', {$cmpdtportaria}, {$id_arqid}
                    ) RETURNING cmpid;
                ";
        }
        $a_cmpid = $db->pegaUm($sql);
    }

    if ($sig_var_entid != '') {
        if ($a_cmpid > 0) {
            $sql = "SELECT entid FROM academico.entidadedetalhe WHERE entid = {$sig_var_entid}";
            $is_ent = $db->pegaUm($sql);
        }

        if ($is_ent > 0) {
            $sql = "
                    UPDATE academico.entidadedetalhe
                        SET edtdsc = '" . addslashes($dados['edtdsc']) . "'
                    WHERE entid = {$sig_var_entid} RETURNING edtid;
                ";
            $edtid = $db->pegaUm($sql);
        } else {
            $sql = "
                    INSERT INTO academico.entidadedetalhe(
                            entid, edtdsc
                        ) VALUES (
                            {$sig_var_entid}, '{$dados['edtdsc']}'
                    ) RETURNING edtid;
                ";
            $edtid = $db->pegaUm($sql);
        }
    }

    if ($edtid > 0) {
        $db->commit();
        $db->sucesso('principal/inserir_entidade', '&acao=C', 'Operação Realizada com Sucesso!');
    } else {
        $db->rollback();
        $db->sucesso('principal/inserir_entidade', '&acao=C', 'Não foi possível executar a carga, tente novamente mais tarde!');
    }
}

# - dowloadDocAnexoPortaria: TELA Dados específicos do Campus - DOWNLOAD - DOCUMENTO DA PORTARIA DE AUTORIZAÇÃO
function dowloadDocAnexoPortaria($dados)
{

    $arqid = $dados['arqidportaria'];

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ($arqid) {
        $file = new FilesSimec("campus", $campos, "academico");
        $file->getDownloadArquivo($arqid);
    }
}

# - excluirDocAnexoPortaria: TELA Dados específicos do Campus - EXCLUIR - DOCUMENTO DA PORTARIA DE AUTORIZAÇÃO.
function excluirDocAnexoPortaria($dados, $files)
{
    global $db;

    $arqid = $dados['arqidportaria'];
    $cmpid = $dados['cmpid'];

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ($arqid != '') {
        $sql = " UPDATE academico.campus SET arqidportaria = NULL WHERE cmpid = {$cmpid} ";
    }

    if ($db->executar($sql)) {
        $file = new FilesSimec("campus", $campos, "academico");
        $file->excluiArquivoFisico($arqid);

        $db->commit();
        $db->sucesso('principal/inserir_entidade');
    }
}

function carregarItensEntidadeVisualizar($dados)
{

    global $db, $anosanalisados, $tituloitens, $_funcoesentidade;
    $sig_var_entid = $dados['entid'];
    $_SESSION['academico']['orgid'] = $_SESSION['academico']['orgid'] ? $_SESSION['academico']['orgid'] : $dados['orgid'];

    if ($dados['porcampus'] == 'sim') {
        $listacampus = $db->carregar("SELECT ent.entid, ent.entnome, cmp.cmpid FROM entidade.entidade ent
                                             LEFT JOIN academico.campus cmp ON cmp.entid = ent.entid
                                             LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                             LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                             WHERE fea.entid='" . $sig_var_entid . "' AND
											 fen.funid='" . $_funcoesentidade[$_SESSION['academico']['orgid']]['campus'] . "'AND cmp.cmpid IS NOT NULL");
        if ($listacampus[0]) {
            foreach ($listacampus as $campus) {
                ?>
                <table width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td class="SubTituloCentro"><?php echo $campus['entnome']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda"><?php
                            if ($_SESSION['academico']['orgid'] <> 2){
                            echo(($tituloitens[$_SESSION['academico']['orgid']][0]) ? $tituloitens[$_SESSION['academico']['orgid']][0] : $tituloitens['default']);
                            ?></td>
                    </tr>
                    <tr>
                        <td><?php
                            //Se tiver anos analisados por tipo de ensino (declarado noconstantes.php), caso não, utilizar o padrão
                            if ($anosanalisados[$_SESSION['academico']['orgid']]) {
                                $anos = $anosanalisados[$_SESSION['academico']['orgid']];
                            } else {
                                $anos = $anosanalisados['default'];
                            }
                            unset($cabecalho, $paramselects);
                            $cabecalho[] = "Itens";
                            foreach ($anos as $ano) {

                                $paramselects[] = "'<input class=\"normal\" id=\"' || itm.itmid ||'" . $ano . "\" name=\"gravacaocampo_' || tpi.tpicampo || '_' ||tpi.tpitipocampo || '[' || itm.itmid || '][" . $ano . "]\" '||
                                                     CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"'
                                                     ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"'
                                                     END ||'  maxlength=\"' || tpi.tpitamanhomax || '\"size=\"14\" type=\"hidden\" value=\"' ||
                                                     CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi
													 WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '" . $ano . "' AND
													 cpi.cmpid = '" . $campus['cmpid'] . "' AND cpi.cpitabnum=0) is null
                                                     THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi
													 WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '" . $ano . "' AND
													 cpi.cmpid = '" . $campus['cmpid'] . "' AND cpi.cpitabnum=0) END || '\">'	AS ano_" . $ano;
                                $cabecalho[] = $ano;
                            }
                            $paramselects = implode(",", $paramselects);
                            //criando o SELECT
                            $sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>'," . $paramselects . "
                                       FROM academico.item itm
                                       LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid
                                       LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
                                       WHERE tei.orgid = '" . $_SESSION['academico']['orgid'] . "' AND
									   itm.itmglobal = false
                                       ORDER BY tei.teiordem";
                            $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');
                            }?></td>
                    </tr>

                    <tr>
                        <td class="SubTituloEsquerda"><?php echo(($tituloitens[$_SESSION['academico']['orgid']][1]) ? $tituloitens[$_SESSION['academico']['orgid']][1] : $tituloitens['default']);
                            ?></td>
                    </tr>
                    <tr>
                        <td><?php
                            //Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
                            if ($anosanalisados[$_SESSION['academico']['orgid']]) {
                                $anos = $anosanalisados[$_SESSION['academico']['orgid']];
                            } else {
                                $anos = $anosanalisados['default'];
                            }
                            unset($cabecalho, $paramselects);
                            $cabecalho[] = "Itens";
                            foreach ($anos as $ano) {

                                $paramselects[] = "'<input class=\"normal\" id=\"' || itm.itmid ||'" . $ano . "\" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || '][" . $ano . "]\" '||
                                                                       CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"'
                                                                       ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"'
                                                                       END ||'  maxlength=\"' || tpi.tpitamanhomax || '\" size=\"14\" type=\"hidden\" value=\"' ||
                                                                       CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi
																	   WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '" . $ano . "' AND
																	   cpi.cmpid = '" . $campus['cmpid'] . "' AND cpi.cpitabnum=1) is null
                                                                       THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi
																	   WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '" . $ano . "' AND
																	   cpi.cmpid = '" . $campus['cmpid'] . "' AND cpi.cpitabnum=1) END || '\">'
																       AS ano_" . $ano;
                                $cabecalho[] = $ano;
                            }
                            $paramselects = implode(",", $paramselects);
                            //criando o SELECT
                            $sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>'," . $paramselects . "
                                       FROM academico.item itm
                                       LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid
                                       LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
                                       WHERE tei.orgid = '" . $_SESSION['academico']['orgid'] . "' AND
									   itm.itmglobal = false
                                       ORDER BY tei.teiordem";
                            $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');
                            ?></td>
                    </tr>
                    <!--<tr><td class="SubTituloEsquerda">Situação Atual</td></tr>
                       <tr><td><?
                    unset($cabecalho);
                    $cabecalho = array("Itens", "Atual");
                    $paramselct = "'<input  class=\"normal\" id=\"' || itm.itmid || '\" name=\"gravacaocampo_' || tpi.tpicampo || '_' || tpi.tpitipocampo || '[' || itm.itmid || ']\" '||
                                       CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"'
                                               ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\', this.value);\"'
                                               END ||'  maxlength=\"' || tpi.tpitamanhomax || '\"type=\"hidden\" value=\"' ||
                                       CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi
									   WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '" . $campus['cmpid'] . "') is null
                                       THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi
									   WHERE itm.itmid = cpi.itmid AND cpi.cmpid = '" . $campus['cmpid'] . "')
									   END || '\">' AS ano";

                    $sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>'," . $paramselct . "
                                       FROM academico.item itm
                                       LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid
                                       LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
                                       WHERE tei.orgid = '" . $_SESSION['academico']['orgid'] . "' AND
									   itm.itmglobal = true
                                       ORDER BY tei.teiordem";
                    $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');
                    ?></td></tr>-->
                </table>
            <?php
            }
        } else {
            echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\"cellPadding=\"3\" align=\"center\">";
            echo "<tr><td class=\"SubTituloCentro\">Não exitem campus associados.</td></tr>";
            echo "</table>";
        }
    } else {

        ?>
        <table width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <?php if ($_SESSION['academico']['orgid'] <> 2) { ?>
                <tr>
                    <td class="SubTituloEsquerda"><?php echo(($tituloitens[$_SESSION['academico']['orgid']][0]) ? $tituloitens[$_SESSION['academico']['orgid']][0] : $tituloitens['default']);
                        ?></td>
                </tr>
                <tr>
                    <td><?php
                        // Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
                        if ($anosanalisados[$_SESSION['academico']['orgid']]) {
                            $anos = $anosanalisados[$_SESSION['academico']['orgid']];
                        } else {
                            $anos = $anosanalisados['default'];
                        }

                        unset($cabecalho, $paramselects);
                        $cabecalho[] = "Itens";

                        foreach ($anos as $ano) {
                            $paramselects[] = "'<input name=\"gravacaocampo_'|| itm.itmid ||'_" . $ano . "\" type=\"hidden\" value=\"'|| CASE WHEN cast((SELECT SUM(cpivalor)
               																															FROM academico.campusitem cpi
                                                                                                                                        LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
                                                                                                                                        LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid
                                                                                                                                        LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                                                                                                                        LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                                                                                                                        WHERE itm.itmid = cpi.itmid AND cpi.cpiano ='" . $ano . "' AND fen.funid='" . $_funcoesentidade[$_SESSION['academico']['orgid']]['campus'] . "'
																																	    AND fea.entid = '" . $sig_var_entid . "' AND cpi.cpitabnum=0) as varchar) is null
                                                                                                                         				THEN ''
                                                                                                                        				ELSE cast((SELECT SUM(cpivalor)
                                                                                                                        						   FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
                                                                                                                                                   LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid
                                                                                                                                                   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                                                                                                                                   LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                                                                                                                                   WHERE itm.itmid = cpi.itmid AND cpi.cpiano ='" . $ano . "' AND fen.funid='" . $_funcoesentidade[$_SESSION['academico']['orgid']]['campus'] . "'
																																				   AND fea.entid = '" . $sig_var_entid . "' AND cpi.cpitabnum=0) as varchar)
                                                                                                                         						   END || '\"'|| CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' ELSE 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\',this.value);\"'
                                                                                                                         						   END ||'maxlength=\"' || tpi.tpitamanhomax || '\"size=\"14\"class=\"normal\" readonly> ' AS ano_" . $ano;
                            $cabecalho[] = $ano;
                        }

                        $paramselects = implode(",", $paramselects);
                        // criando o SELECT
                        $sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>'," . $paramselects . "
                       FROM academico.item itm
                       LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid
                       LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
                       WHERE tei.orgid = '" . $_SESSION['academico']['orgid'] . "' AND
						itm.itmglobal = false
                       ORDER BY tei.teiordem";

                        $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');

                        ?></td>
                </tr>
            <?php } ?>
            <tr>
                <td class="SubTituloEsquerda"><?php echo(($tituloitens[$_SESSION['academico']['orgid']][1]) ? $tituloitens[$_SESSION['academico']['orgid']][1] : $tituloitens['default']);
                    ?></td>
            </tr>
            <tr>
                <td><?php
                    // Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso não, utilizar o padrão
                    if ($anosanalisados[$_SESSION['academico']['orgid']]) {
                        $anos = $anosanalisados[$_SESSION['academico']['orgid']];
                    } else {
                        $anos = $anosanalisados['default'];
                    }

                    unset($cabecalho, $paramselects);
                    $cabecalho[] = "Itens";

                    foreach ($anos as $ano) {
                        $paramselects[] = "'<input name=\"gravacaocampo_'|| itm.itmid ||'_" . $ano . "\" type=\"hidden\" value=\"'|| CASE WHEN cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi
                                                                                                                                               LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
                                                                                                                                               LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid
                                                                                                                                               LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                                                                                                                               LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                                                                                                                               WHERE itm.itmid = cpi.itmid AND cpi.cpiano ='" . $ano . "' AND fen.funid='" . $_funcoesentidade[$_SESSION['academico']['orgid']]['campus'] . "'
																																			   AND fea.entid = '" . $sig_var_entid . "' AND cpi.cpitabnum=1) as varchar) is null
                                                                                                                         					   THEN ''
                                                                                                                         					   ELSE     cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
                                                                                                                                               LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid
                                                                                                                                               LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                                                                                                                               LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                                                                                                                               WHERE itm.itmid = cpi.itmid AND cpi.cpiano ='" . $ano . "' AND fen.funid='" . $_funcoesentidade[$_SESSION['academico']['orgid']]['campus'] . "'
																																			   AND fea.entid = '" . $sig_var_entid . "' AND cpi.cpitabnum=1) as varchar)
                                                                                                                        				       END || '\"'|| CASE WHEN tpi.tpimascara is null THEN 'onkeyup=\"\"' ELSE'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\',this.value);\"' END ||'maxlength=\"' || tpi.tpitamanhomax || '\"size=\"14\"class=\"normal\" readonly> ' AS ano_" . $ano;
                        $cabecalho[] = $ano;
                    }

                    $paramselects = implode(",", $paramselects);
                    // criando o SELECT
                    $sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>'," . $paramselects . "
                       FROM academico.item itm
                       LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid
                       LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
                       WHERE tei.orgid = '" . $_SESSION['academico']['orgid'] . "' AND itm.itmglobal = false
                       ORDER BY tei.teiordem";

                    $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');

                    ?></td>
            </tr>


            <!--<tr><td class="SubTituloEsquerda">Situação Atual</td></tr>
       <tr><td><?
            unset($cabecalho);
            $cabecalho = array("Itens", "Atual");
            $paramselct = "'<input name=\"gravacaocampo_'|| itm.itmid ||'\" type=\"hidden\" value=\"'|| CASE WHEN cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
                                                                                                                   LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid
                                                                                                                   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                                                                                                   LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                                                                                                   WHERE itm.itmid = cpi.itmid AND fea.entid ='" . $sig_var_entid . "') as varchar) is null
                                                                                                         		   THEN ''
                                                                                                         		   ELSE     cast((SELECT SUM(cpivalor)
                                                                                                         		   FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
                                                                                                                                                 LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid
                                                                                                                                                 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                                                                                                                                                 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                                                                                                                                                 WHERE itm.itmid = cpi.itmid AND fea.entid ='" . $sig_var_entid . "') as varchar)
                                                                                                                                                 END || '\"'|| CASE WHEN tpi.tpimascara is null
                                                                                                                                                 THEN 'onkeyup=\"\"'
                                                                                                                                                 ELSE
                                                                                                                                                 'onkeyup=\"this.value=mascaraglobal(\'' || tpi.tpimascara || '\',this.value);\"'
                                                                                                                                                 END ||'maxlength=\"' || tpi.tpitamanhomax || '\"size=\"14\"class=\"normal\" readonly> ' AS ano";

            $sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>'," . $paramselct . "
                       FROM academico.item itm
                       LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid
                       LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid
                       WHERE tei.orgid = '" . $_SESSION['academico']['orgid'] . "' AND itm.itmglobal = true
                       ORDER BY tei.teiordem";
            $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');
            ?></td></tr>-->
        </table>
    <?php
    }
    exit;
}


function definirtipolocalidade($orgid)
{
    switch ($_SESSION['academico']['orgid']) {
        case TPENSSUP:
            $tipolocalidade['nome'] = "Campus";
            $tipolocalidade['artigo+nome'] = "o Campus";
            break;
        case TPENSPROF:
            $tipolocalidade['nome'] = "Campus";
            $tipolocalidade['artigo+nome'] = "o Campus";
            break;
    }
    return $tipolocalidade;
}

/*
 * Função que verifica as permissões no perfil acadêmico
 *
 * @author   ...
 * @since    20-10-2009
 * @param    array $perfilLibera - Deve conter os perfis que podem manter as telas (inserir/editar/deletar).
 * @tutorial Array
                (
                    [0] => 277
                    [1] => 278
                    [2] => 375
                    [3] => 282
                    [4] => 284
                    [5] => 378
                    [6] => 373
                )
 *
 * @return   array $permissoes
 * @tutorial Array
                (
                    [vertipoensino] => Array( [0] => 1 )
                    [remover] => true
                    [gravar] => true
                    [inserircampusuned] => true
                )
 *
 */
function verificaPerfilAcademico($perfilLibera)
{
    global $db;

    $permissoes['insereEntidade'] = false;
    // Se for SuperUser, acesso total.
    if ($db->testa_superuser() || academico_possui_perfil(PERFIL_ADMINISTRADOR) || academico_possui_perfil(PERFIL_ASSESSORIA_ALTA_GESTAO)) {
        $sql = "
                SELECT  orgid
                FROM academico.orgao

                WHERE orgstatus = 'A'
            ";
        $orgid = (array)$db->carregarColuna($sql);

        $permissoes['vertipoensino'] = $orgid;
        $permissoes['remover'] = true;
        $permissoes['gravar'] = true;
        $permissoes['inserircampusuned'] = true;
        $permissoes['insereEntidade'] = true;
    } else {
        // Busca perfis atribuídos ao usuário, exceto SuperUser
        $sql = "
                SELECT  p.pflcod
                FROM seguranca.perfil p

                LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod

                WHERE pu.usucpf = '{$_SESSION['usucpf']}' AND p.pflstatus = 'A' AND p.sisid = '" . SISID . "' AND p.pflsuperuser = 'f'
            ";
        $perfilid = $db->carregarColuna($sql);

        //Busca os orgãos e unidades permitidos ao usuário
        $sql = "
                SELECT  DISTINCT ur.orgid,
                        ent.entid AS unidadeorc,
                        tpe.orgid AS orgiduo
                FROM academico.usuarioresponsabilidade ur

                LEFT JOIN entidade.entidade ent ON ent.entid = ur.entid
                LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                LEFT JOIN academico.orgaouo tpe ON tpe.funid = fen.funid

                WHERE pflcod IN ( '" . implode("','", $perfilid) . "') AND usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'
            ";
        $orgids = (array)$db->carregar($sql);

        foreach ($orgids as $tpe) {
            if ($tpe['orgid']) {
                $permissoes['vertipoensino'][] = $tpe['orgid'];
            }
            if ($tpe['orgiduo']) {
                $permissoes['vertipoensino'][] = $tpe['orgiduo'];
            }

            // Verificar necessidade
            if ($tpe['unidadeorc']) {
                $permissoes['verunidade'][$tpe['orgiduo']][] = $tpe['unidadeorc'];
            }
        }
        $intersecaoArray = array_intersect($perfilid, $perfilLibera);

        if (count($intersecaoArray) > 0) {
            $permissoes['remover'] = true;
            $permissoes['gravar'] = true;

            // Verificar necessidade
            $permissoes['inserircampusuned'] = true;
        } else {
            $permissoes['remover'] = false;
            $permissoes['gravar'] = false;

            // Verificar necessidade
            $permissoes['inserircampusuned'] = false;
        }
    }

    if (!$db->testa_superuser() && (academico_possui_perfil(PERFIL_CONSULTA_GERAL) || academico_possui_perfil(PERFIL_ASSISTENCIA_ESTUDANTIL))) {
        $permissoes['remover'] = false;
        $permissoes['gravar'] = false;

        $sql = "
                SELECT  orgid
                FROM academico.orgao

                WHERE orgstatus = 'A'
            ";
        $orgid = (array)$db->carregarColuna($sql);
        $permissoes['vertipoensino'] = $orgid;

        if (academico_possui_perfil(PERFIL_ASSISTENCIA_ESTUDANTIL)) {
            $permissoes['insereEntidade'] = true;
        }
    }
    return $permissoes;
}

/*
function verificaPerfilAcademico() {
	global $db;

	$sql = "SELECT
				p.pflcod
			FROM
				seguranca.perfil p
			LEFT JOIN
				seguranca.perfilusuario pu ON pu.pflcod = p.pflcod
			WHERE
				pu.usucpf = '". $_SESSION['usucpf'] ."'
				AND p.pflstatus = 'A'
				AND p.sisid =  '". SISID ."'";
	$perfilid = $db->carregarColuna($sql);

	if($db->testa_superuser()) {
		// Selecionando tipos de ensino (TODOS)
		$sql = "SELECT orgid FROM academico.orgao";
		$orgids = (array) $db->carregar($sql);
		foreach($orgids as $tpe) {
			$permissoes['vertipoensino'][] = $tpe['orgid'];
		}
		$permissoes['remover'] = true;
		$permissoes['gravar'] = true;
		$permissoes['inserircampusuned'] = true;

	} else {

		$sql = "SELECT
					DISTINCT ur.orgid, ent.entid AS unidadeorc, tpe.orgid AS tipounidadeorc
				FROM
					academico.usuarioresponsabilidade ur
				LEFT JOIN
					entidade.entidade ent ON ent.entid = ur.entid
				LEFT JOIN
					entidade.funcaoentidade fen ON fen.entid = ent.entid
				LEFT JOIN
					academico.orgaouo tpe ON tpe.funid = fen.funid
				WHERE
					pflcod IN ( '" . implode("','", $perfilid) . "')
					AND usucpf = '". $_SESSION['usucpf'] ."'
					AND rpustatus = 'A'";
		$orgids = (array) $db->carregar($sql);
		foreach($orgids as $tpe) {
			if($tpe['orgid']) {
				$permissoes['vertipoensino'][] = $tpe['orgid'];
			}
			if ($tpe['tipounidadeorc']){
				$permissoes['vertipoensino'][] = $tpe['tipounidadeorc'];
			}
			if($tpe['unidadeorc']) {
				$permissoes['verunidade'][$tpe['tipounidadeorc']][] = $tpe['unidadeorc'];
			}

		}

		if (in_array(PERFIL_MECCADCURSOS,$perfilid) || in_array(PERFIL_ADMINISTRADOR,$perfilid)){
			$permissoes['remover'] 			 = true;
			$permissoes['gravar'] 			 = true;
			$permissoes['inserircampusuned'] = true;
		}else{
			$permissoes['remover'] = false;
			$permissoes['gravar']  = false;
		}
	}
	return $permissoes;
}
*/

function validaAcessoTipoEnsino($permissoes, $orgid)
{
    if (empty($permissoes)) {
        die("<script>
				alert('Você não possui autorização para acessar o TIPO DE ENSINO.');
				window.location = '?modulo=inicio&acao=C';
			 </script>");
    }
    $permissoes = array_flip($permissoes);
    if (!isset($permissoes[$orgid])) {
        die("<script>
				alert('Você não possui autorização para acessar o TIPO DE ENSINO.');
				window.location = '?modulo=inicio&acao=C';
			 </script>");
    }
}


function monta_cabecalho_academico($entid, $tipo = "curso")
{

    global $db;

    $titulo = $tipo == "curso" ? "Cursos da Instituição" : "Processo Seletivo";
    $titulo = $tipo == "tcu" ? "Dados para os Indicadores TCU" : "Processo Seletivo";
    monta_titulo($titulo, '');

    // mensagem de bloqueio
    //$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

    if ($_SESSION['sig_var']['iscampus'] == 'sim') {
        $sql = "SELECT
					ent.entnome as campus,
					ende.estuf,
					mundescricao,
					orgdesc,
					uo.entnome AS unidadeorc,
					uo.entid as unidadeorcid,
					tpe.orgid
				FROM
					entidade.entidade ent
				INNER JOIN
					entidade.funcaoentidade fen ON fen.entid = ent.entid
				INNER JOIN
					entidade.funentassoc fea ON fea.fueid = fen.fueid
				INNER JOIN
					entidade.entidade uo ON uo.entid = fea.entid
				INNER JOIN
					entidade.funcaoentidade fen2 ON fen2.entid = uo.entid
				INNER JOIN
					academico.orgaouo teu ON teu.funid = fen2.funid
				INNER JOIN
					academico.orgao tpe ON tpe.orgid = teu.orgid
				LEFT JOIN
					entidade.endereco ende ON ende.entid = ent.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ende.muncod
												 AND mun.estuf = ende.estuf
				WHERE
					ent.entid = '" . $entid . "'
				ORDER BY
					ent.entnome;";
    } else {
        $sql = "SELECT
					ent.entid as unidadeorcid,
					ent.entnome as unidadeorc,
					ende.estuf,
					mundescricao,
					orgdesc,
					tpe.orgid
				FROM
					entidade.entidade ent
				INNER JOIN
					entidade.funcaoentidade fen ON fen.entid = ent.entid
				INNER JOIN
					academico.orgaouo teu ON teu.funid = fen.funid
				INNER JOIN
					academico.orgao tpe ON tpe.orgid = teu.orgid
				LEFT JOIN
					entidade.endereco ende ON ende.entid = ent.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ende.muncod
												 AND mun.estuf = ende.estuf
				WHERE
					ent.entid = '" . $entid . "'
				ORDER BY
					ent.entnome;";
    }
    $dadosentidade = $db->pegaLinha($sql);

    if (is_array($dadosentidade)) {
        $_SESSION['academico'] = array_merge($dadosentidade, $_SESSION['academico']);
    }

    if ($dadosentidade && $dadosentidade['orgdesc']) {
        echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
        echo "<tr>";
        echo "<td class='SubTituloDireita'>Tipo Ensino :</td><td>" . $dadosentidade['orgdesc'] . "</td>";
        echo "</tr>";
        echo "<tr>";
        echo "<td class='SubTituloDireita'>Instituição :</td><td><a style=\"cursor:pointer;\" onclick=\"window.location='?modulo=principal/editarentidade&acao=A&iscampus=nao&orgid=" . $dadosentidade['tpeid'] . "&entid=" . $dadosentidade['unidadeorcid'] . "';\"><img src=\"../imagens/consultar.gif\" border=\"0\"> " . $dadosentidade['unidadeorc'] . "</a></td>";
        echo "</tr>";
        if ($_SESSION['sig_var']['iscampus'] == 'sim') {
            echo "<tr>";
            echo "<td class='SubTituloDireita'>Campus / Uned :</td><td>" . $dadosentidade['campus'] . "</td>";
            echo "</tr>";
        }
        echo "<tr>";
        echo "<td class='SubTituloDireita'>UF / Munícipio :</td><td>" . $dadosentidade['estuf'] . " / " . $dadosentidade['mundescricao'] . "</td>";
        echo "</tr>";
        echo "</table>";
    } else {
        die("<script>
				alert('Foram encontrados problemas nos parâmetros. Caso o erro persista, entre em contato com o suporte técnico');
				window.location='?modulo=inicio&acao=C';
			 </script>");
    }
    return $bloqueado;
}


/********************************* Funções do Painel da Rede Federal *********************************/


/**
 * Exibe lista das unidades de acordo com o estado, caso exista, e com o tipo de ensino
 *
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param string $estuf
 * @param integer $orgid
 *
 */
function academico_lista_unidades_painel($estuf, $orgid, $filtro = null)
{

    global $db;

    // pega a descricao do estado
    $dados_uf = $db->pegaUm("SELECT estdescricao FROM territorios.estado WHERE estuf = '{$estuf}'");

    //Não exibir as instiuições "Colégio Pedro II", "Instituto Nacional de Educação de Surdos" e "Instituto Benjamin Constant"
    $where_filtro .= "e.entid NOT IN (411791, 411790, 388730) AND ";

    switch ($_REQUEST['estuf']) {
        case 'norte':
            $dados_uf = 'Norte';
            break;
        case 'nordeste':
            $dados_uf = 'Nordeste';
            break;
        case 'sudeste':
            $dados_uf = 'Sudeste';
            break;
        case 'sul':
            $dados_uf = 'Sul';
            break;
        case 'centrooeste':
            $dados_uf = 'Centro-Oeste';
            break;
    }

    if ($filtro['exiid']) {
        $where_filtro .= "exiid='" . $filtro['exiid'] . "' AND ";
        $parametros_filtro .= "&filtro[exiid]=" . $filtro['exiid'];
    }

    if ($filtro['cmpsituacao']) {
        $where_filtro .= "cmpsituacao='" . $filtro['cmpsituacao'] . "' AND ";
        $parametros_filtro .= "&filtro[cmpsituacao]=" . $filtro['cmpsituacao'];
    }

    if ($filtro['cmpinstalacao']) {
        $where_filtro .= "cmpinstalacao='" . $filtro['cmpinstalacao'] . "' AND ";
        $parametros_filtro .= "&filtro[cmpinstalacao]=" . $filtro['cmpinstalacao'];
    }


    // imprime título do estado e abre a div da lista de unidades
    print '<fieldset style="background: #ffffff; height: 90%;"><legend>' . ($dados_uf ? $dados_uf : 'Brasil') . '</legend>'
        . '<div id="tabelalistaunidades" style="overflow: auto; width:97%; height: 220px; border: 1px solid #cccccc; "/>';

    // verifica a função da entidade de acordo com o tipo de ensino
    switch ($orgid) {
        case '1':
            $funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
            break;
        case '2':
            $funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
            break;
        case '3':
            $funid = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";
            break;
    }

    // cria filtro e join caso a pesquisa seja por regiões
    switch ($estuf) {
        case 'norte':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "te.regcod = '1' AND";
            break;
        case 'nordeste':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "te.regcod = '2' AND";
            break;
        case 'sudeste':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "te.regcod = '3' AND";
            break;
        case 'sul':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "te.regcod = '4' AND";
            break;
        case 'centrooeste':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "te.regcod = '5' AND";
            break;
        default:
            $where_estuf = $estuf == 'todos' ? '' : "ed.estuf = '{$estuf}' AND";
            break;
    }

    // cria a query com as unidades de acordo com os filtros
    $sql = "SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga" . $parametros_filtro . "&orgid={$orgid}&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
				'<a onclick=\"atualiza_div( \'unidade\', \'' || e.entid || '\');\" style=\"cursor:pointer;\">' || CASE WHEN e.entsig IS NULL THEN  e.entnome ELSE  e.entsig || ' - ' || e.entnome END || '</a>' as nome,
				'<center>' || count(distinct efc.entid) || '</center>' as qtde_campus,
				'<center>' || count(distinct edc.muncod) || '</center>' as qtde_mun,
				ed.estuf as uf,
				'</tr><tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || e.entid || '\" colspan=\"4\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				entidade.entidade e
			INNER JOIN
				( select min(endid) as endid, entid from entidade.endereco group by entid ) ed2 ON e.entid = ed2.entid
			INNER JOIN
				entidade.endereco ed ON ed.endid = ed2.endid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid

			INNER JOIN entidade.funentassoc eac ON eac.entid = e.entid
			INNER JOIN entidade.funcaoentidade efc ON efc.fueid = eac.fueid
			inner join entidade.endereco edc ON edc.entid = efc.entid
			INNER JOIN academico.campus ac on ac.entid=edc.entid
			INNER JOIN entidade.entidade e2 on e2.entid=ac.entid

			{$join_regiao}
			WHERE
			 	ef.fuestatus='A'  AND
			 	efc.fuestatus='A' AND
			 	e2.entstatus='A' AND
				{$where_estuf}
				{$where_filtro}
				ef.funid {$funid}
			GROUP BY e.entid, e.entnome, e.entsig, ed.estuf
			ORDER BY
				e.entsig, nome";


    // exibe a lista de unidades na tela
    $cabecalho = array("Ação", "Nome da Instituição", "Qtde. Campus / Unidades", "Qtde. Municípios", "UF");

    $LarguraHeader = array(10, 60, 15, 10, 5);

    $db->monta_lista_simples($sql, $cabecalho, 1000, 30, 'N', '100%', '', 'S', $LarguraHeader, 140);
    //$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');


    $sql = "SELECT
				count(distinct efc.entid) as qtde_campus,
				count(distinct ed.muncod) as qtde_mun
			FROM entidade.funcaoentidade ef
			INNER JOIN entidade.entidade e on e.entid=ef.entid
			INNER JOIN entidade.funentassoc eac ON eac.entid = ef.entid
			INNER JOIN entidade.funcaoentidade efc ON efc.fueid = eac.fueid
			inner join entidade.endereco ed ON ed.entid = efc.entid
			INNER JOIN academico.campus ac on ac.entid=ed.entid
			INNER JOIN entidade.entidade e2 on e2.entid=ac.entid

			{$join_regiao}
			WHERE
				ef.fuestatus='A' AND
			 	efc.fuestatus='A' AND
			 	e2.entstatus='A' AND
				{$where_estuf}
				{$where_filtro}
				ef.funid {$funid}";
    $total = $db->pegaLinha($sql);

    $sql = "SELECT
				e.entid
			FROM
				entidade.entidade e
			INNER JOIN
				( select min(endid) as endid, entid from entidade.endereco group by entid ) ed2 ON e.entid = ed2.entid
			INNER JOIN
				entidade.endereco ed ON ed.endid = ed2.endid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid

			INNER JOIN entidade.funentassoc eac ON eac.entid = e.entid
			INNER JOIN entidade.funcaoentidade efc ON efc.fueid = eac.fueid
			inner join entidade.endereco edc ON edc.entid = efc.entid
			INNER JOIN academico.campus ac on ac.entid=edc.entid
			INNER JOIN entidade.entidade e2 on e2.entid=ac.entid

			{$join_regiao}
			WHERE
			 	ef.fuestatus='A'  AND
			 	efc.fuestatus='A' AND
			 	e2.entstatus='A' AND
				{$where_estuf}
				{$where_filtro}
				ef.funid {$funid}
			GROUP BY e.entid";

    $tot_instituicao = $db->carregar($sql);


    $table = '<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" />';
    $table .= '<thead>
			  <tr><td colspan=2 width=70% align=right >Total de instituições: <b>' . count($tot_instituicao) . '</b> / Totais:</td><td style="font-weight:bold"  width=15% align=center  >' . $total['qtde_campus'] . '</td><td  style="font-weight:bold" width=10% align=center  >' . $total['qtde_mun'] . '</td><td width= 5%></td><td ><div style="width:10px" ></div></td></tr>
			  </thead>';
    $table .= "</table>";
    echo $table;
    // fecha a div da lista de unidades
    print '</div>';

}

/**
 * Exibe lista dos campi das unidades de acordo com o estado, caso exista, e com o tipo de ensino
 *
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param integer $entid
 * @param integer $orgid
 *
 */
function academico_lista_campus_painel($entid, $orgid, $link = true)
{

    global $db;

    // verifica a função da entidade de acordo com o tipo de ensino
    switch ($orgid) {
        case '1':
            $funid = ACA_ID_CAMPUS;
            break;
        case '2':
            $funid = ACA_ID_UNED;
            break;
    }

    $filtro = $_REQUEST['filtro'];

    if ($filtro['exiid']) {
        $where_filtro .= "ac.exiid='" . $filtro['exiid'] . "' AND ";
    }

    if ($filtro['cmpsituacao']) {
        $where_filtro .= "cmpsituacao='" . $filtro['cmpsituacao'] . "' AND ";
    }

    if ($filtro['cmpinstalacao']) {
        $where_filtro .= "cmpinstalacao='" . $filtro['cmpinstalacao'] . "' AND ";
    }

    if ($link)
        $nome = "'<a onclick=\"atualiza_div( \'campus\', \'' || e2.entid || '\');\" style=\"cursor:pointer;\">' || e2.entnome || '</a>'";
    else
        $nome = "e2.entnome";

    // cria a query com os campi de acordo com os filtros
    $sql = "SELECT
				$nome,
				tm.mundescricao,
				ex.exidsc as preexistente,
				CASE
					WHEN cmpsituacao = 'F' THEN 'Funcionando'
					WHEN cmpsituacao = 'N' THEN 'Não funcionando'
					ELSE '-'
				END as funcionamento
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.endereco ed ON ed.entid = e.entid
			INNER JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			LEFT JOIN
				academico.campus ac ON e2.entid = ac.entid
			LEFT JOIN
				academico.existencia ex ON ex.exiid = ac.exiid

			WHERE
				" . $where_filtro . "
				ea.entid = {$entid} AND
				e.entstatus = 'A' AND ef.funid = {$funid}
			ORDER BY
				e.entnome, tm.mundescricao";

    // exibe a lista de campi na tela
    $cabecalho = array("Nome do Campus", "Município", "Existência", "Situação");
    $db->monta_lista_simples($sql, $cabecalho, 100, 30, 'N', '100%');

}

/**
 * Exibe a lista da situação atual, por, pais, região, ou estado atravez do tipo de ensino.
 *
 * @author Felipe Chiavicatti
 * @since 29/07/2009
 * @param string $estuf
 * @param integer $orgid
 *
 */
function academico_situacao_atual($orgid, $filtro = '', $tabnum = 1, $filtrocmp = null)
{
    global $db;

    include_once '_funcoesacomprevreal.php';

    $_LISTA[TPENSSUP] = array('Docentes' => array('realizado' => 25, 'previsto' => 3),
        'Técnicos' => array('realizado' => 26, 'previsto' => 4),
        'Matrículas (2008)' => array('realizado' => 45, 'previsto' => false, 'anomax' => '2008'),
        'Vagas' => array('realizado' => 28, 'previsto' => 2, 'anomax' => '2009'),
        'Cursos' => array('realizado' => 29, 'previsto' => 10, 'anomax' => '2009'),
        'Investimentos (2005-2009)' => array('realizado' => 30, 'previsto' => 5)
    );

    $_LISTA[TPENSPROF] = array('Docentes' => array('realizado' => 50, 'previsto' => 49),
        'Técnicos' => array('realizado' => 52, 'previsto' => 51),
        'Matrículas' => array('realizado' => 34, 'previsto' => 11, 'anomax' => '2008'),
        'Vagas' => array('realizado' => 35, 'previsto' => 31),
        'Cursos' => array('realizado' => 36, 'previsto' => 32),
        'Investimentos (2005-2009)' => array('realizado' => 37, 'previsto' => 14)
    );


    echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" >';
    echo "<tr>
		  <td class=SubTituloCentro>Item</td>
		  <td class=SubTituloCentro>Prev.</td>
		  <td class=SubTituloCentro>Real.</td>
		  <td class=SubTituloCentro>%</td>
		  </tr>";

    $cont = 0;


    if ($filtro['estuf'] && $filtro['estuf'] != 'todos') {
        $filtrocampus = "AND ende.estuf='" . $filtro['estuf'] . "'";
    }
    if ($filtro['regcod']) {
        $filtrocampus = "AND est.regcod='" . $filtro['regcod'] . "'";
    }
    if ($filtro['entid2']) {
        $filtrocampus = "AND cam.entid='" . $filtro['entid2'] . "'";
    }

    if ($filtrocmp['exiid']) {
        $filtroc[] = "exiid='" . $filtrocmp['exiid'] . "' ";
    }
    if ($filtrocmp['cmpsituacao']) {
        $filtroc[] = "cmpsituacao='" . $filtrocmp['cmpsituacao'] . "' ";
    }
    if ($filtrocmp['cmpinstalacao']) {
        $filtroc[] = "cmpinstalacao='" . $filtrocmp['cmpinstalacao'] . "'";
    }

    if ($filtro['entid']) {

        switch ($orgid) {
            case TPENSSUP:
                $fune = ACA_ID_UNIVERSIDADE;
                $func = ACA_ID_CAMPUS;
                break;
            case TPENSPROF:
                $fune = ACA_ID_ESCOLAS_TECNICAS;
                $func = ACA_ID_UNED;
                break;
        }

        $sql = "SELECT e2.entid as codigo,
				   e2.entnome as descricao,
				   cmp.cmpid
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			INNER JOIN
				academico.campus cmp ON cmp.entid = e2.entid
			WHERE
				" . (($filtroc) ? implode(" AND ", $filtroc) . " AND " : "") . "
				ea.entid = " . $filtro['entid'] . " AND
				e.entstatus = 'A' AND ef.funid = " . $func . "
			ORDER BY
				e.entnome";

        $campus = $db->carregar($sql);

        unset($camp);
        if ($campus[0]) {
            foreach ($campus as $cam) {
                $camp[] = $cam['cmpid'];
            }
            $filtrocampus = " AND cpm.cmpid IN('" . implode("','", $camp) . "')";
        }

    }

    foreach ($_LISTA[$orgid] as $n => $l) {

        if ($l['anomax']) {
            $filtroano = "AND cpiano='" . $l['anomax'] . "'";
        } else {
            $filtroano = "AND cpiano IN('2005','2006','2007','2008','2009')";
        }

        $corLinha = $cont % 2 ? "#f7f7f7" : "#ffffff";
        $cont++;

        $link_n = base64_encode($n);

        echo "<tr bgcolor=\"$corLinha\" onmouseout=\"this.bgColor='$corLinha';\" onmouseover=\"this.bgColor='#ffffcc';\" ><td class=SubTituloDireita nowrap>" . $n . "</td>";


        $valorreal = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cpm
									 INNER JOIN academico.item itm ON itm.itmid = cpm.itmid
								  	 INNER JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid
								  	 INNER JOIN academico.campus cam ON cam.cmpid = cpm.cmpid
								  	 INNER JOIN entidade.endereco ende ON ende.entid = cam.entid
								  	 INNER JOIN territorios.estado est ON est.estuf = ende.estuf
								  	 WHERE cpm.itmid='" . $l['realizado'] . "' " . $filtrocampus . " " . (($filtroc) ? " AND " . implode(" AND ", $filtroc) : "") . " " . $filtroano . " AND cpitabnum=" . $tabnum . "
								  	 GROUP BY tpimascara");

        unset($valorprev);
        if ($l['previsto']) {

            $valorprev = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cpm
										 INNER JOIN academico.item itm ON itm.itmid = cpm.itmid
									  	 INNER JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid
									  	 INNER JOIN academico.campus cam ON cam.cmpid = cpm.cmpid
									  	 INNER JOIN entidade.endereco ende ON ende.entid = cam.entid
									  	 INNER JOIN territorios.estado est ON est.estuf = ende.estuf
									  	 WHERE cpm.itmid='" . $l['previsto'] . "' " . $filtrocampus . " " . (($filtroc) ? " AND " . implode(" AND ", $filtroc) : "") . " " . $filtroano . " AND cpitabnum=" . $tabnum . "
									  	 GROUP BY tpimascara");
        }


        if ((int)date("Y") > (int)$ano) {
            echo "<td align=right style=\"color:#888888\" >" . (($valorprev['v']) ? mascaraglobal($valorprev['v'], $valorprev['tpimascara']) : "-") . "</td><td align=right style=\"color:#888888\" >" . (($valorreal['v']) ? mascaraglobal($valorreal['v'], $valorreal['tpimascara']) : "-") . "</td>";
        } else {
            echo "<td align=right style=\"color:#888888\" >" . (($valorprev['v']) ? mascaraglobal($valorprev['v'], $valorprev['tpimascara']) : "-") . "</td>";
        }

        if ((int)date("Y") > (int)$ano) {
            echo "<td align=center>" . barraDeProgresso($valorprev['v'], $valorreal['v']) . "</td>";
        }

        echo "</tr>";

        $_LISTACORES[$n] = substr(simec_htmlentities(barraDeProgresso($valorprev['v'], $valorreal['v'])), strpos(simec_htmlentities(barraDeProgresso($valorprev['v'], $valorreal['v'])), "background:") + 11, 7);
    }

    echo "</table>";

    $_SESSION['academico']['cores'][$tabnum] = $_LISTACORES;

}


/**
 * Exibe a lista da situação atual, por, pais, região, ou estado atravez do tipo de ensino.
 *
 * @author Felipe Chiavicatti
 * @since 29/07/2009
 * @param string $estuf
 * @param integer $orgid
 *
 */
function academico_situacao_atual_comparacao($orgid, $filtro = '', $tabnum = 1, $iden = 'x', $filtrocmp = null)
{
    global $db;

    $tabnum = !$tabnum ? 1 : $tabnum;

    include_once '_funcoesacomprevreal.php';

    $_LISTA[TPENSSUP] = array('Docentes' => array('realizado' => 25, 'previsto' => 3),
        'Técnicos' => array('realizado' => 26, 'previsto' => 4),
        'Matrículas (2008)' => array('realizado' => 45, 'previsto' => false, 'anomax' => '2008'),
        'Vagas' => array('realizado' => 28, 'previsto' => 2, 'anomax' => '2009'),
        'Cursos' => array('realizado' => 29, 'previsto' => 10, 'anomax' => '2009'),
        'Investimentos (2005-2009)' => array('realizado' => 30, 'previsto' => 5)
    );


    $_LISTA[TPENSPROF] = array('Docentes' => array('realizado' => 50, 'previsto' => 49),
        'Técnicos' => array('realizado' => 52, 'previsto' => 51),
        'Matrículas' => array('realizado' => 34, 'previsto' => 11, 'anomax' => '2008'),
        'Vagas' => array('realizado' => 35, 'previsto' => 31),
        'Cursos' => array('realizado' => 36, 'previsto' => 32),
        'Investimentos (2005-2009)' => array('realizado' => 37, 'previsto' => 14)
    );


    echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" id="tabela' . $iden . $tabnum . '" >';
    echo "<tr>
		  <td class=SubTituloCentro>Prev.</td>
		  <td class=SubTituloCentro>%</td>
		  </tr>";

    $cont = 0;


    if ($filtro['estuf'] && $filtro['estuf'] != 'todos') {
        $filtrocampus = "AND ende.estuf='" . $filtro['estuf'] . "'";
    }
    if ($filtro['regcod']) {
        $filtrocampus = "AND est.regcod='" . $filtro['regcod'] . "'";
    }
    if ($filtro['entid2']) {
        $filtrocampus = "AND cam.entid='" . $filtro['entid2'] . "'";
    }

    if ($filtrocmp['exiid']) {
        $filtroc[] = "exiid='" . $filtrocmp['exiid'] . "' ";
    }
    if ($filtrocmp['cmpsituacao']) {
        $filtroc[] = "cmpsituacao='" . $filtrocmp['cmpsituacao'] . "' ";
    }
    if ($filtrocmp['cmpinstalacao']) {
        $filtroc[] = "cmpinstalacao='" . $filtrocmp['cmpinstalacao'] . "'";
    }


    if ($filtro['entid']) {

        switch ($orgid) {
            case TPENSSUP:
                $fune = ACA_ID_UNIVERSIDADE;
                $func = ACA_ID_CAMPUS;
                break;
            case TPENSPROF:
                $fune = ACA_ID_ESCOLAS_TECNICAS;
                $func = ACA_ID_UNED;
                break;
        }

        $sql = "SELECT e2.entid as codigo,
				   e2.entnome as descricao,
				   cmp.cmpid
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			INNER JOIN
				academico.campus cmp ON cmp.entid = e2.entid
			WHERE
				" . (($filtroc) ? implode(" AND ", $filtroc) . " AND " : "") . "
				ea.entid = " . $filtro['entid'] . " AND
				e.entstatus = 'A' AND ef.funid = " . $func . "
			ORDER BY
				e.entnome";

        $campus = $db->carregar($sql);

        unset($camp);
        if ($campus[0]) {
            foreach ($campus as $cam) {
                $camp[] = $cam['cmpid'];
            }
            $filtrocampus = " AND cpm.cmpid IN('" . implode("','", $camp) . "')";
        }
    }

    $anosv = array('2010', '2011', '2012');
    $trava = false;
    foreach ($anosv as $a) {

        $idcount = 1;
        foreach ($_LISTA[$orgid] as $n => $l) {

            if ($l['anomax']) {
                $filtroanoreal = "AND cpiano='" . $l['anomax'] . "'";
                $filtroanoprev = "AND cpiano='" . $a . "'";
            } else {
                $filtroanoreal = "AND cpiano IN('2005','2006','2007','2008','2009')";
                unset($filtromeioprev);
                for ($i = 2005; $i <= $a; $i++) $filtromeioprev[] .= $i;
                $filtroanoprev = "AND cpiano IN('" . implode("','", $filtromeioprev) . "')";
            }

            $corLinha = $cont % 2 ? "#f7f7f7" : "#ffffff";
            $cont++;

            $link_n = base64_encode($n);

            echo "<tr bgcolor=\"$corLinha\" onmouseout=\"this.bgColor='$corLinha';\" onmouseover=\"this.bgColor='#ffffcc';\" " . (($trava) ? "style=display:none" : "") . " id=" . $a . ">";
            $idcount++;


            $valorreal = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cpm
										 INNER JOIN academico.item itm ON itm.itmid = cpm.itmid
									  	 INNER JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid
									  	 INNER JOIN academico.campus cam ON cam.cmpid = cpm.cmpid
									  	 INNER JOIN entidade.endereco ende ON ende.entid = cam.entid
									  	 INNER JOIN territorios.estado est ON est.estuf = ende.estuf
									  	 WHERE cpm.itmid='" . $l['realizado'] . "' " . $filtrocampus . " " . (($filtroc) ? " AND " . implode(" AND ", $filtroc) : "") . " " . $filtroanoreal . " AND cpitabnum=" . $tabnum . "
									  	 GROUP BY tpimascara");

            unset($valorprev);
            if ($l['previsto']) {

                $valorprev = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cpm
											 INNER JOIN academico.item itm ON itm.itmid = cpm.itmid
										  	 INNER JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid
										  	 INNER JOIN academico.campus cam ON cam.cmpid = cpm.cmpid
										  	 INNER JOIN entidade.endereco ende ON ende.entid = cam.entid
										  	 INNER JOIN territorios.estado est ON est.estuf = ende.estuf
										  	 WHERE cpm.itmid='" . $l['previsto'] . "' " . $filtrocampus . " " . (($filtroc) ? " AND " . implode(" AND ", $filtroc) : "") . " " . $filtroanoprev . " AND cpitabnum=" . $tabnum . "
										  	 GROUP BY tpimascara");
            }


            echo "<td align=right style=\"color:#888888\" >" . (($valorprev['v']) ? mascaraglobal($valorprev['v'], $valorprev['tpimascara']) : "-") . "</td>";

            echo "<td align=center>" . barraDeProgresso($valorprev['v'], $valorreal['v'], (($_SESSION['academico']['cores'][$tabnum][$n]) ? $_SESSION['academico']['cores'][$tabnum][$n] : "")) . "</td>";

            echo "</tr>";
        }
        $trava = true;

    }
    echo "</table>";

}

/**
 * Exibe lista das situações das obras cadastradas nas unidades de acordo com o estado, caso exista, e com o tipo de ensino
 *
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param string $estuf
 * @param integer $orgid
 * @param integer $entid
 *
 */
function academico_situacao_obras($orgid, $estuf = '', $entid = '', $filtrocmp = null)
{

    global $db;

    if ($filtrocmp['exiid']) {
        $filtroc[] = "exiid='" . $filtrocmp['exiid'] . "' ";
    }
    if ($filtrocmp['cmpsituacao']) {
        $filtroc[] = "cmpsituacao='" . $filtrocmp['cmpsituacao'] . "' ";
    }
    if ($filtrocmp['cmpinstalacao']) {
        $filtroc[] = "cmpinstalacao='" . $filtrocmp['cmpinstalacao'] . "'";
    }


    if ($entid) {
        $join_entid = " INNER JOIN
							 entidade.entidade e ON e.entid = entidunidade OR e.entid = entidcampus";
        $where_entid = " AND e.entid = {$entid} ";
    }
    // cria o join e o filtro de estado caso exista
    $join_estuf = ($estuf && $estuf != 'todos') ? "INNER JOIN entidade.endereco ed ON ed.endid = oi.endid" : "";
    $where_estuf = ($estuf && $estuf != 'todos') ? "AND ed.estuf = '{$estuf}'" : "";

    // cria o join e o filtro por região, caso exista
    switch ($estuf) {
        case 'norte':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "AND te.regcod = '1'";
            break;
        case 'nordeste':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "AND te.regcod = '2'";
            break;
        case 'sudeste':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "AND te.regcod = '3'";
            break;
        case 'sul':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "AND te.regcod = '4'";
            break;
        case 'centrooeste':
            $join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
            $where_estuf = "AND te.regcod = '5'";
            break;
        default:
            break;
    }

    // cria a query com as situações de obras e a quantidade
    $sql = "SELECT
				case when oi.stoid is not null
				then so.stodesc
				else 'Não informado' end as descricao,
				count(oi.obrid) as total
			FROM
				obras.obrainfraestrutura oi
			LEFT JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			LEFT JOIN
				academico.campus cam ON cam.entid = oi.entidcampus
			{$join_estuf}
			{$join_regiao}
			{$join_entid}
			WHERE
				oi.obsstatus = 'A' AND
				oi.orgid = {$orgid}
				{$where_estuf}
				{$where_entid}
				" . (($filtroc) ? " AND " . implode(" AND ", $filtroc) : "") . "
			GROUP BY
				oi.stoid, stodesc
			ORDER BY
				descricao";
//dbg($sql);
    // exibe na tela a lista de situações de obras
    $cabecalho = array("Situação", "Qtd de Obras");
    $arrTamanhos = array(70, 30);
    //$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'S', '100%','N');
    $db->monta_lista_simples($sql, $cabecalho, 1000, 30, 'S', '100%', 'N', '', $arrTamanhos, 100);

}

/**
 *
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param string $estuf
 * @param integer $orgid
 *
 */
function academico_obras_estado($orgid, $estuf = '')
{

    global $db;

    if ($estuf && $estuf != 'todos') {

        $titulo = 'Município';

        $sql = "SELECT
					CASE WHEN tm.mundescricao <> ''
						 THEN '' || tm.mundescricao || ''
						 ELSE 'Não Informado' END as municipio,
					count(oi.obrid) as total
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE
					oi.obsstatus = 'A' AND
					oi.orgid = {$orgid} AND
					ed.estuf = '{$estuf}'
				GROUP BY
					tm.mundescricao, ed.estuf
				ORDER BY
					municipio";
    } else {

        $titulo = 'Estado';

        $sql = "SELECT
					CASE WHEN ed.estuf <> ''
						 THEN '<a onclick=\"atualiza_div( \'obras\', \'' || ed.estuf || '\');\" style=\"cursor:pointer;\">' || ed.estuf || '</a>'
						 ELSE 'Não Informado' END as estado,
					count(oi.obrid) as total
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				WHERE
					oi.obsstatus = 'A' AND
					oi.orgid = {$orgid}
				GROUP BY
					estuf
				ORDER BY
					estuf";

    }

    $cabecalho = array($titulo, "Qtd de Obras");
    $db->monta_lista_simples($sql, $cabecalho, 1000, 30, 'S', '100%');

}

/**
 * Exibe lista das obras cadastradas nas unidade informada e com o tipo de ensino
 *
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param integer $entid
 * @param integer $orgid
 *
 */
function academico_lista_indicadores($entid)
{

    global $db;

    // verifica se é uma unidade ou um campus
    $unidade = $db->pegaUm("SELECT obrid FROM obras.obrainfraestrutura where entidunidade = {$entid}");

    // monta quadro do painel em modelo de unidade
    academico_cabecalho_quadro_painel($entid);


    $sql = "SELECT
				idcdsc,
				--'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2002' AND iettabnum=1),'999g999g999g999d'))||'</center>' as ano2002,
				--'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2003' AND iettabnum=1),'999g999g999g999d'))||'</center>' as ano2003,
				--'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2004' AND iettabnum=1),'999g999g999g999d'))||'</center>' as ano2004,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2005' AND iettabnum=1),'990D00'))||'</center>' as ano2005,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2006' AND iettabnum=1),'990D00'))||'</center>' as ano2006,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2007' AND iettabnum=1),'990D00'))||'</center>' as ano2007,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2008' AND iettabnum=1),'990D00'))||'</center>' as ano2008,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2009' AND iettabnum=1),'990D00'))||'</center>' as ano2009,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2010' AND iettabnum=1),'990D00'))||'</center>' as ano2010,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2011' AND iettabnum=1),'990D00'))||'</center>' as ano2011,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2012' AND iettabnum=1),'990D00'))||'</center>' as ano2012,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2013' AND iettabnum=1),'990D00'))||'</center>' as ano2013,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2014' AND iettabnum=1),'990D00'))||'</center>' as ano2014,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2015' AND iettabnum=1),'990D00'))||'</center>' as ano2015,
				'<center>'||trim(to_char((SELECT SUM(ietvalor) FROM academico.iesindicador WHERE entid='" . $entid . "' AND idcid=i.idcid AND ietano='2016' AND iettabnum=1),'990D00'))||'</center>' as ano2016
			FROM academico.indicador i
			WHERE idcstatus = 'A'
			ORDER BY i.idcdsc";

    //$cabecalho = array("Indicador", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", "2010", "2011", "2012", "2013", "2014", "2015", "2016");
    $cabecalho = array("Indicador", "2005", "2006", "2007", "2008", "2009", "2010", "2011", "2012", "2013", "2014", "2015", "2016");
    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', $par2);

}

/**
 * Exibe lista das obras cadastradas nas unidade informada e com o tipo de ensino
 *
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param integer $entid
 * @param integer $orgid
 *
 */
function academico_lista_obras($entid, $orgid)
{

    global $db;

    // verifica se é uma unidade ou um campus
    $unidade = $db->pegaUm("SELECT obrid FROM obras.obrainfraestrutura where entidunidade = {$entid}");

    if (!empty($unidade)) {

        $sql = "SELECT
					oi.stoid,
					oi.obrid,
					oi.obrdesc as nome,
					oi.obrdtinicio,
					oi.obrdttermino,
					tm.mundescricao||'/'||ed.estuf as local,
					case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
					(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
					oi.obrcomposicao
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid
				LEFT JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				LEFT JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE
					oi.entidunidade = {$entid} AND oi.orgid = {$orgid} AND oi.obsstatus = 'A'
				ORDER BY
					oi.obrdesc, so.stodesc";

        $obras = $db->carregar($sql);

        // monta quadro do painel em modelo de unidade
        academico_cabecalho_quadro_painel($entid);

    } else {

        $sql = "SELECT
					oi.stoid,
					oi.obrid,
					oi.obrdesc as nome,
					oi.obrdtinicio,
					oi.obrdttermino,
					tm.mundescricao||'/'||ed.estuf as local,
					trim(ed.medlatitude) as latitude,
					trim(ed.medlongitude) as longitude,
					case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
					(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
					oi.obrcomposicao
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid
				LEFT JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				LEFT JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE
					oi.entidcampus = {$entid}  AND oi.orgid = {$orgid} AND oi.obsstatus = 'A'
				ORDER BY
					oi.obrdesc, so.stodesc";

        $obras = $db->carregar($sql);

        // monta quadro do painel em modelo de campus
        academico_cabecalho_quadro_painel($entid, 'campus', $orgid);

    }
    ?>
    <table width="98%" cellSpacing="1" cellPadding="3" align="center"
           style="border:1px solid #ccc; background-color:#fff;">
        <tr>
            <td>
                <div id="quadrosituacao1" style="width:100%; border:1px solid #cccccc;"/>
                <table cellspacing="1" cellpadding="3" width="100%">
                    <tr>
                        <td style="text-align: center; background-color: #dedede; font-weight: bold;"> Resumo de Obras
                        </td>
                    </tr>
                    <tr>
                        <td style="padding: 0px; margin: 0px;">
                            <? academico_situacao_obras($_SESSION['academico']['orgid'], '', $entid) ?>
                        </td>
                    </tr>
                </table>
                </div>
            </td>
        </tr>
    </table>
    <div
        style="width: 97%; margin-top: 5px; margin-bottom: 1px; padding:3px; text-align: center; background-color: #dedede; font-weight: bold;">
        Lista de Obras
    </div>

    <?php
    if ($obras[0]) {
        $zoom = "<input type='hidden' id='endzoom'  value='15'/>";
        foreach ($obras as $obr) {

            switch ($obr["stoid"]) {

                case "1":
                    $obr['situacao'] = '<label style="color:#00AA00">' . $obr['situacao'] . '</label>';
                    break;
                case "2":
                    $obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
                    break;
                case "3":
                    $obr['situacao'] = '<label style="color:blue">' . $obr['situacao'] . '</label>';
                    break;
                case "6":
                    $obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
                    break;

            }

            // latitude
            $dadoslatitude = explode(".", $obr["latitude"]);
            $graulatitude = $dadoslatitude[0];
            $minlatitude = $dadoslatitude[1];
            $seglatitude = $dadoslatitude[2];
            $pololatitude = $dadoslatitude[3];

            $latitude = !empty($graulatitude) ? $graulatitude . 'º ' . $minlatitude . '\' ' . $seglatitude . '" ' . $pololatitude : 'Não Informado';

            $campograulatitude = "<input type='hidden' id='{$obr["obrid"]}graulatitude' value='{$graulatitude}'/>";
            $campominlatitude = "<input type='hidden' id='{$obr["obrid"]}minlatitude'  value='{$minlatitude}'/>";
            $camposeglatitude = "<input type='hidden' id='{$obr["obrid"]}seglatitude'  value='{$seglatitude}'/>";
            $campopololatitude = "<input type='hidden' id='{$obr["obrid"]}pololatitude' value='{$pololatitude}'/>";

            $camposhiddenlat = $campograulatitude . $campominlatitude . $campopololatitude . $camposeglatitude;

            // longitude
            $dadoslongitude = explode(".", $obr["longitude"]);
            $graulongitude = $dadoslongitude[0];
            $minlongitude = $dadoslongitude[1];
            $seglongitude = $dadoslongitude[2];

            $longitude = !empty($graulongitude) ? $graulongitude . 'º ' . $minlongitude . '\' ' . $seglongitude . '" W' : 'Não Informado';

            $campograulongitude = "<input type='hidden' id='{$obr["obrid"]}graulongitude' value='{$graulongitude}'/>";
            $campominlongitude = "<input type='hidden' id='{$obr["obrid"]}minlongitude'  value='{$minlongitude}'/>";
            $camposeglongitude = "<input type='hidden' id='{$obr["obrid"]}seglongitude'  value='{$seglongitude}'/>";

            $camposhiddenlog = $campograulongitude . $campominlongitude . $camposeglongitude;

            // visualizar mapa
            $mapa = !empty($graulatitude) && !empty($graulongitude) ? '<tr><td class="SubTituloDireita"></td><td><a style="cursor:pointer;" onclick="abreMapa(' . $obr["obrid"] . ');">Visualizar / Buscar No Mapa</a></td></tr>' : '';

            $obrid = "<input type='hidden' id='obrid'  value='{$obr["obrid"]}'/>";

//			print '
//			<div id="conteudolistaunidades1" style="width:100%;"/>
//				<!-- Quadro de situação da Obra -->
//				<div id="quadrosituacao1" class="caixa_redefederal"/>
//					<table cellspacing="1" cellpadding="3" style="width: 100%;">
//						<tr>
//							<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Obras </td>
//						</tr>
//						<tr>
//							<td style="padding: 0px; margin: 0px;">
//								' . academico_situacao_obras( $_SESSION['academico']['orgid'], '', $entid ) . '
//							</td>
//						</tr>
//					</table>
//				</div>
//
//			</div>';


            print '<form action="" method="post" id="formulario">'
                . '<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">'
                . '		<tr>'
                . '			<td class="SubTituloDireita" style="width:20%;">Nome da obra:</td><td colspan="2" style="width:45%;"><b>' . $obr['nome'] . $obrid . '</b></td>'
                . '			<td class="SubTituloDireita">Município/UF:</td><td>' . $obr['local'] . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Início programado:</td><td colspan="2">' . formata_data($obr['obrdtinicio']) . '</td>'
                . '			<td class="SubTituloDireita">Término programado:</td><td>' . formata_data($obr['obrdttermino']) . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Situação da Obra:</td><td colspan="2">' . $obr['situacao'] . '</td>'
                . '			<td class="SubTituloDireita">% Executado:</td><td colspan="2">' . $obr['percentual'] . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Latitude:</td><td colspan="2">' . $latitude . $camposhiddenlat . '</td>'
                . '			<td class="SubTituloDireita">Longitude:</td><td colspan="2">' . $longitude . $camposhiddenlog . $zoom . '</td>'
                . '		</tr>'
                . $mapa
                . '		<tr>'
                . '			<td class="SubTituloDireita">Descrição:</td><td colspan="4" align="justify">' . (($obr['obrcomposicao']) ? nl2br($obr['obrcomposicao']) : "Nenhuma observação inserida") . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloCentro" colspan="5">Fotos</td>'
                . '		</tr>';

            $sql = "SELECT
							arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao,
							to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao
						FROM
							public.arquivo arq
						INNER JOIN
							obras.arquivosobra oar ON arq.arqid = oar.arqid
						INNER JOIN
							obras.obrainfraestrutura obr ON obr.obrid = oar.obrid
						WHERE
							obr.obrid='" . $obr['obrid'] . "' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
						ORDER BY
							arq.arqid DESC LIMIT 4";

            $fotos = $db->carregar($sql);

            print '<tr>';

            if ($fotos[0]) {
                for ($k = 0; $k < count($fotos); $k++) {

                    $_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array("filtro" => "cnt.obrid=" . $uni['obrid'] . " AND aqostatus = 'A'",
                        "tabela" => "obras.arquivosobra");

                    print "<td valign=\"top\" align=\"center\">"
                        . "<img id='" . $fotos[$k]["arqid"] . "' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obr['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=" . $fotos[$k]["arqid"] . "' hspace='10' vspace='3' style='width:80px; height:80px;' onmouseover=\"return escape( '" . $fotos[$k]["arqdescricao"] . "' );\"/><br />"
                        . $fotos[$k]["aqodtinclusao"] . "<br />"
                        . $fotos[$k]["arqdescricao"]
                        . "</td>";

                }
            } else {
                print "<td colspan='5'>Não existe(m) foto(s) cadastrada(s).</td>";
            }

            print '		</tr>'
                . '</table>'
                . '</form>'
                . '<br/>';

        }
    } else {

        print '<tr><td align="center"><b>Não existe(m) Obra(s) cadastrada(s).</b></td></tr>';

    }

    // fecha quadro do painel
    print '	</table>'
        . '</div>';

}

/**
 * Controla as atualizações do quadro do painel de acordo com a ação passada
 *
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param string $acao
 * @param string $dado
 * @param integer $orgid
 *
 */
function academico_atualiza_quadro($acao, $dado, $orgid, $estuf = '', $entid = '')
{
    switch ($acao) {
        case 'listaindicadores':
            // exibe lista de obras
            academico_lista_indicadores($dado);
            die;
        case 'listaobras':
            // exibe lista de obras
            academico_lista_obras($dado, $orgid);
            die;
            break;
        case 'situacaoobras':
            // exibe lista de obras
            academico_dados_obras($dado, $orgid, $estuf, $entid);
            die;
            break;
        case 'dadosobras':
            // exibe lista de obras
            academico_dados_obras($dado, $orgid);
            die;
            break;
        case 'dadosunidade':
            // exibe dados da unidade
            academico_dados_unidade($dado, $orgid);
            die;
            break;
        case 'dadoscampus':
            // exibe dados do campus
            academico_dados_unidade($dado, $orgid);
            die;
        case 'dadosacademico':
            // exibe dados academico da unidade
            academico_dados_academico($dado, $orgid);
            die;
            break;
        case 'dadosconcursos':
            academico_dados_concursos($dado, $orgid);
            die;
            break;
        case 'dadosfinanceiro':
            academico_dados_financeiro($dado, $orgid);
            die;
            break;
        case 'mapa':
            // exibe novamente o mapa do brasil
            academico_abre_mapa($orgid);
            die;
            break;
        case 'listaCampus':
            // exibe campus da unidade
            academico_lista_campus($dado, $orgid);
            //academico_lista_campus_painel( $dado, $orgid );
            die;
            break;
        default:
            break;

    }

}

function academico_montaabas($itensMenu, $url = false, $acao = 'link')
{

    global $db;

    $url = $url ? $url : $_SERVER['REQUEST_URI'];
    $rs = (is_array($itensMenu)) ? $itensMenu : $db->carregar($itensMenu);


    $menu = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
        . '	<tr>'
        . '		<td>'
        . '			<table cellpadding="0" cellspacing="0" align="left">'
        . '				<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {

        extract($rs[$j]);

        $gifaba = ($j == 0) ? 'aba_nosel_ini.gif' : 'aba_nosel.gif';
        $giffundo_aba = 'aba_fundo_nosel.gif';
        $onclick = $acao == 'link' ? "window.location='" . $link . "'" : $link;

//		$giffundo_aba_mak = "aba_fundo_sel.gif";
//		$giffundo_aba = $giffundo_aba_mak;


        $menu .= '<td height="20" valign="top"><img src="../imagens/' . $gifaba . '" width="11" height="20" alt="" border="0"></td>'
            . '<td height="20" align="center" valign="middle" background="../imagens/' . $giffundo_aba . '" style="padding-left: 10px; padding-right: 10px;cursor:pointer;" onclick="' . $onclick . '">'
            . '<a>' . $descricao . '</b></a></td>';

    }

    $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="../imagens/' . $gifaba . '" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}

/**
 * Cria as abas que compõe o quadro do painel
 *
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param integer $entid
 * @param string $dados
 * @return mixed
 *
 */
function academico_abas_painel($entid, $dados = 'unidade')
{

    global $db;

    // cria o array com os dados do menu
    $menu_painel = array(0 => array("id" => 1, "descricao" => "Dados", "link" => "atualiza_div( '{$dados}', {$entid} );"),
        1 => array("id" => 2, "descricao" => "Concursos", "link" => "atualiza_div( 'concursos', {$entid} );"),
        2 => array("id" => 3, "descricao" => "Acadêmico", "link" => "atualiza_div( 'academico', {$entid} );"),
        3 => array("id" => 4, "descricao" => "Obras", "link" => "atualiza_div( 'obras', {$entid} );")
        //4 => array("id" => 5, "descricao" => "Previsto/Realizado", "link" => "atualiza_div( 'previstoRealizado', {$entid} );")
        //4 => array("id" => 5, "descricao" => "Financeiro", "link" => "atualiza_div( 'financeiro', {$entid} );")
    );
    if ($dados == 'unidade') {
        array_push($menu_painel, array("id" => 6, "descricao" => "Indicadores", "link" => "atualiza_div( 'indicadores', {$entid} );"));
        array_push($menu_painel, array("id" => 7, "descricao" => "Campus", "link" => "atualiza_div( 'listaCampus', {$entid} );"));
        array_push($menu_painel, array("id" => 8, "descricao" => "Financeiro", "link" => "atualiza_div( 'financeiro', {$entid} );"));
    }

    // retorna as abas
    return academico_montaabas($menu_painel, "atualiza_div( '{$dados}', {$entid} );", 'js');

}

/***
 * Cria o cabeçalho do quadro que compõe o painel com as informações da unidade
 *
 * @author Fernando A. Bagno da Silva
 * @since 15/07/2009
 * @param integer $entid
 * @param string $tipo
 * @param string $orgid
 *
 */
function academico_cabecalho_quadro_painel($entid, $tipo = 'unidade', $orgid = '', $obrid = '', $montaAbas = true, $links = true)
{
    global $db;

    // case seja no modelo campus
    if ($tipo == 'campus') {

        // verifica a função da entidade de acordo com o tipo de ensino
        switch ($orgid) {
            case '1':
                $funid = ACA_ID_CAMPUS;
                break;
            case '2':
                $funid = ACA_ID_UNED;
                break;
            case '3':
                $funid = ACA_ID_UNIDADES_VINCULADAS;
                break;
        }

        // pega id da unidade pai
        $idunidade = $db->pegaUm("SELECT ea.entid
								  FROM entidade.funcaoentidade ef
								  INNER JOIN entidade.entidade e ON e.entid = ef.entid
								  INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
								  WHERE ef.entid = {$entid} AND funid = {$funid}");

        // pega a descricao da unidade pai

        if ($idunidade) {
            if (!$montaAbas) {
                if ($links) $spart = "'<a target=\"_blank\" href=\"?modulo=principal/dadosentidade&acao=A&entidunidade=' || entid || '\" >' || UPPER(entnome) || '</a>'";
                else $spart = "UPPER(entnome)";
                $unidade = $db->pegaUm("SELECT " . $spart . " AS entnome FROM entidade.entidade WHERE entid={$idunidade}");
                $unidade = $unidade ? $unidade . '<br/>' : '';
            } else {
                if ($links) $spart = "'<a style=\"cursor:pointer\" title=\"Clique para abrir os dados da instituição\" onclick=\"atualiza_div( \'unidade\', \'' || entid || '\');\">' || UPPER(entnome) || '</a>'";
                else $spart = "UPPER(entnome)";
                $unidade = $db->pegaUm("SELECT " . $spart . " AS entnome FROM entidade.entidade WHERE entid={$idunidade}");
                $unidade = $unidade ? $unidade . '<br/>' : '';
            }
        }

    }

    if ($tipo == 'obra' && $obrid != '') {

        $obra = $db->pegaUm("SELECT obrdesc FROM obras.obrainfraestrutura WHERE obrid = {$obrid}");

    }
    if (!$montaAbas && $tipo != 'campus') {
        if ($links) $spart = "'<a target=\"_blank\" href=\"?modulo=principal/dadosentidade&acao=A&entidunidade=' || entid || '\" >' || UPPER(entnome) || '</a>'";
        else $spart = "UPPER(entnome)";
        // pega o nome das undiades e a sua caracterização
        $nome = $db->pegaUm("SELECT " . $spart . " FROM entidade.entidade WHERE entid={$entid}");
    } elseif (!$montaAbas && $tipo == 'campus') {
        if ($links) $spart = "'<a target=\"_blank\" href=\"?modulo=principal/dadoscampus&acao=A&entid=' || entid || '\" >' || UPPER(entnome) || '</a>'";
        else $spart = "UPPER(entnome)";
        // pega o nome das undiades e a sua caracterização
        $nome = $db->pegaUm("SELECT " . $spart . " FROM entidade.entidade WHERE entid={$entid}");
    } else {
        // pega o nome das undiades e a sua caracterização
        $nome = $db->pegaUm("SELECT  UPPER(entnome)  FROM entidade.entidade WHERE entid={$entid}");
    }

    // monta a tabela com os dados da unidade e as abas
    if ($montaAbas) {
        print '<table width="100%" style="text-align:center;">'
            . '	   <tr><td colspan="2" align="right"> <a onclick="displayMessage(\'?modulo=principal/mapapainel&acao=A\');" style="cursor:pointer;">Abrir Mapa</a> </td></tr>'
            . '    <tr><td>';
        print academico_abas_painel($entid, $tipo) . '';
        print '    </td><tr>';
    } else {
        print '<table width="100%" style="text-align:center;">';
    }
    print '<tr><td colspan="2" class="SubTituloCentro">' . $unidade . $nome . '</td></tr>'
        . '</table>'
        . '<div style="width:100%; border:1px solid #cccccc;">'
        . '<table width="100%">';

}

/**
 * Lista os campus da unidade selecionada
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @since 31/08/2009
 * @param integer $entid
 * @param integer $orgid
 *
 */
function academico_lista_campus($entid, $orgid)
{

    academico_cabecalho_quadro_painel($entid);
    print '		<tr><td>';
    academico_lista_campus_painel($entid, $orgid);
    print '		</td>'
        . '		</tr>'
        . '	</table>'
        . '</div>';
}


/**
 * Exibe os dados da unidade informada
 *
 * @author Fernando A. Bagno da Silva
 * @since 15/07/2009
 * @param integer $entid
 *
 */
function academico_dados_unidade($entid, $orgid)
{

    global $db;

    switch ($orgid) {
        case '1':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
            break;
        case '2':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
            break;
        case '3':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNIDADES_VINCULADAS);
            break;
    }
    // monta quadro do painel
    !empty($campus) ? academico_cabecalho_quadro_painel($entid, 'campus', $orgid) : academico_cabecalho_quadro_painel($entid);

    // pega a caracterização da unidade
    $edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid={$entid}");
    $edtdsc = $edtdsc ? $edtdsc : 'Não Informado';

    // cria a tabela com os dados da unidade
    print '    <tr><td class="SubTituloEsquerda" colspan="2">Caracterização da Instituição</td></tr>'
        . '    <tr><td colspan="2" style="text-align:justify;">' . $edtdsc . '</td></tr>'
        . '    <tr><td class="SubTituloEsquerda" colspan="2">Dados da Instituição</td>';

    // monta os dados do endereço da unidade
    academico_monta_endereco($entid);

    print '		</tr>';
    print '<tr><td class="SubTituloEsquerda" colspan="2">Dados do dirigente</td>';

    // monta dados dos dirigentes da unidade
    academico_monta_dirigente($entid, 'unidade', TPENSSUP);

    print '		</tr>'
        . '	</table>'
        . '</div>';

}

function academico_monta_endereco($id, $exibefotos = true)
{

    global $db;
    ?>
    <tr>
        <td class="SubTituloDireita">Endereço :</td>
        <td>
            <?php
            $linha = $db->pegaLinha("SELECT * FROM entidade.endereco en
							 LEFT JOIN territorios.municipio mun ON mun.muncod = en.muncod
							 WHERE entid={$id}");

            $dadosendvazio = true;
            unset($endes);
            if (trim($linha['endlog'])) {
                $endes[] = $linha['endlog'];
                $dadosendvazio = false;
            }
            if (trim($linha['endnum'])) {
                $endes[] = " número " . $linha['endnum'];
                $dadosendvazio = false;
            }
            if (trim($linha['endbai'])) {
                $endes[] = $linha['endbai'];
                $dadosendvazio = false;
            }
            if (trim($linha['estuf'])) {
                $endes[] = $linha['estuf'];
                $dadosendvazio = false;
            }
            if (trim($linha['endcep'])) {
                $endes[] = "CEP " . $linha['endcep'];
                $dadosendvazio = false;
            }
            if ($dadosendvazio) {
                echo "Não informado";
            } else {
                echo implode(", ", $endes);
            }
            ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" nowrap>Município/UF :</td>
        <td><?= $linha['mundescricao'] . "/" . $linha['estuf'] ?></td>
    </tr>
    <tr>
    <td class="SubTituloDireita" nowrap>Municípios limítrofes :</td>
    <td><?php
        if ($linha['muncod']) {
            unset($dadosvizinhos, $vizs);
            $sql = "SELECT mun.mundescricao FROM territorios.municipiosvizinhos muv
				LEFT JOIN territorios.municipio mun ON muv.muncodvizinho = mun.muncod
				WHERE muv.muncod='" . $linha['muncod'] . "'";

            $dadosvizinhos = $db->carregar($sql);
            if ($dadosvizinhos[0]) {
                foreach ($dadosvizinhos as $viz) {
                    $vizs[] = $viz['mundescricao'];
                }
                echo implode(", ", $vizs);
            } else {
                echo "Não informado";
            }
        } else {
            echo "Não informado";
        }
        ?></td></tr><?php
    //exibe fotos
    require_once APPRAIZ . "includes/classes/entidades.class.inc";
    $entidade = new Entidades();
    $entidade->carregarEntidade(array("entid" => $id));
    $fotos = $entidade->carregarFotosEntidades();
    if (!$exibefotos) $fotos = false;
    $_SESSION['downloadfiles']['pasta'] = array("origem" => "entidades", "destino" => "entidades");
    $_SESSION['imgparams'] = array("filtro" => "cnt.entid =" . $id, "tabela" => "entidade.fotoentidade");

    if ($fotos) {
        print '<tr><td class="SubTituloEsquerda" colspan="2">Fotos da Instituição</td>';
        print '</tr>';
        print '<tr><td colspan=2 >';
        $numFotos = 0;
        foreach ($fotos as $foto) {
            $numFotos++;
            print '<img id="' . $foto['arqid'] . '" src="../slideshow/slideshow/verimagem.php?arqid=' . $foto['arqid'] . '&_sisarquivo=entidades" style="padding:5px;height:68px;width:68px;margin: 0px;opacity: 1" class="imageBox_theImage" title="' . $foto['arqdescricao'] . '" onclick="javascript:window.open(\'../slideshow/slideshow/index.php?pagina=' . $pagina . '&arqid=' . $foto['arqid'] . '&_sisarquivo=entidades\',\'imagem\',\'width=850,height=600,resizable=yes\')" />';
            if ($numFotos == 5)
                break;
        }
        print '</td></tr>';
    }
}


function academico_monta_dirigente($id, $ent, $tipoensino)
{

    global $db, $_funcoes;

    $dirigente = $db->pegaLinha("SELECT ent.*, fundsc FROM entidade.entidade ent
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
								 LEFT JOIN entidade.funcao ef ON ef.funid = fen.funid
								 WHERE fea.entid = '" . $id . "' AND fen.funid='" . $_funcoes[$tipoensino][$ent] . "'");
    if ($dirigente) {
        ?>
        <tr>
            <td class="SubTituloDireita">Nome :</td>
            <td><?php echo(($dirigente['entnome']) ? $dirigente['entnome'] : "Não informado"); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" nowrap>Função / Ocupação :</td>
            <td><?php echo(($dirigente['fundsc']) ? $dirigente['fundsc'] : "Não informado"); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Telefones :</td>
            <td><?php
                $dadostelvazio = true;
                if ($dirigente['entnumresidencial']) {
                    $tels[] = "Residencial: (" . trim($dirigente['entnumdddresidencial']) . ") " . trim($dirigente['entnumresidencial']);
                    $dadostelvazio = false;
                }
                unset($ramal);
                if ($dirigente['entnumcomercial']) {
                    if (trim($dirigente['entnumramalcomercial']))
                        $ramal = " ramal " . trim($dirigente['entnumramalcomercial']);
                    $tels[] = "Comercial: (" . trim($dirigente['entnumdddcomercial']) . ") " . trim($dirigente['entnumcomercial']) . $ramal;
                    $dadostelvazio = false;
                }
                unset($ramal);
                if ($dirigente['entnumfax']) {
                    if (trim($dirigente['entnumramalfax']))
                        $ramal = " ramal " . trim($dirigente['entnumramalfax']);
                    $tels[] = "Fax: (" . trim($dirigente['entnumdddfax']) . ") " . trim($dirigente['entnumfax']) . $ramal;
                    $dadostelvazio = false;
                }
                if ($dadostelvazio) {
                    echo "Não informado";
                } else {
                    echo implode(", ", $tels);
                }
                ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">E-mail :</td>
            <td><?php echo(($dirigente['entemail']) ? $dirigente['entemail'] : "Não informado"); ?></td>
        </tr>
    <?php
    } else {
        ?>
        <tr>
            <td colspan="2">Não Informado</td>
        </tr><?php
    }
}

function academico_dados_academico($entid, $orgid)
{

    global $db, $anosanalisados, $tituloitens;

    switch ($orgid) {
        case '1':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
            break;
        case '2':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
            break;
        case '3':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNIDADES_VINCULADAS);
            break;
    }

    if (!empty($campus)) {

        academico_cabecalho_quadro_painel($entid, 'campus', $orgid);
        echo "<table class=\"tabela\" bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center width=95%>";
        echo "<tr>
				<td class=SubTituloCentro>Situação atual (até 2009)</td>
				<td class=SubTituloCentro>Até";

        $dados = array(0 => array('codigo' => '2010', 'descricao' => '2010'),
            1 => array('codigo' => '2011', 'descricao' => '2011'),
            2 => array('codigo' => '2012', 'descricao' => '2012'));
        $db->monta_combo('anosit', $dados, 'S', '', 'selecionaranocomparacao2', '', '', '', 'N', 'anosit');

        echo "</td></tr>";

        echo "<tr><td>";
        echo "<center>TOTAL</center>";
        academico_situacao_atual($orgid, array('entid2' => $entid));
        echo "<br />";
        if ($orgid == TPENSSUP) {
            echo "<center>REUNI</center>";
            academico_situacao_atual($orgid, array('entid2' => $entid), 0);
        }
        echo "</td><td>";
        echo "<center>TOTAL</center>";
        academico_situacao_atual_comparacao($orgid, array('entid2' => $entid), null, 'xx');
        echo "<br />";
        if ($_SESSION['academico']['orgid'] == TPENSSUP) {
            echo "<center>REUNI</center>";
            academico_situacao_atual_comparacao($orgid, array('entid2' => $entid), 0, 'xx');
        }
        echo "</td></tr></table>";

    } else {

        academico_cabecalho_quadro_painel($entid);
        echo "<table class=\"tabela\" bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center width=95%>";
        echo "<tr>
				<td class=SubTituloCentro>Situação atual (até 2009)</td>
				<td class=SubTituloCentro>Até ";

        $dados = array(0 => array('codigo' => '2010', 'descricao' => '2010'),
            1 => array('codigo' => '2011', 'descricao' => '2011'),
            2 => array('codigo' => '2012', 'descricao' => '2012'));
        $db->monta_combo('anosit', $dados, 'S', '', 'selecionaranocomparacao2', '', '', '', 'N', 'anosit');

        echo "</td></tr>";

        echo "<tr><td>";

        echo "<center>TOTAL</center>";
        academico_situacao_atual($orgid, array('entid' => $entid));
        echo "<br />";
        if ($orgid == TPENSSUP) {
            echo "<center>REUNI</center>";
            academico_situacao_atual($orgid, array('entid' => $entid), 0);
        }

        echo "</td><td>";
        echo "<center>TOTAL</center>";
        academico_situacao_atual_comparacao($orgid, array('entid' => $entid), 1, 'xx');
        echo "<br />";
        if ($_SESSION['academico']['orgid'] == TPENSSUP) {
            echo "<center>REUNI</center>";
            academico_situacao_atual_comparacao($orgid, array('entid' => $entid), 0, 'xx');
        }
        echo "</td></tr></table>";


    }


}

function academico_abre_mapa($orgid)
{

    echo '<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
			<map name="mapaBrasil" id="mapaBrasil">
				<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="academico_listaUnidades(\'todos\', ' . $orgid . ');" title="Brasil"/>
				<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="academico_listaUnidades(\'AC\', ' . $orgid . ');" title="Acre"/>
				<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="academico_listaUnidades(\'AL\', ' . $orgid . ');" title="Alagoas"/>
				<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="academico_listaUnidades(\'AP\', ' . $orgid . ');" title="Amapá"/>
				<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="academico_listaUnidades(\'AM\', ' . $orgid . ');" title="Amazonas"/>
				<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="academico_listaUnidades(\'BA\', ' . $orgid . ');" title="Bahia"/>
				<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="academico_listaUnidades(\'CE\', ' . $orgid . ');" title="Ceará"/>
				<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="academico_listaUnidades(\'DF\', ' . $orgid . ');" title="Distrito Federal"/>
				<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="academico_listaUnidades(\'ES\', ' . $orgid . ');" title="Espírito Santo"/>
				<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="academico_listaUnidades(\'GO\', ' . $orgid . ');" title="Goiás"/>
				<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="academico_listaUnidades(\'MT\', ' . $orgid . ');" title="Mato Grosso"/>
				<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="academico_listaUnidades(\'MS\', ' . $orgid . ');" title="Mato Grosso do Sul"/>
				<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="academico_listaUnidades(\'MA\', ' . $orgid . ');" title="Maranhão"/>
				<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="academico_listaUnidades(\'MG\', ' . $orgid . ');" title="Minas Gerais"/>
				<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="academico_listaUnidades(\'PA\', ' . $orgid . ');" title="Pará"/>
				<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="academico_listaUnidades(\'PB\', ' . $orgid . ');" title="Paraíba"/>
				<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="academico_listaUnidades(\'PR\', ' . $orgid . ');" title="Paraná"/>
				<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="academico_listaUnidades(\'PE\', ' . $orgid . ');" title="Pernambuco"/>
				<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="academico_listaUnidades(\'PI\', ' . $orgid . ');" title="Piauí"/>
				<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="academico_listaUnidades(\'RN\', ' . $orgid . ');" title="Rio Grande do Norte"/>
				<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="academico_listaUnidades(\'RS\', ' . $orgid . ');" title="Rio Grande do Sul"/>
				<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="academico_listaUnidades(\'RJ\', ' . $orgid . ');" title="Rio de Janeiro"/>
				<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="academico_listaUnidades(\'RO\', ' . $orgid . ');" title="Rondônia"/>
				<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="academico_listaUnidades(\'RR\', ' . $orgid . ');" title="Roraima"/>
				<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="academico_listaUnidades(\'SC\', ' . $orgid . ');" title="Santa Catarina"/>
				<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="academico_listaUnidades(\'SP\', ' . $orgid . ');" title="São Paulo"/>
				<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="academico_listaUnidades(\'SE\', ' . $orgid . ');" title="Sergipe"/>
				<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="academico_listaUnidades(\'TO\', ' . $orgid . ');" title="Tocantins"/>
				<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="academico_listaUnidades(\'norte\', ' . $orgid . ');" title="Norte" />
				<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="academico_listaUnidades(\'nordeste\', ' . $orgid . ');" title="Nordeste" />
				<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="academico_listaUnidades(\'centrooeste\', ' . $orgid . ');" title="Centro-Oeste" />
				<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="academico_listaUnidades(\'sudeste\', ' . $orgid . ');" title="Sudeste" />
				<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="academico_listaUnidades(\'sul\', ' . $orgid . ');" title="Sul" />
			</map>';

}

function academico_painel_agrupador()
{

    $agrupador = array("ano", "classe");

    if (!$agrupador) {
        $agrupador = array(
            'tipoensino',
            /*							'ano',
							'portaria',
							'unidade',
							'campus',
							'programa',
							'classe'*/
        );
    } elseif (!is_array($agrupador)) {
        $agrupador = explode(",", $agrupador);
    }

    $agp = array(
        "agrupador" => array(),
        "agrupadoColuna" => array(
            "projetado",
            "autorizado",
            "provimento",
            "publicado",
            "homologado",
            "lepvlrprovefetivados",
            "provimentosnaoefetivados",
            "provimentopendencia",
            "homocolor"
        )
    );

    foreach ($agrupador as $val):
        switch ($val) {
            case 'tipoensino':
                array_push($agp['agrupador'], array(
                        "campo" => "tipoensino",
                        "label" => "Tipo Ensino")
                );
                continue;
                break;
            case 'portaria':
                array_push($agp['agrupador'], array(
                        "campo" => "portaria",
                        "label" => "Portaria")
                );
                continue;
                break;
            case 'campus':
                array_push($agp['agrupador'], array(
                        "campo" => "campus",
                        "label" => "Campus")
                );
                continue;
                break;
            case 'classe':
                array_push($agp['agrupador'], array(
                        "campo" => "classe",
                        "label" => "Classe")
                );
                continue;
                break;
            case 'programa':
                array_push($agp['agrupador'], array(
                        "campo" => "programa",
                        "label" => "Programa")
                );
                continue;
                break;
            case 'unidade':
                array_push($agp['agrupador'], array(
                        "campo" => "unidade",
                        "label" => "Unidade")
                );
                continue;
                break;
            case 'ano':
                array_push($agp['agrupador'], array(
                        "campo" => "ano",
                        "label" => "Ano")
                );
                continue;
                break;
        }
    endforeach;
    return $agp;

}

function academico_painel_sql($orgid, $entid)
{

    global $db;

    switch ($orgid) {
        case '1':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
            break;
        case '2':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
            break;
        case '3':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNIDADES_VINCULADAS);
            break;
    }

    $select = array();
    $from = array();

    $where[] = !empty($orgid) ? " orgid = {$orgid} " : "";

    if (!empty($entid)) {
        $where[] = !empty($campus) ? " idcampus = {$entid} " : " idunidade = {$entid}";
    }

    $agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador;

    //if (strpos($agrupador, "unidade") !== false){
    $select[] = "COALESCE(unidade , 'Não informado') as unidade";
    $select[] = "idunidade as idunidade";
    //}

    //if (strpos($agrupador, "campus") !== false){
    $select[] = "COALESCE(campus, 'Não informado') as campus";
    $select[] = "idcampus as idcampus";
    //}

    //if (strpos($agrupador, "classe") !== false){
    $select[] = "COALESCE(classe, 'Não informado') as classe";
    $select[] = "idclasse as idclasse";
    //}

    //if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
    $select[] = "tipoensino as tipoensino";
    //}

    //if (strpos($agrupador, "programa") !== false){
    $select[] = "programa as programa";
    $select[] = "idprograma as idprograma";
    //}

    //if (strpos($agrupador, "ano") !== false){
    $select[] = "ano";
    //}

    //if (strpos($agrupador, "portaria") !== false){
    $select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
    //}

    $sql = "select
				COALESCE(sum( projetado ),0) as projetado,
				COALESCE(sum( autorizado ),0) as autorizado,
				COALESCE(sum( provimentoautorizado ),0) as provimento,
				COALESCE(sum( publicado ),0) as publicado,
				COALESCE(sum( homologado ),0) as homologado,
				COALESCE(sum(lepvlrprovefetivados),0) as lepvlrprovefetivados,
				COALESCE((sum( provimentoautorizado ) - sum(lepvlrprovefetivados)),0) as provimentosnaoefetivados,
				--(sum( homologado ) - (sum( provimentoautorizado ) - sum(lepvlrprovefetivados)) ) AS provimentopendencia,
				CASE
					WHEN COALESCE((COALESCE(sum( homologado ), 0) - COALESCE(sum(provimentoautorizado), 0)),0) >= 0 THEN COALESCE((COALESCE(sum( homologado ), 0) - COALESCE(sum(provimentoautorizado), 0)),0)
					ELSE 0
				END AS provimentopendencia,
				" . (implode(" , ", $select)) . "
			from
			(


				SELECT
				p.prtid as portaria,
				p.prtnumero as numeroportaria,
				p.prtano as ano,
				0 as projetado,
				0 as autorizado,
				0 as provimentoautorizado,
				sum(lepvlrpublicacao) as publicado,
				sum( lepvlrhomologado ) as homologado,
				sum(lepvlrprovefetivados) as lepvlrprovefetivados,
				COALESCE(ent.entnome , 'Não informado') as unidade ,
				ent.entid as idunidade,
				COALESCE(cp.entnome, 'Não informado') as campus ,
				cp.entid as idcampus,
				COALESCE(cl.clsdsc, 'Não informado') as classe ,
				cl.clsid as idclasse,
				ao.orgdesc as tipoensino ,
				ao.orgid,
				pp.prgdsc as programa,
				p.prgid as idprograma
				FROM
					academico.portarias p
				inner join academico.editalportaria ep ON ep.prtid = p.prtid
				inner join academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid
																	 AND lep.lepstatus = 'A'
				INNER JOIN entidade.entidade cp ON cp.entid = ep.entidcampus
				INNER JOIN entidade.entidade ent ON ent.entid = ep.entidentidade
				inner join academico.cargos c ON c.crgid = lep.crgid
				inner join academico.classes cl ON cl.clsid = c.clsid
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				where
					p.tprid = " . ACA_TPORTARIA_CONCURSO . "
					AND p.prtstatus = 'A'
					AND ep.edpstatus = 'A'
					AND ep.tpeid in ( " . ACA_TPEDITAL_PUBLICACAO . ", " . ACA_TPEDITAL_HOMOLOGACAO . ", " . ACA_TPEDITAL_NOMEACAO . " )
				group by
					p.prtid ,
					p.prtnumero,
					p.prtano,
					COALESCE(ent.entnome , 'Não informado'),
					ent.entid,
					COALESCE(cp.entnome, 'Não informado'),
					cp.entid,
					COALESCE(cl.clsdsc, 'Não informado') ,
					cl.clsid,
					ao.orgdesc ,
					ao.orgid,
					pp.prgdsc,
					p.prgid



			union all


				SELECT
					p.prtid as portaria,
					p.prtnumero as numeroportaria,
					p.prtano as ano,
					ap.acpvalor as projetado,
					lp.lnpvalor as autorizado,
					0 as provimentoautorizado,
					0 as publicado ,
					0 as homologado	,
					0 as lepvlrprovefetivados,
					COALESCE(ent.entnome , 'Não informado') as unidade ,
					ent.entid as idunidade,
					COALESCE(c.entnome, 'Não informado') as campus ,
					c.entid as idcampus,
					COALESCE(cl.clsdsc, 'Não informado') as classe ,
					cl.clsid as idclasse,
					ao.orgdesc as tipoensino ,
					ao.orgid,
					pp.prgdsc as programa,
					p.prgid as idprograma
				FROM
					academico.portarias p
				INNER JOIN
					( SELECT
						prtid, entidcampus, entidentidade, clsid, sum(lnpvalor) as lnpvalor
					  FROM
						academico.lancamentosportaria a
					  WHERE
					  	a.lnpstatus = 'A'
					  GROUP BY
						prtid, entidcampus, entidentidade, clsid ) lp ON lp.prtid = p.prtid
																		 --and lp.lnpano = p.prtano
				INNER JOIN ( SELECT
								prtid, entidcampus, entidentidade, clsid, sum(acpvalor) as acpvalor
							  FROM
								academico.acumuladoprojetado a
							  GROUP BY
								prtid, entidcampus, entidentidade, clsid ) ap ON ap.prtid = p.prtid


				INNER JOIN entidade.entidade ent ON ent.entid = lp.entidentidade and ent.entid = ap.entidentidade
				INNER JOIN entidade.entidade c ON c.entid = lp.entidcampus and c.entid = ap.entidcampus
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = c.entid
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid and fea.entid = ent.entid

				INNER JOIN academico.classes cl ON cl.clsid = lp.clsid and cl.clsid = ap.clsid AND cl.clsstatus = 'A'
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				where
					p.tprid = " . ACA_TPORTARIA_CONCURSO . "
					AND p.prtstatus = 'A'


			union all

				SELECT
					p2.prtid as portaria,
					p2.prtnumero as numeroportaria,
					p2.prtano as ano,
					0 as projetado,
					0 as autorizado,
					lp.lnpvalor as provimentoautorizado,
					0 as publicado ,
					0 as homologado	,
					0 as lepvlrprovefetivados,
					COALESCE(ent.entnome , 'Não informado') as unidade ,
					ent.entid as idunidade,
					COALESCE(c.entnome, 'Não informado') as campus ,
					c.entid as idcampus,
					COALESCE(cl.clsdsc, 'Não informado') as classe ,
					cl.clsid as idclasse,
					ao.orgdesc as tipoensino ,
					ao.orgid,
					pp.prgdsc as programa,
					p.prgid as idprograma
				FROM
					academico.portarias p
				inner join academico.portarias p2 ON p.prtidautprov = p2.prtid
													 AND p2.prtstatus = 'A'
				INNER JOIN
					( SELECT
						prtid, entidcampus, entidentidade, clsid, sum(lnpvalor) as lnpvalor
					  FROM
						academico.lancamentosportaria a
					  WHERE
					  	a.lnpstatus = 'A'
					  GROUP BY
						prtid, entidcampus, entidentidade, clsid ) lp ON lp.prtid = p.prtid --and lp.lnpano = p.prtano
				inner JOIN entidade.entidade ent ON ent.entid = lp.entidentidade
				inner JOIN entidade.entidade c ON c.entid = lp.entidcampus
				INNER JOIN academico.classes cl ON cl.clsid = lp.clsid AND cl.clsstatus = 'A'
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				WHERE
					p.tprid = " . ACA_TPORTARIA_PROVIMENTO . "
					AND p.prtstatus = 'A'
			) as foo
			" . (is_array($where) ? " WHERE " . implode(' AND ', $where) : '') . "
			group by
				portaria,
				numeroportaria,
				unidade,
				idunidade,
				campus,
				idcampus,
				classe,
				idclasse,
				tipoensino,
				orgid,
				programa,
				idprograma,
				ano
			order by
				ano";

    return $sql;

}

function academico_painel_coluna()
{

    $arrPerfil = array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR);
    $permissao = academico_possui_perfil($arrPerfil);

    $coluna = array(
        array(
            "campo" => "autorizado",
            "label" => "Concursos <br/>Autorizados <br/>(B)",
            "type" => "numeric"
        ),
        array(
            "campo" => "publicado",
            "label" => "Concursos <br/>Publicados <br/>(C)",
            "type" => "numeric"
        )
    );

    if ($permissao) {
        array_push($coluna, array(
                "campo" => "homologado",
                "label" => "Concursos <br/>Homologados <br/>(D)",
                "type" => "numeric",
                "html" => "<span style='color:{color}'>{homologado}</span>",
                "php" => array(
                    "expressao" => "{homologado} > {autorizado}",
                    "var" => "color",
                    "true" => "red",
                    "false" => "#0066CC",
                )
            )
        );
    } else {
        array_push($coluna, array(
                "campo" => "homologado",
                "label" => "Concursos <br/>Homologados <br/>(D)",
                "type" => "numeric"
            )
        );

    }

    array_push($coluna, array(
            "campo" => "provimento",
            "label" => "Provimentos <br/>Autorizados <br/>(E)",
            "type" => "numeric"
        ),
        array(
            "campo" => "lepvlrprovefetivados",
            "label" => "Provimentos <br/>Efetivados <br/>(F)",
            "type" => "numeric"
        )
    );


    return $coluna;

}

function academico_dados_concursos($entid, $orgid)
{

    global $db;

    switch ($orgid) {
        case '1':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
            break;
        case '2':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
            break;
        case '3':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNIDADES_VINCULADAS);
            break;
    }

    // monta quadro do painel
    !empty($campus) ? academico_cabecalho_quadro_painel($entid, 'campus', $orgid) : academico_cabecalho_quadro_painel($entid);

    $sql = academico_painel_sql($orgid, $entid);
    $dados = $db->carregar($sql);
    $agrup = academico_painel_agrupador();
    $col = academico_painel_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao($true ? true : false);
    $r->setTotNivel(true);

    echo $r->getRelatorio();


}

function academico_dados_financeiro($entid, $orgid)
{

    global $db;

    switch ($orgid) {
        case '1':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_CAMPUS);
            break;
        case '2':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNED);
            break;
        case '3':
            $campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . ACA_ID_UNIDADES_VINCULADAS);
            break;
    }

    $unicod = $db->pegaUm("SELECT entunicod FROM entidade.entidade WHERE entid='{$entid}'");
    // monta quadro do painel
    !empty($campus) ? academico_cabecalho_quadro_painel($entid, 'campus', $orgid) : academico_cabecalho_quadro_painel($entid);


    // Parâmetros para a nova conexão com o banco do SIAFI
    $servidor_bd = $servidor_bd_siafi;
    $porta_bd = $porta_bd_siafi;
    $nome_bd = $nome_bd_siafi;
    $usuario_db = $usuario_db_siafi;
    $senha_bd = $senha_bd_siafi;

    // Cria o novo objeto de conexão
    $db2 = new cls_banco();

    $sql = array(0 => array("acao" => "<center><img style=\"cursor:pointer;\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio_novo('564', '{$unicod}');\"></center>",
        "descricao" => "Execução por Grupo de Natureza de Despesa (GND)"),
        1 => array("acao" => "<center><img style=\"cursor:pointer;\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio_novo('565', '{$unicod}');\"></center>",
            "descricao" => "Execução por Fonte"),
        2 => array("acao" => "<center><img style=\"cursor:pointer;\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio_novo('566', '{$unicod}');\"></center>",
            "descricao" => "Execução por Programa/Ação"),

    );

    $cabecalho = array("Ação", "Relatórios");
    $db->monta_lista_simples($sql, $cabecalho, 1000, 30, 'N', '100%');

}

function academico_dados_obras($stoid, $orgid, $estuf = '', $entid = '')
{

    global $db;

    $situacao = !empty($stoid) ? $db->pegaUm("SELECT stodesc FROM obras.situacaoobra WHERE stoid = {$stoid}") : 'Não Informado';

    // monta a tabela com os dados da unidade e as abas
    print '<table width="505px" style="text-align:center;">'
        . '    <tr><td>'
        . '    </td><td align="right"> <a onclick="displayMessage(\'?modulo=principal/mapapainel&acao=A\');" style="cursor:pointer;">Abrir Mapa</a> </td></tr>'
        . '    <tr><td colspan="2" class="titulounidade"> Lista de Obras</td></tr>'
        . '</table>'
        . '<div style="overflow:auto; height:280px; width:500px; border:1px solid #cccccc;">'
        . '<table width="100%">';

    $where_estuf = !empty($estuf) ? "AND ed.estuf = '{$estuf}'" : "";
    $where_stoid = !empty($stoid) ? "oi.stoid = " . $stoid : 'oi.stoid is null';
    $where_entid = !empty($entid) ? "AND oi.entidunidade = " . $entid : "";

    $sql = "SELECT
				oi.stoid,
				oi.obrid,
				case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
				obrdesc as nome,
				to_char(oi.obrdtinicio, 'dd/mm/yyyy') as inicio,
				to_char(oi.obrdttermino, 'dd/mm/yyyy') as termino,
				tm.mundescricao||'/'||ed.estuf as local,
				e.entnome as unidade,
				trim(ed.medlatitude) as latitude,
				trim(ed.medlongitude) as longitude,
				(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
				obrcomposicao
			FROM
				obras.obrainfraestrutura oi
			LEFT JOIN
				obras.situacaoobra so ON oi.stoid = so.stoid
			INNER JOIN
				entidade.entidade e ON e.entid = oi.entidunidade
			LEFT JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			LEFT JOIN
				territorios.municipio tm ON ed.muncod = tm.muncod
			WHERE
				{$where_stoid}
				AND oi.orgid = {$orgid}
				{$where_estuf} AND
				oi.obsstatus = 'A'
				{$where_entid}
			ORDER BY
				unidade, nome, situacao";

    $obras = $db->carregar($sql);

    if ($obras[0]) {
        $zoom = "<input type='hidden' id='endzoom'  value='15'/>";
        foreach ($obras as $obr) {

            switch ($obr["stoid"]) {
                case "1":
                    $obr['situacao'] = '<label style="color:#00AA00">' . $obr['situacao'] . '</label>';
                    break;
                case "2":
                    $obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
                    break;
                case "3":
                    $obr['situacao'] = '<label style="color:blue">' . $obr['situacao'] . '</label>';
                    break;
                case "6":
                    $obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
                    break;
            }

            $dadoslatitude = explode(".", $obr["medlatitude"]);
            $graulatitude = $dadoslatitude[0];
            $minlatitude = $dadoslatitude[1];
            $seglatitude = $dadoslatitude[2];
            $pololatitude = $dadoslatitude[3];

            $latitude = !empty($graulatitude) ? $graulatitude . 'º ' . $minlatitude . '\' ' . $seglatitude . '" ' . $pololatitude : 'Não Informado';

            $campograulatitude = "<input type='hidden' id='{$obr["obrid"]}graulatitude' value='{$graulatitude}'/>";
            $campominlatitude = "<input type='hidden' id='{$obr["obrid"]}minlatitude'  value='{$minlatitude}'/>";
            $camposeglatitude = "<input type='hidden' id='{$obr["obrid"]}seglatitude'  value='{$seglatitude}'/>";
            $campopololatitude = "<input type='hidden' id='{$obr["obrid"]}pololatitude' value='{$pololatitude}'/>";

            $camposhiddenlat = $campograulatitude . $campominlatitude . $campopololatitude . $camposeglatitude;

            $dadoslongitude = explode(".", $obr["medlongitude"]);
            $graulongitude = $dadoslongitude[0];
            $minlongitude = $dadoslongitude[1];
            $seglongitude = $dadoslongitude[2];

            $longitude = !empty($graulongitude) ? $graulongitude . 'º ' . $minlongitude . '\' ' . $seglongitude . '" W' : 'Não Informado';

            $campograulongitude = "<input type='hidden' id='{$obr["obrid"]}graulongitude' value='{$graulongitude}'/>";
            $campominlongitude = "<input type='hidden' id='{$obr["obrid"]}minlongitude'  value='{$minlongitude}'/>";
            $camposeglongitude = "<input type='hidden' id='{$obr["obrid"]}seglongitude'  value='{$seglongitude}'/>";
            $camposhiddenlog = $campograulongitude . $campominlongitude . $camposeglongitude;

            // visualizar mapa
            $mapa = !empty($graulatitude) && !empty($graulongitude) ? '<tr><td class="SubTituloDireita"></td><td><a onclick="abreMapa(' . $obr["obrid"] . ');" style="cursor:pointer;">Visualizar / Buscar No Mapa</a></td></tr>' : '';

            $obrid = "<input type='hidden' id='obrid'  value='{$obr["obrid"]}'/>";

            print '<form action="" method="post" id="formulario">'
                . '<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Unidade Implantadora:</td>'
                . '			<td colspan="4"><b>' . (($obr['unidade']) ? nl2br($obr['unidade']) : "Não Informada") . $obrid . '</b></td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita" style="width:20%;">Nome da obra:</td><td colspan="2" style="width:45%;">' . $obr['nome'] . '</td>'
                . '			<td class="SubTituloDireita">Município/UF:</td><td>' . $obr['local'] . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Início programado:</td><td colspan="2">' . $obr['inicio'] . '</td>'
                . '			<td class="SubTituloDireita">Término programado:</td><td>' . $obr['termino'] . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Situação da Obra:</td><td colspan="2">' . $obr['situacao'] . '</td>'
                . '			<td class="SubTituloDireita">% Executado:</td><td colspan="2">' . $obr['percentual'] . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloDireita">Latitude:</td><td colspan="2">' . $latitude . $camposhiddenlat . '</td>'
                . '			<td class="SubTituloDireita">Longitude:</td><td colspan="2">' . $longitude . $camposhiddenlog . $zoom . '</td>'
                . '		</tr>'
                . $mapa
                . '		<tr>'
                . '			<td class="SubTituloDireita">Descrição:</td><td colspan="4" align="justify">' . (($obr['obrcomposicao']) ? nl2br($obr['obrcomposicao']) : "Nenhuma observação inserida") . '</td>'
                . '		</tr>'
                . '		<tr>'
                . '			<td class="SubTituloCentro" colspan="5">Fotos</td>'
                . '		</tr>';

            $sql = "SELECT
								arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao,
								to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao
							FROM
								public.arquivo arq
							INNER JOIN
								obras.arquivosobra oar ON arq.arqid = oar.arqid
							INNER JOIN
								obras.obrainfraestrutura obr ON obr.obrid = oar.obrid
							WHERE
								obr.obrid='" . $obr['obrid'] . "' AND
			  					aqostatus = 'A' AND
			  				   (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
							ORDER BY
								arq.arqid DESC LIMIT 4";

            $fotos = $db->carregar($sql);

            print '<tr>';

            if ($fotos[0]) {
                for ($k = 0; $k < count($fotos); $k++) {

                    $_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array("filtro" => "cnt.obrid=" . $uni['obrid'] . " AND aqostatus = 'A'",
                        "tabela" => "obras.arquivosobra");

                    print "<td valign=\"top\" align=\"center\">"
                        . "<img id='" . $fotos[$k]["arqid"] . "' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obr['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=" . $fotos[$k]["arqid"] . "' hspace='10' vspace='3' style='width:80px; height:80px;' onmouseover=\"return escape( '" . $fotos[$k]["arqdescricao"] . "' );\"/><br />"
                        . $fotos[$k]["aqodtinclusao"] . "<br />"
                        . $fotos[$k]["arqdescricao"]
                        . "</td>";

                }
            } else {
                print "<td colspan='5'>Não existe(m) foto(s) cadastrada(s).</td>";
            }

            print '		</tr>'
                . '</table>'
                . '</form>'
                . '<br/>';

        }
    } else {

        print '<tr><td align="center"><b>Não existe(m) Obra(s) cadastrada(s).</b></td></tr>';

    }

    print '		</tr>'
        . '	</table>'
        . '</div>';

}

function academico_painel_dados_sig($orgid, $estuf = '')
{

    global $db;

    $array_dados = array();
    $arr = array();

    // cria o filtro com o funid
    switch ($orgid) {
        case '1':
            $funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
            $funidFiltro = " AND fen.funid = 18 ";
            break;
        case '2':
            $funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "', '" . ACA_ID_ESCOLAS_AGROTECNICAS . "')";
            $funidFiltro = " AND fen.funid = 17 ";
            break;
    }

    // cria o join e o filtro de estado caso exista
    //$join_estuf   = ( $estuf && $estuf != 'todos' ) ? "LEFT JOIN entidade.endereco ed ON ed.entid = ent.entid" : "";
    $filtro_estuf = ($estuf && $estuf != 'todos') ? "AND edc.estuf = '{$estuf}'" : "";

    // cria o join e o filtro por região, caso exista
    switch ($estuf) {
        case 'norte':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
            $filtro_estuf = "AND te.regcod = '1'";
            break;
        case 'nordeste':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
            $filtro_estuf = "AND te.regcod = '2'";
            break;
        case 'sudeste':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
            $filtro_estuf = "AND te.regcod = '3'";
            break;
        case 'sul':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
            $filtro_estuf = "AND te.regcod = '4'";
            break;
        case 'centrooeste':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
            $filtro_estuf = "AND te.regcod = '5'";
            break;
        default:
            break;
    }

    $sql = "(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpinstalacao || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpinstalacao || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpinstalacao || '&params=\' + params, \'' || ac.cmpinstalacao || '\');\"/></center>' as img,
				'Unidades em funcionamento em instalações definitivas' as nome,
				coalesce(count(ac.cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpinstalacao || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				cmpinstalacao = 'D' AND
				ac.exiid = '1' AND
				ac.cmpsituacao = 'F' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpinstalacao)

				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpinstalacao || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpinstalacao || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpinstalacao || '&params=\' + params, \'' || ac.cmpinstalacao || '\');\"/></center>' as img,
				'Unidades em funcionamento em instalações provisórias' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpinstalacao || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				cmpinstalacao = 'P' AND
				ac.exiid = '1' AND
				ac.cmpsituacao = 'F' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpinstalacao)

				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras concluídas' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				ac.exiid = '1' AND
				ac.cmpsituacaoobra = 'C' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpsituacaoobra)

				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em andamento' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				ac.exiid = '1' AND
				ac.cmpsituacaoobra = 'A' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpsituacaoobra)

				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em licitação' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				ac.exiid = '1' AND
				ac.cmpsituacaoobra = 'L' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpsituacaoobra)

				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em elaboração de projetos' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				ac.exiid = '1' AND
				ac.cmpsituacaoobra = 'E' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpsituacaoobra)

				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'academico.php?modulo=principal/painel&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em dominialidade de imóvel' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				academico.campus ac
			LEFT JOIN
				entidade.entidade ent ON ent.entid = ac.entid
		  	LEFT JOIN
		  		entidade.endereco edc ON edc.entid = ent.entid
		  	LEFT JOIN
		  		territorios.municipio mun ON mun.muncod = edc.muncod
		  	LEFT JOIN
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid
		  	LEFT JOIN
		  		entidade.entidade uor ON uor.entid = fea.entid
		  	LEFT JOIN
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
		  	LEFT JOIN
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE
				ac.exiid = '1' AND
				ac.cmpsituacaoobra = 'D' AND
				tuo.funid {$funid}
		  		{$filtro_estuf}
			GROUP BY
				ac.cmpsituacaoobra)
				";

    $cabecalho = array("Ação", "Descrição", "Qtd.");
    $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'N');

}


function academico_painel_campus_dados_sig($orgid, $dado, $estuf = '')
{

    global $db;

    // cria o filtro com o funid
    switch ($orgid) {
        case '1':
            $funid = ACA_ID_CAMPUS;
            break;
        case '2':
            $funid = ACA_ID_UNED;
            break;
        case '3':
            $funid = ACA_ID_UNIDADES_VINCULADAS;
            break;
    }

    // cria o campo do filtro
    switch ($dado) {
        case 'D':
            $campo = "ac.exiid = '1' AND ac.cmpsituacao = 'F' AND ac.cmpinstalacao";
            break;
        case 'P':
            $campo = "ac.exiid = '1' AND ac.cmpsituacao = 'F' AND ac.cmpinstalacao";
            break;
        case 'C':
            $campo = "ac.exiid = '1' AND ac.cmpsituacaoobra";
            break;
        case 'A':
            $campo = "ac.exiid = '1' AND ac.cmpsituacaoobra";
            break;
        case 'E':
            $campo = "ac.exiid = '1' AND ac.cmpsituacaoobra";
            break;
        case 'L':
            $campo = "ac.exiid = '1' AND ac.cmpsituacaoobra";
            break;
    }

    $join_estuf = ($estuf && $estuf != 'todos') ? "LEFT JOIN entidade.endereco ed ON ed.entid = ee.entid" : "";
    $filtro_estuf = ($estuf && $estuf != 'todos') ? "AND ed.estuf = '{$estuf}'" : "";

    // cria o join e o filtro por região, caso exista
    switch ($estuf) {
        case 'norte':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
            $filtro_estuf = "AND te.regcod = '1'";
            break;
        case 'nordeste':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
            $filtro_estuf = "AND te.regcod = '2'";
            break;
        case 'sudeste':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
            $filtro_estuf = "AND te.regcod = '3'";
            break;
        case 'sul':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
            $filtro_estuf = "AND te.regcod = '4'";
            break;
        case 'centrooeste':
            $join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
            $filtro_estuf = "AND te.regcod = '5'";
            break;
        default:
            break;
    }

    $sql = "SELECT
				'<a onclick=\"atualiza_div( \'unidade\', \'' || ee2.entid || '\');\" style=\"cursor:pointer;\">' || ee2.entnome || '</a>'as unidade,
				'&nbsp;&nbsp;&nbsp;&nbsp;<a onclick=\"atualiza_div( \'unidade\', \'' || ee.entid || '\');\" style=\"cursor:pointer;\">' || ee.entnome || '</a>' as campus
			FROM
				entidade.entidade ee
			INNER JOIN
				academico.campus ac ON ac.entid = ee.entid
			INNER JOIN
				entidade.funcaoentidade fe ON ac.entid = fe.entid
			INNER JOIN
				entidade.funentassoc fa ON fa.fueid = fe.fueid
			INNER JOIN
				entidade.entidade ee2 ON ee2.entid = fa.entid
			{$join_estuf} {$join_regiao}
			WHERE
				{$campo} = '{$dado}' AND funid = {$funid}
				{$filtro_estuf}
			ORDER BY
				unidade, campus";

    $db->monta_lista_grupo($sql, $cabecalho, 1000, 5, 'N', 'N', '', '', 'unidade');

}

/*
 * Função que recupera os dados da tabela 'movimentoindicador'. Se não existir registro, um novo é criado.
 */
function recuperaMovimentoIndicador($entid)
{
    global $db;

    $sql = "SELECT mviid FROM academico.movimentoindicador WHERE entid = {$entid}";
    $mviid = $db->pegaUm($sql);

    if (!$mviid) {
        $sql = "INSERT INTO academico.movimentoindicador
					(entid, docid, mvistatus, mvidtinclusao)
				VALUES
					({$entid}, NULL, 'A', now())
				RETURNING
					mviid";
        $mviid = $db->pegaUm($sql);
        $db->commit();
    }

    return $mviid;
}


function acaMenusTipoCurso($acao, $tpcid, $menu)
{

    if ($tpcid <> 3) {
        $arTipoCurso = array(0 => array("descricao" => "Cursos de Graduação",
            "link" => "academico.php?modulo=principal/cursosevagas/{$menu}&acao={$acao}&tpcid=" . TIPOCURSOGRADUACAO),
            1 => array("descricao" => "Cursos de Pós-Graduação",
                "link" => "academico.php?modulo=principal/cursosevagas/{$menu}&acao={$acao}&tpcid=" . TIPOCURSOPOSGRADUACAO));
    } else {
        $arTipoCurso = array(0 => array("descricao" => "Cursos de Graduação",
            "link" => "academico.php?modulo=principal/cursosevagas/{$menu}&acao={$acao}&tpcid=" . TIPOCURSOGRADUACAO),
            1 => array("descricao" => "Dados do Curso",
                "link" => "academico.php?modulo=principal/cursosevagas/{$menu}&acao={$acao}&tpcid=" . DADOSCURSO));
    }
    return montarAbasArray($arTipoCurso, "academico.php?modulo=principal/cursosevagas/{$menu}&acao={$acao}&tpcid={$tpcid}");


}

function acaDadosCurso($cdtid)
{
    global $db;

    $sql = "SELECT
					c.curid,
					cdtid,
					cdtcodigoemec,
					curdsc,
					pgcdsc,
					entnome,
					e.entid,
					CASE WHEN cdtpactuacao = 'P'
						THEN 'Previsto'
						ELSE 'Executado'
					END AS tipo,
					te.turdsc AS turprev,
					cdtinicioexec,
					arcdsc,
					cdtduracao,
					cdtobs,
					cdtvgprevano2007, cdtvgprevano2008, cdtvgprevano2009, cdtvgprevano2010, cdtvgprevano2011, cdtvgprevano2012,
					cdtvgexecano2007, cdtvgexecano2008, cdtvgexecano2009, cdtvgexecano2010, cdtvgexecano2011, cdtvgexecano2012
				FROM
				    academico.cursodetalhe cd
				INNER JOIN
					public.curso c ON c.curid = cd.curid
				INNER JOIN
					entidade.entidade e ON e.entid = cd.entid
				INNER JOIN
					academico.areacurso ar ON ar.arcid = cd.arcid
				LEFT JOIN
					academico.programacurso pc ON pc.pgcid = cd.pgcid
				LEFT JOIN
					academico.turno tp ON tp.turid = cd.turidprevisto
				LEFT JOIN
					academico.turno te ON te.turid = cd.turidexecutado
				LEFT JOIN
					academico.situacaocurso sc ON sc.stcid = cd.stcid
				WHERE
					cdtid = " . $cdtid;

    $dados = $db->pegaLinha($sql);

    return $dados;
}

function acaMenusTipoCursoEdital($acao, $edttipo)
{

    $arTipoCurso = array(0 => array("descricao" => "Todos",
        "link" => "academico.php?modulo=principal/cursosevagas/listaEditaisVagas&acao={$acao}&edttipo=T"),
        1 => array("descricao" => "Editais de Graduação",
            "link" => "academico.php?modulo=principal/cursosevagas/listaEditaisVagas&acao={$acao}&edttipo=G"),
        2 => array("descricao" => "Editais de Pós-Graduação",
            "link" => "academico.php?modulo=principal/cursosevagas/listaEditaisVagas&acao={$acao}&edttipo=P"));

    return montarAbasArray($arTipoCurso, "academico.php?modulo=principal/cursosevagas/listaEditaisVagas&acao={$acao}&edttipo={$edttipo}");


}

/**
 *
 * @return unknown_type
 */
function acaMontaSqlRelCursos()
{


    extract($_REQUEST);

    // unidade
    if ($entid[0]) {
        $where = array();
        array_push($where, " ea.entid " . (!$entid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $entid) . "') ");
    }

    // campus
    if ($entidcampus[0]) {
        if (!is_array($where)) {
            $where = array();
        };
        array_push($where, " ac.entid " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $entidcampus) . "') ");
    }

    // programa
    if ($pgcid[0]) {
        if (!is_array($where)) {
            $where = array();
        };
        array_push($where, " ac.pgcid " . (!$pgcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $pgcid) . "') ");
    }

    // nível de curso
    if ($tpcid[0]) {
        if (!is_array($where)) {
            $where = array();
        };
        array_push($where, " pc.tpcid " . (!$tpcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $tpcid) . "') ");
    }

    $sql = "SELECT DISTINCT
				curdsc as curso,
				ee2.entnome as unidade,
				ee.entnome as campus,
				pgcdsc as programa,
				tpcdsc as nivel,
				CASE WHEN cdtpactuacao = 'P' THEN 'Previsto' ELSE 'Executado' END as tipo,
				CASE WHEN cdtpactuacao = 'P' THEN coalesce(vgpano2007,0) ELSE (select coalesce(sum(excnumvagas),0) FROM academico.execucaocurso WHERE cdtid = ac.cdtid AND excanoexecucao = '2007' AND excstatus = 'A' ) END as vagas2007,
				CASE WHEN cdtpactuacao = 'P' THEN coalesce(vgpano2008,0) ELSE (select coalesce(sum(excnumvagas),0) FROM academico.execucaocurso WHERE cdtid = ac.cdtid AND excanoexecucao = '2008' AND excstatus = 'A' ) END as vagas2008,
				CASE WHEN cdtpactuacao = 'P' THEN coalesce(vgpano2009,0) ELSE (select coalesce(sum(excnumvagas),0) FROM academico.execucaocurso WHERE cdtid = ac.cdtid AND excanoexecucao = '2009' AND excstatus = 'A' ) END as vagas2009,
				CASE WHEN cdtpactuacao = 'P' THEN coalesce(vgpano2010,0) ELSE (select coalesce(sum(excnumvagas),0) FROM academico.execucaocurso WHERE cdtid = ac.cdtid AND excanoexecucao = '2010' AND excstatus = 'A' ) END as vagas2010,
				CASE WHEN cdtpactuacao = 'P' THEN coalesce(vgpano2011,0) ELSE (select coalesce(sum(excnumvagas),0) FROM academico.execucaocurso WHERE cdtid = ac.cdtid AND excanoexecucao = '2011' AND excstatus = 'A' ) END as vagas2011,
				CASE WHEN cdtpactuacao = 'P' THEN coalesce(vgpano2012,0) ELSE (select coalesce(sum(excnumvagas),0) FROM academico.execucaocurso WHERE cdtid = ac.cdtid AND excanoexecucao = '2012' AND excstatus = 'A' ) END as vagas2012
			FROM
				public.curso pc
			INNER JOIN
				academico.cursodetalhe ac ON ac.curid = pc.curid
			INNER JOIN
				entidade.entidade ee ON ee.entid = ac.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ee.entid = ef.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			INNER JOIN
				entidade.entidade ee2 ON ee2.entid = ea.entid
			INNER JOIN
				academico.programacurso ap ON ap.pgcid = ac.pgcid
			INNER JOIN
				public.tipocurso pt ON pt.tpcid = pc.tpcid
			LEFT JOIN
				academico.vagaspactuacao vp ON vp.cdtid = ac.cdtid
			WHERE
				curstatus = 'A' AND
				cdtstatus = 'A' " . (is_array($where) ? ' AND' . implode(' AND ', $where) : '')
        . (is_array($agrupador) ? " ORDER BY " . implode(",", $agrupador) : "");

    return $sql;

}

/**
 *
 * @return unknown_type
 */
function acaMontaAgpRelCursos()
{

    $agrupador = $_REQUEST['agrupador'];

    $agp = array("agrupador" => array(),
        "agrupadoColuna" => array("vagas2007",
            "vagas2008",
            "vagas2009",
            "vagas2010",
            "vagas2011",
            "vagas2012"));


    foreach ($agrupador as $val) {

        switch ($val) {
            case "unidade":
                array_push($agp['agrupador'], array(
                        "campo" => "unidade",
                        "label" => "Unidade")
                );
                break;
            case "campus":
                array_push($agp['agrupador'], array(
                        "campo" => "campus",
                        "label" => "Campus")
                );
                break;
            case "programa":
                array_push($agp['agrupador'], array(
                        "campo" => "programa",
                        "label" => "Programa")
                );
                break;
            case "curso":
                array_push($agp['agrupador'], array(
                        "campo" => "curso",
                        "label" => "Nome do Curso")
                );
                break;
            case "nivel":
                array_push($agp['agrupador'], array(
                        "campo" => "nivel",
                        "label" => "Nivel( Graduação / Pós-Graduação )")
                );
                break;
            case "tipo":
                array_push($agp['agrupador'], array(
                        "campo" => "tipo",
                        "label" => "Tipo ( Previsto / Executado )")
                );
                break;
        }
    }

    return $agp;

}

/**
 *
 * @return unknown_type
 */
function acaMontaColunaRelCursos()
{

    $coluna = array();

    array_push($coluna, array("campo" => "vagas2007",
        "label" => "2007",
        "blockAgp" => array("unidade", "campus", "programa", "nivel"),
        "type" => "numeric"));

    array_push($coluna, array("campo" => "vagas2008",
        "label" => "2008",
        "blockAgp" => array("unidade", "campus", "programa", "nivel"),
        "type" => "numeric"));

    array_push($coluna, array("campo" => "vagas2009",
        "label" => "2009",
        "blockAgp" => array("unidade", "campus", "programa", "nivel"),
        "type" => "numeric"));

    array_push($coluna, array("campo" => "vagas2010",
        "label" => "2010",
        "blockAgp" => array("unidade", "campus", "programa", "nivel"),
        "type" => "numeric"));

    array_push($coluna, array("campo" => "vagas2011",
        "label" => "2011",
        "blockAgp" => array("unidade", "campus", "programa", "nivel"),
        "type" => "numeric"));

    array_push($coluna, array("campo" => "vagas2012",
        "label" => "2012",
        "blockAgp" => array("unidade", "campus", "programa", "nivel"),
        "type" => "numeric"));

    return $coluna;

}

function academico_trata_workflow($tpcid = null, $esdid = null)
{
    //$tpcid -> Tipo de contrato
    //$esdid -> Estado documento
    if ($esdid) {
        #ENSINO SUPERIOR
        if ($_SESSION['academico']['orgid'] == '1') {
            if ($esdid == WF_BENS_SERVICOS_EM_CADASTRAMENTO) {
                echo '
                        <script>
                            if(document.getElementById("td_acao_2488")) document.getElementById("td_acao_2488").style.display = "none";
                            if(document.getElementById("td_acao_2489")) document.getElementById("td_acao_2489").style.display = "none";
                            if(document.getElementById("td_acao_2490")) document.getElementById("td_acao_2490").style.display = "none";
                        </script>
                    ';
            } elseif ($esdid == WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_REITOR) {
                echo '
                        <script>
                            if(document.getElementById("td_acao_1326")) document.getElementById("td_acao_1326").style.display = "none";
                            if(document.getElementById("td_acao_2738")) document.getElementById("td_acao_2738").style.display = "none";
                            if(document.getElementById("td_acao_2741")) document.getElementById("td_acao_2741").style.display = "none";
                        </script>
                    ';
            } elseif ($esdid == WF_BENS_SERVICOS_EM_ANALISE_CONJUR) {
                echo '
                        <script>
                            //if(document.getElementById("td_acao_2530")) document.getElementById("td_acao_2530").style.display = "none";
                            if(document.getElementById("td_acao_2493")) document.getElementById("td_acao_2493").style.display = "none";
                        </script>
                    ';
            }
        }

        #ENSINO PROFISSIONAL
        if ($_SESSION['academico']['orgid'] == '2') {
            if ($esdid == WF_BENS_SERVICOS_EM_CADASTRAMENTO) {
                echo '
                        <script>
                            if(document.getElementById("td_acao_2488")) document.getElementById("td_acao_2488").style.display = "none";
                            if(document.getElementById("td_acao_2489")) document.getElementById("td_acao_2489").style.display = "none";
                            if(document.getElementById("td_acao_2490")) document.getElementById("td_acao_2490").style.display = "none";
                        </script>
                    ';
            } elseif ($esdid == WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_REITOR) {
                echo '
                        <script>
                            if(document.getElementById("td_acao_1326")) document.getElementById("td_acao_1326").style.display = "none";
                            if(document.getElementById("td_acao_2737")) document.getElementById("td_acao_2737").style.display = "none";
                            if(document.getElementById("td_acao_2738")) document.getElementById("td_acao_2738").style.display = "none";
                            if(document.getElementById("td_acao_1335")) document.getElementById("td_acao_1335").style.display = "none";
                        </script>
                    ';
            } elseif ($esdid == WF_BENS_SERVICOS_EM_ANALISE_CONJUR) {
                if ($tpcid == '1') {
                    echo '
                            <script>
                                //if(document.getElementById("td_acao_2530")) document.getElementById("td_acao_2530").style.display = "none";
                                if(document.getElementById("td_acao_2493")) document.getElementById("td_acao_2493").style.display = "none";
                                if(document.getElementById("td_acao_2742")) document.getElementById("td_acao_2742").style.display = "none";
                            </script>
                        ';
                } else {
                    echo '
                            <script>
                                //if(document.getElementById("td_acao_2530")) document.getElementById("td_acao_2530").style.display = "none";
                                if(document.getElementById("td_acao_2493")) document.getElementById("td_acao_2493").style.display = "none";
                                if(document.getElementById("td_acao_2740")) document.getElementById("td_acao_2740").style.display = "none";
                            </script>
                        ';
                }
            }
        }

        #OUTROS
        if ($_SESSION['academico']['orgid'] == '3') {
            if ($_SESSION['academico']['entid'] == ENT_SAA) {
                if ($esdid == WF_BENS_SERVICOS_EM_CADASTRAMENTO) {
                    echo '
                            <script>
                                if(document.getElementById("td_acao_2488")) document.getElementById("td_acao_2488").style.display = "none";
                                if(document.getElementById("td_acao_2489")) document.getElementById("td_acao_2489").style.display = "none";
                                if(document.getElementById("td_acao_1325")) document.getElementById("td_acao_1325").style.display = "none";
                            </script>
                        ';
                }
//                    elseif($esdid == WF_BENS_SERVICOS_EM_ANALISE_CONJUR){
//                        echo '
//                            <script>
//                                if(document.getElementById("td_acao_2530")) document.getElementById("td_acao_2530").style.display = "none";
//                                if(document.getElementById("td_acao_2493")) document.getElementById("td_acao_2493").style.display = "none";
//                            </script>
//                        ';
//                    }
            } else {
                if ($esdid == WF_BENS_SERVICOS_EM_CADASTRAMENTO) {
                    echo '
                            <script>
                                if(document.getElementById("td_acao_2488")) document.getElementById("td_acao_2488").style.display = "none";
                                if(document.getElementById("td_acao_2489")) document.getElementById("td_acao_2489").style.display = "none";
                                if(document.getElementById("td_acao_2490")) document.getElementById("td_acao_2490").style.display = "none";
                            </script>
                        ';
                } elseif ($esdid == WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_REITOR) {
                    echo '
                            <script>
                                if(document.getElementById("td_acao_2741")) document.getElementById("td_acao_2741").style.display = "none";
                                if(document.getElementById("td_acao_1326")) document.getElementById("td_acao_1326").style.display = "none";
                                if(document.getElementById("td_acao_2737")) document.getElementById("td_acao_2737").style.display = "none";
                            </script>
                        ';
                } elseif ($esdid == WF_BENS_SERVICOS_EM_ANALISE_CONJUR) {
                    echo '
                            <script>
                                //if(document.getElementById("td_acao_2530")) document.getElementById("td_acao_2530").style.display = "none";
                                if(document.getElementById("td_acao_2493")) document.getElementById("td_acao_2493").style.display = "none";
                            </script>
                        ';
                }
            }
        }
    }
}


/*
 ********************************** FUNÇÕES WORKFLOW ***************************
 */

function pegarDocid($greid)
{
    global $db;

    if (!$greid) {
        return false;
    }
    $sql = "SELECT
			 docid
			FROM
			 academico.grupoequivalencia
			WHERE
			 greid  = " . (integer)$greid;

    return $db->pegaUm($sql);

}

/*
 * Criar Documento =>
 * "workflow.documento" e "academico.grupoequivalencia"
 */
function criarDocumento($greid)
{
    global $db;

    if (!$greid) {
        return false;
    }

    $docid = pegarDocid($greid);
    if (!$docid) {
        $tpdid = TPDID_EQUIVALENCIA;

        $docdsc = "Grupo Equivalência ($greid)";

        /*
		 * cria documento WORKFLOW
		 */
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);


        $sql = "UPDATE academico.grupoequivalencia SET
				 docid = " . $docid . "
				WHERE
				 greid = " . $greid;

        $db->executar($sql);
        $db->commit();
    }
    return $docid;
}

/*
 * function: pegarEstadoAtual()
 * date:     15/12/2009
 * params:   $docid
 * desc:     Carrega o estado atual do workflow
 * returns:  $estado;
 */
function pegarEstadoAtual($docid)
{
    global $db;
    $docid = (integer)$docid;

    $sql = "SELECT
				ed.esdid
			FROM
				workflow.documento d
			INNER JOIN
				workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;
    $estado = $db->pegaUm($sql);

    return $estado;
}

function pegarDocidSBS($sbsid)
{
    global $db;
    $sql = "
		SELECT docid
		FROM academico.solicitacaobensservicos
		WHERE sbsid = $sbsid
	";
    return $db->pegaUm($sql);
}

function criarDocumentoSBS($sbsid, $tpdid = 71)
{
    global $db;
    require_once APPRAIZ . 'includes/workflow.php';
    $docid = pegarDocidSBS($sbsid);
    if (!$docid) {
        $docdsc = "Solicitação Bens e Serviços n° {$sbsid}";
        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);
        // atualiza pap do EMI
        $sql = "
			UPDATE academico.solicitacaobensservicos SET
				docid = $docid
			WHERE sbsid = $sbsid
		";
        $db->executar($sql);
        $db->commit();
    }
    return $docid;
}

/*
 ************** INÍCIO - PÓS-AÇÃO => WORKFLOW ***********************************
 *
 *
 */
function aprovarEquivalencia($greid)
{
    $greid = (integer)$greid;

    $c = new CursosEdital();
    $c->aprovarEquivalencia($greid);
    return true;
}

function retornarAprovacaoEquivalencia($greid)
{
    $greid = (integer)$greid;

    $c = new CursosEdital();
    $c->retornarAprovacaoEquivalencia($greid);
    return true;
}

/*
 ************** FIM - PÓS-AÇÃO => WORKFLOW ***********************************
 */

/*
 * ************** INÍCIO - CONDIÇÃO => WORKFLOW ***********************************
 *
 *
 */
function equivalenciaExecutada($greid)
{
    if (empty($greid)) {
        return false;
    }

    $c = new CursosEdital();
    $retorno = $c->equivalenciaExecutada($greid);

    if (empty($retorno))
        $retorno = true;
    else
        $retorno = false;

    return $retorno;
}

/*
 ************** FIM - CONDIÇÃO => WORKFLOW ***********************************
 */

/*
 ********************************** FIM <=> FUNÇÕES WORKFLOW ***************************
 */

function criaAbaPortaria()
{
    if ($_SESSION['academico']['prcid']) {
        $abasAbaPortaria = array(0 => array("descricao" => "Cumprimento do Objeto",
            "link" => "academico.php?modulo=principal/cumprimentoObjeto&acao=A"),
            1 => array("descricao" => "Receita e Despesa",
                "link" => "academico.php?modulo=principal/receitaDespesa&acao=A"),
            2 => array("descricao" => "Físico-Financeira",
                "link" => "academico.php?modulo=principal/fisicoFinanceiro&acao=A"),
            3 => array("descricao" => "Relação de Pagamentos",
                "link" => "academico.php?modulo=principal/relacaoPagamentos&acao=A"),
            4 => array("descricao" => "Anexos",
                "link" => "academico.php?modulo=principal/anexoPortaria&acao=A"),
        );
    } else {
        $abasAbaPortaria = array(0 => array("descricao" => "Cumprimento do Objeto",
            "link" => "academico.php?modulo=principal/cumprimentoObjeto&acao=A")
        );
    }

    return $abasAbaPortaria;
}

function possuiPerfil($pflcods)
{

    global $db;
    if (is_array($pflcods)) {
        $pflcods = array_map("intval", $pflcods);
        $pflcods = array_unique($pflcods);
    } else {
        $pflcods = array((integer)$pflcods);
    }
    if (count($pflcods) == 0) {
        return false;
    }
    $sql = "select
				count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode(",", $pflcods) . " ) ";
    return $db->pegaUm($sql) > 0;
}

function pegaPerfil($usucpf)
{
    global $db;

    $sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE p.sisid = '{$_SESSION['sisid']}'
			AND
			pu.usucpf = '$usucpf'";


    $pflcod = $db->carregar($sql);
    return $pflcod;
}

function pegaUnidadeAssociada($perfil)
{
    global $db;

    $sql = "SELECT e.entnome, e.entunicod, e.entungcod, ur.entid FROM academico.usuarioresponsabilidade  ur
				inner join entidade.entidade e on ur.entid = e.entid
 			WHERE ur.usucpf = '{$_SESSION['usucpf']}' and ur.rpustatus = 'A' and ur.pflcod = '{$perfil}' ";
    $unidade = $db->carregar($sql);

    if ($unidade) {
        return $unidade;
    }

    return false;
}

function possuiPerfilCadastro()
{
    global $db;

    if (possuiPerfil(array(PERFIL_SUPERUSUARIO,
            PERFIL_IFESCADASTRO,
            PERFIL_IFESCADBOLSAS,
            PERFIL_IFESCADCURSOS,
            PERFIL_MECCADBOLSAS,
            PERFIL_MECCADASTRO,
            PERFIL_MECCADCURSOS
        )
    )) {
        return true;
    }

    return false;

}

/*** FUNÇÕES WORKFLOW TERMO ORCAMENTO***/

function tcVerificaEstado($esdid)
{

    global $db;

    $sql = "SELECT esdid FROM workflow.estadodocumento WHERE esdid = {$esdid}";

    return $db->pegaUm($sql);

}

function tcCriarDocumento($tmcid)
{

    global $db;

    $docid = tcPegarDocid($tmcid);

    if (!$docid) {

        // recupera o tipo do documento
        $tpdid = TC_TIPO_DOCUMENTO;

        // descrição do documento
        $docdsc = "Fluxo Termo de Cooperação - n°" . $tmcid;

        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);

        // atualiza pap do EMI
        $sql = "UPDATE
					academico.termocooperacao
				SET
					docid = {$docid}
				WHERE
					tmcid = {$tmcid}";

        $db->executar($sql);
        //$db->commit();
    }

    return $docid;

}

function tcPegarDocid($tmcid)
{

    global $db;

    $sql = "SELECT
				docid
			FROM
				academico.termocooperacao
			WHERE
			 	tmcid = " . (integer)$tmcid;

    return (integer)$db->pegaUm($sql);

}

function tcPegarEstadoAtual($tmcid)
{

    global $db;

    $docid = tcPegarDocid($tmcid);

    $sql = "select
				ed.esdid
			from
				workflow.documento d
			inner join
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;

    $estado = (integer)$db->pegaUm($sql);

    return $estado;

}


function posAcaoCriaProcesso($tmcid)
{
    global $db;

    /*$sql = "INSERT INTO academico.processo(tmcid) VALUES (".$tmcid.") returning prcid";
	$prcid = $db->pegaUm($sql);
	$db->commit();*/

    $prcid = $db->pegaUm("SELECT prcid FROM academico.processo WHERE tmcid = " . $tmcid);

    echo "<script>
			window.opener.location = '/academico/academico.php?modulo=principal/cumprimentoObjeto&acao=A&prcid=$prcid';
		</script>";
    return true;
}

function criarNumeroAutorizacao($aveid)
{
    global $db;
    $entid = $_SESSION['academico']['entid'];
    $data = date("Ymd");
    $sql = "update academico.autviagemexterior set avenumauto = '$entid.$data.$aveid', avecpfresp='' where aveid = $aveid";
    $db->executar($sql);
    $db->commit($sql);
    alertaPerfilEmail($aveid);
    return true;
}

function alertaPerfilEmail($aveid)
{
    global $db;

    $sql = "select
				ed.esdid,
				ent.entnome,
				ent.entid,
				avenumauto,
				avedata
			from
				academico.autviagemexterior tbl1
			inner join
				entidade.entidade ent ON tbl1.entid = ent.entid
			inner join
				workflow.documento d ON d.docid = tbl1.docid
			inner join
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				tbl1.aveid = $aveid";
    $arrDados = $db->pegaLinha($sql);
    if ($arrDados) {
        extract($arrDados);
    } else {
        return false;
    }

    switch ($esdid) {
        case WF_SOLICITACAO_VIAGEM_AGUARDANDO_AUTORIZACAO_REITOR:
            $texto = "
                            Magnífico Reitor,
                            <br><br>
                            Informo que encontra-se cadastrada no SIMEC solicitação de autorização coletiva para concessão de diárias e passagens para o exterior, <b>aguardando sua aprovação e envio para autorização Ministerial.</b>
                            <br>
                            <br>
                            Solicitação cadastrada por: \"{$_SESSION['usunome']}\", em " . date('d/m/Y', strtotime($avedata)) . ", às " . date('H', strtotime($avedata)) . "h.
                        ";
            $titulo = "Solicitação de Autorização - Diárias e Passagens para o Exterior";

            $sql = "select
				usuemail
			from
				seguranca.usuario usu
			inner join
				academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
			where
				ur.pflcod = " . PERFIL_REITOR . "
			and
				ur.entid = ( select a.entid from academico.autviagemexterior a where a.aveid = $aveid)";

            $emaiRreitor = $db->pegaUm($sql);
            //Email Reitor
            if ($emaiRreitor) {
                $arrEmail = array($emaiRreitor);
            }
            break;

        case WF_SOLICITACAO_VIAGEM_AUTORIZADO:
            $texto = "
                            Magnífico Reitor,<br><br>
                            Informo que sua solicitação de autorização coletiva para concessão de diárias e passagens para o exterior encontra-se autorizada sob o nº <b>$avenumauto</b>, e está disponível para impressão no SIMEC.
                            <br><br>
                            LUIZ CLÁUDIO COSTA
                            <br>
                            Secretário Executivo
                        ";

            $titulo = "Autorização - Diárias e Passagens para o Exterior";
            $sql = "
                            select
                                    usuemail
                            from
                                    seguranca.usuario usu
                            inner join
                                    academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
                            where
                                    ur.pflcod = " . PERFIL_REITOR . "
                            and
                                    ur.entid = ( select a.entid from academico.autviagemexterior a where a.aveid = $aveid)
                        ";
            $emaiRreitor = $db->pegaUm($sql);
            //Email Reitor
            if ($emaiRreitor) {
                $arrEmail = array($emaiRreitor);
            }
            break;

        case WF_SOLICITACAO_VIAGEM_AGUARDANDO_AUTORIZACAO_SECRETARIA_EXECUTIVA:
            $texto = "
                            Há solicitação de autorização coletiva para concessão de diárias e passagens para o exterior pendente de autorização Ministerial.
                            <br><br>
                            Unidade Solicitante: <b>$entnome</b>
                            <br>
                            Data e hora da solicitação: <b>" . date("d/m/Y") . " às " . date("H") . "h.</b>
                        ";
            $titulo = "Solicitação de Autorização - Diárias e Passagens para o Exterior";
            //Emails - SECRETARIO EXECUTIVO
            $arrEmail = array("rodrigo.lamego@mec.gov.br", "genilda.mota@mec.gov.br");
            break;

        default:
            return false;
            break;
    }

    $arrEmail = !$arrEmail ? array("julianosouza@mec.gov.br") : $arrEmail;

    if ($_SESSION['baselogin'] == "simec_espelho_producao" || $_SESSION['baselogin'] == "simec_desenvolvimento") {
        $arrEmail = array("cristianocabral@mec.gov.br", "julianosouza@mec.gov.br");
    }

    require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');
    $e = new EmailAgendado();
    $e->setTitle($titulo);
    $e->setText($texto);
    $e->setName("SIMEC");
    $e->setEmailOrigem("simec@mec.gov.br");
    $e->setEmailsDestino($arrEmail);
    $e->enviarEmails();
    return true;
}


function alertaPerfilEmailSBS($sbsid)
{
    global $db;

    if ($sbsid) {
        geraPDFAutorizacaoDecreto($sbsid);
    }

    $docid = $_REQUEST['docid'];

    if ($docid != '') {
        enviarEmailEstadoDocumento($docid);
    }

    $sql = "
        SELECT  sbs.docid,
                ed.esdid,
                ent.entnome,
                ent.entid,
                sbsid,
                sbsdtinclusao,
                to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0') as numero

        FROM academico.solicitacaobensservicos sbs

        INNER JOIN entidade.entidade ent ON sbs.entid = ent.entid
        INNER JOIN workflow.documento d ON d.docid = sbs.docid
        INNER JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid
        INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid

        WHERE sbs.sbsid = {$sbsid}
    ";
    $arrDados = $db->pegaLinha($sql);

    if ($arrDados) {
        extract($arrDados);
    } else {
        return false;
    }

    switch ($esdid) {
        case WF_BENS_SERVICOS_EM_ANALISE_SETEC:
            $texto = "
                    Senhor Dirigente,
                    <br><br>
                    Informo que encontra-se cadastrada no SIMEC solicitação de autorização para a contratação/prorrogação de despesas nos termos do Decreto 7.689/2012, aguardando sua aprovação e envio para autorização Ministerial.
                    <br><br>
                    Solicitação cadastrada por: \"{$_SESSION['usunome']}\", em " . date('d/m/Y', strtotime($sbsdtinclusao)) . ", às " . date('H', strtotime($sbsdtinclusao)) . "h.
                ";
            $titulo = "Solicitação de Autorização – Decreto 7.689/2012";

            $sql = "select
                            usuemail
                    from
                            seguranca.usuario usu
                    inner join
                            academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
                    where
                            ur.rpustatus = 'A' and ur.pflcod = " . PERFIL_REITOR . "
                    and
                            ur.entid = ( select a.entid from academico.solicitacaobensservicos a where a.sbsid = $sbsid)
                ";
            $emaiRreitor = $db->pegaUm($sql);
            //Email Reitor
            if ($emaiRreitor) {
                $arrEmail = array($emaiRreitor);
            }
            break;
        case WF_BENS_SERVICOS_AUTORIZADO_SECRETARIO:
            $texto = "
                    Senhor Dirigente,
                    <br><br>
                    Informo que sua solicitação de autorização para a contratação/prorrogação de despesas nos termos do Decreto 7.689/2012,
                    encontra-se autorizada sob o nº $numero, e está disponível para impressão no SIMEC.
                    <br>
                    Autorização de Governança Finalizada - Contratação devidamente aprovado nos termos do Decreto nº 7.689/12.
                ";
            $titulo = "Autorização - Decreto 7.689/2012";

            #BUSCA TODOS OS E-MAIL DOS ENVOLVIDOS NA TAMITAÇÃO DA AUTÓRIZAÇÃO
            if ($docid > 0) {
                $sql = "
                        SELECT  DISTINCT us.usuemail
                        FROM workflow.historicodocumento hd

                        JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
                        JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
                        JOIN seguranca.usuario us ON us.usucpf = hd.usucpf
                        LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid

                        WHERE hd.docid = {$docid}

                        ORDER BY 1
                    ";
                $arrEmail = $db->pegaUm($sql);
            }
            break;
        case WF_BENS_SERVICOS_AUTORIZADO_MINISTRO:
            $texto = "
                    Senhor Dirigente,
                    <br><br>
                    Informo que sua solicitação de autorização para a contratação/prorrogação de despesas nos termos do Decreto 7.689/2012,
                    encontra-se autorizada sob o nº $numero, e está disponível para impressão no SIMEC.
                    <br>
                    Autorização de Governança Finalizada - Contratação devidamente aprovado nos termos do Decreto nº 7.689/12.
                ";
            $titulo = "Autorização - Decreto 7.689/2012";

            #BUSCA TODOS OS E-MAIL DOS ENVOLVIDOS NA TAMITAÇÃO DA AUTÓRIZAÇÃO
            if ($docid > 0) {
                $sql = "
                        SELECT  DISTINCT us.usuemail
                        FROM workflow.historicodocumento hd

                        JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
                        JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
                        JOIN seguranca.usuario us ON us.usucpf = hd.usucpf
                        LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid

                        WHERE hd.docid = {$docid}

                        ORDER BY 1
                    ";
                $arrEmail = $db->pegaUm($sql);
            }
            break;
        case WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_REITOR:
            $texto = "
                    Senhor Dirigente,
                    <br><br>
                    Informo que encontra-se cadastrada no SIMEC solicitação de autorização para a contratação/prorrogação de despesas nos termos do Decreto 7.689/2012, aguardando sua aprovação e envio para autorização Ministerial.
                    <br><br>
                    Solicitação cadastrada por: \"{$_SESSION['usunome']}\", em " . date('d/m/Y', strtotime($sbsdtinclusao)) . ", às " . date('H', strtotime($sbsdtinclusao)) . "h.
                ";
            $titulo = "Solicitação de Autorização – Decreto 7.689/2012";

            $sql = "select
                            usuemail
                    from
                            seguranca.usuario usu
                    inner join
                            academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
                    where
                            ur.rpustatus = 'A' and ur.pflcod = " . PERFIL_REITOR . "
                    and
                            ur.entid = ( select a.entid from academico.solicitacaobensservicos a where a.sbsid = $sbsid)
                ";
            $emaiRreitor = $db->pegaUm($sql);
            //Email Reitor
            if ($emaiRreitor) {
                $arrEmail = array($emaiRreitor);
            }
            break;
        case WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_MINISTRO:
            $texto = "
                    Há solicitação de autorização para contratação/prorrogação de despesas nos termos do Decreto 7.689/2012, pendente de autorização do Ministro.
                    <br><br>
                    Unidade Solicitante: <b>$entnome</b>
                    <br>
                    Data e hora da solicitação: <b>" . date("d/m/Y") . " às " . date("H") . "h.</b>
                ";
            $titulo = "Solicitação de Autorização – Decreto 7.689/2012";
            #Email dos acessores do Ministro
            $arrEmail = array("vladimir.gorayeb@mec.gov.br", "luis.rebello@mec.gov.br", "mariana.andriotti@mec.gov.br", "luciene.silva@mec.gov.br");
            break;
        case WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_SECRETARIO:
            $texto = "
                    Há solicitação de autorização para contratação/prorrogação de despesas nos termos do Decreto 7.689/2012, pendente de autorização do Secretário Executivo.
                    <br><br>
                    Unidade Solicitante: <b>$entnome<b><br/>
                    Data e hora da solicitação: <b>" . date("d/m/Y") . " às " . date("H") . "h.</b>
                ";
            $titulo = "Solicitação de Autorização – Decreto 7.689/2012";
            #Email dos acessores do Secretario executivo
            $arrEmail = array("rodrigo.lamego@mec.gov.br", "genilda.mota@mec.gov.br");
            break;
        default:
            return false;
            break;
    }
    $arrEmail = !$arrEmail ? array("julianosouza@mec.gov.br") : $arrEmail;

    if ($_SESSION['baselogin'] == "simec_espelho_producao" || $_SESSION['baselogin'] == "simec_desenvolvimento") {
        $arrEmail = array("cristianocabral@mec.gov.br", "julianosouza@mec.gov.br");
    }

    require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');
    $e = new EmailAgendado();
    $e->setTitle($titulo);
    $e->setText($texto);
    $e->setName("SIMEC");
    $e->setEmailOrigem("simec@mec.gov.br");
    $e->setEmailsDestino($arrEmail);
    $e->enviarEmails();
    return true;
}

function verificaEmailReitor($aveid = 0, $sbsid = 0)
{
    global $db;
    $sql = "
		select usuemail
		from seguranca.usuario usu
                inner join academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
		where ur.pflcod = " . PERFIL_REITOR . "
		and   ur.entid  = " . $_SESSION['academico']['entid'] . "
	";
    if ($aveid)
        $sql .= " and ur.entid in
			(select a.entid
             from academico.autviagemexterior a
             where a.aveid = $aveid)
		";
    if ($sbsid)
        $sql .= " and ur.entid in
			(select entid
			 from academico.solicitacaobensservicos
			 where sbsid = $sbsid)
		";

    $emaiRreitor = $db->pegaUm($sql);

    if (!$emaiRreitor) {
        return false;
    } else {
        return true;
    }
}


function verificaEmailReitorPorEntid($entid)
{
    global $db;

    $sql = "select
				usuemail
			from
				seguranca.usuario usu
			inner join
				academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
			where
				ur.pflcod = " . PERFIL_REITOR . "
			and
				ur.entid = $entid";

    $emaiRreitor = $db->pegaUm($sql);

    if (!$emaiRreitor) {
        return false;
    } else {
        return true;
    }
}

function recuperarListaObras($post)
{

    global $db;

    if ($post) {

        extract($post);

        if ($predescricao) {
            $stWhere .= " AND pre.predescricao iLIKE '%{$predescricao}%' ";
        }

        if (trim($municipio) != '' && $bogeratermo == 'true') {
            $stWhere .= " AND mun.muncod = '{$municipio}' ";
        } else {
            if (trim($municipio) != '') {
                $stWhere .= " AND mun.mundescricao iLIKE '%{$municipio}%' ";
            }
        }

    }

    $acoes = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"' || pre.preid || '_' || mun.muncod || '_' || doc.esdid || '\" class=\"mostra\" style=\"cursor:pointer\" /> '";

    $sql = "SELECT DISTINCT
				$acoes as acao,
				pre.predescricao,
				pto.ptodescricao,
				mun.mundescricao,
				--mun.muncod,
				mun.estuf,
				esd.esddsc,
				usu.usunome,
				to_char(hstu.htddata,'DD/MM/YYYY HH24:MI:SS') as htddata,
				(select usunome from (select distinct
							max(hd1.htddata) as data,
							us1.usunome,
							hd1.docid,
							ed1.esddsc
						from workflow.historicodocumento hd1
							inner join workflow.acaoestadodoc ac1 on
								ac1.aedid = hd1.aedid
							inner join workflow.estadodocumento ed1 on
								ed1.esdid = ac1.esdidorigem
							inner join seguranca.usuario us1 on
								us1.usucpf = hd1.usucpf
							left join workflow.comentariodocumento cd1 on
								cd1.hstid = hd1.hstid
						where
							ac1.esdiddestino in (210,211,212)
						and
							hd1.docid = pre.docid
						group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata
						order by data desc limit 1) as foo) as nomeanalista,
				resnumero
			FROM obras.preobra pre
			LEFT  JOIN territorios.municipio 		 mun ON pre.muncod  = mun.muncod
			LEFT  JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
			LEFT  JOIN territorios.tipomunicipio     tpm ON tpm.tpmid   = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
			INNER JOIN workflow.documento 			 doc ON doc.docid   = pre.docid {$filtroAnalista}
			INNER JOIN workflow.estadodocumento 	 esd ON esd.esdid   = doc.esdid
			INNER JOIN obras.pretipoobra 			 pto ON pre.ptoid   = pto.ptoid
			LEFT  JOIN par.resolucao				 res ON res.resid   = pre.resid
			{$stInner}
			LEFT JOIN workflow.historicodocumento hstu ON hstu.hstid=doc.hstid
			LEFT JOIN seguranca.usuario usu ON hstu.usucpf=usu.usucpf
			" . ($poausucpfinclusao ? " INNER " : " LEFT ") . " JOIN (SELECT
								poap.preid,
								poap.poausucpfinclusao,
								usup.usunome as nomeanalista
							FROM obras.preobraanalise poap
							LEFT JOIN seguranca.usuario usup ON usup.usucpf = poap.poausucpfinclusao
					) poa ON poa.preid = pre.preid
			WHERE pre.prestatus = 'A' AND pre.tooid = 3 AND pre.preidpai IS NULL
			{$stWhere}
			ORDER BY htddata DESC";
    return $db->carregar($sql);
}

function recuperarUF($muncod)
{
    global $db;

    $sql = "SELECT estuf FROM territorios.municipio WHERE muncod = '{$muncod}'";
    return $db->pegaUm($sql);
}

function prePegarDocid($preid)
{

    global $db;

    $sql = "SELECT
				docid
			FROM
				obras.preobra
			WHERE
			 	preid = " . (integer)$preid;

    return (integer)$db->pegaUm($sql);

}

function prePegarEstadoAtual($docid)
{

    global $db;

    $sql = "SELECT
				ed.esdid
			FROM
				workflow.documento d
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;

    $estado = (integer)$db->pegaUm($sql);

    return $estado;

}

function carregaAbasPronatec($stPaginaAtual = null)
{
    $preid = $_SESSION['par']['preid'] ? '&preid=' . $_SESSION['par']['preid'] : '';
    $abas = array(
//			0 => array("descricao" => "Termo de Compromisso",
//					   "link" => "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=TermoCompromisso".$preid),
        0 => array("descricao" => "Dados do Imóvel",
            "link" => "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Dados" . $preid)
    );
    if ($_SESSION['par']['preid']) {
        array_push($abas, array("descricao" => "Características do Imóvel",
            "link" => "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Questionario" . $preid));
        array_push($abas, array("descricao" => "Cadastro de Fotos do Imóvel",
            "link" => "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Foto" . $preid));
        array_push($abas, array("descricao" => "Documentos Anexos",
            "link" => "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Documento" . $preid));
        array_push($abas, array("descricao" => "Analise",
            "link" => "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Analise" . $preid));
        array_push($abas, array("descricao" => "Analise Instituto Federal",
            "link" => "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=AnaliseEngenheiro" . $preid));
    }

    $win = false;

    return montarAbasArray($abas, $stPaginaAtual, $win);
}

function cabecalho()
{
    global $db;

    if ($_SESSION['par']['itrid'] == 1) {
        $sql = "SELECT
                    estdescricao as descricao
                FROM
                    territorios.estado
                WHERE
                    estuf = '" . $_SESSION['par']['estuf'] . "'";
        $descricao = $db->pegaUm($sql);
        $desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Descrição:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">" . $descricao . "</td>
				</tr>";
    } else {
        $sql = "SELECT
					estuf,
                    mundescricao as descricao
                FROM
                    territorios.municipio
                WHERE
                    muncod = '" . $_SESSION['par']['muncod'] . "'";
        $municipio = $db->pegaLinha($sql);
        $desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">" . $municipio['descricao'] . "</td>
				</tr>";
    }
    if (!$_SESSION['par']['preid']) {
        $ptodescricao = '-';
    } else {
        $sql = "SELECT
					ptodescricao
				FROM
					obras.pretipoobra pto
				INNER JOIN obras.preobra po ON po.ptoid = pto.ptoid
				WHERE
					po.preid = {$_SESSION['par']['preid']}";

        $ptodescricao = $db->pegaUm($sql);

        $sqlEmp = "SELECT
						SUM(eobvalorempenho) as valor
					FROM
						par.empenhoobra
					WHERE
						preid = " . $_SESSION['par']['preid'] . " and eobstatus = 'A'";
        $valor = $db->pegaUm($sqlEmp);
        if ($valor > 0) {
            $emp = "<tr>
						<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Valor Empenhado:</td>
						<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">
							R$ " . number_format($valor, 2, ',', '.') . "
						</td>
					</tr>";

        }
    }
    if ($_SESSION['par']['prog'] == 'proinf') {
        $tipoobra = "
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Tipo Obra:</td>
							<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">" . $ptodescricao . "</td>
						</tr>";
    }

    echo "
		<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
			<tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">UF:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">" . (($municipio['estuf']) ? $municipio['estuf'] : $_SESSION['par']['estuf']) . "</td>
				</tr>
				{$desc}
				{$tipoobra}
				{$emp}
			</tbody>
		</table>
		";
}

function pegaQrpidPAC($preid, $queid)
{

    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    global $db;

    $sql = "SELECT
            	po.qrpid as qrpid,
            	po.predescricao as predescricao
            FROM
            	obras.preobra po
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
            WHERE
            	po.preid = {$preid}
            	AND q.queid = {$queid}";

    $dados = $db->pegaLinha($sql);

    if (empty($dados['qrpid'])) {
        $arParam = array("queid" => $queid, "titulo" => "OBRAS (" . $preid . " - " . $dados['predescricao'] . ")");
        $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
        $sql = "UPDATE
                    obras.preobra
            	SET
                    qrpid = {$qrpid}
            	WHERE
                    preid = {$preid}";
        $db->executar($sql);
        $db->commit();
    } else {
        $qrpid = $dados['qrpid'];
    }
    return $qrpid;
}

function pegaQrpidAnalisePAC($preid, $queid)
{

    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

    global $db;

    $sql = "SELECT
            	po.qrpid as qrpid
            FROM
            	obras.preobraanalise po
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
            WHERE
            	po.preid = {$preid}
            	AND q.queid = {$queid}";

    $dados = $db->pegaLinha($sql);

    if (empty($dados['qrpid'])) {
        $arParam = array("queid" => $queid, "titulo" => "OBRAS (" . $preid . ")");
        $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
        $sql = "UPDATE
                    obras.preobraanalise
            	SET
                    qrpid = {$qrpid}
            	WHERE
                    preid = {$preid}";
        $db->executar($sql);
        $db->commit();
    } else {
        $qrpid = $dados['qrpid'];
    }
    return $qrpid;
}

function pegaArrayPerfil($usucpf)
{

    global $db;

    $sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";


    $pflcod = $db->carregar($sql);

    foreach ($pflcod as $dados) {
        $arPflcod[] = $dados['pflcod'];
    }

    return $arPflcod;
}

function EnviarArquivoObras($arquivo, $dados, $dir = 'documentos', $boRedirecionar = true)
{
    global $db;
    // obtém o arquivo
    $arquivo = $_FILES['arquivo'];
    if (!is_uploaded_file($arquivo['tmp_name'])) {
        redirecionar($_REQUEST['modulo'], $_REQUEST['acao'], $parametros);
    }
    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if ($arquivo["type"] == 'image/pjpeg') {
        $arquivo["type"] = 'image/jpeg';
    }
    //Insere o registro do arquivo na tabela public.arquivo
    $sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
			values('" . current(explode(".", $arquivo["name"])) . "','" . end(explode(".", $arquivo["name"])) . "','" . substr($dados["arqdescricao"], 0, 255) . "','" . $arquivo["type"] . "','" . $arquivo["size"] . "','" . date('Y-m-d') . "','" . date('H:i:s') . "','" . $_SESSION["usucpf"] . "',15) RETURNING arqid;";
    $arqid = $db->pegaUm($sql);

    //Insere o registro na tabela obras.arquivosobra
    $sql = "INSERT INTO obras.arquivosobra (obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
			values(" . $_SESSION["obra"]["obrid"] . "," . $dados["tpaid"] . "," . $arqid . ",'" . $_SESSION["usucpf"] . "','" . date("Y-m-d H:i:s") . "','A');";
    $db->executar($sql);

    if (!is_dir('../../arquivos/obras/' . floor($arqid / 1000))) {
        mkdir(APPRAIZ . '/arquivos/obras/' . floor($arqid / 1000), 0777);
    }
    $caminho = APPRAIZ . 'arquivos/obras/' . floor($arqid / 1000) . '/' . $arqid;
    switch ($arquivo["type"]) {
        case 'image/jpeg':

            try {

                ini_set("memory_limit", "128M");
                list($width, $height) = getimagesize($arquivo['tmp_name']);
                $original_x = $width;
                $original_y = $height;
                // se a largura for maior que altura
                if ($original_x > $original_y) {
                    $porcentagem = (100 * 640) / $original_x;
                } else {
                    $porcentagem = (100 * 480) / $original_y;
                }
                $tamanho_x = $original_x * ($porcentagem / 100);
                $tamanho_y = $original_y * ($porcentagem / 100);
                $image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
                $image = imagecreatefromjpeg($arquivo['tmp_name']);
                imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                imagejpeg($image_p, $caminho, 100);
                //Clean-up memory
                ImageDestroy($image_p);
                //Clean-up memory
                ImageDestroy($image);

            } catch (Exception $e) {

                if (!move_uploaded_file($arquivo['tmp_name'], $caminho)) {
                    $db->rollback();
                    echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
                    exit;
                }
            }
            break;
        default:
            if (!move_uploaded_file($arquivo['tmp_name'], $caminho)) {
                $db->rollback();
                echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
                exit;
            }
    }


    $db->commit();
    if ($boRedirecionar) $db->sucesso("principal/" . $dir);
    else return true;
}

function DownloadArquivoObras($param)
{
    global $db;

    $sql = "SELECT * FROM public.arquivo WHERE arqid = " . $param['arqid'];
    $arquivo = current($db->carregar($sql));
    $caminho = APPRAIZ . 'arquivos/obras/' . floor($arquivo['arqid'] / 1000) . '/' . $arquivo['arqid'];
    if (!is_file($caminho)) {
        $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
    }
    if (is_file($caminho)) {
        $filename = str_replace(" ", "_", $arquivo['arqnome'] . '.' . $arquivo['arqextensao']);
        header('Content-type: ' . $arquivo['arqtipo']);
        header('Content-Disposition: attachment; filename=' . $filename);
        readfile($caminho);
        exit();
    } else {
        die("<script>alert('Arquivo não encontrado.');window.location='academico.php?modulo=principal/documentosObras&acao=A';</script>");

    }
}

function DeletarDocumentoObras($documento, $caminho = 'principal/documentosObras', $boRedirecionar = true)
{
    global $db;

    $sql = "UPDATE obras.arquivosobra SET aqostatus = 'I' where aqoid=" . $documento["aqoid"];
    $db->executar($sql);

    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=" . $documento["arqid"];
    $db->executar($sql);
    $db->commit();
    $_REQUEST["acao"] = "A";
    if ($boRedirecionar) $db->sucesso($caminho);
    else return true;
}

function validaEnvioDiretoria($tmcid)
{
    global $db;

    $dirid = $db->pegaUm("SELECT dirid FROM academico.termocooperacao WHERE tmcid = $tmcid");
    if (empty($dirid)) {
        return false;
    }
    return true;
}

function validaEnvioCoordenacao($tmcid)
{
    global $db;

    $cooid = $db->pegaUm("SELECT cooid FROM academico.termocooperacao WHERE tmcid = $tmcid");
    if (empty($cooid)) {
        return false;
    }
    return true;
}

/*
* POPUPS Monitoramento de Programas e Ações
* */

function popupMostraCursos()
{

    global $db;

    extract($_REQUEST);

    monta_titulo('Lista de Cursos', 'Previsto');
    ?>
    <script language="JavaScript" src="../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <form name="form" id="form" method="POST">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                    <?php

                    $sql = "SELECT
							curdsc
						FROM
							academico.campuscurso cc
						INNER JOIN public.curso cu ON cu.curid = cc.curid
						WHERE
							cpcano = '2013'
							AND cpcprevisto = 'T'
							AND cc.cmpid = $cmpid";
                    $cabecalho = array("Cursos");
                    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', $par2);
                    ?>
                </td>
            </tr>

        </table>
    </form>
<?php
}

/*
*
* */
function salvarDemanda()
{

    global $db;

    extract($_REQUEST);

    $sql = "UPDATE academico.medidascursos SET
				mdadscmedida = '{$_POST['mdadscmedida']}'
			WHERE
				mdaid = $mdaid";
    $db->executar($sql);
    $db->commit();

    echo "
		<script>
			alert('Dados Salvos com sucesso!');
			window.close();
		</script>";
}

function popupMostraDemanda()
{

    global $db;
    $perfis = pegaPerfilGeral();

    $habilita = 'N';
    if (in_array(PERFIL_CADASTROGERAL, $perfis)) {
        $habilita = 'S';
    };

    if ($_REQUEST['req']) {
        $_REQUEST['req']();
        die();
    }

    extract($_REQUEST);

    $sql = "SELECT
				mdadscmedida
			FROM
				academico.medidascursos
			WHERE
				mdaid = $mdaid";
    $mdadscmedida = $db->pegaUm($sql);

    monta_titulo('MEDIDAS A SEREM ADOTADAS', '');
    ?>
    <script language="JavaScript" src="../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>

    <script language="javascript" type="text/javascript"
            src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script>

        <? if( $habilita == 'S' ){?>
        tinyMCE.init({
            mode: "textareas",
            theme: "advanced",
            plugins: "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
            theme_advanced_buttons1: "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
            theme_advanced_buttons2: "",
            theme_advanced_buttons3: "",
            theme_advanced_toolbar_location: "top",
            theme_advanced_toolbar_align: "left",
            extended_valid_elements: "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
            language: "pt_br",
            entity_encoding: "raw"
        });
        <?} ?>

        jQuery.noConflict()
        jQuery(document).ready(function () {
            jQuery('.salvar').click(function () {
                jQuery('#req').val('salvarDemanda');
                jQuery('#form').submit();
            });
        });
    </script>
    <form name="form" id="form" method="POST" action="">
        <input type="hidden" value="" name="req" id="req"/>
        <input type="hidden" value="<?= $mdaid ?>" name="mdaid" id="mdaid"/>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td colspan="2">
                    <center>
                        <?= campo_textarea('mdadscmedida', 'N', $habilita, '', 120, 20, '', '', 0, '', false, NULL, $mdadscmedida) ?>
                    </center>
                </td>
            </tr>
            <tr>
                <td width="4%"></td>
                <td>
                    <input type="button" class="salvar" style="cursor:pointer" value="Salvar">
                </td>
            </tr>

        </table>
    </form>
<?php
}

/*
*
* */
function salvarArquivo2()
{

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = Array("mdaid" => "'" . $_POST['mdaid'] . "'",
        "tpaid" => "'" . $_POST['tpaid'] . "'");

    $file = new FilesSimec("arqmedidas", $campos, "academico");

    $arquivoSalvo = $file->setUpload($_POST['arqdescricao'], '', true);

    if ($arquivoSalvo) {
        echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
        echo "<script type='text/javascript'>window.opener.location.reload();</script>";
        die;
    }
}

function baixaArquivo()
{

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
}

function excluirArquivo()
{

    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    extract($_REQUEST);

    if ($mdaid) {
        $sql = "UPDATE academico.arqmedidas SET
					amdstatus = 'I'
				WHERE
					arqid = $arqid";

        $db->executar($sql);
        $db->commit();
    }

    echo "
		<script>
			alert('Dados excluídos com sucesso!');
		</script>";
    echo "<script type='text/javascript'>window.opener.location.reload();</script>";
}


function popupMostraDocumentos()
{

    global $db;

    extract($_REQUEST);

    if ($_REQUEST['req']) {
        $_REQUEST['req']();
        die();
    }

    monta_titulo('Documentos', '');
    ?>
    <script language="JavaScript" src="../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

    <script language="javascript" type="text/javascript"
            src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script>

        $(document).ready(function () {
            $('input[type="text"]').keyup();
            $('.download').click(function () {
                $('#req').val('baixaArquivo');
                $('#arqid').val($(this).attr('id'));
                $('#form').submit();
            });
            $('.excluir').click(function () {
                $('#req').val('excluirArquivo');
                $('#arqid').val($(this).attr('id'));
                $('#form').submit();
            });
            $('.salvar').click(function () {
                if ($('#arquivo').val() == '') {
                    $('#arquivo').focus();
                    alert('Campo obrigatório.');
                    return false;
                }
                var erro = false;
                $('.obrigatorio').each(function () {
                    if ($(this).val() == '') {
                        $(this).focus();
                        erro = true;
                        return false;
                    }
                });
                if (erro) {
                    alert('Campo obrigatório.');
                    return false;
                }
                $('#req').val('salvarArquivo2');
                $('#form').submit();
            });
        });
    </script>
    <form name="form" id="form" method="POST" enctype="multipart/form-data">
        <input type="hidden" value="" name="req" id="req"/>
        <input type="hidden" value="<?= $mdaid ?>" name="mdaid" id="mdaid"/>
        <input type="hidden" value="" name="arqid" id="arqid"/>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubtituloEsquerda">
                    Arquivo:
                </td>
                <td>
                    <input type="file" name="arquivo" id="arquivo">
                </td>
            </tr>
            <tr>
                <td class="SubtituloEsquerda">
                    Tipo:
                </td>
                <td>
                    <?php
                    $sql = "SELECT
							tpaid as codigo,
							tpadsc as descricao
						FROM
							academico.tipoarquivo";
                    $db->monta_combo('tpaid', $sql, 'S', 'Selecione', $acao, $opc, '', '200', 'S', 'tpaid', $return = false, $speid, $title = null);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubtituloEsquerda">
                    Descrição:
                </td>
                <td>
                    <?= campo_textarea('arqdescricao', 'S', 'S', '', 80, 5, 250, '', 0, '', false, NULL, $arqdescricao) ?>
                </td>
            </tr>
            <tr>
                <td>
                </td>
                <td>
                    <input class="salvar" type="button" value="Salvar" style="cursor:pointer">
                </td>
            </tr>
        </table>
        <?php
        $sql = "SELECT
 				'<center>
 					<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\"
 						 class=\"excluir imgbot\" id=\"'|| arq.arqid ||'\">
 				</center>' as acao,
 				to_char(arqdata,'DD/MM/YYYY') as arqdata,
 				tpadsc,
 				'<a class=\"download\" id=\"'||arq.arqid||'\" style=\"cursor:pointer;\">'||arqnome||'</a>' as nome,
 				arqtamanho||' Kbs',
 				arqdescricao
			FROM
				academico.arqmedidas amd
			INNER JOIN academico.medidascursos 	mda ON mda.mdaid = amd.mdaid
			INNER JOIN public.arquivo 			arq ON arq.arqid = amd.arqid
			INNER JOIN academico.tipoarquivo	tpa ON tpa.tpaid = amd.tpaid
			WHERE
				mda.mdaid = $mdaid
				";

        $cabecalho = array("Ação", "Data Inclusão", "Tipo Arquivo", "Nome Arquivo", "Tamanho (MB)", "Descrição Arquivo");
        $db->monta_lista($sql, $cabecalho, 50, 5, 'N', '95%', $par2);
        ?>

    </form>
<?php
}


function salvarArquivo4()
{

    if (!$_REQUEST['imiid']) {
        echo '<script type="text/javascript"> alert(" É necessário cadastrar dados da demanda antes de incluir arquivos!");</script>';
        echo "<script>window.location.href = window.close();</script>";
        die;
    }

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = Array("imiid" => "'" . $_REQUEST['imiid'] . "'",
        "tpaid" => "'" . $_POST['tpaid'] . "'");

    $file = new FilesSimec("arqmedidaindicadores", $campos, "academico");

    $arquivoSalvo = $file->setUpload($_POST['arqdescricao'], '', true);

    if ($arquivoSalvo) {
        echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
        echo "<script type='text/javascript'>window.opener.location.reload();</script>";
        echo "<script type='text/javascript'>window.location.href = window.location.href;</script>";
        die;
    }
}

function baixaArquivo2()
{

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
}

function excluirArquivo2()
{

    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    extract($_REQUEST);

    if ($amiid) {
        $sql = "DELETE FROM academico.arqmedidaindicadores WHERE amiid = $amiid";
        $db->executar($sql);
        $db->commit();
    }

    echo "
        <script>
                alert('Dados excluídos com sucesso!');echo
        </script>";
    echo "<script type='text/javascript'>window.opener.location.reload();</script>";

}

# salvarArquivoInfra - USADA NA TELA DEMANDA - CADASTRO DE TIPOLOGIAS POR DEMANDAS
function salvarArquivoInfra()
{
    global $db;

    $dinid = $_REQUEST['dinid'];
    $tpaid_arquivo = $_REQUEST['tpaid_arquivo'];
    $entidcampus = $_REQUEST['entidcampus'];

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = Array(
        "dinid" => "'" . $dinid . "'",
        "tpaid" => "'" . $tpaid_arquivo . "'");

    $file = new FilesSimec("arquivotipologia", $campos, "academico");

    $arquivoSalvo = $file->setUpload($_POST['arqdescricao'], '', true);

    if ($arquivoSalvo) {
        $db->sucesso('principal/mpa/popupMpaInfraestrutura', '&dinid=' . $dinid . '&entidcampus=' . $entidcampus);
    } else {
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/mpa/popupMpaInfraestrutura&acao=A');
    }
}

# downloadArquivoInfra - USADA NA TELA DEMANDA - CADASTRO DE TIPOLOGIAS POR DEMANDAS
function downloadArquivoInfra()
{
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);

    exit;
}

# excluirArquivoInfra - USADA NA TELA DEMANDA - CADASTRO DE TIPOLOGIAS POR DEMANDAS
function excluirArquivoInfra()
{
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    extract($_REQUEST);

    if ($arqid) {
        $sql = "DELETE FROM academico.arquivotipologia WHERE arqid = $arqid";
    }

    if ($db->executar($sql)) {
        $db->commit();
        $db->sucesso('principal/mpa/popupMpaInfraestrutura', '&dinid=' . $dinid);
    } else {
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/mpa/popupMpaInfraestrutura&acao=A');
    }
}

# salvarArquivomE - USADA NA TELA DEMANDA - CADASTRO DE TIPOLOGIAS POR DEMANDAS
function salvarArquivoMedidas()
{
    global $db;
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = Array(
        "imiid" => "'" . $_REQUEST['imiid'] . "'",
        "tpaid" => "'" . $_REQUEST['tpaid'] . "'");

    $file = new FilesSimec("arqmedidaindicadores", $campos, "academico");

    $arquivoSalvo = $file->setUpload($_POST['arqdescricao'], '', true);

    if ($arquivoSalvo) {
        $db->sucesso('principal/mpa/popupMostraDemandaMPAInd', '&idaid=' . $_REQUEST['idaid']);
    } else {
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/mpa/popupMostraDemandaMPAInd&acao=A');
    }
}

# downloadArquivoMedidas - USADA NA TELA INDICADORES ACADÊMICOS - CADASTRO DE MEDIDAS SANEADORES
function downloadArquivoMedidas()
{
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);

    exit;
}

# excluirArquivoInfra - USADA NA TELA INDICADORES ACADÊMICOS - CADASTRO DE MEDIDAS SANEADORES
function excluirArquivomEDIDAS()
{
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    extract($_REQUEST);

    if ($arqid) {
        $sql = "DELETE FROM academico.arqmedidaindicadores WHERE arqid = $arqid";
    }

    if ($db->executar($sql)) {
        $db->commit();
        $db->sucesso('principal/mpa/popupMostraDemandaMPAInd', '&idaid=' . $_REQUEST['idaid']);
    } else {
        $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/mpa/popupMostraDemandaMPAInd&acao=A');
    }
}

function baixaArquivo1()
{

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
}

/*
function excluirArquivo1(){

	global $db;

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	extract($_REQUEST);

	if( $amiid ){
		$sql = "DELETE FROM academico.arqmedidaindicadores
				WHERE
					amiid = $amiid";

		$db->executar($sql);
		$db->commit();
	}

	echo "
		<script>
			alert('Dados excluídos com sucesso!');echo
		</script>";
	echo "<script type='text/javascript'>window.opener.location.reload();</script>";

}
*/

function popupMostraDocumentosMRF()
{

    global $db;

    extract($_REQUEST);

    if ($_REQUEST['req']) {
        $_REQUEST['req']();
        die();
    }

    monta_titulo('Documentos', '');
    ?>
    <script language="JavaScript" src="../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

    <script language="javascript" type="text/javascript"
            src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script>

        $(document).ready(function () {

            $(window).unload(function () {
                alert("Bye now!");
            });

            $('input[type="text"]').keyup();
            $('.download').click(function () {
                $('#req').val('baixaArquivo1');
                $('#arqid').val($(this).attr('id'));
                $('#form').submit();
            });
            /* $('.excluir').click(function(){
             $('#req').val('excluirArquivo1');
             $('#arqid').val($(this).attr('id'));
             $('#amiid').val($(this).attr('amiid'));
             $('#form').submit();
             });
             $('.salvar').click(function(){
             if( $('#arquivo').val() == '' ){
             $('#arquivo').focus();
             alert('Campo obrigatório.');
             return false;
             }
             var erro = false;
             $('.obrigatorio').each(function(){
             if( $(this).val() == '' ){
             $(this).focus();
             erro = true;
             return false;
             }
             });
             if( erro ){
             alert('Campo obrigatório.');
             return false;
             }
             $('#req').val('salvarArquivo3');
             $('#form').submit();
             });*/
        });
    </script>
    <form name="form" id="form" method="POST" enctype="multipart/form-data">
        <input type="hidden" value="" name="req" id="req"/>
        <input type="hidden" value="<?= $imiid ?>" name="imiid" id="imiid"/>
        <input type="hidden" value="" name="arqid" id="arqid"/>
        <input type="hidden" value="" name="amiid" id="amiid"/>
        <!--  <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloEsquerda">
				Arquivo:
			</td>
			<td>
				<input type="file" name="arquivo" id="arquivo">
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda">
				Tipo:
			</td>
			<td>
				<?php
        $sql = "SELECT
							tpaid as codigo,
							tpadsc as descricao
						FROM
							academico.tipoarquivo";
        $db->monta_combo('tpaid', $sql, 'S', 'Selecione', $acao, $opc, '', '200', 'S', 'tpaid', $return = false, $tpaid, $title = null);
        ?>
			</td>
		</tr>
		<tr>
		<td class="SubtituloEsquerda">
				Descrição:
			</td>
			<td>
				<?= campo_textarea('arqdescricao', 'S', 'S', '', 80, 5, 250, '', 0, '', false, NULL, $arqdescricao) ?>
			</td>
		</tr>
		<tr>
			<td>
			</td>
 			<td>
	        	<input class="salvar" type="button" value="Salvar" style="cursor:pointer">
	        </td>
   	 	</tr>
	</table>-->
        <?php
        $sql = "SELECT
				-- '<center>
				-- <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\"
				-- class=\"excluir imgbot\" id=\"'|| arq.arqid ||'\" amiid=\"'|| ami.amiid ||'\" ></center>' as acao,
				to_char(arqdata,'DD/MM/YYYY') as arqdata,
				tpadsc,
				'<a class=\"download\" id=\"'||arq.arqid||'\" style=\"cursor:pointer;\">'||arqnome||'</a>' as nome,
				arqtamanho||' Kbs',
				arqdescricao
			FROM
				academico.arqmedidaindicadores ami

			INNER JOIN academico.medidasindicadores mdi ON mdi.imiid = ami.imiid
			INNER JOIN academico.indicadoresacademicos ida ON ida.idaid = mdi.idaid
			INNER JOIN public.arquivo arq ON arq.arqid = ami.arqid
			INNER JOIN academico.tipoarquivo tpa ON tpa.tpaid = ami.tpaid
			WHERE
				ida.idaid = $idaid";

        $cabecalho = array("Data Inclusão", "Tipo Arquivo", "Nome Arquivo", "Tamanho (MB)", "Descrição Arquivo");
        $db->monta_lista($sql, $cabecalho, 50, 5, 'N', '95%', $par2);
        ?>

    </form>

<?PHP

}

function textoHTML($entid)
{
    global $db;
    $sql = "select * from entidade.entidade  ent
			inner join entidade.endereco ende ON ende.entid = ent.entid
			inner join territorios.municipio mun ON mun.muncod = ende.muncod
			where ent.entid = $entid";
    $arrDados = $db->pegaLinha($sql);

    $sql = "select
				usunome as nome
			from
				seguranca.usuario usu
			inner join
				academico.usuarioresponsabilidade ure ON ure.usucpf = usu.usucpf
			where
				ure.entid = $entid
			and
				ure.pflcod = " . PERFIL_REITOR . "
			and
				ure.rpustatus = 'A'
			and
				usu.usustatus = 'A'
			order by
				rpudata_inc desc";
    $arrReitor = $db->pegaLinha($sql);
    if (!$arrReitor) {
        $sql = "SELECT
				DISTINCT fun.funid, fun.fundsc, ent.entnome as nome, ent.entid
			FROM
				entidade.funcao fun
			LEFT JOIN
				entidade.funcaoentidade fen ON fen.funid = fun.funid
											   AND fen.entid IN (SELECT
											   						fen2.entid
											   					FROM
											   						entidade.funentassoc fea2
			        											LEFT JOIN
			        												entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
																WHERE
																	fea2.entid='$entid'
																	AND fun.funid = fen2.funid)
			LEFT JOIN
				entidade.entidade ent ON fen.entid = ent.entid
			WHERE
				fun.funid IN('21')";
        $arrReitor = $db->pegaLinha($sql);
    }
    ?>
    <div style="text-align:justify;">
    <center>
        <h2>PORTARIA NORMATIVA Nº 14, DE 9 DE JULHO DE 2013</h2>

        <p><b>MINISTÉRIO DA EDUCAÇÃO</b></p>

        <p><b>GABINETE DO MINISTRO</b></p>

        <p><b>DOU de 10/07/2013 (nº 131, Seção 1, pág. 18)</b></p>
    </center>
    <p>Dispõe sobre os procedimentos de adesão das instituições federais de educação superior ao Projeto Mais Médicos e
        dá outras providências.</p>
    <p>O MINISTRO DE ESTADO DA EDUCAÇÃO no uso da atribuição que lhe confere o art. 87, inciso II da Constituição
        Federal, e tendo em vista o disposto na Medida Provisória nº 621, de 8 de julho de 2013, bem como na Portaria
        Interministerial MS/MEC nº 1.369, de 8 de julho de 2013, resolve:</p>
    <p>Art. 1º - Poderão aderir ao Projeto Mais Médicos as instituições federais de educação superior que ofereçam curso
        de Medicina.</p>
    <p>§ 1º - As instituições federais de educação superior interessadas em aderir ao Projeto Mais Médicos deverão
        apresentar termo de pré-adesão, conforme o modelo do <a href="javascript:abreLinkAnexo(<?php echo $entid ?>)">Anexo
            I</a> desta Portaria, no período de 11 a 15 de julho de 2013, ao Ministério da Educação.</p>
    <p>§ 2º - As instituições deverão indicar, no momento da préadesão, um tutor acadêmico responsável pelas atividades
        e, no mínimo, três tutores acadêmicos para fins de cadastro de reserva, que atendam aos requisitos da Portaria
        Interministerial MS/MEC nº 1.369, de 8 de julho de 2013 e desta Portaria.</p>
    <p>§ 3º - As instituições deverão cadastrar via sistema SIMEC, no módulo rede federal, por meio do endereço
        eletrônico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, os tutores indicados
        no termo de pré-adesão.</p>
    <p>§ 4º - No momento da pré-adesão as instituições deverão indicar a unidade responsável pela avaliação e
        autorização de pagamento das bolsas de tutoria e supervisão acadêmicas.</p>
    <p>Art. 2º - O Ministério da Educação decidirá sobre a validação do termo de pré-adesão das instituições que
        atenderem aos requisitos previstos no art. 1º desta Portaria, observadas as necessidades do Projeto Mais
        Médicos.</p>
    <p>Parágrafo único - Em caso de manifestação de interesse de mais de uma instituição por unidade da federação, será
        dada preferência àquela sediada na capital, caso persista o empate, será selecionada àquela que ofertar curso de
        Medicina há mais tempo.</p>
    <p>Art. 3º - As instituições que tiverem seus termos de pré-adesão validados pelo Ministério da Educação deverão
        firmar termo de adesão no prazo máximo de 10 (dez) dias após a divulgação das instituições selecionadas.</p>
    <p>Parágrafo único - O termo de adesão estará disponível para assinatura das instituições selecionadas no sistema
        SIMEC, no módulo rede federal, por meio do endereço eletrônico <a href="http://simec.mec.gov.br"
                                                                          target="_blank">http://simec.mec.gov.br</a>, e
        conterá, no mínimo, as seguintes obrigações para a instituição:</p>
    <p>I - atuar em cooperação com os entes federativos, as Coordenações Estaduais do Projeto e organismos
        internacionais, no âmbito de sua competência, para execução do Projeto Mais Médicos;</p>
    <p>II - coordenar o acompanhamento acadêmico do Projeto;</p>
    <p>III - ratificar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e
        supervisão acadêmicas, indicada no termo de pré-adesão;</p>
    <p>IV - definir mecanismo de avaliação e autorização de pagamento das bolsas de tutoria e supervisão;</p>
    <p>V - ratificar a indicação dos tutores acadêmicos do Projeto, feita no termo de pré-adesão;</p>
    <p>VI - definir critérios e mecanismo de seleção de supervisores;</p>
    <p>VII - realizar seleção dos supervisores do Projeto;</p>
    <p>VIII - monitorar e acompanhar as atividades dos supervisores e tutores acadêmicos no âmbito do Projeto;</p>
    <p>IX - ofertar os módulos de acolhimento e avaliação aos médicos intercambistas; e</p>
    <p>X - ofertar cursos de especialização e atividades de pesquisa, ensino e extensão aos médicos participantes.</p>
    <p>Art. 4º - Os tutores acadêmicos serão selecionados pela instituição entre os docentes da área médica,
        preferencialmente vinculados à área de saúde coletiva ou correlata, ou à área de clínica médica.</p>
    <p>§ 1º - Os tutores acadêmicos perceberão bolsa-tutoria, na forma prevista no termo de adesão.</p>
    <p>§ 2º - Os tutores acadêmicos serão responsáveis pela orientação acadêmica e pelo planejamento das atividades do
        supervisor, trabalhando em parceria com as Coordenações Estaduais do Projeto, e tendo, no mínimo, as seguintes
        atribuições:</p>
    <p>I - coordenar as atividades acadêmicas da integração ensinoserviço, atuando em cooperação com os supervisores e
        os gestores do SUS;</p>
    <p>II - indicar, em plano de trabalho, as atividades a serem executadas pelos médicos participantes e supervisores,
        bem como a metodologia de acompanhamento e avaliação;</p>
    <p>III - monitorar o processo de acompanhamento e avaliação a ser executado pelos supervisores, garantindo sua
        continuidade;</p>
    <p>IV - integrar as atividades do curso de especialização às atividades de integração ensino-serviço;</p>
    <p>V - relatar à instituição pública de ensino superior à qual esteja vinculado a ocorrência de situações nas quais
        seja necessária a adoção de providência pela instituição; e</p>
    <p>VI - apresentar relatórios periódicos da execução de suas atividades no Projeto à instituição à qual esteja
        vinculado e à Coordenação do Projeto.</p>
    <p>Art. 5º - Os supervisores serão selecionados entre profissionais médicos por meio de edital conforme critérios e
        mecanismos estabelecidos pela instituição aderente e validados pela Coordenação Estadual do Projeto Mais
        Médicos.</p>
    <p>§ 1º - Os supervisores selecionados perceberão bolsa, conforme avaliação e autorização das instituições
        aderentes, na forma prevista no termo de adesão.</p>
    <p>§ 2º - Os supervisores selecionados serão responsáveis pelo acompanhamento e fiscalização das atividades de
        ensino-serviço do médico participante, em conjunto com o gestor do SUS no Município, e terão, no mínimo, as
        seguintes atribuições:</p>
    <p>I - realizar visita periódica para acompanhar atividades dos médicos participantes;</p>
    <p>II - estar disponível para os médicos participantes, por meio de telefone e internet;</p>
    <p>III - aplicar instrumentos de avaliação presencialmente; e</p>
    <p>IV - acompanhar e fiscalizar, em conjunto com o gestor do SUS, o cumprimento da carga horária de 40 horas
        semanais prevista pelo Projeto para os médicos participantes, por meio de sistema de informação disponibilizado
        pela Coordenação do Programa.</p>
    <p>Art. 6º - Esta Portaria entra em vigor na data de sua publicação.</p>
    <p>ALOIZIO MERCADANTE OLIVA</p>
<?php
}

function mascara_global_academico($string, $mascara)
{
    $string = str_replace(" ", "", $string);

    for ($i = 0; $i < strlen($string); $i++) {
        $mascara[strpos($mascara, "#")] = $string[$i];
    }
    return $mascara;
}


/**
 * functionName verificaDocuentoAnexado
 *
 * @author Luciano F. Ribeiro <luciano.ribeiro@mec.gov.br>
 *
 * @param integer $sbsid É o id da solicitação de Atorização de Decreto
 *
 * @return boolean retorna true ou false.
 *
 * @version v1
 */
function verificaDocumentoAnexadoConjur($sbsid)
{
    global $db;

    $orgid = $_SESSION['academico']['orgid'];

    $docid = criarDocumentoSBS($sbsid, TPD_BENSSERVICOS);

    $estado = pegarEstadoAtual($docid);

    if ($estado == WF_EM_AJUSTE_PELO_DEMANDANTE) {
        $sql = "
                SELECT arqid
                FROM academico.arquivodecreto

                WHERE sbsid = {$sbsid} AND aqdstatus = 'A' AND tadid = 2
            ";
        $msg = "É necessário anexar Declaração de Regularidade Processual e Disponibilidade Orçamentária";
    } else {
        $sql = "
                SELECT arqid
                FROM academico.arquivodecreto

                WHERE sbsid = {$sbsid} AND aqdstatus = 'A' AND tadid IN (1, 3)
            ";
        $msg = "É necessário anexar Parecer Jurídico ou Solicitação de Ajuste CONJUR";
    }
    $arqid = $db->pegaUm($sql);

    if ($db->testa_superuser()) {
        if ($arqid > 0) {
            return true;
        } else {
            return $msg;
        }
    } else {
        if ($arqid > 0 && $orgid == 3) {
            return true;
        } else {
            return $msg;
        }
    }

}

/**
 * functionName verificaDocuentoAnexado
 *
 * @author Luciano F. Ribeiro <luciano.ribeiro@mec.gov.br>
 *
 * @param integer $sbsid É o id da solicitação de Atorização de Decreto
 *
 * @return boolean retorna true ou false.
 *
 * @version v1
 */
function verificaDocumentoAnexadoSETEC($sbsid)
{
    global $db;

    $orgid = $_SESSION['academico']['orgid'];

    $sql = "
            SELECT  arqid
            FROM academico.arquivodecreto
            WHERE sbsid = {$sbsid} AND aqdstatus = 'A'
        ";
    $arqid = $db->pegaUm($sql);

    if ($db->testa_superuser()) {
        if ($arqid > 0) {
            return true;
        } else {
            return false;
        }
    } else {
        if ($arqid > 0 && $orgid == 2) {
            return true;
        } else {
            return false;
        }
    }
}

/**
 * functionName verificaDocuentoAnexado
 *
 * @author Luciano F. Ribeiro <luciano.ribeiro@mec.gov.br>
 *
 * @param integer $sbsid É o id da solicitação de Atorização de Decreto
 *
 * @return boolean retorna true ou false.
 *
 * @version v1
 */
function verificaDocumentoAnexadoSESu($sbsid)
{
    global $db;

    $orgid = $_SESSION['academico']['orgid'];

    $sql = "
            SELECT arqid
            FROM academico.arquivodecreto
            WHERE sbsid = {$sbsid} AND aqdstatus = 'A'
        ";
    $arqid = $db->pegaUm($sql);

    if ($db->testa_superuser()) {
        if ($arqid > 0) {
            return true;
        } else {
            return false;
        }
    } else {
        if ($arqid > 0 && $orgid == 1) {
            return true;
        } else {
            return false;
        }
    }
}

function pegaEstadoObra($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);
    $sql = "SELECT esdid FROM workflow.documento d WHERE docid = {$docid}";
    $esdid = $db->pegaUm($sql);

    return $esdid;
}

function monta_grid_info_campus_bolsa_formacao($orgid, $arrAnos, $arrDefault)
{
    global $db;

    #ANOS ANALIAZADOS POR TIPO DE ENSINO (declarado no constantes.php).
    if ($arrAnos) {
        $anos = $arrAnos;
    } else {
        $anos = $arrDefault;
    }
    $sql = "
            SELECT
            	'Matrículas Bolsa-Formação (Realizado)' as tipo,
                            dpe.dpeanoref AS ano,
                            entid,
                            sum(dsh.dshqtde::integer) as matriculas
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (2905)
            AND entid = {$_SESSION['academico']['entidcampus']}
            AND sh.sehstatus <> 'I'
            GROUP BY ano, entid
            ORDER BY ano";

    $arrDados = $db->carregar($sql);

    #CABEÇALHO.
    echo "<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" class=\"listagem\">";
    echo "<thead>";
    echo "<tr>";
    foreach ($anos as $ano) {
        echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> {$ano} </td>";
    }

    echo "</tr>";
    echo "</thead>";

    #PREPARA MODIFICANDO O ARRAY RESULTADO DA SQL.
    if ($arrDados != '') {
        foreach ($arrDados as $dados) {
            $arrayAnos[$dados['etapa']][$dados['ano']] = $dados['matriculas'];
        }

        #1º "foreach" - MONTA AS LINHAS DO GRID.
        $cont = 0;
        foreach ($arrayAnos as $etapas => $ano_valor) {
            if ($cont % 2 == 0) {
                $bgcolor = "";
            } else {
                $bgcolor = "#E8E8E8";
            }

            echo "<tr bgcolor=\"{$bgcolor}\">";

            #COLUNA ITENS.
//            echo "<td style=\"text-align:left;font-weight: bold; width:30%;\">{$etapas}</td>";

            #2º "foreach" - MONTA AS COLUNAS DO GRID.
            foreach ($anos as $ano) {
                if ($ano_valor[$ano] != '') {
                    echo "<td style=\"text-align:center; font-weight:bold;\"> {$ano_valor[$ano]} </td>";
                } else {
                    echo "<td style=\"text-align:center; font-weight:bold;\"> - </td>";
                }
            }

            echo "</tr>";
            $cont = $cont + 1;
        }
    } else {
        echo '<tr><td align="center" colspan="12" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
    }
    echo "</table>";
}

function monta_grid_info_campus_educacao_superior($orgid, $arrAnos, $arrDefault)
{
    global $db;

    #ANOS ANALIAZADOS POR TIPO DE ENSINO (declarado no constantes.php).
    if ($arrAnos) {
        $anos = $arrAnos;
    } else {
        $anos = $arrDefault;
    }
    $sql = "(SELECT
                        'Matrículas Educação Superior (Realizado)' as tipo,
                                dpe.dpeanoref AS ano,
                                tid2.tiddsc AS etapa,
                                entid,
                                sum(dsh.dshqtde::integer) as matriculas
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                WHERE sh.indid IN (2347)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid2 IN (5138, 5136, 5137, 5191, 5139, 5140, 5141, 5135, 5134, 5133)
                AND entid = {$_SESSION['academico']['entidcampus']}
                GROUP BY ano, etapa, entid
                ORDER BY ano)
                 UNION ALL
                (SELECT
                    'Matrículas Educação Superior (Realizado)' as tipo,
                    dpe.dpeanoref AS ano,
                    'Total' AS etapa,
                    --tid2.tiddsc AS etapa,
                    entid,
                    sum(dsh.dshqtde::integer) as matriculas
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                WHERE sh.indid IN (2347)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid2 IN (5138, 5136, 5137, 5191, 5139, 5140, 5141, 5135, 5134, 5133)
                AND entid = {$_SESSION['academico']['entidcampus']}
                GROUP BY ano,  entid
                ORDER BY ano )";
    $arrDados = $db->carregar($sql);
    #CABEÇALHO.
    echo "<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" class=\"listagem\">";
    echo "<thead>";
    echo "<tr>";
    echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Itens </td>";

    foreach ($anos as $ano) {
        echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> {$ano} </td>";
    }

    echo "</tr>";
    echo "</thead>";

    #PREPARA MODIFICANDO O ARRAY RESULTADO DA SQL.
    if ($arrDados != '') {
        foreach ($arrDados as $dados) {
            $arrayAnos[$dados['etapa']][$dados['ano']] = $dados['matriculas'];
        }

        #1º "foreach" - MONTA AS LINHAS DO GRID.
        $cont = 0;
        foreach ($arrayAnos as $etapas => $ano_valor) {
            if ($cont % 2 == 0) {
                $bgcolor = "";
            } else {
                $bgcolor = "#E8E8E8";
            }

            echo "<tr bgcolor=\"{$bgcolor}\">";

            #COLUNA ITENS.
            echo "<td style=\"text-align:left;font-weight: bold; width:30%;\">{$etapas}</td>";

            #2º "foreach" - MONTA AS COLUNAS DO GRID.
            foreach ($anos as $ano) {
                if ($ano_valor[$ano] != '') {
                    echo "<td style=\"text-align:center; font-weight:bold;\"> {$ano_valor[$ano]} </td>";
                } else {
                    echo "<td style=\"text-align:center; font-weight:bold;\"> - </td>";
                }
            }

            echo "</tr>";
            $cont = $cont + 1;
        }
    } else {
        echo '<tr><td align="center" colspan="12" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
    }
    echo "</table>";
}


#MONTA DO GRID COM LISTAGEM DE INFORMAÇÕES GERENCIAS DOS CÂMPUS.
function monta_grid_info_campus($orgid, $arrAnos, $arrDefault)
{
    global $db;

    //ver($orgid, d);

    #ANOS ANALIAZADOS POR TIPO DE ENSINO (declarado no constantes.php).
    if ($arrAnos) {
        $anos = $arrAnos;
    } else {
        $anos = $arrDefault;
    }

    #ENSINO SUPERIOR
    if ($orgid == 1) {
        $sql = "
            SELECT  dpe.dpeanoref AS ano,
                    tid2.tiddsc AS etapa,
                    sum(dsh.dshqtde::integer) as matriculas
            FROM painel.seriehistorica sh

            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2

            WHERE sh.indid IN (2347) AND entid = {$_SESSION['academico']['entidcampus']} AND sh.sehstatus <> 'I' AND dsh.tidid2 IN (5138, 5136, 5137, 5191, 5139, 5140, 5141, 5135, 5134, 5133)

            GROUP BY ano, etapa, entid
            ORDER BY etapa, ano
        ";
        $arrDados = $db->carregar($sql);
    }

    #PROFISSIONAL
    if ($orgid == 2) {
        $sql = "
            (
                SELECT          'Matrículas Educação Básica (Realizado)' as tipo,
                                dpe.dpeanoref AS ano,
                                tid1.tiddsc AS etapa,
                                entid,
                                sum(dsh.dshqtde::integer) as matriculas
                FROM painel.detalhetipodadosindicador tid1
                INNER JOIN painel.detalheseriehistorica dsh ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.seriehistorica sh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid

                WHERE sh.indid IN (2575)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 IN (7001, 7000, 6994, 6995, 6996, 6997, 6998, 6999)
                AND entid = {$_SESSION['academico']['entidcampus']}
                GROUP BY ano, etapa, entid
                ORDER BY ano
            ) UNION ALL (
                SELECT          'Matrículas Educação Básica (Realizado)' as tipo,
                                dpe.dpeanoref AS ano,
                                'Total' AS etapa,
                                entid,
                                sum(dsh.dshqtde::integer) as matriculas
                FROM painel.detalhetipodadosindicador tid1
                INNER JOIN painel.detalheseriehistorica dsh ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.seriehistorica sh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (2575)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 IN (7001, 7000, 6994, 6995, 6996, 6997, 6998, 6999)
                AND entid = {$_SESSION['academico']['entidcampus']}
                GROUP BY ano, etapa, entid
                ORDER BY ano
            )
        ";

        $arrDados = $db->carregar($sql);
    }

    #CABEÇALHO.
    echo "<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" class=\"listagem\">";
    echo "<thead>";
    echo "<tr>";
    echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> Itens </td>";

    foreach ($anos as $ano) {
        echo "<td style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;\"> {$ano} </td>";
    }

    echo "</tr>";
    echo "</thead>";

    #PREPARA MODIFICANDO O ARRAY RESULTADO DA SQL.
    if ($arrDados != '') {
        foreach ($arrDados as $dados) {
            $arrayAnos[$dados['etapa']][$dados['ano']] = $dados['matriculas'];
        }

        #1º "foreach" - MONTA AS LINHAS DO GRID.
        $cont = 0;
        foreach ($arrayAnos as $etapas => $ano_valor) {
            if ($cont % 2 == 0) {
                $bgcolor = "";
            } else {
                $bgcolor = "#E8E8E8";
            }

            echo "<tr bgcolor=\"{$bgcolor}\">";

            #COLUNA ITENS.
            echo "<td style=\"text-align:left;font-weight: bold; width:30%;\">{$etapas}</td>";

            #2º "foreach" - MONTA AS COLUNAS DO GRID.
            foreach ($anos as $ano) {
                if ($ano_valor[$ano] != '') {
                    echo "<td style=\"text-align:center; font-weight:bold;\"> {$ano_valor[$ano]} </td>";
                } else {
                    echo "<td style=\"text-align:center; font-weight:bold;\"> - </td>";
                }
            }

            echo "</tr>";
            $cont = $cont + 1;
        }
    } else {
        echo '<tr><td align="center" colspan="12" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
    }
    echo "</table>";
}

function mascaraglobal($value, $mask)
{
    $casasdec = explode(",", $mask);

    // Se possui casas decimais
    if ($casasdec[1])
        $value = sprintf("%01." . strlen($casasdec[1]) . "f", $value);

    $value = str_replace(array("."), array(""), $value);

    if (strlen($mask) > 0) {
        $masklen = -1;
        $valuelen = -1;

        while ($masklen >= -strlen($mask)) {
            if (-strlen($value) <= $valuelen) {
                if (substr($mask, $masklen, 1) == "#") {
                    $valueformatado = trim(substr($value, $valuelen, 1)) . $valueformatado;
                    $valuelen--;
                } else {
                    if (trim(substr($value, $valuelen, 1)) != "") {
                        $valueformatado = trim(substr($mask, $masklen, 1)) . $valueformatado;
                    }
                }
            }
            $masklen--;
        }
    }
    return $valueformatado;
}

function exibirSolicitacaoDescreto($dados){
    global $db;

    include_once APPRAIZ . "includes/classes/dateTime.inc";

    $sql = "
        SELECT  *,
                sbs.tpcid,
                to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0') as numero
        FROM academico.solicitacaobensservicos sbs

        LEFT JOIN workflow.documento doc on doc.docid = sbs.docid
        LEFT JOIN workflow.historicodocumento hst on hst.hstid = doc.hstid
        LEFT JOIN academico.modalidadelicitacao mdl on sbs.mdlid = mdl.mdlid
        LEFT JOIN academico.tipocontratomodalidade tcm on mdl.mdlid = tcm.mdlid
        LEFT JOIN academico.tipocontrato tpc on tcm.tpcid = tpc.tpcid
        LEFT JOIN academico.especiecontratacao epc on sbs.epcid = epc.epcid
        LEFT JOIN entidade.entidade ent on ent.entid = sbs.entid

        WHERE sbs.sbsstatus = 'A' and sbs.sbsid='" . $dados['sbsid'] . "'

        ORDER BY sbs.sbsid
    ";
    $solicitacao = $db->pegaLinha($sql);

    $sql = "
        select  usu.usucpf,
                usu.usunome

        from workflow.historicodocumento hst

        inner join workflow.acaoestadodoc aed on aed.aedid = hst.aedid and (esdiddestino = 495 or esdiddestino = 496)
        inner join seguranca.usuario usu on usu.usucpf = hst.usucpf

        where docid = {$solicitacao['docid']}
        order by hstid desc
    ";
    $rsAssinatura = $db->pegaLinha($sql);

    if ($dados['limparinfo']) $solicitacao = true;

    if ($solicitacao['entid']) {
        $sql = "
            select  ent.entnome,
                    ent.entnumcomercial,
                    ent.entunicod,
                    ende.endcep,
                    ende.endlog,
                    ende.endbai,
                    ende.estuf,
                    mun.mundescricao
            from entidade.entidade ent

            left join entidade.endereco ende ON ende.entid = ent.entid
            left join territorios.municipio mun ON mun.muncod = ende.muncod

            where ent.entid = '{$solicitacao['entid']}'
        ";
        $endereco = $db->pegaLinha($sql);
    }

    ob_clean();
?>
    <style>
        .cabecalho {
            text-align: center;
            font-weight: bold;
            font-size: 18px;
        }

        table.tbl {
            border-collapse: collapse;
        }

        table.tbl tr td {
            border: 1px solid black;
        }

        table.tbl2 {
            border-top: 0px;
        }

        .texto {
            text-align: left;
        }

        table.tbl_not {
            border: 0px;
        }

        * {
            font-family: Arial, Helvetica, sans-serif;
            font-size: 12px;
        }

        .bold {
            font-weight: bold
        }

        @media print {
            .notprint {
                display: none
            }
        }

        @media screen {
            .notscreen {
                display: none
            }
        }
    </style>
    <?PHP
    if ($solicitacao['tpcid'] == TPC_LOCACAO) {
        ?>

        <div class="cabecalho">
            <img width="80px" height="80px" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/brasao.gif"/><br>
            <span style="font-size:14px">MINISTÉRIO DA EDUCAÇÃO</span><br/>
            Autorização para <?= (($solicitacao['epcid'] == EPC_PRORROGACAO) ? "Renovação" : "Celebração") ?> de
            Contratos de Locação de Imóvel
            <br>
        </div>

        <table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td class="bold" width="30%">Nome da Unidade:</td>
                <td colspan="5"><?php echo $solicitacao['entnome'] ?></td>
            </tr>
            <tr>
                <td class="bold" width="30%">Código da Unidade Orçamentária:</td>
                <td width="20%"><?php echo $solicitacao['entunicod'] ? $solicitacao['entunicod'] : "N/A" ?></td>
                <td class="bold" width="30%">Telefone:</td>
                <td colspan="3"
                    width="20%"><?php echo $solicitacao['entnumcomercial'] ? $solicitacao['entnumcomercial'] : "N/A" ?></td>
            </tr>
            <tr>
                <td class="bold" width="30%">Endereço:</td>
                <td colspan="5"><?php echo $endereco['endlog'] ? $endereco['endlog'] . ($endereco['endbai'] ? " - " . $endereco['endbai'] : "") : "N/A" ?></td>
            </tr>
        </table>

        <table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td class="bold" width="16%">CEP:</td>
                <td width="16%"><?php echo $endereco['endcep'] ? mascaraglobal($endereco['endcep'], "#####-###") : "N/A" ?></td>
                <td class="bold" width="16%">Cidade:</td>
                <td width="16%"><?php echo $endereco['mundescricao'] ? $endereco['mundescricao'] : "N/A" ?></td>
                <td class="bold" width="16%">UF:</td>
                <td width="16%"><?php echo $endereco['estuf'] ? $endereco['estuf'] : "N/A" ?></td>
            </tr>
        </table>

        <table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td>
                    <div class="texto">
                        <br>
                        A Sua Excelência o Senhor<br>
                        <?PHP
                        if ($_SESSION['usucpf'] == '') {
                            ?>
                            LUIZ CLÁUDIO COSTA<br>
                            Secretário Executivo do Ministério da Educação<br><br>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            ANTONIO LEONEL DA SILVA CUNHA<br>
                            Secretário Executivo Substituto do Ministério da Educação<br><br>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            WAGNER VILAS BOAS DE SOUZA<br>
                            Secretário Executivo Substituto do Ministério da Educação<br><br>
                        <?PHP
                        }
                        ?>
                        <p>
                            <span style="margin-left:50px">Solicito</span> autorização para a
                            <b><?= (($solicitacao['epcid'] == EPC_PRORROGACAO) ? "Renovação" : "Celebração") ?></b> de
                            Contrato de Locação de Imóvel no âmbito desta Instituição para o exercício de 2012, nos
                            termos do Decreto nº 7.689, de 2 de março de 2012,da Portaria MPOG nº 249,
                            de 13 de junho de 2012 e da Portaria MEC nº. 785, de 18 de junho de 2012.
                        </p>
                    </div>

                    <table class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" width="90%">
                        <tr>
                            <td align="center" width="20%"><b>Nº do processo</b></td>
                            <td align="center" width="60%"><b>Objeto do contrato</b></td>
                            <td align="center" width="20%"><b>Valor Mensal (R$)</b></td>
                        </tr>
                        <tr>
                            <td><?= $solicitacao['sbsnumprocesso'] ?></td>
                            <td><?= $solicitacao['sbsobjeto'] ?></td>
                            <td align="right"><?= number_format($solicitacao['sbsvalor'], 2, ",", ".") ?></td>
                        </tr>
                    </table>

                    <br>

                    <table border=0 class="tbl_not" width="100%" cellSpacing="0" cellPadding="3" align="center">
                        <tr>
                            <td width="50%" align="center">
                                Brasília - <?php echo formata_data($solicitacao['htddata']) ?><br/>
                                Local e Data
                            </td>
                            <?PHP
                            if ($solicitacao['entid']) {
                                $sql = "
                                        select  usu.usunome,
                                                usu.usucpf,
                                                nu_matricula_siape
                                        from seguranca.usuario usu
                                        inner join seguranca.perfilusuario pfu on pfu.usucpf = usu.usucpf
                                        inner join academico.usuarioresponsabilidade urs on urs.usucpf = usu.usucpf
                                        inner join siape.tb_siape_cadastro_servidor_ativos sia on sia.nu_cpf = usu.usucpf
                                        where pfu.pflcod = " . PFL_REITOR . " AND urs.entid = {$solicitacao['entid']} AND rpustatus='A'
                                    ";
                                $dadosreitor = $db->pegaLinha($sql);
                            }
                            ?>
                            <td align="center">
                                <?php echo $dadosreitor['usunome'] ?><br/>
                                CPF: <?php echo $dadosreitor['usucpf'] ?><br/>
                                Matrícula SIAPE: <?php echo $dadosreitor['nu_matricula_siape'] ?>
                            </td>
                        </tr>
                    </table>

                    <br>

                    <p>
                        <?PHP
                        $data = new Data();
                        $data = $data->formataData($solicitacao['htddata'], "Brasília DD de mesTextual de YYYY");
                        ?>
                        Autorização SIMEC nº <?= $solicitacao['numero'] ?>, <?= $data ?>.<br><br>
                        <b><span style="margin-left:50px">Autorizado</span>, devendo essa Instituição observar toda
                            legislação pertinente à matéria.</b><br/>
                    </p>
                    <center>
                        <?PHP
                        if ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario.png"/>
                            <br/>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario_substituto.png"/>
                            <br/>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario_substituto_1.png"/>
                            <br/>
                        <?PHP
                        }
                        ?>
                    </center>
                </td>
            </tr>
            <tr>
                <td class="texto">
                    <p><i>
                            <font size="1">
                                Portaria MPOG nº 249, de 13 de junho de 2012<br>
                                Art. 4º As autorizações de que tratam os arts. 2º e 4º do Decreto nº 7.689, de 2012,
                                constitui ato de governança das contratações
                                estritamente relacionado a uma avaliação sobre a conveniência da despesa pública, não
                                envolvendo a análise técnica e jurídica
                                do procedimento, que são de responsabilidade dos ordenadores de despesa e das unidades
                                jurídicas dos respectivos órgãos e
                                entidades, de acordo com suas competências legais, nem implicando ratificação ou
                                validação dos atos que compõem o processo
                                de contratação.(publicação no DOU de 14/06/2012 e retificação no DOU de 21/06/2012)
                            </font>
                        </i></p>
                </td>
            </tr>
            <tr>
                <td class="texto">
                    <br>
                    Autorização emitida por meio do Sistema Integrado de Monitoramento Execução e Controle do Ministério
                    da Educação - SIMEC
                </td>
            </tr>

        </table>
    <?PHP
    } elseif ($solicitacao['tpcid'] == TPC_LOCACAOADM) {
        ?>
        <div class="cabecalho">
            <img width="80px" height="80px" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/brasao.gif"/><br/>
            <span style="font-size:14px">MINISTÉRIO DA EDUCAÇÃO</span><br/>
            Autorização para <?= (($solicitacao['epcid'] == EPC_PRORROGACAO) ? "Renovação" : "Celebração") ?> de
            Contratos Administrativos <br>
        </div>

        <table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td class="bold" width="30%">Nome da Unidade:</td>
                <td colspan="5"><?php echo $solicitacao['entnome'] ?></td>
            </tr>
            <tr>
                <td class="bold" width="30%">Código da Unidade Orçamentária:</td>
                <td width="20%"><?php echo $solicitacao['entunicod'] ? $solicitacao['entunicod'] : "N/A" ?></td>
                <td class="bold" width="30%">Telefone:</td>
                <td colspan="3"
                    width="20%"><?php echo $solicitacao['entnumcomercial'] ? $solicitacao['entnumcomercial'] : "N/A" ?></td>
            </tr>
            <tr>
                <td class="bold" width="30%">Endereço:</td>
                <td colspan="5"><?php echo $endereco['endlog'] ? $endereco['endlog'] . ($endereco['endbai'] ? " - " . $endereco['endbai'] : "") : "N/A" ?></td>
            </tr>
        </table>

        <table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td class="bold" width="16%">CEP:</td>
                <td width="16%"><?php echo $endereco['endcep'] ? mascaraglobal($endereco['endcep'], "#####-###") : "N/A" ?></td>
                <td class="bold" width="16%">Cidade:</td>
                <td width="16%"><?php echo $endereco['mundescricao'] ? $endereco['mundescricao'] : "N/A" ?></td>
                <td class="bold" width="16%">UF:</td>
                <td width="16%"><?php echo $endereco['estuf'] ? $endereco['estuf'] : "N/A" ?></td>
            </tr>
        </table>


        <table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td>
                    <div class="texto">
                        <br>
                        A Sua Excelência o Senhor<br>
                        <?PHP
                        if ($_SESSION['usucpf'] == '') {
                            ?>
                            LUIZ CLÁUDIO COSTA<br>
                            Secretário Executivo do Ministério da Educação<br><br>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            ANTONIO LEONEL DA SILVA CUNHA<br>
                            Secretário Executivo Substituto do Ministério da Educação<br><br>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            WAGNER VILAS BOAS DE SOUZA<br>
                            Secretário Executivo Substituto do Ministério da Educação<br><br>
                        <?PHP
                        }
                        ?>
                        <p style="font-size: 11px;">
                            <span style="margin-left:50px">Solicito</span> autorização para a
                            <b><?= (($solicitacao['epcid'] == EPC_PRORROGACAO) ? "Renovação" : "Celebração") ?></b> de
                            Contrato Administrativo referente a atividades de custeio no âmbito desta Instituição para o
                            período de <?php echo formata_data($solicitacao['sbsdtiniciovigencia']); ?>
                            até <?php echo formata_data($solicitacao['sbsdtfimvigencia']); ?>,
                            nos termos do Decreto nº 7.689, de 2 de março de 2012, da Portaria MPOG nº. 249, de 13 de
                            junho de 2012 e da Portaria MEC nº. 785, de 18 de junho de 2012.
                        </p>

                        <p style="font-size: 11px;">
                            <span style="margin-left:50px">Aproveito</span> o ensejo para declarar que o processo em
                            epigrafe não possui qualquer ólice de natureza técnica, administrativa e jurídica, conforme
                            parecer jurídico que segue anexo.
                        </p>
                    </div>

                    <table class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" width="90%">
                        <tr>
                            <td align="center" width="20%"><b>Nº do processo</b></td>
                            <td align="center" width="60%"><b>Objeto do contrato</b></td>
                            <td align="center" width="20%"><b>Valor do contrato (R$)</b></td>
                        </tr>
                        <tr>
                            <td><?= $solicitacao['sbsnumprocesso'] ?></td>
                            <td><?= $solicitacao['sbsobjeto'] ?></td>
                            <td align="right"><?= number_format($solicitacao['sbsvalor'], 2, ",", ".") ?></td>
                        </tr>
                    </table>

                    <br>

                    <table border=0 class="tbl_not" width="100%" cellSpacing="0" cellPadding="3" align="center">
                        <tr>
                            <td width="50%" align="center">
                                Brasília - <?php echo formata_data($solicitacao['htddata']) ?><br/>
                                Local e Data
                            </td>
                            <?PHP
                            if ($solicitacao['entid']) {
                                $sql = "
                                        select  usu.usunome,
                                                usu.usucpf,
                                                nu_matricula_siape
                                        from seguranca.usuario usu
                                        inner join seguranca.perfilusuario pfu on pfu.usucpf = usu.usucpf
                                        inner join academico.usuarioresponsabilidade urs on urs.usucpf = usu.usucpf
                                        inner join siape.tb_siape_cadastro_servidor_ativos sia on sia.nu_cpf = usu.usucpf
                                        where pfu.pflcod = " . PFL_REITOR . " AND urs.entid = {$solicitacao['entid']} AND rpustatus='A'
                                    ";
                                $dadosreitor = $db->pegaLinha($sql);
                            }
                            ?>
                            <td align="center">
                                <?php echo $dadosreitor['usunome'] ?><br/>
                                CPF: <?php echo $dadosreitor['usucpf'] ?><br/>
                                Matrícula SIAPE: <?php echo $dadosreitor['nu_matricula_siape'] ?>
                            </td>
                        </tr>
                    </table>

                    <br>

                    <p>
                        <?
                        $data = new Data();
                        $data = $data->formataData($solicitacao['htddata'], "Brasília DD de mesTextual de YYYY");
                        ?>
                        Autorização SIMEC nº <?= $solicitacao['numero'] ?>, <?= $data ?>.<br><br>
                        <b>
                            <span style="margin-left:50px">Autorizado</span>, devendo essa Instituição observar toda legislação pertinente à matéria.
                        </b>
                        <br>
                    </p>
                    <center>
                        <?PHP
                        if ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario.png"/>
                            <br/>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario_substituto.png"/>
                            <br/>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario_substituto_1.png"/>
                            <br/>
                        <?PHP
                        }
                        ?>
                    </center>
                </td>
            </tr>
            <tr>
                <td class="texto">
                    <p><i>
                            <font size="1">
                                Portaria MPOG nº 249, de 13 de junho de 2012 <br>
                                Art. 4º As autorizações de que tratam os arts. 2º e 4º do Decreto nº 7.689, de 2012,
                                constitui ato de governança das contratações
                                estritamente relacionado a uma avaliação sobre a conveniência da despesa pública, não
                                envolvendo a análise técnica e jurídica
                                do procedimento, que são de responsabilidade dos ordenadores de despesa e das unidades
                                jurídicas dos respectivos órgãos e
                                entidades, de acordo com suas competências legais, nem implicando ratificação ou
                                validação dos atos que compõem o processo
                                de contratação.(publicação no DOU de 14/06/2012 e retificação no DOU de 21/06/2012)
                            </font>
                        </i></p>
                </td>
            </tr>
            <tr>
                <td class="texto">
                    <br>
                    Autorização emitida por meio do Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC
                </td>
            </tr>
        </table>

    <? } else { ?>

        <div class="cabecalho">
            <img width="80px" height="80px" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/brasao.gif"/> <br>

            <span style="font-size:14px"> MINISTÉRIO DA EDUCAÇÃO </span> <br>
            Autorização para <?= (($solicitacao['epcid'] == EPC_PRORROGACAO) ? "Renovação" : "Celebração") ?> de Contratos Administrativos <br>
        </div>

        <table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td class="bold" width="30%"> Nome da Unidade:</td>
                <td colspan="5"> <?php echo $solicitacao['entnome'] ?> </td>
            </tr>
            <tr>
                <td class="bold" width="30%"> Código da Unidade Orçamentária :</td>
                <td width="20%"> <?php echo $solicitacao['entunicod'] ? $solicitacao['entunicod'] : "N/A" ?> </td>
                <td class="bold" width="30%"> Telefone:</td>
                <td colspan="3"
                    width="20%"> <?php echo $solicitacao['entnumcomercial'] ? $solicitacao['entnumcomercial'] : "N/A" ?> </td>
            </tr>
            <tr>
                <td class="bold" width="30%"> Endereço:</td>
                <td colspan="5"> <?php echo $endereco['endlog'] ? $endereco['endlog'] . ($endereco['endbai'] ? " - " . $endereco['endbai'] : "") : "N/A" ?> </td>
            </tr>
        </table>
        <table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td class="bold" width="16%"> CEP:</td>
                <td width="16%"> <?php echo $endereco['endcep'] ? mascaraglobal($endereco['endcep'], "#####-###") : "N/A" ?> </td>
                <td class="bold" width="16%">Cidade:</td>
                <td width="16%"> <?php echo $endereco['mundescricao'] ? $endereco['mundescricao'] : "N/A" ?> </td>
                <td class="bold" width="16%">UF:</td>
                <td width="16%"> <?php echo $endereco['estuf'] ? $endereco['estuf'] : "N/A" ?> </td>
            </tr>
        </table>

        <table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center">
            <tr>
                <td>
                    <div class="texto">
                        <br>

                        <p>
                            A Sua Excelência o Senhor<br>
                            <?PHP
                            if( $_SESSION['usucpf'] == '' ){
                            ?>
                                ALOIZIO MERCADANTE<br>
                                Ministro de Estado da Educação<br><br>
                            <?PHP
                            } elseif ($_SESSION['usucpf'] == '') {
                                ?>
                                LUIZ CLÁUDIO COSTA<br>
                                Ministro de Estado da Educação Interino<br><br>
                            <?PHP
                            }
                            ?>
                        </p>
                        <span style="margin-left:50px"> Senhor Ministro, </span> <br>

                        <p style="font-size: 11px;">
                            <span style="margin-left:50px"> Solicito </span> autorização para a
                            <b> <?= (($solicitacao['epcid'] == EPC_PRORROGACAO) ? "Renovação" : "Celebração") ?> </b> de
                            Contrato Administrativo referente a atividades de custeio no âmbito desta Instituição para o
                            período de <?php echo formata_data($solicitacao['sbsdtiniciovigencia']); ?>
                            até <?php echo formata_data($solicitacao['sbsdtfimvigencia']); ?>,
                            nos termos do Decreto nº 7.689, de 2 de março de 2012, da Portaria MPOG nº. 249, de 13 de
                            junho de 2012 e da Portaria MEC nº. 785, de 18 de junho de 2012.
                        </p>

                        <p style="font-size: 11px;">
                            <span style="margin-left:50px">Aproveito</span> o ensejo para declarar que o processo em
                            epigrafe não possui qualquer ólice de natureza técnica, administrativa e jurídica, conforme
                            parecer jurídico que segue anexo.
                        </p>
                    </div>

                    <table class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" width="90%">
                        <tr>
                            <td align="center" width="20%"><b> Nº do processo </b></td>
                            <td align="center" width="60%"><b> Objeto do contrato </b></td>
                            <td align="center" width="20%"><b> Valor do contrato (R$) </b></td>
                        </tr>
                        <tr>
                            <td> <?= $solicitacao['sbsnumprocesso'] ?> </td>
                            <td> <?= $solicitacao['sbsobjeto'] ?> </td>
                            <td align="right"> <?= number_format($solicitacao['sbsvalor'], 2, ",", ".") ?> </td>
                        </tr>
                    </table>

                    <br>

                    <table border=0 class="tbl_not" width="100%" cellSpacing="0" cellPadding="3" align="center">
                        <tr>
                            <td width="50%" align="center">
                                Brasília - <?php echo formata_data($solicitacao['htddata']) ?><br/>
                                Local e Data
                            </td>
                            <?PHP
                            if ($solicitacao['entid']) {
                                $sql = "
                                            select  usu.usunome,
                                                    usu.usucpf,
                                                    nu_matricula_siape
                                            from seguranca.usuario usu
                                            inner join seguranca.perfilusuario pfu on pfu.usucpf = usu.usucpf
                                            inner join academico.usuarioresponsabilidade urs on urs.usucpf = usu.usucpf
                                            inner join siape.tb_siape_cadastro_servidor_ativos sia on sia.nu_cpf = usu.usucpf

                                            where pfu.pflcod = " . PFL_REITOR . " AND urs.entid = {$solicitacao['entid']} AND rpustatus='A'
                                        ";
                                $dadosreitor = $db->pegaLinha($sql);
                            }
                            ?>
                            <td align="center">
                                <?PHP
                                echo $dadosreitor['usunome'] . "<br>";
                                echo "CPF:" . $dadosreitor['usucpf'] . "<br>";
                                echo "Matrícula SIAPE:" . $dadosreitor['nu_matricula_siape'];
                                ?>
                            </td>
                        </tr>
                    </table>

                    <br>

                    <p>
                        <?PHP
                        $data = new Data();
                        $data = $data->formataData($solicitacao['htddata'], "Brasília DD de mesTextual de YYYY");
                        ?>
                        Autorização SIMEC nº <?= $solicitacao['numero'] ?>, <?= $data ?>.<br><br>
                        <b>
                            <span style="margin-left:50px">Autorizado</span>, devendo essa Instituição observar toda legislação pertinente à matéria.
                        </b> 
                        <br>
                    </p>
                    <center>
                        <?PHP
                        if ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_ministro.png"/>
                            <br/>
                        <?PHP
                        } elseif ($_SESSION['usucpf'] == '') {
                            ?>
                            <img width="230" height="100" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_ministro_interino.png"/>
                            <br/>
                        <?PHP
                        }
                        ?>
                    </center>
                </td>
            </tr>
            <tr>
                <td class="texto">
                    <p><i>
                            <font size="1">
                                Portaria MPOG nº 249, de 13 de junho de 2012 <br>
                                Art. 4º As autorizações de que tratam os arts. 2º e 4º do Decreto nº 7.689, de 2012,
                                constitui ato de governança das contratações
                                estritamente relacionado a uma avaliação sobre a conveniência da despesa pública, não
                                envolvendo a análise técnica e jurídica
                                do procedimento, que são de responsabilidade dos ordenadores de despesa e das unidades
                                jurídicas dos respectivos órgãos e
                                entidades, de acordo com suas competências legais, nem implicando ratificação ou
                                validação dos atos que compõem o processo
                                de contratação.(publicação no DOU de 14/06/2012 e retificação no DOU de 21/06/2012)
                            </font>
                        </i></p>
                </td>
            </tr>
            <tr>
                <td class="texto">
                    <br>
                    Autorização emitida por meio do Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC
                </td>
            </tr>
        </table>
    <?php
    }

    $html = ob_get_contents();
    ob_clean();

    //23000.001738/2012-31
    $stNew = "";
    if ($solicitacao['numero'] == '20120912.0038') {
        $stNew = "_2";
    }

    $dir = APPRAIZ . 'arquivos/academico/autorizacoesdecretogovernanca/';

    if (!is_dir($dir)) {
        mkdir($dir, 0777);
    }

    if (is_file($dir . 'autorizacao_' . str_replace(".", "_", $solicitacao['numero']) . $stNew . '.pdf')) {
        $contents = file_get_contents($dir . 'autorizacao_' . str_replace(".", "_", $solicitacao['numero']) . $stNew . '.pdf');
    } else {
        $content = http_build_query(array('conteudoHtml' => utf8_encode($html)));

        $context = stream_context_create(array('http' => array('method' => 'POST', 'content' => $content)));

        $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);

        $fp = fopen($dir . 'autorizacao_' . str_replace(".", "_", $solicitacao['numero']) . $stNew . '.pdf', 'w+');
        fwrite($fp, $contents);
        fclose($fp);
    }

    if ($dados['impri'] == 'N') {
        return true;
    } else {
        header('Content-Type: application/pdf');
        header("Content-Disposition: attachment; filename=autorizacao_" . str_replace(".", "_", $solicitacao['numero']) . $stNew . ".pdf");
        echo $contents;
        exit;
    }
}

function geraPDFAutorizacaoDecreto($sbsid)
{
    $dados['sbsid'] = $sbsid;
    $dados['impri'] = 'N'; #parametro para que não seja impresso o decreto mas, apenas criado o arquivo.

    $result = exibirSolicitacaoDescreto($dados);

    return $result;
}

function formata_valor_sql($valor)
{
    $valor = str_replace('.', '', $valor);
    $valor = str_replace(',', '.', $valor);

    return $valor;
}

function cria_aba_2($abacod_tela, $url, $parametros, Array $arMnuid = array(), $nomeAba = 'Abas')
{
    global $db;
    $parametro = is_array($parametros) ? $parametros[$j] : $parametros;

    $where = "";
    if ($_SESSION['sisid']) {
        $where = " and menu.sisid = {$_SESSION['sisid']} ";
    }

    if ($arMnuid) {
        $filtro = "and menu.mnuid not in (" . implode(',', $arMnuid) . ")";
    }

    //Função cria aba que monta as abas visualmente
    if (simec_trim($abacod_tela) <> '') {
        $sql = <<<DML
            SELECT
                menu.mnuid, menu.mnudsc, menu.mnulink, menu.mnutransacao
            FROM seguranca.menu, seguranca.aba_menu
            WHERE menu.mnuid = aba_menu.mnuid
                AND aba_menu.abacod = $abacod_tela 
                $where
                AND menu.mnuid IN (
                    SELECT DISTINCT m2.mnuid FROM seguranca.perfilmenu m2, seguranca.perfilusuario p
                    WHERE m2.pflcod = p.pflcod AND p.usucpf = '{$_SESSION['usucpf']}'
                ) $filtro
            ORDER BY menu.mnudsc
DML;

        if (isset($_SESSION['abamenu'][$abacod_tela])) {
            $rs = $_SESSION['abamenu'][$abacod_tela];
        } else {
            $rs = $db->carregar($sql);
            $_SESSION['abamenu'][$abacod_tela] = $rs;
        }

        ?>
        <div class="list-group"
             style='font-family: "Helvetica Neue",Helvetica,Arial,sans-serif;font-size: 14px;position:inherit;'>
            <a class='list-group-item list-group-item-info' style='position:inherit;'><span
                    class='glyphicon glyphicon-th-list' style='position:inherit;'></span>
                <strong><?= $nomeAba ?> </strong></a>
            <?php
            if (is_array($rs)) {
                foreach ($rs as $tab):
                    if ($tab['mnulink'] === 'academico.php?modulo=principal/maisMedicos&acao=A') {
                        continue;
                    }
                    ?>
                    <a style='position:inherit;' class='list-group-item <?= $tab['mnulink'] == $url ? 'active' : '' ?>'
                       id="tab<?php echo str_replace(' ', '', $tab['mnutransacao']) ?>"
                       href="<?php echo $tab['mnulink'] . $parametro ?>"><span
                            class='glyphicon glyphicon-<?= $tab['mnulink'] == $url ? 'minus' : 'share-alt' ?>'
                            style='position:inherit;'></span> <?php echo $tab['mnudsc'] ?></a>
                <?php endforeach;
            }
            ?>
        </div>
    <?php
    }
}


function enviarEmailTramiteGm()
{   
    global $db;	
	$autoriazacaoconcursos = new autoriazacaoconcursos();
    $cabecalho= $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);	
    //$sbsid = $_SESSION['academico']['sbsid'];
    $entid = $_SESSION['academico']['entid'];
    $estado = wf_pegarEstadoAnterior($_GET['docid']);
    $acao = wf_pegarAcaoPorId($estado['aedid']);
    $sqlEmail = "SELECT email FROM academico.emailinauguracao
                WHERE status like 'G'";
    $arrEmail = $db->carregarColuna($sqlEmail);
    $assunto = 'SOLICITAÇÃO DE DECRETO PARA ANÁLISE (' . date('d/m/Y') . ')';
    $conteudo = "$cabecalho";
    $conteudo .= "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center' style='margin-top:40px'><tr><td style='width:250px'>Ação :</td><td> $acao[aeddscrealizada]</td></tr></table>";
    $conteudo .= "<br><br>";
    $conteudo .= "<br>";
    $conteudo .= "<br>";
    $conteudo .= "<br>";
    $conteudo .= "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center' style='margin-top:40px'><tr><td style='width:250px'><b>Tramitado por:</b></td><td style='width:250px'>{$_SESSION['usunome']} em " . date('d/m/Y h:m:s')."</td></tr>";
	
    require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');

    if (array_key_exists(0, $arrEmail)) {
        $e = new EmailAgendado();
        $e->setTitle($assunto);
        $e->setText($conteudo);
        $e->setName("SIMEC");
        $e->setEmailOrigem("simec@mec.gov.br");
        $e->setEmailsDestino($arrEmail);
        $e->enviarEmails();

        return true;
    } else {
        return true;
    }
}


function enviarEmailTramiteSetec()
{   
    global $db;	
	$autoriazacaoconcursos = new autoriazacaoconcursos();
    $cabecalho= $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);	
    //$sbsid = $_SESSION['academico']['sbsid'];
    $entid = $_SESSION['academico']['entid'];
    $estado = wf_pegarEstadoAnterior($_GET['docid']);
    $acao = wf_pegarAcaoPorId($estado['aedid']);
    $sqlEmail = "SELECT email FROM academico.emailinauguracao
                WHERE status like 'S'";
    $arrEmail = $db->carregarColuna($sqlEmail);
    $assunto = 'SOLICITAÇÃO DE DECRETO PARA ANÁLISE (' . date('d/m/Y') . ')';
    $conteudo = "$cabecalho";
    $conteudo .= "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center' style='margin-top:40px'><tr><td style='width:250px'>Ação :</td><td> $acao[aeddscrealizada]</td></tr></table>";
    $conteudo .= "<br><br>";
    $conteudo .= "<br>";
    $conteudo .= "<br>";
    $conteudo .= "<br>";
    $conteudo .= "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center' style='margin-top:40px'><tr><td style='width:250px'><b>Tramitado por:</b></td><td style='width:250px'>{$_SESSION['usunome']} em " . date('d/m/Y h:m:s')."</td></tr>";
	
    require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');

    if (array_key_exists(0, $arrEmail)) {
        $e = new EmailAgendado();
        $e->setTitle($assunto);
        $e->setText($conteudo);
        $e->setName("SIMEC");
        $e->setEmailOrigem("simec@mec.gov.br");
        $e->setEmailsDestino($arrEmail);
        $e->enviarEmails();

        return true;
    } else {
        return true;
    }

}

function permissaoInterna($pflcods)
{
    global $db;
    if (is_array($pflcods)) {
        $pflcods = array_map("intval", $pflcods);
        $pflcods = array_unique($pflcods);
    } else {
        $pflcods = array((integer)$pflcods);
    }
    if (count($pflcods) == 0) {
        return false;
    }
    $in = implode(",", $pflcods);
    $sql = <<<DML
        SELECT
            COUNT(*)
        FROM seguranca.perfilusuario
        WHERE usucpf = '{$_SESSION['usucpf']}'
            AND pflcod IN ({$in})
DML;
    return $db->pegaUm($sql) > 0;
}

function verificaExistenciaEntidades()
{
    if($_SESSION['academico']['entid'] == ''){
        echo "<script>
			alert('Seleciona uma Unidade!');
			window.location = 'academico.php?modulo=inicio&acao=C';
		  </script>";
        die;
    }
    if($_SESSION['academico']['entidadenivel'] == 'campus'){
        if($_SESSION['academico']['entidcampus'] == ''){
            echo "<script>
                alert('Seleciona um Campus!');
                window.location = 'academico.php?modulo=principal/listadecampi&acao=A';
              </script>";
            die;
        }
    }
}

function inArrayMultiple($opcoes,$perfis)
{
    if(is_array($opcoes)) {
        return array_intersect($opcoes, $perfis) ? true : false;
    } else {
        return in_array($opcoes,$perfis);
    }
    return false;
}

function alertaPerfilEmailInalguracao()
{
	global $db;
	
	if ($_SESSION['academico']['orgid'] == 1) {
		$funid = ACA_ID_CAMPUS;
	} else {
		$funid = ACA_ID_UNED;
	}
	
	$sql = "select
				hst.htddata as data,
				esd1.esddsc as estadoorigem,
				esd2.esddsc as estadodestino,
				e1.entnome as unidade,
				e2.entnome as unidade2,
				usu.usuemail,
				case 	
					when ina.ingtipo = 'R'
						then 'Reitoria'
						else
							case
								when ina.ingtipo = 'U'
								then 'Unidade'
								else 'Nova Obra'
							end
				end as tipoina,
				ingdscobra,
				ingdscreitor,
				ingnomedirigente,
				to_char(ingdtinstprov, 'DD/MM/YYYY')  as dataprovisória,
				to_char(ingsuginauguracao, 'DD/MM/YYYY') as dtinalguracao,
				to_char(ingdtinstdef, 'DD/MM/YYYY') as dtdefinitiva,	
				inginaugjustificativa,
				edocep as cep,
				edodsc as logradouro,
				edocomplemento as complemento,
				edobairro as bairro,
				endObra.estuf,
				mundescricao,
				ina.*,			
				endObra.*
			from
				academico.inauguracao ina
			inner join 
				academico.endobra endObra ON endObra.ingid = ina.ingid
			inner join
				territorios.municipio mun ON mun.muncod = endObra.muncod
			inner join
				workflow.documento d ON d.docid = ina.docid
			left join
				workflow.historicodocumento hst ON hst.hstid = d.hstid 
			left join
				workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
			left join 
				workflow.estadodocumento esd1 ON esd1.esdid = esdidorigem
			left join 
				workflow.estadodocumento esd2 ON esd2.esdid = esdiddestino
			inner join 
				entidade.entidade e1 ON e1.entid = ina.entidies
			left join
				entidade.entidade e2 ON e2.entid = ina.entidcampus
			inner join  
				entidade.funcaoentidade ef ON ef.entid = e2.entid
			inner join
				seguranca.usuario usu ON usu.usucpf = ina.usucpf
			WHERE	
				entidies ={$_SESSION['academico']['entid']} AND ef.funid = {$funid}";
	
	$arrDados = $db->pegaLinha($sql);
	
	if ($arrDados) {
		extract($arrDados);
	} else {
		return false;
	}

	$texto = "
		Tramitação Realizada em:" . date('d/m/Y', strtotime($data)) . ", às " . date('H', strtotime($data)) . "h.
	<br>
		Usuario:  \"{$_SESSION['usunome']}\"
	<br>
		Origem:\"$estadoorigem\"
	<br>
		Destino:\"$estadodestino\"
	<br>
		\"$unidade\" - \"$unidade2\" - AUTORIZAÇÃO DE INAGURAÇÃO
	<br><br>
		<b> Dados Inauguração </b>
	<br><br>
			Inauguração:\"$tipoina\"
	<br>
			Instituição:\"$unidade\"
	<br>
			Unidade:\"$unidade2\"
	<br>
			Obra:\"$ingdscobra\"
	<br><br>
		<b>	Dirigentes</b>
	<br><br>
			Reitor:\"$ingdscreitor\"
	<br>
			Dirigente:\"$ingnomedirigente\"
	<br><br>
		<b> Data de Início de Funcionamento </b>
	<br><br>
			Instalações Provisórias:\"$dataprovisória\"  Instalações Definitivas:\"$dtdefinitiva\"
	<br>
			Sugestão de Data de Inauguração:\"$dtinalguracao\"
	<br>
			Justificativa para Data de Inauguração Sugerida:\"$inginaugjustificativa\"
	<br><br>
		<b> Endereço </b>
	<br><br>
			CEP:\"$cep\"
	<br>
			Logradouro:\"$logradouro\"
	<br>
			Complemento:\"$complemento\"
	<br>
			Bairro:\"$bairro\"
	<br>
			UF:\"$estuf\"
	<br>
			Município:\"$mundescricao\"
	";
// 	ver($texto,d);
	$titulo = "Autorização de Inauguração";

	$sql = "select
 				email
 			from
 				academico.emailinauguracao
 			where
 				status = 'A'";
	$arrEmail = $db->carregarColuna($sql);
	
// 	$arrEmail = !$arrEmail ? array("LotavinoDunice@mec.gov.br","MarioSiqueira@mec.gov.br") : $arrEmail;
	
	if ($_SESSION['baselogin'] == "simec_espelho_producao" || $_SESSION['baselogin'] == "simec_desenvolvimento") {
		$arrEmail = array("lotavinodunice@mec.gov.br","MarioSiqueira@mec.gov.br");
	}

	if ($usuemail) {
		$arrEmail[] = $usuemail;
	}
	
	simec_email('', $arrEmail, $titulo, $texto);
	return true;
}


function controlaAcessoMonitoramentoPlanejamento($usucpf, $acao){
    global $db;

    if($acao=='E'){
        $sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod = 1428 AND usucpf = '{$usucpf}'";
        $db->executar($sql);

        $sql = "SELECT COUNT(0)
                FROM seguranca.perfilusuario
                WHERE pflcod IN (SELECT pflcod FROM seguranca.perfil WHERE sisid = 132)
                AND usucpf = '{$usucpf}'";
        $possuiAcesso = $db->pegaUm($sql);
        if($possuiAcesso==0){
            $sql = "UPDATE seguranca.usuario_sistema SET suscod = 'B' WHERE sisid = 132 AND usucpf = '{$usucpf}'";
            $db->executar($sql);
        }

    }elseif($acao=='I'){
        $sql = "SELECT suscod
                FROM seguranca.usuario_sistema
                WHERE sisid = 132
                AND usucpf = '{$usucpf}'";
        $possuiAcesso = $db->pegaUm($sql);
        if($possuiAcesso){
            if($possuiAcesso['suscod'] == 'A'){
                $sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod = 1428 AND usucpf = '{$usucpf}'";
                $db->executar($sql);
                $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$usucpf}', 1428)";
                $db->executar($sql);
            }else{
                $sql = "DELETE FROM pde.usuarioresponsabilidade WHERE usucpf = '{$usucpf}'";
                $db->executar($sql);
                $sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod IN (SELECT pflcod FROM seguranca.perfil WHERE sisid = 132) AND usucpf = '{$usucpf}'";
                $db->executar($sql);
                $sql = "UPDATE seguranca.usuario_sistema SET suscod = 'A' WHERE sisid = 132 AND usucpf = '{$usucpf}'";
                $db->executar($sql);
                $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$usucpf}', 1428)";
                $db->executar($sql);
            }
        }else{
            $sql = "INSERT INTO seguranca.usuario_sistema (usucpf, sisid, pflcod) VALUES ('{$usucpf}', 132, 1428)";
            $db->executar($sql);
            $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$usucpf}', 1428)";
            $db->executar($sql);
        }
    }
    $db->commit();
}

?>