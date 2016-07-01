<?php
//
// $Id$
//



cte_verificaSessao();
$dadosSalvos                         = false;


if (array_key_exists('btnGravar', $_REQUEST)) {
    $db->executar(sprintf('DELETE FROM cte.escolacomposicaosubacao WHERE cosid = %d',
                          (integer) $_REQUEST['cosid']));
    $ins = '
    INSERT INTO cte.escolacomposicaosubacao(
        qfaid,
        cosid,
        ecsqtd
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
   	$db->commit();
}





$sql = '
SELECT DISTINCT 
	\'\' as entcodent,		
	\'\' as entcodent, 
	\'\' as entnome, 
	\'<input onkeyup="this.value=mascaraglobal(\'\'###.###.###,#\'\', this.value);" onmouseout="javascript: mouseOutToolTip(this);" onmouseover="javascript: mouseOverToolTip( this );" class="CampoEstilo" size="15" onblur="preencherTodasQtds()" type="text" id="qfaqtdTotal" name="qfaqtdTotal" />    
    <label for="qfaqtdTotal">Qtd Padrão</label>
    <div id="toolTipQtd">Este valor será replicado a todos os campos abaixo.</div>\' as ecsqtd
UNION ALL
SELECT DISTINCT
	s.sbadsc,
	ent.entcodent,
    ent.entnome,
    \'<input
      class="CampoEstilo"
      onmouseover="MouseOver(this);"
      onfocus="MouseClick(this);"
      onmouseout="MouseOut(this);"
      onblur="MouseBlur(this);"
      onfocus="this.select()"
      onclick="this.select()"
      size="15"
      name="ecsqtd[]"
      value="\' ||
    	case 
    		when substring( trim(to_char(coalesce(ecs.ecsqtd, 0), \'9999999990D99\')), position(\',\' in trim(to_char(coalesce(ecs.ecsqtd, 0), \'9999999990D99\'))) +1, 2 ) = \'00\'
    			then trim(to_char(coalesce(ecs.ecsqtd, 0), \'999999999\'))
    		else trim(to_char(coalesce(ecs.ecsqtd, 0), \'9999999990D99\'))
    	END
      || \'" /></center>
    <input type="hidden" name="qfaid[]" value="\' || qfa.qfaid || \'" />\' as ecsqtd
FROM
    cte.qtdfisicoano qfa
LEFT JOIN
    cte.escolacomposicaosubacao ecs ON qfa.qfaid = ecs.qfaid and ecs.cosid  = ' . (integer) $_REQUEST['cosid'] . '
INNER JOIN
    entidade.entidade ent ON qfa.entid = ent.entid
INNER JOIN cte.subacaoindicador s ON s.sbaid = qfa.sbaid
WHERE
    qfa.sbaid  = ' . (integer) $_REQUEST['sbaid'] . '
    and
    qfa.qfaano = ' . (integer) $_REQUEST['qfaano']. '
ORDER BY
    entnome';



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

$db->monta_lista_simples($sql, array('Código INEP', 'Escola', 'Quantidade'),5000, 1000, 'N', '100%');

?>
          </div>
        </td>
      </tr>
      <tr>
        <td class="SubTituloDireita" style="padding-right: 25px;">
        Total: <?php echo $db->pegaUm('SELECT sum(ecsqtd) FROM cte.escolacomposicaosubacao WHERE cosid = ' . $_REQUEST['cosid']); ?>
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <input type="hidden" name="qfaano" value="<?php echo $_REQUEST['qfaano']; ?>" />
          <input type="hidden" name="sbaid" value="<?php  echo $_REQUEST['sbaid'];  ?>" />
          <input type="hidden" name="cosid" value="<?php  echo $_REQUEST['cosid'];  ?>" />
          <input type="button" value="Gerar Relatório" onclick="exibeRelatorio();" />
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
        window.opener.carregarItensComposicao();
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
    function exibeRelatorio()
    {
    	var janela = window.open( 'cte.php?modulo=relatorio/resultRelatorioQuantitativo&acao=A&sbaid=<?=$_REQUEST['sbaid']?>&qfaano=<?=$_REQUEST['qfaano']?>&cosid=<?=$_REQUEST['cosid']?>', 'relatorio_xls', 'width=300,height=300,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
    	janela.focus();
    }
  </script>
</html>