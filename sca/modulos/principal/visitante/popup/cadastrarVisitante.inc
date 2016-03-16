<?php


@session_start();

ini_set("memory_limit", "128M");

include APPRAIZ."sca/classes/Visitante.class.inc";
$oVisitante = new Visitante();

//pega o parâmetro que indidica se o cadastro será de visitante ou autorizado
$tipo = $_GET['tipo'];

$co_interno_uorg = $_REQUEST['co_interno_uorg'];

if($_SERVER['REQUEST_METHOD']=="GET"){

    if($_REQUEST['vttid'] && !empty($_REQUEST['vttid'])){
        $visitanteCarregado = $oVisitante->carregaVisitantePorId($_REQUEST['vttid']);
        extract($visitanteCarregado);

    }
}else if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){

        $retorno = "";

        $visitanteRepetido = $oVisitante->verificaVisitanteCadastrado($_REQUEST['vttid'], $_POST['vttdoc'], $_POST['tidid'], $_POST['ogeid']);

        if(!empty($visitanteRepetido)){
            exibeAlerta($visitanteRepetido);
            extract($_POST);
        }else{
            if($_GET['alteracao']){
                $alteracaoTipo = true;
            }else{
                $alteracaoTipo = false;
            }
            $oVisitante->salvarVisitante($_POST, $_SESSION["usucpf"], $_SESSION['imagemVisitante'], $alteracaoTipo);
        }
    }
}
// Variável $tipo recebe parametro por GET muda a label 
$tipo = $_GET['tipo'];

switch ($tipo){
    case 'A': //V = visitante
        monta_titulo('Cadastro de Autorizados','&nbsp;');        
        break;
    case 'V': //A = Autorizado
        monta_titulo('Cadastro de Visitantes', '&nbsp;');        
        break;
    default:
         $tipo = "V";
         monta_titulo('Cadastro de Visitantes', '&nbsp;');         
         break;
       
}



