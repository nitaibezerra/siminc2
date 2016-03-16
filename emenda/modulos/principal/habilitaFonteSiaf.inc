<?php
if( $_POST['requisicao'] == 'pesquisar' ){
	if( $_POST['foscod'] ){
		$filtro .= " and f.foscod = '".trim($_POST['foscod'])."'"; 
	}
	if( $_POST['fosdsc'] ){
		$filtro .= " and f.fosdsc ilike '%".trim($_POST['fosdsc'])."%'"; 
	}
} else {
	if( $_POST['requisicao'] == 'salvar' ){		
		if( is_array($_POST['foscod']) ){
			//$sql = "UPDATE financeiro.fontesiafi SET fosemendas = false WHERE foscod in ('".implode("','", $_POST['foscod'])."')";
			$sql = "UPDATE financeiro.fontesiafi SET fosemendas = false ";
			$db->executar( $sql );
				
			foreach ($_POST['foscod'] as $key => $v) {
				$sql = "UPDATE financeiro.fontesiafi SET fosemendas = true WHERE foscod = '$v'";
				$db->executar( $sql );
			}
		}
		if($db->commit())
			$db->sucesso('principal/habilitaFonteSiaf');
		else{
			echo "<script>
					alert('Falha na operação.');
					window.location.href = window.location;
				</script>";
			exit();
		}
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';

monta_titulo('Habilita Fonte SIAF', '');

$foscod = $_POST['foscod'];
$fosdsc = $_POST['fosdsc'];

?>

<form action="" method="post" id="formulario" name="formulario">
<input type="hidden" value="" name="requisicao" id="requisicao"/>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
<!-- <tr>
	<td class="SubTituloDireita" style="width:19.5%;">Código:</td>
	<td><?=campo_texto( 'foscod', 'N', 'S', '', 40, 250, '', '','','','','id="foscod"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width:39.5%;">Descrição:</td>
	<td><?=campo_texto( 'fosdsc', 'N', 'S', '', 80, 250, '', '','','','','id="fosdsc"'); ?></td>
</tr>
<tr bgcolor="#D0D0D0">
	<td align="center" width="20%" colspan="2">
		<input type="button" value="Pesquisar" id="btnPesquisa" name="btnPesquisa" onclick="pesquisa();" style="cursor: pointer;"/>
		<input type="button" value="Limpar" id="btnLimpar" name="btnLimpar" onclick="limpar();" style="cursor: pointer;"/>
	</td>
</tr>-->
<tr bgcolor="#D0D0D0">
	<td align="left" width="20%" colspan="2">
		<label for='seleciona_todos'><input type='checkbox' id='seleciona_todos' onclick="selecionaTodos();"  /> <b>Selecionar Todos</b></lable>
	</td>
</tr>
<tr bgcolor="#D0D0D0">
	<td colspan="6">
	<?
	echo '<input type="button" value="Liberar" id="btnSalvar" name="btnSalvar" onclick="liberarEmendas();" style="cursor: pointer;"/>';
	?>
	</td>
</tr>
</table>

<?

$sql = "SELECT DISTINCT
		    case when f.fosemendas = true then
		    	'<center><input type=\"checkbox\" id=\"foscod[' || f.foscod || ']\" name=\"foscod[]\" value=\"' || f.foscod || '\" checked />
		    	<input type=\"hidden\" value=\"salvar\" name=\"requisicao\" id=\"requisicao\"/></center>'
		    else 
		    	case when (select count(exfid) from emenda.execucaofinanceira where exfcodfontesiafi = f.foscod) > 0 then
		        	'<center><input type=\"checkbox\" id=\"foscod[' || f.foscod || ']\" name=\"foscod[]\" value=\"' || f.foscod || '\" disabled=\"disabled\" checked />
		        			<input type=\"hidden\" value=\"salvar\" name=\"requisicao\" id=\"requisicao\"/></center>'
		        else
		    		'<center><input type=\"checkbox\" id=\"foscod[' || f.foscod || ']\" name=\"foscod[]\" value=\"' || f.foscod || '\" />
		    			<input type=\"hidden\" value=\"salvar\" name=\"requisicao\" id=\"requisicao\"/></center>'
		    	end
		    end as bofonte,
		    f.foscod as codigo, 
		    f.fosdsc as descricao
		FROM financeiro.fontesiafi f
		WHERE
			1=1
			$filtro ";

$cabecalho = array( "", "Código", "Descrição");
$db->monta_lista($sql, $cabecalho, 5000000, 10, 'N', 'center', '', 'formlibera', '','');
?>
<script type="text/javascript">
function selecionaTodos(){
	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var foscod 			= document.getElementsByName("foscod[]");
	
	for(var i=0; i<foscod.length; i++) {
		if( foscod[i].disabled == false ){
			if(selecionaTodos.checked) {
				if(!foscod[i].checked && !foscod[i].disabled) foscod[i].checked = true;
			} else {
				if(foscod[i].checked) foscod[i].checked = false;
			}
		}
	}
}
function liberarEmendas(){
	var form = document.getElementById('formlibera');
	form.submit();
}

function pesquisa(){
	var form = document.getElementById('formulario');
	document.getElementById('requisicao').value = 'pesquisar';
	form.submit();
}
function limpar(){
	window.location.href = window.location;
}
</script>