<?php

if($_REQUEST['requisicao']=='evidencia'){
	$sql = "select apgevidencia, apgjustificativa, apgcpfresponsavel, apgnomeresponsavel, apgdtregistro
	from maismedicos.pagamento_autorizacao where apgid = {$_REQUEST['apgid']}";
	$rs = $db->pegaLinha($sql);
	if($rs){
		echo "<script>
					$(function(){
						$('.btnFechar').click(function(){
							$( '#modalDialog' ).dialog('close')
						});
			});
				</script>
				";
		echo '<center><h3>Informações da Validação</h3><p>&nbsp;</p></center>';
		echo '<table class="tabela" width="95%" bgcolor="#f5f5f5" align="center"><tr><td>';
		echo '<p><b><font size="+1">Justificativa:</font></b></p><p>'.$rs['apgjustificativa'].'</p>';		
		echo '<p><b><font size="-1">Responsável: </font></b>'.formatar_cpf($rs['apgcpfresponsavel']).' - '.$rs['apgnomeresponsavel'].'</p>';		
		echo '<p><b><font size="-1">Data da validação: </font></b>'.formata_data($rs['apgdtregistro']).'</p>';		
		echo '<p>&nbsp;</p><p><b><font size="+1">Evidência:</font></b></p>'.$rs['apgevidencia'];
		echo '</td></tr></table>';
		echo '<center><p>&nbsp;</p><p><input type="button" class="btnFechar" value="Fechar" /></p></center>';
	}
	die;
}
include_once APPRAIZ.'maismedicos/classes/Pagamento_RemessaCabecalho.class.inc';
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

$arrPerfil = pegaPerfilGeral();

$habilitado = "S";

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Remessa de Crédito", "Selecione os beneficiários abaixo para realizar a remessa de crédito.");

$sql = "select
					*,
					funnome as funcao,
		ben.funid as idfuncao,
					case when apgjustificativa ilike '%complementar%' then 'Sim' else 'Não' end as complementar
				from
					maismedicos.pagamento_autorizacao apg				
				left join
					maismedicos.pagamento_beneficiario ben ON apg.benid = ben.benid	
				left join 
					maismedicos.pagamento_beneficiariofuncao fun on fun.funid = ben.funid					
				left join
					maismedicos.universidade uni ON uni.uniid = ben.uniid
						and uni.unistatus = 'A'
				left join 
					maismedicos.ifes ife on ife.ifeid = ben.ifeid
				left join 
					maismedicos.ifes_campi ifc on ifc.ifcid = ben.ifcid
				where
					apg.rmdid is null
				and
					apg.apgcpfresponsavel is not null
				and
					apg.apgenviado != true
				and
					apg.apgstatus = 'A'
				order by
					bennome, apgano, apgmes";

// ver($sql, d);
$arrBeneficiarios = $db->carregar($sql);
?>