?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="JavaScript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language="javascript" type="text/javascript">
    function habilitaDesabilitaDocumento(){
        var campoVlrSDoc = document.getElementById('semDocumento').checked;
        var campoTipoDoc = document.getElementById('tidid');
        var campoOgEmDoc = document.getElementById('ogeid');
        var campoNumrDoc = document.formularioCadastroVisitante.elements['vttdoc'];

        campoTipoDoc.value='';
        campoOgEmDoc.value='';
        campoNumrDoc.value='';
        campoTipoDoc.disabled = (campoVlrSDoc)? true : false;
        campoOgEmDoc.disabled = (campoVlrSDoc)? true : false;
        campoNumrDoc.disabled = (campoVlrSDoc)? true : false;
        campoTipoDoc.className = (campoVlrSDoc)? 'disabled CampoEstilo' : 'CampoEstilo obrigatorio';
        campoOgEmDoc.className = (campoVlrSDoc)? 'disabled CampoEstilo' : 'CampoEstilo obrigatorio';
        campoNumrDoc.className = (campoVlrSDoc)? 'disabled CampoEstilo' : 'CampoEstilo obrigatorio';
        campoTipoDoc.readOnly = (campoVlrSDoc)? true : false;
        campoOgEmDoc.readOnly = (campoVlrSDoc)? true : false;
        campoNumrDoc.readOnly = (campoVlrSDoc)? true : false;
    }

    function gravarVisitante(){
        $('btnGravar').disable();
        if(validarVisitante()){
            document.formularioCadastroVisitante.submit();
        }
    }
    function validarVisitante(){
        var formVisitante = document.formularioCadastroVisitante;

        var campoVlrSDoc = document.getElementById('semDocumento').checked;
        if(!campoVlrSDoc){
            if($('tidid').getValue() == '') {
                alert('O campo "Tipo do Documento" é obrigatório!');
                $('tidid').focus();
                $('btnGravar').enable();
                return false;
            }
            if($('ogeid').getValue() == '') {
                alert('O campo "Órgão Emissor" é obrigatório!');
                $('ogeid').focus();
                $('btnGravar').enable();
                return false;
            }
            if($('vttdoc').getValue() == '') {
                alert('O campo "Número do Documento" é obrigatório!');
                $('vttdoc').focus();
                $('btnGravar').enable();
                return false;
            }
            var tidid = $('tidid');
            if(tidid.options[tidid.selectedIndex].innerHTML == 'CPF'){
                if(!validar_cpf($('vttdoc').getValue())){
                    alert('O CPF informado não é válido!');
                    $('vttdoc').select();
                    $('btnGravar').enable();
                    return false;
                }
            } else{
                var numeros = retornarNumerosDistintos($('vttdoc').getValue(),false);
                var letras = retornarCaracteresDistintos($('vttdoc').getValue());
                if(numeros.length == 0 || (letras.length == 0 && numeros == '0')) {
                    alert('O Número do Documento informado não é válido!');
                    $('vttdoc').focus();
                    $('btnGravar').enable();
                    return false;
                }
            }
        }
        if($('vttnome').getValue() == '') {
            alert('O campo "Nome" é obrigatório!');
            $('vttnome').focus();
            $('btnGravar').enable();
            return false;
        }
        if(!validarStringAlfa($('vttnome').getValue())) {
            alert('O Nome informado não é válido!');
            $('vttnome').focus();
            $('btnGravar').enable();
            return false;
        }
        if($('vttdddtel').getValue().length != 2 || retornarNumerosDistintos($('vttdddtel').getValue(),true).length < 1) {
            alert('O DDD informado não é válido!');
            $('vttdddtel').focus();
            $('btnGravar').enable();
            return false;
        }
        if($('vtttel').getValue().length != 9 || retornarNumerosDistintos($('vtttel').getValue(),true).length < 1) {
            alert('O Telefone informado não é válido!');
            $('vtttel').focus();
            $('btnGravar').enable();
            return false;
        }

        $('ogeid').enable();

        return true;
    }
    function limpaCampos(){
        var formEntrada = document.formularioCadastroVisitante;

        for(var i=0; i<formEntrada.elements.length;i++){
            if(formEntrada.elements[i].type != 'button' && formEntrada.elements[i].name != 'requisicao' && formEntrada.elements[i].name != 'no_vttobs'){
                formEntrada.elements[i].value = '';
                if(formEntrada.elements[i].type == 'checkbox'){
                    formEntrada.elements[i].checked = false;
                }
            }
        }
        habilitaDesabilitaDocumento();
    }
    function tratarTipoDocumento(value){
        var tidid = $('tidid');
        var ogeid = $('ogeid');
        var vttdoc = $('vttdoc');

        if(tidid.getValue() > 0){
            var text = tidid.options[tidid.selectedIndex].innerHTML;

            if(text && text == 'CPF'){
                $$('#ogeid option').each(function(d){
                    if(d.text == 'RECEITA FEDERAL'){
                        ogeid.value = d.value;
                        throw $break;
                    }
                 });
                ogeid.disable();
                vttdoc.value = formatarNumeroDocumento(vttdoc.value, 'CPF');
            }else{
                ogeid.enable();
                vttdoc.value = formatarNumeroDocumento(vttdoc.value, null);
            }
        }
    }
    function tratarNumeroDocumento(value){
        var tidid = $('tidid');

        if(tidid.getValue() > 0){
            var text = tidid.options[tidid.selectedIndex].innerHTML;

            if(text && text == 'CPF'){
                $('vttdoc').value = formatarNumeroDocumento(value, 'CPF');
                return;
            }
        }
        $('vttdoc').value = formatarNumeroDocumento(value, null);
    }
