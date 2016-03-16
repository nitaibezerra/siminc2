<?php 
unset($_SESSION['abamenu'][$abacod_tela]);
if($_REQUEST['requisicao'] == 'carregarSecretarias'){
	$sql = "
		Select 	s.secid as codigo, 
				s.secdsc as descricao
		From sic.secretaria s
		Inner Join public.unidade u using(unicod)
		Inner Join public.orgao o using(orgcod)
		Where u.unicod= '".$_REQUEST['unicod']."'
		group by 1,2
		order by 2
	";
	$db->monta_combo('secid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
	die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

$abacod_tela = 57548;
$arMnuid = array(11631, 11636, 11052);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

monta_titulo('Lista de pedidos', $linha2);

if($_POST){
	extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script>
		
	$(function(){
		
		$('#bt_cadastrar').click(function(){
			document.location.href = 'sic.php?modulo=principal/formPedidoInformacao&acao=A';
		});
			
		$('#bt_limpar').click(function(){
			document.location.href = 'sic.php?modulo=principal/listaPedidos&acao=A';
		});
			
		$('[name=unicod]').change(function(){
			$.ajax({
				url		: 'sic.php?modulo=principal/listaPedidos&acao=A',
				type	: 'post',
				data	: 'requisicao=carregarSecretarias&unicod='+this.value,
				success	: function(e){
					$('#td_secretaria').html(e);
				}
			});			
		});
	});

	function alterarPedido(id)
	{
		document.location.href = 'sic.php?modulo=principal/formPedidoInformacao&acao=A&slcid='+id;
	}
</script>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">Pessoa</td>
			<td>
				<input type="radio" name="slctipopessoa" id="slctipopessoa_j" value="J" />&nbsp;Jurídica&nbsp;			
				<input type="radio" name="slctipopessoa" id="slctipopessoa_f" value="F" />&nbsp;Física&nbsp;
			</td>
		</tr>
        <tr>
            <td class="subtituloDireita">Prioridade</td>
            <td>
                <input type="radio" name="slcprioridade" id="slcprioridade_A" value="A" />&nbsp;Alta&nbsp;
                <input type="radio" name="slcprioridade" id="slcprioridade_B" value="B" />&nbsp;Baixa&nbsp;
            </td>
        </tr>
		<tr>
			<td class="subtituloDireita">CPF/CNPJ</td>
			<td><?php echo campo_texto('entnumcpfcnpj', 'N', 'S', '', 20, 255, '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome</td>
			<td><?php echo campo_texto('entnome', 'N', 'S', '', 70, 255, '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Unidade</td>
			<td>
				<?php 
				$sql = "
					Select 	unicod as codigo, 
							unidsc as descricao
					From public.unidade
					Inner Join sic.secretaria using(unicod)
					group by 1,2
					order by 2
				";
				
				$db->monta_combo('unicod', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Secretaria</td>
			<td id="td_secretaria">
				<?php
				$sql = array();
				
				$sql = "
					Select 	s.secid as codigo,
							s.secdsc as descricao
					From sic.secretaria s
					Inner Join public.unidade u using(unicod)
					Inner Join public.orgao o using(orgcod)
					group by 1,2
					order by 2
				";
				
				$db->monta_combo('secid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Forma de resposta</td>
			<td>
				<?php 
				$sql = "select
							tprid as codigo, 
							tprdsc as descricao
						from 
							sic.tiporesposta
						order by 
							tprdsc";
				$db->monta_combo('tprid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número SIC</td>
			<td>
				<?php
				echo campo_texto('slcnumsic', 'N', 'S', '', 15, 20, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Situação</td>
			<td>
				<?php
				
				$sql = "select 
							esdid as codigo, 
							esddsc as descricao 
						from 
							workflow.estadodocumento 
						where 
							tpdid = ".WF_TPDID_SIC;
				
				$db->monta_combo('esdid', $sql, 'S', 'Selecione...', '', '', '');
				
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Período de inclusão</td>
			<td>
				<?php echo campo_data('slcdtinclusaoinicio', 'N', 'S', '', 'N'); ?>
				&nbsp;ate&nbsp;
				<?php echo campo_data('slcdtinclusaofim', 'N', 'S', '', 'N'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Período de prazo final</td>
			<td>
				<?php echo campo_data('slcdtrespostainicio', 'N', 'S', '', 'N'); ?>
				&nbsp;ate&nbsp;
				<?php echo campo_data('slcdtrespostafim', 'N', 'S', '', 'N'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Prazo para resposta</td>
			<td>
				<?php
				echo campo_texto('prazo', 'N', 'S', '', 6, 2, '##', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Prorrogado</td>
			<td>
				<input type="radio" name="slcprorrogado" id="slcprorrogado_t" value="t" <?php echo $slcprorrogado == 't' ? 'checked' : '' ?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="slcprorrogado" id="slcprorrogado_f" value="f" <?php echo $slcprorrogado == 'f' ? 'checked' : '' ?> />&nbsp;Não
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Profissão</td>
			<td>
				<?php 
				$sql = "select 
							proid as codigo,
							prodescricao as descricao
						from 
							sic.profissao
						where prostatus = 'A'
						order by
							prodescricao";
				$db->monta_combo('proid', $sql, 'S', 'Selecione...', '', '', '', '', 'N', '', '', $_REQUEST['proid']);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Classificação?</td>
			<td>
				<?$slcclassificacao = $_REQUEST['slcclassificacao'];?>
				<input type="radio" name="slcclassificacao" id="slcclassificacao_vd" value="verde" <?php echo $slcclassificacao == 'verde' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_verde.gif" />&nbsp;
				<input type="radio" name="slcclassificacao" id="slcclassificacao_am" value="amarelo" <?php echo $slcclassificacao == 'amarelo' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_amarelo.gif" />&nbsp;
				<input type="radio" name="slcclassificacao" id="slcclassificacao_vm" value="vermelho" <?php echo $slcclassificacao == 'vermelho' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_vermelho.gif" />&nbsp;
			</td>
		</tr>
		<tr>
			<td class="subtituloEsquerda"></td>
			<td class="subtituloEsquerda">
				<input type="submit" value="Pesquisar" />
				<!-- input type="button" value="Cadastrar" id="bt_cadastrar" / -->
				<input type="button" value="Limpar" id="bt_limpar" />
			</td>
		</tr>
	</table>
</form>
<?php

$stDataResposta = "case when slcdtinclusao is not null and slcprorrogado = 't' 
						then						
						to_char((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date, 'dd/MM/yyyy')							
					 when slcdtinclusao is not null and (slcprorrogado = 'f' or slcprorrogado is null) 
					 	then 
					 		to_char((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+21 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+22 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+23 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+24 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+25 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date, 'dd/MM/yyyy')
				else to_char(slcdtinclusao, 'dd/MM/yyyy') end";

$stPrazoFinal = "case when slcdtinclusao is not null and esd.esdid not in ( ".WF_ESDID_CANCELADO.") then
					case when slcprorrogado = 't' then			
						case when 
								((
								 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
										then slcdtinclusao+30
									else
										case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
											then slcdtinclusao+31
										else
											case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
												then slcdtinclusao+32
											else
												case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
													then slcdtinclusao+33
												else
													case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
														then slcdtinclusao+34
													else
														case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
															then slcdtinclusao+35
														end
													end
												end
											end
										end				
									end
								)::date)-current_date < 0 							  
							then 0
						else
								((
								 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
										then slcdtinclusao+30
									else
										case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
											then slcdtinclusao+31
										else
											case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
												then slcdtinclusao+32
											else
												case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
													then slcdtinclusao+33
												else
													case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
														then slcdtinclusao+34
													else
														case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
															then slcdtinclusao+35
														end
													end
												end
											end
										end				
									end
								)::date)-current_date
							end
					else
						case when 
								((
								 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+15 not in (select feddata from public.feriados)
										then slcdtinclusao+15
									else
										case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+16 not in (select feddata from public.feriados)
											then slcdtinclusao+16
										else
											case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+17 not in (select feddata from public.feriados)
												then slcdtinclusao+17
											else
												case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+18 not in (select feddata from public.feriados)
													then slcdtinclusao+18
												else
													case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+19 not in (select feddata from public.feriados)
														then slcdtinclusao+19
													else
														case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
															then slcdtinclusao+20
														end
													end
												end
											end
										end				
									end
								)::date)-current_date < 0 
							then 0
						else
								((
								 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+15 not in (select feddata from public.feriados)
										then slcdtinclusao+15
									else
										case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+16 not in (select feddata from public.feriados)
											then slcdtinclusao+16
										else
											case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+17 not in (select feddata from public.feriados)
												then slcdtinclusao+17
											else
												case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+18 not in (select feddata from public.feriados)
													then slcdtinclusao+18
												else
													case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+19 not in (select feddata from public.feriados)
														then slcdtinclusao+19
													else
														case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
															then slcdtinclusao+20
														end
													end
												end
											end
										end				
									end
								)::date)-current_date
							 end
					end
				else
					0 end";

$arWhere = array();

if($_REQUEST['slcdtinclusaoinicio'] || $_REQUEST['slcdtinclusaofim']){
	
	$_REQUEST['slcdtinclusaofim']	 = $_REQUEST['slcdtinclusaofim'] ? formata_data_sql($_REQUEST['slcdtinclusaofim']) : '';
	$_REQUEST['slcdtinclusaoinicio'] = $_REQUEST['slcdtinclusaoinicio'] ? formata_data_sql($_REQUEST['slcdtinclusaoinicio']) : '';
	 
	if($_REQUEST['slcdtinclusaoinicio'] && $_REQUEST['slcdtinclusaofim']){
		
		$arWhere[] = "slcdtinclusao BETWEEN '{$_REQUEST['slcdtinclusaoinicio']}' AND '{$_REQUEST['slcdtinclusaofim']}'";
		
	}else if($_REQUEST['slcdtinclusaoinicio']){
		
		$arWhere[] = "slcdtinclusao = '{$_REQUEST['slcdtinclusaoinicio']}'";
		
	}else if($_REQUEST['slcdtinclusaofim']){
		
		$arWhere[] = "slcdtinclusao = '{$_REQUEST['slcdtinclusaofim']}'";
	}
}

if($_REQUEST['slcdtrespostainicio'] || $_REQUEST['slcdtrespostafim']){
	
	$_REQUEST['slcdtrespostafim']	 = $_REQUEST['slcdtrespostafim'] ? formata_data_sql($_REQUEST['slcdtrespostafim']) : '';
	$_REQUEST['slcdtrespostainicio'] = $_REQUEST['slcdtrespostainicio'] ? formata_data_sql($_REQUEST['slcdtrespostainicio']) : '';
	 
	if($_REQUEST['slcdtrespostainicio'] && $_REQUEST['slcdtrespostafim']){
		
		$arWhere[] = "(".str_replace(array("to_char(", ", 'dd/MM/yyyy')"), '', $stDataResposta).") BETWEEN '{$_REQUEST['slcdtrespostainicio']}' AND '{$_REQUEST['slcdtrespostafim']}'";
		
	}else if($_REQUEST['slcdtrespostainicio']){
		
		$arWhere[] = "(".str_replace(array("to_char(", ", 'dd/MM/yyyy')"), '', $stDataResposta).") = '{$_REQUEST['slcdtrespostainicio']}'";
		
	}else if($_REQUEST['slcdtrespostafim']){
		
		$arWhere[] = "(".str_replace(array("to_char(", ", 'dd/MM/yyyy')"), '', $stDataResposta).") = '{$_REQUEST['slcdtrespostafim']}'";
	}
}
if($_REQUEST['slcnumsic']){
	$arWhere[] = "slcnumsic ilike '%{$_REQUEST['slcnumsic']}%'";
}
if($_REQUEST['tprid']){
	$arWhere[] = "sc.tprid = '{$_REQUEST['tprid']}'";
}
if($_REQUEST['slctipopessoa']){
	$arWhere[] = "slctipopessoa = '{$_REQUEST['slctipopessoa']}'";
}
if($_REQUEST['slcprioridade']){
    $arWhere[] = "slcprioridade = '{$_REQUEST['slcprioridade']}'";
}
if($_REQUEST['entnumcpfcnpj']){
	$arWhere[] = "entnumcpfcnpj ilike '%{$_REQUEST['entnumcpfcnpj']}%'";
}
if($_REQUEST['entnome']){
	$arWhere[] = "entnome ilike '%{$_REQUEST['entnome']}%'";
}
if($_REQUEST['mundescricao']){
	$arWhere[] = "mundescricao ilike '%{$_REQUEST['mundescricao']}%'";
}
if($_REQUEST['estuf']){
	$arWhere[] = "ede.estuf = '{$_REQUEST['estuf']}'";
}
if($_REQUEST['esdid']){
	$arWhere[] = "doc.esdid = ".$_REQUEST['esdid'];
}
if($_REQUEST['secid']){
	$arWhere[] = "sec.secid = ".$_REQUEST['secid'];
}
if($_REQUEST['slcprorrogado']){
	if($_REQUEST['slcprorrogado'] == 't'){
		$arWhere[] = "sc.slcprorrogado = 't'";
	}else{
		$arWhere[] = "(sc.slcprorrogado = 'f' or sc.slcprorrogado is null)";
	}
}

if($_REQUEST['proid']){
	$arWhere[] = "sc.proid = '{$_REQUEST['proid']}'";
}
if($_REQUEST['slcclassificacao']){
	$arWhere[] = "sc.slcclassificacao = '{$_REQUEST['slcclassificacao']}'";
}


if($_REQUEST['unicod']){
	$arWhere[] = "(case when sc.unicod is null then sec.unicod else sc.unicod end) = '{$_REQUEST['unicod']}'";
}

if($_REQUEST['prazo']){
	$arWhere[] = " ( {$stPrazoFinal}) = ".$_REQUEST['prazo'];
}

if( checkPerfil(SIC_PERFIL_AREA_RESPONSAVEL) && !checkPerfil(SIC_PERFIL_SUPER_USUARIO) && !checkPerfil(SIC_PERFIL_ADMINISTRADOR) && !checkPerfil(SIC_PERFIL_GESTOR_SOLICITACOES) ){
	if(!empty($_SESSION['sic']['secretarias'])){
		$arWhere[] = "sc.secid in (".implode(',', $_SESSION['sic']['secretarias']).")";
	}else{
		$arWhere[] = "sc.slcid is null";
	}
}

$sql = "select
			'<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" onclick=\"alterarPedido(' || sc.slcid || ')\"/></center>' as acao,
			case when slcprorrogado = 't' then '<img src=\"../imagens/check_p.gif\" />'
			else '' end as prorrogado,
			ent.entnome,
			slcnumsic,			
			tps.tpsdsc,
			esd.esddsc,	
			case when ($stPrazoFinal) <= 5 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO.",".WF_ESDID_FINALIZADO." ) 
					then '<font color=\"red\">' || to_char(slcdtinclusao ,'dd/MM/yyyy') || '</font>'
				 when ($stPrazoFinal) between 6 and 7 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO." )
				 	then '<font color=\"#CFB53B\">' || to_char(slcdtinclusao ,'dd/MM/yyyy') || '</font>'
				 when ($stPrazoFinal) > 7 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO." )
				 	then '<font color=\"green\">' || to_char(slcdtinclusao ,'dd/MM/yyyy') || '</font>'
			else to_char(slcdtinclusao ,'dd/MM/yyyy') end as slcdtinclusao,			
			{$stDataResposta} as data_resposta,
			case when ($stPrazoFinal) <= 5 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO.",".WF_ESDID_FINALIZADO." ) 
					then '<font color=\"red\">' || u.unidsc || '</font>'
				 when ($stPrazoFinal) between 6 and 7 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO." )
				 	then '<font color=\"#CFB53B\">' || u.unidsc || '</font>'
				 when ($stPrazoFinal) > 7 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO." )
				 	then '<font color=\"green\">' || u.unidsc || '</font>'
			else u.unidsc end as ungdsc,
			case when ($stPrazoFinal) <= 5 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO.",".WF_ESDID_FINALIZADO." ) 
					then '<font color=\"red\">' || sec.secdsc || '</font>'
				 when ($stPrazoFinal) between 6 and 7 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO." )
				 	then '<font color=\"#CFB53B\">' || sec.secdsc || '</font>'
				 when ($stPrazoFinal) > 7 and esd.esdid NOT IN ( ".WF_ESDID_CANCELADO." )
				 	then '<font color=\"green\">' || sec.secdsc || '</font>'
			else sec.secdsc end as secdsc,
			usu.usunome,			
			{$stPrazoFinal} as dias,
			to_char(slcdtprorrogacao, 'DD/MM/YYYY HH24:MI:SS') as slcdtprorrogacao,
			case when slcprioridade = 'A' then 'Alta'
			else 'Baixa' end as slcprioridade
		from
			sic.solicitacao sc
		left join 
			entidade.entidade ent on ent.entid = sc.entid
		left join 
			entidade.endereco ede on ede.entid = sc.entid
		left join 
			territorios.municipio mun on mun.muncod = ede.muncod
		left join 
			workflow.documento doc on doc.docid = sc.docid
		left join 
			workflow.estadodocumento esd on esd.esdid = doc.esdid
		left join 
			sic.secretaria sec on sec.secid = sc.secid			
		left join 
			unidade u ON u.unicod = sec.unicod									
		left join 
			sic.tiporesposta tpr on tpr.tprid = sc.tprid
		left join 
			sic.tiposolicitacao tps on tps.tpsid = sc.tpsid
		left join 
			seguranca.usuario usu on usu.usucpf = sc.usucpfresponsavel
		where 
			slcstatus = 'A'
		".( count($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."
		order by
			ent.entnome, ede.estuf, mun.mundescricao";
//ver($sql,d);
$cabecalho = array('Ação','<span title="Prorrogado" alt="Prorrogado">P</span>','Requerente','Nº SIC','Solicitação','Situação', 'Data inclusão', 'Prazo final', 'Unidade', 'Secretaria', 'Responsável', 'Prazo para resposta', 'Data prorrogação','Prioridade');
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', '');

if($_SESSION['sic']['slcid'])
	unset($_SESSION['sic']['slcid']);
?>