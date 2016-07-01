<?php 
$estuf     = $_REQUEST['estuf'];
$covcod    = $_REQUEST['covcod'];
$grpid     = 2;
 
$somenteLeitura = 'S';

if($_REQUEST['trSubacaoes']){
	
	$sql = "SELECT
				si.sbaid as codigo,
				si.sbadsc as descricao				
			FROM parindigena.convenioindigena 						AS ci
			INNER JOIN financeiro.convenio 							AS cf ON ci.covcod = cf.covcod
			INNER JOIN cte.projetosape 								AS ps ON cf.covprocesso::numeric = ps.prsnumeroprocesso
			INNER JOIN cte.instrumentounidade 						AS iu ON iu.inuid = ps.inuid
			INNER JOIN parindigena.associativaplanodemetasformacao 	AS apf ON apf.prsid = ps.prsid
			INNER JOIN cte.subacaoindicador 						AS si ON si.sbaid = apf.sbaid 
			WHERE ci.coistatus = 'A'
			and cf.estuf  = '{$_REQUEST['estuf']}' 
			and iu.estuf  = '{$_REQUEST['estuf']}'
			and cf.covcod = '{$_REQUEST['covcod']}'";
			
	$db->monta_combo('sbaid',$sql,'S','Selecione uma subação...','','','','400','S','sbaid','','','');
	die();
	
}
 
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
	
    $EarrDataI = explode( "/", $_REQUEST['frcdatainicial']);
    $EformataDataI = $EarrDataI[2].'-'.$EarrDataI[1].'-'.$EarrDataI[0];
    
    $EarrDataF = explode( "/", $_REQUEST['frcdatafim']);
    $EformataDataF = $EarrDataF[2].'-'.$EarrDataF[1].'-'.$EarrDataF[0];
    
    $frcnome             = $_REQUEST['frcnome'];
    $frccargahoraria     = $_REQUEST['frccargahoraria'];
    $locid               = $_REQUEST['locid_disable'] ? $_REQUEST['locid_disable'] : 'NULL';
    $frcparceria         = $_REQUEST['frcparceria'];
    $qtdprofessores      = $_REQUEST['qtdprofessores'] ? $_REQUEST['qtdprofessores'] : 'NULL';
    $escolasatend        = $_REQUEST['escolasatend'] ? $_REQUEST['escolasatend'] : 'NULL';
    $alunosbenef         = $_REQUEST['alunosbenef'] ? $_REQUEST['alunosbenef'] : 'NULL'; 
	
    if ( !$_REQUEST['frcid'] ){    
        if ($EformataDataI != '--' &&  $EformataDataF != '--'  && $_REQUEST['frcqtdeprofessor'] != '' && $_REQUEST['frcqtdeescola'] != '' && $_REQUEST['frcqtdealuno'] != '' ){ 
         
                 //INSERT
                 $sql = "INSERT INTO 
                                parindigena.formacaocontinuada ( 
					 	 						 locid,
                                                 frccargahoraria,                                                  
                                                 frcparceria
                                                 )
                               VALUES (                                                              
                                       '$locid',
                                       '$frccargahoraria',
                                       '$frcparceria'
                                       ) 
                                       returning frcid";                            
                   
                   if ( $frcid = $db->pegaUm( $sql ) ){  
                       $db->commit();
     
                       
                        $numArranjo = count($_REQUEST['terid']);
                        if ($numArranjo > 0 ){                        
                           for ( $i = 0; $i < $numArranjo; $i++ ){    
                               if($_REQUEST['terid'][$i] != ''){
                                   $teride = $_REQUEST['terid'][$i];
                                   insereArranjo( $frcid, $teride );
                               }               
                           }
                        }
    
                        $arrDatamI = explode( "/", $_REQUEST['itmdatainicio']);
                        $formataDatamI = $arrDatamI[2].'-'.$arrDatamI[1].'-'.$arrDatamI[0];
                        
                        $arrDatamF = explode( "/", $_REQUEST['itmdatafim']);
                        $formataDatamF = $arrDatamF[2].'-'.$arrDatamF[1].'-'.$arrDatamF[0];        
                        
                        $covcod = $_REQUEST['covcod'];
                        $grpid = 2;
                        $estuf = $_REQUEST['estuf'];
                        
                        if ( !$_GET['covcod'] ){
                            $covcod = $_POST['covcod_disable'];
                        }
                        else{
                            $covcod = $_GET['covcod'];
                        }                 
                        $sql = "INSERT INTO
                                    parindigena.itemmonitoramento (
                                        itmnome,
                                        itmdatainicio,  
                                        itmdatafim,     
                                        itmstatus,      
                                        usucpf,         
                                        covcod ,        
                                        grpid,          
                                        frcid,          
                                        estuf,
                                        qtdprofessores,
                                        escolasatend,
                                        alunosbenef            
                                        )
                                VALUES
                                    (
                                        '{$_REQUEST['itmnome']}',
                                        '{$formataDatamI}',
                                        '{$formataDatamF}',
                                        'A',
                                        '{$_SESSION['usucpf']}',
                                        '$covcod',
                                        '$grpid',
                                        '$frcid',
                                        '$estuf',
                                        '$qtdprofessores',
                                        '$escolasatend',
                                        '$alunosbenef'                               
                                    )";
                        $db->executar($sql);
                        $db->commit();
                   }
            }
         } 
         else{
             $arrDatamI = explode( "/", $_REQUEST['itmdatainicio']);
             $formataDatamI = $arrDatamI[2].'-'.$arrDatamI[1].'-'.$arrDatamI[0];
             
             $arrDatamF = explode( "/", $_REQUEST['itmdatafim']);
             $formataDatamF = $arrDatamF[2].'-'.$arrDatamF[1].'-'.$arrDatamF[0];      
             
             $sql = " UPDATE parindigena.formacaocontinuada SET
                            frccargahoraria   = '$frccargahoraria', 
                            locid             = $locid,
                            frcparceria       = '$frcparceria'
                      WHERE                       
                            frcid             =  '{$_REQUEST['frcid']}' "; 

             $db->executar( $sql );   
              
              
             $sql = " UPDATE parindigena.itemmonitoramento SET
                            itmnome        =   '{$_REQUEST['itmnome']}', 
                            itmdatainicio  =   '{$formataDatamI}',
                            itmdatafim     =   '{$formataDatamF}',
                            qtdprofessores =   ".$qtdprofessores.",
                            escolasatend   =   ".$escolasatend.",
                            alunosbenef    =   ".$alunosbenef."
                      WHERE                       
                            frcid             =  '{$_REQUEST['frcid']}' "; 

             $db->executar( $sql );   
             
             deletaArranjo( $frcid );
             $numArranjo = count($_REQUEST['terid']);
            
             if ($numArranjo > 0 )
             {                        
                for ( $i = 0; $i < $numArranjo; $i++ )
                {    
                    if($_REQUEST['terid'][$i] != '')
                    {
                        $teride = $_REQUEST['terid'][$i];
                        insereArranjo( $frcid, $teride );
                    }               
                }
             }
             $db->commit();               
          }
         
  
    $db->commit(); 
    $dadosSalvos = true;
    if ( array_key_Exists('btnAddEtapa', $_REQUEST ) ){        
       $url = $_SERVER['argv'][0];
       echo ("<script type=\"text/javascript\"> window.location.href = \"parindigena.php?$url&frcid=$frcid\";\n</script>");        
    }
    elseif( array_key_Exists('btnGravar', $_REQUEST ) ){
        echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';  
    }
}
  
