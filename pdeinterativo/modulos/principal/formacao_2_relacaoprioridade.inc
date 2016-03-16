<?
$permissoes = pegaPermissao();

$sql = "SELECT 
			true 
		FROM 
			pdeinterativo.planoformacaodocente 
		WHERE 
			pfdstatus = 'A' 
			AND pfdprioridade IS NULL 
			AND curid IS NOT NULL
			AND pdeid = ".$_SESSION['pdeinterativo_vars']['pdeid'];
$prioridadeNull = $db->pegaUm($sql);

monta_titulo( "Ordem de Prioridade", ($prioridadeNull == 't' ? "<strong><label style=\"color:red\" >Prioridade não definida. Caso não haja opções, clique em salvar para validar a prioridade padrão.</label></strong>" : '') );


$sql = "SELECT DISTINCT
			cur.curid,
			cur.curdesc,
		    moc.moddesc,
		    moc.modid
		FROM
			catalogocurso.curso cur
		    inner join pdeinterativo.planoformacaodocente pf on pf.curid = cur.curid
		    inner join catalogocurso.modalidadecurso moc on moc.modid = pf.modid
		WHERE
			pf.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}";
$arrCurso = $db->carregar( $sql );
$arrCurso = $arrCurso ? $arrCurso : array();

$sql = "SELECT DISTINCT
			pe.pcfid,
		    pe.pcfdesc
		FROM
		    pdeinterativo.planoformacaodocente pf
		    INNER JOIN pdeinterativo.periodocursoformacao  pe ON pe.pcfid  = pf.pcfid
		WHERE
			pf.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
			and pe.pcfstatus = 'A'
        ORDER BY pe.pcfid";
$arrPeriodo = $db->carregar( $sql );
$arrPeriodo = $arrPeriodo ? $arrPeriodo : array();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function salvarPrioridade(action){
	document.getElementById('requisicao').value = 'salvarPrioridade';
	document.getElementById('action').value = action;
	document.getElementById('formulario').submit();
}

