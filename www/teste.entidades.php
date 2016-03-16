<?php
//require_once "config.inc";
if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('..') . "/");
}


//include "../includes/classes_simec.inc";
//include "../includes/funcoes.inc";



global $nome_bd;
       $nome_bd     = 'simec_desenvolvimento';

global $servidor_bd;
       $servidor_bd = 'simec-d';

global $porta_bd;
       $porta_bd    = '5432';

global $usuario_db;
       $usuario_db  = 'seguranca';

global $senha_bd;
       $senha_bd    = 'phpseguranca';


//$db = new cls_banco();


require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Funcao.php";

if ($_REQUEST['teste']) {

    $fun = new Funcao();
    echo 'var funcoes  = ' , $fun->toJson();
    exit;

}

//                                                                          */
?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #f5f5f5; background-image: url(../imagens/fundo.gif);">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>
      <div id="formEntidades" style="border: 1px solid #ccc; padding: 10px; margin: 15px; display: block"></div>
    </div>

    <script type="text/javascript">
      <!--
<?php
if (!$_REQUEST['entid']) {
    $ent = new Entidade();
    $end = new Endereco();
} else {
    $ent = new Entidade($_REQUEST['entid']);
    $end = $ent->carregarEnderecos();

    if ($end[0] instanceof Endereco)
        $end = $end[0];
    else
        $end = new Endereco();
}
    $ent->funid = 3;
    $arr = $ent->getArrayCampos();
    $str = array();

    foreach ($arr as $campo) {
        if (preg_match('/\d{4}-\d{2}-\d{2}/', $ent->$campo))
            $valor = date("d/m/Y", strtotime($ent->$campo));
        else
            $valor = $ent->$campo;

        $str[] = $campo . ":'" . addslashes($valor) . "'";
    }

    echo 'var entidade={entid:\'' , $ent->getPrimaryKey() , "'," , implode(",", $str);

    $str = array();
    foreach ($end->getArrayCampos() as $campo) {
        $str[] = $campo . ":'" . addslashes($end->$campo) . "'";
    }

    echo ',enderecos:[{endid:\''    , $end->getPrimaryKey() , "'," , implode(",", $str) , "}]};";
    echo 'var funcoes=[];';

?>
      Entidade.setFormAction('?modulo=principal/cadastrarescola&acao=A&opt=salvarRegistro');
      Entidade.buildBlockForm('formEntidades', PESSOA_JURIDICA, entidade, false, true);

      $('entnumcpfcnpj').activate();

      $('frmEntidade').onsubmit  = function(e)
      {
          if (Entidade.validateForm(this, ['entnome', 'entnumcpfcnpj', 'entnumdddcomercial', 'entnumcomercial'])) {
              $('frmEntidade').submit();
          } else {
              return false;
          }
      };

      $('resetEntidade').onclick = function (e)
      {
          return window.close();
      };

      $('entnumcpfcnpj').onblur  = function (e)
      {
          if ($('entid').value != '')
              return false;

          if (this.value == '' || (this.value.length != 18 && this.value.length != 14)) {
              return false;
          } else {
              var req = new Ajax.Request('brasilpro.php?modulo=principal/cadastrarescola&acao=A', {
                                         method: 'post',
                                         parameters: '&opt=buscarCnpj&entnumcpfcnpj=' + this.value,
                                         onComplete: function (res)
                                         {
                                             if (res.responseText != 0) {
                                                 if (confirm('O CNPJ informado já se encontra cadastrado.\n'
                                                            +'Deseja carregar o registro?'))
                                                 {
                                                     window.location.href = 'brasilpro.php?modulo=principal/cadastrarescola&acao=A&entid=' + res.responseText;
                                                 } else {
                                                     this.value = '';
                                                     this.select();
                                                 }
                                             }
                                         }
              });
          }
      }

      $('endcep').onblur         = function (e)
      {
          getEnderecoPeloCEP(this.value);
      }

        -->
    </script>
  </body>
</html>





