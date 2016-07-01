<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ."sca/classes/Visita.class.inc";
include_once APPRAIZ."sca/classes/Visitante.class.inc";
include_once APPRAIZ."sca/classes/Equipamento.class.inc";

print '<br/>';

$oVisita = new Visita();
$oVisitante = new Visitante();
$oEquipamento = new Equipamento();

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Registrar Entrada de Equipamento','&nbsp;');

if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){

        $equipamentos = array();
        $mensagemValidacao = "";

        for($i = 0; $i < count($_POST['eqmid']); $i++){
            $id = $_POST['eqmid'][$i][0];
            if(!empty($id)){
                $equipamentos[] = $id;
                $qtd = $oEquipamento->verificaEntradaSemSaida($id);
                if($qtd >= 1){
                    $mensagemValidacao = "Não existe registro de saída para o equipamento informado.";
                    break;
                }
            }
        }

        if(!empty($mensagemValidacao)){
            extract($_POST);
        }else{
            //faz o registro de entrada dos equipamentos
            $oEquipamento->salvarEntradaEquipamento($_POST['vttid'], $_POST['nu_matricula_siape'], $equipamentos,  $_SESSION["usucpf"], $_SESSION['imagemVisitante']);
            direcionar('?modulo=principal/equipamento/registrarEntradaEquipamento&acao=A','Operação realizada com sucesso!');
            die;
        }
    }
}
?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">