<!-- <script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script> -->
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script>
$(function(){

	$("#modalDialog").dialog({
		height: $(document).height(),
        width: $(document).width(),
	      autoOpen: false,
	      show: {
	        effect: "blind",
	        duration: 1000
	      },
	      hide: {
	        effect: "explode",
	        duration: 1000
	      }
	});

	$('.btnFecharModal').click(function(){
		$( '#modalDialog' ).dialog('close');	
	});

	$('.btnInfo').click(function(){
		
		$.ajax({
			url	: document.location.href,
			type	: 'post',
			data	: '&requisicao=evidencia&apgid='+this.id,
			success	: function(e){
				$( '#modalDialog' ).html(e);
				$( '#modalDialog' ).dialog('open');
				}
			});
		
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
	<input type="hidden" id="apgmes" name="apgmes" value="<?php echo $arrBeneficiarios[0]['apgmes'] ?>" />
	<input type="hidden" id="apgano" name="apgano" value="<?php echo $arrBeneficiarios[0]['apgano'] ?>" />
	
	<table id="gridRemessaCredito" class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	
		<?php if($arrBeneficiarios): ?>
		
			<tr bgcolor="#c9c9c9"  >
				<td class="bold center" >Selecionar <br/> 
				<?php if($habilitado == "S"): ?>
					<input type="checkbox" name="benidtodos" value="" checked="checked" onclick="selecionarTodos(this)" />
				<?php endif; ?>
				</td>
				<td class="bold center" >Mês</td>
				<td class="bold center" >Nome</td>
				<td class="bold center" >CPF</td>
				<td class="bold center" >Função</td>
				<td class="bold center" >Instituição</td>
				<td class="bold center" >Valor (R$)</td>
				<td class="bold center" >Complementar</td>
				<td class="bold center" >Info</td>
			</tr>
			
			<?php foreach($arrBeneficiarios as $n => $t): ?>
			
				<?php 
					unset($erro);
					if(empty($t['bencpf'])){
						$erro[] = 'beneficiário não encontrado';
					}else{
						if(empty($t['bendatanascimento'])){
							$erro[] = 'sem data de nascimento'; 
						}
						if(empty($t['bennomemae'])){
							$erro[] = 'sem nome da Mâe'; 
						}
						if(empty($t['benagencia'])){
							$erro[] = 'sem número da agência'; 
						}						
						//if(empty($t['ifenome'])){
							//$erro[] = 'sem IFES';
						//}
					}
					
					$class = ' tr_bolsistas ';
					if(!empty($erro))
						$class .= " bolsista_erro "; 
					
					if($t['lcbid'] > 0)
						$class .= " lote_complementar ";
					
					$total_lote_complementar 	+= $t['lcbid'] > 0 ? 1 : 0;

					//if(empty($erro)){
						if($t['idfuncao']==FUNCAO_AVALIADOR_CAMEM){
							$valor_total_pagamento += 4000;
						}else
							if($t['idfuncao']==FUNCAO_COORDENADOR_CAMEM){
							$valor_total_pagamento += 5000;
						}
					//}
				?>
			
				<tr class="<?php echo $class; ?>" bgcolor="<?php echo $n%2 ? "#FFFFFF" : "" ?>" <?php echo !empty($erro) ? 'style="color:red"' : ($t['complementar']=='Sim' ? 'style="color:blue;"' : ""); ?> >
					<td class="center" >
					<?php if(!empty($erro)): ?>
						<?php $label = "".ucfirst(implode(', ', $erro)); ?>
						<img src="../imagens/ajuda.gif" title="<?php echo $label; ?>" alt="<?php echo $label; ?>" />
					<?php else: ?>
						<?php if($habilitado == "S"): ?>
							<input type="checkbox" name="benid[]" checked="checked" value="<?php echo $t['benid']; ?>" />
						<?php endif; ?>
					<?php endif; ?>
					</td>
					<td><?php echo $t['apgmes'].'/'.$t['apgano']; ?></td>
					<td><?php echo $t['bennome'] ? $t['bennome'] : $t['apgnome']; ?></td>
					<td><?php echo mascara_global_maismedicos_tela($t['bencpf'] ? $t['bencpf'] : $t['apgcpf'], "###.###.###-##") ?></td>
					<td><?php echo $t['funid'] ?></td>
					<td><?php echo $t['uninome']; ?></td>
					<td class="right" >
						<?php 
						$valor = 0;
						if($t['idfuncao']==FUNCAO_AVALIADOR_CAMEM){
							$valor = 4000;
						}else
						if($t['idfuncao']==FUNCAO_COORDENADOR_CAMEM){
							$valor = 5000;
						}
						echo number_format($valor,2,',','.'); 
						?>
					</td>
					<td align="center"><?php echo $t['complementar']; ?></td>
					<td align="center">
						<img src="../imagens/consultar.gif" class="btnInfo" id="<?php echo $t['apgid']?>" style="cursor:pointer" />
					</td>
				</tr>
				
			<?php endforeach; ?>
			
			
			<?php if($habilitado == "S"): ?>
			
				<tr>
					<td colspan="9" align="center" >
						<input type="button" value="Gerar Remessa de Crédito" name="btn_cadastro" onclick="processarRemessaCredito()" /> 
					</td>
				</tr>
				
			<?php endif; ?>
			<tr>
			<td colspan="9">Valor total da remessa: <?php echo number_format($valor_total_pagamento,2,',','.');?></td>
			</tr>
			
		<?php else: ?>
		
			<tr>
				<td colspan="2" align="center" >Não existem beneficiários disponíveis para remessa de crédito.</td>
			</tr>
			
		<?php endif; ?>
		
	</table>
	
</form>
<div style="padding:5px;text-align:justify;display:none;" id="modalDialog" title="Detalhe da Autorização de Pagamento">
	
</div>
<script>

	function processarRemessaCredito()
	{
		var num = $( "[name='benid[]']:checked" ).length;
		if(num > 0){
			$("#classe").val("Pagamento_RemessaCabecalho");
			$("#requisicao").val("processarRemessaCredito");
			$("#formulario").submit();
		}else{
			alert('Selecione pelo menos 1 beneficiário para processar a remessa de crédito.');
		}
	}
	
	function selecionarTodos(obj)
	{
		if( $(obj).is(':checked') ){
			$("[name='benid[]']").prop('checked',true);
		}else{
			$("[name='benid[]']").prop('checked',false);
		}
	}
	
	function gerarExcelRemessaGrid()
	{
		var grid = document.getElementById('gridRemessa').innerHTML;
		document.location.href = '/maismedicos/maismedicos.php?modulo=principal/pagamento/geraRemessaCredito&acao=A&requisicao=gerarxls&content='+grid;
	}
	
	$(function() {
		<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
			alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
			<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
		<?php endif; ?>

	});

</script>