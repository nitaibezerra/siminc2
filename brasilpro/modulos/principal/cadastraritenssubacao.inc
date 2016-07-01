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
$dadosSalvos                         = false;

if (array_key_exists('excluir', $_REQUEST)) {
    $db->executar('DELETE FROM cte.composicaosubacao WHERE cosid = ' . (integer) $_REQUEST['excluir']);
    $db->commit();
    die('null');
}

$_REQUEST['cosid'] = $_REQUEST['cosid'] ? $_REQUEST['cosid'] : array();

if ($_REQUEST['inserirItensLaboratorio']) {
    $sql ='
    SELECT
        il.*,
        udl.unddid,
        udl.undddsc
    FROM
        cte.laboratorio l JOIN cte.itenslaboratorio il USING(labid)
        JOIN cte.unidademedidadetalhamento udl USING(unddid) 
    WHERE
        l.undid = (SELECT undid FROM cte.subacaoindicador WHERE sbaid = ' .$_REQUEST['sbaid']. ')';

    $res   = $db->carregar($sql);
    $itens = array();

    while (list($i, $lab) = each($res)) {
        $itens[] = '{\'cosid\':\'\','
                 . ' \'cosdsc\':\''   . addslashes(str_replace(array("\n", "\r", "\r\n"), ' ', $lab['ilbdesc'])) . '\','
                 . ' \'unddid\':'     . $lab['unddid']              . ','
                 . ' \'cosano\':'     . $_REQUEST['cosano']         . ','
                 . ' \'modificado\':true,'
                 . ' \'cosqtd\':'     . $lab['ilbqtditens']         . ','
                 . ' \'cosvlruni\':\''. $lab['ilbvlrunitario']      . '\'}';
    }

    header('content-type: application/json;charset=iso-8859-1');
    echo 'var itensLaboratorio=[' , implode(',', $itens) , '];';
    die();
}

//if (array_key_exists('btnGravar', $_REQUEST) && is_array($_REQUEST['cosid'])) {
if ($_REQUEST['gravarForm'] == "true")
{
    if ($_REQUEST['cosqtd'] && $_REQUEST['cosvlruni']) {
        $ins = "INSERT INTO cte.composicaosubacao (
                    sbaid,
                    cosdsc,
                    unddid,
                    cosano,
                    cosqtd,
                    cosvlruni
                ) VALUES (%d, '%s', %d, %d, %f, %f)";
    
        $upd = "UPDATE cte.composicaosubacao SET
                    cosdsc    = '%s',
                    unddid    = %d,
                    cosano    = %d,
                    cosqtd    = %f,
                    cosvlruni = %f
                WHERE
                    cosid     = %d";

        foreach ($_REQUEST['cosid'] as $k => $cosid) {
            if (trim($_REQUEST['cosdsc'][$k]) == '')
                continue;

            if ($cosid == '') {
                $db->executar(sprintf($ins, (integer) $_REQUEST['sbaid'],
                                            (string)  $_REQUEST['cosdsc'][$k],
                                            (integer) $_REQUEST['unddid'][$k],
                                            (integer) $_REQUEST['cosano'],
                                            (integer) $_REQUEST['cosqtd'][$k],
                                            (integer) $_REQUEST['cosvlruni'][$k]));
            } else {
                $db->executar(sprintf($upd, (string)  $_REQUEST['cosdsc'][$k],
                                            (integer) $_REQUEST['unddid'][$k],
                                            (integer) $_REQUEST['cosano'],
                                            (integer) $cosid,
                                            (integer) $_REQUEST['cosqtd'][$k],
                                            (integer) $_REQUEST['cosvlruni'][$k]));
            }
        }
    } else {
        $ins = "INSERT INTO cte.composicaosubacao (
                    sbaid,
                    cosdsc,
                    unddid,
                    cosano
                ) VALUES (%d, '%s', %d, %d)";
    
        $upd = "UPDATE cte.composicaosubacao SET
                    cosdsc = '%s',
                    unddid = %d,
                    cosano = %d
                WHERE
                    cosid  = %d";

        foreach ($_REQUEST['cosid'] as $k => $cosid) {
            if (trim($_REQUEST['cosdsc'][$k]) == '')
                continue;

            if ($cosid == '') {
                $db->executar(sprintf($ins, (integer) $_REQUEST['sbaid'],
                                            (string)  $_REQUEST['cosdsc'][$k],
                                            (integer) $_REQUEST['unddid'][$k],
                                            (integer) $_REQUEST['cosano']));
            } else {
                $db->executar(sprintf($upd, (string)  $_REQUEST['cosdsc'][$k],
                                            (integer) $_REQUEST['unddid'][$k],
                                            (integer) $_REQUEST['cosano'],
                                            (integer) $cosid));
            }
        }
    }
    $db->commit();
    $dadosSalvos = true;
    //     

    echo "<script>
    alert('Operação Efetuada com sucesso!');
    window.opener.location.reload();</script>";
    exit();
}


