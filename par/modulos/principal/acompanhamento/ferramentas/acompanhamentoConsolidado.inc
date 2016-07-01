<?php
include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';
monta_titulo($titulo_modulo, '');
?>

<script type="text/javascript" src="/library/fancybox/helpers/jquery.mousewheel-3.0.6.pack.js"></script>

<!-- <script type="text/javascript" src="../includes/prototype.js"></script> -->

<script type="text/javascript" src="../includes/funcoes.js"></script>

<style type="text/css">
</style>

<script type="text/javascript">

    function submeterTela(valor) {
        document.getElementById('exportarexcel').value = valor;
        document.formulario.submit();
    }

</script>

<form method="post" name="formulario" id="formulario">
    <input type="hidden" name="pesquisa" value="1"/>
    <input type="hidden" name="valuecp" id="valuecp" value="<?= $_REQUEST['classificacaoprocesso'] ?>"/>
    <input type="hidden" id="exporta" name="exporta" value="<?= $exporta; ?>">

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Número de Processo:</td>
            <td>
<?php
$filtro = simec_htmlentities($_REQUEST['numeroprocesso']);
$numeroprocesso = $filtro;
//echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '', '');
echo campo_texto('numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', '', '', '', '', '', '', $numeroprocesso);
?>

            </td>
        </tr>
        <tr>
            <td align='right'  class="SubTituloDireita">Tipo de Processo:</td>
            <td>
                <?php
                $sql = array(
                    array('codigo' => 'PAR', 'descricao' => 'PAR'),
                    array('codigo' => 'OBRAS_PAR', 'descricao' => 'OBRAS PAR'),
                    array('codigo' => 'PAC', 'descricao' => 'PAC')
                );
                $tprocesso = simec_htmlentities($_REQUEST['tipoprocesso']);
                $db->monta_combo("tipoprocesso", $sql, 'S', 'Selecione o Tipo de Processo', 'changeTipoProcesso', '', '', '', '', '', '', $tprocesso);
                ?>
            </td>
        </tr>
        <tr style="display:none;" id="tr_classificacao">
            <td align='right'  class="SubTituloDireita">Classificação do Processo:</td>
            <td>
                <span id="span_classificacao" style="display:none;">

                </span>
            </td>
        </tr>

        <tr>
            <td align='right'  class="SubTituloDireita">Esfera:</td>
            <td>
<?php
$sql = array(
    array('codigo' => 1, 'descricao' => 'Estadual'),
    array('codigo' => 2, 'descricao' => 'Municipal')
);
$esfera = simec_htmlentities($_REQUEST['esfera']);
$db->monta_combo("esfera", $sql, 'S', 'Selecione a esfera', '', '');
?>
            </td>
        </tr>

        <tr>
            <td class="subtituloDireita">UF:</td>
            <td>
<?php
$sql = "SELECT		estuf as codigo,
											estdescricao as descricao
								FROM		territorios.estado
								ORDER BY 	estuf";
$estuf = simec_htmlentities($_REQUEST['estuf']);
$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', '');
?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Município:</td>
            <td>
