<?php
//
// $Id$
//

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/entidades.php";

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="../includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <style type="text/css">
      .entcodent_container_radios,
      .tr_entnumcpfcnpj_container
      {
        display: none;
      }
    </style>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>
<?php
if (!$_REQUEST['entid']) {
    $ent = new Entidade();
} else {
    $ent = new Entidade($_REQUEST['entid']);
}

echo formEntidade($ent, '/cte/cte.php?modulo=principal/cadastrarescola&acao=A', PESSOA_JURIDICA, true, true, false, false);


?>
    </div>
  </body>
</html>