$sql = '
SELECT
    il.labid
FROM
    cte.laboratorio l
INNER JOIN
    cte.itenslaboratorio il ON l.labid = il.labid
WHERE
    l.undid = (SELECT
                   undid
               FROM
                   cte.subacaoindicador
               WHERE
                   sbaid = ' . (integer) $_REQUEST['sbaid'] . ')';;

$labid = $db->pegaUm($sql);


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
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadItensComposicao" name="cadItensComposicao">
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Itens</div>
          <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
            <table id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
              <colgroup>
                <col width="10%" />
                <col width="50%" />
                <col width="40%" />
              </colgroup>

              <tr style="text-align: center">
                <td style="padding: 2px; font-weight: bold;">&nbsp;</td>
                <td style="padding: 2px; font-weight: bold;">Identificação do Item</td>
                <td style="padding: 2px; font-weight: bold;">Unidade de Medida</td>
              </tr>
            </table>
          </div>
          <div style="padding: 5px 0px 0px 5px">
<?php
if ($labid)
    echo '            <span style="cursor:pointer" id="linkInserirItem" onclick="return inserirLaboratorio();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir Item de Composição</span>';
else
    echo '            <span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarItemComposicao(\'\',\'\',\'\',',$_REQUEST['cosano'],',true);"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir Item de Composição</span>';

