<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    function alteraDadosModulo($dados){
        global $db;

        $mdlid = $dados['mdlid'];

        $sql = "
            SELECT mdlid, mdldsc FROM academico.modulo WHERE mdlid = {$mdlid};
        ";
        $dados = $db->pegaLinha($sql);

        //$dados["mdldsc"] = iconv("ISO-8859-1", "UTF-8", $dados["mdldsc"]);

        echo simec_json_encode($dados);
        die;

    }

    function excluirDadosModulo( $dados ){
        global $db;

        $sql    = "SELECT mdlid FROM academico.moduloambiente WHERE mdlid = {$dados['mdlid']}";
        $resp   = $db->pegaUm($sql);
        
        $msg = 'Não foi possível executar a operação. Tente novamente mais tarde!';
        
        if( $resp == '' ){
            $sql = "
                DELETE FROM academico.modulo WHERE mdlid = {$dados['mdlid']} RETURNING mdlid;
            ";
            $mdlid = $db->pegaUm($sql);
        }else{
            $msg = 'O "Módulo" está vinculado a um ambiente, não pode ser deletado!';
        }

        if( $mdlid > 0 ){
            $db->commit();
            $db->sucesso('', '');
        }else{
            $db->sucesso('', '', $msg);
        }
        die();
    }

    function salvarDadosModulo($dados){
        global $db;

        extract($dados);
        
        if($mdlid == ''){
            $sql = "
                INSERT INTO academico.modulo( mdldsc )VALUES( '{$mdldsc}' )RETURNING mdlid;
             ";
        }else{
            $sql = "
                UPDATE academico.modulo
                        SET mdldsc   = '{$mdldsc}'
                WHERE mdlid = {$mdlid} RETURNING mdlid;
            ";
        }
        $mdlid = $db->pegaUm($sql);

        if($mdlid > 0){
            $db->commit();
            $db->sucesso('sistema/tabelaapoio/cad_dados_modulo');
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';

    $abacod_tela = ABAS_TABELA_DE_APOIO_AMBIENTE_MODULO;
    $url = "academico.php?modulo=sistema/tabelaapoio/cad_dados_modulo&acao=A";
    $db->cria_aba($abacod_tela, $url, '');

    monta_titulo('Cadastrar Módulo','Rede federal');

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
                $('#tr_modulo').css("display", "");
                $('#tr_botao').css("display", "");
                //MUDAR A IMAGEM E O VALOR.
                $('#sinal_mais').css("display", "none");
                $('#sinal_menos').css("display", "");
            }else{
                //ESCONDE AS TR
                $('#tr_orgao').css("display", "none");
                $('#tr_botao').css("display", "none");
                //MUDAR A IMAGEM E VALOR
                $('#sinal_mais').css("display", "");
                $('#sinal_menos').css("display", "none");
            }
        }

        function alteraDadosModulo( mdlid ){
            $.ajax({
                type    : "POST",
                url     : "academico.php?modulo=sistema/tabelaapoio/cad_dados_modulo&acao=A",
                data    : "requisicao=alteraDadosModulo&mdlid="+mdlid,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    $('#mdlid').val(dados.mdlid);
                    $('#mdldsc').val(dados.mdldsc);
                }
            });
        }

        function excluirDadosModulo( mdlid ){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            
            if( confirma ){
                $('#mdlid').val(mdlid);
                $('#requisicao').val('excluirDadosModulo');
                $('#formulario').submit();
            }
        }

        function limparDadosTipoOrgao(){
            $('#mdlid').val('');
            $('#mdldsc').val('');
            return true;
        }

        function salvarDadosModulo(){
            var mdldsc   = $('#mdldsc');

            var erro;

            if(!mdldsc.val()){
                alert('O campo "MÓDULO" é um campo obrigatório!');
                mdldsc.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarDadosModulo');
                $('#formulario').submit();
            }
        }

        function pesquisarModulo(param){
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
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR MÓDULO</td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="33%">Código:</td>
                <td>
                    <?PHP
                        echo campo_texto('mdlid', 'S', 'N', 'Código', 20, 12, '', '', '', '', '', 'id="mdlid"', '', $mdlid);
                    ?>
                </td>
            </tr>

            <tr>
                <td class = "subtitulodireita">Módulo:</td>
                <td>
                    <?PHP
                        echo campo_texto('mdldsc', 'N', 'S', 'Descrição', 70, 100, '', '', '', '', '', 'id="mdldsc"', '', $mdldsc);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosModulo();">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="location.href='academico.php?modulo=sistema/tabelaapoio/cad_dados_modulo&acao=A';">
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
                    PESQUISAR POR MÓDULO
                </td>
            </tr>
            <tr id="tr_modulo" style="display: none;">
                <td class = "subtitulodireita" width="33%">Módulo:</td>
                <td>
                    <?PHP
                        echo campo_texto('mdldsc_pes', 'N', 'S', 'Módulo', 56, 30, '', '', '', '', '', 'id="mdldsc_pes"', '', $mdldsc_pes);
                    ?>
                </td>
            </tr>
            <tr id="tr_botao" style="display: none;">
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarModulo('fil');">
                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarModulo('tudo');">
                </td>
            </tr>
        </table>

    </form>
    <br>
    <div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
        <?PHP
            if( $_REQUEST['tipo_fil'] == 'fil' ){
                if ($_REQUEST['mdldsc_pes']) {
                    $mdldsc = removeAcentos(trim($_REQUEST['mdldsc_pes']));
                    $where = " AND public.removeacento(m.mdldsc) ilike ('%{$mdldsc}%') ";
                }
            }else{
                $where = "";
            }

            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alteraDadosModulo('||m.mdlid||');\" title=\"Editar Solicitante\" >
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosModulo('||m.mdlid||');\" title=\"Excluir Solicitante\" >
                </center>
            ";

            $sql = "
                SELECT  '{$acao}' as acao,
                        m.mdldsc
                FROM  academico.modulo AS m

                WHERE 1=1 {$where}

                ORDER BY m.mdldsc
            ";
            $cabecalho = array("Ação", "Descrição");
            $alinhamento = Array('center', 'left');
            $tamanho = Array('3%', '');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        ?>
    </div>