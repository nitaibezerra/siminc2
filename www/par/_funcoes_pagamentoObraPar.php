<?php
function cabecalhoSolicitacaoPagamentoObraPar() {
	global $db;

	if($db->pegaUm("SELECT muncod FROM par.processoobraspar WHERE prostatus = 'A'  and proid = ".$_SESSION['par_var']['proid'])) {
		$_SESSION['par_var']['esfera']='municipal';
		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobraspar p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE p.prostatus = 'A'  and  p.proid='".$_SESSION['par_var']['proid']."'");
	} else {
		$_SESSION['par_var']['esfera']='estadual';
		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   '-' as mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM par.processoobraspar p
								    WHERE p.prostatus = 'A'  and p.proid='".$_SESSION['par_var']['proid']."'");
	}
	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita width=\"30%\">UF:</td>";
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


function obrasPagamento($dados) {
	global $db;

	echo "<h3>Lista de obras do pagamento</h3>";

	$sql = "SELECT pe.preid||' - '||pe.predescricao, pe.prevalorobra, po.poppercentualpag, po.popvalorpagamento
			FROM par.pagamentoobrapar po
			INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
			INNER JOIN obras.preobra pe ON pe.preid = po.preid  AND pe.prestatus = 'A'
			WHERE po.pagid='".$dados['pagid']."' --AND pe.tooid = 1";
	
	$cabecalho = array("Descrição da obra","Total da obra(R$)","% Pagamento","Pagamento da obra(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2, true, false, false, true);
	exit();
}

function listaPagamentoEmpenho($dados) {
	global $db;
	
	if($dados['empnumeroprocesso']) {
		$arrProcesso = $db->pegaLinha("SELECT proid, muncod, estuf FROM par.processoobraspar WHERE prostatus = 'A'  and  pronumeroprocesso='".$dados['empnumeroprocesso']."'");
		$_SESSION['par_var']['proid'] = $arrProcesso['proid'];
		if($arrProcesso['muncod']) $_SESSION['par_var']['esfera'] = 'municipal';
		if($arrProcesso['estuf']) $_SESSION['par_var']['esfera']  = 'estadual';
	}

	if(!$_SESSION['par_var']['proid']) {
		die("<p align=center><b>Número do processo não encontrado. Por favor feche a janela e reinicie o procedimento.</b></p>");
	}

	$where[] = "p.proid='".$_SESSION['par_var']['proid']."'";
	if($_SESSION['par_var']['esfera']=='estadual'){
//		$where[] = "funid = 6";
		$where[] = "funid = ".DUTID_SECRETARIA_ESTADUAL;
	}else{
//		$where[] = "funid = 1";
		$where[] = "funid = ".DUTID_PREFEITURA;
	}

	$sql = "SELECT
					'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarPagamento(\''||e.empid||'\', this);\">' as mais,
					e.empcnpj,
					en.entnome,
					e.empprotocolo,
					(e.empvalorempenho - coalesce(vrlcancelado, 0)) as empvalorempenho, 
					u.usunome,
					e.empsituacao
				FROM par.empenho e
				INNER JOIN par.processoobraspar p ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso) and empcodigoespecie not in ('03', '13', '02', '04')  and p.prostatus = 'A' and empstatus = 'A' 
				LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
				LEFT JOIN par.entidade en ON en.entnumcpfcnpj=e.empcnpj
				left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
		                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
		                    group by 
		                        empnumeroprocesso,
		                        empcodigoespecie,
		                        empidpai) as ep on ep.empidpai = e.empid
				--LEFT JOIN entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj
				--LEFT JOIN entidade.funcaoentidade fun ON fun.entid=en.entid
				".(($where)?"WHERE ".implode(" AND ", $where):"");

	$cabecalho = array("&nbsp;","CNPJ","Entidade","Nº protocolo","Valor empenho(R$)","Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

function listaPagamento($dados) {
	global $db;

	$perfil = pegaPerfilGeral();
	//regras de acesso passada por Thiago em 24/05/2012
	if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
		in_array(PAR_PERFIL_PAGADOR ,$perfil)
	){
		$atualizar = "'<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarPagamento('||p.pagid||','||e.empnumeroprocesso||');>'";
		$cancelar = "'<img src=../imagens/excluir.gif style=cursor:pointer; onclick=cancelarPagamento('||p.pagid||','||e.empnumeroprocesso||');>'";
	}else{
		$atualizar = "''";
		$cancelar = "''";
	}

	$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";

	$sql = "SELECT
				''as mais,
				--'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoPagamento(\''||p.pagid||'\', this);\">' as mais,
				$atualizar as atualizar,
				$cancelar as cancelar,
				pagparcela || '°' as parcela,
				pagmes,
				paganoparcela,
				'R$ ' || to_char(pagvalorparcela,'999G999G999G999D99') as vlr,
				u.usunome,
				paganoexercicio,
				COALESCE(pagsituacaopagamento,'-')
			FROM
				par.pagamento p
			LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
			LEFT JOIN par.empenho e ON e.empid = p.empid and empstatus = 'A'
			WHERE
				p.empid = {$dados['empid']}
				AND p.pagstatus = 'A'";


	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","Parcela","Mês da Parcela","Ano da Parcela","Valor da Parcela","Usuário criação","Exercício","Situação");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','90%',$par2);
}

function listaEmpenho($dados) {
	global $db;
	
	$sql = "SELECT DISTINCT pronumeroprocesso, sisid FROM par.processoobraspar WHERE prostatus = 'A'  and  proid = ".$_SESSION['par_var']['proid'];
	$arrProcesso = $db->pegaLinha($sql);

	$empid = $dados['empid'] ? $dados['empid'] : 0;
	
	$sql = "select distinct md.mdonome, md.mdoid, dv.dpvdatavalidacao as data, md.mdoqtdvalidacao,
				(select
						total
					from(
					select count(empid) as total, sum(e.empvalorempenho - coalesce(vrlcancelado, 0)) as vrlempcancelado 
						from par.empenho e
						left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie 
									from par.empenho
									where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
									group by 
										empnumeroprocesso,
										empcodigoespecie,
										empidpai) as ep on ep.empidpai = e.empid
					where e.empnumeroprocesso = pp.pronumeroprocesso and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
					) as foo
					where vrlempcancelado > 0) as empenho
			from par.vm_documentopar_ativos dp 
				left join par.modelosdocumentos md on md.mdoid = dp.mdoid and md.tpdcod in (21, 102) and md.mdostatus = 'A'
				inner join par.processoobraspar pp on pp.proid = dp.proid and pp.prostatus = 'A' 
				left join par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
			where
				dp.proid = {$_SESSION['par_var']['proid']}
			    and pp.sisid = {$arrProcesso['sisid']}";
	
	$arrDocumento = $db->pegaLinha($sql);
	
	$termoEX = array(68, 42, 67, 65, 41, 66, 69, 73, 74, 75, 77);
	
	$arrErros = array();
	
	if($_SESSION['par_var']['esfera'] == 'estadual') {
		if( $arrProcesso['sisid'] == 23 ){
			$funid = 6;		
			$dutid = DUTID_SECRETARIA_ESTADUAL;
		}
		$label = 'Estado';
		if( !strpos( strtolower($arrDocumento['mdonome']) , strtolower('obras_estado') ) ){
			$arrErros[] = 'O termo de Obras do Estado ainda não foi gerado.';
		}
	}else{
		$funid = 1;
		$dutid = DUTID_PREFEITURA;
		
		$label = 'Município';
		if( !strpos( strtolower($arrDocumento['mdonome']) , strtolower('obras_municipios') ) ){
			$arrErros[] = 'O termo de Obras do Município ainda não foi gerado.';
		}
	}
	
//	if( !in_array($arrDocumento['mdoid'], $termoEX) ){
//ver($arrDocumento['mdoqtdvalidacao'], $arrDocumento['mdoid']);
	if( $arrDocumento['mdoqtdvalidacao'] > 0 ){
		if( empty($arrDocumento['data']) && !empty($arrDocumento['mdonome']) ){
			$arrErros[] = 'O termo de Obras do '.$label.' foi gerado mas não foi validado.';
		}
	}
	
	if( (int)$arrDocumento['empenho'] == 0 ){
		$arrErros[] = 'Não possui empenho para este processo.';
	}
	 
	if( !empty($arrErros) ){
		
		$arrArray = array(
						array('<span style="color: red; font-weight: bold">Não é possível efetuar o pagamento, pois existem pendências.<br>* '.implode('<br>* ', $arrErros).'</span>')
						);
		
		$cabecalho = array("Termos");
		$db->monta_lista_simples($arrArray, $cabecalho, 500, 5, 'N', '100%', $par2);
	} else {
		
	if( $arrProcesso['sisid'] == 23 || $arrProcesso['sisid'] == 57 ){
			$sql = "SELECT distinct
						'<input type=hidden id='|| e.empid ||' value=\"'|| v.saldo ||'\"/> <input type=radio class=teste name=empid value='|| e.empid ||' onclick=\"verDadosPagamento(this.value);\" '|| CASE WHEN e.empnumero IS NULL THEN 'disabled' ELSE '' END  ||' />' as radio,
						CASE WHEN e.empnumero IS NULL THEN 'Aguardando efetivação' ELSE e.empnumero END as empnumero,
						vps.vpsvinculacao,
						e.empfonterecurso||'&nbsp;' as empfonterecurso,
                        --sum(v.saldo) as valor,
                        v.saldo as valor,
                        coalesce((
                        SELECT
                            sum(sub_p.pagvalorparcela) AS valor_pago
                        FROM par.pagamento  sub_p 
                        	INNER JOIN par.empenho sub_e ON sub_e.empid = sub_p.empid and sub_e.empstatus = 'A' 
                        WHERE
                            sub_p.pagsituacaopagamento not ilike '%CANCELADO%' 
                            AND sub_p.pagstatus = 'A'
                            and sub_e.empcodigoespecie not in ('03', '13', '02', '04')
                            AND sub_e.empid = e.empid
                        ),0.00) AS valor_pago,
						empsituacao,
						public.formata_cpf_cnpj(e.empcnpj) as empcnpj,
						en.entnome,
						e.empprotocolo||'&nbsp;' as empprotocolo
					FROM
						par.empenho e
                    LEFT  JOIN par.vinculacaoptressigef	vps ON vps.vpsptres = e.empcodigoptres 
						LEFT JOIN par.entidade en ON en.entnumcpfcnpj=e.empcnpj AND en.entstatus = 'A' AND en.dutid = 6
                        inner join par.v_saldo_por_empenho v on v.empid = e.empid
					WHERE
						e.empnumeroprocesso = '{$arrProcesso['pronumeroprocesso']}'
						and empcodigoespecie not in ('03', '13', '02', '04')
						and v.saldo > 0
					/*group by
                    	e.empid,
                        e.empnumero,
						vps.vpsvinculacao,
                        e.empfonterecurso,
                        e.empsituacao,
                        e.empcnpj,
                        en.entnome,
						e.empprotocolo
					having sum(v.saldo) > 0*/";
		} elseif( $arrProcesso['sisid'] == 14 ){
			$sql = "SELECT distinct
						'<input type=hidden id='|| e.empid ||' value=\"'|| v.saldo ||'\"/> <input type=radio class=teste name=empid value='|| e.empid ||' onclick=\"verDadosPagamento(this.value);\" '|| CASE WHEN e.empnumero IS NULL THEN 'disabled' ELSE '' END  ||' />' as radio,
						CASE WHEN empnumero IS NULL THEN 'Aguardando efetivação' ELSE empnumero END as empnumero,
						vps.vpsvinculacao,
						e.empfonterecurso||'&nbsp;' as empfonterecurso,
                        --sum(v.saldo) as valor,
                        v.saldo as valor,
	                    (SELECT
                            sum(sub_p.pagvalorparcela) AS valor_pago
                        FROM par.pagamento  sub_p 
                        	INNER JOIN par.empenho sub_e ON sub_e.empid = sub_p.empid and sub_e.empstatus = 'A' 
                        WHERE
                            sub_p.pagsituacaopagamento not ilike '%CANCELADO%' 
                            AND sub_p.pagstatus = 'A'
                            and sub_e.empcodigoespecie not in ('03', '13', '02', '04')
                            AND sub_e.empid = e.empid
                        ) AS valor_pago,
						empsituacao,
						public.formata_cpf_cnpj(e.empcnpj) as empcnpj,
						iue.iuenome as entnome,
						e.empprotocolo||'&nbsp;' as empprotocolo
					FROM
						par.empenho e
                    LEFT  JOIN par.vinculacaoptressigef	vps ON vps.vpsptres = e.empcodigoptres 
					LEFT JOIN par.instrumentounidadeentidade iue ON iue.iuecnpj = e.empcnpj AND iue.iuestatus = 'A' and empstatus = 'A'
					inner join par.v_saldo_por_empenho v on v.empid = e.empid
					WHERE
						e.empnumeroprocesso = '{$arrProcesso['pronumeroprocesso']}'
						and empcodigoespecie not in ('03', '13', '02', '04')
						and v.saldo > 0
					/*group by
                    	e.empid,
                        e.empnumero,
						vps.vpsvinculacao,
                        e.empfonterecurso,
                        e.empsituacao,
                        e.empcnpj,
                        iue.iuenome,
						e.empprotocolo
					having sum(v.saldo) > 0*/";
		}
		//ver(simec_htmlentities($sql),d);
		$cabecalho = array("&nbsp;","N° do Empenho", "N° da Vinculação", "Fonte de Recurso", "Valor empenho(R$)", "Valor pago neste empenho(R$)", "Situação empenho","CNPJ","Entidade","Nº protocolo");
		$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	}

}
/*
 * Função Dados Pagamento
* Toda alteração nessa função faz com que seja necessária uma análise das funcionalisades de validação de valores página de pagamento do PAC
* */
function dadosPagamento($dados) {
	global $db;
	
	if(!$_SESSION['par_var']['proid']) die("Processo não identificado. Selecione novamente o processo.");
	
	echo "<input type=hidden name=empid id=empid value=".$dados['empid'].">";

	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Inserir nova parcela</td></tr>";
	echo "</table>";

	$sldid = $_REQUEST['sldid'];
	$clauseSlidCase = "";
	$clauseSlidEnd = "";
	$sldpercpagamento ="
			array_to_string(array(SELECT sd.sldid||' - '||
			
			
				round(
								(
									coalesce(sd.sldpercpagamento,0) 
									- 
									(
										((coalesce(
											(
												select  sum(pg.pagvalorparcela) from par.pagamentodesembolsoobras pd
												INNER JOIN  par.pagamentoobrapar po ON po.popid = pd.popid 
												INNER JOIN  par.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A'
												WHERE pd.sldid = sd.sldid
												AND pdostatus = 'A'
											)
											,0
										)) * 100) / po.prevalorobra 
									)
								
								),2
							 )
			
			
				||'%'
				FROM obras2.solicitacao_desembolso sd 
					inner join workflow.documento d ON d.docid = sd.docid
					left join par.pagamentodesembolsoobras pdo on pdo.sldid = sd.sldid and pdo.pdostatus = 'A'
				WHERE sd.obrid = obr.obrid
					and d.esdid = 1576 /*situação do workflow deferido*/
					and sd.sldstatus = 'A'
				ORDER BY sd.slddatainclusao desc), '<br>' ) as sldpercpagamento	,
	";
	// Caso seja uma solicitação da tela de pagamento Desembolso ele só irá disponibilizar o check para a obra relativa ao desenbolso
	if( ( $sldid ) && ($sldid != '') && ($sldid > 0 ) )
	{
		$preid = $_REQUEST['preidobrid'];
		$clauseSlidCase = "CASE WHEN po.preid = {$preid} THEN";
		$clauseSlidEnd = "ELSE
				''
				END";
		
		$sldpercpagamento ="
		{$clauseSlidCase}
			(	SELECT 
					sd.sldid||' - '
					||
						
						
						
						round(
								(
									coalesce(sd.sldpercpagamento,0) 
									- 
									(
										((coalesce(
											(
												select  sum(pg.pagvalorparcela) from par.pagamentodesembolsoobras pd
												INNER JOIN  par.pagamentoobrapar po ON po.popid = pd.popid 
												INNER JOIN  par.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A'
												WHERE pd.sldid = {$sldid}
												AND pdostatus = 'A'
											)
											,0
										)) * 100) / po.prevalorobra 
									)
								
								),2
							 )
						
						
							
					||'%'||'&nbsp;<input type=hidden name=sldid['||po.preid||'][] value='||sd.sldid||'>'
				FROM 
					obras2.solicitacao_desembolso sd
				WHERE 
					sd.obrid = obr.obrid
					and sd.sldstatus = 'A'
					AND sd.sldid = {$sldid}
				ORDER BY sd.slddatainclusao desc
			) 
		{$clauseSlidEnd}	
			as sldpercpagamento	,
	";
	}
	
	$chk = "
			
			{$clauseSlidCase}
			CASE WHEN (SELECT SUM(popvalorpagamento) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid WHERE p2.preid = po.preid AND pag2.pagstatus = 'A') > 0 AND 
						po.prevalorobra <> (SELECT SUM(popvalorpagamento) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid WHERE p2.preid = po.preid AND pag2.pagstatus = 'A')
				THEN
					CASE WHEN (	SELECT
									coalesce(
										(
											SELECT distinct popdataprazoaprovado
											FROM obras.preobraprorrogacao 
											WHERE popstatus = 'A' AND popvalidacao = 't' AND preid = po.preid
										),
										(pagdatapagamento+
											(
												720+
												coalesce(
													(SELECT sum(popqtddiasaprovado)
													FROM obras.preobraprorrogacao pop
													WHERE pop.preid = po.preid AND popstatus = 'A' AND popdatavalidacao IS NOT NULL)
												,0)
											)::integer
										)::date 
									)
									< 
									now()::date
								FROM
									par.pagamento
								WHERE
									pagid = ( SELECT min(pagid) FROM par.pagamentoobrapar pob WHERE pob.preid = po.preid )
									and pagstatus = 'A' )
					THEN
						'<img style=cursor:pointer; src=../imagens/atencao.png title=\"Esta obra encontra-se vencida.\" 
							onclick=\"alert(''A Obra está vencida desde '||  
											to_char((	SELECT
													coalesce(
														(
															SELECT distinct popdataprazoaprovado 
															FROM obras.preobraprorrogacao 
															WHERE popstatus = 'A' AND popvalidacao = 't' AND preid = po.preid
														),
														(pagdatapagamento+
															(
																720+
																coalesce(
																	(SELECT sum(popqtddiasaprovado)
																	FROM obras.preobraprorrogacao pop
																	WHERE pop.preid = po.preid AND popstatus = 'A' AND popdatavalidacao IS NOT NULL)
																,0)
															)::integer
														)::date 
													)
												FROM
													par.pagamento
												WHERE
													pagid = ( SELECT min(pagid) FROM par.pagamentoobrapar pob WHERE pob.preid = po.preid) and pagstatus = 'A'), 'DD/MM/YYYY') 
											||'. Por favor verifique as divergências.'')\" >'
					ELSE
						CASE 
							WHEN ((SELECT ROUND(SUM(eobvalorempenho)) FROM par.empenhoobrapar WHERE preid = po.preid and eobstatus = 'A' AND empid = eo.empid) - coalesce(ep.vrlcancelado, 0)) <= ROUND(SUM(p.popvalorpagamento)) THEN '<a title=\"Empenho 100% pago.\" style=\"cursor:pointer\"> EMP </a>' 
							WHEN ROUND(po.prevalorobra) <= (SELECT ROUND(SUM(popvalorpagamento)) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid AND pagstatus = 'A' AND pagsituacaopagamento not ilike '%CANCELADO%' WHERE p2.preid = po.preid) THEN '<a title=\"Obra 100% paga.\" style=\"cursor:pointer\"> OBR </a>' 
							ELSE '<input type=\"checkbox\" name=\"preid[]\" value=\"'||po.preid||'\" onclick=\"marcarPreObra(this);\">'
						END
					END
				ELSE
					CASE 
						WHEN ((SELECT ROUND(SUM(eobvalorempenho)) FROM par.empenhoobrapar WHERE preid = po.preid and eobstatus = 'A' AND empid = eo.empid) - coalesce(ep.vrlcancelado, 0)) <= ROUND(SUM(p.popvalorpagamento)) THEN '<a title=\"Empenho 100% pago.\" style=\"cursor:pointer\"> EMP </a>' 
						WHEN ROUND(po.prevalorobra) <= (SELECT ROUND(SUM(popvalorpagamento)) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid AND pagstatus = 'A' AND pagsituacaopagamento not ilike '%CANCELADO%' WHERE p2.preid = po.preid) THEN '<a title=\"Obra 100% paga.\" style=\"cursor:pointer\"> OBR </a>' 
						ELSE '<input type=\"checkbox\" name=\"preid[]\" value=\"'||po.preid||'\" onclick=\"marcarPreObra(this);\">'
					END
			END
			{$clauseSlidEnd}
			";
	/* Alterei a coluna obr para pegar o obrid da tabela do obras, 
	 * poi o que se encontra na tabela preobra está errado!
	 * */	
	$sql = "SELECT DISTINCT
				'<center>'|| $chk ||'</center>' as chk,
				obr.obrid,
				po.preid,
				'<div id=td_nomeobra_'||po.preid||' style=\"display: none\">'||po.predescricao||'</div>' || 
				'<img style=\"cursor:pointer\" src=\"../imagens/fluxodoc.gif\"- title=\"Resumo de Solicitação de Desembolso\" onclick=\"abrirSolicitacaoDesembolso('||obr.obrid||')\">
				 <img style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"abrirDadosObras('||po.preid||', '||po.preano||')\"> ' || po.preid||' - '||po.predescricao as predescricao,
				'<center>'||obr.obrpercentultvistoria::integer||' %</center>' as perc,
				((SELECT SUM(eobvalorempenho) FROM (SELECT DISTINCT eobvalorempenho, preid, empid FROM par.empenhoobrapar WHERE preid = po.preid and eobstatus = 'A' AND empid = eo.empid) as foo) - coalesce(ep.vrlcancelado, 0) ) as valorempenho,
				po.prevalorobra as valorobra,
				SUM(p.popvalorpagamento) as pagamentoempenho,
				(SELECT SUM(popvalorpagamento) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid WHERE p2.preid = po.preid AND pag2.pagstatus = 'A' and pagsituacaopagamento not ilike '%cancelado%') as pagamentooutros,
				((SELECT SUM(popvalorpagamento) FROM par.pagamentoobrapar p2 INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid WHERE p2.preid = po.preid AND pag2.pagstatus = 'A' and pagsituacaopagamento not ilike '%cancelado%')/po.prevalorobra)*100 as perc_pago,
				'0' as execucao_fisica,
				
				{$sldpercpagamento}	
				
				'<input type=text class=disabled onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);cacularValorPagamento(this, '||po.preid||');\" onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);cacularValorPagamento(this, '||po.preid||');\" maxlength=3 size=7 id=porcent name=porcent['||po.preid||'] style=text-align:; disabled>' as porcentpagamento,
				'<input type=text class=disabled onblur=MouseBlur(this); onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);cacularValorPagamento(this, '||po.preid||');\" maxlength=20 size=21 id=valorpagamentoobra name=valorpagamentoobra['||po.preid||'] style=text-align:; disabled>' as valorpagamento,
				count( p.preid ) + 1 || '<input type=hidden name=parcela['||po.preid||'] id=parcela['||po.preid||'] value=' || count( p.preid ) + 1 || '>' as parcelaatual, 
				esd.esddsc as situacao
			FROM par.empenho e
			INNER JOIN ( SELECT DISTINCT preid, empid, eobvalorempenho FROM par.empenhoobrapar WHERE eobstatus = 'A' ) eo ON eo.empid = e.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
			INNER JOIN obras.preobra 			po  ON po.preid = eo.preid AND po.prestatus = 'A'
			LEFT  JOIN obras2.obras 			obr ON obr.preid = po.preid AND obr.obridpai IS NULL AND obr.obrstatus = 'A'
			LEFT  JOIN workflow.documento 		obrd ON obrd.docid = obr.docid
			LEFT  JOIN workflow.estadodocumento esd on esd.esdid = obrd.esdid
			LEFT  JOIN par.pagamento 			pag ON pag.empid = eo.empid AND pag.pagstatus = 'A'
			LEFT  JOIN par.pagamentoobrapar 	p   ON p.preid = po.preid AND pag.pagid = p.pagid
			left join (select sum(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.preid
                    from par.empenhoobrapar eb
                        inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
                    where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
                    group by e1.empidpai, eb.preid
            ) as ep on ep.empidpai = e.empid and ep.preid = eo.preid
			WHERE 
				eo.empid='".$dados['empid']."' 
				AND eo.preid IN (SELECT distinct eo.preid FROM par.processoobraspar pp
            					 INNER JOIN par.empenho 		e  ON e.empnumeroprocesso = pp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                          		 INNER JOIN par.empenhoobrapar 	eo ON eo.empid = e.empid and eobstatus = 'A'
                                 WHERE
                                 		pp.prostatus = 'A' and
                                      e.empid = {$dados['empid']}) 
              	AND e.empcodigoespecie not in ('03', '13', '02', '04')
			GROUP BY po.preid, po.predescricao, po.prevalorobra, obr.obrid, eo.eobvalorempenho, eo.empid, esd.esddsc, ep.vrlcancelado
			ORDER BY predescricao";
//  	ver($sql,d);	
	$cabecalho = Array("&nbsp;","ID da obra","Descrição da Obra", "% de Execução<br> da Obra", "Valor Empenhado (R$)", "Valor da Obra (R$)", "Valor Pago Nesse Empenho (R$)", 
						"Valor Total Pago (R$)", "% Pago da Obra", "Validação <br>da primeira<br> parcela", "<center>ID solicitação - % Deferido</center>", "% Pagamento", "Valor pagamento (R$)", "Parcela", "Situação");
	$arr = $db->carregar($sql);
	
	$sql = "SELECT distinct mdo.mdoqtdvalidacao from par.processoobraspar pp
			    inner join par.vm_documentopar_ativos dp on dp.proid = pp.proid
			    inner join par.modelosdocumentos mdo ON mdo.mdoid = dp.mdoid
			    inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
			    inner join par.empenhoobrapar eo on eo.empid = e.empid and eobstatus = 'A'
			where
				pp.prostatus = 'A'  and
			    e.empid = ".$dados['empid'];
	
	$mdoqtdvalidacao = $db->pegaUm($sql);
	
	if( $mdoqtdvalidacao > 0 ){
		$sql = "select distinct count(eo.preid) 
				from par.processoobraspar pp
				    inner join par.vm_documentopar_ativos dp on dp.proid = pp.proid 
				    inner join par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
				    inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
				    inner join par.empenhoobrapar eo on eo.empid = e.empid and eobstatus = 'A'
				where
					pp.prostatus = 'A'  and
				    e.empid = {$dados['empid']}
				    and dv.dpvdatavalidacao is not null";
		$boDocValidado = $db->pegaUm($sql);
		$boDocValidado = $boDocValidado > 0 ? true : false;
	} else {
		$boDocValidado = true;
	}
	
	//trato o detalhamento do pagamento
	$xx = 0;
	$arRegistro = array();
	if( is_array($arr) ){
		foreach( $arr as $key => $value ){
			foreach ($value as $key2 => $d) {
				$arRegistro[$xx][$key2] = $value[$key2];
			}
			$xx++;
		}
	}

	echo '<table align="center" cellspacing="0" cellpadding="2" border="0" width="95%" class="listagem">';
	echo '<thead>';
	echo '<tr>';
	foreach( $cabecalho as $cab ){
		echo '<td align="" bgcolor="" valign="top"><strong>'.$cab.'</strong></td>';
	}
	echo '</tr>';
	echo "</thead>";
	$total  = 0;
	$total2 = 0;
	$total3 = 0;
	$total4 = 0;
	foreach( $arRegistro as $arr ){
		
		$obridV = $arr['obrid'];
		$preidV = $arr['preid'];
		$preidobrid = $_REQUEST['preidobrid'];
		
		if( ($preidobrid == $obridV ) || ($preidobrid == $preidV ) )
		{
			echo '<tr style="background-color:#888888" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		}
		else
		{
			echo '<tr bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		}
		
		
		foreach( $arr as $k => $v ){
			if( $k != 'preid' ){
				if( $k == 'valorobra' ){
					echo "<td><input type=hidden name=valorobra[".$arr['preid']."] id=valorobra value=".number_format($v,2,'.','').">".number_format($v,2,',','.')."</td>";
					$total = $total + $v;
				} elseif( $k == 'pagamentoempenho' ) {
					echo "<td><a onmouseover=\"SuperTitleAjax('/par/par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&titleFor=".$arr['preid']."&empid=".$dados['empid']."&tp=1',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >".number_format($v,2,',','.')."</a>
								<input type=hidden name=valorpagoempenho[".$arr['preid']."] id=valorpagoempenho value=".number_format($v,2,'.','')."></td>";
					$total2 = $total2 + $v;
				} elseif( $k == 'pagamentooutros' ) {
					echo "<td><a onmouseover=\"SuperTitleAjax('/par/par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&titleFor=".$arr['preid']."&empid=".$dados['empid']."&tp=2',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >".number_format($v,2,',','.')."</a>
							<input type=hidden name=pagamentooutros[".$arr['preid']."] id=pagamentooutros value=".number_format($v,2,'.','')."></td>";
					$total3 = $total3 + $v;
				} elseif( $k == 'valorempenho' ) {
					echo "<td><input type=hidden name=valorempenhado[".$arr['preid']."] id=valorempenhado value=".number_format($v,2,'.','').">".number_format($v,2,',','.')."</td>";
					$total4 = $total4 + $v;
				} elseif( $k == 'perc_pago' ) {
					echo "<td><center><input type=hidden name=valorempenhado[".$arr['preid']."] id=valorempenhado value=".number_format($v,2,'.','').">".number_format($v,0,',','.')." %</center></td>";
				} elseif( $k == 'execucao_fisica' ) {
					if( $arr['obrid'] != '' ){
						$sql = "SELECT
									coalesce(v.vldstatushomologacao, 'N') as homologacao,
									coalesce(v.vldstatus25exec, 'N') as validacao25,
									coalesce(v.vldstatus50exec, 'N') as validacao50
								FROM obras2.validacao v
								WHERE v.obrid = {$arr['obrid']}";
						$arValidacao = $db->pegaLinha($sql);
					}
					$execusaoFisica = (int)1;
					if( $arValidacao['homologacao'] == 'S' ) $execusaoFisica++;
					if( $arValidacao['validacao25'] == 'S' ) $execusaoFisica++;
					if( $arValidacao['validacao50'] == 'S' ) $execusaoFisica++;
					echo "<td align=center >".($execusaoFisica > 1 ? 'Sim' : 'Não')."</td>";
				} else {
					echo '<td>'.$v.'</td>';
				}
			}
		}
		echo '';
		echo '</tr>';
	}
	$geral = $total2 + $total3;

	//total

	echo "<tr bgcolor=#E9E9E9>";
	echo "<td><input type='hidden' name='hdvalor' id='hdvalor' value='".$geral."'><b>Total:</b></td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td><input type='hidden' name='totalempenho' id='totalempenho' value='".$total4."'>".number_format($total4,2,',','.')."</td>";
	echo "<td>".number_format($total,2,',','.')."</td>";
	echo "<td><input type='hidden' name='totalpagnesseempenho' id='totalpagnesseempenho' value='".$total2."'>".number_format($total2,2,',','.')."</td>";
	echo "<td>".number_format($total3,2,',','.')."</td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td align=center>".campo_texto('valorpagamento','N','S','','20','20','[.###],##','','','','','id="valorpagamento" readonly=readonly')."</td>";
	echo "<td colspan='2'></td>";
	echo "</tr>";

	$sql = "SELECT (empvalorempenho - coalesce(vrlcancelado, 0)) FROM par.empenho e
						left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
                    group by 
                        empnumeroprocesso,
                        empcodigoespecie,
                        empidpai) as ep on ep.empidpai = e.empid WHERE e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and e.empid='".$dados['empid']."'";
	$valor = $db->pegaUm( $sql );

	echo "<tr bgcolor=#DCDCDC>";
	echo "<td colspan=12 align='right'>";
	echo "<b>Restante a pagar(R$):</b>";
	echo "</td>";
	echo "<td>";
	echo number_format(($valor-$total2),2,',','.');
	echo "</td>";
	echo "<td>";
	echo "<input type='hidden' id='valempid' value='".($valor-$total2)."'>";
	echo "</td>";
	echo "<td></td>";
	echo "</tr>";

	// parcelas

	$parcela = $db->pegaUm("SELECT COALESCE(MAX(p.pagparcela),0) as parcela FROM par.pagamento p WHERE p.empid = ".$dados['empid']." AND p.pagstatus='A'");

	$sql_mes = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos";

	echo "<tr bgcolor=#DCDCDC>";
	echo "<td align=center colspan=15 ><input type=hidden name=pagparcela value=".($parcela+1)." />
									<input type=hidden name=docvalidado id=docvalidado value=".($boDocValidado ? 'true' : 'false')." />";
			//<b>Parcela: ".($parcela+1)."</b><input type=hidden name=pagparcela value=".($parcela+1)." />&nbsp;&nbsp;&nbsp;
	echo "Mês: ".$db->monta_combo('mes', $sql_mes, 'S', 'Selecione', '', '', '', '', 'S', 'mes', true, date("m"))."&nbsp;&nbsp;&nbsp;";
	echo "Ano: ".$db->monta_combo('ano', $sql_ano, 'S', 'Selecione', '', '', '', '', 'S', 'ano', true, date("Y"))."&nbsp;&nbsp;&nbsp;";
	
	$perfil = pegaPerfilGeral(); 
	if( (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
		in_array(PAR_PERFIL_PAGADOR ,$perfil)) && $boDocValidado
	){
		echo "<input type=\"button\" id=\"solicitar\" name=\"solicitar\"  value=\"Solicitar pagamento\" disabled=\"disabled\" onclick=\"solPag();\" />";
	}else{
		echo "<input disabled=disabled type=\"button\" id=\"solicitar\" name=\"solicitar\"  value=\"Solicitar pagamento\" disabled=\"disabled\"/>";	
	}
	if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){
		echo "<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visPag(); />";
	}
	echo "</td></tr></table><br>";

	carregaDadosPagamento( $dados['empid'] );
}


function executarPagamento($dados) {
	global $db;
	
	$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
	$totalpagamento = $db->pegaUm("SELECT SUM(pagvalorparcela) FROM par.pagamento WHERE empid='".$dados['empid']."' AND pagstatus='A'");
	$totalempenho   = $db->pegaUm("SELECT (empvalorempenho - coalesce(vrlcancelado, 0)) FROM par.empenho e
											left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
					                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
					                    group by 
					                        empnumeroprocesso,
					                        empcodigoespecie,
					                        empidpai) as ep on ep.empidpai = e.empid WHERE e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and e.empid='".$dados['empid']."'");
	$soma = ($totalpagamento+$valor);

	 if( round($soma,2) > round($totalempenho,2) ) {
		die("SIMEC INFORMA : Total de pagamento esta maior que o valor do empenho");
	}

	/* if($dados['preid']) {
		foreach($dados['preid'] as $preid) {

			$sql = "SELECT DISTINCT
						oi.obrid as id,
						oi.preid as idpreobra,
						oi.preid||' - '||oi.obrnome as descricao,
						CASE WHEN (va.vldstatushomologacao = 'N' OR va.vldstatushomologacao IS NULL) THEN 'nao' ELSE 'sim' END as homologacao,
						CASE WHEN (va.vldstatus25exec = 'N' OR va.vldstatus25exec IS NULL) THEN 'nao' ELSE 'sim' END as execucao25,
						CASE WHEN (va.vldstatus50exec = 'N' OR va.vldstatus50exec IS NULL) THEN 'nao' ELSE 'sim' END as execucao50,
						CASE WHEN oi.obrpercentultvistoria IS NULL THEN '0.00 %' ELSE oi.obrpercentultvistoria||' %' END as percexec
					FROM
						obras2.obras oi
					INNER JOIN obras2.empreendimento 	emp ON emp.empid = oi.empid
					LEFT  JOIN obras2.arquivosobra 		ao  ON ao.obrid = oi.obrid AND ao.tpaid = 24 AND ao.aqostatus = 'A'
					LEFT  JOIN public.arquivo 			ar  ON ar.arqid = ao.arqid AND ar.arqtipo <> 'image/jpeg' AND ar.arqtipo <> 'image/png' AND ar.arqtipo <> 'image/gif'
					LEFT  JOIN obras2.validacao 			va  on va.obrid = oi.obrid
					WHERE
						emp.orgid = 3
						AND oi.obrstatus = 'A'
						AND obridpai IS NULL
						AND oi.preid =".$preid;

			$dadospre = $db->pegaLinha($sql);

			$sql = "SELECT
						count(pob.preid) + 1 as parcela
					FROM par.empenhoobrapar  eob
					INNER JOIN par.pagamentoobrapar	pob ON pob.preid = eob.preid
					INNER JOIN par.pagamento 		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A' and pag.pagsituacaopagamento not ilike '%cancelado%'
					WHERE
						eob.preid = {$preid}
						AND eob.eobstatus = 'A' 
						AND eob.empid = {$dados['empid']}";

			$parcela = $db->pegaUm( $sql );

			switch($parcela) {
				case "2":
					if($dadospre['homologacao']=="nao") {
						die("SIMEC INFORMA : ".$dadospre['descricao']." não foi homologada");
					}
					break;
				
				case "3":
					if($dadospre['execucao25']=="nao") {
						die("SIMEC INFORMA : ".$dadospre['descricao']." não foi executada 25%");
					}
					break;
				case "4":
					if($dadospre['execucao50']=="nao") {
						die("SIMEC INFORMA : ".$dadospre['descricao']." não foi executada 50%");
					}
					break;
			}

		}
	} */
	
	$obHabilita = new Habilita();
	
	$inuid = $db->pegaUm( "select inuid FROM par.processoobraspar where prostatus = 'A'  and  proid = {$_SESSION['par_var']['proid']}" );
	$cnpj 		= $obHabilita->pegaCnpj($inuid);
	$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj);
	
	if($habilitado == 'Habilitado'){
		$res_acc = atualizaDadosContaCorrentePag( $dados );
		
		if( $res_acc ){
			$res_cc = consultarContaCorrente($dados);
			if(!$res_cc){
				solicitarContaCorrente($dados);
				echo "Conta corrente solicitada neste momento.";
				return false;
			}
			
			if($res_cc=="cc_criado_sucesso") {
				$res_cc = consultarContaCorrente($dados);
			}
		
			if($res_cc == true){
				$res_se = solicitarPagamento($dados);
			}
		}else{
			solicitarContaCorrente($dados);
		}
	}else{
		if( $habilitado = "Em diligência" ){
			echo "A solicitação de pagamento não pode ser efetuada, pois o município não apresentou a documentação exigida para habilitação junto ao FNDE.";
		}else{
			echo $habilitado;
		}
	}
}


