<?php
 function salva(){
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
 				
 	$sql = sprintf("UPDATE parindigena.itemmonitoramento
		 			SET
		 			 itmprovidencia = '%s',
		 			 itmrestricao   = '%s'
		 			WHERE
		 			 itmstatus = 'A' AND
		 			 %s",
 				$_POST['itmprovidencia'],
 				$_POST['itmrestricao'],
 				$where);
 	
 	 
 	$db->executar($sql);
 	$db->commit();
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
 
if ($_POST){
	salva();
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href = window.location;	
		 </script>');
}


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
	
	$res = array( 
				    array (  "descricao" => "Formação inicial",
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
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}");					

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
	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&frcid={$_REQUEST['frcid']}");					
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

	echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$_REQUEST['madid']}");					
}

$estuf 		 	   = $rsDadosForm[0]['estuf'];
$_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];       


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
		 itmrestricao,
		 itmprovidencia
		FROM
		 parindigena.itemmonitoramento
		WHERE
		 itmstatus = 'A' AND
		$where";

$dados = $db->pegaLinha($sql);
extract($dados);
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
	<script language="JavaScript">
	//Editor de textos
	tinyMCE.init({
		theme : "advanced",
		mode: "specific_textareas",
		editor_selector : "text_editor_simple",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		width : "450px",
		entity_encoding : "raw"
		});
	</script>
	
	<script language="javascript" type="text/javascript">
		
		function cadastrar_controle(){
			if ( validar_formulario_controle() ) {
				document.controle.submit();
			}
		}
		
		function validar_formulario_controle(){
			var validacao = true;
			var mensagem = 'Os seguintes campos não foram preenchidos:';
			if ( tinyMCE.getContent('itmrestricao') == '' ) {
				mensagem += '\nRestrição';
				validacao = false;
			}
			if ( !validacao ) {
				alert( mensagem );
			}
			return validacao;
		}
	</script>
	
	<script language="javascript" type="text/javascript">
		
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
  </head>
  <body>
 <table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Restrição</div>
          <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
            <table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
           	<input type="hidden" name="submetido" id="submetido" value=1 >
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
                        <td><span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span></td>
                        </tr>
                        <tr>
                            <td>
                                
                                <span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Restrição</span>
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
                   #     if ( $_REQUEST['friid'] != '' )
                   #     {
                             
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
                    #    }
                        ?>
                    </table>
                </td>
            </tr>
	<tr>
		<td>
			<form method="post" name="controle" action="">
				<input type="hidden" name="evento" value="cadastrar_controle"/>
				<table bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Restrição:</td>
						<td>
							<textarea name="itmrestricao" id="itmrestricao" rows="10" cols="70" class="text_editor_simple"><?= trim($itmrestricao) ?></textarea>
						</td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Providência:</td>
						<td><?= campo_textarea( 'itmprovidencia', 'N', 'S', '', 70, 2, 250 ); ?></td>
					</tr>
					<tr style="background-color: #cccccc">
						<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
                        
                        <?php
                        #verificando se o status do monitoramento ja esta concluido
                        
                        $itmid = $rsDadosForm[0]['itmid'];
                        
                        $sql = "SELECT itmporcentoexec FROM parindigena.itemmonitoramento WHERE itmid = $itmid AND itmstatus = 'A'";
                        $rs = $db->pegaUm( $sql );
                        
                        if( $rs == 100 )
                        {
                            $disable = "disabled = \"true\"";
                            $msg = "&nbsp;&nbsp;&nbsp;&nbsp;Este curso já está concluido, não é possivel cadastrar uma restrição";
                        }
                        
                        ?>
                        
						<td><input type="button" name="botao" value="Salvar" <?php echo $disable; ?> onclick="cadastrar_controle();"/><?php echo $msg; ?></td>
					</tr>
				</table>
			</form>
		</td>
	</tr>
</table>
