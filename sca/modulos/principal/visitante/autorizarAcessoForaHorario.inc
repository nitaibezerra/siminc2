<?php
// monta cabeçalho

include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ."sca/classes/Visita.class.inc";

print '<br/>';

$oVisita = new Visita();

monta_titulo('Autorizar Acesso Fora do Horário','&nbsp;');

$servidor = $oVisita->recuperarServidor(null, $_SESSION["usucpf"], true);

if($servidor){
    extract(array('no_unidade_org'     => $servidor['no_unidade_org'],
                  'co_interno_uorg'    => $servidor['co_interno_uorg'],
                  'autnomesolicitante' => $servidor['vttnome'],
    			  'no_uorg' => $servidor['no_uorg'],
    		      'co_uorg' => $servidor['co_uorg'],
                  'nu_matricula_siape' => $servidor['nu_matricula_siape']));
}


if($_SERVER['REQUEST_METHOD']=="GET"){

    if($_REQUEST['autid'] && !empty($_REQUEST['autid'])){
        $autorizacao = $oVisita->carregaAutorizacao($_REQUEST['autid']);
        extract($autorizacao);
    }

}elseif($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){

        //explode as datas para fazer verificação
        $data_atual = date('d-m-Y');
        $auxAtual = explode('-',$data_atual);
        $autdatinicio = $_POST['autdatinicio'];
        $auxInicial = explode('/',$autdatinicio);
        $autdatfinal = $_POST['autdatfinal'];
        $auxFinal = explode('/',$autdatfinal);

        $erro = false;

        //caso a data atual seja maior que a data inicial informada, mostra erro
        if(mktime(null,null,null,$auxAtual[1],$auxAtual[0],$auxAtual[2]) > mktime(null,null,null,$auxInicial[1],$auxInicial[0],$auxInicial[2])){

            $erro = true;
            $msgErro = 'A data de início informada é menor que a data corrente.';

        }//caso a data final seja menor que a data inicial informada, mostra erro
        elseif (mktime(null,null,null,$auxFinal[1],$auxFinal[0],$auxFinal[2]) < mktime(null,null,null,$auxInicial[1],$auxInicial[0],$auxInicial[2])){

            $erro = true;
            $msgErro = 'A data final informada é menor que a data início.';
        }

        //faz o registro de entrada dos equipamentos
        if(!$erro){
            $oVisita->salvarAutorizacao($_POST['autid'], $_POST['co_uorg'], $_POST['nu_matricula_siape'],
            	$_POST['autdatinicio'], $_POST['horainicio'], $_POST['autdatfinal'], $_POST['horafim'],
            	$_POST['auttelsolicitante'], $_POST['autjustificativa'], $_POST['autmemorando'], $_POST['autorizados']);
            die;
        }
        else{
            exibeAlerta($msgErro);
            extract($_POST);
        }
    }
}
else{
	$autid = 0;
    $autorizados = array();
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" >

    var popupConsultarPessoa = null;

    function carregarPessoa(vttid, nu_matricula_siape, nome){
        if (vttid || nu_matricula_siape){
            var value = (vttid ? vttid : 0) + '|' + (nu_matricula_siape ? nu_matricula_siape : 0);
            var options = $('autorizados[]').options;
            for(var i = 0; i < options.length; i++){
                if(options[i].value == value){

                    if(popupConsultarPessoa)
                        popupConsultarPessoa.exibirMensagem('Autorizado já inserido!');
                    return;
                }
            }

            var opt = document.createElement('option');
            opt.value = (vttid ? vttid : 0) + '|' + (nu_matricula_siape ? nu_matricula_siape : 0);
            opt.text = nome + (vttid ? ' (Terceirizado)' : ' (Servidor)');
             $('autorizados[]').options.add(opt);
        }
    }

    function carregarPessoaForaHorario(documento) {
		data = documento?documento:$('vttdoc').getValue();
		documento = formatarNumeroDocumento(data, null);
 
        if(documento){
            new Ajax.Request('ajax.php',{
                method: 'post',
                parameters: {'servico': 'consultarPessoaForaHorario', 'documento': documento},
                onComplete: function(transport){

                   var resposta = transport.responseText.evalJSON();
                   
                    if(resposta.descricao){
                    	 var opt = document.createElement('option');
                         opt.value = (resposta.codigovttid? resposta.codigovttid : 0) + '|' + (resposta.codigosiape ? resposta.codigosiape : 0);
                         opt.text = resposta.descricao;
                         $('autorizados[]').options.add(opt);
                    }else{
                            alert('Nenhuma pessoa foi encontrada com o Documento "' + documento + '".');
                    }
                },
                onFailure: function(){ 
                    alert('Ocorreu um erro ao buscar os dados do visitante.');
                }
            });
        }
    }

    function gravarAutorizacao(){
        $('btnAutorizar').disable();
        if(validaFormulario()){
            document.formularioAutorizarAcesso.submit();
        }
    }

    //valida os campos obrigatórios
    function validaFormulario(){

        selectAllOptions(document.getElementById('autorizados[]'));

        if($('autdatinicio').getValue() == ''){
            $('autdatinicio').focus();
            alert('O campo "Data de Início" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        if($('horainicio').getValue() == ''){
            $('horainicio').focus();
            alert('O campo "Horário de Início" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        if($('autdatfinal').getValue() == ''){
            $('autdatfinal').focus();
            alert('O campo "Data Final" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        if($('horafim').getValue() == ''){
            $('horafim').focus();
            alert('O campo "Horário Final" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        if($('auttelsolicitante').getValue() == ''){
            $('auttelsolicitante').focus();
            alert('O campo "Telefone do Solicitante" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        if($('autmemorando').getValue() == ''){
            //$('autmemorando').focus();
            //alert('O campo "Número do Memorando" é obrigatório!');
            //$('btnAutorizar').enable();
            //return false;
        }

        if($('autjustificativa').getValue() == ''){
            $('autjustificativa').focus();
            alert('O campo "Justificativa" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        var found = false;
        var options = $('autorizados[]').options;
        for(var i = 0; i < options.length; i++){
            if(options[i].value != ''){
                found = true;
                break;
            }
        }

        if(!found){
            $('autorizados[]').focus();
            alert('O campo "Autorizados" é obrigatório!');
            $('btnAutorizar').enable();
            return false;
        }

        return true;
    }

    //função que chama a consulta de pessoa
    function consultarPessoa(){
        popupConsultarPessoa = AbrirPopUp('?modulo=principal/visitante/popup/consultarPessoa&acao=A&tipoConsulta=A&pesquisar=filtrar&no_unidade_org=' + $('no_unidade_org').getValue(), 'consultaPessoas', 'scrollbars=yes, width=1000, height=700');
    }

    //função que chama a consulta de pessoa
    function consultarPessoaForaHorario(){
        AbrirPopUp('?modulo=principal/visitante/popup/consultarPessoaForaHorario&acao=A&tipoConsulta=A&pesquisar=filtrar', 'consultaPessoas', 'scrollbars=yes, width=1000, height=700');
    }

    //função que chama a tela de cadastro de visitante
    function novoVisitante(){
        AbrirPopUp('?modulo=principal/visitante/popup/cadastrarVisitante&acao=A&tipo=A&co_interno_uorg=<?php echo $co_interno_uorg; ?>', 'cadastrarVisitante', '\'scrollbars=yes, width=1000, height=380\'');
    }

    function limparAutorizacao(){
        $('autdatinicio').setValue('');
        $('horainicio').setValue('');
        $('autdatfinal').setValue('');
        $('horafim').setValue('');
        $('auttelsolicitante').setValue('');
        $('autjustificativa').setValue('');
        $('autorizados[]').options.length = 0;
        $$('input[name=no_autjustificativa]')[0].setValue(500);
        $('autdatinicio').focus();
    }


    /**
     * Efetua exclusão de opções de um elemento html do tipo select ao pressionar tecla delete.
     */
    function deleteOptionSelectAutorizado( event, elemento )
    {
    	var keynum = event.keyCode;
    	var TECLA_DELETE = 46;
    	if ( keynum == TECLA_DELETE )
    	{
        	$( elemento ).select('option').each(function(o){
        		if (o.selected){
        			o.remove();
        		}
        	});
    	}
    }
</script>
<form name="formularioAutorizarAcesso" id="formularioAutorizarAcesso" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
<?
if($servidor && $co_uorg){
?>
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="2">Autorização</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Unidade Organizacional:</td>
                        <td><?=campo_texto('no_uorg','S','N','Unidade Organizacional',76,100,'','','left','',0,'id="no_org"','',$no_uorg);?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Solicitante:</td>
                        <td ><?=campo_texto('autnomesolicitante','S','N','Solicitante',76,50,'','','left','',0,'id="autnomesolicitante"','',$autnomesolicitante);?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Data de Início:</td>
                        <td>
                            <?=campo_data2('autdatinicio','S','S','Data de Início','N','','',$autdatinicio);?>
                            <?=campo_texto('horainicio', 'N', 'S', '', 5, 5, '##:##', '', 'left', '', 0, 'id="horainicio"','','','validaHora(this);'); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Data Final:</td>
                        <td>
                            <?=campo_data2('autdatfinal','S','S','Data Final','N','','',$autdatfinal);?>
                            <?=campo_texto('horafim', 'N', 'S', '', 5, 5, '##:##', '', 'left', '', 0, 'id="horafim"','','','validaHora(this);'); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Telefone do Solicitante(de preferência celular):</td>
                        <td ><?=campo_texto('auttelsolicitante','S','S','Telefone do Solicitante',21,20,'##-####-####','','left','',0,'id="auttelsolicitante"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Número do Memorando:</td>
                        <td><?=campo_texto('autmemorando','N','S','Número do Memorando',5,6,'######','','right','',0,'id="autmemorando"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Justificativa:</td>
                        <td><?=campo_textarea('autjustificativa','S', 'S', '', 80, 6, 500,'','','','','',''); ?></td>
                    </tr>
                    <tr>
						<td class="SubTituloDireita" valign="top" width="20%">
							Autorizados:
						</td>
	                        <style type="text/css">
	                            select{width:250px!important;}
	                        </style>
							<td width="80%">
				
								<?=campo_texto('vttdoc','N','','Informe o número do documento do visitante',20,20,'','', 'left', '', 0, 'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',null,'carregarPessoaForaHorario(null);');?>
								<img id="pesquisa" src="../imagens/consultar.gif" style="cursor: pointer;", onclick="carregarPessoaForaHorario(null);" >
								&nbsp;
								<input style="cursor: pointer;" type="button" value="Pesquisa Avançada" name="btnPesquisar" onclick="consultarPessoaForaHorario();">
								
								<br /><br />
								
								<select class="combo campoEstilo" onkeypress="deleteOptionSelectAutorizado( event, 'autorizados[]' ); " size="7" name="autorizados[]" id="autorizados[]">
								</select>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" valign="top" width="20%">
							</td>
							<td width="80%">
								<input type='button' class="botao" name='btnNovoVisitante' id='btnNovoVisitante' value='Novo Autorizado' onclick="javascript:novoVisitante();">
							</td>
						</tr>
        <tr>
                </table>
            </td>
            <td width="5%" valign="top">
            </td>
        </tr>
        <tr>
            <th colspan="2">
                <input style="cursor: pointer;" type="button" value="Autorizar" name="btnAutorizar" id="btnAutorizar" onclick="gravarAutorizacao();">
                <input style="cursor: pointer;" type="button" value="Limpar" name="btnLimpar" id="btnLimpar" onclick="javascript:limparAutorizacao();">
                <input style="cursor: pointer;" type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="javascript:history.go(-1);">
            </th>
        </tr>
<?
} else{
?>
        <tr>
            <td>
            	<?php if(empty($co_uorg)): ?>
            		Esta funcionalidade está disponível apenas para Servidores com Unidade Associada.
            	<?php else: ?>
            		Esta funcionalidade está disponível apenas para Servidores.
            	<?php endif; ?>
           	</td>
        </tr>
        <tr>
            <th>
                <input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:history.go(-1);">
            </th>
        </tr>
<?
}
?>
    </table>
    <input type="hidden" name="requisicao" value="salvar" />
    <input type='hidden' name='co_uorg' id='co_interno_uorg' value='<?=$co_uorg?>'>
    <input type='hidden' name='autid' id='autid' value='<?=$autid?>'>
    <input type='hidden' name='nu_matricula_siape' id='nu_matricula_siape' value='<?=$nu_matricula_siape?>'>
</form>
<?
if($servidor){
?>
<script>
    Event.observe(window, 'load', function() {
        $("autdatinicio").focus();
      });
</script>
<?
}
?>
