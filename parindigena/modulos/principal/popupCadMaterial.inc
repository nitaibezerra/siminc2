<?php

$covcod = $_REQUEST['covcod'];
$estuf = $_REQUEST['estuf'];

$download  = $_REQUEST['download'];
$arquivoid = $_REQUEST['arquivoid'];


#cadastra uma Formaçao Inicial
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

if ($download == "1" && !array_key_exists('btnGravar', $_REQUEST )){
//function DownloadArquivo($param){
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arquivoid;		
        $arquivo = $db->pegaLinha($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
        if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO']= "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
        header( 'Content-type: '. $arquivo['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}


if ($_REQUEST['madid']) 
{
    
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
}


function deletaPovo( $madid )
{
     global $db;
    
    $madid = $_REQUEST['madid'];
    
    $sql = "DELETE FROM parindigena.povomaterialdidatico WHERE madid = '$madid'";
    if ( !$db->executar($sql))
    {
       return false;
    }

}

function deletaLingua( $madid )
{
     global $db;
    
    $madid = $_REQUEST['madid'];
    $sql = "DELETE FROM parindigena.linguamaterialdidatico WHERE madid = '$madid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}    
    
function deletaArranjo( $madid )
{
    global $db;
    
    $madid = $_REQUEST['madid'];
    $sql = "DELETE FROM parindigena.territoriomaterialdidatico WHERE madid = '$madid'";
    if ( !$db->executar($sql))
    {
       return false;
    }
}

function inserePovo( $madid, $povide)
{
    global $db;

    if ($_REQUEST['madid'] != '' )
    {
        $madid = $_REQUEST['madid'];
    }
    $sql = "INSERT INTO 
                parindigena.povomaterialdidatico
                                               (
                                                    povid,
                                                    madid
                                                )
                VALUES
                      (
                          '$povide',
                          '$madid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}

function insereLingua( $madid, $linide )
{    
     global $db;
    //$linid = $_REQUEST['linid'][$i];
    if ($_REQUEST['madid'] != '' )
    {
        $madid = $_REQUEST['madid'];
    }
    $sql = "INSERT INTO 
                parindigena.linguamaterialdidatico
                       (
                            linid,
                            madid
                            )
                VALUES
                      (
                          '$linide',
                          '$madid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
}
    
function insereArranjo( $madid, $teride )
{    
     global $db;
   
    if ($_REQUEST['madid'] != '' )
    {
        $madid = $_REQUEST['madid'];
    }
    $sql = "INSERT INTO 
                parindigena.territoriomaterialdidatico
                                               (
                                                    terid,
                                                    madid
                                                )
                VALUES
                      (
                          '$teride',
                          '$madid'
                      )";
    if( !$db->executar( $sql ))
    {
        return false;
    }
    $db->commit();
           
}



if (array_key_exists('btnGravar', $_REQUEST ) ) 
{    
	
   $areid                  		= $_REQUEST['areid'];
   $tipid                  		= $_REQUEST['tipid'];
   $nivid                  		= $_REQUEST['nivid'];        
   $itmnome                		= $_REQUEST['itmnome'];       
   $madautoriacoletiva     		= $_REQUEST['madautoriacoletiva'];
   $madautor             		= $_REQUEST['madautor'];
   $madtiragem              	= $_REQUEST['madtiragem'];      
   $madavaliacaocapema      	= $_REQUEST['madavaliacaocapema'];
   $madobservacaocapema    		= $_REQUEST['madobservacaocapema'];
   $madcustoprevisto       		= $_REQUEST['madcustoprevisto'];
   $madcontrapartidaestado 		= $_REQUEST['madcontrapartidaestado'];
   $madobjetocontrapartida 		= $_REQUEST['madobjetocontrapartida'];
   $madvalorunitario       		= $_REQUEST['madvalorunitario'];
   $madparceria            		= $_REQUEST['madparceria'];

    if( $erro != 1 )
    {    
   
        if ( !$_REQUEST['madid'] ) 
        {    
                 //INSERT
                 $sql = "INSERT INTO 
                                parindigena.materialdidatico ( 
                                                 areid, 
                                                 tipid, 
                                                 nivid,
                                                 madautoriacoletiva, 
                                                 madautor,
                                                 madtiragem,
                                                 madavaliacaocapema,
                                                 madobservacaocapema,
                                                 madcustoprevisto,
                                                 madcontrapartidaestado,
                                                 madobjetocontrapartida,
                                                 madvalorunitario,
                                                 madparceria,
                                                 madstatus
                                                 )
         
                               VALUES (                                                              
                                       '$areid',
                                       '$tipid',
                                       '$nivid',
                                       '$madautoriacoletiva',                                         
                                       '$madautor',
                                       '$madtiragem',
                                       '$madavaliacaocapema',
                                       '$madobservacaocapema',
                                       '$madcustoprevisto',
                                       '$madcontrapartidaestado',
                                       '$madobjetocontrapartida',
                                       '$madvalorunitario',
                                       '$madparceria',
                                       'A'
                                       ) 
                                       returning madid";                            
                   
                   if ( $madid = $db->pegaUm( $sql ) )
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
                                   inserePovo( $madid, $povide );
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
                                       insereLingua( $madid, $linide );
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
                                   insereArranjo( $madid, $teride );
                               }               
                           }
                        }
                        // Inserindo a Formacao Inicial no itemmonitoramento.       
                       
            
                        $covcod = $_REQUEST['covcod_disable'];
                        $grpid = 3;
                        $estuf = $_REQUEST['estuf'];
                        
                        
                 
                        $sql = "INSERT INTO
                                    parindigena.itemmonitoramento (
                            
                                        itmnome,
                                        itmstatus,      
                                        usucpf,         
                                        covcod ,        
                                        grpid,          
                                        madid,          
                                        estuf           
                                        )
                                VALUES
                                    (
                                        '$itmnome',
                                        'A',
                                        '{$_SESSION['usucpf']}',
                                        '$covcod',
                                        '$grpid',
                                        '$madid',
                                        '$estuf'                               
                                    )
                                    RETURNING itmid";
                        $itmid = $db->pegaUm($sql);
                        $db->commit();
                   }
    
                   
                    $numListaAnexo = count($_REQUEST['descricao']);
                   
                   for($i = 0; $i < $numListaAnexo; $i++)
                   {
                       if ( $_REQUEST['anxid'][$i] == '' )
                       {
                            //criado um array de files simulando um array nativo de $_FILES para padronizaçao com a funcao            
                            $_arrFILES = array();
                            $_arrFILES[$i]['name']     = $_FILES['arqid']['name'][$i];
                            $_arrFILES[$i]['type']     = $_FILES['arqid']['type'][$i];
                            $_arrFILES[$i]['tmp_name'] = $_FILES['arqid']['tmp_name'][$i];
                            $_arrFILES[$i]['error']    = $_FILES['arqid']['error'][$i];
                            $_arrFILES[$i]['size']     = $_FILES['arqid']['size'][$i];
                            
                            $descricao = $_REQUEST['descricao'][$i];
                            
                            EnviarArquivo($_arrFILES[$i], '', $itmid, $descricao);
                       }
                   }
                        
             } 
             else 
             {
                 $sql = " UPDATE parindigena.materialdidatico SET
                               areid                  = '$areid',
                               tipid                  = '$tipid', 
                               nivid                  = '$nivid',
                               madautoriacoletiva     = '$madautoriacoletiva' , 
                               madautor               = '$madautor', 
                               madtiragem             = '$madtiragem',
                               madavaliacaocapema     = '$madavaliacaocapema',
                               madobservacaocapema    = '$madobservacaocapema', 
                               madcustoprevisto       = '$madcustoprevisto',
                               madcontrapartidaestado = '$madcontrapartidaestado', 
                               madobjetocontrapartida = '$madobjetocontrapartida',
                               madvalorunitario       = '$madvalorunitario',
                               madparceria            = '$madparceria'         
                           WHERE
                               madid = '{$_REQUEST['madid']}'
                           "; 
    
                  $db->executar( $sql );    
                   $covcod = $_REQUEST['covcod_disable'];
                  $sql = "UPDATE parindigena.itemmonitoramento
                         SET
                             itmnome = '$itmnome',
                             covcod = '{$covcod}'
                         WHERE
                            madid = '{$_REQUEST['madid']}'";
                            
                  $db->executar($sql);
                                    
                 
                  //DELETA TUDO E INSERE DE NOVO
                deletaPovo( $madid );
                $numPovo = count($_REQUEST['povid']);    
                
                if ($numPovo > 0)
                {                        
                   for ( $i = 0; $i < $numPovo; $i++ )
                   {    
                        if($_REQUEST['povid'][$i] != '')
                        {                  
                           $povide = $_REQUEST['povid'][$i];
                           inserePovo( $madid, $povide );
                        }  
                   }
                
                }
                 
                deletaLingua( $madid ); 
                $numLingua = count($_REQUEST['linid']);
                if ( $numLingua > 0 )
                {                        
                   for ( $i = 0; $i < $numLingua; $i++ )
                   {   
                        if($_REQUEST['linid'][$i] != '')
                        {                  
                           $linide = $_REQUEST['linid'][$i];
                           insereLingua( $madid, $linide );
                        }   
                   }
                }
               
               deletaArranjo( $madid );
                $numArranjo = count($_REQUEST['terid']);
                
                if ($numArranjo > 0 )
                {                        
                   for ( $i = 0; $i < $numArranjo; $i++ )
                   {    
                       if($_REQUEST['terid'][$i] != '')
                       {
                           $teride = $_REQUEST['terid'][$i];
                           insereArranjo( $madid, $teride );
                       }               
                   }
                }
                $db->commit();            
    
                   //$numAnexos = count($_FILES['arqid']['name']);
                   $numListaAnexo = count($_REQUEST['descricao']);
                   
                   for($i = 0; $i < $numListaAnexo; $i++)
                   {
                       //dbg( $itmid );
                       //die();
                       if ( $_REQUEST['anxid'][$i] == '' )
                       {
                            //criado um array de files simulando um array nativo de $_FILES para padronizaçao com a funcao                   
                           $_arrFILES = array();
                           $_arrFILES[$i]['name']     = $_FILES['arqid']['name'][$i];
                           $_arrFILES[$i]['type']     = $_FILES['arqid']['type'][$i];
                           $_arrFILES[$i]['tmp_name'] = $_FILES['arqid']['tmp_name'][$i];
                           $_arrFILES[$i]['error']    = $_FILES['arqid']['error'][$i];
                           $_arrFILES[$i]['size']     = $_FILES['arqid']['size'][$i];
                           
                           $descricao = $_REQUEST['descricao'][$i];
                           EnviarArquivo($_arrFILES[$i], '', $itmid, $descricao);
                       }
                   }
     
             }
    }
    echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';  
                  
}
//FIM DO INSERT PARA o MATERIAL DIDATICO.

if (array_key_exists('excluir', $_REQUEST)) 
{
    //echo("<script>alert('n deu');\n</script>");
    $sql = "DELETE FROM parindigena.materialdidaticoetapa WHERE maeid = {$_REQUEST['excluir']}";
   // $db->executar('DELETE FROM cte.itenslaboratorio WHERE ilbid = ' . (integer) $_REQUEST['ilbid'][$k]);
 
    $deleteItemMat = $db->executar( $sql );
    $db->commit();
     
   // die('null');
}



function EnviarArquivo($arquivo,$dados=null,$itmid, $descricao){
	global $db;

	if (!$arquivo || !$itmid)
    {
		return false;
	}

	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
 
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'$descricao',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y-m-d')."',
				'".date('H:i:s')."',
                '". $_SESSION['usucpf'] ."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
    
 
	//Insere o registro na tabela demandas.anexos
	$sql = "INSERT INTO parindigena.anexo 
			(
				arqid,
				itmid

			)VALUES(
			    ". $arqid .",
				". $itmid ."		
			);";
	$db->executar($sql);
	
    
    
	if(!is_dir('../../arquivos/parindigena/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/parindigena/'.floor($arqid/1000), 0777);
        
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}
	
	
	$db->commit();
	return true;

}


$res = array( 0 => array (  "descricao" => "Identificação do material",
						    "id" 		=> "3",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid={$rsDadosMat[0]['madid']}"
				  		  )
			);	  
				
if ($_REQUEST['madid']){
	$perfis = arrayPerfil(); 		  
	if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){	
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "5",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosMat[0]['madid']}"
					  		   ),
		  		   array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
		  		   				),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}else{
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "5",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosMat[0]['madid']}"
					  		   ),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}
	
}

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid={$rsDadosMat[0]['madid']}");

