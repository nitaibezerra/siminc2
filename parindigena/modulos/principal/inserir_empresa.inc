<?php
//
// $Id$
// empresa

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/entidades.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Funcao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoClassificacao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoLocalizacao.php";

if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'])) {
    global $db;

    // Busca CNPJ cadastrado
    if ($_REQUEST['opt'] == 'buscarCnpj') {
        $entidade = Entidade::carregarEntidadePorCnpjCpf($_REQUEST['entnumcpfcnpj'], $db->testa_superuser());

        if ($entidade->getPrimaryKey() !== null) {
            die($entidade->getPrimaryKey());
        } else {
            die('0');
        }
    } elseif ($_REQUEST['opt'] == 'buscarEscola') {
        $entidade = Entidade::carregarEntidadePorEntcodent($_REQUEST['entcodent'], $db->testa_superuser());

        if ($entidade->getPrimaryKey() !== null) {
            die($entidade->getPrimaryKey());
        } else {
            die('0');
        }
    } elseif ($_REQUEST['opt'] == 'salvarRegistro') {
        $entidade = new Entidade();

        if ($_REQUEST['entid'] != '')
    		$entidade->carregar($_REQUEST['entid']);

    	$entidade->BeginTransaction();

    	$entidade->entidassociado       = null;
        $entidade->entnumcpfcnpj        = str_replace(array(".", "-", "/"), "", $_REQUEST['entnumcpfcnpj']);
        $entidade->entnome              = $_REQUEST['entnome'];
        $entidade->entemail             = $_REQUEST['entemail'];
        $entidade->entnuninsest         = $_REQUEST['entnuninsest'];
        $entidade->entobs               = $_REQUEST['entobs'];
        $entidade->entstatus            = $_REQUEST['entstatus'] != '' ? $_REQUEST['entstatus'] : 'A';
        $entidade->entnumdddresidencial = $_REQUEST['entnumdddresidencial'];
        $entidade->entnumresidencial    = $_REQUEST['entnumresidencial'];
        $entidade->entnumdddcomercial   = $_REQUEST['entnumdddcomercial'];
        $entidade->entnumramalcomercial = $_REQUEST['entnumramalcomercial'];
        $entidade->entnumcomercial      = $_REQUEST['entnumcomercial'];
        $entidade->entnumdddfax         = $_REQUEST['entnumdddfax'];
        $entidade->entnumramalfax       = $_REQUEST['entnumramalfax'];
        $entidade->entnumfax            = $_REQUEST['entnumfax'];

        $entidade->njuid                = $_REQUEST['njuid'] != '' ? $_REQUEST['njuid'] : null;
        $entidade->funid                = $_REQUEST['funid'] != '' ? $_REQUEST['funid'] : null;
        $entidade->tpcid                = $_REQUEST['tpcid'] != '' ? $_REQUEST['tpcid'] : null;
        $entidade->tplid                = $_REQUEST['tplid'] != '' ? $_REQUEST['tplid'] : null;
        $entidade->tpsid                = $_REQUEST['tpsid'] != '' ? $_REQUEST['tpsid'] : null;

    	$entidade->save();
        $entidade->Commit();

        $endereco = new Endereco();
        $endereco->BeginTransaction();

        if ($_REQUEST['endid'] != '')
            $endereco->carregar($_REQUEST['endid']);

        $endereco->endcep = str_replace(array('.', '-'), '', $_REQUEST['endereco']['endcep']);
        $endereco->endnum = $_REQUEST['endereco']['endnum'];
        $endereco->endcom = $_REQUEST['endereco']['endcom'];
        $endereco->tpeid  = 1;
        $endereco->endlog = $_REQUEST['endereco']['endlog'];
        $endereco->endbai = $_REQUEST['endereco']['endbai'];
        $endereco->muncod = $_REQUEST['endereco']['muncod'];
        $endereco->estuf  = $_REQUEST['endereco']['estuf'];
        $endereco->entid  = null;//$entidade->getPrimaryKey();

        $endereco->save();
        $endereco->Commit();

        EntidadeEndereco::adicionar($entidade, $endereco);

        echo '
		        <script type="text/javascript">
			        window.opener.document.getElementById("entnomeempresa").innerHTML = \'' . $entidade->entnome . '\';
			        window.opener.document.getElementById("entidempresa").value       = \'' . $entidade->getPrimaryKey() . '\';
			        window.close();
		        </script>
			';
    }
}


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
    <script type="text/javascript" src="/includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>
<?php

if (!$_REQUEST['entid'] || $_REQUEST['entid'] == '') {
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
	echo formEntidade($ent, 'parindigena.php?modulo=principal/inserir_empresa&acao=A&opt=salvarRegistro', PESSOA_JURIDICA, true, false);
?>
    </div>

    <script type="text/javascript">
        $('frmEntidade').onsubmit  = function(e)
        {
            $('muncod').removeAttribute('disabled');
            $('endbai').removeAttribute('disabled');

        	return true;
        }
<?php

?>
/*
		if ($('entcodent')) {
	        $('entcodent').onblur  = function (e)
	        {
	            if (this.value == '' || this.value == this.defaultValue)
	                return false;

	            var req = new Ajax.Request('obras.php?modulo=principal/inserir_empresa&acao=A', {
	                                       method: 'post',
	                                       parameters: '&opt=buscarEscola&entcodent=' + this.value,
	                                       onComplete: function (res)
	                                       {
	                                           if (res.responseText != 0) {
	                                               if (confirm('O código informado já se encontra cadastrado.\n' 
	                                                          +'Deseja carregar o registro?'))
	                                               {
	                                                   window.location.href = 'obras.php?modulo=principal/inserir_empresa&acao=A&busca=entcodent&entid=' + res.responseText;
	                                               } else {
	                                                   $('entcodent').value = '';
	                                                   $('entcodent').select();
	                                               }
	                                           }
	                                       }
	            });
	        }
        }
        //                                                                  */

		/*
		if ($('tpcid') && $('tpctgid_container')) {
	        $('tpcid').onchange = function(e)
	        {
	            if (this.value == 4) {
	                Element.show('tpctgid_container');
	            } else {
	                Element.hide('tpctgid_container');
	            }
	        }
        }
        */
<?php

?>

        $('entnumcpfcnpj').onblur  = function (e)
        {
            if (this.value == '' || trim(this.value) == trim(this.defaultValue) || (this.value.length != 18 && this.value.length != 14))
                return false;

            var req = new Ajax.Request('parindigena.php?modulo=principal/inserir_empresa&acao=A', {
                                       method: 'post',
                                       parameters: '&opt=buscarCnpj&entnumcpfcnpj=' + this.value,
                                       onComplete: function (res)
                                       {
                                           if (res.responseText != 0) {
                                               if (confirm('O CNPJ informado já se encontra cadastrado.\n' 
                                                          +'Deseja carregar o registro?'))
                                               {
                                                   window.location.href = 'parindigena.php?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&entid=' + res.responseText;
                                               } else {
                                                   $('entnumcpfcnpj').value = '';
                                                   $('entnumcpfcnpj').activate();
                                               }
                                           }
                                       }
            });
        } // !$('entnumcpfcnpj').onblur

        //$('entnumcpfcnpj').activate();

    </script>
  </body>
</html>
