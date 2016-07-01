<?php
ini_set("memory_limit", "3000M");
set_time_limit(0);

if ($_POST['requisicao'] == 'gera') {
    relatorio();
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

function mascaraglobal($value, $mask) {
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

monta_titulo('Relatório de Subações', '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form name="formulario" id="formulario" method="post">
    <table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
        <input type="hidden" id="requisicao" name="requisicao" value="">
<!--         <tr> -->
<!--             <td class="SubTituloDireita" >Tipo:</td> -->
<!--             <td colspan="2"> -->
<!--                 <input type="radio" disabled name="item" value="S" >Subação -->
<!--                 <input type="radio" disabled name="item" value="I" >Item de Composição -->
<!--             </td>  -->
<!--         </tr> -->
        <tr>
            <td class="SubTituloDireita" >Esfera:</td>
            <td colspan="2">
                <input type="radio" name="esfera" value="M" >Municipal
                <input type="radio" name="esfera" value="E" >Estadual
            </td> 
        </tr>
        <tr>
            <td class="SubTituloDireita" >Estado:</td>
            <td colspan="2">
                <?php
                $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                $db->monta_combo("estuf", $sql, 'S', 'Todas as Unidades Federais', '', '');
                ?>
            </td>
        </tr>
        <tr bgcolor="#D0D0D0">
            <td colspan="4" style="text-align: center">
                <input type="button" value="Gerar Excel" name="btnGerar" id="btnGerar" style="cursor: pointer;" onclick="gerarExcel();">
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
    function gerarExcel() {
        $('#requisicao').val('gera');
        $('#formulario').submit();
    }
</script>
<?php

function relatorio() {
    global $db;

    if ($_POST['estuf']) {
        $where = " AND (iu.estuf = '" . $_POST['estuf'] . "' OR iu.mun_estuf = '" . $_POST['estuf'] . "')";
    }
    
    if ($_POST['esfera'] == 'M') {
        $where .= " AND iu.itrid = 2";
    } elseif ($_POST['esfera'] == 'E') {
        $where .= " AND iu.itrid = 1";
    }

    $sql = "SELECT DISTINCT
                CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as uf,
                CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao || ' - ' || iu.mun_estuf END as entidade,
                TO_CHAR(prp.prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as processo,
                dop.dopnumerodocumento as documento, 
                CASE WHEN dpv.dpvid IS NOT NULL THEN 'Validado' ELSE 'Não validado' END as validacao,
                tpd.tpddsc as tipo,
                par.retornacodigosubacao(sd.sbaid) as codigosubacao,
                REPLACE(REPLACE(s.sbadsc, '\n', ''), '\r', '') as subacao,
                sd.sbdano as ano,
                par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano) as valorsubacao,
                de.saldo as valorempenho,
                CASE WHEN pag.pagid IS NOT NULL THEN sum( pobvalorpagamento ) END as valorpagamento,
                pag.pagsituacaopagamento as situacaopagamento,
                CASE WHEN iu.itrid = 1 THEN 'Estadual' ELSE 'Municipal' END as esfera,
                CASE WHEN s.frmid IN ( 14, 15 ) THEN 'Sim' ELSE 'Não' END as emenda,
                ssu.ssudescricao as status,
                par.recuperaquantidadeitensplanejadossubacaoporano(sd.sbaid, sd.sbdano) as itensplanejados,
                par.recuperaquantidadeitensvalidadossubacaoporano(sd.sbaid, sd.sbdano) as itensvalidados,
                iue.iuesituacaohabilita habilita,
                dop.dopdatafimvigencia as vigencia,
                CASE WHEN iu.itrid = 1 
                        THEN (SELECT CASE  WHEN COUNT (*) > 0 THEN 'Sim' ELSE 'Não' END FROM obras2.vm_pendencia_obras WHERE estuf = iu.estuf AND empesfera = 'E')
                        Else (SELECT CASE  WHEN COUNT (*) > 0 THEN 'Sim' ELSE 'Não' END FROM obras2.vm_pendencia_obras WHERE muncod = iu.muncod AND empesfera = 'M')
                END as pendencia_obra,
                CASE WHEN (SELECT COUNT(s.sbaid) FROM par.subacao s INNER JOIN par.subacaodetalhe sd  ON sd.sbaid = s.sbaid INNER JOIN par.termocomposicao tc  ON tc.sbdid = sd.sbdid WHERE tc.dopid = dop.dopid) > 0 THEN 'Sim' ELSE 'Não' END as contrato_inserido,
                CASE WHEN (SELECT COUNT(sic.sbaid) FROM par.subacaoitenscomposicao sic INNER JOIN par.subacaoitenscomposicaonotasfiscais sicnf ON sicnf.icoid = sic.icoid AND sic.sbaid = s.sbaid ) > 0 THEN 'Sim' ELSE 'Não' END as nota_fiscal_inserida,
                (SELECT count(DISTINCT ses.escid) 
				FROM par.subacaoescolas ses
				INNER JOIN par.escolas 				e   ON ses.escid = e.escid 
				INNER JOIN entidade.entidade 			ent ON ent.entid = e.entid
				INNER JOIN par.subacao				sba ON sba.sbaid = ses.sbaid
				INNER JOIN par.acao 	 			a   ON a.aciid  = sba.aciid
				INNER JOIN par.pontuacao 			p   ON p.ptoid  = a.ptoid
				INNER JOIN par.instrumentounidade 		inu ON inu.inuid = p.inuid
				WHERE 
					ses.sbaid = sd.sbaid AND ses.sesano = sd.sbdano AND ( (inu.itrid = 1 AND ent.tpcid = 1) OR (inu.itrid = 2 AND ent.tpcid = 3) )
					AND (ent.entescolanova = false OR ent.entescolanova IS NULL) AND ent.entstatus = 'A' AND ses.sesstatus = 'A') as qtd_escolas_atendidas
            FROM 
                par.instrumentounidade iu
            INNER JOIN par.processopar 					prp ON prp.inuid = iu.inuid
            left  join par.instrumentounidadeentidade 	iue ON iue.iuecnpj = prpcnpj
            LEFT  JOIN par.vm_documentopar_ativos		dop 
	            INNER JOIN par.modelosdocumentos 		mdo ON mdo.mdoid = dop.mdoid
	            INNER JOIN public.tipodocumento  		tpd ON tpd.tpdcod = mdo.tpdcod
	            LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dop.dopid 
            												ON dop.prpid = prp.prpid
            LEFT  JOIN territorios.municipio 			mun ON mun.muncod = iu.muncod
            INNER JOIN par.processoparcomposicao 		ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
            INNER JOIN par.subacaodetalhe 				sd  ON sd.sbdid = ppc.sbdid
            INNER JOIN par.subacao 						s   ON s.sbaid = sd.sbaid
            INNER JOIN par.statussubacao 				ssu ON ssu.ssuid = sd.ssuid
            LEFT  JOIN par.v_dadosempenhosubacao 		de  ON de.sbdid = sd.sbdid AND de.processo = prp.prpnumeroprocesso
            LEFT  JOIN par.pagamentosubacao 			pas ON pas.sbaid = sd.sbaid AND pas.pobano = sd.sbdano and pobstatus = 'A'
            LEFT  JOIN par.pagamento 					pag ON pag.pagid = pas.pagid AND pag.pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pag.pagstatus = 'A' AND pag.empid = de.empid
            WHERE
                1 = 1
                " . $where . "
            GROUP BY
                iu.itrid,
                iu.estuf,
                iu.mun_estuf,
                mun.mundescricao,
                prp.prpnumeroprocesso,
                dop.dopnumerodocumento, 
                dop.dopdatafimvigencia, 
                dop.dopid,
                dpv.dpvid,
                tpd.tpddsc,
                sd.sbaid, 
                sd.sbdano,
                s.sbadsc,
                s.frmid,
                s.sbaid,
                pag.pagsituacaopagamento,
                pag.pagid,
                ssu.ssudescricao,
                habilita,
                pendencia_obra,
                de.saldo
            ORDER BY
                1,2,3,4";
    $registros = $db->carregar($sql);

    if ($registros[0]) {

        $arCabecalho = array('UF', 'Entidade', 'Processo', 'Documento', 'Vigência', 'Validação', 'Tipo', 'Código da Subação', 'Subação', 'Ano', 'Valor da Subação', 'Valor Empenhado', 'Valor Pago', 'Situação Pagamento', 'Esfera', 'Subação de Emenda', 'Status da Subação', 'Quantidade de Itens Planejados', 'Quantidade de Itens Validados', 'Habilitação', 'Pendência Obra', 'Contrato Inserido', 'Nota Fiscal Inserida', 'Qtd de Escolas Atendidas');

        foreach ($registros as $reg) {

            $arDados[] = array('uf' => $reg['uf'],
                'entidade' => $reg['entidade'],
                'processo' => $reg['processo'],
                'documento' => $reg['documento'],
                'vigencia' => $reg['vigencia'],
                'validacao' => $reg['validacao'],
                'tipo' => $reg['tipo'],
                'codigosubacao' => $reg['codigosubacao'],
                'subacao' => $reg['subacao'],
                'ano' => $reg['ano'],
                'valorsubacao' => str_replace(".", ",", round($reg['valorsubacao'], 2)),
                'valorempenho' => str_replace(".", ",", round($reg['valorempenho'], 2)),
                'valorpagamento' => str_replace(".", ",", round($reg['valorpagamento'], 2)),
                'situacaopagamento' => $reg['situacaopagamento'],
                'esfera' => $reg['esfera'],
                'emenda' => $reg['emenda'],
                'status' => $reg['status'],
                'itensplanejados' => $reg['itensplanejados'],
                'itensvalidados' => $reg['itensvalidados'],
                'habilita' => $reg['habilita'],
                'pendencia_obra' => $reg['pendencia_obra'],
                'contrato_inserido' => $reg['contrato_inserido'],
                'nota_fiscal_inserida' => $reg['nota_fiscal_inserida'],
                'qtd_escolas_atendidas' => $reg['qtd_escolas_atendidas']
            );
        }
    }

    ob_clean();
    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=SIMEC_Relat" . date("Ymdhis") . ".xls");
    header("Content-Disposition: attachment; filename=SIMEC_RelatPAR" . date("Ymdhis") . ".xls");
    header("Content-Description: MID Gera excel");
    $db->monta_lista_tabulado($arDados, $arCabecalho, 100000, 5, 'N', '100%', $par2);
    exit;
}
?>