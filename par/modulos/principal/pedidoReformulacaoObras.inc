<?php
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ."includes/cabecalho.inc";

if( $_REQUEST['requisicao'] == 'pegavalorprojetoajaxnovo' ){
	ob_clean();
	
	if( !empty($_POST['ptoid']) ){
		$sql = "select sum(p.pirvalor * p.pirqtd) from obras.preitencomposicao_regiao p
					inner join obras.preitenscomposicao pic on pic.itcid = p.itcid
				where pic.ptoid = {$_POST['ptoid']} and p.estuf = '{$_POST['estuf']}'
					and p.pirstatus = 'A' and pic.itcstatus = 'A'";
		$valor = $db->pegaUm($sql);
		
		echo ($valor ? simec_number_format($valor, '2', ',', '.') : '0,00');
	} else {
		echo '0,00';
	} 
	
	exit();
}
 
if( $_REQUEST['requisicao'] == 'enviarValidacaoAnalise' ){
	ob_clean(); 
	//ver($_POST,d);
	$proid 						= $_POST['proid'];	
	$arrObra					= $_POST['preid'][$proid];
	$pprjustificativa 			= $_POST['pprjustificativa'][$proid];
	$estadoAtualProcesso		= $_POST['estadoAtualProcesso'][$proid];
	$prmanalisecontrapartida	= $_POST['prmanalisecontrapartida'][$proid];
	
	$sql = "SELECT distinct
				d.esdid
			FROM par.processoobraspaccomposicao pc
				inner join obras.preobra p on p.preid = pc.preid
			    inner join workflow.documento d on d.docid = p.docid
			WHERE
				pc.pocstatus = 'A'
				and p.ptoid IN (43, 42, 44, 45, 73, 74)
			    and pc.proid = $proid";
	$arrEstadoProcesso = $db->carregarCoLuna($sql);
	
	$pprjustificativa	= ($pprjustificativa ? "'".pg_escape_string($pprjustificativa)."'" : 'null');
	
	$sql = "UPDATE par.processopedidoreformulacao SET pprstatus = 'I' WHERE proid = $proid";
	$db->executar($sql);
	
	$sql = "INSERT INTO par.processopedidoreformulacao(proid, pprjustificativa, pprdata, usucpf, pprstatus)
			VALUES ($proid, $pprjustificativa, now(), '".$_SESSION['usucpf']."', 'A')returning pprid";
	$pprid = $db->pegaUm($sql);
	
	
	if( is_array($arrObra) ){
		foreach ($arrObra as $preid) {
			$sfoacaosolicitada 			= $_POST['sfoacaosolicitada'][$proid][$preid];
			$ptoid 						= $_POST['ptoid'][$proid][$preid];
			$sfojustificativa			= $_POST['sfojustificativa'][$proid][$preid];
			$sfocontrapartida			= $_POST['sfocontrapartida'][$proid][$preid];
			$estadoAtual				= $_POST['estadoAtual'][$proid][$preid];
			$sfocontrapartidainformada	= $_POST['sfocontrapartidainformada'][$proid][$preid];
			
			$sfocontrapartida 			= ( !empty($sfocontrapartida) ? retiraPontosBD($sfocontrapartida) : 'null');
			$sfocontrapartidainformada 	= ( !empty($sfocontrapartidainformada) ? retiraPontosBD($sfocontrapartidainformada) : 'null');
			
			if( $sfoacaosolicitada == 'RE' ) $sfojustificativa = '';
			if( $sfoacaosolicitada == 'CO' ) $ptoid = '';
			if( $sfoacaosolicitada == 'SA' ){
				$sfojustificativa = '';
				$ptoid = '';
			}
			
			$sfojustificativa	= ($sfojustificativa ? "'".pg_escape_string($sfojustificativa)."'" : 'null');
			$ptoid				= ($ptoid ? $ptoid : 'null');
			
			$total = $db->pegaUm("select count(sfoid) from par.solicitacaoreformulacaoobras where preid = $preid and sfostatus = 'A'");
			
			if( (int)$total > 0 ){
				$sql = "UPDATE par.solicitacaoreformulacaoobras SET 
							sfoacaosolicitada = '$sfoacaosolicitada', 
							ptoidsolicitado = $ptoid,  
						    sfodataultimaatualizacao= now(),
						    usucpfatualizacao = '{$_SESSION['usucpf']}',
						    sfocontrapartida = $sfocontrapartida,
						    sfocontrapartidainformada = $sfocontrapartidainformada,
						    pprid = $pprid
						 WHERE preid = $preid
							and sfostatus = 'A'";
				$db->executar($sql);
			} else {
				$sql = "INSERT INTO par.solicitacaoreformulacaoobras(preid, pprid, sfoacaosolicitada, ptoidsolicitado, sfodataprimeirasolicitacao, usucpfsolicitacao, sfocontrapartida, sfocontrapartidainformada, sfostatus)
			    		VALUES ($preid,
								$pprid,
								'$sfoacaosolicitada',
								$ptoid,
								now(),
								'{$_SESSION['usucpf']}',
								$sfocontrapartida,
								$sfocontrapartidainformada,
								'A'
								)";
				$db->executar($sql);
			}
			
			if($db->commit()){
				
				if( $sfoacaosolicitada == 'RE' ){
					$docid = $db->pegaUm("select docid from obras.preobra where preid = $preid and prestatus = 'A'");
					$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";
					$esdid = $db->pegaUm( $sql );
					
					if( in_array(WF_TIPO_OBRA_APROVADA, $arrEstadoProcesso) && in_array(WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL, $arrEstadoProcesso) ){
						$esdiddestino = WF_TIPO_EM_ANALISE_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA;
					} else {
						if( in_array(WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL, $arrEstadoProcesso) ){
							$esdiddestino = WF_TIPO_EM_ANALISE_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA;
						} else {
							$esdiddestino = WF_TIPO_EM_AGUARDANDO_VALIDACAO_REFORMULACAO_MI_PARA_CONVENCIONAL;
						}
					}
					$cmddsc = 'Pedido de Reformulação';
					
					$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
					$aedid = $db->pegaUm( $sql );
					
					if( $aedid && $docid ){
						$arDados = array( 'preid' => $preid );						
						$retorno = wf_alterarEstado( $docid, $aedid, $cmddsc, $arDados );
					}
				}
			}
		}
	}
	$db->sucesso('principal/pedidoReformulacaoObras');
	exit();
}
if( $_REQUEST['requisicao'] == 'carregarListaObras' ){
	ob_clean();
	
	$sql = "select distinct
				p.preid,
			    p.preid||' - '||p.predescricao as predescricao,
			    pt.ptodescricao,
			    pt.ptoid,
			    p.docid,
			    es1.esdid as situacaopreobra,
			    es.esddsc, o.obrid, 
			    cast(((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0)) as numeric(20,2)) || '%' as percentual_execucao,
			    case when (select count(oa.oarid) from obras2.obras_arquivos oa where oa.obrid = o.obrid and oa.tpaid = 30 and oa.oarstatus = 'A') > 0 then 'Sim'
			    else 'Não' end as distrato,
			    cast(p.prevalorobra as numeric(20,2)) as vlrobra,
			    sr.ptoidsolicitado,
    			sr.sfoacaosolicitada,
    			sr.sfojustificativa,
    			sr.sfocontrapartida,
    			sr.sfocontrapartidainformada
			from
				par.processoobra po
			    inner join par.processoobraspaccomposicao pc on pc.proid = po.proid and pc.pocstatus = 'A'
			    inner join obras.preobra p on p.preid = pc.preid and p.prestatus = 'A'
			    inner join obras.pretipoobra pt on pt.ptoid = p.ptoid
			    inner join obras2.obras o on o.obrid = p.obrid and o.obrstatus = 'A'
			    inner join workflow.documento d on d.docid = o.docid
			    inner join workflow.estadodocumento es on es.esdid = d.esdid
			    inner join workflow.documento d1 on d1.docid = p.docid
    			inner join workflow.estadodocumento es1 on es1.esdid = d1.esdid
			    left join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
			where
				po.proid = {$_POST['proid']}
			order by 2";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$sql = "select pprjustificativa from par.processopedidoreformulacao where proid = ".$_POST['proid']." and pprstatus = 'A'";
	$pprjustificativa = $db->pegaUm($sql);
	
	$sql = "select distinct pf.prmparecer, coalesce(pf.prmanalisecontrapartida, 'N') as prmanalisecontrapartida, pf.esdid
			from par.processopedidoreformulacao pp
				inner join par.parecerreformulacaomi pf on pf.pprid = pp.pprid and pf.prmstatus = 'A'
			where
				pp.proid = {$_POST['proid']}
			    and pp.pprstatus = 'A'";
	$arrParecer = $db->pegaLinha($sql);
	
	$parecerdiligencia = $arrParecer['prmparecer'];
	$prmanalisecontrapartida = $arrParecer['prmanalisecontrapartida'];
	if( empty($prmanalisecontrapartida) ) $prmanalisecontrapartida = 'N';
	?>
	<script type="text/javascript">
	  
	$1_11(function() {		
		$1_11( '.glyphicon-question-sign' ).tooltip({

			content: function() {
		          var text = jQuery(this).prev().val();
		          return text;
		      }
	      
		});
	  });
	</script>
	<style>
  .ui-tooltip {
    border: 2px solid white;
    color: black;
    border-radius: 10px;
    /*font: 12px "Helvetica Neue", Sans-Serif;*/
    box-shadow: 0 0 7px black;
    z-index: 9999;
    max-width: 400px;
    position: absolute;
    -webkit-box-shadow: 0 0 8px #aaa;
  }
  .quadro_resumo th{
  	/*background-color: #00A5E0;*/
  }
  .titulo_resumo{
  	background-color: rgb(31, 73, 125); 
  	color: rgb(250, 125, 0); 	
  }
  </style>
	<form name="formulario_<?php echo $_POST['proid']?>" id="formulario_<?php echo $_POST['proid']?>" method="post">   
    	<input type="hidden" name="requisicao" id="requisicao" value="">
    	<input type="hidden" name="proid" id="proid" value="<?php echo $_POST['proid']; ?>">
    	<input type="hidden" name="prmanalisecontrapartida_<?php echo $_POST['proid']; ?>" id="prmanalisecontrapartida" value="<?php echo $prmanalisecontrapartida ?>">
    	<input type="hidden" name="estuf_post" id="estuf_post" value="<?php echo $_POST['estuf']; ?>">
    	<input type="hidden" name="estadoAtualProcesso" id="estadoAtualProcesso" value="<?php echo $arrParecer['esdid']; ?>">
	<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<thead>
		<tr>
			<th colspan="15" style="text-align: left; color: red" id="th_vrl_geral_contra_msg_<?php echo $_POST['proid']; ?>"><div id="div_vrl_geral_contra_msg_<?php echo $_POST['proid']; ?>"></div>
Sendo que deverá seguir as seguintes regras:<br>
1 - O valor informado pelo município de contrapartida não pode ultrapassar o valor individual de contrapartida da obra<br>
2 - O somatório distribuído de contrapartida nas obras não pode ultrapassar o valor total de contrapartida do Termo.<br>
O repasse da contra partida será feito da seguinte forma:<br>
<table  style="width: 30% !important; color: red" align="left" border="0" cellspacing="0" cellpadding="2" align="center">
	<tr>
		<td><b>Fase da obra</b></td>
		<td><b>% do valor da  contrapartida a ser depositado</b></td>
	</tr>
	<tr bgcolor="#CCCCCC" style="border-color: black !important;">
		<td>Até 20% da execução</td>
		<td>20%</td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td>Até 40% da execução</td>
		<td>25%</td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td>Até 60% da execução</td>
		<td>35%</td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td>Até 80% da execução</td>
		<td>20%</td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td>Total</td>
		<td>100%</td>
	</tr>
</table></th>
		</tr>
		<tr>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ações</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Nome da Obra</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Tipo de Obra</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação do Obras2</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Percentual de Execução</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Tem Distrato</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor da Obra</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Reformular para:</td>
			<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Projeto</td>
			<td class="title cab_projeto_analise_<?php echo $_POST['proid']; ?>" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Contrapartida</td>
			<td class="title cab_projeto_analise_<?php echo $_POST['proid']; ?>" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Obra Terá Contrapartida</td>
		</tr>
	</thead>
	<tbody>	
	<?php
	
	if( $arrDados ){
		$valorObra = 0;
		$valorProjeto = 0;
		$valorTotalContraPartidaInformada = 0;
		$boRecusa = 'N';
		
		foreach ($arrDados as $key => $v) {
			
			$docid = $v['docid'];
			$estadoAtual = wf_pegarEstadoAtual( $docid );
			
			$valorObra += $v['vlrobra'];
			$valorTotalContraPartidaInformada += $v['sfocontrapartidainformada'];
			$key % 2 ? $cor = "#dedfde" : $cor = ""; 
			
			$disabled = '';
			/* if( !in_array($v['ptoid'], array(42, 43, 44, 45)) ){
				$disabled = 'disabled="disabled"';
			} */
			if( $v['ptoid'] == 73 ){//Projeto 1 Convencional
				$qtd = 188;
			}
			if( $v['ptoid'] == 74 ){//Projeto 2 Convencional
				$qtd = 94;
			}
			if( in_array($v['ptoid'], array(43, 44) ) ){ //Escola Proinfância B
				$qtd = 120;
			}
			if( in_array($v['ptoid'], array(42, 45)) ){ //Escola Proinfância C
				$qtd = 60;
			}
			
			$sql = "select count(hd.hstid)
					from obras.preobra p
					    inner join workflow.historicodocumento hd on hd.docid = p.docid
					where hd.aedid in (3639, 3701)
						and p.docid = $docid";
			$totRecusa = $db->pegaUm($sql);
			if( (int)$totRecusa > 0 && $boRecusa == 'N' ) $boRecusa = 'S';
			
			//glyphicon 
			$sfocontrapartida = $v['sfocontrapartida'];
			$v['sfocontrapartida'] = ($v['sfocontrapartida'] ? simec_number_format($v['sfocontrapartida'], '2', ',', '.') : '');
			?>
			<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td valign="middle">			
					<input type="hidden" name="preid[<?php echo $_POST['proid']?>][]" value="<?php echo $v['preid']; ?>">
					<input type="hidden" name="predescricao[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['predescricao']; ?>">
					<input type="hidden" name="ptodescricao[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['ptodescricao']; ?>">
					<input type="hidden" name="alunosatendido[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $qtd; ?>">
					<input type="hidden" name="distrato[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['distrato']; ?>">
					<input type="hidden" name="ptoid_atual[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['ptoid']; ?>">
					<input type="hidden" name="situacaopreobra[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['situacaopreobra']; ?>">					
					<input type="hidden" name="sfocontrapartida[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['sfocontrapartida']; ?>">
					<input type="hidden" name="estadoAtual[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $estadoAtual['esdid']; ?>">
					<input type="hidden" name="boRecusa[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $boRecusa; ?>">
					
					<input type="hidden" value="" class="pendeciaobra_<?php echo $v['preid']; ?>">
					
					<span id="span_<?=$v['preid']; ?>" style="font-size:14px; cursor:pointer; color: red;" title="" class="glyphicon glyphicon-question-sign"></span>
					<input type="radio" name="sfoacaosolicitada[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" <?php echo ($v['sfoacaosolicitada'] == 'RE' ? 'checked="checked"' : '' ); /*echo $disabled;*/ ?> id="sfoacaosolicitada_re_<?php echo $_POST['proid']?>_<?php echo $v['preid']; ?>" onclick="verificaTipoAcao(<?php echo $_POST['proid']?>, <?php echo $v['preid']; ?>);" value="RE"> Reformular<br>
					<input type="radio" name="sfoacaosolicitada[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" <?php echo ($v['sfoacaosolicitada'] == 'CO' ? 'checked="checked"' : '' ); /*echo $disabled;*/ ?> id="sfoacaosolicitada_co_<?php echo $_POST['proid']?>_<?php echo $v['preid']; ?>" onclick="verificaTipoAcao(<?php echo $_POST['proid']?>, <?php echo $v['preid']; ?>);" value="CO"> Cancelar Obra<br>
					<input type="radio" name="sfoacaosolicitada[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" <?php echo ( ($v['sfoacaosolicitada'] == 'SA' /*|| !empty($disabled)*/ ) ? 'checked="checked"' : '' ); ?> id="sfoacaosolicitada_sa_<?php echo $_POST['proid']?>_<?php echo $v['preid']; ?>" onclick="verificaTipoAcao(<?php echo $_POST['proid']?>, <?php echo $v['preid']; ?>);" value="SA"> Sem alteração
					</td>
				<td valign="middle"><?=$v['predescricao']; ?></td>
				<td valign="middle"><?=$v['ptodescricao']; ?></td>
				<td valign="middle"><?=$v['esddsc']; ?></td>
				<td valign="middle"><?=$v['percentual_execucao']; ?></td>
				<td valign="middle"><?=$v['distrato']; ?></td>
				<td valign="middle" align="right" style="color:#999999;"><div id="div_valorobra_<?php echo $v['preid']; ?>"><?=simec_number_format($v['vlrobra'], '2', ',', '.'); ?></div></td>
				<td valign="middle" style="text-align: left">
					<div id="ptoid_combo_<?php echo $_POST['proid']?>_<?php echo $v['preid']; ?>" style="display: <?php echo ($v['sfoacaosolicitada'] == 'RE' ? '' : 'none' ); ?>">
					<?
					$sql = "select 
								p.ptoid as codigo,
							    p.ptodescricao as descricao
							from
								obras.pretipoobra p
							where
								p.ptostatus = 'A'
							    and p.ptoid in (73, 74)
							order by p.ptodescricao";
					//$db->monta_combo( "ptoid[".$_POST['proid']."][".$v['preid']."]", $sql, 'S', 'Selecione', '', '', '', '280', '', '', '', $v['ptoidsolicitado'] );
					?>
					<select name="ptoid[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" class="CampoEstilo" style="width:190px;" onchange="pegaValorProjetoAjax(this.value, <?php echo $_POST['proid']; ?>, <?php echo $v['preid']; ?>, '<?php echo $_POST['estuf']; ?>'); AddTableRow(<?php echo $_POST['proid']?>);">
						<option value="">Selecione</option>
						<option value="73" <?php echo ($v['ptoidsolicitado'] == '73' ? 'selected="selected"' : '' )?> >Projeto 1 Convencional</option>
						<option value="74" <?php echo ($v['ptoidsolicitado'] == '74' ? 'selected="selected"' : '' )?> >Projeto 2 Convencional</option>
					</select>
					</div>
				</td>
				<td valign="middle" style="color:#999999;"><div id="div_valorprojeto_<?php echo $v['preid']; ?>"><?php
				if( !empty($v['ptoidsolicitado']) ){ 
										
					$sql = "select sum(p.pirvalor * p.pirqtd)  from obras.preitencomposicao_regiao p
								inner join obras.preitenscomposicao pic on pic.itcid = p.itcid
							where pic.ptoid = {$v['ptoidsolicitado']} and p.estuf = '{$_POST['estuf']}'
								and p.pirstatus = 'A' and pic.itcstatus = 'A'";
					$valor = $db->pegaUm($sql);
					
					$valorProjeto += $valor;
					echo ($valor ? simec_number_format($valor, '2', ',', '.') : '0,00');
				}
				if( $v['sfoacaosolicitada'] == 'SA' ) echo simec_number_format($v['vlrobra'], '2', ',', '.');
				
				$valorContraPartida += $sfocontrapartida;
				?></div> <?php 
				echo '<input type="hidden" name="valorprojeto['.$_POST['proid'].']['.$v['preid'].']" value="'.( ($v['sfoacaosolicitada'] == 'SA') ? $v['vlrobra'] : $valor ).'">';
				?>
				</td>
				<!--  <td valign="middle">
					<? //campo_texto( "sfocontrapartida[".$_POST['proid']."][".$v['preid']."]", 'N', 'N', 'Valor Total da Contrapartida', 11, 15, '[###.]###,##', '','','','','id="sfocontrapartida['.$_POST['proid'].']['.$v['preid'].']"', '', $v['sfocontrapartida'], "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
				</td> -->
				<td valign="middle" style="color:#999999;" class="td_projeto_analise_<?php echo $_POST['proid']?>"><div id="div_projeto_analise_<?php echo $_POST['proid']?>_<?php echo $v['preid']; ?>"><?php echo $v['sfocontrapartida']; ?></div></td>
				<td class="td_projeto_analise_<?php echo $_POST['proid']?>">
					<input type="text" style="text-align:left;" name="sfocontrapartidainformada[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" size="10" maxlength="30" value="<?php echo ($v['sfocontrapartidainformada'] ? simec_number_format($v['sfocontrapartidainformada'], 2, ',', '.' ) : '0,00');?>" 
						onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" 
						onblur="MouseBlur(this);this.value=mascaraglobal('[###.]###,##',this.value); calculaContraPartida(<?php echo $_POST['proid']?>, <?php echo $v['preid']; ?>, 'N');" title="" class=" normal">
				</td>
			</tr>
		<?} ?>
	</tbody>
	<tfoot>
		<tr bgcolor="#D0D0D0">
			<td align="right"><b>Totais:</b></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"><b><?=simec_number_format($valorObra, '2', ',', '.'); ?></b></td>
			<td align="right"></td>
			<td align="right"><b><div id="div_total_vrlprojeto_<?php echo $_POST['proid']?>"><?=simec_number_format($valorProjeto, '2', ',', '.'); ?></div></b></td>
			<td align="right" class="title cab_projeto_analise_<?php echo $_POST['proid']; ?>"><b><div id="div_total_vrlcontrapartida_<?php echo $_POST['proid']?>"><?=simec_number_format($valorContraPartida, '2', ',', '.'); ?></div></b></td>
			<td align="right" class="title cab_projeto_analise_<?php echo $_POST['proid']; ?>"><b><div id="div_total_vrlcontrapartidainformada_<?php echo $_POST['proid']?>"><?=simec_number_format($valorTotalContraPartidaInformada, '2', ',', '.'); ?></div></b></td>
		</tr>
		<tr id="tr_campo_just_<?php echo $_POST['proid']?>">
			<td colspan="10">
				<table width="100%">
					<tr>
						<td>
						<table width="100%">
							<tr bgcolor="#D0D0D0" height="30px;" id="tr_label_just_<?php echo $_POST['proid']?>">
								<td align="center"><b>Parecer de Solicitação</b></td>
							</tr>
							<tr>
								<td align="center">
									<textarea id="pprjustificativa[<?php echo $_POST['proid']?>]" name="pprjustificativa[<?php echo $_POST['proid']?>]" 
											cols="80" rows="3" title="Justificativa" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" 
											class="obrigatorio txareanormal"><?php echo $pprjustificativa; ?></textarea>
								</td>
							</tr>
						</table>
						</td>
						<td>
<?php				if( $arrParecer['esdid'] == WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL ){?>
						<table width="100%">
							<tr bgcolor="#D0D0D0" height="30px;">
								<td align="center"><b>Parecer de Diligência</b></td>
							</tr>
							<tr>
								<td align="center">
									<textarea id="parecerdiligencia[<?php echo $_POST['proid']?>]" disabled="disabled" name="parecerdiligencia[<?php echo $_POST['proid']?>]" 
											cols="80" rows="3" title="Justificativa" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" 
											class="obrigatorio txareanormal"><?php echo $parecerdiligencia; ?></textarea>
								</td>
							</tr>
						</table>
<?php 				}?>
<?php 				if( $arrParecer['esdid'] == WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL ){?>
						<table width="100%">
							<tr bgcolor="#D0D0D0" height="30px;">
								<td align="center"><b>Parecer de Aprovação</b></td>
							</tr>
							<tr>
								<td align="center">
									<textarea id="pareceraprovacao[<?php echo $_POST['proid']?>]" disabled="disabled" name="pareceraprovacao[<?php echo $_POST['proid']?>]" 
											cols="80" rows="3" title="Justificativa" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" 
											class="obrigatorio txareanormal"><?php echo $parecerdiligencia; ?></textarea>
								</td>
							</tr>
						</table>
<?php 				}?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0" height="30px;">
			<td align="center" colspan="12">
<?php 			if( $boRecusa == 'N' ) {?>
					<input type="button" name="btnValidacaoAnalise" id="btnValidacaoAnalise_<?php echo $_POST['proid']; ?>" value="Enviar para Validação de Análise" onclick="enviarValidacaoAnalise(<?php echo $_POST['proid']; ?>);">
<?php			}?>
			</td>
		</tr>
		<tr>
			<td colspan="10" id="td_products_<?php echo $_POST['proid'];?>">
				<table id="products-table_<?php echo $_POST['proid'];?>" style="width: 95%" align="center" cellspacing="1" cellpadding="1" class="quadro_resumo">
				<tbody>
				<tr>
					<th colspan="9" height="20px" class="titulo_resumo" style="color: white;">Quadro Resumo</th>
				</tr>
				<tr>
					<th colspan="5" height="20px" width="50%" class="titulo_resumo" style="color: white;">Dados da Obra Atual</th>
					<th valign="middle" class="titulo_resumo" width="10%" id="td_row_<?php echo $_POST['proid'];?>" style="text-align: center;">
Resultado após<br>reformulações para<br>Atendimento as crianças</th>
					<th colspan="3" height="20px" width="40%" class="titulo_resumo" style="color: white;">Dados da Obra Alterado</th>
				</tr>
				<tr>
					<th class="titulo_resumo" height="20px" width="02%">ID</th>
					<th class="titulo_resumo" width="20%">Obras</th>
					<th class="titulo_resumo" width="20%">Tipo de obra Atual</th>
					<th class="titulo_resumo" width="10%">Valor Atual</th>
					<th class="titulo_resumo" width="10%">QTD de alunos atendidos</th>
					<th class="titulo_resumo" width="10%">Alteração do Tipo de obra</th>
					<th class="titulo_resumo" width="10%">Novo Valor</th>
	  				<th class="titulo_resumo" width="10%">QTD de alunos atendidos</th>
				</tr>
				</tbody>							
		  		</table>		  
			</td>
		</tr>
	</tfoot>
	<?} else { ?>
		<tr><td align="center" style="color:#cc0000;" colspan="10">Não foram encontrados registros de Obras.</td></tr>
	<?} ?>
	</table>
	</form><?php
	exit();
	//<img src='/imagens/proximo500.gif' height="05px">
}

