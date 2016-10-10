<?php

if($_REQUEST['requisicao']=='gerarxls'){

	ob_clean();

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/SIMEC_PMMB_REMESSA_CREDITO_".date("YmdHis").".xls;");
	header ( "Content-Disposition: attachment; filename=SIMEC_PMMB_REMESSA_CREDITO_".date("YmdHis").".xls");
	header ( "Content-Description: MID Gera excel" );

	echo '<center>'.str_replace('<table>','<table border="1">',$_REQUEST['content']).'</center>';
	die();
}

if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

$arrPerfil = pegaPerfilGeral();

if(in_array(PERFIL_CONSULTA,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Remessa de Crédito", "Selecione os tutores / supervisores abaixo para realizar a remessa de crédito.");

$sql = "select
					*,
					case when apgjustificativa ilike '%complementar%' then 'Sim' else 'Não' end as complementar
				from
					maismedicos.autorizacaopagamento apg				
				left join
					maismedicos.tutor tut ON apg.apgcpf = tut.tutcpf and apg.apgtipo = tut.tuttipo
						and tut.tutcpf is not null
						and tut.tutcpf in (select det.nu_cpf from maismedicos.remessadetalhe det where det.strid = ".STRID_REGISTRO_OK." AND det.cs_tipo_lote = '".TPLID_CADASTRO."')
						and tutid not in (2362,2372,2376,2339,2329,2377,2367,2377,2367,2342,2331,2329,2330,2357,2339,2372,2376,2325,2362,2375,2374)
				left join
					maismedicos.universidade uni ON uni.uniid = tut.uniid
						and uni.unistatus = 'A'
				where
					apg.rmdid is null
				and
					apg.apgenviado != true
				and
					apg.apgstatus = 'A'
				order by
					tutnome, apgano, apgmes";

// ver($sql, d);
$arrTutores = $db->carregar($sql);
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script>
$(function(){
	$('.mostrar').click(function(){
		$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
		$('.tr_bolsistas').hide();
		$('.'+this.id).show();
	});
});
</script>
<style>
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.center{text-align:center}
	.right{text-align:right;}
</style>

<form name="formulario" id="formulario" method="post" action="" >
	
	<input type="hidden" id="requisicao" name="requisicao" value="" />
	<input type="hidden" id="classe" name="classe" value="RemessaCabecalho" />
	<input type="hidden" id="apgmes" name="apgmes" value="<?php echo $arrTutores[0]['apgmes'] ?>" />
	<input type="hidden" id="apgano" name="apgano" value="<?php echo $arrTutores[0]['apgano'] ?>" />
	
	<p>
		<center>
			<a class="mostrar" href="javascript:void(0)" id="bolsista_erro">Mostrar bolsistas com erro</a>&nbsp;|&nbsp;
			<a class="mostrar" href="javascript:void(0)" id="lote_complementar">Mostrar lote complementar</a>&nbsp;|&nbsp; 
			<a class="mostrar" href="javascript:void(0)" id="tr_bolsistas">Mostrar tudo</a>
		</center> 
	</p>
	
	<table id="gridRemessaCredito" class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	
		<?php if($arrTutores): ?>
		
			<tr>
				<td colspan="4" class="center bold" >A remessa de crédito será gerada para o mês de <?php echo retornaMesMaisMedicos($arrTutores[0]['apgmes']) ?> de <?php echo $arrTutores[0]['apgano'] ?>.</td>
			</tr>
			<tr bgcolor="#c9c9c9"  >
				<td class="bold center" >Selecionar <br/> 
				<?php if($habilitado == "S"): ?>
					<input type="checkbox" name="tutidtodos" value="" checked="checked" onclick="selecionarTodos(this)" />
				<?php endif; ?>
				</td>
				<td class="bold center" >Nome</td>
				<td class="bold center" >CPF</td>
				<td class="bold center" >Instituição</td>
				<td class="bold center" >Mês</td>
				<td class="bold center" >Valor (R$)</td>
				<td class="bold center" >Complementar</td>
			</tr>
			
			<?php foreach($arrTutores as $n => $t): ?>
			
				<?php 
					unset($erro);
					if(empty($t['tutcpf'])){
						$sql = "select det.nu_cpf from maismedicos.remessadetalhe det where nu_cpf = '{$t['apgcpf']}' and det.strid = ".STRID_REGISTRO_OK." AND det.cs_tipo_lote = '".TPLID_CADASTRO."'";						
						if(!$db->pegaUm($sql)){
							$erro[] = 'sem dados cadastrais';
						}else{
							$erro[] = 'bolsista não encontrado';
						}
					}else{
						if(empty($t['tutdatanascimento'])){
							$erro[] = 'sem data de nascimento'; 
						}
						if(empty($t['tutnomemae'])){
							$erro[] = 'sem nome da Mâe'; 
						}
						if(empty($t['tutagencia'])){
							$erro[] = 'sem número da agência'; 
						}						
						if(empty($t['uninome'])){
							$erro[] = 'sem universidade';
						}
					}
					
					$class = ' tr_bolsistas ';
					if(!empty($erro))
						$class .= " bolsista_erro "; 
					
					if($t['lcbid'] > 0)
						$class .= " lote_complementar ";
					
					
					$total_lote_complementar 	+= $t['lcbid'] > 0 ? 1 : 0;
					 
					$total_supervisores_erro 	+= !empty($erro) && $t['apgtipo'] == "S" ? 1 : 0;
					$total_tutores_erro 		+= !empty($erro) && $t['apgtipo'] == "T" ? 1 : 0;
					
					$total_tutores 				+= $t['apgtipo'] == "T" ? 1 : 0;
					$total_supervisores 		+= $t['apgtipo'] == "S" ? 1 : 0;

					if(empty($erro))
						$valor_total_pagamento	+=  $t['apgtipo'] == "T" ? "5000" : "4000";
				?>
			
				<tr class="<?php echo $class; ?>" bgcolor="<?php echo $n%2 ? "#FFFFFF" : "" ?>" <?php echo !empty($erro) ? 'style="color:red"' : ($t['complementar']=='Sim' ? 'style="color:blue;"' : ""); ?> >
					<td class="center" >
					<?php if(!empty($erro)): ?>
						<?php $label = "".ucfirst(implode(', ', $erro)); ?>
						<img src="../imagens/ajuda.gif" title="<?php echo $label; ?>" alt="<?php echo $label; ?>" />
					<?php else: ?>
						<?php if($habilitado == "S"): ?>
							<input type="checkbox" name="tutid[]" checked="checked" value="<?php echo $t['tutid']; ?>" />
						<?php endif; ?>
					<?php endif; ?>
					</td>
					<td><?php echo $t['tutnome'] ? $t['tutnome'] : $t['apgnome']; ?></td>
					<td><?php echo mascara_global_maismedicos_tela($t['tutcpf'] ? $t['tutcpf'] : $t['apgcpf'], "###.###.###-##") ?></td>
					<td><?php echo $t['uninome']; ?></td>
					<td><?php echo $t['apgmes'].'/'.$t['apgano']; ?></td>
					<td class="right" >
						<?php 
						$valor = $t['apgtipo'] == "T" ? "5000" : "4000";
						echo number_format($valor,2,',','.'); 
						?>
					</td>
					<td align="center"><?php echo $t['complementar']; ?></td>
				</tr>
				
			<?php endforeach; ?>
			
			<tr bgcolor="#c9c9c9" >
				<td class="bold" colspan="6" >
				
					 <span style="margin-left:0px">Total de Tutores:</span>&nbsp;
					 	<?php echo number_format($total_tutores,0,',','.'); ?> 
					 
					 <span style="margin-left:50px">Total de Supervisores:</span>&nbsp; 
					 	<?php echo number_format($total_supervisores,0,',','.'); ?>
					  
					 <span style="margin-left:50px">Total de Bolsistas:</span>&nbsp;
					 	<?php echo number_format($total_tutores+$total_supervisores,0,',','.'); ?>
					 
					 <span style="cursor:pointer;margin-left:50px;color:red">
					 	Tutores com ERRO:&nbsp;<?php echo number_format($total_tutores_erro,0,',','.'); ?></span> 
					 	
					 <span style="cursor:pointer;margin-left:50px;color:red">
					 	Supervisores com ERRO:&nbsp;<?php echo number_format($total_supervisores_erro,0,',','.'); ?></span> 
					 	  
					 <span style="margin-left:50px;color:blue">
					 	Total de Bolsistas a serem pagos:&nbsp;<?php echo number_format(($total_tutores-$total_tutores_erro)+($total_supervisores-$total_supervisores_erro),0,',','.'); ?></span>
					 	  
					 <span style="margin-left:50px;color:blue">
					 	Lote Complementar:&nbsp;<?php echo number_format($total_lote_complementar,0,',','.'); ?></span>
				</td>
				<td class="bold right" >
					 <?php echo number_format($valor_total_pagamento,2,',','.'); ?>
				</td>
			</tr>
			
			<?php if($habilitado == "S"): ?>
			
				<tr>
					<td colspan="7" align="center" >
						<input type="button" value="Gerar Remessa de Crédito de <?php echo retornaMesMaisMedicos($arrTutores[0]['apgmes']) ?> de <?php echo $arrTutores[0]['apgano'] ?>" name="btn_cadastro" onclick="processarRemessaCredito()" /> 
					</td>
				</tr>
				
			<?php endif; ?>
			
		<?php else: ?>
		
			<tr>
				<td colspan="2" align="center" >Não existem tutores / supervisores disponíveis para remessa de crédito.</td>
			</tr>
			
		<?php endif; ?>
		
	</table>
	
</form>

<?php if($arrTutores): ?>

	<center>
	
		<p>&nbsp;</p>
		<div id="gridRemessa">
		<font size="3"><b><?php echo CODIGO_ORGAO_SISTEMA. ' - '. NOME_ORGAO_SISTEMA. ' - '. SIGLA_ORGAO_SISTEMA; ?></b></font><br/><br/>
		<font size="2"><b>26443 - EMPRESA BRASILEIRA DE SERVIÇOS HOSPITALARES - EBSERH</b></font><br/><br/>
		<b>Sistema WebPortfólio UNASUS<br/>
		Ação 20 GK - MAIS MÉDICOS PARA O BRASIL<br/>
		DEMONSTRATIVO DE PAGAMENTO DE BOLSAS<br/>
		MÊS DE REFERÊNCIA: OUTUBRO DE 2014<br/><br/></b>
		<table>
			<thead>
				<tr>
					<th>Nº</th>
					<th>ITEM/DESCRIÇÃO</th>
					<th>UNIDADE DE MEDIDA</th>
					<th>QUANTIDADE</th>
					<th>VALOR UNITÁRIO</th>
					<th>VALOR TOTAL</th>
				</tr>
			</thead>			
			<tbody>
				<tr>
					<td>1</td>
					<td>Bolsa Tutoria</td>
					<td>Bolsa</td>
					<td><?php echo number_format($total_tutores-$total_tutores_erro,0,',','.'); ?></td>
					<td>5.000,00</td>
					<td><?php echo number_format(($total_tutores-$total_tutores_erro)*5000,2,',','.'); ?></td>
				</tr>
				<tr>
					<td>2</td>
					<td>Bolsa Supervisão</td>
					<td>Bolsa</td>
					<td><?php echo number_format($total_supervisores-$total_supervisores_erro,0,',','.'); ?></td>
					<td>4.000,00</td>
					<td><?php echo number_format(($total_supervisores-$total_supervisores_erro)*4000,2,',','.'); ?></td>
				</tr>
			</tbody>
			
			<tr>
				<td colspan="3" align="center">Total</td>
				<td><?php echo number_format(($total_tutores-$total_tutores_erro)+($total_supervisores-$total_supervisores_erro),0,',','.'); ?></td>
				<td></td>
				<td><?php echo number_format($valor_total_pagamento,2,',','.'); ?></td>
			</tr>
			
		</table>
		<br/>
		<p><b>Responsabilidade técnica pela validação das informações:  Diretoria <br/>de Desenvolvimento da Educação em Saúde - DDES/SESU/MEC.</b></p>
		
		<p> <input type="button" value="Gerar Excel" onclick="gerarExcelRemessaGrid()" /> </p>
		</div>
		
	</center>
	
<?php endif; ?>

<script>

	function processarRemessaCredito()
	{
		var num = $( "[name='tutid[]']:checked" ).length;
		if(num > 0){
			$("#classe").val("RemessaCabecalho");
			$("#requisicao").val("processarRemessaCredito");
			$("#formulario").submit();
		}else{
			alert('Selecione pelo menos 1 tutor para processar a remessa de crédito.');
		}
	}
	
	function selecionarTodos(obj)
	{
		if( $(obj).is(':checked') ){
			$("[name='tutid[]']").prop('checked',true);
		}else{
			$("[name='tutid[]']").prop('checked',false);
		}
	}
	
	function gerarExcelRemessaGrid()
	{
		var grid = document.getElementById('gridRemessa').innerHTML;
		document.location.href = '/maismedicos/maismedicos.php?modulo=principal/remessaCredito&acao=A&requisicao=gerarxls&content='+grid;
	}
	
	$(function() {
		<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
			alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
			<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
		<?php endif; ?>
	});

</script>