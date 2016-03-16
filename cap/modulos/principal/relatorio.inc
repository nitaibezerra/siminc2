<?PHP
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $perfil = pegarPerfil($_SESSION['usucpf']);

    if( $_REQUEST['requisicao'] == 'cadastrar' ){
        salvarRelatorio($_POST);
        imprimirAfastamento('pdf', false);
        $arqid = verificarRelatorioPDFExiste($_SESSION['cap']['afpid'], $perfil);

        echo '
            <script>
                alert("Operação efetuada com sucesso. \n\nPor favor, imprima e depois valide o Relatório de Viagem no final deste Formulário.");
                location.href = "cap.php?modulo=principal/relatorio&acao=A";
            </script>
        ';
        exit();
    }

    if( $_REQUEST['requisicao'] == 'alterar' ){
        alterarRelatorio($_POST);
        imprimirAfastamento('pdf', false);
        $arqid = verificarRelatorioPDFExiste($_SESSION['cap']['afpid'], $perfil);

        if ($arqid) {
            header("Location:cap.php?modulo=principal/relatorio&acao=A&download=S&arqid=" . $arqid);
        } else {
            header("Location:cap.php?modulo=principal/relatorio&acao=A");
        }
        exit();
    }

    if( $_REQUEST['requisicao'] == 'validar_afastamento' ){
        $afastamento = validarAfastamento($_POST);
        if( $afastamento == 'S' ){
            extract($_POST);
            imprimirAfastamento('pdf', true);
            $arqid = verificarRelatorioPDFExiste($afpid, $perfil);
            $hab_validacao = verificarValidacaoAfastamento($_SESSION['cap']['afpid']);
            echo $hab_validacao;
        }else{
            echo '';
        }
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
        $aryDadosC = exibirDadosRelatorioCabecalho($_SESSION['cap']['afpid']);
        $aryDados = exibirDadosRelatorio($_SESSION['cap']['afpid']);
        extract($aryDadosC);
        extract($aryDados);
    }

    $esdid = pegarEstadoDocumento($docid);

    #CHAMADA DE PROGRAMA
    include APPRAIZ . "includes/cabecalho.inc";
    include_once APPRAIZ . "includes/workflow.php";

    if (in_array(CAP_PERFIL_SUPER_USUARIO, $perfil) || in_array(CAP_PERFIL_ADMINISTRADOR, $perfil)) {
        $modulo = "Módulo Administrativo";
    } else {
        $modulo = "Módulo Servidor";
    }

    $titulo_modulo = "Afastamento do País";
    monta_titulo($titulo_modulo, $modulo);

    cabecalhoCAP();
    
    if( $esdid == WF_PREENC_RELATORIO_VIAGEM ){
        $habilitado = "S";
    }else{
        $habilitado = "N";
    }
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
    function validarRelatorio() {
        if ($('[name=rafatividadesfatos]').val() == '') {
            alert('O campo "Atividades/Fatos Transcorridos" é obrigatório!');
            $('[name=rafatividadesfatos]').focus();
            return false;
        }
        if ($('[name=rafconclusao]').val() == '') {
            alert('O campo "Conclusões Alcançadas" é obrigatório!');
            $('[name=rafconclusao]').focus();
            return false;
        }
        if ($('[name=rafsugestao]').val() == '') {
            alert('O campo "Sugestões em relação aos benefícios que podem ser auferidos para a área da Educação" é obrigatório!');
            $('[name=rafsugestao]').focus();
            return false;
        }
        if ($('[name=rafobservacao]').val() == '') {
            alert('O campo "Observações" é obrigatório!');
            $('[name=rafobservacao]').focus();
            return false;
        }
        return true;
    }

    function validarAfastamento() {
        if (confirm("Deseja validar o Relatório de Viagem?")) {
            //data: { requisicao: 'validar_afastamento', rafid: $('#rafid').val(), afpid: $('#afpid').val()},
            $.ajax({
                url: 'cap.php?modulo=principal/relatorio&acao=A',
                type: 'POST',
                data: 'requisicao=validar_afastamento&rafid=' + $('#rafid').val() + '&afpid=' + $('#afpid').val(),
                async: false,
                success: function (data) {
                    if (trim(data) == '') {
                        alert('Relatório de Viagem já validado!');
                    } else {
                        $('#btnValidar').attr('disabled', true);
                        $('#div_validado').html(data);
                        alert('Afastamento validado com sucesso! \n\n Por favor, Envie para análise do relatório no lado direito deste Formulário!');
                        location.href = 'cap.php?modulo=principal/relatorio&acao=A';
                    }
                }
            });
        }
    }

    function imprimirAfastamento(tipo) {
        window.open("cap.php?modulo=principal/impressao&acao=A&tipo=" + tipo, 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }
</script>

<?PHP
    if( !empty($_SESSION['cap']['afpid']) ){
?>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr bgcolor="#CCCCCC">
            <td><b>Servidor:</b>&nbsp;<?PHP echo exibirNomeServidor($_SESSION['cap']['afpid']); ?></td>
        </tr>
    </table>
    <br>
<?PHP
    }
    $arMnuid = array();

    $db->cria_aba(ABA_CAD_AFASTAMENTO, $url, '', $arMnuid);
?>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align="center">
            Formulário de Autorização de Afastamento do País - MISSÃO OFICIAL
            <br>
            Prazo limite para a entrega da solicitação: 15 dias antes do ínicio da missão)
        </td>
    </tr>
