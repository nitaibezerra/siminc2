<?php

header('Cache-control: must-revalidate');
header('Expires: Fri, 01 Jan 1999 00:00:00 GMT');

include_once( APPRAIZ. "cte/classes/MonitoramentoInterno.class.inc" );
include_once( APPRAIZ. "cte/classes/MonitoramentoInternoEntidade.class.inc" );

if( !$_REQUEST['mniano'] || !$_REQUEST['inuid'] || !$_REQUEST['ppsid'] ){ die();}

$obMonitoramentoInterno = new MonitoramentoInterno();
$obMonitoramentoInterno->carregarPorInuidPpsidAno( $_REQUEST['inuid'], $_REQUEST['ppsid'], $_REQUEST['mniano'] );

if( array_key_exists('btnGravar', $_REQUEST) ){
	
	$mniqtd = is_array( $_REQUEST['entid'] ) ? count( $_REQUEST['entid'] ) : '0';
	$obMonitoramentoInterno->mniqtd = $mniqtd;
	$obMonitoramentoInterno->usucpf = $_SESSION['usucpf'];	
	$obMonitoramentoInterno->salvar();
	$obMonitoramentoInterno->commit();
	
	if( $mniqtd ){
		
		foreach( $_REQUEST['entid'] as $count => $entid ){
			
			$obMonitoramentoInternoEntidade = new MonitoramentoInternoEntidade();
			if( !$count ) $obMonitoramentoInternoEntidade->excluirTodosPorMniid( $obMonitoramentoInterno->mniid );
			
			$obMonitoramentoInternoEntidade->mniid = $obMonitoramentoInterno->mniid;
			$obMonitoramentoInternoEntidade->entid = $entid;
			$obMonitoramentoInternoEntidade->mneqtd = $_REQUEST['qfaqtd'][$entid];
			
			$obMonitoramentoInternoEntidade->salvar();
			$obMonitoramentoInternoEntidade->commit();
		}
	}
	
    echo "	<script>
			    alert('Operação Efetuada com sucesso!');
			    window.opener.location = 'cte.php?modulo=principal/monitoramentoInterno/cadastrarMonitoramentoInterno&acao=A&ppsid=".$obMonitoramentoInterno->ppsid."&mniano=".$obMonitoramentoInterno->mniano."'
    		</script>";
    exit();
    
}

$sql = "select entid, mneqtd
		from cte.monitoramentointerno mi
			inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
		where mi.inuid = '{$_REQUEST['inuid']}'
		and mi.ppsid = '{$_REQUEST['ppsid']}'
		and mi.mniano = '{$_REQUEST['mniano']}'";

$checks = (array) $db->carregar($sql);

$inuid = $_REQUEST['inuid'];

$itrid = cte_pegarItrid( $inuid );
$estuf = cte_pegarEstuf( $inuid );
$muncod = cte_pegarMuncod( $inuid );

$todas = (array) $db->carregar( cte_pegarSQLTodasEscolas( $itrid, $estuf, $muncod ) );
$sqlComplemento = cte_pegarSQLTodasEscolas( $itrid, $estuf, $muncod, false );

$sql = '
(
SELECT DISTINCT
    \'<input type="checkbox" onchange="return selecionarTodas();" id="selecionar" />\' as checkbox,
    \'<label for="selecionar" style="cursor:pointer">Selecionar Todas</label>\' as entcodent,
    null as mundescricao,
    null as entnome,
	\'<input onmouseout="javascript: mouseOutToolTip(this);" onmouseover="javascript: mouseOverToolTip( this );" class="normal" style="width:10ex;" size="10" onblur="preencherTodasQtds()" type="text" id="qfaqtdTotal" name="qfaqtdTotal" />    
    <label for="qfaqtdTotal">Qtd Padrão</label>
    <div id="toolTipQtd">Este valor será replicado a todos os campos abaixo.</div>\' as qfaqtd
)
UNION ALL';
$sql .= "($sqlComplemento)";

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
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmEscolas" onsubmit="return validarFrmEscolas();">
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Cadastrar Escolas</div>
          <div style="padding: 5px; text-align: center">Unidade de Medida: <?=$select_unidadeMedida; ?> </div>
          
          <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
				<?php $db->monta_lista_simples($sql, array('', 'Município', 'Código INEP', 'Escola', 'Qtd'), 5000, 10, 'N', '100%');?>
          </div>
          
          <br />
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <input type="hidden" name="qfaano" value="<?php echo $_REQUEST['qfaano']; ?>" />
          <input type="hidden" name="sbaid" value="<?php echo $_REQUEST['sbaid']; ?>" />
          <input type="submit" name="btnGravar" value="Gravar" />
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  <!--
    /**
     * 
     */
    function selecionarTodas()
    {
        var items = Form.getElements($('frmEscolas'));

        for (var i = 0; i < items.length; i++) {
            if (items[i].getAttribute('type') == 'checkbox' &&
                /entid/.test(items[i].getAttribute('id')))
            {
                items[i].checked = $('selecionar').checked;
            }
        }
    }

    function validarFrmEscolas()
    {
        var checks = $('frmEscolas').getInputs('checkbox', 'entid[]');
        var submit = true;
        
        for (var i = 0, length = checks.length; i < length; i++) {
            if (checks[i].checked || checks[i].checked == 'checked') {
                var qfaqtd = $('frmEscolas').getInputs('text');
				
				if(qfaqtd[i+1].value == '' || qfaqtd[i+1].value == 0 ){
					 //qfaqtd.setStyle({backgroundColor: '#ffd'});
					 submit = false;
				}
				
            }
        }

		 if (submit == false){
		 	alert('Para escolas selecionadas, a quantidade deve ser informada!');
		  	return submit;
		 }else{
		  	return submit;
		 }
		 

    }
    
    function preencherTodasQtds(){
    	var qtdPadrao = document.getElementById( 'qfaqtdTotal' ).value;
        var arInputQtd = document.getElementsByTagName('input');
        	
        if( qtdPadrao ){
	    	for( i = 0; i < arInputQtd.length; i++ ){
	    		if( arInputQtd[i].name.substr( 0, 6 ) == 'qfaqtd' )
    				arInputQtd[i].value = qtdPadrao;
	    	}
    	}
    }
    
    function mouseOverToolTip( objeto ){
    	MouseOver( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "block";    
    }
    
    function mouseOutToolTip( objeto ){
    	MouseOut( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "none";    
    }


<?php
foreach ($todas as $ent) {
    foreach ($checks as $check) {
        if ($ent['entid'] == $check['entid'] && $ent['entid']){
        	echo '$("entid_' .trim($check['entid']). '").setAttribute("checked","checked");';
        	echo "document.getElementById( 'qfaqtd[".$check['entid']."]' ).value = ".$check["mneqtd"].";";
        }
    }
}
?>

    $('loader-container').hide();
  -->
  </script>
</html>