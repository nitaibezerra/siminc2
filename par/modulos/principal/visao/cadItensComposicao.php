<?php 
if($_POST['icodetalhe']){
	
	$obSubacaoControle = new SubacaoControle();
	
//	ver($anoref, $sbaid, $_SESSION, d);
	
	$count = count($_REQUEST['icodetalhe']);
	for($x=0;$x<$count;$x++){
		
		$icoid = explode("_",$_REQUEST['icoid_'][$x]);
		
		if($_REQUEST['icodetalhe'][$x] != ''){
			$arDados['icoid_'] = $icoid[1]; 
			$arDados['icodetalhe'] = $_REQUEST['icodetalhe'][$x];
			$obSubacaoControle->salvarSubacaoItensComposicao($arDados);
		}		
	}
	alert('Gravado com sucesso.');
	echo "<script>document.location.href = '{$_SERVER['HTTP_REFFER']}'</script>";
}
?>
<script type="text/javascript">
jQuery(document).ready(function(){
	jQuery('.add').click(function(){

		var totalItens = <?php echo count($arItensComposicao) ?>;

		var filhos = jQuery('#itensComposicao').children().length;

		if(filhos < totalItens){
		
			var $campos = jQuery('#itensComposicao'),
				$tr = $campos.find('tr:first').clone();			
				$tr.find("input").val("");
				$tr.find("select").val("");
				$tr.find("select[name=icoid_[]]").attr("id", "icoid_"+filhos);
				$tr.find("select[name=unddid_[]_disable]").attr("id", "unddid_"+filhos);				
				$campos.append($tr);			
		}else{
			alert('Máximo de itens permitido.');
		}

//		for(x=0;x<filhos;x++){
//			var valor = jQuery("#icoid_"+x).val();
//			jQuery("#icoid_"+filhos+" option[value='"+valor+"']").remove();
//		}
		
		return false;
	});
	
	jQuery('.removeItens').live('click',function(){
		var filhos = jQuery('#itensComposicao').children().length;		
		if (filhos > 1) {
			jQuery(this).parent().parent().remove();
		}
		return false;
	});

});

function mudaDado(obj){
	
	var linhas = jQuery('#itensComposicao').children().length;
	var valor1 = obj.value;
	var indice = obj.id.split("_");	
	
	valida = 0;
	for(x=0;x<linhas;x++){
		var valor2 = jQuery("#icoid_"+x).val();
		
		if(valor1 == valor2){
			valida++;
			if(valida > 1){									
				alert('Item já selecionado.');					
				jQuery("#icoid_"+indice[1]).val('');
			}
		}
	}

	separa = valor1.split("_");	
	jQuery('#unddid_'+indice[1]).val(separa[0]);	
	
	return false;
}

</script>
<?php monta_titulo( 'Cadastro de Itens Composição', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );	?>
<form action="" method="post" name="formulario" id="formulario">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<thead>			
			<tr bgcolor="#e9e9e9" align="center">
				<td align="center">Ação</td>
				<td align="left">Terreno</td>
				<td align="left">Descrição</td>
				<td align="left">Unidade de Medida</td>
			</tr>
		</thead>
		<tbody id="itensComposicao">
			<?php $indice = 0 ?>
			<?php foreach($arItensComposicao as $item): ?>
				<?php if(!empty($item['icodetalhe'])): ?>
					<tr>
						<td align="center">					
							<img src="../imagens/excluir.gif" />
						</td>						
						<td align="left">					
							<?php //echo $db->monta_combo('icoid_[]', $arItensComposicao, 'S', 'Selecione...', 'mudaDado', '', '', '', '', 'icoid_0') ?>
							<select class="CampoEstilo" name="icoid_[]" id="icoid_<?php echo $indice ?>" onchange="mudaDado(this)" >
								<option value="">Selecione</option>
								<?php									
								foreach ($arItensComposicao as $dados) {																		
									echo "<option value='" . $dados['codigo'] . "' ".(($dados['icoid'] == $item['icoid']) ? 'selected="selected"' : "" ).">" . $dados['descricao'] . "</option>";
									
								}
								?>
							</select>
						</td>
						<td align="left">
							<?php echo campo_texto( 'icodetalhe[]', 'N', 'S', '', 35, 40, '', '', '', '','', '', '', $item['icodetalhe']); ?>
						</td>
						<td align="left" valign="top">
							<?php echo $db->monta_combo('unddid_[]', $arUnidadeMedida, 'N', '', '', '', '', '', '', 'unddid_'.$indice, '', $item['unddid']) ?>											
						</td>
					</tr>
					<?php $indice++ ?>					
				<?php endif; ?>
			<?php endforeach; ?>			
			<?php if($indice == 0): ?>
				<tr>
					<td align="center">					
						<img src="../imagens/excluir.gif" />
					</td>
					<td align="left">					
						<?php //echo $db->monta_combo('icoid_[]', $arItensComposicao, 'S', 'Selecione...', 'mudaDado', '', '', '', '', 'icoid_0') ?>
						<select class="CampoEstilo" name="icoid_[]" id="icoid_0" onchange="mudaDado(this)" >
							<option value="">Selecione</option>
							<?php 
							foreach ($arItensComposicao as $dados) {									
								echo "<option value='" . $dados['codigo'] . "'>" . $dados['descricao'] . "</option>";
								
							}
							?>
						</select>
					</td>
					<td align="left">
						<?php echo campo_texto( 'icodetalhe[]', 'N', 'S', '', 35, 40, '', '', '', '','', '', '', $item['icodetalhe']); ?>
					</td>
					<td align="left" valign="top">
						<?php echo $db->monta_combo('unddid_[]', $arUnidadeMedida, 'N', '', '', '', '', '', '', 'unddid_0') ?>											
					</td>					
				</tr>
			<?php endif; ?>			
		</tbody>
		<tfoot>
			<tr>
				<td colspan="4" bgcolor="#e9e9e9">
					<span class="add" style="padding-left:15px; cursor:pointer">
						<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" /> Inserir itens de composição
					</span>
				</td>
			</tr>
			<tr>
				<td colspan="4" class="SubTituloEsquerda">
					<input type="submit" value="Salvar">
				</td>
			</tr>
		</tfoot>
	</table>
</form>