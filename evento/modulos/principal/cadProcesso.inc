<?php
include_once APPRAIZ . "includes/classes/dateTime.inc";
 
if($_REQUEST['acao'] == 'I'){
	unset($_SESSION['copid']);
}

if($_REQUEST['copid'] && $_REQUEST['acao'] == 'A'){
	$_SESSION['copid'] = $_REQUEST['copid'];
} elseif($_SESSION['copid'] && $_REQUEST['acao'] == 'A'){
	$_REQUEST['copid'] = $_SESSION['copid'];
}
if($_POST['action']){
	salvar();
}

if( $_REQUEST['copid'] != '' && $_REQUEST['acao'] == 'A')
{ 
	$sql = "SELECT 				 
				co.copnumprocesso,
				co.copdsc,
				to_char(co.copdataabertura::date,'YYYY-MM-dd') AS copdataabertura,		 
				co.cooid, 		 
				co.cocid, 		 
				co.conid, 
				to_char(co.copdatalimite::date,'YYYY-MM-dd') AS copdatalimite,	
				co.usucpf,
				co.copobjdsc,
				us.usunome
			FROM 
				evento.coprocesso AS co 
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = co.usucpf 
			WHERE
				co.copid = '".$_REQUEST['copid']."'
				";
	$rsDadosProcesso = $db->carregar( $sql );
	$_SESSION['copid'] = $_REQUEST['copid'];
	
}

if($_GET['coiidExcluir']){
	$sql = "select distinct ip.cotid from evento.codemandaitem di
				inner join evento.coitemprocesso ip on di.cotid = ip.cotid 
			where ip.coiid = {$_GET['coiidExcluir']} and ip.copid = {$_SESSION['copid']}";
	$cotid = $db->pegaUm($sql);
	if($cotid){
		echo "<script>
				alert('Este item está vinculado com uma demanda, exclua a demanda primeiro.');
				history.back(-1);
		  	</script>";
		die;
	}
	
	$sql = "DELETE FROM evento.coitemprocesso WHERE copid = {$_SESSION['copid']} and coiid = ".$_GET['coiidExcluir'];
	$db->executar($sql);
	$db->commit();
	unset($_GET['coiidExcluir']);
}

function insereObjeto( $copid, $cooide ){
    global $db;
    if ($_REQUEST['copid'] != '' )    {
        $copid = $_REQUEST['copid'];
    }
    $sql = "INSERT INTO 
                evento.coprocessoobjeto
				(
					cooid,
					copid
				)
                VALUES
				(
					'$cooide',
					'$copid'
				)";
    
    if( !$db->executar( $sql )){
        return false;
    }   
}

function deletaObjeto( $copid ){
	global $db;    
	$sql = "DELETE FROM evento.coprocessoobjeto WHERE copid = '$copid'";
	if ( !$db->executar($sql)){
		return false;
	}
}

function insereElementoDespesa( $copid, $edpcod ){
    global $db;
    if ($_REQUEST['copid'] != '' )    {
        $copid = $_REQUEST['copid'];
    }
    $sql = "INSERT INTO 
                evento.coprocessoelementodespesa
				(
					edpcod,
					copid
				)
                VALUES
				(
					'$edpcod',
					'$copid'
				)";
    
    if( !$db->executar( $sql )){
        return false;
    }   
}

function deletaElementoDespesa( $copid ){
	global $db;    
	$sql = "DELETE FROM evento.coprocessoelementodespesa WHERE copid = '$copid'";
	if ( !$db->executar($sql)){
		return false;
	}
}

