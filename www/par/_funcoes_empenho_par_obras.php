<?php
function pegaCnpj($inuid, $proid){
	global $db;
	$cnpj = $db->pegaUm("SELECT procnpj FROM par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
	if( $cnpj ){
		return $cnpj;
	} else {
		return $db->pegaUm("SELECT iue.iuecnpj
		                     FROM par.instrumentounidade iu
		                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		                     WHERE
		                     	iu.inuid = {$inuid}
		                     	and iue.iuestatus = 'A'
		                        and iue.iuedefault = true");
	}
}

/* function listaHistoricoEmpenho($dados) {
	global $db;
	$sql = "SELECT u.usunome, to_char(hepdata, 'dd/mm/YYYY HH24:MI') as data, empsituacao, ds_problema, valor_total_empenhado, valor_saldo_pagamento
			FROM par.historicoempenho h
			LEFT JOIN seguranca.usuario u ON u.usucpf=h.usucpf
			WHERE h.empid='".$dados['empid']."'";
	echo $sql;
	$cabecalho = array("Usuário atualização","Data","Situação","Problema encontrado","Valor empenhado(R$)","Valor pagamento(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

} */

function carregarPtres($dados){

	global $db;

	$sql = "select distinct
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$dados['plicod']}'";

	if( $dados['preid'] ){
		$ptres = $db->pegaUm("select distinct sd.sbdptres from par.subacaoobra so
									inner join par.subacaodetalhe sd on sd.sbaid = so.sbaid and sd.sbdano = so.sobano
								where so.preid = {$dados['preid']}");
	}

	$db->monta_combo("ptres", $sql, 'S', 'Selecione...', '',"","","","N","","", $ptres);

}

function cabecalhoSolicitacaoEmpenho($proid)  {
	global $db;

	if($_SESSION['par_var']['esfera']=='estadual') {

		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   '-' as mundescricao,
										   p.pronumeroprocesso,
										  '' as tipoobra,
										   p.protipo
									FROM par.processoobraspar p
								    WHERE
									p.prostatus = 'A' and
								    p.proid = '$proid'");

	} else {

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   '' as tipoobra,
										   p.protipo
									FROM par.processoobraspar p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod and p.prostatus = 'A' 
								    WHERE p.proid= '$proid'");


	}

	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita width='30%'>UF:</td>";
	echo "<td>".$arrDados['estuf']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Município:</td>";
	echo "<td>".$arrDados['mundescricao']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Nº processo:</td>";
	echo "<td>".$arrDados['pronumeroprocesso']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Tipo obra:</td>";
	echo "<td>".$arrDados['tipoobra']."</td>";
	echo "</tr>";
	echo "</table>";
}

function listaObrasEmpenhadas( $dados ){
	global $db;
	$empid = $dados['empid'];
	$proid = $dados['proid'];
		

	$where[] = "empid = $empid";

	$sql = "SELECT distinct
				'<img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"carregarDadosObra( \'\', '||po.preid||', '||po.preano||');\">' as mais,
				'<a class=vizualisa_obra preid='||po.preid||' sbaid=\'\' sobano='||po.preano||' >'||po.preid || ' - ' || po.predescricao||'</a>' as nomedaobra,
				po.prevalorobra as vlr,
				'<center>'||((( dfo.saldo )/ROUND(po.prevalorobra, 2))*100)::numeric(20,2)||'</center>' as porcempenho,
				dfo.saldo  as vlr_empenho
			FROM obras.preobra po
			INNER JOIN par.v_saldo_obra_por_empenho dfo ON dfo.preid = po.preid
			WHERE 
				".implode(" AND ", $where)." ";
// 	ver( simec_htmlentities($sql),d );
	$cabecalho = array("&nbsp;","Nome da obra","Valor da obra","% Empenho","Valor empenhado");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','95%','S');
}


function listaPreObras($dados) {
	global $db;
	$proid = $dados['proid'];
	$dadosse = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo, sisid FROM par.processoobraspar WHERE prostatus = 'A' and proid='".$proid."'");

	if($_SESSION['par_var']['esfera']=='estadual') {
		if(!($dadosse['sisid'] == 14)){
			$where[] = "po.preesfera='E'";
		}

		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   p.pronumeroprocesso,
										   '' as tipoobra,
										   p.protipo
									FROM par.processoobraspar p
								    WHERE p.prostatus = 'A' and
								    p.proid='".$proid."'");

	} else {
		if(!$dadosse['sisid'] == 14){
			$where[] = "po.preesfera='M'";
		}

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   '' as tipoobra,
										   p.protipo
									FROM par.processoobraspar p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE p.prostatus = 'A' and
								    p.proid='".$proid."'");


	}


	// PARAMETROS FIXOS
	$where[] = "po.prestatus='A'";

	if($dadosse['sisid'] == 14){
		$where[] = "doc.esdid in ('397', '337')";
		$where[] = "po.tooid in ('6', '2')";

	}else{
		//	$where[] = "po.tooid='2'";
		//$where[] = "doc.esdid IN ('".WF_PAR_OBRA_APROVADA."','".WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL."', '".WF_PAR_EM_REVISAO_DE_ANALISE."','".WF_PAR_OBRA_EM_REFORMULACAO."', '".WF_PAR_EM_DILIGENCIA."')";
	}
	// FIM PARAMETROS FIXOS

	if($arrDados['estuf']) {
		$where[] = "po.estuf='".$arrDados['estuf']."'";
	}

	if($arrDados['muncod']) {
		$where[] = "po.muncod='".$arrDados['muncod']."'";
	}

	if($arrDados['protipo']) {
		//$where[] = "pp.ptoclassificacaoobra='".$arrDados['protipo']."'";
	}

	/* Filtro padrão pegando municipio, UF, e tipo de obra */
	if($where) {
		$clwhere = "WHERE ".implode(" AND ", $where)." ";
	}

    $sql = "SELECT DISTINCT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarObrasEmpenhadasPAR(\''||emp.empid||'\', this, \'".$proid."\');\">' as mais,
						emp.empnumero
				FROM obras.preobra po
				INNER JOIN obras.pretipoobra pp on po.ptoid = pp.ptoid
				INNER JOIN workflow.documento 		doc ON doc.docid = po.docid
				INNER JOIN obras.preitenscomposicao itc ON po.ptoid = itc.ptoid --AND itcquantidade > 0
				LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid   = ppo.itcid AND ppo.preid = po.preid
				INNER JOIN par.empenhoobrapar emo ON emo.preid   = po.preid and eobstatus = 'A' 
				INNER JOIN par.empenho emp ON emp.empid = emo.empid and emp.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
				WHERE 	
					emo.empid IN (
    								SELECT distinct emp.empid FROM par.empenho emp
									  INNER JOIN par.processoobraspar pro ON emp.empnumeroprocesso = pro.pronumeroprocesso
									  inner join par.v_dadosempenhos v on v.empid = emp.empid
									WHERE pro.proid = $proid
										and emp.empcodigoespecie not in ('03', '13', '02', '04') 
									    and pro.prostatus = 'A' 
									    and emp.empstatus = 'A' 
									group by emp.empid, emp.empnumero
									having sum(v.saldo) > 0)
					AND emo.preid in (	select po.preid
										from obras.preobra po
											INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
											INNER JOIN par.empenhoobrapar emo ON emo.preid   = po.preid and eobstatus = 'A'
											inner join par.empenho e on e.empid = emo.empid and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
											left join (
												select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
													from par.empenhoobrapar eb
														inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
													where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
													group by e1.empidpai, eb.preid
											) as emc on emc.empidpai = e.empid and emc.preid = emo.preid
											WHERE prevalorobra > 0
											group by po.preid 
											having sum((( (eobvalorempenho - coalesce(emc.vlrempenhocancelado, 0)) * 100) / prevalorobra))  >= 99.9																		
										)						
					and ".implode(" AND ", $where)."
				GROUP BY emp.empnumero,emp.empid";
 	//ver(simec_htmlentities($sql),d);
	echo "<h3>Obras 100% empenhadas</h3>";
	$cabecalho = array("&nbsp;","Nº do Empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','N');

	if($dadosse) {
		$sql = "
			select 
				chk,
				nomedaobra,
				'<center>'||(SELECT obrpercentultvistoria FROM obras2.obras WHERE preid = foo.preid AND obrstatus = 'A' AND obridpai IS NULL)::integer||' %</center>' as perc,
				vlr,
				empenhos,
				percentual_Empenhado,
				vlr_empenhado,
				porcempenho,
				--vlr_empenho,
				'<input type=text id=id_vlr_'||preid||' value=\'0,00\' 
					name=name_vlr_'||preid||' size=20 onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" 
					onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenhoObraPar('||preid||', \'v\');\" class=\"disabled vrlaempenhar\" readonly=readonly onfocus=\"this.select();\">
				<input type=\"hidden\" name=\"vlrobra_'||preid||'\" id=\"vlrobra_'||preid||'\" value=\"'||vlr||'\">
				<input type=\"hidden\" name=\"vlr_empenhado_'||preid||'\" id=\"vlr_empenhado_'||preid||'\" value=\"'||vlr_empenhado||'\">' as valoraempenhar,
				ano
			from(
			SELECT
				case when po.preidpai is not null then
                	'<img src=../imagens/restricao_ico.png style=cursor:pointer; title=\"Obra Em Reformulação\">'
                else
                	'<input type=checkbox name=chk[] onclick=marcarChkObrasParEmp(this); id=chk_'||po.preid||' value='||po.preid||'>'
                end ||'<img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra('||so.sbaid||', '||po.preano||')\">' as chk,
                po.preid,
				'<a class=vizualisa_obra preid='||po.preid||' sbaid='||so.sbaid||' sobano='||so.sobano||' >'||po.preid || ' - ' || po.predescricao||'</a>' as nomedaobra,
				--po.preid || ' - ' || po.predescricao as nomedaobra,
				ROUND(po.prevalorobra, 2) as vlr,
				par.retorna_numero_empenhos_obra_par(po.preid) as empenhos,
				--'<center>'||SUM(emo.eobpercentualemp)::text||'</center> <input type=hidden id=porcentagem_'||po.preid||' value='||SUM(emo.eobpercentualemp)||' >' as percentual_Empenhado,
				CASE WHEN po.prevalorobra > 0 
					THEN '<center>'|| ( (SUM(emo.eobvalorempenho) - coalesce(SUM(emc.vlrempenhocancelado), 0)) * 100 ) / po.prevalorobra ||' </center> <input type=hidden id=porcentagem_'||po.preid||' value='||( (SUM(emo.eobvalorempenho) - coalesce(SUM(emc.vlrempenhocancelado), 0)) * 100 ) / po.prevalorobra||' >' 
					ELSE '0,00<input type=hidden id=porcentagem_'||po.preid||' value=0 >'
				END as percentual_Empenhado,
					SUM(emo.eobvalorempenho) - coalesce(SUM(emc.vlrempenhocancelado), 0) as vlr_empenhado,
				CASE WHEN SUM(emo.eobpercentualemp) = NULL
					THEN '<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPar('||po.preid||', \'p\');\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(ROUND(po.prevalorobra, 2)*tpo.ptopercentualempenho/100)||'>'
					ELSE '<input type=text id=id_'||po.preid||' value=\'0,00\' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPar('||po.preid||', \'p\');\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value=\'\'>'
				END as porcempenho,

				CASE WHEN SUM(emo.eobpercentualemp) = NULL
					THEN ROUND(ROUND(po.prevalorobra, 2) - (ROUND(po.prevalorobra, 2)*tpo.ptopercentualempenho/100), 2)
					ELSE NULL
				END as vlr_empenho,
				po.preano||'&nbsp;' as ano
         FROM obras.preobra po
         INNER JOIN workflow.documento                                   doc ON doc.docid = po.docid
         INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
         INNER JOIN par.empenhoobrapar emo ON emo.preid   = po.preid and eobstatus = 'A'
         inner join par.processoobrasparcomposicao poc on poc.preid = po.preid and poc.pocstatus = 'A' 
         inner join par.subacaoobra so on so.preid = po.preid -- and so.sobano = po.preano
         inner join par.subacao s on s.sbaid = so.sbaid and s.sbastatus = 'A'
         left join (
				select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
				from par.empenhoobrapar eb
					inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
				where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
				group by e1.empidpai, eb.preid
			) as emc on emc.preid = emo.preid and emc.empidpai = emo.empid
         {$join}
		 where
		  emo.preid in (
		  				select emo.preid
						from par.processoobraspar p
							INNER JOIN par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							INNER JOIN par.empenhoobrapar emo ON emo.empid   = e.empid and eobstatus = 'A'
							inner join obras.preobra pre on pre.preid = emo.preid
							left join (
								select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
									from par.empenhoobrapar eb
										inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
									where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
									group by e1.empidpai, eb.preid
							) as emc on emc.empidpai = e.empid and emc.preid = emo.preid
							where inuid = '".$_SESSION['par_var']['inuid']."' and proid='".$proid."'
							AND p.prostatus = 'A' and p.sisid = '".$dadosse['sisid']."'
							GROUP BY emo.preid HAVING round( sum((((eobvalorempenho - coalesce(vlrempenhocancelado, 0)) * 100) / pre.prevalorobra)))  < 99.9 -- sum(emo.eobpercentualemp) <> 100
										)

		 AND emo.empid IN (       SELECT empid FROM par.empenho emp
                 					INNER JOIN par.processoobraspar pro ON emp.empnumeroprocesso = pro.pronumeroprocesso and emp.empcodigoespecie not in ('03', '13', '02', '04') and pro.prostatus = 'A' and empstatus = 'A' 
                 					WHERE pro.proid='".$proid."')

		AND po.prestatus = 'A'

        GROUP BY po.preid, po.predescricao, tpo.ptopercentualempenho, po.preidpai, po.prevalorobra, po.preano, so.sbaid, so.sobano -- , emc.vlrempenhocancelado

        UNION ALL


        SELECT  case when po.preidpai is not null then
                	'<img src=../imagens/restricao_ico.png style=cursor:pointer; title=\"Obra Em Reformulação\">'
                else
        			'<input type=checkbox name=chk[] onclick=marcarChkObrasParEmp(this); id=chk_'||po.preid||' value='||po.preid||'>'
        		end ||'<img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra('||so.sbaid||', '||sobano||')\">' as chk,
        		po.preid,
        		
				'<a class=vizualisa_obra preid='||po.preid||' sbaid='||so.sbaid||' sobano='||so.sobano||' >'||po.preid || ' - ' || po.predescricao||'</a>' as nomedaobra,
				--po.preid || ' - ' || po.predescricao as nomedaobra,
				--sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
				po.prevalorobra as vlr,
				par.retorna_numero_empenhos_obra_par(po.preid) as empenhos,
				'0,00<input type=hidden id=porcentagem_'||po.preid||' value=0 >' as percentual_Empenhado,
				0 as vlr_empenhado,
				'<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPar('||po.preid||', \'p\');\" class=\"disabled percempenho\" readonly=readonly onfocus=\"this.select();\">' as porcempenho,
				--sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)  vlr_empenho,
				po.prevalorobra as vlr_empenho,
				sobano||'&nbsp;' as ano

			FROM par.subacaoobra so
			INNER JOIN par.subacao s ON s.sbaid = so.sbaid
			INNER JOIN par.acao a ON a.aciid = s.aciid
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
			INNER JOIN obras.preobra po ON po.preid = so.preid
			INNER JOIN obras.pretipoobra pp ON po.ptoid = pp.ptoid
			INNER JOIN workflow.documento doc ON doc.docid = po.docid
			INNER JOIN par.processoobraspar pop ON pop.inuid = p.inuid and pop.prostatus = 'A' 
			INNER JOIN obras.preitenscomposicao itc ON po.ptoid = itc.ptoid --AND itcquantidade > 0
			LEFT JOIN obras.preplanilhaorcamentaria ppo ON itc.itcid = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra tpo ON tpo.ptoid  = po.ptoid
			inner join par.processoobrasparcomposicao poc on poc.preid = po.preid and poc.proid = pop.proid and poc.pocstatus = 'A'
			{$clwhere}
			   	AND p.inuid = '".$_SESSION['par_var']['inuid']."'
				AND p.ptostatus = 'A'
				AND a.acistatus = 'A'
				AND s.sbastatus = 'A'
				AND po.prestatus = 'A'
				-- AND po.tooid='2'
				AND sobano in (2011, 2012, 2013, 2014)
				AND pop.proid = '".$proid."'
				AND pop.sisid = '".$dadosse['sisid']."'
				AND po.preid not in (select op.preid from par.empenho e 
                                          inner join par.empenhoobrapar op on op.empid = e.empid and eobstatus = 'A' and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                                      where e.empnumeroprocesso = '{$dadosse['pronumeroprocesso']}')
			GROUP BY po.preid, po.predescricao, po.prevalorobra, tpo.ptopercentualempenho, po.preidpai, sobano, so.sbaid
		) as foo";

////<img src="../imagens/consultar.gif" style="cursor:pointer;" title="Consultar histórico do documento no sistema Documenta do FNDE" border="0">
//
       if($dadosse['sisid'] == 14){
        	$sql = "select 
						chk,
						nomedaobra,
						'<center>'||(SELECT obrpercentultvistoria FROM obras2.obras WHERE preid = foo.preid AND obrstatus = 'A' AND obridpai IS NULL)::integer||' %</center>' as perc,
						vlr,
						empenhos,
						percentual_Empenhado,
						vlr_empenhado,
						porcempenho,
						--vlr_empenho,
						'<input type=text id=id_vlr_'||preid||' value=\'0,00\' 
							name=name_vlr_'||preid||' size=20 onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" 
							onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenhoObraPar('||preid||', \'v\');\" class=\"disabled vrlaempenhar\" readonly=readonly onfocus=\"this.select();\">
						<input type=\"hidden\" name=\"vlrobra_'||preid||'\" id=\"vlrobra_'||preid||'\" value=\"'||vlr||'\">
						<input type=\"hidden\" name=\"vlr_empenhado_'||preid||'\" id=\"vlr_empenhado_'||preid||'\" value=\"'||vlr_empenhado||'\">' as valoraempenhar,
						ano
					from(
	        			SELECT
			        		'<input type=checkbox name=chk[] onclick=marcarChkObrasParEmp(this); id=chk_'||po.preid||' value='||po.preid||'>'
			                ||'<img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra('||so.sbaid||', '||preano||')\">' as chk,
			                po.preid,
							po.preid || ' - ' || po.predescricao as nomedaobra,
							ROUND(po.prevalorobra, 2) as vlr,
							par.retorna_numero_empenhos_obra_par(po.preid) as empenhos,
							CASE WHEN po.prevalorobra > 0 
								THEN
									'<center>'|| ( (SUM(emo.eobvalorempenho) - coalesce(SUM(emc.vlrempenhocancelado), 0)) * 100 ) / po.prevalorobra ||
									' </center> <input type=hidden id=porcentagem_'||po.preid||' value='|| 
									( (SUM(emo.eobvalorempenho) - coalesce(SUM(emc.vlrempenhocancelado), 0)) * 100 ) / po.prevalorobra||' >' 
								ELSE
									'0,00<input type=hidden id=porcentagem_'||po.preid||' value=0 >'
							END as percentual_Empenhado,
							SUM(emo.eobvalorempenho) - coalesce(SUM(emc.vlrempenhocancelado), 0) as vlr_empenhado,
	
							CASE WHEN SUM(emo.eobpercentualemp) = NULL
								THEN 	'<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||
										' size=6 onKeyUp=\"calculaEmpenhoObraPar('||po.preid||', ''p'');\" class=\"disabled percempenho\" readonly=readonly onfocus=\"this.select();\">
										<input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(ROUND(po.prevalorobra, 2)*tpo.ptopercentualempenho/100)||'>'
								ELSE 	'<input type=text id=id_'||po.preid||' value=\'\' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPar('||po.preid||', ''p'');\" 
										class=\"disabled percempenho\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value=\'\'>'
							END  as porcempenho,
							CASE WHEN SUM(emo.eobpercentualemp) = NULL
								THEN ROUND(po.prevalorobra, 2) - (ROUND(po.prevalorobra, 2)*tpo.ptopercentualempenho/100)
								ELSE 0
							end as vlr_empenho,
							po.preano||'&nbsp;' as ano
			         	FROM obras.preobra po
					 		INNER JOIN workflow.documento doc ON doc.docid = po.docid
					        INNER JOIN obras.pretipoobra tpo ON tpo.ptoid = po.ptoid
					        INNER JOIN par.empenhoobrapar emo ON emo.preid = po.preid and eobstatus = 'A'
					        inner join par.processoobrasparcomposicao poc on poc.preid = po.preid and poc.pocstatus = 'A'
					        inner join cte.subacaoobra so on so.preid = po.preid and so.sobano = po.preano
					        left join par.subacao s on s.sbaid = so.sbaid and s.sbastatus = 'A'
					        left join (
									select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
									from par.empenhoobrapar eb
										inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
									where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
									group by e1.empidpai, eb.preid
								) as emc on emc.preid = po.preid and emc.empidpai = emo.empid
			         {$join}
					 where
					  	emo.preid in (		
	  									select emo.preid
										from par.processoobraspar p
										INNER JOIN par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
										INNER JOIN par.empenhoobrapar emo ON emo.empid   = e.empid and eobstatus = 'A'
										inner join obras.preobra pre on pre.preid = emo.preid
										left join (
											select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
											from par.empenhoobrapar eb
												inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
											where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
											group by e1.empidpai, eb.preid
										) as emc on emc.empidpai = e.empid and emc.preid = emo.preid
										where p.prostatus = 'A' and p.inuid = '".$_SESSION['par_var']['inuid']."' and proid='".$proid."'
										AND p.sisid = '".$dadosse['sisid']."'
										GROUP BY emo.preid HAVING round( sum((((eobvalorempenho - coalesce(emc.vlrempenhocancelado, 0)) * 100) / pre.prevalorobra)))  < 99.9 -- sum(emo.eobpercentualemp) <> 100
									)		
					 	AND emo.empid IN (SELECT empid FROM par.empenho emp
			                 				INNER JOIN par.processoobraspar pro ON emp.empnumeroprocesso = pro.pronumeroprocesso and emp.empcodigoespecie not in ('03', '13', '02', '04') and pro.prostatus = 'A' and empstatus = 'A'
			                 			WHERE pro.proid='".$proid."')
			         	AND po.prestatus = 'A'		
			        GROUP BY po.preid, po.predescricao, po.prevalorobra, tpo.ptopercentualempenho,po.preano, so.sbaid
			
			        UNION ALL
			
			        SELECT  '<input type=checkbox name=chk[] onclick=marcarChkObrasParEmp(this); id=chk_'||po.preid||' value='||po.preid||'>'
			        		||'<img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra('||so.sbaid||', '||sobano||')\">' as chk,
			        		po.preid,
							po.preid || ' - ' || po.predescricao as nomedaobra,
							--sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
							po.prevalorobra as vlr,
							par.retorna_numero_empenhos_obra_par(po.preid) as empenhos,
							'0,00<input type=hidden id=porcentagem_'||po.preid||' value=0 >' as percentual_Empenhado,
							0::numeric as vlr_empenhado,
							'<input type=text id=id_'||po.preid||' value='||pp.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPar('||po.preid||', \'p\');\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(ROUND(po.prevalorobra, 2)*pp.ptopercentualempenho/100)||'>'  as porcempenho,
							po.prevalorobra as vlr_empenho, 
							sobano||'&nbsp;' as ano			
						FROM par.processoobraspar pop
	                        INNER JOIN par.processoobrasparcomposicao poc on pop.proid = poc.proid and poc.pocstatus = 'A' -- AND poc.preid = po.preid 
	                        INNER JOIN obras.preobra po ON po.preid = poc.preid
	                        INNER JOIN obras.pretipoobra pp ON po.ptoid = pp.ptoid
	                        INNER JOIN obras.preitenscomposicao itc ON po.ptoid = itc.ptoid 
	                        LEFT JOIN obras.preplanilhaorcamentaria ppo ON itc.itcid = ppo.itcid AND ppo.preid = po.preid
	                        INNER JOIN workflow.documento doc ON doc.docid = po.docid
	                    	
	                        INNER JOIN cte.subacaoobra so ON so.preid = poc.preid
	                        INNER JOIN cte.subacaoindicador s ON s.sbaid = so.sbaid
	                        INNER JOIN cte.acaoindicador a ON a.aciid = s.aciid
	                        INNER JOIN cte.pontuacao p ON p.ptoid = a.ptoid
						{$clwhere}
						   	AND pop.inuid = '".$_SESSION['par_var']['inuid']."'
							AND p.ptostatus = 'A'
							AND po.prestatus = 'A'
							AND sobano in (2011, 2012, 2013,2014)
							AND pop.sisid = '".$dadosse['sisid']."'
							AND pop.proid = '".$proid."'
							AND po.preid not in (select op.preid from par.empenho e 
			                                          inner join par.empenhoobrapar op on op.empid = e.empid and eobstatus = 'A' and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
			                                      where e.empnumeroprocesso = '{$dadosse['pronumeroprocesso']}')
						GROUP BY po.preid, po.predescricao, pp.ptopercentualempenho, sobano, so.sbaid, po.prevalorobra
				) as foo";
       }
    }
  	//ver(simec_htmlentities($sql),d);
	echo "<h3>Obras a serem empenhadas</h3>";

	echo "<form id=\"formpreobras\">";

	$cabecalho = array("&nbsp;","Nome da obra", "% de Execução<br> da Obra","Valor da obra","N° dos Empenhos","% empenhado","Valor empenhado","% Empenho","Valor a empenhar","Ano");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	echo "</form>";


}

function carregarPlanoInterno( $dados ){
	global $db;

	$sql = "select distinct sd.sbdano, s.prgid from par.subacaoobra so
				inner join par.subacaodetalhe sd on sd.sbaid = so.sbaid and sd.sbdano = so.sobano
			    inner join par.subacao s on s.sbaid = sd.sbaid
			where so.preid = ".$dados['preid'];
	$dadosSub = $db->pegaLinha( $sql );

	if( $dadosSub['prgid'] ){
		$sql = "SELECT 	DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno as descricao
				FROM
					par.planointerno
				WHERE pliano = ".$dadosSub['sbdano']."  AND prgid = ".$dadosSub['prgid'];
	} else {
		$sql = array();
	}

	$planointerno = $db->pegaUm( "select distinct sd.sbdplanointerno from par.subacaoobra so
										inner join par.subacaodetalhe sd on sd.sbaid = so.sbaid and sd.sbdano = so.sobano
									where so.preid = ".$dados['preid'] );

	$db->monta_combo( "planointerno", $sql, 'S', 'Selecione', 'filtraPTRESObrasPar', '', '','','N', 'planointerno', false, $planointerno, 'Plano Interno' );
	//$db->monta_combo( "planointerno", $sql, 'S', 'Selecione', 'filtraPTRES', '', '','','N', 'planointerno', false, $planointerno, 'Plano Interno' );
}


function executarEmpenho($dados) {
	
	$obHabilita = new Habilita();
	$proid = $dados['proid'];
	
	//$cnpj 		= $obHabilita->pegaCnpj($_SESSION['par_var']['inuid']);
	$cnpj 		= pegaCnpj($_SESSION['par_var']['inuid'], $proid);
	$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj);

//	if($habilitado == 'Habilitado'){
		$res_sp = solicitarProcesso($dados);
		$res_cc = consultarContaCorrente($dados);
		if(!$res_cc) $res_sc = solicitarContaCorrente($dados);
		$res_se = solicitarEmpenho($dados);
//	} else {
//		echo $habilitado;
//	}

}


function consultarContaCorrente($dados) {
	
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

		$proseqconta = $db->pegaUm("SELECT proseqconta FROM par.processoobraspar WHERE prostatus = 'A'  and  proid='".$dados['proid']."'");

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$proseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;


		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		}

		if($proseqconta) {

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();

			$xmlRetorno = $xml;

		    $xml = simplexml_load_string( stripslashes($xml));

		    if($xml->body->row->seq_conta) {
		    	$db->executar("UPDATE par.processoobraspar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE proseqconta='".$proseqconta."'");
		    	$db->commit();
		    }

			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'consultarContaCorrente',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    $result = (integer) $xml->status->result;


		    if($result) {

		    	$codigo = (integer) $xml->body->row->co_situacao_conta;

		    	if( $codigo == 24 ){
		    		echo "MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.";
			    } elseif( $codigo == 25 ){
			    	echo "MSG SIMEC : Conta Corrente Bloqueada Definitivamente.";
			    } elseif( $codigo == 14 ){
			    	echo "MSG SIMEC : Conta Corrente Inativa.";
			    }

			    return false;

		    } else {
		    	return true;
		    }

		} else {
			return false;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

	}
}


function solicitarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $dadoscc = $db->pegaLinha("SELECT procnpj, pronumeroprocesso, probanco, proagencia, muncod, protipo, sisid FROM par.processoobraspar WHERE prostatus = 'A'  and  proid='".$dados['proid']."'");

        if($dadoscc) {
        	if( $dadoscc['sisid'] == 23 ){
        		$cnpjProcesso = pegaCnpj($_SESSION['par_var']['inuid'], $dados['proid']);
        	} else {
        		$cnpjProcesso = $dadoscc['procnpj'];
        	}
	        // numero do processo (No desenvolvimento é fixo)
        	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
        	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
        	   	//$nu_processo='23034655466200900';
        	   	$nu_processo=$dadoscc['pronumeroprocesso'];//234000005642011
        	} else {
	        	$nu_processo=$dadoscc['pronumeroprocesso'];
        	}

	        // constante=001
	        $nu_banco=$dadoscc['probanco'];
	        // esperando envio
	        $nu_agencia=$dadoscc['proagencia'];
        }

        if($cnpjProcesso){
    		$nu_identificador = $cnpjProcesso;
    	}else{
			$nu_identificador=pegaCnpj($_SESSION['par_var']['inuid'], $dados['proid']);
    	}



  		// constante=1
        $tp_identificador="1";

        // constante=nulo
        $nu_conta_corrente=null;
        // constante=01
        $tp_solicitacao="01";
        // constante=0032
        $motivo_solicitacao="0032";
        // constante=nulo
        $convenio_bb=null;
        // constante=N
        $tp_conta="N";
        // constante=5
        $nu_sistema="7";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW

        $co_programa_fnde="CM";



    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
		   $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE CONTA CORRENTE ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {

		    $db->executar("UPDATE par.processoobraspar SET proseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE proid='".$dados['proid']."'");

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'solicitarContaCorrente - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

	}
}

function consultarEmpenho($dados) {
	global $db;

	try {

		$data_created = date("c");

		/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else {*/
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];

		    $dadosemp = $db->pegaLinha("SELECT e.empprotocolo, op.proid FROM par.empenho e
											inner join par.processoobraspar op on e.empnumeroprocesso = op.pronumeroprocesso and op.prostatus = 'A' and empstatus = 'A' 
										WHERE e.empid = '".$dados['empid']."'");

	        if($dadosemp) {
	        	$nu_seq_ne = $dadosemp['empprotocolo'];
	        }


		//}

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

		/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {*/
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		//}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));
		//ver($xml,d);
		$result = (integer) $xml->status->result;

		if($result) $hwpwebservice="consultarEmpenho - Sucesso";
		else $hwpwebservice="consultarEmpenho - Erro";

		$sql = "INSERT INTO par.historicowsprocessoobrapar(
			    	proid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$dadosemp['proid']."',
			    		'".$hwpwebservice."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		/* if( (string)$xml->body->row->co_especie_empenho == '03' ){
			$situacaoEmpenho = 'CANCELADO';
		} else { */
			$situacaoEmpenho = iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento);
		//}

		if( trim($xml->body->row->data_documento) ){
			$arData_documento = $xml->body->row->data_documento;
			$arData_documento = explode('/', $arData_documento);
			if( strlen($arData_documento[0]) == 2 ){
				$data_documento = "'".formata_data_sql($xml->body->row->data_documento)."'";
			} else {
				$data_documento = "'".$xml->body->row->data_documento."'";
			}
		} else {
			$data_documento = 'null';
		}


	    echo "------ CONSULTA DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
		echo "*** Detalhes da consulta ***\n\n";
		echo "* Nº processo 		: ".(($xml->body->row->processo)?$xml->body->row->processo:"-")."\n";
		echo "* CNPJ				: ".$xml->body->row->nu_cnpj."\n";
		echo "* Valor(R$) 			: ".number_format((string)$xml->body->row->valor_ne,2,",",".")."\n";
		echo "* Data 				: ".$xml->body->row->data_documento."\n";
		echo "* Nº documento 		: ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
		echo "* Valor empenhado(R$) : ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."\n";
		echo "* Saldo pagamento(R$) : ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."\n";
		echo "* Espécie Empenho		: ".(string)$xml->body->row->co_especie_empenho."\n";
		echo "* Situação 			: ".$situacaoEmpenho."\n\n";

		$set = array();
		if( $xml->body->row->nu_cnpj ) 						$set[] = "empcnpj = '".$xml->body->row->nu_cnpj."'";
		if( $xml->body->row->processo ) 					$set[] = "empnumeroprocesso = '".$xml->body->row->processo."'";
		if( $xml->body->row->nu_seq_ne ) 					$set[] = "empprotocolo = '".$xml->body->row->nu_seq_ne."'";
		if( $xml->body->row->co_especie_empenho ) 			$set[] = "empcodigoespecie = '".$xml->body->row->co_especie_empenho."'";
		if( $xml->body->row->situacao_documento ) 			$set[] = "empsituacao = '".$situacaoEmpenho."'";
		if( $xml->body->row->numero_documento ){
			$empnumerooriginal 	= substr($xml->body->row->numero_documento, 6);
			$empanooriginal 	= substr($xml->body->row->numero_documento, 0, 4);

			$set[] = "empnumero = '".$xml->body->row->numero_documento."'";
			$set[] = "empnumerooriginal = ".($empnumerooriginal ? $empnumerooriginal : 'null');
			$set[] = "empanooriginal = ".($empanooriginal ? "'".$empanooriginal."'" : 'null');

		}
		if( trim($xml->body->row->valor_ne) ) 						$set[] = "empvalorempenho = ".$xml->body->row->valor_ne;
		if( trim($xml->body->row->ds_problema) ) 					$set[] = "ds_problema = '".$xml->body->row->ds_problema."'";
		if( trim($xml->body->row->valor_total_empenhado) )			$set[] = "valor_total_empenhado = ".$xml->body->row->valor_total_empenhado;
		if( trim($xml->body->row->valor_saldo_pagamento) )			$set[] = "valor_saldo_pagamento = ".$xml->body->row->valor_saldo_pagamento;
		if( trim($xml->body->row->data_documento) )					$set[] = "empdata = ".$data_documento;
		if( trim($xml->body->row->unidade_gestora_responsavel) )	$set[] = "empunidgestoraeminente = '".$xml->body->row->unidade_gestora_responsavel."'";
		if( trim($xml->body->row->tp_especializacao) )				$set[] = "tp_especializacao = '".$xml->body->row->tp_especializacao."'";
		if( trim($xml->body->row->co_diretoria) )					$set[] = "co_diretoria = '".$xml->body->row->co_diretoria."'";

		if($set) {
			$sql = "UPDATE par.empenho SET ".(($set)?implode(",",$set):"")."
						   WHERE empid='".$dados['empid']."'";

			$db->executar($sql);
		}

		$sql = "INSERT INTO par.historicoempenho(
           		usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado,
            	valor_saldo_pagamento)
    			VALUES ('".$_SESSION['usucpf']."',
    					'".$dados['empid']."',
    					NOW(),
    					'".$situacaoEmpenho."',
    					'".$xml->body->row->co_especie_empenho."',
    					'".$xml->body->row->ds_problema."',
    					".((strlen($xml->body->row->valor_total_empenhado))? $xml->body->row->valor_total_empenhado:"NULL").",
    					".((strlen($xml->body->row->valor_saldo_pagamento))? $xml->body->row->valor_saldo_pagamento:"NULL").");";

		$db->executar($sql);
		$db->commit();

		if($result) {
			return false;
		} else {
		   	return true;
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";


	}
}

function carregarFonteRecurso($dados){
	global $db;

	$sql = "SELECT
				fonte as codigo,
				fonte || ' - ' || dscfonte as descricao
			FROM
				financeiro.empenhopar ep
			INNER JOIN par.subacaodetalhe sd ON sd.sbdptres = ep.ptres
			WHERE
				sbdid = ".$dados['sbdid'];

	$db->monta_combo( "fonte", $sql, 'S', 'Selecione', '', '' );
}

function solicitarEmpenho($dados) {
	global $db;
	
	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	//$usuario = 'MECTIAGOT';
	$senha   = $dados['wssenha'];
	//$senha   = 'M3135689';

    $dadosse = $db->pegaLinha("SELECT procnpj, pronumeroprocesso, probanco, proagencia, muncod, protipo, sisid FROM par.processoobraspar WHERE prostatus = 'A'  and  proid='".$dados['proid']."'");
    
    if($dadosse) {
    	if( $dadosse['sisid'] == 23 ){
    		$cnpjProcesso=pegaCnpj($_SESSION['par_var']['inuid'],$dados['proid']);
    	} else {
    		$cnpjProcesso = $dadosse['procnpj'];
        }
	        // numero do processo (No desenvolvimento é fixo)
        	if($_SESSION['baselogin'] == "simec_desenvolvimento1" ||
        	   $_SESSION['baselogin'] == "simec_espelho_producao1" ){

        	   	$nu_processo='23034655466200900';
				$co_fonte_recurso_solic= $dados['fonte'];  // "0100000000";
				$co_plano_interno_solic="PFB02F52BWN";
				$co_ptres_solic="020990";
				$co_natureza_despesa_solic="44504200";



        	} else {
	        	$nu_processo=$dadosse['pronumeroprocesso'];

	        	if($_SESSION['par_var']['esfera']=='estadual') {
	        		$co_natureza_despesa_solic="44304200";
	        	}else{
	        		$co_natureza_despesa_solic="44404200";
	        	}

	        	if($dadosse['protipo'] == 'P') {
					// constante=MEC00001
					$co_plano_interno_solic="MEC00001";
					// constante=037825
					if( date("Y") == 2011 ){
						$co_ptres_solic="037825";
					} else {
						$co_ptres_solic="043990";
					}
					$frpfuncionalprogramatica="12365203012KU0001";

	        	} else {
					$co_plano_interno_solic="MEC00002";
					if( date("Y") == 2011 ){
						$co_ptres_solic="037826";
					} else {
						$co_ptres_solic="043991";
					}
					$frpfuncionalprogramatica="12368203012KV0001";
	        	}
        	}
        }

		 if($cnpjProcesso){
    		$nu_cnpj_favorecido = $cnpjProcesso;
    	 }else{
    		$nu_cnpj_favorecido=pegaCnpj($_SESSION['par_var']['inuid'], $dados['proid']);
    	 }



		// nulo
		$nu_empenho_original=null;
		// nulo
		$an_exercicio_original=null;
		// total do empenho, calculado na tela
		$vl_empenho=str_replace(array(".",","),array("","."),$dados['name_total']);
		// constante=01
		$co_especie_empenho="01";
		// constante=1
		$co_esfera_orcamentaria_solic="1";
		// constante=69500000000
		if($dados['gestaosolicitacao']){
			$co_centro_gestao_solic		= $dados['gestaosolicitacao']; 
		}else{
			if($dadosse){
				if($dadosse['sisid'] == 57){
					$co_centro_gestao_solic="61700000000";
				} else {
					$co_centro_gestao_solic="69500000000";
				}
			} else {
				$co_centro_gestao_solic="69500000000";
			}
		}
		// nulo
		$an_convenio=null;
		// nulo
		$nu_convenio=null;
		// constante=2
		$co_observacao="2";
		// constante=3
		$co_tipo_empenho="3";
		// constante=0010
		$co_descricao_empenho="0010";
		// constante=15253
		$co_gestao_emitente="15253";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        if($dadosse['protipo'] == 'P') $co_programa_fnde="BW";
        else $co_programa_fnde="CN";
		// constante=153173
		$co_unidade_gestora_emitente="153173";
		// constante=26298
		$co_unidade_orcamentaria_solic="26298";
		// nulo
		$nu_proposta_siconv=null;
		// constante=5
		$nu_sistema="7";

		$co_natureza_despesa_solic  = $dados['naturezadespesasolicitacao'];
		$co_fonte_recurso_solic		= $dados['fonte'];
		$co_ptres_solic				= $dados['ptres'];
		$co_plano_interno_solic		= $dados['planointerno'];
		
		$co_programa_fnde			= "CM";
		$an_convenio=null;
		$nu_convenio=null;
		$co_tipo_empenho			= "3";
		$co_especie_empenho			= "01";
		$co_esfera_orcamentaria_solic = "1";
		$co_observacao = "2";
		$co_descricao_empenho = "0010";
		$co_gestao_emitente = "15253";
		$co_unidade_gestora_emitente = "153173";
		$co_unidade_orcamentaria_solic = "26298";
		$nu_proposta_siconv = null;
		//$co_centro_gestao_solic="61400000000";
		
		if( $dados['tipo'] != 'visualiza' ){
			$sql = "SELECT distinct l.lwsid FROM par.logws l
					    inner join par.historicowsprocessoobrapar h ON l.lwsid = h.lwsid
					WHERE
					    h.proid = {$dados['proid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessoobrapar where proid = {$dados['proid']})
						and l.lwstiporequest = '$co_especie_empenho'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
						'lwstiporequest' 	=> $co_especie_empenho,
		        		'usucpf' 			=> $_SESSION['usucpf']
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}
		}

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

	if( $dados['tipo'] == 'visualiza' ){
		echo '<pre>';
		echo simec_htmlentities($arqXml);
		exit;
    }

	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	}
	
	$arrParam = array(
			'lwsrequestdata' 	=> 'now()',
			'lwsurl' 			=> $urlWS,
			'lwsmetodo' 		=> 'solicitar',
			'lwsid' 			=> $request_id
	);
	logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
	
	$arrParam = array(
			'proid' 		=> $dados['proid'],
			'lwsid' 		=> $request_id,
			'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
			'hwpdataenvio' 	=> 'now()',
			'usucpf' 		=> $_SESSION['usucpf']
	);
	$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'insert' );
		
	try {
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;
		
		$arrParam = array(
				'hwpid'			=> $hwpid,
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
		
		
		$arrParam = array(
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
			$arrParam = array(
					'lwserro' => true,
					'lwsid' => $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarEmpenho - Erro'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
			
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
		    return false;
		} else {
			
			$arrParam = array(
					'lwserro' => false,
					'lwsid' => $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarEmpenho - Sucesso'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );

			$sql = "INSERT INTO par.empenho(
            empcnpj,
            empnumerooriginal,
            empanooriginal,
            empvalorempenho,
            empnumeroprocesso,
            empcodigoespecie,
            empcodigopi,
            empcodigoesfera,
            empcodigoptres,
            empfonterecurso,
            empcodigonatdespesa,
            empcentrogestaosolic,
            empanoconvenio,
            empnumeroconvenio,
            empcodigoobs,
            empcodigotipo,
            empdescricao,
            empgestaoeminente,
            empunidgestoraeminente,
            empprogramafnde,
            empnumerosistema,
            usucpf,
            empprotocolo,
            empsituacao
            )
		    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
		    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
		    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
		    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
		            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
		            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
		            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
		            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
		            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
		            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
		            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
		            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
		            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
		            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
		            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
		            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
		            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
		            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
		            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
		            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
		            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
		            '".$_SESSION['usucpf']."',
		            '".$xml->body->nu_seq_ne."',
		            '8 - SOLICITAÇÃO APROVADA'
		            ) RETURNING empid;";

			$empid = $db->pegaUm($sql);

			$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);
			$db->commit();

			if($dados['chk']) {
				foreach($dados['chk'] as $preid)
				{
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp, eobvalorempenho, eobpercentualemp2)
    						VALUES ($preid, $empid, ".str_replace(',', '.', $dados['name_'.$preid]).", ".retiraPontosBD($dados['name_vlr_'.$preid]).", ".round($dados['name_'.$preid]).");";

					$db->executar($sql);
					$db->commit();
				}
				foreach($dados['chk'] as $preid)
				{
					$sql = "SELECT obr.obrid, obrstatus FROM obras.preobra pre
							LEFT JOIN obras2.obras obr ON obr.obrid = pre.obrid
							WHERE pre.preid = $preid";
					$obra = $db->pegaLinha($sql);
					
					if( $obra['obrstatus'] == 'I' ){
						
						$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empid";
						$empnumero = $db->pegaUm($sql);
							
						insereHistoricoStatusObra( $empid, Array( $preid ), 'A', "Obra reativada pelo empenho $empnumero" );
					
					}elseif( $obra['obrid'] == '' ){
						
						$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empid";
						$empnumero = $db->pegaUm($sql);
							
						insereHistoricoStatusObra( $empid, Array( $preid ), 'A', "Obra importada pelo empenho $empnumero" );
					}
					
					$obrid_1 = importarObrasPar( $preid );
// 					importarObras2Par( $preid, $obrid_1 );
					$preObra = new PreObra( $preid );
					$obrid = $preObra->importarPreobraParaObras2( $preid );
				}
			}

			$db->commit();
			
			cargaViewEmpenhoObras( $nu_processo );

			return true;
		}

	} catch (Exception $e){
		$arrParam = array(
				'lwserro' => true,
				'lwsid' => $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'solicitarEmpenho - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
	}
}

function importarObras2Par( $preid, $obrid_1 = null ){
	global $db;
	/*** INICIO - Importação dos dados para o sistema de Obras - INICIO ***/
	$obrid_1 = $obrid_1 ? $obrid_1 : 'NULL';
	/*** Só executa a importação caso a obra não exista ***/
	$esdid = $db->pegaUm("SELECT
								d.esdid
							FROM
								par.subacao s
							INNER JOIN par.subacaoobra so ON s.sbaid = so.sbaid
							INNER JOIN workflow.documento d ON d.docid = s.docid
							WHERE
								so.preid = ".$preid);

	if( $esdid != WF_SUBACAO_DILIGENCIA_CONDICIONAL && $esdid != WF_SUBACAO_APROVACAO_CONDICIONAL ){ // Se for condicional não importam os dados da obra (a pedido do Thiago no dia 01/08/2013)

		$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
		$existeObra = $db->pegaUm($sql);
		
		if( (integer)$existeObra < 1 ){
			/*** Recupera dados da Pre Obra ***/
			$sql = "SELECT DISTINCT
						p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
						--ent.entid as unidade_implantadora,
						p.precep,
						p.prelogradouro,
						p.precomplemento,
						p.prebairro,
						p.muncod,
						p.estuf,
						p.prenumero,
						p.prelatitude,
						p.prelongitude,
						CASE WHEN iu.itrid = 1 THEN 'E' ELSE 'M' END as preesfera,
						pto.tpoid as tipologiaobra,
						/*CASE
							WHEN pop.sisid = 23 THEN 39 --PAR
							ELSE 42 --Emenda Parlamentar
						END AS programa,*/
						CASE
							WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
							WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
							WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
							ELSE 54 --OUTROS
		                END as programa,
						CASE
							WHEN i.indcod in (3,4,7,8) THEN 1
							WHEN i.indcod in (5,6,10,9) THEN 2
						END AS modalidadedeensino,
						CASE
							WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
							WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
							ELSE 1 --CONSTRUÇÃO
						END AS tipodeobra,
						CASE
							WHEN sisid = 23 THEN 11 -- TD
							WHEN sisid = 57 THEN 4 -- EMENDAS
							ELSE 11 -- TD
						END AS fonte,
						CASE
							WHEN s.ppsid in (652,695,810,896,897,965,966,971,972,977,983,987,989,1015,1016,1091,1093,1099,1104,1105,1115,1116,1119,1120,1122,1124) THEN 1 --RURAL
							WHEN s.ppsid in (494,495,521,555,568,577,624,633,671,676,698,718,783,802,867,882,957,958,961,962,963,964,981,982,1013,1014,1088,1089,1090,1098,1111,1112,1117,1118,1121,1123,1153,1154,1158,1159) THEN 2 --URBANA
							WHEN s.ppsid in (605,655,854,900,901,969,970,975,976,980,986,988,990,1094) THEN 3 --QUILOMBOLA
							WHEN s.ppsid in (542,710,768,801,898,899,967,968,973,974,978,979,984,985) THEN 4 --INDÍGENA
						ELSE 2 --URBANA
						END AS classificacaoobra,
						prevalorobra as valorobra,
						pop.pronumeroprocesso
					FROM
						par.dimensao d
						INNER JOIN par.area                 a ON a.dimid = d.dimid
						INNER JOIN par.indicador          i ON i.areid = a.areid
						INNER JOIN par.criterio             c ON c.indid = i.indid
						INNER JOIN par.pontuacao        po ON c.crtid = po.crtid AND ptostatus = 'A'
						INNER JOIN par.instrumentounidade iu ON iu.inuid = po.inuid
						INNER JOIN par.acao                ac ON ac.ptoid = po.ptoid AND ac.acistatus = 'A'
						INNER JOIN par.subacao                      s ON s.aciid = ac.aciid AND s.sbastatus = 'A'
						INNER JOIN par.subacaoobra    so  ON so.sbaid = s.sbaid
						INNER JOIN obras.preobra         p  ON so.preid = p.preid
						LEFT JOIN obras.pretipoobra pto ON pto.ptoid = p.ptoid
						INNER JOIN par.processoobraspar pop ON pop.inuid = iu.inuid and pop.prostatus = 'A' 
						INNER JOIN par.processoobrasparcomposicao poc ON poc.proid = pop.proid and poc.preid = p.preid and poc.pocstatus = 'A'
						INNER JOIN territorios.municipio mun on p.muncod = mun.muncod
						--INNER JOIN entidade.endereco ende ON ende.muncod = p.muncod
						--INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
						--INNER JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
					WHERE
						p.preid = ".$preid;

			$dadosPreObra = $db->carregar($sql);
			
			//DEFINDO A ENTIDADE
			if($dadosPreObra[0]['preesfera']=='M'){
				$sql = "SELECT ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid
						WHERE ent.entstatus = 'A'
						AND fue.funid IN (1)
						AND fue.fuestatus = 'A'
						AND ed.muncod = '".$dadosPreObra[0]['muncod']."'";
				$unidade_implantadora = $db->pegaUm($sql);
			}else{
				$sql = "SELECT ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid
						WHERE ent.entstatus = 'A'
						AND fue.funid IN (6)
						AND fue.fuestatus = 'A'
						AND ed.estuf = '".$dadosPreObra[0]['estuf']."'";
				$unidade_implantadora = $db->pegaUm($sql);
			}

			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO
						entidade.endereco (tpeid,
										   endcep,
										   endlog,
										   endcom,
										   endbai,
										   muncod,
										   estuf,
										   endnum,
										   medlatitude,
										   medlongitude,
										   endstatus)
						VALUES
							( 4,
							  '".$dadosPreObra[0]['precep']."',
							  '".$dadosPreObra[0]['prelogradouro']."',
							  '".$dadosPreObra[0]['precomplemento']."',
							  '".substr(removerEspacoDuplicado($dadosPreObra[0]['prebairro']),0,100)."',
							  '".$dadosPreObra[0]['muncod']."',
							  '".$dadosPreObra[0]['estuf']."',
							  '".$dadosPreObra[0]['prenumero']."',
							  '".$dadosPreObra[0]['prelatitude']."',
							  '".$dadosPreObra[0]['prelongitude']."',
							  'A' ) RETURNING endid";

			$endid = $db->pegaUm($sql);
			$db->commit();
			
			/*** Insere a nova obra ***/
			$sql = "INSERT INTO obras2.empreendimento(
				            orgid,
				            moeid,
				            empesfera,
				            tpoid,
				            prfid,
				            tobid,
				            tooid,
				            cloid,
				            entidunidade,
				            empdsc,
				            empvalorprevisto,
				            endid,
				            preid,
				            obrid_1
					) VALUES (
							3,
							".($dadosPreObra[0]['modalidadedeensino'] ? "'".$dadosPreObra[0]['modalidadedeensino']."'" : 'null').",
							'".$dadosPreObra[0]['preesfera']."',
							" . ($dadosPreObra[0]['tipologiaobra'] ? $dadosPreObra[0]['tipologiaobra'] : 'NULL') . ",
							'".$dadosPreObra[0]['programa']."',
							'".$dadosPreObra[0]['tipodeobra']."',
							'".$dadosPreObra[0]['fonte']."',
							'".$dadosPreObra[0]['classificacaoobra']."',
				            ".$unidade_implantadora.",
				            '".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
				            '".$dadosPreObra[0]['valorobra']."',
				            $endid,
				            '".$preid."',
				            ".$obrid_1.") RETURNING empid;";

			$empid = $db->pegaUm( $sql );

			/*** Insere a nova obra ***/
			$sql = "INSERT INTO obras2.obras(
						obrnome,
						entid,
						tooid,
						preid,
						endid,
						tpoid,
						tobid,
						cloid,
						obrvalorprevisto,
						empid,
						obrid_1)
					VALUES('".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
							".$unidade_implantadora.",
							'".$dadosPreObra[0]['fonte']."',
							'".$preid."',
							'".$endid."',
							" . ($dadosPreObra[0]['tipologiaobra'] ? $dadosPreObra[0]['tipologiaobra'] : 'NULL') .",
							'".$dadosPreObra[0]['tipodeobra']."',
							'".$dadosPreObra[0]['classificacaoobra']."',
							'".$dadosPreObra[0]['valorobra']."',
							'" . $empid . "',
							" . $obrid_1 . ")
					RETURNING obrid";
			$obrid = $db->pegaUm($sql);

			/*
			 * Cria Documento WF - Início
			 */
			require_once APPRAIZ . 'includes/workflow.php';
			$docdsc = "Fluxo de obra do módulo Obras II - obrid " . $obrid;
			if($dadosPreObra[0]['tipologiaobra']==OBR_TPOID_MI_TIPO_B || $dadosPreObra[0]['tipologiaobra']==OBR_TPOID_MI_TIPO_C){
				$docid = wf_cadastrarDocumento(TPDID_OBJETO, $docdsc, OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO);
				$sql = "UPDATE obras2.obras SET docid={$docid} WHERE obrid={$obrid}";
				$db->executar($sql);
			}else{
				$docid = wf_cadastrarDocumento(TPDID_OBJETO, $docdsc);
				$sql = "UPDATE obras2.obras SET docid={$docid} WHERE obrid={$obrid}";
				$db->executar($sql);
			}
			/*
			 * Cria Documento WF - Fim
			 */

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN
						obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN
						obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
					AND
						(substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);

			if( $fotosTerreno )
			{
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto)
				{
					$sql = "INSERT INTO obras2.obras_arquivos(
					            obrid,
					            tpaid,
					            arqid,
					            oardata,
					            oardtinclusao
							)VALUES (
								".$obrid.",
								21,
								".$foto['arqid'].",
								NOW(),
								NOW()
							);";

					$db->executar($sql);
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN
						public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos ){
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$sql = "INSERT INTO obras2.obras_arquivos(
					            obrid,
					            tpaid,
					            arqid,
					            oardata,
					            oardtinclusao
							)VALUES (
								".$obrid.",
								21,
								".$anexo['arqid'].",
								NOW(),
								NOW()
							);";

					$db->executar($sql);
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid = ".$obrid." WHERE preid = ".$preid;
			$db->executar($sql);
			$db->commit();
			
		}else{
			$sql = "SELECT obrid FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
			$obrid = $db->pegaUm($sql);
			
			$sql = "UPDATE obras2.obras SET obrstatus = 'A' WHERE obrid = $obrid";
			$db->executar($sql);
			$db->commit();
		}
		/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
	}
}