/*** DECLARAÇÃO DE VARIAVEIS E FUNCOES ***/
if( !$_SESSION['par']['inuid'] ){
	echo "<script>
                    alert('Faltam dados na sessão!');
                    history.back(-1);
              </script>";
	die;
}

$estuf = $db->pegaUm("select case when estuf is null then mun_estuf else estuf end from par.instrumentounidade where inuid = ".$_SESSION['par']['inuid']);
$_SESSION['par']['estuf'] = ( $_SESSION['par']['estuf'] ? $_SESSION['par']['estuf'] :  $estuf);

/*** INSTANCIA AS DE CLASSES ***/
$obPreObraControle 		= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();
 
monta_titulo( 'Pedido/Acompanhamento de Solicitação de Reformulação de Obras', '' );
//$db->cria_aba( $abacod_tela, $url, '' );

/* Array com os itens da aba de identificação */
$menu = array (
		0 => array (
				"id" => 0,
				"descricao" => "Solicitações/Acompanhamento",
				"link" => "par.php?modulo=principal/pedidoReformulacaoObras&acao=A"
		)
);

echo montarAbasArray ( $menu, $url );

if(empty($_SESSION['par']['muncod']) && !empty($_SESSION['par']['estuf'])) {
	$descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['estuf'], 1);
} else {
	$descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);
}
?>
<div class="tabela" style="margin-left: 7px; margin-right: 5px; width: 97.5% !important;">
    <span class="TituloTela" style="text-align: center;width: 100%">OBRAS - <?= $descricao;?></span>
    <span style="float: right; color: blue; font-size: 12px;">
        <a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
        <a href="par.php?modulo=principal/orgaoEducacao&acao=A">Dados da Unidade |</a>
        <a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
    </span>
