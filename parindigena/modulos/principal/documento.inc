<?php

function excluirArquivo ($arqid = null){
	global $db;
	
	if (!$arqid)
		return false;
		
	$sql = "UPDATE
			 public.arquivo
			SET
			 arqstatus = 'I'
			WHERE
			 arqid = ".$arqid;
	$db->executar($sql);
	$db->commit();
	
	return true;
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
				arqstatus,
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
				'A',
                '". $_SESSION['usucpf'] ."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
    
 
	//Insere o registro na tabela parindigena.anexos
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


function salva (){
	global $db;

 	$where = ($_REQUEST['friid'] ? 
 				"friid = {$_REQUEST['friid']}" 
 			  : 
 				($_REQUEST['frcid'] ? 
 					"frcid = {$_REQUEST['frcid']}" 
 				 :
 					"madid = {$_REQUEST['madid']}" 						 						 
 				)
 			 );	
	
	$sql = "SELECT
			 itmid
			FROM
			 parindigena.itemmonitoramento
			WHERE
			 itmstatus = 'A' AND
			 $where";
	$itmid = $db->pegaUm($sql);

	EnviarArquivo($_FILES['arquivo'],'',$itmid,$_POST['anedescricao']);
}

if ($_POST){
	salva();
}

if ($_GET['arqidDel']){
	if (excluirArquivo($_GET['arqidDel']))
		$msg = 'Operação realizada com sucesso!';
	else
		$msg = 'Não foi possível excluir o arquivo!';
		
	die('<script>
			alert(\''.$msg.'\');
			location.href = \'?modulo=principal/documento&acao=A&madid=98\';
		 </script>');
}

 	$where = ($_REQUEST['friid'] ? 
 				"friid = {$_REQUEST['friid']}" 
 			  : 
 				($_REQUEST['frcid'] ? 
 					"frcid = {$_REQUEST['frcid']}" 
 				 :
 					"madid = {$_REQUEST['madid']}" 						 						 
 				)
 			 );	
	
	$sql = "SELECT
			 itmid
			FROM
			 parindigena.itemmonitoramento
			WHERE
			 itmstatus = 'A' AND
			 $where";
	$itmid = $db->pegaUm($sql);
#dbg($sql,1);
if ($_REQUEST['friid']){
	
		$sql = "select
			 * 
            from 
             parindigena.itemmonitoramento as im 
             inner join parindigena.formacaoinicial as f 
                on f.friid = im.friid 
            where 
             f.friid= {$_REQUEST['friid']} and 
             im.itmstatus = 'A'";
          

    $rsDadosForm = $db->carregar( $sql );
	
	$res = array(   array (  "descricao" => "Formação inicial",
						    "id" 		=> "0",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
				  		  ),
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
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/documento&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}");					

}elseif ($_REQUEST['frcid']){

	$sql = "SELECT
             *  
            FROM
             parindigena.formacaocontinuada AS fi
             INNER JOIN parindigena.itemmonitoramento AS m ON m.frcid = fi.frcid
            WHERE
             fi.frcid = {$_REQUEST['frcid']} and 
             m.itmstatus = 'A'";               

    $rsDadosForm = $db->carregar( $sql );
	
	$res = array( 0 => array (  "descricao" => "Formação Continuada",
						    "id" 		=> "3",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid={$rsDadosForm[0]['frcid']}"
				  		  	   ),
				  1 => array ("descricao" => "Etapas",
							    "id"        => "5",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
				  2 => array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&frcid={$rsDadosForm[0]['frcid']}"
							   ),
		   		  3 => array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&frcid={$rsDadosForm[0]['frcid']}"
					  		   ),
		  		  4 => array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
		  		   			),
				  5 => array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
					  		   )
					);					
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/documento&acao=A&frcid={$_REQUEST['frcid']}");					
}elseif ($_REQUEST['madid']){

	$sql = "select
			 * 
            from 
             parindigena.itemmonitoramento as im 
             inner join parindigena.materialdidatico as m on m.madid = im.madid 
            where
             m.madid = {$_REQUEST['madid']} and 
             im.itmstatus = 'A'";
   
    $rsDadosForm = $db->carregar( $sql );
	
	$res = array( 0 => array (  "descricao" => "Identificação do material",
						    "id" 		=> "3",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid={$rsDadosMat[0]['madid']}"
				  		  	   ),
				  1 => array ("descricao" => "Etapas",
							    "id"        => "5",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
				  2 => array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
		   		  3 => array ("descricao" => "Documentos",
							    "id"		=> "1",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosMat[0]['madid']}"
					  		   ),
		  		  4 => array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
		  		   				),
				  5 => array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
					  		   )  		  
				);	  

	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$_REQUEST['madid']}");					
}