</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<form name="formularioCadastroVisitante" id="formularioCadastroVisitante" method="post" enctype="multipart/form-data">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="5" cellspacing="1">
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Tipo do Documento:</td>
                        <td width="212px"><?php $oVisitante->montaComboTipoDocumento();?></td>
                        <td>
                        <?
                        if($tipo == "A"){
                        ?>
                            <input type="checkbox" id="semDocumento" onclick="habilitaDesabilitaDocumento();"> Autorizado sem Documento
                        <?
                        }
                        else{
                        ?>
                            <input type="checkbox" id="semDocumento" onclick="habilitaDesabilitaDocumento();"> Visitante sem Documento
                        <?
                        }
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Órgão Emissor:</td>
                        <td colspan="2"><?php $oVisitante->montaComboOrgaoEmissor();?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Número do Documento:</td>
                        <td colspan="2">
                            <?=campo_texto('vttdoc','S','S','Número do Documento',21,20,'','','left','',0,'id="vttdoc"','tratarNumeroDocumento(this.value);',null,'tratarNumeroDocumento(this.value);');?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Nome:</td>
                        <td colspan="2">
                            <?=campo_texto('vttnome','S','S','Nome',56,100,'','','left','',0,'id="vttnome"');?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >DDD - Telefone:</td>
                        <td colspan="2">
                            <?=campo_texto('vttdddtel','S','S','DDD',3,2,'##','','left','',0,'id="vttdddtel"');?>
                            <?=campo_texto('vtttel','S','S','Telefone',10,9,'####-####','','left','',0,'id="vtttel"');?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Observação:</td>
                        <td colspan="2">
                            <?=campo_textarea('vttobs','N', 'S', '', 61, 6, 500,'','','','','',''); ?>
                        </td>
                    </tr>
                    
                    <?php if($_GET['alteracao'] == 'tipo'): ?>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Unidade:</td>
                        <td colspan="2">
                            
                            <?php

                                /*
                                $sql = "SELECT
                                            co_interno_uorg as codigo,
                                            no_unidade_org as descricao
                                        FROM
                                            sca.vwunidadeorganizacional";

                                */
                                $sql = "select 
                                            co_interno_uorg as codigo,
                                            (co_interno_uorg ||' - '||sg_unidade_org||' - '||no_unidade_org) as descricao
                                		from 
                                            sca.vwunidadeorganizacional
			                            order 
                                            by no_unidade_org";




                                $db->monta_combo("co_interno_uorg",$sql,'S',"Selecione...","","","","200","N","co_interno_uorg","",$co_uorg);
                            ?>
                            
                        </td>
                    </tr>
                    <?php else: ?>
                        <input type='hidden' name='co_interno_uorg' id='co_interno_uorg' value='<?=$co_interno_uorg?>'>
                    <?php endif; ?>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Terceirizado:</td>
                        <td colspan="2">
                            <?php

                                
                                if($vttterceiro == 't'){
                                    $checkedT = 'checked';
                                }

                            ?>
                            <input type="checkbox" name="vttterceiro" id="vttterceiro" class="CampoEstilo" <?php echo $checkedT; ?>> Sim
                        </td>
                    </tr>
                </table>
            </td>
            <td width="5%" valign="top">
                <div id="fotoVisitante">*<?=$oVisitante->editarFotoVisitante($vttid, $nu_matricula_siape)?></div>
            </td>
        </tr>
        <tr>
            <th colspan="2">
                <input type="hidden" name="requisicao" value="salvar" />
                <input type='hidden' name='tipo' id='tipo' value='<?=$_GET['alteracao']?>'>
                <input type="hidden" id="vttid" name="vttid" value="<?=$vttid?>"/>
                <input style="cursor: pointer;"  type="button" value="Salvar" name="btnGravar" id="btnGravar" onclick="gravarVisitante();">
                <input style="cursor: pointer;"  type="button" value="Limpar" name="btnLimpar" onclick="javascript:limpaCampos();">
            </th>
        </tr>
    </table>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("tidid").focus();
        tratarTipoDocumento('');
      });
</script>
