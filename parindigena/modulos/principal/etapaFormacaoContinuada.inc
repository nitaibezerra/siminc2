<?php 
$estuf     = $_REQUEST['estuf'];
$covcod    = $_REQUEST['covcod'];
$grpid     = 2;
 
$somenteLeitura = 'S';

if ($_REQUEST['frcid']){
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

	$sql = "	SELECT apmp.sbaid,  coalesce(pss.pssano,0) as pssano
				FROM parindigena.associativaplanodemetasprogramas apmp
				INNER JOIN cte.projetosapesubacao pss on apmp.sbaid = pss.sbaid 
				WHERE appid = ".$_REQUEST['frcid']." and apptipo = 2 ";
	
	$sub = $db->carregar( $sql );

	if($sub){
		
		$valoreTotalExecutado = 0;
		
		foreach($sub as $dados){
			
			$sbaids[] 	= $dados['sbaid']; 
			$pssanos[] 	= $dados['pssano'];
			
			$sql = "select sum(coalesce(hic.hmsvalortotalpago,0)) as total 
			from cte.historicomonitoramentoconvenio  hmc
			inner join  cte.historicomonitoramentoconvsubac hms on hmc.hmcid = hms.hmcid  
			inner join cte.historicoconvitemcomposicao 	hic on   hic.hmsid = hms.hmsid
			where hms.sbaid = ".$dados['sbaid']." and hic.hmsano = ".$dados['pssano'];
			
			$valoreTotalExecutado += $db->pegaUm( $sql );
		}
	}
    
}



function deletaPovo( $frcid ){
     global $db;
    
    $fceid = $_REQUEST['alterar'];
    
    $sql = "DELETE FROM parindigena.povoformacaocontinuadaetapa WHERE fceid = '$fceid'";
    if ( !$db->executar($sql))
    {
       return false;
    }

}

function deletaLocalPolo( $fceid ){
     global $db;
    
    $fceid = $_REQUEST['alterar'];
    
    $sql = "DELETE FROM parindigena.etapapolo WHERE etpidetapa = '$fceid'";
    if ( !$db->executar($sql))
    {
       return false;
    }

}

