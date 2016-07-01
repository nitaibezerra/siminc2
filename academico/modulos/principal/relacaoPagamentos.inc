<?php
require_once APPRAIZ . "academico/classes/Processo.class.inc";
require_once APPRAIZ . 'academico/classes/Pagamento.class.inc';
require_once APPRAIZ . 'academico/classes/NaturezaDespesa.class.inc';
require_once APPRAIZ . "includes/classes/dateTime.inc";

# verifica se tem permissão de gravar
$habilitado = (possuiPerfilCadastro()) ? 'S' : 'N';
$habilitaComboPopup = ($habilitado == 'S') ? false : true;

if($_POST && $habilitado == 'S'){
	
	extract($_POST);
	
 	$pagvalor = str_replace(".", "", $pagvalor);
	$pagvalor = str_replace(",", ".", $pagvalor);
	
	$obPagamento = new Pagamento();
	
	$obPagamento->ndpcod = $ndpcod;
		
	$obPagamento->pagid 		  = $pagid;
	$obPagamento->prcid 		  = $_SESSION['academico']['prcid'];
	# $obPagamento->entid 		  = $entid;
	//$obPagamento->ndpid 		  = $ndpid;
	$obPagamento->pagcredor 	  = $pagcredor;
	$obPagamento->pagnumerodoc 	  = $pagnumerodoc;
	$obPagamento->pagdtdoc 		  = formata_data_sql($pagdtdoc);
	$obPagamento->pagnumerotitulo = $pagnumerotitulo;
	$obPagamento->pagdttitulo 	  = formata_data_sql($pagdttitulo);
	$obPagamento->pagvalor 		  = $pagvalor;
	$obPagamento->salvar();
	$boSucesso = $obPagamento->commit();
	if(!$boSucesso){
		$obPagamento->rollback();
	}
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/relacaoPagamentos","");
	unset($_POST);
	unset($obPagamento);
	die;
}


if($_GET['pagidExcluir']){
	$obPagamento = new Pagamento();
	$obPagamento->excluir($_GET['pagidExcluir']);
	$obPagamento->commit();
	unset($_GET['pagidExcluir']);
	unset($obPagamento);
	$db->sucesso("principal/relacaoPagamentos");
	die;
}

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';

echo montarAbasArray( criaAbaPortaria(), "academico.php?modulo=principal/relacaoPagamentos&acao=A" );

$titulo_modulo = "RELATÓRIO DE PAGAMENTOS";
monta_titulo( $titulo_modulo, 'Pagamentos' );

$obPagamento = new Pagamento($_GET['pagid']);
$obNaturezaDespesa = new NaturezaDespesa();
if($obPagamento->ndpid){
	$ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obPagamento->ndpid);	
}

$obData = new Data();

$obProcesso = new Processo($_SESSION['academico']['prcid']);

include_once APPRAIZ . "includes/workflow.php";
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-fixedheadertable/fixedHeaderTable.css" />
<script type="text/javascript" src="../includes/jquery-fixedheadertable/jquery.fixedheadertable.1.0.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function() {
	//$('.scrollCreditoOrc').fixedHeaderTable();
	
	$('.trRelacaoPagamento')
		.live('mouseover',function(){
			$('td', this).addClass('highlight');
		})
		.live('mouseout',function(){
			$('td', this).removeClass('highlight');
		});

	$('#salvar').click(function(){
		
		var msg = "";
		
		if($('#pagcredor').val() == ''){
			msg += "O campo Credor é obrigatório.\n";
		}
		if($('#ndpcod').val() == ''){
			msg += "O campo Nat. Despesa é obrigatório.\n";
		}
		if($('#pagnumerodoc').val() == ''){
			msg += "O campo Doc. Pag Número é obrigatório.\n";
		}
		if($('#pagdtdoc').val() == ''){
			msg += "O campo Doc. Pag Data é obrigatório.\n";
		}
		if($('#pagnumerotitulo').val() == ''){
			msg += "O campo Tit. Crédito Número é obrigatório.\n";
		}
		if($('#pagdttitulo').val() == ''){
			msg += "O campo Tit. Crédito Data é obrigatório.\n";
		}
		if($('#pagvalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		
		if(msg){
			alert(msg);
			return false;
		}
		
		$('#formulario').submit();	
	});

	$('.alterar').click(function(){
		window.location.href = "academico.php?modulo=principal/relacaoPagamentos&acao=A&pagid="+$(this).attr('id');
	});

	$('.excluir').click(function(){
		if(confirm('Deseja excluir registro')){
			window.location.href = "academico.php?modulo=principal/relacaoPagamentos&acao=A&pagidExcluir="+$(this).attr('id');
			return true;
		} else {
			return false;
		}
	});

	/*$('#ndpcod').blur(function(){
		var ndpcod = $(this).val();
		if(ndpcod != ''){
			var param = new Array();
				param.push({name : 'requisicao', value : 'existeCodNatureza'}, 
						   {name : 'ndpcod', value : ndpcod}
				);
			
			$.ajax({
				type	 : "POST",
				url		 : "academico.php?modulo=principal/relacaoPagamentos&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
								if(data == false){
									alert('Não existe Natureza de código '+ndpcod);
									//$('#ndpcod').focus();
								}
						   }
				 });
		}
	});*/
	
});

