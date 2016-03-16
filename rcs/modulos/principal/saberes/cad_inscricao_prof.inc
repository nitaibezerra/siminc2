<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Reconhecimento de Saberes e Competências', 'Processo Seletivo para Comissão Avaliadora' );

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_aval_documental&acao=A';
    $parametros     = '';
    //$db->cria_aba($abacod_tela, $url, $parametros);

    $perfil = pegaPerfilGeral($_SESSION['usucpf']);

    if($srpid == ''){
        $srpid = $_REQUEST['srpid'];
    }

    #QUANDO O ACESSADO PELO USUÁRIO PROFESSOR.
    if($_REQUEST['acUsProf'] == 'S' ){
        $srpnumcpf = $_SESSION['usucpf'];
        $sql = "
            SELECT srpid FROM rcs.servidoresprofessor WHERE srpnumcpf = '{$srpnumcpf}';
        ";
        $srpid = $db->pegaUm($sql);
    }

    if( $srpid != '' ){
        $dados = buscarDadosProfessores( $srpid );

        if( $dados['dsatermoaceite01'] = 't' ){
            $checked_01 = 'checked="checked"';
        }else{
            $checked_01 = '';
        }
        if( $dados['dsatermoaceite02'] = 't' ){
            $checked_02 = 'checked="checked"';
        }else{
            $checked_02 = '';
        }
        if( $dados['dsatermoaceite03'] = 't' ){
            $checked_03 = 'checked="checked"';
        }else{
            $checked_03 = '';
        }
        $srptipoavaliador   = $dados['srptipoavaliador'];
        $srptipoavaliado    = $dados['srptipoavaliado'];
    }
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>


