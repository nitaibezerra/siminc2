<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>

<?php

include_once( APPRAIZ. "pes/modulos/principal/acompanhamento/_funcoes_despesas.php" );

$entcodigo = $_SESSION['pes']['codigo_entidade'];
$unicod    = $_SESSION['pes']['codigo_unidadeorcamentaria'];

include APPRAIZ . 'includes/cabecalho.inc';

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

$controllerGeral = new Controller_Geral();

if($controllerGeral->permission() < 3)
    $save = 'S';
else
    $save = 'N';

if(isset($_POST['tidcodigo']) && !empty($_POST['tidcodigo'])){
    $tidcodigo = $_POST['tidcodigo'];
} else $tidcodigo = null;
?>
<form method="post" name="form" id="form">
    <table class="tabela" bgcolor="#f5f5f5" align="center">

            <tr bgcolor="#cccccc">
            <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Tipo de despesa: </label></td>
                <td>
                    <?php 
                        $sql = "select tidnome as descricao, tidcodigo as codigo
                                                       from  pes.pestipodespesa
                                                       where tidcodigo not in (" . K_DESPESA_DIARIAS . ", " . K_DESPESA_PASSAGENS . ", " . K_DESPESA_COLETA_SELETIVA . ", " . K_DESPESA_GENERICA . ")
                                   order by tidordem desc, descricao";
                        $db->monta_combo("tidcodigo", $sql, 'S', "Selecione", '', '', '', '', 'N', 'tidcodigo', '', $tidcodigo, '', ''); 
                    ?>
                </td>
            </tr>
    <!--        <tr>
                <td colspan="2">-->

    <!--            </td>
            </tr>-->
    </table>
</form>
<script>
$('#tidcodigo').change(function() {
    $('#form').submit();
});
</script>

<?php

if ($tidcodigo) {
    montaTabelaConsolidadoUO($unicod, $entcodigo , $tidcodigo);
    $ano = AEXANO;
    $anoAnterior = ($ano - 1);
}

montaTabelaConsolidadoUO($unicod, $entcodigo );
