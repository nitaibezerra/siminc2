<?php
require_once APPRAIZ . "academico/classes/Processo.class.inc";
require_once APPRAIZ . 'academico/classes/FisicoFinanceiro.class.inc';

# verifica se tem permissão de gravar
$habilitado = (possuiPerfilCadastro()) ? 'S' : 'N';

if($_POST && $habilitado == 'S'){
	
	extract($_POST);
	
 	$fsfvlrconcedente = str_replace(".", "", $fsfvlrconcedente);
	$fsfvlrconcedente = str_replace(",", ".", $fsfvlrconcedente);
	
	$fsfvlrexecutor = str_replace(".", "", $fsfvlrexecutor);
	$fsfvlrexecutor = str_replace(",", ".", $fsfvlrexecutor);
	
	$fsfvlrtotal = str_replace(".", "", $fsfvlrtotal);
	$fsfvlrtotal = str_replace(",", ".", $fsfvlrtotal);
	
	$obFisicoFinanceiro = new FisicoFinanceiro();
	$obFisicoFinanceiro->fsfid 			  = $fsfid;
	$obFisicoFinanceiro->prcid 			  = $_SESSION['academico']['prcid'];
	$obFisicoFinanceiro->fsfdsc 		  = $fsfdsc;
	$obFisicoFinanceiro->fsfprogramado 	  = $fsfprogramado;
	$obFisicoFinanceiro->fsfexecutado 	  = $fsfexecutado;
	$obFisicoFinanceiro->fsfvlrconcedente = $fsfvlrconcedente;
	$obFisicoFinanceiro->fsfvlrexecutor   = $fsfvlrexecutor;
	$obFisicoFinanceiro->fsfvlrtotal 	  = $fsfvlrtotal;
	$obFisicoFinanceiro->fsfstatus 		  = 'A';
	if(!$fsfid){
		$obFisicoFinanceiro->fsfdtinclusao = date('Y-m-d');		
	}
	$obFisicoFinanceiro->salvar();
	$boSucesso = $obFisicoFinanceiro->commit();
	if(!$boSucesso){
		$obFisicoFinanceiro->rollback();
	}
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/fisicoFinanceiro","");
	unset($_POST);
	unset($obFisicoFinanceiro);
	die;
}


if($_GET['fsfidExcluir'] && $habilitado == 'S'){
	$obFisicoFinanceiro = new FisicoFinanceiro();
	$obFisicoFinanceiro->excluir($_GET['fsfidExcluir']);
	$obFisicoFinanceiro->commit();
	unset($_GET['fsfidExcluir']);
	unset($obFisicoFinanceiro);
	$db->sucesso("principal/fisicoFinanceiro");
	die;
}

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';

echo montarAbasArray( criaAbaPortaria(), "academico.php?modulo=principal/fisicoFinanceiro&acao=A" );

$titulo_modulo = "RELATÓRIO DE EXECUÇÃO FÍSICO-FINANCEIRA";
monta_titulo( $titulo_modulo, 'Físico-Financeira' );


$obFisicoFinanceiro = new FisicoFinanceiro($_GET['fsfid']);

$obProcesso = new Processo($_SESSION['academico']['prcid']);

