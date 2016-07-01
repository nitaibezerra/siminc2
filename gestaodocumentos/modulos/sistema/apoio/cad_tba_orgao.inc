<?PHP

    if($_REQUEST['requisicao']) {        
        $_REQUEST['requisicao']($_REQUEST);
    }
      
    
    function buscaDadosOrgao($dados){
        global $db;
        
        $ogsid = $dados['ogsid'];

        $sql = "
            SELECT * FROM gestaodocumentos.orgaosolicitante WHERE ogsid = {$ogsid};
        ";
        $dados = $db->pegaLinha($sql);
        
        $dados["ogsdsc"] = iconv("ISO-8859-1", "UTF-8", $dados["ogsdsc"]);
        
        echo simec_json_encode($dados);
        die;
    
    }
    
    function excluirDadosOrgao( $dados ){
        global $db;

        $sql = "
            DELETE FROM gestaodocumentos.orgaosolicitante WHERE ogsid = {$dados['ogsid']} RETURNING ogsid;
        ";
        $solid = $db->pegaUm($sql);

        if($solid > 0){
            $db->commit();
            echo '<resp>';
            echo 'OK';
            echo '</resp>';
        }
        die();
    }
    
    function salvarDadosOrgao($dados){
        global $db;

        extract($dados);
        
        if($ogsid == ''){
            $sql = "
                INSERT INTO gestaodocumentos.orgaosolicitante(
                    tpoid, 
                    ogsdsc
                )VALUES(
                    {$tpoid},
                    '{$ogsdsc}'
                )RETURNING ogsid;
             ";
        }else{
            $sql = "
                UPDATE gestaodocumentos.orgaosolicitante
                        SET tpoid   = '{$tpoid}',
                            ogsdsc  = '{$ogsdsc}'
                WHERE ogsid = {$ogsid} RETURNING ogsid;
            ";
        }
        $ogsid = $db->pegaUm($sql);
        
        if($ogsid > 0){
            $db->commit();
            $db->sucesso('', '');
        }
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';
    
    //$db->cria_aba($abacod_tela, $url, '');
    
    monta_titulo('Cadastrar Orgão','Gestão de Demandas SERES');
    
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
                $('#tr_orgao').css("display", "");
                $('#tr_tipo_orgao').css("display", "");
                $('#tr_botao').css("display", "");
                //MUDAR A IMAGEM E O VALOR.
                $('#sinal_mais').css("display", "none");
                $('#sinal_menos').css("display", "");
            }else{
                //ESCONDE AS TR
                $('#tr_orgao').css("display", "none");
                $('#tr_tipo_orgao').css("display", "none");
                $('#tr_botao').css("display", "none");
                //MUDAR A IMAGEM E VALOR
                $('#sinal_mais').css("display", "");
                $('#sinal_menos').css("display", "none");
            }
        }
        
        function buscaDadosOrgao( ogsid ){ 
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=buscaDadosOrgao&ogsid="+ogsid,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    $('#ogsid').val(dados.ogsid);
                    $('#tpoid').val(dados.tpoid);
                    $('#ogsdsc').val(dados.ogsdsc);
                }
            });
        }
        
        function excluirDadosOrgao(ogsid){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            if( confirma ){
                $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=excluirDadosOrgao&ogsid="+ogsid,
                    asynchronous: false,
                    success: function(resp){
                        resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                        if( trim(resp) == 'OK' ){
                            alert('Operação Realizada com sucesso!');
                            window.location.reload();
                        }else{
                            alert('Ocorreu um problema, não foi possivél realizar a operação. Este solicitante deve esta sendo usado em alguma Demanda!');
                        }
                    }
                });
            }
        }

        function limparDadosTipoOrgao(){
            $('#tpoid').val('');
            $('#tpodsc').val('');
            return true;
        }

        function salvarDadosOrgao(){
            var tpoid   = $('#tpoid');
            var ogsdsc  = $('#ogsdsc');
            
            var erro;
            
            if(!tpoid.val()){
                alert('O campo "Tipo de Orgão" é um campo obrigatório!');
                tpoid.focus();
                erro = 1;
                return false;
            }            
            
            if(!ogsdsc.val()){
                alert('O campo "Descrição" é um campo obrigatório!');
                ogsdsc.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarDadosOrgao');
                $('#formulario').submit();
            }
        }
        
        function pesquisarOrgao(param){
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
            <input type="hidden" name="ogsid" id="ogsid" value="" />
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR ORGÃO</td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="33%">Tipo de Orgão:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tpoid as codigo, 
                                    tpodsc as descricao 
                            FROM gestaodocumentos.tipoorgao
                            ORDER BY tpodsc
                        ";
                        $db->monta_combo("tpoid", $sql, 'S', 'Selecione...', '', '', '', '420', 'S', 'tpoid', false, null, 'Tipo de Orgão');
                    ?>
                </td>
            </tr>

            <tr>
                <td class = "subtitulodireita">Descrição:</td>
                <td>
                    <?PHP
                        echo campo_texto('ogsdsc', 'N', 'S', 'Descrição', 56, 100, '', '', '', '', '', 'id="ogsdsc"', '', $ogsdsc); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosOrgao();">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDadosTipoOrgao();"> 
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
                    PESQUISAR POR ORGÃO
                </td>
            </tr>
            <tr id="tr_orgao" style="display: none;">
                <td class = "subtitulodireita" width="33%">Orgão:</td>
                <td>
                    <?PHP
                        echo campo_texto('ogsdsc_pes', 'N', 'S', 'Orgão', 56, 30, '', '', '', '', '', 'id="ogsdsc_pes"', '', $ogsdsc_pes); 
                    ?>
                </td>
            </tr>
            <tr id="tr_tipo_orgao" style="display: none;">
                <td class = "subtitulodireita" width="30%">Tipo de Orgão:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tpoid as codigo, 
                                    tpodsc as descricao 
                            FROM gestaodocumentos.tipoorgao
                            ORDER BY tpodsc
                        ";
                        $db->monta_combo("tpoid_pes", $sql, 'S', 'Selecione...', '', '', '', '420', 'N', 'tpoid_pes', false, null, 'Tipo de Orgão');
                    ?>
                </td>
            </tr>
            <tr id="tr_botao" style="display: none;">
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarOrgao('fil');">  
                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarOrgao('tudo');">  
                </td>
            </tr>
        </table>
        
    </form>
    <br>
    <div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
        <?PHP
            if( $_REQUEST['tipo_fil'] == 'fil' ){
                if ($_REQUEST['ogsdsc_pes']) {
                    $ogsdsc = removeAcentos(trim($_REQUEST['ogsdsc_pes']));
                    $where = " AND public.removeacento(o.ogsdsc) ilike ('%{$ogsdsc}%') ";
                }

                if ($_REQUEST['tpoid_pes']) {
                    $tpoid = $_REQUEST['tpoid_pes'];
                    $where .= " AND t.tpoid = {$tpoid} ";
                }
            }else{
                $where = "";
            }

            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDadosOrgao('||o.ogsid||');\" title=\"Editar Solicitante\" >
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosOrgao('||o.ogsid||');\" title=\"Excluir Solicitante\" >
                </center>        
            ";

            $sql = "
                SELECT  '{$acao}' as acao,
                        t.tpodsc,
                        o.ogsdsc
                FROM gestaodocumentos.orgaosolicitante AS o
                
                JOIN gestaodocumentos.tipoorgao AS t ON t.tpoid = o.tpoid

                WHERE 1=1 {$where} 

                ORDER BY o.ogsdsc
            ";
            $cabecalho = array("Ação", "Tipo", "Descrição");  
            $alinhamento = Array('center', 'left', 'left');
            $tamanho = Array('3%', '30%', '40%');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        ?>
    </div>