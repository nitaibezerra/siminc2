<?php
if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

/* Removido devido solicitacao da Renilda em reunião dia 16/10/2015
if( $_REQUEST['requisicao'] == 'bloqueiar' ){
	$sql = "update obras.preobra set preblockreformulacao = 'S' where preid = {$_REQUEST['preid']}";
	$db->executar($sql);
	
	echo $db->commit();
	exit();
}*/

if( $_REQUEST['requisicao'] == 'carregarListaObras' ){

	if($_REQUEST['filtro'] == 'bloqueio'){

		if( $_REQUEST['situacao'] == 'BL')
		{
			$filtro = " and preblockreformulacao = 'S'";
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							</center>' as acoes,";
		}
		else
		{
			$filtro = " and preblockreformulacao != 'S' and p.preid not in (SELECT preid FROM obras.preobraprorrogacao WHERE popdataprazoaprovado is null and popstatus = 'P' and popvalidacao = 'f')";
			$join = "";
			$acoes = "'<center>
                                    <span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
				</center>'  as acoes,";
		}
	} else {
		$join = " inner join obras.preobraprorrogacao pp on pp.preid = p.preid ";
		$filtro = '';
		if( $_REQUEST['situacao'] == 'VO' ){
			$filtro = " and popdataprazoaprovado is null and popstatus = 'P' and popvalidacao = 'f' ";
				
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							<span style=\"font-size:18px; color:green; cursor:pointer;\" title=\"Validar a Prorrogação de Obras\" class=\"glyphicon glyphicon-check\" onclick=\"validarProrrogacao('||p.preid||', \''||v.processo||'\')\"></span>
							<span style=\"font-size:18px; color:red; cursor:pointer;\" title=\"Recusar a Prorrogação da Obras\" class=\"glyphicon glyphicon-share\" onclick=\"recusarProrrogacao('||p.preid||', \''||v.processo||'\')\"></span>
							<span style=\"font-size:18px; cursor:pointer;\" title=\"Editar a Prorrogação de Obras\" class=\"glyphicon glyphicon-edit\" onclick=\"editarProrrogacao('||p.preid||', \''||v.processo||'\')\"></span></center>' as acoes,";
		}
		else if( $_REQUEST['situacao'] == 'BL')
		{
			$filtro = " and preblockreformulacao = 'S'";
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							</center>' as acoes,";
		}
		else {
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							<span style=\"font-size:18px; color:gray;\" cursor:pointer;\" title=\"Validar a Prorrogação de Obras\" class=\"glyphicon glyphicon-check\"></span>
							<span style=\"font-size:18px; color:gray;\" cursor:pointer;\" title=\"Recusar a Prorrogação da Obras\" class=\"glyphicon glyphicon-share\"></span>
							<span style=\"font-size:18px; color:gray;\" cursor:pointer;\" title=\"Editar a Prorrogação de Obras\" class=\"glyphicon glyphicon-edit\"></span></center>' as acoes,";
		}
	}
	
	$sql = "select
				$acoes
				p.preid,
			    p.obrid,
			    p.predescricao,
			    cast(p.prevalorobra as numeric(20,2)) as valorobra,
			    vlo.totalpago as valorpagoobra,
			    pt.ptodescricao,
			    res.res_estado,
			    str.strdsc as estado2,
			    p.preano,
				(SELECT popqtddiassolicitado FROM obras.preobraprorrogacao WHERE  preid = p.preid order by popid desc limit 1) as diasvalidado,
				((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as porcentagem_execucao,
			    (select count(pp.popid) from obras.preobraprorrogacao pp where pp.preid = p.preid) as totalPro
			from
				par.v_saldo_obra_por_empenho v 
			    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
			    left join par.v_pagamento_total_por_obra vlo ON vlo.preid = p.preid
			    LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obridpai IS NULL AND o.obrstatus = 'A'
			    left join obras2.situacao_registro str on str.strid = o.strid and str.strstatus = 'A'
			    left join obras.pretipoobra pt on pt.ptoid = p.ptoid and pt.ptostatus = 'A'
			    left join( select
                                case when count(r.rstid) > 0 then
                                    case when d.esdid in (1142, 1143) then 'Não' else 'Sim' end
                                else 'Não' end as res_estado,    	
                                count(r.rstid), p1.obrid, d.esdid
                            from
                                obras.preobra p1
                                left join obras2.restricao r
                                    inner join workflow.documento d on d.docid = r.docid
                                on r.obrid = p1.obrid and r.tprid = 17 and r.rstitem = 'R' and r.rststatus = 'A'
                            group by p1.obrid, d.esdid
                        ) res ON res.obrid = p.obrid 
			    $join
			where
				v.tipo = 'PAC'
			    and v.processo = '{$_REQUEST['processo']}'
			    and v.saldo > 0
			    $filtro

			group by p.prevalorobra, p.predescricao, p.preano, p.preid, pt.ptodescricao,str.strdsc, p.obrid, p.preblockreformulacao, vlo.totalpago, porcentagem_execucao, res.res_estado, v.processo";
	  
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$arrRegistro = array();
	foreach ($arrDados as $v) {
		$sql = "select 
					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
					MIN(pag.pagdatapagamentosiafi) + 720 as prazo
				from 
					par.pagamentoobra po 
				inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				where 
					po.preid = {$v['preid']}";
		$dataPrimeiroPagamento = $db->carregar($sql);
		
		$sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$v['preid'];
		$prorrogado = $db->pegaUm($sql);
		
		if( $prorrogado ){
			$dataAtual = $prorrogado;
		} else {
			$dataAtual = $dataPrimeiroPagamento[0]['prazo'];
		}
		array_push($arrRegistro, array(
									'acoes' => $v['acoes'],
									'preid' => $v['preid'],
									'obrid' => $v['obrid'],
									'restricao' => $v['res_estado'],
									'predescricao' => $v['predescricao'],
									'ptodescricao' => $v['ptodescricao'],
									'estado2' => $v['estado2'],
									'valorobra' => $v['valorobra'],
									'valorpagoobra' => $v['valorpagoobra'],
									'dtvigencia' => ($dataAtual ? formata_data($dataAtual) : ''),
									'porcentagem_execucao' => $v['porcentagem_execucao'],
									'tempo' => $v['diasvalidado'],
									)
				);
	}
	
	$cabecalho = array('&nbsp;&nbsp;Ações&nbsp;&nbsp;', 'preid', 'obrid', 'Restrições tipo Diligência do Obras 2', 'Descrição', 'Tipo da Obra','Situação Obras 2.0', 'Valor da Obra','Valor pago da Obra', 'Data de Vigência', '% de Execução da Obra', 'Tempo de Prorrogação');
	$db->monta_lista_simples( $arrRegistro, $cabecalho,100,5,'N','100%','S', true, false, false, false);
	
	exit();
}

include APPRAIZ."includes/cabecalho.inc";
 
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<div id="dialog" title="Solicitações do Processo" style="display:none;">
</div>
<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="reqs" id="reqs" value="">
	<input type="hidden" name="preid" id="preid" value="">
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="subtituloDireita" width="30%"> Fases da Reprogramação:</td>
			<td>
				<input type="radio" name="situacao_pro" value="VO" checked="checked" />&nbsp;Aguardando Validação de Obra <br>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> Situação das obras na reprogramação:</td>
			<td>
				<input type="radio" name="situacao_pro" value="BL" <?php echo ($_REQUEST['situacao_pro'] == 'BL' ? 'checked="checked"' : ''); ?>/>&nbsp;Vigência de Obras bloqueadas<BR>
				<input type="radio" name="situacao_pro" value="SP" <?php echo ($_REQUEST['situacao_pro'] == 'SP' ? 'checked="checked"' : ''); ?>/>&nbsp;Obras que estão disponível para Solicitar Prorrogação
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Preid:</td>
            <td><?php echo campo_texto( 'preid', 'N', 'S', 'Preid', 10, 8, '[#]', '', '', '', '', '', "this.value=mascaraglobal('[#]',this.value)", $_REQUEST['preid']); ?></td>
        </tr>
		<tr>
			<td class="SubTituloDireita">Número de Processo:</td>
			<td colspan="3">
			<?php
				$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
				$numeroprocesso = $filtro;
				echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
			?>
			</td>
	
		</tr>
		<tr>
			<td class="SubTituloDireita">Número do Termo:</td>
			<td colspan="3">
			<?php
				$terid = $_REQUEST['terid'];
				echo campo_texto( 'terid', 'N', 'S', '[#]', 20, 50, '', '');
			?>
			</td>
	
		</tr>
		<!--  <tr>
			<td class="subtituloDireita" width="30%"> Esfera</td>
			<td>
				<?php

				$sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),Array('codigo'=>'E', 'descricao'=>'Estadual'));
				$db->monta_combo( "esfera", $sql, 'S', 'Todas as esferas', '', '' );
				?>
			</td>
		</tr>-->
		<tr>
			<td class="subtituloDireita" width="30%">UF:</td>
			<td>
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao 
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município:</td>
			<td id="td_muncod">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Fim de Vigência</td>
			<td>
			<?php 
			$dt_fim_vigencia_inicio = ($_POST['dt_fim_vigencia_inicio'] ? formata_data_sql($_POST['dt_fim_vigencia_inicio']) : '');
			$dt_fim_vigencia_fim = ($_POST['dt_fim_vigencia_fim'] ? formata_data_sql($_POST['dt_fim_vigencia_fim']) : '');
			?>
				De: <?=campo_data2('dt_fim_vigencia_inicio', 'N', 'S', '', 'S' ); ?>
				Até: <?=campo_data2('dt_fim_vigencia_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" class="pesquisar" value="Pesquisar"/>
				<input type="button" class="xls" value="Exportar XLS"/>
			</td>
		</tr>
	</table>
</form>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td width="15%"><span style="font-size:18px;" title="Abrir Lista de Obras" class="glyphicon glyphicon-download"></span> Abrir Lista de Obras</td>
	<td width="15%"><span style="font-size:18px; title="Abrir Informações da Obra" class="glyphicon glyphicon-question-sign"></span> Abrir Informações da Obra</td>
	<td width="15%"><span style="font-size:18px; color:green;" title="Validar a Prorrogação de Obras" class="glyphicon glyphicon-check"></span> Validar a Prorrogação de Obras</td>
	<td width="15%"><span style="font-size:18px;" title="Validar a Prorrogação de Obras" class="glyphicon glyphicon-edit"></span> Editar a Prorrogação de Obras</td>
	<td width="15%"><span style="font-size:18px; color:red;" title="Recusar a Prorrogação da Obras" class="glyphicon glyphicon-share"></span> Recusar a Prorrogação da Obras</td>
</tr>
</table>

<div id="lista">
</div>
<?php 
if( isset($_REQUEST['numeroprocesso']) ){
	
	$arWere = array();
	$imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pro.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pro.pronumeroprocesso||'\', '||pro.proid||', \'\', this);\"></span>";
	if( $_REQUEST['situacao_pro'] == 'VO' ){
		array_push($arWere, "pro.pronumeroprocesso in (SELECT distinct
														    e.empnumeroprocesso
														FROM obras.preobraprorrogacao pr
														    inner join par.empenhoobra eo on eo.preid = pr.preid and eo.eobstatus = 'A'
														    inner join par.empenho e on e.empid = eo.empid and e.empstatus = 'A'
														    inner join obras.preobra p on p.preid = eo.preid and p.prestatus = 'A'
														WHERE pr.popstatus = 'P' and pr.popvalidacao = 'f'
														    and p.preblockreformulacao != 'S') "); 
	}
	
	if( $_REQUEST['situacao_pro'] == 'SP' ){
		$imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pro.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pro.pronumeroprocesso||'\', '||pro.proid||', \'bloqueio\', this);\"></span>";
		array_push($arWere, "pro.pronumeroprocesso in (select distinct empnumeroprocesso
													from(
													SELECT
													    coalesce(cast(MAX(popdataprazoaprovado) as date),
													    cast((MIN(pag.pagdatapagamentosiafi) + 720) as date) ) - cast(now() as date) as prazo,
													    po.preid,
													    emp.empnumeroprocesso
													FROM
													    par.pagamentoobra po
													    inner join obras.preobra pre on pre.preid = po.preid and pre.prestatus = 'A'
													    inner join workflow.documento esd on esd.docid = pre.docid
														inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
													    inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
														left join obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
													where
														esd.esdid not in (".WF_PAR_OBRA_AGUARDANDO_PRORROGACAO.")
													    and pag.pagdatapagamentosiafi is not null
														and preblockreformulacao <> 'S'
													group by po.preid, emp.empnumeroprocesso
													) as foo
													where prazo < 91)");
 	}
	if( $_REQUEST['situacao_pro'] == 'BL' ){
	
		$imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pro.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pro.pronumeroprocesso||'\', '||pro.proid||', \'bloqueio\', this);\"></span>"; 
		array_push($arWere, "pro.pronumeroprocesso in (SELECT distinct
														    e.empnumeroprocesso
														FROM obras.preobra p
														    inner join par.empenhoobra eo on eo.preid = p.preid and eo.eobstatus = 'A'
														    inner join par.empenho e on e.empid = eo.empid and e.empstatus = 'A'
														WHERE p.preblockreformulacao = 'S' and p.prestatus = 'A')");
	}
	if( !empty($_REQUEST['numeroprocesso']) ){
		$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
		$where[] = "p.prpnumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
		
		array_push($arWere, "pro.pronumeroprocesso = '{$_REQUEST['numeroprocesso']}'");
	}
	if( !empty($_REQUEST['terid']) ){
		array_push($arWere, "tc.terid = '{$_REQUEST['terid']}'"); 
	}
	
	if( !empty($_REQUEST['muncod']) ){
		array_push($arWere, "pro.muncod = '{$_REQUEST['muncod']}'"); 
	} else {
		if( !empty($_REQUEST['estuf']) ){
			array_push($arWere, "(pro.estuf = '{$_REQUEST['estuf']}' OR m.estuf = '{$_REQUEST['estuf']}')");
		}
	}
	
	if( $_POST['dt_fim_vigencia_inicio'] != '' && $_POST['dt_fim_vigencia_fim'] != '' ){
		array_push($arWere, " cast(to_char( cast(vig.data as date), 'YYYY-MM-DD') as date) between cast('".formata_data_sql($_POST['dt_fim_vigencia_inicio'])."' as date) and cast('".formata_data_sql($_POST['dt_fim_vigencia_fim'])."' as date)");
	}
	
	if( !empty($_REQUEST['preid']) ) array_push($arWere, "pro.pronumeroprocesso in (SELECT distinct
																					    emp.empnumeroprocesso
																					FROM
																					    obras.preobra pre 
																					    inner join par.empenhoobra eo on eo.preid = pre.preid and eo.eobstatus = 'A'
																					    inner join par.empenho emp on emp.empid = eo.empid and emp.empstatus = 'A'
																					where
																						pre.prestatus = 'A'
																					    and pre.preid = {$_REQUEST['preid']})");
	if( $_REQUEST['reqs'] == 'xls' ){
		$acao = "";
		$cabecalho = array("Nº do Processo", 'Município/Estado', 'UF', "Nº do Documento", "Data da Validação", "Vigência do Termo", "Usuário da Validação", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancários", "Saldo Bancário<br />(CC + CP + Fundo)");

	}
	else
	{
		$acao = "'<center>
					$imgMais	
				</center>' as mais,	";
		$cabecalho = array("&nbsp;","Nº do Processo", 'Município/Estado', 'UF', "Nº do Documento", "Data da Validação", "Vigência do Termo", "Usuário da Validação", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancários", "Saldo Bancário<br />(CC + CP + Fundo)");
	}
	$sql = "SELECT
				{$acao}			
				
				substring(pro.pronumeroprocesso  from 1 for 5)||'.'||
		   		substring(pro.pronumeroprocesso from 6 for 6)||'/'||
		   		substring(pro.pronumeroprocesso from 12 for 4)||'-'||
		   		substring(pro.pronumeroprocesso from 16 for 2) as pronumeroprocesso,
			    case when m.mundescricao is not null then m.mundescricao else e.estdescricao end as municipio,
				CASE WHEN pro.estuf IS NOT NULL THEN pro.estuf ELSE m.estuf END as estuf,
				'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as ternum,
				CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE to_char(tc.terdataassinatura, 'DD/MM/YYYY') END as data,
				to_char(vig.data, 'DD/MM/YYYY') as datafimvigencia,
				CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE u.usunome END as usu,
                                (select count(pc.preid) from par.processoobra po inner join par.processoobraspaccomposicao pc on pc.proid = po.proid where
                                        pc.pocstatus = 'A' and po.proid = pro.proid) as qtdObra,
				( select sum( prevalorobra ) from par.termoobra ter inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A' WHERE ter.terid = tc.terid ) as valor_termo,
				vve.saldo as valorempenho,
				sum(pm.valor_pagamento) as valorpagamentosolicitado,
				sum(ps.valor_pagamento) as valorpagamentoefetivado,
				'Banco: '||coalesce(probanco, 'n/a')||'<br>Conta: '||coalesce(proagencia, 'n/a')||'<br>Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
				 (
			          select coalesce(saldo, 'Não Informado') as saldo from
			          (
			                  select par.retornasaldoprocesso(pro.pronumeroprocesso) as saldo
			          ) as saldomes
			      ) as saldobancario
			FROM 
				par.termocompromissopac  tc
				INNER JOIN par.processoobra 	pro ON pro.proid = tc.proid and pro.prostatus = 'A'
				LEFT  JOIN seguranca.usuario 	u 	ON u.usucpf = tc.usucpfassinatura 
				LEFT JOIN territorios.estado 	e ON e.estuf = pro.estuf 
				LEFT JOIN territorios.municipio m ON m.muncod = pro.muncod
				left join par.vm_saldo_empenho_do_processo vve on vve.processo = pro.pronumeroprocesso and vve.tipo = 'PAC'
				LEFT JOIN (
					SELECT 
					prp.pronumeroprocesso, sum( pobvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
				FROM 
					par.processoobra 	prp 
					INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
					INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento ilike '%EFETIVADO%'
					INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid  
				where
					prp.prostatus = 'A'
				GROUP BY prp.pronumeroprocesso, pagsituacaopagamento ) pm ON pm.pronumeroprocesso = pro.pronumeroprocesso
				LEFT JOIN (
					SELECT 
					prp.pronumeroprocesso, sum( pobvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
				FROM 
					par.processoobra 	prp 
					INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
					INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
					INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid  
				where
					prp.prostatus = 'A'
				GROUP BY prp.pronumeroprocesso, pagsituacaopagamento ) ps ON ps.pronumeroprocesso = pro.pronumeroprocesso
				left join (
						select distinct
						    terid,
						    max(prazo) as data
						from(
						    SELECT
						        po.preid,
						        case when vig.data is not null then vig.data else (MIN(pag.pagdatapagamentosiafi) + 720) end  as prazo,
						        tc.terid, popvalidacao
						    FROM
						        par.pagamentoobra po
						        inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
						        inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
						        inner join par.termoobraspaccomposicao tc on tc.preid = po.preid						                                    
						        left join obras.preobraprorrogacao pp on pp.preid = po.preid and pp.popdataprazoaprovado is null and pp.popstatus = 'P' and pp.popvalidacao = 'f'
						        left join(
						            SELECT popdataprazoaprovado as data, preid FROM obras.preobraprorrogacao WHERE popstatus = 'A'
						        ) vig on vig.preid = po.preid
						group by po.preid, vig.data, tc.terid, popvalidacao
						) as foo 
						group by terid) vig on vig.terid = tc.terid
			WHERE 
				tc.terstatus = 'A'	
				".($arWere ? ' and '.implode(' and ', $arWere) : '')."
			GROUP BY
				tc.usucpfassinatura, tc.terassinado, tc.proid, tc.terid, tc.terdatainclusao, tc.terdataassinatura,
				u.usunome, m.mundescricao, pro.estuf, m.estuf, e.estdescricao,
				vve.saldo, pro.proid, vig.data,
				pro.probanco, pro.proagencia, pro.nu_conta_corrente, pro.pronumeroprocesso
			ORDER BY 4, 3";
//	$cabecalho = array("&nbsp;","Nº do Processo","Nº do Documento", "Tipo de documento","Data de Vigência", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancário", "Saldo Bancário<br />(CC + CP + Fundo)");
	$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','100%','S', true, false, false, true);
	if( $_REQUEST['reqs'] == 'xls' ){
		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=SIMEC_ValidacaoExecucaoFinanceira" . date("Ymdhis") . ".xls");
		$db->monta_lista_tabulado($sql, $cabecalho, 1000000, 5, 'N', '100%');
		die();
	}
}

?>
<script type="text/javascript">

/* Esta função é a cópia fiel da requisição da página documentoPar.
* */
 
function abreObras( preid, ano ){
	return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid, 
	    	   'ProInfancia', 
	    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
} 

$('.pesquisar').click(function(){

	if(jQuery('[name="situacao_pro"]:checked').length > 0){
		jQuery('#reqs').val('');
		jQuery('[name="formulario"]').submit();
	} else {
		alert('Selecione a Situação da Reprogramação!');
		return false
	}
});

$('.xls').click(function(){

	if(jQuery('[name="situacao_pro"]:checked').length > 0){
		jQuery('#reqs').val('xls');
		jQuery('[name="formulario"]').submit();
	} else {
		alert('Selecione um item nas "Fases da Reprogramação" ou "Situação das obras na reprogramação"');
		return false
	}
});

function carregaTermoMinuta(terid){
	window.open('par.php?modulo=principal/visualizaTermoCompromisso&acao=A&terid='+terid,'visualizatermo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

$(document).ready(function(){

	/* Só mostra Município se n for esfera municipal
	*/
	$('[name="esfera"]').change(function(){
		if( $(this).val() != 'E' ){
			$('#tr_muncod').show();
		}else{
			$('#tr_muncod').hide();
			$('[name="muncod"]').val('');
		}
	});

	/* Carrega Municpios de acordo com o estado
	*/
	$('[name="estuf"]').change(function(){
		if( $('[name="esfera"]').val() != 'E' ){
			if( $(this).val() != '' ){
				$('#aguardando').show();
				$.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "requisicao=carregaMunicipios&estuf="+$(this).val(),
			   		async: false,
			   		success: function(resp){
			   			$('#td_muncod').html(resp);
			   			$('#aguardando').hide();
			   		}
			 	});
			}else{
				$('#td_muncod').html('Escolha uma UF.');
			}
		}
	});
        
	$("input[name='terid']").bind("keyup blur focus", function(e) {
    	e.preventDefault();
        var expre = /[^0-9]/g;
        if ($(this).val().match(expre))
        	$(this).val($(this).val().replace(expre,''));
	});
});

function carregarListaObras(processo, proid, acao, obj) {

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-upload');
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=12;
		ncol.innerHTML="Carregando...";
		ncol.innerHTML="<div id='listaobraprocesso_" + processo + "' ></div>";
		carregarListaDivObras( processo, acao );
	} else {
		obj.title='mais';
		jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-download');
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function carregarListaDivObras( processo, acao ){
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=carregarListaObras&processo="+processo+"&filtro="+acao+'&situacao='+jQuery('[name="situacao_pro"]:checked').val(),
   		async: false,
   		success: function(msg){
   	   		$('#listaobraprocesso_'+processo).html(msg);
   		}
	});
}

function validarProrrogacao(preid, processo){	
	window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&preid='+preid+'&processo='+processo+'&acompanhamento=S', 
					   'Prorrogação', 
					   "height=600,width=800,scrollbars=yes,top=50,left=200" ).focus();
}

function editarProrrogacao(preid, processo) {
    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&editar=1&preid=' + preid+'&processo='+processo+'&acompanhamento=S',
            'Prorrogação',
            "height=650,width=800,scrollbars=yes,top=50,left=200").focus();
}

function recusarProrrogacao(preid, processo) {
    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&recusar=S&preid=' + preid+'&processo='+processo+'&acompanhamento=S',
            'Prorrogação',
            "height=650,width=800,scrollbars=yes,top=50,left=200").focus();
}

/* Removido devido solicitacao da Renilda em reunião dia 16/10/2015
function bloqueiarProrrogacao( preid, acao, processo ){
	
	if(confirm('Tem certeza que deseja bloqueiar as solicitações de reprogramação para está obra?')){
		if( acao == 'bloqueiar_cancelar' ){
			window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&boSolicitacao=S&recusar=S&preid=' + preid+'&processo='+processo+'&acompanhamento=S',
	                'Prorrogação',
	                "height=600,width=800,scrollbars=yes,top=50,left=200").focus();
		} else {
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "requisicao=bloqueiar&processo="+processo+"&preid="+preid,
		   		async: false,
		   		success: function(msg){
			   		if( msg == '1' ){
			   			alert('Obra bloqueiada para solicitação de reformulação');
			   			carregarListaDivObras( processo, 'bloqueio' );
			   		} else {
				   		alert('Obra não bloqueiada');
				   	}
		   		}
			});
		}
	}
}
*/
</script>
<?php 
function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
	FROM territorios.municipio
	WHERE estuf = '$estuf'
	ORDER BY 2 ASC";

	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}

?>