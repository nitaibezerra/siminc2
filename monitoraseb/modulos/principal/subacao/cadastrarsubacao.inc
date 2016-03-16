<?php

ini_set("memory_limit", "128M");
include_once APPRAIZ."monitoraseb/classes/Subacao.class.inc";

$sa = new SubAcao();

if ($_REQUEST['requisicao'] == 'atualizar'){
    $sa->salvar($_SESSION["monitoraseb"]["scoid"], $_SESSION["monitoraseb"]["docid"], $_POST, $_FILES);
    die;
}

if($_REQUEST['excluirAnexo']){
    $sa->excluirAnexo($_REQUEST['excluirAnexo']);
    die;
}

if($_REQUEST['abrirAnexo']){
    $sa->abrirAnexo($_REQUEST['abrirAnexo']);
    die;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo('Subação','&nbsp;');

if( $_REQUEST['scoid'] || $_SESSION["monitoraseb"]["scoid"] ){

    $_SESSION["monitoraseb"]["scoid"] = !empty($_REQUEST['scoid']) ? $_REQUEST['scoid'] : $_SESSION["monitoraseb"]["scoid"];

    $subacao = $sa->consultarSubAcao($_SESSION["monitoraseb"]["scoid"]);

    extract($subacao);

    $_SESSION["monitoraseb"]["docid"] = $docid;
}

$permissoes = $sa->validarPermissaoGravar($_SESSION["usucpf"], $scoid);

?>

<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/monitoraseb.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
    function inserirNovosArquivos() {
        var tabela = document.getElementById('inserirarquivos');
        var line = tabela.insertRow(tabela.rows.length);
        line.insertCell(0).innerHTML = replaceAll(tabela.rows[1].cells[0].innerHTML, '_0', '_' + (tabela.rows.length - 2));
        line.insertCell(1).innerHTML = replaceAll(tabela.rows[1].cells[1].innerHTML, '_0', '_' + (tabela.rows.length - 2));
        line.insertCell(2).innerHTML = replaceAll(tabela.rows[1].cells[2].innerHTML, '_0', '_' + (tabela.rows.length - 2));
        line.insertCell(3).innerHTML = replaceAll(tabela.rows[1].cells[3].innerHTML, '_0', '_' + (tabela.rows.length - 2));
    }

    function limparAnexo(id){
        var tabela = document.getElementById('inserirarquivos');
        var line = tabela.rows[parseInt(id.substring(1)) + 1];
        line.cells[1].innerHTML = line.cells[1].innerHTML;
        document.formulario.elements['arqdescricao' + id].value = '';
        document.formulario.elements['tidid' + id].value = '';
    }

    function excluirAnexo(arqid){
        if (confirm('Deseja excluir o Documento?')) {
            location.href = window.location+'&excluirAnexo='+arqid;
        }
    }

    function abrirAnexo(arqid){
        location.href = window.location+'&abrirAnexo='+arqid;
    }

    function validarFormularioPrincipal(){
    	return validarSubacao();
    }

    function gravarSubacao(){
        if(validarSubacao()){
            document.formulario.submit();
        }
    }

    function enviarFormulario(){
    	document.formulario.submit();
    }

    function validarSubacao(){

        selectAllOptions(document.getElementById('funid'));

        document.getElementById('btnGravar').disabled=true;

        if(document.formulario.elements['scpobjgeral'].value == "") {
            document.formulario.elements['scpobjgeral'].focus();
            alert('O campo "Objetivos Gerais" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['scpobjespecifico'].value == "") {
            document.formulario.elements['scpobjespecifico'].focus();
            alert('O campo "Objetivos Específicos" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['scpdsc'].value == "") {
            document.formulario.elements['scpdsc'].focus();
            alert('O campo "Descrição da Subação" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!document.formulario.elements['funid'].value) {
            document.formulario.elements['funid'].focus();
            alert('O campo "Público-Alvo" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        var i = 0;
        while (document.formulario.elements['arquivos_' + i]) {
            var arquivo = document.formulario.elements['arquivos_' + i];
            var tipo = document.formulario.elements['tidid_' + i];
            var descricao = document.formulario.elements['arqdescricao_' + i];

            if (arquivo.value || tipo.value || descricao.value != "") {
                if (!(arquivo.value && tipo.value && descricao.value != "")) {
                    if (!arquivo.value) {
                        arquivo.focus();
                        alert('O campo "Documento" é obrigatório!');
                    } else if (!tipo.value) {
                        tipo.focus();
                        alert('O campo "Tipo do Documento" é obrigatório!');
                    } else if (descricao.value == "") {
                        descricao.focus();
                        alert('O campo "Descrição do Documento" é obrigatório!');
                    }
                    document.getElementById('btnGravar').disabled=false;
                    return false;
                }
            }
            i++;
        }

        return true;
    }
</script>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" value="atualizar">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="95%" valign="top">
                <table class="listagem" width="100%">
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Coordenador Técnico:</td>
                        <td colspan="2"><? $sa->comboCoordenadorTecnico($usucpf) ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Sigla / Coordenação:</td>
                        <td colspan="2"><? $sa->comboCoordenacao($coonid) ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Subação (Programa Fantasia):</td>
                        <td colspan="2"><? $sa->comboSubacao($sbaid) ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Ação:</td>
                        <td colspan="2"><? $sa->listaAcoes($sbaid) ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Objetivos Gerais:</td>
                        <td colspan="2"><?=campo_textarea('scpobjgeral','S', (($permissoes['gravar'])?'S':'N'), '', 110, 3, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Objetivos Específicos:</td>
                        <td colspan="2"><?=campo_textarea('scpobjespecifico','S', (($permissoes['gravar'])?'S':'N'), '', 110, 3, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Descrição da Subação:</td>
                        <td colspan="2"><?=campo_textarea('scpdsc','S', (($permissoes['gravar'])?'S':'N'), '', 110, 3, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%">Público-Alvo:</td>
                        <td colspan="2">
                            <? $sa->comboPublicoAlvo($scoid, $permissoes['gravar']); ?>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2">Documentos da Subação:</td>
                        <td colspan="2">
                            <? if($permissoes['gravar']) {?>
                            <a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a>
                            <table class="listagem" width="100%" id="inserirarquivos">
                                <tr>
                                    <th width="5%">Ação:</th>
                                    <th width="32%">Documento:</th>
                                    <th width="23%">Tipo Documento:</th>
                                    <th width="45%">Descrição Documento:</th>
                                </tr>
                                <tr>
                                    <td><center>
                                        <a onclick="javascript:limparAnexo('_0');" style="cursor: pointer;">
                                            <img border="0" title="Limpar" src="/imagens/borracha.gif">
                                        </a></center>
                                    </td>
                                    <td><input type="file" name="arquivos_0"></td>
                                    <td><?php $sa->comboTipoDocumento(); ?></td>
                                    <td><?php echo campo_texto('arqdescricao_0', 'S', 'S', 'Descrição', 50, 255, '', '', '', '', 0, 'id="arqdescricao_0"' ); ?></td>
                                </tr>
                            </table>
                            <? } ?>
                        </td>
                    </tr>
                   <tr>
		             	<td colspan="2">
							<div id="arqid_on" style="display:none;">
							</div>
							<div id="arqid_off" style="color:#909090;"></div>
			      		</td>
			        </tr>
			        <script>
			        	atualizaDocumentoAnexo(null,'Subacao',<?=$scoid?>, <?=($permissoes['gravar']!=null && $permissoes['gravar'])?'"S"':'"N"'?>);
			        </script>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="3" align="center">
                            <? if($permissoes['gravar']) {?>
                            <input style="cursor: pointer;" type="button" value="Salvar" name="btnGravar" id="btnGravar" onclick="gravarSubacao();">
                            <? } ?>
                            <input style="cursor: pointer;" type="button" value="Voltar" name="Voltar" onclick="history.back(-1);">
                        </td>
                    </tr>
                </table>
            </td>
            <td width="5%" valign="top">
                <?
                $dados_wf = array('scoid' => $scoid);
                wf_desenhaBarraNavegacao($docid, $dados_wf);
                ?>
            </td>
        </tr>
    </table>
</form>
<div id="erro"></div>