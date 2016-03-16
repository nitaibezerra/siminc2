<?php
if($_REQUEST['requisicao']=='salvarCustoAtividade'){
	salvarCustoAtividade($_REQUEST);
	exit();
} 

if($_REQUEST['requisicao']=='alterarCustoAtividade'){
	alterarCustoAtividade($_REQUEST);
	exit();
} 

if($_REQUEST['requisicao']=='excluirCustoAtividade'){
	excluirCustoAtividade($_REQUEST['orcid']);
	exit();
} 

if($_REQUEST['requisicao']=='visualizarCustoAtividade'){
	header('Content-Type: text/html; charset=iso-8859-1');
	visualizarCustoAtividade($_REQUEST['orcid'],'json');
	exit();
}

if($_REQUEST['requisicao']=='atualizarSaldoPlanejamento2'){
	header('Content-Type: text/html; charset=iso-8859-1');
	atualizarSaldoPlanejamento2($_REQUEST);
	exit();	
}

if($_REQUEST['orcid']){ 
	$habilitado = "N";
	$custo = visualizarCustoAtividade($_REQUEST['orcid'],'html');
	extract($custo);
} else {
	$habilitado = "S";
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}

$planejamento = carregarDadosPlanejamento();
extract($planejamento);

$saldo = $saldodisp - $saldoacoes; 
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	function salvarCustoAtividade() {
		if(jQuery('#gdeid').val()=='') {
			alert('O campo "Grupo de Despesa" é obrigatório!');
			jQuery('#gdeid').focus();
			return false;
		}
		jQuery('#orcvlrunitario').val(mascaraglobal('###.###.###,##',jQuery('#orcvlrunitario').val()));
		if(jQuery('#orcvlrunitario').val()=='') {
			alert('O campo "Valor Unitário (R$)" é obrigatório!');
			jQuery('#orcvlrunitario').focus();
			return false;
		}
		
		if(jQuery('#orcdescricao').val()=='') {
			alert('O campo "Detalhamento" é obrigatório!');
			jQuery('#orcdescricao').focus();
			return false;
		}
		if(jQuery('#orctipo').val() == ''){
			alert('O campo "Fonte de Recursos" é obrigatório!');
			jQuery('#orctipo').focus();
			return false;
		}
		jQuery('#formulario').submit();
	}

	function visualizarCustoAtividade(orcid){
		jQuery.ajax({
			type: 'POST',
			url: 'sisfor.php?modulo=principal/coordenador/inserircustosatividade&acao=A',
			data: { requisicao: 'visualizarCustoAtividade', orcid: orcid},
			dataType: 'json',
			async: false,
			success: function(data){
				if(data){
					jQuery('#orcid').val(data.orcid);
					jQuery('#gdeid').val(data.gdeid);
					jQuery('#orcvlrunitario').val(trim(data.orcvlrunitario));
					jQuery('#orcdescricao').val(data.orcdescricao);
					jQuery('#orctipo').val(data.orctipo);
					jQuery('#btnSalvar').hide();
					jQuery('#btnAlterar').show();
					jQuery('#requisicao').val('alterarCustoAtividade');
				} else {
					jQuery('#orcid').val('');
					jQuery('#gdeid').val('');
					jQuery('#orcvlrunitario').val('');
					jQuery('#orcdescricao').val('');
					jQuery('#orctipo').val('');
					jQuery('#requisicao').val('salvarCustoAtividade');
					alert("Atividade Custo não encontrada!");
				}	
			}
		});
	}

	function excluirCustoAtividade(orcid){
		if(confirm("Deseja realmente excluir o Custo Atividade?")){
			divCarregando();
			jQuery.ajax({
				type: 'POST',
				url: 'sisfor.php?modulo=principal/coordenador/inserircustosatividade&acao=A',
				data: { requisicao: 'excluirCustoAtividade', orcid: orcid},
				async: false,
				success: function(data) {
					alert('Custo Atividade excluído com sucesso!');
					jQuery("#divListarCustoAtividade").html(data);
					divCarregado();
			    }
			});
		}
	}

	function atualizarSaldo(loa){
		if(jQuery('#orctipo').val() == ''){
			alert('Selecione a Fonte de Recursos!');
			jQuery('#orcvlrunitario').val('');
			jQuery('#orctipo').focus();
		} else {	
			if(jQuery('#orctipo').val() == '1'){	
				jQuery.ajax({
					url: 'sisfor.php?modulo=principal/coordenador/inserircustosatividade&acao=A',
					dataType: 'json',				
					data: { requisicao: 'atualizarSaldoPlanejamento2', loa: loa},
					async: false,
					success: function(data){
						if(trim(data.resultado) == 'N'){
							jQuery('#saldo').html('R$ ' + data.saldo);
							alert('Não há mais saldo disponível!');
							jQuery('#sifvalorloa').focus();
							jQuery('#btnSalvar').attr('disabled','disabled');
						} else {
							jQuery('#saldo').html('R$ ' + data.saldo);
							jQuery('#btnSalvar').removeAttr('disabled','disabled');
						}
				    }
				});
			}
		}
	}
