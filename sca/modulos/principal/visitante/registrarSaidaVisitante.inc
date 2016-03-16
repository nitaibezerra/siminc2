<?php
include_once APPRAIZ."sca/classes/Visita.class.inc";
$oVisita = new Visita();

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Registrar Saída de Visitante','&nbsp;');

//caso seja para salvar os dados
if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){
        $oVisita->salvarSaidaVisitante($_POST, $_SESSION["usucpf"], $_SESSION['imagemVisitante']);
    }
}

?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">

    function carregarPessoa(vttid, nu_matricula_siape, nome){
        if (vttid || $('vttdoc').getValue() != ''){
            if(vttid||nu_matricula_siape){
            	consultarVisitanteSaida(vttid, null, nu_matricula_siape, 'V', null);
            } else {
            	consultarVisitanteSaida(vttid, $('vttdoc').getValue(), nu_matricula_siape, 'V', null);
            }
        } else{
            limparSaidaVisitante();
        }
    }

    function carregarPessoaOnTab(){

    	if ($('vttdoc').getValue() != ''){
    		 consultarVisitanteSaida(null, $('vttdoc').getValue(), null, 'V');
        } else{
        	 limparSaidaVisitante();
        }
    }

  //função que abre popup para registro duplicado
    function popupRegistroDuplicado(tipo){
    	AbrirPopUp("?modulo=principal/visitante/popup/consultarPessoaDuplicada&acao=A&vttdoc="+$('vttdoc').getValue()+"&tipo="+tipo+"&saida=true", "Pessoas", "scrollbars=0, width=1000, height=700");
    }

    //função que abre em popup a tela de consulta de pessoa
    function consultarPessoa(){
        AbrirPopUp('?modulo=principal/visitante/popup/consultarPessoa&acao=A&tipoConsulta=V', 'Pessoas', '\'scrollbars=yes, width=1000, height=700\'');
    }

  //função que abre em popup a tela que busca equipamento pela etiqueta digitada
    function selecionaEquipamento(){
        AbrirPopUp('?modulo=principal/equipamento/popup/selecionarEquipamento&acao=A&tipo=S', 'Selecao', '\'scrollbars=yes, width=700, height=260\'');
    }

    //submete o formulário
    function gravarVisita(){
        $('btnGravar').disable();
        if(validarSaidaVisitante()){
            document.formularioCadastroVisita.submit();
        }else{
			$('btnGravar').enable();
		}
    }

    //valida os campos obrigatórios
    function validarSaidaVisitante(){
        if($('vttid').getValue() == '' && $('nu_matricula_siape').getValue() == '' && $('vttdoc').getValue() == ''){
            $('vttdoc').focus();
            alert('O campo "Número do Documento" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        if($('edfid') && $('edfid').getValue() == ''){
            $('edfid').focus();
            alert('O campo "Edifício" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        if($('edaid') && $('edaid').getValue() == ''){
            $('edaid').focus();
            alert('O campo "Andar" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        if($('dstid') && $('dstid').getValue() == ''){
            $('dstid').focus();
            alert('O campo "Destino" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        if($('numsalforahorario') && $('numsalforahorario').getValue().trim() == ''){
            $('numsalforahorario').focus();
            alert('O campo "Sala" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        return true;
    }
</script>
<form name="formularioCadastroVisita" id="formularioCadastroVisita" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="6">Visitante</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px" >Número do Documento ou Crachá:</td>
                        <td width="137px"><?=campo_texto('vttdoc','S','S','Informe o número do documento ou crachá do visitante',20,20,'','', 'left', '', 0, 'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',null,'carregarPessoaOnTab();');?></td>
                        <td><!--img id="pesquisa" src="../imagens/consultar.gif" onclick="carregarPessoa(null, null, null);" style="cursor: pointer;" --></td>
                        <td colspan="3"><input style="cursor: pointer;" type="button" value="Pesquisa Avançada" name="btnPesquisar" onclick="javascript:consultarPessoa();"></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Nome:</td>
                        <td colspan="5"><?=campo_texto('vttnome','N','N','',58,100,'','','left','',0,'id="vttnome"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Observação:</td>
                        <td colspan="5"><?=campo_textarea('vttobs','N', 'N', '', 61, 6, '','','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <td colspan="6">
                            <div id="edificioVisitante"></div>
                        </td>
                    </tr>
                    <tr>
                        <th colspan="6">Equipamento</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="170px">Equipamentos:</td>
                        <td colspan="5"><div id='lista_equipamentos'></div></td>
                        <td><img src="../imagens/busca.gif" onclick="javascript:selecionaEquipamento();"></td>
                    </tr>
                </table>
            </td>
            <td width="5%" valign="top">
                <div id="fotoVisitante"></div>
            </td>
        </tr>
        <tr>
            <th colspan="2">
                <input style="cursor: pointer;"  type="button" value="Confirmar Saída" id="btnGravar" onclick="gravarVisita();" disabled="disabled">
                <input style="cursor: pointer;"  type="button" value="Limpar" name="btnLimpar" id='btnLimpar' onclick="javascript:limparSaidaVisitante();">
                <input style="cursor: pointer;"  type="button" value="Voltar" name="btnVoltar" id='btnVoltar' onclick="javascript:history.go(-1);">
            </th>
        </tr>
    </table>
    <input type="hidden" name="requisicao" value="salvar" />
    <input type='hidden' id='vstid' name='vstid' value='<?=$vstid?>'>
    <input type='hidden' id='vttid' name='vttid' value='<?=$vttid?>'>
    <input type='hidden' id='nu_matricula_siape' name='nu_matricula_siape' value='<?=$nu_matricula_siape?>'>
</form>
<script>
    ajustarAbas();

    Event.observe(window, 'load', function() {
        $("vttdoc").focus();
      });
</script>