echo "\n";
?>
          </div><br />
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <span style="display: block;font-weight:bold;font-size:110%;margin-bottom:5px;color:red">Atenção! Itens sem identifição serão descartados</span>
          <!-- <input type="submit" name="btnGravar" value="Gravar" /> -->
          <input type="hidden" name="gravarForm" value="true" />
          <input type="button" name="btnGravarOnclick" value="Gravar" onclick="validaFormulario()" />
         <!-- <input type="button" name="btnFechar" value="Fechar" onclick="fecharJanela();" />  -->
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
    
    function getElementsByName_iefix(tag, name) 
	{             
	var elem = document.getElementsByTagName(tag);  
	var arr = new Array();  
		for(i = 0,iarr = 0; i < elem.length; i++) {  
			att = elem[i].getAttribute("name");  
			if(att == name) 
			{  
				arr[iarr] = elem[i];  
				iarr++;  
			}  
		}  
		return arr;  
	}       
    
    function validaFormulario()
    {
    	
    	var arItenComp = getElementsByName_iefix( "input", "cosdsc[]" );		
               
		for( i=0; i<arItenComp.length; i++ ){
	               
			if( arItenComp[i].value == '')
			{
				alert("O campo IDENTIFICAÇÃO DO ITEM é obrigatório!");
				arItenComp[i].focus();
				return false;				
			}
		}   	
    	
    	document.cadItensComposicao.submit();    	
    	return true;
    	
    }

    /**
     * 
     */
    function removerItemComposicao(linha, cosid)
    {
        var linha = $(linha);
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
            $('loader-container').show();
            try {
                if (cosid != '') {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + cosid,
                                                   onComplete: function(r)
                                                   {
                                                       //if (r.responseText == 'null')
                                                           linha.parentNode.removeChild(linha);
                                                       //else
                                                       //    alert('Não foi possível excluir o item.');
                                                           linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
                } else {
                    linha.parentNode.removeChild(linha);
                }
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }

    var labInserido = false;
    function inserirLaboratorio()
    {
        if (labInserido)
            return false;

        new Ajax.Request(window.location.href, {
                         method: 'post',
                         parameters: 'inserirItensLaboratorio=1',
                         onComplete: function(res)
                         {
                             eval(res.responseText);

                             for (var i = 0, l = itensLaboratorio.length; i < l; i++) {
                                 adicionarItemComposicao(itensLaboratorio[i].cosid,
                                                         itensLaboratorio[i].cosdsc,
                                                         itensLaboratorio[i].unddid,
                                                         itensLaboratorio[i].cosano,
                                                         itensLaboratorio[i].modificado,
                                                         itensLaboratorio[i].cosqtd,
                                                         itensLaboratorio[i].cosvlruni);
                             }
                         }
        });

        labInserido = true;
    }

    /**
     * 
     */
    function adicionarItemComposicao(cosid, cosdsc, unddid, cosano, modificado, cosqtd, cosvlruni)
    {
        var input_cosdsc;
        var input_unddid;

        var btnExcluirItemComposicao     = document.createElement('img');
        btnExcluirItemComposicao.src     = '/imagens/excluir.gif';
        btnExcluirItemComposicao.onclick = function()
        {
            if (labInserido)
                alert('Não é possível excluir itens de laboratórios.');
            else
                removerItemComposicao(this.parentNode.parentNode.getAttribute("id"), cosid);
        }

        input_cosdsc    = document.createElement('input');
        input_cosdsc.setAttribute('type', 'text');
        input_cosdsc.setAttribute('class', 'normal');
        input_cosdsc.setAttribute('name', 'cosdsc[]');
        input_cosdsc.setAttribute('maxlength', '250');
        input_cosdsc.setAttribute('size', '30');
        input_cosdsc.setAttribute('value', cosdsc);

        input_cosid     = document.createElement('input');
        input_cosid.setAttribute('type', 'hidden');
        input_cosid.setAttribute('name', 'cosid[]');
        input_cosid.setAttribute('value', cosid);

        input_unddid   = document.createElement('select');
        input_unddid.setAttribute('class', 'CampoEstilo');
        input_unddid.setAttribute('name', 'unddid[]');
        <?php
    $res = (array) $db->carregar('select unddid, undddsc from cte.unidademedidadetalhamento order by undddsc');

    while (list(, $item) = each($res)) {
        $item = array_map('addslashes', $item);
        echo 'selected=unddid==' , $item["unddid"]
            , ';input_unddid.options[input_unddid.options.length]=new Option(\'' , $item["undddsc"] , '\',\'' , $item["unddid"] , '\',selected,selected);';
    }

?>

        input_cosdsc.setAttribute('value', cosdsc);

        var tabelaItensComposicaoSubAcao = $('itensComposicaoSubAcao');
        var novaLinha                    = tabelaItensComposicaoSubAcao.insertRow(tabelaItensComposicaoSubAcao.rows.length);
        novaLinha.id                     = 'linhaItensComposicaoSubAcao' + tabelaItensComposicaoSubAcao.rows.length + 1;
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
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_cosdsc.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_cosdsc);
        novaCelula2.appendChild(input_cosid);

        /**
         * Unidade de medida
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula3.appendChild(input_unddid);

        if (cosqtd) {
            var input_cosqtd = document.createElement('input');
            input_cosqtd.setAttribute('type', 'hidden');
            input_cosqtd.setAttribute('name', 'cosqtd[]');
            input_cosqtd.setAttribute('value', cosqtd);

            novaCelula3.appendChild(input_cosqtd);
        }

        if (cosvlruni) {
            var input_cosvlruni = document.createElement('input');
            input_cosvlruni.setAttribute('type', 'hidden');
            input_cosvlruni.setAttribute('name', 'cosvlruni[]');
            input_cosvlruni.setAttribute('value', cosvlruni);

            novaCelula3.appendChild(input_cosvlruni);
        }


        if (cosdsc == null)
            input_cosdsc.select();

        _modificado = modificado;

        labInserido = true;
        return false;
    }


    function carregarItensComposicao(){<?php
if ($_REQUEST['sbaid'] && $_REQUEST['cosano']) {
    $itens = $db->carregar('SELECT
                                cs.cosid,
                                cs.cosdsc,
                                um.unddid
                            FROM
                                cte.composicaosubacao cs
                            INNER JOIN
                                cte.unidademedidadetalhamento um ON cs.unddid = um.unddid
                            WHERE
                                cs.sbaid  = ' . $_REQUEST['sbaid']   . '
                                AND
                                cs.cosano = ' . $_REQUEST['cosano']  . '
                            ORDER BY
                                cs.cosdsc ASC,
                                cs.cosord ASC');

    if (!is_array($itens)) {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);
            echo "adicionarItemComposicao(" , $item['cosid'] , ",'" , $item['cosdsc'] , "','" , $item['unddid'] , "'," , $_REQUEST['cosano'] , ",false);";
        }
    }
} else {
    echo 'return false;';
}



?>}/*!@ !function carregarItensComposicao() */
    carregarItensComposicao();
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
    	var parsubacaoanocrt = <?php echo $_REQUEST["cosano"]; ?>;
    	// Aditivo
    	var sbaidpai  =  <?php echo $sbaidPai;  ?>;
    	var anoconvenio = <?php echo $anoAditivo;  ?>;
    	var aditivo		= <?php echo $aditivo; ?>
    	// Fim Aditivo
    	
        //window.opener.carregarItensComposicao();
       // window.opener.location.href = '/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&parsubacaoanocrt='+ parsubacaoanocrt +'&sbaid='+ sbaid + '&sbaidpai='+ sbaidpai + '&anoconvenio='+ anoconvenio + '&ad='+aditivo;
       // window.opener.location.href = '/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&parsubacaoanocrt='+ parsubacaoanocrt +'&sbaid='+ sbaid;        
    }

  -->
  </script>
</html>
