<?php

if( $_POST['requisicao'] == 'salvar' ){
	//ver($_POST,d);
	$prpid 				= $_POST['prpid'];
	$empenho			= $_POST['chkemp'][0];
	$sbaidSub 			= $_POST['sbaidSub'];
	$vlrtotalaempenho 	= $_POST['vlrtotalempenho'];
	$vlrsubacaoemp 		= ($_POST['vlrsubacaoemp'] ? $_POST['vlrsubacaoemp'] : $_POST['valortotalempenhado']);
	$valorempenhado 	= $_POST['valorempenhado'][$empenho];
	$eobano 			= $_POST['eobano'][$empenho];
	
	$vlrtotalaempenho 	= retiraPontosBD($vlrtotalaempenho);
	$vlrsubacaoemp 		= retiraPontosBD($vlrsubacaoemp);

	if( is_array($_POST['chk']) ){		
		foreach ($_POST['chk'] as $sbdidaEmpenhar) {
			$sbdano					= $_POST['sbdano'][$sbdidaEmpenhar];			
			$sbaid					= $_POST['sbaid'][$sbdidaEmpenhar];			
			$valorjaempenhado		= $_POST['valorjaempenhado'][$sbdidaEmpenhar];			
			$vrlpercentaempenhar 	= retiraPontosBD($_POST['vrlpercentaempenhar'][$sbdidaEmpenhar]);
			$vrlaempenhar 			= retiraPontosBD($_POST['vrlaempenhar'][$sbdidaEmpenhar]);
			
			$sql = "select
						es.empid,
						es.eobid,
					    es.eobano,
					    es.eobpercentualemp,
					    vve.vrlempenhocancelado as eobvalorempenho
					from
						par.processopar prp
						inner join par.empenho e on e.empnumeroprocesso = prp.prpnumeroprocesso and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
						inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
					    inner join par.empenhosubacao es on es.empid = e.empid and eobstatus = 'A'
					where
					    prp.prpid = $prpid
					    and es.sbaid = $sbaid 
					    and es.eobstatus = 'A' 
					    and es.eobano = $sbdano";
			$arEmpenho = $db->carregar($sql);
			$arEmpenho = $arEmpenho ? $arEmpenho : array();
			
			if( $arEmpenho[0] ){
				foreach ($arEmpenho as $arEmp) {
					
					if( $arEmp['empid'] == $empenho ){
						$vrlaempenhar = (float)$vrlaempenhar + (float)$arEmp['eobvalorempenho'];
						$vrlpercentaempenhar = (float)$vrlpercentaempenhar + (float)$arEmp['eobpercentualemp'];
					
						$sql = "update par.empenhosubacao set eobstatus = 'I' where eobid = {$arEmp['eobid']};";
						$db->executar( $sql );
// 						ver($sql);
					}
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
			    			VALUES (".$sbaid.", ".$empenho.", ".$vrlpercentaempenhar.", ".$vrlaempenhar.", '".$sbdano."');";
					$db->executar( $sql );
					//ver($sql);
					
					$sql = "select ps.sbaid, p.pagid, ps.pobpercentualpag, ps.pobvalorpagamento, ps.pobano, p.empid 
							from par.pagamento p
								inner join par.pagamentosubacao ps on ps.pagid = p.pagid
							where 
								p.empid = $empenho
							    and p.pagstatus = 'A'
							    and ps.pobstatus = 'A'";
					$arrPagamento = $db->carregar( $sql );
					//ver($sql);
					if( !empty($arrPagamento[0]) ){
						foreach ($arrPagamento as $pag ) {
							/*$sql = "update par.pagamentosubacao set pobstatus = 'I' where pagid = {$pag['pagid']}";
							$db->executar( $sql );*/
							
							$sql = "INSERT INTO par.pagamentosubacao(sbaid, pagid, pobpercentualpag, pobvalorpagamento, pobano, pobstatus) 
									VALUES ({$sbaid}, {$pag['pagid']}, {$pag['pobpercentualpag']}, {$pag['pobvalorpagamento']}, {$pag['pobano']}, 'A')";
							$db->executar( $sql );
							//ver($sql);
						}
					}
				}
			} else {
				$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
		    			VALUES (".$sbaid.", ".$empenho.", ".$vrlpercentaempenhar.", ".$vrlaempenhar.", '".$sbdano."');";
				$db->executar( $sql );
				//ver($sql);
				
				$sql = "select ps.sbaid, p.pagid, ps.pobpercentualpag, ps.pobvalorpagamento, ps.pobano, p.empid 
						from par.pagamento p
							inner join par.pagamentosubacao ps on ps.pagid = p.pagid
						where 
							p.empid = $empenho
						    and p.pagstatus = 'A'
						    and ps.pobstatus = 'A'";
				$arrPagamento = $db->carregar( $sql );
				//ver($sql);
				if( !empty($arrPagamento[0]) ){
					foreach ($arrPagamento as $pag ) {			
						$sql = "update par.pagamentosubacao set pobstatus = 'I' where pagid = {$pag['pagid']}";
						$db->executar( $sql );
						//ver($sql);
						$sql = "INSERT INTO par.pagamentosubacao(sbaid, pagid, pobpercentualpag, pobvalorpagamento, pobano, pobstatus) 
								VALUES ({$sbaid}, {$pag['pagid']}, {$pag['pobpercentualpag']}, {$pag['pobvalorpagamento']}, {$pag['pobano']}, 'A')";
						$db->executar( $sql );
						//ver($sql);
					}
				}
			}
		}
		
		$sql = "UPDATE par.empenhosubacao SET eobstatus = 'I' WHERE empid = $empenho and sbaid = $sbaidSub and eobano = '{$eobano}'";
		$db->executar( $sql );
		//ver($sql,d);		
		
		$db->commit();
	}
	$db->sucesso('principal/alterarDadosEmpenho', "&sbaid=$sbaidSub&prpid=$prpid&ano={$_REQUEST['ano']}");
	exit();
}

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, '' );