$(document).ready(function(){

	$('.troca').change(function(){
		var esse    = $(this).attr('name');
		var esseVal = $(this).val();
		var classe  = $('[id='+esse+'_sem]').val();
		
		$('.'+classe).each(function(){
			if( $(this).attr('name') != esse && $(this).val() == esseVal ){
				$(this).val('');
			}
		});
	});

	$('#salvar').click(function(){

		$(this).attr('disabled',true);

		var incompleto = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				alert('Campo obrgatório.');
				$(this).focus();
				$('#salvar').attr('disabled',false);
				incompleto = true;
				return false;
			}
		});

		if(incompleto){
			return false;
		}

		$('#requisicao').val('salvarPrioridade');
		$('#action').val('salvar');
		$('#formulario').submit();
	});

	$('#salvarCont').click(function(){

		$(this).attr('disabled',true);

		var incompleto = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				alert('Campo obrgatório.');
				$(this).focus();
				$('#salvar').attr('disabled',false);
				incompleto = true;
				return false;
			}
		});

		if(incompleto){
			return false;
		}

		$('#requisicao').val('salvarPrioridade');
		$('#action').val('continuar');
		$('#formulario').submit();
	});
		
});
</script>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="action" id="action" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
	<tr>
		<td class="blue subtituloDireita" width="10%">Orientações</td>
		<td class="blue" >
			Nesta tela, é possível escolher a ordem de prioridade dos professores para participação nos cursos de formação continuada. Só é necessário estabelecer uma prioridade caso mais de um professor tenha sido indicado para o mesmo curso no mesmo período.
			<br><br><b>Essa informação será útil quando houver uma limitação e a oferta de vagas for menor que a demanda.</b>
		</td>
	</tr>
	<tr align="left">
		<td class="subtituloDireita" >Ordem de Prioridade</td>
		<td>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr bgcolor="#dedfde">
					<td style="text-align:center" colspan="10"><b>Relação de Profissionais</b></td>
				</tr>
				<?if( $arrCurso ){ ?>
				<tr>
					<td class="subtituloDireita" style="text-align:center" ><b>Curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Modalidade</b></td>
					<?foreach ($arrPeriodo as $periodo) {
						echo '<td class="subtituloDireita" style="text-align:center" ><b>'.$periodo['pcfdesc'].'</b></td>';
					}
					 ?>
				</tr>
				<?
				foreach ($arrCurso as $key => $curso) {
					$key % 2 ? $cor = "#dedfde" : $cor = ""; 
					?>
					<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td style="text-align:center" ><?=$curso['curdesc']; ?></td>
						<td style="text-align:center" ><?=$curso['moddesc']; ?></td>
						<?
						foreach ($arrPeriodo as $periodo) {
							$sql = "SELECT pesnome, pfdprioridade, pfdid FROM (
										SELECT DISTINCT
											pes.pesnome,
											pf.pfdprioridade,
											pf.pfdid
										FROM
											pdeinterativo.pessoa pes
										    INNER JOIN pdeinterativo.planoformacaodocente pf ON pf.pesid  = pes.pesid
										WHERE
											pf.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
										    and pf.pcfid = {$periodo['pcfid']}
										    and pf.curid = {$curso['curid']}
										    and pf.modid = {$curso['modid']}
										union all
										SELECT DISTINCT
										    d.no_docente,
										    pf.pfdprioridade,
										    pf.pfdid
										FROM 
										    educacenso_".ANO_CENSO.".tab_docente d 
										INNER JOIN pdeinterativo.planoformacaodocente pf ON pf.pk_cod_docente = d.pk_cod_docente
										INNER JOIN educacenso_".ANO_CENSO.".tab_dado_docencia  dc ON dc.fk_cod_docente = d.pk_cod_docente
										WHERE 
											id_tipo_docente = 1
										    and pf.pcfid ={$periodo['pcfid']}
										    and pf.curid = {$curso['curid']}
										    and pf.modid = {$curso['modid']}
										  	AND pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
									) as f
    								order by 1";
							
							$arNome = $db->carregar( $sql );
							$arNome = $arNome ? $arNome : array();
							
							$valor = '';
							$html = '<td style="text-align:left" >';
							$habilita = 'N';
							if( sizeof($arNome) > 1 ) $habilita = 'S';
							 
							foreach ($arNome as $chave => $nome) {
								$pfdprioridade = $nome['pfdprioridade'] ? $nome['pfdprioridade'] : $chave + 1;
								if( count($arNome) > 1 ){
									$valor .= '<input type="hidden" id="pfdprioridade['.$nome['pfdid'].']_sem" value="'.$curso['curid'].'_'.$curso['modid'].'_'.$periodo['pcfid'].'" />';
									$valor .= '<select '.($permissoes['gravar']?'':'disabled').' id="pfdprioridade['.$nome['pfdid'].']" 
												style="width: auto" class="CampoEstilo obrigatorio troca '.$curso['curid'].'_'.$curso['modid'].'_'.$periodo['pcfid'].'" 
												name="pfdprioridade['.$nome['pfdid'].']">
													<option value="">Selecione...</option>';
									foreach ($arNome as $x => $y){
										$valor .= '<option '.( ($x+1) == $pfdprioridade ? 'selected="selected"' : '' ).'value="'.($x+1).'">'.($x+1).'</option>';
									}
									$valor .= '</select><img border="0" src="../imagens/obrig.gif">'.' '.$nome['pesnome'].'<br>';
								}else{
									$valor .= '<input type="hidden" name="pfdprioridade['.$nome['pfdid'].']" value="1"/>
												<select id="pfdprioridade['.$nome['pfdid'].']" disabled="disabled" style="width: auto" 
												class="CampoEstilo" name="pfdprioridade['.$nome['pfdid'].']_disable">
													<option value="1">1</option>
												</select>'.' '.$nome['pesnome'].'<br>'; 
								}
									
//								$valor .= campo_texto('pfdprioridade['.$nome['pfdid'].']', 'N', $habilita, 'Ordem de Prioridade', 4, 5, '[#]', '','','','','id="pfdprioridade"', '', $pfdprioridade).' '.$nome['pesnome'].'<br>';
							}
							
							$html .= $valor.'</td>';
							echo $html;
						}
						?>
					</tr>		
			<?  }
			} else {
				print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
				print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
				print '</table>';
			} ?>
			</table>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
				<tr>
					<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
					<td>
						<input type="button" name="anterior"   value="Anterior" onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_planoformacao';"/>
						<?php if( $permissoes['gravar'] ): ?>
							<input type="button" name="salvar"     id="salvar" value="Salvar" />
							<input type="button" name="salvarCont" id="salvarCont" value="Salvar e Continuar" onclick="salvarPrioridade('continuar');" />
						<?php endif;?> 
						<input type="button" name="proximo"    value="Próximo"  onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social';"/>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
</form>