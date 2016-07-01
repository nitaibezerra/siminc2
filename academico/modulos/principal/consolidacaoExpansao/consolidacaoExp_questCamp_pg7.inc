<input type="hidden" name="req" value=""/>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
	<tr>
		<td colspan="5" style="font-size:12px;" >
			<?php 
			$param['link1'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=5";
			$param['link2'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=7";
			$param['link2tipo'] = 4;
			montarAbasConsolidacao( $param );
			?>
		</td>
	</tr>
	<tr>
		<td style="font-size:12px;" >
<!--			<b>4. Demanda</b>-->
		</td>
	</tr>
	<?php 
	//Inicio Lista Questões
	$sql = "SELECT 
				perid, 
				pernumero, 
				pertitulo
			FROM 
				academico.pergunta 
			WHERE
				pernumero ilike '4.3.%'";
	
	$item = $db->pegaLinha($sql);
	?>
	<tr>
		<td style="font-size:12px;" >
			<input type="hidden" name="perid" value="<?=$item['perid'] ?>"/>
			<input type="hidden" name="tipoSalvar" value="tabela"/>
			<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' style="width:100%;" >
				<tr>
					<td bgcolor="#DCDCDC" rowspan="3" align="center"><b>Demanda</b></td>
					<td bgcolor="#DCDCDC" rowspan="3" align="center"><b>Área[m²]</b></td>
					<td bgcolor="#DCDCDC" rowspan="3" align="center"><b>Alunos<br>Beneficiados</b></td>
					<td bgcolor="#DCDCDC" colspan="7" align="center"><b>Valor Previsto</b></td>
					<td bgcolor="#DCDCDC" colspan="2" align="center"><b>Possui<br>Projeto<br>Executivo/<br>Especificações<br>Técnicas*</b></td>
				</tr>
				<tr>
					<td bgcolor="#DCDCDC" colspan="2" align="center"><b>2013</b></td>
					<td bgcolor="#DCDCDC" colspan="2" align="center"><b>2014</b></td>
					<td bgcolor="#DCDCDC" colspan="2" align="center"><b>2015</b></td>
					<td bgcolor="#DCDCDC" rowspan="2" align="center"><b>Total</b></td>
					<td bgcolor="#DCDCDC" rowspan="2" align="center"><b>Sim</b></td>
					<td bgcolor="#DCDCDC" rowspan="2" align="center"><b>Não</b></td>
				</tr>
				<tr>
					<td bgcolor="#DCDCDC" align="center"><b>Custeio</b></td>
					<td bgcolor="#DCDCDC" align="center"><b>Capital</b></td>
					<td bgcolor="#DCDCDC" align="center"><b>Custeio</b></td>
					<td bgcolor="#DCDCDC" align="center"><b>Capital</b></td>
					<td bgcolor="#DCDCDC" align="center"><b>Custeio</b></td>
					<td bgcolor="#DCDCDC" align="center"><b>Capital</b></td>
				</tr>
				<?php 
				$sql = "SELECT
							tbtid,
							tbttipo
						FROM
							academico.tabelatipo
						WHERE
							tbtid < 6";
				$tipos = $db->carregar($sql);
				foreach($tipos as $tipo){
					$sql = "";
				?>
				<tr>
					<td colspan="12" bgcolor="white">
						<b><?=$tipo['tbttipo'] ?></b>
						<?php if( $bloq == 'S' ){ ?> 
						<a class="novaLinha" name="<?=$tipo['tbtid'] ?>" style="cursor:pointer;" title="Inserir Linha." >
							<img border="0" src="../imagens/incluir_p.gif" width="12px" class="novaLinha" name="<?=$tipo['tbtid'] ?>"> Nova Linha
						</a>
						<?php }?>
					</td>
				</tr>
				<?php 
				
				$sql = "SELECT
							tbrid as linha,
							tbrdemanda,
							tbrarea,
							tbraluno,
							tbrprojeto
						FROM
							academico.tabelaresposta
						WHERE
							perid = ".$item['perid']."
							AND entid = ".$_SESSION['academico']['entidcampus']."
							AND tbtid = ".$tipo['tbtid']."
						ORDER BY
							1";
				$linhas = $db->carregar($sql);
				$linhas = $linhas ? $linhas : Array();
				foreach( $linhas as $linha ){
					$blur1 = $blur1 == '' ? $tipo['tbtid'] : $blur1;
					$blur2 = $blur2 == '' ? $linha['linha'] : $blur2;
					$sql = "SELECT tbvcusteio, tbvcapital FROM academico.tabelavalor WHERE tbvano = 2013 AND tbrid = ".$linha['linha'];
					$tbv2013 = $db->pegaLinha($sql);
					$sql = "SELECT tbvcusteio, tbvcapital FROM academico.tabelavalor WHERE tbvano = 2014 AND tbrid = ".$linha['linha'];
					$tbv2014 = $db->pegaLinha($sql);
					$sql = "SELECT tbvcusteio, tbvcapital FROM academico.tabelavalor WHERE tbvano = 2015 AND tbrid = ".$linha['linha'];
					$tbv2015 = $db->pegaLinha($sql);
				?>
				<tr><td><input type="hidden" name="linha[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" value="<?=$linha['linha']?>"/>
					<input type="hidden" name="tbrid2[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" value="<?=$linha['linha']?>"/>
					<?php if( $bloq == 'S' ){ ?>
					<img border="0" src="../imagens/excluir_2.gif" width="12px" class="excluirlinha" title="Excluir Linha." id="<?=$linha['linha']?>" style="cursor:pointer;"> 
					<?php }?>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$linha['tbrdemanda'] ?>" maxlength="100" 
					size="21" name="tbrdemanda[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onmouseover="MouseOver(this);" 
					value="<?=str_replace('.',',',$linha['tbrarea']) ?>" maxlength="100" 
					size="21" name="tbrarea[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onkeyup="this.value=mascaraglobal('[#]',this.value);" onmouseover="MouseOver(this);" 
					value="<?=$linha['tbraluno'] ?>" maxlength="100" 
					size="21" name="tbraluno[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);"
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);"
					onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="<?=str_replace('.',',',$tbv2013['tbvcusteio']) ?>" maxlength="13" 
					size="11" name="tbvcusteio[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>][2013]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
					onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="<?=str_replace('.',',',$tbv2013['tbvcapital']) ?>" maxlength="13" 
					size="11" name="tbvcapital[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>][2013]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
					onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="<?=str_replace('.',',',$tbv2014['tbvcusteio']) ?>" maxlength="13" 
					size="11" name="tbvcusteio[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>][2014]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
					onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="<?=str_replace('.',',',$tbv2014['tbvcapital']) ?>" maxlength="13" 
					size="11" name="tbvcapital[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>][2014]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
					onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="<?=str_replace('.',',',$tbv2015['tbvcusteio']) ?>" maxlength="13" 
					size="11" name="tbvcusteio[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>][2015]" style="text-align:left;">
				</td><td>
					<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
					onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="<?=str_replace('.',',',$tbv2015['tbvcapital']) ?>" maxlength="13" 
					size="11" name="tbvcapital[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>][2015]" style="text-align:left;">
				</td>
				<td align="right"><b id="total_<?=$tipo['tbtid'] ?>_<?=$linha['linha']?>">0,00</b></td>
				<td align="right"><input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="radio" name="tbfprojeto[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" value="true" <?=($linha['tbrprojeto']=='t'?'checked="checked"':'') ?>/></td>
				<td align="right"><input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="radio" name="tbfprojeto[<?=$tipo['tbtid'] ?>][<?=$linha['linha']?>]" value="false" <?=($linha['tbrprojeto']=='f'?'checked="checked"':'') ?>/></td>
				</tr>
				<?php 
				}
				?>
				<tr id="novalinha_<?=$tipo['tbtid'] ?>">
					<td colspan="12" bgcolor="#DCDCDC">
					</td>
				</tr>
				<?php 
				}
				?>
				<tr>
					<td colspan="3" align="right"><b>Total:</b></td>
					<td align="right"><b id="total_2013_cu">0</b></td>
					<td align="right"><b id="total_2013_ca">0</b></td>
					<td align="right"><b id="total_2014_cu">0</b></td>
					<td align="right"><b id="total_2014_ca">0</b></td>
					<td align="right"><b id="total_2015_cu">0</b></td>
					<td align="right"><b id="total_2015_ca">0</b></td>
					<td align="right"><b id="total">0</b></td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(document).ready(function(){
	$('[name="tbvcusteio[<?=$blur1?>][<?=$blur2?>][2013]"]').blur();
	somaValores();
});

$('.baixar').live('click',function(){
	return window.open('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&arqid='+$(this).attr('id'), 
			   'ajuda', 
			   "height=300,width=450,scrollbars=yes,top=50,left=200" );
});
$('[name^="tbvcusteio"]').live( 'blur' ,function(){ somaValores() });
$('[name^="tbvcapital"]').live( 'blur' ,function(){ somaValores() });
$('.excluirlinha').live('click',function(){
	var idTbrid = $(this).attr('id');
	$.ajax({
	  	url: window.location,
	  	data: { req: 'excluirLinhaTabela', tbrid: idTbrid, entid: '<?=$_SESSION['academico']['entidcampus'] ?>',perid: '<?=$item['perid'] ?>'},
	  	success: function(data) {
	  	}
	});
	$(this).parent().parent().remove();
});
function somaValores( ){
	var linha;
	var total = 0;
	var total_linha;
	var temp_valor;
	//Total Custeio 2013	
	var total1 = 0;
	$('[name^="tbvcusteio"][name$="[2013]"]').each(function(){
		//Total da Linha
		linha = $(this).attr('name').replace('tbvcusteio','');
		linha = linha.replace('[2013]','');
		total_linha = 0;
		var temp_valor2 = 0;
		$('[name*="'+linha+'"]').each(function(){
			if( $(this).hasClass('valor') && $(this).val() != '' ){
				temp_valor2 = $(this).val();
				if( temp_valor2.indexOf(',') < 0 ){
					temp_valor2 = $(this).val()+',00';
					$(this).val(temp_valor2);
				}
				temp_valor2 = $(this).val().replace('.','');
				temp_valor2 = temp_valor2.replace('.','');
				temp_valor2 = temp_valor2.replace('.','');
				temp_valor2 = temp_valor2.replace(',','');
				total_linha = parseInt(total_linha)+parseInt(temp_valor2); 
			}
		});
		linha = linha.replace('[','_');
		linha = linha.replace('[','_');
		linha = linha.replace(']','');
		linha = linha.replace(']','');
		$('#total'+linha).html(mascaraglobal('[.###],##',total_linha));
		//FIM Total da Linha
		if( $(this).val() != '' ){
			temp_valor = $(this).val().replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace(',','');
			total1 = parseInt(total1)+parseInt(temp_valor);
		}
	});
	$('#total_2013_cu').html(mascaraglobal('[.###],##',total1));
	//Total Capital 2013	
	var total2 = 0;
	$('[name^="tbvcapital"][name$="[2013]"]').each(function(){
		if( $(this).val() != '' ){
			temp_valor = $(this).val().replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace(',','');
			total2 = parseInt(total2)+parseInt(temp_valor);
		}
	});
	$('#total_2013_ca').html(mascaraglobal('[.###],##',total2));
	//Total Custeio 2014
	var total3 = 0;	
	$('[name^="tbvcusteio"][name$="[2014]"]').each(function(){
		if( $(this).val() != '' ){
			temp_valor = $(this).val().replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace(',','');
			total3 = parseInt(total3)+parseInt(temp_valor);
		}
	});
	$('#total_2014_cu').html(mascaraglobal('[.###],##',total3));
	//Total Capital 2014	
	var total4 = 0;	
	$('[name^="tbvcapital"][name$="[2014]"]').each(function(){
		if( $(this).val() != '' ){
			temp_valor = $(this).val().replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace(',','');
			total4 = parseInt(total4)+parseInt(temp_valor);
		}
	});
	$('#total_2014_ca').html(mascaraglobal('[.###],##',total4));
	//Total Custeio 2015	
	var total5 = 0;	
	$('[name^="tbvcusteio"][name$="[2015]"]').each(function(){
		if( $(this).val() != '' ){
			temp_valor = $(this).val().replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace(',','');
			total5 = parseInt(total5)+parseInt(temp_valor);
		}
	});
	$('#total_2015_cu').html(mascaraglobal('[.###],##',total5));
	//Total Capital 2015	
	var total6 = 0;
	$('[name^="tbvcapital"][name$="[2015]"]').each(function(){
		if( $(this).val() != '' ){
			temp_valor = $(this).val().replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace('.','');
			temp_valor = temp_valor.replace(',','');
			total6 = parseInt(total6)+parseInt(temp_valor);
		}
	});
	$('#total_2015_ca').html(mascaraglobal('[.###],##',total6));
	total = parseInt(total1)+parseInt(total2)+parseInt(total3)+parseInt(total4)+parseInt(total5)+parseInt(total6);
	$('#total').html(mascaraglobal('[.###],##',total));
}
$('.novaLinha').live('click',function(){

	var tbtid = $(this).attr('name');
	var linha;
	$('[name^="linha['+tbtid+']"]').each(function(){
		linha = parseInt($(this).val())+1;
	});
	if( !linha ){
		linha = 1;
	}
	var html = '<tr><td><input type="hidden" name="linha['+tbtid+']['+linha+']" value="'+linha+'"/>'
					+'<input type="hidden" name="tbrid2['+tbtid+']['+linha+']" value=""/>'
					+'<img border="0" src="../imagens/excluir_2.gif" width="12px" class="excluirlinha" title="Excluir Linha." style="cursor:pointer;">  '
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" ' 
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" maxlength="100" ' 
					+'size="21" name="tbrdemanda['+tbtid+']['+linha+']" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" ' 
					+'onfocus="MouseClick(this);this.select();" onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" onmouseover="MouseOver(this);" value="" maxlength="100" ' 
					+'size="21" name="tbrarea['+tbtid+']['+linha+']" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" ' 
					+'onfocus="MouseClick(this);this.select();" onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onmouseover="MouseOver(this);" value="" maxlength="100" '
					+'size="21" name="tbraluno['+tbtid+']['+linha+']" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);"' 
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);"' 
					+'onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" value="" maxlength="13"' 
					+'size="11" name="tbvcusteio['+tbtid+']['+linha+'][2013]" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);"' 
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
					+'onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" value="" maxlength="13" '
					+'size="11" name="tbvcapital['+tbtid+']['+linha+'][2013]" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" '
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
					+'onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" value="" maxlength="13" '
					+'size="11" name="tbvcusteio['+tbtid+']['+linha+'][2014]" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" '
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
					+'onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" value="" maxlength="13" '
					+'size="11" name="tbvcapital['+tbtid+']['+linha+'][2014]" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" '
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
					+'onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" value="" maxlength="13" '
					+'size="11" name="tbvcusteio['+tbtid+']['+linha+'][2015]" style="text-align:left;">'
				+'</td><td>'
					+'<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="text" class=" normal valor obrigatorio" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" '
					+'onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" '
					+'onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" value="" maxlength="13" '
					+'size="11" name="tbvcapital['+tbtid+']['+linha+'][2015]" style="text-align:left;">'
				+'</td>'
				+'<td align="right"><b id="total_'+tbtid+'_'+linha+'">0,00</b></td>'
				+'<td align="right"><input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="radio" name="tbfprojeto['+tbtid+']['+linha+']" value="true"/></td>'
				+'<td align="right"><input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="radio" name="tbfprojeto['+tbtid+']['+linha+']" value="false"/></td>'
				+'</tr>';
	$('#novalinha_'+tbtid).before(html);
});
</script>