</div>
<div style="clear: both;"></div>

<?php

//monta_titulo( $titulo_modulo, '' );

$processoEmReformulacao = "po.proid IN (
								SELECT DISTINCT
									poc.proid
								FROM
									par.processoobra pob
								INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pob.proid AND poc.pocstatus = 'A'
								INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.prestatus = 'A'
								INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid AND pto.ptoid in (42, 43, 44, 45)
								INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND doc.esdid IN ( SELECT esdid
																															FROM workflow.estadodocumento
																															WHERE esddsc ILIKE '%eformulação%' AND esddsc NOT ILIKE 'RMC - %' AND tpdid IN (37, 45)
																															ORDER BY esdid )
								)";

$imgMais = "CASE WHEN $processoEmReformulacao 
				THEN '<center><span style=\"font-size:16px; cursor:pointer; color:red\" onclick=\"alert(this.title)\" class=\"glyphicon glyphicon-download\" 
									title=\"Este processo possui obra(s) em Reformulação. Para prosseguir com a solicitação cancele ou finalize a reformulação vigente.\" ></span></center>'
				ELSE '<center><span style=\"font-size:16px; cursor:pointer;\" title=\"mais\" id=\"image_'||po.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||po.pronumeroprocesso||'\', '||po.proid||', \'".$estuf."\', this);\"></span></center>'
			END";

