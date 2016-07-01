<?php
if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
	die();
}

if( $_SESSION['catalogo']['curid'] ){
    $dadosCurso = recuperaDadosCurso(array( 'curid' => $_SESSION['catalogo']['curid']));
	$permissoes = testaPermissao();
} else {
	$permissoes['gravar'] = true;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

$_SESSION['catalogo']['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

if( $_REQUEST['orcid'] ){
	$dadosOrganizacao = recuperaOrganizacaoCurso($_REQUEST);
	if(is_array($dadosOrganizacao)){
		foreach($dadosOrganizacao as $k => $dado){
			$$k = $dado;
		}
	}
}

monta_abas(); ?>
<style>
	textarea.disabled2{
		border:none;
		border-left:#888888 3px solid;
		olor:#404040;
		width:64ex;
	}
	
	select.disabled2{
		border:none;
		border-left:#888888 3px solid;
		olor:#404040;
		width:64ex;
	}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_organizacao.js"></script>
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

<form method="post" name="frmOrganizacao" id="frmOrganizacao">
	<input type="hidden" value="" id="req"  name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?php echo $permissoes['gravar'];?>" id="gravar" name="gravar"/>
	<input type="hidden" value="<?php echo $_REQUEST['orcid']; ?>" id="orcid" name="orcid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Tipo:</td>
			<td>
				<?php 
				if($_SESSION['exercicio']==ANO_EXERCICIO_2014){
					$sql = "SELECT 	DISTINCT tioid
							FROM	catalogocurso2014.organizacaocurso
							WHERE	orcstatus = 'A' AND curid = {$_SESSION['catalogo']['curid']}";
					
					$tioid = $db->pegaUm($sql);
				
					if($tioid){
						$disabledTioid = true;
					}
				}
				monta_combo_tipo($_POST); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><?php echo campo_texto('orcdesc', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '60', '250','','', '', '', '', 'id="orcdesc"', '', ''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Carga horária:</td>
			<td><?php echo campo_texto('orcchmim', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '10', '[#]', '', '', '', '', 'id="orcchmim"','',''); ?>&nbsp;Horas.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição:</td>
			<td><?php echo campo_textarea('orcementa', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Descrição da Subdivisão', '80', '7', '5000'); ?></td>
		</tr>
		<tr>
			<td colspan="2"> 
				<center>
					<?php $trava = !( $tioid!=TO_CRITERIO_IES&&$permissoes['gravar'] ); ?>
					<input type="button" id="voltar" value="Voltar"/>
					<input type="button" id="novo" value="Novo" />
					<input type="button" id="salvar" <?php echo $trava ? "disabled" : ""; ?> value="Salvar"/>
					<input type="button" id="salvarC" <?php echo $trava ? "disabled" : ""; ?> value="Salvar e Continuar"/>
					<input type="button" id="pesquisar" value="Pesquisar"/>
					<input type="button" id="proximo" value="Próximo"/>
				</center>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" language="javascript">
	$(document).ready(function() {
		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
	    jQuery('#top_content_label_sistema').html(exercicio);

		$('#novo').click(function(){
			window.location = 'catalogocurso2014.php?modulo=principal/cadOrganizacaoCurso&acao=A';
		});
		
		$('#voltar').click(function(){
			window.location = 'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A';
		});
		
		$('#proximo').click(function(){
			window.location = 'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A';
		});
		
		$('#salvarC').click(function(){
			$('#link').val('proximo');
			$('#salvar').click();
		});
	
	    $('#salvar').click(function(){
	
	        $(this).attr('disabled',true);
	
	        var erro = false;
	        if($('#tioid').val()!=1){
	            $('.obrigatorio').each(function(){
	                if($(this).val() == ''){
	                    vazio = $(this);
	                    erro = true;
	                    return false;
	                }
	            });
	        }
	
	        if(erro){
	            alert('Campo obrigatório.');
	            vazio.focus();
	            $(this).removeAttr('disabled');
	            return false;
	        }
	
	        var orcchmim  = parseInt($('#orcchmim').val() );
	        var orcchmax  = parseInt($('#orcchmax').val() );
	
	        if( orcchmim > orcchmax ){
	            alert('Campo MÍNIMO deve ser MENOR ou IGUAL a campo MÁXIMO');
	            $('#orcchmim').focus();
	            $(this).attr('disabled',false);
	            return false;
	        }
	
	        $('#req').val('salvarOrganizacaoCurso');
	        $(this).attr('disabled',false);
	        $('#frmOrganizacao').submit();
	    });
		
		$('#pesquisar').click(function(){
			$('#req').val('');
			$('#frmOrganizacao').submit();
		});
		
		$('#tioid').change(function(){
			if($(this).val()==1){
				bloquearCamposClasse('addClass');
				bloquearCampos(true);
			}else{
				bloquearCamposClasse('removeClass');
				bloquearCampos(false);
			}
		});
		if($('#tioid').val()=='1'||$('#gravar').val()!=1){
			bloquearCamposClasse('addClass');
			bloquearCampos(true);
		}else{
			bloquearCamposClasse('removeClass');
			bloquearCampos(false);
		}
		$('#modid').change(function(){
			
			if($(this).val()==1){
				$('#preexigida').hide();
			}else{
				$('#preexigida').show();
			}
		});
		
	});

    $('#salvar').click(function(){
    	$(this).attr('disabled',true);
		if($('#tioid').val() == ''){
			alert('O campo "Tipo" é obrigatório!');
			$('#tioid').focus();
			return false;
		}	

		if($('#orcdesc').val() == ''){
			alert('O campo "Nome" é obrigatório!');
			$('#orcdesc').focus();
			return false;
		}	

		if($('#orcchmim').val() == ''){
			alert('O campo "Carga horária" é obrigatório!');
			$('#orcchmim').focus();
			return false;
		}	
 
        var orcchmim = parseInt($('#orcchmim').val() );
        var orcchmax = parseInt($('#orcchmax').val() );

        if( orcchmim > orcchmax ){
            alert('Campo MÍNIMO deve ser MENOR ou IGUAL a campo MÁXIMO');
            $('#orcchmim').focus();
            $(this).attr('disabled',false);
            return false;
        }

        <?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
        <?php if($dadosCurso && is_array($dadosCurso) && count($dadosCurso) > 0){ ?>
        if(orcchmim > <?php echo $dadosCurso['curchmim']; ?>){
            alert('A carga horária total das disciplinas / módulos supera a carga horária total do curso. Por favor, reveja os valores.');
            $('#orcchmim').focus();
            $(this).attr('disabled',false);
            return false;
        }
        <?php } } ?>

        $('#req').val('salvarOrganizacaoCurso');
        $(this).attr('disabled',false);
        $('#frmOrganizacao').submit();
    });
</script>
<?php listaOrganizacao($_POST);?>