<?php
$filtro = simec_htmlentities($_REQUEST['municipio']);
$municipio = $filtro;
echo campo_texto('municipio', 'N', 'S', '', 70, '', '', '');
?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Vigência (##/####):</td>
            <td>
<?php
$vigencia = $_REQUEST['vigencia'];
echo campo_texto('vigencia', 'N', 'S', '', 8, '', '##/####', '');
?>
            </td>
        </tr>
        <tr>
            <td colspan="3" class="SubTituloDireita" style="text-align:  center;">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="submeterTela('nao')" />
                <input type="button" name="excel" value="Exportar Excel" onclick="submeterTela('sim')" />
                <input type="hidden" name="exportarexcel" id="exportarexcel" value="nao" />
            </td>
        </tr>

    </table>
</form>

<?php
if ($_REQUEST['estuf']) {
    $w = " AND (uf = '{$_REQUEST['estuf']}') ";
}

if ($_REQUEST['municipio']) {
    $w .= " AND removeacento(municipio) ILIKE removeacento('%" . $_REQUEST['municipio'] . "%') ";
}
if ($_REQUEST['vigencia']) {
    $filtros .= " AND dopdatafimvigencia = '" . $_REQUEST['vigencia'] . "' ";
    $wPAC .= " AND vigencia = '" . $_REQUEST['vigencia'] . "' ";
}
if ($_REQUEST['esfera']) {
    $filtros = " AND CASE WHEN " . $_REQUEST['esfera'] . " = 1 THEN (iu.itrid = 1 OR iu.inuid = 1) ELSE (iu.itrid = 2 AND iu.inuid <> 1) END ";
    $wPAC .= " AND esfera = " . $_REQUEST['esfera'] . " ";
}
if ($_REQUEST['tipoprocesso']) {
    $w .= " AND tipoprocesso = '" . $_REQUEST['tipoprocesso'] . "' ";
}
if ($_REQUEST['numeroprocesso']) {
    $w .= " AND processo = '" . str_replace(array("-", ".", "/"), "", $_REQUEST['numeroprocesso']) . "' ";
}

if ($_REQUEST['exportarexcel'] == 'nao') {
    $acao = "'-' as acoes, ";
}

$sql = " -- PAR
		(SELECT DISTINCT
			{$acao} 
			substring(processo from 12 for 4) as ano,
			uf as uf,
			municipio as municipio,
			substring(processo from 1 for 5)||'.'||
			substring(processo from 6 for 6)||'/'||
			substring(processo from 12 for 4)||'-'||
			substring(processo from 16 for 2) as processo,
			doc::character varying, 
			dopvalortermo as vt, 
			valorempenho as ve, 
			raf as valorraf,
			complemento as valorcomplemento,
			valorpagamentosolicitado as ps,
			valorpagamento as vp,  
			par.retornasaldoprocesso(processo) as sb,
			dopdatafimvigencia as vigencia,
			tipodocumento, 
			habilitado as habilitacao,
			CASE 
				WHEN acompanhamento THEN 'Em Acompanhamento'
				WHEN EXISTS ( SELECT si.icoid 
						FROM par.subacao s
						INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
						INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
						INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano 
						INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
						INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
						WHERE 	s.sbaid in ( SELECT DISTINCT s.sbaid
									FROM par.subacao s
									inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
									inner join par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
									inner join par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
									WHERE dp.dopid = id )
							AND tc.dopid = id )
					THEN 'Acompanhamento finalizado'
				ELSE 'Acompanhar' 
			END as acompanhamento,
			CASE WHEN mdoqtdvalidacao = 0 THEN 'Validado' WHEN mdoqtdvalidacao = validacoes THEN 'Validado' ELSE 'Não Validado' END as validado,
			CASE WHEN prpfinalizado IS NULL THEN '' ELSE 'Processo Finalizado' END as status,
			'-' as pendencias,
			'-' as restricao,
			CASE WHEN (SELECT count(atpid) FROM par.anexotorpar WHERE atpstatus = 'A' AND dopid = id) > 0 THEN 'Sim' ELSE 'Não' END as tr,
			CASE WHEN reformulacaoS > 0 THEN 'Sim' ELSE 'Não' END as reformulacaoS,
			CASE WHEN reformulacaoP > 0 THEN 'Sim' ELSE 'Não' END as reformulacaoP
		FROM (
			SELECT 	
				dp.dopid as id,
				dp.dopacompanhamento as acompanhamento, 
				dp.dopidpai as dopidpai, 
				d.mdoid as mdoid,
				d.tpdcod as tipo_doc, 
				d.mdonome as tipodocumento, 
				iu.inuid, 
				iu.itrid, 
				iu.estuf, 
				iu.muncod, 
				dp.dopusucpfvalidacaogestor, 
				d.mdoqtdvalidacao, 
				dp.dopnumerodocumento as doc, 
				pp.prpnumeroprocesso as processo,
				(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, 
				dp.dopvalortermo::numeric(20,2), 
				em.saldo as valorempenho,
				pgs.valor_pagamento as valorpagamentosolicitado,
				pge.valor_pagamento as valorpagamento,
				dp.arqid,
				dp.dopdatafimvigencia,
				'Banco: '||coalesce(prpbanco, 'n/a')||'
				Conta: '||coalesce(prpagencia, 'n/a')||'
				Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
				CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as uf,
				CASE WHEN iu.itrid = 1 THEN '' ELSE m.mundescricao END as municipio,
				'PAR'::character varying as tipoprocesso,
				dv.validacoes,
				prpfinalizado,
				CASE 
					WHEN iue.iuesituacaohabilita = 'Habilitado' THEN '<center><img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\"></center>' 
	              	ELSE '<center><img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\"></center>' 
	            END as habilitado,
				(SELECT sum(sd.sbdrepassevlrrafaprovado) 
				FROM par.subacaodetalhe sd
				WHERE sd.sbdid IN ( SELECT sbdid FROM par.termocomposicao tc WHERE dopid = dp.dopid ) AND 
					sd.sbdrepassevlrrafaprovado IS NOT NULL )::character varying as raf,
				(SELECT sum(sd.sbdrepassevlrcomplementaraprovado) 
				FROM par.subacaodetalhe sd 
				WHERE sd.sbdid IN ( SELECT sbdid FROM par.termocomposicao tc WHERE dopid = dp.dopid ) AND 
					sd.sbdrepassevlrcomplementaraprovado IS NOT NULL )::character varying as complemento,
				(SELECT count(sbdid)
				FROM par.documentoparreprogramacaosubacao dps 
				INNER JOIN par.reprogramacao rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
				WHERE dpsstatus in ('A', 'P') AND dps.dopid = dp.dopid) as reformulacaoS,
				(SELECT count(dopid)
				FROM par.documentoparreprogramacao
				WHERE dprstatus IN ('P') AND dopid = dp.dopid) as reformulacaoP
			FROM 
				par.vm_documentopar_ativos  dp
			INNER JOIN par.modelosdocumentos   			d   ON d.mdoid = dp.mdoid
			INNER JOIN par.processopar 					pp  ON pp.prpid = dp.prpid 
			INNER JOIN par.instrumentounidade 			iu  ON iu.inuid = pp.inuid
			LEFT  JOIN par.instrumentounidadeentidade 	iue ON iue.inuid = iu.inuid and iue.iuestatus = 'A' and iue.iuedefault = true
			LEFT  JOIN territorios.municipio 			m   ON m.muncod = iu.muncod
			LEFT  JOIN ( 	
				SELECT dopid, count(dpvid) as validacoes
				FROM par.documentoparvalidacao 
				WHERE dpvstatus = 'A'
				GROUP BY dopid 
    			) dv ON dv.dopid = dp.dopid
			INNER JOIN par.vm_saldo_empenho_do_processo em ON em.processo = pp.prpnumeroprocesso
			LEFT  JOIN (
				SELECT em.empnumeroprocesso as empnumeroprocesso, COALESCE(SUM(pg.pagvalorparcela),0.00) as valor_pagamento
				FROM par.pagamento pg
				INNER JOIN par.empenho em ON em.empid = pg.empid AND empstatus = 'A' AND pg.pagstatus = 'A' 
										     AND trim(pg.pagsituacaopagamento) IN ( 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 
										     										'8 - SOLICITAÇÃO APROVADA')
				GROUP BY empnumeroprocesso, pagstatus
				) as pgs on pgs.empnumeroprocesso = pp.prpnumeroprocesso 
			LEFT  JOIN(
				SELECT em.empnumeroprocesso as empnumeroprocesso, coalesce(sum(pg.pagvalorparcela),0.00) as valor_pagamento
				FROM par.pagamento pg
				INNER JOIN par.empenho em on em.empid = pg.empid and empstatus = 'A' AND pg.pagstatus = 'A' 
				WHERE TRIM(pg.pagsituacaopagamento) ILIKE '%EFETIVADO%'
				GROUP BY empnumeroprocesso, pagstatus
				) as pge ON pge.empnumeroprocesso = pp.prpnumeroprocesso 
			WHERE d.tpdcod in (21, 102, 103, 16) $filtros
			) as foo
		WHERE id IS NOT NULL $w 
		ORDER BY uf, municipio)
			
		UNION ALL
		
		-- Obras PAR
		(SELECT
				{$acao}
				substring(processo from 12 for 4) as ano,
				uf as uf,
				municipio as municipio,
				substring(processo from 1 for 5)||'.'||
	   			substring(processo from 6 for 6)||'/'||
	   			substring(processo from 12 for 4)||'-'||
	   			substring(processo from 16 for 2) as processo,
				doc::character varying, 
				dopvalortermo as vt, 
				valorempenho as ve,
				'-' as valorraf,
				'-' as valorcomplemento, 
				valorpagamentosolicitado as ps,
				valorpagamento as vp, 
				par.retornasaldoprocesso(processo) as sb,
				dopdatafimvigencia as vigencia,
				tipodocumento, 
				habilitado as habilitacao,
				'' as acompanhamento,
				CASE WHEN mdoqtdvalidacao = 0 THEN 'Validado' ELSE CASE WHEN mdoqtdvalidacao = validacoes THEN 'Validado' ELSE 'Não Validado' END END as validado,
				'' as status,
				'-' as pendencias,
				'-' as restricao,
				'-' as tr,
				'-' as reformulacaoS,
				'-' as reformulacaoP
			FROM (
				SELECT 
					dp.dopid as id, dp.dopidpai, d.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, dp.dopnumerodocumento as doc,
					'Banco: '||coalesce(probanco, 'n/a')||'<br> Conta: '||coalesce(proagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
					(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem,
					dp.dopvalortermo::numeric(20,2), 
					em.valor as valorempenho,
					pgs.valor_pagamento as valorpagamentosolicitado,
					pm.valor_pagamento as valorpagamento,
					--CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento END as valorpagamento,
					pp.pronumeroprocesso as processo,
					dp.dopdatafimvigencia,
					CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as uf,
					CASE WHEN iu.itrid = 1 THEN '' ELSE m.mundescricao END as municipio,
					'OBRAS_PAR'::character varying as tipoprocesso,
					--dv.mdoqtdvalidacao as validacao,
					dv.validacoes,
					case when iue.iuesituacaohabilita = 'Habilitado' then
	              		'<center><img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\"></center>' 
	              	else 
	              		'<center><img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\"></center>' end as habilitado
				FROM par.vm_documentopar_ativos dp
				INNER JOIN par.modelosdocumentos   			d   ON d.mdoid = dp.mdoid
				INNER JOIN par.processoobraspar 			pp  ON pp.proid = dp.proid and pp.prostatus = 'A'
				INNER JOIN par.instrumentounidade 			iu  ON iu.inuid = pp.inuid
				LEFT  JOIN par.instrumentounidadeentidade 	iue ON iue.inuid = iu.inuid and iue.iuestatus = 'A' AND iue.iuedefault = true
				LEFT  JOIN territorios.municipio 			m   ON m.muncod = iu.muncod
				LEFT  JOIN (
					SELECT
						dp.dopid,
						--mdo.mdoqtdvalidacao,
						count(dpvid) as validacoes
					FROM
						par.vm_documentopar_ativos dp
					INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dp.mdoid
					INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid AND dpv.dpvstatus = 'A'
					GROUP BY
						dp.dopid,
						mdo.mdoqtdvalidacao 
    				) dv ON dv.dopid = dp.dopid
				LEFT JOIN ( 
					SELECT
						dopid,
						sum(valor) as valor
					FROM
						(
						SELECT DISTINCT
							d.dopid, 
							vve.empid,
							vve.vrlempenhocancelado as valor
						FROM 
							par.documentopar d
						INNER JOIN par.processoobraspar prp on prp.proid = d.proid and prp.prostatus = 'A'
						INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
						INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp.empid
						LEFT  JOIN (
							SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
							FROM par.empenho
							WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
							GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
							) as emr ON emr.empidpai = emp.empid 
						inner join par.empenhoobrapar ems on ems.empid = emp.empid and eobstatus = 'A'  
						) as foo
					GROUP BY dopid
					) em ON em.dopid = dp.dopid
				LEFT JOIN (	SELECT 
								d.dopid, sum( pobvalorpagamento ) as valor_pagamento
				            FROM 
				            	par.vm_documentopar_ativos d
							INNER JOIN par.processopar 		prp ON prp.prpid = d.prpid
				            INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
				            INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
				            INNER JOIN par.pagamentosubacao ps  ON ps.pagid = pag.pagid AND pobstatus = 'A' 
				            GROUP BY d.dopid, pagsituacaopagamento
					) pm ON pm.dopid = dp.dopid
				LEFT JOIN (	SELECT 
								d.dopid, sum( pobvalorpagamento ) as valor_pagamento
							FROM 
								par.vm_documentopar_ativos d
							INNER JOIN par.processopar 		prp ON prp.prpid = d.prpid
							INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', 'Enviado ao SIGEF', 'AUTORIZADO', '0 - AUTORIZADO')
							INNER JOIN par.pagamentosubacao ps  ON ps.pagid = pag.pagid AND pobstatus = 'A' 
							GROUP BY d.dopid, pagsituacaopagamento
    					) pgs ON pgs.dopid = dp.dopid
				 WHERE d.tpdcod in (21, 102, 103, 16) $filtros
			) as foo 
			WHERE 1=1 {$w} )
			
			UNION ALL
			
			-- PAC
			
			(SELECT
				{$acao}
				substring(processo from 12 for 4) as ano,
				uf,
				municipio,
				substring(processo from 1 for 5)||'.'||
	   			substring(processo from 6 for 6)||'/'||
	   			substring(processo from 12 for 4)||'-'||
	   			substring(processo from 16 for 2) as processo,
				doc,
				vt,
				ve,
				'-' as valorraf,
				'-' as valorcomplemento,
                                ps,
				vp,
				sb,
				vigencia,
				'Termo do PAC' as tipodocumento, 
				'' as habilitacao,
				'' as acompanhamento,
				CASE WHEN terassinado = 't' THEN 'Validado' ELSE 'Não Validado' END as validado,
				'' as status,
				'-' as pendencias,
				'-' as restricao,
				'-' as tr,
				'-' as reformulacaoS,
				'-' as reformulacaoP
			FROM (
				SELECT 
					CASE WHEN pro.muncod IS NULL THEN pro.estuf ELSE m.estuf END as uf,
					CASE WHEN pro.muncod IS NULL THEN '' ELSE m.mundescricao END as municipio,
					CASE WHEN pro.muncod IS NULL THEN 1 ELSE 2 END as esfera,
					pro.pronumeroprocesso as processo,
					'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as doc,
					( select sum( prevalorobra ) from par.termoobra ter inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A' WHERE ter.terid = tc.terid ) as vt, 
					em.valor as ve, 
                                        sum(CASE WHEN pm.pagsituacaopagamento IN ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', 'Enviado ao SIGEF', 'AUTORIZADO', '0 - AUTORIZADO') THEN pm.valor_pagamento ELSE 0 END) as ps,
					sum(CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento ELSE 0 END) as vp,
					(par.retornasaldoprocesso(pronumeroprocesso)) as sb,
					terassinado,
					dadosp.fim as vigencia,
				--	*,
					'PAC'::character varying as tipoprocesso
				FROM 
					par.termocompromissopac  tc
				INNER JOIN par.processoobra 	pro ON pro.proid = tc.proid and pro.prostatus = 'A'
				LEFT JOIN territorios.municipio m   ON m.muncod = pro.muncod
				LEFT  JOIN seguranca.usuario 	u 	ON u.usucpf = tc.usucpfassinatura 
				LEFT JOIN (
					SELECT
						terid,
						sum(valor) as valor
					FROM
						( 
						SELECT DISTINCT
							tc.terid, 
							vve.empid,
							vve.vrlempenhocancelado + coalesce(emr.vrlreforco,0) as valor
						FROM 
							par.termocompromissopac tc
						INNER JOIN par.processoobra 			prp ON prp.proid = tc.proid and prp.prostatus = 'A'
						INNER JOIN par.empenho 					emp ON emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
						INNER JOIN par.v_vrlempenhocancelado 	vve ON vve.empid = emp.empid
						LEFT  JOIN (
							SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
							FROM par.empenho
							WHERE empcodigoespecie in ('02') AND empstatus = 'A'
							GROUP BY empnumeroprocesso, empcodigoespecie, empidpai) as emr on emr.empidpai = emp.empid 
						INNER JOIN par.empenhoobra ems on ems.empid = emp.empid and eobstatus = 'A' 
						) as foo
					GROUP BY terid
					) as em ON em.terid = tc.terid
				LEFT JOIN (
					SELECT 
						tc.terid, sum( pobvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
					FROM 
						par.termocompromissopac tc
					INNER JOIN par.processoobra 	prp ON prp.proid = tc.proid and prp.prostatus = 'A'
					INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
					INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento <> 'CANCELADO'
					INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid  
					GROUP BY tc.terid, pagsituacaopagamento ) pm ON pm.terid = tc.terid
				LEFT JOIN ( select
                               CASE WHEN pop.preid IS NOT NULL THEN TO_CHAR( MAX(popdataprazoaprovado), 'mm/YYYY' ) ELSE TO_CHAR(( MIN(p.pagdatapagamento) + 720 ), 'mm/YYYY' ) END as fim,
                               pro.proid
			                from
								par.pagamentoobra po
			                inner join par.pagamento p on p.pagid = po.pagid AND p.pagsituacaopagamento <> 'CANCELADO' AND p.pagstatus = 'A' AND p.pagparcela = 1
			                inner join par.empenho emp on emp.empid = p.empid AND emp.empsituacao <> 'CANCELADO' AND emp.empstatus = 'A'
			                inner join par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso AND pro.prostatus = 'A'
			                left  join obras.preobraprorrogacao pop on pop.preid = po.preid AND popvalidacao = 't'
			                GROUP BY pro.proid, pop.preid
			                ) as dadosp on dadosp.proid = pro.proid
					
				WHERE 
					tc.terstatus = 'A'
				GROUP BY
					tc.usucpfassinatura, tc.terassinado, tc.proid, tc.terid, tc.terdatainclusao, tc.terdataassinatura,
					u.usunome,
					em.valor,  dadosp.fim,
					pro.probanco, pro.proagencia, pro.nu_conta_corrente, pro.pronumeroprocesso, pro.muncod, pro.estuf, m.estuf, m.mundescricao
				ORDER BY
					tc.terid) as foo 
				WHERE 1 = 1 {$w} {$wPAC}
				)
		";
// ver($sql);
if ($_REQUEST['exportarexcel'] == 'sim') {
    ob_clean();
    $cabecalho = array("Ano do Processo", "UF", "Município", "Nº Processo", "Nº do Termo", "Valor do Termo", "Valor Empenhado", "Valor RAF", "Valor Complemento", "Pagamento Solicitado", "Pagamento Efetivado", "Saldo Bancário", "Vigência", "Tipo do Termo", "Habilitação", "Acompanhamento", "Termo Validado", "Status da Execução", "Pendência de Obras", "Restrição", "Termos de Referência", "Reformulação subação solicitada", "Reformulação prazo solicitada");
    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=SIMEC_RelatConsolidado" . date("Ymdhis") . ".xls");
    header("Content-Disposition: attachment; filename=SIMEC_RelatConsolidado" . date("Ymdhis") . ".xls");
    header("Content-Description: MID Gera excel");
    $db->monta_lista_tabulado($sql, $cabecalho, 1000000, 5, 'N', '100%', $par2);
    exit;
} else {
    // Só monta lista quando clicar em pesquisar
    if (isset($_REQUEST['estuf'])) {
        $cabecalho = array("Ação", "Ano do Processo", "UF", "Município", "Nº Processo", "Nº do Termo", "Valor do Termo", "Valor Empenhado", "Valor RAF", "Valor Complemento", "Pagamento Solicitado", "Pagamento Efetivado", "Saldo Bancário", "Vigência", "Tipo do Termo", "Habilitação", "Acompanhamento", "Termo Validado", "Status da Execução", "Pendência de Obras", "Restrição", "Termos de Referência", "Reformulação subação solicitada", "Reformulação prazo solicitada");
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', '', 'N', false);
    }
}
?>
</table>
<div id="divDebug"></div>