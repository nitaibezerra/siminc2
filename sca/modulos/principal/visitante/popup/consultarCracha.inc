<?php
$titulo = "Relação de Visitantes";
monta_titulo( $titulo, '&nbsp;' );

include APPRAIZ."sca/classes/Visita.class.inc";
$oVisita = new Visita();

?>

<html>
<head>
    <script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
    <script language='javascript' type='text/javascript' src='./js/sca.js'></script>
    <script language="javascript" type="text/javascript">

    /**
     * Recupera opcao selecionada
     * @name selecionar
     * @return void
     */
    function selecionarPessoa(vttid, nu_matricula_siape){
        executarScriptPai("carregarPessoa(" + vttid + "," + nu_matricula_siape + ",null);");
        self.close();
    }

    </script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
</head>
<body>
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td colspan='2'>
                <div style='height:120px;overflow-y:scroll;'>
                <?php $oVisita->montaListaSaidaPorDocumentoCracha($_GET['documento']); ?>
                </div>
            </td>
        </tr>
    </table>
</body>
</html>