function atualizaDadosContaCorrentePag($dados) {
	global $db;
	
    $dadosse = $db->pegaLinha("SELECT p.pronumeroprocesso, p.muncod, p.probanco, p.proagencia, p.prodatainclusao, p.usucpf, p.proseqconta, p.protipo, 
									p.seq_conta_corrente, p.nu_conta_corrente, p.procnpj
    						   FROM par.processoobraspar p
    						   WHERE p.prostatus = 'A' and proid = {$_SESSION['par_var']['proid']}");
	
    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo = $dadosse['pronumeroprocesso'];
    	$tp_processo = 1; // O que vai ser no PAR
		
    	$nu_cnpj_favorecido=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobraspar WHERE prostatus = 'A'  and pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");
    	
    	/*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobraspar WHERE pronumeroprocesso = '{$dadosse['pronumeroprocesso']}'");
        }else{
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
        }*/
	    
    }

    $data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$somente_conta_ativa	= 'S';
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
			//$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
    		$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
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
			$erros = $xml->status->error->message;
			if(count($erros)>0) {	
				foreach($erros as $err) {	
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}				
			echo mensagem('ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF', $mensagem);
				
			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'atualizaDadosContaCorrenteObraPar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

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
						  	par.processoobraspar  
						SET 
						  	probanco = $nu_banco,
						  	proagencia = $nu_agencia,
						  	proseqconta = $seq_solic_cr,
						  	seq_conta_corrente = $seq_conta,
						  	nu_conta_corrente = $nu_conta_corrente						 
						WHERE 
						  	proid = {$_SESSION['par_var']['proid']}";
						  	
				$db->executar($sql);
			} else {					
				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);				
				echo mensagem('ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF', $mensagem);
				
				$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'atualizaDadosContaCorrenteObraPar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$db->executar($sql);
				$db->commit();
	
			    return false;
			}
			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'atualizaDadosContaCorrenteObraPar - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}
}

