<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

//    function alteraDadosModulo($dados){
//        global $db;
//
//        $mdlid = $dados['mdlid'];
//
//        $sql = "
//            SELECT mdlid, mdldsc FROM academico.modulo WHERE mdlid = {$mdlid};
//        ";
//        $dados = $db->pegaLinha($sql);
//
//        //$dados["mdldsc"] = iconv("ISO-8859-1", "UTF-8", $dados["mdldsc"]);
//
//        echo simec_json_encode($dados);
//        die;
//
//    }

    function excluirDadosModuloAmbiente( $dados ){
        global $db;

        $sql = "
            DELETE FROM academico.moduloambiente WHERE mdlid = {$dados['mdlid']} AND ambid = {$dados['ambid']} RETURNING mdlid;
        ";
        $mdlid = $db->pegaUm($sql);

        if( $mdlid > 0 ){
            $db->commit();
            $db->sucesso('sistema/tabelaapoio/cad_dados_modulo_ambiente');
        }
        die();
    }

    function salvarDadosAmbienteModulo($dados){
        global $db;

        extract($dados);
        
        if($ambid != '' && $mdlid != ''){
            $sql = "
                INSERT INTO academico.moduloambiente( ambid, mdlid )VALUES( {$ambid}, {$mdlid}  )RETURNING ambid;
            ";
        }
        $ambmdlid = $db->pegaUm($sql);

        if($ambmdlid > 0){
            $db->commit();
            $db->sucesso('sistema/tabelaapoio/cad_dados_modulo_ambiente');
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';

    $abacod_tela = ABAS_TABELA_DE_APOIO_AMBIENTE_MODULO;
    $url = "academico.php?modulo=sistema/tabelaapoio/cad_dados_modulo_ambiente&acao=A";
    $db->cria_aba($abacod_tela, $url, '');

    monta_titulo('Cadastrar o Ambiente do Módulo','Rede federal');

?>
    <head>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">

         $1_11(document).ready(function(){
            //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
            $1_11(window).unload(
                $1_11('.chosen-select').chosen()
            );
        });

        function abrirPesquisa(opc){
            if(opc == 'mais'){
                //ABRE AS TR
                $('#tr_modulo').css("display", "");
                $('#tr_ambiente').css("display", "");
                $('#tr_botao').css("display", "");
                //MUDAR A IMAGEM E O VALOR.
                $('#sinal_mais').css("display", "none");
                $('#sinal_menos').css("display", "");
            }else{
                //ESCONDE AS TR
                $('#tr_modulo').css("display", "none");
                $('#tr_ambiente').css("display", "none");
                $('#tr_botao').css("display", "none");
                //MUDAR A IMAGEM E VALOR
                $('#sinal_mais').css("display", "");
                $('#sinal_menos').css("display", "none");
            }
        }

//        function alteraDadosAmbiente( mdlid ){
//            $.ajax({
//                type    : "POST",
//                url     : window.location,
//                data    : "requisicao=alteraDadosModulo&mdlid="+mdlid,
//                success: function(resp){
//                    var dados = $.parseJSON(resp);
//                    $('#mdlid').val(dados.mdlid);
//                    $('#mdldsc').val(dados.mdldsc);
//                }
//            });
//        }

        function excluirDadosModuloAmbiente( mdlid, ambid  ){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            
            if( confirma ){
                $('#mdlid').val(mdlid);
                $('#ambid').val(ambid);
                $('#requisicao').val('excluirDadosModuloAmbiente');
                $('#formulario').submit();
            }
        }

        function limparDadosAmbienteModulo(){
            $('#mdlid').val('');
            $('#ambid').val('');

            //return true;
        }

        function salvarDadosAmbienteModulo(){
            var mdlid = $('#mdlid');
            var ambid = $('#ambid');


            var erro;

            if(!mdlid.val()){
                alert('O campo "MÓDULO" é um campo obrigatório!');
                mdldsc.focus();
                erro = 1;
                return false;
            }
            if(!ambid.val()){
                alert('O campo "AMBIENTE" é um campo obrigatório!');
                ambid.focus();
                erro = 1;
                return false;
            }


            if(!erro){
                $('#requisicao').val('salvarDadosAmbienteModulo');
                $('#formulario').submit();
            }
        }

        function pesquisarModuloAmbiente(param){
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
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR O AMBIENTE DO MÓDULO</td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Módulo:</td>
                <td>
                    <?PHP
                        $mdlid = $dados['mdlid'];
                        $sql = "
                            SELECT  m.mdlid as codigo,
                                    m.mdldsc as descricao
                            FROM  academico.modulo AS m
                            ORDER BY m.mdldsc
                        ";
                        $db->monta_combo('mdlid', $sql, 'S', "Digite o Módulo procurado...", '', '', '', 400, 'S', 'mdlid', '', $mdlid, 'Módulo', '', 'chosen-select');
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="33%">Ambiente:</td>
                <td>
                    <?PHP
                        $ambid = $dados['ambid'];
                        $sql = "
                            SELECT  a.ambid as codigo,
                                    a.ambdsc as descricao
                            FROM  academico.ambiente AS a
                            ORDER BY a.ambdsc
                        ";
                        $db->monta_combo('ambid', $sql, 'S', "Digite o Ambiente procurado...", '', '', '', 400, 'S', 'ambid', '', $ambid, 'Ambiente', '', 'chosen-select');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosAmbienteModulo();">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="location.href='academico.php?modulo=sistema/tabelaapoio/cad_dados_modulo_ambiente&acao=A';">
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
                    PESQUISAR PELO AMBIENTE DO MÓDULO
                </td>
            </tr>
            <tr id="tr_ambiente" style="display: none;">
                <td class = "subtitulodireita" width="33%">Módulo:</td>
                <td>
                    <?PHP
                        echo campo_texto('mdldsc_pes', 'N', 'S', 'Módulo', 56, 30, '', '', '', '', '', 'id="mdldsc_pes"', '', $mdldsc_pes);
                    ?>
                </td>
            </tr>
            <tr id="tr_modulo" style="display: none;">
                <td class = "subtitulodireita" width="33%">Ambiente:</td>
                <td>
                    <?PHP
                        echo campo_texto('ambdsc_pes', 'N', 'S', 'Ambiente', 56, 30, '', '', '', '', '', 'id="ambdsc_pes"', '', $ambdsc_pes);
                    ?>
                </td>
            </tr>
            <tr id="tr_botao" style="display: none;">
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarModuloAmbiente('fil');">
                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="location.href='academico.php?modulo=sistema/tabelaapoio/cad_dados_modulo_ambiente&acao=A';">
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
                if ($_REQUEST['mdldsc_pes']) {
                    $mdldsc = removeAcentos(trim($_REQUEST['mdldsc_pes']));
                    $where = " AND public.removeacento(m.mdldsc) ilike ('%{$mdldsc}%') ";
                }
            }else{
                $where = "";
            }

            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosModuloAmbiente(\''|| ma.mdlid ||'\',\''|| ma.ambid ||'\');\" title=\"Excluir Solicitante\" >
                </center>
            ";

            /*
            $sql = "
                SELECT  '{$acao}', 
                        mdldsc,
                        ambdsc,
                        unddsc
                FROM academico.moduloambiente AS ma

                JOIN academico.ambiente AS a ON a.ambid = ma.ambid
                JOIN academico.modulo AS m ON m.mdlid = ma.mdlid
                JOIN academico.unidademed AS u ON u.undid = ma.undid


                WHERE 1=1 {$where}

                ORDER BY m.mdldsc
            ";
            */
            $sql = "
                SELECT  '{$acao}',
                        mdldsc,
                        ambdsc
                FROM academico.moduloambiente AS ma

                JOIN academico.ambiente AS a ON a.ambid = ma.ambid
                JOIN academico.modulo AS m ON m.mdlid = ma.mdlid

                WHERE 1=1 {$where}

                ORDER BY m.mdldsc
            ";
            //$cabecalho = array("Ação", "Módulo", "Ambiente", "Und. Medida");
            $cabecalho = array("Ação", "Módulo", "Ambiente");
            $alinhamento = Array('center', 'left');
            $tamanho = Array('3%', '');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        ?>
    </div>