<?php 
if($_REQUEST['requisicao']=='atualizarCoordenacaoResponsavel'){
	atualizarCoordenacaoResponsavel($_REQUEST);
	exit();
}

if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

if( $_SESSION['catalogo']['curid'] ){
	$permissoes = testaPermissao();
} else {
	$permissoes['gravar'] = true;
}

$dadosCurso = recuperaDadosCurso($_SESSION['catalogo']);
if(is_array($dadosCurso)){
    extract($dadosCurso);
}

$dadosContato = recuperaContato($_REQUEST);
if(is_array($dadosContato)){
	foreach($dadosContato as $k => $dado){
		$$k = $dado;
	}
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
monta_abas(); ?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_contato.js"></script>
<script language="javascript">
	function validateEmail($email) {
    	var emailReg = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
       	if( !emailReg.test( $email ) ) {
        	return false;
        } else {
            return true;
        }
	}

    $(document).ready(function() {
    	$('#voltar').click(function(){
        	window.location = 'catalogocurso2014.php?modulo=principal/cadIesOfertante&acao=A';
        });

        $('#salvar').click(function(){
        	<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2015){ ?>
    		if(jQuery('#coordid').val() == ''){
    			alert('O campo "Diretoria Responsável" é obrigatório!');
    			jQuery('#coordid').focus();
    			return false;
    		}	
    		
    		if(jQuery('#corid').val() == ''){
    			alert('O campo "Coordenação Responsável" é obrigatório!');
    			jQuery('#corid').focus();
    			return false;
    		}	  
    		<?php } else { ?>
    		if(jQuery('#coordid').val() == ''){
    			alert('O campo "Coordenação responsável no MEC ou na CAPES" é obrigatório!');
    			jQuery('#coordid').focus();
    			return false;
    		}	
    		<?php } ?>
    			
    		if(jQuery('#curconttel').val() == ''){
    			alert('O campo "Telefone Principal" é obrigatório!');
    			jQuery('#curconttel').focus();
    			return false;
    		}	  			

            var cont  = $('#curconttel').val();
            if(cont.length != '12' && cont != ''){
            	alert('O Telefone deve estar no formato ##-####-####');
                $('#curconttel').focus();
                return false;
            }

            cont  = $('#curconttel2').val();
            if(cont.length != 12 && cont != ''){
            	alert('O Telefone deve estar no formato ##-####-####');
                $('#curconttel2').focus();
                return false;
            }
                
    		if(jQuery('#curconttel').val() == ''){
    			alert('O campo "Telefone Principal" é obrigatório!');
    			jQuery('#curconttel').focus();
    			return false;
    		}	  	

    		if(jQuery('#curcontdesc').val() == ''){
    			alert('O campo "Nome" é obrigatório!');
    			jQuery('#curcontdesc').focus();
    			return false;
    		}	  

    		if(jQuery('#curcontemail').val() == ''){
    			alert('O campo "E-mail" é obrigatório!');
    			jQuery('#curcontemail').focus();
    			return false;
    		}	  						

            if(validaEmail($('#curcontemail').val()) == false){
            	alert('E-mail Incorreto!');
                $('#curcontemail').focus();
               	return false;
            }

            $('#req').val('salvarContato');
            $('#frmContato').submit();
		});

   		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
   	    jQuery('#top_content_label_sistema').html(exercicio);
	});

	function atualizarCoordenacaoResponsavel(coordid){
		jQuery.ajax({
			type: 'POST',
			url: 'catalogocurso2014.php?modulo=principal/cadContato&acao=A',
			data: {requisicao:'atualizarCoordenacaoResponsavel', coordid: coordid},
			async: false,
			success: function(data){
				jQuery('#divCoordenacaoResponsavel').html(data);
		    }
		});
	}	

	<?php if($coordid){ ?>
	jQuery(document).ready(function(){
		atualizarCoordenacaoResponsavel(<?php echo $coordid; ?>);
		$('#corid').val(<?php echo $corid; ?>);
	});
	<?php } ?>
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

<form method="post" name="frmContato" id="frmContato">
	<input type="hidden" value="" id="req" name="req"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td class="SubTituloDireita"><?php echo ($_SESSION['exercicio'] == ANO_EXERCICIO_2014) ? 'Coordenação responsável no<br> MEC ou na CAPES:' : 'Diretoria Responsável:'; ?></td>
            <td><?php monta_combo_resp($_POST); ?></td>
        </tr>
        <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){ ?>
        <tr>
        	<td class="SubTituloDireita" width="15%">Coordenação Responsável:</td>
        	<td id="divCoordenacaoResponsavel">
				<select class="CampoEstilo" id="corid" name="corid" data-placeholder="Selecione" style="width:450px;"><option>Selecione uma Diretoria Responsável...</option></select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        	</td>
        </tr>
        <?php } ?>
		<tr>
			<td class="SubTituloDireita" width="15%">Telefone Principal:</td>
			<td><?php echo campo_texto('curconttel', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '15', '30','##-####-####|####-#######', '', '', '', '', 'id="curconttel"'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone Adicional:</td>
			<td><?php echo campo_texto('curconttel2', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '15', '30','##-####-####|####-#######', '', '', '', '', 'id="curconttel2"'); ?></td>
		</tr>
		<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2014){ ?>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><?php echo campo_texto('curcontdesc', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','','','','','','id="curcontdesc"'); ?></td>
		</tr>
		<?php } ?>
		<tr>
			<td class="SubTituloDireita">E-mail:</td>
			<td><?php echo campo_texto('curcontemail', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','','' , '' , '' , '' , 'id="curcontemail"'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Site:</td>
			<td><?php echo campo_texto('curcontsite', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','',''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Outras Informações:</td>
			<td><?php echo campo_textarea('curcontinfo', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Ementa', '75', '4', '5000'); ?></td>
		</tr>
		<tr>
			<td align="center" colspan="2">
				<input type="button" id="voltar" value="Voltar"/>
				<input type="button" id="salvar" value="Salvar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
                <input  type="button" value="Proximo" onclick="javascript:window.location.href = '/catalogocurso2014/catalogocurso2014.php?modulo=principal/cadAbrangencia&acao=A'" />
			</td>
		</tr>
	</table>
</form>