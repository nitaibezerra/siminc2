<?php 
#Excluir previsão orçamentaria
if($_REQUEST['requisicao'] == 'excluirPrevisao'){

	if($_REQUEST['id']){
		$sql = "delete from monitora.previsaoorcamentaria where proid = {$_REQUEST['id']}";
		$db->executar($sql);
		if($db->commit()){
			die('1');
		}
	}
	die('0');
}

$contador = 1;

#Carrega planos de trabalhos autocomplete
if($_REQUEST['requisicao'] == 'carregaPlanosTrabalho'){

	$sql = "SELECT * FROM (
				SELECT DISTINCT
					p.ptrid as codigo,
					ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
				FROM monitora.ptres p
				JOIN public.unidadegestora u
					ON u.unicod = p.unicod
				WHERE p.ptrano = '{$_SESSION['exercicio']}'
				AND p.ptrstatus = 'A'
				AND u.unicod IN ( '26101','26298','26291','26290' )
			) AS foo
			".($_GET["q"] ? "WHERE descricao ILIKE '%{$_GET["q"]}%'" : "")."";

	$arDados = $db->carregar($sql);

	$q = strtolower($_GET["q"]);
	if($arDados){
		foreach ($arDados as $key=>$value){
			echo "{$value['descricao']}|{$value['codigo']}\n";
		}
	}else{
		echo "Sem registros|";
	}
	die;
}

