<?php 
if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
	exit();
}

if( $_SESSION['catalogo']['curid'] ){
	$permissoes = testaPermissao();
} else {
	$permissoes['gravar'] = true;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

$_SESSION['catalogo']['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

$dadosEquipe = recuperaDadosEquipe($_REQUEST);

if(is_array($dadosEquipe)){
	foreach($dadosEquipe as $k => $dado){
		$$k = $dado;
	}
}

monta_abas(); ?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_equipe.js"></script>
<script type="text/javascript" language="javascript">
	$(document).ready(function() {
		$('#salvar').click(function(){
			var mim = parseInt($('#eqcminimo').val());
			var max = parseInt($('#eqcmaximo').val());

			if(jQuery('#camid').val() == ''){
				alert('O campo "Categoria de Membro de Equipe" é obrigatório!');
				jQuery('#camid').focus();
				return false;
			}			
			
			if(jQuery('[name=eqcbolsista]:checked').length <= 0){
				alert('O campo " Bolsista?" é obrigatório!');
				jQuery('[name=eqcbolsista]').focus();
				return false;
			}	 		

			<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
			if(jQuery('#eqcvalorunitario').val() == ''){
				alert('O campo "Valor Unitário da Bolsa (R$)" é obrigatório!');
				jQuery('#eqcvalorunitario').focus();
				return false;
			}	
			<?php } ?>
			if(jQuery('#eqccargahorariames').val() == ''){
				alert('O campo "Carga Horária mensal" é obrigatório!');
				jQuery('#eqccargahorariames').focus();
				return false;
			}	
			<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2015){ ?>
			if(jQuery('#eqcnumminvaga').val() == ''){
				alert('O campo "Número mínimo de vagas" é obrigatório!');
				jQuery('#eqcnumminvaga').focus();
				return false;
			}			
			<?php } ?>
			if(mim>max){
				alert('Valores inválidos.');
				$('#eqcminimo').focus();
				return false;
			}
			<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2015){ ?>
			$('input[name$="cod_escolaridade[]"]').attr('disabled',false);
			$('#eqcatribuicao').attr('disabled',false);
			<?php } ?>	
			$('#frmEquipe').submit();
		});	

		$('#camid').change(function(){
			//Se for equipe UAB
			if(!$('#gravar').val()){

				$('input[name$="curbolsista"]').attr('disabled',true);
				
				$('#qtdfuncao').attr('disabled',true);
				$('#qtdfuncao').removeClass('obrigatorio');
				
				$('#eqcfuncao').attr('disabled',true);
				$('#eqcfuncao').removeClass('obrigatorio');
				
				$('#eqcminimo').attr('disabled',true);
				$('#eqcminimo').removeClass('obrigatorio');
				
				$('#eqcmaximo').attr('disabled',true);
				$('#eqcmaximo').removeClass('obrigatorio');
				
				$('#unrid').attr('disabled',true);
				$('#unrid').removeClass('obrigatorio');

				<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
				$('input[name$="cod_escolaridade[]"]').attr('disabled',true);
				
				$('#eqcatribuicao').attr('disabled',true);
				<?php } ?>
				
				$('#eqcoutrosreq').attr('disabled',true);
			}else if($(this).val()==8){
				
				$('input[name$="curbolsista"]').attr('disabled',true);
				$('input[name$="curbolsista"]').attr('checked',false);
				
				$('#qtdfuncao').val('');
				$('#qtdfuncao').attr('disabled',true);
				$('#qtdfuncao').removeClass('obrigatorio');
				
				$('#eqcfuncao').val('');
				$('#eqcfuncao').attr('disabled',true);
				$('#eqcfuncao').removeClass('obrigatorio');
				
				$('#eqcminimo').val('');
				$('#eqcminimo').attr('disabled',true);
				$('#eqcminimo').removeClass('obrigatorio');
				
				$('#eqcmaximo').val('');
				$('#eqcmaximo').attr('disabled',true);
				$('#eqcmaximo').removeClass('obrigatorio');
				
				$('#unrid').val('');
				$('#unrid').attr('disabled',true);
				$('#unrid').removeClass('obrigatorio');

				<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
				$('input[name$="cod_escolaridade[]"]').attr('disabled',true);
				$('input[name$="cod_escolaridade[]"]').attr('checked',false);
				
				$('#eqcatribuicao').val('');
				$('#eqcatribuicao').attr('disabled',true);
				<?php } ?>
				
				$('#eqcoutrosreq').val('');
				$('#eqcoutrosreq').attr('disabled',true);
			}else{
				$('input[name$="curbolsista"]').attr('disabled',false);
				
				$('#qtdfuncao').attr('disabled',false);
				$('#qtdfuncao').addClass('obrigatorio');
				
				$('#eqcfuncao').attr('disabled',false);
				$('#eqcfuncao').addClass('obrigatorio');
				
				$('#eqcminimo').attr('disabled',false);
				$('#eqcminimo').addClass('obrigatorio');
				
				$('#eqcmaximo').attr('disabled',false);
				$('#eqcmaximo').addClass('obrigatorio');
				
				$('#unrid').attr('disabled',false);
				$('#unrid').addClass('obrigatorio');

				<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
				$('input[name$="cod_escolaridade[]"]').attr('disabled',false);
				$('#eqcatribuicao').attr('disabled',false);
				<?php }?>
				$('#eqcoutrosreq').attr('disabled',false);
			}
		});		

		<?php if($fueid){ ?>
		carregarEquipeAtribuicaoEscolaridade(<?php echo $fueid; ?>);
		<?php } ?>

		$(document).ready(function() {
			var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
		    jQuery('#top_content_label_sistema').html(exercicio);
		});		
	});
