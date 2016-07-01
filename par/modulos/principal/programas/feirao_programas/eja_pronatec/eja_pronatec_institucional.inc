<?php
    include_once '_funcoes_eja_pronatec.php';
    
    if( !$_SESSION['par']['inuid'] ){
    	echo "
            <script>
                alert('Sessão expirada.');
                window.location.href = 'par.php?modulo=inicio&acao=C';
            </script>";
    	die();
    }
    
    if( !$_SESSION['par']['pfaid'] ){
    	echo "
            <script>
                alert('Sessão expirada.');
                window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
            </script>";
    	die();
    }

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if( $_SESSION['par']['adpid'] == ''){
        $_SESSION['par']['adpid'] = $db->pegaUm( "SELECT MAX(adpid) FROM par.pfadesaoprograma WHERE pfaid = {$_SESSION['par']['pfaid']} AND inuid = {$_SESSION['par']['inuid']}" );
    }

    if( $_SESSION['par']['adpid'] == '' || $_SESSION['par']['prgid'] == '' ){
        $db->sucesso( 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';
    $sql = " SELECT prgdsc FROM par.programa WHERE prgid = ".$_SESSION['par']['prgid'];
    $nomePrograma = $db->pegaUm($sql);

    #PEGA, AVERIGUA O MUNICÍPIO OU ESTADO SELECIONADO OU LOGADO PELO USUÁRIO. É USADO PARA CRIAR O TITULO.
    if($_SESSION['par']['muncod']){
        $sql = "SELECT mundescricao, estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
        $rsMunicipio = $db->pegaLinha($sql);
        $descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
    }else{
        $sql = "SELECT estdescricao, estuf FROM territorios.estado WHERE estuf = '{$_SESSION['par']['estuf']}'";
        $rsEstado = $db->pegaLinha($sql);
        $descricao = $rsEstado['estdescricao'].' - '.$rsEstado['estuf'];
    }

    monta_titulo($nomePrograma." - Cadastro Institucional", $descricao);
    echo '<br>';

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_institucional&acao=A');
    
    $dadosInstitucional = buscaDadosInstitucional ( $_SESSION['par']['adpid'] );
    
    
    if( $_SESSION['par']['muncod'] != '' ){
        #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
        $textoSecretaria = 'Prefeitura Municipal';
        $obrig = 'N';

        $texto = '
            <span style="font-size: 18px;"><b>AVISO:</b></span>
            <p style="text-align: justify; font-size: 13px;">Deve?a, obrigatoriamente, ser servidor da Prefeitura Municipal de Educação.</p>
        ';
    }else{
        #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
        $textoSecretaria = 'Secretaria Estadual';
        $obrig = 'S';
        
        $texto = '
            <span style="font-size: 18px;"><b>AVISO:</b></span>
            <p style="text-align: justify; font-size: 13px;">Deve?a, obrigatoriamente, ser servidor da Secretaria Estadual de Educação.</p>
        ';
    }
    
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">
    
    function atualizaComboMunicipio( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipio&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }
    
    function buscarEndereceCEP( cep ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarEndereceCEP&cep="+cep,
            success: function(resp){
                var dados = $.parseJSON(resp);
                
                $('#epilogradouro').val(dados.logradouro);
                $('#epibairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function salvarEJA_Institucional(){
        //var epidsc              = $('#epidsc');
        var epicnpj             = $('#epicnpj');
        var epitelefonenumero   = $('#epitelefonenumero');
        var epiemail            = $('#epiemail');
        var epicep              = $('#epicep');
        var epilogradouro       = $('#epilogradouro');
        var epicomplemento      = $('#epicomplemento');
        var epibairro           = $('#epibairro');
        var estuf               = $('#estuf');
        //var muncod              = $('#muncod');

        var erro;
        /*
        if(!epidsc.val()){
            alert('O campo "Secretaria Estadual de Educação" é um campo obrigatório!');
            epidsc.focus();
            erro = 1;
            return false;
        }
        */
        if(!epicnpj.val()){
            alert('O campo "CNPJ" é um campo obrigatório!');
            epicnpj.focus();
            erro = 1;
            return false;
        }
        if(!epitelefonenumero.val()){
            alert('O campo "Telefone DDD/Número" é um campo obrigatório!');
            epitelefonenumero.focus();
            erro = 1;
            return false;
        }
        if(!epiemail.val()){
            alert('O campo "E-mail" é um campo obrigatório!');
            epiemail.focus();
            erro = 1;
            return false;
        }
        if(!epicep.val()){
            alert('O campo "CEP" é um campo obrigatório!');
            epicep.focus();
            erro = 1;
            return false;
        }
        if(!epilogradouro.val()){
            alert('O campo "Logradouro" é um campo obrigatório!');
            epilogradouro.focus();
            erro = 1;
            return false;
        }
        if(!epicomplemento.val()){
            alert('O campo "Complemento" é um campo obrigatório!');
            epicomplemento.focus();
            erro = 1;
            return false;
        }
        if(!epibairro.val()){
            alert('O campo "Bairro" é um campo obrigatório!');
            epibairro.focus();
            erro = 1;
            return false;
        }
        if(!estuf.val()){
            alert('O campo "UF" é um campo obrigatório!');
            estuf.focus();
            erro = 1;
            return false;
        }
        /*
        if(!muncod.val()){
            alert('O campo "Município" é um campo obrigatório!');
            muncod.focus();
            erro = 1;
            return false;
        }
        */
        if(!erro){
            $('#requisicao').val('salvarEJA_Institucional');
            $('#formulario').submit();
        }
    }
    
    function verificarCPFValido( cnpj ){
        if(cnpj != ''){
            var valido = validarCnpj( cnpj );
        
            if(!valido){
                alert( "CNPJ inválido! Favor informar um CNPJ válido!" );
                $('#epicnpj').focus();
                return false;
            }
        }
        return true;
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="epiid" name="epiid" value="<?= $dadosInstitucional['epiid']; ?>"/>
    
    <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class ="SubTituloDireita" width="30%" > <?=$textoSecretaria;?>: </td>
            <td width="65%">
               <?php
                    $epidsc = $dadosInstitucional['epidsc'];
                    echo campo_texto('epidsc', $obrig, $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epidsc"', '', $epidsc, null, '', null);
                ?> 
            </td>
            <td rowspan="12" width="5%" style="margin-right: 10px; vertical-align: central;">
                <?php
                    include_once APPRAIZ . "includes/workflow.php";

                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> CNPJ: </td>
            <td>
               <?php
                    $epicnpj = $dadosInstitucional['epicnpj']; //03.847.655/0001-98
                    echo campo_texto('epicnpj', 'S', $habilitado['TEXT'], '', 14, 19, '##.###.###/####-##', '', 'right', '', 0, 'id="epicnpj"', '', $epicnpj, 'verificarCPFValido(this.value)', '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone DDD/Número: </td>
            <td>
               <?php
                    $epitelefonenumero = $dadosInstitucional['epitelefonenumero'];
                    echo campo_texto('epitelefonenumero', 'S', $habilitado['TEXT'], '', 14, 14, '##-####-####', '', 'right', '', 0, 'id="epitelefonenumero"', '', $epitelefonenumero, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> E-mail: </td>
            <td>
               <?php
                    $epiemail = $dadosInstitucional['epiemail'];
                    echo campo_texto('epiemail', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epiemail"', '', $epiemail, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td colspan="2" class ="SubTituloCentro"> Endereço </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> CEP: </td>
            <td>
               <?php
                    $epicep = $dadosInstitucional['epicep'];
                    echo campo_texto('epicep', 'S', $habilitado['TEXT'], '', 14, 10, '#####-###', '', 'left', '', 0, 'id="epicep"', '', $epicep, 'buscarEndereceCEP(this.value)', '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Logradouro: </td>
            <td>
               <?php
                    $epilogradouro = $dadosInstitucional['epilogradouro'];
                    echo campo_texto('epilogradouro', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epilogradouro"', ';', $epilogradouro, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Complemento: </td>
            <td>
               <?php
                    $epicomplemento = $dadosInstitucional['epicomplemento'];
                    echo campo_texto('epicomplemento', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epicomplemento"', ';', $epicomplemento, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Bairro: </td>
            <td>
               <?php
                    $epibairro = $dadosInstitucional['epibairro'];
                    echo campo_texto('epibairro', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epibairro"', ';', $epibairro, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td>
                <?php
                    $estuf = $dadosInstitucional['estuf'];
                    $sql = "
                         SELECT  estuf as codigo, 
                                 estuf as descricao 
                         FROM territorios.estado 
                         ORDER BY estuf
                     ";
                     $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'atualizaComboMunicipio', '', '', 250, 'N', 'estuf', '', $estuf);
                ?> 
            </td>
        </tr>
        <?PHP 
            if( $_SESSION['par']['muncod'] != "" ){
        ?>
            <tr>
                <td class ="SubTituloDireita"> Município: </td>
                <td id="td_combo_municipio">
                    <?PHP
                        $muncod = $dadosInstitucional['muncod'];
                        $sql = "
                            SELECT  muncod AS codigo,
                                    mundescricao AS descricao
                            FROM territorios.municipio
                            ORDER BY descricao
                        ";
                         $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 250, 'N', 'muncod', '', $muncod);
                    ?> 
                </td>
            </tr>
            
        <?PHP } ?>
            
    </table>
    <br>
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="salvar" value="Salvar" onclick="salvarEJA_Institucional();"/>
                <input type="button" name="cancelar" value="Cancelar" onclick=""/>
            </td>
        </tr>
    </table>
</form>