$sql = "select 
			substring(p.prpnumeroprocesso, 12, 4) as ano, 
		    p.prpnumeroprocesso as processo, 
		    case when p.sisid = 57 then 'Subações de Emendas no PAR'
		                        	else 'Subações Genérico do PAR' end as tipo
		from par.processopar p where prpid = {$_REQUEST['prpid']}";
$arProcesso = $db->pegaLinha($sql);

$sql = "select
			par.retornacodigosubacao(s.sbaid) as local,
			s.sbadsc,
			s.frmid,
			sd.sbdid,
			sum(coalesce((SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), 0)) as vlrsubacao,
			vve.vrlempenhocancelado as vlrempenhado
		from par.subacao s
        	inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid
        	inner join par.empenhosubacao es on es.sbaid = s.sbaid and eobstatus = 'A'
        	inner join par.v_vrlempenhocancelado vve on vve.empid = es.empid
		where
			s.sbaid = {$_REQUEST['sbaid']} and sd.sbdano = '{$_REQUEST['ano']}'
        group by
        	local,
            s.sbadsc, s.frmid, sd.sbdid, vve.vrlempenhocancelado";

$arSubacao = $db->pegaLinha($sql);

$processo = substr($arProcesso['processo'],0,5) . ".".substr($arProcesso['processo'],5,6)."/".substr($arProcesso['processo'],11,4) . "-".substr($arProcesso['processo'],15,2);
?>
<html>
	<head>
		<title><?php echo SIGLA_SISTEMA; ?> - Alterar Dados Empenho</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
