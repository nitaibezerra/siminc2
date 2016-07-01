<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Wescley Lima
 */
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "escolaativa/classes/FuncaoDesignada.class.inc";

//ver($_GET['fudid'],"SELECT fudid FROM escolaativa.funcaodesignada WHERE fudid = '{$_GET['fudid']}' ",d);

if($_GET['fudid']){
	
	$bExiste = $db->pegaUm("SELECT fudid FROM escolaativa.funcaodesignada WHERE fudid = '{$_GET['fudid']}' ");
	if(!$bExiste){
		echo "<script>
				alert('Função não encontrada.');
				history.back(-1);
			  </script>";
		die;
	}
	
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                escolaativa.funcaodesignada 
            WHERE
                fudid = {$_GET['fudid']}";               
    
    $rsDadosFuncao = $db->carregar( $sql );
}

if($_REQUEST['excluir']){
	
	$oFuncaoDesignada = new FuncaoDesignada();
	$oFuncaoDesignada->fudid = $_GET['excluir'];
//	$oFuncaoDesignada->excluir();
	$oFuncaoDesignada->fudativo = 'f';
	$oFuncaoDesignada->alterar();
	$oFuncaoDesignada->commit();
	
}

if (array_key_exists('btnGravar', $_POST)){	
	
	$oFuncaoDesignada = new FuncaoDesignada();
	$oFuncaoDesignada->fudid 		= $_POST['fudid'];
	$oFuncaoDesignada->fuddsc 		= $_POST['fuddsc'];
	$oFuncaoDesignada->fudvlrbolsa 	= $_POST['fudvlrbolsa'];
	$oFuncaoDesignada->fuddtvigencia = formata_data_sql($_POST['fuddtvigencia']);
	$oFuncaoDesignada->fudativo 	= true;
	$oFuncaoDesignada->salvar();	
	$oFuncaoDesignada->commit();
	
}

?>
<br>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;">Função designada</label>
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
			<input type="hidden" id="fudid" name="fudid" value="<?=$rsDadosFuncao[0]["fudid"];?>">
			<?php
			$fuddsc = $rsDadosFuncao[0]["fuddsc"];				
			echo campo_texto('fuddsc', 'N', 'S', '', 80, 600, '', '', 'left', '',  0, 'id="fuddsc" onblur="MouseBlur(this);"' ); 
			?>
		</td>
	</tr>
	<tr>
		<td class ="SubTituloDireita" align"right"> Valor da bolsa: </td>
		<td>
			<?php
			$fudvlrbolsa = str_replace("R$","",$rsDadosFuncao[0]["fudvlrbolsa"]);				
			echo campo_texto('fudvlrbolsa', 'N', 'S', '', 16, 30, '', '', 'left', '',  0, 'id="fudvlrbolsa" onblur="MouseBlur(this);" onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);"' ); 
			?>
		</td>
	</tr>
	<tr>
		<td class ="SubTituloDireita" align"right"> Data de vigência: </td>
		<td>
			<?php
			$fuddtvigencia = formata_data($rsDadosFuncao[0]["fuddtvigencia"]);				
			echo campo_data2('fuddtvigencia', 'N', 'S', '', 'dd/mm/yyyy', '', '', ''); 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan = "2">			
			<input type="submit" name="btnGravar" value="Salvar" />
			<?php if(isset($_GET['fudid'])){ ?>
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
						'<img onclick=\"return alterarFuncaoDesignada('|| fudid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
						 <img onclick=\"return excluirFuncaoDesignada('|| fudid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acao,
						fuddsc,
						fudvlrbolsa,
						--fuddtvigencia
						to_char(fuddtvigencia, 'DD/MM/YYYY')
					FROM 
						escolaativa.funcaodesignada";
                                     
			if( strtoupper($_POST['fuddsc'])) {
                     	     
				$sql.=" WHERE 
							UPPER(fuddsc)  ILIKE '%" .strtoupper($_POST['fuddsc']). "%' ";
			}
                   
			if( strtoupper($_POST['fudvlrbolsa'])) {
				     
				$sql.=" ORDER BY
							fudvlrbolsa ";
			}                                    
			
		} else {
			
			$sql = "SELECT
						'<img onclick=\"return alterarFuncaoDesignada('|| fudid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
						 <img onclick=\"return excluirFuncaoDesignada('|| fudid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acao,
						fuddsc,
						fudvlrbolsa,
						--fuddtvigencia
						to_char(fuddtvigencia, 'DD/MM/YYYY')
					FROM 
						escolaativa.funcaodesignada
					WHERE fudativo = 't'";  
		}
		$cabecalho = array( "Opções", "Descrição ", "Valor da bolsa", "Data de vigência" );
        
		$db->monta_lista($sql, $cabecalho, 20, 10, 'N', '', '' );
                
		?>  
		</td>
	</tr>
</table>      
<script type="text/javascript">

function novoForm(){

	document.location.href = "escolaativa.php?modulo=principal/funcaoDesignada&acao=A";
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

	var fuddsc = document.getElementById('fuddsc');
	var fudvlrbolsa = document.getElementById('fudvlrbolsa');
	var fuddtvigencia = document.getElementById('fuddtvigencia');
	
	if(fuddsc.value == ''){
	
		alert("Preencha a descrição da função!");
		fuddsc.focus();
		return false;	
	}
	
	if(fudvlrbolsa.value == ''){
	
		alert("Preencha o valor da bolsa!");
		fudvlrbolsa.focus();
		return false;	
	}
	
	if(fuddtvigencia.value == ''){
	
		alert("Preencha a data de vigência!");
		fuddtvigencia.focus();
		return false;	
	}

}

function excluirFuncaoDesignada(fudid) {

	if (confirm('Deseja excluir o registro?')) {
             
		return window.location.href = 'escolaativa.php?modulo=principal/funcaoDesignada&acao=A&excluir=' + fudid;                
	}
}
         
function alterarFuncaoDesignada(fudid){
    
    return window.location.href = 'escolaativa.php?modulo=principal/funcaoDesignada&acao=A&fudid=' + fudid;
}
 
</script>