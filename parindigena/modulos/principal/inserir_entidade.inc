<?php

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "includes/entidades.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Funcao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoClassificacao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoLocalizacao.php";

if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'] || $_REQUEST['entunicod'])) {
    global $db;

    // Busca CNPJ cadastrado
    if ($_REQUEST['opt'] == 'buscarCnpj') {
        $entidade = Entidade::carregarEntidadePorCnpjCpf($_REQUEST['entnumcpfcnpj'], $db->testa_superuser());

        if ($entidade->getPrimaryKey() !== null) {
            die($entidade->getPrimaryKey());
        } else {
            die('0');
        }
    } elseif ($_REQUEST['opt'] == 'buscarEntunicod') {
        $entidade = Entidade::carregarEntidadePorEntunicod($_REQUEST['entunicod'], $db->testa_superuser());

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
        //$entidade->entnumcpfcnpj        = str_replace(array(".", "-", "/"), "", $_REQUEST['entnumcpfcnpj']);
        //$entidade->entnome              = $_REQUEST['entnome'];
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

        // Seleciona o funid da entidade selecionada
        $funid = $db->pegaUm("
			        	SELECT 
							funid
						FROM
							entidade.entidade 
						WHERE
							entid = {$entidade->getPrimaryKey()}");
        
        // Valida se a entidade pode ser relacionada ao órgão escolhido
        if(($funid != ID_UNIVERSIDADE) && ($_REQUEST["orgid"] == ORGAO_SESU)){
        	
        	echo '
	        	<script type="text/javascript">
        			alert("A entidade '.$entidade->entnome.' não é uma universidade!");
        			window.opener.document.getElementById("entnome").innerHTML = \'\';
	        	</script>';
        	
        }else if(($funid != ID_ESCOLAS_TECNICAS) && ($_REQUEST["orgid"] == ORGAO_SETEC)){
        	
        	echo '
	        	<script type="text/javascript">
        			alert("A entidade '.$entidade->entnome.' não é uma escola técnica!");
        			window.opener.document.getElementById("entnome").innerHTML = \'\';
	        	</script>';
        }else{
        
        	// Cria os campos da entidade e do Campu da entidade na tela
	        $campus = pg_query("
								SELECT 
									entid as codigo,
									entnome as descricao 
								FROM 
									entidade.entidade
								WHERE
									entidassociado = {$entidade->getPrimaryKey()} AND
									funid = 18");
			
	        $combo = '<select class="CampoEstilo" id="entidcampus" name="entidcampus">';
	        
	        while ($tipos = pg_fetch_array($campus)){
	        	$codigo = $tipos['codigo'];
	        	$descricao = $tipos['descricao'];
	           	$combo .= '<option value="' . $codigo . '">' . $descricao . '</option>'; 
	        }
	        
	        $combo .= '</select>';
	        
			echo '
		        <script type="text/javascript">
			        window.opener.document.getElementById("entnome").innerHTML = \'' . $entidade->entnome . '\';
			        window.opener.document.getElementById("entid").value       = \'' . $entidade->getPrimaryKey() . '\';
			        window.close();
				</script>';
		}
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
    $ent      = new Entidade();
    $end      = new Endereco();
    $editavel = false;
} else {
    $ent      = new Entidade($_REQUEST['entid']);
    $end      = $ent->carregarEnderecos();
    $editavel = true;

    if ($end[0] instanceof Endereco)
        $end = $end[0];
    else
        $end = new Endereco();
}

if ($_REQUEST['orgid'] == ORGAO_SESU) {      // SESU
	echo formEntidade($ent, 'parindigena.php?modulo=principal/inserir_entidade&acao=A&opt=salvarRegistro&orgid=' . ORGAO_SESU, PESSOA_JURIDICA, true, true, false, $editavel, array(), true);
    $campos = 'Form.disable("frmEntidade");'
            . '$("entunicod").removeAttribute("disabled");'
            . '$("entunicod").select();';
} elseif ($_REQUEST['orgid'] == ORGAO_SETEC) {// SETEC
	echo formEntidade($ent, 'parindigena.php?modulo=principal/inserir_entidade&acao=A&opt=salvarRegistro&orgid=' . ORGAO_SETEC, PESSOA_JURIDICA, true, true, false, $editavel);
    $campos = 'Form.disable("frmEntidade");'
            . '$("entcodent").removeAttribute("disabled");'
            . '$("entnumcpfcnpj").removeAttribute("disabled");'
            . '$("entcodent_0").removeAttribute("disabled");'
            . '$("entcodent_1").removeAttribute("disabled");';
} else {                            // FNDE
	echo formEntidade($ent, 'parindigena.php?modulo=principal/inserir_entidade&acao=A&opt=salvarRegistro&orgid=' . ORGAO_FNDE, PESSOA_JURIDICA, true, true, false, $editavel, array(1  => 'Prefeitura',
                                                                                                                                                          3  => 'Escola',
                                                                                                                                                          12 => 'Universidade'));
    $campos = 'Form.disable("frmEntidade");'
            . '$("entcodent").removeAttribute("disabled");'
            . '$("entnumcpfcnpj").removeAttribute("disabled");'
            . '$("entcodent_0").removeAttribute("disabled");'
            . '$("entcodent_1").removeAttribute("disabled");';
}
?>
    </div>

    <script type="text/javascript">
        $('frmEntidade').onsubmit  = function(e)
        {
            if (trim($F('entnome')) == '') {
                alert('O nome da entidade é obrigatório.');
                return false;
            }

            if (($('entcodent')     && trim($F('entcodent')) == '') &&
                ($('entnumcpfcnpj') && trim($F('entnumcpfcnpj')) == ''))
            {
                alert('Você deve preencher pelo menos um dos campos abaixo:\nCódigo INEP OU CPNJ.');
                return false;
            }

            if ($('entunicod') && $F('entunicod') == '') {
                alert('O código da universidade é obrigatório.');
                return false;
            }

            Form.enable('frmEntidade');

        	return true;
        }
<?php

?>

		if ($('entcodent')) {
	        $('entcodent').onblur  = function (e)
	        {
	            if (this.value == '' || trim(this.value) == (this.defaultValue))
	                return false;

	            var req = new Ajax.Request('parindigena.php?modulo=principal/inserir_entidade&acao=A', {
	                                       method: 'post',
	                                       parameters: '&opt=buscarEscola&entcodent=' + this.value,
	                                       onComplete: function (res)
	                                       {
	                                           if (res.responseText != 0) {
	                                               if (confirm('O código informado já se encontra cadastrado.\n' 
	                                                          +'Deseja carregar o registro?'))
	                                               {
	                                                   window.location.href = 'parindigena.php?modulo=principal/inserir_entidade&acao=A&busca=entcodent&orgid=<?php echo $_REQUEST['orgid']; ?>&entid=' + res.responseText;
	                                               } else {
	                                                   $('entcodent').value = '';
	                                                   $('entcodent').select();
	                                               }
	                                           } else {
                                                   alert('Não há unidade cadastrada com o código informado.\nPor favor entrar em contato com o gestor do sistema.');
                                               }
	                                       }
	            });
	        }
        }
        //                                                                  */

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
<?php

?>

        if ($('entnumcpfcnpj')) {
            $('entnumcpfcnpj').onblur  = function (e)
            {
                if (this.value == '' || this.value.replace(/[^0-9]/gi, '') == this.defaultValue)
                    return false;

                var req = new Ajax.Request('parindigena.php?modulo=principal/inserir_entidade&acao=A', {
                                           method: 'post',
                                           parameters: '&opt=buscarCnpj&entnumcpfcnpj=' + this.value,
                                           onComplete: function (res)
                                           {
                                               if (res.responseText != 0) {
                                                   if (confirm('O CNPJ informado já se encontra cadastrado.\n' 
                                                              +'Deseja carregar o registro?'))
                                                   {
                                                       window.location.href = 'parindigena.php?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&orgid=<?php echo $_REQUEST['orgid']; ?>&entid=' + res.responseText;
                                                   } else {
                                                       $('entnumcpfcnpj').value = '';
                                                       $('entnumcpfcnpj').activate();
                                                   }
                                               } else {
                                                   alert('Não há unidade cadastrada com o CNPJ informado.\nPor favor entrar em contato com o gestor do sistema.');
                                               }
                                           }
                });
            } // !$('entnumcpfcnpj').onblur

            $('entnumcpfcnpj').activate();
        }


        if ($('entunicod')) {
            $('entunicod').onblur  = function (e)
            {
                if (this.value == '' || this.value.replace(/[^0-9]/gi, '') == this.defaultValue)
                    return false;

                var req = new Ajax.Request('parindigena.php?modulo=principal/inserir_entidade&acao=A', {
                                           method: 'post',
                                           parameters: '&opt=buscarEntunicod&entunicod=' + this.value,
                                           onComplete: function (res)
                                           {
                                               if (res.responseText != 0) {
                                                   if (confirm('O CNPJ informado já se encontra cadastrado.\n' 
                                                              +'Deseja carregar o registro?'))
                                                   {
                                                       window.location.href = 'parindigena.php?modulo=principal/inserir_entidade&acao=A&busca=buscarEntunicod&orgid=<?php echo $_REQUEST['orgid']; ?>&entid=' + res.responseText;
                                                   } else {
                                                       $('entunicod').value = '';
                                                       $('entunicod').activate();
                                                   }
                                               } else {
                                                   alert('Não há unidade cadastrada com o código informado.\nPor favor entrar em contato com o gestor do sistema.');
                                               }
                                           }
                });
            } // !$('entnumcpfcnpj').onblur

            $('entunicod').activate();
        }


<?php
        if ($_REQUEST['busca'])
        {
        	if ($_REQUEST['busca'] == 'entnumcpfcnpj' && strlen($ent->entnumcpfcnpj) == 14) {
                echo '        $("entcodent_0").checked=true;';
                echo '$("entcodent_1").checked = false;';
                echo 'Element.show(\'tr_entnumcpfcnpj_container\'); Element.hide(\'tr_entcodent_container\');';
            } elseif ($_REQUEST['busca'] == 'entcodent') {
                echo '        $("entcodent_1").checked=true;';
                echo '$("entcodent_0").checked = false;';
                echo 'Element.hide(\'tr_entnumcpfcnpj_container\'); Element.show(\'tr_entcodent_container\');';
            } else {
                echo 'if($("entunicod"))$("entunicod").setAttribute("readOnly", "readOnly");';
            }
        }

        echo $campos;
?>
        window.onunload = function()
        {
            window.opener.document.getElementById("entnome").innerHTML = $F('entnome');
            window.opener.document.getElementById("entid").value       = $F('entid');
        }

        $('entemail','endlog','endcep','endlog','endbai','endnum','endcom','endlogradouro','closeWindow','submitEntidade','resetEntidade').each(
            function(a)
            {
                if (a)
                    a.removeAttribute('disabled');
            }
        );
    </script>
  </body>
</html>
