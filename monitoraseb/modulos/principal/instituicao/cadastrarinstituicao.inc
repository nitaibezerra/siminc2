<?php

include_once APPRAIZ."monitoraseb/classes/Subacao.class.inc";
include_once APPRAIZ."monitoraseb/classes/InstituicaoHabilitada.class.inc";

$sa = new SubAcao();
$ih = new InstituicaoHabilitada();

if($_REQUEST['requisicao'] == 'carregarInstituicao'){
    $dados = $ih->consultarInstituicao($_REQUEST['entid']);

    if ($dados && $dados[0]){
        $dados[0]['entnome'] = utf8_encode($dados[0]['entnome']);
        $dados[0]['entendlog'] = utf8_encode($dados[0]['entendlog']);
        $dados[0]['entmundsc'] = utf8_encode($dados[0]['entmundsc']);
        $dados[0]['entdscsite'] = utf8_encode($dados[0]['entdscsite']);
        $dados[0]['dlcnome'] = utf8_encode($dados[0]['dlcnome']);
        $dados[0]['entnumcpfcnpj'] = formatar_cpf_cnpj($dados[0]['entnumcpfcnpj']);
        $dados[0]['dlccpf'] = formatar_cpf_cnpj($dados[0]['dlccpf']);
        $dados[0]['entendcep'] = formata_cep($dados[0]['entendcep']);

        echo simec_json_encode($dados[0]);
    }
    exit;

} elseif($_REQUEST['requisicao'] == 'salvar'){
    if($ih->salvar($_POST)){
        die;
    }else{
        extract($_POST);

        if($cmtid==null && $cmtid_disable!=null){
            $cmtid = $cmtid_disable;
        }
        if($sbaid==null && $sbaid_disable!=null){
            $sbaid = $sbaid_disable;
        }

        if($ihdid){
            $permissoes = $ih->validarPermissaoGravar($_SESSION["usucpf"], $ihdid);
        }else{
            $permissoes['gravar'] = 'S';
        }
    }
} else{

    if( $_REQUEST['ihdid']){

        $instituicao = $ih->consultarInstituicaoHabilitada($_REQUEST["ihdid"]);

        extract($instituicao);
        $ihdclccpf = formatar_cpf_cnpj($ihdclccpf);

        $permissoes = $ih->validarPermissaoGravar($_SESSION["usucpf"], $ihdid);
    }else{
        $permissoes['gravar'] = 'S';
    }
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$titulo = "Cadastrar Instituição Habilitada";
monta_titulo( $titulo, '&nbsp;' );




?>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/monitoraseb.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
    function listarSubacao(coordenacaoId){
        mostraComboSubAcao(coordenacaoId,'mostraListaAcao','','','',document.getElementById('usucpf').value);
    }

    function mostraListaAcao(sbaid){
        var cmtid = (document.getElementById('cmtid'))?document.getElementById('cmtid').value:null;
        mostraListaAcoes(sbaid, cmtid, document.getElementById('coonid').value);
    }

    function pesquisarInstituicao(){
        return windowOpen('monitoraseb.php?modulo=principal/instituicao/pesquisarinstituicao&acao=A','blank','height=400,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function carregarInstituicao(){
        var entid = document.getElementById('entid').value;

        new Ajax.Request('monitoraseb.php?modulo=principal/instituicao/cadastrarinstituicao&acao=A',
                {
                    method:     'post',
                    parameters: '&requisicao=carregarInstituicao&entid=' + entid,
                    onComplete: function (res)
                    {
                        inserir_itens(eval('(' + res.responseText + ')'));
                    }
                });
    }

    function inserir_itens(lista){
        if(lista){
            document.getElementById('entid').value = lista['entid'];
            document.getElementById('entnome').value = lista['entnome'];
            document.getElementById('entnumcpfcnpj').value = lista['entnumcpfcnpj'] ? lista['entnumcpfcnpj'] : '';
            document.getElementById('entemail').value = lista['entemail'];
            document.getElementById('entendlog').value = lista['entendlog'];
            document.getElementById('entregcod').value = lista['entregcod'];
            document.getElementById('entmundsc').value = lista['entmundsc'];
            document.getElementById('entendcep').value = lista['entendcep'] ? lista['entendcep'] : '';
            document.getElementById('entdscsite').value = lista['entdscsite'];
            document.getElementById('entnumcomercial').value = lista['entnumcomercial'];
            document.getElementById('dlcnome').value = lista['dlcnome'];
            document.getElementById('dlccpf').value = lista['dlccpf'] ? lista['dlccpf'] : '';
            document.getElementById('dlcnumcomercial').value = lista['dlcnumcomercial'];
            document.getElementById('dlcemail').value = lista['dlcemail'];
        }
    }

    function validarInstituicao(){

        document.getElementById('btnGravar').disabled=true;

        if(!document.getElementById('usucpf').value) {
            document.getElementById('usucpf').focus();
            alert('O campo "Coordenador Técnico" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!document.getElementById('coonid').value) {
            document.getElementById('coonid').focus();
            alert('O campo "Coordenação" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!document.getElementById('sbaid').value) {
            document.getElementById('sbaid').focus();
            alert('O campo "Subação" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!document.getElementById('cmtid').value) {
            document.getElementById('cmtid').focus();
            alert('O campo "Curso Mestre" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['entid'].value == "") {
            document.formulario.elements['pesquisar'].focus();
            alert('O campo "Instituição" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['ihdstatus']) {
            var radio = document.formulario.elements['ihdstatus'];
            var checked = false;
            for (var i = 0; i < radio.length; i++){
                if(radio[i].checked){
                    checked = true;
                    break;
                }
            }

            if(!checked){
                alert('O campo "Status da Instituição" é obrigatório!');
                document.getElementById('btnGravar').disabled=false;
                return false;
            }
        }

        if(document.formulario.elements['ihdjststatus'].value == "") {
            document.formulario.elements['ihdjststatus'].focus();
            alert('O campo "Justificativa" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['ihdclcnome'].value == "") {
            document.formulario.elements['ihdclcnome'].focus();
            alert('O campo "Nome do Coordenador Local" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!validar_cpf(document.formulario.elements['ihdclccpf'].value)) {
            document.formulario.elements['ihdclccpf'].focus();
            alert('O campo "CPF do Coordenador Local" não é valido!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['ihdclcfuncao'].value == "") {
            document.formulario.elements['ihdclcfuncao'].focus();
            alert('O campo "Função do Coordenador Local" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.formulario.elements['ihdclctelefone'].value == "") {
            document.formulario.elements['ihdclctelefone'].focus();
            alert('O campo "Telefone do Coordenador Local" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!validaEmail(document.formulario.elements['ihdclcemail'].value)) {
            document.formulario.elements['ihdclcemail'].focus();
            alert('O campo "E-mail do Coordenador Local" não é válido!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        return true;
    }

    function validarFormularioPrincipal(){
        return validarInstituicao();
    }

    function gravarInstituicao(){
        if(validarInstituicao()){
            document.formulario.submit();
        }
    }

    function enviarFormulario(){
        document.formulario.submit();
    }
</script>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" value="salvar">
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td>
                <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Coordenador Técnico:</td>
                        <td colspan="2"><? $sa->comboCoordenadorTecnico(($usucpf ? $usucpf : $_SESSION['usucpf']), true, ($usucpf ? $usucpf : $_SESSION['usucpf']), true) ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Sigla / Coordenação:</td>
                        <td colspan="2">
                            <div id="coonid_on" style="display:none;">
                            </div>
                            <div id="coonid_off" style="color:#909090;">Selecione um Coordenador Técnico.</div>
                            <script type="text/javascript">
                                if(document.getElementById('usucpf') != null && document.getElementById('usucpf') != ''){
                                    listar_coordenacao(document.getElementById('usucpf').value, <?=($coonid && !empty($coonid))?$coonid:'null'?> , <?=($sbaid && !empty($sbaid))?$sbaid:'null'?>, <?=($cmtid && !empty($cmtid))?$cmtid:'null'?>, '<?=$permissoes['gravar']?>');
                                }
                            </script>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Subação (Programa Fantasia):</td>
                        <td colspan="2" align="left">
                            <div id="sbaid_on" style="display:none;" align="left">
                            </div>
                            <div id="sbaid_off" style="color:#909090;">Selecione uma Coordenação.</div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Ações:</td>
                        <td colspan="2" align="left">
                            <div id="acaid_on" style="display:none;" align="left">
                            </div>
                            <div id="acaid_off" style="color:#909090;">Selecione uma Subação.</div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Curso Mestre:</td>
                        <td colspan="2" align="left">
                            <div id="cmtid_on" style="display:none;" align="left">
                            </div>
                            <div id="cmtid_off" style="color:#909090;">Selecione uma Subação.</div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="5">Instituição:</td>
                        <td colspan="2" align="left">
                            <input type="hidden" name="entid" id="entid" value="<?=$entid?>">
                            <p>Nome da Instituição</p>
                            <?=campo_texto('entnome', 'S', 'N', '', 50, 255, '', '', '', '', 0, 'id="entnome"');?>
                            <? if($permissoes['gravar'] == 'S'){ ?>
                                 <input type="button" id="pesquisar" name="pesquisar" value="Pesquisar" onclick="pesquisarInstituicao();">
                            <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <p>CNPJ</p>
                            <?=campo_texto('entnumcpfcnpj', 'N', 'N', '', 50, 150, '', '', '', '', 0, 'id="entnumcpfcnpj"');?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <p>Endereço</p>
                            <?=campo_texto('entendlog', 'N', 'N', '', 50, 150, '', '', '', '', 0, 'id="entendlog"');?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td width="122px">UF</td>
                                    <td width="350px">Município</td>
                                    <td>CEP</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('entregcod', 'N', 'N', '', 13, 2, '', '', '', '', 0, 'id="entregcod"');?></td>
                                    <td><?=campo_texto('entmundsc', 'N', 'N', '', 52, 50, '', '', '', '', 0, 'id="entmundsc"');?></td>
                                    <td><?=campo_texto('entendcep', 'N', 'N', '', 15, 9, '', '', '', '', 0, 'id="entendcep"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td width="122px">Telefone</td>
                                    <td width="350px">E-Mail</td>
                                    <td>Site</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('entnumcomercial', 'N', 'N', '', 13, 13, '', '', '', '', 0, 'id="entnumcomercial"');?></td>
                                    <td><?=campo_texto('entemail', 'N', 'N', '', 52, 100, '', '', '', '', 0, 'id="entemail"');?></td>
                                    <td><?=campo_texto('entdscsite', 'N', 'N', '', 50, 100, '', '', '', '', 0, 'id="entdscsite"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2">Dirigente Local:</td>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td width="350px">Nome</td>
                                    <td>CPF</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('dlcnome', 'N', 'N', '', 50, 255, '', '', '', '', 0, 'id="dlcnome"');?></td>
                                    <td><?=campo_texto('dlccpf', 'N', 'N', '', 20, 14, '###.###.###-##', '', '', '', 0, 'id="dlccpf"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td width="122px">Telefone</td>
                                    <td>E-Mail</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('dlcnumcomercial', 'N', 'N', '', 13, 13, '', '', '', '', 0, 'id="dlcnumcomercial"');?></td>
                                    <td><?=campo_texto('dlcemail', 'N', 'N', '', 52, 100, '', '', '', '', 0, 'id="dlcemail"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2">Status da Instituição:</td>
                        <td colspan="2" align="left">
                            <?
                                $radio = campo_radio('ihdstatus', array('Ativo'   => array('valor' => 'A',
                                                                                           'id'    => 'entstatus_a'),
                                                                        'Inativo' => array('valor' => 'I',
                                                                                           'id'    => 'entstatus_i')), 'h', true);
                                if($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2){
                                    echo $radio;
                                }else{
                                    echo str_replace('input type', 'input disabled="disabled" type', $radio);
                                }
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <p>Justificativa:</p>
                            <?=campo_textarea('ihdjststatus','S', (($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2) ? 'S' : 'N'), '', 54, 6, '','onkeyup=\'limitarTextoCampo(this, 255);\'','','','','',''); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="3">Coordenador Local do Projeto:</td>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td width="350px">Nome</td>
                                    <td>CPF</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('ihdclcnome', 'S', (($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2) ? 'S' : 'N'), '', 50, 255, '', '', '', '', 0, 'id="ihdclcnome"');?></td>
                                    <td><?=campo_texto('ihdclccpf', 'S', (($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2) ? 'S' : 'N'), '', 20, 14, '###.###.###-##', '', '', '', 0, 'id="ihdclccpf"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td>Função</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('ihdclcfuncao', 'S', (($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2) ? 'S' : 'N'), '', 50, 255, '', '', '', '', 0, 'id="ihdclcfuncao"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                <tr>
                                    <td width="122px">Telefone</td>
                                    <td>E-Mail</td>
                                </tr>
                                <tr>
                                    <td><?=campo_texto('ihdclctelefone', 'S', (($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2) ? 'S' : 'N'), '', 14, 14, '##########', '', '', '', 0, 'id="ihdclctelefone"');?></td>
                                    <td><?=campo_texto('ihdclcemail', 'S', (($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2) ? 'S' : 'N'), '', 52, 100, '', '', '', '', 0, 'id="ihdclcemail"');?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <th style="text-align: left;">
                            <? if($permissoes['gravar'] == 'S' || $permissoes['nivel'] == 2){ ?>
                                <input style="cursor: pointer;" type="button" value="Salvar" name="btnGravar" id="btnGravar" onclick="gravarInstituicao();">
                               <? } ?>
                            <input style="cursor: pointer;"  type="button" value="Cancelar" name="btnCancelar" onclick="window.location = '?modulo=principal/instituicao/consultarinstituicao&acao=A'">
                            <input type="hidden" id="docid" name="docid" value="<?=$docid?>"/>
                            <input type="hidden" id="ihdid" name="ihdid" value="<?=$ihdid?>"/>
                            <script>
                                 carregarInstituicao();
                            </script>
                        </th>
                    </tr>
                </table>
            </td>
             <td width="5%" valign="top">
                <? if($docid){
                    $dados_wf = array('ihdid' => $ihdid);
                    wf_desenhaBarraNavegacao($docid, $dados_wf);
                }
                ?>
            </td>
        </tr>
    </table>
</form>