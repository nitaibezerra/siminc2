<?php

include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');

$somenteLeitura = 'S';
$estuf 			= $_REQUEST['estuf'];

if ($_REQUEST['friid']) {
    
    $sql = "select * 
                from 
                    parindigena.itemmonitoramento as im 
            inner join parindigena.formacaoinicial as f 
                on f.friid = im.friid 
            where 
                f.friid= {$_REQUEST['friid']}
            and im.itmstatus = 'A'";

    $rsDadosForm       = $db->carregar( $sql );
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf']; 
    
    $sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
    $rs = $db->carregar( $sql );
    if($rs[0]['covnumero']){
    	$dadosconv = explode('/', $rs[0]['covnumero']);
    	$covcod = $dadosconv[0];
    }else{
    	$covcod = '';
    }

    $tipo 	=   1;  
    $apmid 	= $_REQUEST['friid']; 
} else if ($_REQUEST['frcid']){
    $sql = "SELECT
                *  
            FROM
                parindigena.formacaocontinuada AS fi
            INNER JOIN parindigena.itemmonitoramento AS m ON m.frcid = fi.frcid
            WHERE
                fi.frcid = {$_REQUEST['frcid']}
                and m.itmstatus = 'A'";               

    $rsDadosForm 	   = $db->carregar( $sql );
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];    
    $tipo = 2;
    $apmid 		= $_REQUEST['frcid'];       
} else if ($_REQUEST['madid']) {
    
    $sql = "select * 
            from 
                parindigena.itemmonitoramento as im 
            inner join parindigena.materialdidatico as m on m.madid = im.madid 
            where m.madid = {$_REQUEST['madid']}
            and im.itmstatus = 'A'";
   
    $rsDadosMat = $db->carregar( $sql );
    
    $itmid 			   = $rsDadosMat[0]['itmid'];
    $estuf 		 	   = $rsDadosMat[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosMat[0]['estuf'];  
    $tipo = 3;
    $apmid 		= $_REQUEST['madid'];   

}

if($_REQUEST['salvar']){
	$totalSub 	= count($_REQUEST['subacoes']);
	$todasSub = explode(',', $_REQUEST['todassub']);
	if($totalSub > 0 && $apmid ){
		$executado = false;
		$sql = "SELECT sbaid FROM parindigena.associativaplanodemetasprogramas  WHERE  appid = ".$apmid;
		$subExisteBanco = $db->carregar($sql);
		if(is_array($subExisteBanco)){
			foreach($subExisteBanco as $sub){
				if (in_array($sub['sbaid'], $todasSub)) { 
					$sql = "DELETE FROM parindigena.associativaplanodemetasprogramas  WHERE  appid = ".$apmid." and sbaid = ".$sub['sbaid'];
					$db->executar($sql);
				}
			}
		}
		
		foreach($_REQUEST['subacoes'] as $subacao){
			$sql = "INSERT INTO parindigena.associativaplanodemetasprogramas (sbaid, appid, apptipo) VALUES (".$subacao.", ".$apmid.", ".$tipo.")";
			if($db->executar($sql)){
				$executado = true;
			}
		}
		if($executado){
			$db->commit();
			echo "<script> 
					alert( 'Associação feita com sucesso. ' ); 
			  	  </script>";
		}else{
			$db->rollback();
			echo "<script> 
					alert( 'Ocorreu um erro ao tentar salvar a vinculação entre os dados. ' ); 
					window.close();
			  	  </script>";
			die();
		}
	}
}

$filtros = '';
if($_REQUEST['filtro'] && $_REQUEST['descricaosub'] ){
	$filtros = "and sbadsc ilike lower('%".$_REQUEST['descricaosub']."%')";
}

$buscaporconvenio = '';

if($covcod != ''){
	$buscaporconvenio = " and prsnumconvsape = ".$covcod;
}

$sqlSubacoes = "SELECT  distinct '<input type=\"checkbox\" name=\"subacoes[]\" id=\"subacoes[]\" value=\"'|| si.sbaid ||'\"'|| 
						CASE WHEN  apm.sbaid is not null
						THEN 'checked' 
						ELSE '' END ||'>' as acao,
						 si.sbadsc
				FROM cte.projetosapesubacao psb
				INNER JOIN cte.projetosape ps ON ps.prsid = psb.prsid
				INNER JOIN cte.subacaoindicador si ON si.sbaid = psb.sbaid
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = ps.inuid
				LEFT JOIN  parindigena.associativaplanodemetasprogramas apm ON apm.sbaid = si.sbaid AND apm.sbaid = psb.sbaid AND apm.apptipo = ".$tipo." AND apm.appid = ".$apmid."
				WHERE iu.estuf = '".$estuf."' 
				 and iu.itrid = 1 
				 and si.frmid in (2,5) 
				".$buscaporconvenio."
				".$filtros."
				GROUP BY iu.inuid, si.sbaid, si.sbadsc, apm.sbaid
				ORDER BY si.sbadsc";

$temConvenio = $db->pegaLinha($sqlSubacoes);

$cabecalho = array( "Ação", "Descrição da Subação" );

if ($_REQUEST['friid']){
	$res = array(  array (  "descricao" => "Formação inicial",
							    "id" 		=> "0",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		  )
						);
			
	array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoInicial&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   ),
					array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   ),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "5",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
			  
	
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}");
			  
} else if ($_REQUEST['madid']){
	$res = array( 0 => array (  "descricao" => "Identificação do material",
								    "id" 		=> "0",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid={$rsDadosMat[0]['madid']}"
						  		  )
							);
	
	array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosMat[0]['madid']}"
					  		   ),
		  		   array ("descricao" => "Vinculação a subações do PAR Genêrico",
				    			"id"		=> "4",
				    			"link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
		  		   				),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "5",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
			  
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}");
			  
} else if ($_REQUEST['frcid']){
	$res = array( 0 => array (  "descricao" => "Formação Continuada",
								    "id" 		=> "0",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid={$rsDadosForm[0]['frcid']}"
						  		  )
							);
	
	array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&frcid={$rsDadosForm[0]['frcid']}"
					  		   ),
					array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
					  		   			),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "5",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
			  
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}");
 
}