<body>
<form name="formulario" id="formulario" method="post">   
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="sbaidSub" id="sbaidSub" value="<?=$_REQUEST['sbaid']; ?>">
    <input type="hidden" name="prpid" id="prpid" value="<?=$_REQUEST['prpid']; ?>">
    <input type="hidden" name="vlrsubacaoemp" id="vlrsubacaoemp" value="<?=($arSubacao['vlrsubacao'] ? $arSubacao['vlrsubacao'] : $arSubacao['vlrempenhado']); ?>">
    
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita" colspan="8"><center><b>Dados do Processo</b></center></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%"><b>Processo:</b></td>
			<td width="20%"><?=$processo ?></td>
			<td class="SubTituloDireita" width="10%"><b>Ano:</b></td>
			<td width="20%"><?=$arProcesso['ano'] ?></td>
			<td class="SubTituloDireita" width="10%"><b>Tipo:</b></td>
			<td width="30%"><?=$arProcesso['tipo'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="8"><center><b>Dados da Subação Empenhada</b></center></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Subação:</b></td>
			<td colspan="5"><?=$arSubacao['local'] ?> - <?=$arSubacao['sbadsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Valor da Subação:</b></td>
			<td>R$ <?=number_format(($arSubacao['vlrsubacao'] ? $arSubacao['vlrsubacao'] : $arSubacao['vlrempenhado']), 2, ',', '.'); ?></td>
			<td class="SubTituloDireita"><b>Ano da Subação:</b></td>
			<td><?=$_REQUEST['ano']; ?></td>
		</tr>
		<? if( in_array($arSubacao['frmid'], array(14, 15)) ){ ?>
		<tr>
			<td class="SubTituloDireita"><b>Emenda:</b></td>
			<td><?
			$arEmenda = $db->carregarColuna("select e.emecod from par.subacaoemendapta sep 
												inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
												inner join emenda.emenda e on e.emeid = ed.emeid
											where sep.sbdid = {$arSubacao['sbdid']} and sep.sepstatus = 'A'");
			echo implode(', ', $arEmenda);
			?></td>
		</tr>
		<?} ?>
		<tr>
			<td class="SubTituloDireita"><b>Dados de Empenho:</b></td>
			<td colspan="5">
			<?
			$sql1 = "select						
						'<center><input type=\"radio\" name=\"chkemp[]\" id=\"chkemp\" value=\"'||e.empid||'\" onclick=\"adicionarEmpenho();\"></center>' as acoes,
						e.empnumero,
						e.empid,
						es.eobpercentualemp,
						sd.sbdid,
					    vve.vrlempenhocancelado as eobvalorempenho,
					    es.eobano
					from par.empenhosubacao es
						inner join par.empenho e on e.empid = es.empid and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and eobstatus = 'A'
						inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
						inner join par.subacaodetalhe sd on sd.sbaid = es.sbaid and sd.sbdano = es.eobano
					where es.sbaid = {$_REQUEST['sbaid']} and es.eobstatus = 'A' and e.empsituacao = '2 - EFETIVADO'";
			
			$arrDados = $db->carregar($sql1);
			$arrDados = $arrDados ? $arrDados : array();
			?>
			<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
			<thead>
				<tr>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Selecione</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Nº Empenho</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">% Empenho</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Empenho</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ano</td>
					<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Pagamento</td>
				</tr>
			</thead>
			<tbody>
			<?
			$valorempenhado = 0;
			if( $arrDados ){
				foreach ($arrDados as $key => $v) {
					$key % 2 ? $cor = "#dedfde" : $cor = ""; 
					$valorempenhado += $v['eobvalorempenho']; ?>
				<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
					<td valign="middle" title="Selecione"><?=$v['acoes']; ?>
		    			<input type="hidden" name="valorempenhado[<?=$v['empid'] ?>]" id="valoremp" value="<?=$v['eobvalorempenho'] ?>">
		    			<input type="hidden" name="eobano[<?=$v['empid'] ?>]" id="eobano" value="<?=$v['eobano'] ?>">
		    			<input type="hidden" name="sbdidempenho[<?=$v['empid'] ?>]" id="sbdidempenho" value="<?=$v['sbdid'] ?>">
					</td>
					<td valign="middle" title="Nº Empenho"><?=$v['empnumero']; ?></td>
					<td valign="middle" align="right" title="% Empenho" style="color:#999999;"><?=$v['eobpercentualemp']; ?></td>
					<td valign="middle" align="right" title="Valor Empenho" style="color:#999999;"><?=number_format($v['eobvalorempenho'], '2', ',', '.'); ?></td>
					<td valign="middle" title="Ano"><?=$v['eobano']; ?></td>
					<td valign="middle" title="Pagamento">
					<?
					$sql = "select ps.pobvalorpagamento, ps.pobpercentualpag, ps.pobano 
							from par.pagamentosubacao ps
								inner join par.pagamento p on p.pagid = ps.pagid and p.pagsituacaopagamento not ilike '%CANCELADO%' and pagstatus = 'A'
							where ps.sbaid = {$_REQUEST['sbaid']} and ps.pobstatus = 'A' and p.empid = {$v['empid']}";
					
					$arrDadosPag = $db->carregar($sql);
					$arrDadosPag = $arrDadosPag ? $arrDadosPag : array();
					?>
						<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
						<thead>
							<tr>
								<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ano</td>
								<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">%</td>
								<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Pago</td>
							</tr>
						</thead>
						<tbody>
						<?
						if( $arrDadosPag ){
							foreach ($arrDadosPag as $pag) {?>
								<tr>
									<td valign="top" align="right" title="% Empenho" style="color:#999999;"><?=$pag['pobano']; ?></td>
									<td valign="top" align="right" title="% Empenho" style="color:#999999;"><?=$pag['pobpercentualpag']; ?></td>
									<td valign="top" align="right" title="% Empenho" style="color:#999999;"><?=number_format($pag['pobvalorpagamento'], '2', ',', '.'); ?></td>
								</tr>
							<?
							}
						} else {
							print '<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados Registros de Pagamento.</td></tr>';
						} ?>
						</tbody>
						</table>
					</td>
				</tr>
				<?} ?>
				</tbody>
				<tfoot>
					<tr>
						<td align="right" title="Selecione">Totais: </td>
						<td align="right" title="Nº Empenho"></td>
						<td align="right" title="% Empenho"></td>
						<td align="right" title="Valor Empenho">
							<input type="hidden" name="valortotalempenhado" id="valortotalempenhado" value="<?=$valorempenhado; ?>">
						<?=number_format($valorempenhado, '2', ',', '.'); ?></td>
						<td align="right" title="Ano"></td>
						<td align="right" title="Pagamento"></td>
					</tr>
				</tfoot>
			<?} else { ?>
				<tr><td align="center" style="color:#cc0000;" colspan="6">Não foram encontrados Registros de Empenho para está subação.</td></tr>
			<?} ?>
			</table>
			</td>
		</tr>
	</table>
    
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<th colspan="2">Subações disponível para adicionar ao empenho</th>
		</tr>
		<tr>
			<td colspan="2"><?
			if( $_SESSION['usucpf'] == '' ){
				$img = "&nbsp;<img src=\"/imagens/alterar.gif\" title=\"Subacao\" onclick=\"w = window.open(\'par.php?modulo=principal/subacao&acao=A&sbaid='||sbaid||'&anoatual='||sbdano||'\',\'Subacao\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();\">";
			}
			
			$filtro = "";
			if( in_array($arSubacao['frmid'], array(14, 15)) ) $filtro = "and s.frmid = {$arSubacao['frmid']}"; 
			
			$sql = "select
						'<center><input type=\"checkbox\" name=\"chk[]\" disabled id=\"chk\" class=\"chkEmpenho\" value=\"'||sbdid||'\" onclick=\"verificaChk(this);\">$img</center>' as acoes,
						local,
						sbaid||' - '||subacao as subacao,
						ano,
						valorsub,
						valorjaempenhado,
						sbdano,
						'<input type=\"hidden\" name=\"vlrsubacao['||sbdid||']\" id=\"vlrsubacao\" value=\"'||(valorsub)||'\">
						<input type=\"hidden\" name=\"sbdano['||sbdid||']\" id=\"sbdano\" value=\"'||sbdano||'\">
						<input type=\"hidden\" name=\"sbaid['||sbdid||']\" id=\"sbaid\" value=\"'||sbaid||'\">
						<input type=\"hidden\" name=\"valorjaempenhado['||sbdid||']\" id=\"valorjaempenhado\" value=\"'||valorjaempenhado||'\">
						<input type=\"text\" style=\"text-align:left;\" readonly name=\"vrlpercentaempenhar['||sbdid||']\" size=\"7\" maxlength=\"6\" value=\"'||(valorjaempenhado*100/valorsub)||'\"  
							onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" title=\"\" class=\"normal percempenho\">'
						as txtpercent,
						'<input type=\"text\" style=\"text-align:left;\" readonly name=\"vrlaempenhar['||sbdid||']\" size=\"20\" maxlength=\"200\" value=\"'||valorjaempenhado||'\" onkeyup=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" 
							onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" title=\"\" class=\"normal valorjaempenhado\">'
						as txtvlrempenho
					from (
						SELECT DISTINCT
				            par.retornacodigosubacao(s.sbaid)||'&nbsp;' as local,
				            s.sbaid,
				            sd.sbdid,
				            s.sbadsc as subacao,
				            coalesce(sum(es.eobpercentualemp), 0) as percent,
	                        coalesce(sum(vve.vrlempenhocancelado), 0.00) as valorjaempenhado,
							sd.sbdano||'&nbsp;' as ano,
							sd.sbdano,
							coalesce((SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), 0) as valorsub						
						FROM 
							par.processopar prp
							inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
				            inner join par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
				            inner join par.subacao s ON s.sbaid = sd.sbaid
				            left join par.empenho e 
					            inner join par.empenhosubacao es on es.empid = e.empid and empstatus = 'A' and eobstatus = 'A'
					            inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
                            on e.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04')
				            	and es.sbaid = s.sbaid and es.eobstatus = 'A' and es.eobano = sd.sbdano
						WHERE
							ppc.prpid = {$_REQUEST['prpid']}
							and s.sbaid not in ({$_REQUEST['sbaid']})
							and sd.ssuid not in (20, 21)
							$filtro
						GROUP BY
	                    	s.sbaid,
	                    	sd.sbdid,
				            s.sbadsc,
							sd.sbdano
						ORDER BY
							s.sbadsc
					) as foo
					where valorsub > 0";
			
			$arrDados = $db->carregar($sql);
			$arrDados = $arrDados ? $arrDados : array();
			
			$arrRegistro = array();
			foreach ($arrDados as $v) {
				array_push($arrRegistro, array(
											'acoes' => $v['acoes'],
											'local' => $v['local'],
											'subacao' => $v['subacao'],
											'ano' => $v['ano'],
											'valorsub' => number_format($v['valorsub'], '2', ',', '.'),
											'valorjaempenhado' => number_format($v['valorjaempenhado'], '2', ',', '.'),
											'txtpercent' => $v['txtpercent'],
											'txtvlrempenho' => $v['txtvlrempenho'],
											));
			}
			
			
			$cabecalho = array('Selecione', 'Localização', 'Subação', 'Ano', 'Valor Subação', 'Valor já Empenhado', '% Empenho', 'Valor à Empenhar');
			$db->monta_lista_simples($arrRegistro, $cabecalho, 50, 10, 'S', '100%', 'S', false, '', '', true );
			?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="50%"><b>Valor Total Empenho:</b></td>
			<td>
				<input type="text" name="vlrtotalempenho" size="31" maxlength="13" value="0,00" onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);" 
					onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" 
					onblur="MouseBlur(this);this.value=mascaraglobal('[###.]###,##',this.value);" id="vlrtotalempenho" readonly="readonly" title="Valor Total Empenho" class=" normal">
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td align="center" colspan="2">
				<input type="button" name="btnSalvar" id="btnSalvar" value="Salvar Alteração" onclick="salvarAlteracao();">
				<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="fechar();">
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
	
	$(document).ready(function(){
		$('.valorjaempenhado').each(function(){			
			$(this).val( mascaraglobal('[###.]###,##', parseFloat($(this).val()).toFixed(2)) );
		});
		$('.percempenho').each(function(){
			if( $(this).val() < 100 && $(this).val() != 0 ){
				$(this).val( mascaraglobal('[###.]###,##', parseFloat($(this).val()).toFixed(2)) );
			} else if( $(this).val() > 100 ) {
				$(this).val( mascaraglobal('[###.]###,##', parseFloat($(this).val()).toFixed(2)) );
			}
		});
	});
	
	function adicionarEmpenho(){
		$('.chkEmpenho').attr('disabled', false);
		
		$('[name="chk[]"]').each(function(){
			$(this).attr('checked', false);
			
			var codigo 		= $(this).val();
			$("[name='vrlaempenhar["+codigo+"]']").val( mascaraglobal('[###.]###,##', parseFloat(0).toFixed(2)) );
			$("[name='vrlpercentaempenhar["+codigo+"]']").val( 0 );
		});
	}
	
	function verificaChk( thisCheck ){
		var sbdid = thisCheck.value;
		
		if( thisCheck.checked == true ){
			distribuiValor( thisCheck );
			calculaTotalEmpenho(thisCheck);
		} else {
			$("[name='vrlaempenhar["+sbdid+"]']").val('0,00');
			$("[name='vrlpercentaempenhar["+sbdid+"]']").val('0');
			calculaTotalEmpenho(thisCheck);
		}
	}
	
	function distribuiValor( thisCheck ){
		//Dados do Empenho
		var empenho			= $('[name="chkemp[]"]:checked').val();		
		var sbdidEmpenhado	= $('[name="sbdidempenho['+empenho+']"]').val();		
		var valorempenhado 	= $("[name='valorempenhado["+empenho+"]']").val();
		
		//Subações disponível ao empenho
		var sbdid 			= thisCheck.value;
		var vlrsubacao 		= $("[name='vlrsubacao["+sbdid+"]']").val();
		
		if( parseFloat(vlrsubacao) < parseFloat(valorempenhado) ){
			alert('O valor de R$ '+mascaraglobal('[###.]###,##', parseFloat(valorempenhado).toFixed(2))+' da subação empenhada não pode ser distribuído para a subação selecionada, pois falta apenas empenhar o valor de R$ '+mascaraglobal('[###.]###,##', parseFloat(vlrsubacao).toFixed(2)) );
						
			$("[name='vrlaempenhar["+sbdid+"]']").val( mascaraglobal('[###.]###,##', parseFloat(0).toFixed(2)) );
			$("[name='vrlpercentaempenhar["+sbdid+"]']").val( 0 );
			thisCheck.checked = false;
		} else {		
			$("[name='vrlaempenhar["+sbdid+"]']").val( mascaraglobal('[###.]###,##', parseFloat(valorempenhado).toFixed(2)) );
			var vrlaempenhar	 = $("[name='vrlaempenhar["+sbdid+"]']").val();
			vrlaempenhar 		 = vrlaempenhar.replace(/\./gi,"");
			vrlaempenhar 		 = vrlaempenhar.replace(/\,/gi,".");
			var percentaEmpenhar = parseFloat(vrlaempenhar)*100/vlrsubacao;
			
			$("[name='vrlpercentaempenhar["+sbdid+"]']").val( mascaraglobal('[###.]###,##', parseFloat(percentaEmpenhar).toFixed(2)) );
		}		
	}
	
	function calculaTotalEmpenho( sbdidSelect ){
		var totalValorInformado = 0;
		
		var empenho			= $('[name="chkemp[]"]:checked').val();
		var vlrsubacaoemp	= $("[name='valorempenhado["+empenho+"]']").val();
		
		$('[name="chk[]"]:checked').each(function(){
							
			var sbdid 			= $(this).val();
			var vrlaempenhar 	= $("[name='vrlaempenhar["+sbdid+"]']").val();
			var vlrsubacao 		= $("[name='vlrsubacao["+sbdid+"]']").val();
			
			if( vrlaempenhar != '' ){
				vrlaempenhar = vrlaempenhar.replace(/\./gi,"");
				vrlaempenhar = vrlaempenhar.replace(/\,/gi,".");
			
				totalValorInformado = parseFloat(totalValorInformado) + parseFloat(vrlaempenhar);
			}
		});
		$('#vlrtotalempenho').val( mascaraglobal('[###.]###,##', parseFloat(totalValorInformado).toFixed(2)) );
		
		if( parseFloat(totalValorInformado) > parseFloat(vlrsubacaoemp) ){
			alert('O valor informado para empenho da subação: R$ '+mascaraglobal('[###.]###,##', parseFloat(totalValorInformado).toFixed(2))+' é maior que o valor da subação: R$ '+mascaraglobal('[###.]###,##', parseFloat(vlrsubacaoemp).toFixed(2)));
			
			$('#vlrtotalempenho').val( mascaraglobal('[###.]###,##', parseFloat(vlrsubacaoemp).toFixed(2)) );
			$("[name='vrlaempenhar["+sbdidSelect.value+"]']").val( mascaraglobal('[###.]###,##', parseFloat(0).toFixed(2)) );
			$("[name='vrlpercentaempenhar["+sbdidSelect.value+"]']").val( 0 );
			sbdidSelect.checked = false;
		}
	}
	
	function salvarAlteracao(){
		
		if( $('[name="chk[]"]:checked').length == 0 ){
			alert('Selecione uma Subações disponível para adicionar ao Empenho!');
			return false;
		}
		$('[name="requisicao"]').val('salvar');
		$('[name="formulario"]').submit();
	}
	
	function fechar(){
		window.close();
		window.opener.location.href = window.opener.location;
	}
</script>
</body>
</html>