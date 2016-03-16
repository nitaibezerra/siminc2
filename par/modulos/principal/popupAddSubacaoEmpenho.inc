<?php
$processo 	= $_REQUEST['processo'];
$empenho 	= $_REQUEST['empenho'];
$tipo 		= $_REQUEST['tipo'];

if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$tipo = $_REQUEST['tipo'];
	
	if( is_array($_REQUEST['chk']) ){
		foreach ($_REQUEST['chk'] as $codigo) {
			$vlrdisponivel 	= $_REQUEST['vlrdisponivel'][$codigo];
			$ano 			= $_REQUEST['ano'][$codigo];
			
			if( $tipo == 'PAR' ){
				$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
						VALUES ($codigo, $empenho, 0, 0, '$ano')";
			} elseif( $tipo == 'OBRA' ){
				$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
						VALUES ($codigo, $empenho, 0, 0, 0)";
			} elseif( $tipo == 'PAC' ){
				$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
						VALUES ($codigo, $empenho, 0, 0, 0)";
			}
			$db->executar($sql);
		}
		$db->commit();
	}
	echo "<script>
				alert('Operação Realizada com Sucesso.');
				window.close();
				window.opener.location.href = window.opener.location;
			</script>";
}

$arrDados = verificaEmpenhoValorDivergente($processo, $empenho, $tipo);

$empenho 			= $arrDados[0]['empenho'];
$vrlempenho 		= $arrDados[0]['vrlempenho'];
$vrlempcomposicao 	= $arrDados[0]['vrlempcomposicao'];
?>

<html>
	<head>
		<title>SIMEC - Alterar Dados Empenho</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script language="JavaScript" src="../estrutura/js/funcoes.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
