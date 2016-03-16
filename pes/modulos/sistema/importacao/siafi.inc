<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>

<?php
include_once( APPRAIZ . "pes/classes/PesCelulaAcompanhamento.class.inc" );
include_once( APPRAIZ . "pes/classes/PesCelulaAcompNatDespesa.class.inc" );
include_once( APPRAIZ . "pes/modulos/principal/acompanhamento/_funcoes_despesas.php" );

$entcodigo = $_SESSION['pes']['codigo_entidade'];
$unicod = $_SESSION['pes']['codigo_unidadeorcamentaria'];

extract($_REQUEST);

$concodigo = $concodigo ? $concodigo : 0;
$tidcodigo = $tidcodigo ? $tidcodigo : 0;
$aexano = $aexano ? $aexano : date('Y');
$ano_import = $aexano;

include APPRAIZ . 'includes/cabecalho.inc';

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
print '<br/>';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');


$controllerGeral = new Controller_Geral();
if ($controllerGeral->permission() < 3) {
    $save = 'S';
} else {
    $save = 'N';
}

$sql_combo_ano = " select aexano as codigo, aexano as descricao from pes.pesanoexercicio order by descricao";

$validador_insert = false;

if ( $_FILES['arquivo'] != NULL) {

    $arquivo = fopen($_FILES['arquivo']["tmp_name"], "r");
    $row = 0;
    $sql = '';

    while (($data = fgetcsv($arquivo, 500, ";")) !== FALSE) {
        if (count($data) == 4) {

            $valor = trim($data[3]);
            $valor = str_replace(array('.', ','), array('', '.'), $valor);

            $sql .= " INSERT INTO pes.pesacompconsolidado (uorcodigo, accmes, tidcodigo, accvalor, accano) "
                 .  " VALUES ('" . $data[0] . "'," . $data[1] . ", " . $data[2] . ", '" . $valor . "', '" . $_POST['ano_import'] . "'); ";
            $validador_insert = true;
        } else {
            echo '<script>'
            . 'alert("Formato inválido! \nO arquivo .csv deve ter 4 colunas, sendo elas: \nUnidade Orçamentária, Mês da Liquidação, Código do Tipo de Despesa, Valor.")'
            . '</script>';
            $validador_insert = false;
            $sql = '';
            break;
        }
        $row++;
    }
    if ($validador_insert) {
        $sql_del = 'DELETE FROM pes.pesacompconsolidado WHERE accano = ' . $_POST['ano_import'] . ';';
        try {
            $db->executar($sql_del);
            $db->executar($sql);
            $db->commit();
            $str_ok = "Arquivo importado com sucesso! Foram cadastradas " . $row . " linhas.";
            echo '<script type="text/javascript">
                    alert("' . $str_ok . '");
                  </script>';
        } catch (Exception $ex) {
            $db->rollback();
            echo '<script type="text/javascript">'
            . 'alert("Erro na importação.")'
            . '</script>';
        }
    }
} else {
    if ($_FILES['arquivo'] != NULL) {
        echo '<script type="text/javascript">'
        . 'alert("Formato inválido! \nA extensão do arquivo deve ser .csv com os valores separados por ponto-e-vírgula(;).")'
        . '</script>';
    }
}
?>
<form action="" method="post" name="formulario_import" id="formulario_import" enctype="multipart/form-data">
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                A planilha para importação deverá seguir o seguinte formato e salva com a extensão .csv (valores separados por ponto-e-vírgula ';' ).
            </td>
            <td>
                <table style="border: none; width: 50%">
                    <tr>
                        <td style="border: 1px #000 solid;">Unidade Orçamentária</td>
                        <td style="border: 1px #000 solid;">Mês da Liquidação</td>
                        <td style="border: 1px #000 solid;">Código do Tipo de Despesa</td>
                        <td style="border: 1px #000 solid;">Valor</td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr class="exibir-info">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Selecione o ano:</td>
            <td>
                <?php
                    echo $db->monta_combo("ano_import", $sql_combo_ano, 'S', "", "", "", "", "100", "S", "", "", $ano_import);
                ?>
            </td>
        </tr>
        
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;"><label for="arquivo">Arquivo:</label></td>
            <td> <input type="file" name="arquivo" id="arquivo"> </td>
        </tr>
        
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="botao_import" id="botao_import" value="Importar" />
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
    jQuery(function() {

        jQuery('#botao_import').click(function() {
            //jQuery('#formulario_gravar').submit();

            var ano_import = jQuery('[name="ano_import"] option:selected').val();

            var resp = confirm("Atenção! Esta carga irá substituir todos os dados do ano de " + ano_import + "! Deseja continuar?")
            if (resp == true) {
                jQuery('#formulario_import').submit();
            } else {
                alert('Importação cancelada!');
            }
        });
    });
</script>