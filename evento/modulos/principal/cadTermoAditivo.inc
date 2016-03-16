<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

include_once APPRAIZ . "includes/classes/dateTime.inc";
 
//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();



if($_REQUEST['tdaid'] && $_REQUEST['op'] == 'Excluir'){
	$sql = "UPDATE evento.cttermoaditivo SET tdastatus = 'I' WHERE tdaid = ".$_REQUEST['tdaid'];
	$db->executar($sql);
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadTermoAditivo', "");
	die;
}


if($_POST['action']){
	salvar();
}


function salvar(){
	global $db; 
 	 
	$formataDatamA = formata_data_sql( $_REQUEST['tdadata'] );
	$formataDatamI = formata_data_sql( $_REQUEST['tdainiciovig'] );
	$formataDatamF = formata_data_sql( $_REQUEST['tdafimvig'] );
	
	$vlr = str_replace(',','.',str_replace('.','',$_REQUEST['tdavlr']));
	if(!$vlr) $vlr = 'NULL';
	else $vlr = "'".$vlr."'";		
		 
 	if(!$_REQUEST['tdaid']){
	    $sql = "INSERT INTO 
	    		evento.cttermoaditivo
	    		(
	    		 ctrid,
				 tdanum, 
				 tdadata, 
				 tdaobj, 
				 tdavlr, 
				 tdainiciovig, 
            	 tdafimvig,
            	 tdastatus
				)
				VALUES
				(
					'".$_SESSION['ctrid']."',
					'".$_POST['tdanum']."',
					'".$formataDatamA."',
					'".$_POST['tdaobj']."',
					".$vlr.",									
					'".$formataDatamI."',
					'".$formataDatamF."',
					'A'	    
				) 
	    		RETURNING tdaid";
				 
 	}elseif($_REQUEST['tdaid']){                    
		$sql = "UPDATE evento.cttermoaditivo 
				SET
						 tdanum 		= '".$_POST['tdanum']."',
						 tdadata 		= '".$formataDatamA."',
						 tdaobj 		= '".$_POST['tdaobj']."',
						 tdavlr 		= ".$vlr.",
						 tdainiciovig 	= '".$formataDatamI."',
		            	 tdafimvig		= '".$formataDatamF."'
								WHERE
					tdaid = ".$_REQUEST['tdaid']."
					RETURNING tdaid
		"; 
		
 	}
 	//dbg($sql,1);
	$tdaid = $db->pegaUm($sql);

	$db->commit();

	if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], '', $tdaid)){ 	
	}
		
			
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadTermoAditivo', "");
	die;
} 


