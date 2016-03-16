<?PHP

    if($_REQUEST['requisicao']) {        
        $_REQUEST['requisicao']($_REQUEST);
    }
      
    
    function buscaDadosTipoOrgao($dados){
        global $db;
        
        $tpoid = $dados['tpoid'];

        $sql = "
            SELECT * FROM gestaodocumentos.tipoorgao WHERE tpoid = {$tpoid};
        ";
        $dados = $db->pegaLinha($sql);
        
        $dados["tpodsc"] = iconv("ISO-8859-1", "UTF-8", $dados["tpodsc"]);
        
        echo simec_json_encode($dados);
        die;
    
    }
    
    function excluirDadosTipoOrgao( $dados ){
        global $db;

        $sql = "
            DELETE FROM gestaodocumentos.tipoorgao WHERE tpoid = {$dados['tpoid']} RETURNING tpoid;
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
    
    function salvarDadosTipoOrgao($dados){
        global $db;

        extract($dados);
        
        if($tpoid == ''){
            $sql = "
                INSERT INTO gestaodocumentos.tipoorgao(
                    tpodsc,
                    tpsid
                )VALUES(
                    '{$tpodsc}',
                    {$tpsid}
                )RETURNING tpoid;
             ";
        }else{
            $sql = "
                UPDATE gestaodocumentos.tipoorgao
                        SET tpodsc   = '{$tpodsc}',
                            tpsid    = {$tpsid}
                WHERE tpoid = {$tpoid} RETURNING tpoid;
            ";
        }
        $tpoid = $db->pegaUm($sql);
        
        if($ogsid > 0){
            $db->commit();
            $db->sucesso('', '');
        }
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';
    monta_titulo('Cadastrar Tipo de Orgão','Gestão de Demandas SERES');
    
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
                $('#tr_tipo_solicitante').css("display", "");
                $('#tr_botao').css("display", "");
                //MUDAR A IMAGEM E O VALOR.
                $('#sinal_mais').css("display", "none");
                $('#sinal_menos').css("display", "");
            }else{
                //ESCONDE AS TR
                $('#tr_orgao').css("display", "none");
                $('#tr_tipo_orgao').css("display", "none");
                $('#tr_tipo_solicitante').css("display", "none");
                $('#tr_botao').css("display", "none");
                //MUDAR A IMAGEM E VALOR
                $('#sinal_mais').css("display", "");
                $('#sinal_menos').css("display", "none");
            }
        }
        
        function buscaDadosTipoOrgao( tpoid ){ 
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=buscaDadosTipoOrgao&tpoid="+tpoid,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    $('#tpoid').val(dados.tpoid);
                    $('#tpsid').val(dados.tpsid);
                    $('#tpodsc').val(dados.tpodsc);
                }
            });
        }
        
        function excluirDadosTipoOrgao( tpoid ){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            if( confirma ){
                $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=excluirDadosTipoOrgao&tpoid="+tpoid,
                    asynchronous: false,
                    success: function(resp){
                        resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                        if( trim(resp) == 'OK' ){
                            alert('Operação Realizada com sucesso!');
                            window.location.reload();
                        }else{
                            alert('Ocorreu um problema, não foi possivél realizar a operação. Este Tipo de órgão deve esta sendo usado em alguma Demanda!');
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
        
        function salvarDadosTipoOrgao(){
            var tpsid   = $('#tpsid');
            var tpodsc   = $('#tpodsc');
            
            var erro;
            
            if(!tpsid.val()){
                alert('O campo "Tipo Solicitante" é um campo obrigatório!');
                tpsid.focus();
                erro = 1;
                return false;
            }
            if(!tpodsc.val()){
                alert('O campo "Descrição" é um campo obrigatório!');
                tpodsc.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarDadosTipoOrgao');
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
            <input type="hidden" name="tpoid" id="tpoid" value="" />
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR TIPO DE ORGÃO</td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="33%">Tipo Solicitante:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tpsid as codigo, 
                                    tpsdsc as descricao 
                            FROM gestaodocumentos.tiposolicitante
                            WHERE tpstipo = 3
                            ORDER BY tpsdsc
                        ";
                        $db->monta_combo("tpsid", $sql, 'S', 'Selecione...', '', '', '', '420', 'S', 'tpsid', false, null, 'Tipo Solicitante');
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="33%">Descrição:</td>
                <td>
                    <?PHP
                        echo campo_texto('tpodsc', 'S', 'S', 'Descrição', 57, 100, '', '', '', '', '', 'id="tpodsc"', '', $tpodsc); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosTipoOrgao();"> 
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
                    PESQUISAR TIPO DE ORGÃO
                </td>
            </tr>
            <tr id="tr_tipo_solicitante" style="display: none;">
                <td class = "subtitulodireita" width="33%">Tipo Solicitante:</td>
                <td>
                    <?PHP
                        $sql = "
                            SELECT  tpsid as codigo, 
                                    tpsdsc as descricao 
                            FROM gestaodocumentos.tiposolicitante
                            WHERE tpstipo = 3
                            ORDER BY tpsdsc
                        ";
                        $db->monta_combo("tpsid_pes", $sql, 'S', 'Selecione...', 'pesquisarOrgao(\'fil\')', '', '', '420', 'S', 'tpsid_pes', false, null, 'Tipo Solicitante');
                    ?>
                </td>
            </tr>
            <tr id="tr_tipo_orgao" style="display: none;">
                <td class = "subtitulodireita" width="33%">Tipo de Orgão:</td>
                <td>
                    <?PHP
                        echo campo_texto('tpodsc_pes', 'N', 'S', 'Tipo de Orgão', 56, 30, '', '', '', '', '', 'id="tpodsc_pes"', '', $tpodsc_pes); 
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
                if($_REQUEST['tpsid_pes']){
                    $tpsid = $_REQUEST['tpsid_pes'];
                    $where .= " AND t.tpsid = {$tpsid} ";
                }
                
                if ($_REQUEST['tpodsc_pes']) {
                    $tpodsc = removeAcentos(trim($_REQUEST['tpodsc_pes']));
                    $where .= " AND public.removeacento(t.tpodsc) ilike ('%{$tpodsc}%') ";
                }
            }else{
                $where = "";
            }

            $acao = "
                <center>
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDadosTipoOrgao('||t.tpoid||');\" title=\"Editar Solicitante\" >
                    <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosTipoOrgao('||t.tpoid||');\" title=\"Excluir Solicitante\" >
                </center>        
            ";

            $sql = "
                SELECT  '{$acao}' as acao,
                        t.tpodsc,
                        ts.tpsdsc
                FROM gestaodocumentos.tipoorgao AS t
                JOIN gestaodocumentos.tiposolicitante AS ts ON ts.tpsid = t.tpsid
                
                WHERE ts.tpstipo = 3 {$where} 

                ORDER BY t.tpodsc ASC
            ";
            $cabecalho = array("Ação", "Descrição", "Tipo Solicitante");  
            $alinhamento = Array('center', 'left', );
            $tamanho = Array('5%', '55%', '40%');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        ?>
    </div>