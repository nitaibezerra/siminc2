<?php
require_once APPRAIZ . "academico/classes/Portaria.class.inc";

if($_POST['acao']){
	extract($_POST);
	
	$obPortaria = new Portaria();
	$obPortaria->prtid    		= $prtid;
	$obPortaria->prtdsc 		= $prtdsc;
	$obPortaria->prtdata 		= formata_data_sql($prtdata);
	$obPortaria->prtstatus    	= $prtstatus;
	
	$obPortaria->salvar();
	$boSucesso = $obPortaria->commit();
	if(!$boSucesso){
		$obPortaria->rollback();
	}
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/listaPortaria2","");
	unset($obPortaria);
	die;
}

$obPortaria = new Portaria($_GET['prtid']);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
//$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(document).ready(function() {
	$('#formulario').submit(function(){
		var nomeform 		= 'formulario';
 		var submeterForm 	= false;
 		var campos 			= new Array("prtdsc","prtdata");
 		var tiposDeCampos 	= new Array("texto","data");
 
 		if( !validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
 			return false;
 		}
	});
});
//-->
</script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario" id="formulario" action="/academico/academico.php?modulo=principal/cadastroPortaria&acao=A">
<input type="hidden" name="acao" id="acao" value="1" />
<input type="hidden" name="prtid" id="prtid" value="<?php echo $obPortaria->prtid; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="font-weight: bold" colspan="2">
					Portaria
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Descricao
			</td>
			<td><?php
					$prtdsc = $obPortaria->prtdsc;
					echo campo_texto('prtdsc', 'S', $permissao_formulario, 'Descriçao', 40, 250, '', '','','','','id="prtdsc"'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Data
			</td>
			<td><?php
					$prtdata = $obPortaria->prtdata ? formata_data($obPortaria->prtdata) : '';
					echo campo_data2('prtdata', 'S', 'S','Data da Portaria', '', '', '', $prtdata, '', '', 'prtdata');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Situaçao
			</td>
			<td>
				<?php
					$arStatus = array(
						array(
							'codigo'=>'A',
							'descricao'=>'Ativo'
						),
						array(
							'codigo'=>'I',
							'descricao'=>'Inativo'
						)
					);
					$prtstatus = $obPortaria->prtstatus;
					$db->monta_combo( "prtstatus", $arStatus, 'S', '', '', '' );
				?>
			</td>
		</tr>
	</table>
	<div id=buttonAcao>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					<input type="submit" id="btnSalvar" value="Salvar"  /> <input type="button" value="Lista" onclick="window.location.href='academico.php?modulo=principal/listaPortaria2&acao=A';" />
				</td>
			</tr>
	</table>
	</div>
</form>