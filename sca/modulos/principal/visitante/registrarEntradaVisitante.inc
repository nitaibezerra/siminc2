<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ."sca/classes/Visita.class.inc";
include_once APPRAIZ."sca/classes/Visitante.class.inc";
//include_once APPRAIZ."sca/classes/VisitanteEtiqueta.class.inc";
include_once APPRAIZ."sca/classes/CrachaProvisorio.class.inc";

    /* configurações */
   ini_set("memory_limit", "5000M");
   set_time_limit(0);
   /* FIM configurações */

print '<br/>';

$oVisita = new Visita();
$oVisitante = new Visitante();
//$oVisitanteEtiqueta = new VisitanteEtiqueta();

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Registrar Entrada de Visitante','&nbsp;');

$entradaAutorizado = false;

if($_SERVER['REQUEST_METHOD']=="GET"){

    if($_REQUEST['audid'] && !empty($_REQUEST['audid']) ||
       $_REQUEST['nu_matricula_siape'] && !empty($_REQUEST['nu_matricula_siape'])){

        //$cracha = new CrachaProvisorio();
        //$expediente = $cracha->verificarExpedienteTrabalho();
        $expediente = SCA_EXPEDIENTE_NORMAL;

        if($expediente == SCA_EXPEDIENTE_NORMAL){
        	$mensagemValidacao = "Não é possível realizar a entrada de um autorizado durante o horário normal de expediente.";
        } else{

            $autorizado = $oVisita->carregaAutorizado($_REQUEST['audid'], $_REQUEST['nu_matricula_siape']);
            extract($autorizado);
            $entradaAutorizado = true;
        }
    }
}//caso seja para salvar os dados
elseif($_SERVER['REQUEST_METHOD']=="POST"){

    if ($_REQUEST['requisicao'] == 'salvar'){
        
        if(empty($_POST['vttid']) && empty( $_POST['nu_matricula_siape'])){
                echo "<script>alert('Atenção! Verificar pendência no cadastro!');</script>";
        }else{
            # Valida e caso não haja erro salva os dados
            $mensagemValidacao = $oVisita->salvarEntradaVisitante($_POST, $_SESSION["usucpf"], $_SESSION['imagemVisitante'], false);

            $visita =  $oVisita->recuperaAtributos();

            $_POST['vstid'] = $visita[0]['vstid'];
            $_POST['vttid'] = empty( $_POST['vttid'] )? $visita[0]['vttid']: $_POST['vttid'];
            $_POST['vsnumcrachasistema'] = $visita[0]['vsnumcrachasistema'];
            $_POST['vstdatentrada'] = $visita[0]['vstdatentrada'];
            
            $etiquetaVisitante =  $oVisitante->geraEtiqueta( $_POST );

            if($_SESSION['usucpf']==''){
                ver('gerou a etiqueta pdf');
                ver($etiquetaVisitante);
            }

            if(!empty($mensagemValidacao)){
                extract($_POST);
            }
        }    
    }
}

?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>

<script language="javascript" type="text/javascript" >
//	Abre Popup de etiqueta
<?php 
if($_SERVER['REQUEST_METHOD']=="POST"){
    if($_REQUEST['requisicao'] == 'salvar' && $etiquetaVisitante){
        ?>
		    AbrirPopUp('?modulo=principal/visitante/popup/imprimirEtiqueta&acao=A&file=<?php echo $etiquetaVisitante ;?>', 'imprimirEtiqueta', 'scrollbars=yes, width=300, height=300');
	    <?
    }
}
?>

