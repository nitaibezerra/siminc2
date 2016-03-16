<?php
include APPRAIZ."sca/classes/Equipamento.class.inc";
$oEquipamento = new Equipamento();

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Registrar Saída de Equipamento','&nbsp;');

if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){
        $oEquipamento->salvarSaidaEquipamento($_POST['vttid'], $_POST['nu_matricula_siape'], $_POST['eqmid'], $_SESSION["usucpf"], $_SESSION['imagemVisitante']);
        direcionar('?modulo=principal/equipamento/registrarSaidaEquipamento&acao=A','Operação realizada com sucesso!');
        die;
    }
}
?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
/*
    function carregarPessoa(vttid, nu_matricula_siape, nome){
        if (vttid || $('vttdoc').getValue() != ''){
            consultarVisitanteSaida(vttid, $('vttdoc').getValue(), nu_matricula_siape, 'T', null);
        } else{
            limparSaidaVisitante();
        }
    }
*/
	function carregarPessoa(vttid, nu_matricula_siape){
	
		if (vttid || nu_matricula_siape || $('vttdoc').getValue() != ''){
	
			if (vttid || nu_matricula_siape) {
				consultarVisitanteSaida(vttid, null, nu_matricula_siape, 'T',null, true);
			} else {
				consultarVisitanteSaida(null, $('vttdoc').getValue(), null, 'T', null, true);
			}
	
		} else{
	        limparEntradaVisitante('T', false);
	    }
	}

    //função que abre em popup a tela de consulta de pessoa
    function consultarPessoa(){
        AbrirPopUp('?modulo=principal/visitante/popup/consultarPessoa&acao=A&tipoConsulta=T', 'Pessoas', '\'scrollbars=yes, width=1000, height=700\'');
    }

    //submete o formulário
    function gravarSaida(){
        $('btnGravar').disable();
        if(validarSaidaVisitante()){
            document.formularioSaida.submit();
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

        if($$('input[id=eqmid]').length == 0){
            $('vttdoc').focus();
            alert('Não existe registro de entrada para nenhum equipamento!');
            $('btnGravar').enable();
            return false;
        }

        if($$('input[id=eqmid]:checked').length == 0){
            $('eqmid').focus();
            alert('Selecione ao menos um equipamento para registrar saída!');
            $('btnGravar').enable();
            return false;
        }

        return true;
    }

    //função que abre em popup a tela que busca equipamento pela etiqueta digitada
    function selecionaEquipamento(){
        AbrirPopUp('?modulo=principal/equipamento/popup/selecionarEquipamento&acao=A&tipo=S', 'Selecao', '\'scrollbars=yes, width=700, height=260\'');
    }

    function carregarPorEtiqueta(event){

        var keyCode = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;

        if(keyCode == 13){
            console.log( buscarDocumentoPorEtiqueta($('eqmnumetiqueta').getValue()) );
            
            if( buscarDocumentoPorEtiqueta($('eqmnumetiqueta').getValue())  == true)
            {
                carregarPessoa(null, null, null);
            }else{
                alert('Nenhum registro de entrada foi encontrado com o Número de Documento/Crachá informado.');
            }
                
        }

    }

</script>
<form name="formularioSaida" id="formularioSaida" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="6">Visitante / Servidor</th>
                    </tr>
                                               <!--<td class="SubTituloDireita" width="200">Número da Etiqueta:</td>
 <tr>
                            <td><? //=campo_texto('eqmnumetiqueta','','','',21,60,'','','left','',0,'id="eqmnumetiqueta" onkeyup="carregarPorEtiqueta(event)" onchange="buscarDocumentoPorEtiqueta(this.value);"');?></td>
                    </tr>
                    <tr>-->
                        <td class="SubtituloDireita" width="168px" >Número do Documento:</td>
                        <td width="138px"><?=campo_texto('vttdoc','S','S','Informe o número do documento do visitante',20,20,'','', 'left', '', 0, 'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',null,'carregarPessoa(null, null, null);');?></td>
                        
                        <td colspan="3"><input style="cursor: pointer;" type="button" value="Pesquisa Avançada" name="btnPesquisar" onclick="javascript:consultarPessoa();"></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Nome:</td>
                        <td colspan="5"><?=campo_texto('vttnome','N','N','',58,100,'','','left','',0,'id="vttnome"');?></td>
                    </tr>
                    <tr id="cargoServidorRol" style="display:none;">
                        <td class="SubtituloDireita">Cargo do Servidor:</td>
                        <td colspan="5"><?=campo_texto('vttcargo','N','N','',58,100,'','','left','',0,'id="vttcargo"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Observação:</td>
                        <td colspan="5"><?=campo_textarea('vttobs','N', 'N', '', 61, 6, '','','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <th colspan="6">Equipamento</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="170px">Equipamentos:</td>
                        <td colspan="4"><div id='lista_equipamentos'></div></td>
                        <td><img src="../imagens/busca.gif" onclick="javascript:selecionaEquipamento();"></td>
                    </tr>
                </table>
            </td>
            <td width="5%" valign="top">
                <div id="fotoVisitante">
                <?
                if(!empty($vttid) || !empty($nu_matricula_siape)){
                    echo $oVisitante->recuperarFotoVisitante($vttid, $nu_matricula_siape);
                }
                ?>
                </div>
            </td>
        </tr>
        <tr>
            <th colspan="2">
                <input style="cursor: pointer;" type="button" value="Confirmar Saída" id='btnGravar' name="btnGravar" onclick="gravarSaida();">
                <input style="cursor: pointer;" type="button" value="Limpar" name="btnLimpar" onclick="javascript:limparSaidaVisitante();">
                <input style="cursor: pointer;" type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="javascript:history.go(-1);">
            </th>
        </tr>
    </table>
    <input type="hidden" name="requisicao" value="salvar" />
    <input type="hidden" name="vttdoc" value="12" />
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
<?php
//caso tenha ocorrido algum erro na validação
if(!empty($mensagemValidacao)){
    exibeAlerta($mensagemValidacao);
}
?>