<script type="text/javascript">
    $1_11(document).ready(function(){
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11(window).unload(
            $1_11('.chosen-select').chosen()
        );
    });

    function atualizaComboMunicipio( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
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
            url     : window.location.href,
            data    : "requisicao=buscarEndereceCEP&cep="+cep,
            success: function(resp){
                var dados = $.parseJSON(resp);
                $('#srplogradouro').val(dados.logradouro);
                $('#srpbairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function buscarProfessorCPF( cpf ){
        /*
         * NEB = Não Existe na Base;
         * ECB = Existe CPF na Base;
        */
       var srpnumcpf = replaceAll( replaceAll( cpf, ".", "" ), "-", "" );

        if( srpnumcpf != '' ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=buscarProfessorCPF&srpnumcpf="+srpnumcpf,
                success: function(resp){
                    var dados = $.parseJSON(resp);

                    if(dados.tp_result == 'ECB' ){
                        if(dados.srptipoavaliador == 't'){
                            $('.container_localizacao').fadeIn();
                            $('.termo_compromisso').fadeIn();
                        }

                        $.each(dados, function( index, value ){
                            if( index == 'srptipoavaliador' ){
                                if( value == 't' ){
                                    $('#srptipoavaliador_s').attr('checked', true);
                                }else
                                    if( value == 'f' ){
                                    $('#srptipoavaliador_n').attr('checked', true);
                                }else{
                                    $('#srptipoavaliador_s').attr('checked', false);
                                    $('#srptipoavaliador_n').attr('checked', false);
                                }
                            }

                            if( index == 'srptipoavaliado' ){
                                if( value == 't' ){
                                    $('#srptipoavaliado_s').attr('checked', true);
                                }else
                                    if( value == 'f' ){
                                        $('#srptipoavaliado_n').attr('checked', true);
                                    }else{
                                        $('#srptipoavaliado_s').attr('checked', false);
                                        $('#srptipoavaliado_n').attr('checked', false);
                                    }
                            }

                            if( index == 'srplotacao' ){
                                var selectValue = $('#srplotacao').find('option[selected]').html();
                                $('#td_combo_lotacao').find('a > span').html(selectValue);
                            }

                        });

                        if( srpnumcpf != '' ){
                            $.ajax({
                                type    : "POST",
                                url     : window.location.href,
                                data    : "requisicao=buscarGraduacaoProf&srpnumcpf="+srpnumcpf,
                                success: function( resp ){
                                    $('#td_graduacao').html(resp);
                                }
                            });

                            $.ajax({
                                type    : "POST",
                                url     : window.location.href,
                                data    : "requisicao=buscarEspecializacaoProf&srpnumcpf="+srpnumcpf,
                                success: function( resp ){
                                    $('#td_especializacao').html(resp);
                                }
                            });

                            $.ajax({
                                type    : "POST",
                                url     : window.location.href,
                                data    : "requisicao=buscarMestradoProf&srpnumcpf="+srpnumcpf,
                                success: function( resp ){
                                    $('#td_mestrado').html(resp);
                                }
                            });

                            $.ajax({
                                type    : "POST",
                                url     : window.location.href,
                                data    : "requisicao=buscarDoutoradoProf&srpnumcpf="+srpnumcpf,
                                success: function( resp ){
                                    $('#td_doutorado').html(resp);
                                }
                            });
                        }

                        if( dados.permicao == 'naoAutorizado' ){
                            $('#dsamatricula').attr('readonly', true);
                            $('#srplotacao').attr('readonly', true);
                            $('#srplotacao_chosen').addClass('chosen-disabled');
                            $('#texto_info_alt_siape').css("display", "");
                        }
                    }

                    if(dados.tp_result == 'NEB' ){
                        limparFormulario();

                        $('#dsamatricula').attr('disabled', false);
                        $('#srplotacao').attr('disabled', false);
                        $('#srplotacao_chosen').removeClass('chosen-disabled');
                        $('#texto_info_alt_siape').css("display", "none");

                        $.ajax({
                            url : '/includes/webservice/cpf.php',
                            type: 'POST',
                            data: {
                                ajaxCPF: srpnumcpf
                            },
                            dataType: "html",
                            success : function ( data ){
                                pessoaFisica = formatarDadosPessoaFisicaReceita(data);

                                console.log(pessoaFisica);

                                if(typeof(pessoaFisica['no_pessoa_rf']) == "undefined"){
                                    alert('O CPF informado não foi encontrado na base de dados da Receita federal.');
                                    return false;
                                }
                                else {
                                    var srpnumcpf   = pessoaFisica['nu_cpf_rf'];
                                    var srpdsc      = pessoaFisica['no_pessoa_rf'];
                                    var dtNsc       = pessoaFisica['dt_nascimento_rf'];

                                    //#ENDEREÇO
                                    var srpcep          = pessoaFisica['nu_cep'];
                                    var srplogradouro   = pessoaFisica['ds_logradouro'];
                                    var srpcomplemento  = pessoaFisica['ds_logradouro_comp'];
                                    var srpnumero       = pessoaFisica['ds_numero'];
                                    var srpbairro       = pessoaFisica['ds_bairro'];
                                    var estuf           = pessoaFisica['sg_uf'];
                                    var muncod          = pessoaFisica['co_cidade'];

                                    //#FONEM PESSOAL - CONCIDENRANDO COMO CELULAR.
                                    var srptelcelular       = pessoaFisica['ds_contato_pessoa'];
                                    var srptdddelcelular    = srptelcelular.substring(0, 2);

                                    //#TRATAMENTO COM AS VARIAVEIS. QUANDO NECESSÁRIO
                                    srpnumcpf = srpnumcpf.substring(0, 3)+'.'+srpnumcpf.substring(3, 6)+'.'+srpnumcpf.substring(6, 9)+'-'+srpnumcpf.substring(9, 11);
                                    dtNsc = dtNsc.substring(6, 8)+'/'+dtNsc.substring(4, 6)+'/'+dtNsc.substring(0, 4);
                                    srpcep = srpcep.substring(0, 5)+'-'+srpcep.substring(5, 8);
                                    muncod = muncod.substring(8, 15);
                                    
                                    srptelcelular = srptelcelular.substring(3, 7) +'-'+ srptelcelular.substring(7, 11);

                                    //#ATRIBUINDO OS VALORES NAS VARIAVEIS
                                    $('#srpnumcpf').val( srpnumcpf );
                                    $('#srpdsc').val( srpdsc );
                                    $('#srpdtnascimento').val( dtNsc );
                                    
                                    $('#srpcep').val( srpcep );
                                    $('#srplogradouro').val( srplogradouro );
                                    $('#srpcomplemento').val( srpcomplemento );
                                    $('#srpnumero').val( srpnumero );
                                    $('#srpbairro').val( srpbairro );
                                    $('#estuf').val( estuf );
                                    $('#muncod').val( muncod );
                                    
                                    $('#srptdddelcelular').val( srptdddelcelular );
                                    $('#srptelcelular').val( srptelcelular );
                                }
                            }

                        });
                    }
                }
            });
        }
    }

    function limparFormulario(){
        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }

            if( $(this).attr('type') == 'select-multiple' ){

                if( $(this).attr('id') == 'grdid' ){
                    var obj = document.getElementById('grdid');
                    for (var i=0; i < obj.options.length; i++) {
                        obj.options[i] = null;
                    }
                    $('#grdid').append( $("<option/>", {value:'', text: 'Duplo clique para selecionar da lista'}) );
                }

                if( $(this).attr('id') == 'espid' ){
                    var obj = document.getElementById('espid');
                    for (var i=0; i < obj.options.length; i++) {
                        obj.options[i] = null;
                    }
                    $('#espid').append( $("<option/>", {value:'', text: 'Duplo clique para selecionar da lista'}) );
                }

                if( $(this).attr('id') == 'mesid' ){
                    var obj = document.getElementById('mesid');
                    for (var i=0; i < obj.options.length; i++) {
                        obj.options[i] = null;
                    }
                    $('#mesid').append( $("<option/>", {value:'', text: 'Duplo clique para selecionar da lista'}) );
                }

                if( $(this).attr('id') == 'douid' ){
                    var obj = document.getElementById('douid');
                    for (var i=0; i < obj.options.length; i++) {
                        obj.options[i] = null;
                    }
                    $('#douid').append( $("<option/>", {value:'', text: 'Duplo clique para selecionar da lista'}) );
                }
            }
        });
    }


    function formatarDadosPessoaFisicaReceita(text){
        var pessoaFisica = [];
        linhaItem = text.split('|');
        for (i=0; i < linhaItem.length; i++){
            colunaItem = linhaItem[i].split('#');
            pessoaFisica[colunaItem[0]] = colunaItem[1];
        }

        return pessoaFisica;
    }

    function salvarDadosProfessores(){
        var erro = 0;
        var campos = '';
        var formulario = document.formulario;

        var extid_tec_1 = $('#extid_tec_1');
        var extid_tec_2 = $('#extid_tec_2');
        var extid_tec_3 = $('#extid_tec_3');

        var extid_cog_1 = $('#extid_cog_1');
        var extid_cog_2 = $('#extid_cog_2');
        var extid_cog_3 = $('#extid_cog_3');

        var srptelcomercial     = $('#srptelcomercial');
        var srptelresidencial   = $('#srptelresidencial');
        var srptelcelular       = $('#srptelcelular');

        $('#salvar').attr('disabled', true);

        $.each($(".obrigatorio"), function(i, v){
            // VALIDANDO SOMENTE OS CAMPOS QUE ESTAO EXIBINDO NA TELA.
            if($(this).closest('tr').is(':visible') && $(this).closest('table').is(':visible')){

                if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                    if ( $(v).val() == '' ){
                        erro = 1;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
                }else
                    if( $(this).attr('type') == 'radio' ){
                        var name  = $(this).attr('name');
                        var value = $(this).attr('value');
                        var radio_box = $('input:radio[name='+name+']:checked');

                        if(!radio_box.val()){
                            erro = 1;
                            if(value == 'S'){
                                campos += '- ' + $(this).attr('title') + " \n";
                            }
                        }
                    }else
                        if( $(this).attr('type') == 'checkbox' ){
                            var name  = $(this).attr('name');
                            var value = $(this).attr('value');
                            var check_box = $('input:checkbox[name='+name+']:checked');

                            if(!check_box.val()){
                                erro = 1;
                                if(value == 'S'){
                                    campos += '- ' + $(this).attr('title') + " \n";
                                }
                            }
                    }
            }
        });

        if( srptelcomercial.val() ){
            var result = ValidaTelefone( srptelcomercial, 'S' );
            if( result == 'f' ){
                erro = 1;
                campos += '- Verifique o telefone Comercial!' + "\n";
            }
        }

        if( srptelresidencial.val() ){
            var result = ValidaTelefone( srptelresidencial, 'S' );
            if( result == 'f' ){
                erro = 1;
                campos += '- Verifique o telefone Residencial!' + "\n";
            }
        }

        if( srptelcelular.val() ){
            var result = ValidaTelefone( srptelcelular, 'S' );
            if( result == 'f' ){
                erro = 1;
                campos += '- Verifique o telefone Celular!' + "\n";
            }
        }

        if( !( extid_tec_1.val() || extid_tec_2.val() || extid_tec_3.val() ) && !( extid_cog_1.val() || extid_cog_2.val() || extid_cog_3.val() ) ){
            erro = 1;
            campos += '- Eixos Tecnológicos e/ou Eixos Cognitivos' + "\n";
        }

        selectAllOptions( formulario.grdid );
        selectAllOptions( formulario.espid );
        selectAllOptions( formulario.mesid );
        selectAllOptions( formulario.douid );

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            $('#salvar').attr('disabled', false);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosProfessores');
            $('#formulario').submit();
        }
    }

    function atualizaComboEixoTec_2( extid_tec_1 ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboEixoTec_2&extid_tec_1="+extid_tec_1,
            async: false,
            success: function(resp){
                $('#td_eixo_tec_2').html(resp);
            }
        });
    }

    function atualizaComboEixoTec_3( extid_tec_2 ){
        var extid_tec_1 = $('#extid_tec_1').val();
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboEixoTec_3&extid_tec_1="+extid_tec_1+"&extid_tec_2="+extid_tec_2,
            async: false,
            success: function(resp){
                $('#td_eixo_tec_3').html(resp);
            }
        });
    }

    function atualizaComboEixoCog_2( extid_cog_1 ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboEixoCog_2&extid_cog_1="+extid_cog_1,
            async: false,
            success: function(resp){
                $('#td_eixo_cog_2').html(resp);
            }
        });
    }

    function atualizaComboEixoCog_3( extid_cog_2 ){
        var extid_cog_1 = $('#extid_cog_1').val();
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboEixoCog_3&extid_cog_1="+extid_cog_1+"&extid_cog_2="+extid_cog_2,
            async: false,
            success: function(resp){
                $('#td_eixo_cog_3').html(resp);
            }
        });
    }

    function ValidaTelefone(tel, ops){
	var exp = /\d{4,5}\-\d{4}/;
        var id;
        
        if(ops == 'C'){
            id = tel.id;
        }else{
            id = tel.attr('id');
        }
        
        var tel = $('#'+id).val();
        var ddd = $('#'+id).prev('input').val();

        if( tel != '' ){
            if( ddd == '' ){
                alert('O campo DDD está em branco é necessário o preenchimento do DDD!');
                return 'f';
            }
            if( !exp.test( tel ) ){
                if(ops == 'C'){
                    alert('Numero de Telefone Invalido!');
                }
                return 'f';
            }else{
                return 't';
            }
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'rcs.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }

    function verificarTipoAvaliacao( opc ){
        if( opc != 'V' ){
            if( opc == 'S' ){
                $('.container_localizacao').fadeIn();
                $('.termo_compromisso').fadeIn();
            } else {
                $('.container_localizacao').hide();
                $('.termo_compromisso').hide();
            }
        }else{
            if($('input[name=srptipoavaliador]:checked').val() == 't'){
                $('.container_localizacao').fadeIn();
                $('.termo_compromisso').fadeIn();
            }else{
                $('.container_localizacao').hide();
                $('.termo_compromisso').hide();
            }
        }
    }
    
    function verificaUsusarioAtivo(){
        var srpnumcpf = replaceAll( replaceAll( $('#srpnumcpf').val(), ".", "" ), "-", "" );
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=verificaUsusarioAtivo&srpnumcpf="+srpnumcpf,
            success: function(resp){
                if(resp == 'NP'){
                    alert('Não é possivél ativar esse usuário, já existe um outro cadastro para esse usuário ativo!');
                    $('#salvar').attr('disabled', true);
                    $('#srpstatus_i').attr("checked", "checked");
                }
            }
        });
    }

