<?
monta_titulo( "Demanda Social", '<center><img border="0" align="top" src="../imagens/obrig.gif">Indica campo obrigatório</center>');
$permissoes = pegaPermissao();

require_once APPRAIZ . "www/includes/webservice/cpf.php";
?>
<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>		
<script type="text/javascript" src="/includes/entidadesn.js"></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){

	jQuery('.novaIndicacao').click(function(){
		var cur_periodo = jQuery(this).attr('id');

		var novo = jQuery('#novo_'+cur_periodo).clone();
		novo.show();
		novo.attr('id','');
		jQuery('[id=tabela_'+cur_periodo+'] tr:last').after(novo);
		
		var x = 0;
		jQuery('[id=tabela_'+cur_periodo+'] tr').each(function(){
			if( x == 0 ){
				jQuery(this).attr('bgcolor','#e9e9e9');
			}else if(x%2 == 0){
				jQuery(this).attr('bgcolor','white');
			}else{
				jQuery(this).attr('bgcolor','#f5f5f5');
			}
			x = x+1;
		});
	});

	jQuery('.excluir').live('click',function(){
		jQuery(this).parent().parent().remove();
		var x = 0;
		jQuery(this).parent().parent().parent().each(function(){
			if( x == 0 ){
				jQuery(this).attr('bgcolor','#e9e9e9');
			}else if(x%2 == 0){
				jQuery(this).attr('bgcolor','white');
			}else{
				jQuery(this).attr('bgcolor','#f5f5f5');
			}
			x = x+1;
		});
	});

	jQuery('#salvarCont').click(function(){
		jQuery('#proximo').val('proximo');
		jQuery('#salvar').click();
	});

	jQuery('#salvar').click(function(){
		jQuery(this).attr('disabled'.true);
		var erro = false;
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).parent().parent().css('display') != 'none' && jQuery(this).parent().parent().parent().css('display') != 'none'  ){
				if( jQuery(this).val() == '' ){
					alert('Campo obrigatório');
					jQuery(this).focus();
					erro = true;
					return false;
				}
				var nome = jQuery(this).attr('name');
			}
		});
		jQuery('[name^=mdsemail]').each(function(){
			if( jQuery(this).val() != '' ){
				if( !validaEmail( jQuery(this).val() ) ){
					alert('Email inválido');
					jQuery(this).focus();
					erro = true;
					return false;
				}
			}
		});
		if( erro ){
			jQuery(this).attr('disabled'.false);
			return false;
		}
		jQuery('#requisicao').val('gravarDemandaSocial');
		jQuery('#formulario').submit();
	});

	jQuery('[name^=mdscpf]').live('blur',function(){

		var comp = new dCPF();
		var cpf = jQuery(this).val();
		cpf = cpf.replace('.','');
		cpf = cpf.replace('.','');
		cpf = cpf.replace('-','');
		comp.buscarDados( cpf );
		
		var id = jQuery(this).attr('name');
		id = id.substring(6, id.indexOf('[]'));
		if( comp.dados.no_pessoa_rf != '' ){
			jQuery(this).parent().parent().find('td:eq(1)').find('input:first').val(comp.dados.no_pessoa_rf);
		}else{
			alert('CPF não encontrado na base da Receita Federal.');
			jQuery(this).val('');
			jQuery(this).parent().parent().find('td:eq(1)').find('input:first').val('');
			return false;
		}
	});
});
</script>
<form id="formulario" name="formulario" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="proximo" id="proximo" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
	<tr>
		<td class="blue subtituloDireita" width="8%">Orientações</td>
		<td class="blue" >
			Caso, na tela "2.1 Proposta da Escola", a escola tenha indicado algum profissional do corpo docente ou diretivo para participar de cursos que admitem "demanda social", abaixo é possível especificar essa demanda. Cada pessoa poderá ser indicada para apenas um curso. O preenchimento da demanda social é facultado à escola e à Secretaria de Educação, que deve validar a proposta final.
		</td>
	</tr>
	<tr align="left">
		<td class="subtituloDireita" >Demanda Social</td>
		<td>
		<?php 
			$cursos = recuperaCursosDemandaSocial();
			$cursos = $cursos ? $cursos : Array();
			
			if( is_array($cursos) ){
			foreach( $cursos as $curso ){
		?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="5" align="center">
				<tr>
					<td class="subtituloDireita" style="text-align:center" colspan="2"><b>Nome do Curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Área Temática</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Publico-alvo da demanda social</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Nível do Curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Modalidade do curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Carga horária total</b></td>
				</tr>
				<tr>
					<td class="subtituloDireita" style="text-align:center" colspan="2"><?=$curso['curdesc'] ?></td>
					<td class="subtituloDireita" style="text-align:center" ><?=$curso['atedesc'] ?></td>
					<td class="subtituloDireita" style="text-align:center" >
						<img border="0" style="cursor:pointer;" align="top" src="../imagens/info.gif" 
							 onmouseover="SuperTitleAjax('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=toolPublicoAlvoDemandaSocialCurso&curid=<?=$curso['curid'] ?>')" 
					     	 onmouseout="SuperTitleOff( this )">
					     <?php $publicos = recuperaPublicoAlvoDemandaSocialCurso( $curso['curid'] ); ?>
					</td>
					<td class="subtituloDireita" style="text-align:center" ><?=$curso['ncudesc'] ?></td>
					<td class="subtituloDireita" style="text-align:center" >
						<?php 
						
						$modalidades = recuperaModalidadeCurso( $curso['curid'] );
						
						if( is_array($modalidades) ){
							foreach( $modalidades as $modalidade){
								echo $modalidade."<br>";
							}
						}
						?>
					</td>
					<td class="subtituloDireita" style="text-align:center" >De <?=$curso['curchmim'] ?> até <?=$curso['curchmax'] ?></td>
				</tr>
				<?php 
				
				$semestres = pegaSemestresCurso( $curso['curid'] );
				
				if( is_array($semestres) ){
				foreach( $semestres as $semestre ){ 
				?>
				<tr>
					<td  colspan="7">
					<b>Indicados para o <?=$semestre['pcfdesc'] ?></b>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="5" align="center" id="tabela_<?=$curso['curid']."_".$semestre['pcfid']  ?>" style="width:100%" >
							<tr bgcolor="#e9e9e9">
								<td><b>CPF</b></td>
								<td><b>Nome</b></td>
								<td><b>Publico-alvo da demanda social</b></td>
								<td><b>Modalidade</b></td>
								<td><b>Email</b></td>
								<td><b>Telefone Fixo</b></td>
								<td><b>Telefone Celular</b></td>
							</tr>
						<?php 
						
						$docentes = recuperaProfissionaisCurso( $curso['curid'], $semestre['pcfid'] );
						
						if( is_array($docentes) ){
						$modids = Array();
						foreach( $docentes as $k => $docente ){
							$modids[] = $docente['modid'];
						?>
							<tr <?=($k%2==0) ? 'style="background-color:white"' : '' ?> >
								<td><b><?=str_replace(',','.',$docente['num_cpf']) ?></b></td>
								<td><b><?=$docente['no_docente'] ?></b></td>
								<td><b> - </b></td>
								<td><b><?=$docente['moddesc'] ?></b></td>
								<td><b><?=$docente['pfdemail'] ?></b></td>
								<td><b><?=$docente['pfdtelefone'] ?></b></td>
								<td><b><?=$docente['pfdecelular'] ?></b></td>
							</tr>
						<?php 
						}
						}
						?>
						<tr id="novo_<?=$curso['curid']."_".$semestre['pcfid']  ?>" style="display:none;background-color:white">
								<td><?=campo_texto('mdscpf['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome', '11', '255','###.###.###-##',''); ?></td>
								<td><?=campo_texto('mdsnome['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome', '20', '255','','','','','','readonly'); ?></td>
								<td>
									<?php 
									
									$sql = "SELECT
												pad.padid as codigo,
												pad.paddesc as descricao
											FROM
												catalogocurso.cursodemandasocial cms
											INNER JOIN catalogocurso.publicoalvodemandasocial pad ON pad.padid = cms.padid 
											WHERE
												curid = ".$curso['curid'];
									$db->monta_combo('padid['.$curso['curid'].']['.$semestre['pcfid'].'][]', $sql, ($permissoes['gravar']?'S':'N'), 'Selecione...', '', '', 'Publico-Alvo', '150', 'S', 'padid', '', $_POST['padid']); 
									?>
								</td>
								<td>
									<?php 
									
									$sql = "SELECT 
												mod.modid as codigo,
												mod.moddesc as descricao
											FROM 
												catalogocurso.modalidadecurso_curso mcu
											INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = mcu.modid
											WHERE
												mod.modid IN (".implode(',',$modids).") AND
												mcu.curid = ".$curso['curid'];
									$db->monta_combo('modid['.$curso['curid'].']['.$semestre['pcfid'].'][]', $sql, ($permissoes['gravar']?'S':'N'), 'Selecione...', '', '', 'Modalidade do Curso', '', 'S', 'modid', '', $_POST['modid']);
									?>
								</td>
								<td><?=campo_texto('mdsemail['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Telefone fixo', '20', '50','',''); ?></td>
								<td><?=campo_texto('mdstelefonefixo['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Telefone fixo', '10', '30','##-####-####',''); ?></td>
								<td>
									<?=campo_texto('mdscelular['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Telefone celular', '10', '30','##-####-####',''); ?>
									<img border="0" align="top" title="Exluir indicação." class="excluir" src="../imagens/excluir.gif">
								</td>
							</tr>
						<?php 
						
						$membros = recuperaMembrosCurso( $curso['curid'], $semestre['pcfid'] );
						
						if(is_array($membros)){
							foreach( $membros as $k => $membro ){
						?>
							<tr <?=($k%2==0) ? 'style="background-color:white"' : '' ?> >
								<td>
									<?=campo_texto('mdscpf['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'S', 
									($permissoes['gravar'] ? 'S' : 'N'), 'CPF', '11', '255','###.###.###-##','',
									'','','','','',trim($membro['mdscpf'])); ?></td>
								<td>
									<?=campo_texto('mdsnome['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'S', 
										($permissoes['gravar'] ? 'S' : 'N'), 'Nome', '20', '255','','',
										'','','','','',$membro['mdsnome']); ?></td>
								<td>
									<?php 
									
									$sql = "SELECT
												pad.padid as codigo,
												pad.paddesc as descricao
											FROM
												catalogocurso.cursodemandasocial cms
											INNER JOIN catalogocurso.publicoalvodemandasocial pad ON pad.padid = cms.padid 
											WHERE
												curid = ".$curso['curid'];
									$db->monta_combo('padid['.$curso['curid'].']['.$semestre['pcfid'].'][]', $sql, ($permissoes['gravar']?'S':'N'), 
													 'Selecione...', '', '', 'Publico-Alvo', '150', 'S', 'padid', '', $membro['padid']); 
									?>
								</td>
								<td>
									<?php 
									
									$sql = "SELECT 
												mod.modid as codigo,
												mod.moddesc as descricao
											FROM 
												catalogocurso.modalidadecurso_curso mcu
											INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = mcu.modid
											WHERE
												mcu.curid = ".$curso['curid'];
									$db->monta_combo('modid['.$curso['curid'].']['.$semestre['pcfid'].'][]', $sql, ($permissoes['gravar']?'S':'N'), 
													 'Selecione...', '', '', 'Modalidade do Curso', '', 'S', 'modid', '', $membro['modid']);
									?>
								</td>
								<td><?=campo_texto('mdsemail['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 
												   'Telefone fixo', '20', '50','','',
												   '','','','','',$membro['mdsemail']); ?></td>
								<td><?=campo_texto('mdstelefonefixo['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 
												   'Telefone fixo', '10', '30','##-####-####','',
												   '','','','','',$membro['mdstelefonefixo']); ?></td>
								<td>
									<?=campo_texto('mdscelular['.$curso['curid'].']['.$semestre['pcfid'].'][]', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 
									  			   'Telefone celular', '10', '30','##-####-####','',
												   '','','','','',$membro['mdscelular']); ?>
									<img border="0" align="top"  title="Exluir indicação." class="excluir" src="../imagens/excluir.gif">
								</td>
							</tr>
						<?php 
							}
						}
						?>
						</table>
						<?php 
							if($permissoes['gravar']){
								if( count($publicos) > 0 ){
						?>
							<input type="button" value="Nova indicação" class="novaIndicacao" id="<?=$curso['curid']."_".$semestre['pcfid']  ?>" />
						<?php 
								}else{
						?>
							<label style="color:red;">Curso sem publico-alvo da demanda social definido.</label>
						<?php 
								}
							}
						?>
					</td>
				</tr>
				<?php 
				}
				}
				?>
			</table>
		<?php 
			}
			}
		?>
		</td>
	</tr>
</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita">O que deseja fazer agora?</td>
		<td>
			<input type="button" name="anterior"   value="Anterior" onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_2_relacaoprioridade';">
			<?php if( $permissoes['gravar'] ){?>
			<input type="button" name="salvar"     id="salvar" value="Salvar" >
			<input type="button" name="salvarCont" id="salvarCont" value="Salvar e Continuar">
			<?php }?>
			<input type="button" name="proximo"    value="Próximo"  onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_1_cursosugerido';">
		</td>
	</tr>
</table>
</form>