<?php

$somenteLeitura = 'S';
$estuf = $_REQUEST['estuf'];

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
	$temConvenio = $db->pegaLinha($sql);
	if(!$temConvenio){
		echo "<span style=\"color:red;\" >Não existe convênio informado no módulo 'PAR - Plano de Metas'.</br> Entre em contato com o gestor do sistema.</span>";
	}else{
        $db->monta_combo('sbaid',$sql,'S','Selecione uma subação...','','','','400','S','sbaid','','','');
	}
	die();
	
}

if ($_REQUEST['friid']) 
{
    
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
}

 
function deletaPovo( $friid )
{
     global $db;
    
    $friid = $_REQUEST['friid'];
    
    $sql = "DELETE FROM parindigena.povoformacaoinicial WHERE friid = '$friid'";
    if ( !$db->executar($sql))
    {
       return false;
    }

}

function deletaLingua( $friid )
{
     global $db;
    
    $friid = $_REQUEST['friid'];
    $sql = "DELETE FROM parindigena.linguaformacaoinicial WHERE friid = '$friid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}    
    
function deletaArranjo( $friid )
{
    global $db;
    
    $friid = $_REQUEST['friid'];
    $sql = "DELETE FROM parindigena.territorioformacaoinicial WHERE friid = '$friid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}

function inserePovo( $friid, $povide)
{
    global $db;

    if ($_REQUEST['friid'] != '' )
    {
        $friid = $_REQUEST['friid'];
    }
    $sql = "INSERT INTO 
                parindigena.povoformacaoinicial
                                               (
                                                    povid,
                                                    friid
                                                )
                VALUES
                      (
                          '$povide',
                          '$friid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}

function insereLingua( $friid, $linide )
{    
     global $db;
    //$linid = $_REQUEST['linid'][$i];
    if ($_REQUEST['friid'] != '' )
    {
        $friid = $_REQUEST['friid'];
    }
    $sql = "INSERT INTO 
                parindigena.linguaformacaoinicial
                       (
                            linid,
                            friid
                            )
                VALUES
                      (
                          '$linide',
                          '$friid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}
    
function insereArranjo( $friid, $teride )
{    
     global $db;
   
    if ($_REQUEST['friid'] != '' )
    {
        $friid = $_REQUEST['friid'];
    }
    $sql = "INSERT INTO 
                parindigena.territorioformacaoinicial
                                               (
                                                    terid,
                                                    friid
                                                )
                VALUES
                      (
                          '$teride',
                          '$friid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
           
} 

if (array_key_exists('btnGravar', $_REQUEST ) ) {    
    if( $erro != 1 ){ 
        
        $itmnome            	= $_REQUEST['itmnome'];
        $fricargahoraria     	= $_REQUEST['fricargahoraria'];
        $fridocaprovacao     	= $_REQUEST['fridocaprovacao'];        
        $friqtdeprofessor    	= $_REQUEST['friqtdeprofessor'];       
        $friqtdeescola       	= $_REQUEST['friqtdeescola'];
        $friqtdealunos       	= $_REQUEST['friqtdealunos'];
        $friparceria         	= $_REQUEST['friparceria'];      
    
        if ( !$_REQUEST['friid'] ){    
                 //INSERT
                 $sql = "INSERT INTO 
                                parindigena.formacaoinicial ( 
                               
                                                 fricargahoraria, 
                                                 fridocaprovacao, 
                                                 friqtdeprofessor,
                                                 friqtdeescola, 
                                                 friqtdealunos,
                                                 friparceria,
                                                 fristatus
                                                 )
                               VALUES (                                                              
    
                                       '$fricargahoraria',
                                       '$fridocaprovacao',
                                       '$friqtdeprofessor',
                                       '$friqtdeescola',                                         
                                       '$friqtdealunos',
                                       '$friparceria',
                                       'P'
                                       ) 
                                       returning friid";                            
                   
                 if ( $friid = $db->pegaUm( $sql ) ) {
                 	
                 	if($_REQUEST['sbaid']){
	                 	
	                 	$sql = "INSERT INTO parindigena.associativaplanodemetasprogramas (sbaid, appid, apptipo) VALUES ('{$_REQUEST['sbaid']}','{$friid}','{$_REQUEST['grpid']}')";
	                 	$db->executar($sql);
	                 }  
                 	$db->commit();
                  	$numPovo = count($_REQUEST['povid']);    

                        if ($numPovo > 0){                        
	                        for ( $i = 0; $i < $numPovo; $i++ ){    
	                            if($_REQUEST['povid'][$i] != ''){                  
	                               $povide = $_REQUEST['povid'][$i];
	                               inserePovo( $friid, $povide );
	                            }  
	                        }	                    
	                     }
                       
	                     $numLingua = count($_REQUEST['linid']);
	                     if ( $numLingua > 0 ){                        
	                           for ( $i = 0; $i < $numLingua; $i++ ){   
	                                if($_REQUEST['linid'][$i] != ''){                  
	                                   $linide = $_REQUEST['linid'][$i];
	                                   insereLingua( $friid, $linide );
	                                }   
	                           }
	                     }
	                            
	                     $numArranjo = count($_REQUEST['terid']);
	                     if ($numArranjo > 0 ){                        
	                        for ( $i = 0; $i < $numArranjo; $i++ ){    
	                           if($_REQUEST['terid'][$i] != ''){
	                               $teride = $_REQUEST['terid'][$i];
	                               insereArranjo( $friid, $teride );
	                           }               
	                        }
	                     }
                        $covcod = trim($_REQUEST['covcod_disable']);
                   
                        $estuf  = $_REQUEST['estuf'];
                 
                        $sql = "INSERT INTO
                                    parindigena.itemmonitoramento (
                                        covcod,      
                                        itmnome,  
                                        itmstatus,      
                                        usucpf,      
                                        grpid,          
                                        friid,          
                                        estuf           
                                        )
                                VALUES
                                    (
                                        '$covcod',
                                        '$itmnome',
                                        'A',
                                        '{$_SESSION['usucpf']}',
                                         1,
                                        '$friid',
                                        '$estuf'                               
                                    )";
                        $db->executar($sql); 
                        $db->commit();
                   }    
             } 
             else{   
                 $sql = " UPDATE parindigena.formacaoinicial SET
                                fricargahoraria   = '$fricargahoraria', 
                                fridocaprovacao   = '{$_REQUEST['fridocaprovacao']}',
                                friqtdeprofessor  = '{$_REQUEST['friqtdeprofessor']}',
                                friqtdeescola     = '$friqtdeescola', 
                                friqtdealunos     = '$friqtdealunos',
                                friparceria       = '$friparceria'
                          WHERE                       
                                friid            =  '{$_REQUEST['friid']}' RETURNING friid"; 
    
                 $friid = $db->pegaUm( $sql );    
                 
                 $sql = "UPDATE parindigena.itemmonitoramento
                             SET
                                 itmnome = '$itmnome',
                                 covcod = '{$_REQUEST['covcod_disable']}' 
                         WHERE
                                 friid = '{$_REQUEST['friid']}'
                         AND 
                                 itmstatus = 'A'";
                    
                 $db->executar($sql);
                   
                deletaPovo( $friid );
                $numPovo = count($_REQUEST['povid']);    
                
                if ($numPovo > 0){                        
                    for ( $i = 0; $i < $numPovo; $i++ ){    
                        if($_REQUEST['povid'][$i] != ''){                  
                           $povide = $_REQUEST['povid'][$i];
                           inserePovo( $friid, $povide );
                        }  
                    }                
                }
                 
                deletaLingua( $friid ); 
                $numLingua = count($_REQUEST['linid']);
                if ( $numLingua > 0 ){                        
                    for ( $i = 0; $i < $numLingua; $i++ ){   
                        if($_REQUEST['linid'][$i] != ''){                  
                           $linide = $_REQUEST['linid'][$i];
                           insereLingua( $friid, $linide );
                        }   
                    }
                }
               
                deletaArranjo( $friid );
                $numArranjo = count($_REQUEST['terid']);
                
                if ($numArranjo > 0 ){                        
                   for ( $i = 0; $i < $numArranjo; $i++ ){    
                       if($_REQUEST['terid'][$i] != ''){
                           $teride = $_REQUEST['terid'][$i];
                           insereArranjo( $friid, $teride );
                       }               
                   }
                }
                $db->commit();               
            }   
    }
     
    echo '<script type="text/javascript">
    		alert(\'Operação realizada com sucesso!\');
    		window.location.href = \'?modulo=principal/etapaFormacaoInicial&acao=A&friid='.$friid.'&estuf='.$_REQUEST['estuf'].'\';  
    	  </script>';    
} 

if (array_key_exists('excluir', $_REQUEST)) 
{
    $sql = "DELETE FROM parindigena.formacaoinicialetapa WHERE fieid = {$_REQUEST['excluir']}";
    
    $deleteItemForm = $db->executar( $sql );
    $db->commit(); 
}
$res = array(  array (  "descricao" => "Formação inicial",
						    "id" 		=> "0",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
				  		  )
			);	  
				
if ($_REQUEST['friid']){
	$perfis = arrayPerfil(); 		  
	if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){	
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
	}else{
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
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "5",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}
}
echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}");

$sqlConvenios = "SELECT 
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
                            
$temConvenio = $db->pegaLinha($sqlConvenios);

if(!$temConvenio){
	echo '<script type="text/javascript"> 
		alert("Não existe convênio de Formação Inicial cadastrado no PAR Indígena. \n Entre em contato com o gestor do sistema.");
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

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

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
	 
	function calculaValorCL()
	{
	
	    divElement=document.getElementById('itensEtapasCurso');
	    inputs = divElement.getElementsByTagName('input');
	    var custolicitado = 0.00;
	
	
	    for(i=0;i<inputs.length;i++)
	    {
	          if (inputs[i].name == 'fiecustolicitado[]')
	          {              
	              inputs[i].value = inputs[i].value.replace('.', '');
	              custolicitado = Number(custolicitado) + Number(inputs[i].value.replace(',', '.'));              
	              document.formulario.totlicitado.value = custolicitado.toFixed(2).replace('.', ',');  
	          }
	          
	    }  
	}
	
	function calculaValorCE()
	{ 
	    divElement=document.getElementById('itensEtapasCurso');
	    inputs = divElement.getElementsByTagName('input');
	 
	    var contrapartida = 0.00;
	
	    for(i=0;i<inputs.length;i++)
	    { 
	          if (inputs[i].name == 'fiecontrapartidaestado[]')
	          { 
	              inputs[i].value = inputs[i].value.replace('.', '');
	              contrapartida = Number(contrapartida) + Number(inputs[i].value.replace(',', '.'));     
	              document.formulario.totcontrapartida.value = contrapartida.toFixed(2).replace('.', ','); 
	              
	          }  
	    }  
	}
	 
	function processaCombosPopup()
	{ 
		selectAllOptions( document.getElementById( 'povid' ) );
		selectAllOptions( document.getElementById( 'linid' ) );
		selectAllOptions( document.getElementById( 'terid' ) );
		
		var numPovo   = document.getElementById( 'povid' ).value;
		var numLingua = document.getElementById( 'linid' ).value;
		var numTerri  = document.getElementById( 'terid' ).value;
		
		
		if( numPovo == '' )
		{
			alert('Você deve selecionar no mínimo um Povo Indígena.');
			return false;
		}
		if( numLingua == '' )
		{
			alert('Você deve selecionar no mínimo uma Lingua Indígena.');
			return false;
		}
		if( numTerri == '' )
		{
			alert('Você deve selecionar no mínimo um Território.');
			return false;
		}
		
	
	} 
	 
	function validaForm(){
	 
	    if(formulario.covcod_disable.value == '')
	    {
	        alert('Selecione um convênio');
	        formulario.covcod_disable.focus();
	        return false;
	    } 
	   
		if (!validaBranco( itmnome, 'Nome' ))
		{
		return false;
		}
		
		if (!validaBranco( fricargahoraria, 'Carga Horaria' ))
		{
		return false;
		}
		
		if (!validaBranco( friqtdeprofessor, 'Qtd. Professor' ))
		{
		return false;
		}
		
		if (!validaBranco( friqtdeescola, 'Qtd. Escola' ))
		{
		return false;
		}
		
		if (!validaBranco( friqtdealunos, 'Qtd. Alunos' ))
		{
		return false;
		}
	
	return processaCombosPopup();
	}
	
	function habCampoDoc()
	{    
	    document.formulario.fridocaprovacao.disabled = 0;    
	}
	
	function desabCampoDoc()
	{    
	    document.formulario.fridocaprovacao.disabled = 1;    
	}
	
	function fecharPopup()
	{
	    window.close();
	}
	
	    this._closeWindows = false;
	    var _modificado    = false;      
	
	function abrirCadLocalPolo()
	{
	    return windowOpen('parindigena.php?modulo=principal/popupLocalPolo&acao=I&tipocombo=prototype',
	                  'abrirCadLocalPolo',
	                  'height=360,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	} 
	</script>      
</head>
  
<body>
<form id="formulario" name="formulario" action="<?=$_SERVER['REQUEST_URI'];?>" onSubmit="return validaForm(this);" method="post">
    <table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Cadastro de Formação Inicial</div>
          <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
            <table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">           
            <tr>
                <td colspan = "2" align="center">
                    <table width = "100%" class="tabela listagem">
                    <tr bgcolor="#ffffcc">
                    	<td>
                    	<? 
                    	if( $_REQUEST['friid']){
                    		echo verificaEtapa( $_REQUEST['friid']);
                    	}
                    	?>	
                    	</td>
                    </tr>
                        <tr>
                        <td><span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span></td>
                        </tr>
                        <tr>
                            <td>
                                
                                <span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Formação Inicial</span>
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
                        if ( $_REQUEST['friid'] != '' )
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
				if($temConvenio){
	                if ( $_REQUEST['covcod'] )
	                {
	                    $covcod = $_REQUEST['covcod'];
	                }
	                else
	                {
	                    $covcod = $rsDadosForm[0]["covcod"];
	                }
	                $db->monta_combo('covcod', $sqlConvenios, $somenteLeituraUnidade, "Selecione...", 'mostrarSubacao', '', '', '390', 'S', 'covcod');
				}
                ?>

				</td>
			</tr>
			<tr id="trSubacoes" style="display:none;">
                <td class="subtitulodireita">Escolha uma subação do Plano de Metas:</td>
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
            <td class="subtitulodireita" > Carga Horária: </td>
                <td  align="left">
                 <?php
 
                 ?>
                  <?php $fricargahoraria = $rsDadosForm[0]["fricargahoraria"]; ?>
                  <?= campo_texto('fricargahoraria', 'S', $somenteLeitura, '', 6, 4, '######', '', 'left', '',  0, 'id="fricargahoraria" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                 <td class="subtitulodireita"> Aprovação CEE? </td>
                 <td  align="left">
                 <input id="aprov_cee_sim" type="radio"  onClick = "habCampoDoc();" name="aprov_cee" value="S" <?php if ( $rsDadosForm[0]['fridocaprovacao'] == 't' )  { echo("checked = \"checked\"");   } ?>/>
                    <label for="laprov_cee_sim">Sim</label>
                    <input id="aprov_cee_nao" type="radio" onClick = "desabCampoDoc();" name="aprov_cee" value="N" <?php if (  $rsDadosForm[0]['fridocaprovacao'] == 'f' ) {  echo("checked = \"checked\""); } ?>/>
                    <label for="laprov_cee_nao">Não</label>                
                 </td>
            </tr>
            <tr>
                 <td class="subtitulodireita"> Documento de Aprovacão: </td>
                 <td   align="left">
                 <?php $fridocaprovacao = $rsDadosForm[0]["fridocaprovacao"]; ?>
                    <?= campo_texto('fridocaprovacao', 'N', $somenteLeitura, '', 40, 45, '', '', 'left', '',  0, 'id="fridocaprovacao" onblur="MouseBlur(this);"' ); ?>
                 </td>                 
            </tr>
            <tr>
                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Dados de Atendimento do Curso</div></td>
            </tr>
            <tr>
                <td class="subtitulodireita"> Quantidade de Professores: </td>
                <td align="left" >  
                <?php $friqtdeprofessor = $rsDadosForm[0]["friqtdeprofessor"]; ?>
                <?= campo_texto('friqtdeprofessor', 'S', $somenteLeitura, '', 6, 18, '####', '', 'left', '',  0, 'id="friqtdeprofessor" onblur="MouseBlur(this);"' ); ?>
            </tr>
            <tr>
                <td class="subtitulodireita" > Escolas Atendidas: </td>
                <td  align="left">  
                <?php $friqtdeescola = $rsDadosForm[0]["friqtdeescola"]; ?>
                <?= campo_texto('friqtdeescola', 'S', $somenteLeitura, '', 6, 18, '####', '', 'left', '',  0, 'id="friqtdeescola" onblur="MouseBlur(this);"' ); ?>
                </td> 
            </tr>
            <tr>
                <td class="subtitulodireita"> Alunos Beneficiados: </td>
                <td  align="left">  
                 <?php $friqtdealunos = $rsDadosForm[0]["friqtdealunos"]; ?>
                <?= campo_texto('friqtdealunos', 'S', $somenteLeitura, '', 6, 18, '####', '', 'left', '',  0, 'id="friqtdealunos" onblur="MouseBlur(this);"' ); ?>
                
               </td> 
            </tr> 

            <tr>
                <td class="SubTituloDireita">Povos Indígenas Atendidos:</td>
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
                                            parindigena.povoformacaoinicial AS pf
                                         INNER JOIN parindigena.povoindigena p ON pf.povid = p.povid
                                         WHERE pf.friid = '{$_REQUEST['friid']}'";
                            if( $_REQUEST['friid'] != '' )
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
                                  parindigena.linguaformacaoinicial AS lf
                             INNER JOIN parindigena.linguaindigena l ON lf.linid = l.linid
                             WHERE 
                                  lf.friid = '{$_REQUEST['friid']}'";
                            if( $_REQUEST['friid'] != '' )
                            {
                                $linid    = $db->carregar($sqlLinid);  
                            }

                combo_popup( "linid", $sql, "Selecione a(s) Língua(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
                ?>
                <img src="../imagens/obrig.gif">
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
                                parindigena.territorioetnoeducacional as territorio
                                ORDER BY descricao ASC";
                   
                    $sqlArrid = "SELECT 
                                     af.terid as codigo,
                                     a.ternome as descricao
                                FROM 
                                     parindigena.territorioformacaoinicial AS af
                                INNER JOIN parindigena.territorioetnoeducacional a ON af.terid = a.terid
                                WHERE 
                                     af.friid = '{$_REQUEST['friid']}'
                                     ORDER BY descricao ASC ";
                    if( $_REQUEST['friid'] != '' )
                    {
                        $terid    = $db->carregar($sqlArrid);            
                    }
      
                    combo_popup( "terid", $sql, "Selecione o(s) Território(s) Etno-educaciona(l)(is)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
                   
                ?>
                <img src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita">Parcerias:</td>
                <td align="left" >
                <?php $friparceria = $rsDadosForm[0]["friparceria"]; ?>
                <TEXTAREA COLS="56" ROWS="3" NAME="friparceria" onblur="MouseBlur(this);textCounter(this.form.friparceria,this.form.no_entobs,500);if(this.value.length>(MAXLEN=500))this.value=this.value.substr(0,MAXLEN)" onkeydown="textCounter(this.form.friparceria,this.form.no_entobs,500);" onkeyup="textCounter(this.form.friparceria,this.form.no_entobs,500);"><?php echo $friparceria; ?></TEXTAREA>
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
			<? } else { ?>
				<tr style="background-color: #cccccc;">
					<td class="SubTituloEsquerda"  align="left">            
						<input type="submit" name="btnGravar" value="Gravar"/>
						<input type="button" name="btnCancel" value="Fechar" onclick="fecharPopup();"/>
					</td>
				</tr> 	  	  
			<?php } ?>      
		</table>
	</div>
	</table>
</form>
</body>
</html>