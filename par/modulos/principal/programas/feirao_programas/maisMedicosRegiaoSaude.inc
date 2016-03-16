<?php 
include_once '_funcoes_maismedicos.php';
$municipio = recuperarMunicipio();

if($_REQUEST['requisicao']=='pesquisar'){
	header('Content-Type: text/html; charset=iso-8859-1');
	pesquisarRegiaoSaude($_POST);
	exit();
}

if($_REQUEST['requisicao']=='salvarRegiao'){
	header('Content-Type: text/html; charset=iso-8859-1');
	salvarRegiaoMunicipio($_POST, $_FILES);
	exit();
}

if($_REQUEST['requisicao']=='alterarRegiao'){
	header('Content-Type: text/html; charset=iso-8859-1');
	alterarRegiaoMunicipio($_POST,$_FILES);
	exit();
}
?>

<br>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	function pesquisarRegiao(){
		divCarregando();		
		jQuery.ajax({
		type: 'POST',
		url: 'par.php?modulo=principal/programas/feirao_programas/maisMedicosRegiaoSaude&acao=A',
		data: jQuery('#formulario').serialize(),
		async: false,
		success: function(data) {
				jQuery("#div_regiao").html(data);
				divCarregado();
	    	}
		});
	}

	function salvarRegiao(){
        if(validarRegiao()){
        	jQuery('#formulario_regiao').submit();
        }
	}

    function validarRegiao(){
        var erro = 0;
    	if (jQuery("input[name='muncod[]']:checked").length > 0) {
    		jQuery("input[name='muncod[]']:checked").each(function(){
				var cod = jQuery(this).val();
    			if(jQuery('#pmmnumleitos'+cod).val() == ''){
					alert('Informe o Nº Leitos da Parceira');
					jQuery('#pmmnumleitos'+cod).focus();	
					erro = 1;
					return false;
				}
    		});
    	} else {
    		alert("Selecione pelo menos um Município!");
    		erro = 1;
    	}

    	if( erro == 0 ){
			return true;
    	} else {
			return false;
    	}
    }
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
    <tr>
        <td align="center" bgcolor="#DCDCDC"><b>Lista de Município Região de Saúde: <?php echo $municipio;?></b></td>
    </tr>
</table>

<?php if(!$_REQUEST['pmmid']){ ?>
<form id="formulario" method="post" name="formulario" action="#">
    <input type="hidden" id="requisicao" name="requisicao" value="pesquisar"/>
        <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
            <tr>
                <td class="subtituloDireita">Descrição:</td>
                <td><?php echo campo_texto('descricao', 'N', 'S', '', '50', '100', '', '', '', '', '', 'id="descricao"'); ?></td>
            </tr>				
            <tr>
                <td align="center" bgcolor="#CCCCCC" colspan="2">
                    <input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarRegiao();"/>
                </td>
            </tr>
    </table>
</form>
<br>
<?php } ?>

<table align="center" bgcolor="#FFFFFF" cellspacing="2" cellpadding="3" border="0" width="95%">
    <tr>
        <td align="left" colspan="2" style="color: #FF0000; font-size: 14px;">&nbsp;<b>Obs: Parceria permitida para municípios com no mínimo 50 leitos.</b></td>
    </tr>
</table>

<div id="div_regiao">
	<?php 
	$leitos = verificarLeitosDisp();
	
	if( $_REQUEST['pmmid'] ){            
            exibirRegiaoMunicipio( $_REQUEST['pmmid'] );	
	} else { 
            pesquisarRegiaoSaude(); 	
	}?>
</div>

<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
    <tr>
        <td align="center" bgcolor="#CCCCCC" colspan="2">
            <input type="submit" value="Salvar" id="btnSalvar" onclick="salvarRegiao();" style="cursor:pointer;"/>
            <input type="button" value="Fechar" id="btnFechar" onclick="window.close();"/>
        </td>
    </tr>
</table>

<script language="javascript" type="text/javascript">
	jQuery('#formulario_regiao').append('<input id="rqmid" type="hidden" name="rqmid" value="<?php echo $_SESSION['par']['rqmid'];?>" />');
	<?php if($_REQUEST['pmmid']){ ?>
		jQuery('#formulario_regiao').append('<input id="requisicao" type="hidden" name="requisicao" value="alterarRegiao" />');
		jQuery('#formulario_regiao').append('<input id="pmmid" type="hidden" name="pmmid" value="<?php echo $_REQUEST['pmmid'];?>" />');
	<?php } else { ?>
		jQuery('#formulario_regiao').append('<input id="requisicao" type="hidden" name="requisicao" value="salvarRegiao" />');
	<?php } ?>

	<?php if($leitos){ ?>
		jQuery('#btnSalvar').attr('disabled','true');	
	<?php } ?>
</script>
