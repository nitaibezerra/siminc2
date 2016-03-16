<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    function salvarGeraOsExtraor( $dados, $anexo ){
        global $db;

        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        $eveid              = $_SESSION['evento']['eveid'];
        $jusid              = $dados['jusid'];
        $jus_usucpf         = $dados['jus_usucpf'];
        $jus_motivo         = addslashes( $dados['jus_motivo'] );
        $jus_dsc_just       = addslashes( $dados['jus_dsc_just'] );
        $jus_data_insercao  = date('Y-m-d H:m:s');
        $jus_data_alteracao = date('Y-m-d H:m:s');

        if( $jusid == '' ){
            $sql = "
                INSERT INTO evento.just_os_extraordinario(
                        eveid, jus_usucpf, jus_motivo, jus_dsc_just, jus_data_insercao
                    ) VALUES (
                        {$eveid}, '{$jus_usucpf}', '{$jus_motivo}', '{$jus_dsc_just}', '{$jus_data_insercao}'
                ) RETURNING jusid;
            ";
            $result_jusid = $db->pegaUm($sql);
        }else{
            $sql = "
                UPDATE evento.just_os_extraordinario
                    SET jus_usucpf          = '{$jus_usucpf}',
                        jus_motivo          = '{$jus_motivo}',
                        jus_dsc_just        = '{$jus_dsc_just}',
                        jus_data_alteracao  = '{$jus_data_alteracao}'
                    WHERE jusid = {$jusid}  RETURNING jusid;
            ";
            $result_jusid = $db->pegaUm($sql);
        }
        
        if( $result_jusid > 0 && $anexo['arquivo']['size'] > 0 ){

            $campos = array(
                'jusid'         => $result_jusid,
                'anxjus_usucpf' => "'{$jus_usucpf}'"
            );

            $files = new FilesSimec('anexo_just_os_extraordinario', $campos, 'evento');

            if ( $files ) {
                $arquivoSalvo = $files->setUpload('Adminstrativo - Eventos - Anexo para Justificativa de Geração de OS extraordinaria');//, NULL, true, 'anxjusid');
            }
        }
        
         if( $result_jusid > 0){
            $_SESSION['evento']['jusid'] = $result_jusid;
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_gera_os_estraordinario', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_gera_os_estraordinario', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }
    
    function buscarDadosJustificativa(){
        global $db;
        
        $eveid = $_SESSION['evento']['eveid'];
        
        $sql = "
            SELECT * FROM evento.just_os_extraordinario where eveid = {$eveid}
        ";
        return $db->pegaLinha($sql);
    }
    
    $dados = buscarDadosJustificativa();
    
    if( $_SESSION['evento']['jusid'] == '' ){
        if( $dados['jusid'] > 0 ){
            $_SESSION['evento']['jusid'] = $dados['jusid'];
        }
    }
    
    
    echo "<br>";
    monta_titulo( 'Administrativo - Eventos', 'Justificativa para Gerar OS. de forma extraordinaria' );
    echo "<br>";

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">
    
    $(document).ready(function(){
        //É PASSADO PARA A JANELA PAI APENAS PARA CONTROLE. VERIFICAÇÃO QUE JA FOI FEITA A JUSTIFICATIVA DA OS EXTRAORDINARIA.
        $(opener.document.getElementById("jusid")).val(<?=$_SESSION['evento']['jusid']?>);
    });

    function dowloadAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadAnexo');
        $('#formulario').submit();
    }

    function excluirAnexo( arqid ){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirAnexo');
            $('#formulario').submit();
        }
    }

    function salvarGeraOsExtraor(){
        var erro;
        var campos = '';
        var jusid = $('#jusid');

        if( jusid.val() == '' ){
            $.each($(".obrigatorio"), function(i, v){
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
            });
        }

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarGeraOsExtraor');
            $('#formulario').submit();
        }
    }

    function fecharJanela(){
        window.close();
    }

</script>


<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="jusid" name="jusid" value="<?=$dados['jusid'];?>"/>
    <input type="hidden" id="arqid" name="arqid" value=""/>
    <input type="hidden" id="jus_usucpf" name="jus_usucpf" value="<?=$_SESSION['usucpf']?>"/>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="20%">Responsável:</td>
            <td>
                <?PHP
                    $usunome = $_SESSION['usunome'];
                    echo campo_texto('usunome', 'N', 'N', 'Responsável', 77, 200, '', '', 'left', '', 0, 'id="usunome"', '', $usunome, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="20%"> Motivo: </td>
            <td>
                <?PHP
                    $jus_motivo = $dados['jus_motivo'];
                    echo campo_texto('jus_motivo', 'S', 'S', 'Motivo', 77, 100, '', '', 'left', '', 0, 'id="jus_motivo"', '', $jus_motivo, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Justificativa: </td>
            <td>
                <?PHP
                    $jus_dsc_just = $dados['jus_dsc_just'];
                    echo campo_textarea('jus_dsc_just', 'S', 'S', '', 100, 7, '500', '', 0, '', false, 'Justificativa', $jus_dsc_just,'85%');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Arquivo: </td>
            <td>
                <input type="file" class="obrigatorio" name="arquivo" id="arquivo" title="Arquivo"/>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarGeraOsExtraor();"/>
                <input type="button" id="cancelar" name="cancelar" value="Fechar Janela" onclick="fecharJanela();"/>                
            </td>
        </tr>
    </table>
</form>

<?PHP

   $acao = "
        <!--img title=\"Apagar arquivo\" onclick=\"excluirAnexo('|| aj.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" /-->
        <img title=\"Baixar arquivo\" onclick=\"dowloadAnexo('|| aj.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
    ";

    $sql = "
        SELECT  '{$acao}',
                u.usunome,
                jus_motivo,
                a.arqnome||'.'||arqextensao AS arquivo,
                to_char(jus_data_insercao, 'DD/MM/YYYY') AS data_insercao

        FROM evento.just_os_extraordinario AS j

        JOIN seguranca.usuario AS u ON u.usucpf = j.jus_usucpf        
        JOIN evento.anexo_just_os_extraordinario AS aj ON aj.jusid = j.jusid
        JOIN public.arquivo AS a ON a.arqid = aj.arqid
        
        WHERE j.eveid = {$_SESSION['evento']['eveid']}

    ";
    $cabecalho = array("Ação", "Usuário", "Motivo", "Arquivo", "Data de Inserção");
    $alinhamento = Array('center', '', '', '', '', '');
    $tamanho = Array('3%', '', '', '', '', '');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>