/**
 * Função que retorna o codigo da pessoa, para mostrar o popup caso o registro for duplicado.
 */
	function verificarDuplicado(){
		if ($('vttdoc').getValue() != ''){
			consultarVisitanteDuplicadoEntrada($('vttdoc').getValue(), null);
        } else{
            limparEntradaVisitante('T', false);
        }
	}
	
    function carregarPessoa(vttid, nu_matricula_siape){

    	if (vttid || nu_matricula_siape || $('vttdoc').getValue() != ''){

    		if (vttid || nu_matricula_siape) {
            	consultarVisitanteEntrada(vttid, null, nu_matricula_siape, 'T');
    		} else {
    			consultarVisitanteEntrada(null, $('vttdoc').getValue(), null, 'T');
    		}

    	} else{
            limparEntradaVisitante('T', false);
        }
    }

    function carregarPessoaOnTab(){
    	if ($('vttdoc').getValue() != ''){
            consultarVisitanteEntrada(null, $('vttdoc').getValue(), null, 'V');
        } else{
            limparEntradaVisitante('V', false);
        }
    }

    //submete o formulário
    function gravarEntrada(){
        $('btnGravar').disable();
        if(validaFormulario()){
            document.formularioEntrada.submit();
        }
    }

    //valida os campos obrigatórios
    function validaFormulario(){

        selectAllOptions(document.getElementById('eqmid[]'));

        if($('vttid').getValue() == '' && $('nu_matricula_siape').getValue() == '' && $('vttdoc').getValue() == ''){
            $('vttdoc').focus();
            alert('O campo "Número do Documento" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        var found = false;
        var options = $('eqmid[]').options;
        for(var i = 0; i < options.length; i++){
            if(options[i].value != ''){
                found = true;
                break;
            }
        }

        if(!found){
            $('eqmid[]').focus();
            alert('O campo "Equipamentos" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        return true;
    }

    //função que abre popup para registro duplicado
    function popupRegistroDuplicado(tipo){
    	AbrirPopUp("?modulo=principal/visitante/popup/consultarPessoaDuplicada&acao=A&vttdoc="+$('vttdoc').getValue()+"&tipo="+tipo, "Pessoas", "scrollbars=0, width=1000, height=700");
    	//AbrirPopUp("?modulo=principal/visitante/popup/consultarPessoa&acao=A&vttdoc="+$('vttdoc').getValue()+"&tipo="+tipo, "Pessoas", "scrollbars=0, width=1000, height=700");
    }

    //função que chama a consulta de pessoa
    function consultarPessoa(){
        AbrirPopUp("?modulo=principal/visitante/popup/consultarPessoa&acao=A&tipoConsulta=T", "Pessoas", "scrollbars=0, width=1000, height=700");
    }

    //abre o cadastro de equipamento em popup
    function novoEquipamento(){
        AbrirPopUp('?modulo=principal/equipamento/popup/cadastrarEquipamento&acao=A', 'Equipamentos', '\'scrollbars=yes, width=1000, height=260\'');
    }

    //função que abre em popup a tela que busca equipamento pela etiqueta digitada
    function selecionaEquipamento(){
        AbrirPopUp('?modulo=principal/equipamento/popup/selecionarEquipamento&acao=A&tipo=E', 'Selecao', '\'scrollbars=yes, width=700, height=260\'');
    }

</script>
<form name="formularioEntrada" id="formularioEntrada" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="6">Visitante / Servidor</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px" >Número do Documento:</td>
                        <td width="137px"><?=campo_texto('vttdoc','S','S','Informe o número do documento do visitante',20,20,'','', 'left', '', 0, 'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',null,'verificarDuplicado();');?></td>
                        
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
                        <td colspan="5"><?=campo_textarea('vttobs','N', 'S', '', 61, 6, '','','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <th colspan="6">Equipamento</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="170px" >Equipamentos:</td>
                        <td colspan="5">
                            <?=$oVisita->montaComboEquipamento($eqmid);?>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                            <img src="../imagens/busca.gif" onclick="javascript:selecionaEquipamento();">
                            <script type="text/javascript">

                            // remove a opção inicial do combo
                            optionComboWorkaround();
                            
                            function optionComboWorkaround() {
	                            var opcoesCombo = document.getElementById('eqmid[]').getElementsByTagName('option');
	                            var opcaoVazia = null;
	                            for(var i = 0; i < opcoesCombo.length; i++) {
	                                var atual = opcoesCombo[i];
	
	                                if(atual.value == '') {
	                                    atual.innerHTML = "Duplo clique para a leitura de etiqueta";
	                                    return;
	                                }
	                            }
                            }


                            document.getElementById('eqmid[]').ondblclick =
                                    function(e){
                                        selecionaEquipamento();
                                        optionComboWorkaround();
                                    };

                                    // Faz o mesmo procedimento para acima o Icone ao lado do combo 
                                    var imgs = document.getElementsByTagName('img');
    								
                                    for(var i=0; i < imgs.length; i++ ){
                                		var result = imgs[i].getAttribute('src').split('/');
                                		if( result[result.length - 1] == 'pop_p.gif' ){
                                        	imgs[i].onclick = function(e){
                                        	    selecionaEquipamento();
                                        	    optionComboWorkaround();
                                                };
                                        }
                                    }

                            document.getElementById('eqmid[]').onkeypress = function(e) {
                            	optionComboWorkaround();
                            }                   
                            </script>
                            
                            <input type='button' class="botao" name='btnNovoEquipamento' id='btnNovoEquipamento' value='Novo Equipamento' onclick="javascript:novoEquipamento();">
                        </td>
                        
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
                <input style="cursor: pointer;"  type="button" value="Confirmar Entrada" id='btnGravar' name="btnGravar" onclick="gravarEntrada();">
                <input style="cursor: pointer;"  type="button" value="Limpar" name="btnLimpar" onclick="javascript:limparEntradaVisitante('T', true);">
                <input style="cursor: pointer;" type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="javascript:history.go(-1);">
            </th>
        </tr>
    </table>
    <input type="hidden" name="requisicao" value="salvar" />
    <input type='hidden' id='vstid' name='vstid' value='<?=$vstid?>'>
    <input type='hidden' id='vttid' name='vttid' value='<?=$vttid?>'>
    <input type='hidden' id='audid' name='audid' value='<?=$audid?>'>
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