if(	$emCadastramentoCorrecao && in_array(UO_EQUIPE_TECNICA, $perfis) && $boEmAlteracao){
	$habilita 				= 'S';
	$habilita_Und_G 		= 'S';
	$habilitaSalvar 		= 'S';
	$habilitaInserir 		= 'S';
	$habilita_Und_G 		= 'S';
	$habilita_pro_con 		= 'S';
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<style>
	.linkLote{
		cursor:pointer;
		color:blue;
	}
	.linkLote:hover{
		cursor:pointer;
		text-decoration:underline;	
		color:blue;
	}
</style>
<script type="text/javascript">
	$(function(){

		$('.linkLote').click(function(){
			var visualizarLote = window.open( '../elabrev/geral/visualizarPagamentoLote.php?codncsiafi='+$(this).attr('codigo'), 'Combopopup', "height=190,width=400,scrollbars=yes,top=250,left=200" );
			visualizarLote.focus();
		});

		// Inserir linha de previsão orçamentária 
		$('.inserirPrev').click(function(){
			var tamanho = $('#previsao tr:last').attr('id').replace('tr_','');
			if( tamanho == 'titulo' ){
				tamanho = 0;
			}
			tamanho = parseInt(tamanho)+1;
			tamanho = tamanho+'NEW';
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=novaLinhaPrevisao&cod="+tamanho,
				async: false,
				success: function(msg){
					$('#previsao tr:last').after(msg);
					$.each($("[name=crdmesexecucao[]]"), function(i, v){
						if(i==0){
							valor = $(v).val();
						}else{
							$(v).val(valor).attr('disabled', true);
						}
					});	
				}
			});
		});

		// Botão excluir linha previsão orçamentária 
		$('.excluirPrevisao').live('click',function(){
			if(confirm('Deseja deletar este registro?')){
				var id = $(this).attr('id');				
				if(id.indexOf('NEW')>0){
					$(this).parent().parent().remove();
				}else{
					$.ajax({
						url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao',
						type	: 'post',
						data	: 'requisicao=excluirPrevisao&id='+id,
						success	: function(e){		
							if(e=='1'){
								alert('Operação realizada com sucesso.');
								$('#tr_'+id).remove();
							}else{
								alert('Falha ao tentar excluir o anexo!');
							}
						}
					});
				}
			}
		});

		// Seleciona plano de trabalho
		$('[name="ptrid[]"]').live('change',function(){		
			linha = $(this).parent().attr('id');
			linha = linha.split('_');
			var ptrid = $(this).val();	
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaNomeAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){
					jQuery('#td_acao_'+linha[2]).html(msg);
				}
			});
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaPI&ptrid="+ptrid,
				async: false,
				success: function(msg){
					jQuery('#td_pi_'+linha[2]).html(msg);
				}
			});		
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaDescAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){				
					jQuery('#td_acaodsc_'+linha[2]).html(msg);
				}
			});
			return true;
		});

		// Substitui os meses de execução pelo primeiro
		$('[name=crdmesexecucao[]]').change(function(){
			valor = this.value;
			$.each($('[name=crdmesexecucao[]]'), function(i,v){	
				if(i>0){
					$(v).attr('disabled', false).val(valor).attr('disabled', true);
				}
			});
		});
		$('[name=crdmesexecucao[]]').live('change',function(){
			valor = this.value;
			$.each($('[name=crdmesexecucao[]]'), function(i,v){	
				if(i>0){
					$(v).attr('disabled', false).val(valor).attr('disabled', true);
				}
			});
		});

		// Bloqueia mês de execucão
		$.each($("[name=crdmesexecucao[]]"), function(i, v){
			if(i==0){
				valor = $(v).val();
			}else{
				$(v).val(valor).attr('disabled', true);
			}
		});

		var urlPlanoTrabalho = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao&requisicao=carregaPlanosTrabalho';

		$("[name^='ptrid_temp']").autocomplete(urlPlanoTrabalho, {			
			matchContains: true,
			minChars: 0, 			
			cacheLength:1000,
			width: 440,
			autoFill: false	,max: 1000    		
		}).result(function(event, data, formatted) {
			
			id_temp = event.target.id
			arTemp = id_temp.split('_');
			$("#ptrid_"+arTemp[1]).val(data[1]);
						
			var ptrid = data[1];
			
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaNomeAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){									
					jQuery('#td_acao_'+arTemp[1]).html(msg);
				}
			});
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaPI&ptrid="+ptrid,
				async: false,
				success: function(msg){
					jQuery('#td_pi_'+arTemp[1]).html(msg);
				}
			});		
			$.ajax({
				type: "POST",
				url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
				data: "req=atualizaDescAcao&ptrid="+ptrid,
				async: false,
				success: function(msg){				
					jQuery('#td_acaodsc_'+arTemp[1]).html(msg);
				}
			});
			
		});

		// Calcula subtotais por ano		
		$("[name^='provalor'], [name^='proanoreferencia']").live('change', function(){

			var arrAnos = new Array();
						
			$("[name^='proanoreferencia']").each(function(i,v){			
				if($.inArray($(v).val(), arrAnos)<0){					
					arrAnos.push($(v).val());					
				}
			});

			var arrAnosTd = new Array();
			$("[id^='td_subtotalano_']").each(function(i,v){
				anoTd = $(v).attr('id').split('_')[2];			
				if($.inArray(anoTd, arrAnosTd)<0){					
					arrAnosTd.push(anoTd);					
				}
			});

			// Oculta/Mostra td com anos sem valores
			if(arrAnosTd.length>0){
				$.each(arrAnosTd, function(indice, ano){
					if($.inArray(ano, arrAnos)<0){
						if($('#td_subtotalano_'+ano).length>0){
							$('#td_subtotalano_'+ano).parent().hide();
						}
					}else{
						if($('#td_subtotalano_'+ano).length>0){
							$('#td_subtotalano_'+ano).parent().show();
						}
					}
				});
			}
			
			// Soma subtotais
			if(arrAnos.length>0){
				$.each(arrAnos, function(indice,ano){					
					subTotalAno = 0;					
					$("[name^='proanoreferencia']").each(function(key,obj){
						if($(obj).val() == ano){
							id = $(obj).attr('id').split('_')[1];

							valor = $('#provalor_'+id).val();
							
							if(valor)
								valor = replaceAll(valor, '.', '').replace(',', '.');
							else
								valor = 0;

							subTotalAno = parseFloat(subTotalAno)+parseFloat(valor);							
						}
					});

					if(subTotalAno>0){
						subTotalAno = MascaraMonetario(''+subTotalAno+'');
					}
					
					if($('#td_subtotalano_'+ano).length == 0){					
						
						var tamanho = $('#previsao tr:last').attr('id').replace('tr_','');
						if( tamanho == 'titulo' ){
							tamanho = 0;
						}
						tamanho = parseInt(tamanho)+1;
						tamanho = tamanho+'NEW';
						
						strNewTd = '<tr bgcolor="#f0f0f0" id="tr_'+tamanho+'">';
						strNewTd +=	'<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td align="right"><b>Subtotal ('+ano+')</b>&nbsp;</td>';
						strNewTd += '<td align="right" style="font-weight:bold;" id="td_subtotalano_'+ano+'">R$ '+subTotalAno+'</td>';
						strNewTd += '<td>&nbsp;</td>';
						strNewTd += '<td>&nbsp;</td>';							
						strNewTd += '</tr>';
						
						$('#previsao tr:last').after(strNewTd);
						
					}else{
						
						$('#td_subtotalano_'+ano).text('R$ '+subTotalAno+'');
						
					}
					
				});
			}
		});
		
	});
