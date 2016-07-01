<?php
//
// $Id$
//

cte_verificaSessao();
$_SESSION['_parSubacaoAnoExercicio'] = $_REQUEST['sabano'];

$inuid = (integer) $_SESSION["inuid"];

$sbaid = $_REQUEST['sbaid'];
$select_unidadeMedida    = $db->pegaUm('select u.unddsc
										from cte.subacaoindicador s
										left join cte.programa p on p.prgid = s.prgid
										inner join cte.unidademedida u on u.undid = s.undid
										where
    									s.sbaid ='    . (integer) $sbaid);

header('Cache-control: must-revalidate');
header('Expires: Fri, 01 Jan 1999 00:00:00 GMT');

if (array_key_exists('btnGravar', $_REQUEST) &&
    array_key_exists('sbaid',     $_REQUEST))
{
    if (is_array($_REQUEST['entid'])) {
        $sql = 'DELETE FROM
                    cte.qtdfisicoano
                WHERE
                    qfaano = ' . (integer) $_REQUEST['qfaano'] . '
                    AND
                    sbaid  = ' . (integer) $_REQUEST['sbaid']  . '
                    AND
                    entid NOT IN (' . implode(', ', $_REQUEST['entid']) . ')';

        $db->executar($sql);

        $ins = 'INSERT INTO cte.qtdfisicoano (
                    sbaid,
                    qfaano,
                    qfaqtd,
                    entid
                ) VALUES (' . (integer) $_REQUEST['sbaid']  . ',
                          ' . (integer) $_REQUEST['qfaano'] . ', %d, %d)';

        $upd = 'UPDATE cte.qtdfisicoano SET
                    qfaqtd = %d
                WHERE
                    sbaid  = ' . (integer) $_REQUEST['sbaid']  . '
                    AND
                    qfaano = ' . (integer) $_REQUEST['qfaano'] . '
                    AND
                    entid  = %d';

        $db->Executar('
                PREPARE count_qfaid(int) as
                    SELECT
                        count(qfaid)
                    FROM
                        cte.qtdfisicoano
                    WHERE
                        qfaano = ' . (integer) $_REQUEST['qfaano'] . '
                        AND
                        sbaid  = ' . (integer) $_REQUEST['sbaid']  . '
                        AND
                        entid  = $1');

        foreach ($_REQUEST['entid'] as $entid) {
            $qfaqtd = array_key_exists($entid, $_REQUEST['qfaqtd']) ? $_REQUEST['qfaqtd'][$entid] : 0;
            $res    = $db->pegaUm('EXECUTE count_qfaid(' . $entid . ')');

            if ($res == 1) {
                $db->executar(sprintf($upd, (integer) $qfaqtd,
                                            (integer) $entid));
            } else {
                $db->executar(sprintf($ins, (integer) $qfaqtd,
                                            (integer) $entid));
            }
        }
        
        // Inserindo escolas en instrumentounidadeescola

		
        $sqlBase = " insert into cte.instrumentounidadeescola ( inuid, entid )
											   			values( %d, %d )";
        
        $sql = "select entid from cte.instrumentounidadeescola where inuid = '$inuid'";
        $arEntidades = $db->carregarColuna( $sql );
        
       // dbg( in_array( $entid, $arEntidades ), 1 );
        
		foreach ( $_REQUEST['entid'] as $entid )
		{	
			if( in_array( $entid, $arEntidades ) ) continue;
			
			$sql = sprintf( $sqlBase, $inuid, $entid );
			$db->executar( $sql );
		}
        
    } else {
        $sql = 'DELETE FROM
                    cte.qtdfisicoano
                WHERE
                    qfaano = ' . (integer) $_REQUEST['qfaano'] . '
                    AND
                    sbaid  = ' . (integer) $_REQUEST['sbaid'];

        $db->executar($sql);
    }

    $db->commit();

    echo("<script>
				window.opener.carregarEscolas();
				window.close();
			</script>");

    //header('Location: ' . $_SERVER['REQUEST_URI']);
  
    
}

$sql = '
SELECT
    e.entid, qfaqtd
FROM
    cte.qtdfisicoano q
INNER JOIN
    entidade.entidade e on q.entid = e.entid
WHERE
    q.qfaano = ' . $_REQUEST['qfaano'] . '
    AND
    q.sbaid  = ' . $_REQUEST['sbaid'];

$checks = (array) $db->carregar($sql);

$itrid = cte_pegarItrid( $inuid );
$estuf = cte_pegarEstuf( $inuid );
$muncod = cte_pegarMuncod( $inuid );

$todas = (array) $db->carregar( cte_pegarSQLTodasEscolas( $itrid, $estuf, $muncod ) );
$sqlComplemento = cte_pegarSQLTodasEscolas( $itrid, $estuf, $muncod, false );

$sql = $sqlComplemento;

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
  <body onunload="window.opener.carregarEscolas();">
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmEscolas" onsubmit="return validarFrmEscolas();">
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
<?php

$db->monta_lista_simples($sql, array('', 'Município', 'Código INEP', 'Escola', 'Qtd'), 5000, 10, 'N', '100%');

?>
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <input type="hidden" name="qfaano" value="<?php echo $_REQUEST['qfaano']; ?>" />
          <input type="hidden" name="sbaid" value="<?php echo $_REQUEST['sbaid']; ?>" />
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  	window.print();
  	window.close();
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
        	//echo "alert(".$check['qfaqtd'].");";
        	echo "document.getElementById( 'qfaqtd[".$check['entid']."]' ).value = ".$check["qfaqtd"].";";
//        	echo '$("qfaqtd[' .trim($check['entid']). ']").setAttribute("value", '.$check["qfaqtd"].');';
			//qfaqtd[270081]        
        }
    }
}
?>

    $('loader-container').hide();
  -->
  </script>
</html>