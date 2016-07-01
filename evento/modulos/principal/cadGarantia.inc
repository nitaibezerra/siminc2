<?php
include_once APPRAIZ . "includes/classes/dateTime.inc";
 

//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();




function salvar(){
	global $db; 
 	 
	$formataDatamI = formata_data_sql( $_REQUEST['ctrdtinigar'] );
	$formataDatamF = formata_data_sql( $_REQUEST['ctrdtfimgar'] ); 
	
	$ctrvlr = str_replace(',','.',str_replace('.','',$_REQUEST['ctrvlr']));
	if(!$ctrvlr) $ctrvlr = 'NULL';
	else $ctrvlr = "'".$ctrvlr."'";	
	
	                    
	$sql = "UPDATE evento.ctcontrato 
			SET
				ctrvlrgarantia	= ".$ctrvlr.",
				mogid			= ".$_POST['mogid'].",
				ctrdtinigar		= '".$formataDatamI."', 
				ctrdtfimgar		= '".$formataDatamF."', 
				ctrobsgar		= '".$_POST['ctrobsgar']."',
				ctrobservacao		= '".$_POST['ctrobservacao']."'
			WHERE
				ctrid = ".$_SESSION['ctrid']."
				RETURNING ctrid
	"; 
		
 	//dbg($sql,1);
 	
	$ctrid = $db->pegaUm($sql);

	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadGarantia', "");
	die;
} 


if($_POST['action']){
	salvar();
}

// busca dadas para edição			
$sql = "SELECT      		
	    		ctrvlrgarantia as ctrvlr,
       			ctr.mogid, 
       			to_char(ctr.ctrdtinigar::date,'YYYY-MM-dd') AS ctrdtinigar,
       			to_char(ctr.ctrdtfimgar::date,'YYYY-MM-dd') AS ctrdtfimgar,       		
       			ctr.ctrobsgar,
       			ctr.ctrobservacao
  		FROM evento.ctcontrato ctr 
		WHERE
			ctr.ctrid = '".$_SESSION['ctrid']."'
			";
$rsDadosContrato = $db->carregar( $sql );
	




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
    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr>
                <td class="SubTituloDireita" align="right">Modalidade: </td> 
                <td> 
                 <?
                 $sql = "SELECT 
                            mogid AS codigo, 
                            mogdsc AS descricao
                        FROM
                            evento.ctmodalidadegarantia order by codigo";
				 $mogid = $rsDadosContrato[0]['mogid'];
				 
				 $db->monta_combo('mogid', $sql, 'S', "Selecione...", '', '', '', '200', 'N', 'mogid',false,null,'Modalidade');
                 ?>
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right">Valor:</td>
                <td>
                 <?
                 $ctrvlr = $rsDadosContrato[0]['ctrvlr'];  
                 if($ctrvlr) $ctrvlr = number_format($ctrvlr,2,",",".");                             
                 ?>   
                 <?= campo_texto( 'ctrvlr', 'N', $somenteLeitura, 'Valor', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="ctrvlr" onblur="MouseBlur(this);"'); ?>       
                 <?//= campo_texto('ctrvlrgarantia', 'S', $somenteLeitura, 'Valor da Garantia', 20, 200, '', '', 'left', '',  0, 'id="ctrvlrgarantia" onblur="MouseBlur(this);"' ); ?>
                 
                </td>
                         
            </tr>

             
        	<tr>
                <td class ="SubTituloDireita" align="right">Observação: </td> 
                <td> 
                 <?
				 $ctrobsgar = $rsDadosContrato[0]['ctrobsgar'];
                 ?>
            	 <?= campo_texto('ctrobsgar', 'N', $somenteLeitura, 'Observação', 100, 200, '', '', 'left', '',  0, 'id="ctrobsgar" onblur="MouseBlur(this);"' ); ?>
                    	  	                
                </td>
            </tr>
             
           
            <tr>
                <td class ="SubTituloDireita" align="right">Data de Início:</td>
                <td>
                <?php 
                $ctrdtinigar = $rsDadosContrato[0]["ctrdtinigar"];   
                ?>
                <?= campo_data( 'ctrdtinigar','N', 'S', 'Data de início', 'S' ); ?>
                
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Data de Término</td>
                <td>
                <?php 
                $ctrdtfimgar = $rsDadosContrato[0]["ctrdtfimgar"];   
                ?>
                <?= campo_data( 'ctrdtfimgar','N', 'S', 'Data de Término', 'S' ); ?>
                
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Observação</td>
                <td>
                <?php 
                $ctrobservacao = $rsDadosContrato[0]["ctrobservacao"];   
                ?>
                <?= campo_textarea('ctrobservacao', 'N', 'S', 'Observação', 73, 5, 2000, '', '', '', '', 'Observação'); ?>
                
                </td>
            </tr>
            
            
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	  </td>
            </tr>
      </table>
      </form>     


<script type="text/javascript">
function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "mogid";
	campos[1]			= "ctrvlr"; 
	campos[2]			= "ctrobsgar";		
	campos[3]			= "ctrdtinigar";
	campos[4]			= "ctrdtfimgar";	
	campos[5]			= "$ctrobservacao";	
					 
	tiposDeCampos[0] 	= "select";
	tiposDeCampos[1] 	= "texto";
	tiposDeCampos[2] 	= "texto";
	tiposDeCampos[3] 	= "data";
	tiposDeCampos[4] 	= "data";
	tiposDeCampos[5] 	= "texto";
 
	

	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}

</script>