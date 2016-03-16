<?PHP

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    function buscarNomeColecao( $colid ){
        global $db;

        $sql = "
            SELECT coltitulo as colecao
            FROM livro.colecao
            WHERE colid = {$colid} AND prsano = '{$_SESSION['exercicio']}'
        ";
        $coltitulo = $db->pegaUm( $sql );

        return $coltitulo;
    }

    function salvarParecerMidia($dados){
        global $db;

        $tpm_id = $dados['tpm_id'];
        $ravid  = $dados['ravid'];
        $colid  = $dados['colid'];

        $sql = "
            UPDATE livro.resenhaavaliacao SET tpm_id = {$tpm_id} WHERE ravid = {$ravid} RETURNING ravid;
        ";
        $ravid = $db->pegaUm( $sql );

        if( $ravid > 0 ){
            $db->commit();
            $db->sucesso('principal/pnld_resenha_colecao', '&colid='.$colid, 'Operação realizada com sucesso', 'S', 'S');
        }else{
            $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/pnld_resenha_colecao&acao&acao=A');
        }
    }



?>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script type="text/javascript" src="../../includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

    <script type="text/javascript">

        function salvarParecerMidia(){
            var tpm_id = $('#tpm_id');

            var erro;

            if(!tpm_id.val()){
                alert('O campo "Parecer" é um campo obrigatório!');
                tpm_id.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarParecerMidia');
                $('#formulario').submit();
            }
        }
    </script>

    <form id="formulario" name="formulario" method="POST" action="">
        <input type="hidden" id="requisicao" name="requisicao" value="" />
        <input type="hidden" id="ravid" name="ravid" value="<?= $_REQUEST['ravid'] ?>" />
        <input type="hidden" id="colid" name="colid" value="<?= $_REQUEST['colid'] ?>" />

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
            <tr>
                <td class="SubtituloCentro" colspan="2">PARECER DA MÍDIA QUE COMPÕEM A COLEÇÃO</td>
            </tr>
        </table>
        <br>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
            <tr>
                <td align="right" class="SubTituloDireita" style="width:100px;">Coleção:</td>
                <td>
                    <?PHP
                        if( $_REQUEST['colid'] != '' ) {
                            $colid = $_REQUEST['colid'];
                            echo $coltitulo = buscarNomeColecao($colid);
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td align="right" class="SubTituloDireita">Parecer:</td>
                <td nowrap="nowrap">
                    <?PHP
                        $sql = "
                            SELECT  tpm_id AS codigo,
                                    tpm_descricao AS descricao
                            FROM livro.tipoparecermidia
                            ORDER BY 2
                        ";
                        $db->monta_combo('tpm_id', $sql, 'S', 'Selecione...', '', '', '', '310', 'S', 'tpm_id', false, $tpm_id, null);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" value="Salvar" id="btSalvar" onclick="salvarParecerMidia();">
                    <input type="button" value="Fechar" onclick="self.close();">
                </td>
            </tr>
        </table>
    </form>
