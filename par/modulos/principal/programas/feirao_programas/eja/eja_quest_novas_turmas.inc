<?php
    include_once '_funcoes_eja.php';
    
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
        $descricao = $_SESSION['par']['estuf'];
    }

    monta_titulo($nomePrograma." - Novas Turmas EJA", $descricao);
    echo '<br>';

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja/eja_quest_novas_turmas&acao=A');

    if( $_SESSION['par']['muncod'] == '' ){
        #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
        $unid_1         = 'estado';
        $unid_2         = 'Unidade da Federação não identificada';
        $muncod_estuf   = 'UF';
        $colspan        = '8';
        $descricao_rede = 'estadual';

        $num_mat = carregarMatriculaPBA('E');
        
        #O TEMPO DE CARGA DA SQL QUE BUSCA OS DADOS DA MATRICULA EJA É MUITO AUTO, PARA QUE OCORRA APENAS UMA VEZ A BUSCA FOI CRIADO ESSAS SESSÕES.
        if( $_SESSION['total_matricula']['controle'] != 'S' ){
            $total_matricula    = carregarMatriculasRedPublEJA('E');
            $_SESSION['total_matricula']['controle'] = 'S';
            $_SESSION['total_matricula']['2010'] = $total_matricula['2010'];
            $_SESSION['total_matricula']['2011'] = $total_matricula['2011'];
            $_SESSION['total_matricula']['2012'] = $total_matricula['2012'];
            $_SESSION['total_matricula']['2013'] = $total_matricula['2013'];
        } else {
            $total_matricula['2010'] = $_SESSION['total_matricula']['2010'];
            $total_matricula['2011'] = $_SESSION['total_matricula']['2011'];
            $total_matricula['2012'] = $_SESSION['total_matricula']['2012'];
            $total_matricula['2013'] = $_SESSION['total_matricula']['2013'];
        }

        $texto = '
            <span style="font-size: 18px;"><b>AVISO:</b></span>
            <p style="text-align: justify; font-size: 13px;">Aos Estados e Distrito Federal será permitido selecionar entidade de ensino médio e fundamental.</p>
        ';
    }else{
        #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
        $unid_1         = 'município';
        $unid_2         = 'Município não identificado';
        $muncod_estuf   = 'M';
        $colspan        = '6';
        $descricao_rede = 'municipal';

        $num_mat = carregarMatriculaPBA('M');
        
         #O TEMPO DE CARGA DA SQL QUE BUSCA OS DADOS DA MATRICULA EJA É MUITO AUTO, PARA QUE OCORRA APENAS UMA VEZ A BUSCA FOI CRIADO ESSAS SESSÕES.
        if( $_SESSION['total_matricula']['controle'] != 'S' ){
            $total_matricula    = carregarMatriculasRedPublEJA('M');
            
            $_SESSION['total_matricula']['controle'] = 'S';
            $_SESSION['total_matricula']['2010'] = $total_matricula['2010'];
            $_SESSION['total_matricula']['2011'] = $total_matricula['2011'];
            $_SESSION['total_matricula']['2012'] = $total_matricula['2012'];
            $_SESSION['total_matricula']['2013'] = $total_matricula['2013'];
            
        }else{
            $total_matricula['2010'] = $_SESSION['total_matricula']['2010'];
            $total_matricula['2011'] = $_SESSION['total_matricula']['2011'];
            $total_matricula['2012'] = $_SESSION['total_matricula']['2012'];
            $total_matricula['2013'] = $_SESSION['total_matricula']['2013'];
        }
        
        $texto = '
            <span style="font-size: 18px;"><b>AVISO:</b></span>
            <p style="text-align: justify; font-size: 12px;">Os municípios só poderão solicitar matrícula em novas turmas de ensino fundamental. As matrículas do ensino médio, nesse caso, não serão consideradas pelo FUNDEB.</p>
        ';
    }

    if($_SESSION['entro_primeira_vez'] == ''){
        popupAlertaGeral($texto, '350px', '130px');
        $_SESSION['entro_primeira_vez'] = 'S';
    }

    $dadosQuest = buscaQuestionarioEJA( $_SESSION['par']['inuid'] );
    
    $existeAlfabetizado = verificaRegrasAlfabetizado();
    $existeQuilombola   = verificaRegrasQuilombola();
    $existeIndigina     = verificaRegrasIndiginas();
    $habilitado         = permissaoPrevisaoOferta();
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">

    function editarParecerEJA( pejid ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=editarParecerEJA&pejid="+pejid,
            asynchronous: false,
            success: function(resp){
                var dados = $.parseJSON(resp);
                if(dados.permissao == 'N'){
                    $('#pejsituacao_S').attr('disabled', true);
                    $('#pejsituacao_N').attr('disabled', true);
                    $('#pejid').attr('disabled', true);
                    $('#pejresposta').attr('disabled', true);
                    $('#salvarParecer').attr('disabled', true);
                }else{
                    $('#pejsituacao_S').attr('disabled', false);
                    $('#pejsituacao_N').attr('disabled', false);
                    $('#pejid').attr('disabled', false);
                    $('#pejresposta').attr('disabled', false);
                    $('#salvarParecer').attr('disabled', false);
                }

                if(dados.pejsituacao == 't'){
                    $('#pejsituacao_S').attr('checked', true);
                    $('#pejsituacao_N').attr('checked', false);
                }else{
                    $('#pejsituacao_S').attr('checked', false);
                    $('#pejsituacao_N').attr('checked', true);
                }
                $('#pejid').val(dados.pejid);
                $('#pejresposta').val(dados.pejresposta);
            }
        });

    }

    function deletaParecerEJA( pejid ){
        var confirma = confirm("Deseja realmente excluir o Registro?");

        if( confirma ){
            $('#pejid').val(pejid);
            $('#requisicao').val('deletaParecerEJA');
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

    function somarNaoPrior(){
        var qejpubliconprioranosiniciais        = parseInt($('#qejpubliconprioranosiniciais').val())        ? parseInt($('#qejpubliconprioranosiniciais').val()) : 0;
        var qejpubliconprioranosfinais          = parseInt($('#qejpubliconprioranosfinais').val())          ? parseInt($('#qejpubliconprioranosfinais').val()) : 0;
	var qejpubliconpriorejaqualiprof        = parseInt($('#qejpubliconpriorejaqualiprof').val())        ? parseInt($('#qejpubliconpriorejaqualiprof').val()) : 0;
	var qejpubliconpriorejaqualiprof_fund   = parseInt($('#qejpubliconpriorejaqualiprof_fund').val())   ? parseInt($('#qejpubliconpriorejaqualiprof_fund').val()) : 0;
        var qejpubliconpriorejaqualiprof_medi   = parseInt($('#qejpubliconpriorejaqualiprof_medi').val())   ? parseInt($('#qejpubliconpriorejaqualiprof_medi').val()) : 0;
        var total;

        total = (qejpubliconprioranosiniciais + qejpubliconprioranosfinais + qejpubliconpriorejaqualiprof + qejpubliconpriorejaqualiprof_fund + qejpubliconpriorejaqualiprof_medi);

        $('#total_nao_prior').val(total).css( "color", "green" );

        somarTotalGeral();
    }

    function somarCampo(){
        var qejcampoanosiniciais        = parseInt($('#qejcampoanosiniciais').val())        ? parseInt($('#qejcampoanosiniciais').val()) : 0;
        var qejcampoanosfinais          = parseInt($('#qejcampoanosfinais').val())          ? parseInt($('#qejcampoanosfinais').val()) : 0;
	var qejcampoejaqualiprof        = parseInt($('#qejcampoejaqualiprof').val())        ? parseInt($('#qejcampoejaqualiprof').val()) : 0;
	var qejcampoejaqualiprof_fund   = parseInt($('#qejcampoejaqualiprof_fund').val())   ? parseInt($('#qejcampoejaqualiprof_fund').val()) : 0;
        var qejcampoejaqualiprof_medi   = parseInt($('#qejcampoejaqualiprof_medi').val())   ? parseInt($('#qejcampoejaqualiprof_medi').val()) : 0;
        var total;

        total = (qejcampoanosiniciais+qejcampoanosfinais+qejcampoejaqualiprof+qejcampoejaqualiprof_fund+qejcampoejaqualiprof_medi);

        $('#total_campo').val(total).css( "color", "green" );

        somarTotalPrioritario();
    }

    function somarQuilombolas(){
        var qejquilombolaanosiniciais     = parseInt($('#qejquilombolaanosiniciais').val())     ? parseInt($('#qejquilombolaanosiniciais').val()) : 0;
        var qejquilombolaanosfinais       = parseInt($('#qejquilombolaanosfinais').val())       ? parseInt($('#qejquilombolaanosfinais').val()) : 0;
	var qejquilombolaqualiprof        = parseInt($('#qejquilombolaqualiprof').val())        ? parseInt($('#qejquilombolaqualiprof').val()) : 0;
	var qejquilombolaqualiprof_fund   = parseInt($('#qejquilombolaqualiprof_fund').val())   ? parseInt($('#qejquilombolaqualiprof_fund').val()) : 0;
        var qejquilombolaqualiprof_medi   = parseInt($('#qejquilombolaqualiprof_medi').val())   ? parseInt($('#qejquilombolaqualiprof_medi').val()) : 0;
        var total;

        total = (qejquilombolaanosiniciais+qejquilombolaanosfinais+qejquilombolaqualiprof+qejquilombolaqualiprof_fund+qejquilombolaqualiprof_medi);

        $('#total_quilombolas').val(total).css( "color", "green" );

        somarTotalPrioritario();
    }

    function somarIndigenas(){
        var qejindigenasanosiniciais    = parseInt($('#qejindigenasanosiniciais').val())    ? parseInt($('#qejindigenasanosiniciais').val()) : 0;
        var qejindigenasanosfinais      = parseInt($('#qejindigenasanosfinais').val())      ? parseInt($('#qejindigenasanosfinais').val()) : 0;
	var qejindigenaqualiprof        = parseInt($('#qejindigenaqualiprof').val())        ? parseInt($('#qejindigenaqualiprof').val()) : 0;
	var qejindigenaqualiprof_fund   = parseInt($('#qejindigenaqualiprof_fund').val())   ? parseInt($('#qejindigenaqualiprof_fund').val()) : 0;
        var qejindigenaqualiprof_medi   = parseInt($('#qejindigenaqualiprof_medi').val())   ? parseInt($('#qejindigenaqualiprof_medi').val()) : 0;
        var total;

        total = (qejindigenasanosiniciais+qejindigenasanosfinais+qejindigenaqualiprof+qejindigenaqualiprof_fund+qejindigenaqualiprof_medi);

        $('#total_indigenas').val(total).css( "color", "green" );

        somarTotalPrioritario();
    }

    function somarLiberdade(){
        var qejprivliberanosiniciais    = parseInt($('#qejprivliberanosiniciais').val())    ? parseInt($('#qejprivliberanosiniciais').val()) : 0;
        var qejprivliberanosfinais      = parseInt($('#qejprivliberanosfinais').val())      ? parseInt($('#qejprivliberanosfinais').val()) : 0;
	var qejprivliberqualiprof       = parseInt($('#qejprivliberqualiprof').val())       ? parseInt($('#qejprivliberqualiprof').val()) : 0;
	var qejprivliberqualiprof_fund  = parseInt($('#qejprivliberqualiprof_fund').val())  ? parseInt($('#qejprivliberqualiprof_fund').val()) : 0;
        var qejprivliberqualiprof_medi  = parseInt($('#qejprivliberqualiprof_medi').val())  ? parseInt($('#qejprivliberqualiprof_medi').val()) : 0;
        var total;

        total = (qejprivliberanosiniciais+qejprivliberanosfinais+qejprivliberqualiprof+qejprivliberqualiprof_fund+qejprivliberqualiprof_medi);

        $('#total_liberdade').val(total).css( "color", "green" );

        somarTotalPrioritario();
    }

    function somarTotalPrioritario(){
        var qejpublicoprioritariobraalf = parseInt($('#qejpublicoprioritariobraalf').val()) ? parseInt($('#qejpublicoprioritariobraalf').val()) : 0;
        var total_campo                 = parseInt($('#total_campo').val())         ? parseInt($('#total_campo').val()) : 0;
        var total_quilombolas           = parseInt($('#total_quilombolas').val())   ? parseInt($('#total_quilombolas').val()) : 0;
        var total_indigenas             = parseInt($('#total_indigenas').val())     ? parseInt($('#total_indigenas').val()) : 0;
        var total_liberdade             = parseInt($('#total_liberdade').val())     ? parseInt($('#total_liberdade').val()) : 0;
        var total;

        total = (qejpublicoprioritariobraalf+total_campo+total_quilombolas+total_indigenas+total_liberdade);

        $('#total_prioritario').val(total).css( "color", "green" );

        somarTotalGeral();
    }

    function somarTotalGeral(){
        var total_nao_prior     = parseInt($('#total_nao_prior').val())     ? parseInt($('#total_nao_prior').val()) : 0;
        var total_prioritario   = parseInt($('#total_prioritario').val())   ? parseInt($('#total_prioritario').val()) : 0;
        var total;

        total = (total_nao_prior+total_prioritario);

        $('#total_geral').val(total).css( "color", "green" );
    }

    function somarTotalGeralPropostoMEC(){
        var qejvlrmecpublnaopri         = parseInt($('#qejvlrmecpublnaopri').val()) ? parseInt($('#qejvlrmecpublnaopri').val()) : 0;
        var qejvlrmecegresso            = parseInt($('#qejvlrmecegresso').val()) ? parseInt($('#qejvlrmecegresso').val()) : 0;
        var qejvlrmeccomcampo           = parseInt($('#qejvlrmeccomcampo').val()) ? parseInt($('#qejvlrmeccomcampo').val()) : 0;
        var qejvlrmecquilombolas        = parseInt($('#qejvlrmecquilombolas').val()) ? parseInt($('#qejvlrmecquilombolas').val()) : 0;
        var qejvlrmecindigenas          = parseInt($('#qejvlrmecindigenas').val()) ? parseInt($('#qejvlrmecindigenas').val()) : 0;
        var qejvlrmecpessoaliberdade    = parseInt($('#qejvlrmecpessoaliberdade').val()) ? parseInt($('#qejvlrmecpessoaliberdade').val()) : 0;
        
        var total;
        var total_prioritario;
        var total_liberado_mec;
        
        var valor_por_aluno = 1777.38;
        
        total_prioritario = (qejvlrmecegresso+qejvlrmeccomcampo+qejvlrmecquilombolas+qejvlrmecindigenas+qejvlrmecpessoaliberdade);
        total = (qejvlrmecpublnaopri+qejvlrmecegresso+qejvlrmeccomcampo+qejvlrmecquilombolas+qejvlrmecindigenas+qejvlrmecpessoaliberdade);

        $('#qejvlrmec_total').val(total_prioritario);
        $('#qejvlrmec_total_geral').val(total);
        
        total_liberado_mec = parseFloat( total * valor_por_aluno );
        
        $( '#qejvlrmec_valor_liberado' ).val( mascaraglobal( "[.###],##", total_liberado_mec.toFixed( 2 ) ) ).css( "color", "red" );
    }

    function salvarParecerEJA(){
        var pejresposta = $('#pejresposta');
        var pejsituacao = $('input:radio[name=pejsituacao]:checked');

        var erro;

        if(!pejresposta.val()){
            alert('O campo "Descrição do Parecer" é um campo obrigatório!');
            pejresposta.focus();
            erro = 1;
            return false;
        }
        if(!pejsituacao.val()){
            alert('O campo "Favorável/Não Favorável" é um campo obrigatório!');
            pejsituacao.focus();
            erro = 1;
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarParecerEJA');
            $('#formulario').submit();
        }
    }

    function salvarQuestEJA(){
        var erro;
        var inputs = $("#formulario").find('INPUT[class*="obrigatorio normal"]');
        var qejqejprevinicio = $('#qejqejprevinicio');

        if(!qejqejprevinicio.val()){
            alert('O campo "Previsão de Início" é um campo obrigatório!');
            qejqejprevinicio.focus();
            erro = 1;
            return false;
        }

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
            $('#requisicao').val('salvarQuestEJA');
            $('#formulario').submit();
        }
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="qejid" name="qejid" value="<?= $dadosQuest['qejid']; ?>"/>
    <input type="hidden" id="pejid" name="pejid" value=""/>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td width="95%">
                <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td class ="SubTituloDireita" width="18%">Previsão de Início:</td>
                        <td width="10%">
                            <?php
                                $texto_ajuda = "Somente serão contempladas com recursos do FUNDEB em 2014 as turmas que iniciarem sua atividades até a última quarta-feira do mês de maio.";
                                $qejqejprevinicio = $dadosQuest['qejqejprevinicio'];
                                echo campo_data2('qejqejprevinicio','S',$habilitado['TEXT'],'','DD/MM/YYYY', $texto_ajuda, '', $qejqejprevinicio, '', null, 'qejqejprevinicio' );
                            ?>
                        </td>
                        <td colspan="<?=$colspan?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                        <? if( $habilitado['ANALISE'] == 'S' ){ ?>
                            <td width="8%" class ="SubTituloDireitaMenor">Total Liberado Após Análise do MEC</td>
                        <? }?>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita">Matrícula público não prioritário: <br><b>Anos Iniciais</b></td>
                        <td width="9%">
                            <?php
                                $qejpubliconprioranosiniciais = $dadosQuest['qejpubliconprioranosiniciais'];
                                echo campo_texto('qejpubliconprioranosiniciais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejpubliconprioranosiniciais"', 'somarNaoPrior();', $qejpubliconprioranosiniciais, null, '', null);
                            ?>
                        </td>
                        <td class ="SubTituloDireitaMenor" width="5%">Anos Finais:</td>
                        <td width="8%">
                            <?php
                                $qejpubliconprioranosfinais = $dadosQuest['qejpubliconprioranosfinais'];
                                echo campo_texto('qejpubliconprioranosfinais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejpubliconprioranosfinais"', 'somarNaoPrior();', $qejpubliconprioranosfinais, null, '', null);
                            ?>
                        </td>

                        <? if($muncod_estuf == 'M'){ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                <td>
                                    <?php
                                        $qejpubliconpriorejaqualiprof = $dadosQuest['qejpubliconpriorejaqualiprof'];
                                        echo campo_texto('qejpubliconpriorejaqualiprof', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejpubliconpriorejaqualiprof"', 'somarNaoPrior();', $qejpubliconpriorejaqualiprof, null, '', null);
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                <td width="8%">
                                    <?php
                                        $qejpubliconpriorejaqualiprof_fund = $dadosQuest['qejpubliconpriorejaqualiprof_fund'];
                                        echo campo_texto('qejpubliconpriorejaqualiprof_fund', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejpubliconpriorejaqualiprof_fund"', 'somarNaoPrior();', $qejpubliconpriorejaqualiprof_fund, null, '', null);
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                <td width="8%">
                                    <?php
                                        $qejpubliconpriorejaqualiprof_medi = $dadosQuest['qejpubliconpriorejaqualiprof_medi'];
                                        echo campo_texto('qejpubliconpriorejaqualiprof_medi', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejpubliconpriorejaqualiprof_medi"', 'somarNaoPrior();', $qejpubliconpriorejaqualiprof_medi, null, '', null);
                                    ?>
                                </td>
                        <? } ?>

                        <td class ="SubTituloDireitaMenor" width="8%">Total:</td>
                        <td width="6%">
                            <?php
                                $total_nao_prior = $dadosQuest['total_nao_prior'];
                                echo campo_texto('total_nao_prior', 'N', 'N', '', 11, 6, '#####', '', 'right', '', 0, 'id="total_nao_prior"', '', $total_nao_prior, null, array('color'=>'green'));
                            ?>
                        </td>
                        
                        <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                <td align="right">
                                    <?php
                                        $qejvlrmecpublnaopri = $dadosQuest['qejvlrmecpublnaopri'];
                                        echo campo_texto('qejvlrmecpublnaopri', 'N', 'S', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmecpublnaopri"', 'somarTotalGeralPropostoMEC();', $qejvlrmecpublnaopri, null, array('color'=>'red'));
                                    ?>
                                </td>
                        <?  } ?>
                                
                    </tr>
                    <tr>
                        <td colspan="11" class ="SubTituloEsquerda">Matrícula público prioritário.<br>Modalidades:</td>
                    </tr>

                    <!--INÍCIO PERGUNTA A-->
                    <tr>
                        <td class ="SubTituloDireita">A - Egresso do Programa Brasil Alfabetizado:</td>
                            <?php
                                $qejpublicoprioritariobraalf = $dadosQuest['qejpublicoprioritariobraalf'];
                                if($existeAlfabetizado == 'S'){
                                    echo "<td>";
                                    echo campo_texto('qejpublicoprioritariobraalf', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejpublicoprioritariobraalf"', 'somarTotalPrioritario();', $qejpublicoprioritariobraalf, null, '', null);
                                    echo "</td>";
                                    echo "<td colspan=\"{$colspan}\" class =\"SubTituloDireitaMenor\">&nbsp;</td>";
                                }else{
                                    echo "<td colspan=\"9\" class=\"SubTituloEsquerdaMenor\"> <b style=\"color: red; font-size: 11px;\"> - Opçao permitida apenas para os {$unid_1} que realizaram adesão em pelo menos um dos seguintes anos: 2010, 2011 e 2012.</b> </td>";
                                }
                            ?>
                        </td>
                        <?  if( $habilitado['ANALISE'] == 'S' && $existeAlfabetizado == 'S'){ ?>
                                <td align="right">
                                    <?php
                                        $qejvlrmecegresso = $dadosQuest['qejvlrmecegresso'];
                                        echo campo_texto('qejvlrmecegresso', 'N', 'S', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmecegresso"', 'somarTotalGeralPropostoMEC();', $qejvlrmecegresso, null, array('color'=>'red'));
                                    ?>
                                </td>
                        <?  } ?>
                    </tr>
                    <!--FIM PERGUNTA A-->

                    <!--INÍCIO PERGUNTA B-->
                    <tr>
                        <td class ="SubTituloDireita">B - Estudantes das comunidades do Campo: <br><b>Anos Iniciais</b></td>
                        <td>
                            <?php
                                $qejcampoanosiniciais = $dadosQuest['qejcampoanosiniciais'];
                                echo campo_texto('qejcampoanosiniciais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejcampoanosiniciais"', 'somarCampo();', $qejcampoanosiniciais, null, '', null);
                            ?>
                        </td>
                        <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                        <td>
                            <?php
                                $qejcampoanosfinais = $dadosQuest['qejcampoanosfinais'];
                                echo campo_texto('qejcampoanosfinais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejcampoanosfinais"', 'somarCampo();', $qejcampoanosfinais, null, '', null);
                            ?>
                        </td>

                        <? if($muncod_estuf == 'M'){ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                <td>
                                    <?php
                                        $qejcampoejaqualiprof = $dadosQuest['qejcampoejaqualiprof'];
                                        echo campo_texto('qejcampoejaqualiprof', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejcampoejaqualiprof"', 'somarCampo();', $qejcampoejaqualiprof, null, '', null);
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                <td>
                                    <?php
                                        $qejcampoejaqualiprof_fund = $dadosQuest['qejcampoejaqualiprof_fund'];
                                        echo campo_texto('qejcampoejaqualiprof_fund', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejcampoejaqualiprof_fund"', 'somarCampo();', $qejcampoejaqualiprof_fund, null, '', null);
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                <td>
                                    <?php
                                        $qejcampoejaqualiprof_medi = $dadosQuest['qejcampoejaqualiprof_medi'];
                                        echo campo_texto('qejcampoejaqualiprof_medi', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejcampoejaqualiprof_medi"', 'somarCampo();', $qejcampoejaqualiprof_medi, null, '', null);
                                    ?>
                                </td>
                        <? } ?>

                        <td class ="SubTituloDireitaMenor">Total:</td>
                        <td>
                            <?php
                                $total_campo = $dadosQuest['total_campo'];
                                echo campo_texto('total_campo', 'N', 'N', '', 11, 5, '#####', '', 'right', '', 0, 'id="total_campo"', '', $total_campo, null, array('color'=>'green'));
                            ?>
                        </td>
                        
                        <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                <td align="right">
                                    <?php
                                        $qejvlrmeccomcampo = $dadosQuest['qejvlrmeccomcampo'];
                                        echo campo_texto('qejvlrmeccomcampo', 'N', 'S', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmeccomcampo"', 'somarTotalGeralPropostoMEC();', $qejvlrmeccomcampo, null, array('color'=>'red'));
                                    ?>
                                </td>
                        <?  } ?>
                        
                    </tr>
                    <!--FIM PERGUNTA B-->

                    <!--INÍCIO PERGUNTA C-->
                    <tr>
                        <td class ="SubTituloDireita">C - Quilombolas: <br><b>Anos Iniciais</b></td>
                        <?  if($existeQuilombola == 'S'){ ?>
                                <td>
                                    <?php
                                        $qejquilombolaanosiniciais = $dadosQuest['qejquilombolaanosiniciais'];
                                        echo campo_texto('qejquilombolaanosiniciais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejquilombolaanosiniciais"', 'somarQuilombolas();', $qejquilombolaanosiniciais, null, '', null);
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                                <td>
                                    <?php
                                        $qejquilombolaanosfinais = $dadosQuest['qejquilombolaanosfinais'];
                                        echo campo_texto('qejquilombolaanosfinais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejquilombolaanosfinais"', 'somarQuilombolas();', $qejquilombolaanosfinais, null, '', null);
                                    ?>
                                </td>

                                <? if($muncod_estuf == 'M'){ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                        <td>
                                            <?php
                                                $qejquilombolaqualiprof = $dadosQuest['qejquilombolaqualiprof'];
                                                echo campo_texto('qejquilombolaqualiprof', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejquilombolaqualiprof"', 'somarQuilombolas();', $qejquilombolaqualiprof, null, '', null);
                                            ?>
                                        </td>
                                <? }else{ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                        <td>
                                            <?php
                                                $qejquilombolaqualiprof_fund = $dadosQuest['qejquilombolaqualiprof_fund'];
                                                echo campo_texto('qejquilombolaqualiprof_fund', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejquilombolaqualiprof_fund"', 'somarQuilombolas();', $qejquilombolaqualiprof_fund, null, '', null);
                                            ?>
                                        </td>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                        <td>
                                            <?php
                                                $qejquilombolaqualiprof_medi = $dadosQuest['qejquilombolaqualiprof_medi'];
                                                echo campo_texto('qejquilombolaqualiprof_medi', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejquilombolaqualiprof_medi"', 'somarQuilombolas();', $qejquilombolaqualiprof_medi, null, '', null);
                                            ?>
                                        </td>
                                <? } ?>

                                <td class ="SubTituloDireitaMenor">Total:</td>
                                <td>
                                    <?php
                                        $total_quilombolas = $dadosQuest['total_quilombolas'];
                                        echo campo_texto('total_quilombolas', 'N', 'N', '', 11, 6, '#####', '', 'right', '', 0, 'id="total_quilombolas"', '', $total_quilombolas, null, array('color'=>'green'));
                                    ?>
                                </td>
                                
                                <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                        <td align="right">
                                            <?php
                                                $qejvlrmecquilombolas = $dadosQuest['qejvlrmecquilombolas'];
                                                echo campo_texto('qejvlrmecquilombolas', 'N', 'S', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmecquilombolas"', 'somarTotalGeralPropostoMEC();', $qejvlrmecquilombolas, null, array('color'=>'red'));
                                            ?>
                                        </td>
                                <?  } ?>
                                
                        <? }else{ ?>
                                <td colspan="10" class="SubTituloEsquerdaMenor"> <b style="color: red; font-size: 11px;"> - <?=$unid_2;?> no censo escolar com atendimento aos Quilombolas.</b></td>
                        <? } ?>
                    </tr>
                    <!--FIM PERGUNTA C-->

                    <!--INÍCIO PERGUNTA D-->
                    <tr>
                        <td class ="SubTituloDireita">D - Indígenas: <br><b>Anos Iniciais</b></td>
                        <? if($existeIndigina == 'S'){ ?>
                                <td>
                                    <?php
                                        $qejindigenasanosiniciais = $dadosQuest['qejindigenasanosiniciais'];
                                        echo campo_texto('qejindigenasanosiniciais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejindigenasanosiniciais"', 'somarIndigenas();', $qejindigenasanosiniciais, null, '', null);
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                                <td>
                                    <?php
                                        $qejindigenasanosfinais = $dadosQuest['qejindigenasanosfinais'];
                                        echo campo_texto('qejindigenasanosfinais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejindigenasanosfinais"', 'somarIndigenas();', $qejindigenasanosfinais, null, '', null);
                                    ?>
                                </td>

                                <? if($muncod_estuf == 'M'){ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                        <td>
                                            <?php
                                                $qejindigenaqualiprof = $dadosQuest['qejindigenaqualiprof'];
                                                echo campo_texto('qejindigenaqualiprof', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejindigenaqualiprof"', 'somarIndigenas();', $qejindigenaqualiprof, null, '', null);
                                            ?>
                                        </td>
                                <? }else{ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                        <td>
                                            <?php
                                                $qejindigenaqualiprof_fund = $dadosQuest['qejindigenaqualiprof_fund'];
                                                echo campo_texto('qejindigenaqualiprof_fund', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejindigenaqualiprof_fund"', 'somarIndigenas();', $qejindigenaqualiprof_fund, null, '', null);
                                            ?>
                                        </td>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                        <td>
                                            <?php
                                                $qejindigenaqualiprof_medi = $dadosQuest['qejindigenaqualiprof_medi'];
                                                echo campo_texto('qejindigenaqualiprof_medi', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejindigenaqualiprof_medi"', 'somarIndigenas();', $qejindigenaqualiprof_medi, null, '', null);
                                            ?>
                                        </td>
                                <? } ?>

                                <td class ="SubTituloDireitaMenor">Total:</td>
                                <td>
                                    <?php
                                        $total_indigenas = $dadosQuest['total_indigenas'];
                                        echo campo_texto('total_indigenas', 'N', 'N', '', 11, 6, '#####', '', 'right', '', 0, 'id="total_indigenas"', '', $total_indigenas, null, array('color'=>'green'));
                                    ?>
                                </td>
                                    
                                <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                        <td align="right">
                                            <?php
                                                $qejvlrmecindigenas = $dadosQuest['qejvlrmecindigenas'];
                                                echo campo_texto('qejvlrmecindigenas', 'N', 'S', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmecindigenas"', 'somarTotalGeralPropostoMEC();', $qejvlrmecindigenas, null, array('color'=>'red'));
                                            ?>
                                        </td>
                                <?  } ?>
                                    
                        <? }else{ ?>
                                <td colspan="9" class="SubTituloEsquerdaMenor"> <b style="color: red; font-size: 11px;"> - <?=$unid_2;?> no censo escolar com atendimento à comunidade Indígenas.</b> </td>
                        <? } ?>
                    </tr>
                    <!--FIM PERGUNTA D-->

                    <!--INÍCIO PERGUNTA E-->
                    <tr>
                        <td class ="SubTituloDireita">E - Pessoas privadas de Liberdade: <br><b>Anos Iniciais</b></td>
                        <td>
                            <?php
                                $qejprivliberanosiniciais = $dadosQuest['qejprivliberanosiniciais'];
                                echo campo_texto('qejprivliberanosiniciais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejprivliberanosiniciais"', 'somarLiberdade();', $qejprivliberanosiniciais, null, '', null);
                            ?>
                        </td>
                        <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                        <td>
                            <?php
                                $qejprivliberanosfinais = $dadosQuest['qejprivliberanosfinais'];
                                echo campo_texto('qejprivliberanosfinais', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejprivliberanosfinais"', 'somarLiberdade();', $qejprivliberanosfinais, null, '', null);
                            ?>
                        </td>

                        <? if($muncod_estuf == 'M'){ ?>
                                <td class ="SubTituloDireitaMenor" width="15%">EJA Integrada a qualificação profissional:</td>
                                <td width="8%">
                                    <?php
                                        $qejprivliberqualiprof = $dadosQuest['qejprivliberqualiprof'];
                                        echo campo_texto('qejprivliberqualiprof', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejprivliberqualiprof"', 'somarLiberdade();', $qejprivliberqualiprof, null, '', null);
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação<br>profissional/fundamental:</td>
                                <td>
                                    <?php
                                        $qejprivliberqualiprof_fund = $dadosQuest['qejprivliberqualiprof_fund'];
                                        echo campo_texto('qejprivliberqualiprof_fund', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejprivliberqualiprof_fund"', 'somarLiberdade();', $qejprivliberqualiprof_fund, null, '', null);
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                <td>
                                    <?php
                                        $qejprivliberqualiprof_medi = $dadosQuest['qejprivliberqualiprof_medi'];
                                        echo campo_texto('qejprivliberqualiprof_medi', 'S', $habilitado['TEXT'], '', 11, 6, '#####', '', 'right', '', 0, 'id="qejprivliberqualiprof_medi"', 'somarLiberdade();', $qejprivliberqualiprof_medi, null, '', null);
                                    ?>
                                </td>
                        <? } ?>

                        <td class ="SubTituloDireitaMenor">Total:</td>
                        <td>
                            <?php
                                $total_liberdade = $dadosQuest['total_liberdade'];
                                echo campo_texto('total_liberdade', 'N', 'N', '', 11, 6, '#####', '', 'right', '', 0, 'id="total_liberdade"', '', $total_liberdade, null, array('color'=>'green'));
                            ?>
                        </td>
                            
                        <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                <td align="right">
                                    <?php
                                        $qejvlrmecpessoaliberdade = $dadosQuest['qejvlrmecpessoaliberdade'];
                                        echo campo_texto('qejvlrmecpessoaliberdade', 'N', 'S', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmecpessoaliberdade"', 'somarTotalGeralPropostoMEC();', $qejvlrmecpessoaliberdade, null, array('color'=>'red'));
                                    ?>
                                </td>
                        <?  } ?>
                            
                    </tr>
                    <!--FIM PERGUNTA E-->

                    <tr>
                        <td class ="SubTituloDireita">F - Total de Matrículas do Público Prioritário:</td>
                        <td colspan="<?=$colspan;?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                        <td>
                            <?php
                                $total_prioritario = $dadosQuest['total_prioritario'];
                                echo campo_texto('total_prioritario', 'N', 'N', '', 11, 6, '#####', '', 'right', '', 0, 'id="total_prioritario"', '', $total_prioritario, null, array('color'=>'green'));
                            ?>
                        </td>
                        
                        <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                <td align="right">
                                    <?php
                                        $qejvlrmec_total = $dadosQuest['qejvlrmec_total'];
                                        echo campo_texto('qejvlrmec_total', 'N', 'N', '', 11, 6, '#####', '', 'right', '', 0, 'id="qejvlrmec_total"', 'somarTotalGeralPropostoMEC();', $qejvlrmec_total, null, array('color'=>'red'));
                                    ?>
                                </td>
                        <?  } ?>
                        
                    </tr>
                    <tr>
                        <td colspan="11" class ="SubTituloEsquerda">&nbsp;</td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita">Total Geral:</td>
                        <td colspan="<?=$colspan;?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                        <td>
                            <?php
                                $total_geral = $dadosQuest['total_geral'];
                                echo campo_texto('total_geral', 'N', 'N', '', 11, 20, '######', '', 'right', '', 0, 'id="total_geral"', '',$total_geral, null, array('color'=>'green'));
                            ?>
                        </td>
                        
                        <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                                <td align="right">
                                    <?php
                                        $qejvlrmec_total_geral = $dadosQuest['qejvlrmec_total_geral'];
                                        echo campo_texto('qejvlrmec_total_geral', 'N', 'N', '', 11, 20, '[.###],##', '', 'right', '', 0, 'id="qejvlrmec_total_geral"', '', $qejvlrmec_total_geral, null, array('color'=>'red'));
                                    ?>
                                </td>
                        <?  } ?>
                        
                    </tr>
                    <?  if( $habilitado['ANALISE'] == 'S' ){ ?>
                        <tr>
                            <td class ="SubTituloDireita">Valor Liberado pelo MEC R$:</td>
                            <td colspan="<?=$colspan+1?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                            <td align="right">
                                <?php
                                    $qejvlrmec_valor_liberado = $dadosQuest['qejvlrmec_valor_liberado'];
                                    echo campo_texto('qejvlrmec_valor_liberado', 'N', 'N', '', 11, 30, '[.###],##', '', 'right', '', 0, 'id="qejvlrmec_valor_liberado"', '', $qejvlrmec_valor_liberado, null, array('color'=>'red') );
                                ?>
                            </td>
                        </tr>
                    <?  } ?>
                </table>
                <br>

                <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td colspan="3" class ="SubTituloEsquerda"> Na previsão informada acima está previsto o atendimento de: </td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita" width="23%">Catadores de Materiais Recicláveis</td>
                        <td width="13%">
                            <input type="radio" name="qejcatmatrecicla" id="qejcatmatrecicla_S" value="S" onclick="habilitaCamposExterna(this);" <? if ($dadosQuest['qejcatmatrecicla'] == 't') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Sim
                            &nbsp;&nbsp;&nbsp;
                            <input type="radio" name="qejcatmatrecicla" id="qejcatmatrecicla_N" value="N" onclick="habilitaCamposExterna(this);" <? if ($dadosQuest['qejcatmatrecicla'] == 'f') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Não
                        </td>
                        <td>
                            <?php
                                if($habilitado['RADIO'] == ''){
                                    $habilit = $dadosQuest['qejcatmatrecicla'] == 't' ? 'S' : 'N';
                                }else{
                                    $habilit = 'N';
                                }
                                $qejcatmatreciclaqtd = $dadosQuest['qejcatmatreciclaqtd'];
                                echo campo_texto('qejcatmatreciclaqtd', 'N', $habilit, '', 11, 20, '######', '', 'right', '', 0, 'id="qejcatmatreciclaqtd"', $qejcatmatreciclaqtd, null, '', null);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita" width="23%">Trabalhadores rurais empregados</td>
                         <td width="13%">
                            <input type="radio" name="qejtrabrural" id="qejtrabrural_S" value="S" onclick="habilitaCamposExterna(this);" <? if ($dadosQuest['qejtrabrural'] == 't') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Sim
                            &nbsp;&nbsp;&nbsp;
                            <input type="radio" name="qejtrabrural" id="qejtrabrural_N" value="N" onclick="habilitaCamposExterna(this);" <? if ($dadosQuest['qejtrabrural'] == 'f') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Não
                        </td>
                        <td>
                            <?php
                                if($habilitado['RADIO'] == ''){
                                    $habilit = $dadosQuest['qejtrabrural'] == 't' ? 'S' : 'N';
                                }else{
                                    $habilit = 'N';
                                }
                                $qejtrabruralqtd = $dadosQuest['qejtrabruralqtd'];
                                echo campo_texto('qejtrabruralqtd', 'N', $habilit, '', 11, 20, '######', '', 'right', '', 0, 'id="qejtrabruralqtd"', $qejtrabruralqtd, null, '', null);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita" width="23%">Pessoas em situação de rua</td>
                         <td width="13%">
                            <input type="radio" name="qejpessoarua" id="qejpessoarua_S" value="S" onclick="habilitaCamposExterna(this);" <? if ($dadosQuest['qejpessoarua'] == 't') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Sim
                            &nbsp;&nbsp;&nbsp;
                            <input type="radio" name="qejpessoarua" id="qejpessoarua_N" value="N" onclick="habilitaCamposExterna(this);" <? if ($dadosQuest['qejpessoarua'] == 'f') echo 'checked=checked'; echo $habilitado['RADIO']; ?> > Não
                        </td>
                        <td>
                            <?php
                                if($habilitado['RADIO'] == ''){
                                    $habilit = $dadosQuest['qejpessoarua'] == 't' ? 'S' : 'N';
                                }else{
                                    $habilit = 'N';
                                }
                                $qejpessoaruaqtd = $dadosQuest['qejpessoaruaqtd'];
                                echo campo_texto('qejpessoaruaqtd', 'N', $habilit, '', 11, 20, '######', '', 'right', '', 0, 'id="qejpessoaruaqtd"', $qejpessoaruaqtd, null, '', null);
                            ?>
                        </td>
                    </tr>
                </table>

                <br>
                <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">
                    <?php 
                        $somamatricula = ($total_matricula['2010'] ? $total_matricula['2010'] : '0') + ($total_matricula['2011'] ? $total_matricula['2011'] : '0') + ($total_matricula['2012'] ? $total_matricula['2012'] : '0') + ($total_matricula['2013'] ? $total_matricula['2013'] : '0'); 
                        $media = $somamatricula/4; 
                    ?>
                    <tr>
                        <td colspan="2" class ="SubTituloEsquerda"> Total de matrículas da rede pública <?php echo $descricao_rede;?> de EJA - Ensino Fundamental / Presencial </td>
                    </tr>
                    <tr>
                        <td colspan="2" class ="SubTituloEsquerda">
                            <table align="center" border="0" class="Listagem" cellpadding="3" cellspacing="1" width="40%" style="text-align: center;">
                            	<input id="media" type="hidden" name="media" value="<?php echo $media+$media; ?>">
                                <tr>
                                    <td colspan="4" style="text-align: center; font-weight: bold;">Ano</td>
                                    <td style="text-align: center; font-weight: bold;">&nbsp;</td>
                                </tr>
                                <tr>
                                    <td style="font-weight: bold;">2010</td>
                                    <td style="font-weight: bold;">2011</td>
                                    <td style="font-weight: bold;">2012</td>
                                    <td style="font-weight: bold;">2013</td>
                                    <td style="font-weight: bold;">Média</td>
                                </tr>
                                <tr>
                                    <td><?php echo $total_matricula['2010'] ? $total_matricula['2010'] : '0'; ?></td>
                                    <td><?php echo $total_matricula['2011'] ? $total_matricula['2011'] : '0'; ?></td>
                                    <td><?php echo $total_matricula['2012'] ? $total_matricula['2012'] : '0'; ?></td>
                                    <td><?php echo $total_matricula['2013'] ? $total_matricula['2013'] : '0'; ?></td>
                                    <td><?php echo $media; ?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" class ="SubTituloEsquerda"> Total de matrículas dos Egressos do PBA </td>
                    </tr>
                    <tr>
                        <td colspan="2" class ="SubTituloEsquerda">
                            <table align="center" border="0" class="Listagem" cellpadding="3" cellspacing="1" width="40%" style="text-align: center;">
                                <tr>
                                    <td colspan="8" style="text-align: center; font-weight: bold;">Ano</td>
                                    <td colspan="2" style="text-align: center; font-weight: bold;">&nbsp;</td>
                                </tr>
                                <tr>
                                    <td colspan="2" style="font-weight: bold;">2010</td>
                                    <td colspan="2" style="font-weight: bold;">2011</td>
                                    <td colspan="2" style="font-weight: bold;">2012</td>
                                    <td colspan="2" style="font-weight: bold;">2013</td>
                                    <td colspan="2" style="font-weight: bold;">Média</td>
                                </tr>
                                <tr>
                                    <td>Prefeitura</td>
                                    <td>Secretaria</td>
                                    <td>Prefeitura</td>
                                    <td>Secretaria</td>
                                    <td>Prefeitura</td>
                                    <td>Secretaria</td>
                                    <td>Prefeitura</td>
                                    <td>Secretaria</td>    
                                    <td>Prefeitura</td>
                                    <td>Secretaria</td>                                                                       
                                </tr>
                                <tr>
                                    <td><?php echo $num_mat[0]['numeromatriculas'] = ( $num_mat[0]['ano'] == '2010' && $num_mat[0]['ent_matricula'] == 'PREF' && $num_mat[0]['numeromatriculas'] != '' ) ? $num_mat[0]['numeromatriculas'] : '0'; ?></td>
                                    <td><?php echo $num_mat[1]['numeromatriculas'] = ( $num_mat[1]['ano'] == '2010' && $num_mat[1]['ent_matricula'] == 'SECR' && $num_mat[1]['numeromatriculas'] != '' ) ? $num_mat[1]['numeromatriculas'] : '0'; ?></td>

                                    <td><?php echo $num_mat[2]['numeromatriculas'] = ( $num_mat[2]['ano'] == '2011' && $num_mat[2]['ent_matricula'] == 'PREF' && $num_mat[2]['numeromatriculas'] != '' ) ? $num_mat[2]['numeromatriculas'] : '0'; ?></td>
                                    <td><?php echo $num_mat[3]['numeromatriculas'] = ( $num_mat[3]['ano'] == '2011' && $num_mat[3]['ent_matricula'] == 'SECR' && $num_mat[3]['numeromatriculas'] != '' ) ? $num_mat[3]['numeromatriculas'] : '0'; ?></td>

                                    <td><?php echo $num_mat[4]['numeromatriculas'] = ( $num_mat[4]['ano'] == '2012' && $num_mat[4]['ent_matricula'] == 'PREF' && $num_mat[4]['numeromatriculas'] != '' ) ? $num_mat[4]['numeromatriculas'] : '0'; ?></td>
                                    <td><?php echo $num_mat[5]['numeromatriculas'] = ( $num_mat[5]['ano'] == '2012' && $num_mat[5]['ent_matricula'] == 'SECR' && $num_mat[5]['numeromatriculas'] != '' ) ? $num_mat[5]['numeromatriculas'] : '0'; ?></td>
 
                                    <td><?php echo $num_mat[6]['numeromatriculas'] = ( $num_mat[6]['ano'] == '2013' && $num_mat[6]['ent_matricula'] == 'PREF' && $num_mat[6]['numeromatriculas'] != '' ) ? $num_mat[6]['numeromatriculas'] : '0'; ?></td>
                                    <td><?php echo $num_mat[7]['numeromatriculas'] = ( $num_mat[7]['ano'] == '2013' && $num_mat[7]['ent_matricula'] == 'SECR' && $num_mat[7]['numeromatriculas'] != '' ) ? $num_mat[7]['numeromatriculas'] : '0'; ?></td>
                                    
                                    <td><?php echo ($num_mat[0]['numeromatriculas'] + $num_mat[2]['numeromatriculas'] + $num_mat[4]['numeromatriculas'] +  $num_mat[6]['numeromatriculas'])/4; ?></td>
                                    <td><?php echo ($num_mat[1]['numeromatriculas'] + $num_mat[3]['numeromatriculas'] + $num_mat[5]['numeromatriculas'] +  $num_mat[7]['numeromatriculas'])/4; ?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>
                <br>
                <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td class="SubTituloCentro" colspan="2">
                            <?php if( $habilitado['BOTAO'] == 'S' ){ ?>
                                <input type="button" name="salvar" value="Salvar" onclick="salvarQuestEJA();"/>
                                <input type="button" name="cancelar" value="Cancelar" onclick="pesquisarServidor('ver');"/>
                            <?php }else{ ?>
                                <input type="button" name="salvar" value="Salvar" disabled="disabled"/>
                                <input type="button" name="cancelar" value="Cancelar" disabled="disabled"/>
                            <?php } ?>

                        </td>
                    </tr>
                </table>
                <br>
                <?php if( $habilitado['PARECER_HAB'] == 'S' ){ ?>
                        <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="98%">
                            <tr>
                                <td colspan="2" class ="SubTituloCentro" style="text-align: left;">Descrição do Parecer</td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                    <?php
                                        $pejresposta = $dadosParecer['pejresposta'];
                                        echo campo_textarea('pejresposta', 'S', $habilitado['DESCRICAO_P'], '', '210', 10, 1000, '', 0, '', false, null);
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td width="10%">
                                    <input type="radio" name="pejsituacao" id="pejsituacao_S" value="S" <?=$habilitado['RADIO_PAREC'];?> > Favorável
                                </td>
                                <td>
                                    <input type="radio" name="pejsituacao" id="pejsituacao_N" value="N" <?=$habilitado['RADIO_PAREC'];?> > Não Favorável
                                </td>
                            </tr>
                        </table>
                        <br>
                        <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="SubTituloCentro" colspan="2">
                                    <input type="button" id="salvarParecer" name="salvarParecer" value="Salvar Parecer" <?=$habilitado['BOTAO_PAREC'];?> />
                                </td>
                            </tr>
                        </table>
                        <br>
                <?php
                        echo litagemParecerEja();

                    }
                ?>
            </td>
            <td align="center" valign="top" width="5%">
                <?php
                    include_once APPRAIZ . "includes/workflow.php";

                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                ?>
            </td>
        </tr>
    </table>
</form>