<?php
include_once APPRAIZ . "includes/classes/dateTime.inc";
 
if($_REQUEST['acao'] == 'I'){
	unset($_SESSION['ctaid']);
}

if($_REQUEST['ctaid'] && $_REQUEST['acao'] == 'A'){
	$_SESSION['ctaid'] = $_REQUEST['ctaid'];
} elseif($_SESSION['ctaid'] && $_REQUEST['acao'] == 'A'){
	$_REQUEST['ctaid'] = $_SESSION['ctaid'];
}
if($_POST['action']){
	salvar();
}

if( $_REQUEST['ctaid'] != '' && $_REQUEST['acao'] == 'A')
{ 
			
	$sql = "SELECT  
					ctaid, 
					ctanome, 
					ctacnpj, 
					ctacep, 
					ctalog, 
					ctanum, 
					ctacom, 
					ctabai,      
					muncod, 
					estuf, 
					ctafoneddd, 
					ctafonenum, 
					ctafaxddd, 
					ctafaxnum,       
					ctafonedddopc, 
					ctafonenumopc, 
					ctaemail 
	  			FROM contratos.ctcontratada  
			WHERE
				ctaid = '".$_REQUEST['ctaid']."'
				";
	$rsDadosContrato = $db->carregar( $sql );
	$_SESSION['ctaid'] = $_REQUEST['ctaid'];
	
}
/*
function insereObjeto( $ctaid, $cooide ){
    global $db;
    if ($_REQUEST['ctaid'] != '' )    {
        $ctaid = $_REQUEST['ctaid'];
    }
    $sql = "INSERT INTO 
                contratos.coprocessoobjeto
				(
					cooid,
					ctaid
				)
                VALUES
				(
					'$cooide',
					'$ctaid'
				)";
    
    if( !$db->executar( $sql )){
        return false;
    }   
}

function deletaObjeto( $ctaid ){
	global $db;    
	$sql = "DELETE FROM contratos.coprocessoobjeto WHERE ctaid = '$ctaid'";
	if ( !$db->executar($sql)){
		return false;
	}
}

function insereElementoDespesa( $ctaid, $edpcod ){
    global $db;
    if ($_REQUEST['ctaid'] != '' )    {
        $ctaid = $_REQUEST['ctaid'];
    }
    $sql = "INSERT INTO 
                contratos.coprocessoelementodespesa
				(
					edpcod,
					ctaid
				)
                VALUES
				(
					'$edpcod',
					'$ctaid'
				)";
    
    if( !$db->executar( $sql )){
        return false;
    }   
}

function deletaElementoDespesa( $ctaid ){
	global $db;    
	$sql = "DELETE FROM contratos.coprocessoelementodespesa WHERE ctaid = '$ctaid'";
	if ( !$db->executar($sql)){
		return false;
	}
}
*/
function salvar(){
	global $db; 
 	 
 	if(!$_REQUEST['ctaid'] && $_REQUEST['acao'] == 'I'){
	    $sql = "INSERT INTO 
	    		contratos.ctcontratada
	    		(
						ctaid, 
						ctanome, 
						ctacnpj, 
						ctacep, 
						ctalog, 
						ctanum, 
						ctacom, 
						ctabai,      
						muncod, 
						estuf, 
						ctafoneddd, 
						ctafonenum, 
						ctafaxddd, 
						ctafaxnum,       
						ctafonedddopc, 
						ctafonenumopc, 
						ctaemail 
	    			    						)
				VALUES
				(
					'".$_POST['ctaid']."',
					'".$_POST['ctanome']."',
					'".$_POST['ctacnpj']."',
					'".$_POST['ctacep']."',
					'".$_POST['ctalog']."',					
					'".$_POST['ctanum']."',		
					'".$_POST['ctacom']."',
   					'".$_POST['ctabai']."',
					".( $_POST['muncod'] ? $_POST['muncod'] : 'NULL' ).",   							
					".( $_POST['estuf'] ? $_POST['estuf'] : 'NULL' ).",				
					'".$_POST['ctafoneddd']."',
					'".$_POST['ctafonenum']."',
					'".$_POST['ctafaxddd']."',
					'".$_POST['ctafaxnum']."',
					'".$_POST['ctafonedddopc']."',
					'".$_POST['ctafonenumopc']."',					
					'".$_POST['ctaemail']."'				
				) 
	    		RETURNING ctaid";
				 
 	}else{                    
		$sql = "UPDATE contratos.ctcontratada 
				SET				
					ctaid			= '".$_POST['ctaid']."',  
					ctanome			= '".$_POST['ctanome']."',
					ctacnpj			= '".$_POST['ctacnpj']."',
					ctacep			= '".$_POST['ctacep']."', 
					ctalog			= '".$_POST['ctalog']."', 
					ctanum			= '".$_POST['ctanum']."', 
					ctacom			= '".$_POST['ctacom']."', 
					ctabai			= '".$_POST['ctabai']."',      
					muncod			= ".( $_POST['muncod'] ? $_POST['muncod'] : 'NULL' ).", 
					estuf			= ".( $_POST['estuf'] ? $_POST['estuf'] : 'NULL' ).", 
					ctafoneddd		= '".$_POST['ctafoneddd']."', 
					ctafonenum		= '".$_POST['ctafonenum']."', 
					ctafaxddd		= '".$_POST['ctafaxddd']."', 
					ctafaxnum		= '".$_POST['ctafaxnum']."',       
					ctafonedddopc	= '".$_POST['ctafonedddopc']."', 
					ctafonenumopc	= '".$_POST['ctafonenumopc']."', 
					ctaemail		= '".$_POST['ctaemail']."' 					
				WHERE
					ctaid = ".$_REQUEST['ctaid']."
					RETURNING ctaid
		"; 
		
 	}
	$ctaid = $db->pegaUm($sql);
/*	deletaObjeto( $ctaid );
	$numObjeto = count($_REQUEST['cooid']);   
	if ($numObjeto > 0){                        
		for ( $i = 0; $i < $numObjeto; $i++ ){   
			if($_REQUEST['cooid'][$i] != ''){                  
				$cooide = $_REQUEST['cooid'][$i];
				insereObjeto( $ctaid, $cooide ); 
			}  
		}	                    
	}

	deletaElementoDespesa( $ctaid );
	$arElementoDespesa = count($_REQUEST['elemento']);   
	if ($arElementoDespesa > 0){                        
		for ( $i = 0; $i < $arElementoDespesa; $i++ ){
			if($_REQUEST['elemento'][$i] != ''){                  
				$edpcod = $_REQUEST['elemento'][$i];
				insereElementoDespesa( $ctaid, $edpcod ); 
			}  
		}	                    
	}
*/
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadContratada', "&ctaid=".$ctaid);
	die;
} 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
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
                <td class ="SubTituloDireita" align="right">Razão Social:</td>
                <td>
                 <?
                 $ctanome = $rsDadosContrato[0]['ctanome'];                               
                 ?>          
                 <?= campo_texto('ctanome', 'S', $somenteLeitura, 'Razão Social da Contratada', 100, 200, '', '', 'left', '',  0, 'id="ctanome" onblur="MouseBlur(this);"' ); ?>
                 
                </td>
                         
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">CNPJ:</td>
                <td>
                 <?
                 $ctacnpj = $rsDadosContrato[0]['ctacnpj'];
                  ?>
                  <?= campo_texto('ctacnpj', 'S', $somenteLeitura, 'C', 15, 200, '', '', 'left', '',  0, 'id="ctacnpj" onblur="MouseBlur(this);"' ); ?>
                
                </td>
                         
            </tr>
            
        	<tr>
                <td class ="SubTituloDireita" align="right">CEP: </td> 
                <td> 
                 <?
				 $ctacep = $rsDadosContrato[0]['ctacep'];
                 ?>
            	 <?= campo_texto('ctacep', 'S', $somenteLeitura, 'Número do Processo de Contratação', 30, 200, '', '', 'left', '',  0, 'id="ctacep" onblur="MouseBlur(this);"' ); ?>
                    	  	                
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right">Logrodouro:</td>
                <td>
                 <?
				 $ctalog = $rsDadosContrato[0]['ctalog'];
                 ?>
            	 <?= campo_texto('ctalog', 'S', $somenteLeitura, 'Número do Processo de Contratação', 30, 200, '', '', 'left', '',  0, 'id="ctalog" onblur="MouseBlur(this);"' ); ?>
 				</td>
			</tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Número: </td>
                <td>
            	 <?
            	 $ctanum = $rsDadosContrato[0]['ctanum'];
            	 ?>
            	 <?= campo_texto('ctanum', 'S', $somenteLeitura, 'Número Modalidade Contratação', 20, 200, '', '', 'left', '',  0, 'id="ctanum" onblur="MouseBlur(this);"' ); ?>
 				</td>
			</tr>
		           		                      
           	<tr>
                <td class ="SubTituloDireita" align="right">Complemento: </td> 
                <td> 
                 <?
				 $ctacom = $rsDadosContrato[0]['ctacom'];
                 ?>
            	 <?= campo_texto('ctacom', 'S', $somenteLeitura, 'Número do Processo de Execução Financeira', 30, 200, '', '', 'left', '',  0, 'id="ctacom" onblur="MouseBlur(this);"' ); ?>
            	 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Bairro:</td>
                <td>
            	 <?
            	 $ctrprocexecctr = $rsDadosContrato[0]['ctabai'];
            	  ?>
            	  <?= campo_texto('ctabai', 'S', $somenteLeitura, 'Nome do Bairro da Contratada', 30, 200, '', '', 'left', '',  0, 'id="ctabai" onblur="MouseBlur(this);"' ); ?>            	 
            	         
 				</td>
			</tr>           
         
	        <tr>
                <td class="SubTituloDireita" valign="top">Cidade:</td>
                <td align="left">
                <?php
                                 
                $sql = "SELECT 
                           muncod as codigo, 
                           mundescricao as descricao
                        FROM 
                            territorios.municipio";
                
                $muncod = $rsDadosContrato[0]["muncod"];
                
                $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'muncod',false,null,'Cidade');
                ?>

            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">UF:</td>
                <td>
                              
                <?
                $estuf = $rsDadosContrato[0]['estuf'];
                 ?>
                 <?= campo_texto('estuf', 'S', $somenteLeitura, 'UF da Contratada', 3, 200, '', '', 'left', '',  0, 'id="estuf" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
                        
            
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Telefone (DDD + Número):</td>
                <td>
                <?php 
                $ctrafoneddd  = $rsDadosContrato[0]["ctafoneddd"];
                ?>
                <?= campo_texto('ctafoneddd', 'S', $somenteLeitura, 'Valor Inicial do Contrato', 3, 200, '', '', 'left', '',  0, 'id="ctafoneddd" onblur="MouseBlur(this);"' ); ?>

                <?php 
                $ctafonenum  = $rsDadosContrato[0]["ctafonenum"];
                ?>
                <?= campo_texto('ctafonenum', 'S', $somenteLeitura, 'Valor Inicial do Contrato', 10, 200, '', '', 'left', '',  0, 'id="ctafonenum" onblur="MouseBlur(this);"' ); ?>
                
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Telefone 2(DDD + Número):</td>
                <td>
                <?php 
                $ctrafonedddopc  = $rsDadosContrato[0]["ctafonedddopc"];
                ?>
                <?= campo_texto('ctafonedddopc', 'S', $somenteLeitura, 'Valor Inicial do Contrato', 3, 200, '', '', 'left', '',  0, 'id="ctafonedddopc" onblur="MouseBlur(this);"' ); ?>

                <?php 
                $ctafonenumopc  = $rsDadosContrato[0]["ctafonenumopc"];
                ?>
                <?= campo_texto('ctafonenumopc', 'S', $somenteLeitura, 'Valor Inicial do Contrato', 10, 200, '', '', 'left', '',  0, 'id="ctafonenumopc" onblur="MouseBlur(this);"' ); ?>
                
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Fax (DDD + Número):</td>
                <td>
                <?php 
                $ctafaxddd  = $rsDadosContrato[0]["ctafaxddd"];
                ?>
                <?= campo_texto('ctafaxddd', 'S', $somenteLeitura, 'Valor Inicial do Contrato', 3, 200, '', '', 'left', '',  0, 'id="ctafaxddd" onblur="MouseBlur(this);"' ); ?>

                <?php 
                $ctafonenum  = $rsDadosContrato[0]["ctafaxnum"];
                ?>
                <?= campo_texto('ctafaxnum', 'S', $somenteLeitura, 'Valor Inicial do Contrato', 10, 200, '', '', 'left', '',  0, 'id="ctafaxnum" onblur="MouseBlur(this);"' ); ?>
                
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right" colspan="1">Email:</td>
                <td>
                <?php 
                $ctaemail  = $rsDadosContrato[0]["ctaemail"];
                ?>
                <?= campo_texto('ctaemail', 'S', $somenteLeitura, 'Valor Anual do Contrato', 50, 200, '', '', 'left', '',  0, 'id="ctaemail" onblur="MouseBlur(this);"' ); ?>
     
                </td>
            </tr>

            
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="hidden" name="ctaid" value="<?=$_SESSION['ctaid']; ?>" id="ctaid">
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	   <? if($_REQUEST['acao'] == 'A'){ ?><input type="button" value="Incluir Novo" onclick="window.location.href='contratos.php?modulo=principal/cadProcesso&acao=I';" /><? } ?>  
            	  </td>
            </tr>
      </table>
      </form>     
<?php
/*
if($_REQUEST['acao'] == 'A' && $_SESSION['ctaid']){
$sql = "SELECT 
			DISTINCT 
			('<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"removeVinculoItem('''||i.coiid||''');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>') as acao,
			i.coicodsiasg, i.coidsc,
			um.umedescricao, 
			coalesce( i.coivlrreferenciamin, 0 ) / 1 as coivlrreferenciamin,
			coalesce( i.coivlrreferenciamax, 0 ) / 1 as coivlrreferenciamax
	    FROM contratos.coitem i
			LEFT JOIN contratos.unidademedida um on i.umeid = um.umeid
			INNER JOIN contratos.coitemprocesso ip  on i.coiid = ip.coiid
			WHERE i.coistatus = 'A' and ip.ctaid = {$_SESSION['ctaid']} 
			and i.coiiditempai is null";
$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Cód. SIASG", "Descrição", "Unidade de Medida", "Valor Máx.", "Valor Min." ); 
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '');
?>      
    <? if($_REQUEST['acao'] == 'A'){ ?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td style="background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<label style="cursor: pointer" onclick="adicionarItem();"><img src="/imagens/gif_inclui.gif" id="add" border=0 alt="Ir" title="Adicionar Item"> Adicionar Item</label>
			</td>
		</tr>
	</table>
<? } 
}
*/	
?>

<script type="text/javascript">
function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "ctanome";
	campos[1]			= "ctacnpj"; 
	campos[2]			= "ctacep";
	campos[3]			= "ctalog";
	campos[4]			= "ctanum";
	campos[5]			= "ctacom";
	campos[6]			= "ctabai";
	campos[7]			= "muncod";
	campos[8]			= "estuf";
	campos[9]			= "ctafoneddd";
	campos[10]			= "ctafonenum";
	campos[11]			= "ctafaxddd";	
	campos[12]			= "ctafaxnum";	
	campos[13]			= "ctafonedddopc";
	campos[14]			= "ctaemail";	
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "texto"; 
	tiposDeCampos[2] 	= "texto";
	tiposDeCampos[3] 	= "texto"; 
	tiposDeCampos[4] 	= "texto"; 
	tiposDeCampos[5] 	= "texto";
	tiposDeCampos[6] 	= "texto"; 
	tiposDeCampos[7] 	= "select";
	tiposDeCampos[8] 	= "texto";
	tiposDeCampos[9] 	= "texto";
	tiposDeCampos[10] 	= "texto";
	tiposDeCampos[11] 	= "texto";
	tiposDeCampos[12] 	= "texto";
	tiposDeCampos[13] 	= "texto";
	tiposDeCampos[14] 	= "texto";
 
	
/*	if( !processaComboPopup() ){
		return false;
	}*/
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}
/*
function adicionarItem(){
	window.open('contratos.php?modulo=principal/popupItem&acao=A','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
}

function removeVinculoItem(coiid){
	if(confirm('Deseja desvincular este item?')){
		window.location.href = "/evento/contratos.php?modulo=principal/cadProcesso&acao=A&coiidExcluir="+coiid;
		return true;
	} else {
		return false;
	}
}

function processaComboPopup(){ 
	var elemento = document.getElementById('elemento');
	var objeto 	 = document.getElementById('cooid');
	var msg = '';	

	selectAllOptions( elemento );
	selectAllOptions( objeto ); 
	
	if( elemento.value == '' ){
		msg += 'Você deve selecionar no mínimo Elemento Despesa.\n';
	}
	
	if( objeto.value == '' ){
		msg += 'Você deve selecionar no mínimo Objeto.\n';
	}

	if(msg != ''){
		alert(msg);
		return false;
	}
	
	return true;
} */
</script>