</script>

<br>

<?php monta_titulo('Inserir Custo Atividade',$cies); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloCentro" width="25%">&nbsp;</td>
		<td class="subtituloEsquerda" width="25%">SEB / SECADI</td>
	</tr>
	<tr>
		<td class="subtituloDireita">Saldo Disponível:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldodisp); ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita">Recurso Gasto:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldoacoes); ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita">Saldo:</td>
		<td id="saldo" class="subtituloCentro">R$ <?php echo formata_valor($saldo); ?></td>
	</tr>			
</table>

<form method="post" id="formulario" enctype="multipart/form-data">
	<?php if($_REQUEST['orcid']){ ?>
	<input id="requisicao" type="hidden" name="requisicao" value="alterarCustoAtividade">
	<input id="sifid" type="hidden" name="sifid" value="<?php echo $sifid; ?>">
	<?php } else { ?>
	<input id="sifid" type="hidden" name="sifid" value="<?php echo $_REQUEST['sifid']; ?>">
	<input id="requisicao" type="hidden" name="requisicao" value="salvarCustoAtividade">	
	<?php } ?>
	<input id="orcid" type="hidden" name="orcid" value="<?php echo $orcid; ?>">	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Inserir Custos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">Grupo de Despesa:</td>
			<td><?php	$sql = "SELECT 		gdeid AS codigo,
				                			gdedesc AS descricao
								FROM        sispacto.grupodespesa 
				            	WHERE      	gdestatus='A'";
		
				$db->monta_combo('gdeid', $sql, 'S', 'Selecione o Grupo de Despesa', '', '', '', '400', 'S', 'gdeid', '', $gdeid); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade de Medida:</td>
			<td>Verba</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Fonte de Recursos:</td>
			<td>
	        	<?php 	
	        		$arrayRecurso = array(array('codigo' => '1', 'descricao' => 'LOA'),
										  array('codigo' => '2', 'descricao' => 'Outras Fontes')); ?>
	        	<?php $db->monta_combo('orctipo',$arrayRecurso,$habilitado,'Selecione a Fonte de Recursos','','','','400','S','orctipo'); ?>	
        	</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita">Valor Total (R$):</td>
			<td>
				<?php echo campo_texto('orcvlrunitario', 'S', 'S', "Valor Unitário (R$)", 15, 14, '[.###],##', '', '', '', 0, 'id="orcvlrunitario"', '', '','atualizarSaldo(this.value);'); ?>
				<b id="divLoa" style="display:none;">Não há saldo suficiente de recursos LOA.</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Memória de cálculo:</td>
			<td><?php echo campo_textarea('orcdescricao', 'S', 'S', '', '70', '4', '500'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<?php if(!$_REQUEST['orcid']){ ?>
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarCustoAtividade();" <?php echo $hab == 'N' ? 'disabled="disabled"' : '' ?>/>
				<?php } else { ?>
				<input type="button" value="Alterar" id="btnAlterar" onclick="salvarCustoAtividade();" <?php echo $hab == 'N' ? 'disabled="disabled"' : '' ?>/>				
				<?php } ?>
				<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
				<?php if(!$_REQUEST['orcid']){ ?>
				<input type="button" value="Novo" id="btnSalvar" onclick="location.reload();" <?php echo $hab == 'N' ? 'disabled="disabled"' : '' ?>/>
				<?php } ?>
			</td>
		</tr>
	</table>
</form>

<br>