include_once APPRAIZ . "includes/workflow.php";
?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-fixedheadertable/fixedHeaderTable.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-fixedheadertable/jquery.fixedheadertable.1.0.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function() {
	//$('.scrollCreditoOrc').fixedHeaderTable();
	$('.trFisicoFinanceiro')
		.live('mouseover',function(){
			$('td', this).addClass('highlight');
		})
		.live('mouseout',function(){
			$('td', this).removeClass('highlight');
		});

	$('#salvar').click(function(){

		var msg = "";
		
		if($('#fsfdsc').val() == ''){
			msg += "O campo Descrição é obrigatório.\n";
		}
		if($('#fsfprogramado').val() == ''){
			msg += "O campo Físico Programado é obrigatório.\n";
		}
		if($('#fsfexecutado').val() == ''){
			msg += "O campo Físico Executado é obrigatório.\n";
		}
		if($('#fsfvlrconcedente').val() == ''){
			msg += "O campo Financeiro Concedente é obrigatório.\n";
		}
		if($('#fsfvlrexecutor').val() == ''){
			msg += "O campo Financeiro Executor é obrigatório.\n";
		}
		if($('#fsfvlrtotal').val() == ''){
			msg += "O campo Financeiro Total é obrigatório.\n";
		}
		
		if(msg){
			alert(msg);
			return false;
		}
		
		$('#formulario').submit();	
	});

	$('.alterar').click(function(){
		window.location.href = "academico.php?modulo=principal/fisicoFinanceiro&acao=A&fsfid="+$(this).attr('id');
	});

	$('.excluir').click(function(){
		if(confirm('Deseja excluir registro')){
			window.location.href = "academico.php?modulo=principal/fisicoFinanceiro&acao=A&fsfidExcluir="+$(this).attr('id');
			return true;
		} else {
			return false;
		}
	});
});

function limparCampos()
{
	$('#fsfid').val('');
	$('#prcid').val('');
	$('#fsfdsc').val('');
	$('#fsfprogramado').val('');
	$('#fsfexecutado').val('');
	$('#fsfvlrconcedente').val('');
	$('#fsfvlrexecutor').val('');
	$('#fsfvlrtotal').val('');
	return false;
}

