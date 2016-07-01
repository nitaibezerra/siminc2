<?PHP
    header('content-type: text/html; charset=ISO-8859-1');

    if( $_REQUEST['feaid'] == '' && $_REQUEST['opc'] != 'ED' ){
        $parametros = "&entid={$_REQUEST['entid']}&funid={$_REQUEST['funid']}&opc={$_REQUEST['opc']}";
        $db->sucesso( 'principal/editardirigente', $parametros, 'Não é permitido pular etapa. Mesmo que exista dados na Aba "Editar Dirigentes", confira e/ou atualize os dados e "GRAVE", para dar continuidade ao processo!');
    }

    $funid  = $_REQUEST['funid'];
    $sit    = $_REQUEST['sit'];

    #_funcoes_dados_dirigentes.php
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    if($_REQUEST['download'] == 'S'){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($_GET['arqid']);
        exit;
    }

    if( $_REQUEST['opc'] == 'ED' ){
        $sql = "
            SELECT entid
            FROM entidade.entidade ent
            WHERE ent.entid = {$_REQUEST['entid']}
        ";
        $dg_entid = $db->pegaUm($sql);

        if( $funid != '' ){
            $feaid = buscarIdFuncaoAssoc($dg_entid, $funid);
        }

        if( $dg_entid > 0 ){
            $sit = $sit == 'A' ? 't' : 'f';
            $sql = "
                SELECT mcsid, arqid FROM academico.membroconselho WHERE entid = {$dg_entid} AND feaid = {$feaid} AND mcssituacao = '{$sit}'
            ";
            $resp = $db->pegaLinha($sql);

            if( $resp['arqid'] > 0 ){
                $habilita = 'disabled="disabled"';
            }
        }
    }else{
        $dg_entid   = $_REQUEST['entid'];
        $feaid      = $_REQUEST['feaid'];
    }

    if( $_REQUEST['mcsid'] != '' || $resp['mcsid'] > 0 ){
        $mcsid = $_REQUEST['mcsid'] ? $_REQUEST['mcsid'] : $resp['mcsid'];
        $dados = buscarDadosAdicionaisDirigentes( $mcsid );
    }

    $abacod_tela    = ABAS_DADOS_ADICIONAIS_DIRIGENTES;
    $url            = "academico.php?modulo=principal/dados_adicionais_dirigentes&acao=A";
    $parametros     = "&entid={$_REQUEST['entid']}&funid={$_REQUEST['funid']}&opc={$_REQUEST['opc']}";
    $db->cria_aba( $abacod_tela, $url, $parametros );

    $gravar = true;
    if($_SESSION['academico']['entidadenivel'] != 'campus' && permissaoInterna(array(PERFIL_DIRETOR_CAMPUS,PERFIL_INTERLOCUTOR_CAMPUS))){
        $gravar = false;
    }
