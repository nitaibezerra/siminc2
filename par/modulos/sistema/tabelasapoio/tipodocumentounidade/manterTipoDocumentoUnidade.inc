<?php
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

monta_titulo( 'Manter Tipo Documento Unidade', '&nbsp' );
$arrTipoDocumentoUnidade = array();
$sqlResultado = listAll(NULL);
#Controladora de acao
switch ($_REQUEST['action']) {
    case 'create':
        create($_REQUEST);
        break;
    case 'edit':
        if (!empty($_REQUEST['tdudescricao'])) {
            edit($_REQUEST, $_REQUEST['tduid']);
        } else {
            $arrTipoDocumentoUnidade = listById($_REQUEST['tduid']);
        }
        break;
    case 'delete':
        delete($_REQUEST['tduid']);
        break;
    case 'filtrar' :
        $sqlResultado = listAll($_REQUEST);
}


function listById($id){
    global $db;

    $sql = "SELECT
                tduid,
                tdudescricao,
                tdustatus
            FROM par.tipodocumentounidade
            WHERE tduid = {$id}
            AND tdustatus = 'A'";
    return $db->pegaLinha($sql);
}

function listAll($arrPost, $booSQL = TRUE){
    global $db;
    $strBotao = "'<div style=\"white-space: nowrap\">
                    <img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"link\" onclick=\"edit(\''||tduid||'\')\">
                    <img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"link\" onclick=\"apagar(\''||tduid||'\')\">
                </div>'";
    $sql = "SELECT
                $strBotao,
                tdudescricao,
                tdustatus
            FROM par.tipodocumentounidade
            WHERE tdustatus = 'A'";
    if (!empty($arrPost['tdudescricao_busca'])) {
        $sql .= " AND tdudescricao ILIKE '%{$arrPost['tdudescricao_busca']}%'";
    }
    $sql .= "ORDER BY tdudescricao";
    return ($booSQL) ? $sql : $db->carregar($sql);
}

function edit($arrPost, $id){
    global $db;
    $sql = "UPDATE
                par.tipodocumentounidade
            SET tdudescricao = '{$arrPost['tdudescricao']}'
            WHERE tduid = {$id}";
    if ($db->executar($sql)) {
        echo "<script>
                    alert('Registro alterado com sucesso.');
                    var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
                    window.location = urlBase;
                </script>";
    }
}

function create($arrPost) {
    global $db;
    $sql = "INSERT
              INTO par.tipodocumentounidade (tdudescricao, tdustatus)
            VALUES ('{$arrPost['tdudescricao']}', 'A')";
    if ($db->executar($sql)) {
        echo "<script>
                    alert('Registro inserido com sucesso.');
                    var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
                    window.location = urlBase;
                </script>";
    } else{
        echo "<script>
                    alert('Falha ao Inserir Registro.');
                    var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
                    window.location = urlBase;
                </script>";
    }
}

function delete($id) {
    global $db;
    $sql = "SELECT COALESCE(COUNT(tduid), 0) FROM par.documentounidade WHERE tduid = {$id}";
    $resultado = $db->pegaUm($sql);

    if ($resultado == 0) {
        $sql = "UPDATE
                  par.tipodocumentounidade
                SET tdustatus = 'I'
                WHERE tduid = {$id}";
        if ($db->executar($sql)) {
            echo "<script>
                    alert('Deletado com Sucesso.');
                    var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
                    window.location = urlBase;
                </script>";
        } else{
            echo "<script>
                    alert('Falha ao Deletar.');
                    var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
                    window.location = urlBase;
                </script>";
        }
    } else {
        echo "<script>
                alert('Não é possível excluir o registro, pois encontra-se com vínculo em outra tabela.');
                var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
                window.location = urlBase;
            </script>";
    }
}

?>
<script type="text/javascript">
    var urlBase = '/par/par.php?modulo=sistema/tabelasapoio/tipodocumentounidade/manterTipoDocumentoUnidade&acao=A';
    function edit(id) {
        window.location = urlBase + '&action=edit&tduid='+id;
    }

    function apagar(id) {
        if(confirm('Deseja Excluir?')) {
            window.location = urlBase + '&action=delete&tduid='+id;
        }
    }

    function manterTipoDocumentoUnidade(){
        var tdudescricao = document.getElementsByName('tdudescricao');
        if (tdudescricao[0].value == '') {
            alert('O campo Tipo Documento Unidade é Obrigatório.');
            return false;
        } else {
            document.getElementById("formulario").submit();
        }
    }
</script>

<form name="formulario_tabela_apoio" id="formulario" method="post" action="">
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tbody>
        <tr>
            <td width="25%" class="SubtituloDireita">
                <span>Tipo Documento Unidade</span>
            </td>
            <td>
                <div class="notprint">
                    <input type="hidden" name="tduid" value="<?php echo $arrTipoDocumentoUnidade['tduid'] ?>"/>
                    <input type="hidden" name="action" value="<?php echo ($_REQUEST['action'] == 'edit')? 'edit' : 'create' ?>"/>
                    <?php echo campo_texto('tdudescricao', 'N', 'S', 'Preid', 60, 200, '', '', '', '', '', '', '', $arrTipoDocumentoUnidade['tdudescricao']); ?>
                </div>
            </td>
        </tr>
        <tr>
            <td class="SubtituloTabela">&nbsp;</td>
            <td class="SubtituloTabela">
                <input type="button" name="btn_manter" onclick="manterTipoDocumentoUnidade()" id="btn_manter" value="<?php echo ($_REQUEST['action'] == 'edit')? 'Editar' : 'Salvar' ?>" />
            </td>
        </tr>
        </tbody>
    </table>
</form>
<form name="formulario_tabela_apoio" id="formulario_tabela_apoio" method="post" action="">
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tbody>
        <tr>
            <td width="25%" class="SubTituloDireita">Busca:</td>
            <td>
                <input type="hidden" name="action" value="filtrar"/>
                <?php echo campo_texto('tdudescricao_busca', 'N', 'S', 'Preid', 60, 200, '', '', '', '', '', '', '', $_REQUEST['tdudescricao_busca']); ?>
                <input type="submit" name="btn_buscar" id="btn_buscar" value="OK">
            </td>
        </tr>
        </tbody>
    </table>
</form>
    <?php
    global $db;
    $arrCabecalho = array("Ação", "Descrição", "Status");
    $db->monta_lista($sqlResultado, $arrCabecalho, 20, 10, 'N', '', '' );
    //ver($_SERVER, d);
    ?>