<?php

function listaHistoricoEmpenhoFilhos($dados) {
	global $db;
	$sql = "SELECT DISTINCT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/refresh2.gif style=cursor:pointer; title=\"Consultar Empenho\" onclick=consultarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_consultar,
				   e.empnumero,
				   vve.vrlempenhocancelado as empvalorempenho,
				   u.usunome,
				   tee.teedescricao as tipo
			FROM par.empenho e
			INNER JOIN par.processopar p ON trim(e.empnumeroprocesso) = trim(p.prpnumeroprocesso) and empcodigoespecie not in ('03', '13', '02', '04') and p.prpstatus = 'A' and empstatus = 'A'
			inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
			INNER JOIN execucaofinanceira.tipoespecieempenho tee ON tee.teecodigo = e.empcodigoespecie
			LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
			WHERE empstatus <> 'I' AND empnumerooriginalpai='".$dados['empnumerooriginalpai']."'";
	$cabecalho = array("&nbsp;", "Consultar empenho", "Nº empenho","Valor empenho(R$)","Efetivado por","Tipo de Empenho");
/*
	$arrHist = $db->carregar($sql);

	$aHistorico = array();
	$sUltimoHistorico = '';
	foreach ($arrHist as $historico) {
		if ($sUltimoHistorico == $historico['empsituacao']) {
			continue;
		}
		$aHistorico[] = $historico;
		$sUltimoHistorico = $historico['empsituacao'];
	}
	$db->monta_lista_simples($aHistorico,$cabecalho,500,5,'N','100%',$par2);
*/
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

function listaHistoricoEmpenho($dados) {
	global $db;
	$sql = "SELECT
				u.usunome,
				to_char(hepdata, 'dd/mm/YYYY HH24:MI') as data,
				case when h.co_especie_empenho = '01' then 'Solicitação'
					when h.co_especie_empenho = '02' then 'Reforço'
					when h.co_especie_empenho = '03' then 'Cancelamento/Anulação'
				else '-' end as especie,
				h.empsituacao,
				h.ds_problema,
				CASE WHEN h.valor_total_empenhado IS NOT NULL THEN h.valor_total_empenhado ELSE e.valor_total_empenhado END as valor_total_empenhado,
				h.valor_saldo_pagamento
			FROM par.historicoempenho h
			LEFT JOIN seguranca.usuario u ON u.usucpf=h.usucpf
			INNER JOIN par.empenho e ON e.empid=h.empid and empstatus = 'A'
			WHERE empstatus <> 'I' AND h.empid='".$dados['empid']."'
				and length(h.empsituacao) > 2";

	/* $arrHist = $db->carregar($sql);

	$aHistorico = array();
	$sUltimoHistorico = '';
	foreach ($arrHist as $historico) {
		if ($sUltimoHistorico == $historico['empsituacao']) {
			continue;
		}
		$aHistorico[] = $historico;
		$sUltimoHistorico = $historico['empsituacao'];
	} */

	$cabecalho = array("Usuário atualização","Data", "Espécie Empenho", "Situação","Problema encontrado","Valor empenhado(R$)","Valor pagamento(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}


function cabecalhoSolicitacaoEmpenho()  {
	global $db;

	if($_SESSION['par_var']['esfera']=='estadual') {

		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   '-' as mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobra p
								    WHERE p.prostatus = 'A' and p.proid='".$_SESSION['par_var']['proid']."'");

	} else {

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE  p.prostatus = 'A' and p.proid='".$_SESSION['par_var']['proid']."'");


	}

	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>UF:</td>";
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

/*function listaEmpenhoProcesso($dados) {
	global $db;
	if($dados['empnumeroprocesso']) {
		$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";
	} elseif($dados['proid']) {
		$where[] = "proid='".$dados['proid']."'";
	}


	if($_SESSION['par_var']['esfera']=='estadual') {
		$leftFuncao = " and funid = 6";
	}else{
		$leftFuncao = " and funid = 1";
	}

	#Regras passados por Analista Thiago.
	#regras de perfil: é dado inico ao trabalho em 24/05/2012
	$perfil = pegaPerfilGeral();
	if(
		!(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
			in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
			in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
		)
	){
		$acaoConsulta = "''";
		$acaoReduzir = "''";
		$acaoCancela = "''";
	}else{
		$acaoConsulta = "'<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\');>'";
		$acaoReduzir = "'<img align=absmiddle onclick=\"reduzirEmpenhoPAC('||empid||',\'' || trim(empnumeroprocesso) || '\', \'reduzir\');\" title=\"Reduzir Empenho\" style=cursor:pointer; src=../imagens/restricao_ico.png>'";
		$acaoCancela = "'<img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=cancelarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\');>'";
		
	}
	$acaoCancelamento = "'<img src=../imagens/icone_lupa.png align=absmiddle style=cursor:pointer; title=\"Historico de Cancelamento\" onclick=\"historicoCancelamento('||empid||');\">'";

// 	$sql = "SELECT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
// 				   CASE WHEN e.empsituacao!='CANCELADO' THEN $acaoConsulta ELSE '&nbsp;' END as acao_consultar,
// 				   CASE WHEN e.empsituacao!='CANCELADO' THEN $acaoReduzir ELSE '&nbsp;' END as acao_reduzir,
// 				   CASE WHEN e.empsituacao!='CANCELADO' THEN $acaoCancela ELSE '&nbsp;' END as acao_cancelar,
// 				   e.empcnpj,  iu.iuenome, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao FROM par.empenho e
// 			LEFT JOIN par.processoobra p ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso)
// 			LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
// 			left join par.instrumentounidadeentidade iu on iu.iuecnpj = e.empcnpj
			
// 			".(($where)?"WHERE ".implode(" AND ", $where):"");

// 	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","&nbsp;","CNPJ","Entidade","Nº protocolo","Nº empenho","Valor empenho(R$)","Usuário criação","Situação empenho");
// 	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
	
		$sql = "select 
					DISTINCT mais ||
						CASE WHEN situacao = 'CANCELADO' 
					   		THEN $acaoCancelamento 
					   		ELSE '' END as acao,
					   CASE WHEN empsituacao != 'CANCELADO' THEN $acaoConsulta ELSE '&nbsp;' END as acao_consultar,
					   CASE WHEN empvalorempenho > vrlcancelado 
					   		THEN $acaoReduzir 
					   		ELSE '' 
					   END as acao_reduzir,
					   CASE WHEN empvalorempenho > vrlcancelado 
					   		THEN $acaoCancela 
					   		ELSE '' 
					   END as acao_cancelar,
					  	empcnpj, empprotocolo, empnumero, empvalorempenho, 
					  	vrlcancelado, 
					  	usunome, 
					   empsituacao 
				from(
				SELECT DISTINCT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
					   e.empid, e.empnumeroprocesso, e.empcnpj,  e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao,
					   case when (select count(empid) from par.empenho where empidpai = e.empid and empcodigoespecie in ('03', '13', '04')) > 0 then 'CANCELADO' else e.empsituacao end as situacao,
					   (select coalesce(sum(empvalorempenho), 0.00) from par.empenho where empidpai = e.empid and empcodigoespecie in ('03', '13', '04')) as vrlcancelado
				FROM par.empenho e
					LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
				WHERE
					e.empcodigoespecie not in ('03', '13', '02', '04')
				".(($where)?" and  ".implode(" AND ", $where):"").") as foo";
//ver($sql);
	$cabecalho = array("Histórico", "Atualizar", "Reduzir", "Cancelar", "CNPJ","Nº protocolo","Nº empenho","Valor empenho(R$)", "Valor Cancelado(R$)", "Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','S', true, '', false, true);
}
*/
function listaObrasEmpenhadas($dados){
	global $db;
	
	$empid = $dados['empid'];
	$proid = $dados['proid'];

	if($_SESSION['par_var']['esfera']=='estadual') {

		$where[] = "po.preesfera='E'";

		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobra p
								    WHERE  p.prostatus = 'A' and p.proid='".$proid."'");

	} else {

		$where[] = "po.preesfera='M'";

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE  p.prostatus = 'A' and p.proid='".$proid."'");


	}

	// PARAMETROS FIXOS
	//$where[] = "doc.esdid='228'";
	$where[] = "po.prestatus='A'";
	$where[] = "po.tooid='1'";
	// FIM PARAMETROS FIXOS

	if($arrDados['estuf']) {
		$where[] = "po.estuf='".$arrDados['estuf']."'";
	}

	if($arrDados['muncod']) {
		$where[] = "po.muncod='".$arrDados['muncod']."'";
	}

	if($arrDados['protipo']) {
		$where[] = "pp.ptoclassificacaoobra='".$arrDados['protipo']."'";
	}

	$sql = "SELECT terid FROM par.termocompromissopac WHERE proid='".$proid."'";
	$terid = $db->pegaUm($sql);

	/* Se o processo ja tiver gerado termo, apresentar apenas as obras do termo */
	if($terid) {

		$join = "INNER JOIN par.termoobra teo ON teo.preid = po.preid AND teo.terid='".$terid."'";

	} else {

		/* Filtro padrão pegando municipio, UF, e tipo de obra */
		if($where) {
			$join = "WHERE ".implode(" AND ", $where)."  -- AND emo.eobid IS NULL";
		}

	}
		
	$sql = "SELECT  
				'' as mais,
				'<a class=vizualisa_obra preid='||po.preid||' muncod='||po.muncod||' >'||po.preid || ' - ' || po.predescricao||'</a>' as nomedaobra,
				CASE WHEN pp.ptocategoria IS NOT NULL 
				THEN 
					sum(coalesce(itc2.itcvalorunitario, 0)*itc2.itcquantidade)
				ELSE 
					sum(coalesce(ppo.ppovalorunitario, 0)*coalesce(itcr.pirqtd, itc.itcquantidade))
				END as vlr,
				'<div style=\"float:right\">'||to_char(( e.saldo * 100 /po.prevalorobra  ),'999D99')||'</div>' as porcempenho,
				e.saldo as vlr_empenho,
				res.resdescricao
			FROM par.v_saldo_obra_por_empenho e
			INNER JOIN obras.preobra po ON po.preid = e.preid
			INNER JOIN obras.pretipoobra pp on po.ptoid = pp.ptoid
			LEFT JOIN par.resolucao res ON res.resid = po.resid
			LEFT JOIN obras.preitenscomposicaomi      itc2 ON po.ptoid   = itc2.ptoid AND itc2.itcquantidade > 0 AND po.preid = itc2.preid
			LEFT JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid /*AND itc.itcquantidade > 0*/   AND itc.ptoid not in (43,42, 44, 45)
			left join obras.preitencomposicao_regiao itcr on itcr.itcid = itc.itcid and itcr.estuf = po.estuf and itcr.pirqtd > 0
			LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid   = ppo.itcid AND ppo.preid = po.preid
			WHERE ".implode(" AND ", $where)." 		
				AND CASE WHEN pp.ptocategoria IS NOT NULL 
				THEN 
					itc2.preid IS NOT NULL
				ELSE 
					itc.itcid IS NOT NULL AND coalesce(itcr.pirqtd, itc.itcquantidade) > 0
				END
				AND e.empid = ".$empid."
				AND po.preid in ( SELECT pr.preid FROM par.vm_saldo_empenho_por_obra v INNER JOIN obras.preobra pr ON pr.preid = v.preid WHERE ( coalesce(pr.prevalorobra, 0) - v.saldo < 0.01 ) and pr.preid = po.preid )	
			GROUP BY po.preid,
				po.muncod,
				po.predescricao,
				pp.ptocategoria,
				e.saldo,
				res.resdescricao,
				po.prevalorobra";
	
	//dbg($sql,1);
	$cabecalho = array("&nbsp;","Nome da obra","Valor da obra","% Empenho","Valor empenhado","Resolução");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S', '', $arrHeighTds, $heightTBody);
}


function listaPreObras($dados) {
	global $db;

	if($_SESSION['par_var']['esfera']=='estadual') {

		$where[] = "po.preesfera='E'";

		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobra p
								    WHERE p.prostatus = 'A'  and p.proid='".$_SESSION['par_var']['proid']."'");

	} else {

		$where[] = "po.preesfera='M'";

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE p.prostatus = 'A'  and p.proid='".$_SESSION['par_var']['proid']."'");


	}

	// PARAMETROS FIXOS
	//$where[] = "doc.esdid='228'";
	$where[] = "po.tooid='1'";
	$where[] = "po.prestatus='A'";
	// FIM PARAMETROS FIXOS

	if($arrDados['estuf']) {
		$where[] = "po.estuf='".$arrDados['estuf']."'";
	}

	if($arrDados['muncod']) {
		$where[] = "po.muncod='".$arrDados['muncod']."'";
	}
	
	$sql = "SELECT DISTINCT 
				'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarObrasEmpenhadas(\''||e.empid||'\', this, \'".$_SESSION['par_var']['proid']."\');\">' as mais,
				COALESCE(e.empnumero,'EMPENHO EFETIVADO') as empnumero
			FROM par.empenho e
			INNER JOIN par.empenhoobra eo ON eo.empid = e.empid
			INNER JOIN par.processoobra p ON p.pronumeroprocesso = e.empnumeroprocesso
			WHERE 	p.proid = {$_SESSION['par_var']['proid']}
				AND eo.preid IN (
					SELECT pr.preid 
					FROM par.vm_saldo_empenho_por_obra v 
					INNER JOIN obras.preobra pr ON pr.preid = v.preid 
					WHERE 	( pr.prevalorobra - v.saldo < 0.01 )
					AND pr.preid = eo.preid
				) and e.empcodigoespecie not in ('03', '13', '02', '04') and e.empstatus = 'A'
	";
//ver(simec_htmlentities($sql),d);
	echo "<h3>Obras 100% empenhadas</h3>";
	$cabecalho = array("&nbsp;","Nº do Empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','N');
	
	$sql = "SELECT
				chk,
				nomedaobra,
				'<center>'||(SELECT obrpercentultvistoria FROM obras2.obras WHERE preid = foo.preid AND obrstatus = 'A' AND obridpai IS NULL)::integer||' %</center>' as perc,
				vlr,
				empenhos,
				percentual_empenhado,
				vlr_empenhado,
				porcempenho,
				'<input type=text id=id_vlr_'||preid||' value=\'0,00\' 
					name=name_vlr_'||preid||' size=15 onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" 
					onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenhoObraPac('||preid||', \'v\');\" class=\"disabled vrlaempenhar\" readonly=readonly onfocus=\"this.select();\">
				<input type=\"hidden\" name=\"vlrobra_'||preid||'\" id=\"vlrobra_'||preid||'\" value=\"'||vlr||'\">
				<input type=\"hidden\" name=\"vlr_empenhado_'||preid||'\" id=\"vlr_empenhado_'||preid||'\" value=\"'||vlr_empenhado||'\">' as valoraempenhar,				
				resdescricao
			FROM(
				SELECT 	
					case when po.prereformulacao = true then
						'<img src=../imagens/restricao_ico.png style=cursor:pointer; title=\"Obra em Reformulação\">'
					else 
						'<input type=checkbox name=chk[] onclick=marcarChkObrasPacEmp(this); id=chk_'||po.preid||' value='||po.preid||'>'
					end as chk,					
					'<a class=vizualisa_obra preid='||po.preid||' muncod='||po.muncod||' >'||po.preid || ' - ' || po.predescricao||'</a>' as nomedaobra,
					--po.preid || ' - ' || po.predescricao as nomedaobra,
					po.preid,
					ROUND(po.prevalorobra, 2) as vlr,
					par.retorna_numero_empenhos_obra_pac(po.preid) as empenhos,
					CASE WHEN ( SELECT count(emo.preid) FROM par.empenhoobra emo INNER JOIN par.empenho emp on emp.empid = emo.empid AND empstatus <> 'I' and eobstatus = 'A'  WHERE emo.preid = po.preid ) > 0 THEN
						(	SELECT '<center>'||((SUM(eo.eobvalorempenho - coalesce(emc.vlrempenhocancelado, 0))*100)/po.prevalorobra)||'</center> <input type=hidden id=porcentagem_'||po.preid||' value='||((SUM(eo.eobvalorempenho - coalesce(emc.vlrempenhocancelado, 0))*100)/po.prevalorobra)||' >' 
							FROM par.empenho e 
							INNER JOIN par.empenhoobra eo ON eo.empid = e.empid  and eobstatus = 'A' and e.empcodigoespecie not in ('03', '13', '02', '04')
							left join (
							      select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
								  from par.empenhoobra eb
								      inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
								  where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
								  group by e1.empidpai, eb.preid
							  ) as emc on emc.empidpai = e.empid and emc.preid = eo.preid
							WHERE e.empstatus <> 'I' and eo.preid = po.preid  
						) ELSE
							'<center>'||0.00::numeric(20,2)||'</center> <input type=hidden id=porcentagem_'||po.preid||' value='||0.00||' >'
					END as percentual_Empenhado,
					CASE WHEN ( SELECT count(emo.preid) FROM par.empenhoobra emo INNER JOIN par.empenho emp on emp.empid = emo.empid and empstatus <> 'I' and eobstatus = 'A'  WHERE emo.preid = po.preid ) > 0 THEN
					ROUND((	SELECT SUM(eo.eobvalorempenho - coalesce(emc.vlrempenhocancelado, 0))  
						FROM par.empenho e 
						INNER JOIN par.empenhoobra eo ON eo.empid = e.empid and eobstatus = 'A'
						left join (
						      select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
							  from par.empenhoobra eb
							      inner join par.empenho e1 on e1.empid = eb.empid and empstatus <> 'I'
							  where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
							  group by e1.empidpai, eb.preid
						  ) as emc on emc.empidpai = e.empid and emc.preid = eo.preid
						WHERE eo.preid = po.preid and empstatus <> 'I'  and e.empcodigoespecie not in ('03', '13', '02', '04')
					),2) ELSE
						0.00					
					END as vlr_empenhado,
					CASE WHEN ( SELECT count(emo.preid) FROM par.empenhoobra emo INNER JOIN par.empenho emp on emp.empid = emo.empid and empstatus <> 'I' and eobstatus = 'A'  WHERE emo.preid = po.preid ) > 0 THEN
					(	SELECT 
							CASE WHEN SUM(eo.eobpercentualemp) = NULL
								THEN '<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPac('||po.preid||', \'p\');\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(po.prevalorobra*tpo.ptopercentualempenho/100)||'>'
								ELSE '<input type=text id=id_'||po.preid||' value=\'0,00\' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPac('||po.preid||', \'p\');\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value=\'\'>'
							END 
						FROM par.empenho e 
						INNER JOIN par.empenhoobra eo ON eo.empid = e.empid and eobstatus = 'A' 
						WHERE eo.preid = po.preid and empstatus <> 'I' 
					) ELSE
						'<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoObraPac('||po.preid||', \'p\');\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(po.prevalorobra*tpo.ptopercentualempenho/100)||'>'
					END as porcempenho,
					
					CASE WHEN 
							( 	SELECT count(emo.preid) 
								FROM par.empenhoobra emo 
								INNER JOIN par.empenho emp on emp.empid = emo.empid  and eobstatus = 'A' and empstatus = 'A'
								WHERE emo.preid =  po.preid and empstatus <> 'I'
							) > 0 
						THEN 
							(
								SELECT 
									CASE WHEN SUM(eo.eobpercentualemp) = NULL
										THEN po.prevalorobra - (po.prevalorobra*tpo.ptopercentualempenho/100)
										ELSE NULL
									END  
								FROM par.empenho e 
								INNER JOIN par.empenhoobra eo ON eo.empid = e.empid and eobstatus = 'A' and empstatus = 'A'
								WHERE eo.preid =  po.preid and empstatus <> 'I'
								 
							)
							
						ELSE
							 ROUND(po.prevalorobra, 2) 
						END
						as vlr_empenho,	
					res.resdescricao	
				FROM obras.preobra po
				INNER JOIN par.processoobraspaccomposicao pop on pop.preid = po.preid and pop.pocstatus = 'A'
				INNER JOIN obras.pretipoobra tpo ON tpo.ptoid  = po.ptoid
				LEFT JOIN par.resolucao res ON res.resid = po.resid
				WHERE pop.proid = {$_SESSION['par_var']['proid']}
				AND po.preid in (
						SELECT pr.preid FROM obras.preobra pr
						LEFT  JOIN par.vm_saldo_empenho_por_obra v  ON pr.preid = v.preid 
						WHERE 
							( pr.prevalorobra - coalesce(v.saldo, 0) > 0.01 ) and pr.preid = po.preid 
					)
				GROUP BY po.prevalorobra, po.preid, po.predescricao, po.preidpai, tpo.ptopercentualempenho, res.resdescricao, po.muncod, po.prereformulacao
			) as foo";
// 	ver(simec_htmlentities($sql),d);

	echo "<h3>Obras a serem empenhadas</h3>";
	echo "<form id=\"formpreobras\">";
	$cabecalho = array("<left><input type='checkbox' title='Selecionar Todos' id='todos' name='todos' onclick='abretodosPac(this);'></left>","Nome da obra", "% de Execução<br> da Obra","Valor da obra","N° dos Empenhos","% empenhado","Valor empenhado","% Empenho<br><input type=text id=percentualTodos name=percentualTodos size=6 onKeyUp='calculaPercentualTodosPac(this);'>","Valor a empenhar","Resolução");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	echo "</form>";


}


function executarEmpenho($dados) {
		
	$res_acc = atualizaDadosContaCorrentePac( $dados );

	if($res_acc){
		$res_sp = solicitarProcesso($dados);
		if($res_sp){
			$res_cc = consultarContaCorrente($dados);
			if(!$res_cc){
				$res_sc = solicitarContaCorrente($dados);
			}
		}

		if($res_sp &&  ($res_cc || $res_sc)  ){
			$res_se = solicitarEmpenho($dados); 
		}
	}else{
		$res_sp = solicitarProcesso($dados);
		if($res_sp){
			$res_sc = solicitarContaCorrente($dados);
			if($res_sc && $res_sp ){
				$res_se = solicitarEmpenho($dados);
			}
		}
	}

	// Se não existe conta solicita conta;
	//if($res_acc == 'naoexisteconta'){
	//	$res_sp = solicitarProcesso($dados);
	//	$res_sc = solicitarContaCorrente($dados);
		// Se solicitar conta foi com sucesso solicita empenho
	//	if($res_sc){
	//		$res_se = solicitarEmpenho($dados);
	//	}
	// Se os dados do processo e conta foram atualizados com sucesso consulta conta
	//}else


		/*
		 * --------- ANTES

		$res_cc = consultarContaCorrente($dados);
		// Se conta ativa solicita empenho
		if($res_cc == true ){
			$res_se = solicitarEmpenho($dados);
		// Se conta bloqueada ou inativa solicita a conta corrente
		}else{
			$res_sc = solicitarContaCorrente($dados);
			// Conta corrente solicitada com sucesso empenha
			if($res_sc){
				$res_se = solicitarEmpenho($dados);
			}
		}
			------------ ANTES

	//}else if($res_acc == false){
	//	echo "Erro ao atualizar dados da conta corrente;";
	//}
	}else{
		$res_sp = solicitarProcesso($dados);
		$res_sc = solicitarContaCorrente($dados);
		 //Se solicitar conta foi com sucesso solicita empenho
		if($res_sc){
			$res_se = solicitarEmpenho($dados);
		}
	}
	/*
	$res_acc = atualizaDadosContaCorrentePac( $dados );
	if( $res_acc ){
		$res_sp = solicitarProcesso($dados);
		$res_cc = consultarContaCorrente($dados);
		if($res_cc) $res_sc = solicitarContaCorrente($dados);
		$res_se = solicitarEmpenho($dados);
	}
	*/


}

function atualizaDadosContaCorrentePac($dados) {
	global $db;

    $dadosse = $db->pegaLinha("SELECT p.pronumeroprocesso, p.muncod, p.probanco, p.proagencia, p.prodatainclusao, p.usucpf, p.proseqconta, p.protipo,
									p.seq_conta_corrente, p.nu_conta_corrente, p.procnpj
    						   FROM par.processoobra p
    						   WHERE p.prostatus = 'A'  and proid = {$dados['proid']}");

    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo = $dadosse['pronumeroprocesso'];
    	$tp_processo = 1; // O que vai ser no PAR

    	$nu_cnpj_favorecido = $db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE prostatus = 'A'  and  pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");

    	/*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura

        }else{
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
        }

        $nu_cnpj_favorecido = ($nu_cnpj_favorecido ? $nu_cnpj_favorecido : trim($dadosse['procnpj']));*/
    }
        $nu_cnpj_favorecido = ($nu_cnpj_favorecido ? $nu_cnpj_favorecido : trim($dadosse['procnpj']));

    $data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$somente_conta_ativa	= 'N';
	$numero_de_linhas		= '200';

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$data_created}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_cnpj_favorecido</nu_identificador>
			<nu_processo>$nu_processo</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;


		if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
			//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
				->execute();


		$xmlRetorno = $xml;

		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

		if(!$result) {

			$mensagem = 'ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF:';

			$erros = $xml->status->error->message;
			//$errosConta = $xml->response->body->row->status;

			if(count($erros)>0) {
				foreach($erros as $err) {
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
				$mensagem .= '';

				echo $mensagem;

			$sql = "INSERT INTO par.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'atualizaDadosContaCorrentePac - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			//$mensagemDeRetorno			= substr($errosConta, 0, 1 );
			//if($mensagemDeRetorno == '0'){
			//	return 'naoexisteconta';
			//}

		    return false;
		} else {
			$obContaCorrenteWS = $xml->body->row->children();

			$seq_solic_cr 		= !empty($obContaCorrenteWS->seq_solic_cr) ? "'".(int)$obContaCorrenteWS->seq_solic_cr."'" : 'null';
			$seq_conta 			= !empty($obContaCorrenteWS->seq_conta) ? "'".(int)$obContaCorrenteWS->seq_conta."'" : 'null';
			$dt_movimento 		= !empty($obContaCorrenteWS->dt_movimento) ? "'".(string)$obContaCorrenteWS->dt_movimento."'" : 'null';
			$nu_banco 			= !empty($obContaCorrenteWS->nu_banco) ? "'".(string)$obContaCorrenteWS->nu_banco."'" : 'null';
			$nu_agencia 		= !empty($obContaCorrenteWS->nu_agencia) ? "'".(string)$obContaCorrenteWS->nu_agencia."'" : 'null';
			$nu_conta_corrente	= !empty($obContaCorrenteWS->nu_conta_corrente) ? "'".(string)$obContaCorrenteWS->nu_conta_corrente."'" : 'null';
			$fase_solicitacao	= !empty($obContaCorrenteWS->fase_solicitacao) ? "'".(string)$obContaCorrenteWS->fase_solicitacao."'" : 'null';
			$co_situacao_conta	= !empty($obContaCorrenteWS->co_situacao_conta) ? "'".(string)$obContaCorrenteWS->co_situacao_conta."'" : 'null';
			$situacao_conta 	= !empty($obContaCorrenteWS->situacao_conta) ? "'".(string)$obContaCorrenteWS->situacao_conta."'" : 'null';
			$nu_processo 		= !empty($obContaCorrenteWS->nu_processo) ? "'".(string)$obContaCorrenteWS->nu_processo."'" : 'null';
			$nu_identificador 	= !empty($obContaCorrenteWS->nu_identificador) ? "'".(string)$obContaCorrenteWS->nu_identificador."'" : 'null';
			$ds_razao_social 	= !empty($obContaCorrenteWS->ds_razao_social) ? "'".(string)$obContaCorrenteWS->ds_razao_social."'" : 'null';
			$ds_problema		= "'-'";
			$rnum 				= (int)		$obContaCorrenteWS->rnum;
			$status 			= (string)	$obContaCorrenteWS->status;
			$co_status			= substr( $status, 0, 1 );

			if( trim($co_status) != 0 ){
				$sql = "UPDATE
						  	par.processoobra
						SET
						  	probanco = $nu_banco,
						  	proagencia = $nu_agencia,
						  	proseqconta = $seq_solic_cr,
						  	seq_conta_corrente = $seq_conta,
						  	nu_conta_corrente = $nu_conta_corrente
						WHERE
						  	proid = {$dados['proid']}";

				$db->executar($sql);
				$db->commit();

				$sql = "INSERT INTO par.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'atualizaDadosContaCorrentePac - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$db->executar($sql);
				$db->commit();

				return true;
			} else {
				$mensagem = 'ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF: ';


				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);
				$mensagem .= '';

				echo $mensagem;
				$sql = "INSERT INTO par.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'atualizaDadosContaCorrentePac - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$db->executar($sql);
				$db->commit();

			    return false;
			}
		}
}


function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

		$proseqconta = $db->pegaUm("SELECT proseqconta FROM par.processoobra WHERE prostatus = 'A'  and  proid='".$dados['proid']."'");

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
		    	$db->executar("UPDATE par.processoobra SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE proseqconta='".$proseqconta."'");
		    	$db->commit();
		    }

			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";

			$sql = "INSERT INTO par.historicowsprocessoobra(
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
		    	$fasesolicitacao = (string) $xml->body->row->fase_solicitacao;

		    	if( $codigo == 24 ){
		    		echo "MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.";
		    		return false;
			    } elseif( $codigo == 25 ){
			    	echo "MSG SIMEC : Conta Corrente Bloqueada Definitivamente.";
			    	return false;
			    } elseif( $codigo == 14 ){
			    	echo "MSG SIMEC : Conta Corrente Inativa.";
			    	return false;
			    }elseif($codigo == 13 ){
			    	return true;
			    }
			    if($fasesolicitacao == '01 SOLICITADA' || empty($codigo)){
			    	return true;
			    }
		    } else {
		    	return false;
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

		return false;

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

        $dadoscc = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo FROM par.processoobra WHERE prostatus = 'A'  and proid='".$dados['proid']."'");

        if($dadoscc) {
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
		$nu_identificador=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE prostatus = 'A'  and  pronumeroprocesso =  '{$dadoscc['pronumeroprocesso']}'");

        /*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_identificador=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE pronumeroprocesso =  '{$dadoscc['pronumeroprocesso']}'");
        }else{
        	// CNPJ da prefeitura
			$nu_identificador=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadoscc['muncod']."'");
        }*/



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
        $nu_sistema="5";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        if($dadoscc['protipo'] == 'P') $co_programa_fnde="BW";
        else $co_programa_fnde="CN";



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

			$sql = "INSERT INTO par.historicowsprocessoobra(
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

		    $db->executar("UPDATE par.processoobra SET proseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE proid='".$dados['proid']."'");

			$sql = "INSERT INTO par.historicowsprocessoobra(
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

		/* if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else { */
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
		//}

		$dadosemp = $db->pegaLinha("SELECT e.empprotocolo, op.proid
		 							FROM par.empenho e
										inner join par.processoobra op on e.empnumeroprocesso = op.pronumeroprocesso and op.prostatus = 'A' 
									WHERE empstatus <> 'I' and e.empid = '".$dados['empid']."'");

        if($dadosemp) {
        	$nu_seq_ne = $dadosemp['empprotocolo'];
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
		
		$result = (integer) $xml->status->result;

		if($result) $hwpwebservice="consultarEmpenho - Sucesso";
		else $hwpwebservice="consultarEmpenho - Erro";

		$sql = "INSERT INTO par.historicowsprocessoobra(
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


	    echo "------ CONSULTA DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
		echo "*** Detalhes da consulta ***\n\n";
		echo "* Nº processo : ".(($xml->body->row->processo)?$xml->body->row->processo:"-")."\n";
		echo "* CNPJ : ".$xml->body->row->nu_cnpj."\n";
		echo "* Valor(R$) : ".number_format((string)$xml->body->row->valor_ne,2,",",".")."\n";
		echo "* Data : ".$xml->body->row->data_documento."\n";
		echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
		echo "* Valor empenhado(R$) : ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."\n";
		echo "* Saldo pagamento(R$) : ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."\n";
		echo "* Situação : ".$situacaoEmpenho."\n\n";

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
			$set[] = "empnumerooriginal = ".($empnumerooriginal ? "'".$empnumerooriginal."'" : 'null');
			$set[] = "empanooriginal = ".($empanooriginal ? "'".$empanooriginal."'" : 'null');

		}
		if( $xml->body->row->valor_ne ) 					$set[] = "empvalorempenho = '".$xml->body->row->valor_ne."'";
		if( $xml->body->row->ds_problema ) 					$set[] = "ds_problema = '".$xml->body->row->ds_problema."'";
		if( $xml->body->row->valor_total_empenhado )		$set[] = "valor_total_empenhado = '".$xml->body->row->valor_total_empenhado."'";
		if( $xml->body->row->valor_saldo_pagamento )		$set[] = "valor_saldo_pagamento = '".$xml->body->row->valor_saldo_pagamento."'";
		if( $xml->body->row->data_documento )				$set[] = "empdata = '".$xml->body->row->data_documento."'";
		if( $xml->body->row->unidade_gestora_responsavel )	$set[] = "empunidgestoraeminente = '".$xml->body->row->unidade_gestora_responsavel."'";
		if( $xml->body->row->tp_especializacao )			$set[] = "tp_especializacao = '".$xml->body->row->tp_especializacao."'";
		if( $xml->body->row->co_diretoria )					$set[] = "co_diretoria = '".$xml->body->row->co_diretoria."'";

		if($set) {
			$db->executar("UPDATE par.empenho SET ".(($set)?implode(",",$set):"")."
						   WHERE empid='".$dados['empid']."'");
		}

		$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento)
    			VALUES ('".$_SESSION['usucpf']."',
    					'".$dados['empid']."',
    					NOW(),
    					'".$situacaoEmpenho."',
    					'".$xml->body->row->co_especie_empenho."',
    					'".$xml->body->row->ds_problema."',
    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";

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

function solicitarEmpenho($dados) {
	global $db;
	
		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadosse = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo FROM par.processoobra WHERE prostatus = 'A'  and  proid='".$dados['proid']."'");
        if($dadosse) {

	        // numero do processo (No desenvolvimento é fixo)
        	if($_SESSION['baselogin'] == "simec_desenvolvimento1" ||
        	   $_SESSION['baselogin'] == "simec_espelho_producao1" ){

        	   	$nu_processo='23034655466200900';
				$co_fonte_recurso_solic="0100000000";
				//$co_plano_interno_solic="PFB02F52BWN";
				//$co_ptres_solic="020990";
				$co_ptres_solic="061655";
				$co_natureza_despesa_solic="44504200";



        	} else {

	        	$nu_processo=$dadosse['pronumeroprocesso'];
				// constante=4042

	        	if($_SESSION['par_var']['esfera']=='estadual') {
	        		$co_natureza_despesa_solic="44304200";
	        	}else{
	        		$co_natureza_despesa_solic="44404200";
	        	}

	        	if($dadosse['protipo'] == 'P') {
					// constante=MEC00001
					//$co_plano_interno_solic="MEC00001";
					// constante=037825
					if( date("Y") == 2011 ){
						$co_ptres_solic="037825";
					} else {
						//$co_ptres_solic="043990";
						$co_ptres_solic="061655";
					}
					// constante=12365144812KU0001
					//$frpfuncionalprogramatica="12365144812KU0001";
					$frpfuncionalprogramatica="12365203012KU0001";

	        	} else {
					// constante=MEC00002
					//$co_plano_interno_solic="MEC00002";
					// constante=037826
					if( date("Y") == 2011 ){
						$co_ptres_solic="037826";
					} else {
						//$co_ptres_solic="043991";
						$co_ptres_solic="061654";
					}
					// constante=12365144812KV0001
					//$frpfuncionalprogramatica="12812144812KV0001";
					$frpfuncionalprogramatica="12365203012KU0001";
	        	}

				//$sql = "SELECT * FROM par.fonterecursopac WHERE frpfuncionalprogramatica='".$frpfuncionalprogramatica."' ORDER BY frpid";
        	}
        	if( $dados['frpid'] ){
	       		$sql = "SELECT frpcodigo, frpsaldo, frpfuncionalprogramatica, frptitulo FROM par.fonterecursopac WHERE frpid = {$dados['frpid']} ORDER BY frpid";
				$fonterecursopac = $db->pegaLinha($sql);

				if($fonterecursopac) {
					$sql = "SELECT sum(vve.vrlempenhocancelado) as vlr FROM par.empenho e
							inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
							INNER JOIN par.processoobra p ON p.pronumeroprocesso = e.empnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and p.prostatus = 'A' 
							WHERE empstatus <> 'I' and p.protipo='".$dadosse['protipo']."' AND e.empfonterecurso='".$fonterecursopac['frpcodigo']."'";
					$valor_recurso_pac = $db->pegaUm($sql);
					$total_parcial = ($valor_recurso_pac + str_replace(array(".",","),array("","."),$dados['name_total']));
					$frpfuncionalprogramatica = $fonterecursopac['frpfuncionalprogramatica'];

					if($total_parcial <= $fonterecursopac['frpsaldo']) {
						$co_fonte_recurso_solic = $fonterecursopac['frpcodigo'];
						//return false;
					}
					if(!$co_fonte_recurso_solic) {
					    echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
					    echo "MSG SIMEC : Fonte de recurso ".$fonterecursopac['frpcodigo']." - ".$fonterecursopac['frptitulo']." finalizada \n";
					    echo "Saldo da fonte : ".$fonterecursopac['frpsaldo']." < (Total de empenho : ".$valor_recurso_pac." + Valor empenho atual : ".str_replace(array(".",","),array("","."),$dados['name_total']).")";
					    return false;
					}

				}
        	} else {
        		echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
        		echo "MSG SIMEC : Fonte de recurso não informada!";
        		return false;
        	}
        }

		$nu_cnpj_favorecido=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE prostatus = 'A'  and pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");

		/*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");
        }else{
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
        }*/



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
		//$co_centro_gestao_solic="69500000000"; //"61700000000";
		$co_centro_gestao_solic= $dados['centroGestao']; 
		
		$co_ptres_solic= $dados['ptres']; 
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
		$nu_sistema="5";

		$co_centro_gestao_solic= $dados['centroGestao'];  //  "69500000000"; //"61700000000";
		
		$co_plano_interno_solic = $dados['planoInterno'];
		
		/*
	  if($co_plano_interno_solic == 'RFF34I414DN' || $co_plano_interno_solic == 'RFF34I413DN' ){
        	$co_programa_fnde="CM";
        	$nu_sistema="7";
        }
		*/
		if(!$co_plano_interno_solic){
			echo'Necessário selecionar um Plano Interno.';
			return false;
		}
		
		if( $dados['tipo'] != 'visualiza' ){
			$sql = "SELECT distinct l.lwsid FROM par.logws l
						inner join par.historicowsprocessoobra h ON l.lwsid = h.lwsid
					WHERE
						h.proid = {$dados['proid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessoobra where proid = {$dados['proid']})
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
				'lwsrequestdata'	=> 'now()',
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
		$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'insert' );
		
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
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
		
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
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
			
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
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );

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
				            )
					RETURNING empid;";

			$empid = $db->pegaUm($sql);

			$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);
			$db->commit();

			if($dados['chk']) {
				
				foreach($dados['chk'] as $preid)
				{
					$sql = "INSERT INTO par.empenhoobra(
            				preid, empid, eobpercentualemp, eobvalorempenho, eobpercentualemp2)
    						VALUES ('".$preid."', '".$empid."', '".str_replace(",",".", $dados['name_'.$preid])."', '".retiraPontosBD($dados['name_vlr_'.$preid])."', '".round($dados['name_'.$preid])."');";

					$db->executar($sql);
					$db->commit();
					
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
						
					$obrid_1 = importarObrasPac( $preid );
// 					importarObras2Pac( $preid, $obrid_1 );
					$preObra = new PreObra( $preid );
					$preObra->importarPreobraParaObras2( $preid );
				}
			}

			$db->commit();
			
			cargaViewEmpenhoObras( $nu_processo );
			
			return true;
		}

	} catch (Exception $e){
		$arrParam = array(
				'lwserro' 			=> true,
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
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
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";


	}
}

function importarObras2Pac( $preid, $obrid_1 ){
	global $db;
	/*** INICIO - Importação dos dados para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra não exista ***/
	$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
	$existeObra = $db->pegaUm($sql);

	if( (integer)$existeObra < 1 ){
		/*** Recupera dados da Pre Obra ***/
		$sql = "SELECT
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
					UPPER(p.preesfera) AS preesfera,
					CASE
						WHEN pt.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
						WHEN pt.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
						WHEN pt.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
						ELSE 54 --OUTROS
	                END as programa,
	                CASE WHEN pt.ptoclassificacaoobra = 'Q' THEN 3 ELSE 1 END as modalidadeensino, -- MODALIDADE DE ENSINO
					CASE
						WHEN pt.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
						WHEN pt.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
						ELSE 1 --CONSTRUÇÃO
					END AS tipodeobra,
					CASE
						WHEN REPLACE(UPPER(p.predescricao), 'Í', 'I') ILIKE '%INDIGENA%' THEN 4 -- INDÍGENA
						WHEN UPPER(p.predescricao) ILIKE '%RURAL%' THEN 1 -- RURAL
						WHEN UPPER(p.predescricao) ILIKE '%QUILOMBO%' THEN 3 -- QUILOMBO
					ELSE 2 --URBANO
					END AS classificacaoobra,
					p.prevalorobra as valorobra,
					pt.tpoid,
					pro.pronumeroprocesso
				FROM
					obras.preobra p
				INNER JOIN territorios.municipio mun on p.muncod = mun.muncod
				LEFT JOIN par.processoobraspaccomposicao 	poc ON poc.preid = p.preid
				LEFT JOIN par.processoobra 				pro ON pro.proid = poc.proid
				LEFT JOIN obras.pretipoobra pt ON pt.ptoid = p.ptoid
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
		$sql = "INSERT INTO entidade.endereco (
					   tpeid,
					   endcep,
					   endlog,
					   endcom,
					   endbai,
					   muncod,
					   estuf,
					   endnum,
					   medlatitude,
					   medlongitude,
					   endstatus
					)VALUES(
					  4,
					  '".$dadosPreObra[0]['precep']."',
					  '".$dadosPreObra[0]['prelogradouro']."',
					  '".$dadosPreObra[0]['precomplemento']."',
					  '".$dadosPreObra[0]['prebairro']."',
					  '".$dadosPreObra[0]['muncod']."',
					  '".$dadosPreObra[0]['estuf']."',
					  '".$dadosPreObra[0]['prenumero']."',
					  '".$dadosPreObra[0]['prelatitude']."',
					  '".$dadosPreObra[0]['prelongitude']."',
					  'A' ) RETURNING endid";

		$endid = $db->pegaUm($sql);

		/*** Insere a nova obra ***/
		$sql = "INSERT INTO obras2.empreendimento(
			            orgid,
			            empesfera,
			            tpoid,
			            prfid,
			            tobid,
			            tooid,
			            cloid,
			            moeid,
			            entidunidade,
			            empdsc,
			            empvalorprevisto,
			            endid,
			            preid,
			            obrid_1
				) VALUES (
						3,
						'".$dadosPreObra[0]['preesfera']."',
						" . ($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'NULL') . ",
						".$dadosPreObra[0]['programa'].",
						'".$dadosPreObra[0]['tipodeobra']."',
						1,
						'".$dadosPreObra[0]['classificacaoobra']."',
						".$dadosPreObra[0]['modalidadeensino'].",
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
					obrid_1,
					obrnumprocessoconv)
				VALUES('".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
						".$unidade_implantadora.",
						1,
						'".$preid."',
						'".$endid."',
						" . ($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'NULL') .",
						'".$dadosPreObra[0]['tipodeobra']."',
						'".$dadosPreObra[0]['classificacaoobra']."',
						'".$dadosPreObra[0]['valorobra']."',
						'" . $empid . "',
						".$obrid_1.",
						'".($dadosPreObra[0]['pronumeroprocesso'] ? $dadosPreObra[0]['pronumeroprocesso'] : 'NULL')."')
				RETURNING obrid";
		$obrid = $db->pegaUm($sql);

		/*
		 * Cria Documento WF - Início
		 */
		require_once APPRAIZ . 'includes/workflow.php';
		$docdsc = "Fluxo de obra do módulo Obras II - obrid " . $obrid;
		if($dadosPreObra[0]['tpoid'] == OBR_TPOID_MI_TIPO_B || $dadosPreObra[0]['tpoid'] == OBR_TPOID_MI_TIPO_C){
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
	}else{
		$sql = "SELECT obrid FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
		$obrid = $db->pegaUm($sql);
		
		$sql = "UPDATE obras2.obras SET obrstatus = 'A' WHERE obrid = $obrid";
		$db->executar($sql);
		$db->commit();
	}
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
}

function importarObrasPac( $preid ){
	global $db;
	/*** INICIO - Importação dos dados para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra não exista ***/
	$sql = "SELECT obrid FROM obras.preobra WHERE preid = ".$preid." AND obrid_1 is not null";
	$obrid = $db->pegaUm($sql);

	if( (integer)$obrid < 1 ){
		unset($obrid);
		/*** Recupera dados da Pre Obra ***/
		$sql = "SELECT
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
					UPPER(p.preesfera) as preesfera,
					CASE
						WHEN pt.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
						WHEN pt.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
						WHEN pt.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
						ELSE 54 --OUTROS
	                END as programa,
	                CASE WHEN pt.ptoclassificacaoobra = 'Q' THEN 3 ELSE 1 END as modalidadeensino, -- MODALIDADE DE ENSINO
					CASE
						WHEN pt.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
						WHEN pt.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
						ELSE 1 --CONSTRUÇÃO
					END AS tipodeobra,
					CASE
						WHEN REPLACE(UPPER(p.predescricao), 'Í', 'I') ILIKE '%INDIGENA%' THEN 4 -- INDÍGENA
						WHEN UPPER(p.predescricao) ILIKE '%RURAL%' THEN 1 -- RURAL
						WHEN UPPER(p.predescricao) ILIKE '%QUILOMBO%' THEN 3 -- QUILOMBO
					ELSE 2 --URBANO
					END AS classificacaoobra,
					p.prevalorobra as valorobra,
					pt.tpoid
				FROM
					obras.preobra p
				INNER JOIN
					territorios.municipio mun on p.muncod = mun.muncod
				--INNER JOIN
				--	entidade.endereco ende ON ende.muncod = p.muncod
				--INNER JOIN
				--	entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
				--INNER JOIN
				--	entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
				LEFT JOIN
					obras.pretipoobra pt ON pt.ptoid = p.ptoid
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
		$sql = "INSERT INTO entidade.endereco (
					   endcep,
					   endlog,
					   endcom,
					   endbai,
					   muncod,
					   estuf,
					   endnum,
					   medlatitude,
					   medlongitude,
					   endstatus
					)VALUES(
					  '".$dadosPreObra[0]['precep']."',
					  '".$dadosPreObra[0]['prelogradouro']."',
					  '".$dadosPreObra[0]['precomplemento']."',
					  '".$dadosPreObra[0]['prebairro']."',
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
		$sql = "INSERT INTO obras.obrainfraestrutura(obrdesc, entidunidade, orgid, tooid, preid, endid, obrtipoesfera, tpoid, prfid, tobraid, cloid, moeid, obrvalorprevisto, obsstatus )
				VALUES('".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
						".$unidade_implantadora.",
						3,
						1,
						".$preid.",
						".$endid.",
						'".$dadosPreObra[0]['preesfera']."',
						".$dadosPreObra[0]['tpoid'].",
						".$dadosPreObra[0]['programa'].",
						'".$dadosPreObra[0]['tipodeobra']."',
						'".$dadosPreObra[0]['classificacaoobra']."',
						'".$dadosPreObra[0]['modalidadeensino']."',
						'".$dadosPreObra[0]['valorobra']."',
						'I')
				RETURNING obrid";
		$obrid = $db->pegaUm($sql);

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

		if( $anexos )
		{
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
	} else {
		$obrid = $db->pegaUm("SELECT obrid_1 FROM obras.preobra WHERE preid = ".$preid);
	}
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
	return $obrid;
}

function cancelarEmpenho($dados) {
	global $db;

	try {
		/*
		// a pedido do julio dia 01/08/2014 15:41
		// validando se tem termo
		$sql = "SELECT te.terid FROM par.empenhoobra o
				INNER JOIN par.termoobra t ON t.preid = o.preid and eobstatus = 'A'
				INNER JOIN par.termocompromissopac te ON te.terid = t.terid
				WHERE o.empid='".$dados['empid']."' AND te.terassinado=TRUE AND te.terstatus = 'A'";

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

		$dadosemp = $db->pegaLinha("SELECT e.empnumeroprocesso, e.empprogramafnde , to_char(e.empdata, 'YYYY') as datadoenvio, e.empcnpj, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, e.empprotocolo, op.proid, e.empfonterecurso
			 							FROM par.empenho e
											inner join par.processoobra op on e.empnumeroprocesso = op.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and op.prostatus = 'A' 
											inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
										WHERE empstatus <> 'I' and e.empid = '".$dados['empid']."'");

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
	        	$co_ptres_solic="061655";
	        }
	        
	        $frpfuncionalprogramatica="12365203012KU0001";
	    } else {
	    	$co_plano_interno_solic="MEC00002";
	    	if( date("Y") == 2011 ){
	        	$co_ptres_solic="037826";
	       	} else {
	        	$co_ptres_solic="061654";
	        }
	        $frpfuncionalprogramatica="12368203012KV0001";
	    }
	        
	    $nu_cnpj_favorecido=$dadosemp['empcnpj'];
	        
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
	    	
	    	if($dadosemp['datadoenvio'] == date('Y') ){
	    		$co_especie_empenho="03";
	    	}else{
	    		$co_especie_empenho="13";
	    	}
	    	
	    	$nu_empenho_original=null;
	        $an_exercicio_original=null;
	    }
	    $vl_empenho=$dadosemp['empvalorempenho'];
		$co_esfera_orcamentaria_solic="1";
	    $co_centro_gestao_solic="69500000000"; //"61700000000"; //"69500000000";
	    $an_convenio=null;
	    $nu_convenio=null;
	    $co_observacao="2";
		$co_tipo_empenho="3";
		$co_descricao_empenho="0011";
		$co_gestao_emitente="15253";
		$co_programa_fnde=$dadosemp['empprogramafnde'];
		$co_fonte_recurso_solic=$dadosemp['empfonterecurso'];
		$co_unidade_gestora_emitente="153173";
		$co_unidade_orcamentaria_solic="26298";
		$nu_proposta_siconv=null;
		$nu_sistema="5";

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

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
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
		$result = (integer) trim($result);

		if($result == 1) {
			echo "------ CANCELAMENTO DE EMPENHO ------\n\n";
			echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
			
			$errodecancelamento = false;
			$sql = "INSERT INTO par.historicowsprocessoobra(
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

			//$db->executar("UPDATE par.empenho SET empsituacao='CANCELADO' WHERE empid='".$dados['empid']."'");
			
			$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, 
											empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
								            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, 
											empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
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
					FROM par.empenhoobra WHERE eobstatus <> 'I' and empid = {$dados['empid']}";
			$arrEmpSub = $db->carregar($sql); 
			$arrEmpSub = $arrEmpSub ? $arrEmpSub : array();
			
			foreach ($arrEmpSub as $v) {
				$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
						VALUES ('".$v['preid']."', '".$empid."', '".$v['eobpercentualemp2']."', '".$v['eobvalorempenho']."', '".$v['eobpercentualemp']."');";
				$db->executar($sql);
			}
			
			$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'CANCELADO SIGEF');";
			$db->executar($sql);
			
			$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empid";
			$empnumero = $db->pegaUm($sql);
					
			insereHistoricoStatusObra( $empid, Array(), 'I', "Obra inativada pelo cancelamento de empenho $empnumero" );
			
			inativaObras2SemSaldoEmpenho( $empid, Array() );

			//$db->executar("UPDATE par.empenhoobra set eobstatus = 'I' WHERE empid = '".$dados['empid']."'");
			
			cargaViewEmpenhoObras( $nu_processo );
			
			$db->commit();

		} else {
			$errodecancelamento = true;
			

			$sql = "INSERT INTO par.historicowsprocessoobra(
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

		   //	return false;
		}
	
		if($errodecancelamento) {
			
			$sql = "SELECT distinct l.lwsid FROM par.logws l
					    inner join par.historicowsprocessoobra h ON l.lwsid = h.lwsid
					WHERE
					    h.proid = {$dadosemp['proid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessoobra where proid = {$dadosemp['proid']})
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
			$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'insert' );

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();

			$xmlRetorno = $xml;
			
			$arrParam = array(
					'hwpid'			=> $hwpid,
					'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
			
			$arrParam = array(
					'lwsresponsedata' 	=> 'now()',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		    $xml = simplexml_load_string( stripslashes($xml));

		    echo "------ SOLICITAÇÃO DE CANCELAMENTO DE EMPENHO ------\n\n";
			echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

			$result = (integer) $xml->status->result;
			$result = (integer) trim($result);

			if($result){				
				$arrParam = array(
						'lwserro' => false,
						'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'cancelarEmpenho2 - Sucesso'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
				
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
						FROM par.empenhoobra WHERE empid = {$dados['empid']}";
				$arrEmpObra = $db->carregar($sql);
				$arrEmpObra = $arrEmpObra ? $arrEmpObra : array();
					
				foreach ($arrEmpObra as $v) {
					$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ('".$v['preid']."', '".$empid."', '".$v['eobpercentualemp2']."', '".$v['eobvalorempenho']."', '".$v['eobpercentualemp']."');";
					$db->executar($sql);
				}
					
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    					VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'CANCELADO SIGEF\SIAFI');";
				$db->executar($sql);
				$db->commit();
				
				cargaViewEmpenhoObras( $nu_processo );
				
			}else{
				$arrParam = array(
					'lwserro' => true,
					'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
				
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'cancelarEmpenho2 - Erro'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
			}
			$db->commit();
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

    $dadosse = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo
    						   FROM par.processoobra
    						   WHERE prostatus = 'A'  and proid='".$dados['proid']."'");

    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['pronumeroprocesso'];
    	$tp_processo=1;
    	if($dadosse['protipo'] == 'P') $co_programa_fnde="BW";
        else $co_programa_fnde="CN";

    }
    $nu_cnpj_favorecido=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE prostatus = 'A'  and pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");

		/*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");
        }else{
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
        }*/

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

			$sql = "INSERT INTO par.historicowsprocessoobra(
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

			$sql = "INSERT INTO par.historicowsprocessoobra(
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