//-->
</script>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
	<tbody>
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
					<input type="hidden" name="fsfid" id="fsfid" value="<?php echo $obFisicoFinanceiro->fsfid; ?>" />
					<div class="scrollCreditoOrc" >
						<table class="tabela" align="center" bgcolor="#000" background="#dcdcdc" border="0" cellpadding="3" cellspacing="1" width="80%">
							<thead>
								<tr>
									<th rowspan="2" class="SubTituloCentro" style="background-color: #dcdcdc">Descrição</th>
									<th colspan="2" class="SubTituloCentro" style="background-color: #dcdcdc">Físico(%)</th>
									<th colspan="4" class="SubTituloCentro" style="background-color: #dcdcdc">Financeiro</th>
									<!-- <th class="SubTituloCentro" style="background-color: #dcdcdc">&nbsp;</th> -->
								</tr>
								<tr>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Programado</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Executado</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Concedente</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Executor</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Total</th>
									<th class="SubTituloCentro" style="background-color: #dcdcdc">Ações</th>
								</tr>
								<tr bgcolor="#ffffff">
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php 
											$fsfdsc = $obFisicoFinanceiro->fsfdsc;
											echo campo_texto( 'fsfdsc', 'N', $habilitado, '', 12, 500, '', '', '', '','', "id='fsfdsc'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff; width: 300px">
										<?php
											$fsfprogramado = $obFisicoFinanceiro->fsfprogramado; 
											echo campo_texto( 'fsfprogramado', 'N', $habilitado, '', 12, 20, '##############', '', '', '','', "id='fsfprogramado'" );
										?> 
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$fsfexecutado = $obFisicoFinanceiro->fsfexecutado; 
											echo campo_texto( 'fsfexecutado', 'N', $habilitado, '', 12, 20, '##############', '', '', '','', "id='fsfexecutado'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$fsfvlrconcedente = ($obFisicoFinanceiro->fsfvlrconcedente) ? number_format($obFisicoFinanceiro->fsfvlrconcedente,2,",",".") : "" ;		 
											echo campo_texto( 'fsfvlrconcedente', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='fsfvlrconcedente'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$fsfvlrexecutor = ($obFisicoFinanceiro->fsfvlrexecutor) ? number_format($obFisicoFinanceiro->fsfvlrexecutor,2,",",".") : "" ; 
											echo campo_texto( 'fsfvlrexecutor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='fsfvlrexecutor'" ); 
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php
											$fsfvlrtotal = ($obFisicoFinanceiro->fsfvlrtotal) ? number_format($obFisicoFinanceiro->fsfvlrtotal,2,",",".") : "" ; 
										echo campo_texto( 'fsfvlrtotal', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='fsfvlrtotal'" );  
										?>
									</th>
									<th class="SubTituloCentro" style="background-color: #ffffff">
										<?php if($habilitado == 'S'): ?>
											<img src="/imagens/gif_inclui.gif" id="salvar" style="cursor: pointer;" border=0 title="Salvar" />
										<?php else:?>
											<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
										<?php endif;?>
										&nbsp; 
										<a href="javascript: void(0);" onclick="limparCampos();"> <img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
									</th>
								</tr>
							</thead>
							<tbody>
								<?php 
									$sql = "select fsfid, fsfdsc, fsfprogramado, fsfexecutado, fsfvlrconcedente, fsfvlrexecutor, fsfvlrtotal from academico.fisicofinanceiro where prcid = {$_SESSION['academico']['prcid']}";
						            $arDados = $db->carregar( $sql );
						            $arDados = ($arDados) ? $arDados : array();
								?>
								<?php if($arDados): ?>
									<?php foreach ($arDados as $dados): ?>
									<tr class="trFisicoFinanceiro">
										<td style="background-color: #ffffff"><?php echo $dados['fsfdsc']; ?></td>
										<td style="background-color: #ffffff"><?php echo $dados['fsfprogramado']; ?></td>
										<td style="background-color: #ffffff"><?php echo $dados['fsfexecutado']; ?></td>
										<td style="background-color: #ffffff; text-align: right"><?php echo number_format($dados['fsfvlrconcedente'],2,",","."); ?></td>
										<td style="background-color: #ffffff; text-align: right"><?php echo number_format($dados['fsfvlrexecutor'],2,",","."); ?></td>
										<td style="background-color: #ffffff; text-align: right"><?php echo number_format($dados['fsfvlrtotal'],2,",","."); ?></td>
										<td style="background-color: #ffffff; text-align: center; width: 50px">
											<?php if($habilitado == 'S'): ?>
												<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $dados['fsfid']; ?>" class="alterar" />&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $dados['fsfid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluir" />
											<?php else:?>
												<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
											<?php endif;?>
										</td>
									</tr>
									<?php 
										$somafsfvlrconcedente = $somafsfvlrconcedente + $dados['fsfvlrconcedente'];
										$somafsfvlrexecutor = $somafsfvlrexecutor + $dados['fsfvlrexecutor'];
										$somafsfvlrtotal = $somafsfvlrtotal + $dados['fsfvlrtotal'];
									?>
									<?php endforeach;?>
									<tr bgcolor="#ffffff" class="trFisicoFinanceiro">
								    	<td colspan="3" style="background-color: #ffffff; text-align: right"><strong>TOTAL:</strong></td>
								    	<td style="background-color: #ffffff; text-align: right"><strong><?php echo number_format($somafsfvlrconcedente,2,",","."); ?></strong></td>
								    	<td style="background-color: #ffffff; text-align: right"><strong><?php echo number_format($somafsfvlrexecutor,2,",","."); ?></strong></td>
								    	<td style="background-color: #ffffff; text-align: right"><strong><?php echo number_format($somafsfvlrtotal,2,",","."); ?></strong></td>
								    	<td style="background-color: #ffffff;">&nbsp;</td>
								    </tr>
								<?php else: ?>
									<tr bgcolor="#ffffff">
								    	<td colspan="7"><strong><center>Não foram encontrados registros</center></strong></td>
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
	</tbody>
</table>
<style type="text/css">
div.scrollCreditoOrc {
	/*width: 100%;
	height: 500px;
	*/
}
</style>
<div id="divDebug"></div>