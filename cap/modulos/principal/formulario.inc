<?PHP
    include_once APPRAIZ . "includes/workflow.php";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ($_REQUEST['requisicao'] == 'atualizarDiaria') {
        echo atualizarDiaria($_REQUEST['afpid']);
        die();
    }

    if ($_REQUEST['requisicao'] == 'atualizarValorDiaria') {
        echo atualizarValorDiaria($_REQUEST['afpid'], $_REQUEST['afpfuncao'], $_REQUEST['afpnivelcargo']);
        die();
    }

    if ($_REQUEST['requisicao'] == 'atualizarValorTotal') {
        echo atualizarValorTotal($_REQUEST['afpid'], $_REQUEST['afpfuncao'], $_REQUEST['afpnivelcargo']);
        die();
    }

    $perfil = pegarPerfil($_SESSION['usucpf']);
    if ($_REQUEST['requisicao'] == 'cadastrarid') {
        #VERIFICA SE EXISTE AFASTAMENTO PENDENTE
        $cpf = corrige_cpf($_POST['fdpcpf']);
        $sql = "
            SELECT afa.afpid
            FROM cap.afastamento afa
            INNER JOIN workflow.documento doc on doc.docid = afa.docid
            WHERE afa.afpstatus='A'
                AND afa.fdpcpf = '{$cpf}'
                AND doc.esdid in (".WF_CADASTRO_FORMULARIO_VAIAGEM.", " . WF_CADASTRO_NOVO_FORMULARIO . ")
        ";
        $existe = $db->pegaUm($sql);

        if ($existe){
            echo '
                <script>
                    alert("Já existe um registro de afastamento em andamento para este servidor.");
                    history.back();
                </script>
            ';
            exit();
        }
        #SALVA AFASTAMENTO
        salvarFormularioID($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'alterar') {
        alterarFormulario($_FILES, $_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'buscar_servidor') {
        buscarDadosServidor($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'excluir') {
        header('Content-Type: text/html; charset=iso-8859-1');
        excluirDocAnexo($_REQUEST['arquivo']);
        exibirListaDocAnexo('documentos', $perfil);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'salvar_dataviagem') {
        salvarDataViagem($_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'excluir_evento') {
        header('Content-Type: text/html; charset=iso-8859-1');
        excluirEvento($_POST['aftid']);
        exibirListaEvento($perfil);
        exit();
    }

    if ($_GET['download'] == 'S') {
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($_GET['arqid']);
        echo"<script>window.location.href = 'cap.php?modulo=principal/documentos&acao=A';</script>";
        exit;
    }

    if (empty($_SESSION['cap']['afpid']) && isset($_REQUEST['afpid'])) {
        $_SESSION['cap']['afpid'] = $_REQUEST['afpid'];
    }

    if ($_SESSION['cap']['afpid']) {
        $docid = criarDocumento($_SESSION['cap']['afpid']);
        $aryDados = exibirDadosFormulario($_SESSION['cap']['afpid']);
        extract($aryDados);
    }

    $esdid = pegarEstadoDocumento($docid);

    #CHAMADA DE PROGRAMA
    include APPRAIZ . "includes/cabecalho.inc";
    $file = new FilesSimec();

    if (in_array(CAP_PERFIL_SUPER_USUARIO, $perfil) || in_array(CAP_PERFIL_ADMINISTRADOR, $perfil)) {
        $modulo = "Módulo Administrativo";
    } else {
        $modulo = "Módulo Servidor";
    }

    $titulo_modulo = "Afastamento do País - Cód. # " . $_SESSION['cap']['afpid'];
    monta_titulo($titulo_modulo, $modulo);

    if ($_SESSION['cap']['afpid']) {
        cabecalhoCAP();
    }
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
    jQuery.noConflict();

    function buscarDadosServidor(campo) {
        jQuery.ajax({
            type: 'POST',
            dataType: 'json',
            url: 'cap.php?modulo=principal/formulario&acao=A',
            data: {requisicao: 'buscar_servidor', fdpcpf: jQuery("#fdpcpf").val(), fdpsiape: jQuery("#afpnumsiape").val()},
            async: false,
            success: function (data) {
                if (data) {
                    jQuery('#afpnumsiape').val(data.afpnumsiape);
                    jQuery('#fdpcpf').val(data.fdpcpf);
                    jQuery('#fdpnome').val(data.fdpnome);
                } else {
                    jQuery('#afpnumsiape').val('');
                    jQuery('#fdpcpf').val('');
                    jQuery('#fdpnome').val('');
                    alert("Servidor não encontrado!");
                }
            }
        });
    }

    function excluirDocAnexo(arqid) {
        if (confirm("Deseja realmente excluir o Arquivo?")) {
            jQuery.ajax({
                url: 'cap.php?modulo=principal/documentos&acao=A',
                data: {requisicao: 'excluir', arquivo: arqid},
                async: false,
                success: function (data) {
                    alert('Arquivo excluído com sucesso!');
                    jQuery("#div_documento").html(data);
                }
            });
        }
    }

    function validarFormularioID() {
        if (jQuery('#afpnumsiape').val() == '') {
            alert('O campo "Nº SIAPE" é obrigatório!');
            jQuery('#afpnumsiape').focus();
            return false;
        }
        if (jQuery('#fdpcpf').val() == '') {
            alert('O campo "CPF"! é obrigatório!');
            jQuery('#fdpcpf').focus();
            return false;
        }
        if (jQuery('#fdpnome').val() == '') {
            alert('O campo "Nome do Servidor" é obrigatório!');
            jQuery('#fdpnome').focus();
            return false;
        }
        if (jQuery('#afpcargofuncao').val() == '') {
            alert('O campo "Cargo/Função" é obrigatório!');
            jQuery('#afpcargofuncao').focus();
            return false;
        }
        if (jQuery('#afptelefone').val() == '') {
            alert('O campo "Ramal" é obrigatório!');
            jQuery('#afptelefone').focus();
            return false;
        }
        if (jQuery('[name=uamid]').val() == '') {
            alert('O campo "Área MEC" é obrigatório!');
            jQuery('[name=uamid]').focus();
            return false;
        }
        return true;
    }

    function validarFormulario() {
        if (jQuery('#afpnumsiape').val() == '') {
            alert('O campo "Nº SIAPE" é obrigatório!');
            jQuery('#afpnumsiape').focus();
            return false;
        }
        if (jQuery('#fdpcpf').val() == '') {
            alert('O campo "CPF"! é obrigatório!');
            jQuery('#fdpcpf').focus();
            return false;
        }
        if (jQuery('#fdpnome').val() == '') {
            alert('O campo "Nome do Servidor" é obrigatório!');
            jQuery('#fdpnome').focus();
            return false;
        }
        if (jQuery('#afpcargofuncao').val() == '') {
            alert('O campo "Cargo/Função" é obrigatório!');
            jQuery('#afpcargofuncao').focus();
            return false;
        }
        if (jQuery('#afptelefone').val() == '') {
            alert('O campo "Ramal" é obrigatório!');
            jQuery('#afptelefone').focus();
            return false;
        }
        if (jQuery('[name=uamid]').val() == '') {
            alert('O campo "Área MEC" é obrigatório!');
            jQuery('[name=uamid]').focus();
            return false;
        }
        if (jQuery('#afpnivelcargo').val() == '') {
            alert('O campo "Nível do Cargo" é obrigatório!');
            jQuery('#afpnivelcargo').focus();
            return false;
        }
        if (jQuery('#afpfuncao').val() == '') {
            alert('O campo "Função" é obrigatório!');
            jQuery('#afpfuncao').focus();
            return false;
        }
        // if(jQuery('[name=afpdtrealizacaoinicial]').val() == '' || jQuery('[name=afpdtrealizacaofinal]').val() == ''){
        // 	alert('O campo "Período Total do(s) Evento(s)" é obrigatório!');
        // 	jQuery('[name=afpdtrealizacaoinicial]').focus();
        // 	return false;
        // }
        if (jQuery('[name=afppertraninicial]').val() == '' || jQuery('[name=afppertranfinal]').val() == '') {
            alert('O campo "Período com Trânsito Incluso" é obrigatório!');
            jQuery('[name=afppertraninicial]').focus();
            return false;
        }
        if (jQuery('[name=tonid]:checked').length <= 0) {
            alert('O campo "Tipo de Natureza" é obrigatório!');
            jQuery('[name=tonid]').focus();
            return false;
        }
        return true;
    }

    function cadastrarEvento() {
        if (jQuery('#afppertraninicial').val() == '' || jQuery('#afppertranfinal').val() == '') {
            alert('O campo "Período com Trânsito Incluso" é obrigatório!');
            jQuery('#afppertraninicial').focus();
        } else {
            jQuery.ajax({
                url: 'cap.php?modulo=principal/formulario&acao=A',
                data: {requisicao: 'salvar_dataviagem', afpid: jQuery('#afpid').val(), afppertraninicial: jQuery('#afppertraninicial').val(), afppertranfinal: jQuery('#afppertranfinal').val()},
                async: false,
                type: 'POST',
                success: function (data) {
                    if (trim(data) == 'S') {
                        return window.open('cap.php?modulo=principal/cadastroevento&acao=A', 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
                    } else {
                        alert('O campo "Período com Trânsito Incluso" é obrigatório!');
                    }
                }
            });
        }
    }

    function alterarEvento(aftid) {
        return window.open('cap.php?modulo=principal/cadastroevento&acao=A&aftid=' + aftid, 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }

    function excluirEvento(aftid) {
        if (confirm("Deseja realmente excluir o Evento / País?")) {
            jQuery.ajax({
                url: 'cap.php?modulo=principal/formulario&acao=A',
                data: {requisicao: 'excluir_evento', aftid: aftid},
                async: false,
                type: 'POST',
                success: function (data) {
                    alert('Evento excluído com sucesso!');
                    jQuery("#div_evento").html(data);
                }
            });
        }
    }

    jQuery(document).ready(function () {
        if (jQuery("input[name=tonid]:checked").val() == 1 || jQuery("input[name=tonid]:checked").val() == 4) {
            jQuery('#div_tiponatureza').show();
            jQuery('[name=afporgaofinanciador]').parent().parent().show();
        } else {
            jQuery('#div_tiponatureza').hide();
            jQuery('[name=afporgaofinanciador]').parent().parent().hide();
        }
        <?php if ($_SESSION['cap']['afpid'] && !$tonid) {?>
        verificaOnus();
        <?php } ?>
        jQuery("input[name=tonid]").click(function () {
            if (jQuery("input[name=tonid]:checked").val() == 1 || jQuery("input[name=tonid]:checked").val() == 4) {
                jQuery('#div_tiponatureza').show();
                jQuery('[name=afporgaofinanciador]').parent().parent().show();
            } else {
                jQuery('#div_tiponatureza').hide();
                jQuery('[name=afporgaofinanciador]').parent().parent().hide();
            }
        });
    });

    function verificaOnus(){
        
        if( jQuery("input[name=afpsitpassagem]:checked").val() ){
            jQuery('#afpvlrpassagem').attr('readonly', false);
            jQuery('#afpvlrpassagem').removeClass('disabled');
            jQuery('#afpclasse').attr('readonly', false);
            jQuery('#afpclasse').removeClass('disabled');
            jQuery('#afptrecho').attr('readonly', false);
            jQuery('#afptrecho').removeClass('disabled');
            
            jQuery('#arquivo').attr('readonly', false);
            jQuery('#arquivo').removeClass('disabled');
            jQuery('#arquivo').show();
        } else {
            jQuery('#afpvlrpassagem').attr('readonly', true);
            jQuery('#afpvlrpassagem').addClass('disabled');
            jQuery('#afpclasse').attr('readonly', true);
            jQuery('#afpclasse').addClass('disabled');
            jQuery('#afptrecho').attr('readonly', true);
            jQuery('#afptrecho').addClass('disabled');
            jQuery('#arquivo').attr('readonly', true);
            jQuery('#arquivo').hide();
            jQuery('#afpvlrpassagem').val('');
            jQuery('#afpclasse').val('');
            jQuery('#afptrecho').val('');
        }

        if (jQuery("input[name=afpsitdiarias]:checked").val()) {
            jQuery('#afpnumdiarias').attr('readonly', false);
            jQuery('#afpnumdiarias').removeClass('disabled');
            jQuery('#afpvlrtotaldiarias').attr('readonly', false);
            jQuery('#afpvlrtotaldiarias').removeClass('disabled');
            jQuery('#afpvlrtotalbolsa').attr('readonly', false);
            jQuery('#afpvlrtotalbolsa').removeClass('disabled');
        } else {
            jQuery('#afpnumdiarias').attr('readonly', true);
            jQuery('#afpnumdiarias').addClass('disabled');
            jQuery('#afpvlrtotaldiarias').attr('readonly', true);
            jQuery('#afpvlrtotaldiarias').addClass('disabled');
            jQuery('#afpvlrtotalbolsa').attr('readonly', true);
            jQuery('#afpvlrtotalbolsa').addClass('disabled');
            jQuery('#afpnumdiarias').val('');
            jQuery('#afpvlrtotaldiarias').val('');
            jQuery('#afpvlrtotalbolsa').val('');
        }

        if ( jQuery("input[name=tonid]:checked").val() == 2 || jQuery("input[name=tonid]:checked").val() == 3 ){
            jQuery('#afpnumdiarias').attr('readonly', true);
            jQuery('#afpnumdiarias').addClass('disabled');
            jQuery('#afpvlrtotaldiarias').attr('readonly', true);
            jQuery('#afpvlrtotaldiarias').addClass('disabled');
            jQuery('#afpvlrtotalbolsa').attr('readonly', true);
            jQuery('#afpvlrtotalbolsa').addClass('disabled');
            jQuery('#afpvlrpassagem').attr('readonly', true);
            jQuery('#afpvlrpassagem').addClass('disabled');
            jQuery('#afpclasse').attr('readonly', true);
            jQuery('#afpclasse').addClass('disabled');
            jQuery('#afptrecho').attr('readonly', true);
            jQuery('#afptrecho').addClass('disabled');
            jQuery('#arquivo').attr('readonly', true);
            jQuery('#afpnumdiarias').val('');
            jQuery('#afpvlrtotaldiarias').val('');
            jQuery('#afpvlrtotalbolsa').val('');

            if (jQuery('input[name=afpsitdiarias]').is(':checked')) {
                jQuery("#afpsitdiarias").removeAttr('checked');
            }

            if (jQuery('input[name=afpsitpassagem]').is(':checked')) {
                jQuery("#afpsitpassagem").removeAttr('checked');
            }
        }

        if ( jQuery("input[name=tonid]:checked").val() == 4 ){
            jQuery('#afpsitdiarias').attr('disabled', true);
            jQuery('#afpsitdiarias').addClass('disabled');
            jQuery('#afpnumdiarias').attr('readonly', true);
            jQuery('#afpnumdiarias').addClass('disabled');
            jQuery('#afpvlrtotaldiarias').attr('readonly', true);
            jQuery('#afpvlrtotaldiarias').addClass('disabled');
            jQuery('#afpvlrtotalbolsa').attr('readonly', true);
            jQuery('#afpvlrtotalbolsa').addClass('disabled');
            jQuery('#afpvlrpassagem').attr('readonly', true);
            jQuery('#afpvlrpassagem').addClass('disabled');
            jQuery('#afpclasse').attr('readonly', true);
            jQuery('#afpclasse').addClass('disabled');
            jQuery('#afptrecho').attr('readonly', true);
            jQuery('#afptrecho').addClass('disabled');
            jQuery('#arquivo').attr('readonly', true);
            jQuery('#afpnumdiarias').val('');
            jQuery('#afpvlrtotaldiarias').val('');
            jQuery('#afpvlrtotalbolsa').val('');

            if (jQuery('input[name=afpsitdiarias]').is(':checked')) {
                jQuery("#afpsitdiarias").removeAttr('checked');
            }

//            if (jQuery('input[name=afpsitpassagem]').is(':checked')) {
//                jQuery("#afpsitpassagem").removeAttr('checked');
//            }
        }
    }

    function atualizaDiaria(afpid) {
        if (jQuery("input[name=afpsitdiarias]:checked").val()) {
            jQuery.post(window.location + '&requisicao=atualizarDiaria&afpid=' + afpid, function (response) {
                jQuery('#afpnumdiarias').val(response);
            });
        } else {
            jQuery('#afpnumdiarias').val('');
            jQuery('#afpvlrtotaldiarias').val('');
            jQuery('#afpvlrtotalbolsa').val('');
        }
    }

    function atualizaValorDiaria(afpid, afpfuncao, afpnivelcargo) {
        if (jQuery("input[name=afpsitdiarias]:checked").val()) {
            jQuery.post(window.location + '&requisicao=atualizarValorDiaria&afpid=' + afpid + '&afpfuncao=' + afpfuncao + '&afpnivelcargo=' + afpnivelcargo, function (response) {
                jQuery('#afpvlrtotaldiarias').val(response);
            });
        } else {
            jQuery('#afpnumdiarias').val('');
            jQuery('#afpvlrtotaldiarias').val('');
            jQuery('#afpvlrtotalbolsa').val('');
        }
    }

    function atualizaValorTotal(afpid, afpfuncao, afpnivelcargo) {
        if (jQuery("input[name=afpsitdiarias]:checked").val()) {
            jQuery.post(window.location + '&requisicao=atualizarValorTotal&afpid=' + afpid + '&afpfuncao=' + afpfuncao + '&afpnivelcargo=' + afpnivelcargo, function (response) {
                jQuery('#afpvlrtotalbolsa').val(response);
            });
        } else {
            jQuery('#afpnumdiarias').val('');
            jQuery('#afpvlrtotaldiarias').val('');
            jQuery('#afpvlrtotalbolsa').val('');
        }
    }
</script>

<?PHP
    if (!empty($_SESSION['cap']['afpid'])) {
?>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr bgcolor="#CCCCCC">
            <td><b>Servidor:</b>&nbsp;<?PHP echo exibirNomeServidor($_SESSION['cap']['afpid']); ?></td>
        </tr>
    </table>

<?PHP
    }

    $arMnuid = array();

    $db->cria_aba(ABA_CAD_AFASTAMENTO, $url, '', $arMnuid);
?>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align="center">
            Formulário de Autorização de Afastamento do País - MISSÃO OFICIAL<br>
            Prazo limite para a entrega da solicitação: 15 dias antes do ínicio da missão)
        </td>
    </tr>
</table>

<?PHP
    if (in_array(CAP_PERFIL_SERVIDOR, $perfil)) {
        $fdpcpf = $_SESSION['usucpf'];
    }

    if ($_SESSION['cap']['afpid']) {
        $submit = "return validarFormulario();";
    } else {
        $submit = "return validarFormularioID();";
    }

    if ($_REQUEST['afpid'] && $_SESSION['cap']['afpid']) {
        $habilitado = "N";
    } else {
        $habilitado = "S";
    }

    if ($esdid == WF_CADASTRO_FORMULARIO_VAIAGEM || $esdid == WF_CORRECOES_FORMULARIO_VIAGEM || $esdid == WF_CADASTRO_NOVO_FORMULARIO || $esdid == WF_CORRECOES_RELATORIO_VIAGEM ) {
        if( $_SESSION['cap']['afpid'] != '' ){
            $habilitado_p = 'S';
        }
        $habilitado = "S";
    }else{
        if( $_SESSION['cap']['afpid'] == '' ){
            $habilitado_p = 'S';
        }else{
            $habilitado_p = 'N';
        }
        $habilitado = "N";
    }

    if ($_SESSION['cap']['afpid'] && !empty($tonid)) {
        $desabilitaOnus = "N";
    }
    else{
        $desabilitaOnus = "S";
    }

?>

<form id="formulario" method="post" name="formulario" action="/cap/cap.php?modulo=principal/formulario&acao=A" onsubmit="<?PHP echo $submit; ?>" enctype="multipart/form-data">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td width="92%">
                <?PHP if ($_SESSION['cap']['afpid']) { ?>
                    <input type="hidden" id="requisicao" name="requisicao" value="alterar"/>
                    <input type="hidden" id="afpid" name="afpid" value="<?PHP echo $afpid; ?>"/>
                <?PHP } else { ?>
                    <input type="hidden" id="requisicao" name="requisicao" value="cadastrarid"/>
                <?PHP } ?>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Identificação</b></td>
                    </tr>
                    <?PHP if ($_REQUEST['afpid'] && $_SESSION['cap']['afpid']) { ?>
                        <tr>
                            <td class="subtituloDireita" width="25%">Nº do Processo:</td>
                            <td>
                                <?PHP
                                    echo campo_texto('afpnumprocesso', 'S', $habilitado_p, 'Nº do Processo', '50', '25', '', '', '', '', '', 'id="afpnumprocesso"', '', '', '');
                                ?>
                            </td>
                        </tr>
                    <?PHP } ?>
                    <tr>
                        <td class="subtituloDireita" width="25%">Nº SIAPE:</td>
                        <td>
                            <?PHP
                                echo campo_texto('afpnumsiape', 'S', $habilitado_p, 'Nº SIAPE', '50', '11', '', '', '', '', '', 'id="afpnumsiape"', '', '', 'buscarDadosServidor()');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">CPF:</td>
                        <td>
                            <?PHP
                                echo campo_texto('fdpcpf', 'S', $habilitado_p, 'CPF', '50', '14', '###.###.###-##', '', '', '', '', 'id="fdpcpf"', '', '', 'buscarDadosServidor()');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Nome do Servidor:</td>
                        <td>
                            <?PHP
                                echo campo_texto('fdpnome', 'S', $habilitado_p, 'Nome do Servidor', '50', '150', '', '', '', '', '', 'id="fdpnome"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Email do Servidor:</td>
                        <td>
                            <?PHP
                                echo campo_texto('afpemailservidor', 'S', $habilitado_p, 'Nome do Servidor', '50', '150', '', '', '', '', '', 'id="afpemailservidor"');
                            ?>
                        </td>

                    </tr>
                    <tr>
                        <td class="subtituloDireita">Cargo:</td>
                        <td>
                            <?PHP
                            if( $afpnivelcargo == 'M' ){
                                $checked_m =  'checked="checked"';
                            }else
                                if( $afpnivelcargo == 'S' || !$afpnivelcargo ){
                                   $checked_s =  'checked="checked"';
                            }

                            echo campo_texto('afpcargofuncao', 'S', $habilitado_p, 'Cargo', '50', '150', '', '', '', '', '', 'id="afpcargofuncao"');
                            ?>
                            <input type="radio" name="afpnivelcargo" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="M"  <?=$checked_m;?>> <b>Nível Médio</b>
                            <input type="radio" name="afpnivelcargo" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="S" <?=$checked_s;?>> <b>Nível Superior</b>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Função:</td>
                        <td>
                            <?PHP
                                if ($afpfuncao == 6) {
                                    $checked_6 = 'checked="checked"';
                                }
                                if ($afpfuncao == 5) {
                                    $checked_5 = 'checked="checked"';
                                }
                                if ($afpfuncao == 4) {
                                    $checked_4 = 'checked="checked"';
                                }
                                if ($afpfuncao == 2) {
                                    $checked_2 = 'checked="checked"';
                                }
                                if ($afpfuncao == 'N' || !$afpfuncao) {
                                    $checked_n = 'checked="checked"';
                                }
                            ?>
                            <input type="radio" name="afpfuncao" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="6" <?=$checked_6;?>> <b>DAS 6</b> &nbsp;
                            <input type="radio" name="afpfuncao" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="5" <?=$checked_5;?>> <b>DAS 5</b> &nbsp;
                            <input type="radio" name="afpfuncao" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="4" <?=$checked_4;?>> <b>DAS 4 OU 3</b> &nbsp;
                            <input type="radio" name="afpfuncao" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="2" <?=$checked_2;?>> <b>DAS 2 OU 1</b> &nbsp;
                            <input type="radio" name="afpfuncao" <?php if(!($habilitado_p == 'S')){echo "disabled='disabled'";}?> value="N" <?=$checked_n;?>> <b>Nenhuma das opções anteriores</b>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Ramal:</td>
                        <td>
                            <?PHP
                                echo campo_texto('afptelefone', 'S', $habilitado_p, 'Ramal', '50', '20', '', '', '', '', '', 'id="afptelefone"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Lotação:</td>
                        <td>
                            <?PHP
                                $sql = "SELECT uamid AS codigo, uamsigla || ' - ' || uamdsc AS descricao FROM public.unidadeareamec where uamstatuscap = 'A'  ORDER BY uamsigla, uamdsc";
                                $db->monta_combo('uamid', $sql, $habilitado_p, 'Selecione...', '', '', '', '372', 'S', '', '', $uamid);
                            ?>
                        </td>
                    </tr>
                    <?PHP
                        if (!$_REQUEST['afpid'] && !$_SESSION['cap']['afpid']) {
                    ?>
                        <tr>
                            <td align="center" bgcolor="#CCCCCC" colspan="2">
                                <input type="submit" value="Preencher Formulário" id="btnSalvarID"/>
                            </td>
                        </tr>
                    <?PHP
                        }

                        if ($_SESSION['cap']['afpid']) {
                    ?>
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Data da Realização do Evento</b></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Período com Trânsito Incluso:</td>
                            <td>
                                <?PHP
                                    echo campo_data2('afppertraninicial', 'S', $habilitado, 'Período com Trânsito Incluso Inicial', 'S', '', '', $afppertraninicial);
                                    echo ' à ';
                                    echo campo_data2('afppertranfinal', 'S', $habilitado, 'Período com Trânsito Incluso Final', 'S', '', '', $afppertranfinal);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">País/Evento:</td>
                            <td>
                                <?PHP
                                    if( !in_array(PERFIL_GABINETE, $perfil) ){
                                        if( $habilitado == 'S' ){
                                ?>
                                            <span class="adicionar_evento" style="cursor:pointer" onclick="cadastrarEvento();">
                                                <img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">
                                                <b>Adicionar Evento</b>
                                            </span>
                                            <br>
                                            <br>
                                <?PHP
                                        }
                                    }

                                    exibirListaEvento($perfil, $habilitado, $afpfuncao);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Natureza do Afastamento</b></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Tipo de Afastamento:</td>
                            <td>
                                <?PHP
                                    if( $habilitado == 'S' ){
                                        $disabled = '';
                                    }else{
                                        $disabled = 'disabled="disabled"';
                                    }
                                    
                                    switch( $tonid ){
                                        case 1:
                                            $check_tonid_1 = 'checked="checked"';
                                        break;                                    
                                        case 2:
                                            $check_tonid_2 = 'checked="checked"';
                                        break;                                    
                                        case 3:
                                            $check_tonid_3 = 'checked="checked"';
                                        break;                                    
                                        case 4:
                                            $check_tonid_4 = 'checked="checked"';
                                        break;                                    
                                    }                                    
                                ?>
                                <input type="radio" value="1" name="tonid" <?=$disabled;?> id="tonid" <?=$check_tonid_1;?> >Com Ônus &nbsp;

                                <input type="radio" value="4" name="tonid" <?=$disabled;?> id="tonid" <?=$check_tonid_4;?> onclick="verificaOnus()">Com Ônus [50% da(s) diária(s)] &nbsp;
                                
                                <input type="radio" value="2" name="tonid" <?=$disabled;?> id="tonid" <?=$check_tonid_2;?> onclick="verificaOnus()">Com Ônus Limitado &nbsp;
                                
                                <input type="radio" value="3" name="tonid" <?=$disabled;?> id="tonid" <?=$check_tonid_3;?> onclick="verificaOnus()">Sem Ônus
                                
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."> <br><br>
                                
                                <table border="1"bgcolor="#00FF00"style="width:100%">
                                    <tr>
                                        <td><b>Com ônus</b> - Quando o custeio de pousada, alimentação e locomoção urbana for pelo servidor.<br>
                                        <br><b>Com Ônus [50% da(s) diária(s)]</b> - Quando o custeio de pousada ou alimentação for por governo estrangeiro ou organismo internacional.<br>
                                        <br><b>Com Ônus limitado</b> - Quando governo estrangeiro ou organismo internacional custear pousada, alimentação e locomoção urbana. <br>
                                        <br><b>Sem Ônus</b> - Quando o servidor deseja se afastar sem remuneração ou diárias.</td></tr></table><br>

                                <div id="div_tiponatureza" style="display: block;">
                                    <?PHP
                                        if( $tonid == 4 ){
                                            $disable_diarias = 'disabled="disabled"';
                                            $habil = 'N';
                                        }else{
                                            if( $habilitado == 'S' ){
                                                $disabled = '';
                                                $habil = $habilitado;
                                            }else{
                                                $disabled = 'disabled="disabled"';
                                                $habil = $habilitado;
                                            }                                            
                                        }
                                        
                                        if( $afpsitdiarias == 't' ){
                                            $check_afpsitdiarias = 'checked="checked"';
                                        }
                                        if( $afpsitpassagem == 't' ){
                                            $check_afpsitpassagem = 'checked="checked"';
                                        }                                                                           
                                    ?>
                                    <input type="checkbox" id="afpsitdiarias" name="afpsitdiarias" value="1" <?=$disable_diarias;?> <?=$disabled;?> class="normal" style="margin-top:-1px;" <?=$check_afpsitdiarias;?> onclick="verificaOnus();"/> Diária(s)
                                    <input type="checkbox" id="afpsitpassagem" name="afpsitpassagem" value="1" <?=$disabled;?> class="normal" style="margin-top:-1px;" <?=$check_afpsitpassagem;?> onclick="verificaOnus();"/> Passagem Aérea 
                                    <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Órgão Financiador:</td>
                            <td><?PHP echo campo_texto('afporgaofinanciador', 'N', $habilitado, 'Órgão Financiador', '50', '100', '', '', '', '', '', 'id="afporgaofinanciador"'); ?></td>
                        </tr>
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Dados da Viagem</b></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Valor da Passagem R$:</td>
                            <td><?PHP echo campo_texto('afpvlrpassagem', 'N',$habil, 'Valor da Passagem US$', '25', '15', '[.###],##', '', '', '', '', 'id="afpvlrpassagem"'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Classe:</td>
                            <td><?PHP echo campo_texto('afpclasse', 'N', $habil, 'Classe', '50', '20', '', '', '', '', '', 'id="afpclasse"'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Trecho:</td>
                            <td><?PHP echo campo_texto('afptrecho', 'N', $habil, 'Trecho', '50', '300', '', '', '', '', '', 'id="afptrecho"'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Cópia da reserva da passagem:</td>
                            <td>
                                <input type="hidden" name="arqdescricao" value="Cópia da reserva da passagem"/>
                                <input type="hidden" name="tpdid" value="6"/>
                                <input type="file" name="arquivo" id="arquivo" <?php if(($habilitado == "N"))echo "style='display:none'"?> />&nbsp;
                                <br><br>
                                <div id="div_documento"><?PHP exibirListaDocAnexo('passagem', $perfil, 'S'); ?></div>
                            </td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Nº de Diárias:</td>
                            <td>
                                <?PHP
                                    if ($tonid == 4) {
                                       $hab = 'N';
                                    }else{
                                       $hab = $habilitado;
                                    }
                                    echo campo_texto('afpnumdiarias', 'N', $habilitado, 'Nº de Diárias', '10', '4', '####', '', '', '', '', 'id="afpnumdiarias"', '', $afpnumdiarias);
                                ?>
                                </td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Valor Total da Diária US$:</td>
                            <td>
                                <?PHP
                                    if ($tonid == 4) {
                                       $hab = 'N';
                                    }else{
                                       $hab = $habilitado;
                                    }
                                    echo campo_texto('afpvlrtotaldiarias', 'N', $habilitado, 'Valor Total da Diária', '25', '15', '[.###],##', '', '', '', '', 'id="afpvlrtotaldiarias"');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Justificativa para Participação na missão</b></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Objetivo da viagem:</td>
                            <td><?PHP echo campo_textarea('afttxtobjetivoviagem', 'N', $habilitado, 'Objetivo da viagem', '80', '5', '2000'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Vinculação do serviço ou evento a programas, projetos ou ações em andamento no MEC:</td>
                            <td><?PHP echo campo_textarea('afpvincservico', 'N', $habilitado, '', '80', '5', '2000'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Relevância da prestação do serviço ou participação do servidor para as finalidades do MEC:</td>
                            <td><?PHP echo campo_textarea('afprelevancia', 'N', $habilitado, '', '80', '5', '2000'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Pertinência da viagem de acordo com as atribuições do cargo que ocupa e o interesse do afastamento para a instituição:</td>
                            <td><?PHP echo campo_textarea('afppertinencia', 'N', $habilitado, '', '80', '5', '2000'); ?></td>
                        </tr>
                        <tr>
                            <td class="subtituloDireita">Justificativa para viagem urgente ou para embarque ou desembarque fora do período de 07 as 21hs ou durante finais de semana ou para desembarque que não cumpra antecedência mínima de 03 horas do evento:</td>
                            <td><?PHP echo campo_textarea('afpjustifespecial', 'N', $habilitado, '', '80', '5', '2000'); ?></td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <p align="justify">
                                    Observações:<br><br>
                                    1) O solicitante deverá prestar todas as informações necessárias à perfeita descrição das viagens,
                                    incluindo os dados relativos à justificativa dos deslocamentos e às datas, os locais e os horários dos
                                    compromissos assumidos. <br><br>
                                    2) A participação de servidores em feiras, fóruns, seminários, congressos, simpósios, grupos de trabalho
                                    e outros eventos será de, no máximo, um representante por unidade para eventos no exterior. Favor justificar
                                    quando houver necessidade de ampliação do número de participantes.<br><br>
                                </p>
                            </td>
                        </tr>
                        <tr>
                            <td align="center" bgcolor="#CCCCCC" colspan="2">
                                <? if ($habilitado == 'S') { ?>
                                <input type="submit" value="Salvar" id="btnSalvar"/>
                                <? } ?>
                            </td>
                        </tr>
                    </table>
<?PHP } ?>
        </td>
<?PHP if ($docid) { ?>
            <td valign="top"><br><?PHP wf_desenhaBarraNavegacao($docid, array('docid' => $docid, 'afpid' => $_SESSION['cap']['afpid']), ''); ?></td>
<?PHP } ?>
    </tr>
</table>
            </form>

<?PHP if (in_array(PERFIL_GABINETE, $perfil) || ($esdid == WF_VIAGEM_FINALIZADA && in_array(CAP_PERFIL_SERVIDOR, $perfil))) { ?>
    <script language="javascript" type="text/javascript">
        jQuery('#btnSalvar').attr('disabled', true);
        jQuery('input, select, textarea').attr('disabled', true);
        jQuery('#btnImprimir').attr('disabled', false);
        jQuery(document).ready(function () {
            if (trim(jQuery("input[name=tonid]:checked").val()) == '1') {
                jQuery('#div_tiponatureza').show();
            } else {
                jQuery('#div_tiponatureza').hide();
            }
        });
    </script>
<?PHP } ?>

<?PHP if ($tonid == 1) { ?>
    <script language="javascript" type="text/javascript">
        jQuery(document).ready(function () {
            jQuery('#div_tiponatureza').show();
        });
    </script>
<?PHP } ?>

<?PHP if (in_array(CAP_PERFIL_SERVIDOR, $perfil)) { ?>
    <script language="javascript" type="text/javascript">
        buscarDadosServidor();
    </script>
<?PHP } ?>