</script>
<?php

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Termo de Cooperação", '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();


?>
<form method="post" name="formulario" id="formulario">

	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	
	<input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Previsão Orçamentária</b></center>
			</td>
		</tr>
	</table>
	
	<table id="previsao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro" >&nbsp;</td>
			<td class="subtitulocentro" >&nbsp;</td>
			<td class="subtitulocentro" >Ano</td>
			<td class="subtitulocentro" >Ação</td>
			<td class="subtitulocentro" >Programa de Trabalho</td>
			<td class="subtitulocentro" >Plano Interno</td>
			<td class="subtitulocentro" >Descrição da Ação Constante da LOA</td>
			<td class="subtitulocentro" >Nat.da Despesa</td>
			<td class="subtitulocentro" >Valor (em R$ 1,00)</td>
			<td class="subtitulocentro" >Mês da Liberação</td>
			<td class="subtitulocentro" >Prazo para o cumprimento do objeto</td>
		</tr>
		<?php 
		$sql = "SELECT DISTINCT
					pro.proid,
					ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
					substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
					substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
					pro.ptrid,
					a.acacod,
					pro.pliid,
					case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
					pro.ndpid,
					to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
					coalesce(pro.provalor, 0) as valor,
					crdmesliberacao,
					crdmesexecucao,
					pro.proid,
					pro.proanoreferencia,
					pro.prodata,
					(select ltb.tcpnumtransfsiafi from elabrev.previsaoparcela ltb where pro.proid = ltb.proid and ppacancelarnc = false) as lote
				FROM monitora.previsaoorcamentaria pro
				LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
				LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
				LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
				LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
				LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
				LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
				LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
				WHERE pro.prostatus = 'A'
				AND tcpid = ".$_SESSION['elabrev']['tcpid']."
				ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao";
		// ver($sql,d);
				
		$dados = !empty($_SESSION['elabrev']['tcpid']) ? $db->carregar($sql) : array();
		
			if( $dados[0] != '' ){
				
				$arAnosPrevisao = array();
				$arLote = array();
				$totalPrevisao = count($dados)-1;

				foreach( $dados as $k => $dado ){
					
					#Imprime subtotal por ano
					if(!in_array($dado['proanoreferencia'], $arAnosPrevisao)){					
		  				if($subTotalPorAno>0){
		  						echo '
									<tr bgcolor="#f0f0f0" id="tr_'.$k.'">
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td align="right"><b>Subtotal ('.($anoAnterior ? $anoAnterior : 'ano não informado').')</b>&nbsp;</td>
										<td align="right" id="td_subtotalano_'.($anoAnterior ? $anoAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorAno).'</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>										
									</tr>
								';
		  				}
						array_push($arAnosPrevisao, $dado['proanoreferencia']);
	  					$subTotalPorAno = 0;
	  					$anoAnterior = $dado['proanoreferencia'];
					}

					#Imprime subtotal por lote
					if(!in_array($dado['lote'], $arLote)){
		  				if($subTotalPorLote>0){
		  						echo '
									<tr bgcolor="#f0f0f0" id="tr_'.$k.'">
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>
										<td align="right"><span class="linkLote" codigo="'.$loteAnterior.'"><b>Subtotal ('.($loteAnterior ? $loteAnterior : 'lote não encontrado').')</b></span>&nbsp;</td>
										<td align="right" id="td_subtotalano_'.($loteAnterior ? $loteAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorLote).'</td>
										<td>&nbsp;</td>
										<td>&nbsp;</td>										
									</tr>
								';
		  				}
						array_push($arLote, $dado['lote']);
	  					$subTotalPorLote = 0;
	  					$loteAnterior = $dado['lote'];
					}
					
				?>
				<tr id="tr_<?=$dado['proid']?>">

					<td align="center">
						<?php if( $concedente != 'fnde' && !$dado['lote'] )
							echo "<input name='checkEnvio' id='checkEnvio' value='{$dado['proid']}' type='checkbox'/>"; ?>
					</td>
				
					<td align="center">
						<?php if($habilita_botao == 'S'){ ?> 
							<?php if( $k <> '0' ){?>
								<img border="0" id="<?=$dado['proid'] ?>" class="excluirPrevisao" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
							<?php }?>
						<?php }else{?>
							<img border="0" id="<?=$k ?>" title="Excluir" src="/imagens/excluir_01.gif">
						<?php }?>
						<input type="hidden" name="linha[]" value="<?=$dado['proid'] ?>"/>
					</td>
					
					<td align="center" id="td_anoref_<?=$dado['proid']; //$k ?>">
						<?php if($habilita_Natur == 'S'){?>
							<?php 
							for($z=0;$z<=10;$z++){
								$arAnosRef[$z]['codigo']	= 2013+$z; 
								$arAnosRef[$z]['descricao']	= 2013+$z;
							}
							
							$db->monta_combo('proanoreferencia[]',$arAnosRef, 'S', 'Selecione...','',$opc,'','','S', 'proanoreferencia_'.$dado['proid'], '', $dado['proanoreferencia'], $title= null);
							?>
						<?php }else{ ?>
							<?php echo $dado['proanoreferencia'] ? $dado['proanoreferencia'] : '-'; ?>
							<input type="hidden" name="proanoreferencia[]" id="proanoreferencia_<?php echo $dado['proid']; ?>" value="<?=$dado['proanoreferencia'] ?>"/>
						<?php } ?>
					</td>
					
					<td align="center" id="td_acao_<?=$k ?>">
						<?=$dado['acacod'] ?>
					</td>
					
					<?php if($habilita_Plano == 'S'){?>
						<td align="center" id="td_prg_<?=$dado['proid']; //$k ?>">
							<?php 
							$sql = "SELECT DISTINCT
										p.ptrid as codigo,
		 								ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
									FROM monitora.ptres p
									JOIN public.unidadegestora u 
										ON u.unicod = p.unicod
									WHERE p.ptrano = '2013' 
									AND p.ptrstatus = 'A'
									AND u.unicod IN ( '26101','26298','26291','26290' )";
								
							//$db->monta_combo('ptrid[]',$sql, 'S', 'Selecione...','',$opc,'','200','S', 'combobox', '', $dado['ptrid'], $title= null);
							?>
							<?=campo_texto('ptrid_temp[]','S','S','',30,21,'','', 'left', '', '', 'id="ptridtemp_'.$dado['proid'].'"', '', $dado['ptrid_descricao']);?>
							<input type="hidden" name="ptrid[]" id="ptrid_<?php echo $dado['proid']; ?>" value="<?php echo $dado['ptrid']; ?>" />
						</td>
					<?php }else{ ?>
						<td align="center" id="td_prg_<?=$k ?>">
							<input type="hidden" name="ptrid[]" value="<?=$dado['ptrid']?>">
							<?=$dado['ptrid_descricao']?$dado['ptrid_descricao']:'-'?>
						</td>
					<?php }?>
					
					<?php if($habilita_Plano == 'S'){?>
						<td align="center" id="td_pi_<?=$dado['proid']; //$k ?>">
							<?php 
								if( $dado['ptrid'] != '' ){
									$sql = "
										SELECT p.pliid as codigo,
												plicod||' - '||plidsc as descricao
										FROM monitora.pi_planointerno p
										INNER JOIN monitora.pi_planointernoptres pt on pt.pliid = p.pliid
										WHERE pt.ptrid = ".$dado['ptrid']." 
										ORDER by 2
									";
									
									echo $db->monta_combo('pliid[]',$sql, 'S', 'Selecione...','',$opc,'','100','S', 'pliid', '', $dado['pliid'], $title= null);
								}
							?>
						</td>
					<?php }else{?>
						<td align="center" id="td_pi_<?=$dado['proid']; //$k ?>">
							<input type="hidden" name="pliid[]" value="<?=$dado['pliid']?>">
							<?=$dado['pliid_descricao']?$dado['pliid_descricao']:'-'?>
						</td>
					<?php }?>
					
					<td align="center" id="td_acaodsc_<?=$dado['proid']; //$k ?>">
						<?=$dado['acatitulo']?$dado['acatitulo']:'-'?>
					</td>
					
					<?php if($habilita_Natur == 'S'){?>
						<td align="center">
							<?php 
								$sql = "
									SELECT 	DISTINCT ndpid as codigo,
											substr(ndpcod, 1, 6) || ' - ' || ndpdsc as descricao
									FROM public.naturezadespesa
									WHERE ndpstatus = 'A' and sbecod = '00' and edpcod != '00' and substr(ndpcod,1,2) not in ( '31', '32', '46' )
									AND ( substr(ndpcod, 3, 2) in ('80', '90', '91') or substr(ndpcod, 1, 6) in ('335039', '445041', '333041') )
									order by 2
								";
								$db->monta_combo('ndpid[]',$sql,'S','Selecione...','',$opc,'','300','S', 'ndpid', '', $dado['ndpid'], $title= null);
							?>
						</td>
					<?php }else{?>
						<td align="center">
							<input type="hidden" name="ndpid[]" value="<?=$dado['ndpid']?>">
							<?=$dado['ndp_descricao']?$dado['ndp_descricao']:'-'?>							
						</td>
					<?php }?>
					
					<?php if($habilita_Natur == 'S'){?>
						<td align="center">
							<?=campo_texto('provalor[]','S','S','',30,21,'[.###],##','', 'right', '', '', 'id="provalor_'.$dado['proid'].'"', '', $dado['provalor']);?>
						</td>
					<?php }else{?>
						<td align="center">
							<input type="hidden" name="provalor[]" value="<?=$dado['provalor']?>">
							<?=$dado['provalor']?>
						</td>
					<?php }?>
					
					<?php if($habilita_Meslib == 'S'){ ?>
						<td align="center">
							<?php 
								$sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
											 Array('codigo'=>2,'descricao'=>'Fevereiro'),
											 Array('codigo'=>3,'descricao'=>'Março'),
											 Array('codigo'=>4,'descricao'=>'Abril'),
											 Array('codigo'=>5,'descricao'=>'Maio'),
											 Array('codigo'=>6,'descricao'=>'Junho'),
											 Array('codigo'=>7,'descricao'=>'Julho'),
											 Array('codigo'=>8,'descricao'=>'Agosto'),
											 Array('codigo'=>9,'descricao'=>'Setembro'),
											 Array('codigo'=>10,'descricao'=>'Outubro'),
											 Array('codigo'=>11,'descricao'=>'Novembro'),
											 Array('codigo'=>12,'descricao'=>'Dezembro')
										);
								$db->monta_combo('crdmesliberacao[]',$sql,'S','Selecione...','',$opc,'','100','S', 'crdmesliberacao', '', $dado['crdmesliberacao'], $title= null);
							?>
						</td>
					<?php }else{ ?>
						<td align="center">
							<input type="hidden" name="crdmesliberacao[]" id="'crdmesliberacao[]" value="<?=$dado['crdmesliberacao']?>">
							<?php echo $dado['crdmesliberacao'] ? mes_extenso($dado['crdmesliberacao']) : '-';?>
						</td>
					<?php }?>
						
					<?php if($habilita_Parc == 'S'){?>
						<td align="center">
							<?php
								$sql = array();
								for($i = 1; $i <= 50; $i++){
									$sql[$i-1]['codigo'] = $i; 
									$sql[$i-1]['descricao'] = $i.' Mês(s)';						
								}
								array_push($sql, $sql);
								$db->monta_combo('crdmesexecucao[]', $sql,'S','Selecione...','',$opc,'','100','S', 'crdmesexecucao', '', $dado['crdmesexecucao'], $title= null);					
							?>
						</td>
					<?php }else{?>
						<td align="center">
							<input type="hidden" name="crdmesexecucao[]" id="'crdmesexecucao[]" value="<?=$dado['crdmesexecucao']?>">
							<?php echo $dado['crdmesexecucao'].' Mês(s)' ?>
						</td>
					<?php }?>
				</tr>					
				<?php

				// TODO: realizar a somatória baseada no mês
					if( $dado['lote'] != null )
						$subTotalPorLote = $subTotalPorLote+$dado['valor'];
					
					#Imprime ultimo subtotal se houver
					if($totalPrevisao==$k && $loteAnterior){
						echo '
						<tr bgcolor="#f0f0f0" id="tr_'.($k+1).'">
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td align="right"><span class="linkLote" codigo="'.$loteAnterior.'"><b>Subtotal ('.$loteAnterior.')</b></span>&nbsp;</td>
							<td align="right" id="td_subtotalano_'.($loteAnterior ? $loteAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorLote).'</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>							
						</tr>
						';
					}
				
					$subTotalPorAno = $subTotalPorAno+$dado['valor'];
					
					#Imprime ultimo subtotal se houver
					if($totalPrevisao==$k){
						echo '
						<tr bgcolor="#f0f0f0" id="tr_'.($k+1).'">
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
							<td align="right"><b>Subtotal ('.$anoAnterior.')</b>&nbsp;</td>
							<td align="right" id="td_subtotalano_'.($anoAnterior ? $anoAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorAno).'</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>							
						</tr>
						';
					}
					 
				}
			}
		?>
	</table>
	<?php if( ($habilitaInserir == 'S' && in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA, ALTERAR_TERMO_COOPERACAO))) || in_array(PERFIL_SUPER_USUARIO, $perfis) ):?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
			<tr>
				<td colspan="2">
					<a href="#" class="inserirPrev"> 
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos">						
						Inserir Nova Previsão</a>
				</td>
				<td style="width:56%;">
					<?php if( $concedente != 'fnde' ): ?>
						<input type='button' onclick='javascript:janelaDeEnvioParaPagamento();' value='Enviar para pagamento'/>
					<?php endif; ?>
				</td>
			</tr>
		</table>
	<?php endif; ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if($habilitaSalvar == 'S'): ?>
					<input type="button" id="descentralizacao" value="Gravar e Voltar" class="navegarSalvar"/> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>
					<input type="button" id="<?php echo in_array(UO_EQUIPE_TECNICA, $perfis) ? 'anexo' : 'parecertecnico' ?>" value="Gravar e Continuar" class="navegarSalvar"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>

<script>
function janelaDeEnvioParaPagamento(){

	var celulasOrcamentarias = new Array();
	$.each( $('[name="checkEnvio"]'), function(index,value){ 
		if( $(value).is(':checked') == true ){
			celulasOrcamentarias.push( $(value).parent().parent().attr('id').substring(3) );
		}
	});
	if( celulasOrcamentarias.length < 1 ){
		alert('Deve haver pelo menos 1 célula orçamentária marcada para envio.');
		return false;
	}

	var pagamento = window.open( '../elabrev/geral/envioParaPagamento.php', 'Combopopup', "height=180,width=400,scrollbars=yes,top=250,left=200" );
	pagamento.focus();
}
</script>