if( $_SESSION['par']['itrid'] == 2 ){
	$filtro = " and po.muncod = '{$_SESSION['par']['muncod']}' ";
} else {
	$filtro = " and po.estuf = '{$_SESSION['par']['estuf']}' and po.muncod is null ";
}

$sql = "select distinct
			$imgMais as mais,
			po.pronumeroprocesso||'&nbsp;' as processo,
		    ter.termonumero,
		    ter.vrltermo,
		    (select sum(se.saldo) from par.vm_saldo_empenho_por_obra se where se.processo = po.pronumeroprocesso) as vrlempenhado,
		    pag.vrlpagamento,
		    est.estado
		from par.processoobra po
		    inner join(
		    	select sum(po.prevalorobra) as vrltermo, 
		        	(select par.retornanumerotermopac(tc.proid)) as termonumero, tc.proid 
		        from par.termocompromissopac tc
		        	inner join par.termoobraspaccomposicao toc on toc.terid = tc.terid
		            inner join obras.preobra po on po.preid = toc.preid and po.prestatus = 'A'
		        where
		        	tc.terstatus = 'A'
		        	$filtro
		        group by tc.proid
		    ) ter on ter.proid = po.proid
		    left join(
		    	select sum(pgo.pobvalorpagamento) as vrlpagamento, e.empnumeroprocesso from par.empenho e
		        	inner join par.pagamento pg on pg.empid = e.empid and pg.pagstatus = 'A'
		            inner join par.pagamentoobra pgo on pgo.pagid = pg.pagid
		            inner join obras.preobra po on po.preid = pgo.preid and po.prestatus = 'A'
		        where
		        	e.empstatus = 'A'
		        	$filtro
		            and pg.pagsituacaopagamento not ilike '%cancelado%'
		        group by e.empnumeroprocesso
		    ) pag on pag.empnumeroprocesso = po.pronumeroprocesso
		    left join( 
			    	SELECT case when es1.esdid = 1578 then '<span style=\'color: red\'>'||es1.esddsc||'</span>'
			    				when es1.esdid in (1561, 1579) then '<span style=\'color: blue\'>'||es1.esddsc||'</span>'
			    		 else es1.esddsc end as estado, to_char(max(hd.htddata), 'DD/MM/YYYY HH24:MI:SS') as dataanalise, pc.proid 
			        FROM par.processoobraspaccomposicao pc
			            inner join obras.preobra po on po.preid = pc.preid and pc.pocstatus = 'A'
			            inner join par.solicitacaoreformulacaoobras sr on sr.preid = po.preid and sr.sfostatus = 'A' and sr.sfoacaosolicitada = 'RE'
			            inner join workflow.documento d1 on d1.docid = po.docid and d1.esdid not in (228)
			            inner join workflow.estadodocumento es1 on es1.esdid = d1.esdid
			            left join workflow.historicodocumento hd on hd.docid = d1.docid and hd.aedid in (3597, 3699) 
			        WHERE po.ptoid IN (43, 42, 44, 45, 73, 74) $filtro
			    	GROUP BY es1.esddsc, pc.proid, es1.esdid
			    ) est on est.proid = po.proid
		where
			po.prostatus = 'A' 
			and po.proid in (select distinct
								po.proid
							from
								par.processoobra po
							    inner join par.processoobraspaccomposicao pc on pc.proid = po.proid and pc.pocstatus = 'A'
							    inner join obras.preobra p on p.preid = pc.preid and p.prestatus = 'A'
							    inner join obras.pretipoobra pt on pt.ptoid = p.ptoid and pt.ptoid in (42, 43, 44, 45)
							 union all
                             select distinct pp.proid from par.processopedidoreformulacao pp where pp.pprstatus = 'A'
							 )
			$filtro";

