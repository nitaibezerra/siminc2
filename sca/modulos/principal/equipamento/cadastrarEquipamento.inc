<?php

include APPRAIZ."sca/classes/Equipamento.class.inc";
$oEquipamento = new Equipamento();

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo('Cadastro de Equipamentos','&nbsp;');

//caso a requisição seja de alteração
if($_SERVER['REQUEST_METHOD']=="GET"){

	$permissoes = 'S';
	if($_REQUEST['eqmid'] && !empty($_REQUEST['eqmid'])){
	    //die('aqui');
		$Equipamento = $oEquipamento->carregaEquipamentoPorId($_REQUEST['eqmid']);
		extract($Equipamento);

	}
//caso a requisição seja de inclusão
}else if($_SERVER['REQUEST_METHOD']=="POST"){
	if ($_REQUEST['requisicao'] == 'salvar'){

        $oEquipamento->salvarEquipamento($_POST,'sempopup');
        die;
	}
}
?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
//submete o formulário
function gravarEquipamento(){
	$('btnGravar').disable();
	if(validaFormulario()){
		document.formularioCadastro.submit();
	}
}

//valida os campos obrigatórios
function validaFormulario(){

	if($('chkNovoTipo').checked && $('txtNovoTipo').getValue() == ''){
		alert('Informe o novo "Tipo"!');
		$('txtNovoTipo').focus();
		$('btnGravar').enable();
        return false;
	}

	if($('txtNovoTipo').getValue() != '' && !$('chkNovoTipo').checked){
		$('chkNovoTipo').checked = true;
	}

	if($('chkNovaMarca').checked && $('txtNovaMarca').getValue() == ''){
		alert('Informe a nova "Marca"!');
		$('txtNovaMarca').focus();
		$('btnGravar').enable();
        return false;
	}

	if($('txtNovaMarca').getValue() != '' && !$('chkNovaMarca').checked){
		$('chkNovaMarca').checked = true;
	}

	if($('tpeid').getValue() == '' && $('txtNovoTipo').getValue() == '') {
		alert('O campo "Tipo" é obrigatório!');
		$('tpeid').focus();
        $('btnGravar').enable();
        return false;
	}

	if($('mceid').getValue() == '' && $('txtNovaMarca').getValue() == '') {
		alert('O campo "Marca" é obrigatório!');
		$('mceid').focus();
		$('btnGravar').enable();
        return false;
	}

	if($('eqmnumserie').getValue() == '') {
		alert('O campo "Número de Série" é obrigatório!');
		$('eqmnumserie').focus();
		$('btnGravar').enable();
        return false;
	}

	return true;

}

//função que limpa os campos do form
function limpaCampos(){
	var formEquipamento = document.formularioCadastro;

	for(var i=0; i<formEquipamento.elements.length;i++){
		if(formEquipamento.elements[i].type != 'button' && formEquipamento.elements[i].name != 'requisicao'){
		   formEquipamento.elements[i].value = '';
		   if(formEquipamento.elements[i].type == 'checkbox'){
			   formEquipamento.elements[i].checked = false;
		   }
		}
	}
	// limpa foto
	limparFoto();
}


//função que habilita/desabilita os campos de tipo e marca de equipamento
//de acordo com a marcação da checkbox correspondente
function verificaCampos(onde){

	if(onde == 'tipo'){
		var chk = $('chkNovoTipo').checked;
		if(chk){
			$('tpeid').disable();
			$('tpeid').value = '';
			$('txtNovoTipo').enable();
			$('txtNovoTipo').setAttribute('class', 'obrigatorio normal');
		}
		else if(!chk){
			$('tpeid').enable();
			$('txtNovoTipo').disable();
			$('txtNovoTipo').setAttribute('class', 'disabled');
			$('txtNovoTipo').value = '';
		}
	}
	else if(onde == 'marca'){
		var chk = $('chkNovaMarca').checked;
		if(chk){
			$('mceid').disable();
			$('mceid').value = '';
			$('txtNovaMarca').enable();
			$('txtNovaMarca').setAttribute('class', 'obrigatorio normal');
		}
		else if(!chk){
			$('mceid').enable();
			$('txtNovaMarca').disable();
			$('txtNovaMarca').setAttribute('class', 'disabled');
			$('txtNovaMarca').value = '';
		}
	}
}

</script>



<form name="formularioCadastro" id="formularioCadastro" method="post">
<table  class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
  <tr>
    <td align='right' class="SubTituloDireita" width='25%'>Número da Etiqueta:</td>
    <td><?=campo_texto('eqmnumetiqueta','','N','',30,13,'','');?></td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita" width='25%'>Tipo:</td>
    <td><?php $oEquipamento->montaComboTipoEquipamento();?>
    <input type='checkbox' id='chkNovoTipo' name='chkNovoTipo' onclick="javascript:verificaCampos('tipo');">
    Novo tipo?&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=campo_texto('txtNovoTipo','S','','',30,254,'','','left','',0,'id="txtNovoTipo"');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita" width='25%'>Marca:</td>
    <td><?php $oEquipamento->montaComboMarcaEquipamento();?>
    <input type='checkbox' id='chkNovaMarca' name='chkNovaMarca' onclick="javascript:verificaCampos('marca');">
    Nova marca?&nbsp;&nbsp;<?=campo_texto('txtNovaMarca','S','','',30,60,'','','left','',0,'id="txtNovaMarca"');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita" width='25%'>Número de Série:</td>
    <td><?=campo_texto('eqmnumserie','S','','',30,60,'','','left','',0,'id="eqmnumserie"');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita" width='25%'>Status:</td>
    <td><?php echo $statussaida;?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita" width='25%'>Responsável:</td>
    <td><?php echo ($nomeresponsavel ? $nomeresponsavel : ' - ')?></td>
  </tr>

  <tr bgcolor="#C0C0C0">
    <td colspan='2' style="text-align:center;">
    <input type="hidden" name="requisicao" value="salvar" />
    <input type="hidden" name="eqmid" value="<?=$_REQUEST['eqmid']?>" />
    <input type='button' class="botao" name='btnGravar' id='btnGravar' value='Salvar' onclick="javascript:gravarEquipamento();">
    <input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpaCampos();">
    <?php if(!empty($_REQUEST['eqmid'])){ ?>
    <input type='button' class="botao" name='btnImprimir' id='btnImprimir' value='Imprimir Etiqueta' onclick="AbrirPopUp('?modulo=principal/equipamento/popup/imprimirEtiqueta&acao=A&eqmid=<?=$_REQUEST['eqmid']?>', 'imprimirEtiqueta', 'scrollbars=yes, width=50, height=50');">
    <?php } ?>
    <input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:window.location='sca.php?modulo=principal/equipamento/consultarEquipamento&acao=A';">
    </td>
  </tr>
</table>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("tpeid").focus();
      });

    $('txtNovoTipo').disable();
    $('txtNovoTipo').setAttribute('class', 'disabled');

    $('txtNovaMarca').disable();
    $('txtNovaMarca').setAttribute('class', 'disabled');
</script>