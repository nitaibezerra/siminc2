<?php
function listaDocumento(){
	global $db;
	$sql = "SELECT
				'<center>' || 
				CASE WHEN anx.anxdesc similar to '%'||nd.nudnumero::varchar(3)||'%'
					THEN '<img title=\"Atualizar Numero\"   hrc=\"#\" style=\"cursor: pointer;\" src=\"../..//imagens/refresh2.gif\" onclick=\"atualizaNumeroDoc(' || nd.nudid || ',' || anx.anxid || ')\" />' 
					ELSE '<img title=\"Inativar Documento\" hrc=\"#\" style=\"cursor: pointer;\" src=\"../..//imagens/excluir.gif\"  onclick=\"inativaDoc(' || anx.anxid || ')\" />' 
				END
				|| '</center>' as acao,
				'<a href=\"#\" onclick=\"window.location.href=\'?modulo=principal/editarprocesso&acao=A&prcid='|| anx.prcid ||'\'\">'|| prc.prcnumsidoc ||'</a>' as processo,
				anx.anxdesc,
				'<center>' || nd.nudnumero || '</center>',
				(select tpddsc from conjur.tipodocumento where tpdid = anx.tpdid) as tipoDocumento
			FROM 
				conjur.anexos anx
			INNER JOIN conjur.numeracaodocumento nd ON nd.prcid  = anx.prcid AND nd.tpdid = anx.tpdid AND nudstatus='A'
			INNER JOIN conjur.processoconjur 	prc ON prc.prcid = anx.prcid
			WHERE 
				anx.nudid is null 
				AND anxdtinclusao > '2011-02-10'
				AND anxstatus='A'
				AND not anx.tpdid isnull
			ORDER BY
				prc.prcnumsidoc,anx.tpdid";
	
	$cabecalho = Array("Atualizar","Processo","Documento","Numero Sugerido","Tipo Documento");
	$db->monta_lista_simples($sql,$cabecalho,100,5,'','', 'N' );
}

if( $_POST['req'] == 'atualiza' ){
	
	$sql = "UPDATE conjur.anexos
			SET
				nudid = ".$_POST['nudid']."
			WHERE
				anxid = ".$_POST['anxid'];
	$db->executar($sql);
	$db->commit();
	listaDocumento();
	die();
}

if( $_POST['req'] == 'inativa' ){
	
	$sql = "UPDATE conjur.anexos
			SET
				anxstatus = 'I'
			WHERE
				anxid = ".$_POST['anxid'];
	$db->executar($sql);
	$db->commit();
	listaDocumento();
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";

monta_titulo( 'Manutenção de Documentos', '&nbsp;' );

?>
<script src="./js/conjur.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td id="tdDoc">
			<?php 
				listaDocumento();
			?>
			</td>
		</tr>
	</table>
</form>
<script>

var td = $('#tdDoc');

function atualizaNumeroDoc(nudid,anxid){
	if( confirm('Realmente deseja atualizar este documento com o respectivo numero?') ){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "req=atualiza&nudid="+nudid+"&anxid="+anxid,
			success: function(msg){
				td.html(msg);
			}
		});
	}
}

function inativaDoc(anxid){
	if( confirm('Realmente deseja inativar este documento?') ){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "req=inativa&anxid="+anxid,
			success: function(msg){
				td.html(msg);
			}
		});
	}
}

</script>
