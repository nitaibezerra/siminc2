<?php
//
// $Id$
//
//Aditivo //
$sbaidPai 	= $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; 
$anoAditivo = $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; 
$aditivo 	= $_REQUEST['ad'] ? $_REQUEST['ad'] : 0;
//Fim Aditivo //



cte_verificaSessao();

if (array_key_exists('excluir', $_REQUEST)) {
	
	$sql = "DELETE FROM cte.subacaobeneficiario 
			WHERE benid = " . (integer) $_REQUEST['excluir'] ."
			AND sabano = ". (integer) $_REQUEST['sabano'] ."
			AND sbaid = ". (integer) $_REQUEST['sbaid'];

    $db->executar( $sql );
    $db->commit();
    die('null');

}

/*
if (array_key_exists('btnGravar', $_REQUEST)) {
*/
if ($_REQUEST['gravarForm'] == "true")
{
    $db->executar('DELETE FROM
                    cte.subacaobeneficiario
                   WHERE
                    sbaid  = ' . (integer) $_REQUEST['sbaid'] . '
                    AND
                    sabano = ' . (integer) $_REQUEST['sabano']);

    $sql = 'INSERT INTO cte.subacaobeneficiario (
                sbaid,
                benid,
                vlrurbano,
                vlrrural,
                sabano
            ) VALUES (%d, %d, %d, %d, %d)';

    $benidInseridos = array();

    if (is_array($_REQUEST['benid']))
        foreach ($_REQUEST['benid'] as $k => $benid) {
            if (!in_array($benid, $benidInseridos)) {
                $db->executar(sprintf($sql, (integer) $_REQUEST['sbaid'],
                                            (integer) $benid,
                                            (integer) $_REQUEST['vlrurbano'][$k],
                                            (integer) $_REQUEST['vlrrural'][$k],
                                            (integer) $_REQUEST['sabano']));

                $benidInseridos[] = $benid;
            }
        }

	$db->commit();
    $dadosSalvos = true;
    
    echo "<script>
    alert('Operação Efetuada com sucesso!');
    window.opener.location.reload();</script>";
    exit();
}



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
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" name="form" onSubmit="return validaFormulario();">
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Beneficiários</div>
          <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
            <table id="beneficiariosSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
              <colgroup>
                <col width="10%" />
                <col width="50%" />
                <col width="20%" />
                <col width="20%" />
              </colgroup>

              <tr style="text-align: center">
                <td style="padding: 2px; font-weight: bold;">&nbsp;</td>
                <td style="padding: 2px; font-weight: bold;">Beneficiário</td>
                <td style="padding: 2px; font-weight: bold;">Zona Urbana</td>
                <td style="padding: 2px; font-weight: bold;">Zona Rural</td>
              </tr>
            </table>
          </div>
          <div style="padding: 5px 0px 0px 5px">
            <span style="cursor:pointer" onclick="return adicionarBeneficiario(0,0,0,<?php echo $_REQUEST['sabano'];?>,true);"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Adicionar Beneficiário</span>
          </div><br />
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <span style="display: block;font-weight:bold;font-size:110%;margin-bottom:5px;color:red">Atenção! Beneficiários duplicados serão descartados</span>
		  <input type="hidden" name="gravarForm" value="true" />
          <!-- <input type="submit" name="btnGravar" value="Gravar" /> -->
          <input type="button" name="btnGravarOnclick" value="Gravar" onclick="validaFormulario()" />
          <!--  <input type="button" name="btnFechar" value="Fechar" onclick="fecharJanela();" /> -->
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  <!--
    this._closeWindows = false;
    var _modificado    = false;
    //this.window.close = true;    
    
    function removerBeneficiario(linha, benid, sabano)
    {
    
        var linha   = $(linha);
        var excluir = true;
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
            $('loader-container').show();
            try {
                if (benid != null) {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: '&excluir=' + benid + '&sabano=' + sabano,
                                                   onComplete: function(r)
                                                   {
                                                       if (r.responseText == 'null') {
                                                           excluir = false;
                                                           alert('Não foi possível excluir o item.');
                                                       }
                                                   }
                                               });
                }

                if (excluir)
                    linha.parentNode.removeChild(linha);

                linha.style.backgroundColor = '#f0f0f0';
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }
    
    function validaFormulario() {
    	if(!document.getElementsByName("vlrurbano[]")) {
    		alert('É obrigatório adicionar beneficiário.');
    		return false;
    	
    	}
    	if(document.form.elements["vlrurbano[]"].value) {
    		if(document.form.elements["vlrurbano[]"].value == '0' && document.form.elements["vlrrural[]"].value == '0') {
    			alert('Os dados da zona rural e/ou zona urbana devem ser preenchidas.');
    			return false;
    		}
    	}
    	if(document.form.elements["vlrurbano[]"].length) {
    		for(i = 0; i < document.form.elements["vlrurbano[]"].length; i++) {
    			if(document.form.elements["vlrurbano[]"][i].value == '0' && document.form.elements["vlrrural[]"][i].value == '0') {
    				alert('Os dados da zona rural e/ou zona urbana devem ser preenchidas.');
    				return false;
    			}
    		}
    	}   	   	
    	
    	document.form.submit();  	
    	return true;
    }

    /**
     * 
     */
    function adicionarBeneficiario(benid, vlrurbano, vlrrural, sabano, modificado)
    {
        var input_benid;
        var input_vlrurbano;
        var input_vlrrural;

        var btnExcluirItemComposicao     = document.createElement('img');
        btnExcluirItemComposicao.src     = '/imagens/excluir.gif';
        btnExcluirItemComposicao.onclick = function()
        {
            removerBeneficiario(this.parentNode.parentNode.getAttribute('id'), benid, sabano);
        }

        input_benid   = document.createElement('select');
        input_benid.setAttribute('class', 'CampoEstilo');
        input_benid.setAttribute('style', '250px');
        input_benid.setAttribute('name', 'benid[]');
        <?php


    $res = (array) $db->carregar('select benid, upper(bendsc) as bendsc from cte.beneficiario order by bendsc asc');

    while (list(, $item) = each($res)) {
        $item = array_map('addslashes', $item);
        echo 'selected=benid==' , $item['benid']
            , ';input_benid.options[input_benid.options.length]=new Option(\'' , $item['bendsc'] , '\',\'' , $item['benid'] , '\',selected,selected);';
    }


?>

        input_vlrurbano = document.createElement('input');
        input_vlrurbano.setAttribute('type', 'text');
        input_vlrurbano.setAttribute('class', 'normal');
        input_vlrurbano.setAttribute('name', 'vlrurbano[]');
        input_vlrurbano.setAttribute('maxlength', '5');
        input_vlrurbano.setAttribute('size', '5');
        input_vlrurbano.setAttribute('value', vlrurbano);

        input_vlrrural  = document.createElement('input');
        input_vlrrural.setAttribute('type', 'text');
        input_vlrrural.setAttribute('class', 'normal');
        input_vlrrural.setAttribute('name', 'vlrrural[]');
        input_vlrrural.setAttribute('maxlength', '5');
        input_vlrrural.setAttribute('size', '5');
        input_vlrrural.setAttribute('value', vlrrural);


        var tabelabeneficiariosSubAcao   = $('beneficiariosSubAcao');
        var novaLinha                    = tabelabeneficiariosSubAcao.insertRow(tabelabeneficiariosSubAcao.rows.length);
        novaLinha.id                     = 'linhabeneficiariosSubAcao' + tabelabeneficiariosSubAcao.rows.length + 1;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(btnExcluirItemComposicao);

        /**
         * Descricao
         */
        var novaCelula1 = novaLinha.insertCell(novaLinha.cells.length);
        input_benid.setAttribute('id', novaLinha.cells.length);
        novaCelula1.appendChild(input_benid);

        /**
         * Unidade de medida
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula2.appendChild(input_vlrurbano);

        /**
         * Unidade de medida
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula3.appendChild(input_vlrrural);

        _modificado = modificado;
        return false;
    }


    function carregarBeneficiarios(){<?php
if ($_REQUEST['sbaid'] && $_REQUEST['sabano']) {
    $itens = $db->carregar('SELECT
                                sb.benid,
                                sb.vlrurbano,
                                sb.vlrrural
                            FROM
                                cte.subacaobeneficiario sb
                            INNER JOIN
                                cte.beneficiario be ON sb.benid = be.benid
                            WHERE
                                sb.sbaid  = ' . $_REQUEST['sbaid']   . '
                                AND
                                sb.sabano = ' . $_REQUEST['sabano']  . '
                            ORDER BY
                                be.bendsc ASC');

    if (!is_array($itens)) {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            echo "adicionarBeneficiario(" , $item['benid'] , ",'" , $item['vlrurbano'] , "','" , $item['vlrrural'] , "'," , $_REQUEST['sabano'] , ",false);";
        }
    }
} else {
    echo 'return false;';
}



?>}/*!@ !function carregarBeneficiarios() */
    carregarBeneficiarios();
    $('loader-container').hide();

    function fecharJanela()
    {
        if (_modificado) {
            if (!confirm('Existem dados que não foram salvos.\nDeseja fechar a janela sem salvá-los?'))
                return false;
        }

        window.close();
    }

    window.onunload = function()
    {
    	var sbaid 			 = <?php echo $_REQUEST["sbaid"]; ?>;
    	var parsubacaoanocrt = <?php echo $_REQUEST["sabano"]; ?>;
    	var sbaidpai  = <?php echo $sbaidPai;  ?>;
    	var anoconvenio = <?php echo $anoAditivo; ?>;
    	var aditivo		= <?php echo $aditivo; ?>
    	
        //window.opener.carregarBeneficiarios();
        //window.opener.location.href = '/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&parsubacaoanocrt='+ parsubacaoanocrt +'&sbaid='+ sbaid;    
   	    //window.opener.location.href = '/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&parsubacaoanocrt='+ parsubacaoanocrt +'&sbaid='+ sbaid + '&sbaidpai='+ sbaidpai + '&anoconvenio='+anoconvenio + '&ad='+aditivo;
   
    }
  -->
  </script>
</html>
