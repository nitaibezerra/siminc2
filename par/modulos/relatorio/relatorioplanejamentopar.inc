<?php
ini_set("memory_limit", "2048M");
set_time_limit(0);

if( $_POST['relat_planejamento'] ){
	switch ($_POST['relat_planejamento']) {
		case 'par':
			relatorioPlanejamentoPar();
		break;
		case 'saldo_bancario_par':
			relatorioPlanejamentoParSaldoBancario();
		break;
		case 'traduzido':
			relatorioPlanejamentoTraduzido();
		break;
		case 'pac':
			relatorioPlanejamentoPac();
		break;
		case 'saldo_bancario':
			relatorioPlanejamentoPacSaldoBancario();
		break;
		case 'pac_aprovadas':
			relatorioPlanejamentoPacAprovadas();
		break;
		case 'tablet':
			relatorioPlanejamentoTablet();
		break;
		case 'projetor':
			relatorioPlanejamentoProjetor();
		break;
		case 'onibus':
			relatorioPlanejamentoOnibus();
		break;
		case 'mobiliario':
			relatorioPlanejamentoMobiliario();
		break;
		case 'ar_condicionado':
			relatorioPlanejamentoArCondicionado();
		break;
		case 'financeira_par':
			relatorioExecucaoFinanceiraPAR();
		break;
		case 'planejamento_pr':
			relatorioPlanejamentoPR();
		break;
	}
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}
monta_titulo( 'Relatório Planejamento', 'Clique no botão XLS para gerar o relatório' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form name="formulario" id="formulario" method="post">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td>
		<table border="0" width="40%" cellspacing="1" cellpadding="2" align="center">
			<tr>
				<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_par" value="par">
					<b>Relatório de planejamento PAR</b>
					</label>
				</td>
				
				<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_pac" value="pac">
					<b>Relatório de planejamento PAC</b>
					</label>
				</td>
			</tr>
			<tr>
                <td width="20%">
                    <label style="cursor: pointer;">
                        <input type="radio" name="relat_planejamento" id="relat_planejamento_onibus" value="onibus">
                        <b>Ônibus</b>
                    </label>
                </td>
				<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_projetor" value="projetor">
					<b>Projetor</b>
					</label>
				</td>
			</tr>
			<tr>
                <td width="20%">
                    <label style="cursor: pointer;">
                        <input type="radio" name="relat_planejamento" id="relat_planejamento_ar_condicionado" value="ar_condicionado">
                        <b>Ar condicionado</b>
                    </label>
                </td>
				<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_mobiliario" value="mobiliario">
					<b>Mobiliário</b>
					</label>
				</td>
			</tr>
			<tr>
				<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_execucao_financeira_par" value="financeira_par">
					<b>Execução Financeira PAR</b>
					</label>
				</td>
				<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_traduzido" value="traduzido">
					<b>Relatório de planejamento traduzido</b>
					</label>
				</td>
			</tr>
			<tr>
                <td width="20%">
                    <label style="cursor: pointer;">
                        <input type="radio" name="relat_planejamento" id="relat_planejamento_tablet" value="tablet">
                        <b>Tablets</b>
                    </label>
                </td>
                
                <td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_pac_saldo" value="saldo_bancario">
					<b>Relatório de Planejamento PAC - Saldo Bancário</b>
					</label>
				</td>

				<!--<td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="relat_planejamento_pac" value="pac_aprovadas">
					<b>Relatório de planejamento PAC - Aprovadas</b>
					</label>
				</td>
			-->
			</tr>
			<tr>                
                <td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="saldo_bancario_par" value="saldo_bancario_par">
					<b>Relatório de Planejamento PAR - Saldo Bancário</b>
					</label>
				</td>
                <td width="20%">
					<label style="cursor: pointer;">
					<input type="radio" name="relat_planejamento" id="planejamento_pr" value="planejamento_pr">
					<b>Relatório Planejamento - PR</b>
					</label>
				</td>
			</tr>
			</table>
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
function gerarExcel(){
	var boChecked = false;
	$('input=[name="relat_planejamento"]').each(function (){
		if($(this).attr('checked') ){
			boChecked = true;
		}
	});
	if(boChecked){
		$('#formulario').submit();
	} else {
		alert('Selecione um item para gerar o excel.');
		return false;
	}
}
</script>
<?

function relatorioPlanejamentoPR() {
	global $db;

	$sql = "SELECT DISTINCT    mun.muncod as cod_ibge,
                           mun.mundescricao as nome_municipio,
                           mun.estuf as uf,
                           pre.preid as preid,
                           o.obrid as obrid,
                           o.obrnome as nome_creche,
                           ende.endlog || ', ' || ende.endcom || ', ' || ende.endbai || ', ' || ende.endcep as endereco_obra,
                   ende.medlatitude || ' / ' || ende.medlongitude as coordenada_geografica,
                   case when o.preid is not null and pre.tooid = 1 then pre.preanometa end as ano_aprovacao,
                   prf.prfdesc as vinculo_recurso,
                   tpo.tpodsc AS tipologia,
                                  esd_obra.esddsc as situacao_obra,
                                  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao,
                                  o.obrvalorprevisto as valor_obra_aprov,
                                  pre.prevalorobra as valor_obra_repas
            FROM obras2.obras                                                          o
            LEFT JOIN obras2.EMPREENDIMENTO      EMP       ON EMP.EMPID = O.EMPID
            LEFT JOIN obras2.programafonte                                               PRF        ON PRF.PRFID = EMP.PRFID
                    LEFT JOIN obras.preobra                         PRE        ON         pre.preid = o.preid
                    LEFT JOIN obras.tipologiaobra              TPO       ON         tpo.tpoid = o.tpoid         
            LEFT JOIN workflow.documento                                 doc_obra ON doc_obra.docid  = o.docid
            LEFT JOIN workflow.estadodocumento   esd_obra ON esd_obra.esdid = doc_obra.esdid
            LEFT JOIN ENTIDADE.ENDERECO                 ENDE     ON         ENDE.ENDID = O.ENDID
            LEFT JOIN territorios.municipio                    mun      ON mun.muncod = ENDE.muncod
            LEFT JOIN territorios.muntipomunicipio mtm        ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf
            LEFT JOIN territorios.tipomunicipio            tmn       ON tmn.tpmid = mtm.tpmid

            WHERE 
                    o.obrstatus = 'A' AND o.obridpai IS NULL and
                    o.tpoid in (20,19,21,16,9,10,104,105,17,22,18,108,109,110,111)
                    and emp.prfid in (41,50,55)
          
               
            ORDER BY
                    uf,nome_municipio
		";

	$registros = $db->carregar($sql);

	if($registros[0]) {
		
		$arCabecalho = array('Cod.IBGE','Município','UF','Preid','Obrid','Nome da creche','Endereço obra','Coordenada Geográfica','Ano aprovação','Vínculo recurso',
		'Tipologia','Situação obra','% execução','Valor obra aprovada','Valor obra repassada');

		foreach($registros as $reg) {
				
			$arDados[] = array('cod_ibge'=>$reg['cod_ibge'],
					'nome_municipio'=>$reg['nome_municipio'],
					'uf'=>$reg['uf'],
					'preid'=>$reg['preid'],
					'obrid'=>$reg['obrid'],
					'nome_creche'=>$reg['nome_creche'],
					'endereco_obra'=>$reg['endereco_obra'],
					'coordenada_geografica'=>$reg['coordenada_geografica'],
					'ano_aprovacao'=>$reg['ano_aprovacao'],
					'vinculo_recurso'=>$reg['vinculo_recurso'],
					'tipologia'=>$reg['tipologia'],
					'situacao_obra'=>$reg['situacao_obra'],
					'percentual_execucao'=>$reg['percentual_execucao'],
					'valor_obra_aprov'=>str_replace(".",",",round($reg['valor_obra_aprov'],2)),
					'valor_obra_repas'=>str_replace(".",",",round($reg['valor_obra_repas'],2))
			);
		}
	}

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Exec".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Exec".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioExecucaoFinanceiraPAR() {
	global $db;
	
	$sql = "SELECT 
				s.uf, s.mund as municipio, s.munc as ibge, SUM(s.empenho) empenho, SUM(s.pagamento) pagamento, s.tipo esfera
			FROM 
			(
				-- Subações PAR
				SELECT 
					CASE WHEN inu.itrid = 1 THEN inu.estuf ELSE inu.mun_estuf END UF , 
					mun.mundescricao as mund, mun.muncod as munc,
					--sum(vve.vrlempenhocancelado) empenho, 
					SUM(vve.saldo) as empenho,
					SUM(pag.valorpag) as pagamento, 
					CASE WHEN inu.itrid = 1  THEN 'Estadual' ELSE 'Municipal' END as tipo 
				FROM
					par.empenho emp 
				INNER JOIN par.processopar 			rp 		ON prp.prpnumeroprocesso = emp.empnumeroprocesso AND empstatus = 'A'
				INNER JOIN par.v_dadosempenhos 		vve 	ON vve.empid = emp.empid
				--inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
				INNER JOIN par.instrumentounidade 	inu 	ON inu.inuid = prp.inuid 
				/*left  join (
					select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
					from par.empenho
					where empcodigoespecie in ('02') and empstatus = 'A'
					group by 
						empnumeroprocesso,
						empcodigoespecie,
						empidpai) as ref on ref.empidpai = emp.empid*/
				LEFT JOIN territorios.municipio 	mun 	ON mun.muncod = inu.muncod
				LEFT JOIN (
					SELECT empid empipag, SUM(pagvalorparcela) as valorpag 
					FROM par.pagamento WHERE pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) 				pag 	ON pag.empipag = EMP.EMPID 
				WHERE empsituacao <> 'CANCELADO' 
				GROUP BY uf, mun.mundescricao, mun.muncod, tipo
			
				UNION
			
				-- Obras PAR
				SELECT 
					CASE WHEN inu.itrid = 1  THEN inu.estuf ELSE inu.mun_estuf END as uf, mun.mundescricao as mund, mun.muncod as munc,
					--sum(vve.vrlempenhocancelado) empenho, 
					SUM(vve.saldo) as empenho, SUM(pag.valorpag) as pagamento, CASE WHEN inu.itrid = 1  THEN 'Estadual' ELSE 'Municipal' END as tipo 
				FROM
					par.empenho emp 
				INNER JOIN par.processoobraspar 	prp 	ON prp.pronumeroprocesso = emp.empnumeroprocesso AND prp.prostatus = 'A' AND empstatus = 'A'
				INNER JOIN par.v_dadosempenhos 		vve 	ON vve.empid = emp.empid
				--inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
				INNER JOIN par.instrumentounidade 	inu 	ON inu.inuid = prp.inuid 
				/*left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
											from par.empenho
											where empcodigoespecie in ('02') and empstatus = 'A'
											group by 
												empnumeroprocesso,
												empcodigoespecie,
												empidpai) as ref on ref.empidpai = emp.empid*/
				LEFT  JOIN territorios.municipio 	mun 	ON mun.muncod = inu.muncod
				LEFT  JOIN (
					SELECT empid empipag, SUM(pagvalorparcela) as valorpag 
					FROM par.pagamento 
					WHERE pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) pag ON pag.empipag = EMP.EMPID 
				WHERE empsituacao <> 'CANCELADO' 
				GROUP BY uf, mun.mundescricao, mun.muncod, tipo
			
				UNION
			
				-- PAC
				SELECT 
					CASE WHEN prp.muncod IS NULL THEN prp.estuf ELSE mun.estuf END as uf,  mun.mundescricao as mund, prp.muncod as munc,
					--sum(vve.vrlempenhocancelado) empenho, 
					SUM(vve.saldo) as empenho, SUM(pag.valorpag) as pagamento, CASE WHEN prp.muncod IS NULL THEN 'Estadual' ELSE 'Municipal' END as tipo 
				FROM
					par.empenho emp 
				INNER JOIN par.processoobra 		prp 	ON prp.pronumeroprocesso = emp.empnumeroprocesso AND prp.prostatus = 'A' AND empstatus = 'A'
				INNER JOIN par.v_dadosempenhos 		vve 	ON vve.empid = emp.empid
				--inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
				/*left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
											from par.empenho
											where empcodigoespecie in ('02') and empstatus = 'A'
											group by 
												empnumeroprocesso,
												empcodigoespecie,
												empidpai) as ref on ref.empidpai = emp.empid*/
				LEFT  JOIN territorios.municipio 	mun 	ON mun.muncod = prp.muncod
				LEFT  JOIN (
					SELECT empid empipag, SUM(pagvalorparcela) as valorpag 
					FROM par.pagamento 
					WHERE pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) pag ON pag.empipag = emp.empid 
				WHERE empsituacao <> 'CANCELADO' AND prp.estuf = 'AC' AND prp.muncod IS NULL
				GROUP BY uf, mun.mundescricao, prp.muncod, tipo
				ORDER BY tipo,uf,mund, munc
			
			) s
			GROUP BY s.uf, s.mund, s.munc,s.tipo
			ORDER BY s.tipo,s.uf, s.mund, s.munc";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {
		
		$arCabecalho = array('UF','Município','IBGE','Empenho','Pagamento','Esfera');
	
		foreach($registros as $reg) {
			
			$arDados[] = array('uf'=>$reg['uf'],
							   'municipio'=>$reg['municipio'],
							   'ibge'=>$reg['ibge'],
							   'empenho'=>str_replace(".",",",round($reg['empenho'],2)),
							   'pagamento'=>str_replace(".",",",round($reg['pagamento'],2)),
							   'esfera'=>$reg['esfera']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Exec".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Exec".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoPar(){
	global $db;
	
	$sql = "SELECT DISTINCT   
                    pto.ptodescricao as tipo_obra,
                    pre.preid as n_protocolo_simec,
                    pre.estuf as uf,
                    o.obrid as obrid,
                    mun.mundescricao as municipio_beneficiado,
                    mun.muncod as cod_ibge,
                    ' ' as grupo_pac,
                    pto.ptodescricao as tipo_projeto,
                    pre.prevalorobra::numeric(20,2) as valor_obra,
                    esd.esddsc as situacao_analise_fnde,
                    CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
                    pre.predescricao as nome_obra,
                    pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
                    pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
                    ter.data_termo,
                    sum(emo.saldo) as valor_empenho_solicitado,
                    (SELECT count(*) FROM par.pagamento pp2
                    INNER JOIN par.empenho 		ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    INNER JOIN par.empenhoobra 	emo2 ON emo2.empid = ep3.empid AND eobstatus = 'A'
                    WHERE pp2.pagstatus='A' AND emo2.preid=pre.preid) as numero_parcelas,
                    (SELECT TO_CHAR(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') FROM par.pagamento pp2
                    INNER JOIN par.empenho ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    WHERE pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1) as data_pagamento_siafi_1_parcela,
                    (SELECT SUM(ppo.popvalorpagamento) FROM par.pagamentoobrapar ppo
                    INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
                    WHERE ppo.preid = pre.preid )  as valor_pagamento_solicitado ,
                    res.resdescricao,
                    CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
                    esd_obra.esddsc as situacao_obra,
                    coalesce(o.obrpercentultvistoria,0) as percentual_execucao,
                    (SELECT emp.empanooriginal 
			FROM par.v_saldo_obra_por_empenho vsoe
			INNER JOIN par.empenho emp on emp.empid = vsoe.empid AND emp.empstatus = 'A'
			WHERE vsoe.preid = pre.preid AND vsoe.saldo > 0 ORDER BY emp.empid ASC LIMIT 1
                    ) as ano_empenho,
                    to_char(o.obrdtinicio,'dd/mm/YYYY') as data_de_inicio_da_obra,
                    to_char(o.obrdtfim,'dd/mm/YYYY') as data_de_termino_da_obra,
                    (SELECT DISTINCT
                        ts.tpsdescricao
			FROM par.tiposubacao ts 
			INNER JOIN par.propostatiposubacao pts ON ts.tpsid = pts.tpsid
			INNER JOIN par.subacao s ON pts.ptsid = s.ptsid 
			WHERE s.sbaid = so.sbaid
			LIMIT 1) as tipo_subacao,
			s.sbadsc subacao
                FROM obras.preobra pre
                INNER JOIN par.subacaoobra 					so  ON so.preid = pre.preid
                inner join par.subacao 						s   ON s.sbaid = so.sbaid
                INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid 
                LEFT  JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid 
                LEFT  JOIN obras2.obras 					o   ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
                LEFT  JOIN workflow.documento 				doc_obra ON doc_obra.docid  = o.docid
                LEFT  JOIN workflow.estadodocumento 		esd_obra ON esd_obra.esdid = doc_obra.esdid 
                LEFT  JOIN territorios.municipio 			mun ON mun.muncod = pre.muncod 
                LEFT  JOIN workflow.estadodocumento 		esd ON doc.esdid = esd.esdid 
                LEFT  JOIN par.v_dadosfinanceiroporobra 	dfo ON dfo.preid = pre.preid 
                LEFT  JOIN par.vm_saldo_empenho_por_obra 	emo on emo.preid = pre.preid
                LEFT  JOIN par.processoobraspar 			pro ON pro.pronumeroprocesso = emo.processo AND pro.prostatus = 'A' 
                LEFT  JOIN par.resolucao 					res ON res.resid = pro.resid               
                LEFT  JOIN(  SELECT TO_CHAR(MAX(dp.dopdatainclusao),'dd/mm/YYYY') data_termo, tc.preid
                            From par.vm_documentopar_ativos dp
                            INNER JOIN par.termocomposicao tc on tc.dopid = dp.dopid
                            INNER JOIN par.documentoparvalidacao dpv on dpv.dopid = dp.dopid
                            WHERE dpv.dpvstatus = 'A'
                            GROUP BY tc.preid
                ) as ter on ter.preid = pre.preid
                WHERE  doc.esdid IN('624','625','337','626','627') AND pre.docid IS NOT NULL AND pre.prestatus = 'A' AND pre.tooid <> 1	
                GROUP BY  
                    pto.ptoclassificacaoobra,
                    pre.preid,
                    PRE.ESTUF,
                    o.obrid,
                    mun.mundescricao, mun.muncod,
                    pto.ptodescricao,
                    pre.prevalorobra,
                    esd.esddsc,
                    pre.predescricao,
                    endereco_obra,
                    coordenada_geografica,
                    data_termo,
                    numero_parcelas,
                    data_pagamento_siafi_1_parcela,
                    valor_pagamento_solicitado ,
                    res.resdescricao,
                    pre.preesfera,
                    situacao_obra,
                    percentual_execucao,
                    o.obrdtinicio,
                    o.obrdtfim,
                    dfo.saldo,
                    so.sbaid,
                    s.sbadsc
                ORDER BY 
                      pre.preid
";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {
            $arCabecalho = array("tipo_obra",
                "n_protocolo_simec",
                "uf",
                "obrid",
                "municipio_beneficiado",
                "cod_ibge",
                "tipo_projeto",
                "valor_obra",
                "situacao_analise_fnde",
                "nome_obra",
                "endereco_obra",
                "coordenada_geografica",
                "data_termo",
                "resdescricao",
                "valor_empenho_solicitado",
                "valor_pagamento_solicitado",
                "numero_parcelas",
                "data_pagamento_siafi_1_parcela",
                "preesfera",
                "situacao_obra",
                "percentual_execucao",
                "ano_empenho",
                "data_de_inicio_da_obra",
                "data_de_termino_da_obra",
                "tipo_subacao",
				"subacao"
            );
	
            foreach($registros as $reg) {
                $arDados[] = array(
                    'tipo_obra'=>$reg['tipo_obra'],
                    'n_protocolo_simec'=>$reg['n_protocolo_simec'],
                    'uf'=>$reg['uf'],
                    'obrid'=>$reg['obrid'],
                    'municipio_beneficiado'=>$reg['municipio_beneficiado'],
                    'cod_ibge'=>$reg['cod_ibge'],
                    'tipo_projeto'=>$reg['tipo_projeto'],
                    'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
                    'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
                    'nome_obra'=>$reg['nome_obra'],
                    'endereco_obra'=>$reg['endereco_obra'],
                    'coordenada_geografica'=>$reg['coordenada_geografica'],
                    'data_termo'=>$reg['data_termo'],
                    'resdescricao'=>$reg['resdescricao'],
                    'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
                    'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
                    'numero_parcelas'=>$reg['numero_parcelas'],
                    'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
                    'preesfera'=>$reg['preesfera'],
                    'situacao_obra'=>$reg['situacao_obra'],
                    'percentual_execucao'=>$reg['percentual_execucao'],
                    'ano_empenho'=>$reg['ano_empenho'],
                    'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
                    'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra'],
                    'tipo_subacao'=>$reg['tipo_subacao'],
					'subacao'=>$reg['subacao']
                );
            }
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatPlanejamentoPAR".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatPlanejamentoPAR".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoParSaldoBancario(){
	global $db;
	
	$sql = "SELECT DISTINCT   
                    pto.ptodescricao as tipo_obra,
                    pre.preid as n_protocolo_simec,
                    pre.estuf as uf,
                    o.obrid as obrid,
                    mun.mundescricao as municipio_beneficiado,
                    mun.muncod as cod_ibge,
                    ' ' as grupo_pac,
                    pto.ptodescricao as tipo_projeto,
                    pre.prevalorobra::numeric(20,2) as valor_obra,
                    esd.esddsc as situacao_analise_fnde,
                    CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
                    pre.predescricao as nome_obra,
                    pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
                    pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
                    ter.data_termo,
                    sum(emo.saldo) as valor_empenho_solicitado,
                    (SELECT count(*) FROM par.pagamento pp2
                    INNER JOIN par.empenho 		ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    INNER JOIN par.empenhoobra 	emo2 ON emo2.empid = ep3.empid AND eobstatus = 'A'
                    WHERE pp2.pagstatus='A' AND emo2.preid=pre.preid) as numero_parcelas,
                    (SELECT TO_CHAR(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') FROM par.pagamento pp2
                    INNER JOIN par.empenho ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    WHERE pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1) as data_pagamento_siafi_1_parcela,
                    (SELECT SUM(ppo.popvalorpagamento) FROM par.pagamentoobrapar ppo
                    INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
                    WHERE ppo.preid = pre.preid )  as valor_pagamento_solicitado ,
                    res.resdescricao,
                    CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
                    esd_obra.esddsc as situacao_obra,
                    coalesce(o.obrpercentultvistoria,0) as percentual_execucao,
                    (SELECT emp.empanooriginal 
					FROM par.v_saldo_obra_por_empenho vsoe
					INNER JOIN par.empenho emp on emp.empid = vsoe.empid AND emp.empstatus = 'A'
					WHERE vsoe.preid = pre.preid AND vsoe.saldo > 0 ORDER BY emp.empid ASC LIMIT 1
		                    ) as ano_empenho,
		                    to_char(o.obrdtinicio,'dd/mm/YYYY') as data_de_inicio_da_obra,
		                    to_char(o.obrdtfim,'dd/mm/YYYY') as data_de_termino_da_obra,
		                    (SELECT DISTINCT
		                        ts.tpsdescricao
					FROM par.tiposubacao ts 
					INNER JOIN par.propostatiposubacao pts ON ts.tpsid = pts.tpsid
					INNER JOIN par.subacao s ON pts.ptsid = s.ptsid 
					WHERE s.sbaid = so.sbaid
					LIMIT 1) as tipo_subacao,
					s.sbadsc subacao,
                	pro.pronumeroprocesso as pronumeroprocesso,
                	par.retornasaldoprocesso(pro.pronumeroprocesso) as saldobancario
                FROM obras.preobra pre
                INNER JOIN par.subacaoobra 					so ON so.preid = pre.preid
                inner join par.subacao 						s on s.sbaid = so.sbaid
                INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid 
                LEFT  JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid 
                LEFT  JOIN obras2.obras 					o ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
                LEFT  JOIN workflow.documento 				doc_obra ON doc_obra.docid  = o.docid
                LEFT  JOIN workflow.estadodocumento 		esd_obra ON esd_obra.esdid = doc_obra.esdid 
                LEFT  JOIN territorios.municipio 			mun ON mun.muncod = pre.muncod 
                LEFT  JOIN workflow.estadodocumento 		esd ON doc.esdid = esd.esdid 
                LEFT  JOIN par.v_dadosfinanceiroporobra 	dfo ON dfo.preid = pre.preid 
                LEFT  JOIN par.vm_saldo_empenho_por_obra 	emo on emo.preid = pre.preid
                LEFT  JOIN par.processoobraspar 			pro ON pro.pronumeroprocesso = emo.processo AND pro.prostatus = 'A' 
                LEFT  JOIN par.resolucao 					res ON res.resid = pro.resid               
                LEFT  JOIN(  SELECT TO_CHAR(MAX(dp.dopdatainclusao),'dd/mm/YYYY') data_termo, tc.preid
                            From par.vm_documentopar_ativos dp
                            INNER JOIN par.termocomposicao tc on tc.dopid = dp.dopid
                            INNER JOIN par.documentoparvalidacao dpv on dpv.dopid = dp.dopid
                            WHERE dpv.dpvstatus = 'A'
                            GROUP BY tc.preid
                ) as ter on ter.preid = pre.preid
                WHERE  doc.esdid IN('624','625','337','626','627') AND pre.docid IS NOT NULL AND pre.prestatus = 'A' AND pre.tooid <> 1	
                GROUP BY  
                    pto.ptoclassificacaoobra,
                    pre.preid,
                    PRE.ESTUF,
                    o.obrid,
                    mun.mundescricao, mun.muncod,
                    pto.ptodescricao,
                    pre.prevalorobra,
                    esd.esddsc,
                    pre.predescricao,
                    endereco_obra,
                    coordenada_geografica,
                    data_termo,
                    numero_parcelas,
                    data_pagamento_siafi_1_parcela,
                    valor_pagamento_solicitado ,
                    res.resdescricao,
                    pre.preesfera,
                    situacao_obra,
                    percentual_execucao,
                    o.obrdtinicio,
                    o.obrdtfim,
                    dfo.saldo,
                    so.sbaid,
                    s.sbadsc,
                	pro.pronumeroprocesso
                ORDER BY 
                      pre.preid";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {
            $arCabecalho = array("tipo_obra",
                "n_protocolo_simec",
                "uf",
                "obrid",
                "municipio_beneficiado",
                "cod_ibge",
                "tipo_projeto",
                "valor_obra",
                "situacao_analise_fnde",
                "nome_obra",
                "endereco_obra",
                "coordenada_geografica",
                "data_termo",
                "resdescricao",
                "valor_empenho_solicitado",
                "valor_pagamento_solicitado",
                "numero_parcelas",
                "data_pagamento_siafi_1_parcela",
                "preesfera",
                "situacao_obra",
                "percentual_execucao",
                "ano_empenho",
                "data_de_inicio_da_obra",
                "data_de_termino_da_obra",
                "tipo_subacao",
				"subacao",
				"processo",
				"saldobancario"
            );
	
            foreach($registros as $reg) {
                $arDados[] = array(
                    'tipo_obra'=>$reg['tipo_obra'],
                    'n_protocolo_simec'=>$reg['n_protocolo_simec'],
                    'uf'=>$reg['uf'],
                    'obrid'=>$reg['obrid'],
                    'municipio_beneficiado'=>$reg['municipio_beneficiado'],
                    'cod_ibge'=>$reg['cod_ibge'],
                    'tipo_projeto'=>$reg['tipo_projeto'],
                    'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
                    'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
                    'nome_obra'=>$reg['nome_obra'],
                    'endereco_obra'=>$reg['endereco_obra'],
                    'coordenada_geografica'=>$reg['coordenada_geografica'],
                    'data_termo'=>$reg['data_termo'],
                    'resdescricao'=>$reg['resdescricao'],
                    'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
                    'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
                    'numero_parcelas'=>$reg['numero_parcelas'],
                    'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
                    'preesfera'=>$reg['preesfera'],
                    'situacao_obra'=>$reg['situacao_obra'],
                    'percentual_execucao'=>$reg['percentual_execucao'],
                    'ano_empenho'=>$reg['ano_empenho'],
                    'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
                    'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra'],
                    'tipo_subacao'=>$reg['tipo_subacao'],
					'subacao'=>$reg['subacao'],
					'processo'=> formata_Processo($reg['pronumeroprocesso']),
					'saldobancario'=> number_format($reg['saldobancario'],2,',','.')
                );
            }
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatPlanejamentoPAR".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatPlanejamentoPAR".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoPacSaldoBancario(){
	global $db;
	
	$sql = "SELECT DISTINCT 
                    CASE WHEN ptoclassificacaoobra='P' THEN 'Proinfancia' WHEN ptoclassificacaoobra='Q' THEN 'Quadra' WHEN ptoclassificacaoobra='C' THEN 'Cobertura' END as tipo_obra,
                    pre.preid as n_protocolo_simec,
                    pre.estuf as uf,
                    o.obrid as obrid,
                    mun.mundescricao as municipio_beneficiado,
                    mun.muncod as cod_ibge,
                    tmn.tpmdsc as grupo_pac,
                    pto.ptodescricao as tipo_projeto,
                    pre.prevalorobra as valor_obra,
                    esd.esddsc as situacao_analise_fnde,
                    CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
                    pre.predescricao as nome_obra,
                    pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
                    pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
                    ter.data_termo,
                    --SUM(emo.eobvalorempenho - coalesce(vrlcancelado,0)) as valor_empenho_solicitado,
                    sum(emo.saldo) as valor_empenho_solicitado,
                    (SELECT count(*) FROM par.pagamento pp2
                    INNER JOIN par.empenho 		ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    INNER JOIN par.empenhoobra 	emo2 ON emo2.empid = ep3.empid AND eobstatus = 'A'
                    WHERE pp2.pagstatus='A' AND emo2.preid=pre.preid) as numero_parcelas,
                    (SELECT TO_CHAR(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') FROM par.pagamento pp2
                    INNER JOIN par.empenho ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    WHERE pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1) as data_pagamento_siafi_1_parcela,
                    (SELECT SUM(ppo.pobvalorpagamento) FROM par.pagamentoobra ppo
                    INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
                    WHERE ppo.preid = pre.preid )  as valor_pagamento_solicitado ,
                    res.resdescricao,
                    CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
                    esd_obra.esddsc as situacao_obra,
                    ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao,
                    pre.preanometa as ano_meta,
                    case when o.obrdtinicio is not null or  trim(to_char(o.obrdtinicio,'dd/mm/YYYY')) = ''
                    then to_char(o.obrdtinicio,'dd/mm/YYYY') 
                    else 
                    case when (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao  WHERE obrid = o.obrid  ) is NOT null   
                    then (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao  WHERE obrid = o.obrid  )
                    ELSE
                    (select  to_char(obrdtinicio,'dd/mm/YYYY')  from obras2.obras where obrid = o.obridvinculado )
                    END
                    end as data_de_inicio_da_obra,
                    dataob.dataobra as data_de_termino_da_obra,
                    pre.entcodent,
                    pre.preanoselecao,
                	pro.pronumeroprocesso as pronumeroprocesso,
                	(par.retornasaldoprocesso(pro.pronumeroprocesso)) as saldobancario
            FROM obras.preobra pre
            INNER JOIN workflow.documento 			doc 	ON doc.docid = pre.docid
            LEFT  JOIN obras.pretipoobra 			pto 	ON pto.ptoid = pre.ptoid
            LEFT  JOIN obras2.obras 				o 		ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
            LEFT  JOIN (
                    SELECT 
                            oi.obrid AS obrid,
                            (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A' AND staid = 3) AS dataobra
                    FROM obras2.obras oi
                    INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid
                    WHERE oi.obrstatus = 'A'
                    AND doc2.esdid = 693) 				dataob 	ON dataob.obrid = o.obrid
            LEFT  JOIN workflow.documento 			doc_obra ON doc_obra.docid  = o.docid
            LEFT  JOIN workflow.estadodocumento 	esd_obra ON esd_obra.esdid = doc_obra.esdid
            LEFT  JOIN territorios.municipio 		mun 	ON mun.muncod = pre.muncod
            LEFT  JOIN territorios.muntipomunicipio mtm 	ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf
            LEFT  JOIN territorios.tipomunicipio 	tmn 	ON tmn.tpmid = mtm.tpmid
            LEFT  JOIN workflow.estadodocumento 	esd   	ON doc.esdid = esd.esdid
            --LEFT JOIN (SELECT preid,empid,SUM(eobpercentualemp) eobpercentualemp,SUM(eobvalorempenho) eobvalorempenho FROM par.empenhoobra where eobstatus = 'A' GROUP BY preid,empid) emo ON emo.preid = pre.preid
            --LEFT  JOIN par.v_dadosfinanceiroporobra	emo 	ON emo.preid = pre.preid 
            LEFT JOIN par.vm_saldo_empenho_por_obra emo on emo.preid = pre.preid
            LEFT  JOIN par.processoobra 			pro 	ON pro.pronumeroprocesso = emo.processo AND pro.prostatus = 'A'
            LEFT  JOIN par.resolucao 				res 	ON res.resid = pro.resid
            LEFT JOIN(
                    SELECT to_char(MAX(terdatainclusao),'dd/mm/YYYY') data_termo, teo.preid
                    FROM par.termocompromissopac ter
                    JOIN par.termoobra teo ON teo.terid = ter.terid
                    WHERE ter.terassinado = true -- and ter.terstatus='A'
                    GROUP BY teo.preid
                    ) as 								ter 	ON ter.preid = pre.preid
            WHERE 
                    tmn.tpmid IN(163,164,165)
                    AND pre.docid IS NOT NULL
                    AND pre.prestatus = 'A'
                    AND pre.tooid = 1
                    --AND doc.esdid in ('228','360','365','754','683') -- Aprovadas
            GROUP BY
                    pto.ptoclassificacaoobra,
                    pre.preid,
                    pre.estuf,
                    o.obrid,
                    mun.mundescricao, mun.muncod,
                    o.obridvinculado,
                    tmn.tpmdsc,
                    pto.ptodescricao,
                    pre.prevalorobra,
                    esd.esddsc,
                    pre.predescricao,
                    endereco_obra,
                    coordenada_geografica,
                    data_termo,
                    numero_parcelas,
                    data_pagamento_siafi_1_parcela,
                    valor_pagamento_solicitado ,
                    res.resdescricao,
                    pre.preesfera,
                    situacao_obra,
                    percentual_execucao,
                    pre.preanometa,
                    o.obrdtinicio,
                    data_de_termino_da_obra,
                    pre.entcodent,
                    pre.preanoselecao,
                	pro.pronumeroprocesso
                  
            ORDER BY
                    pre.preid";
	$registros = $db->carregar($sql);
	
	if($registros[0]) {
            $arCabecalho = array(
                "tipo_obra",
                "n_protocolo_simec",
                "uf",
                "obrid",
                "municipio_beneficiado",
                "cod_ibge",
                "grupo_pac",
                "tipo_projeto",
                "valor_obra",
                "situacao_analise_fnde",
                "nome_obra",
                "endereco_obra",
                "coordenada_geografica",
                "data_termo",
                "resdescricao",
                "valor_empenho_solicitado",
                "valor_pagamento_solicitado",
                "numero_parcelas",
                "data_pagamento_siafi_1_parcela",
                "preesfera",
                "situacao_obra",
                "percentual_execucao",
                "ano_meta",
                "data_de_inicio_da_obra",
                "data_de_termino_da_obra",
                "codigo_inep",
                "ano_selecao",
                "processo",
                "saldo_bancario"
            );

            foreach($registros as $reg) {
                $arDados[] = array(
                    'tipo_obra'=>$reg['tipo_obra'],
                    'n_protocolo_simec'=>$reg['n_protocolo_simec'],
                    'uf'=>$reg['uf'],
                    'obrid'=>$reg['obrid'],
                    'municipio_beneficiado'=>$reg['municipio_beneficiado'],
                    'cod_ibge'=>$reg['cod_ibge'],
                    'grupo_pac'=>$reg['grupo_pac'],
                    'tipo_projeto'=>$reg['tipo_projeto'],
                    'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
                    'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
                    'nome_obra'=>$reg['nome_obra'],
                    'endereco_obra'=>$reg['endereco_obra'],
                    'coordenada_geografica'=>$reg['coordenada_geografica'],
                    'data_termo'=>$reg['data_termo'],
                    'resdescricao'=>$reg['resdescricao'],
                    'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
                    'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
                    'numero_parcelas'=>$reg['numero_parcelas'],
                    'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
                    'preesfera'=>$reg['preesfera'],
                    'situacao_obra'=>$reg['situacao_obra'],
                    'percentual_execucao'=>$reg['percentual_execucao'],
                    'ano_meta'=>$reg['ano_meta'],
                    'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
                    'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra'],
                    'codigo_inep'=>$reg['entcodent'],
                    'ano_selecao'=>$reg['preanoselecao'],
                    'processo'=> formata_Processo($reg['pronumeroprocesso']),
                    'saldo_bancario'=>number_format($reg['saldobancario'],2,',','.')
                );
            }
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PAC".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PAC".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoPac(){
	global $db;
	
	$sql = "SELECT DISTINCT     
		    CASE WHEN ptoclassificacaoobra is null then prf.prfdesc else
			CASE WHEN ptoclassificacaoobra='P' THEN 'PROINFÂNCIA' WHEN ptoclassificacaoobra='Q' THEN 'QUADRA' WHEN ptoclassificacaoobra='C' THEN 'COBERTURA' END 
		    end as tipo_obra,	
                    pre.preid as n_protocolo_simec,
                    pre.estuf as uf,
                    o.obrid as obrid,
                    mun.mundescricao as municipio_beneficiado,
                    mun.muncod as cod_ibge,
                    tmn.tpmdsc as grupo_pac,
                    pto.ptodescricao as tipo_projeto,
                    pre.prevalorobra as valor_obra,
                    esd.esddsc as situacao_analise_fnde,
                    CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
                    pre.predescricao as nome_obra,
                    pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
                    pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
                    ter.data_termo,
                    --SUM(emo.eobvalorempenho - coalesce(vrlcancelado,0)) as valor_empenho_solicitado,
                    sum(emo.saldo) as valor_empenho_solicitado,
                    (SELECT count(*) FROM par.pagamento pp2
                    INNER JOIN par.empenho 		ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    INNER JOIN par.empenhoobra 	emo2 ON emo2.empid = ep3.empid AND eobstatus = 'A'
                    WHERE pp2.pagstatus='A' AND emo2.preid=pre.preid) as numero_parcelas,
                    (SELECT TO_CHAR(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') FROM par.pagamento pp2
                    INNER JOIN par.empenho ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
                    WHERE pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1) as data_pagamento_siafi_1_parcela,
                    (SELECT SUM(ppo.pobvalorpagamento) FROM par.pagamentoobra ppo
                    INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
                    WHERE ppo.preid = pre.preid )  as valor_pagamento_solicitado ,
                    res.resdescricao,
                    CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
                    esd_obra.esddsc as situacao_obra,
                    ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao,
                    pre.preanometa as ano_meta,
                    case when o.obrdtinicio is not null or  trim(to_char(o.obrdtinicio,'dd/mm/YYYY')) = ''
                    then to_char(o.obrdtinicio,'dd/mm/YYYY') 
                    else 
                    case when (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao  WHERE obrid = o.obrid  ) is NOT null   
                    then (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao  WHERE obrid = o.obrid  )
                    ELSE
                    (select  to_char(obrdtinicio,'dd/mm/YYYY')  from obras2.obras where obrid = o.obridvinculado )
                    END
                    end as data_de_inicio_da_obra,
                    dataob.dataobra as data_de_termino_da_obra,
                    pre.entcodent,
                    pre.preanoselecao
            FROM obras.preobra pre
            INNER JOIN workflow.documento 			doc ON doc.docid = pre.docid
            LEFT  JOIN obras.pretipoobra 			pto ON pto.ptoid = pre.ptoid
            LEFT  JOIN obras2.obras 				o 	ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
            LEFT  JOIN obras2.empreendimento		e 	on e.empid = o.empid
            LEFT  JOIN obras2.programafonte			prf	on prf.prfid = e.prfid
            LEFT  JOIN (
                    SELECT 
                            oi.obrid AS obrid,
                            (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A' AND staid = 3) AS dataobra
                    FROM obras2.obras oi
                    INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid
                    WHERE oi.obrstatus = 'A'
                    AND doc2.esdid = 693) 				dataob 	ON dataob.obrid = o.obrid
            LEFT  JOIN workflow.documento 			doc_obra ON doc_obra.docid  = o.docid
            LEFT  JOIN workflow.estadodocumento 	esd_obra ON esd_obra.esdid = doc_obra.esdid
            LEFT  JOIN territorios.municipio 		mun 	ON mun.muncod = pre.muncod
            LEFT  JOIN territorios.muntipomunicipio mtm 	ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf
            LEFT  JOIN territorios.tipomunicipio 	tmn 	ON tmn.tpmid = mtm.tpmid
            LEFT  JOIN workflow.estadodocumento 	esd   	ON doc.esdid = esd.esdid
            --LEFT JOIN (SELECT preid,empid,SUM(eobpercentualemp) eobpercentualemp,SUM(eobvalorempenho) eobvalorempenho FROM par.empenhoobra where eobstatus = 'A' GROUP BY preid,empid) emo ON emo.preid = pre.preid
            --LEFT  JOIN par.v_dadosfinanceiroporobra	emo 	ON emo.preid = pre.preid 
            LEFT JOIN par.vm_saldo_empenho_por_obra emo on emo.preid = pre.preid
            LEFT  JOIN par.processoobra 			pro 	ON pro.pronumeroprocesso = emo.processo AND pro.prostatus = 'A'
            LEFT  JOIN par.resolucao 				res 	ON res.resid = pro.resid
            LEFT JOIN(
                    SELECT to_char(MAX(terdatainclusao),'dd/mm/YYYY') data_termo, teo.preid
                    FROM par.termocompromissopac ter
                    JOIN par.termoobra teo ON teo.terid = ter.terid
                    WHERE ter.terassinado = true -- and ter.terstatus='A'
                    GROUP BY teo.preid
                    ) as 								ter 	ON ter.preid = pre.preid
            WHERE 
                    tmn.tpmid IN(163,164,165)
                    AND pre.docid IS NOT NULL
                    AND pre.prestatus = 'A'
                   -- and pre.preid = 751
                    AND pre.tooid = 1
                    --AND doc.esdid in ('228','360','365','754','683') -- Aprovadas
            GROUP BY
                    pto.ptoclassificacaoobra,
                    prf.prfdesc,
                    pre.preid,
                    pre.estuf,
                    o.obrid,
                    mun.mundescricao, mun.muncod,
                    o.obridvinculado,
                    tmn.tpmdsc,
                    pto.ptodescricao,
                    pre.prevalorobra,
                    esd.esddsc,
                    pre.predescricao,
                    endereco_obra,
                    coordenada_geografica,
                    data_termo,
                    numero_parcelas,
                    data_pagamento_siafi_1_parcela,
                    valor_pagamento_solicitado ,
                    res.resdescricao,
                    pre.preesfera,
                    situacao_obra,
                    percentual_execucao,
                    pre.preanometa,
                    o.obrdtinicio,
                    data_de_termino_da_obra,
                    pre.entcodent,
                    pre.preanoselecao
                  
            ORDER BY
                    pre.preid";
	$registros = $db->carregar($sql);
	
	if($registros[0]) {
            $arCabecalho = array(
                "tipo_obra",
                "n_protocolo_simec",
                "uf",
                "obrid",
                "municipio_beneficiado",
                "cod_ibge",
                "grupo_pac",
                "tipo_projeto",
                "valor_obra",
                "situacao_analise_fnde",
                "nome_obra",
                "endereco_obra",
                "coordenada_geografica",
                "data_termo",
                "resdescricao",
                "valor_empenho_solicitado",
                "valor_pagamento_solicitado",
                "numero_parcelas",
                "data_pagamento_siafi_1_parcela",
                "preesfera",
                "situacao_obra",
                "percentual_execucao",
                "ano_meta",
                "data_de_inicio_da_obra",
                "data_de_termino_da_obra",
                "codigo_inep",
                "ano_selecao"
            );

            foreach($registros as $reg) {
                $arDados[] = array(
                    'tipo_obra'=>$reg['tipo_obra'],
                    'n_protocolo_simec'=>$reg['n_protocolo_simec'],
                    'uf'=>$reg['uf'],
                    'obrid'=>$reg['obrid'],
                    'municipio_beneficiado'=>$reg['municipio_beneficiado'],
                    'cod_ibge'=>$reg['cod_ibge'],
                    'grupo_pac'=>$reg['grupo_pac'],
                    'tipo_projeto'=>$reg['tipo_projeto'],
                    'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
                    'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
                    'nome_obra'=>$reg['nome_obra'],
                    'endereco_obra'=>$reg['endereco_obra'],
                    'coordenada_geografica'=>$reg['coordenada_geografica'],
                    'data_termo'=>$reg['data_termo'],
                    'resdescricao'=>$reg['resdescricao'],
                    'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
                    'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
                    'numero_parcelas'=>$reg['numero_parcelas'],
                    'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
                    'preesfera'=>$reg['preesfera'],
                    'situacao_obra'=>$reg['situacao_obra'],
                    'percentual_execucao'=>$reg['percentual_execucao'],
                    'ano_meta'=>$reg['ano_meta'],
                    'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
                    'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra'],
                    'codigo_inep'=>$reg['entcodent'],
                    'ano_selecao'=>$reg['preanoselecao']
                );
            }
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PAC".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PAC".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoPacAprovadas(){
	global $db;

	$sql = "SELECT DISTINCT 
				CASE WHEN ptoclassificacaoobra='P' THEN 'Proinfancia' WHEN ptoclassificacaoobra='Q' THEN 'Quadra' WHEN ptoclassificacaoobra='C' THEN 'Cobertura' END as tipo_obra,
				pre.preid as n_protocolo_simec,
				pre.estuf as uf,
				o.obrid as obrid,
				mun.mundescricao as municipio_beneficiado,
				mun.muncod as cod_ibge,
				tmn.tpmdsc as grupo_pac,
				pto.ptodescricao as tipo_projeto,
				pre.prevalorobra as valor_obra,
				esd.esddsc as situacao_analise_fnde,
				CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
				pre.predescricao as nome_obra,
				pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
				pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
				ter.data_termo,
				--SUM(emo.eobvalorempenho - coalesce(vrlcancelado,0)) as valor_empenho_solicitado,
				emo.saldo as valor_empenho_solicitado,
				(SELECT count(*) FROM par.pagamento pp2
				INNER JOIN par.empenho 		ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
				INNER JOIN par.empenhoobra 	emo2 ON emo2.empid = ep3.empid AND eobstatus = 'A'
				WHERE pp2.pagstatus='A' AND emo2.preid=pre.preid) as numero_parcelas,
				(SELECT TO_CHAR(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') FROM par.pagamento pp2
				INNER JOIN par.empenho ep3 ON ep3.empid = pp2.empid AND empstatus = 'A'
				WHERE pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1) as data_pagamento_siafi_1_parcela,
				(SELECT SUM(ppo.pobvalorpagamento) FROM par.pagamentoobra ppo
				INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
				WHERE ppo.preid = pre.preid )  as valor_pagamento_solicitado ,
				res.resdescricao,
				CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
				esd_obra.esddsc as situacao_obra,
				coalesce(o.obrpercentultvistoria,0) as percentual_execucao,
				pre.preanometa as ano_meta,
				to_char(o.obrdtinicio,'dd/mm/YYYY') as data_de_inicio_da_obra,
				dataob.dataobra as data_de_termino_da_obra,
				pre.entcodent,
				pre.preanoselecao
			FROM obras.preobra pre
			INNER JOIN workflow.documento 			doc 	ON doc.docid = pre.docid
			LEFT  JOIN obras.pretipoobra 			pto 	ON pto.ptoid = pre.ptoid
			LEFT  JOIN obras2.obras 				o 		ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
			LEFT  JOIN (
				SELECT 
					oi.obrid AS obrid,
					(SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A' AND staid = 3) AS dataobra
				FROM obras2.obras oi
				INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid
				WHERE oi.obrstatus = 'A'
				AND doc2.esdid = 693) 				dataob 	ON dataob.obrid = o.obrid
			LEFT  JOIN workflow.documento 			doc_obra ON doc_obra.docid  = o.docid
			LEFT  JOIN workflow.estadodocumento 	esd_obra ON esd_obra.esdid = doc_obra.esdid
			LEFT  JOIN territorios.municipio 		mun 	ON mun.muncod = pre.muncod
			LEFT  JOIN territorios.muntipomunicipio mtm 	ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf
			LEFT  JOIN territorios.tipomunicipio 	tmn 	ON tmn.tpmid = mtm.tpmid
			LEFT  JOIN workflow.estadodocumento 	esd   	ON doc.esdid = esd.esdid
			--LEFT JOIN (SELECT preid,empid,SUM(eobpercentualemp) eobpercentualemp,SUM(eobvalorempenho) eobvalorempenho FROM par.empenhoobra where eobstatus = 'A' GROUP BY preid,empid) emo ON emo.preid = pre.preid
			--LEFT  JOIN par.v_dadosfinanceiroporobra	emo 	ON emo.preid = pre.preid 
			LEFT JOIN par.vm_saldo_empenho_por_obra emo on emo.preid = pre.preid
			LEFT  JOIN par.processoobra 			pro 	ON pro.pronumeroprocesso = emo.processo AND pro.prostatus = 'A'
			LEFT  JOIN par.resolucao 				res 	ON res.resid = pro.resid
			LEFT JOIN(
				SELECT to_char(MAX(terdatainclusao),'dd/mm/YYYY') data_termo, teo.preid
				FROM par.termocompromissopac ter
				JOIN par.termoobra teo ON teo.terid = ter.terid
				WHERE ter.terassinado = true and ter.terstatus='A'
				GROUP BY teo.preid
				) as 								ter 	ON ter.preid = pre.preid
			WHERE 
				tmn.tpmid IN(163,164,165)
				AND pre.docid IS NOT NULL
				AND pre.prestatus = 'A'
				AND pre.tooid = 1
				AND doc.esdid in ('228','360','365','754','683') -- Aprovadas
			GROUP BY
				pto.ptoclassificacaoobra,
				pre.preid,
				pre.estuf,
				o.obrid,
				mun.mundescricao, mun.muncod,
				tmn.tpmdsc,
				pto.ptodescricao,
				pre.prevalorobra,
				esd.esddsc,
				pre.predescricao,
				endereco_obra,
				coordenada_geografica,
				data_termo,
				numero_parcelas,
				data_pagamento_siafi_1_parcela,
				valor_pagamento_solicitado ,
				res.resdescricao,
				pre.preesfera,
				situacao_obra,
				percentual_execucao,
				pre.preanometa,
				o.obrdtinicio,
				data_de_termino_da_obra,
				pre.entcodent,
				pre.preanoselecao,
				emo.saldo
			ORDER BY
				pre.preid
	";
//	ver(simec_htmlentities($sql), d);
	$registros = $db->carregar($sql);

	if($registros[0]) {

		$arCabecalho = array("tipo_obra",
							 "n_protocolo_simec",
							 "uf",
							 "obrid",
							 "municipio_beneficiado",
							 "cod_ibge",
							 "grupo_pac",
							 "tipo_projeto",
							 "valor_obra",
							 "situacao_analise_fnde",
							 "nome_obra",
							 "endereco_obra",
							 "coordenada_geografica",
							 "data_termo",
				  			 "resdescricao",
							 "valor_empenho_solicitado",
							 "valor_pagamento_solicitado",
							 "numero_parcelas",
							 "data_pagamento_siafi_1_parcela",
							 "preesfera",
							 "situacao_obra",
							 "percentual_execucao",
							 "ano_meta",
							 "data_de_inicio_da_obra",
							 "data_de_termino_da_obra",
							 "cod_inep",
							 "ano_selecao"
		);

		foreach($registros as $reg) {

			$arDados[] = array('tipo_obra'=>$reg['tipo_obra'],
							   'n_protocolo_simec'=>$reg['n_protocolo_simec'],
							   'uf'=>$reg['uf'],
							   'obrid'=>$reg['obrid'],
							   'municipio_beneficiado'=>$reg['municipio_beneficiado'],
							   'cod_ibge'=>$reg['cod_ibge'],
							   'grupo_pac'=>$reg['grupo_pac'],
							   'tipo_projeto'=>$reg['tipo_projeto'],
							   'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
							   'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
							   'nome_obra'=>$reg['nome_obra'],
							   'endereco_obra'=>$reg['endereco_obra'],
							   'coordenada_geografica'=>$reg['coordenada_geografica'],
							   'data_termo'=>$reg['data_termo'],
							   'resdescricao'=>$reg['resdescricao'],
							   'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
							   'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
							   'numero_parcelas'=>$reg['numero_parcelas'],
							   'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
							   'preesfera'=>$reg['preesfera'],
							   'situacao_obra'=>$reg['situacao_obra'],
							   'percentual_execucao'=>$reg['percentual_execucao'],
							   'ano_meta'=>$reg['ano_meta'],
							   'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
							   'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra'],
							   'cod_inep'=>$reg['entcodent'],
							   'ano_selecao'=>$reg['preanoselecao']


			);
		}
	}

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PAC_AP".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PAC_AP".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}


function relatorioPlanejamentoTraduzido(){
	global $db;

	$sql = "
SELECT      distinct CASE WHEN ptoclassificacaoobra is null then prf.prfdesc else
			CASE WHEN ptoclassificacaoobra='P' THEN 'PROINFÂNCIA' WHEN ptoclassificacaoobra='Q' THEN 'QUADRA' WHEN ptoclassificacaoobra='C' THEN 'COBERTURA' END 
		    end as tipo_obra,
                  pre.preid as n_protocolo_simec,
                  pre.estuf as uf,
                  o.obrid as obrid,
                  mun.mundescricao as municipio_beneficiado,
                  mun.muncod as cod_ibge,
                  tmn.tpmdsc as grupo_pac,
                  pto.ptodescricao as tipo_projeto,
                  case when esd.esdid not in (754,683,360,365,366,367)  then pre.prevalorobra else (select prevalorobra from obras.preobra where preidpai = pre.preid limit 1) end valor_obra,
                 -- pre.prevalorobra as valor_obra,
                  esd.esddsc as situacao_analise_fnde,
                  CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
                  pre.predescricao as nome_obra,
                  pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
                  pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
                  ter.data_termo,
                  sum(emo.saldo) as valor_empenho_solicitado,
                  (select count(pp2.pagid)  from par.pagamentoobra po
		  inner join par.pagamento pp2 on pp2.pagid = po.pagid
                  where pp2.pagstatus='A' and po.preid=pre.preid) as numero_parcelas,
                  min((select to_char(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') from par.pagamento pp2 
                  inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
                  where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1)) as data_pagamento_siafi_1_parcela,


		 max(case when   (select popdataprazoaprovado from obras.preobraprorrogacao where popid  = (select max(popid) from obras.preobraprorrogacao where preid = pre.preid group by preid)) is not null 
			then to_char((select popdataprazoaprovado from obras.preobraprorrogacao where popid  = (select max(popid) from obras.preobraprorrogacao where preid = pre.preid group by preid)),'dd/mm/YYYY')
			else
		  
				case when (select to_char(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') from par.pagamento pp2 
					inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
					where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1) is not null 
				then 
					to_char(((select to_char(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') from par.pagamento pp2 
					inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
					where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emo.processo AND pp2.pagparcela= 1)::date) + 720,'dd/mm/YYYY')
				end  
			end)  data_prevista_termino,

		  
		  (select sum(ppo.pobvalorpagamento) from par.pagamentoobra ppo
                  inner join par.pagamento pag on pag.pagid = ppo.pagid and pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
                  where ppo.preid = pre.preid )  as valor_pagamento_solicitado ,
                  CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
                  esd_obra.esddsc as situacao_obra,
                  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao,
                  pre.preanometa as ano_meta,
                  case when o.obrdtinicio is not null or  trim(to_char(o.obrdtinicio,'dd/mm/YYYY')) = ''
                       then to_char(o.obrdtinicio,'dd/mm/YYYY') 
			else 
				case when (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao  WHERE obrid = o.obrid  ) is NOT null   
				then (SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao  WHERE obrid = o.obrid  )
				ELSE
				(select  to_char(obrdtinicio,'dd/mm/YYYY')  from obras2.obras where obrid = o.obridvinculado )
			END
		  end as data_de_inicio_da_obra,
                  dataob.dataobra as data_de_termino_da_obra,
                  pre.entcodent,
		  pre.preanoselecao,
		  		  -- Ação preparatória
		  case when esd_obra.esdid in (689,771,1230) or (esd_obra.esdid in (766,768,875) and coalesce(o.obrpercentultvistoria,0) = 0) or (esd_obra.esdid in (884) AND (ter.data_termo is not null  or trim(ter.data_termo) <> '' ) )  then 'Ação preparatória'
		  -- Cancelado
		    when esd_obra.esdid in (769,767)  then 'Cancelado'
		  -- Concluida
		    when esd_obra.esdid in (693)  then 'Concluída'
		  -- Em obra
		   when  (esd_obra.esdid in (689,763,764) and o.obridpai is not null) or esd_obra.esdid in (690,691,770,1084) or (esd_obra.esdid in (766,768,875) and coalesce(o.obrpercentultvistoria,0) <> 0)
		   or esd_obra.esdid  in ( 873,861,864,872) then 'Em obra'
		    -- Em licitação de obra
		    when esd_obra.esdid in (763,764,870,862,863,871,874,875)  then 'Em licitação de obra'
		  -- Em contratação 
		    when (esd_obra.esdid in (689) and (ter.data_termo is  null  or trim(ter.data_termo) = '' ))
		    or ((esd_obra.esddsc is null and trim(esd.esddsc) in ('Obra Aprovada','Em reformulação')) and ((ter.data_termo is null or trim(ter.data_termo) = '' ) and emo.saldo is null )) 
		    OR ( pre.preanometa IS NOT NULL AND (ter.data_termo is  null  or trim(ter.data_termo) = '' ))   
		    then 'Em contratação'
		  END as situacao_planejamento,

		  -- Trata previsão de término em 12/2014
		  case when esd_obra.esdid = 690 then ( case when (SELECT round((((extract(days from ('2014-12-31' -  max(supdata) ) )::numeric)/30)* 5),2) FROM obras2.supervisao WHERE obrid = o.obrid) + ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)>= 100 then '100.00'
			ELSE
			(SELECT round((((extract(days from ('2014-12-31' -  max(supdata) ) )::numeric)/30)* 5) + ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2),2) 
			FROM obras2.supervisao WHERE obrid = o.obrid) end)::numeric(20,2) end  as previsao_execucao,		

		-- Trata Previsão Financeira
		-- Verifica se tem termo e diferente de execucao e calcula 20%
		case 	when    (ter.data_termo is not null  or trim(ter.data_termo) <> '' ) and esd_obra.esdid in (884,763,764,766,767,771,862,863,872,864,873,874,871,861,870,689,1230) 
				then 
				round((((case when esd.esdid not in (754,683,360,365,366,367)  then pre.prevalorobra else (select prevalorobra from obras.preobra where preidpai = pre.preid limit 1 ) end) * .20)  -
				(select sum(ppo.pobvalorpagamento) from par.pagamentoobra ppo
				inner join par.pagamento pag on pag.pagid = ppo.pagid and pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
				where ppo.preid = pre.preid ))::numeric(20,2),2)
		-- Verifica se está entre 0 e 29 e calcula 50%		
			when    (((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) > 0 and
				((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) <= 29) 
				then
				round((((case when esd.esdid not in (754,683,360,365,366,367)  then pre.prevalorobra else (select prevalorobra from obras.preobra where preidpai = pre.preid limit 1 ) end) * .50)  -
				(select sum(ppo.pobvalorpagamento) from par.pagamentoobra ppo
				inner join par.pagamento pag on pag.pagid = ppo.pagid and pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
				where ppo.preid = pre.preid ))::numeric(20,2) ,2)
		-- Verifica se está entre 30 e 59 e calcula 75%		
			when    (((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) >= 30 and
				((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) <= 59) 
				then
				round((((case when esd.esdid not in (754,683,360,365,366,367)  then pre.prevalorobra else (select prevalorobra from obras.preobra where preidpai = pre.preid limit 1 ) end) * .75)  -
				(select sum(ppo.pobvalorpagamento) from par.pagamentoobra ppo
				inner join par.pagamento pag on pag.pagid = ppo.pagid and pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
				where ppo.preid = pre.preid ))::numeric(20,2) ,2)
		-- Verifica se está entre 60 e 100 e calcula 100%		
			when    (((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) >= 60) 
				then
				round((((case when esd.esdid not in (754,683,360,365,366,367)  then pre.prevalorobra else (select prevalorobra from obras.preobra where preidpai = pre.preid limit 1 ) end))  -
				(select sum(ppo.pobvalorpagamento) from par.pagamentoobra ppo
				inner join par.pagamento pag on pag.pagid = ppo.pagid and pag.pagstatus = 'A' and pag.pagsituacaopagamento ilike '%EFETIVADO%'
				where ppo.preid = pre.preid ))::numeric(20,2) ,2)				
			
		end as previsao_financeira
            
            FROM obras.preobra pre
            LEFT JOIN workflow.documento doc ON doc.docid = pre.docid 
            LEFT JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid 
			LEFT JOIN obras2.obras o ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
			LEFT JOIN obras2.empreendimento 			e 	on e.empid = o.empid
            LEFT JOIN obras2.programafonte			prf	on prf.prfid = e.prfid
            LEFT JOIN (
			SELECT oi.obrid AS obrid,
			(SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A' AND staid = 3) AS dataobra
			FROM obras2.obras oi
			INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid 
			WHERE oi.obrstatus = 'A'
			AND doc2.esdid = 693) dataob ON dataob.obrid = o.obrid
            LEFT JOIN workflow.documento doc_obra ON doc_obra.docid  = o.docid
            LEFT JOIN workflow.estadodocumento esd_obra  on esd_obra.esdid = doc_obra.esdid 
            LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
            LEFT JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf 
            LEFT JOIN territorios.tipomunicipio tmn ON tmn.tpmid = mtm.tpmid 
            LEFT JOIN workflow.estadodocumento esd   ON doc.esdid = esd.esdid   
            LEFT JOIN par.vm_saldo_empenho_por_obra emo on emo.preid = pre.preid

            
            Left Join(
                  Select to_char(MAX(terdatainclusao),'dd/mm/YYYY') data_termo, teo.preid
                  From par.termocompromissopac ter 
                  Join par.termoobra teo on teo.terid = ter.terid 
                  Where ter.terassinado = true --and ter.terstatus='A' 
				  GROUP BY teo.preid
                  --Order by ter.terid desc limit 1
            ) as ter on ter.preid = pre.preid
            
            WHERE tmn.tpmid IN(163,164,165)  AND pre.docid IS NOT NULL AND pre.prestatus = 'A' AND pre.tooid = 1 -- and pre.preid = 9597

      
            GROUP BY  
                  pto.ptoclassificacaoobra,
				  prf.prfdesc,
                  pre.preid,
                  pre.estuf,
                  o.obrid,
                  mun.mundescricao, mun.muncod,
                  tmn.tpmdsc,
                  pto.ptodescricao,
                  pre.prevalorobra,
                  pre.predatareformulacao, 
                  esd.esddsc,
                  esd_obra.esdid,
                  esd.esdid,
                  pre.predescricao,
                  endereco_obra,
                  coordenada_geografica,
                  data_termo,
                  o.obridvinculado,
                  numero_parcelas,
                  valor_pagamento_solicitado ,
                  pre.preesfera,
                  situacao_obra,
                  percentual_execucao,
                  obrperccontratoanterior,
                  obrpercentultvistoria,
                  pre.preanometa,
                  o.obrdtinicio,
                  data_de_termino_da_obra,
                  pre.entcodent,
				  pre.preanoselecao,
				  situacao_planejamento
					ORDER BY
				  pre.preid
	";
	
	
	//ver(simec_htmlentities($sql), d);
	$registros = $db->carregar($sql);

	if($registros[0]) {

		$arCabecalho = array("tipo_obra",
				"n_protocolo_simec",
				"uf",
				"obrid",
				"municipio_beneficiado",
				"cod_ibge",
				"grupo_pac",
				"tipo_projeto",
				"valor_obra",
				"situacao_analise_fnde",
				"nome_obra",
				"endereco_obra",
				"coordenada_geografica",
				"data_termo",
				"resdescricao",
				"valor_empenho_solicitado",
				"valor_pagamento_solicitado",
				"numero_parcelas",
				"data_pagamento_siafi_1_parcela",
				"preesfera",
				"situacao_obra",
				"percentual_execucao",
				"ano_meta",
				"data_de_inicio_da_obra",
				"data_prevista_termino",
				"data_de_termino_da_obra",
				"cod_inep",
				"ano_selecao",
				"situacao_planejamento",
				"previsao_execucao",
				"previsao_financeira"
		);

		foreach($registros as $reg) {

			$arDados[] = array('tipo_obra'=>$reg['tipo_obra'],
					'n_protocolo_simec'=>$reg['n_protocolo_simec'],
					'uf'=>$reg['uf'],
					'obrid'=>$reg['obrid'],
					'municipio_beneficiado'=>$reg['municipio_beneficiado'],
					'cod_ibge'=>$reg['cod_ibge'],
					'grupo_pac'=>$reg['grupo_pac'],
					'tipo_projeto'=>$reg['tipo_projeto'],
					'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
					'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
					'nome_obra'=>$reg['nome_obra'],
					'endereco_obra'=>$reg['endereco_obra'],
					'coordenada_geografica'=>$reg['coordenada_geografica'],
					'data_termo'=>$reg['data_termo'],
					'resdescricao'=>$reg['resdescricao'],
					'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
					'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
					'numero_parcelas'=>$reg['numero_parcelas'],
					'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
					'preesfera'=>$reg['preesfera'],
					'situacao_obra'=>$reg['situacao_obra'],
					'percentual_execucao'=>$reg['percentual_execucao'],
					'ano_meta'=>$reg['ano_meta'],
					'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
					'data_prevista_termino'=>$reg['data_prevista_termino'],
					'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra'],
					'cod_inep'=>$reg['entcodent'],
					'ano_selecao'=>$reg['preanoselecao'],
					'situacao_planejamento'=>$reg['situacao_planejamento'],
					'previsao_execucao'=>$reg['previsao_execucao'],
					'previsao_financeira'=>$reg['previsao_financeira']

			);
		}
	}

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PAC_Trad".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PAC_Trad".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}



function relatorioPlanejamentoTablet(){
	
	global $db;
	
	$sql = "SELECT 
				rel.uf, rel.mund, rel.muncod, rel.sbadsc, 
            	rel.ordem, rel.sbaid, rel.quantidade,
				SUM(rel.empenho_solicitado_subacao) Empenhado, 
            	SUM(rel.pagamento_solicitado_subacao) Pago, rel.tipo ,ptres
			FROM 
				(
				SELECT DISTINCT
					CASE WHEN inu.itrid = 1 THEN inu.estuf ELSE inu.mun_estuf END  as uf,
					mun.mundescricao as mund,
					mun.muncod, 
					sba.sbaid,
					sba.sbadsc  as sbadsc,
					sba.sbaordem as ordem,
					emp.empnumero,
					CASE WHEN sba.sbacronograma = 1 
						THEN sum(ico.icoquantidadetecnico)
						ELSE sum(ssi.seiqtdtecnico)
					END as quantidade,       
					--((empsub.eovalor + coalesce(ref.vlrref,0)) - coalesce(vrlcancelado,0)) 
					empsub.saldo as empenho_solicitado_subacao, 
					pag.valorpag as pagamento_solicitado_subacao, 
					CASE WHEN inu.itrid = 2 AND inu.inuid <> 1 THEN 'Municipal' ELSE 'Estadual' END tipo,
					sbd.sbdptres as ptres
				FROM 
					par.subacao sba 
				LEFT  JOIN (
					SELECT DISTINCT sbaid sbaid, sbdptres sbdptres 
					FROM par.subacaodetalhe 
					WHERE sbdptres IS NOT NULL ) 				sbd 	ON sbd.sbaid = sba.sbaid 
				INNER JOIN par.v_dadosempenhosubacao 			empsub 	ON empsub.sbaid = sba.sbaid
				/*
				inner join (
					select empid empid, sbaid sbaid, sum(eobvalorempenho) eovalor 
					from par.empenhosubacao 
					where eobstatus = 'A' group by empid,sbaid)	empsub on empsub.sbaid = sba.sbaid */
				LEFT  JOIN (
					SELECT empnumeroprocesso, empidpai, SUM(empvalorempenho) as vlrref, empcodigoespecie 
					FROM par.empenho
					WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
					GROUP BY 
						empnumeroprocesso,
						empcodigoespecie,
 						empidpai) as 							ref 	ON ref.empidpai = empsub.empid
				LEFT  JOIN (
					SELECT SUM(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid
					FROM par.empenhosubacao eb
					INNER JOIN par.empenho e1 ON e1.empid = eb.empid AND empstatus = 'A' AND eobstatus = 'A'
					WHERE e1.empcodigoespecie IN ('03', '13', '04') AND empidpai IS NOT NULL
					GROUP BY e1.empidpai, eb.sbaid
					) as 										emc 	ON emc.empidpai = empsub.empid AND emc.sbaid = empsub.sbaid 
				LEFT  JOIN (
            		SELECT empid as empipag, SUM(pagvalorparcela) valorpag 
            		FROM par.pagamento 
            		WHERE pagsituacaopagamento ilike '%EFETIVADO%' AND pagstatus = 'A' 
            		GROUP BY EMPID) 							pag 	ON pag.empipag = empsub.empid
				INNER JOIN par.empenho 							emp 	ON emp.empid = empsub.empid AND emp.empstatus = 'A' AND emp.empcodigoespecie NOT IN ('03', '13', '04', '02')
				INNER JOIN par.subacaoitenscomposicao 			ico 	ON ico.sbaid = sba.sbaid AND ico.icoano = 2012 AND ico.icostatus = 'A' AND ico.icovalidatecnico = 'S'
				LEFT  JOIN par.subescolas_subitenscomposicao 	ssi 	ON ssi.icoid = ico.icoid 
				INNER JOIN par.propostaitemcomposicao 			pic 	ON pic.picid = ico.picid AND pic.picstatus = 'A'
				INNER JOIN par.processopar 						prp 	ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				INNER JOIN par.instrumentounidade 				inu 	ON inu.inuid = prp.inuid 
				LEFT  JOIN territorios.municipio 				mun 	ON mun.muncod = prp.muncod
				WHERE emp.empsituacao <> 'CANCELADO' AND sba.sbastatus = 'A' AND ico.picid in (1083,1084)
				GROUP BY uf, mun.mundescricao, mun.muncod,sba.sbaid,sba.sbaordem,sba.sbadsc,empenho_solicitado_subacao,pag.valorpag,tipo,sba.sbacronograma,emp.empnumero,sbd.sbdptres 
				ORDER BY tipo,uf,mund)  rel 
			GROUP BY 
				rel.uf, rel.mund, rel.muncod, rel.sbadsc, rel.ordem, rel.sbaid, rel.quantidade, rel.tipo,ptres
			ORDER BY 
				rel.tipo, rel.uf, rel.mund";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {

		$arCabecalho = array("uf",
							 "mund",
							 "muncod",
							 "sbadsc",
							 "ordem",
							 "sbaid",
							 "quantidade",
							 "Empenhado",
							 "Pago",
							 "tipo",
							 "ptres"
		);
	
		foreach($registros as $reg) {
			$arDados[] = array('uf'=>$reg['uf'],
							   'mund'=>$reg['mund'],
							   'muncod'=>$reg['muncod'],
							   'sbadsc'=>$reg['sbadsc'],
							   'ordem'=>$reg['ordem'],
							   'sbaid'=>$reg['sbaid'],
							   'quantidade'=>$reg['quantidade'],
							   'Empenhado'=>$reg['empenhado'],
							   'Pago'=>$reg['pago'],
							   'tipo'=>$reg['tipo'],
							   'ptres'=>$reg['ptres']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Tablet".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Tablet".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoProjetor(){
	global $db;
	
	$sql = "SELECT 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,
				rel.quantidade,SUM(rel.empenho_solicitado_subacao) Empenhado, 
				SUM(rel.pagamento_solicitado_subacao) Pago, rel.tipo ,ptres
			FROM 
				(
				SELECT 
					CASE WHEN inu.itrid = 1 THEN inu.estuf ELSE inu.mun_estuf END as uf,
					mun.mundescricao as mund,
					mun.muncod, 
					sba.sbaid,
					sba.sbadsc sbadsc,
					sba.sbaordem ordem,
					emp.empnumero,
					CASE WHEN sba.sbacronograma = 1 
						THEN sum(ico.icoquantidadetecnico)
						ELSE sum(ssi.seiqtdtecnico)
					END as quantidade,  
					---------------                                                                             
					--((empsub.eovalor + coalesce(ref.vlrref,0)) - coalesce(vrlcancelado,0)) as empenho_solicitado_subacao,
					empsub.saldo as empenho_solicitado_subacao, 
					---------------
					pag.valorpag pagamento_solicitado_subacao, 
					case when inu.itrid = 2 and inu.inuid <> 1 then 'Municipal' else 'Estadual' end   tipo ,
					sbd.sbdptres ptres
				FROM 
					par.subacao sba 
				LEFT  JOIN(
					SELECT DISTINCT sbaid sbaid, sbdptres sbdptres 
					FROM par.subacaodetalhe 
					WHERE sbdptres IS NOT NULL ) 				sbd 	ON sbd.sbaid = sba.sbaid 
				--------------
				INNER JOIN par.v_dadosempenhosubacao 			empsub 	ON empsub.sbaid = sba.sbaid
				/*inner join (
					select empid empid, sbaid sbaid, sum(eobvalorempenho) eovalor 
					from par.empenhosubacao 
					where eobstatus = 'A' 
					group by empid,sbaid) empsub on empsub.sbaid = sba.sbaid 
				left join (
					select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
					from par.empenho
					where empcodigoespecie in ('02') and empstatus = 'A'
					group by empnumeroprocesso, empcodigoespecie, empidpai) as ref on ref.empidpai = empsub.empid*/
				-----------
				LEFT JOIN(
					SELECT SUM(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid
					FROM par.empenhosubacao eb
					INNER JOIN par.empenho e1 ON e1.empid = eb.empid AND empstatus = 'A' AND eobstatus = 'A'
					WHERE e1.empcodigoespecie IN ('03', '13', '04') AND empidpai IS NOT NULL
					GROUP BY e1.empidpai, eb.sbaid
					) as 										emc 	ON emc.empidpai = empsub.empid AND emc.sbaid = empsub.sbaid 
				LEFT  JOIN(
					SELECT empid empipag, SUM(pagvalorparcela) as valorpag 
					FROM par.pagamento 
					WHERE pagsituacaopagamento ilike '%EFETIVADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) 							pag 	ON pag.empipag = empsub.empid
				INNER JOIN par.empenho 							emp 	ON emp.empid = empsub.empid AND emp.empstatus = 'A' AND emp.empcodigoespecie NOT IN ('03', '13', '04')
				INNER JOIN par.subacaoitenscomposicao 			ico 	ON ico.sbaid = sba.sbaid AND ico.icoano = 2012 AND ico.icostatus = 'A' AND ico.icovalidatecnico = 'S'
				LEFT  JOIN par.subescolas_subitenscomposicao 	ssi 	ON ssi.icoid = ico.icoid 
				INNER JOIN par.propostaitemcomposicao 			pic 	ON pic.picid = ico.picid AND pic.picstatus = 'A'
				INNER JOIN par.processopar 						prp 	ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				INNER JOIN par.instrumentounidade 				inu 	ON inu.inuid = prp.inuid 
				LEFT  JOIN territorios.municipio	 			mun 	ON mun.muncod = prp.muncod
				WHERE emp.empsituacao <> 'CANCELADO' AND sba.sbastatus = 'A' AND ico.picid IN (198)
				GROUP BY uf, mun.mundescricao, mun.muncod,sba.sbaid,sba.sbaordem,sba.sbadsc,empenho_solicitado_subacao,pag.valorpag,tipo,sba.sbacronograma,emp.empnumero,sbd.sbdptres 
				ORDER BY tipo,uf,mund)  rel 
			GROUP BY 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,rel.tipo,ptres
			ORDER BY 
				rel.tipo,rel.uf,rel.mund";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {

		$arCabecalho = array("uf",
							 "mund",
							 "muncod",
							 "sbadsc",
							 "ordem",
							 "sbaid",
							 "quantidade",
							 "Empenhado",
							 "Pago",
							 "tipo",
							 "ptres"
		);
	
		foreach($registros as $reg) {
			$arDados[] = array('uf'=>$reg['uf'],
							   'mund'=>$reg['mund'],
							   'muncod'=>$reg['muncod'],
							   'sbadsc'=>$reg['sbadsc'],
							   'ordem'=>$reg['ordem'],
							   'sbaid'=>$reg['sbaid'],
							   'quantidade'=>$reg['quantidade'],
							   'Empenhado'=>$reg['empenhado'],
							   'Pago'=>$reg['pago'],
							   'tipo'=>$reg['tipo'],
							   'ptres'=>$reg['ptres']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Projetor".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Projetor".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoOnibus(){
	global $db;
	
	$sql = "SELECT 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,
				rel.quantidade,sum(rel.empenho_solicitado_subacao) Empenhado, sum (rel.pagamento_solicitado_subacao) Pago, rel.tipo,ptres 
			FROM 
				(
				SELECT 
					CASE WHEN inu.itrid = 1 THEN inu.estuf ELSE inu.mun_estuf END  as uf,
					mun.mundescricao as mund,
					mun.muncod, 
					sba.sbaid,
					par.retornacodigosubacao(sba.sbaid)||' - '||sba.sbadsc as sbadsc,
					sba.sbaordem as ordem,
					SUM(ico.icoquantidadetecnico) as quantidade,
					---------------------
					--((empsub.eovalor + coalesce(ref.vlrref,0)) - coalesce(vrlcancelado,0)) 
					empsub.saldo as empenho_solicitado_subacao, 
					---------------------
					pag.valorpag as pagamento_solicitado_subacao, 
					CASE WHEN inu.itrid = 2 AND inu.inuid <> 1 THEN 'Municipal' ELSE 'Estadual' END as tipo,
					sbd.sbdptres as ptres
				FROM 
					par.subacao sba 
				LEFT JOIN (
					SELECT DISTINCT sbaid sbaid, sbdptres sbdptres 
					FROM par.subacaodetalhe 
					WHERE sbdptres IS NOT NULL ) 		sbd 	ON sbd.sbaid = sba.sbaid 
				----------------------
				INNER JOIN par.v_dadosempenhosubacao 	empsub 	ON empsub.sbaid = sba.sbaid
				/*inner join (
					select empid empid, sbaid sbaid, sum(eobvalorempenho) eovalor 
					from par.empenhosubacao 
					where eobstatus = 'A' 
					group by empid,sbaid) empsub on empsub.sbaid = sba.sbaid 
				left  join (
					select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
					from par.empenho
					where empcodigoespecie in ('02') and empstatus = 'A'
					group by 
						empnumeroprocesso,
						empcodigoespecie,
						empidpai) as ref on ref.empidpai = empsub.empid*/
				----------------------
				LEFT JOIN (
					SELECT SUM(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid
					FROM par.empenhosubacao eb
					INNER JOIN par.empenho e1 ON e1.empid = eb.empid AND empstatus = 'A' AND eobstatus = 'A'
					WHERE e1.empcodigoespecie IN ('03', '13', '04') AND empidpai IS NOT NULL
					GROUP BY e1.empidpai, eb.sbaid
				) as 									emc 	ON emc.empidpai = empsub.empid AND emc.sbaid = empsub.sbaid 
				LEFT JOIN (
					SELECT empid empipag, SUM(pagvalorparcela) as valorpag 
					FROM par.pagamento 
					WHERE pagsituacaopagamento ilike '%EFETIVADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) 					pag 	ON pag.empipag = empsub.empid
				INNER JOIN par.empenho 					emp 	ON emp.empid = empsub.empid AND emp.empstatus = 'A' AND emp.empcodigoespecie NOT IN ('03', '13', '04')
				INNER JOIN par.subacaoitenscomposicao 	ico 	ON ico.sbaid = sba.sbaid AND ico.icoano = 2012 AND ico.icostatus = 'A' AND ico.icovalidatecnico = 'S'
				INNER JOIN par.propostaitemcomposicao 	pic 	ON pic.picid = ico.picid AND pic.picstatus = 'A'
				INNER JOIN par.processopar 				prp 	ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				INNER JOIN par.instrumentounidade 		inu 	ON inu.inuid = prp.inuid 
				LEFT  JOIN territorios.municipio 		mun 	ON mun.muncod = prp.muncod
				WHERE 
					emp.empsituacao <> 'CANCELADO' AND sba.sbastatus = 'A' AND ico.picid IN (362,361,363,364,365,366,359,1082,1080,1081,1079)
				GROUP BY 
					uf, mun.mundescricao, mun.muncod,sba.sbaid,sba.sbaordem,sba.sbadsc,empenho_solicitado_subacao,pag.valorpag,tipo,ptres
				HAVING 
					SUM(ico.icoquantidadetecnico) > 0
				ORDER BY 
					tipo,uf,mund
				)  rel 
			GROUP BY 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,rel.tipo,ptres
			ORDER BY 
				rel.tipo,rel.uf,rel.mund";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {

		$arCabecalho = array("uf",
							 "mund",
							 "muncod",
							 "sbadsc",
							 "ordem",
							 "sbaid",
							 "quantidade",
							 "Empenhado",
							 "Pago",
							 "tipo",
							 "ptres"
		);
	
		foreach($registros as $reg) {
			$arDados[] = array('uf'=>$reg['uf'],
							   'mund'=>$reg['mund'],
							   'muncod'=>$reg['muncod'],
							   'sbadsc'=>$reg['sbadsc'],
							   'ordem'=>$reg['ordem'],
							   'sbaid'=>$reg['sbaid'],
							   'quantidade'=>$reg['quantidade'],
							   'Empenhado'=>$reg['empenhado'],
							   'Pago'=>$reg['pago'],
							   'tipo'=>$reg['tipo'],
							   'ptres'=>$reg['ptres']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Onibus".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Onibus".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoMobiliario(){
	global $db;
	
	$sql = "SELECT 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,
				SUM(rel.empenho_solicitado_subacao) Empenhado, SUM(rel.pagamento_solicitado_subacao) Pago, 
				rel.tipo ,ptres
			FROM 
				(
				SELECT 
					CASE WHEN inu.itrid = 1 THEN inu.estuf ELSE inu.mun_estuf END as uf,
					mun.mundescricao as mund,
					mun.muncod, 
					sba.sbaid,
					sba.sbadsc as sbadsc,
					sba.sbaordem as ordem,
					emp.empnumero,
					CASE WHEN sba.sbacronograma = 1 
						THEN sum(ico.icoquantidadetecnico)
						ELSE sum(ssi.seiqtdtecnico)
					END as quantidade,                                                                               
					--((empsub.eovalor + coalesce(ref.vlrref,0)) - coalesce(vrlcancelado,0)) empenho_solicitado_subacao,                                                                               
					empsub.saldo as empenho_solicitado_subacao, 
					pag.valorpag as pagamento_solicitado_subacao, 
					CASE WHEN inu.itrid = 2 AND inu.inuid <> 1 THEN 'Municipal' ELSE 'Estadual' END as tipo,
					sbd.sbdptres ptres
				FROM 
					par.subacao sba 
				LEFT  JOIN (
					SELECT DISTINCT sbaid sbaid, sbdptres as sbdptres 
					FROM par.subacaodetalhe 
					WHERE sbdptres IS NOT NULL ) 				sbd 	ON sbd.sbaid = sba.sbaid 
				------------
				INNER JOIN par.v_dadosempenhosubacao 			empsub 	ON empsub.sbaid = sba.sbaid
				/*inner join (
					select empid empid, sbaid sbaid, sum(eobvalorempenho) eovalor 
					from par.empenhosubacao where eobstatus = 'A' 
					group by empid,sbaid) empsub on empsub.sbaid = sba.sbaid 
				left  join (
					select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
					from par.empenho
					where empcodigoespecie in ('02') and empstatus = 'A'
					group by 
						empnumeroprocesso,
						empcodigoespecie,
						empidpai) as ref on ref.empidpai = empsub.empid*/
				------------
				LEFT  JOIN (
					SELECT SUM(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid
					FROM par.empenhosubacao eb
					INNER JOIN par.empenho e1 ON e1.empid = eb.empid AND empstatus = 'A' AND eobstatus = 'A'
					WHERE e1.empcodigoespecie IN ('03', '13', '04') AND empidpai IS NOT NULL
					GROUP BY e1.empidpai, eb.sbaid
					) as 										emc 	ON emc.empidpai = empsub.empid AND emc.sbaid = empsub.sbaid 
				LEFT  JOIN (
					SELECT empid empipag, SUM(pagvalorparcela) valorpag 
					FROM par.pagamento 
					WHERE pagsituacaopagamento ilike '%EFETIVADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) 							pag 	ON pag.empipag = empsub.empid
				INNER JOIN par.empenho 							emp 	ON emp.empid = empsub.empid AND emp.empstatus = 'A' AND emp.empcodigoespecie NOT IN ('03', '13', '04')
				INNER JOIN par.subacaoitenscomposicao 			ico 	ON ico.sbaid = sba.sbaid AND ico.icoano = 2012 AND ico.icostatus = 'A' AND ico.icovalidatecnico = 'S'
				LEFT  JOIN par.subescolas_subitenscomposicao 	ssi 	ON ssi.icoid = ico.icoid 
				INNER JOIN par.propostaitemcomposicao 			pic	 	ON pic.picid = ico.picid AND pic.picstatus = 'A'
				INNER JOIN par.processopar 						prp 	ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				INNER JOIN par.instrumentounidade 				inu 	ON inu.inuid = prp.inuid 
				LEFT  JOIN territorios.municipio 				mun 	ON mun.muncod = prp.muncod
				WHERE emp.empsituacao <> 'CANCELADO' AND sba.sbastatus = 'A' AND ico.picid IN (468,467,466,471,472,469,470)
				GROUP BY uf, mun.mundescricao, mun.muncod,sba.sbaid,sba.sbaordem,sba.sbadsc,empenho_solicitado_subacao,pag.valorpag,tipo,sba.sbacronograma,emp.empnumero,sbd.sbdptres 
				ORDER BY tipo,uf,mund)  rel 
			GROUP BY 
                rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,rel.tipo,ptres
			ORDER BY 
                rel.tipo,rel.uf,rel.mund";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {

		$arCabecalho = array("uf",
							 "mund",
							 "muncod",
							 "sbadsc",
							 "ordem",
							 "sbaid",
							 "quantidade",
							 "Empenhado",
							 "Pago",
							 "tipo",
							 "ptres"
		);
	
		foreach($registros as $reg) {
			$arDados[] = array('uf'=>$reg['uf'],
							   'mund'=>$reg['mund'],
							   'muncod'=>$reg['muncod'],
							   'sbadsc'=>$reg['sbadsc'],
							   'ordem'=>$reg['ordem'],
							   'sbaid'=>$reg['sbaid'],
							   'quantidade'=>$reg['quantidade'],
							   'Empenhado'=>$reg['empenhado'],
							   'Pago'=>$reg['pago'],
							   'tipo'=>$reg['tipo'],
							   'ptres'=>$reg['ptres']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Mobiliario".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Mobiliario".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

function relatorioPlanejamentoArCondicionado(){
	global $db;
	
	$sql = "SELECT 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,
				rel.quantidade,SUM(rel.empenho_solicitado_subacao) Empenhado, 
				SUM(rel.pagamento_solicitado_subacao) Pago, rel.tipo ,ptres
			FROM 
				(
				SELECT 
					CASE WHEN inu.itrid = 1 THEN inu.estuf ELSE inu.mun_estuf END as uf,
					mun.mundescricao as mund,
					mun.muncod, 
					sba.sbaid,
					sba.sbadsc as sbadsc,
					sba.sbaordem as ordem,
					emp.empnumero,
					CASE WHEN sba.sbacronograma = 1 
						THEN sum(ico.icoquantidadetecnico)
						ELSE sum(ssi.seiqtdtecnico)
					END as quantidade,                                                                               
					--((empsub.eovalor + coalesce(ref.vlrref,0)) - coalesce(vrlcancelado,0)) as empenho_solicitado_subacao, 
					empsub.saldo as empenho_solicitado_subacao, 
					pag.valorpag pagamento_solicitado_subacao, 
					CASE WHEN inu.itrid = 2 AND inu.inuid <> 1 THEN 'Municipal' ELSE 'Estadual' END as tipo,
					sbd.sbdptres ptres
				FROM 
					par.subacao sba 
				LEFT  JOIN (
					SELECT distinct sbaid sbaid, sbdptres sbdptres 
					FROM par.subacaodetalhe 
					WHERE sbdptres IS NOT NULL ) 				sbd 	ON sbd.sbaid = sba.sbaid 
				------------
				INNER JOIN par.v_dadosempenhosubacao 			empsub 	ON empsub.sbaid = sba.sbaid
				/*inner join (
					select empid empid, sbaid sbaid, sum(eobvalorempenho) eovalor 
					from par.empenhosubacao 
					where eobstatus = 'A' 
					group by empid,sbaid) empsub on empsub.sbaid = sba.sbaid 
				left  join (
					select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
					from par.empenho
					where empcodigoespecie in ('02') and empstatus = 'A'
					group by 
						empnumeroprocesso,
						empcodigoespecie,
						empidpai) as ref on ref.empidpai = empsub.empid*/
				------------
				LEFT  JOIN(
					SELECT SUM(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid
					FROM par.empenhosubacao eb
					INNER JOIN par.empenho e1 ON e1.empid = eb.empid AND empstatus = 'A' AND eobstatus = 'A'
					WHERE e1.empcodigoespecie IN ('03', '13', '04') AND empidpai IS NOT NULL
					GROUP BY e1.empidpai, eb.sbaid) as 			emc 	ON emc.empidpai = empsub.empid AND emc.sbaid = empsub.sbaid 
				LEFT  JOIN(
					SELECT empid empipag, SUM(pagvalorparcela) as valorpag 
					FROM par.pagamento 
					WHERE pagsituacaopagamento ilike '%EFETIVADO%' AND pagstatus = 'A' 
					GROUP BY EMPID) 							pag 	ON pag.empipag = empsub.empid
				INNER JOIN par.empenho 							emp 	ON emp.empid = empsub.empid AND emp.empstatus = 'A' AND emp.empcodigoespecie NOT IN ('03', '13', '04')
				INNER JOIN par.subacaoitenscomposicao 			ico 	ON ico.sbaid = sba.sbaid and ico.icoano = 2012 and ico.icostatus = 'A' and ico.icovalidatecnico = 'S'
				LEFT  JOIN par.subescolas_subitenscomposicao 	ssi 	ON ssi.icoid = ico.icoid 
				INNER JOIN par.propostaitemcomposicao 			pic 	ON pic.picid = ico.picid AND pic.picstatus = 'A'
				INNER JOIN par.processopar 						prp 	ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				INNER JOIN par.instrumentounidade 				inu 	ON inu.inuid = prp.inuid 
				LEFT  JOIN territorios.municipio 				mun 	ON mun.muncod = prp.muncod
				WHERE 
					emp.empsituacao <> 'CANCELADO' AND sba.sbastatus = 'A' AND ico.picid IN (228,229,230,232,233,234,235,385,386,387,822,1262)
				GROUP BY 
					uf, mun.mundescricao, mun.muncod,sba.sbaid,sba.sbaordem,sba.sbadsc,empenho_solicitado_subacao,pag.valorpag,tipo,sba.sbacronograma,emp.empnumero,sbd.sbdptres 
				ORDER BY 
					tipo,uf,mund)  rel 
			GROUP BY 
				rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,rel.tipo,ptres
			ORDER BY 
				rel.tipo,rel.uf,rel.mund";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {

		$arCabecalho = array("uf",
							 "mund",
							 "muncod",
							 "sbadsc",
							 "ordem",
							 "sbaid",
							 "quantidade",
							 "Empenhado",
							 "Pago",
							 "tipo",
							 "ptres"
		);
	
		foreach($registros as $reg) {
			$arDados[] = array('uf'=>$reg['uf'],
							   'mund'=>$reg['mund'],
							   'muncod'=>$reg['muncod'],
							   'sbadsc'=>$reg['sbadsc'],
							   'ordem'=>$reg['ordem'],
							   'sbaid'=>$reg['sbaid'],
							   'quantidade'=>$reg['quantidade'],
							   'Empenhado'=>$reg['empenhado'],
							   'Pago'=>$reg['pago'],
							   'tipo'=>$reg['tipo'],
							   'ptres'=>$reg['ptres']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_AC".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_AC".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}

?>