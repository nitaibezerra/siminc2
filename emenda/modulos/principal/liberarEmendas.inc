<?php

if( $_POST['requisicao'] == 'salvar' ){
	//ver( $_POST,d );
	$arrEmeid = array();
	if( is_array( $_POST['emeidcheck'] ) ){
		foreach ($_POST['emeidcheck'] as $key => $v) {
			array_push( $arrEmeid, array("emeid" => $key, "emelibera" => $v));
		}		
	}
	$arEmeid = ( is_array( $_POST['emeid'] ) ? $_POST['emeid'] : array() );
	foreach ($arrEmeid as $key => $v) {
		if( in_array( $v['emeid'], $arEmeid ) ){
			if( $v['emelibera'] == 'N' ){
			 	$sql = "UPDATE emenda.emenda SET emelibera = 'S' WHERE emeid = ".$v['emeid'];
				$db->executar( $sql );
			}
		} else {
			$sql = "UPDATE emenda.emenda SET emelibera = 'N' WHERE emeid = ".$v['emeid'];
			$db->executar( $sql );
		}
	}
	if($db->commit())
		$db->sucesso('principal/liberarEmendas');
	else{
		echo "<script>
				alert('Falha na operação.');
				window.location.href = window.location;
			</script>";
		exit();
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo('Liberar Emendas', '');
?>
<form action="" method="post" id="formliberacaoEmenda" name="formliberacaoEmenda">
<input type="hidden" value="" name="requisicao" id="requisicao"/>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
<tr bgcolor="#D0D0D0">
	<td align="left" width="20%">
		<label for='seleciona_todos'><input type='checkbox' id='seleciona_todos' onclick="selecionaTodos();"  /> <b>Selecionar Todos</b></lable>
	</td>
</tr>
<tr bgcolor="#D0D0D0">
	<td colspan="6">
	<?
	//$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
	//if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil )){
		echo '<input type="button" value="Liberar" id="btnSalvar" name="btnSalvar" onclick="liberarEmendas();" style="cursor: pointer;"/>';
	/*} else {
		echo '<input type="button" value="Salvar Limite" id="btnSalvar" name="btnSalvar" disabled style="cursor: pointer;"/>';
	}*/
	?>
	</td>
</tr>
</table>
<script type="text/javascript">
function selecionaTodos(){
	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var emeid 			= document.getElementsByName("emeid[]");
	
	for(var i=0; i<emeid.length; i++) {
		if( emeid[i].disabled == false ){
			if(selecionaTodos.checked) {
				if(!emeid[i].checked && !emeid[i].disabled) emeid[i].checked = true;
			} else {
				if(emeid[i].checked) emeid[i].checked = false;
			}
		}
	}
}
function liberarEmendas(){
	var form = document.getElementById('formlibera');
	form.submit();
}
</script>
</form>
<?
$sql = "SELECT
		  CASE WHEN eme.emelibera = 'N' THEN
			'<center><input type=\"checkbox\" id=\"emeid[' || eme.emeid || ']\" name=\"emeid[]\" value=\"' || eme.emeid || '\" />
						<input type=\"hidden\" id=\"requisicao\" name=\"requisicao\" value=\"salvar\">
						<input type=\"hidden\" id=\"emeidcheck[]\" name=\"emeidcheck['||eme.emeid||']\" value=\"'||eme.emelibera||'\"></center>'
          ELSE 
            CASE WHEN (SELECT count(emd.emdid) FROM emenda.emendadetalhe emd
  						inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid WHERE emd.emeid = eme.emeid) > 0 THEN
            	'<center><input type=\"checkbox\" id=\"emeid[' || eme.emeid || ']\" name=\"emeid[]\" value=\"' || eme.emeid || '\" disabled=\"disabled\" checked /><input type=\"hidden\" id=\"requisicao\" name=\"requisicao\" value=\"salvar\"></center>'
            ELSE
            	'<center><input type=\"checkbox\" id=\"emeid[' || eme.emeid || ']\" name=\"emeid[]\" value=\"' || eme.emeid || '\" checked />
            				<input type=\"hidden\" id=\"requisicao\" name=\"requisicao\" value=\"salvar\">
            				<input type=\"hidden\" id=\"emeidcheck[]\" name=\"emeidcheck['||eme.emeid||']\" value=\"'||eme.emelibera||'\"></center>'
            END	
          END as acao,
		  eme.emecod,
		  case when eme.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
		  vfun.fupfuncionalprogramatica,
		  res.resdsc,
		  eme.emeano
		FROM 
		  emenda.emenda eme
		  inner join emenda.autor aut on aut.autid = eme.autid
		  left join emenda.v_funcionalprogramatica vfun on vfun.acaid = eme.acaid and vfun.acastatus = 'A'
		  left join emenda.responsavel res on res.resid = eme.resid and res.resstatus = 'A'
		WHERE
			eme.emeano = '{$_SESSION['exercicio']}'
		ORDER BY
			eme.emecod";

$cabecalho = array( "", "Código", "Autor", "Funcional Prg", "Responsável", "Ano");
$db->monta_lista($sql, $cabecalho, 5000000, 10, 'N', 'center', '', 'formlibera', '','');
?>