<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function gravarQuestionario() {

	<? if($perguntainicial) : ?>
	var conf = confirm('<?=$perguntainicial ?>');
	if(!conf) {
		return true;
	}
	<? endif; ?>


	

	<? if($es) : ?>
		<? foreach($es as $campo => $arr) : ?>

		<? if($arr['opcoes']) : ?>
		
			<? if($arr['tipo']=='checkbox' || $arr['tipo']=='radio') : ?>
			
				var opcoesmarcadas    = jQuery("[name^='<?=$campo.$htmlidmaster ?>'][type='<?=$arr['tipo'] ?>']:enabled:checked").length;
				
				if(opcoesmarcadas==0) {
					alert('Marque a questão: <?=$arr['texto']?>');
					return false;
				}

				<? foreach($arr['opcoes'] as $op) : ?>
					<? if($op['complementotexto']) : ?>
					
						var opcoesmarcadasesp    = jQuery("[name^='<?=$campo ?>'][value^='<?=$op['valor'] ?>']:enabled:checked").length;
						if(opcoesmarcadasesp==1) {
							if(jQuery('#tx_<?=$campo ?>_<?=$op['valor']?>').val()=='') {
								alert('Preencha : <?=$op['complementotexto'] ?>, <?=$op['descricao'] ?>');
								return false;
							}
						}
	
	
					<? endif; ?>
				<? endforeach; ?>
				
			<? endif; ?>
		
		<? endif; ?>

		<? if($arr['datas']) : ?>

			<? foreach($arr['datas'] as $dt) : ?>

			if(jQuery('#<?=$campo.$dt['valor'] ?>').val()=='') {
				alert('Preencha a data: <?=$dt['descricao'] ?>');
				return false;
			}

			<? endforeach; ?>


		<? endif; ?>

		<? if($arr['text']) : ?>

		if(jQuery('#tx_<?=$campo ?>').val()=='') {
			alert('Preencha: <?=$arr['texto']?>');
			return false;
		}
		
		<? endif; ?>

		<? if($arr['tipo']=='gridradio') : ?>

		<? foreach($arr['linhas'] as $linha) : ?>

		if(jQuery( "input:radio[name=<?=$campo.$linha['codigo'].$htmlidmaster ?>]:checked" ).length==0) {
			alert('Selecione opção : <?=$linha['descricao'] ?>');
			return false;
		}
		
		<? endforeach; ?>

		<? endif; ?>
		
		<? endforeach; ?>
	<? endif; ?>
	

	
	divCarregando();
	
	jQuery('#salvar').attr('disabled','disabled');
	
	document.getElementById('formulario').submit();
}

function exibir_dv_chk(id,obj) {

	if(obj.checked) {
		jQuery('#dv_'+id+'_'+obj.value).css('display','');
	} else {
		jQuery('#tx_'+id+'_'+obj.value).val('');
		jQuery('#dv_'+id+'_'+obj.value).css('display','none');
	}
	
}

function exibir_dv_rd(id,obj) {
	jQuery("[id^='dv_"+id+"']").css('display','none');
	jQuery("[id^='tx_"+id+"']").val('');
	
	if(obj.checked) {
		jQuery('#dv_'+id+'_'+obj.value).css('display','');
	}
	
}

jQuery(document).ready(function() {});


