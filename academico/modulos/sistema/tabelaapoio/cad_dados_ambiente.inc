<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }


    function alteraDadosAmbiente($dados){
        global $db;

        $ambid = $dados['ambid'];

        $sql = "
            SELECT ambid, ambdsc, undid FROM academico.ambiente WHERE ambid = {$ambid};
        ";
        $dados = $db->pegaLinha($sql);

        //$dados["ambdsc"] = iconv("ISO-8859-1", "UTF-8", $dados["ambdsc"]);

        echo simec_json_encode($dados);
        die;

    }

    function excluirDadosAmbiente( $dados ){
        global $db;
        
        $sql    = "SELECT ambid FROM academico.moduloambiente WHERE ambid = {$dados['ambid']}";
        $resp   = $db->pegaUm($sql);
        
        $msg = 'Não foi possível executar a operação. Tente novamente mais tarde!';
        
        if( $resp == '' ){
            $sql = "
                DELETE FROM academico.ambiente WHERE ambid = {$dados['ambid']} RETURNING ambid;
            ";
            $ambid = $db->pegaUm($sql);
        }else{
            $msg = 'O "Ambiente" está vinculado a um módulo, não pode ser deletado!';
        }

        if( $ambid > 0 ){
            $db->commit();
            $db->sucesso('sistema/tabelaapoio/cad_dados_ambiente', '');
        }else{
            $db->sucesso('sistema/tabelaapoio/cad_dados_ambiente', '', $msg);
        }
        die();
    }

    function salvarDadosAmbiente($dados){
        global $db;

        extract($dados);

        if($ambid == ''){
            $sql = "
                INSERT INTO academico.ambiente( ambdsc, undid )VALUES( '{$ambdsc}', {$undid} )RETURNING ambid;
             ";
        }else{
            $sql = "
                UPDATE academico.ambiente
                        SET ambdsc   = '{$ambdsc}', undid = {$undid}
                WHERE ambid = {$ambid} RETURNING ambid;
            ";
        }
        $ambid = $db->pegaUm($sql);

        if($ambid > 0){
            $db->commit();
            $db->sucesso( 'sistema/tabelaapoio/cad_dados_ambiente' );
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';

    $abacod_tela = ABAS_TABELA_DE_APOIO_AMBIENTE_MODULO;
    $url = 'academico.php?modulo=sistema/tabelaapoio/cad_dados_ambiente&acao=A';
    $db->cria_aba($abacod_tela, $url, '');

    monta_titulo('Cadastrar Ambiente','Rede federal');

?>
    <head>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">

        $( document ).ready( function() {

        });

        function abrirPesquisa(opc){
            if(opc == 'mais'){
                //ABRE AS TR
                $('#tr_ambiente').css("display", "");
                $('#tr_botao').css("display", "");
                //MUDAR A IMAGEM E O VALOR.
                $('#sinal_mais').css("display", "none");
                $('#sinal_menos').css("display", "");
            }else{
                //ESCONDE AS TR
                $('#tr_ambiente').css("display", "none");
                $('#tr_botao').css("display", "none");
                //MUDAR A IMAGEM E VALOR
                $('#sinal_mais').css("display", "");
                $('#sinal_menos').css("display", "none");
            }
        }

        function alteraDadosAmbiente( ambid ){
            $.ajax({
                type    : "POST",
                url     : "academico.php?modulo=sistema/tabelaapoio/cad_dados_ambiente&acao=A",
                data    : "requisicao=alteraDadosAmbiente&ambid="+ambid,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    $('#ambid').val(dados.ambid);
                    $('#ambdsc').val(dados.ambdsc);
                    $('#undid').val(dados.undid);
                }
            });
        }

        function excluirDadosAmbiente( ambid ){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            
            if( confirma ){
                $('#ambid').val( ambid );
                $('#requisicao').val('excluirDadosAmbiente');
                $('#formulario').submit();
            }
        }

        function limparDadosAmbiente(){
            $('#ambid').val('');
            $('#ambdsc').val('');
            $('#undid').val('');
            return true;
        }

        function salvarDadosAmbiente(){
            var ambdsc   = $('#ambdsc');
            var undid = $('#undid');

            var erro;

            if(!ambdsc.val()){
                alert('O campo "AMBIENTE" é um campo obrigatório!');
                ambdsc.focus();
                erro = 1;
                return false;
            }
            if(!undid.val()){
                alert('O campo "UNIDADE DE MEDIDA" é um campo obrigatório!');
                undid.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarDadosAmbiente');
                $('#formulario').submit();
            }
        }

        function pesquisarAmbiente(param){
            if(trim(param) == 'fil'){
                $('#tipo_fil').val('fil');
                $('#formulario').submit();
            }else{
                $('#tipo_fil').val('');
                $('#formulario').submit();
            }
        }

    </script>

    </head>
    <br>
    <form name="formulario" id="formulario" action="" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR AMBIENTE</td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="33%">Código:</td>
                <td>
                    <?PHP
                        echo campo_texto('ambid', 'S', 'N', 'Código do Ambiente', 20, 12, '', '', '', '', '', 'id="ambid"', '', $ambid);
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Ambiente:</td>
                <td>
                    <?PHP
                        echo campo_texto('ambdsc', 'N', 'S', 'Descrição', 70, 100, '', '', '', '', '', 'id="ambdsc"', '', $ambdsc);
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Unidade de medida:</td>
                <td>
                    <?PHP
                    $sql = "
                            SELECT  u.undid as codigo,
                                    u.unddsc as descricao
                            FROM academico.unidademed as u
                            ORDER BY u.unddsc
                        ";
                    $db->monta_combo('undid', $sql, 'S', "Digite a Unidade de medida...", '', '', '', 400, 'S', 'undid', '', $undid, 'Unidade de medida', '', 'chosen-select');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosAmbiente();">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDadosAmbiente();">
                </td>
            </tr>
        </table>

        <br>

        <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">
                    <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
                    <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
                    &nbsp;&nbsp;
                    PESQUISAR POR AMBIENTE
                </td>
            </tr>
            <tr id="tr_ambiente" style="display: none;">
                <td class = "subtitulodireita" width="33%">Ambiente:</td>
                <td>
                    <?PHP
                        echo campo_texto('ambdsc_pes', 'N', 'S', 'Ambiente', 56, 30, '', '', '', '', '', 'id="ambdsc_pes"', '', $ambdsc_pes);
                    ?>
                </td>
            </tr>
            <tr id="tr_botao" style="display: none;">
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarAmbiente('fil');">
                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarAmbiente('tudo');">
                </td>
            </tr>
        </table>

    </form>
    <br>
    <div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
        <?PHP
            if( $_REQUEST['tipo_fil'] == 'fil' ){
                if ($_REQUEST['ambdsc_pes']) {
                    $ambdsc = removeAcentos(trim($_REQUEST['ambdsc_pes']));
                    $where = " AND public.removeacento(a.ambdsc) ilike ('%{$ambdsc}%') ";
                }
            }else{
                $where = "";
            }

            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alteraDadosAmbiente('||a.ambid||');\" title=\"Editar Solicitante\" >
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosAmbiente('||a.ambid||');\" title=\"Excluir Solicitante\" >
                </center>
            ";

            /*
            $sql = "
                SELECT  '{$acao}' as acao,
                        a.ambdsc
                FROM  academico.ambiente AS A

                WHERE 1=1 {$where}

                ORDER BY a.ambdsc
            ";
            */
        $sql = "
                SELECT  '{$acao}' as acao,
                        a.ambdsc,
                        unddsc
                FROM  academico.ambiente AS a
                JOIN academico.unidademed AS u ON u.undid = a.undid

                WHERE 1=1 {$where}

                ORDER BY a.ambdsc
            ";
            //$cabecalho = array("Ação", "Descrição");
            $cabecalho = array("Ação", "Descrição", "Und. Medida");
            $alinhamento = Array('center', 'left', 'left');
            $tamanho = Array('3%', '');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        ?>
    </div>