function consultarAndamentoContaCorrente($dados){
		global $db;
		$data_created 		= date("c");
		$usuario 			= $dados['wsusuario'];
		$senha   			= $dados['wssenha'];
		$numero_de_linhas 	= '200';

        $dadoscc = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo, trim(procnpj) as procnpj FROM par.processoobraspar WHERE prostatus = 'A'  and proid='".$_SESSION['par_var']['proid']."'");

        $co_programa_fnde = $db->pegaUm("SELECT tipprogramafnde FROM execucaofinanceira.tipoprocesso WHERE tipid = 2");
        
        if($dadoscc) {
	       $nu_processo = $dadoscc['pronumeroprocesso'];
        }
       /* if($dadoscc['protipo'] == 'P'){
        	$co_programa_fnde = "BW";
        } else{
        	$co_programa_fnde = "CN";
        }*/
		$nu_identificador = $dadoscc['procnpj'];
		
		/*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_identificador = $dadoscc['procnpj'];
        }else{
        	// CNPJ da prefeitura
			$nu_identificador = $db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadoscc['muncod']."'");
        }*/

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
        <nu_processo>$nu_processo</nu_processo>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
        <somente_conta_ativa>N</somente_conta_ativa>
        <numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;


	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'https://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	} else {
		$urlWS = 'https://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	}

	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
			->execute();


	$xmlRetorno = $xml;

    $xml = simplexml_load_string( stripslashes($xml));

	$result = (integer) $xml->status->result;
	if(!$result) {
		$erros = $xml->status->error->message;
		
		echo mensagem('CONSULTAR ANDAMENTO DE CONTA CORRENTE', $erros);

    	$sql = "INSERT INTO par.historicowsprocessoobrapar(
			    	proid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$_SESSION['par_var']['proid']."',
			    		'Consultar andamento de conta - Erro - PROID = ".$_SESSION['par_var']['proid']."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		 return false;

	}else{
		$sql = "INSERT INTO par.historicowsprocessoobrapar(
			    	proid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$_SESSION['par_var']['proid']."',
			    		'Consultar andamento de conta - Sucesso - PROID =  ".$_SESSION['par_var']['proid']."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		$db->executar("UPDATE par.processoobraspar SET proseqconta='".$xml->body->row->seq_solic_cr."' WHERE proid='".$_SESSION['par_var']['proid']."'");
		$db->commit();

		return true;
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

        $dadoscc = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo, trim(procnpj) as procnpj FROM par.processoobraspar WHERE prostatus = 'A' and proid='".$_SESSION['par_var']['proid']."'");

        $co_programa_fnde = $db->pegaUm("SELECT tipprogramafnde FROM execucaofinanceira.tipoprocesso WHERE tipid = 2");
        
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
		
        $nu_identificador= $dadoscc['procnpj'];
        
		/*if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_identificador= $dadoscc['procnpj'];
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
       // $nu_sistema="5";
         $nu_sistema="7";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
    //    if($dadoscc['protipo'] == 'P') $co_programa_fnde="BW";
     //   else $co_programa_fnde="CN";



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

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		if(!$result) {
			$erros = $xml->status->error->message->text;			
			echo mensagem('SOLICITAÇÃO DE CONTA CORRENTE', $erros);

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {
			$erros = $xml->status->error->message->text;
			echo mensagem('SOLICITAÇÃO DE CONTA CORRENTE', $erros);
			
		    $db->executar("UPDATE par.processoobraspar SET proseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE prostatus = 'A' and proid='".$_SESSION['par_var']['proid']."'");

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
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
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		echo mensagem('SOLICITAÇÃO DE CONTA CORRENTE', $erroMSG);
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
		
		if(!$_SESSION['par_var']['proid']) {
			echo "------ MENSAGEM SIMEC - PROCESSO NÃO ENCONTRADO ------<br>";
			echo "Foram encontrados alguns problemas internos. Feche a tela e clique novamente na lista de processo.";
			exit;
		}
		
        $proseqconta = $db->pegaUm("SELECT proseqconta FROM par.processoobraspar WHERE prostatus = 'A' and proid='".$_SESSION['par_var']['proid']."'");

	if(!$proseqconta) {
		$existeAndamentoConta = consultarAndamentoContaCorrente($dados);
		if(!$existeAndamentoConta){
		 // RETORNO FALSE - SE NÃO EXISTE CONTA  EM ANDAMENTO PARA SER ABERTA SOLICITA CONTA.
			$r = solicitarContaCorrente($dados);
			if($r){
				return "cc_criado_sucesso";
	       	}else{
	       		return false;
	       	}
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
        <seq_solic_cr>$proseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
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

		    $result = (integer) $xml->status->result;
		    $resultConsultaConta = (integer) $xml->body->row->co_situacao_conta;

		    $sql = "INSERT INTO par.historicowsprocessoobrapar(
					    	proid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$_SESSION['par_var']['proid']."',
					    		'consultarContaCorrente',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			if($result != 1 ) { // 1 = sucesso
				
				echo mensagem('CONSULTA DE CONTA CORRENTE', $xml->status->error->message->text);

		    	if( $resultConsultaConta == 24 ){
		    		$r = solicitarContaCorrente($dados);
		    		if($r){
						return "cc_criado_sucesso";
			       	}else{
			       		return false;
			       	}
		    		//die("MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.");
			    } elseif( $resultConsultaConta == 25 ){
			    	$r = solicitarContaCorrente($dados);
			    	if($r){
						return "cc_criado_sucesso";
			       	}else{
			       		return false;
			       	}
			    	//die("MSG SIMEC : Conta Corrente Bloqueada Definitivamente.");
			    } elseif( $resultConsultaConta == 14 ){
			    	$r = solicitarContaCorrente($dados);
			    	if($r){
						return "cc_criado_sucesso";
			       	}else{
			       		return false;
			       	}
			    	//die("MSG SIMEC : Conta Corrente Inativa.");
			    }

		    	return false;
		    } else {
		    	$statusContaSucessos = array('13','11','09');
				if(in_array($resultConsultaConta, $statusContaSucessos )){
				    if($xml->body->row->seq_conta) {
				    	$db->executar("UPDATE par.processoobraspar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE proseqconta='".$proseqconta."'");
				    	$db->commit();
				    }

					$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
					$msg.= "*** Detalhes da consulta ***<br>";
					$msg.= "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."<br>";
					$msg.= "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."<br>";
					$msg.= "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")<br>";
					//return $result;
					
					echo mensagem('CONSULTA DE CONTA CORRENTE', $msg);

					return true;
				}else{
					$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
					$msg.= "* A conta corrente não está ativa.<br>";

			    	if( $resultConsultaConta == 24 ){
			    		$msg.= "MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.";
				    } elseif( $resultConsultaConta == 25 ){
				    	$msg.= "MSG SIMEC : Conta Corrente Bloqueada Definitivamente.";
				    } elseif( $resultConsultaConta == 14 ){
				    	$msg.= "MSG SIMEC : Conta Corrente Inativa.";
				    }				    
				    echo mensagem('ERRO AO CONSULTAR CONTA CORRENTE', $msg);
					return false;
				}
		    }

		} else {
			$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
			$msg.= "*** Erro de integração entre SIMEC e SIGEF ***<br>";
			$msg.= "* Descrição do Erro:O sequencial da conta no SIMEC não foi encontrado.<br>";

	    	if( $resultConsultaConta == 24 ){
	    		$msg.= "MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.";
		    } elseif( $resultConsultaConta == 25 ){
		    	$msg.= "MSG SIMEC : Conta Corrente Bloqueada Definitivamente.";
		    } elseif( $resultConsultaConta == 14 ){
		    	$msg.= "MSG SIMEC : Conta Corrente Inativa.";
		    }
			echo mensagem('CONSULTAR CONTA CORRENTE', $msg);
			return false;
		}



	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		echo mensagem('CONSULTAR CONTA CORRENTE', $erroMSG);
	}
}

function consultarEmpenho($dados) {
	global $db;

	try {
		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadosemp = $db->pegaLinha("SELECT * FROM par.empenho WHERE empid='".$dados['empid']."'");

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

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

	    $xml = simplexml_load_string( stripslashes($xml));

		$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
		$msg.= "*** Detalhes da consulta ***<br>";
		$msg.= "* Nº processo: ".$xml->body->row->processo."<br>";
		$msg.= "* CNPJ: ".$xml->body->row->nu_cnpj."<br>";
		$msg.= "* Valor(R$): ".number_format($xml->body->row->valor_ne,2,",",".")."<br>";
		$msg.= "* Data: ".$xml->body->row->data_documento."<br>";
		$msg.= "* Nº documento: ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."<br>";
		$msg.= "* Valor empenhado(R$): ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."<br>";
		$msg.= "* Saldo pagamento(R$): ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."<br>";
		$msg.= "* Situação: ".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."<br><br>";
		
		echo mensagem('CCONSULTA DE EMPENHO', $msg);

		$db->executar("UPDATE par.empenho SET empnumero='".$xml->body->row->numero_documento."',
											  ds_problema='".$xml->body->row->ds_problema."',
									  		  valor_total_empenhado=".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
											  valor_saldo_pagamento=".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").",
											  empsituacao='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'
					   WHERE empid='".$dados['empid']."'");

		$sql = "INSERT INTO par.historicoempenho(
           		usucpf, empid, hepdata, empsituacao, ds_problema, valor_total_empenhado,
            	valor_saldo_pagamento)
    			VALUES ('".$_SESSION['usucpf']."',
    					'".$dados['empid']."',
    					NOW(),
    					'".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."',
    					'".$xml->body->row->ds_problema."',
    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";

		$db->executar($sql);


		$db->commit();

		// simulando sem validacão do XML
		// return true;

		$result = (integer) $xml->status->result;

		if($result) {
			return false;
		} else {
		   	return true;
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo mensagem('CONSULTAR CONTA CORRENTE', $erroMSG);
	}
}

function solicitarPagamento($dados) {
	global $db;

	if(!$dados['empid']) {
		echo "Empenho não selecionado. Por favor, selecione um empenho";
		return false;
	}

	$data_created = date("c");

	$dadosse = $db->pegaLinha("SELECT emp.empcnpj, pro.proseqconta, pro.seq_conta_corrente,
									  emp.empnumeroprocesso, emp.empprogramafnde,
									  emp.empnumerosistema, emp.empanooriginal,
									  emp.empnumero, pro.pronumeroprocesso, trim(procnpj) as procnpj, empcodigonatdespesa
							   FROM par.empenho emp
							   INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A' and empstatus = 'A' 
							   WHERE empid='".$dados['empid']."'");
    if($dadosse) {

		// numero do processo (No desenvolvimento é fixo)
       /* if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){

            $usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_processo="23034655466200900";
			$nu_documento_siafi_ne = "340001";
//			$nu_cgc_favorecido = "12262713000102";
			$nu_cgc_favorecido = "15024029000180";
			$nu_seq_conta_corrente_favorec = "510793";

       	} else {*/
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
			$nu_processo = $dadosse['empnumeroprocesso'];
			$nu_documento_siafi_ne = substr($dadosse['empnumero'],strpos($dadosse['empnumero'], 'NE')+2);
			$nu_cgc_favorecido = $dadosse['empcnpj'];

	       	if($_SESSION['par_var']['esfera']=='estadual') {
	        	// CNPJ da prefeitura
				$nu_cgc_favorecido = $dadosse['procnpj'];

	        }else{
	        	// CNPJ da prefeitura
				$nu_cgc_favorecido = $dadosse['empcnpj'];
	        }




			$nu_seq_conta_corrente_favorec = $dadosse['seq_conta_corrente'];
       // }

		$nu_cpf_favorecido = null;
		$nu_banco = null;
		$nu_agencia = null;
		$nu_conta_corrente = null;
		$an_convenio_original = null;
		$nu_convenio_original = null;
		$nu_convenio_siafi = null;
		$nu_proposta_siconv = null;
		$termo_aditivo_original = null;
		$apostilamento_original = null;
		
		// Custeio
//		if( $dadosse['empcodigonatdespesa'] == 33404100 || $dadosse['empcodigonatdespesa'] == 33304100 || $dadosse['empcodigonatdespesa'] == 44304200 ){
		if( $dadosse['empcodigonatdespesa'] == 33404100 || $dadosse['empcodigonatdespesa'] == 33304100 ){
			$vl_custeio = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
			$vl_capital = "0";
		} else { // Capital
			$vl_custeio = "0";
			$vl_capital = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		}
		$an_referencia = date("Y");
		$sub_tipo_documento = "01";
		$nu_sistema = $dadosse['empnumerosistema'];
		$unidade_gestora = "153173";
		$gestao = "15253";
		//$co_programa_fnde = $dadosse['empprogramafnde'];
		$co_programa_fnde = $db->pegaUm("SELECT tipprogramafnde FROM execucaofinanceira.tipoprocesso WHERE tipid = 2");
		$parcela = $dados['pagparcela'];
		$darf = null;
		$tp_avaliador = null;
		$id_solicitante = null;
		
		$dadosNE = explode("NE", $dadosse['empnumero']);
		$an_exercicio = $dadosNE[0];
		/*
		$an_exercicio = $db->pegaUm("SELECT to_char(hepdata,'YYYY') as ano FROM par.historicoempenho
									 WHERE empid='".$dados['empid']."' ORDER BY hepdata ASC LIMIT 1");
		*/
		/*
		 * Se ele não tem o ano no historico eu pego o ano do cadastro do empenho.
		 * 
		 */
		if(!$an_exercicio){
			$an_exercicio = $db->pegaUm("SELECT to_char( empdata, 'YYYY' ) as ano FROM par.empenho WHERE empid='".$dados['empid']."'");
		}
		
		$nu_mes = sprintf("%02d", $dados['mes']);
		$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		
		if( $dados['tipo'] != 'visualiza' ){
			$sql = "SELECT distinct l.lwsid FROM par.logws l
					    inner join par.historicowsprocessoobrapar h ON l.lwsid = h.lwsid
					WHERE
					    h.proid = {$_SESSION['par_var']['proid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessoobrapar where proid = {$_SESSION['par_var']['proid']})
						and l.lwstiporequest = '05'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
						'lwstiporequest' 	=> '05',
		        		'usucpf' 			=> $_SESSION['usucpf']
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}
        	
        	$arrParam = array(
        			'mes' 				=> $dados['mes'],
        			'ano' 				=> $an_referencia,
        			'exercicio'			=> $an_exercicio,
        			'pagparcela'		=> $dados['pagparcela'],
        			'valor' 			=> $valor,
        			'empnumero' 		=> $dadosse['empnumero'],
        			'empid' 			=> $dados['empid'],
        			'request_id' 		=> $request_id,
        			'sistema' 			=> 'OBRA',
        			'obra_sub'			=> $dados['preid'],
        			'percentual'		=> $dados['porcent'],
        			'vlrpagamentoItem' 	=> $dados['valorpagamentoobra'],
        			'sldid' 			=> $dados['sldid'],
        	);
        	$id_pagamento = salvarDadosPagamento( $arrParam );
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
			<request_id>$id_pagamento</request_id>
			<nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>$nu_seq_conta_corrente_favorec</nu_seq_conta_corrente_favorec>
			<nu_processo>$nu_processo</nu_processo>
			<vl_custeio>$vl_custeio</vl_custeio>
			<vl_capital>$vl_capital</vl_capital>
			<an_referencia>$an_referencia</an_referencia>
			<sub_tipo_documento>$sub_tipo_documento</sub_tipo_documento>
			<nu_sistema>$nu_sistema</nu_sistema>
			<unidade_gestora>$unidade_gestora</unidade_gestora>
			<gestao>$gestao</gestao>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<detalhamento_pagamento>
				<item>
					<nu_parcela>$parcela</nu_parcela>
					<an_exercicio>$an_exercicio</an_exercicio>
					<vl_parcela>$valor</vl_parcela>
					<an_parcela>$an_referencia</an_parcela>
					<nu_mes>{$nu_mes}</nu_mes>
					<nu_documento_siafi_ne>{$nu_documento_siafi_ne}</nu_documento_siafi_ne>
				</item>
			</detalhamento_pagamento>
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
			$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}
		
		$arrParam = array(
				'lwsrequestdata' 	=> 'now()',
				'lwsurl' 			=> $urlWS,
				'lwsmetodo' 		=> 'solicitar',
				'lwsid' 			=> $request_id,
				'pagid' 			=> $id_pagamento
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
		
		$arrParam = array(
				'proid' 		=> $_SESSION['par_var']['proid'],
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
				'lwsid'				=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		$xml = simplexml_load_string( stripslashes($xml));
		
		$result = (integer) $xml->status->result;

		if(!$result) {
			/* $sql = "UPDATE par.pagamento SET pagstatus = 'I' WHERE pagid = $id_pagamento";
			$db->executar($sql);
			$db->commit(); */
			
			$arrParam = array(
					'lwserro' 		=> true,
					'lwsmsgretorno' => $xml->status->error->message->text,
					'lwsid' 		=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarPagamento - Erro'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
			
			$msg = $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."<br><br>";		
			echo mensagem('SOLICITAÇÃO DE PAGAMENTO', $msg);
			
			if($id_pagamento != '')
			{
				$sql = "select popid
					from par.pagamentoobrapar po
					inner join par.pagamento p on p.pagid = po.pagid
					where p.pagid = {$id_pagamento}
				";
				
				$arrPopid = $db->carregar($sql);
				$arrPopid = ($arrPopid) ? $arrPopid : Array();
				$sqlPopid = " ";
				foreach($arrPopid as $pobid){
					$id = $pobid['popid'];
					$sqlPopid .= "update par.pagamentodesembolsoobras set pdostatus = 'I'  where popid = {$id};";
				}
				$db->executar($sqlPopid);
				$db->commit();
			}
			
		} else {
			$arrParam = array(
					'lwserro' 		=> false,
					'lwsmsgretorno' => $xml->status->message->text,
					'lwsid' 		=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarPagamento - Sucesso'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
			
			echo mensagem('SOLICITAÇÃO DE PAGAMENTO', $xml->status->message->text);
			
			$sql = "UPDATE par.pagamento SET
  						parnumseqob = ".(($xml->body->nu_registro_ob)?"'".$xml->body->nu_registro_ob."'":"NULL")."
  					WHERE pagid = $id_pagamento";
			$db->executar($sql);
			$db->commit();
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
			echo "Erro-Serviço Solicitar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'solicitarPagamento - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobrapar', 'alter' );
		
		if($id_pagamento != '')
		{
			$sql = "select popid
					from par.pagamentoobrapar po
					inner join par.pagamento p on p.pagid = po.pagid
					where p.pagid = {$id_pagamento}
				";
				
			$arrPopid = $db->carregar($sql);
			$arrPopid = ($arrPopid) ? $arrPopid : Array();
			$sqlPopid = " ";
			foreach($arrPopid as $pobid){
				$id = $pobid['popid'];
				$sqlPopid .= "update par.pagamentodesembolsoobras set pdostatus = 'I'  where popid = {$id};";
			}
			$db->executar($sqlPopid);
			$db->commit();
		}
		
		echo mensagem('SOLICITAÇÃO DE PAGAMENTO', $erroMSG);
	}
	}
}


function consultarPagamento($dados) {
	global $db;

	try {
		
		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		$senha   = $dados['wssenha'];

	    $dadospag = $db->pegaLinha("SELECT pagid, parnumseqob, pagparcela, pagvalorparcela FROM par.pagamento WHERE pagid='".$dados['pagid']."'");
	    $nu_seq_ob = $dadospag['parnumseqob'];

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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;
	    $xml = simplexml_load_string( stripslashes($xml));
		$result = (integer) $xml->status->result;

		if(!$result) {
			$erros = $xml->status->error->message->text;
			
			echo mensagem('CONSULTA DE PAGAMENTO', $erros);

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'consultarPagamento - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return false;


		} else {
			$nu_seq_ob 			= (string) $xml->body->row->nu_seq_ob;
			$numero_documento 	= (string) $xml->body->row->numero_documento;
			$nu_cnpj 			= (string) $xml->body->row->nu_cnpj;
			$ds_problema 		= (string) $xml->body->row->ds_problema;
			$data_documento		= (string) $xml->body->row->data_documento;
			$valor_ob 			= (string) $xml->body->row->valor_ob;
			$situacao_documento = (string) $xml->body->row->situacao_documento;
			$processo 			= (string) $xml->body->row->processo;
			$nu_favorecido 		= (string) $xml->body->row->nu_favorecido;
			$nu_seq_ob 			= (string) $xml->body->row->nu_seq_ob;                    
			$status 			= (string) $xml->body->row->status;
			$co_status			= substr( $status, 0, 1 );
			
			if( trim($co_status) == 0 ){
				echo mensagem('CONSULTA DE PAGAMENTO', iconv("UTF-8", "ISO-8859-1", $status) );
			} else {
			
				$sql = "INSERT INTO par.historicowsprocessoobrapar(
					    	proid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$_SESSION['par_var']['proid']."',
					    		'consultarPagamento - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$db->executar($sql);
				$db->commit();
	
				$msg = iconv("UTF-8", "ISO-8859-1", $status)."<br>";
				$msg.= "*** Detalhes da consulta ***<br><br>";
				$msg.= "* Situação : ".$situacao_documento."<br>";
				$msg.= "* Data : ".$data_documento."<br>";
				$msg.= "* Valor(R$) : ".number_format($valor_ob,2,",",".")."<br>";
				$msg.= "* Processo : ".$processo."<br>";
				$msg.= "* Nº documento : ".((strlen($numero_documento)) ? $numero_documento : "-")."<br>";
				$msg.= "* CNPJ : ".((strlen($nu_favorecido)) ? $nu_favorecido : "-")."<br>";
				$msg.= "* Status : ".((strlen($status)) ? $status : "-")."<br>";
				
				echo mensagem('CONSULTA DE PAGAMENTO', $msg);
	
				if( $xml->body->row->data_documento ){
					$db->executar("UPDATE par.pagamento SET
								   pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $situacao_documento)."',
								   pagdatapagamentosiafi = ".(trim($data_documento) ? "'".formata_data_sql(iconv("UTF-8", "ISO-8859-1", $data_documento))."'" : 'null')."
								   WHERE pagid='".$dadospag['pagid']."'");
				} else {
					$db->executar("UPDATE par.pagamento SET
								   pagsituacaopagamento = '".iconv("UTF-8", "ISO-8859-1", $situacao_documento)."'
								   WHERE pagid='".$dadospag['pagid']."'");
				}
	
				$db->executar("INSERT INTO par.historicopagamento(
		           			   pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
		   					   VALUES ('".$dadospag['pagid']."', NOW(), '".$_SESSION['usucpf']."',
		   					   		   '".$dadospag['pagparcela']."', '".$dadospag['pagvalorparcela']."', '".iconv("UTF-8", "ISO-8859-1", $situacao_documento)."');");
	
				$db->commit();
	
				return true;
			}
		}
	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		echo mensagem('CONSULTA DE PAGAMENTO', $erroMSG);
	}
}

function cancelarPagamento($dados) {
	global $db;

	try {

		$data_created 	= date("c");
		$usuario 		= $dados['wsusuario'];
		$senha   		= $dados['wssenha'];		
		$dadospag 		= $db->pegaLinha("SELECT pagid, parnumseqob, pagparcela, pagvalorparcela FROM par.pagamento WHERE pagid='".$dados['pagid']."'");
	    $nu_seq_ob 		= $dadospag['parnumseqob'];

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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));
		$result = (integer) $xml->status->result;

		if($result) {
			$status 			= (string) $xml->body->status;
			$co_status			= substr( $status, 0, 1 );
			
			if( trim($co_status) == 0 ){
				echo mensagem('CANCELAMENTO DE PAGAMENTO', iconv("UTF-8", "ISO-8859-1", $status) );
			} else {
		
				$db->executar("UPDATE par.pagamento SET pagsituacaopagamento='CANCELADO', pagstatus='I' WHERE pagid='".$dadospag['pagid']."'");
				
				$db->executar("update par.pagamentodesembolsoobras set pdostatus = 'I' where popid in (select popid from par.pagamentoobrapar where pagid = {$dadospag['pagid']})");
		
				$db->executar("INSERT INTO par.historicopagamento(
		            		   pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
		    				   VALUES ('".$dadospag['pagid']."', NOW(), '".$_SESSION['usucpf']."',
		    				   		   '".$dadospag['pagparcela']."', '".$dadospag['pagvalorparcela']."', 'CANCELADA');");
			}
			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'cancelarPagamento - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();
			return true;
		} else {
			$erros = $xml->status->error->message->text;
			
			echo mensagem('CANCELAMENTO DE PAGAMENTO', $erros);

			$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'cancelarPagamento - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
		   	return false;
		}
	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Cancelar Pagamento encontra-se temporariamente indisponível. Favor tente mais tarde.".'<br>';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo mensagem('CANCELAMENTO DE PAGAMENTO', $erroMSG);
	}
}

function mensagem( $title, $msg ){
	$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
				'.$title.'
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
			</div>
		 	<br>';
	return $html;
}

function listaSolicitacaoDesembolso( $dados ){
	global $db;
	
	$obrid = $dados['obrid'];
	
	$sql = "SELECT 
			    pre.obrid,
			    pre.preid||' - '||pre.predescricao as predescricao,
			    pg.pagnumeroempenho,
			    sd.sldpercsolicitado,
			    coalesce(sd.sldpercpagamento, 0) as sldpercpagamento,
                cast( (SUM(po.popvalorpagamento) / pre.prevalorobra)*100 as numeric(10,2)) as perc_pago
			FROM
				obras.preobra pre 
			    inner join obras2.solicitacao_desembolso sd on sd.obrid = pre.obrid and sd.sldstatus = 'A'
			    inner join workflow.documento d ON d.docid = sd.docid
			    inner join par.pagamentoobrapar po on po.preid = pre.preid
			    inner join par.pagamento pg on pg.pagid = po.pagid and pg.pagstatus = 'A'
			    inner join par.pagamentodesembolsoobras pd on pd.popid = po.popid and pd.pdostatus = 'A'
			WHERE
				pre.prestatus = 'A'
			    and pg.pagsituacaopagamento not ilike '%CANCELADO%'
			    and pre.obrid = $obrid
			    and d.esdid = 1576
			group by pre.obrid, pre.preid, pre.predescricao, pg.pagnumeroempenho, sd.sldpercsolicitado, sd.sldpercpagamento
			";
	
	echo '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">
			<tbody>
			<tr>
				<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
					<p align="center"><b>Lista de Solicitações de Desembolso</b></p>
				</td>
			</tr>
			</tbody>
		</table>';
	$cabecalho = array("Obrid", "Obra", "Nº Empenho", "% Solicitado", "% Aprovado", "% Pago");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%', '', true, false, false, true);
}
?>