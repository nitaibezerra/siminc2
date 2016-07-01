<?PHP
    $perfil = pegarPerfil($_SESSION['usucpf']);

    #AJAX
    if ($_REQUEST['requisicao'] == 'abrirSubLista') {
        header('content-type: text/html; charset=ISO-8859-1');
        montaSubLista($_REQUEST['afpid']);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'pesquisar') {
        header('Content-Type: text/html; charset=iso-8859-1');
        pesquisarViagem($perfil, $_POST);
        exit();
    }

    if ($_REQUEST['requisicao'] == 'excluir') {
        header('Content-Type: text/html; charset=iso-8859-1');
        excluirFormulario($_REQUEST['afpid']);
        pesquisarViagem($perfil);
        exit();
    }

    if (in_array(CAP_PERFIL_SUPER_USUARIO, $perfil) || in_array(CAP_PERFIL_ADMINISTRADOR, $perfil)) {
        $modulo = "Módulo Administrativo";
    } else {
        $modulo = "Módulo Servidor";
    }

    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";

    $titulo_modulo = "Afastamento do País";
    monta_titulo($titulo_modulo, $modulo);

    echo '<br>';

    if ($_SESSION['cap']['afpid']) {
        unset($_SESSION['cap']['afpid']);
    }

    if (!in_array(CAP_PERFIL_SERVIDOR, $perfil)) {
        $arMnuid = array();
        $db->cria_aba(ABA_PESQUISA, $url, '', $arMnuid);
    }
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript">
    jQuery.noConflict();

    jQuery('.adicionar').live('click', function () {
        window.location.href = 'cap.php?modulo=principal/formulario&acao=A';
    });

    function pesquisarViagem() {
        divCarregando();
        jQuery.ajax({
            type: 'POST',
            url: 'cap.php?modulo=principal/listaviagem&acao=A&requisicao=pesquisar',
            data: jQuery('#formulario').serialize(),
            async: false,
            success: function (data) {
                jQuery("#div_viagem").html(data);
                divCarregado();
            }
        });
    }

    function excluirFormulario(afpid) {
        if (confirm("Deseja realmente excluir o Formulário?")) {
            divCarregando();
            jQuery.ajax({
                url: 'cap.php?modulo=principal/listaviagem&acao=A',
                data: {requisicao: 'excluir', afpid: afpid},
                async: false,
                success: function (data) {
                    alert('Formulário excluído com sucesso!');
                    jQuery("#div_viagem").html(data);
                    divCarregado();
                }
            });
        }
    }

    function alterarFormulario(afpid) {
        window.location.href = 'cap.php?modulo=principal/formulario&acao=A&afpid=' + afpid;
    }

    function montaSubLista(afpid){
        var td = document.getElementById('td_' + afpid);
        var img_mais = document.getElementById('img_mais_' + afpid);
        var img_menos = document.getElementById('img_menos_' + afpid);

        // Faz uma requisição ajax, passando o parametro 'ordid', via POST
        /*
         var req = new Ajax.Request('cap.php?modulo=principal/listaviagem&acao=A', {
         method:     'post',
         parameters: '&afpidAjax=' + afpid,
         onComplete: function (res)
         {
         td.style.display = '';
         //td.style.padding = '5px';
         img_mais.style.display = 'none';
         img_menos.style.display = '';
         td.innerHTML = res.responseText;
         }
         });
         */
        divCarregando();
        jQuery.ajax({
            url: 'cap.php?modulo=principal/listaviagem&acao=A',
            data: {requisicao: 'abrirSubLista', afpid: afpid},
            async: false,
            success: function (data) {
                //alert('Formulário excluído com sucesso!');
                //jQuery("#div_viagem").html(data);

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = data;

                divCarregado();
            }
        });
    }

    function desmontaSubLista(afpid){
        var td = document.getElementById('td_' + afpid);
        var img_mais = document.getElementById('img_mais_' + afpid);
        var img_menos = document.getElementById('img_menos_' + afpid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';
    }
</script>

<?PHP
    if (!in_array(PERFIL_GABINETE, $perfil)) {
?>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr>
            <td class="SubTituloEsquerda" colspan="2">
                <span class="adicionar" style="cursor:pointer"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">&nbsp;Inserir Novo Formulário</span>
            </td>
        </tr>
    </table>
    <br>
<?PHP
    }

    if (!in_array(CAP_PERFIL_SERVIDOR, $perfil)) {
?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
        <tr>
            <td class="SubTituloEsquerda" colspan="2">Argumentos de Pesquisa</td>
        </tr>
    </table>

    <form id="formulario" method="post" name="formulario" action="">
        <input type="hidden" id="requisicao" name="requisicao" value="pesquisar"/>
        
        <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
            <tr>
                <td class="SubTituloDireita">Nº SIAPE:</td>
                <td>
                    <?PHP 
                        echo campo_texto('afpnumsiape', 'S', 'S', '', '50', '11', '', '', '', '', '', 'id="afpnumsiape"'); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Nome do Servidor:</td>
                <td><?PHP echo campo_texto('fdpnome', 'N', 'S', '', 50, 150, '', ''); ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Período:</td>
                <td>
                    <?PHP 
                        echo campo_data2('dataini', 'S', 'S', '', 'S', '', '', $dataini);
                        echo 'á';
                        echo campo_data2('datafim', 'S', 'S', '', 'S', '', '', $datafim);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Lotação:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  uamid AS codigo, 
                                    uamsigla || ' - ' || uamdsc AS descricao
                            FROM public.unidadeareamec
                            ORDER BY uamsigla, uamdsc
                        ";
                        $db->monta_combo('uamid', $sql, 'S', 'Selecione...', '', '', '', '372', 'S', '', '', $uamid);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Situação:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  esdid as codigo,
                                    esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = ".WF_TPDID_CONTROLE_AFASTAMENTO;
                        $db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Cidade:</td>
                <td>
                    <?PHP 
                        echo campo_texto('cidade', 'N', 'S', '', 50, '', '', ''); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">País:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  paiid as codigo,
                                    paidescricao as descricao
                            FROM territorios.pais
                        ";
                        $db->monta_combo('paiid', $sql, 'S', 'Selecione', '', '', '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Expressão chave:</td>
                <td>
                    <?PHP 
                        echo campo_texto('expressao_chave', 'N', 'S', '', 50, '', '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td align="center" bgcolor="#DCDCDC" colspan="2">
                    <input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarViagem();"/>
                </td>
            </tr>
        </table>
    </form>
    <br>
<?PHP } ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
    <tr>
        <td class="SubTituloEsquerda" colspan="2">Legenda</td>
    </tr>
    <tr>
        <td>
            <img border="0" style="vertical-align:middle;" src="../imagens/icones/bg.png" title="Relatório de Viagem - Preenchimento Completo">&nbsp; Relatório de Viagem - Preenchimento Completo&nbsp;
            <img border="0" style="vertical-align:middle;" src="../imagens/icones/br.png" title="Relatório de Viagem - Preenchimento Parcial">&nbsp;Relatório de Viagem - Preenchimento Parcial&nbsp;
        </td>
    </tr>
</table>
    
<br>

<?PHP if (!in_array(PERFIL_GABINETE, $perfil)) { ?>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr>
            <td class="SubTituloEsquerda" colspan="2">
                <span class="adicionar" style="cursor:pointer"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">&nbsp;Inserir Novo Formulário</span>
            </td>
        </tr>
    </table>
    <br>
<?PHP } ?>

<div id="div_viagem"> <?PHP pesquisarViagem($perfil); ?> </div>