</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2"><?=$titulo ?></td>
	</tr>
	<? if($modoRelatorio) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">TOTAL DE RESPOSTAS</td>
		<td><?=count($registros) ?></td>
	</tr>
	<? elseif(!$consulta_q) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&rfuparcela=".$rfuparcela); ?></td>
	</tr>	
	<? endif; ?>
	
	<tr>
		<td colspan="2">
	<?
	if($es) {

		echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		
		foreach($es as $campo => $arr) {
			
			$cp = explode(";",$$campo);
			unset($ff);
			foreach($cp as $vv) {
				$pp = explode("||",$vv);
				$ff[] = $pp[0];
				$var = 'tx_'.$campo.'_'.$pp[0];
				$$var = $pp[1];
			}
			
			echo '<tr>';
			echo '<td class="SubTituloDireita" width="25%">'.$arr['texto'].'</td>';
			
			echo '<td>';
			
			
			if($arr['tipo']=='gridradio') {
				
				echo '<table width="100%">';
				
				echo '<tr>';
				echo '<td class="SubTituloCentro">&nbsp;</td>';
				foreach($arr['colunas'] as $coluna) {
					echo '<td class="SubTituloCentro">'.$coluna['descricao'].'</td>';
				}
				echo '</tr>';
				
				
				foreach($arr['linhas'] as $linha) {
					echo '<tr>';
					echo '<td>'.$linha['descricao'].'</td>';
					
					foreach($arr['colunas'] as $coluna) {
						
						
						if($modoRelatorio) {
							$x = $campo.$linha['codigo'].$htmlidmaster;
							echo '<td align=center>'.$arrFinal[$x][$coluna['codigo']].' / '.round(($arrFinal[$x][$coluna['codigo']]/count($registros))*100,2).'%</td>';
						} else {
							$rr = $campo.$linha['codigo'];
							echo '<td align=center><input type=radio '.(($consulta_q)?'disabled':'').' name='.$campo.$linha['codigo'].$htmlidmaster.' value='.$coluna['codigo'].' '.(($$rr==$coluna['codigo'])?'checked':'').'></td>';
						}
					}
					
					echo '</tr>';
				}
				
				echo '</table>';
			
			}
						
			if($arr['text']) {
				if($modoRelatorio) {
					echo 'Questão não agrupável';
				} else {
					if($consulta_q) {
						echo nl2br($$campo);
					} else {
						echo '<div class="notprint"><textarea id="tx_'.$campo.'" name="tx_'.$campo.'" cols="'.$arr['text']['cols'].'" rows="'.$arr['text']['rows'].'" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); textCounter( this.form.tx_'.$campo.', this.form.no_tx_'.$campo.', '.$arr['text']['maxsize'].');" style="width:70ex;" onkeydown="textCounter( this.form.tx_'.$campo.', this.form.no_tx_'.$campo.', '.$arr['text']['maxsize'].' );" onkeyup="textCounter( this.form.tx_'.$campo.', this.form.no_tx_'.$campo.', '.$arr['text']['maxsize'].');" class="txareanormal">'.$$campo.'</textarea><br><input readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_tx_'.$campo.'" size="6" maxlength="6" value="'.$arr['text']['maxsize'].'" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font></div>';
						echo (($arr['text']['dica'])?'<br><span style=font-size:x-small;>Dica : '.$arr['text']['dica'].'</span>':'');
					}
				}
			}
			
			if($arr['opcoes']) {
				foreach($arr['opcoes'] as $op) {

					if($arr['tipo']=='radio') {
						if($modoRelatorio) {
							echo $op['descricao'].' - '.$arrFinal[$campo.$htmlidmaster][$op['valor']].' / '.round(($arrFinal[$campo.$htmlidmaster][$op['valor']]/count($registros))*100,2).'%<br>';
						} else { 
							echo '<input type="radio" '.(($consulta_q)?'disabled':'').' name="'.$campo.$htmlidmaster.'" '.((in_array($op['valor'],$ff))?'checked':'').' value="'.$op['valor'].'" id="'.$campo.'_'.$op['valor'].'" onclick="exibir_dv_rd(\''.$campo.'\',this)"> '.$op['descricao'].'<br>';
						}
					} elseif($arr['tipo']=='checkbox') {
						if($modoRelatorio) {
							echo $op['descricao'].' - '.$arrFinal[$campo][$op['valor']].' / '.round(($arrFinal[$campo][$op['valor']]/count($registros))*100,2).'%<br>';
						} else {
							echo '<input type="checkbox" '.(($consulta_q)?'disabled':'').' '.((in_array($op['valor'],$ff))?'checked':'').' name="'.$campo.'[]" value="'.$op['valor'].'" '.(($op['complementotexto'])?'onclick="exibir_dv_chk(\''.$campo.'\',this)"':'').'> '.$op['descricao'].'<br>';
						}
					}
					
					if($op['complementotexto']) {
						$var2 = 'tx_'.$campo.'_'.$op['valor']; 
						if($consulta_q) {
							echo nl2br($$var2);
						} else {
							echo '<div style="width: 50%;padding: 10px;border: 3px solid gray;margin: 0px; '.((in_array($op['valor'],$ff))?'':'display:none;').'" id="dv_'.$campo.'_'.$op['valor'].'">';
							echo $op['complementotexto'].' <div class="notprint"><textarea id="tx_'.$campo.'_'.$op['valor'].'" name="tx_'.$campo.'_'.$op['valor'].'" cols="20" rows="3" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); textCounter( this.form.tx_'.$campo.'_'.$op['valor'].', this.form.no_tx_'.$campo.'_'.$op['valor'].', 50);" style="width:70ex;" onkeydown="textCounter( this.form.tx_'.$campo.'_'.$op['valor'].', this.form.no_tx_'.$campo.'_'.$op['valor'].', 50 );" onkeyup="textCounter( this.form.tx_'.$campo.'_'.$op['valor'].', this.form.no_tx_'.$campo.'_'.$op['valor'].', 50);" class="txareanormal">'.$$var2.'</textarea><br><input readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_tx_'.$campo.'_'.$op['valor'].'" size="6" maxlength="6" value="50" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font></div>';
							echo '</div>';
						}
						
					}
					
				}
			}
			
			if($arr['datas']) {
				if($modoRelatorio) {
					echo 'Questão não agrupável';
				} else {
					echo '<table>';
					foreach($arr['datas'] as $dt) {
						echo '<tr>';
						echo '<td align="right">'.$dt['descricao'].'</td>';
						echo '<td>';
						$d = campo_data2($campo.$dt['valor'],'S', (($consulta_q)?'N':'S'), $dt['descricao'], 'S', '', '', '', '', '', $campo.$dt['valor']);
						echo $d;
						echo '</td>';
						echo '</tr>';
					}
					echo '</table>';
				}
			}
				
				
			echo '</td>';
			
			echo '</tr>';

		}
		
		echo '</table>';
		
	}
	
	
	?>
	
		</td>
	</tr>
	<? if(!$modoRelatorio && !$consulta_q) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Salvar" id="salvar" onclick="gravarQuestionario();"> 
		</td>
	</tr>
	<? endif; ?>
</table>

