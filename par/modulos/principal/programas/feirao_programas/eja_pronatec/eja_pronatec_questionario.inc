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

    monta_titulo($nomePrograma." - Estimativa de Matrículas", $descricao);
    echo '<br>';

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_questionario&acao=A');

    $dadosQuestPronatec = buscaDadosQuestPronatec ( $_SESSION['par']['inuid'] );

    if($dadosQuestPronatec['qepejatecnicointegrado'] == 't'){
        $check_int = 'checked = "checked"';
    }else{
        $check_int = '';
    }

    if($dadosQuestPronatec['qepejatecnicoconcomitante'] == 't'){
        $check_con = 'checked = "checked"';
    }else{
        $check_con = '';
    }

    if($dadosQuestPronatec['qepejaficmedio'] == 't'){
        $check_med = 'checked = "checked"';
    }else{
        $check_med = '';
    }

    if($dadosQuestPronatec['qepejaficfundamental'] == 't'){
        $check_fun = 'checked = "checked"';
    }else{
        $check_fun = '';
    }

    $existeAlfabetizado = verificaRegrasAlfabetizado();
    $existeIndigina     = verificaRegrasIndiginas();

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">

    function salvarQuestEJA_PRONATEC(){
        var erro;
        var inputs = $("#formulario").find('INPUT[class*="obrigatorio normal"]');

        $.each(inputs, function(i, v) {
            if ( $(v).val() == '' ){
                erro = 1;
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! É obrigatório digitar um valor mesmo que 0.');
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarQuestEJA_PRONATEC');
            $('#formulario').submit();
        }
    }

    function habilitaCamposExterna( obj ){
        var hab     = obj.value;
        var name_obj= obj.name;
        if(hab == 'S'){
            $('#'+name_obj+'qtd').removeAttr('disabled');
            $('#'+name_obj+'qtd').removeAttr('readonly');
            $('#'+name_obj+'qtd').removeClass('disabled');
            $('#'+name_obj+'qtd').addClass('obrigatorio normal');
        }else{
            $('#'+name_obj+'qtd').removeAttr();
            $('#'+name_obj+'qtd').val('');
            $('#'+name_obj+'qtd').removeClass('obrigatorio normal');
            $('#'+name_obj+'qtd').addClass('disabled');
        }
    }

    function somarTotalGeralFundamental(){

        var qeppnpensinofund        = parseInt($('#qeppnpensinofund').val()) ? parseInt($('#qeppnpensinofund').val()) : 0;
        var qepppbraalfensfund      = parseInt($('#qepppbraalfensfund').val()) ? parseInt($('#qepppbraalfensfund').val()) : 0;
        var qeppppopcampoensfund    = parseInt($('#qeppppopcampoensfund').val()) ? parseInt($('#qeppppopcampoensfund').val()) : 0;
        var qepppquilombolaensfund  = parseInt($('#qepppquilombolaensfund').val()) ? parseInt($('#qepppquilombolaensfund').val()) : 0;
        var qepppindigenasensfund   = parseInt($('#qepppindigenasensfund').val()) ? parseInt($('#qepppindigenasensfund').val()) : 0;
        var qepppprovliberensfund   = parseInt($('#qepppprovliberensfund').val()) ? parseInt($('#qepppprovliberensfund').val()) : 0;
        var qepppmedsocioensfund    = parseInt($('#qepppmedsocioensfund').val()) ? parseInt($('#qepppmedsocioensfund').val()) : 0;
        var qepppmatreciclensfund   = parseInt($('#qepppmatreciclensfund').val()) ? parseInt($('#qepppmatreciclensfund').val()) : 0;
        var qepppsitruaensfund      = parseInt($('#qepppsitruaensfund').val()) ? parseInt($('#qepppsitruaensfund').val()) : 0;
        var qeppppescadoresensfund  = parseInt($('#qeppppescadoresensfund').val()) ? parseInt($('#qeppppescadoresensfund').val()) : 0;

        var total;

        total = (qeppnpensinofund+qepppbraalfensfund+qeppppopcampoensfund+qepppquilombolaensfund+qepppindigenasensfund+qepppprovliberensfund+qepppmedsocioensfund+qepppmatreciclensfund+qepppsitruaensfund+qeppppescadoresensfund);

        $('#total_geral_fundamental').val(total);
    }


    function somarTotalGeralMedio(){

        var qepppbraalfensmedio      = parseInt($('#qepppbraalfensmedio').val()) ? parseInt($('#qepppbraalfensmedio').val()) : 0;
        var qeppppopcampoensmedio    = parseInt($('#qeppppopcampoensmedio').val()) ? parseInt($('#qeppppopcampoensmedio').val()) : 0;
        var qepppquilombolaensmedio  = parseInt($('#qepppquilombolaensmedio').val()) ? parseInt($('#qepppquilombolaensmedio').val()) : 0;
        var qepppindigenasensmedio   = parseInt($('#qepppindigenasensmedio').val()) ? parseInt($('#qepppindigenasensmedio').val()) : 0;
        var qepppprovliberensmedio   = parseInt($('#qepppprovliberensmedio').val()) ? parseInt($('#qepppprovliberensmedio').val()) : 0;
        var qepppmedsocioensmedio    = parseInt($('#qepppmedsocioensmedio').val()) ? parseInt($('#qepppmedsocioensmedio').val()) : 0;
        var qepppmatreciclensmedio   = parseInt($('#qepppmatreciclensmedio').val()) ? parseInt($('#qepppmatreciclensmedio').val()) : 0;
        var qepppsitruaensmedio      = parseInt($('#qepppsitruaensmedio').val()) ? parseInt($('#qepppsitruaensmedio').val()) : 0;
        var qeppppescadoresensmedio  = parseInt($('#qeppppescadoresensmedio').val()) ? parseInt($('#qeppppescadoresensmedio').val()) : 0;

        var total;

        total = (qepppbraalfensmedio+qeppppopcampoensmedio+qepppquilombolaensmedio+qepppindigenasensmedio+qepppprovliberensmedio+qepppmedsocioensmedio+qepppmatreciclensmedio+qepppsitruaensmedio+qeppppescadoresensmedio);

        $('#total_geral_medio').val(total);
    }
</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="qepid" name="qepid" value="<?= $dadosQuestPronatec['qepid']; ?>"/>

    <table class="Tabela Listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1">
        <tr>
            <td class="SubTituloEsquerdaMenor" colspan="6">INFORME A(S) MODALIDADE(S) DE DEMANDA</td>
            <td rowspan="15" width="5%" style="margin-right: 10px; vertical-align: central;">
                <?php
                    include_once APPRAIZ . "includes/workflow.php";

                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%" > Modalidades: </td>
            <td width="12%"> &nbsp; </td>
            <td width="22%" colspan="3">
                <table border="0">
                <tr>
                    <td width="50%"> <input type="checkbox" name="qepejatecnicointegrado" id="qepejatecnicointegrado" value="S" <?=$check_int;?> > PRONATEC EJA - Técnico Integrado </td>
                    <td> <input type="checkbox" name="qepejatecnicoconcomitante" id="qepejatecnicoconcomitante" value="S" <?=$check_con;?> > PRONATEC EJA - Técnico Concomitante </td>
               </tr>
                <tr>
                    <td> <input type="checkbox" name="qepejaficmedio" id="qepejaficmedio" value="S" <?=$check_med;?> > PRONATEC EJA FIC - Médio </td>
                    <td> <input type="checkbox" name="qepejaficfundamental" id="qepejaficfundamental" value="S" <?=$check_fun;?> > PRONATEC EJA FIC - Fundamental </td>
                </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="SubTituloEsquerdaMenor" colspan="6">PUBLÍCO NÃO PRIORITÁRIO</td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%" > Publíco não Prioritário: </td>
            <td width="10%"> &nbsp; </td>
            <td width="22%" colspan="3">
               <?php
                    $qeppnpensinofund = $dadosQuestPronatec['qeppnpensinofund'];
                    echo campo_texto('qeppnpensinofund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qeppnpensinofund"', 'somarTotalGeralFundamental();', $qeppnpensinofund, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloEsquerdaMenor" colspan="6">PUBLÍCO PRIORITÁRIO</td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> A - Atendidos no Programa Brasil Alfabetizado: </td>
            <td class ="SubTituloDireita" rowspan="9">Ensino Fundamental</td>
            <td width="12%">
                <?php
                    if( $existeAlfabetizado == 'S' ){
                        $qepppbraalfensfund = $dadosQuestPronatec['qepppbraalfensfund'];
                        echo campo_texto('qepppbraalfensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppbraalfensfund"', 'somarTotalGeralFundamental();', $qepppbraalfensfund, null, '', null);

                ?>
            </td>
            <td width="10%" class ="SubTituloDireita" rowspan="9">Ensino Médio</td>
            <td width="40%">
               <?php
                        $qepppbraalfensmedio = $dadosQuestPronatec['qepppbraalfensmedio'];
                        echo campo_texto('qepppbraalfensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppbraalfensmedio"', 'somarTotalGeralMedio();', $qepppbraalfensmedio, null, '', null);
                    }else{
                        echo "<b style=\"color: red; font-size: 11px;\">NESTE MUNICÍPIO NÃO HÁ ATENDIDOS NO PROGRAMA BRASIL ALFABETIZADO.</b>";
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> B - Populações do Campo: </td>
            <td>
               <?php
                    $qeppppopcampoensfund = $dadosQuestPronatec['qeppppopcampoensfund'];
                    echo campo_texto('qeppppopcampoensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qeppppopcampoensfund"', 'somarTotalGeralFundamental();', $qeppppopcampoensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qeppppopcampoensmedio = $dadosQuestPronatec['qeppppopcampoensmedio'];
                    echo campo_texto('qeppppopcampoensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qeppppopcampoensmedio"', 'somarTotalGeralMedio();', $qeppppopcampoensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> C - Quilombolas: </td>
            <td>
               <?php
                    $qepppquilombolaensfund = $dadosQuestPronatec['qepppquilombolaensfund'];
                    echo campo_texto('qepppquilombolaensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppquilombolaensfund"', 'somarTotalGeralFundamental();', $qepppquilombolaensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qepppquilombolaensmedio = $dadosQuestPronatec['qepppquilombolaensmedio'];
                    echo campo_texto('qepppquilombolaensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppquilombolaensmedio"', 'somarTotalGeralMedio();', $qepppquilombolaensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> D - Indiginas: </td>
            <td>
               <?php
                    if( $existeIndigina == 'S'){
                        $qepppindigenasensfund = $dadosQuestPronatec['qepppindigenasensfund'];
                        echo campo_texto('qepppindigenasensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppindigenasensfund"', 'somarTotalGeralFundamental();', $qepppindigenasensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                        $qepppindigenasensmedio = $dadosQuestPronatec['qepppindigenasensmedio'];
                        echo campo_texto('qepppindigenasensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppindigenasensmedio"', 'somarTotalGeralMedio();', $qepppindigenasensmedio, null, '', null);
                    }else{
                        echo "<b style=\"color: red; font-size: 11px;\">NESTE MUNICÍPIO NÃO HÁ ENSINO VOLTADO À POPULAÇÃO INDÍGINA.</b>";
                    }
                ?>
            </td>

        </tr>
        <tr>
            <td class ="SubTituloDireita"> E - Pessoas que cumprem pena em privação de liberdade: </td>
            <td>
                <?php
                    $qepppprovliberensfund = $dadosQuestPronatec['qepppprovliberensfund'];
                    echo campo_texto('qepppprovliberensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppprovliberensfund"', 'somarTotalGeralFundamental();', $qepppprovliberensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qepppprovliberensmedio = $dadosQuestPronatec['qepppprovliberensmedio'];
                    echo campo_texto('qepppprovliberensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppprovliberensmedio"', 'somarTotalGeralMedio();', $qepppprovliberensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> F - Jovem em Cumprimento de Medidas Socioeducativas: </td>
            <td>
               <?php
                    $qepppmedsocioensfund = $dadosQuestPronatec['qepppmedsocioensfund'];
                    echo campo_texto('qepppmedsocioensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppmedsocioensfund"', 'somarTotalGeralFundamental();', $qepppmedsocioensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qepppmedsocioensmedio = $dadosQuestPronatec['qepppmedsocioensmedio'];
                    echo campo_texto('qepppmedsocioensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppmedsocioensmedio"', 'somarTotalGeralMedio();', $qepppmedsocioensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> G - Catadores de Materiais Recicláveis: </td>
            <td>
               <?php
                    $qepppmatreciclensfund = $dadosQuestPronatec['qepppmatreciclensfund'];
                    echo campo_texto('qepppmatreciclensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppmatreciclensfund"', 'somarTotalGeralFundamental();', $qepppmatreciclensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qepppmatreciclensmedio = $dadosQuestPronatec['qepppmatreciclensmedio'];
                    echo campo_texto('qepppmatreciclensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppmatreciclensmedio"', 'somarTotalGeralMedio();', $qepppmatreciclensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> H - Populaçao em situação de Rua: </td>
            <td>
               <?php
                    $qepppsitruaensfund = $dadosQuestPronatec['qepppsitruaensfund'];
                    echo campo_texto('qepppsitruaensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppsitruaensfund"', 'somarTotalGeralFundamental();', $qepppsitruaensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qepppsitruaensmedio = $dadosQuestPronatec['qepppsitruaensmedio'];
                    echo campo_texto('qepppsitruaensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qepppsitruaensmedio"', 'somarTotalGeralMedio();', $qepppsitruaensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> I - Pescadores e Aquicultores:</td>
            <td>
               <?php
                    $qeppppescadoresensfund = $dadosQuestPronatec['qeppppescadoresensfund'];
                    echo campo_texto('qeppppescadoresensfund', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qeppppescadoresensfund"', 'somarTotalGeralFundamental();', $qeppppescadoresensfund, null, '', null);
                ?>
            </td>
            <td width="40%">
               <?php
                    $qeppppescadoresensmedio = $dadosQuestPronatec['qeppppescadoresensmedio'];
                    echo campo_texto('qeppppescadoresensmedio', 'S', $habilitado['TEXT'], '', 14, 6, '######', '', 'right', '', 0, 'id="qeppppescadoresensmedio"', 'somarTotalGeralMedio();', $qeppppescadoresensmedio, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Total Geral: </td>
            <td> &nbsp;</td>
            <td colspan="2">
               <?php
                    $total_geral_fundamental = $dadosQuestPronatec['total_geral_fundamental'];
                    echo campo_texto('total_geral_fundamental', 'N', 'N', '', 14, 6, '######', '', 'right', '', 0, 'id="total_geral_fundamental"', '', $total_geral, null, array('color'=>'red'));
                ?>
            </td>
            <!-- -->
            <td>
               <?php
                    $total_geral_medio = $dadosQuestPronatec['total_geral_medio'];
                    echo campo_texto('total_geral_medio', 'N', 'N', '', 14, 6, '######', '', 'right', '', 0, 'id="total_geral_medio"', '', $total_geral, null, array('color'=>'red'));
                ?>
            </td>
            <!-- -->
        </tr>

        <tr>
            <td class="SubTituloEsquerdaMenor" colspan="5">NOS QUANTITATIVOS ACIMA ESTÁ PREVISTO O ATENDIMENTO DE:</td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> A) Trabalhadores rurais empregados</td>
             <td>
                <input type="radio" name="qepquestao01" id="qepquestao01_S" value="S" onclick="habilitaCamposExterna(this);" <? if ($dadosQuestPronatec['qepquestao01'] == 't') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Sim
                &nbsp;&nbsp;&nbsp;
                <input type="radio" name="qepquestao01" id="qepquestao01_N" value="N" onclick="habilitaCamposExterna(this);" <? if ($dadosQuestPronatec['qepquestao01'] == 'f') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Não
            </td>
            <td colspan="3">
                <?php
                    if($habilitado['RADIO'] == ''){
                        $habilit = $dadosQuestPronatec['qepquestao01qtd'] == 't' ? 'S' : 'N';
                    }else{
                        $habilit = 'N';
                    }
                    $qepquestao01qtd = $dadosQuestPronatec['qepquestao01qtd'];
                    echo campo_texto('qepquestao01qtd', 'N', $habilit, '', 14, 6, '######', '', 'right', '', 0, 'id="qepquestao01qtd"', $qepquestao01qtd, null, '', null);
                ?>
            </td>
        </tr>
    </table>
    <br>
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="salvar" value="Salvar" onclick="salvarQuestEJA_PRONATEC();"/>
                <input type="button" name="cancelar" value="Cancelar" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>