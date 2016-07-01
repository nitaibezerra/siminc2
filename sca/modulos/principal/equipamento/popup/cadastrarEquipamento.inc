<?php

include_once APPRAIZ."sca/classes/Equipamento.class.inc";
$oEquipamento = new Equipamento();

monta_titulo('Cadastro de Equipamentos','&nbsp;');

//caso a requisição seja de alteração
if($_SERVER['REQUEST_METHOD']=="GET"){

    if($_REQUEST['eqmid'] && !empty($_REQUEST['eqmid'])){
        $Equipamento = $oEquipamento->carregaEquipamentoPorId($_REQUEST['eqmid']);
        extract($Equipamento);
    }
//caso a requisição seja de inclusão
}else if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){

        $oEquipamento->salvarEquipamento($_POST,'popup');
        die;
    }
}
?>
<html>
<head></head>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
//submete o formulário
function gravarEquipamento(){
    $('btnGravar').disable();
    if(validarEquipamento()){
        document.formularioCadastro.submit();
    }
}

//valida os campos obrigatórios
function validarEquipamento(){

    if($('chkNovoTipo').checked && $('txtNovoTipo').getValue().match(/^\s*$/) != null){
        alert('Informe o novo "Tipo de equipamento"!');
        $('txtNovoTipo').focus();
        $('btnGravar').enable();
        return false;
    }
    if(!$('chkNovoTipo').checked && $('tpeid').getValue() == '') {
        alert('O campo "Tipo de equipamento" é obrigatório!');
        $('tpeid').focus();
        $('btnGravar').enable();
        return false;
    }
    if($('chkNovaMarca').checked && $('txtNovaMarca').getValue().match(/^\s*$/) != null){
        alert('Informe a nova "Marca de equipamento"!');
        $('txtNovaMarca').focus();
        $('btnGravar').enable();
        return false;
    }
    if(!$('chkNovaMarca').checked && $('mceid').getValue() == '') {
        alert('O campo "Marca de equipamento" é obrigatório!');
        $('mceid').focus();
        $('btnGravar').enable();
        return false;
    }
    if($('eqmnumserie').getValue().match(/^\s*$/) != null) {
        alert('O campo "Número de Série" é obrigatório!');
        $('eqmnumserie').focus();
        $('btnGravar').enable();
        return false;
    }

    return true;
}

function limpaCampos(){

    $('tpeid').setValue('');
    $('tpeid').enable();
    $('chkNovoTipo').checked = false;
    $('txtNovoTipo').disable();
    $('txtNovoTipo').setAttribute('class', 'disabled');
    $('txtNovoTipo').value = '';

    $('mceid').setValue('');
    $('mceid').enable();
    $('chkNovaMarca').checked = false;
    $('txtNovaMarca').disable();
    $('txtNovaMarca').setAttribute('class', 'disabled');
    $('txtNovaMarca').value = '';

    $('eqmnumserie').setValue('');
}

function incluirEquipamento() {
    var etiqueta = $('eqmnumetiqueta').getValue();
    executarScriptPai('buscarEquipamento(\''+etiqueta+'\',\'E\')');
}

//função que habilita/desabilita os campos de tipo e marca de equipamento
//de acordo com a marcação da checkbox correspondente
function habilitarNovoTipoMarca(onde){

    if(onde == 'tipo'){
        if($('chkNovoTipo').checked){
            $('tpeid').disable();
            $('tpeid').value = '';
            $('txtNovoTipo').enable();
            $('txtNovoTipo').setAttribute('class', 'obrigatorio normal');
            $('txtNovoTipo').focus();
        }
        else{
            $('tpeid').enable();
            $('txtNovoTipo').disable();
            $('txtNovoTipo').setAttribute('class', 'disabled');
            $('txtNovoTipo').value = '';
            $('tpeid').focus();
        }
    }
    else if(onde == 'marca'){
        if($('chkNovaMarca').checked){
            $('mceid').disable();
            $('mceid').value = '';
            $('txtNovaMarca').enable();
            $('txtNovaMarca').setAttribute('class', 'obrigatorio normal');
            $('txtNovaMarca').focus();
        }
        else{
            $('mceid').enable();
            $('txtNovaMarca').disable();
            $('txtNovaMarca').setAttribute('class', 'disabled');
            $('txtNovaMarca').value = '';
            $('mceid').focus();
        }
    }
}
</script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<body>
<form name="formularioCadastro" id="formularioCadastro" method="post">
<table  class="tabela" align="center" border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
    
    <tr style="display: none;" >
        <td align='right' class="SubTituloDireita" width="200px" height="25px">Número da Etiqueta:</td>
        <td><?=campo_texto('eqmnumetiqueta','','N','',32,13,'','','left','',0,'id="eqmnumetiqueta"');?></td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" height="25px">Tipo:</td>
        <td>
            <?=$oEquipamento->montaComboTipoEquipamento();?>
            <input type='checkbox' id='chkNovoTipo' name='chkNovoTipo' onclick="javascript:habilitarNovoTipoMarca('tipo');">
            Novo tipo?&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=campo_texto('txtNovoTipo','S','','',30,254,'','','left','',0,'id="txtNovoTipo"');?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" height="25px">Marca:</td>
        <td>
            <?=$oEquipamento->montaComboMarcaEquipamento();?>
            <input type='checkbox' id='chkNovaMarca' name='chkNovaMarca' onclick="javascript:habilitarNovoTipoMarca('marca');">
            Nova marca?&nbsp;&nbsp;<?=campo_texto('txtNovaMarca','S','','',30,60,'','','left','',0,'id="txtNovaMarca"');?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" height="25px">Número de Série:</td>
        <td><?=campo_texto('eqmnumserie','S','','',30,60,'','','left','',0,'id="eqmnumserie"');?></td>
    </tr>
    <tr bgcolor="#C0C0C0">
        <td colspan='2' style="text-align:center;">
            <input type="hidden" name="requisicao" value="salvar" />
            <input type="hidden" name="eqmid" value="<?=$_REQUEST['eqmid']?>" />
            <input type='button' class="botao" name='btnGravar' id='btnGravar' value='Salvar' onclick="javascript:gravarEquipamento();">
            <input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpaCampos();">
            <?php if(!empty($_REQUEST['eqmid'])){ ?>
            <input type='button' class="botao" name='btnFechar' id='btnFechar' value='Fechar' onclick="javascript:self.close();">
            <?php } ?>
        </td>
    </tr>
</table>
</form>
<script>
    Event.observe(window, 'load', function() {

        $("tpeid").focus();
        
        <?
            if($_REQUEST['eqmid'] && !empty($_REQUEST['eqmid']) && $_REQUEST['imprimir'] && !empty($_REQUEST['imprimir'])){
                echo "AbrirPopUp('?modulo=principal/equipamento/popup/imprimirEtiqueta&acao=A&eqmid=" . $_REQUEST['eqmid'] . "', 'imprimirEtiqueta', 'scrollbars=yes, width=50, height=50');";
                echo "incluirEquipamento();";
                echo "self.close();";
            }else if(!empty($_REQUEST['eqmid'])){
                echo "incluirEquipamento();";
                echo "self.close();";
            }
        ?>

      });

    $('txtNovoTipo').disable();
    $('txtNovoTipo').setAttribute('class', 'disabled');

    $('txtNovaMarca').disable();
    $('txtNovaMarca').setAttribute('class', 'disabled');
</script>
</body>
</html>