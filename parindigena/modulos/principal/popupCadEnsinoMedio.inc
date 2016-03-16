<?php
//
#cadastra uma Formaï¿½ao Inicial
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
			
	$db->monta_combo('sbaid',$sql,'S','Selecione uma subação...','','','','400','S','sbaid','','','');
	die();
	
}

if ($_REQUEST['emiid']) 
{
    
    $sql = "select * 
                from 
                    parindigena.itemmonitoramento as im 
            inner join parindigena.ensinomediointegrado as f 
                on f.emiid = im.emiid 
            where 
                f.emiid= {$_REQUEST['emiid']}
            and im.itmstatus = 'A'";                                     
          

    $rsDadosForm       = $db->carregar( $sql );
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];   
}

function deletaPovo( $emiid )
{
     global $db;
    
    $emiid = $_REQUEST['emiid'];
    
    $sql = "DELETE FROM parindigena.povoensinomediointegrado WHERE emiid = '$emiid'";
    if ( !$db->executar($sql))
    {
       return false;
    }

}

function deletaLingua( $emiid )
{
     global $db;
    
    $emiid = $_REQUEST['emiid'];
    $sql = "DELETE FROM parindigena.linguaensinomediointegrado WHERE emiid = '$emiid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}    
    
function deletaArranjo( $emiid )
{
    global $db;
    
    $emiid = $_REQUEST['emiid'];
    $sql = "DELETE FROM parindigena.territorioensinomediointegrado WHERE emiid = '$emiid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}

function inserePovo( $emiid, $povide)
{
    global $db;

    if ($_REQUEST['emiid'] != '' )
    {
        $emiid = $_REQUEST['emiid'];
    }
    $sql = "INSERT INTO 
                parindigena.povoensinomediointegrado
                                               (
                                                    povid,
                                                    emiid
                                                )
                VALUES
                      (
                          '$povide',
                          '$emiid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}

function insereLingua( $emiid, $linide )
{    
     global $db;
    //$linid = $_REQUEST['linid'][$i];
    if ($_REQUEST['emiid'] != '' )
    {
        $emiid = $_REQUEST['emiid'];
    }
    $sql = "INSERT INTO 
                parindigena.linguaensinomediointegrado
                       (
                            linid,
                            emiid
                            )
                VALUES
                      (
                          '$linide',
                          '$emiid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}
    
function insereArranjo( $emiid, $teride )
{    
     global $db;
   
    if ($_REQUEST['emiid'] != '' )
    {
        $emiid = $_REQUEST['emiid'];
    }
    $sql = "INSERT INTO 
                parindigena.territorioensinomediointegrado
                                               (
                                                    terid,
                                                    emiid
                                                )
                VALUES
                      (
                          '$teride',
                          '$emiid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
           
} 

if (array_key_exists('btnGravar', $_REQUEST ) ) 
{    
   
    
    if( $erro != 1 )
    {
           
        $itmnome                = $_REQUEST['itmnome'];
        $emicargahoraria     	= $_REQUEST['emicargahoraria'];
        $emidocaprovacao     	= $_REQUEST['emidocaprovacao'];         
        $emiqtdeescola       	= $_REQUEST['emiqtdeescola'];
        $emiqtdealunos       	= $_REQUEST['emiqtdealunos'];
        $emiparceria         	= $_REQUEST['emiparceria'];      
    
        if ( !$_REQUEST['emiid'] ) 
        {    
                 //INSERT
                 $sql = "INSERT INTO 
                                parindigena.ensinomediointegrado ( 
                               
                                                 emicargahoraria, 
                                                 emidocaprovacao, 
                                                 emiqtdeescola, 
                                                 emiqtdealunos,
                                                 emiparceria
                                                 )
                               VALUES (                                                              
    
                                       '$emicargahoraria',
                                       '$emidocaprovacao',
                                       '$emiqtdeescola',                                         
                                       '$emiqtdealunos',
                                       '$emiparceria'
                                       ) 
                                       returning emiid";                            
                   
                   if ( $emiid = $db->pegaUm( $sql ) )
                   {  
                       $db->commit();
     
                    $numPovo = count($_REQUEST['povid']);    
                    if ($numPovo > 0)
                    {                        
                       for ( $i = 0; $i < $numPovo; $i++ )
                       {    
                            if($_REQUEST['povid'][$i] != '')
                            {                  
                               $povide = $_REQUEST['povid'][$i];
                               inserePovo( $emiid, $povide );
                            }  
                       }
                    
                    }
                       
                     $numLingua = count($_REQUEST['linid']);
                     if ( $numLingua > 0 )
                        {                        
                           for ( $i = 0; $i < $numLingua; $i++ )
                           {   
                                if($_REQUEST['linid'][$i] != '')
                                {                  
                                   $linide = $_REQUEST['linid'][$i];
                                   insereLingua( $emiid, $linide );
                                }   
                           }
                     }
                            
                     $numArranjo = count($_REQUEST['terid']);
                     if ($numArranjo > 0 )
                     {                        
                       for ( $i = 0; $i < $numArranjo; $i++ )
                       {    
                           if($_REQUEST['terid'][$i] != '')
                           {
                               $teride = $_REQUEST['terid'][$i];
                               insereArranjo( $emiid, $teride );
                           }               
                       }
                     }
    
                        $arrDatamI = explode( "/", $_REQUEST['itmdatainicio']);
                        $formataDatamI = $arrDatamI[2].'-'.$arrDatamI[1].'-'.$arrDatamI[0];
                        
                        $arrDatamF = explode( "/", $_REQUEST['itmdatafim']);
                        $formataDatamF = $arrDatamF[2].'-'.$arrDatamF[1].'-'.$arrDatamF[0];        
                        
                        // Inserindo a Formacao Inicial no itemmonitoramento.       
    
                        //para efeito de teste... simulando variaveis abaixo. que vï¿½o vir da arvore, terei que preparar o explode para capturar os dados
       
                        //verificando qual ï¿½ o grpid sem usar request, via cod...
                                            
                        $covcod = $_REQUEST['covcod_disable'];
                   
                        $estuf  = $_REQUEST['estuf'];
                 
                        $sql = "INSERT INTO
                                    parindigena.itemmonitoramento (
                                        covcod,      
                                        itmnome,
                                        --itmdatainicio,  
                                        --itmdatafim,     
                                        itmstatus,      
                                        usucpf,      
                                        grpid,          
                                        emiid,          
                                        estuf           
                                        )
                                VALUES
                                    (
                                        '$covcod',
                                        '$itmnome',
                                        --'{$formataDatamI}',
                                        --'{$formataDatamF}',
                                        'A',
                                        '{$_SESSION['usucpf']}',
                                         1,
                                        '$emiid',
                                        '$estuf'                               
                                    )";
                        $db->executar($sql);
           
                        $db->commit();
                   }
    
             } 
             else 
             {
                $arrDatamI = explode( "/", $_REQUEST['itmdatainicio']);
                $formataDatamI = $arrDatamI[2].'-'.$arrDatamI[1].'-'.$arrDatamI[0];
                
                $arrDatamF = explode( "/", $_REQUEST['itmdatafim']);
                $formataDatamF = $arrDatamF[2].'-'.$arrDatamF[1].'-'.$arrDatamF[0];       
                        
                 $sql = " UPDATE parindigena.ensinomediointegrado SET
                                emicargahoraria   = '$emicargahoraria', 
                                emidocaprovacao   = '{$_REQUEST['emidocaprovacao']}',
                                emiqtdeescola     = '$emiqtdeescola', 
                                emiqtdealunos     = '$emiqtdealunos',
                                emiparceria       = '$emiparceria'
                          WHERE                       
                                emiid            =  '{$_REQUEST['emiid']}' "; 
    
                  $db->executar( $sql );    
                 
                 $sql = "UPDATE parindigena.itemmonitoramento
                             SET
                                 itmnome = '$itmnome',
                                 covcod = '{$_REQUEST['covcod_disable']}'
                                 --itmdatainicio = '{$formataDatamI}',
                                 --itmdatafim = '{$formataDatamF}'
                         WHERE
                                 emiid = '{$_REQUEST['emiid']}'
                         AND 
                                 itmstatus = 'A'";
                    
                 $db->executar($sql);
                  
                  //DELETA TUDO E INSERE DE NOVO
                deletaPovo( $emiid );
                $numPovo = count($_REQUEST['povid']);    
                
                if ($numPovo > 0)
                {                        
                   for ( $i = 0; $i < $numPovo; $i++ )
                   {    
                        if($_REQUEST['povid'][$i] != '')
                        {                  
                           $povide = $_REQUEST['povid'][$i];
                           inserePovo( $emiid, $povide );
                        }  
                   }
                
                }
                 
                deletaLingua( $emiid ); 
                $numLingua = count($_REQUEST['linid']);
                if ( $numLingua > 0 )
                {                        
                   for ( $i = 0; $i < $numLingua; $i++ )
                   {   
                        if($_REQUEST['linid'][$i] != '')
                        {                  
                           $linide = $_REQUEST['linid'][$i];
                           insereLingua( $emiid, $linide );
                        }   
                   }
                }
               
               deletaArranjo( $emiid );
                $numArranjo = count($_REQUEST['terid']);
                
                if ($numArranjo > 0 )
                {                        
                   for ( $i = 0; $i < $numArranjo; $i++ )
                   {    
                       if($_REQUEST['terid'][$i] != '')
                       {
                           $teride = $_REQUEST['terid'][$i];
                           insereArranjo( $emiid, $teride );
                       }               
                   }
                }
                $db->commit();               
             }
             
             
             
             
             //inserindo/alterando as etapas...
             
             
              if (  $_REQUEST['emeid']) 
              {
                for( $k = 0; $k < count( $_REQUEST['emeid'] ); $k++) 
                {
                    
                                
                    $arrDataI = explode( "/", $_REQUEST['emedatainicial'][$k]);
                    $formataDataI = $arrDataI[2].'-'.$arrDataI[1].'-'.$arrDataI[0];
                    
                    $arrDataF = explode( "/", $_REQUEST['emedatafinal'][$k]);
                    $formataDataF = $arrDataF[2].'-'.$arrDataF[1].'-'.$arrDataF[0];
        
                    
                    if ($_REQUEST['emeid'][$k] == '' &&  $formataDataI != '--' &&  $formataDataF != '--') 
                    {
                    
                        if ($_REQUEST['emiid'] != '' )
                        {
                            $emiid = $_REQUEST['emiid'];
                        }
                        
        
                            $sql = " INSERT INTO
                                        parindigena.ensinomediointegradoetapa (
                                                emedatainicial,
                                                emedatafinal,
                                                locid,
                                                emiid,
                                                emecustolicitado,
                                                emecontrapartidaestado)
                                     VALUES ( 
                                             '{$formataDataI}',       
                                             '{$formataDataF}', 
                                             ".($_REQUEST['locid'][$k] ? $_REQUEST['locid'][$k] : 'NULL').",
                                              '$emiid',
                                             '" . str_replace('.','', $_REQUEST['emecustolicitado'][$k]). "',
                                             '" . str_replace('.','', $_REQUEST['emecontrapartidaestado'][$k]). "')";
        
                    }
                    else
                    {       
                        if (  $formataDataI != '--' &&  $formataDataF != '--' )
                        {
                                                        
                                $sql = "UPDATE 
                                            parindigena.ensinomediointegradoetapa 
                                        SET
                                                emedatainicial         = '{$formataDataI}',
                                                emedatafinal           = '{$formataDataF}', 
                                                locid                  =  ".($_REQUEST['locid'][$k] ? $_REQUEST['locid'][$k] : 'NULL').", 
                                                emecustolicitado       = '" . str_replace('R$','', $_REQUEST['emecustolicitado'][$k]). "',
                                                emecontrapartidaestado = '" . str_replace('R$','', $_REQUEST['emecontrapartidaestado'][$k]). "'
                                        WHERE
                                            emeid     =  '{$_REQUEST['emeid'][$k]}' "; 
                          
                       
                        }
                    }
                $db->executar($sql);
                $db->commit();
               
                
                $dadosSalvos = true;
                
                }
              
          }
    }
         
    echo '<script type="text/javascript">
    		alert(\'Operaï¿½ï¿½o realizada com sucesso!\');
    		window.opener.location.href = \'?modulo=inicio&acao=C&estuf='.$estuf.'\'; 
    		window.close();
    	  </script>';         
                 
}


//FIM DO INSERT PARA A FORMACAO INICIAL.

if (array_key_exists('excluir', $_REQUEST)) 
{
    $sql = "DELETE FROM parindigena.ensinomediointegradoetapa WHERE emeid = {$_REQUEST['excluir']}";
   // $db->executar('DELETE FROM cte.itenslaboratorio WHERE ilbid = ' . (integer) $_REQUEST['ilbid'][$k]);
 
    $deleteItemForm = $db->executar( $sql );
    $db->commit();
     
   // die('null');
}

$res = array( 0 => array (  "descricao" => "Formaï¿½ï¿½o inicial",
						    "id" 		=> "3",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadEnsinoMedio&acao=I&emiid={$rsDadosForm[0]['emiid']}"
				  		  )
			);	  
				
if ($_REQUEST['emiid']){
	array_push($res,
					array ("descricao" => "Restriï¿½ï¿½o",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&emiid={$rsDadosForm[0]['emiid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&emiid={$rsDadosForm[0]['emiid']}"
					  		   )
			  );
}
echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/popupCadEnsinoMedio&acao=I&emiid={$rsDadosForm[0]['emiid']}");

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
    </script>
  </head>
  <body marginheight="0" marginwidth="0">

<form onSubmit="return validaForm(this);"action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario">
    <table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Cadastro de Ensino Médio Integrado</div>
          <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
            <table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
           
            <tr>
                <td colspan = "2" align="center">
                    <table width = "100%" class="tabela listagem">
                        <tr>
                        <td><span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span></td>
                        </tr>
                        <tr>
                            <td>
                                
                                <span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Ensino Médio Integrado</span>
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
                                <span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)
                                </td>
                            </tr>
                        <?php
                        }
                        if ( $_REQUEST['emiid'] != '' )
                        {
                             
                        $sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
                        $rs = $db->carregar( $sql );
                        $convenio = $rs[0]['covnumero'];
                        
                        ?>
                            <tr>
                                <td>
                                <span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)
                                </td>
                            </tr>
                            <tr>
                                <td>
                                <span style="display: block;font-size:100%;color:black; font-weight:bold;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?php  echo $rsDadosForm[0]["itmnome"]; ?>
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
                            
                            
                
                if ( $_REQUEST['covcod'] )
                {
                    $covcod = $_REQUEST['covcod'];
                }
                else
                {
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
                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Identificação do Curso</td>
            </tr> 
            <tr>
                <td class="subtitulodireita">Nome do Curso: </td>
                <td align="left" >
                   <?php $itmnome = $rsDadosForm[0]["itmnome"]; ?>
                  <?= campo_texto('itmnome', 'S', $somenteLeitura, '', 70, 100, '', '', 'left', '',  0, 'id="itmnome" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <!-- 
            <tr>
                <td class="subtitulodireita" > Data de início: </td>
                <td  align="left">
                 <?php 
                 ?>
                 <?php $itmdatainicio = $rsDadosForm[0]["itmdatainicio"]; ?>

                  <?= campo_data( 'itmdatainicio','S', $somenteLeitura, '', 'S' ); ?>
                </td>
            </tr>
            <tr>
            <td class="subtitulodireita" > Data de Término: </td>
                <td  align="left">
                 <?php 
                 ?>
                 <?php $itmdatafim = $rsDadosForm[0]["itmdatafim"]; ?>
                 <?= campo_data( 'itmdatafim', 'S', $somenteLeitura, '', 'S' ); ?>
                </td>
            </tr>
            -->
            <tr>
            <td class="subtitulodireita" > Carga Horária: </td>
                <td  align="left">
                 <?php
 
                 ?>
                  <?php $emicargahoraria = $rsDadosForm[0]["emicargahoraria"]; ?>
                  <?= campo_texto('emicargahoraria', 'N', $somenteLeitura, '', 10, 18, '######', '', 'left', '',  0, 'id="emicargahoraria" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                 <td class="subtitulodireita"> Aprovação CEE? </td>
                 <td  align="left">
                 <input id="aprov_cee_sim" type="radio"  onClick = "habCampoDoc();" name="aprov_cee" value="S" <?php if ( $rsDadosForm[0]['emidocaprovacao'] == 't' )  { echo("checked = \"checked\"");   } ?>/>
                    <label for="laprov_cee_sim">Sim</label>
                    <input id="aprov_cee_nao" type="radio" onClick = "desabCampoDoc();" name="aprov_cee" value="N" <?php if (  $rsDadosForm[0]['emidocaprovacao'] == 'f' ) {  echo("checked = \"checked\""); } ?>/>
                    <label for="laprov_cee_nao">Nao</label>                
                 </td>
            </tr>
            <tr>
                 <td class="subtitulodireita"> Documento de Aprovação </td>
                 <td   align="left">
                 <?php $emidocaprovacao = $rsDadosForm[0]["emidocaprovacao"]; ?>
                    <?= campo_texto('emidocaprovacao', 'N', $somenteLeitura, '', 40, 45, '', '', 'left', '',  0, 'id="emidocaprovacao" onblur="MouseBlur(this);"' ); ?>
                 </td>                 
            </tr>
            <tr>
                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Dados de Atendimento do Curso</div></td>
            </tr>
           
            <tr>
                <td class="subtitulodireita" > Escolas Atendidas: </td>
                <td  align="left">  
                <?php $emiqtdeescola = $rsDadosForm[0]["emiqtdeescola"]; ?>
                <?= campo_texto('emiqtdeescola', 'S', $somenteLeitura, '', 6, 18, '####', '', 'left', '',  0, 'id="emiqtdeescola" onblur="MouseBlur(this);"' ); ?>
                </td> 
            </tr>
            <tr>
                <td class="subtitulodireita"> Alunos Beneficiados: </td>
                <td  align="left">  
                 <?php $emiqtdealunos = $rsDadosForm[0]["emiqtdealunos"]; ?>
                <?= campo_texto('emiqtdealunos', 'S', $somenteLeitura, '', 6, 18, '####', '', 'left', '',  0, 'id="emiqtdealunos" onblur="MouseBlur(this);"' ); ?>
                
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
                                            parindigena.povoensinomediointegrado AS pf
                                         INNER JOIN parindigena.povoindigena p ON pf.povid = p.povid
                                         WHERE pf.emiid = '{$_REQUEST['emiid']}'";
                            if( $_REQUEST['emiid'] != '' )
                            {
                                $povid    = $db->carregar($sqlPovid);
                            }
                
                combo_popup( "povid", $sql, "Selecione o(s) Povo(s) Indï¿½gena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
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
                                  parindigena.linguaensinomediointegrado AS lf
                             INNER JOIN parindigena.linguaindigena l ON lf.linid = l.linid
                             WHERE 
                                  lf.emiid = '{$_REQUEST['emiid']}'";
                            if( $_REQUEST['emiid'] != '' )
                            {
                                $linid    = $db->carregar($sqlLinid);  
                            }

                combo_popup( "linid", $sql, "Selecione a(s) Lï¿½ngua(s) Indï¿½gena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
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
                                     parindigena.territorioensinomediointegrado AS af
                                INNER JOIN parindigena.territorioetnoeducacional a ON af.terid = a.terid
                                WHERE 
                                     af.emiid = '{$_REQUEST['emiid']}'
                                     ORDER BY descricao ASC ";
                    if( $_REQUEST['emiid'] != '' )
                    {
                        $terid    = $db->carregar($sqlArrid);            
                    }
      
                    combo_popup( "terid", $sql, "Selecione o(s) Territórios(s) Etno-educaciona(l)(is)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
                   
                ?>
                <img src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita">Parcerias</td>
                <td align="left" >
                <?php $emiparceria = $rsDadosForm[0]["emiparceria"]; ?>
                <TEXTAREA COLS="56" ROWS="3" NAME="emiparceria"><?php echo $emiparceria; ?></TEXTAREA>
                <?php
                                 
                ?>
                  
                </td>
            </tr>
             
            <div id="loader-container">
               <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div> 
            </div>
             
            <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
              <tr style="background-color: #cccccc;">
                <td>
                  <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Etapas do Curso</div>
                  <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
                    <table id="itensEtapasCurso" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
                      <colgroup>
                        <col width="7%" />
                        <col width="3%" />
                        <col width="15%" />
                        <col width="15%" />
                        <col width="20%" />
                        <col width="20%" />
                        <col width="20%" />
                      </colgroup>
       
                      <tr style="text-align: center">
                        <td style="padding: 2px; font-weight: bold;">&nbsp;</td>
                        <td style="padding: 2px; font-weight: bold;">Etapa </td>
                        <td style="padding: 2px; font-weight: bold;">Data de Inï¿½cio</td>
                        <td style="padding: 2px; font-weight: bold;">Data Fim</td>
                        <td style="padding: 2px; font-weight: bold;">Local / Pólo &nbsp;&nbsp;&nbsp;&nbsp;<span style="cursor:pointer; text-align: center;" id="linkInserirItem" onclick="return abrirCadLocalPolo();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none "  /> Cadastrar</span></td>
                        <td style="padding: 2px; font-weight: bold;">Custo da Etapa (R$)</td>
                        <td style="padding: 2px; font-weight: bold;">Contrapartida do Estado (R$)</td>

                      </tr>

                    </table>
                   
                    </div>
              
  
                     <table id="totalEtapas" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
                        <tr>
                            <th align="left" colspan="4" width="59%"> Total R$:</th>
                            <th>Custo das Etapas (R$)</th>
                            <th>Contrapartida do Estado (R$)</th>
                        </tr>
                        <tr>
                            <td align="left" colspan="4" >&nbsp;</td>
                            <td idth="18%"><?= campo_texto('totlicitado', 'N', 'N', '', 30, 50, '#.###.###,##', '', 'left', '',  0, 'id="totlicitado" onblur="MouseBlur(this);" ' ); ?></td>
                            <td width="21%"><?= campo_texto('totcontrapartida', 'N', 'N', '', 32, 50, '#.###.###,##', '', 'left', '',  0, 'id="totcontrapartida" onblur="MouseBlur(this);"' ); ?></td>
                        </tr>
                     </table>
              
              <div style="padding: 5px 0px 0px 5px"></div>
            <span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarEtapa('','','','', '', '');"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir Etapas</span>
          
          <br />
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
	         	<td class="SubTituloEsquerda"  align="left"> 
	         	</td>
	       	 </tr> 
	  	  <?	
	  	  }
	  	  else{
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
    </form>
   </table>
  </body>
  <script type="text/javascript">
  <!--
  ////

  
function calculaValorCL()
{

    divElement=document.getElementById('itensEtapasCurso');
    inputs = divElement.getElementsByTagName('input');
    var custolicitado = 0.00;


    for(i=0;i<inputs.length;i++)
    {
          if (inputs[i].name == 'emecustolicitado[]')
          {              
              inputs[i].value = inputs[i].value.replace('.', '');
              custolicitado = Number(custolicitado) + Number(inputs[i].value.replace(',', '.'));              
              //custolicitado.replace(',', '');
              //custolicitado.replace('.', '');
              //arseFloat(custolicitado);
              //dado_a += custolicitado;              
              document.formulario.totlicitado.value = custolicitado.toFixed(2).replace('.', ','); 
              
  
          }
          
    } 
    
    //document.formulario.totLicitado.value = custolicitado;    
   //document.formulario.totContrapartida.value = contrapartida;
}

function calculaValorCE()
{

    divElement=document.getElementById('itensEtapasCurso');
    inputs = divElement.getElementsByTagName('input');
;
    var contrapartida = 0.00;

    for(i=0;i<inputs.length;i++)
    {
         
          if (inputs[i].name == 'emecontrapartidaestado[]')
          {
              
              
              inputs[i].value = inputs[i].value.replace('.', '');
              contrapartida = Number(contrapartida) + Number(inputs[i].value.replace(',', '.'));              
              //custolicitado.replace(',', '');
              //custolicitado.replace('.', '');
              //arseFloat(custolicitado);
              //dado_a += custolicitado;              
              document.formulario.totcontrapartida.value = contrapartida.toFixed(2).replace('.', ','); 
              
          } 

    } 
    
    //document.formulario.totLicitado.value = custolicitado;    
   //document.formulario.totContrapartida.value = contrapartida;
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
   // formulario.submit();
    
  //Funï¿½ao que abre a popup para listar os laboratorios cadastrados.
 
function validaForm()
{
    if(formulario.covcod_disable.value == '')
    {
        alert('Selecione um convï¿½nio');
        formulario.covcod_disable.focus();
        return false;
    }
  
    
    divElement=document.getElementById('itensEtapasCurso');
    inputs = divElement.getElementsByTagName('input');
    var dataei;
    var dataef;
    for(i=0;i<inputs.length;i++)
    {
      if (inputs[i].name == 'emedatainicial[]'){
          dataei = inputs[i]; 
      }
      if (inputs[i].name == 'emedatafinal[]'){
          dataef = inputs[i];
      } 
      if ((dataei.value != null && dataei.value != '') && (dataef != null && dataef.value !='') ){
          if (!validaDataMaior(dataei, dataef)){
              alert('Data inicial da etapa nao pode ser maior que a data final');
              dataei.focus();
              return false;
          }
      }
    }    
     
  
if (!validaBranco( itmnome, 'Nome' ))
{
return false;
}
if (!validaBranco( emicargahoraria, 'Carga Horaria' ))
{
return false;
}

if (!validaBranco( emiqtdeescola, 'Qtd. Escola' ))
{
return false;
}
if (!validaBranco( emiqtdealunos, 'Qtd. Alunos' ))
{
return false;
}


return processaCombosPopup();

}

function habCampoDoc()
{
    
    document.formulario.emidocaprovacao.disabled = 0;
    
}

function desabCampoDoc()
{
    
    document.formulario.emidocaprovacao.disabled = 1;
    
}

function fecharPopup()
{
    window.close();
}

    this._closeWindows = false;
    var _modificado    = false;
    /**
     * 
     */
    function removerEtapa(linha, emeid)
    {
        var linha = $(linha);
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
            $('loader-container').show();
            try {
                if (emeid != '') {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + emeid,
                                                   onComplete: function(r)
                                                   {
                                                       //if (r.responseText == 'null')
                                                           linha.parentNode.removeChild(linha);
                                                       //else
                                                       //    alert('Nï¿½o foi possï¿½vel excluir o item.');
                                                           linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
                } else {
                    linha.parentNode.removeChild(linha);
                }
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }

function verificaDataDaEtapa(data, tipo)
{
    var data;
    var tipo;
    var datai = formulario.itmdatainicio;
    var dataf = formulario.itmdatafim;
    
    if (!validaDataMaior(datai, data))
    {
        alert('A data da Etapa deve estar entre da data de início e a data final do curso');
        data.focus();
        data.value = ''; 
        return false;
    }
    if (!validaDataMaior(data, dataf))
    {
        alert('A data da Etapa deve estar entre da data de início e a data final do curso');
        data.focus();
        data.value = '';
        return false;
    }
     
}
    /**
     * 
     */
    function adicionarEtapa(emeid, emedatainicial,  emedatafinal, locid, emecustolicitado, emecontrapartidaestado)
    {

        top.verified = 10;
        
        var input_emedatainicial;
        var input_locid;  
        var input_emedatafinal;        
        var input_emecustolicitado;
        var input_emecontrapartidaestado;
        var input_emeid;
 
       
        var btnExcluirEtapa = document.createElement('img');
        btnExcluirEtapa.src     = '/imagens/excluir.gif';
        btnExcluirEtapa.onclick = function()
        {
            removerEtapa(this.parentNode.parentNode.getAttribute("id"), emeid);
        }
        
        //NUMERO DA ETAPA

        //Data inicial
        input_emedatainicial    = document.createElement('input');
        input_emedatainicial.setAttribute('type', 'text');
        input_emedatainicial.setAttribute('class', 'normal');
        input_emedatainicial.setAttribute('name', 'emedatainicial[]');
        input_emedatainicial.setAttribute('maxlength', '100');
        input_emedatainicial.setAttribute('size', '20');
        input_emedatainicial.setAttribute('value', mascaraglobal('##/##/####',emedatainicial));
        input_emedatainicial.setAttribute('onKeyUp', 'this.value=mascaraglobal("##/##/####", this.value)');
        input_emedatainicial.setAttribute('onBlur', 'verificaDataDaEtapa(this, "i")');
        
        //hidden, id do material
        input_emeid     = document.createElement('input');
        input_emeid.setAttribute('type', 'hidden');
        input_emeid.setAttribute('name', 'emeid[]');
        input_emeid.setAttribute('value', emeid);

        //combo dos locais
        input_locid   = document.createElement('select');
        input_locid.setAttribute('class', 'CampoEstilo');
        input_locid.setAttribute('name', 'locid[]');
        input_locid.options[input_locid.options.length] = new Option('Selecione...' , '');
        <?php
        
        //carregando a combo das etapas.
       $res = (array) $db->carregar('select
       								  locid, 
       								  locnome 
       								 from
       								  parindigena.localpolo pl
       								  INNER JOIN entidade.endereco e ON e.endid = pl.endid AND
       								  									e.estuf = \''.$_REQUEST['estuf'].'\'  
       								 order by
       								  locnome');
       
    	if ($res[0]){
	        while (list(, $item) = each($res)) {
	            $item = array_map('addslashes', $item);
	            echo 'selected=locid==' , $item["locid"]
	                , ';input_locid.options[input_locid.options.length]=new Option(\'' , $item["locnome"] , '\',\'' , $item["locid"] , '\',selected,selected);';
	        }
    	}
	
        ?>

        //Data Final
        input_emedatafinal    = document.createElement('input');
        input_emedatafinal.setAttribute('type', 'text');
        input_emedatafinal.setAttribute('class', 'normal');
        input_emedatafinal.setAttribute('name', 'emedatafinal[]');
        input_emedatafinal.setAttribute('maxlength', '100');
        input_emedatafinal.setAttribute('size', '20');
        input_emedatafinal.setAttribute('value', mascaraglobal('##/##/####',emedatafinal));
        input_emedatafinal.setAttribute('onKeyUp', 'this.value=mascaraglobal("##/##/####", this.value)');
        input_emedatafinal.setAttribute('onBlur', 'verificaDataDaEtapa(this, "f")');
              
        //Custo Licitado
        input_emecustolicitado    = document.createElement('input');
        input_emecustolicitado.setAttribute('type', 'text');
        input_emecustolicitado.setAttribute('class', 'normal');
        input_emecustolicitado.setAttribute('name', 'emecustolicitado[]');
        input_emecustolicitado.setAttribute('maxlength', '300');
        input_emecustolicitado.setAttribute('value', mascaraglobal('###.###.###,##',emecustolicitado));
        input_emecustolicitado.setAttribute('onKeyUp', '  this.value=mascaraglobal("###.###.###,##", this.value); calculaValorCL(); ');
        //input_emecustolicitado.setAttribute('onKeyPress', 'calculaValor()');
              
        //Contrapartida do Estado
        input_emecontrapartidaestado    = document.createElement('input');
        input_emecontrapartidaestado.setAttribute('type', 'text');
        input_emecontrapartidaestado.setAttribute('class', 'normal');
        input_emecontrapartidaestado.setAttribute('name', 'emecontrapartidaestado[]');
        input_emecontrapartidaestado.setAttribute('maxlength', '300');
        input_emecontrapartidaestado.setAttribute('value', mascaraglobal('###.###.###,##',emecontrapartidaestado));
        input_emecontrapartidaestado.setAttribute('onKeyUp', ' this.value=mascaraglobal("###.###.###,##", this.value); calculaValorCE(); ');
        //input_emecontrapartidaestado.setAttribute('onKeyPress', 'calculaValor()');
             
        var tabelaEtapasCurso = $('itensEtapasCurso');
        var novaLinha                    = tabelaEtapasCurso.insertRow(tabelaEtapasCurso.rows.length);
        novaLinha.id                     = 'linhaitensEtapasCurso' + tabelaEtapasCurso.rows.length + 1;
        novaLinha.style.textAlign        = 'center';
        novaLinha.style.backgroundColor  = '#f0f0f0';
        
        var numRows = (Number(tabelaEtapasCurso.rows.length) - 1);

        input_num     = document.createElement('text');
        input_num.setAttribute('value',  numRows );
        input_num.setAttribute('name', 'numRows');
        input_num.setAttribute('style','font-family:verdana; color: blue;');
        
        input_num.innerHTML = numRows;
 
        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(btnExcluirEtapa);

        //numero da etapa
        var novaCelulaNum = novaLinha.insertCell(novaLinha.cells.length);
        novaCelulaNum.appendChild(input_num);

        
        
        /**
         * Data inicio
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_emedatainicial.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_emedatainicial);
        novaCelula2.appendChild(input_emeid);


        /**
         * Data fim
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
      
        novaCelula3.appendChild(input_emedatafinal);
        
        
        /**
         *Local / Polo
         */
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);

        novaCelula4.appendChild(input_locid);
        
        
         /**
         * Custo Licitado
         */
        var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);
      
        novaCelula5.appendChild(input_emecustolicitado);
        
         /**
         * Contrapartida do Estado
         */
        var novaCelula6 = novaLinha.insertCell(novaLinha.cells.length);
      
        novaCelula6.appendChild(input_emecontrapartidaestado);
        
      
        
         if (locid == null)
            input_locid.select();

        return false;
    }


    function carregarEtapasCurso(){
    <?php
        
    if( $_REQUEST['emiid'] )
    {    
   
        $itens = $db->carregar('SELECT
                                    fi.emeid,
                                    fi.locid,
                                    fi.emedatainicial,
                                    fi.emedatafinal,
                                    fi.emecustolicitado,
                                    fi.emecontrapartidaestado
                                FROM
                                    parindigena.ensinomediointegradoetapa fi
                                WHERE
                                    fi.emiid  = ' . $_REQUEST['emiid'] . '
                                ');
       
    }

    if (!is_array($itens)) {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);
            
            $arrexpI = explode( "-", $item['emedatainicial']);
            $converteDataI = $arrexpI[2].'-'.$arrexpI[1].'-'.$arrexpI[0];
            
            $arrexpF = explode( "-", $item['emedatafinal']);
            $converteDataF = $arrexpF[2].'-'.$arrexpF[1].'-'.$arrexpF[0];

            
            echo "adicionarEtapa(" , $item['emeid'] , ",'" , $converteDataI , "','" , $converteDataF , "'," , ($item['locid'] ? $item['locid'] : "''" ),"," , str_replace(array('.',',','R$'), '', $item['emecustolicitado']),"," , str_replace(array('.',',','R$'), '', $item['emecontrapartidaestado']),");";
        }
    }
?>}
<?php if ( $_REQUEST['emiid'] ) 
{ ?>
    carregarEtapasCurso();
    $('loader-container').hide();
<?php } ?>    
 $('loader-container').hide();
function fecharJanela()
{
    if (_modificado) {
        if (!confirm('Existem dados que não foram salvos.\nDeseja fechar a janela sem salvá-los?'))
            return false;
    }

    window.close();
}

 
<?php
if( $_GET['emiid'] != '' )
{
?>    
     calculaValorCL();
     calculaValorCE();
<?php
}
?>

function abrirCadLocalPolo()
{
    return windowOpen('parindigena.php?modulo=principal/popupLocalPolo&acao=I&tipocombo=prototype',
                  'abrirCadLocalPolo',
                  'height=360,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

} 
  </script>
  
</html>
           