$cabecalho = array('Abrir/Fechar', 'Processo', 'Nº Termo', 'Valor Pactuado', 'Valor empenhado', 'Valor pago', 'Situação do Fluxo (Workflow)');
$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','100%','S', true, false, false, true);
?>

<script>
	function verificaTipoAcao( proid, preid ){
		
		var sfoacaosolicitada 	= jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').val();

		jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val('');
		
		if( sfoacaosolicitada == 'RE' ){
			jQuery('#ptoid_combo_'+proid+'_'+preid).css('display', '');
		}
		else if( sfoacaosolicitada == 'CO' ){
			jQuery('#ptoid_combo_'+proid+'_'+preid).css('display', 'none');
			jQuery('[name="ptoid['+proid+']['+preid+']"]').val('');
			jQuery('#div_valorprojeto_'+preid).html('');
			jQuery('[name="valorprojeto['+proid+']['+preid+']"]').val('');
		} else {
			jQuery('#ptoid_combo_'+proid+'_'+preid).css('display', 'none');
			jQuery('[name="ptoid['+proid+']['+preid+']"]').val('');
			var valorObra = jQuery('#div_valorobra_'+preid).html();
			jQuery('#div_valorprojeto_'+preid).html( valorObra );
			jQuery('[name="valorprojeto['+proid+']['+preid+']"]').val( retiraPontosPedido(valorObra) );
		}		
		AddTableRow( proid );
	}
		
	function enviarValidacaoAnalise( proid ){
		var erro = false;

		var vrlcontrapartidaTotal	= jQuery('[name="contrapartidatotal_'+proid+'"]').val();			
		var prmanalisecontrapartida	= jQuery('[name="prmanalisecontrapartida_'+proid+'"]').val();			
		var vlrTotContraInformada 	= 0;
		
		jQuery('[name="preid['+proid+'][]"]').each(function(){
			
			var preid 						= jQuery(this).val();
			var ptoid 						= jQuery('[name="ptoid['+proid+']['+preid+']"]').val();
			var boSfoacaosolicitada 		= jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').length;
			var predescricao 				= jQuery('[name="predescricao['+proid+']['+preid+']"]').val();
			var estadoAtual 				= jQuery('[name="estadoAtual['+proid+']['+preid+']"]').val();
			var sfocontrapartidainformada 	= jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val();
			var sfocontrapartida 			= jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val();

			sfocontrapartidainformada = retiraPontosPedido(sfocontrapartidainformada);
			
			if( boSfoacaosolicitada == 0 ){
				alert('E necessario informar o tipo de ação que deseja solicitar.');
				erro = true;
				return false;
			}
			var sfoacaosolicitada 	= jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').val();
			
			if( sfoacaosolicitada == 'RE' && ptoid == '' ){
				alert('Para a obra "'+predescricao+'" deve ser informado o tipo de projeto que deseja reformular.');
				jQuery('[name="ptoid['+proid+']['+preid+']"]').focus();
				erro = true;
				return false;
			}
			
			vlrTotContraInformada = parseFloat(vlrTotContraInformada) + parseFloat(sfocontrapartidainformada);
		});
		if( jQuery('#tr_campo_just_'+proid).css('display') != 'none' && jQuery('[name="pprjustificativa['+proid+']"]').val() == '' ){
			alert('O preenchimento da Justificativa é obrigatório.');
			jQuery('[name="pprjustificativa['+proid+']"]').focus();
			erro = true;
			return false;
		}

		if( prmanalisecontrapartida == 'S' ){ 
			vrlcontrapartidaTotal = number_format(vrlcontrapartidaTotal, 2, ',', '.' );
			vrlcontrapartidaTotal = retiraPontosPedido(vrlcontrapartidaTotal);
			
			if( parseFloat(vlrTotContraInformada) != parseFloat(vrlcontrapartidaTotal) ){
				alert('O Valor Geral de Contrapartida deve ser totalmente distribuidos entre as obras');
				erro = false;
			}
		}
		
		if( erro == false ){		
			jQuery('[name="requisicao"]').val('enviarValidacaoAnalise');
			jQuery('[name="formulario_'+proid+'"]').submit();
		}
	}

	function pegaValorProjetoAjax( ptoid, proid, preid, estuf ){
		
		jQuery.ajax({
	   		type: "POST",
	   		url: 'par.php?modulo=principal/pedidoReformulacaoObras&acao=A',
	   		data: "requisicao=pegavalorprojetoajaxnovo&ptoid="+ptoid+"&estuf="+estuf,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#div_valorprojeto_'+preid).html(msg);
	   			jQuery('[name="valorprojeto['+proid+']['+preid+']"]').val( retiraPontosPedido(msg) );
	   		}
		});
	}
	
	function carregarListaObras(processo, proid, estuf, obj) {
		
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
			
			carregarListaDivObras( processo, proid, estuf);
		} else {
			obj.title='mais';
			jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-download');
			var nlinha = tabela.deleteRow(linha.rowIndex);
		}

		var prmanalisecontrapartida	= jQuery('[name="prmanalisecontrapartida_'+proid+'"]').val();
		
		var boObraAprovada = true;
		var boHabilitaContPartida = false;
		jQuery('[name="preid['+proid+'][]"]').each(function(){
			var titulo = '';
			var preid 				= jQuery(this).val();
			var distrato 			= jQuery('[name="distrato['+proid+']['+preid+']"]').val();
			var ptoid_atual 		= jQuery('[name="ptoid_atual['+proid+']['+preid+']"]').val();
			var situacaopreobra 	= jQuery('[name="situacaopreobra['+proid+']['+preid+']"]').val();
			var sfocontrapartidainformada = jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val();
			var boSfoacaosolicitada = jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').length;
			var sfocontrapartida 	= jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val();
			var boRecusa 			= jQuery('[name="boRecusa['+proid+']['+preid+']"]').val();
			
			if( (sfocontrapartidainformada != '' && sfocontrapartidainformada != '0,00' || prmanalisecontrapartida == 'S') && boHabilitaContPartida == false ) boHabilitaContPartida = true;

			if( boSfoacaosolicitada > 0 ){
				if( situacaopreobra != 228 && situacaopreobra != 1578 ){ //228 - Obra Aprovada, 1578 - Diligencia 
					jQuery('#sfoacaosolicitada_re_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_co_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_sa_'+proid+'_'+preid).attr('disabled', true);					
					jQuery('[name="ptoid['+proid+']['+preid+']"]').attr('disabled', true);					
					jQuery('[name="ptoid['+proid+']['+preid+']"]').attr('disabled', true);					
					jQuery('#btnValidacaoAnalise_'+proid).attr('disabled', true);
				}
				if( parseFloat(sfocontrapartida) == 0 || parseFloat(sfocontrapartida) == '0,00' ){
					jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').attr('readonly', true);
				}
				
				if( situacaopreobra != 228 ) boObraAprovada = false;
			} else {
				if( situacaopreobra != 228 && boObraAprovada == true ){
					boObraAprovada = false;
				}
				if( situacaopreobra != 228 && situacaopreobra != 1578 ){ //228 - Obra Aprovada, 1578 - Diligencia
					jQuery('#sfoacaosolicitada_re_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_co_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_sa_'+proid+'_'+preid).attr('checked', true);
					
					titulo += '* A situação atual da obra tem que ser Aprovada<br>';
					jQuery('#sfoacaosolicitada_re_'+proid+'_'+preid).attr('title', '');
					
				} else if( ptoid_atual != 42 && ptoid_atual != 43 && ptoid_atual != 44 && ptoid_atual != 45 ){
					jQuery('#sfoacaosolicitada_re_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_co_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_sa_'+proid+'_'+preid).attr('disabled', true);
					jQuery('#sfoacaosolicitada_sa_'+proid+'_'+preid).attr('checked', true);
					titulo += '* Tipo da obra não é MI<br>';
				}
			} 
			if( boRecusa == 'S' ){
				titulo += '* Para esta obra foi recusada a solicitação de reformulação pelo tecnico, sendo assim não podendo solicitar novamente.<br>';
			}
			if( titulo != '' ){
				var texto = '<b>Esta obra não pode ser Reformulada / Cancelada:</b><br>'+titulo;
				jQuery('.pendeciaobra_'+preid).val(texto);
			} else {
				jQuery('#span_'+preid).css('display', 'none');
			}
		});
		
		if( (prmanalisecontrapartida == 'N' && boHabilitaContPartida == false) || boObraAprovada == true ){
			jQuery('.td_projeto_analise_'+proid).css('display', 'none');
			jQuery('.cab_projeto_analise_'+proid).css('display', 'none');
		} else {
			jQuery('.td_projeto_analise_'+proid).css('display', '');
			jQuery('.cab_projeto_analise_'+proid).css('display', '');
		}

		AddTableRow(proid);
	}

	function carregarListaDivObras( processo, proid, estuf ){
		jQuery.ajax({
	   		type: "POST",
	   		url: 'par.php?modulo=principal/pedidoReformulacaoObras&acao=A',
	   		data: "requisicao=carregarListaObras&processo="+processo+'&proid='+proid+'&estuf='+estuf,
	   		async: false,
	   		success: function(msg){
	   	   		jQuery('#listaobraprocesso_'+processo).html(msg);
	   		}
		});
	}
	
	function retiraPontosPedido(v){
		if( v != 0 ){
			var valor = v.replace(/\./gi,"");
			valor = valor.replace(/\,/gi,".");
		} else {
			var valor = v;
		}
		
		return valor;
	}

	function calculaContraPartida(proid, preid, bolimpa){
		
		var vrlcontrapartidaTotal	= jQuery('[name="contrapartidatotal_'+proid+'"]').val();			
		var vlrTotContraInformada 	= 0;
		
		jQuery('[name="preid['+proid+'][]"]').each(function(){
			var preid = jQuery(this).val();

			if( bolimpa == 'N' ){
				
				var acaosolicitada 				= jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').val();
				var sfocontrapartidainformada 	= jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val();
				var sfocontrapartida		 	= jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val();
	
				if( acaosolicitada == 'RE' ){
					sfocontrapartida = retiraPontosPedido(sfocontrapartida);
					sfocontrapartidainformada = retiraPontosPedido(sfocontrapartidainformada);
					vlrTotContraInformada = parseFloat(vlrTotContraInformada) + parseFloat(sfocontrapartidainformada);
					
					if( parseFloat(sfocontrapartida) < parseFloat(sfocontrapartidainformada) ){
						alert('A contrapartida informada não pode ser maior que a contrapartida disponível');
						jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val('0,00');
						vlrTotContraInformada = parseFloat(vlrTotContraInformada) - parseFloat(sfocontrapartidainformada);
					}
				}
			} else {
				jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val('0,00');
			}
		});

		if( parseFloat(vlrTotContraInformada) > parseFloat(vrlcontrapartidaTotal) ){
			alert('O valor total de contrapartida informada não pode ser maior que o Valor Geral de Contrapartida');
			calculaContraPartida(proid, '', 'S');
		}
		jQuery('#div_total_vrlcontrapartidainformada_'+proid).html(number_format(vlrTotContraInformada, 2, ',', '.' ));
	}

	(function(jQuery) {
		//jQuery('[type="radio"]').
		
		  /*RemoveTableRow = function(handler) {
		    var tr = jQuery(handler).closest('tr');

		    tr.fadeOut(400, function(){ 
		      tr.remove(); 
		    }); 

		    return false;
		  };*/
		  
		  AddTableRow = function( proid ) {
			//jQuery('#td_products').show();
			
			var boSelect = false;
			var totalObra = 0;
			var totalProjeto = 0;
			var totAlunos = 0;
			var totAlunosAlt = 0;
			var totLinha = 0;
			var boJustificativa = false;
			var total_total_vrlcontrapartida = 0;
			var sfocontrapartidainformada 	= 0 ;
			var estadoAtual 				= '';
			
			jQuery('[name="preid['+proid+'][]"]').each(function(){
				var preid 						= jQuery(this).val();
				var boSfoacaosolicitada 		= jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').length;
				var ptoid 						= jQuery('[name="ptoid['+proid+']['+preid+']"]').val();
				var predescricao 				= jQuery('[name="predescricao['+proid+']['+preid+']"]').val();
				var ptodescricao 				= jQuery('[name="ptodescricao['+proid+']['+preid+']"]').val();
				//var sfojustificativa 			= jQuery('[name="sfojustificativa['+proid+']['+preid+']"]').val();
				var tipoAlteradoText			= jQuery("[name='ptoid["+proid+"]["+preid+"]'] :selected").text();
				var tipoAlterado				= jQuery("[name='ptoid["+proid+"]["+preid+"]']").val();
				var alunosatendido 				= jQuery('[name="alunosatendido['+proid+']['+preid+']"]').val();
				sfocontrapartidainformada 		= jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val();
				estadoAtual 					= jQuery('[name="estadoAtual['+proid+']['+preid+']"]').val();
				var valorProjeto				= jQuery('#div_valorprojeto_'+preid).html();
				var valorObra					= jQuery('#div_valorobra_'+preid).html();				
				
				//jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]').tooltip();
								
				if( valorProjeto == '' ) {
					valorProjeto = 0;
					jQuery('[name="valorprojeto['+proid+']['+preid+']"]').val(0);
				}
				
				var alunosatendidoAlt = 0;
				if( tipoAlterado == 73 ){//Projeto 1 Convencional
					alunosatendidoAlt = 188;
				}
				if( tipoAlterado == 74 ){//Projeto 2 Convencional
					alunosatendidoAlt = 94;
				}
				
				//if( sfojustificativa != '' ) tipoAlteradoText = '<center>-</center>';

				if( boSfoacaosolicitada > 0 ){
					boSelect = true;
					jQuery('#td_products_'+proid).css('display', '');
					jQuery('#'+preid).remove();

					var acaosolicitada = jQuery('[name="sfoacaosolicitada['+proid+']['+preid+']"]:checked').val();
					
					//if( acaosolicitada == 'RE' ) jQuery('[name="sfojustificativa['+proid+']['+preid+']"]').val('');
					if( acaosolicitada == 'RE' ){
						var vrlProjeto = retiraPontosPedido(valorProjeto);
						var vrlObra = retiraPontosPedido(valorObra);
						if( parseFloat(vrlProjeto) > parseFloat(vrlObra) ){
							var contrapartida = (parseFloat(vrlProjeto) - parseFloat(vrlObra));
							jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val( number_format(contrapartida, 2, ',', '.' ) );
							jQuery('#div_projeto_analise_'+proid+'_'+preid).html( number_format(contrapartida, 2, ',', '.' ) );
						} else {
							jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val( '0,00' );
							/*jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').addClass('disabled');
							jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').attr('readonly', true);*/
						}
						
						boJustificativa = true;

						if( estadoAtual == 1578 ){ //Diligencia 
							jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').attr('disabled', false);
						} else {
							jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').attr('disabled', true);
							//jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val('0,00');
						}
					} else {
						jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').attr('disabled', true);
						jQuery('#div_projeto_analise_'+proid+'_'+preid).html( '0,00' );
						//jQuery('[name="sfocontrapartidainformada['+proid+']['+preid+']"]').val('0,00');
					}
					
					var div_total_vrlcontrapartida	= jQuery('#div_projeto_analise_'+proid+'_'+preid).html();
					total_total_vrlcontrapartida = parseFloat(total_total_vrlcontrapartida) + parseFloat(retiraPontosPedido(div_total_vrlcontrapartida));
					
					if( acaosolicitada == 'SA' ){
						if( alunosatendido == '' ) alunosatendido = 0;

						valorProjeto = valorObra;
						tipoAlteradoText = ptodescricao;

						alunosatendidoAlt = alunosatendido;
					}
					//var sfocontrapartida 	= jQuery('[name="sfocontrapartida['+proid+']['+preid+']"]').val();
					
					if( tipoAlteradoText == 'Selecione' ) tipoAlteradoText = '<center>-</center>'; 

					var cor = (totLinha % 2) ? 'style="background-color: rgb(191, 213, 239);"': '';
					
					var newRow = jQuery('<tr '+cor+' id="'+preid+'">');
					var cols = "";
					
	      			cols += '<td height="20px"><b>'+preid+'</b></td>';
	      			cols += '<td><b>'+predescricao+'</b></td>';
	      			cols += '<td><b>'+ptodescricao+'</b></td>';
	      			cols += '<td><b>'+valorObra+'</b></td>';
	      			cols += '<td align=center><b>'+alunosatendido+'</b></td>';
	      			cols += '<td height="20px"><b>'+tipoAlteradoText+'</b></td>';
	      			cols += '<td><b>'+valorProjeto+'</b></td>';
					cols += '<td align=center><b>'+alunosatendidoAlt+'</b></td>';
	      
	      			newRow.append(cols);	      
	      			jQuery("#products-table_"+proid).append(newRow);
	      			if( valorProjeto == '' ) valorProjeto = 0;
	      			totalObra = parseFloat(totalObra) + parseFloat(retiraPontosPedido(valorObra));
	      			totalProjeto = parseFloat(totalProjeto) + parseFloat(retiraPontosPedido(valorProjeto));

	      			totAlunos += parseFloat(alunosatendido);
	    			totAlunosAlt += parseFloat(alunosatendidoAlt);
				}
				
				totLinha++;
			});

			if( boJustificativa == true ){
				jQuery('#tr_label_just_'+proid).css('display', '');
				jQuery('#tr_campo_just_'+proid).css('display', '');
			} else {
				jQuery('#tr_label_just_'+proid).css('display', 'none');
				jQuery('#tr_campo_just_'+proid).css('display', 'none');
			}

			jQuery('#div_total_vrlprojeto_'+proid).html(number_format(totalProjeto, 2, ',', '.' ));
			
			//Insere tabela de Totais 
			jQuery('#total_'+proid).remove();
			var newRowTot = jQuery('<tr id="total_'+proid+'">');
			var colsTot = "";
			colsTot += '<th class="titulo_resumo" height="20px">&nbsp;</th>';
			colsTot += '<th class="titulo_resumo" style="text-align: right; "><b>Totais:</b></th>';
			colsTot += '<th class="titulo_resumo">&nbsp;</th>';
			colsTot += '<th class="titulo_resumo" style="text-align: left;"><b>'+number_format(totalObra, 2, ',', '.' )+'</b></th>';
			colsTot += '<th class="titulo_resumo" align=center>'+totAlunos+'</th>';
			colsTot += '<th class="titulo_resumo" style="text-align: right; "><b>Totais:</b></th>';
			colsTot += '<th class="titulo_resumo" style="text-align: left;"><b>'+number_format(totalProjeto, 2, ',', '.' )+'</b></th>';
			colsTot += '<th class="titulo_resumo" align=center>'+totAlunosAlt+'</th>';  
			newRowTot.append(colsTot);	      
  			jQuery("#products-table_"+proid).append(newRowTot);


  			var contrapartidaTot = 0;  			
  			if( parseFloat(totalProjeto) > parseFloat(totalObra) ){
				contrapartidaTot = (parseFloat(totalProjeto) - parseFloat(totalObra));				
			}

  			/*
			1561 - WF_TIPO_EM_AGUARDANDO_VALIDACAO_REFORMULACAO_MI_PARA_CONVENCIONAL
			1579 - WF_TIPO_EM_ANALISE_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA
			1578 - WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL 
			*/
			if( (sfocontrapartidainformada == '' || parseFloat(sfocontrapartidainformada) == parseFloat('0,00')) && 
					(estadoAtual != 1561 && estadoAtual != 1579 && estadoAtual != 1578 && estadoAtual != 228) ){
				contrapartidaTot = '0,00';
			}
			
  			//Insere Tabela de Contrapartida
  			jQuery('#contra_'+proid).remove();
			var newRowContra = jQuery('<tr id="contra_'+proid+'">');
			var colsContra  = '<th style="text-align: center;" colspan=15 height="25px" class="titulo_resumo"><b>Proposta de Valor Geral de Contra Partida: '+number_format(contrapartidaTot, 2, ',', '.' )+'</b>';  
				colsContra += '<input type="hidden" id="contrapartidatotal_'+proid+'" name="contrapartidatotal_'+proid+'" value="'+contrapartidaTot+'"></th>';  
			newRowContra.append(colsContra);	      
  			jQuery("#products-table_"+proid).append(newRowContra);

  			if( jQuery('.cab_projeto_analise_'+proid).css('display') == 'none' ){
  				jQuery("#th_vrl_geral_contra_msg_"+proid).css('display', 'none');
  	  		} else {  			
	  			jQuery("#th_vrl_geral_contra_msg_"+proid).css('display', '');
	  			jQuery("#div_vrl_geral_contra_msg_"+proid).html('Valor total de contrapartida a ser dsitribuido entre as obras: R$ ' + number_format(contrapartidaTot, 2, ',', '.' ) );
  	  		}

  			jQuery("#div_total_vrlcontrapartida_"+proid).html( number_format(total_total_vrlcontrapartida, 2, ',', '.' ) );

			var totalTR = jQuery('#products-table_'+proid).find('tr').length;

			jQuery('#td_row_'+proid).attr('rowspan', (totalTR - 2));
			
			if( boSelect == false ){
				jQuery('#td_products_'+proid).css('display', 'none');
			}
		  };
		  
		})(jQuery);
</script>