$res = array( 0 => array (  "descricao" => "Formação Continuada",
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

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid={$rsDadosForm[0]['frcid']}");
 
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
    <script type="text/javascript">
	    function mostrarSubacao(covcod){
		
			var trSubacoes = $('trSubacoes');
			var tdSubacoes = $('tdSubacoes');		
		
			new Ajax.Request('parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I',
			{
				method: 'post',
				parameters: '&trSubacaoes=true&estuf=<?=$_REQUEST['estuf'];?>&covcod='+covcod,
				onComplete: function(res){
				
					tdSubacoes.innerHTML = res.responseText;
					trSubacoes.style.display = 'table-row';
				
				}
			});
		}
    </script>
</head>
<body> 
<form onSubmit="return validaForm(this);"action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" >
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
			                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Convênio</div></td>
			            </tr>
			            <tr>
			                <td class="subtitulodireita">Escolha o Convênio:</td>
			                <td align="left" >
			                <?php 
			                $sql = "SELECT 
			                            pc.covcod AS codigo, 
			                            fc.covnumero AS descricao
			                        FROM
			                            parindigena.convenioindigena AS pc
			                        INNER JOIN
			                            financeiro.convenio AS fc ON fc.covcod = pc.covcod
			                        WHERE 
			                            fc.estuf = '$estuf'
			                      	 AND 
			                            pc.coitipo = 'F'
			                         ORDER BY
			                        	fc.estuf, fc.covnumero";
			                            
			                if ( $_REQUEST['covcod'] ){
			                    $covcod = $_REQUEST['covcod'];
			                }
			                else{
			                    $covcod = $rsDadosForm[0]["covcod"];
			                }
			                $db->monta_combo('covcod', $sql, $somenteLeituraUnidade, "Selecione...", 'mostrarSubacao', '', '', '390', 'S', 'covcod');
			                ?>			                
			              
			               </td>
			            </tr>
			            <tr id="trSubacoes" style="display:none;">
			                <td class="subtitulodireita">Escolha uma subacao:</td>
			                <td align="left" id="tdSubacoes">
			                              
							</td>
						</tr>
			            <tr>
			                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Identificação do Curso</div></td>
			            </tr>
						<tr>
							<td class="subtitulodireita">Nome do Curso: </td>
							<td align="left" >
								<?php $itmnome = $rsDadosForm[0]["itmnome"]; ?>
								<?= campo_texto('itmnome', 'S', $somenteLeitura, '', 70, 100, '', '', 'left', '',  0, 'id="itmnome" onblur="MouseBlur(this);"' ); ?>
							</td>
						</tr>
			            <tr>
			                <td class="subtitulodireita" > Data de início: </td>
			                <td align="left">
								<?php $itmdatainicio = $rsDadosForm[0]["itmdatainicio"]; ?>
								<?= campo_data( 'itmdatainicio','S', $somenteLeitura, '', 'S' ); ?>
			                </td>
			            </tr>
			            <tr>
			                <td class="subtitulodireita"> Data de Término: </td>
			                <td align="left">
								<?php $itmdatafim = $rsDadosForm[0]["itmdatafim"]; ?>
								<?= campo_data( 'itmdatafim', 'S', $somenteLeitura, '', 'S' ); ?>
			                </td>
			            </tr>
			            <tr>
			                <td class="subtitulodireita" >Carga Horária: </td>
			                <td align="left">
								<?php $frccargahoraria = $rsDadosForm[0]["frccargahoraria"]; ?>
								<?= campo_texto('frccargahoraria', 'S', $somenteLeitura, '', 6, 4, '####', '', 'left', '',  0, 'id="frccargahoraria" onblur="MouseBlur(this);"' ); ?>
			                </td>
			            </tr>
						<tr>
							<td class="subtitulodireita">Local/Pólo:</td>
							<td align="left" >
				                <?php 
				                $sql = "SELECT 
				                		 locid as codigo,
				                         locnome as descricao
				                        FROM
				       					 parindigena.localpolo pl
				       					 INNER JOIN entidade.endereco e ON e.endid = pl.endid AND
				       								  					   e.estuf = '{$_REQUEST['estuf']}'
				       					order by
				       					 locnome";
				       			
				                $locid = $rsDadosForm[0]["locid"];
				                $db->monta_combo('locid', $sql, $somenteLeituraUnidade, "Selecione...", '', '', '', '390', 'S', 'locid');
				                ?>
                 				<span style="cursor:pointer" id="linkInserirItem" onclick="return abrirCadLocalPolo();"><img src="/imagens/gif_inclui.gif" align="middle" style="border: none" /> Cadastrar Local/Pólo</span>
                 			</td>
             			</tr>
			            <tr>
			                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Dados de Atendimento do Cuso</div></td>
			            </tr>
			            <tr>
			                <td class="subtitulodireita"> Quantidade de Professores: </td>
			                <td align="left" > 
			                	<?php $qtdprofessores = $rsDadosForm[0]["qtdprofessores"]; ?>
								<?= campo_texto('qtdprofessores', 'S', $somenteLeitura, '', 6, 100, '', '', 'left', '',  0, 'id="qtdprofessores" onblur="MouseBlur(this);"' ); ?> 
			            </tr>
			            <tr>
			                <td class="subtitulodireita" > Escolas Atendidas: </td>
			                <td  align="left">  
			                	<?php $escolasatend = $rsDadosForm[0]["escolasatend"]; ?>
								<?= campo_texto('escolasatend', 'S', $somenteLeitura, '', 6, 100, '', '', 'left', '',  0, 'id="escolasatend" onblur="MouseBlur(this);"' ); ?> 
			                </td> 
			            </tr>
			            <tr>
			                <td class="subtitulodireita"> Alunos Beneficiados: </td>
			                <td  align="left">  
			                	<?php $alunosbenef = $rsDadosForm[0]["alunosbenef"]; ?>
								<?= campo_texto('alunosbenef', 'S', $somenteLeitura, '', 6, 100, '', '', 'left', '',  0, 'id="alunosbenef" onblur="MouseBlur(this);"' ); ?> 
			                </td> 
			            </tr>
			            <tr>
			                <td class="subtitulodireita">Territórios Etno-educacionais:</td>
			                <td align="left">
			                <?php
			                $sql = "SELECT 
			                           terid as codigo, 
			                           ternome as descricao
			                        FROM 
			                            parindigena.territorioetnoeducacional as territorio";
			               
			                $sqlArrid = "SELECT 
			                                 af.terid as codigo,
			                                 a.ternome as descricao
			                            FROM 
			                                 parindigena.territorioformacaocontinuada AS af
			                            INNER JOIN parindigena.territorioetnoeducacional a ON af.terid = a.terid
			                            WHERE 
			                                 af.frcid = '{$_REQUEST['frcid']}'";
			                if( $_REQUEST['frcid'] != '' ){
			                    $terid    = $db->carregar($sqlArrid);            
			                }
			                combo_popup( "terid", $sql, "Selecione o(s) Território(s) Etno-educaciona(l)(is)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
			                ?>
			                <img src="../imagens/obrig.gif">
			                </td>
			            </tr>
			            <tr>
			                <td class="subtitulodireita">Parcerias</td>
			                <td align="left">			                
			                <TEXTAREA cols="56" rows="3" name="frcparceria" onblur="MouseBlur(this);textCounter(this.form.frcparceria,this.form.no_entobs,500);if(this.value.length>(MAXLEN=500))this.value=this.value.substr(0,MAXLEN)" onkeydown="textCounter(this.form.frcparceria,this.form.no_entobs,500);" onkeyup="textCounter(this.form.frcparceria,this.form.no_entobs,500);"><?php echo $rsDadosForm[0]['frcparceria']; ?></TEXTAREA>
			                <br>
			                <input readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_entobs" size="6" maxlength="6" value="500" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font>
			                </td>
			            </tr>
							<?php
							$perfis = arrayPerfil(); 
							if( !in_array( PARIND_SUPER_USUARIO, $perfis ) &&
								!in_array( PARIND_ADMINISTRADOR, $perfis ) &&
								!in_array( PARIND_COORDENACAO, $perfis) &&
								!in_array( PARIND_EQUIPE_TECNICA, $perfis) ){
							?>
							<tr style="background-color: #cccccc;">
								<td class="SubTituloEsquerda"  align="left"></td>
							</tr> 
							<?	
							} else {
							?>
							<tr style="background-color: #cccccc;">
								<td class="SubTituloEsquerda"  align="left">            
									<input type="submit" name="btnGravar" value="Gravar"/>
									<input type="button" name="btnCancel" value="Fechar" onclick="fecharPopup();"/>
								</td>
							</tr> 	  	  
							<?
							}	  	  
							?>      						
					</table>
				</div>				
   		</table>
	</form>   		
</body>
<script type="text/javascript">
function processaCombosPopup(){ 
	selectAllOptions( document.getElementById( 'terid' ) );
	var numTerri  = document.getElementById( 'terid' ).value; 
	if( numTerri == '' ){
		alert('Você deve selecionar no mínimo um Território.');
		return false;
	} 
	return true;
}
  
function validaForm(){
     var numCampodataI           = formulario.itmdatainicio.value.length;
     var numCampodataF           = formulario.itmdatafim.value.length;

    if(formulario.covcod_disable.value == '')
    {
        alert('Selecione um convênio');
        formulario.covcod_disable.focus();
        return false;
    }

    
   
    if(( numCampodataI > 0 ) && ( numCampodataF > 0)){ 
        var datai = formulario.itmdatainicio.value;
        var arrdatai = datai.split( "/" );
        var diai = parseFloat(arrdatai[0]);
        var mesi = parseFloat(arrdatai[1]);
        var anoi = parseFloat(arrdatai[2]); 
      
        if( (diai > 31) || (mesi > 12) )
        {
            alert('data inesistente');
            formulario.itmdatainicio.focus();
            return false;
        } 
        var dataf = formulario.itmdatafim.value;
        var arrdataf = dataf.split( "/" );
        var diaf = parseFloat(arrdataf[0]);
        var mesf = parseFloat(arrdataf[1]);
        var anof = parseFloat(arrdataf[2]);
    
        if( (diaf > 31) || (mesf > 12) )
        {
            formulario.itmdatainicio.focus();
            alert('data inesistente');
            return false;
        }
        
	    if(anoi > anof){
	        alert('A data inicial não pode ser maior do que a data final.');
            formulario.itmdatainicio.focus();
	        return false;
	    }
        else if (anoi == anof){
	        if(mesi > mesf){
	            alert('A data inicial não pode ser maior do que a data final.');
                formulario.itmdatainicio.focus();
	            return false;
	        }
            else if (mesi == mesf){
	            if(diai > diaf){
	                alert('A data inicial não pode ser maior do que a data final.');
                    formulario.itmdatainicio.focus();
	                return false;
	            }
	        }
	    }
    }        
    if (!validaBranco( itmnome, 'Nome' ))    {
       formulario.itmnome.focus();
       return false;
    }
    if (!validaBranco( frccargahoraria, 'Carga Horária' ))    {
       formulario.frccargahoraria.focus();
       return false;
    }
    
    if (numCampodataI < 10 )    {
        alert('Data de início inválida');
        formulario.itmdatainicio.focus();
        return false;   
    }
    
    if (numCampodataF < 10 )    {
        alert('Data final inválida');
        formulario.itmdatafim.focus();
        return false;   
    } 
    if (!validaBranco( qtdprofessores, 'Quantidade de Professores' )){
		formulario.qtdprofessores.focus();
		return false;
    }
    if (!validaBranco( escolasatend, 'Escolas Atendidas' )){
		formulario.escolasatend.focus();
		return false;
    }
    if (!validaBranco( alunosbenef, 'Alunos Beneficiados' )){
		formulario.alunosbenef.focus();
		return false;
    }
    if( !processaCombosPopup() ){ 
    	return false;
    } 
    <?
	if( $_REQUEST['alterar'] != '' ){
	?>
		 if( !validaEtapa() ){
		 	return false;
		 }
	<? } ?> 
	window.location.href='parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid=<?=$_REQUEST['frcid'];?>&estuf=<?=$estuf;?>';
	return true;
}
  
function fecharPopup(){
    if( !confirm('Deseja realmente fechar esta janela? Os dados nao salvos até o momento serão perdidos.') )
    {
        return false;
    }
    return window.close(); 
}
 
function abrirCadLocalPolo(){
    return windowOpen('parindigena.php?modulo=principal/popupLocalPolo&acao=I',
                  'abrirCadLocalPolo',
                  'height=360,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}      
</script>
</html>