function importarObrasPar( $preid ){
	global $db;
	/*** INICIO - Importação dos dados para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra não exista ***/

	$esdid = $db->pegaUm("SELECT
								d.esdid
							FROM
								par.subacao s
							INNER JOIN par.subacaoobra so ON s.sbaid = so.sbaid
							INNER JOIN workflow.documento d ON d.docid = s.docid
							WHERE
								so.preid = ".$preid);

	if( $esdid != WF_SUBACAO_DILIGENCIA_CONDICIONAL && $esdid != WF_SUBACAO_APROVACAO_CONDICIONAL ){ // Se for condicional não importam os dados da obra (a pedido do Thiago no dia 01/08/2013)

		$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid_1 is not null";
		$existeObra = $db->pegaUm($sql);

		if( (integer)$existeObra < 1 ){
			/*** Recupera dados da Pre Obra ***/
			$sql = "SELECT DISTINCT
						p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
						--ent.entid as unidade_implantadora,
						p.precep,
						p.prelogradouro,
						p.precomplemento,
						p.prebairro,
						p.muncod,
						p.estuf,
						p.prenumero,
						p.prelatitude,
						p.prelongitude,
						CASE WHEN iu.itrid = 1 THEN 'E' ELSE 'M' END as preesfera,
						pto.tpoid as tipologiaobra,
						/*CASE
							WHEN pop.sisid = 23 THEN 39 --PAR
							ELSE 42 --Emenda Parlamentar
						END AS programa,*/
						CASE
							WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
							WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
							WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
							ELSE 54 --OUTROS
		                END as programa,
						CASE
							WHEN i.indcod in (3,4,7,8) THEN 1
							WHEN i.indcod in (5,6,10,9) THEN 2
						END AS modalidadedeensino,
						CASE
							WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
							WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
							ELSE 1 --CONSTRUÇÃO
						END AS tipodeobra,
						CASE
							WHEN sisid = 23 THEN 11 -- TD
							WHEN sisid = 57 THEN 4 -- EMENDAS
							ELSE 11 -- TD
						END AS fonte,
						CASE
							WHEN s.ppsid in (652,695,810,896,897,965,966,971,972,977,983,987,989,1015,1016,1091,1093,1099,1104,1105,1115,1116,1119,1120,1122,1124) THEN 1 --RURAL
							WHEN s.ppsid in (494,495,521,555,568,577,624,633,671,676,698,718,783,802,867,882,957,958,961,962,963,964,981,982,1013,1014,1088,1089,1090,1098,1111,1112,1117,1118,1121,1123,1153,1154,1158,1159) THEN 2 --URBANA
							WHEN s.ppsid in (605,655,854,900,901,969,970,975,976,980,986,988,990,1094) THEN 3 --QUILOMBOLA
							WHEN s.ppsid in (542,710,768,801,898,899,967,968,973,974,978,979,984,985) THEN 4 --INDÍGENA
						ELSE 2 --URBANA
						END AS classificacaoobra,
						prevalorobra as valorobra
					FROM
						par.dimensao d
						INNER JOIN par.area                 a ON a.dimid = d.dimid
						INNER JOIN par.indicador          i ON i.areid = a.areid
						INNER JOIN par.criterio             c ON c.indid = i.indid
						INNER JOIN par.pontuacao        po ON c.crtid = po.crtid AND ptostatus = 'A'
						INNER JOIN par.instrumentounidade iu ON iu.inuid = po.inuid
						INNER JOIN par.acao                ac ON ac.ptoid = po.ptoid AND ac.acistatus = 'A'
						INNER JOIN par.subacao                      s ON s.aciid = ac.aciid AND s.sbastatus = 'A'
						INNER JOIN par.subacaoobra    so  ON so.sbaid = s.sbaid
						INNER JOIN obras.preobra         p  ON so.preid = p.preid
						LEFT JOIN obras.pretipoobra pto ON pto.ptoid = p.ptoid
						INNER JOIN par.processoobraspar pop ON pop.inuid = iu.inuid and pop.prostatus = 'A' 
						INNER JOIN par.processoobrasparcomposicao poc ON poc.proid = pop.proid and poc.preid = p.preid and poc.pocstatus = 'A'
						INNER JOIN territorios.municipio mun on p.muncod = mun.muncod
						--INNER JOIN entidade.endereco ende ON ende.muncod = p.muncod
						--INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
						--INNER JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
					WHERE
						p.preid = ".$preid;

			$dadosPreObra = $db->carregar($sql);

			//DEFINDO A ENTIDADE
			if($dadosPreObra[0]['preesfera']=='M'){
				$sql = "SELECT ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid
						WHERE ent.entstatus = 'A'
						AND fue.funid IN (1)
						AND fue.fuestatus = 'A'
						AND ed.muncod = '".$dadosPreObra[0]['muncod']."'";
				$unidade_implantadora = $db->pegaUm($sql);
			}else{
				$sql = "SELECT ent.entid
						FROM entidade.entidade ent
						INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
						INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid
						WHERE ent.entstatus = 'A'
						AND fue.funid IN (6)
						AND fue.fuestatus = 'A'
						AND ed.estuf = '".$dadosPreObra[0]['estuf']."'";
				$unidade_implantadora = $db->pegaUm($sql);
			}

			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO
						entidade.endereco (endcep,
										   endlog,
										   endcom,
										   endbai,
										   muncod,
										   estuf,
										   endnum,
										   medlatitude,
										   medlongitude,
										   endstatus)

					VALUES
						( '".$dadosPreObra[0]['precep']."',
						  '".$dadosPreObra[0]['prelogradouro']."',
						  '".$dadosPreObra[0]['precomplemento']."',
						  '".substr(removerEspacoDuplicado($dadosPreObra[0]['prebairro']),0,100)."',
						  '".$dadosPreObra[0]['muncod']."',
						  '".$dadosPreObra[0]['estuf']."',
						  '".$dadosPreObra[0]['prenumero']."',
						  '".$dadosPreObra[0]['prelatitude']."',
						  '".$dadosPreObra[0]['prelongitude']."',
						  'A' ) RETURNING endid";

			$endid = $db->pegaUm($sql);

			/* 04/07/2013 (a publicação será alguns dias depois)
			 * A pedido do Thiaguinho a obra no obras1 será inserida inativa (I), pois a partir de então utilzaremos o obras2;
			*/
			/*** Insere a nova obra ***/

			$sql = "INSERT INTO
						obras.obrainfraestrutura(
						obrdesc, entidunidade, orgid, tooid, preid, endid, obrtipoesfera, tpoid, prfid,
						moeid, tobraid, cloid, obrvalorprevisto, obsstatus)
					VALUES('".addslashes($dadosPreObra[0]['nome_obra'])."',
							".$unidade_implantadora.",
							3,
							'".$dadosPreObra[0]['fonte']."',
							".$preid.",
							".$endid.",
							'".$dadosPreObra[0]['preesfera']."',
							".($dadosPreObra[0]['tipologiaobra'] ? $dadosPreObra[0]['tipologiaobra'] : 'NULL').",
							'".$dadosPreObra[0]['programa']."',
							".($dadosPreObra[0]['modalidadedeensino'] ? $dadosPreObra[0]['modalidadedeensino'] : 'NULL').",
							'".$dadosPreObra[0]['tipodeobra']."',
							'".$dadosPreObra[0]['classificacaoobra']."',
							'".$dadosPreObra[0]['valorobra']."',
							'I')
					RETURNING obrid";
			$obrid = $db->pegaUm($sql);
			$db->commit();

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN
						obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN
						obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
					AND
						(substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);

			if( $fotosTerreno )
			{
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto)
				{
					$sql = "INSERT INTO
							obras.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
							VALUES
							(".$obrid.", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
					$db->executar($sql);
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN
						public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos ){
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$sql = "INSERT INTO
							obras.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
							VALUES
							(".$obrid.", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
					$db->executar($sql);
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid_1 = ".$obrid." WHERE preid = ".$preid;
			$db->executar($sql);
		}  else {
			$obrid = $db->pegaUm("SELECT obrid_1 FROM obras.preobra WHERE preid = ".$preid);
		}
		/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
	}
	
	$db->commit();
	return $obrid;
}


function cancelarEmpenho($dados) {
	
	global $db;
	
	header('content-type: text/html; charset=ISO-8859-1');
	
	try {
		/*
		// validando se tem termo
		$sql = "SELECT te.terid FROM par.empenhoobrapar o
				INNER JOIN par.termoobra t ON t.preid = o.preid and eobstatus = 'A'
				INNER JOIN par.termocompromissopac te ON te.terid = t.terid
				WHERE o.empid='".$dados['empid']."' AND te.terassinado=TRUE";

		$existe_termo = $db->pegaUm($sql);

		if($existe_termo) {
	    	echo "------ EMPENHO NÃO PODE SER CANCELADO ------\n\n";
	    	echo "Obras existentes neste empenho pertencem a um termo de compromisso";
	    	exit;
		}
		*/

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		$senha   = $dados['wssenha'];

		$dadosemp = $db->pegaLinha("SELECT e.empnumeroprocesso, e.empprogramafnde, e.empcnpj, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, e.empprotocolo, e.empcentrogestaosolic, op.proid, e.empfonterecurso, e.empcodigopi, e.empcodigoptres
			 							FROM par.empenho e
											inner join par.processoobraspar op on e.empnumeroprocesso = op.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and op.prostatus = 'A' and empstatus = 'A'
											inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
										WHERE e.empid = '".$dados['empid']."'");

		if($dadosemp) {
        	$nu_seq_ne = $dadosemp['empprotocolo'];
        }        
        $nu_processo=$dadosemp['empnumeroprocesso'];
        
        if($_SESSION['par_var']['esfera']=='estadual') {
        	$co_natureza_despesa_solic="44304200";
        }else{
        	$co_natureza_despesa_solic="44404200";
        }
        
        if($dadosemp['empprogramafnde'] == 'BW') {
        	$co_plano_interno_solic="MEC00001";
        	if( date("Y") == 2011 ){
        		$co_ptres_solic="037825";
        	} else {
        		$co_ptres_solic="043990";
        	}
        	$frpfuncionalprogramatica="12365203012KU0001";
        } else {
        	$co_plano_interno_solic="MEC00002";
        	if( date("Y") == 2011 ){
        		$co_ptres_solic="037826";
        	} else {
        		$co_ptres_solic="043991";
        	}
        	$frpfuncionalprogramatica="12368203012KV0001";
        }
        
        $nu_cnpj_favorecido=pegaCnpj($_SESSION['par_var']['inuid'], $dadosemp['proid']);
        
        if($dadosemp['empnumero']) {
        	$arrNumero = explode("NE",$dadosemp['empnumero']);
        	$nu_empenho_original=$arrNumero[1];
        	$an_exercicio_original=$arrNumero[0];
        
        	#A pedido da Sâmara via e-mail dia 06/11/2013, que alterasse a espécie empenho de 03 para 13
        	if( $an_exercicio_original == date('Y') ){
        		$co_especie_empenho="03";
        	} else {
        		$co_especie_empenho="13";
        	}
        } else {
        	$nu_empenho_original=null;
        	$an_exercicio_original=null;
        	$co_especie_empenho="13";
        }
        
        $vl_empenho=$dadosemp['empvalorempenho'];
        $co_esfera_orcamentaria_solic="1";
        $co_centro_gestao_solic= $dadosemp['empcentrogestaosolic'];
        $an_convenio=null;
        $nu_convenio=null;
        $co_observacao="2";
        $co_tipo_empenho="3";
        $co_descricao_empenho="0011";
        $co_gestao_emitente="15253";
        $co_programa_fnde=$dadosemp['empprogramafnde'];
        $co_unidade_gestora_emitente="153173";
        $co_unidade_orcamentaria_solic="26298";
        $nu_proposta_siconv=null;
        $nu_sistema="7";
        $co_fonte_recurso_solic=$dadosemp['empfonterecurso'];
        $co_plano_interno_solic= $dadosemp['empcodigopi'];
        $co_ptres_solic = $dadosemp['empcodigoptres'];

	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

        $errodecancelamento = true;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

		if($result) {
			$status 	= (string)$xml->body->status;
			$co_status	= substr( $status, 0, 1 );

			$errodecancelamento = false;
			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dadosemp['proid']."',
				    		'cancelarEmpenho - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			if( trim($co_status) == 0 ){
				$errodecancelamento = true;
			} else {
				echo "------ CANCELAMENTO DE EMPENHO ------\n\n";
				echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
				
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
				            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
				            empprogramafnde, empnumerosistema, usucpf, empprotocolo, empidpai, empsituacao)
					    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
					            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
					            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
					            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
					            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
					            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
					            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
					            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
					            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
					            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
					            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
					            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
					            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
					            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
					            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
					            '".$_SESSION['usucpf']."',
					            '123123',
					            '".$dados['empid']."',
					            '2 - EFETIVADO'
					            ) RETURNING empid;";
					
				$empid = $db->pegaUm($sql);
					
				$sql = "SELECT eobid, preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp, eobstatus 
						FROM par.empenhoobrapar WHERE empid = {$dados['empid']}";
				$arrEmpSub = $db->carregar($sql);
				$arrEmpSub = $arrEmpSub ? $arrEmpSub : array();
					
				foreach ($arrEmpSub as $v) {
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ('".$v['preid']."', '".$empid."', '".$v['eobpercentualemp2']."', '".$v['eobvalorempenho']."', '".$v['eobpercentualemp']."');";
					$db->executar($sql);
				}
				
				$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empid";
				$empnumero = $db->pegaUm($sql);
					
				insereHistoricoStatusObra( $empid, Array(), 'I', "Obra inativada pelo cancelamento de empenho $empnumero" );
				
				inativaObras2SemSaldoEmpenho( $empid, Array() );
					
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    					VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'CANCELADO SIGEF');";
				$db->executar($sql);
				$db->commit();
				
				cargaViewEmpenhoObras( $nu_processo );
				
				echo 'cancelado com sucesso';
			}

		} else {
			$errodecancelamento = true;

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dadosemp['proid']."',
				    		'cancelarEmpenho - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
		}
		
		if($errodecancelamento) { 
			
			$sql = "SELECT distinct l.lwsid FROM par.logws l
					    inner join par.historicowsprocessoobrapar h ON l.lwsid = h.lwsid
					WHERE
					    h.proid = {$dadosemp['proid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessoobrapar where proid = {$dadosemp['proid']})
						and l.lwstiporequest = '$co_especie_empenho'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
						'lwstiporequest' 	=> $co_especie_empenho,
		        		'usucpf' 			=> $_SESSION['usucpf']
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			}
			
			$arrParam = array(
					'lwsrequestdata'	=> 'now()',
					'lwsurl' 			=> $urlWS,
					'lwsmetodo' 		=> 'solicitar',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
				
			$arrParam = array(
					'proid' 		=> $dadosemp['proid'],
					'lwsid' 		=> $request_id,
					'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
					'hwpdataenvio' 	=> 'now()',
					'usucpf' 		=> $_SESSION['usucpf']
			);
			$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'insert' );

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();

			$xmlRetorno = $xml;
			
			$arrParam = array(
					'hwpid'			=> $hwpid,
					'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
				
			$arrParam = array(
					'lwsresponsedata' 	=> 'now()',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		    $xml = simplexml_load_string( stripslashes($xml));

		    echo "------ SOLICITAÇÃO DE EMPENHO (ANULAÇÃO) ------\n\n";
			$result = (integer) $xml->status->result;

			if($result) {
				echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
				$hwpwebservice = "anularEmpenho - Sucesso";
// 				$db->executar("UPDATE par.empenho SET empsituacao = 'CANCELADO' WHERE empid = '".$dados['empid']."'");

// 				$db->executar("INSERT INTO par.historicoempenho( usucpf, empid, hepdata, co_especie_empenho, empsituacao)
// 	    					   VALUES ('".$_SESSION['usucpf']."', '".$dados['empid']."', NOW(), '$co_especie_empenho', 'CANCELADO');");

// 				$db->executar("UPDATE par.empenhoobrapar set eobstatus = 'I' WHERE empid = '".$dados['empid']."'");
// 				$db->commit();

				$arrParam = array(
						'lwserro' => false,
						'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'cancelarEmpenho2 - Sucesso'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );

				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
				            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
				            empprogramafnde, empnumerosistema, usucpf, empprotocolo, empidpai, empsituacao)
					    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
					            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
					            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
					            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
					            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
					            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
					            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
					            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
					            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
					            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
					            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
					            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
					            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
					            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
					            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
					            '".$_SESSION['usucpf']."',
					            '".$xml->body->nu_seq_ne."',
					            '".$dados['empid']."',
					            '2 - EFETIVADO'
					            ) RETURNING empid;";
					
				$empid = $db->pegaUm($sql);
					
				$sql = "SELECT eobid, preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp, eobstatus
						FROM par.empenhoobrapar WHERE empid = {$dados['empid']}";
				$arrEmpSub = $db->carregar($sql);
				$arrEmpSub = $arrEmpSub ? $arrEmpSub : array();
							
				foreach ($arrEmpSub as $v) {
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ('".$v['preid']."', '".$empid."', '".$v['eobpercentualemp2']."', '".$v['eobvalorempenho']."', '".$v['eobpercentualemp']."');";
					$db->executar($sql);
				}
				
				$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empid";
				$empnumero = $db->pegaUm($sql);
					
				insereHistoricoStatusObra( $empid, Array(), 'I', "Obra inativada pelo cancelamento de empenho $empnumero" );
				
				inativaObras2SemSaldoEmpenho( $empid, Array() );
						
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    					VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'CANCELADO SIGEF\SIAFI');";
				$db->executar($sql);
				$db->commit();
				cargaViewEmpenhoObras( $nu_processo );
			}else{
				echo "*** Descrição do erro ***\n\n";
				$erros = $xml->status->error->message;

				if(count($erros)>0) {
					foreach($erros as $err) {
						echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					}
				}
				$arrParam = array(
					'lwserro' => true,
					'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
				
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'cancelarEmpenho2 - Erro'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
			}
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Cancelar Empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Cancelar Empenho no SIGEF: $erroMSG";


	}
}


function solicitarProcesso($dados) {
	global $db;

    $dadosse = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo, procnpj, sisid
    						   FROM par.processoobraspar
    						   WHERE prostatus = 'A'  and proid='".$dados['proid']."'");

    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['pronumeroprocesso'];
    	$tp_processo=1;
    	$co_programa_fnde= "CM";
    	if( $dadosse['sisid'] == 23 ){
    		$cnpjProcesso=pegaCnpj($_SESSION['par_var']['inuid'], $dados['proid']);
    	} else {
    		$cnpjProcesso = $dadosse['procnpj'];
    	}

    }

    if($cnpjProcesso){
    	$nu_cnpj_favorecido = $cnpjProcesso;
    }else{
    	$nu_cnpj_favorecido=pegaCnpj($_SESSION['par_var']['inuid'], $dados['proid'] );
    }

	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
      	<nu_processo>$nu_processo</nu_processo>
      	<tp_processo>$tp_processo</tp_processo>
      	<an_processo>$an_processo</an_processo>
      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;


		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE PROCESSO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					echo "\n";
				}
			}

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'solicitarProcesso - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'solicitarProcesso - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

}

?>