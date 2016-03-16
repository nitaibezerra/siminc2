<?php
//
// $Id$
//



cte_verificaSessao();
$dadosSalvos                         = false;
$_SESSION['_parSubacaoAnoExercicio'] = $_REQUEST['sabano'];





if (array_key_exists('entid', $_REQUEST) && is_array($_REQUEST['entid'])) {
    foreach ($_REQUEST['entid'] as $entid) {
        $sql = 'INSERT INTO cte.escolacomposicaosubacao (
                    cosid,
                    entid,
                    ecsqtd
                ) VALUES (%d, %d, %d, 0)';

        $db->executar(sprintf($sql, (integer) $_REQUEST['cosid'],
                                    (integer) $_REQUEST['entid'],
                                    (integer) $entid));
    }

    $db->commit();
} elseif (array_key_exists('ecsqtd', $_REQUEST) && is_array($_REQUEST['ecsqtd'])) {
    foreach ($_REQUEST['ecsqtd'] as $qfaid => $qfaqtd) {
        $sql = 'UPDATE cte.escolacomposicaosubacao SET
                    ecsqtd = %d
                WHERE
                    entid  = %d
                    AND
                    cosid  = %d';

        $db->executar(sprintf($sql, (integer) $qfaqtd,
                                    (integer) $qfaid));
    }

    $db->commit();
}





$sql = '
SELECT DISTINCT
    ent.entcodent,
    ent.entnome,
    \'<input class="normal" size="10" type="text" name="qfaqtd[\' || qfa.qfaid || \']" value="\' || coalesce(ecs.qfaqtd, 0) || \'" />\' as ecsqtd
FROM
    cte.instrumentounidadeescola iue
INNER JOIN
    entidade.entidade ent on ent.entid = iue.entid
RIGHT OUTER JOIN
    cte.qtdfisicoano qfa ON iue.entid = qfa.entid
/*
RIGHT JOIN
    cte.escolacomposicaosubacao ecs on
 */
WHERE
    qfa.qfaano = ' . $_REQUEST['qfaano'] . '
    AND
    qfa.sbaid  = ' . $_REQUEST['sbaid']  . '
ORDER BY
    ent.entnome ASC';





?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
    </style>
  </head>
  <body>
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmEscolas">
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center"><?php echo $tituloCabecalho ?></div>
          <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
<?php

$db->monta_lista_simples($sql, array('Código INEP', 'Escola', 'Quantidade'), 1000, 1000, 'N', '100%');

?>
          </div>
          <br />
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <input type="hidden" name="qfaano" value="<?php echo $_REQUEST['qfaano']; ?>" />
          <input type="hidden" name="sbaid" value="<?php  echo $_REQUEST['sbaid'];  ?>" />
          <input type="hidden" name="cosid" value="<?php  echo $_REQUEST['cosid'];  ?>" />
          <input type="submit" name="btnGravar" value="Gravar" />
          <input type="button" name="btnFechar" value="Fechar" onclick="window.close();" />
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  <!--
    $('loader-container').hide();
  -->
  </script>
</html>
