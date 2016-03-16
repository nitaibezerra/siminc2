<?php
$ano 	= (integer) $_REQUEST['qfaano'];
$sbaid 	= $_REQUEST['sbaid'];
if($ano == 'null'){
?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>Selecione o Ano</title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
	</head>
	<body>
	<form action="" method="post" id="frmAno" name="frmAno">
	<input type=hidden name="qfaano" value="null" />
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr>
      	<td class="SubtituloEsquerda"> Selecione o ano em que a escola será cadastrada.
      	</td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td>
		<select id="ano" name="ano" onchange="selecionaAno(this.value)">
			<option value="0">Selecione</option>
			<option value="2010">2010</option>
			<option value="2011">2011</option>
		</select>
		</td>
	  </tr>
	</table>
	</form>
	<script type="text/javascript">
		function selecionaAno(ano){ 
			if(ano != 0){
	        	document.frmAno.qfaano.value = ano;
				document.frmAno.submit();
				return true;
			}else{
				alert('Selecione um ano.');
			}
		}
	</script>
	</body>
</html>
<?php
	die();
}else{

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
            $qfaqtd = $_REQUEST['qfaqtd'];
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

$todas = (array) $db->carregar( cte_pegarSQLTodasEscolas2( $itrid, $estuf, $muncod ) );
$sqlComplemento = cte_pegarSQLTodasEscolas2( $itrid, $estuf, $muncod, false );


$sql = '
(
SELECT DISTINCT
    \'<input type="checkbox" onchange="return selecionarTodas();" id="selecionar" />\' as checkbox,
    \'<label for="selecionar" style="cursor:pointer">Selecionar Todas</label>\' as entcodent,
    null as mundescricao,
    null as entnome
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
  <body onunload="window.opener.carregarEscolas();">
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
<?php

$db->monta_lista_simples($sql, array('', 'Município', 'Código INEP', 'Escola'), 5000, 10, 'N', '100%');

?>
          </div>
          <br />
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <input type="hidden" name="qfaano" value="<?php echo $_REQUEST['qfaano']; ?>" />
          <input type="hidden" name="sbaid" id="sbaid" value="<?php echo $_REQUEST['sbaid']; ?>" />
          <input type="hidden" name="qfaqtd" id="qfaqtd" value="1" />
          <input type="submit" name="btnGravar" value="Gravar" />
          <input type="button" name="btnPagAno" value="Voltar" onclick="votarPagAno();" />
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  function votarPagAno(){
	  	var sbaid = document.getElementById( 'sbaid' ).value;
		window.location.href="cte.php?modulo=principal/programas/cadastraEscolasMobEscolar&acao=A&sbaid="+sbaid;
		return false;
  }
  
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
				
				
            }
        }

		 if (submit == false){
		 	alert('Para escolas selecionadas, a quantidade deve ser informada!');
		  	return submit;
		 }else{
		  	return submit;
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
        	//echo "document.getElementById( 'qfaqtd[".$check['entid']."]' ).value = ".$check["qfaqtd"].";";
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
<?php 
	}
	
function cte_pegarSQLTodasEscolas2( $itrid, $estuf, $muncod, $boEntidades = true ){

	if( $boEntidades ){
		$stClausulaSelect = ' t.entid ';
	}
	else{
		$stClausulaSelect = '\'<input type="checkbox" name="entid[]" id="entid_\' || t.entid || \'" value="\' || t.entid || \'" />\' as checkbox,
				    		 m.mundescricao,
				    		 \'<label for="entid_\' || t.entid || \'" style="cursor:pointer">\' || t.entcodent || \'</label>\' as entcodent,
				    		 entnome';
	}

	if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL )
	{
	    $sqlComplemento = 'select distinct '.$stClausulaSelect.'					
	            from entidade.entidade t
		            inner join entidade.funcaoentidade f on f.entid = t.entid
		            inner join entidade.entidadedetalhe ed on t.entid = ed.entid
		            inner join entidade.endereco d on t.entid = d.entid
		            left join territorios.municipio m on m.muncod = d.muncod
	            where (t.entescolanova = false or t.entescolanova is null)
	            and f.funid = 3 and
	            t.tpcid = 1 and
	            m.estuf = \'' . $estuf . '\'
	            group by t.entid, entnome, t.entcodent, m.mundescricao ';
	    $sqlComplemento .= $boEntidades ? "" : 'order by m.mundescricao, entnome';
	} else {
	    $sqlComplemento = 'select distinct '.$stClausulaSelect.'
	            from entidade.entidade t
		            left join entidade.entidadedetalhe entd on t.entcodent = entd.entcodent
		                and (
		                     entdreg_infantil_creche = \'1\' or
		                     entdreg_infantil_preescola = \'1\' or
		                     entdreg_fund_8_anos        = \'1\' or
		                     entdreg_fund_9_anos        = \'1\'
						)
		            inner join entidade.endereco ende on t.entid = ende.entid
					left join territorios.municipio m on m.muncod = ende.muncod	                
	            where (t.entescolanova = false or t.entescolanova is null)
	            and ende.muncod = \'' . $muncod . '\'
	            and t.tpcid = 3
                and
                t.entstatus = \'A\' ';
	    
	    $sqlComplemento .= $boEntidades ? "" : 'order by m.mundescricao, entnome';
	}
	//if( !$boEntidades ) dbg($sqlComplemento, 1);
	
	return $sqlComplemento;	
}
?>