function salvar(){
	global $db; 
 	 
	$formataDatamA = formata_data_sql( $_REQUEST['copdataabertura'] );
	$formataDatamL = formata_data_sql( $_REQUEST['copdatalimite'] ); 
 	if(!$_REQUEST['copid'] && $_REQUEST['acao'] == 'I'){
	    $sql = "INSERT INTO 
	    		evento.coprocesso
	    		(
				 copnumprocesso,
				 copdsc, 
				 copdataabertura,		 
				 cocid, 		 
				 conid,   
				 copdatalimite,
				 usucpf,
				 copobjdsc
				)
				VALUES
				(
					'".$_POST['copnumprocesso']."',
					'".$_POST['copdsc']."',
					'".( $formataDatamA ? $formataDatamA : NULL )."',
					".( $_POST['cocid'] ? $_POST['cocid'] : 'NULL' ).",
					".( $_POST['conid'] ? $_POST['conid'] : 'NULL' ).",
					'".( $formataDatamL ? $formataDatamL : NULL )."',
					'{$_SESSION['usucpf']}',					
					'{$_POST['copobjdsc']}'
				) 
	    		RETURNING copid";
				 
 	}else{                    
		$sql = "UPDATE evento.coprocesso 
				SET
					copnumprocesso  = '".$_POST['copnumprocesso']."',
					copdsc  = '".$_POST['copdsc']."',
					copdataabertura = '".( $formataDatamA ? $formataDatamA : NULL )."',
					 
					cocid			= ".( $_POST['cocid'] ? $_POST['cocid'] : 'NULL' ).",
					conid 			= ".( $_POST['conid'] ? $_POST['conid'] : 'NULL' ).",
					copdatalimite 	= '".( $formataDatamL ? $formataDatamL : NULL )."',
					copobjdsc 		= '".$_POST['copobjdsc']."' 
				WHERE
					copid = ".$_REQUEST['copid']."
					RETURNING copid
		"; 
		
 	}
	$copid = $db->pegaUm($sql);
	deletaObjeto( $copid );
	$numObjeto = count($_REQUEST['cooid']);   
	if ($numObjeto > 0){                        
		for ( $i = 0; $i < $numObjeto; $i++ ){   
			if($_REQUEST['cooid'][$i] != ''){                  
				$cooide = $_REQUEST['cooid'][$i];
				insereObjeto( $copid, $cooide ); 
			}  
		}	                    
	}

	deletaElementoDespesa( $copid );
	$arElementoDespesa = count($_REQUEST['elemento']);   
	if ($arElementoDespesa > 0){                        
		for ( $i = 0; $i < $arElementoDespesa; $i++ ){
			if($_REQUEST['elemento'][$i] != ''){                  
				$edpcod = $_REQUEST['elemento'][$i];
				insereElementoDespesa( $copid, $edpcod ); 
			}  
		}	                    
	}

	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadProcesso', "&copid=".$copid);
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
                <td class ="SubTituloDireita" align="right">Nº Processo: </td> 
                <td> 
                 <?
				 $copnumprocesso = $rsDadosProcesso[0]['copnumprocesso'];
                 ?>
            	 <?= campo_texto('copnumprocesso', 'S', $somenteLeitura, 'Número de Processo', 40, 100, '', '', 'left', '',  0, 'id="copnumprocesso" onblur="MouseBlur(this);"' ); ?>               
                </td>
            </tr>
            
        	<tr>
                <td class ="SubTituloDireita" align="right">Título do Processo: </td> 
                <td> 
                 <?
				 $copdsc = $rsDadosProcesso[0]['copdsc'];
                 ?>
            	 <?= campo_texto('copdsc', 'S', $somenteLeitura, 'Título do Processo', 60, 200, '', '', 'left', '',  0, 'id="copdsc" onblur="MouseBlur(this);"' ); ?>               
                </td>
            </tr>
           	
           	<tr>
				<td class="SubTituloDireita" valign="top">
					Elemento de Despesa:
					<input type="hidden" id="elemento_campo_flag" name="elemento_campo_flag" value="0"/>
				</td>
				<td>
					<div id="elemento_campo_on">
						<?php 
							//$sql_combo = "select ele.edpcod as codigo, ele.edpcod || ' - ' || ele.edpdsc as descricao from financeiro.execucao r inner join public.elementodespesa ele on ele.edpcod = r.edpcod where r.rofano = '" . date("Y") . "' and ele.edpstatus = 'A' group by ele.edpcod, ele.edpdsc order by ele.edpcod, ele.edpdsc";
							
							$sql_combo = "select ele.edpcod as codigo, ele.edpcod || ' - ' || ele.edpdsc as descricao 
							from public.elementodespesa ele 
							where ele.edpstatus = 'A' 
							group by ele.edpcod, ele.edpdsc 
							order by ele.edpcod, ele.edpdsc";

							$sqlMarcados = "select ed.edpcod as codigo, ed.edpcod || ' - ' || ed.edpdsc as descricao from evento.coprocessoelementodespesa ped
												inner join public.elementodespesa ed on ped.edpcod = ed.edpcod
											where ped.copid = '{$_SESSION['copid']}'";
                           
							if( $_SESSION['copid'] ){
								$elemento = $db->carregar( $sqlMarcados );
							}
						?>
						<?php combo_popup( 'elemento', $sql_combo, 'Selecione o(s) Elemento(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
           	 
           	<tr>
                <td class ="SubTituloDireita" align="right">Data prevista de Abertura: </td>
                <td>  
                <?php 
                $copdataabertura  = $rsDadosProcesso[0]["copdataabertura"];
                ?>
                <?= campo_data( 'copdataabertura','S', 'S', 'Data prevista de Abertura', 'S' ); ?>
                </td> 
            </tr>
            
            <tr>
	      		<td class="subtitulodireita"> 	
	      	 		Descrição do Objeto do Processo:
	      	 	</td>
	      	 	<td align="left">
	      	 	<?php
				$copobjdsc = $rsDadosProcesso[0]["copobjdsc"];
	      	 	?>
		      	 	<textarea name="copobjdsc" title="Descrição do Objeto" rows="15" cols="80" class="text_editor_simple"><?= $copobjdsc ?></textarea>
		      	 </td>
	        </tr>
	        <tr>
                <td class="SubTituloDireita" valign="top">Objetos:</td>
                <td align="left">
                <?php
                                 
                $sql = "SELECT 
                           cooid as codigo, 
                           coodsc as descricao
                        FROM 
                            evento.coobjeto
                        WHERE coostatus = 'A'";
                
                $sqlMarcados = "SELECT 
								cp.cooid as codigo ,
								c.coodsc as descricao
								FROM 
								evento.coprocessoobjeto AS cp
								INNER JOIN evento.coobjeto c ON cp.cooid = c.cooid
								WHERE cp.copid =  '{$_SESSION['copid']}'";
                           
				if( $_SESSION['copid'] != '' ){
					$cooid = $db->carregar( $sqlMarcados );
				}
                
                combo_popup( "cooid", $sql, "Selecione o(s) Objeto(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
                ?>
                <img src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Tipo de Cotação: </td>
                <td> 
                <?php 
                $sql = "SELECT 
                            cocid AS codigo, 
                            cocdsc AS descricao
                        FROM
                            evento.cotipocotacao order by codigo";
                             
                $cocid = $rsDadosProcesso[0]["cocid"];
                 
                $db->monta_combo('cocid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'cocid',false,null,'Tipo Cotação');
                ?> 
              </td> 
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nível da Contratação: </td>
               <td> 
                <?php 
                $sql = "SELECT 
                            conid AS codigo, 
                            condsc AS descricao
                        FROM
                            evento.conivelcontratacao
                       ";
                             
                $conid = $rsDadosProcesso[0]["conid"];
                 
                $db->monta_combo('conid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'conid',false,null,'Nível da Contratação');
                ?> 
              </td> 
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Termo de referência: </td>
                <td> 
                <?php
				$sql = "SELECT 
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadCompraAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>'
                     FROM
                        ((public.arquivo arq INNER JOIN evento.coanexo aqb
                        ON arq.arqid = aqb.arqid) INNER JOIN evento.tipoanexo tarq
                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  aqb.copid = " .$_SESSION['copid']."
                    AND tarq.tpaid = ".ID_TERMO_REFERENCIA."
                    LIMIT 1";
				if( $_SESSION['copid'] != '' ){
                	$termo = $db->pegaUm( $sql );
				}
				if( $termo ){
                	echo $termo;
                }else{
                	echo "Termo de referência não anexado";
                }
				?> 
                </td> 
            </tr>	
            <tr>
                <td class ="SubTituloDireita" align="right">Data limite para adesão: </td>
                <td>  
                <?php 
                $copdatalimite = $rsDadosProcesso[0]["copdatalimite"];   
                ?>
                <?= campo_data( 'copdatalimite','S', 'S', 'Data limite para adesão', 'S' ); ?> 
                </td> 
            </tr>
            
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="hidden" name="copid" value="<?=$_SESSION['copid']; ?>" id="copid">
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	   <? if($_REQUEST['acao'] == 'A'){ ?><input type="button" value="Incluir Novo" onclick="window.location.href='evento.php?modulo=principal/cadProcesso&acao=I';" /><? } ?>  
            	  </td>
            </tr>
      </table>
      </form> 
<?php
if($_REQUEST['acao'] == 'A' && $_SESSION['copid']){
$sql = "SELECT 
			DISTINCT 
			('<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"removeVinculoItem('''||i.coiid||''');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>') as acao,
			i.coicodsiasg, i.coidsc,
			um.umedescricao, 
			coalesce( i.coivlrreferenciamin, 0 ) / 1 as coivlrreferenciamin,
			coalesce( i.coivlrreferenciamax, 0 ) / 1 as coivlrreferenciamax
	    FROM evento.coitem i
			LEFT JOIN evento.unidademedida um on i.umeid = um.umeid
			INNER JOIN evento.coitemprocesso ip  on i.coiid = ip.coiid
			WHERE i.coistatus = 'A' and ip.copid = {$_SESSION['copid']} 
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
?>

<script type="text/javascript">
function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "copnumprocesso";
	campos[1] 			= "copdsc";
	campos[2]			= "copdataabertura"; 
	campos[3]			= "copdatalimite";
	campos[4]			= "cocid";
	campos[5]			= "conid";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "texto"; 
	tiposDeCampos[2] 	= "data";
	tiposDeCampos[3] 	= "data"; 
	tiposDeCampos[4] 	= "select"; 
	tiposDeCampos[5] 	= "select"; 
	
	if( !processaComboPopup() ){
		return false;
	}
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}

function adicionarItem(){
	window.open('evento.php?modulo=principal/popupItem&acao=A','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
}

function removeVinculoItem(coiid){
	if(confirm('Deseja desvincular este item?')){
		window.location.href = "/evento/evento.php?modulo=principal/cadProcesso&acao=A&coiidExcluir="+coiid;
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
} 
</script>