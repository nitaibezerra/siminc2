<?php
/**
            [name] => ppp.sql
            [type] => text/x-sql
            [tmp_name] => /tmp/phpRJOcW6
            [error] => 0
            [size] => 13963
 */

cte_verificaSessao();

require_once APPRAIZ . 'includes/ActiveRecord/classes/parecerpar.php';
require_once APPRAIZ . 'includes/ActiveRecord/classes/parecerinstrumento.php';

define('FILEPATH', APPRAIZ . 'arquivos/anexos/parecerpar-');

if ($_REQUEST['acao']     == 'A')
    $tppid = 1;
elseif ($_REQUEST['acao'] == 'B')
    $tppid = 2;
elseif ($_REQUEST['acao'] == 'C')
    $tppid = 3;

$parecer = ParecerPar::carregarParecerParPorInuid($_SESSION['inuid'], $tppid);

if ($_REQUEST['excluirParecer']) {
    try {
        $parecer->BeginTransaction();
        ParecerInstrumento::remover($parecer, $_SESSION['inuid']);
        $parecer->excluir();
        $parecer->Commit();
         echo "<script type=\"text/javascript\">"
            ."alert('Parecer excluido com sucesso.');"
            ."window.location.href='cte.php?modulo=principal/estrutura_avaliacao&acao=A';"
            ."</script>\n";
        //header('Location: http://' .$_SERVER['SERVER_NAME']. '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A');
        
    } catch (Exception $e) {
        $parecer->Rollback();
        header('Location: ' . $url);
    }

    exit;
}

if ($_REQUEST['partexto'] != '') {
    $parecer->partexto = $_REQUEST['partexto'];
    $parecer->pardata  = date('Y-m-d H:i:s');
    $parecer->usucpf   = $_SESSION['usucpf'];

    try {
        $parecer->BeginTransaction();
        $parecer->tppid = $tppid;
        $incluir        = $parecer->getPrimaryKey() === null;
        $parecer->save();

        if ($incluir)
            ParecerInstrumento::adicionar($parecer, $_SESSION['inuid']);

        $parecer->Commit();
        echo "<script type=\"text/javascript\">"
            ."alert('Parecer salvo com sucesso.');"
            ."window.location.href='cte.php?modulo=principal/estrutura_avaliacao&acao=A';"
            ."</script>\n";

    } catch (Exception $e) {
        $parecer->Rollback();

        die($e->getMessage());
    }
}

//$itrid = cte_pegarItrid( $_SESSION['inuid'] );
//$itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL

global $partexto,
       $arqdescricao;

$partexto = $parecer->partexto;
$input_partexto     = campo_textarea('partexto', 'S', 'S', 'Parecer', '100', '10', null);


include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

$db->cria_aba($abacod_tela, $url, '');
cte_montaTitulo($saida['mnudsc'], $titulo_modulo);


?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="POST" name="formulario" onsubmit="return validarForm();" enctype="multipart/form-data">
    <input type="hidden" name="parid" value="<?php echo $parecer->getPrimaryKey(); ?>" />
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="25%" align='right' class="SubTituloDireita">Tipo de parecer:</td>
          <td><?php echo $titulo_modulo;?></td>
        </tr>

        <tr>
            <td align='right' class="SubTituloDireita">Parecer:</td>
          <td><?php echo $input_partexto;?></td>
        </tr>
        <tr bgcolor="#C0C0C0">
            <td colspan="2" align="center">
            <input type="submit" class="botao" name="submit" value="Gravar" />
<?php
    if ($parecer->getPrimaryKey() !== null)
        echo '<input type="button" class="botao" name="excluir" value="Excluir" onclick="return excluirParecer(\'' , $_REQUEST['acao'] , '\');" />';
?>

            </td>
        </tr>
    </table>
</form>

<script type="text/javascript">
    function excluirParecer(tppid)
    {
        window.location.href = 'cte.php?modulo=principal/cadastrarparecer&acao=' + tppid + '&excluirParecer=true';
    }

    function validarForm()
    {
        var partexto = document.getElementById('partexto');

        if (partexto.value == '') {
            alert('O campo Parecer é obrigatório.');
            return false;
        }

        return true;
    }
</script>