</table>

<form id="formulario" method="post" name="formulario" action="/cap/cap.php?modulo=principal/relatorio&acao=A" onsubmit="return validarRelatorio();">
    <input type="hidden" id="afpid" name="afpid" value="<?PHP echo $_SESSION['cap']['afpid']; ?>"/>
    <?PHP if ($rafid) { ?>
        <input type="hidden" id="requisicao" name="requisicao" value="alterar"/>
        <input type="hidden" id="rafid" name="rafid" value="<?PHP echo $rafid; ?>"/>
    <?PHP } else { ?>
        <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    <?PHP } ?>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td width="92%">
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>1. Órgão</b></td>
                    </tr>
                    <tr>
                        <td>&nbsp;<?PHP echo $orgao; ?></td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>2. Identificação do Servidor:</b></td>
                    </tr>
                    <tr>
                        <td>
                            <table border="0" bordercolor="#000000" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                                <tr>
                                    <td width="50%"><b>Nome:</b>&nbsp;&nbsp;<?PHP echo $fdpnome; ?></td>
                                    <td width="50%"><b>Matrícula/SIAPE:</b>&nbsp;&nbsp;<?PHP echo $afpnumsiape; ?></td>
                                </tr>
                                <tr>
                                    <td width="50%"><b>Cargo/Função:</b>&nbsp;&nbsp;<?PHP echo $afpcargofuncao; ?></td>
                                    <td width="50%"><b>Ramal:</b>&nbsp;&nbsp;<?PHP echo $afptelefone; ?></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>3. Período de Afastamento:</b></td>
                    </tr>
                    <tr>
                        <td>
                            <table border="0" bordercolor="#000000" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                                <tr>
                                    <td width="50%"><b>Data de Saída:</b>&nbsp;&nbsp;<?PHP echo $afpdtrealizacaoinicial; ?></td>
                                    <td width="50%"><b>Data de Chegada:</b>&nbsp;&nbsp;<?PHP echo $afpdtrealizacaofinal; ?></td>
                                </tr>
                                <tr>
                                    <td colspan="2" align="left"><b>Trecho(s):</b>
                                        <?PHP
                                            #RECUPERA O PAIS E OBJETIVOS
                                            if( $_SESSION['cap']['afpid'] ){
                                                $sql_pais = "
                                                    SELECT  pai.prddsc AS paidescricao,
                                                            to_char(aft.aftdtinicio::TIMESTAMP,'DD/MM/YYYY') as aftdtinicio,
                                                            to_char(aft.aftdtfinal::TIMESTAMP,'DD/MM/YYYY') as aftdtfinal
                                                     FROM cap.afastamentotrecho aft
                                                     JOIN cap.paisdiarias pai ON pai.idpdr = aft.idpdr
                                                     WHERE aft.afpid = {$_SESSION['cap']['afpid']}
                                                     ORDER BY aft.idpdr, aft.aftdtinicio
                                                ";
                                                $result = $db->carregar($sql_pais);

                                                if ($result) {
                                                    $tr_objetivo_pais = '
                                                        <table width="40%" cellSpacing="0" cellPadding="3">
                                                            <tr>
                                                                <td align="center" bgcolor="#CCCCCC" ><b>País</b></td>
                                                                <td align="center" bgcolor="#CCCCCC" ><b>Período</b></td>
                                                            </tr>
                                                    ';

                                                    foreach ($result as $rs) {
                                                        $tr_objetivo_pais .= '
                                                            <tr>
                                                                <td align="center">' . $rs["paidescricao"] . '</td>
                                                                <td align="center">' . $rs["aftdtinicio"] . ' à ' . $rs["aftdtfinal"] . '</td>
                                                            </tr>
                                                        ';
                                                        //$objetivosTexto = $rs["aftobjetivo"] .' - País: '.$rs["paidescricao"].',';
                                                    }
                                                    $tr_objetivo_pais .= '</table>';
                                                    echo $tr_objetivo_pais;
                                                }
                                            }
                                        ?>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>4. Atividades/Fatos Transcorridos:</b></td>
                    </tr>
                    <tr>
                        <td><?PHP echo campo_textarea('rafatividadesfatos', 'S', 'S', '', '120', '10', '2000'); ?></td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>5. Conclusões Alcançadas:</b></td>
                    </tr>
                    <tr>
                        <td><?PHP echo campo_textarea('rafconclusao', 'S', 'S', '', '120', '10', '2000'); ?></td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>6. Sugestões em relação aos benefícios que podem ser auferidos para a área da Educação:</b></td>
                    </tr>
                    <tr>
                        <td><?PHP echo campo_textarea('rafsugestao', 'S', 'S', '', '120', '10', '2000'); ?></td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>7. Observações:</b></td>
                    </tr>
                    <tr>
                        <td><?PHP echo campo_textarea('rafobservacao', 'S', 'S', '', '120', '10', '2000'); ?></td>
                    </tr>
                </table>
                <?PHP
                    $hab_validacao = verificarValidacaoAfastamento($_SESSION['cap']['afpid']);

                    if( in_array(CAP_PERFIL_SUPER_USUARIO, $perfil) || in_array(CAP_PERFIL_ADMINISTRADOR, $perfil) || in_array(PERFIL_GABINETE, $perfil) ){

                        if( $hab_validacao != '' ){
                            $checked_validar = 'checked="checked"';
                            $msg = $hab_validacao;
                        }else{
                            $checked_validar = '';
                        }

                        if( $esdid != WF_PREENC_RELATORIO_VIAGEM ){
                            $disabled_valida = 'disabled="disabled"';
                        }
                ?>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2"> Validação </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <input type="checkbox" id="btnValidar"  name="btnValidar" value="Validar" onclick="validarAfastamento();" <?=$checked_validar;?> <?=$disabled_valida;?>>
                            <span style="position:relative; top:-3px; margin-left:1px; font-weight:bold;"> - Validado pela AI. </span>
                            <span style="position:relative; top:-3px; margin-left:10px; font-weight:bold;"> <?=$msg?> </span>
                        </td>
                    </tr>
                </table>
                <?PHP
                    }

                    $esdid = pegarEstadoDocumento($docid);
                    if ( $_SESSION['cap']['afpid'] ) {
                        $hab_imprimir = verificarPreenchimentoRelatorio($_SESSION['cap']['afpid']);
                    }
                ?>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td class="SubTituloCentro" colspan="2">
                            <?PHP
                                if( $habilitado == 'S' ){
                            ?>
                                    <input type="submit" value="Salvar" id="btnSalvar" <?PHP echo $hab_validacao ? 'disabled="disabled"' : ''; ?>>
                            <?PHP
                                }

                                if( $hab_imprimir == 'S' ){
                            ?>
                                <input type="button" value="Imprimir" id="btnImprimir" onclick="imprimirAfastamento('html');"/>
                            <?PHP
                                }
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
            <?PHP
                if($docid){
            ?>
                <td valign="top"><br><?PHP wf_desenhaBarraNavegacao($docid, array('docid' => $docid, 'afpid' => $_SESSION['cap']['afpid']), ''); ?></td>
            <?PHP
                }
            ?>
    </table>
</form>

<?PHP if (in_array(PERFIL_GABINETE, $perfil) || ($esdid == WF_VIAGEM_FINALIZADA && in_array(CAP_PERFIL_SERVIDOR, $perfil))) { ?>
    <script type="text/javascript">
        jQuery('#btnSalvar').attr('disabled', true);
        jQuery('input, select, textarea').attr('disabled', true);
        jQuery('#btnValidar').attr('disabled', false);
        //jQuery('#btnImprimir').attr('disabled', false);
    </script>
<?PHP } ?>
