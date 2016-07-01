<?
/*if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}*/

monta_titulo( "Sugestão de Curso", "&nbsp;");

$permissoes = pegaPermissao();

$arrFormacao = carregaCursoSugerido();

extract($arrFormacao);
$focformcontinuada = empty($focformcontinuada) ? 'N' : $focformcontinuada;

if( !empty($focid) ){
	$fexid = $db->carregar( "SELECT fex.fexid as codigo, fex.fexdesc as descricao 
							FROM pdeinterativo.cursosugeridofuncao 	csf 
							    inner join catalogocurso.funcaoexercida fex on fex.fexid = csf.fexid
							WHERE
								fex.fexstatus = 'A'
							    and csf.focid = $focid" );
	
	$pk_cod_etapa_ensino = $db->carregar( "SELECT tee.pk_cod_etapa_ensino as codigo, tee.no_etapa_ensino as descricao 
											FROM pdeinterativo.cursosugeridoetapa 	cse 
											    inner join educacenso_".ANO_CENSO.".tab_etapa_ensino tee on tee.pk_cod_etapa_ensino = cse.pk_cod_etapa_ensino
											WHERE
											    cse.focid = $focid" );
	
	$pk_cod_disciplina = $db->carregar( "SELECT tad.pk_cod_disciplina as codigo, tad.no_disciplina as descricao 
										FROM pdeinterativo.cursosugeridodisciplina 	csd 
										    inner join educacenso_".ANO_CENSO.".tab_disciplina tad on tad.pk_cod_disciplina = csd.pk_cod_disciplina
										WHERE
										    csd.focid = $focid" );
	
	$pk_cod_mod_ensino = $db->carregar( "SELECT tme.pk_cod_mod_ensino as codigo, tme.no_mod_ensino as descricao 
										FROM pdeinterativo.cursosugeridomodalidade csm 
										    inner join educacenso_".ANO_CENSO.".tab_mod_ensino tme on tme.pk_cod_mod_ensino 	= csm.pk_cod_mod_ensino
										WHERE
										    csm.focid = $focid" );
}

?>
<style type="text/css">
.disabled2 {
	border:none;
	border-left:#888888 3px solid;
	color:#404040;
	width:100ex;
	text-align:left;
}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function bloquear( val ){

	if(val){

		jQuery('.campos').hide();
		
		$('#foccurso').addClass('disabled');
		$('#focdescricao').removeClass('txareanormal');
		$('#focjustificativa').removeClass('txareanormal');
		$('#focdescricao').addClass('disabled2');
		$('#focjustificativa').addClass('disabled2');
		$('#fexid').removeClass('CampoEstilo');
		$('#fexid').addClass('disabled');
		$('#pk_cod_etapa_ensino').removeClass('CampoEstilo');
		$('#pk_cod_etapa_ensino').addClass('disabled');
		$('#pk_cod_disciplina').removeClass('CampoEstilo');
		$('#pk_cod_disciplina').addClass('disabled');
		$('#pk_cod_mod_ensino').removeClass('CampoEstilo');
		$('#pk_cod_mod_ensino').addClass('disabled');
		$('#focqtdprof').addClass('disabled');
		
		$('#foccurso').val(' ');
		$('#focdescricao').val(' ');
		$('#focjustificativa').val(' ');
		$('#fexid option').each(function(){
			$(this).remove();
		});
		$('#pk_cod_etapa_ensino option').each(function(){
			$(this).remove();
		});
		$('#pk_cod_disciplina option').each(function(){
			$(this).remove();
		});
		$('#pk_cod_mod_ensino option').each(function(){
			$(this).remove();
		});
		$('#focqtdprof').val(' ');
		
	}else{

		jQuery('.campos').show();
		
		$('#foccurso').removeClass('disabled');
		$('#focdescricao').addClass('txareanormal');
		$('#focjustificativa').addClass('txareanormal');
		$('#focdescricao').removeClass('disabled2');
		$('#focjustificativa').removeClass('disabled2');
		$('#fexid').addClass('CampoEstilo');
		$('#fexid').removeClass('disabled');
		$('#pk_cod_etapa_ensino').addClass('CampoEstilo');
		$('#pk_cod_etapa_ensino').removeClass('disabled');
		$('#pk_cod_disciplina').addClass('CampoEstilo');
		$('#pk_cod_disciplina').removeClass('disabled');
		$('#pk_cod_mod_ensino').addClass('CampoEstilo');
		$('#pk_cod_mod_ensino').removeClass('disabled');
		$('#focqtdprof').removeClass('disabled');

		if( $('#fexid option').length == 0 ){
			$('#fexid').html('<option value="">Duplo clique para selecionar da lista</option>');
			$('#pk_cod_etapa_ensino').html('<option value="">Duplo clique para selecionar da lista</option>');
			$('#pk_cod_disciplina').html('<option value="">Duplo clique para selecionar da lista</option>');
			$('#pk_cod_mod_ensino').html('<option value="">Duplo clique para selecionar da lista</option>');
		}
	}
	
	$('#foccurso').attr('disabled',val);
	$('#focdescricao').attr('disabled',val);
	$('#focjustificativa').attr('disabled',val);
	$('#fexid').attr('disabled',val);
	$('#pk_cod_etapa_ensino').attr('disabled',val);
	$('#pk_cod_disciplina').attr('disabled',val);
	$('#pk_cod_mod_ensino').attr('disabled',val);
	$('#focqtdprof').attr('disabled',val);
	
	
}

$(document).ready(function() {

	$('input[name=focformcontinuada]').click(function(){
		if( $(this).val() == 'N' ){
			if( confirm('Essa ação apagará as informações abaixo. Deseja coninuar?') ){
				bloquear(true);
			}else{
				$('#focformcontinuadaS').click(); 
			}
		}else{
			bloquear(false);
		}
	});

	$('#salvarCont').click(function(){
		$('#proximo').val('1');
		$('#salvar').click();
	});

	$('#salvar').click(function(){
		$('#requisicao').val('manterCursoSugerido');

		var erro = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				alert('Campo obrigatório.');
				$(this).focus();
				erro = true;
				return false;
			}
		});
		if(erro){
			return false;
		}

		var test = false;
		erro = false;
		$('#fexid option').each(function(){
			if($(this).val()==''){
				erro = true;
				vazio = $('#fexid');
			}
		});
		$('#pk_cod_etapa_ensino option').each(function(){
			if($(this).val()==''){
				erro = true;
				vazio = $('#pk_cod_etapa_ensino');
			}
		});
		$('#pk_cod_disciplina option').each(function(){
			if($(this).val()==''){
				erro = true;
				vazio = $('#pk_cod_disciplina');
			}
		});
		$('#pk_cod_mod_ensino option').each(function(){
			if($(this).val()==''){
				erro = true;
				vazio = $('#pk_cod_mod_ensino');
			}
		});
		if(erro){
			alert('Campo obrigatório.');
			vazio.focus();
			$(this).removeAttr('disabled');
			return false;
		}

		if(erro){
			return false;
		}
		
		selectAllOptions( document.formulario.fexid );
		selectAllOptions( document.formulario.pk_cod_etapa_ensino );
		selectAllOptions( document.formulario.pk_cod_disciplina );
		selectAllOptions( document.formulario.pk_cod_mod_ensino );
		
		$('#formulario').submit();
	});

	if( $('#focformcontinuadaN').attr('checked') ){
		bloquear(true);
	}else{
		bloquear(false);
	}

});

</script>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="proximo"    id="proximo" value="">
<input type="hidden" name="focid" 	   id="focid" value="<?=$focid; ?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
	<tr>
		<td class="blue subtituloDireita" width="10%">Orientações</td>
		<td class="blue" colspan="2">
			Caso a escola identifique alguma demanda por curso não disponível no Catálogo, poderá oferecer uma sugestão, a ser validada pelo gestor da rede, preenchendo todos os campos do quadro abaixo. Depois de finalizada a sugestão, clique em "Salvar".
			<br><br>Cada escola poderá sugerir apenas um curso por ano, mas é importante salientar que apenas as sugestões validadas pela Secretaria de Educação serão encaminhadas ao Fórum Estadual e, se acatadas, submetidas ao MEC para deliberação no Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais do Magistério da Educação Básica. Se aprovadas, as sugestões serão incluídas no Catálogo de Cursos para oferta no ano subseqüente. 
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita" rowspan="6">Sugestão de Curso</td>
		<td width="15%" align="right">
			A escola deseja sugerir curso de formação continuada ? 
		</td>
		<td>
			<input type="radio" value="S" <?=($permissoes['gravar'] ? '' : 'disabled')?>  name="focformcontinuada" <?=($focformcontinuada == 'S' ? 'checked="checked"' : ''); ?> id="focformcontinuadaS"/>Sim
			<input type="radio" value="N" <?=($permissoes['gravar'] ? '' : 'disabled') ?> name="focformcontinuada" <?=($focformcontinuada == 'N' ? 'checked="checked"' : ''); ?> id="focformcontinuadaN"/>Não
		</td>
	</tr>
	<tr class="campos" <?=($focformcontinuada != 'S' ? 'style="display:none"' : ''); ?>>
		<td align="right">
			Nome do Curso
		</td>
		<td>
			<?php echo campo_texto("foccurso","S",($permissoes['gravar'] ? 'S' : 'N'),"Nome do Curso","95","100","","","","","","id='foccurso'","")?>
		</td>
	</tr>
	<tr class="campos" <?=($focformcontinuada != 'S' ? 'style="display:none"' : ''); ?>>
		<td align="right">
			Descrição
		</td>
		<td>
			<?=campo_textarea('focdescricao', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Descrição do Curso', '100', '3', '500'); ?>
		</td>
	</tr>
	<tr class="campos" <?=($focformcontinuada != 'S' ? 'style="display:none"' : ''); ?>>
		<td align="right">
			Justificativa e diferença em relação aos cursos já apresentados
		</td>
		<td>
			<?=campo_textarea('focjustificativa', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '100', '5', '500'); ?>
		</td>
	</tr>
	<tr class="campos" <?=($focformcontinuada != 'S' ? 'style="display:none"' : ''); ?>>
		<td align="right">
			Público Alvo
		</td>
		<td>
			Função Exercida:<br>
			<?php			
			$sql = "SELECT
		                fexid as codigo,
		                fexdesc as descricao
		            FROM
		                catalogocurso.funcaoexercida
		            WHERE
		                fexstatus = 'A'
		            order by fexdesc";
			
			combo_popup('fexid', $sql, 'Função Exercida', '400x400', '',
						'', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 5, 500 , $onpop = null, $onpush = null, 
						$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
						$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
			
			?>
			<img border="0" src="../imagens/obrig.gif">
			<br><br>
			Etapa de ensino em que leciona<br>
			<?php			
			$sql = "SELECT
		                pk_cod_etapa_ensino as codigo,
		                no_etapa_ensino as descricao
		            FROM
		                educacenso_".ANO_CENSO.".tab_etapa_ensino
		            ORDER BY
		                cod_etapa_ordem";
			
			combo_popup('pk_cod_etapa_ensino', $sql, 'Etapas de ensino', '400x400', '',
						'', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 5, 500 , $onpop = null, $onpush = null, 
						$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
						$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
			
			?>
			<img border="0" src="../imagens/obrig.gif">
			<br><br>
			Disciplina em que leciona<br>
			<?php			
			$sql = "SELECT
		                pk_cod_disciplina as codigo,
		                no_disciplina as descricao
		            FROM
		                educacenso_".ANO_CENSO.".tab_disciplina
		            order by no_disciplina";
			
			combo_popup('pk_cod_disciplina', $sql, 'Disciplina', '400x400', '',
						'', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 5, 500 , $onpop = null, $onpush = null, 
						$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
						$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
			
			?>
			<img border="0" src="../imagens/obrig.gif">
			<br><br>
			Modalidade em que leciona<br>
			<?php			
			$sql = "SELECT
		                pk_cod_mod_ensino as codigo,
		                no_mod_ensino as descricao
		            FROM
		                educacenso_".ANO_CENSO.".tab_mod_ensino
		            order by no_mod_ensino";
			
			combo_popup('pk_cod_mod_ensino', $sql, 'Modalidade', '400x400', '',
						'', 'Clique aqui para selecionar um item...', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 5, 500 , $onpop = null, $onpush = null, 
						$param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 
						$funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null);
			
			?>
			<img border="0" src="../imagens/obrig.gif">
			<br><br>
		</td>
	</tr>
	<tr class="campos" <?=($focformcontinuada != 'S' ? 'style="display:none"' : ''); ?>>
		<td align="right">
			Quantidade de professores interessados
		</td>
		<td>
			<?php echo campo_texto("focqtdprof","S",($permissoes['gravar'] ? 'S' : 'N'),"Nome do Curso","15","15","[#]","","","","","id='focqtdprof'","",$arrDados['focqtdprof'])?>
		</td>
	</tr>
</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
		<td>
			<input type="button" name="anterior"   value="Anterior" onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social';">
			<?php if( $permissoes['gravar'] ){?>
			<input type="button" name="salvar" id="salvar" value="Salvar">
			<?php } ?>
			<input type="button" name="salvarCont" id="salvarCont" value="Salvar e Continuar">
			<input type="button" name="proximo"    value="Próximo"  onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_4_vizualizacao';">
		</td>
	</tr>
</table>
</form>