</script>

<?PHP
    if( $dados && $dados['srptipoavaliador'] == 't' && in_array(PERFIL_RCS_INTERLOCUTOR, $perfil) ){
        $habilita = 'N';
    } else if( in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
        $cpf = $_SESSION['usucpf'];
        $verificaExisteUnd = verificaUnidadesAssociadasProfessor($cpf);

        if( $verificaExisteUnd == 'S' ){
            $habilita = 'S';
        }else{
            $habilita = 'N';
        }
    }else{
        $cpf = $dados['srpnumcpf'];
        $habilita = 'S';
    }
    
    if( $cpf != '' ){
       buscaUnidadesAssociadas( $cpf );
    }
?>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="srpid" name="srpid" value="<?=$srpid?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <?PHP
            if( $dados['srpstatus'] == 'A' ){
                $status_a = 'checked="checked"';
                $status_i = '';
            }else if( $dados['srpstatus'] == 'I' ){
                $status_a = '';
                $status_i = 'checked="checked"';
            }else{
                $status_a = 'checked="checked"';
            }
            if( ( in_array(PERFIL_RCS_ADMINISTRADOR, $perfil) || in_array(PERFIL_RCS_SUPER_USUARIO, $perfil) ) ){
        ?>
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;">Status Usuário Professor</td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%">Professor Ativo/Inativo?</td>
            <td colspan="2">
                <input type="radio" id="srpstatus_a" name="srpstatus" value="A" <?=$status_a;?> onclick="verificaUsusarioAtivo();"> Ativo
                &nbsp;&nbsp;&nbsp;
                <input type="radio" id="srpstatus_i"name="srpstatus" value="I" <?=$status_i;?>> Inativo
            </td>
        </tr>
        <?PHP
            }
        ?>
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;">Dados de Avaliação</td>
        </tr>
        <?PHP
            // Se for perfil professor, nao é para exibir o campo "Professor sera avaliado" e deixar marcado por padrao como true.
            if( $habilita == 'N' ){
                $disabled = 'disabled="disabled"';
            }else{
                $disabled = '';
            }

            if( $srptipoavaliador == '' ){
                if( in_array( PERFIL_RCS_INTERLOCUTOR, $perfil ) ){
                    $checked_sim = 'checked="checked"';
                    $checked_nao = '';
                }else{
                    $checked_sim = '';
                    $checked_nao = 'checked="checked"';
                }
            }else{
                if( $srptipoavaliador == 't' ){
                    $checked_sim = 'checked="checked"';
                    $checked_nao = '';
                }else{
                    $checked_sim = '';
                    $checked_nao = 'checked="checked"';
                }
            }

            if( $srptipoavaliado != '' ){
                if( $srptipoavaliado == 't' ){
                    $checked_aval_sim = 'checked="checked"';
                    $checked_aval_nao = '';
                }else{
                    $checked_aval_sim = '';
                    $checked_aval_nao = 'checked="checked"';
                }
            }else{
                $checked_aval_sim = 'checked="checked"';
                $checked_aval_nao = '';
            }

            if( in_array( PERFIL_RCS_ADM_INSTITUCIONAL, $perfil ) || in_array( PERFIL_RCS_INTERLOCUTOR, $perfil ) || in_array(PERFIL_RCS_ADMINISTRADOR, $perfil) ){
        ?>
                <tr>
                    <td class="SubTituloDireita" width="25%">Professor será avaliador?</td>
                    <td colspan="2">
                        <input type="radio" name="srptipoavaliador" id="srptipoavaliador_s" value="t" class="obrigatorio" onclick="verificarTipoAvaliacao('S');" <?=$disabled;?> <?=$checked_sim;?> title="Professor será avaliador"> Sim
                        &nbsp;&nbsp;&nbsp;
                        <input type="radio" name="srptipoavaliador" id="srptipoavaliador_n" value="f" class="obrigatorio" onclick="verificarTipoAvaliacao('N');" <?=$disabled;?> <?=$checked_nao;?>> Não
                        <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                    </td>
                </tr>
        <?PHP } ?>

        <tr>
            <td class="SubTituloDireita" width="25%">Professor será avaliado?</td>
            <td colspan="2">
                <input <?=$disabled;?> <?=$checked_aval_sim;?> type="radio" name="srptipoavaliado" id="srptipoavaliado_s" value="t" class="obrigatorio" title="Professor será avaliado"/> Sim
                &nbsp;&nbsp;&nbsp;
                <input <?=$disabled;?> <?=$checked_aval_nao;?> type="radio" name="srptipoavaliado" id="srptipoavaliado_n" value="f" class="obrigatorio" /> Não
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Dados Pessoais </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    #CASO O USUÁRIO TENHA O PERFIL PROFESSOR. É PERMITIDO QUE ELE CADASTRE APENAS O PROPRIO CPF;
                    if( in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
                        $dados['srpnumcpf'] = mascara_global($_SESSION['usucpf'], '###.###.###-##' ) ;
                    }
                    $srpnumcpf = $dados['srpnumcpf'];
                    echo campo_texto('srpnumcpf', 'S', $habilita, 'CPF', 14, 14, '###.###.###-##', '', '', '', 0, 'id="srpnumcpf"', '', $srpnumcpf, 'buscarProfessorCPF(this.value)', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Matricula SIAPE: </td>
            <td colspan="2">
                <?PHP
                    $dsamatricula = $dados['dsamatricula'];
                    echo campo_texto('dsamatricula', 'S', $habilita, 'Matricula SIAPE', 14, 7, '#######', '', '', '', 0, 'id="dsamatricula"', '', $dsamatricula, '', null);
                ?>
                <span id="texto_info_alt_siape" style="margin-left:6px; display:none; color:red;">
                    CAMPO SIAPE É BLOQUEADO PARA ALTERAÇÃO. PARA CADASTRAR O PROFESSOR COM UM NOVO SIAPE E LOTAÇÃO ENTRE EM CONTATO COM OS GESTORES DO SISTEMA.
                </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Candidato: </td>
            <td colspan="2">
                <?PHP
                    $srpdsc = $dados['srpdsc'];
                    echo campo_texto('srpdsc', 'S', $habilita, 'Candidato', 60, 100, '', '', '', '', 0, 'id="srpdsc"', '', $srpdsc, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Data de Nascimento: </td>
            <td colspan="2">
                <?PHP
                    $srpdtnascimento = $dados['srpdtnascimento'];
                    echo campo_data2('srpdtnascimento', 'S', $habilita, 'Data de Nascimento', '##/##/####', '', '', $srpdtnascimento, '', '', 'srpdtnascimento');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Lotação: </td>
            <td id="td_combo_lotacao" colspan="2">
                <?PHP
                    $srplotacao = $dados['srplotacao'];
                    $sql = "
                        SELECT  DISTINCT ee.entid as codigo,
                                CASE WHEN entsig <> ''
                                    THEN UPPER(entsig)||' - '||initcap(ee.entnome)
                                    ELSE initcap(ee.entnome)
                                END as descricao
                        FROM entidade.entidade ee
                        INNER JOIN entidade.funcaoentidade efe ON efe.entid = ee.entid
                        INNER JOIN rcs.orgaofuncao rof ON rof.funid = efe.funid
                        INNER JOIN rcs.orgao ro ON ro.orgid = rof.orgid AND ro.orgstatus = 'A'
                        ORDER BY descricao
                    ";
                    $db->monta_combo('srplotacao', $sql, 'S', "Digite a Lotação procurada...", '', '', '', 400, 'S', 'srplotacao', '', $srplotacao, 'Lotação', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> E-mail: </td>
            <td colspan="2">
                <?PHP
                    $srpemail = $dados['srpemail'];
                    echo campo_texto('srpemail', 'N', $habilita, 'E-mail', 60, 100, '', '', '', '', 0, 'id="srpemail"', '', $srpemail, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Link Currículo Lattes: </td>
            <td>
                <?PHP
                    $srpcurriculolattes = $dados['srpcurriculolattes'];
                    echo campo_texto('srpcurriculolattes', 'N', $habilita, 'Link Currículo Lattes', 60, 100, '', '', '', '', 0, 'id="srpcurriculolattes"', '', $srpcurriculolattes, '', null);
                ?>
            </td>
            <td width="47%">
                <!-- Clique aqui caso não possua currículo na Plataforma Lattes: -->
                <input type="button"  name="lattes" id="lattes" value="Clique aqui caso não possua currículo na Plataforma Lattes" onclick="window.open('http://lattes.cnpq.br/');">
            </td>
        </tr>

        <tr class="container_localizacao">
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Localização </td>
        </tr>

        <tr class="container_localizacao">
            <td class ="SubTituloDireita"> CEP: </td>
            <td colspan="2">
               <?PHP
                    $srpcep = $dados['srpcep'];
                    echo campo_texto('srpcep', 'S', $habilita, 'CEP', 14, 9, '#####-###', '', 'left', '', 0, 'id="srpcep"', '', $srpcep, 'buscarEndereceCEP(this.value)', '', null);
                ?>
            </td>
        </tr>
        <tr class="container_localizacao">
            <td class ="SubTituloDireita"> Logradouro: </td>
            <td colspan="2">
               <?PHP
                    $srplogradouro = stripslashes( $dados['srplogradouro'] );
                    echo campo_texto('srplogradouro', 'S', $habilita, 'Logradouro', 60, 100, '', '', 'left', '', 0, 'id="srplogradouro"', '', $srplogradouro, null, '', null);
                ?>
            </td>
        </tr>
        <tr class="container_localizacao">
            <td class ="SubTituloDireita"> Complemento: </td>
            <td colspan="2">
               <?PHP
                    $srpcomplemento = $dados['srpcomplemento'];
                    echo campo_texto('srpcomplemento', 'N', $habilita, 'Complemento', 60, 100, '', '', 'left', '', 0, 'id="srpcomplemento"', '', $srpcomplemento, null, '', null);
                ?>
            </td>
        </tr>
        <tr class="container_localizacao">
            <td class ="SubTituloDireita"> Número: </td>
            <td colspan="2">
               <?PHP
                    $srpnumero = $dados['srpnumero'];
                    echo campo_texto('srpnumero', 'N', $habilita, '', 14, 6, '######', '', 'left', '', 0, 'id="srpnumero"', '', $srpnumero, null, '', null);
                ?>
            </td>
        </tr>
        <tr  class="container_localizacao">
            <td class ="SubTituloDireita"> Bairro: </td>
            <td colspan="2">
               <?PHP
                    $srpbairro = $dados['srpbairro'];
                    echo campo_texto('srpbairro', 'S', $habilita, 'Bairro', 60, 100, '', '', 'left', '', 0, 'id="srpbairro"', '', $srpbairro, null, '', null);
                ?>
            </td>
        </tr>
        <tr  class="container_localizacao">
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
                <?PHP
                    $estuf = $dados['estuf'];
                    $sql = "
                         SELECT  estuf as codigo,
                                 estuf as descricao
                         FROM territorios.estado
                         ORDER BY estuf
                     ";
                     $db->monta_combo('estuf', $sql, $habilita, "Selecione...", 'atualizaComboMunicipio', '', '', 180, 'S', 'estuf', '', $estuf, 'UF');
                ?>
            </td>
        </tr>
        <tr  class="container_localizacao">
            <td class ="SubTituloDireita"> Município: </td>
            <td id="td_combo_municipio" colspan="2">
                <?PHP
                    $muncod = $dados['muncod'];
                    $sql = "
                        SELECT  muncod AS codigo,
                                mundescricao AS descricao
                        FROM territorios.municipio
                        ORDER BY descricao
                    ";
                    $db->monta_combo('muncod', $sql, $habilita, "Selecione...", '', '', '', 180, 'S', 'muncod', '', $muncod, 'Município');
                ?>
            </td>
        </tr>
        <tr  class="container_localizacao">
            <td class ="SubTituloDireita"> Telefone Comercial: </td>
            <td colspan="2">
               <?PHP
                    $srpdddtelcomercial     = $dados['srpdddtelcomercial'];
                    $srptelcomercial        = $dados['srptelcomercial'];

                    echo campo_texto('srpdddtelcomercial', 'N', $habilita, '', 1, 2, '##', '', 'left', '', 0, 'id="srpdddtelcomercial"', '', $srpdddtelcomercial, null, '', null);
                    echo '&nbsp';
                    echo campo_texto('srptelcomercial', 'N', $habilita, 'Telefone. Digite apenas o número!', 14, 10, '#####-####', '', 'left', '', 0, 'id="srptelcomercial"', '', $srptelcomercial, 'ValidaTelefone(this, \'C\')', '', null);
                ?>
                <span style="font-size:10px;color:#00008B;"> Ex: |61| 12345-1234</span>
            </td>
        </tr>
        <tr  class="container_localizacao">
            <td class ="SubTituloDireita"> Telefone Residencial: </td>
            <td colspan="2">
               <?PHP
                    $srpdddtelresidencial   = $dados['srpdddtelresidencial'];
                    $srptelresidencial      = $dados['srptelresidencial'];

                    echo campo_texto('$srpdddtelresidencial', 'N', $habilita, '', 1, 2, '##', '', 'left', '', 0, 'id="$srpdddtelresidencial"', '', $srpdddtelresidencial, null, '', null);
                    echo '&nbsp';
                    echo campo_texto('srptelresidencial', 'N', $habilita, 'Telefone. Digite apenas o número!', 14, 10, '#####-####', '', 'left', '', 0, 'id="srptelresidencial"', '', $srptelresidencial, 'ValidaTelefone(this, \'C\')', '', null);
                ?>
                <span style="font-size:10px;color:#00008B;"> Ex: |61| 12345-1234</span>
            </td>
        </tr>
        <tr  class="container_localizacao">
            <td class ="SubTituloDireita"> Celular: </td>
            <td colspan="2">
               <?PHP
                    $srptdddelcelular   = $dados['srptdddelcelular'];
                    $srptelcelular      = $dados['srptelcelular'];

                    echo campo_texto('srptdddelcelular', 'N', $habilita, '', 1, 2, '##', '', 'left', '', 0, 'id="srptdddelcelular"', '', $srptdddelcelular, null, '', null);
                    echo '&nbsp';
                    echo campo_texto('srptelcelular', 'N', $habilita, 'Celular. Digite apenas o número!', 14, 10, '#####-####', '', 'left', '', 0, 'id="srptelcelular"', '', $srptelcelular, 'ValidaTelefone(this, \'C\')', '', null);
                ?>
                <span style="font-size:10px;color:#00008B;"> Ex: |61| 12345-1234</span>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Formação Academica - Área de Formação</td>
        </tr>

        <tr>
            <td class ="SubTituloDireita"> Graduação: </td>
            <td id="td_graduacao" colspan="2">
                <?PHP
                    if( $srpid != '' ){
                        $sql = "
                             SELECT i.grdid as codigo,
                                    g.grddsc as descricao
                             FROM rcs.itemgraduacao i
                             RIGHT JOIN rcs.graduacao g ON g.grdid = i.grdid
                             WHERE i.srpid = {$srpid}
                             ORDER BY g.grddsc
                        ";
                        $grdid = $db->carregar($sql);
                    }
                    $sql = "
                         SELECT  grdid as codigo,
                                 grddsc as descricao
                         FROM rcs.graduacao
                         WHERE 1 = 1
                         ORDER BY grddsc
                     ";
                    #codigo é o campo que vai ser usado para fazer a busca. descricao é o nome do campo ou titulo do campo.
                    $arrayCampoPesquisa = array(
                        Array(
                            'codigo' => 'grddsc',
                            'descricao' => 'Graduação'
                        )
                    );
                    combo_popup('grdid', $sql, 'Graduação', '400x400', '0', '', '', $habilita, false, false, 3, 550 , null, null, false, $arrayCampoPesquisa, $grdid, true, true, "", false, null , null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Especialização: </td>
            <td id="td_especializacao" colspan="2">
                <?PHP
                    if( $srpid != '' ){
                        $sql = "
                            SELECT  i.sbcid as codigo,
                                    a.arcdsc ||' - '||s.sbcdsc AS descricao
                            FROM rcs.itemcursopos AS i

                            JOIN rcs.subareacurso AS s ON s.sbcid = i.sbcid
                            JOIN rcs.areacurso AS a ON a.arcid = s.arcid

                            WHERE i.icptipo = 'E' AND i.srpid = {$srpid}
                            ORDER BY descricao
                        ";
                        $resp_espid = $db->carregar($sql);
                    }

                    $sql_1 = "
                         SELECT  arcid as codigo,
                                 arcdsc as descricao
                         FROM rcs.areacurso
                         ORDER BY arcdsc
                     ";
                    $sql_2 = "
                         SELECT  sbcid as codigo,
                                 sbcdsc as descricao
                         FROM rcs.subareacurso
                     ";

                    $sql = array(
                        'combo_1'=>
                            array(
                                'titulo_combo' => 'Área - Cursos',
                                'sql_combo' => $sql_1,
                                'name_combo' => 'arcid',
                                'id_combo' => 'arcid'
                            ),
                        'combo_2'=>
                            array(
                                'titulo_combo' => 'Sub área - Cursos',
                                'sql_combo' => $sql_2,
                                'where'=> 'WHERE arcid = %s',
                                'orderby'=> 'ORDER BY sbcdsc;',
                                'name_combo' => 'sbcid',
                                'id_combo' => 'sbcid'
                        )
                    );
                    select_popup('espid', $sql, 'Especialização', '300x680', $habilita, 3, 550, null, $resp_espid, $funcao);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Mestrado: </td>
            <td id="td_mestrado" colspan="2">
                <?PHP
                    if( $srpid != '' ){
                        $sql = "
                            SELECT  i.sbcid as codigo,
                                    a.arcdsc ||' - '||s.sbcdsc AS descricao
                            FROM rcs.itemcursopos AS i

                            JOIN rcs.subareacurso AS s ON s.sbcid = i.sbcid
                            JOIN rcs.areacurso AS a ON a.arcid = s.arcid

                            WHERE i.icptipo = 'M' AND i.srpid = {$srpid}
                            ORDER BY descricao
                        ";
                        $resp_mesid = $db->carregar($sql);
                    }
                    $sql_1 = "
                         SELECT  arcid as codigo,
                                 arcdsc as descricao
                         FROM rcs.areacurso
                         ORDER BY arcdsc
                     ";
                    $sql_2 = "
                         SELECT  sbcid as codigo,
                                 sbcdsc as descricao
                         FROM rcs.subareacurso
                     ";

                    $sql = array(
                        'combo_1'=>
                            array(
                                'titulo_combo' => 'Área - Cursos',
                                'sql_combo' => $sql_1,
                                'name_combo' => 'arcid',
                                'id_combo' => 'arcid'
                            ),
                        'combo_2'=>
                            array(
                                'titulo_combo' => 'Sub área - Cursos',
                                'sql_combo' => $sql_2,
                                'where'=> 'WHERE arcid = %s',
                                'orderby'=> 'ORDER BY sbcdsc;',
                                'name_combo' => 'sbcid',
                                'id_combo' => 'sbcid'
                        )
                    );
                    select_popup('mesid', $sql, 'Mestrado', '300x680', $habilita, 3, 550, null, $resp_mesid, $funcao);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Doutorado: </td>
            <td id="td_doutorado" colspan="2">
                <?PHP
                    if( $srpid != '' ){
                        $sql = "
                            SELECT  i.sbcid as codigo,
                                    a.arcdsc ||' - '||s.sbcdsc AS descricao
                            FROM rcs.itemcursopos AS i

                            JOIN rcs.subareacurso AS s ON s.sbcid = i.sbcid
                            JOIN rcs.areacurso AS a ON a.arcid = s.arcid

                            WHERE i.icptipo = 'D' AND i.srpid = {$srpid}
                            ORDER BY descricao
                        ";
                        $resp_douid = $db->carregar($sql);
                    }
                   $sql_1 = "
                         SELECT  arcid as codigo,
                                 arcdsc as descricao
                         FROM rcs.areacurso
                         ORDER BY arcdsc
                     ";
                    $sql_2 = "
                         SELECT  sbcid as codigo,
                                 sbcdsc as descricao
                         FROM rcs.subareacurso
                     ";

                    $sql = array(
                        'combo_1'=>
                            array(
                                'titulo_combo' => 'Área - Cursos',
                                'sql_combo' => $sql_1,
                                'name_combo' => 'arcid',
                                'id_combo' => 'arcid'
                            ),
                        'combo_2'=>
                            array(
                                'titulo_combo' => 'Sub área - Cursos',
                                'sql_combo' => $sql_2,
                                'where'=> 'WHERE arcid = %s',
                                'orderby'=> 'ORDER BY sbcdsc;',
                                'name_combo' => 'sbcid',
                                'id_combo' => 'sbcid'
                        )
                    );
                    select_popup('douid', $sql, 'Doutorado', '300x680', $habilita, 3, 550, null, $resp_douid, $funcao);
                ?>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Área de Atuação - Eixos Tecnológicos </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita"> 1º Eixo: </td>
            <td id="td_eixo_tec_1" colspan="2">
                <?PHP
                    $extid_tec_1 = $dados['extid_tec_1'];
                    $sql = "
                         SELECT  extid as codigo,
                                 extdsc as descricao
                         FROM rcs.eixotecnologico
                         WHERE exttipo = 'T'
                         ORDER BY extdsc
                     ";
                     $db->monta_combo('extid_tec_1', $sql, $habilita, "Selecione...", 'atualizaComboEixoTec_2', '', '', 420, 'N', 'extid_tec_1', '', $extid_tec_1, '1º Eixo - Eixos Tecnológicos');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> 2º Eixo: </td>
            <td id="td_eixo_tec_2" colspan="2">
                <?PHP
                    $extid_tec_2 = $dados['extid_tec_2'];
                    $sql = "
                         SELECT  extid as codigo,
                                 extdsc as descricao
                         FROM rcs.eixotecnologico
                         WHERE exttipo = 'T'
                         ORDER BY extdsc
                     ";
                     $db->monta_combo('extid_tec_2', $sql, $habilita, "Selecione...", 'atualizaComboEixoTec_3', '', '', 420, 'N', 'extid_tec_2', '', $extid_tec_2, '2º Eixo');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> 3º Eixo: </td>
            <td id="td_eixo_tec_3" colspan="2">
                <?PHP
                    $extid_tec_3 = $dados['extid_tec_3'];
                    $sql = "
                        SELECT  extid as codigo,
                                extdsc as descricao
                        FROM rcs.eixotecnologico
                        WHERE exttipo = 'T'
                        ORDER BY extdsc
                     ";
                     $db->monta_combo('extid_tec_3', $sql, $habilita, "Selecione...", '', '', '', 420, 'N', 'extid_tec_3', '', $extid_tec_3, '3º Eixo');
                ?>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Área de Atuação - Eixos Cognitivos </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita"> 1º Eixo: </td>
            <td id="td_eixo_cog_1" colspan="2">
                <?PHP
                    $extid_cog_1 = $dados['extid_cog_1'];
                    $sql = "
                         SELECT  extid as codigo,
                                 extdsc as descricao
                         FROM rcs.eixotecnologico
                         WHERE exttipo = 'C'
                         ORDER BY extdsc
                     ";
                     $db->monta_combo('extid_cog_1', $sql, $habilita, "Selecione...", 'atualizaComboEixoCog_2', '', '', 420, 'N', 'extid_cog_1', '', $extid_cog_1, '1º Eixo - Eixos Cognitivos');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> 2º Eixo: </td>
            <td id="td_eixo_cog_2" colspan="2">
                <?PHP
                    $extid_cog_2 = $dados['extid_cog_2'];
                    $sql = "
                         SELECT  extid as codigo,
                                 extdsc as descricao
                         FROM rcs.eixotecnologico
                         WHERE exttipo = 'C'
                         ORDER BY extdsc
                     ";
                     $db->monta_combo('extid_cog_2', $sql, $habilita, "Selecione...", 'atualizaComboEixoCog_3', '', '', 420, 'N', 'extid_cog_2', '', $extid_cog_2, '2º Eixo');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> 3º Eixo: </td>
            <td id="td_eixo_cog_3" colspan="2">
                <?PHP
                    $extid_cog_3 = $dados['extid_cog_3'];
                    $sql = "
                         SELECT  extid as codigo,
                                 extdsc as descricao
                         FROM rcs.eixotecnologico
                         WHERE exttipo = 'C'
                         ORDER BY extdsc
                     ";
                     $db->monta_combo('extid_cog_3', $sql, $habilita, "Selecione...", '', '', '', 420, 'N', 'extid_cog_3', '', $extid_cog_3, '3º Eixo');
                ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela termo_compromisso">
        <tr>
            <td align="center" colspan="3">
                <b style="font-size: 18px; padding: 25px;">Termo de Compromisso e Responsabiliade</b>
            </td>
        </tr>
        <tr>
            <td width="12%" style="text-align: right;">
                <input type="checkbox" id="dsatermoaceite01" name="dsatermoaceite01" value="S" class="obrigatorio" title="Declaro que todas as informações..." <?=$checked_01;?>>
            </td>
            <td style="font-family: verdana, times new roman; font-size: 12px; text-align: justify;">
                Declaro que todas as informações acima apresentadas são autênticas e integralmente verídicas. Entendo que estou sujeito às devidas sansões, na esfera administrativa, cível e penal, caso preste informações falsas, conforme a lei.
            </td>
            <td rowspan="6" width="12%">&nbsp;</td>
        </tr>
        <tr>
            <td width="12%" style="text-align: right;">
                <input type="checkbox" id="dsatermoaceite02" name="dsatermoaceite02" value="S" class="obrigatorio" title="Comprometo-me, por meio deste Termo..." <?=$checked_02;?>>
            </td>
            <td style="font-family: verdana, times new roman; font-size: 12px;">
                <b>Comprometo-me, por meio deste Termo de Responsabilidade, às seguintes obrigações</b>:
            </td>
        </tr>
        <tr>
            <td>&nbsp;</td>
            <td style="font-family: verdana, times new roman; font-size: 12px; text-align: justify;">
                <p> Guardar a privacidade e o sigilo das informações que venha a ter conhecimento em razão do exercício de minhas atividades, bem como das informações disponibilizadas pelo Ministério da Educação, institutos e/ou universidades. </p><br>
                <p> Utilizar as informações disponibilizadas somente nas atividades a que compete exercer, não podendo transferi-las a terceiros, seja a título oneroso ou gratuito, estando ciente de que suas ações ou consultas serão acompanhadas e eventualmente auditadas. </p> <br>
                <p> Informar imediatamente ao Ministério da Educação acerca de qualquer violação das regras de proteção das informações eletrônicas ou não, por sua parte ou de quaisquer outras pessoas, inclusive nos casos de violação não intencional ou culposa, do sigilo do processo. </p>
            </td>
        </tr>
        <tr>
            <td>&nbsp;</td>
            <td style="font-family: verdana, times new roman; font-size: 12px; text-align: justify; color: #CD0000;">
                <b>O avaliador poder&aacute; ser responsabilizados administrativamente,  nos termos da Lei 8.112, de 1990, pelo uso ou divulga&ccedil;&otilde;es indevidas das  informa&ccedil;&otilde;es disponibilizadas para esse processo, sem preju&iacute;zo das san&ccedil;&otilde;es  c&iacute;veis e penais.</b>
            </td>
        </tr>
        <tr>
            <td width="12%" style="text-align: right;">
                <input type="checkbox" id="dsatermoaceite03" name="dsatermoaceite03" value="S" class="obrigatorio" title="Declaro, para os devidos fins..." <?=$checked_03;?>>
            </td>
            <td style="font-family: verdana, times new roman; font-size: 12px;">
                Declaro, para os devidos fins, que li o Edital do processo seletivo, seus anexos e retifica&ccedil;&otilde;es e concordo com todas as regras e conte&uacute;dos nele contidos.
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <?PHP
                    if( $habilita == 'S' ){
                ?>
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosProfessores();"/>
                        <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="voltarPaginaPrincipal();"/>
                <?PHP
                    }else{
                ?>
                        <input type="button" id="salvar" name="salvar" value="Salvar" disabled="disabled"/>
                        <input type="button" id="cancelar" name="cancelar" value="Cancelar"disabled="disabled"/>
                <?PHP
                    }
                ?>
            </td>
        </tr>
    </table>
</form>

<?PHP
    executaAcoesVerificacao();
?>