<body>
<form name="formulario" id="formulario" method="post">   
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="empenho" id="empenho" value="<?=$empenho; ?>">
    <input type="hidden" name="vrlempenho" id="vrlempenho" value="<?=$vrlempenho; ?>">
    <input type="hidden" name="vrlempcomposicao" id="vrlempcomposicao" value="<?=$vrlempcomposicao; ?>">
    
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita" colspan="8"><center><b>Dados do Empenho</b></center></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Processo:</td>
			<td><?php echo formatarProcesso($arrDados[0]['processo']); ?></td>
			<td class="subtitulodireita">Valor Empenhado:</td>
			<td><?php echo number_format( $arrDados[0]['vrlempenho'], 2, ',', '.' ); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nota de Empenho:</td>
			<td><?php echo $arrDados[0]['notaempenho']; ?></td>
			<td class="subtitulodireita">Sequencial NE:</td>
			<td><?php echo $arrDados[0]['sequencial']; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Espécie NE:</td>
			<td><?php echo $arrDados[0]['especie']; ?></td>
			<td class="subtitulodireita">Tipo Processo:</td>
			<td><?php echo $arrDados[0]['tipo']; ?></td>
		</tr>
	</table>
	
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<th colspan="2">Itens disponíveis para adicionar ao empenho</th>
		</tr>
		<tr>
			<td colspan="2"><?
			$img = "''";
			if( $_SESSION['usucpf'] == '' ){
				$img = "'<img src=\"/imagens/alterar.gif\" title=\"Subacao\" onclick=\"w = window.open(\'par.php?modulo=principal/subacao&acao=A&sbaid='||codigo||'&anoatual='||ano||'\',\'Subacao\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();\">&nbsp;'";
			}
			
			$sql = "select
						local||'&nbsp;' as local,
					    $img||descricao as descricao,
					    codigo,
					    processo,
					    cast(valor as numeric(20,2)) as valor,
					    cast(valorempenhado as numeric(20,2)) as valorempenhado,
					    (valor - valorempenhado) as vrldisponivel,
					    ano
					from(
					    SELECT DISTINCT
					        par.retornacodigosubacao(s.sbaid) as local,
					        s.sbaid as codigo,
					        s.sbadsc as descricao,
					        prp.prpnumeroprocesso as processo,
					        sum((coalesce(eja.vlrempenhado, 0.00) + coalesce(vr.vrlreforco, 0.00)) - coalesce(emc.vrlcancelado, 0.00)) as valorempenhado,
					        sd.sbdano as ano,
					        coalesce((SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), 0) as valor
					    FROM 
					        par.processopar prp
					        inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
					        inner join par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
					        inner join par.subacao s ON s.sbaid = sd.sbaid
							left join (select
											sum(es.eobvalorempenho) as vlrempenhado,
										    es.sbaid, es.eobano, e1.empid
										from par.empenhosubacao es
											inner join par.empenho e1 on e1.empid = es.empid
										where 
											es.eobstatus = 'A'
										    and e1.empstatus = 'A'
										    and e1.empcodigoespecie not in ('03', '13', '04')
										group by es.sbaid, es.eobano, e1.empid) eja on eja.sbaid = sd.sbaid and eja.eobano = sd.sbdano
                             left join (select sum(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid, eb.eobano
                                    from par.empenhosubacao eb
                                        inner join par.empenho e1 on e1.empid = eb.empid and e1.empstatus = 'A' and eb.eobstatus = 'A'
                                    where e1.empcodigoespecie in ('03', '13', '04') and e1.empidpai is not null
                                    group by e1.empidpai, eb.sbaid, eb.eobano
                            ) as emc on emc.empidpai = eja.empid and emc.sbaid = eja.sbaid and emc.eobano = eja.eobano
                            left join (select sum(eobvalorempenho) as vrlreforco, e1.empidpai, eb.sbaid, eb.eobano
                                    from par.empenhosubacao eb
                                        inner join par.empenho e1 on e1.empid = eb.empid and e1.empstatus = 'A' and eb.eobstatus = 'A'
                                    where e1.empcodigoespecie in ('02') and e1.empidpai is not null
                                    group by e1.empidpai, eb.sbaid, eb.eobano
                            ) as vr on vr.empidpai = eja.empid and vr.sbaid = eja.sbaid and vr.eobano = eja.eobano
					    group by 
                        	s.sbaid,
					        s.sbadsc,
                            sd.sbdano,
					        prp.prpnumeroprocesso
					                
					    union all
					    
					    SELECT DISTINCT
					        p.preid||'&nbsp;' as local,
					        p.preid as codigo,
					        p.predescricao as descricao,
					        prp.pronumeroprocesso as processo,
					        coalesce(eja.vlrempenhado, 0.00) as valorempenhado,
					        p.preano as ano,
					        coalesce(p.prevalorobra, 0) as valor
					    FROM 
					        par.processoobraspar prp
					        inner join par.processoobrasparcomposicao ppc on ppc.proid = prp.proid and ppc.pocstatus = 'A'
					        inner join obras.preobra p ON p.preid = ppc.preid
						    left join (select
											sum(es.eobvalorempenho) as vlrempenhado,
										    es.preid
										from par.empenhoobrapar es
											inner join par.empenho e1 on e1.empid = es.empid
										where 
											es.eobstatus = 'A'
										    and e1.empstatus = 'A'
										    and e1.empcodigoespecie not in ('03', '13', '04')
										group by es.preid) eja on eja.preid = p.preid
					
						union all
					    
					    SELECT DISTINCT
					        p.preid||'&nbsp;' as local,
					        p.preid as codigo,
					        p.predescricao as descricao,
					        prp.pronumeroprocesso as processo,					        
					        coalesce(eja.vlrempenhado, 0.00) as valorempenhado,
					        p.preano as ano,
					        coalesce(p.prevalorobra, 0) as valor
					    FROM 
					        par.processoobra prp
					        inner join par.processoobraspaccomposicao ppc on ppc.proid = prp.proid and ppc.pocstatus = 'A'
					        inner join obras.preobra p ON p.preid = ppc.preid
							left join (select
											sum(es.eobvalorempenho) as vlrempenhado,
										    es.preid
										from par.empenhoobra es
											inner join par.empenho e1 on e1.empid = es.empid
										where 
											es.eobstatus = 'A'
										    and e1.empstatus = 'A'
										    and e1.empcodigoespecie not in ('03', '13', '04')
										group by es.preid) eja on eja.preid = p.preid
					) as foo
					where
						processo = '$processo'";
			
			$arrSubDados = $db->carregar($sql);
			$arrSubDados = $arrSubDados ? $arrSubDados : array();
			
			$sql = "select
					    e1.empvalorempenho as vlrempenhado,
					    ep.vrlcancelado,
					    e1.empcodigoespecie,
					    e1.empnumero
					from par.empenho e1 
						left join (
					    		select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie 
					    		from par.empenho
								where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
								group by 
									empnumeroprocesso,
									empcodigoespecie,
									empidpai) as ep on ep.empidpai = e1.empid 
					where 
					    e1.empstatus = 'A'
					    and e1.empnumeroprocesso = '{$processo}'
					    and e1.empid = {$empenho}
					    and e1.empcodigoespecie not in ('03', '04', '13')";
			$arEmpCancel = $db->pegaLinha($sql);
			
			$habilita = 'N';
			if( (float)$arEmpCancel['vlrempenhado'] == (float)$arEmpCancel['vrlcancelado'] ){
				$habilita = 'S';
			}
			
			$arrRegistro = array();
			foreach ($arrSubDados as $v) {
/*
 * case when cast(valorempenhado as numeric(20,2)) < cast(valor as numeric(20,2)) then
							'<center><input type=\"checkbox\" name=\"chk[]\" id=\"chk\" class=\"chkEmpenho\" value=\"'||codigo||'\" onclick=\"verificaChk();\"></center>'
						else '<center><input type=\"checkbox\" disabled name=\"chk[]\" id=\"chk\" class=\"chkEmpenho\" value=\"'||codigo||'\"></center>' end as acoes,
 * */
				if( ( ((float)$v['valorempenhado'] < (float)$v['valor']) || $habilita == 'S') && (float)$v['vrldisponivel'] > 0 ){
					$acao = '<center><input type="checkbox" name="chk[]" id="chk" class="chkEmpenho" value="'.$v['codigo'].'" onclick="verificaChk();"></center>';
				} else {
					$acao = '<center><input type="checkbox" disabled name="chk[]" id="chk" class="chkEmpenho" value="'.$v['codigo'].'"></center>';
				}
				array_push($arrRegistro, array(
											'acoes' => $acao,
											'local' => $v['local'],
											'descricao' => $v['descricao'],
											'valor' => number_format($v['valor'], '2', ',', '.'),
											'valorempenhado' => number_format($v['valorempenhado'], '2', ',', '.'),
											'vrldisponivel' => number_format($v['vrldisponivel'], '2', ',', '.'),
											'ano' => $v['ano'].'<input type="hidden" name="vlrdisponivel['.$v['codigo'].']" id="vlrdisponivel" value="'.$v['vrldisponivel'].'">
					        									<input type="hidden" name="ano['.$v['codigo'].']" id="ano" value="'.$v['ano'].'">'
											));
			}
			
			
			$cabecalho = array('Selecione', 'Código', 'Descricao', 'Valor', 'Valor já Empenhado', 'Valor Disponivel', 'Ano');
			$db->monta_lista_simples($arrRegistro, $cabecalho, 50, 10, 'S', '100%', 'S', false, '', '', true );
			?></td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td align="center" colspan="2">
				<input type="button" name="btnSalvar" id="btnSalvar" value="Adicionar ao Processo" onclick="salvarAlteracao();">
				<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="fechar();">
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

function verificaChk(){
	/* var vrlTotal = 0;
	jQuery('[name="chk[]"]:checked').each(function(){
		var codigo = $(this).val();
		var vrlDisponivel = jQuery('[name="vlrdisponivel['+codigo+']"]').val();
		
		vrlTotal = parseFloat(vrlTotal) + parseFloat(vrlDisponivel);
	});
	
	jQuery('[name="vlrtotalempenho"]').val( number_format(vrlTotal, 2, ',', '.' ) ); */
}

function salvarAlteracao(){
	
	if( $('[name="chk[]"]:checked').length == 0 ){
		alert('Selecione um Item disponível para adicionar ao Empenho!');
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