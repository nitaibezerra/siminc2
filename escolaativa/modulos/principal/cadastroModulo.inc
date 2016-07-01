<?php

require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "escolaativa/classes/Modulo.class.inc";

//ver($_GET['modid'],"SELECT modid FROM escolaativa.funcaodesignada WHERE modid = '{$_GET['modid']}' ",d);

if($_GET['modid']){
	
	$bExiste = $db->pegaUm("SELECT modid FROM escolaativa.modulo WHERE modid = '{$_GET['modid']}' ");
	if(!$bExiste){
		echo "<script>
				alert('Módulo não encontrado.');
				history.back(-1);
			  </script>";
		die;
	}
	
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                escolaativa.modulo 
            WHERE
                modid = {$_GET['modid']}";               
    
    $rsDadosFuncao = $db->carregar( $sql );
}

if($_REQUEST['excluir']){
	
	$oModulo = new Modulo();
	$oModulo->modid = $_GET['excluir'];
	$oModulo->modinativo = 'f';
//	$oModulo->excluir();
	$oModulo->alterar();
	$oModulo->commit();
	
}

if (array_key_exists('btnGravar', $_POST)){	
	
	$oModulo = new Modulo();
	$oModulo->modid 		= $_POST['modid'];
	$oModulo->moddsc 		= $_POST['moddsc'];
	$oModulo->modinativo 	= true;
	$oModulo->salvar();	
	$oModulo->commit();
	
}

?>
<br>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;">Módulo</label>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td>
	</tr>
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmFuncaoDesignada">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">         
	<tr>
		<td class ="SubTituloDireita" align"right"> Descrição: </td>
		<td>
			<input type="hidden" id="modid" name="modid" value="<?=$rsDadosFuncao[0]["modid"];?>">
			<?php
			$moddsc = $rsDadosFuncao[0]["moddsc"];				
			echo campo_texto('moddsc', 'N', 'S', '', 80, 600, '', '', 'left', '',  0, 'id="moddsc" onblur="MouseBlur(this);"' ); 
			?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloCentro" colspan = "2">			
			<input type="submit" name="btnGravar" value="Salvar" />
			<?php if(isset($_GET['modid'])){ ?>
			<input type="button" name="btnNovo" value="Novo" onclick="novoForm()"/>
			<?php } ?>
			<input type="submit" name="btnBuscar" value="Buscar" />		              
		</td>
	</tr>        
</table>
</form> 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
		<?php
                
		if (array_key_exists('btnBuscar', $_POST)) {
			
			$sql = "SELECT
						'<img onclick=\"return alterarModulo('|| modid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
						 <img onclick=\"return excluirModulo('|| modid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acao,
						moddsc
					FROM 
						escolaativa.modulo";
                                     
			if( strtoupper($_POST['moddsc'])) {
                     	     
				$sql.=" WHERE 
							UPPER(moddsc)  ILIKE '%" .strtoupper($_POST['moddsc']). "%' ";
			}
			
		} else {
			
			$sql = "SELECT
						'<img onclick=\"return alterarModulo('|| modid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
						 <img onclick=\"return excluirModulo('|| modid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acao,
						moddsc
					FROM 
						escolaativa.modulo
					WHERE modinativo = 't'";  
		}
		$cabecalho = array( "Opções", "Descrição " );
		$cellWidth = array("100px");
		
		$db->monta_lista($sql, $cabecalho, 20, 10, 'N', '', '','',$cellWidth );
                
		?>  
		</td>
	</tr>
</table>      
<script type="text/javascript">

function novoForm(){

	document.location.href = "escolaativa.php?modulo=principal/cadastroModulo&acao=A";
}

function validaForm() {
  
//	var total = frmFuncaoDesignada.elements.length;
//	i = 0;
//	while (i <= total) {
//    
//		if(frmFuncaoDesignada.elements[i].value == "") {
//         
//			alert("É necessário o preenchimento de todos os campos");
//           
//			frmFuncaoDesignada.elements[i].focus();
//			return false;
//			break;
//		}
//		i++;
//	}

	var moddsc = document.getElementById('moddsc');
	
	if(moddsc.value == ''){
	
		alert("Preencha a descrição do módulo!");
		moddsc.focus();
		return false;
	
	}
   
}

function excluirModulo(modid) {

	if (confirm('Deseja excluir o registro?')) {
             
		return window.location.href = 'escolaativa.php?modulo=principal/cadastroModulo&acao=A&excluir=' + modid;                
	}
}
         
function alterarModulo(modid){
    
    return window.location.href = 'escolaativa.php?modulo=principal/cadastroModulo&acao=A&modid=' + modid;
}
 
</script>