function limparCampos()
{
	$('#pagid').val('');
	$('#prcid').val('');
	$('#pagcredor').val('');
	$('#pagnumerodoc').val('');
	$('#pagdtdoc').val('');
	$('#pagnumerotitulo').val('');
	$('#pagdttitulo').val('');
	$('#pagvalor').val('');
	$('#ndpcod').val('');
	//campo_popup_remover_item( 'ndpid');
	return false;
}

//-->
</script>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
		<tr>
			<td width="50%" valign="top"><strong>Executor N°: <?php echo $db->pegaUm("select unicod || ' - ' || unidsc as descricao from public.unidade where unistatus = 'A' and unicod = '{$obProcesso->unicodexecutor}'"); ?></strong></td>
			<td width="50%">
				<table>
					<tr>
						<td width="200px" class="SubTituloDireita" valign="bottom">Portaria N°:</td>
						<td width="500px"><?php echo "<strong>".$obProcesso->prtid."</strong>"; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" valign="bottom">Periodo de Execução:</td>
						<td>
							<?php 
								echo "<strong>".$obProcesso->prndtinicio.' - '.$obProcesso->prndtfim."</strong>"; 
							?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="2" width="50%">
				<form action="" id="formulario" name="formulario" method="post">
					<input type="hidden" name="pagid" id="pagid" value="<?php echo $obPagamento->pagid; ?>" />
					<div class="scrollCreditoOrc" >
						<table class="tabela" align="center" bgcolor="#000" background="#dcdcdc" border="0" cellpadding="3" cellspacing="1" width="80%">
							<thead>
								<tr>
									<th rowspan="2" class="SubTituloCentro" style="background-color: #dcdcdc">CPF/CNPJ do Credor</th>
									<th rowspan="2" class="SubTituloCentro" style="background-color: #dcdcdc">Nat. Despesa</th>
									<th colspan="2" class="SubTituloCentro" style="background-color: #dcdcdc">Ordem Bancária</th>
									<th colspan="3" class="SubTituloCentro" style="background-color: #dcdcdc">Título de Crédito</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">&nbsp;</th>
								</tr>
								<tr>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Número</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Data</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Número</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Data</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Valor</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Ações</th>
								</tr>
								<tr bgcolor="#FFFFFF">
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php 
											$pagcredor = $obPagamento->pagcredor;
											echo campo_texto( 'pagcredor', 'N', $habilitado, '', 12, 500, '', '', '', '','', "id='pagcredor'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff; width: 300px">
										<?php
											/*if($obPagamento->ndpid){
												$sql = "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obPagamento->ndpid} ";
												$ndpid_dsc = $db->pegaUm($sql);
												$ndpid = array(
														"codigo" => $obPagamento->ndpid,
														"descricao" => $ndpid_dsc
														);
											}
											
											ver($ndpid);*/
											
											
											/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
											campo_popup('ndpid',$sql,'Selecione','','400x800','20', array(
																										array("codigo" => "ndpcod",
															  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																										array("codigo" => "ndpdsc",
																											  "descricao" => "<b>Descrição:</b>","string" => "1")
																										), 1, false, $habilitaComboPopup);
											*/
											//$complemento = "";
											$sql = "select ndpcod as codigo, 
														   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
											texto_popup( 'ndpcod', $sql, 'Natureza', 8, 10, '########', '', $complemento );
										?> 
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$pagnumerodoc = $obPagamento->pagnumerodoc;
											echo campo_texto( 'pagnumerodoc', 'N', $habilitado, '', 12, 500, '', '', '', '','', "id='pagnumerodoc'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											if($obPagamento->pagdtdoc){
												$pagdtdoc = $obData->formataData($obPagamento->pagdtdoc,"dd/mm/YYYY");										
											}
											echo campo_data2('pagdtdoc', 'N', $habilitado, 'Data', '##/##/####'); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$pagnumerotitulo = $obPagamento->pagnumerotitulo;
											echo campo_texto( 'pagnumerotitulo', 'N', $habilitado, '', 12, 500, '', '', '', '','', "id='pagnumerotitulo'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											if($obPagamento->pagdttitulo){
												$pagdttitulo = $obData->formataData($obPagamento->pagdttitulo,"dd/mm/YYYY");										
											}
											echo campo_data2('pagdttitulo', 'N', $habilitado, 'Data', '##/##/####'); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$pagvalor = ($obPagamento->pagvalor) ? number_format($obPagamento->pagvalor,2,",",".") : "" ;
											echo campo_texto( 'pagvalor', 'N', $habilitado, '', 12, 500, '###.###.###.###,##', '', '', '','', "id='pagvalor'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php if($habilitado == 'S'): ?>
											<img src="/imagens/gif_inclui.gif" id="salvar" style="cursor: pointer;" border=0 title="Salvar" />
										<?php else:?>
											<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
										<?php endif;?>
										&nbsp; <a href="javascript: void(0);" onclick="limparCampos();"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
									</th>
								</tr>
							</thead>
							<tbody>
								<?php 
									//$sql = "SELECT entnumcpfcnpj, entnome, entemail, entnuninsest, entobs, entstatus, entnumrg FROM entidade.entidade limit 100";
									$sql = "SELECT p.pagid, 
												   p.ndpid,
												   nd.ndpcod||' - '||nd.ndpdsc as descricaoNatureza,
												   p.pagcredor, 
												   p.pagnumerodoc, 
												   p.pagdtdoc, 
												   p.pagnumerotitulo, 
												   p.pagdttitulo, 
												   p.pagvalor 
											FROM academico.pagamento p
												inner join public.naturezadespesa nd on p.ndpid = nd.ndpid
											where prcid = {$_SESSION['academico']['prcid']}
											--limit 20
									";
						            $arDados = $db->carregar( $sql );
						            $arDados = ($arDados) ? $arDados : array();
								?>
								<?php if($arDados): ?>
									<?php foreach ($arDados as $dados): ?>
									<tr class="trRelacaoPagamento">
										<td style="background-color: #ffffff;"><?php echo $dados['pagcredor']; ?></td>
										<td style="background-color: #ffffff" ><?php echo $dados['descricaonatureza']; ?></td>
										<td style="background-color: #ffffff" ><?php echo $dados['pagnumerodoc']; ?></td>
										<td style="background-color: #ffffff; text-align: center" ><?php echo $obData->formataData($dados['pagdtdoc'],"dd/mm/YYYY"); ?></td>
										<td style="background-color: #ffffff" ><?php echo $dados['pagnumerotitulo']; ?></td>
										<td style="background-color: #ffffff; text-align: center" ><?php echo $obData->formataData($dados['pagdttitulo'],"dd/mm/YYYY"); ?></td>
										<td style="background-color: #ffffff; text-align: right" ><?php echo number_format($dados['pagvalor'],2,",","."); ?></td>
										<td style="background-color: #ffffff; text-align: center; width: 50px">
											<?php if($habilitado == 'S'): ?>
												<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $dados['pagid']; ?>" class="alterar">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $dados['pagid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluir">
											<?php else:?>
												<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
											<?php endif;?>
										</td>
									</tr>
									<?php 
										$somapagvalor = $somapagvalor + $dados['pagvalor'];
									?>
									<?php endforeach;?>
									<tr bgcolor="#ffffff" class="trFisicoFinanceiro">
								    	<td colspan="6" style="background-color: #ffffff; text-align: right"><strong>TOTAL:</strong></td>
								    	<td style="background-color: #ffffff; text-align: right"><strong><?php echo number_format($somapagvalor,2,",","."); ?></strong></td>
								    	<td style="background-color: #ffffff;">&nbsp;</td>
								    </tr>
								<?php else: ?>
									<tr bgcolor="#ffffff">
								    	<td colspan="8"><strong><center>Não foram encontrados registros</center></strong></td>
								    </tr>
								<?php endif;?>
							</tbody>
						</table>
					</div>
				</form>
			</td>
			<td>
			<?php 
				$tmcid = $_SESSION['academico']['tmcid'];
				if( $tmcid )
				{
					$docid = tcPegarDocid($tmcid);
					
					if($docid)
					{ 
						wf_desenhaBarraNavegacao( $docid , array( 'tmcid' => $tmcid ) );
					}				
				}
				?> 
			</td>
		</tr>
</table>
<div id="divDebug"></div>
<style type="text/css">
div.scrollCreditoOrc {
	/*width: 100%;
	height: 170px;*/
}
</style>