include APPRAIZ."includes/arquivo.inc";
//include APPRAIZ."demandas/modulos/principal/cadDemanda.inc";
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
<body>
<form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" enctype="multipart/form-data">
<!-- Inicio da 1 tabela -->
	<table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr style="background-color: #cccccc;">
			<td>
				<div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Identificação do Material</div>
				<div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
				<table id="EtapasProducao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">            
					<tr>
						<td colspan = "6" align="center" >
<!-- Inicio da 2 tabela -->						
							<table width = "900" class="tabela listagem" >
								<tr>
									<td>
										<span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span>
									</td>
								</tr>
								<tr>
									<td>                                
										<span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Material Didático</span>
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
								if ( $_REQUEST['madid'] != '' )
								{                             
									$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosMat[0]["covcod"]}'";
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
										<span style="display: block;font-size:100%;color:black; font-weight:bold;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?php  echo $rsDadosMat[0]["itmnome"]; ?></span>
									</td>
								</tr>
								<?php
								}
								?>
<!-- Fim da 2 tabela -->								
							</table>
						</td>
					</tr>            
					<tr>
						<td colspan="6" align='left' class="SubTituloDireita">
							<div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Convênio</div>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Escolha o Convênio:</td>
						<td align="left" colspan="5">
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
		                            pc.coitipo = 'F'";
		         
		                if ( $_REQUEST['covcod'] != '' )
		                {
		                    $covcod = $_REQUEST['covcod'];
		                }
		                else
		                {
		                    $covcod = $rsDadosMat[0]["covcod"];
		                }
		                            
		                $db->monta_combo('covcod', $sql, $somenteLeituraUnidade, "Selecione...", 'mostrarSubacao', '', '', '390', 'S', 'covcod');
						?>
						</td>
					</tr>
					<tr id="trSubacoes" style="display:none;">
		                <td class="subtitulodireita">Escolha uma subacao:</td>
		                <td align="left" id="tdSubacoes" colspan="5">
		                              
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Nome do Material:</td>
						<td colspan="5" align="left">
							<?php $itmnome = $rsDadosMat[0]["itmnome"]; ?>
							<?= campo_texto('itmnome', 'S', 'S', '', 75, 50, '', '', 'left', '',  0, 'id="itmnome" onblur="MouseBlur(this);"' ); ?>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Tipo do Material:</td>
						<td colspan="5" align="left">
						<?php                
		                $sql = "SELECT tipid as codigo,
		                               tipdescricao as descricao
		                        FROM
		                               parindigena.tipomaterial";
		                $tipid = $rsDadosMat[0]["tipid"];
		                $db->monta_combo('tipid', $sql, 'S', "Selecione...", '', '', '', '390', 'S');
		                ?>
						</td>              
					</tr>
					<tr>
						<td colspan="6" align='left' class="SubTituloDireita">
							<div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Dados de Atendimento do Material</div>
						</td>
					</tr>          
					<tr>
						<td class="subtitulodireita">Povos Indígenas Atendidos:</td>
						<td colspan="5" align="left" >
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
			                                            parindigena.povomaterialdidatico AS pf
			                                         INNER JOIN parindigena.povoindigena p ON pf.povid = p.povid
			                                         WHERE pf.madid = '{$_REQUEST['madid']}'";
			                            if( $_REQUEST['madid'] != '' )
			                            {
			                                $povid    = $db->carregar($sqlPovid);
			                            }
			                
			                combo_popup( "povid", $sql, "Selecione o(s) Povo(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
			                ?>
							<img src="../imagens/obrig.gif">
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Línguas:</td>
						<td colspan="5" align=left >
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
			                                  parindigena.linguamaterialdidatico AS lf
			                             INNER JOIN parindigena.linguaindigena l ON lf.linid = l.linid
			                             WHERE 
			                                  lf.madid = '{$_REQUEST['madid']}'";
			                            if( $_REQUEST['madid'] != '' )
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
						<td colspan="5" align="left">
			                <?php
			                $sql = "SELECT 
			                           terid as codigo, 
			                           ternome as descricao
			                        FROM 
			                            parindigena.territorioetnoeducacional as territorio
			                            ORDER BY descricao";
			               
			                $sqlArrid = "SELECT 
			                                 af.terid as codigo,
			                                 a.ternome as descricao
			                            FROM 
			                                 parindigena.territoriomaterialdidatico AS af
			                            INNER JOIN parindigena.territorioetnoeducacional a ON af.terid = a.terid
			                            WHERE 
			                                 af.madid = '{$_REQUEST['madid']}'
			                                 ORDER BY descricao";
			                if( $_REQUEST['madid'] != '' )
			                {
			                    $terid    = $db->carregar($sqlArrid);            
			                }
			  
			                combo_popup( "terid", $sql, "Selecione o(s) Território(s) Etno-educaciona(l)(is)", "192x400", 0, array(), "", "S", false, false, 5, 310 );			               
			                ?>
							<img src="../imagens/obrig.gif">
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Área temática</td>
						<td colspan="5" align="left">
			                <?php		                
			                $sql = "SELECT areid as codigo,
			                               aredescricao as descricao
			                        FROM
			                               parindigena.areatematica";
			                $areid = $rsDadosMat[0]["areid"];
			                $db->monta_combo('areid', $sql, 'S', "Selecione...", '', '', '', '390', 'S');
			                ?>
						</td>              
					</tr>             
					<tr>
						<td class="subtitulodireita" > Autoria:</td>
						<td align="left" colspan="5">
							<?php $madautoriacoletiva = $rsDadosMat[0]["madautoriacoletiva"]; ?>
							<input type="radio" name="madautoriacoletiva" value=true <?php if ( $madautoriacoletiva == true ) { ?> checked = "checked" <?php }  ?>>Coletiva<br>
							<input type="radio" name="madautoriacoletiva" value=false <?php if ( $madautoriacoletiva == false ) { ?> checked = "checked" <?php }  ?>>Individual
						</td>                
					</tr>
					<tr>
						<td class="subtitulodireita">Nome do(s) Autor(es):</td>
						<td colspan="5" align="left">
							<?php $madautor = $rsDadosMat[0]["madautor"]; ?>                   
							<?= campo_textarea( 'madautor', 'S', 'S', 'Autor ', 100 , 8, 2000, 'id="madautor" onKeyPress = "return controlar_foco( event );"', $acao = 0, $txtdica = '', $tab = false ); ?>                    
						</td>                    
             		</tr>             
					<tr>
						<td class="subtitulodireita">Tiragem:</td>
						<td colspan="5" align="left">
							<?php $madtiragem = $rsDadosMat[0]["madtiragem"]; ?>
							<?= campo_texto('madtiragem', 'S', $somenteLeitura, '', 6, 4, '####', '', 'left', '',  0, 'id="madtiragem" onblur="MouseBlur(this);"' ); ?>
						</td>
					</tr>   
					<tr>
						<td colspan="2" class="subtitulodireita">Nível de Ensino:</td>
						<td colspan="4" align="left">
			                <?php			                
			                $sql = "SELECT nivid as codigo,
			                               nivdescricao as descricao
			                        FROM
			                               parindigena.nivelensino
			                               ORDER BY codigo";
			                $nivid = $rsDadosMat[0]["nivid"];
			                $db->monta_combo('nivid', $sql, 'S' , "Selecione...", '', '', '', '390', 'S');
			                ?>
						</td>              
					</tr>
					<tr>
						<td class="subtitulodireita"> Avaliação CAPEMA:</td>
						<td align="left">
							<?php $madavaliacaocapema = $rsDadosMat[0]["madavaliacaocapema"]; ?>
							<input type="radio" name="madavaliacaocapema" value="S" <?php if ( $madavaliacaocapema == "S" ) { ?> checked = "checked" <?php }  ?>>Aprovado<br>
							<input type="radio" name="madavaliacaocapema" value="N" <?php if ( $madavaliacaocapema == "N" ) { ?> checked = "checked" <?php }  ?>>Aprovado com Ressalva<br>
							<input type="radio" name="madavaliacaocapema" value="R" <?php if ( $madavaliacaocapema == "R" ) { ?> checked = "checked" <?php }  ?>>Reprovado<br>
							<input type="radio" name="madavaliacaocapema" value="V" <?php if ( $madavaliacaocapema == "V" ) { ?> checked = "checked" <?php }  ?>>Não Avaliado
						</td>
						<td>Observação</td>
						<td colspan="3" align="left">
							<?php $madobservacaocapema = $rsDadosMat[0]["madobservacaocapema"]; ?>							
							<TEXTAREA COLS="56" ROWS="3" NAME="madobservacaocapema" onblur="MouseBlur(this);textCounter(this.form.madobservacaocapema,this.form.no_entobs,500);if(this.value.length>(MAXLEN=500))this.value=this.value.substr(0,MAXLEN)" onkeydown="textCounter(this.form.madobservacaocapema,this.form.no_entobs,500);" onkeyup="textCounter(this.form.madobservacaocapema,this.form.no_entobs,500);"><?php echo $madobservacaocapema; ?></TEXTAREA><br>							
							<input readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_entobs" size="6" maxlength="6" value="500" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font>
						</td>                    
					</tr>
					<tr>              
						<td colspan="6"><br />
<!-- Inicio da 3 tabela -->
							<table align="center" border="0" width="500" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
								<tr style="background-color: #cccccc;">
									<td>
										<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Anexos da Avaliação</div>
										<div style="margin: 0; padding: 0; height: 100px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
<!-- Inicio da 4 tabela -->										
											<table id="anexosAjax" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
												<colgroup>
													<col width="20%" />
													<col width="40%" />
													<col width="40%" />
												</colgroup>        
												<tr style="text-align: center">
													<td style="padding: 2px; font-weight: bold;">Opções</td>
													<td style="padding: 2px; font-weight: bold;">Descrição</td>
													<td style="padding: 2px; font-weight: bold;">Arquivo</td>
												</tr>
											</table>
<!-- Final da 4 tabela -->											
										</div>
										<div style="padding: 5px 0px 0px 5px"></div>        	
										<span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarAnexo('','','','');"><img src="/imagens/gif_inclui.gif" align="middle" style="border: none" /> Inserir Anexos</span>										<br />
        							</td>
								</tr>
<!-- Final da 3 tabela -->								
							</table>
							<tr>
								<td class="subtitulodireita"> Custo Previsto R$: </td>
								<td colspan="5" align="left">
									<?php $madcustoprevisto = $rsDadosMat[0]["madcustoprevisto"]; ?>
									<?= campo_texto('madcustoprevisto', 'N', $somenteLeitura, '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="madcustoprevisto" onblur="MouseBlur(this);"' ); ?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita"> Contrapartida do Estado R$: </td>
								<td colspan="5" align="left">  
									<?php $madcontrapartidaestado = $rsDadosMat[0]["madcontrapartidaestado"]; ?>
									<?= campo_texto('madcontrapartidaestado', 'N', $somenteLeitura, '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="madcontrapartidaestado" onblur="MouseBlur(this);"' ); ?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita"> Objeto da Contrapartida: </td>
								<td colspan="5" align="left">  
									<?php $madobjetocontrapartida = $rsDadosMat[0]["madobjetocontrapartida"]; ?>
									<?= campo_texto('madobjetocontrapartida', 'N', $somenteLeitura, '', 90, 200, '', '', 'left', '',  0, 'id="madobjetocontrapartida" onblur="MouseBlur(this);"' ); ?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Valor Unitário do Material R$:</td>
								<td colspan="5" align="left">
									<?php $madvalorunitario = $rsDadosMat[0]["madvalorunitario"]; ?>
									<?= campo_texto('madvalorunitario', 'N', $somenteLeitura, '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="madvalorunitario" onblur="MouseBlur(this);"' ); ?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita"> Parcerias:</td>
								<td colspan="5" align="left">
									<?php $madparceria = $rsDadosMat[0]["madparceria"]; ?>                   
									<TEXTAREA COLS="56" ROWS="3" NAME="madparceria" onblur="MouseBlur(this);textCounter(this.form.madparceria,this.form.no_entobs_par,500);if(this.value.length>(MAXLEN=500))this.value=this.value.substr(0,MAXLEN)" onkeydown="textCounter(this.form.madparceria,this.form.no_entobs_par,500);" onkeyup="textCounter(this.form.madparceria,this.form.no_entobs_par,500);"><?php echo $madparceria; ?></TEXTAREA>
									<br>
									<input readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_entobs_par" size="6" maxlength="6" value="500" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font>
								</td>
							</tr>           
							<div id="loader-container">
								<div id="loader">
									<img src="../imagens/wait.gif" border="0" align="middle">
									<span>Aguarde! Carregando Dados...</span>
								</div>
							</div>
<!-- Inicio da 5 tabela -->							
							<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
								
								<?php
								$perfis = arrayPerfil(); 
								if( !in_array( PARIND_SUPER_USUARIO, $perfis ) &&
									!in_array( PARIND_ADMINISTRADOR, $perfis ) &&
									!in_array( PARIND_COORDENACAO, $perfis) &&
									!in_array( PARIND_EQUIPE_TECNICA, $perfis) )
								{
								?>
								<tr style="background-color: #cccccc;">
									<td class="SubTituloEsquerda"  align="left"> 
									</td>
								</tr> 
								<?php	
								} else {
								?>
								<tr style="background-color: #cccccc;">
									<td class="SubTituloEsquerda" colspan="2" align="left">     
										<input type="hidden" name="download"   value="0">
										<input type="hidden" name="arquivoid"  value="0">
										<input type="submit" name="btnGravar"  value="Gravar"/>
										<input type="button" name="btnCancel"  value="Fechar" onclick="fecharPopup();"/>
									</td>
								</tr> 	  
								<?php } ?>
<!-- Fim da 5 tabela -->								
							</table>						
					</table>
				</div>
			</td>
		</tr>
	</table>
</form>
</body>
  
<script type="text/javascript">

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
  
  //Funçao que abre a popup para listar os laboratorios cadastrados.
 
function validaForm()
{
  
    var madautor = document.getElementById('madautor');
    marcado = -1
    
    if(formulario.covcod_disable.value == '')
    {
        alert('Selecione um convênio');
        formulario.covcod_disable.focus();
        return false;
    }
    if(formulario.itmnome.value == '')
    {
        alert('Selecione um Nome para o Material');
        formulario.itmnome.focus();
        return false;
    }
    if ((formulario.tipid.value == '') || (formulario.tipid.value == null))
    {
       alert('Selecione um Tipo de material');
       formulario.tipid.focus();
       return false;
    }
    
    if ((formulario.areid.value == '') || (formulario.areid.value == null))
    {
       alert('Selecione uma Área temática');
       formulario.areid.focus();
       return false;
    }
    if( madautor.value.length > 1000 ){
    	alert('Campo "Autor" muito longo maximo de 1000 caracteres.');
    	madautor.focus;
    	return false;
    }
    if ((formulario.madtiragem.value == '') || (formulario.madtiragem.value == null))
    {
       alert('Campo "tiragem" obrigatório');
       formulario.madtiragem.focus();
       return false;
    }
    if ((formulario.nivid.value == '') || (formulario.nivid.value == null))
    {
       alert('Selecione um Nível de Ensino');
       formulario.nivid.focus();
       return false;
    }
      	
	for (i=0; i<formulario.madavaliacaocapema.length; i++) {
	
		if (formulario.madavaliacaocapema[i].checked) {
			marcado = i			
		}
		
		if(marcado == -1){
		
			alert('Selecione uma avaliacao CAPEMA');        	
        	return false;
		}		
	}
	
    divElement=document.getElementById('itensEtapasProducao');
    inputs = divElement.getElementsByTagName('input');
    var datai;
    var dataf;
    for(i=0;i<inputs.length;i++)
    {
      if (inputs[i].name == 'maedatainicial[]'){
          datai = inputs[i]; 
      }
      if (inputs[i].name == 'maedatafinal[]'){
          dataf = inputs[i];
      } 
      if ((datai != null && datai.value != '') && (dataf != null && dataf.value !='') ){
          if (!validaDataMaior(datai, dataf)){
              alert('Data inicial da etapa nao pode ser maior que a data final');
              datai.focus();
              return false;
          }
      }
    }
    return processaCombosPopup();
}


 function fecharPopup()
{
    window.close();
}
 
    this._closeWindows = false;
    var _modificado    = false;


    function popup_arquivo( madid, nome, width, height )
		{
           
			window.open( '../geral/popup_arquivoupload.php?madid=' + madid + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
		}

<?php 
if ( $_REQUEST['madid'] ) 
{ 
?>
    carregarAnexosAvaliacao();
//    carregarEtapasProducao();
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

function adicionarAnexo(anxid, descricao, arqid, id)
    {
        
        var input_descricao;
        var input_anxid;
        var input_arqid;        
        var input_itmid;
        
        var linkExcluir = document.createElement('a');
        linkExcluir.setAttribute('href', '#');
        linkExcluir.onclick = function()
        {
            removerEtapa(this.parentNode.parentNode.getAttribute("id"), anxid);
        }
       	
        var btnExcluirEtapa = document.createElement('img');
        btnExcluirEtapa.src     = '/imagens/excluir.gif';
        btnExcluirEtapa.setAttribute('border', '0');
        
        linkExcluir.appendChild(btnExcluirEtapa);        
        
        var link = document.createElement('a');
        link.setAttribute('href', '#');        
        link.onclick = function()
        {
        	removerEtapa(this.parentNode.parentNode.getAttribute("id"), anxid);
        }        
        
        <?$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($anexo['anxid']/1000) .'/'. $anexo['anxid'];?>
        
        var btnAnexo = document.createElement('img');
        btnAnexo.src = '/imagens/consultar.gif';
        btnAnexo.setAttribute('border', '0');
        
        var link = document.createElement('a');
        link.setAttribute('href', '#');
        link.onclick = function()
        {
        	document.formulario.download.value="1";
        	document.formulario.arquivoid.value=id;
        	document.formulario.submit();
        }        
        link.appendChild(btnAnexo);
        
        var btnpixel = document.createElement('img');
        btnpixel.src = '/imagens/pixel.gif';
        
        // descricao
        input_descricao    = document.createElement('input');
        input_descricao.setAttribute('type', 'text');
        input_descricao.setAttribute('class', 'normal');
        input_descricao.setAttribute('name', 'descricao[]');
        input_descricao.setAttribute('maxlength', '100');
        input_descricao.setAttribute('size', '30');
        input_descricao.setAttribute('value', descricao);
      	        
        //hidden, id da etapa
        input_anxid     = document.createElement('input');
        input_anxid.setAttribute('type', 'hidden');
        input_anxid.setAttribute('name', 'anxid[]');
        input_anxid.setAttribute('value', anxid);

        //combo das etapas
        
        input_arqid   = document.createElement('input');
        input_arqid.setAttribute('class', 'normal');
        input_arqid.setAttribute('name', 'arqid[]');
        input_arqid.setAttribute('type', 'file');
        input_arqid.setAttribute('size', '40');
        input_arqid.setAttribute('value', arqid );

        var tabelaanexosAjax = $('anexosAjax');
        var novaLinha                    = tabelaanexosAjax.insertRow(tabelaanexosAjax.rows.length);
        novaLinha.id                     = 'linhaanexosAjax' + tabelaanexosAjax.rows.length + 1;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(linkExcluir);
        novaCelula0.appendChild(btnpixel);
        novaCelula0.appendChild(link);        
        
        /**
         * Botao em branco (separador)
         */
        novaCelula0.appendChild(btnpixel);
                
        /**
         * Botao anexo
         */
        novaCelula0.appendChild(link);
        
        /**
         * Data inicio
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_descricao.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_descricao);
        novaCelula2.appendChild(input_anxid);

        /**
         * Data fim
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
      
        novaCelula3.appendChild(input_arqid);

        return false;
    }

    
    function carregarAnexosAvaliacao(){<?php
        
    if( $_REQUEST['madid'] )
    {    
   
        $anexos = $db->carregar('SELECT
                                    an.anxid,
                                    an.arqid,
                                    an.itmid,
                                    pa.arqdescricao,
                                    pa.arqnome,
                                    pa.arqextensao                                    
                                FROM
                                    parindigena.anexo an
                                INNER JOIN public.arquivo AS pa ON pa.arqid = an.arqid
                                    WHERE
                                an.itmid  = ' . $itmid . '
                                ORDER BY
                                    an.arqid ASC');
    }

    if (!is_array($anexos)) 
    {
        echo 'return false;';
    } else {
        while (list(, $anexo) = each($anexos)) {
            $anexo = array_map('addslashes', $anexo);
            
            
            $arquivo = $anexo['arqnome'].'.'.$anexo['arqextensao'];
            
            
            echo "adicionarAnexo(", $anexo['anxid'] , ",'" , $anexo['arqdescricao'] , "','" , $arquivo , "'," .$anexo['arqid'].");";
        }
    }

?>
} 
</script>
</html>