?>
<html>
<head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../cte/includes/par_subacao.css"/>

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
	<script type="text/javascript"> 
	
		function salvarDados(){
			var arIdSbaid 	= document.getElementsByName('subacoes[]');
			var TotalIdSbaid = arIdSbaid.length;
			var sucesso = false;
			var todasSubs = new Array();
			if(TotalIdSbaid > 0){
				for( x=0; x<TotalIdSbaid; x++ ){
					todasSubs[x] = arIdSbaid[x].value;
					if(arIdSbaid[x].checked){
						sucesso = true;
					}
				}
				$('todassub').value = todasSubs;
			}
			if(sucesso){
				document.formulario.salvar.value = true;
				document.formulario.submit();
			}else{
				alert('Selecione pelo menos uma subação.');
				return false;
			}
		}

		function filtrar(){
			$('filtro').value  = true;
			document.busca.submit();
		}
		
	</script>
	</head>
	<body>
		<form name="busca" id="busca" action="" method="post">
			<input type=hidden id="filtro" name="filtro" value="false" />
    		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
    		 	<tr style="background-color: #cccccc;">
        			<td colspan="2">
        				<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Vinculação de subações entre Par Indígena e Par Genérico</div>
        			</td>
        		</tr>
        		<tr>
        			<td colspan="2" class="SubTituloEsquerda">
        			Filtros para lista de subação:
        			</td>
        		</tr>
        		<tr>
        			<td class="SubtituloEsquerda" >
        			Descrição da subação:
        			</td>
        			<td>
        				<?= campo_texto( 'descricaosub', 'N', 'S', '', 50, 200, '', '' ); ?>
        			</td>
        		</tr>
        		<tr>
        			<td colspan="2" class="SubTituloDireita">
        			<input type="button" name="filtar" value="buscar"  onclick="filtrar();" />
        			</td>
        		</tr>
        		</table>
        		</form>
        		<form name="formulario" id="formulario" action="" method="post">
        		<input type=hidden name="salvar" value="false" />
				<input type=hidden name="todassub" id="todassub" value="" />
        		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
        		<tr>
        			<td colspan="2" class="SubTituloCentro">
        			Selecione as subações referentes ao Programa de Formação Diferenciado, Intercultural e Bilíngue de Professores Indígenas 
        			</td>
        		</tr>
        		<tr>
        			<td colspan="2">
        			<?php 
        			if(!$temConvenio){
						echo "<span style=\"color:red;\" >Não existe convênio cadastrado no módulo 'PAR - Plano de Metas'.</br> Verifique com os gestores dos sistemas.</span>";
					}else{
					    $obMontaListaAjax = new MontaListaAjax($db, true);
        				$obMontaListaAjax->montaLista($sqlSubacoes, $cabecalho,1000, 10, 'S', '', '', '', '', '', '', '' );  
					}
        			
        			?>
        			
        			</td>
        		</tr>
        		<tr>
        			<td colspan="2" class="SubTituloDireita">
        				<input type="button" name="salvar" value="Vincular subações"  onclick="salvarDados();" />
        			</td>
        		</tr>
    		</table>
    	</form>
    </body>
</html>