</script>

<div class="row">
    <div class="col-md-12">
        <ul class="breadcrumb">
            <li>
                <strong>Curso: </strong>
                <?php echo $_SESSION['catalogo']['curid'] .' - ' . $_SESSION['catalogo']['curdesc']; ?>
            </li>
        </ul>
    </div>
</div>

<form method="post" enctype="multipart/form-data" name="frmEquipe" id="frmEquipe">
	<input type="hidden" value="salvarEquipe" id="req" name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?php echo $permissoes['gravar']; ?>" id="gravar" name="gravar"/>
	<input type="hidden" value="<?php echo $_REQUEST['eqcid']; ?>" id="eqcid" name="eqcid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="20%">Categoria de Membro de Equipe:</td>
			<td colspan="5">
				<?php monta_combo_categoriaMembroEquipe($_POST); ?>&nbsp;
				<b>Bolsista?</b>&nbsp;
				<?php monta_radio_bolsistas($_POST); ?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ ?>
		<tr>
            <td class="SubTituloDireita">Valor Unitário da Bolsa (R$):</td>
			<td colspan="4"> 
				<?php echo campo_texto('eqcvalorunitario', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Valor Unitário da Bolsa', '16', '14', '###.###.###,##', '','left', '', '','id="eqcvalorunitario"') ?>
			</td>
		</tr>
		<?php } ?>
		<tr>
            <td class="SubTituloDireita">Parâmetros:</td>
			<td width="5%">
				Quantidade:<br>
				<?php echo campo_texto('qtdfuncao', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '5','##','','','','','id="qtdfuncao"'); ?>
			</td>
			<td width="20%">
				Função:<br>
				<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ 
					echo campo_texto('eqcfuncao', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '39', '50','','','','','','id="eqcfuncao"');
				} else { 
					monta_combo_Funcao($_POST);
				} ?>
				&nbsp; por
			</td>
			<td width="13%">
				Mínimo:<br>
				<?php echo campo_texto('eqcminimo', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '10', '10','','','','','','id="eqcminimo"'); ?>
				&nbsp; a
			</td>
			<td width="13%">
				Máximo:<br>
				<?php echo campo_texto('eqcmaximo', 'N', ($permissoes['gravar'] ? 'S' : 'N'), '', '10', '10','','','','','','id="eqcmaximo"'); ?>
			</td>
			<td>
				Unidade de Referência: <br>
				<?php monta_combo_unidadeReferencia($_POST); ?>
			</td>
		</tr>
		<tr>
            <td class="SubTituloDireita">Carga Horária mensal:</td>
			<td colspan="5"><?php echo campo_texto('eqccargahorariames', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Carga Horária por mês', '16', '9', '',  '','left', '', '','id="eqccargahorariames"'); ?></td>
		</tr>	
		<tr>
            <td class="SubTituloDireita">Nivel de Escolaridade Permitido:</td>
            <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ ?> 
			<td colspan="5">
                <?php
                $sql = "(SELECT			to_char(pk_cod_escolaridade,'9') AS codigo, 
										pk_cod_escolaridade||' - '||no_escolaridade AS descricao,
										h.nivel
				  		 FROM 			educacenso_".(ANO_CENSO).".tab_escolaridade e
				  		 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
				  		 WHERE			h.hneano = {$_SESSION['exercicio']})
				  		 UNION ALL
				  		(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo, 
										pk_pos_graduacao||'0 - '||no_pos_graduacao AS descricao,
										h.nivel
						 FROM 			educacenso_".(ANO_CENSO).".tab_pos_graduacao e
				  		 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
				  		 WHERE			h.hneano = {$_SESSION['exercicio']})";
				  		 
                $niveis = $db->carregar($sql);
                $nv = Array();
                if(is_array($niveis)){
                    foreach($niveis as $nivel){
                        if(!in_array($nivel['nivel'],$nv)){
                            array_push($nv, $nivel['nivel']);
                        }
                        $checked = '';
                        if( is_array($cod_escolaridade) ){
                            foreach($cod_escolaridade as $resp){
                                if( $resp['codigo'] == $nivel['codigo'] ){
                                    $checked = 'checked="checked"';
                                }
                            }
                        }
                        echo "<br>
								  <input type=\"checkbox\" id=\"".$nivel['nivel']."\" class=\"".implode(" ",$nv)."\" 
								  value=\"".$nivel['codigo']."\" $checked ".($permissoes['gravar'] ? '' : 'disabled="disabled"')." name=\"cod_escolaridade[]\"> ".$nivel['descricao'];
                    }
                } ?>
			</td>
			<?php } else { ?>
			<td colspan="5" id="divNivelEscolaridade">Selecione a Função.</td>
			<?php } ?>
		</tr>
		<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){ ?> 
		<tr>
			<td class="SubTituloDireita">Experiência Mínima:</td>
			<td colspan="5" id="divExperiencia">Selecione a Função.</td>
		</tr>
		<?php } ?>
		<tr>
            <td class="SubTituloDireita">Atribuição:</td>
			<td width="25%" colspan="5">
				<?php echo campo_textarea('eqcatribuicao', 'N', 'N', 'Atribuição', '100', '7', '5000','','','','','',$eqcatribuicao); ?>
			</td>
		</tr>
		<tr>
            <td class="SubTituloDireita">Outros Requisitos:</td>
            <td colspan="5">
                <?php echo campo_textarea('eqcoutrosreq', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Outros Requisitos', '100', '7', '5000','','','','','',$eqcoutrosreq); ?>
            </td>
		</tr>
		<tr>
			<td colspan="6" style="text-align: center; background-color:#E8E8E8;">
				<input type="button" id="voltar" value="Voltar"/>
				<input type="button" id="salvar" value="Salvar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
				<input type="button" id="salvarC" value="Salvar e Continuar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
				<input type="button" id="pesquisar" value="Pesquisar"/>
				<input type="button" id="proximo" value="Próximo"/>
			</td>
		</tr>
	</table>
</form>

<?php listaEquipes($_POST);?>