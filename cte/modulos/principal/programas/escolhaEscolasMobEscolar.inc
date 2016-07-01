<?php
if($_REQUEST['qtd2010']){
	$valoresQtd 			= str_replace('\"', '"', $_REQUEST['qtd2010']);
	$qtdEscItens 	= json_decode($valoresQtd);
	echo '<div id="composicao" style="display:none;"  >'.simec_json_encode($qtdEscItens).'</div>';
}else if($_REQUEST['qtd2011']){
	$valoresQtd 			= str_replace('\"', '"', $_REQUEST['qtd2011']);
	$qtdEscItens 	= json_decode($valoresQtd);
	echo '<div id="composicao" style="display:none;"  >'.simec_json_encode($qtdEscItens).'</div>';
}

cte_verificaSessao();
$dadosSalvos                         = false;

$sql = '
SELECT DISTINCT 
	\'\' as entcodent, 
	\'\' as entnome, 
	\'<input onkeyup="this.value" onmouseout="javascript: mouseOutToolTip(this);" onmouseover="javascript: mouseOverToolTip( this );" class="CampoEstilo" size="15" onblur="preencherTodasQtds()" type="text" id="qfaqtdTotal" name="qfaqtdTotal" />    
    <label for="qfaqtdTotal">Qtd Padrão</label>
    <div id="toolTipQtd">Este valor será replicado a todos os campos abaixo.</div>\' as ecsqtd
UNION ALL
SELECT DISTINCT
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
WHERE
    sbaid  = ' . (integer) $_REQUEST['sbaid'] . '
    and
    qfaano = ' . (integer) $_REQUEST['qfaano']. '
ORDER BY
    entnome';

$total = $db->carregar($sql);

$total = count($total);
if($total == 1){
	$sql = array();
	echo '<script type="text/javascript">
			alert("Não existem escolas cadastradas para este ano. \n Para poder cadastrar quantidades informe as escolas.");
			window.close();
		</script>';
	die();
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

$db->monta_lista_simples($sql, array('Código INEP', 'Escola', 'Quantidade'), 1000, 1000, 'N', '100%');

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
          <input type="hidden" name="qfaano" id="qfaano" value="<?php echo $_REQUEST['qfaano']; ?>" />
          <input type="hidden" name="numlinha" id="numlinha" value="<?php echo $_REQUEST['linha']; ?>" />
          <input type="hidden" name="sbaid" value="<?php  echo $_REQUEST['sbaid'];  ?>" />
          <input type="hidden" name="cosid" value="<?php  echo $_REQUEST['cosid'];  ?>" />
          <input type="submit" name="btnGravar" value="Gravar" />
           <!--  <input type="button" name="teste" value="teste" onclick="testelista();" /> -->
          <!-- <input type="button" name="btnFechar" value="Fechar" onclick="window.close();" /> -->
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">

    window.onunload = function()
    {	
	    var linha =  $('numlinha').value;
	  	var qfaano = $('qfaano').value;
	  	var total = 0;
//  		var arEsQtd = new Array(); 
		var arEsQtd = new Array(); 
  		var esQtd = ''; 
    	var arQtd 	= document.getElementsByName('ecsqtd[]');
    	var arQfaid = document.getElementsByName('qfaid[]');
    	
  	  	for( o=0; o<arQtd.length; o++ ){
  	  		arEsQtd[o] = '{"qtd" : ' + arQtd[o].value + ', "id" : ' + arQfaid[o].value + '}';
  	  		total = Number(total) + Number(arQtd[o].value);
        }
		
		if(total == 0){
			alert('A quantidade tem que ser informada em pelo menos um das escola. ');
			return false;
		}
		
  	  	esQtd = '[' + arEsQtd.join(', ') + ']';	
        window.opener.atualizaItensComposicao(esQtd, qfaano, linha, total);
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

    <?php
    if($_REQUEST['qtd2010'] || $_REQUEST['qtd2011'] ){
    ?>
  	var arDados 	= new Object;
	var arDados = eval('(' + document.getElementById('composicao').innerHTML + ')');
	var arteste 	= document.getElementsByName('ecsqtd[]');
	var arIdEscola 	= document.getElementsByName('qfaid[]');
	if(arDados){
		for( z=0; z<arteste.length; z++ ){
			if(arteste[z].value == 0 ){
				for(j=0; j<arDados.length; j++){
					if(arIdEscola[z].value == arDados[j].id ){
						arteste[z].value = arDados[j].qtd;
					}
				}
			}
		}  
	}
	<?php  } ?>
   
  </script>
</html>