$estuf 		 	   = $rsDadosForm[0]['estuf'];
$_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];       
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
	<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
	<script language="javascript" type="text/javascript">
		function cadastrar_anexo(){
			if ( validar_formulario_anexo() ) {
				document.anexo.submit();
			}
		}
	
		function validar_formulario_anexo(){
			var validacao = true;
			var mensagem = 'Os seguintes campos não foram preenchidos:';
			
			document.anexo.anedescricao.value = trim( document.anexo.anedescricao.value );
						
			if ( document.anexo.anedescricao.value == '' ) {
				mensagem += '\nDescrição';
				validacao = false;
			}
			if ( document.anexo.arquivo.value == '' ) {
				mensagem += '\nArquivo';
				validacao = false;
			}
			if ( !validacao ) {
				alert( mensagem );
			}
			return validacao;
		}
	
		function excluirAnexo( arqid ){
			if ( confirm( 'Deseja excluir o Documento?' ) ) {
				location.href= window.location+'&arqidDel='+arqid;
			}
		}
		
		function cadastrar_versao( formulario ){
			if ( formulario.arquivo.value == '' ) {
				return;
			}
			formulario.submit();
		}
		
		function ltrim( value ){
			var re = /\s*((\S+\s*)*)/;
			return value.replace(re, "$1");
		}
		
		function rtrim( value ){
			var re = /((\s*\S+)*)\s*/;
			return value.replace(re, "$1");
		}
		
		function trim( value ){
			return ltrim(rtrim(value));
		}
	</script>
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
<table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<tr style="background-color: #cccccc;">
        <td>
			<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Documentos</div>
			<div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
			<table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">           	
	            <tr>
	                <td colspan = "2" align="center">
	                    <table width = "100%" class="tabela listagem">
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
									<td>
										<span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span>
									</td>
								</tr>
								<tr>
									<td>                                
										<span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Documentos</span>
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
		                    </table>				
							<!-- NOVO ANEXO -->
							<form method="post" name="anexo" enctype="multipart/form-data">
								<input type="hidden" name="submetido" id="submetido" value=1 >
								<input type="hidden" name="evento" value="cadastrar_anexo"/>
								<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
									<tr>
										<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
										<td>
											<input type="file" name="arquivo"/>
											<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
										</td>
									</tr>
									<!--
									<tr>
										<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
										<td>
										<?php
											$sql = "select taaid as codigo, taadescricao as descricao from pde.tipoanexoatividade order by taadescricao asc";
											$db->monta_combo( "taaid", $sql, 'S', '&nbsp;', '', '', '', 200, 'S' );
										?>
										</td>
									</tr>				
									<tr>
										<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
										<td>
											<select style="width: 200px;" class="CampoEstilo" name="taaid">
												<optgroup label="Instrumento Legal">
													<?php
														$sql = sprintf( "select taaid, taadescricao from pde.tipoanexoatividade where taalegal = true order by taadescricao asc" );
													?>
													<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
													<option value="<?= $tipo['taaid'] ?>"><?= $tipo['taadescricao'] ?></option>
													<?php endforeach; ?>
												</optgroup>
												<optgroup label="Instrumento de Trabalho">
													<?php
														$sql = sprintf( "select taaid, taadescricao from pde.tipoanexoatividade where taalegal = false order by taadescricao asc" );
													?>
													<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
													<option value="<?= $tipo['taaid'] ?>"><?= $tipo['taadescricao'] ?></option>
													<?php endforeach; ?>
												</optgroup>
											</select>
										</td>
									</tr>
									-->
									<tr>
										<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
										<td><?= campo_textarea( 'anedescricao', 'S', 'S', '', 70, 2, 250 ); ?></td>
									</tr>
									<tr style="background-color: #cccccc">
										<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
										<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_anexo();"/></td>
									</tr>
								</table>
							</form>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<?
$sql = "SELECT
				 DISTINCT
				 '&nbsp;<img src=\"/imagens/excluir.gif\" onclick = \"excluirAnexo('|| ar.arqid ||')\">' AS img,		 
		
				CASE
					WHEN length(arqdescricao) > 25 THEN SUBSTRING(arqdescricao, 1, 25) || '...'
					ELSE arqdescricao
				END AS arqdescricao,				
				 
				 arqnome || '.' || arqextensao
		FROM
				 parindigena.anexo an
				 INNER JOIN public.arquivo ar ON an.arqid = ar.arqid AND
				 								 (ar.arqstatus IS NULL OR
				 								  ar.arqstatus = 'A') 
				WHERE
				 an.itmid = $itmid";	 

$cabecalho = array("Ação","Descrição","Arquivo");
$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
?>	
</body>
</html>