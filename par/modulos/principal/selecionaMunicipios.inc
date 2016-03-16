<?php

if (array_key_exists('btnGravar', $_REQUEST)) {
//	ver($_REQUEST, d);

	$db->executar('DELETE FROM par.questionarioprofuncionariomun WHERE qrpid = '.$_REQUEST['qrpid'].' AND perid = '.$_REQUEST['perid']);

	foreach( $_REQUEST['ecsqtd'] as $k => $qtd  ){
		if( $qtd ){
			$sql = "INSERT INTO
						par.questionarioprofuncionariomun( qrpid, perid, muncod, qpmqtd ) 
					VALUES
						( {$_REQUEST['qrpid']}, {$_REQUEST['perid']}, {$_REQUEST['qfaid'][$k]}, {$qtd} )";
			
			$db->executar( $sql );
		}
	}
	$db->commit();
/*    $db->executar(sprintf('DELETE FROM cte.escolacomposicaosubacao WHERE cosid = %d',
                          (integer) $_REQUEST['cosid']));
    $ins = '
    INSERT INTO par.questionarioprofuncionariomun(
        qrpid,
        perid,
        inuid,
        qpmqtd
    ) VALUES (%d, %d, %f)';
	if($_REQUEST['qfaid']) {
    	foreach ($_REQUEST['qfaid'] as $k => $qfaid) {
        	$db->executar(sprintf($ins,
            	                  $qfaid,
                	              $_REQUEST['cosid'],
                    	          str_replace(',', '.', str_replace('.', '', $_REQUEST['ecsqtd'][$k]))));
           $total += str_replace(',', '.', str_replace('.', '', $_REQUEST['ecsqtd'][$k]));
	    }
	    $sql = "UPDATE cte.composicaosubacao SET cosqtd = '{$total}' WHERE cosid = '{$_REQUEST['cosid']}'";
	    $db->executar($sql);
	}	
   	$db->commit();*/
}

$inuid = $_SESSION['par']['inuid'];
$sql = "SELECT estuf FROM par.instrumentounidade WHERE inuid = {$inuid}";
$estuf = $db->pegaUm( $sql );

$sql = '
SELECT DISTINCT 
	\'\' as codigo, 
	\'\' as descricao, 
	\'<input onmouseout="javascript: mouseOutToolTip(this);" onmouseover="javascript: mouseOverToolTip( this );" class="CampoEstilo" size="15" onblur="preencherTodasQtds()" type="text" id="qfaqtdTotal" name="qfaqtdTotal" />    
    <label for="qfaqtdTotal">Qtd Padrão</label>
    <div id="toolTipQtd">Este valor será replicado a todos os campos abaixo.</div>\' as ecsqtd
UNION ALL
SELECT DISTINCT

	m.muncod as codigo,
	m.mundescricao as descricao,
    \'<input
      class="CampoEstilo"
      onmouseover="MouseOver(this);"
      onfocus="MouseClick(this);"
      onmouseout="MouseOut(this);"
      onblur="MouseBlur(this);"
      onfocus="this.select()"
      onclick="this.select()"
      size="15"
      name="ecsqtd[]" \' ||
      CASE WHEN
      		qpm.qpmqtd IS NOT NULL
      THEN
      		\' value="\' || qpm.qpmqtd || \'" \'
      ELSE
      		\' value="" \'
      END  
      || \'
      /></center>
    <input type="hidden" name="qfaid[]" value=" \' || m.muncod || \' "  />\' as ecsqtd
FROM
	territorios.municipio m
LEFT JOIN
	par.questionarioprofuncionariomun qpm ON qpm.muncod = m.muncod AND qpm.perid = '.$_REQUEST['perid'].'
WHERE
	m.estuf = \''.$estuf.'\'
ORDER BY
    descricao';

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>

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
        
        #toolTipQtd{
        	width: 100px; 
        	height: 50px; 
        	background: #fff; 
        	position: absolute;
        	border: 1px #555 solid;
        	padding: 5px;
        	display: none;
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
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Cadastro de Quantitativos<br /><?php echo $db->pegaUm('SELECT cosdsc FROM cte.composicaosubacao WHERE cosid = ' . (integer) $_REQUEST['cosid']); ?></div>
          <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
			<?php
				$db->monta_lista_simples($sql, array('Código do Município', 'Município', 'Quantidade'), 5000, 1000, 'N', '100%');
			?>
          </div>
        </td>
      </tr>
      <tr>
        <td class="SubTituloDireita" style="padding-right: 25px;">
        Total: <?php echo $db->pegaUm('SELECT COALESCE(sum(qpmqtd),0) FROM par.questionarioprofuncionariomun WHERE qrpid = ' . $_REQUEST['qrpid'] .' AND perid = ' . $_REQUEST['perid']); ?>
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <input type="hidden" name="perid" value="<?php echo $_REQUEST['perid']; ?>" />
          <input type="hidden" name="qrpid" value="<?php  echo $_REQUEST['qrpid'];  ?>" />
          <input type="hidden" name="inuid" value="<?php  echo $inuid;  ?>" />
          <input type="submit" name="btnGravar" value="Gravar" />
          <!-- <input type="button" name="btnFechar" value="Fechar" onclick="window.close();" /> -->
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">

    window.onunload = function()
    {
    	window.opener.jQuery("input:button").next().trigger('click');
        window.close();
    }

    $('loader-container').hide();

    function mouseOverToolTip( objeto ){
    	MouseOver( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "block";    
    }
    
    function mouseOutToolTip( objeto ){
    	MouseOut( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "none";    
    }    
    
    function preencherTodasQtds(){
    	var qtdPadrao = document.getElementById( 'qfaqtdTotal' ).value;
        var arInputQtd = document.getElementsByName('ecsqtd[]');
        	
        if( qtdPadrao ){
	    	for( i = 0; i < arInputQtd.length; i++ ){
   				arInputQtd[i].value = qtdPadrao;
	    	}
    	}
    }    
    
  </script>
</html>