function deletaLingua( $fceid ){
    global $db;
    
    $fceid = $_REQUEST['alterar'];
    $sql = "DELETE FROM parindigena.linguaformacaocontinuadaetapa WHERE fceid = '$fceid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}    
    
function deletaArranjo( $frcid ){
    global $db;
    
    $frcid = $_REQUEST['frcid'];
    $sql = "DELETE FROM parindigena.territorioformacaocontinuada WHERE frcid = '$frcid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}

function inserePovo( $fceid, $povide){
    global $db;

    if ($_REQUEST['alterar'] != '' )
    {
        $fceid = $_REQUEST['alterar'];
    }
    $sql = "INSERT INTO 
                parindigena.povoformacaocontinuadaetapa
                                               (
                                                    povid,
                                                    fceid
                                                )
                VALUES
                      (
                          '$povide',
                          '$fceid'
                      )";
                      
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}

function insereLingua( $fceid, $linide ){    
    global $db;
    
    if ($_REQUEST['alterar'] != '' )
    {
        $fceid = $_REQUEST['alterar'];
    }
    $sql = "INSERT INTO 
                parindigena.linguaformacaocontinuadaetapa
                       (
                            linid,
                            fceid
                            )
                VALUES
                      (
                          '$linide',
                          '$fceid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}

function insereLocalPolo( $fceid, $locid, $etapaTipo ){    
    global $db;
    
    if ($_REQUEST['alterar'] != '' )
    {
        $fceid = $_REQUEST['alterar'];
    }
    $sql = "INSERT INTO 
                parindigena.etapapolo
                       (
                            etpidetapa,
                            locid,
                            etptipo
                            )
                VALUES
                      (
                          '$fceid',
                          '$locid',
                          '$etapaTipo'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}
    
function insereArranjo( $frcid, $teride ){    
     global $db;
   
    if ($_REQUEST['frcid'] != '' )
    {
        $frcid = $_REQUEST['frcid'];
    }
    $sql = "INSERT INTO 
                parindigena.territorioformacaocontinuada
                                               (
                                                    terid,
                                                    frcid
                                                )
                VALUES
                      (
                          '$teride',
                          '$frcid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
           
}
$number_REQUEST = count( $_REQUEST );

if (  array_key_exists('btnGravar', $_REQUEST ) ||  ( $number_REQUEST > 10 ) ){    
	
/*	echo'<pre>';
	print_r( $_REQUEST );
	echo'</pre>';
	die();*/
    $EarrDataI = explode( "/", $_REQUEST['frcdatainicial']);
    $EformataDataI = $EarrDataI[2].'-'.$EarrDataI[1].'-'.$EarrDataI[0];
    
    $EarrDataF = explode( "/", $_REQUEST['frcdatafim']);
    $EformataDataF = $EarrDataF[2].'-'.$EarrDataF[1].'-'.$EarrDataF[0];
    

         
     if ( !$_REQUEST['alterar'] ){
        if ($_REQUEST['frcid'] != '' ){
            $frcid = $_REQUEST['frcid'];
        }
        if ($EformataDataI != '--' &&  $EformataDataF != '--'){        
            
        
            $sql = " INSERT INTO
                        parindigena.formacaocontinuadaetapa (
                                frcdescricao,
                        		frcdatainicial,
                                frcdatafim,    
                                frcid,
                                frccustolicitado,
                                frccontrapartidaestado,
                                frcqtdeprofessor,
                                frcqtdeescola,
                                frcqtdealuno)
                     VALUES ( 
                             '{$_REQUEST['frcdescricao']}',       
                             '{$EformataDataI}',       
                             '{$EformataDataF}',
                              '$frcid',
                             '" . str_replace('.','', $_REQUEST['frccustolicitado']). "',
                             '" . str_replace('.','', $_REQUEST['frccontrapartidaestado']). "',
                             999,999,999)
                             
                     RETURNING fceid   
                         ";

            $fceid = $db->pegaUm($sql);             
            $db->commit();                     
            $numPovo = count($_REQUEST['povid']);    
            if ($numPovo > 0){                        
                for ( $i = 0; $i < $numPovo; $i++ ){    
                    if($_REQUEST['povid'][$i] != ''){                  
                       $povide = $_REQUEST['povid'][$i];
                       inserePovo( $fceid, $povide );
                    }  
                }            
            } 
            $numLingua = count($_REQUEST['linid']);
            if ( $numLingua > 0 ){                        
               for ( $i = 0; $i < $numLingua; $i++ ){   
                    if($_REQUEST['linid'][$i] != ''){                  
                       $linide = $_REQUEST['linid'][$i];
                       insereLingua( $fceid, $linide );
                   }   
               }
            }
            $numLocalPolo = count($_REQUEST['locid']);
            $etapaTipo = 2;
            if ( $numLocalPolo > 0 ){                        
                for ( $i = 0; $i < $numLocalPolo; $i++ ){   
                    if($_REQUEST['locid'][$i] != ''){                  
                       $locide = $_REQUEST['locid'][$i];
                       insereLocalPolo( $fceid, $locide, $etapaTipo );
                    }   
                }
            }
        } 
    }
    else{                               
        if (  $EformataDataI != '--' &&  $EformataDataF != '--' ){
                                        
                $sql = "UPDATE 
                            parindigena.formacaocontinuadaetapa 
                        SET
                                frcdescricao           = '{$_REQUEST['frcdescricao']}',
                                frcdatainicial         = '{$EformataDataI}',
                                frcdatafim             = '{$EformataDataF}', 
                                frccustolicitado       = '" . str_replace('R$','', $_REQUEST['frccustolicitado']). "',
                                frccontrapartidaestado = '" . str_replace('R$','', $_REQUEST['frccontrapartidaestado']). "'
                        WHERE
                            fceid     =  '{$_REQUEST['alterar']}' "; 
                            
                            $db->executar($sql);
          
            deletaPovo( $fceid );
            $numPovo = count($_REQUEST['povid']);                
            if ($numPovo > 0){                        
                for ( $i = 0; $i < $numPovo; $i++ ){    
                    if($_REQUEST['povid'][$i] != ''){                  
                       $povide = $_REQUEST['povid'][$i];
                       inserePovo( $fceid, $povide );
                    }  
                }            
            }             
            deletaLingua( $fceid ); 
            $numLingua = count($_REQUEST['linid']);
            if ( $numLingua > 0 ){                        
                for ( $i = 0; $i < $numLingua; $i++ ){   
                    if($_REQUEST['linid'][$i] != ''){                  
                       $linide = $_REQUEST['linid'][$i];
                       insereLingua( $fceid, $linide );
                    }   
                }
            }
        	deletaLocalPolo( $fceid ); 
            $numLocalPolo = count($_REQUEST['locid']);
            $etapaTipo = 2;
            if ( $numLocalPolo > 0 ){                        
                for ( $i = 0; $i < $numLocalPolo; $i++ ){   
                    if($_REQUEST['locid'][$i] != ''){                  
                       $locide = $_REQUEST['locid'][$i];
                       insereLocalPolo( $fceid, $locide, $etapaTipo );
                    }   
                }
            }
             
        }
        else{   
            echo("<script>alert('voce deve preencher todos os dados da etapa');\n </script>");
        }
    }    
    $db->commit(); 
    $dadosSalvos = true;
    if ( array_key_Exists('btnAddEtapa', $_REQUEST ) ){        
       $url = $_SERVER['argv'][0];
       echo ("<script type=\"text/javascript\"> window.location.href = \"parindigena.php?$url&frcid=$frcid\";\n</script>");        
    }

}

function carregaComponentes(){
	global $db;
    $sql = "SELECT * FROM parindigena.formacaocontinuadaetapa WHERE fceid = '{$_REQUEST['alterar']}'";

    return $db->carregar($sql); 
}    

if ( $_REQUEST['alterar'] ){
    $rsDadosEtapa = carregaComponentes();
} 


if (array_key_exists('excluir', $_REQUEST)){
    
	    $sql = "SELECT 
	                pf.fceid
	            from
	                parindigena.formacaocontinuadaetapa AS pf
	            LEFT JOIN
	                parindigena.povoformacaocontinuadaetapa AS pc ON pc.fceid = pf.fceid
	            LEFT JOIN 
	                parindigena.linguaformacaocontinuadaetapa AS lc ON lc.fceid = pf.fceid
	            WHERE 
	            	pf.fceid = '{$_REQUEST['excluir']}'";
    
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
        
         if ($num > 1 ){                         
             echo("<script>alert('Nao é possivel a exclusao desta Etapa, pois a mesma possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else{  
            $sql = "DELETE FROM parindigena.formacaocontinuadaetapa WHERE fceid = {$_REQUEST['excluir']}";
         
            $deleteItemForm = $db->executar( $sql );
            $db->commit();
            header('Location: parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid='.$_REQUEST['frcid'].'');
         }  
         if ($var == 1){              
             echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid='.$_REQUEST['frcid'].'";</script>';
         }
}   
$res = array( 0 =>
					 array (  "descricao" => "Formação Continuada",
						    "id" 		=> "3",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid={$rsDadosForm[0]['frcid']}"
				  		  )
			);	  
				
if ($_REQUEST['frcid']){
	$perfis = arrayPerfil(); 		  
	if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){	
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "5",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&frcid={$rsDadosForm[0]['frcid']}"
					  		   ),
		  		   array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
		  		   			   ),
				   array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}else{
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "5",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&frcid={$rsDadosForm[0]['frcid']}"
					  		   ),
				   array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}
	
}

/////////////

$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
$rs = $db->carregar( $sql );
$convenioAno = $rs[0]['covnumero'];
$dadosConvenio = explode('/', $convenioAno);
$numConvenio = $dadosConvenio[0];
$ano = $dadosConvenio[1];
$_REQUEST['covcod'] = $numConvenio;

$sql = "SELECT	 
			si.sbaid,
			si.sbaporescola
		FROM parindigena.associativaplanodemetasprogramas app
		inner join cte.subacaoindicador 	si ON si.sbaid = app.sbaid
		WHERE app.appid = '{$_REQUEST['frcid']}'";

$arSubacoes = $db->carregar($sql);

if(is_array($arSubacoes)){

	$totalValorProgramado = 0;

	foreach($arSubacoes as $dados){
		$arSbaids[] = $dados['sbaid'];
		$arValores = calculoValorProgramado($dados['sbaid'], $ano, $dados['sbaporescola']);
		$totalValorProgramado += $arValores['cronograma']; 	
	}	
}

$totalValorProgramado = $totalValorProgramado ? $totalValorProgramado : 0;

///////////////

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid={$rsDadosForm[0]['frcid']}");
 
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
	<script src="../includes/calendario.js"></script>
    
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

	<style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER {
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
<form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" >
	<table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr style="background-color: #cccccc;">
			<td>
				<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Formação Continuada</div>
				<div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
					<table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">           
			            <tr>
							<td colspan = "2" align="center">
								<table width = "100%" class="tabela listagem">
									<tr>
										<td>
											<span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span>
										</td>
									</tr>
									<tr>
										<td>                                
											<span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Formação Continuada</span>
										</td>
									</tr>                        
			                        <?php                       
			                        if ( $_REQUEST['covcod'] != '' )
			                        {                             
				                        $sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$_REQUEST['covcod']}'";
				                        $rs = $db->carregar( $sql );
				                        $convenio = $rs[0]['covnumero']; 
									?>
		                            <tr>
		                                <td>
		                                <span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)</span>
		                                </td>
		                            </tr>
									<?php
			                        }
			                        if ( $_REQUEST['frcid'] != '' )
			                        {				                             
				                        $sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
				                        $rs = $db->carregar( $sql );
				                        $convenio = $rs[0]['covnumero'];				                        
									?>
									<tr>
										<td>
											<span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)</span>
										</td>
									</tr>
		                            <tr>
		                                <td>
		                                <span style="display: block;font-size:100%;color:black; font-weight:bold;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?php  echo $rsDadosForm[0]["itmnome"]; ?></span>
		                                </td>
		                            </tr>
									<?php
			                        }
			                        ?>
								</table>
							</td>
						</tr>
			            <tr>
			                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Etapas do Curso</div></td>
			            </tr>          	
						<tr>
		                	<td class="subtitulodireita" > Data de início: </td>
		                	<td align="left">
			                	<?php  $frcdatainicial = $rsDadosEtapa[0]['frcdatainicial']; ?>
			                	<?= campo_data( 'frcdatainicial','S', $somenteLeitura,  '', 'S' ); ?>
		                	</td>
						</tr>
						<tr>
			                <td class="subtitulodireita"> Data de Término: </td>
			                <td align="left">
				                <?php  $frcdatafim = $rsDadosEtapa[0]['frcdatafim']; ?>
				                <?= campo_data( 'frcdatafim', 'S', $somenteLeitura, '', 'S' ); ?>
			                </td>
						</tr>
			            <tr>
			                <td class="subtitulodireita"> Descrição: </td>
			                <td align="left">
				                <?php $frcdescricao = $rsDadosEtapa[0]["frcdescricao"]; ?>
				                <?= campo_texto('frcdescricao', 'N', $somenteLeitura, '', 30, 100, '', '', 'left', '',  0, 'id="frcdescricao" onblur="MouseBlur(this);"' ); ?>
			                </td>
          				</tr>               
			            <tr>
			                <td class="subtitulodireita"> Custo da Etapa R$: </td>
			                <td align="left">
				                <?php $frccustolicitado = $rsDadosEtapa[0]["frccustolicitado"]; ?>
				                <?= campo_texto('frccustolicitado', 'N', $somenteLeitura, '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="frccustolicitado" onblur="MouseBlur(this);"' ); ?>
			                </td>
          				</tr>               
						<tr>
			                <td class="subtitulodireita"> Contrapartida do Estado R$: </td>
			                <td align="left">  
				                <?php $frccontrapartidaestado = $rsDadosEtapa[0]["frccontrapartidaestado"]; ?>
				                <?= campo_texto('frccontrapartidaestado', 'N', $somenteLeitura, '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="frccontrapartidaestado" onblur="MouseBlur(this);"' ); ?>
                			</td>
             			</tr>
            			<tr>
			                <td class="subtitulodireita">Povos Indígenas</td>
			                <td align="left">
				                <?php
				                $sql = "SELECT 
				                           povid as codigo, 
				                           povnome as descricao
				                        FROM 
				                            parindigena.povoindigena";
				                
				                            $sqlPovid = "SELECT 
				                                           pf.povid as codigo,
				                                           p.povnome as descricao
				                                         FROM 
				                                            parindigena.povoformacaocontinuadaetapa AS pf
				                                         INNER JOIN parindigena.povoindigena p ON pf.povid = p.povid
				                                         WHERE pf.fceid = '{$_REQUEST['alterar']}'";
				                            if( $_REQUEST['alterar'] != '' )
				                            {
				                                $povid    = $db->carregar($sqlPovid);
				                            }
				                combo_popup( "povid", $sql, "Selecione o(s) Povo(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
				                ?>
	               				<img src="../imagens/obrig.gif">
                			</td>
            			</tr>
            			<tr>
			                <td class="subtitulodireita"> Línguas:</td>
			                <td align="left">
			                <?php
			               $sql = "SELECT 
			                           linid as codigo, 
			                           linnome as descricao
			                        FROM 
			                            parindigena.linguaindigena";
			
			                $sqlLinid = "SELECT 
			                                  lf.linid as codigo,
			                                  l.linnome as descricao
			                             FROM 
			                                  parindigena.linguaformacaocontinuadaetapa AS lf
			                             INNER JOIN parindigena.linguaindigena l ON lf.linid = l.linid
			                             WHERE 
			                                  lf.fceid = '{$_REQUEST['alterar']}'";
			                            if( $_REQUEST['alterar'] != '' ){
			                                $linid    = $db->carregar($sqlLinid);  
			                            }
			
			                combo_popup( "linid", $sql, "Selecione a(s) Língua(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
			                ?>
			                <img src="../imagens/obrig.gif">
			                </td>
			            </tr>
			            <tr>
			                <td class="subtitulodireita">Local / Pólo</td>
			                <td align="left">
				                <?php
				                $sql = "SELECT 
											locid as codigo,
											UPPER(locnome) as descricao
										FROM parindigena.localpolo ";

				                $sqlLocid = "SELECT 
	                                           elp.locid as codigo,
	                                           lp.locnome as descricao
	                                         FROM 
	                                            parindigena.etapapolo AS elp
	                                         INNER JOIN parindigena.localpolo lp ON elp.locid = lp.locid
	                                         WHERE elp.etpidetapa = '{$_REQUEST['alterar']}'";
	                                         
				                            if( $_REQUEST['alterar'] != '' )
				                            {
				                                $locid    = $db->carregar($sqlLocid);				                             
				                            }
							                    
								echo combo_popup( 'locid' , $sql, "Selecione o(s) Local(is) / Pólo(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
				                ?>
	               				<img src="../imagens/obrig.gif">
                			</td>
            			</tr>
			            
			            
			            <tr style ="background-color: #cccccc;">
			                <td  class = "SubtituloEsquerda" colspan=2 >
				                <?php
				                if ( $_GET['alterar'] != ''){
				                    $disabled = "enabled";
				                    $name     = "Gravar Etapa";
				                }
				                else{
				                	if( $_REQUEST['frcid'] != ''){
				                		$disabled = "enabled";
				                   		$name     = "Adicionar Etapa";
				                	}else{
				                		$disabled = "enabled"; 
				                   		$name     = "Gravar Etapa";
				                	}
				                }
				                ?>
			                
				                <?php
								  $perfis = arrayPerfil(); 
								  if( !in_array( PARIND_SUPER_USUARIO, $perfis ) &&
								  	  !in_array( PARIND_ADMINISTRADOR, $perfis ) &&
								  	  !in_array( PARIND_COORDENACAO, $perfis) &&
								  	  !in_array( PARIND_EQUIPE_TECNICA, $perfis) ){
	
								  }else{	
								?>				          
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btnAddEtapa" value="<?=$name; ?>" <?php echo $disabled; ?> onclick="validaEtapa();"/>
					            <input type="button" name="btnAddNew" value="[+] Nova Etapa" <?php echo $disabled; ?> onclick="newEtapa();"/>			               		 
			               		<? } ?>
		                	</td>
		                </tr>
		                
			            <tr>
			                <td colspan="2">
					            <table style="width: 100%;" class = "tabela_listagem">
					            <tr>
					            	<td style="width: 100%;">
					                <?php
					                $sql = "SELECT
					                            '<img onclick=\"return alterarEtapa('|| fceid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirEtapa('|| fceid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acoes,
					                             frcdescricao as descricao,
					                             to_char(frcdatainicial::date,'DD/MM/YYYY') as dataini,
					                             to_char(frcdatafim::date,'DD/MM/YYYY') as datafim,
					                             frccustolicitado,
					                             frccontrapartidaestado     
					                        FROM 
					                             parindigena.formacaocontinuadaetapa
					                        WHERE
					                        frcid = '{$_REQUEST['frcid']}'
					                             ";  
					                
					                $cabecalho = array( "Editar / Excluir", "Descricao","Data de Início", "Data de Término", "Custo Licitado(R$)", "Contrapartida do Estado(R$)" );
					
					                if( $_REQUEST['frcid'] != '' ){		                    
					                    $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '' );  		                
					                }
					                ?>
					                </td>
					                </tr>
					           </table>
							</td>
						</tr>
						<?php 
						
						if($sbaids){
							
							$sql = "select distinct
										app.sbaid,
										count(*) as total
									from parindigena.associativaplanodemetasprogramas app
									inner join cte.subacaoindicador si on si.sbaid = app.sbaid
									inner join cte.acaoindicador ai on ai.aciid = si.aciid
									inner join cte.pontuacao pt on pt.ptoid = ai.ptoid
									inner join cte.instrumentounidade iu on pt.inuid = iu.inuid
									where app.apptipo = '2'
									and iu.itrid = '1'
									and iu.estuf = '{$estuf}'									
									group by app.sbaid";
									
							$nrSubacoes = $db->carregar($sql);
							
							foreach($nrSubacoes as $dados){
								if($dados['total'] > 1){
									foreach($sbaids as $sbaid){
										if($sbaid == $dados['sbaid']){
											$mostraAlerta = 'true';
										}
									}
								}
							}
						}
						
						$mostraAlerta = $mostraAlerta ? $mostraAlerta : false;
						
						if($mostraAlerta){ 
						?>
						<tr>
							<td  colspan="2" class="SubTituloDireita" >
							<span style="color:red;">
							<b>
							Os valores apresentados correspondem ao somatório de todas as subações do Material Didático.
							</b>
							</span>
							</td>										
						</tr>
						<?php } ?>
						<tr>
							<td  class="SubTituloDireita" >Valor total programado:</td>
							<td class="SubTituloEsquerda">R$ <?=number_format(str_replace(',','',$totalValorProgramado),2,',','.');?></td>
						</tr>
						<tr>
							<td  class="SubTituloDireita" >Valor total executado:</td>
							<td class="SubTituloEsquerda">R$ <?=number_format(str_replace(',','',$valoreTotalExecutado),2,',','.');?> </td>
						</tr>  
			            <tr>
							<?php
							$perfis = arrayPerfil(); 
							if( !in_array( PARIND_SUPER_USUARIO, $perfis ) &&
								!in_array( PARIND_ADMINISTRADOR, $perfis ) &&
								!in_array( PARIND_COORDENACAO, $perfis) &&
								!in_array( PARIND_EQUIPE_TECNICA, $perfis) ){
							?>
							<tr style="background-color: #cccccc;">
								<td colspan="2" class="SubTituloEsquerda"  align="left"></td>
							</tr> 
							<?	
							} else {
							?>
							<?
							}	  	  
							?>
									
									
						
					</table>
				</div>				
   		</table>
	</form>   		
</body>
<script type="text/javascript">
function processaCombosPopupEtapa(){ 
	selectAllOptions( document.getElementById( 'povid' ) );
	selectAllOptions( document.getElementById( 'linid' ) );
	selectAllOptions( document.getElementById( 'locid' ) );
	   
	var numPovo   = document.getElementById( 'povid' ).value;
	var numLingua = document.getElementById( 'linid' ).value;
	  
	if( numPovo == '' ){
		alert('Você deve selecionar no mínimo um Povo Indígena.');
		return false;
	}
	if( numLingua == '' ){
		alert('Você deve selecionar no mínimo uma Lingua Indígena.');
		return false;
	} 
	return true;
} 
function validaEtapa(){ 

    var frcdescricao            = formulario.frcdescricao.value.length;
    var frcdatainicial          = formulario.frcdatainicial.value.length;
    var frcdatafim              = formulario.frcdatafim.value.length;
    var frccustolicitado        = formulario.frccustolicitado.value.length;
    var frccontrapartidaestado  = formulario.frccontrapartidaestado.value.length;
    var numCampodataEI          = formulario.frcdatainicial.value.length;
    var numCampodataEF          = formulario.frcdatafim.value.length;
    
    var msg = 'Para adicionar uma etapa é necessário preencher todos os campos.';
     
    if(frcdatainicial == 0)
    {
        alert(msg);
 
        return false;
    }
    if(frcdatafim == 0)
    {
        alert(msg);
 
        return false;
    }
    if(frccustolicitado == 0){
        alert(msg);
 
        return false;
    }
    if(frccontrapartidaestado == 0){
        alert(msg);
 
        return false;
    }
    var dataei    = formulario.frcdatainicial.value;
    var arrdataei = dataei.split( "/" );
    var diaei     = parseFloat(arrdataei[0]);
    var mesei     = parseFloat(arrdataei[1]);
    var anoei     = parseFloat(arrdataei[2]);
    
    if( (diaei > 31) || (mesei > 12) ){
        alert('data inesistente');
        formulario.frcdatainicial.focus();
        return false;
    }
    
    var dataef    = formulario.frcdatafim.value;
    var arrdataef = dataef.split( "/" );
    var diaef     = parseFloat(arrdataef[0]);
    var mesef     = parseFloat(arrdataef[1]);
    var anoef     = parseFloat(arrdataef[2]);
    
    if( (diaef > 31) || (mesef > 12) ){
        alert('data inesistente');
        formulario.frcdatafim.focus();
        return false;
    } 
    if(anoei > anoef){
        alert('A data inicial da etapa não pode ser maior do que a data final.');
        formulario.frcdatainicial.focus();
        return false;
    }
    else if (anoei == anoef){
        if(mesei > mesef){
            alert('A data inicial da etapa não pode ser maior do que a data final.');
            formulario.frcdatainicial.focus();
            return false;
        }
        else if (mesei == mesef){
            if(diaei > diaef){
                alert('A data inicial da etapa não pode ser maior do que a data final.');
                formulario.frcdatainicial.focus();
                return false;
            }
        }
    }  
    if( !processaCombosPopupEtapa() ){  
    	return false;
    }
    formulario.submit();
    return true;
}
  
function fecharPopup(){
    if( !confirm('Deseja realmente fechar esta janela? Os dados nao salvos até o momento serão perdidos.') )
    {
        return false;
    }
    return window.close(); 
}
 
function alterarEtapa( fceid ){    
    return window.location.href = 'parindigena.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&alterar='+fceid;
}

function adicionarEtapa(){
    return window.location.href = 'parindigena.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&addEtapa=s';
}
 
function excluirEtapa(fceid){
    if ( !confirm('Deseja realmente excluir esta etapa?') )
    {
        return false;
    }
    return window.location.href = 'parindigena.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&excluir='+fceid;
} 
function newEtapa(){
	return window.location.href='parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid=<?=$_REQUEST['frcid'];?>&estuf=<?=$estuf;?>';
}	
</script>
</html>