?>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

        <link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
        <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"></link>

        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
        <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
        <script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

    </head>
    <br>
    <form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="arqid" id="arqid" value=""/>
            <input type="hidden" name="mcsid" id="mcsid" value="<?=$resp['mcsid'];?>" />
            <input type="hidden" name="dg_entid" id="dg_entid" value="<?=$dg_entid;?>" />
            <input type="hidden" name="feaid" id="feaid" value="<?=$feaid;?>" />
            <tr>
                <td class = "subtitulodireita" colspan="2" style="text-align: center;"> <h5> <b>Informações Adicionais</b> </h5> </td>
            </tr>
            <?PHP
                if( $funid == '21' || $funid == '24' || $funid == '40' ){
                    $style_tipo_membro = 'style="display:none;"';
                    $habi_tipo_membro = 'N';
                }else{
                    $style_tipo_membro = '';
                    $habi_tipo_membro = 'S';
                }
            ?>
            <tr id="tr_tipo_membro" <?=$style_tipo_membro;?>>
                <td class="subtitulodireita" width="25%">Tipo de Membro:</td>
                <td>
                    <?PHP
                        $tpmid = $dados['tpmid'];

                        if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
                            $where = " WHERE t.tpmnivel = 'I' ";
                        }else{
                            $where = " WHERE t.tpmnivel = 'C' ";
                        }

                        $sql = "
                            SELECT  t.tpmid AS codigo,
                                    t.tpmdsc AS descricao

                            FROM academico.tipomembro AS t
                            {$where}
                            ORDER BY t.tpmdsc
                        ";
                        $db->monta_combo('tpmid', $sql, $habi_tipo_membro, "Selecione...", 'abreDescricaoTipoMembro', '', '', 310, $habi_tipo_membro, 'tpmid', '', $tpmid, 'Tipo de Membro', '', '');
                    ?>
                </td>
            </tr>
            <?PHP
                if( $funid == '40' ){
                    $style_mandato = 'style="display:none;"';
                    $hab_mandato = 'N';
                }else{
                    $style_mandato = '';
                    $hab_mandato = 'S';
                }
            ?>
            <tr id="tr_num_mandato" <?=$style_mandato;?>>
                <td class = "subtitulodireita" width="25%">Número do Mandato:</td>
                <td>
                    <?PHP
                        #VERIFICA SE A MANDATO
                        $result = verificaMandato($_REQUEST['entid'], $feaid, $mcsid);

                        if( $result['nummandato'] == '' ){
                            $sql = array(
                                array('codigo'=>'1', 'descricao'=>'Primeiro mandato')
                            );
                        }else{
                            if( $result['intevalo_anos_madato'] == 'S' ){
                                $sql = array(
                                    array('codigo'=>'1', 'descricao'=>'Primeiro mandato'),
                                    array('codigo'=>'2', 'descricao'=>'Segundo mandato')
                                );
                            }else{
                                $sql = array(
                                    array('codigo'=>'1', 'descricao'=>'Primeiro mandato')
                                );
                            }
                        }

                        $nummandato = $dados['nummandato'];
                        $db->monta_combo('nummandato', $sql, $hab_mandato, "Selecione...", '', '', '', 310, $hab_mandato, 'nummandato', '', $nummandato, 'Número do Mandato', '', '');
                    ?>
                </td>
            </tr>
            <tr id="tr_tipo_mandato" <?=$style_mandato;?>>
                <td class = "subtitulodireita" width="25%">Tipo de Mandato:</td>
                <td>
                    <?PHP
                        $tipomandato = $dados['tipomandato'];

                        $sql = "
                            SELECT  'T' AS codigo,
                                    'Pro Tempore' AS descricao
                            UNION
                            SELECT  'E' AS codigo,
                                    'Eleito' AS descricao
                            ORDER BY 1
                        ";
                        $db->monta_combo('tipomandato', $sql, $hab_mandato, "Selecione...", '', '', '', 310, $hab_mandato, 'tipomandato', '', $tipomandato, 'Tipo de Mandato', '', '');
                    ?>
                </td>
            </tr>
            <?PHP
                if( $dados['mcsdscmembro'] == '' ){
                    $style_dsc_tipo = 'style="display:none;"';
                }
            ?>
            <tr id="tr_descricao_tipo" <?=$style_dsc_tipo;?>>
                <td class="subtitulodireita" width="25%">Descrição do tipo:</td>
                <td>
                    <table>
                        <tr>
                            <td>
                                <?PHP
                                    $mcsdscmembro = $dados['mcsdscmembro'];
                                    echo campo_textarea('mcsdscmembro', 'N', 'S', '', 47, 3, 150, '', 0, '', false, 'Descrição do tipo', $mcsdscmembro);
                                ?>
                            </td>
                            <td> <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif" style="position: absolute; margin-top: -22px;"> </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="25%">Situação:</td>
                <td>
                    <?PHP
                        if( $_REQUEST['opc'] == 'NV' ){
                            if( $funid == '21' || $funid == '24' || $funid == '40' ){
                                if( $sit == 'A' ){
                                    $mcssituacao = 't';
                                }else{
                                    $mcssituacao = 'f';
                                }
                            }
                        }else{
                            $mcssituacao = $dados['mcssituacao'];
                        }

                        $sql = array(
                            array( "codigo"=>"t", "descricao"=>"Ativo"),
                            array( "codigo"=>"f", "descricao"=>"Inativo")
                        );
                        $db->monta_combo('mcssituacao', $sql, 'S', "Selecione...", '', '', '', 310, 'S', 'mcssituacao', '', $mcssituacao, 'Situação', '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Período de Vigência: </td>
                <td colspan="2">
                    <?PHP
                        $mcsvigenciainicial = $dados['mcsvigenciainicial'];
                        echo campo_data2('mcsvigenciainicial','S','S','Data inicial','DD/MM/YYYY','','', $mcsvigenciainicial, '', '', 'mcsvigenciainicial' );
                        echo " até ";
                        $mcsvigenciafinal = $dados['mcsvigenciafinal'];
                        echo campo_data2('mcsvigenciafinal','S','S','Data final','DD/MM/YYYY','','', $mcsvigenciafinal, '', '', 'mcsvigenciafinal' );
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Arquivo:</td>
                <td colspan="2">
                    <input type="file" name="arquivo" id="arquivo" style="display: inherit !important;" title="Arquivo" <?=$habilita;?>/>
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
        </table>

        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <?PHP
                        if($gravar){
                    ?>
                            <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosFomulario();">
                    <?PHP
                        }
                    ?>
                            <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">
                </td>
            </tr>
        </table>
    </form>

<?PHP

    if( $mcsid > 0 ){
        listagemAtoLegalDirigentes( $mcsid );
    }

?>

<script type="text/javascript">

    function abreDescricaoTipoMembro( tmpid ){
        if( tmpid == 16 || tmpid == 17 || tmpid == 33 || tmpid == 34 ){
            $('#tr_descricao_tipo').css('display', '');
            $('#mcsdscmembro').addClass('obrigatorio');
            $("#mcsdscmembro").append("<h1>Título</h1>");
        }else{
            $('#tr_descricao_tipo').css('display', 'none');
            $('#mcsdscmembro').attr('disabled', 'disabled');
            $('#mcsdscmembro').removeClass('obrigatorio');
        }
    }

    function excluirDocAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirDocAnexo');
            $('#formulario').submit();
        }
    }

    function fecharPopup(){
        return window.close();
    }

    function salvarDadosFomulario(){
        var objData = new Data();

        var erro;
        var campos = '';
        var resp;

        $.each($(".obrigatorio"), function(i, v){
            if (($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox')) {
                if ($(v).val() == '') {
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            } else if ($(this).attr('type') == 'radio') {
                var name = $(this).attr('name');
                var value = $(this).attr('value');
                var radio_box = $('input:radio[name=' + name + ']:checked');
                if (!radio_box.val()) {
                    erro = 1;
                    if (value == 'O') {
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
                }
            } else if ($(this).attr('type') == 'checkbox') {
                var name = $(this).attr('name');
                var value = $(this).attr('value');
                var check_box = $('input:checkbox[name=' + name + ']:checked');

                if (!check_box.val()) {
                    erro = 1;
                    if (value == 'S') {
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
                }
            }
        });

        var mcsvigenciainicial  = $('#mcsvigenciainicial').val();
        var mcsvigenciafinal    = $('#mcsvigenciafinal').val();
        var mcssituacao         = $('#mcssituacao').val();

        var fullDate = new Date()
        var twoDigitMonth = ((fullDate.getMonth().length+1) === 1) ? (fullDate.getMonth()+1) : '0' + (fullDate.getMonth()+1);
        var currentDate = fullDate.getDate() + "/" + twoDigitMonth + "/" + fullDate.getFullYear();

        if( mcssituacao == 't' ){
            resp = objData.comparaData( currentDate, mcsvigenciafinal, '>' );

            if( resp == true ){
                alert('A data de "Final da Vigência é menor que a data de Hoje!');
                return false;
            }
        }

        if( mcssituacao == 'f' ){
            resp = objData.comparaData( currentDate, mcsvigenciafinal, '<' );

            if( resp == true ){
                alert('A data de "Final da Vigência é maior que a data de Hoje!');
                return false;
            }
        }

        var result = objData.comparaData( mcsvigenciainicial, mcsvigenciafinal, '>' );

        if( result == true ){
            alert('A data de "Início da Vigência é maior que a data "Fim da Vigência"');
            $('#mcsvigenciainicial').val('');
            $('#mcsvigenciafinal').val('');
            return false;
        }

        if (erro > 0) {
            alert('Existem "campos obrigatorios" vazios! São eles: \n' + campos);
            return false;
        }

        if (!erro) {
            $('#requisicao').val('salvarDadosFomulario');
            $('#formulario').submit();
        }
    }
</script>