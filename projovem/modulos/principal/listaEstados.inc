<?php

if(!$_SESSION['par']['boPerfilSuperUsuario'] && !$_SESSION['par']['boPerfilConsulta']){
	if( (!isset($_SESSION['par']['arEstuf']) && !count($_SESSION['par']['arEstuf'])) && $_SESSION['par']['estuf'] ){
		header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );
		exit;
	}	
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

echo montarAbasArray( criaAbaPar(), "par.php?modulo=principal/listaEstados&acao=A" );

monta_titulo( $titulo_modulo, '' );

# Estado
$_SESSION['par']['itrid'] = 1;
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<input type="hidden" name="pesquisa" value="1>"/>
					<div style="float: left;">
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									<strong>Filtros</strong>
								</td>
							</tr>
							</div>
							<tr>
								<td colspan="2"> 
								<div style="padding-bottom:10px;padding-top:15px;">	
									<label><input type="checkbox" name="naocomecoudiagnostico" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['naocomecoudiagnostico'] == 1 ? 'checked="checked"' : '' ?>/> Não Iniciou preenchimento</label>
									<br><p></p>
									<label><input type="checkbox" name="comecoudiagnostico" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['comecoudiagnostico'] == 1 ? 'checked="checked"' : '' ?>/> Iniciou preenchimento</label>
									<br><p></p>
									<label><input type="checkbox" name="dadosunidadecheck" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['dadosunidadecheck'] == 1 ? 'checked="checked"' : '' ?>/> 100% dados da unidade preenchido</label>
									<br><p></p>
									<label><input type="checkbox" name="questoespontuaischeck" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['questoespontuaischeck'] == 1 ? 'checked="checked"' : '' ?>/> 100% questões pontuais respondido / iniciou a pontuação</label>
								</td>
							</tr>
							<tr>
								<td colspan="2">Situação<br/>
									<?php
							
									
									$esdid = $_POST["esdid"];
									$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 44 order by esdordem";
									
									$res = $db->carregar( $sql );
									
									$db->monta_combo( "esdid", $res , 'S', 'Todas as Situações', '', '' );
									?>
								</td>
								
							</tr>
						</table>
						<br/>
					<div style="float: left;">
						<input type="submit" name="pesquisar" value="Pesquisar" />
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php 
$obEntidadeParControle = new EntidadeParControle();
$obEntidadeParControle->listaEstado($_POST);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function abrePlanoTrabalho(entidadePar, estuf, muncod, inuid){

	var data = new Array();
		data.push({name : 'requisicao', value : 'verificaInstrumentoUnidade'}, 
				  {name : 'entidadePar', value : entidadePar}, 
				  {name : 'estuf', value : estuf},
				  {name : 'inuid', value : inuid}
				 );	
	$.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(msg){
									var inuid = msg; 
									//$('#divDebug').html(msg);
									if(inuid > 0){
										//return $(location).attr('href', 'par.php?modulo=principal/planoTrabalhoPac2&acao=A');
										return $(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore');
									}
					  }
		 });

}
</script>
<div id="divDebug"></div>