/**
 * Função que retorna o codigo da pessoa, para mostrar o popup caso o registro for duplicado.
 */
	function verificarDuplicado(){
		if ($('vttdoc').getValue() != ''){

			var callBackTrue = function(){
				alert('Documento registrado para um servidor, favor informar documento para um visitante.');
				limparEntradaVisitante('T', false);
			}

			var callBackFalse = function(){
				consultarVisitanteDuplicadoEntrada($('vttdoc').getValue(), 'visitante');
			}
			
			verificarPessoaServidor($('vttdoc').getValue(), callBackTrue, callBackFalse);
			
        } else{
            limparEntradaVisitante('T', false);
        }
	}
	
    function carregarPessoa(vttid, nu_matricula_siape){

    	if (vttid || nu_matricula_siape || $('vttdoc').getValue() != ''){

    		if (vttid || nu_matricula_siape) {
            	consultarVisitanteEntrada(vttid, null, nu_matricula_siape, 'V');
    		} else {
    			consultarVisitanteEntrada(null, $('vttdoc').getValue(), null, 'V');
    		}

            if( $('reimprimir').getValue() != "" ){
            		$('btnReimprimir').enable();
                }else{
                	$('btnReimprimir').disable();
                }
            	
        } else{
            limparEntradaVisitante('V', false);
        }
    }

    //função que abre popup para registro duplicado
    function popupRegistroDuplicado(tipo){
    	AbrirPopUp("?modulo=principal/visitante/popup/consultarPessoaDuplicada&acao=A&vttdoc="+$('vttdoc').getValue()+"&tipo="+tipo, "Pessoas", "scrollbars=0, width=1000, height=700");
    }
    
    //função que abre em popup a tela de consulta de pessoa
    function consultarPessoa(){
        limparEntradaVisitante('V', true);
        AbrirPopUp('?modulo=principal/visitante/popup/consultarPessoa&acao=A&tipoConsulta=V', 'consultaPessoas', 'scrollbars=yes, width=1000, height=700');
         limparEntradaVisitante('V', true);
    }

    //função que abre em popup a tela de cadastro de visitante
    function cadastrarVisitante() {
        AbrirPopUp('?modulo=principal/visitante/popup/cadastrarVisitante&acao=A', 'cadastrarVisitante', 'scrollbars=yes, width=1000, height=380');
    }

    function validarEntradaVisitante(){

        selectAllOptions(document.getElementById('eqmid[]'));

        if($('vttid').getValue() == '' && $('vttdoc').getValue() == ''){
            $('vttdoc').focus();
            alert('O campo "Número do Documento" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        /*if($('vstnumcracha') && $('vstnumcracha').getValue() == ''){
            $('vstnumcracha').focus();
            alert('O campo "Número do Crachá" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }*/
        if($('edfid').getValue() == ''){
            $('edfid').focus();
            alert('O campo "Edifício" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        if($('edaid').getValue() == ''){
            $('edaid').focus();
            alert('O campo "Andar" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        if($('dstid').getValue() == ''){
            $('dstid').focus();
            alert('O campo "Destino" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }
        if($('vstnumcracha').getValue() != ''){
        	var visitante = verificaSeVisitanteFoiRegistradoPeloNumeroEtiquetaManual($('vstnumcracha').getValue());
            if(visitante.registrado){
            	alert('Número do Crachá já informado.');
            	return !registrado;
            }
        }
        return true;
    }

    //função que submete o formulário
    function gravarVisita(){
        if(validarEntradaVisitante()){
            document.formularioCadastroVisita.submit();
            return true;
        }else{
            return false}
    }

    //abre o cadastro de equipamento em popup
    function novoEquipamento(){
        AbrirPopUp('?modulo=principal/equipamento/popup/cadastrarEquipamento&acao=A', 'Equipamentos', '\'scrollbars=yes, width=1000, height=260\'');
    }

    //função que abre em popup a tela que busca equipamento pela etiqueta digitada
    function selecionaEquipamento(){
        AbrirPopUp('?modulo=principal/equipamento/popup/selecionarEquipamento&acao=A&tipo=E', 'Selecao', '\'scrollbars=yes, width=700, height=260\'');
    }

    //função que abre em popup a tela para edição do registro de visitante
    function editarVisitante(){
        var vttid = $('vttid').getValue();
        if(vttid != ''){
            AbrirPopUp('?modulo=principal/visitante/popup/cadastrarVisitante&acao=A&vttid='+vttid, 'cadastrarVisitante', 'scrollbars=yes, width=1000, height=380');
        }
        else{
            alert('Visitante não selecionado');
        }
    }
</script>

<form name="formularioCadastroVisita" id="formularioCadastroVisita" method="post">
	<input type="hidden" name="reimprimir"  id="reimprimir" value="<?php echo  $etiquetaVisitante ?>"/>
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="6">Visitante</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px" >Número do Documento:</td>
                        <?php if(!$entradaAutorizado){?>
                        <td width="137px"><?=campo_texto('vttdoc','S','S','Informe o número do documento do visitante',20,20,'','', 'left', '', 0, 'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',null,'verificarDuplicado();');?></td>
                        <td><img id="pesquisa" src="../imagens/consultar.gif" onclick="verificarDuplicado();" style="cursor: pointer;" ></td>
                        <td><input style="cursor: pointer;" type="button" value="Pesquisa Avançada" name="btnPesquisar" onclick="javascript:consultarPessoa();"></td>
                        <td><input style="cursor: pointer;" type="button" value="Novo Visitante" name="btnNovo" onclick="cadastrarVisitante();" ></td>
                        <td><input style="cursor: pointer;" type="button" value="Editar Visitante" name="btnEditar" id="btnEditar" disabled="disabled" onclick="javascript:editarVisitante();"></td>
                        <?php } else {?>
                        <td width="137px"><?=campo_texto('vttdoc','S','N','Informe o número do documento do visitante',20,20,'','', 'left', '', 0, 'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',null,'verificarDuplicado();');?></td>
                        <td colspan="4"><img id="pesquisa" src="../imagens/consultar.gif" style="cursor: pointer;" ></td>
                        <?php } ?>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Nome:</td>
                        <td colspan="5"><?=campo_texto('vttnome','N','N','',58,100,'','','left','',0,'id="vttnome"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Cargo:</td>
                        <td colspan="5"><?=campo_texto( 'ds_cargo_emprego','N','N','',58,100,'','','left','',0,'id="ds_cargo_emprego"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Observação:</td>
                        <td colspan="5"><?=campo_textarea('vttobs','N', 'N', '', 61, 6, '','','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <th colspan="6">Visita</th>
                    </tr>
                    <?php if(!$entradaAutorizado){?>
                    <tr>
                        <td class="SubtituloDireita" width="175px">Número do Crachá:</td>
                        <td colspan="5">
                            <?=campo_texto ('vstnumcracha','N','S','Informe o número do crachá',21,6,'######','','left','',0,"id='vstnumcracha'disabled='disabled'");?>
                        </td>
                    </tr>
                    <?php } ?>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Edifício:</td>
                        <td colspan="5"><?=$oVisita->montaComboEdificio();?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px" >Andar:</td>
                        <td colspan="5"><?=$oVisita->montaComboAndarEdificio($edfid, $edaid);?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px" >Destino:</td>
                        <td colspan="5"><?=$oVisita->montaComboDestino();?></td>
                    </tr>
                    <tr>
                        <th colspan="6">Equipamento</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="170px" >Equipamentos:</td>
                        <td colspan="5">
                            <?=$oVisita->montaComboEquipamento($eqmid);?>
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
                <input style="cursor: pointer;" type="button" value="Re-impirmir Etiqueta" id="btnReimprimir" onclick="reimprimirEtiqueta();" disabled="disabled">
                <input style="cursor: pointer;" type="button" value="Confirmar Entrada" id="btnGravar" onclick="gravarVisita();">
                <input style="cursor: pointer;" type="button" value="Limpar" name="btnLimpar" onclick="javascript:limparEntradaVisitante('V', true);">
                <input style="cursor: pointer;" type="button" value="Voltar" name="btnVoltar" id='btnVoltar' onclick="javascript:history.go(-1);">
                <!-- input style="cursor: pointer;" type="button" value="Imprimir Etiqueta" name="btnImprmirEtiqueta" id='btnImprmirEtiqueta' onclick="javascript:alert('Imprimir Etiqueta');"-->
            </th>
        </tr>
    </table>
    <input type="hidden" id="requisicao" name="requisicao" value="salvar" />
    <input type='hidden' id='vstid' name='vstid' value='<?=$vstid?>'>
    <input type='hidden' id='vttid' name='vttid' value='<?=$vttid?>'>
    <input type='hidden' id='audid' name='audid' value='<?=$audid?>'>
    <input type='hidden' id='entrada' name='entrada' value='<?php echo $_SESSION['entrada']; ?>'>
    <input type='hidden' id='nu_matricula_siape' name='nu_matricula_siape' value='<?=$nu_matricula_siape?>'>
</form>
<script type="text/javascript">
    ajustarAbas();

    Event.observe(window, 'load', function() { $('vttdoc').focus(); });
      
      
    if ($('vstnumcracha').getValue() != '' && $$('#btnGravar[disable]')=='disable'){
		alert('vstnumcracha');
    }

</script>
<?php
//caso tenha ocorrido algum erro na validação
if(!empty($mensagemValidacao)){
    exibeAlerta($mensagemValidacao);
}
?>