######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$tdaid){
	global $db;

	if (!$arquivo || !$tdaid)
		return false;
		
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
				sisid,
				arqstatus
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'Termo aditivo de contrato',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y/m/d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] .",
				'A'
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela eveno.ctanexo
	$sql = "INSERT INTO evento.ctanexo 
			(
	    	ctrid, 
	    	arqid, 
	    	tpaid, 
            ancdatainclusao, 
	    	usucpf, 
	    	ancstatus, 
            tdaid 
			)VALUES(
			    ".$_SESSION['ctrid'].",
				". $arqid .",
				".ID_TERMO_ADITIVO.",
				now(),
				'".$_SESSION["usucpf"]."',
				'A',
				". $tdaid ."
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/evento/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/evento/'.floor($arqid/1000), 0777);
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

######## Fim funções de UPLOAD ########


if( $_REQUEST['tdaid'] ){
		
	$sql = "SELECT 				 
					tda.tdanum,
					to_char(tda.tdadata::date,'YYYY-MM-dd') AS tdadata, 
					tda.tdaobj, 
					tda.tdavlr  as tdavlr,	
					to_char(tda.tdainiciovig::date,'YYYY-MM-dd') AS tdainiciovig,
					to_char(tda.tdafimvig::date,'YYYY-MM-dd') AS tdafimvig
				FROM 
					evento.cttermoaditivo AS tda 
			WHERE
				tda.ctrid = ".$_SESSION['ctrid']."
				AND tda.tdaid = '".$_REQUEST['tdaid']."'
				";
	$rsDadosProcesso = $db->carregar( $sql );

}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
cabecalhoContrato( $_SESSION['ctrid'] );
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoContrato($_SESSION['ctrid']);
?> 
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script> 
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
    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" enctype="multipart/form-data">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr>	
                <td class ="SubTituloDireita" align="right">Nº Termo: </td> 
                <td> 
                 <?
				 $tdanum = $rsDadosProcesso[0]['tdanum'];
                 ?>
            	 <?= campo_texto('tdanum', 'S', $somenteLeitura, 'Nº Termo', 40, 30, '', '', 'left', '',  0, 'id="tdanum" onblur="MouseBlur(this);"' ); ?>               
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right">Data: </td>
                <td>  
                <?php 
                $tdadata = $rsDadosProcesso[0]["tdadata"];   
                ?>
                <?= campo_data( 'tdadata','S', 'S', 'Data', 'S' ); ?> 
                </td> 
            </tr>
         	         	 
            
            <tr>
	      		<td class="subtitulodireita" valign="top"> 	
	      	 		Descrição do Objeto:
	      	 	</td>
	      	 	<td align="left">
	      	 	<?php
				$tdaobj = $rsDadosProcesso[0]["tdaobj"];
	      	 	?>
	      	 		<!-- 
		      	 	<textarea name="tdaobj" title="Descrição do Objeto" rows="15" cols="80" class="text_editor_simple"><?//= $tdaobj ?></textarea>
		      	 	-->
					<div style="float:left">
		      	 	<textarea name="tdaobj" title="Descrição do Objeto" rows="15" cols="80"><?= $tdaobj ?></textarea>
		      	 	</div>
		      	 	<div style="float:left">
		      	 	&nbsp;<?php echo obrigatorio();?>
		      	 	</div>		      	 	
		      	 </td>
	        </tr>
        	<tr>
                <td class ="SubTituloDireita" align="right">Valor: </td> 
                <td> 
                 <?
				 $tdavlr = $rsDadosProcesso[0]['tdavlr'];
				 if($tdavlr) $tdavlr = number_format($tdavlr,2,",",".");   
                 ?>
            	 <?= campo_texto( 'tdavlr', 'N', 'S', 'Valor', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="tdavlr" onblur="MouseBlur(this);"'); ?>               
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Data Início Vigência: </td>
                <td>  
                <?php 
                $tdainiciovig = $rsDadosProcesso[0]["tdainiciovig"];   
                ?>
                <?= campo_data( 'tdainiciovig','S', 'S', 'Data início da vigência', 'S' ); ?> 
                </td> 
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Data Fim Vigência: </td>
                <td>  
                <?php 
                $tdafimvig = $rsDadosProcesso[0]["tdafimvig"];   
                ?>
                <?= campo_data( 'tdafimvig','N', 'S', 'Data fim da vigência', 'S' ); ?> 
                </td> 
            </tr>

		    <tr>
		        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
		        <td>     	
		 			<input name="anexo" type="file" style="text-align: left; width: 83ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" class="normal" size="81">	            
		        </td>       
		    </tr>
            
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="hidden" name="tdaid" value="<?=$_REQUEST['tdaid']; ?>" id="tdaid">
            	   <?if(!$_REQUEST['tdaid']){?>
            	   		<input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	   <?}else{?>
            	   		<input type="button" name="btnGravar" value="Alterar" id="btnGravar" onclick="enviaForm();">
            	   		&nbsp;&nbsp;
            	   		<input type="button" name="btnGravar" value="Novo" id="btnNovo" onclick="window.location.href = 'evento.php?modulo=principal/cadTermoAditivo&acao=A';">
            	   <?}?>
            	  </td>
            </tr>
      </table>
      </form> 
<?php

$sql = "SELECT 
			DISTINCT 
			('<center><img align=\"absmiddle\" src=\"/imagens/alterar.gif\"  style=\"cursor: pointer\" onclick=\"javascript: selecionarTermo('|| tda.tdaid ||' );\" title=\"Selecionar Termo Aditivo\">
			<img align=\"absmiddle\" src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"removeTermo('''||tda.tdaid||''');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>') as acao,
			CASE
			  		WHEN anc.tdaid IS NULL THEN ''
			  		WHEN anc.tdaid IS NOT NULL THEN '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" /><img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\"> </a>' 
	 		END as doc,
			tda.tdanum,
			to_char(tda.tdadata::date,'dd/MM/YYYY') AS tdadata, 
			tda.tdaobj, 
			coalesce( tda.tdavlr, 0 ) / 1 as tdavlr,
			to_char(tda.tdainiciovig::date,'dd/MM/YYYY') AS tdainiciovig,
			to_char(tda.tdafimvig::date,'dd/MM/YYYY') AS tdafimvig
		    FROM evento.cttermoaditivo tda
       		LEFT JOIN evento.ctanexo anc on anc.tdaid = tda.tdaid
       		LEFT JOIN public.arquivo arq on arq.arqid = anc.arqid      			    
			WHERE tda.tdastatus = 'A' and tda.ctrid = {$_SESSION['ctrid']}
			order by 2"; 
			
$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;Ação"," ", "TA nº", "Data", "Objeto", "Valor", "Vigência Início", "Vigência Término" ); 
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '');

?>      
	

<script type="text/javascript">

function selecionarTermo( tdaid )
{   
	window.location.href = 'evento.php?modulo=principal/cadTermoAditivo&acao=A&tdaid='+tdaid; 
}

function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "tdanum";
	campos[1] 			= "tdadata";
	campos[2]			= "tdaobj"; 
	campos[3]			= "tdavalor";
	campos[4]			= "tdainiciovig";
	campos[5]			= "tdafimvig";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "data"; 
	tiposDeCampos[2] 	= "textarea";
	tiposDeCampos[3] 	= "texto"; 
	tiposDeCampos[4] 	= "data"; 
	tiposDeCampos[5] 	= "data"; 
	

	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}

function selecionaArquivo( id ){
	var req = new Ajax.Request('evento.php?modulo=principal/cadTermoAditivo&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
								window.location.href = '?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid='+id;
							}
		});
}


function removeTermo(tdaid){
	if(confirm('Deseja excluir este item?')){
		window.location.href = 'evento.php?modulo=principal/cadTermoAditivo&acao=A&op=Excluir&tdaid='+tdaid; 
		return true;
	} else {
		return false;
	}
}


</script>