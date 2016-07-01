<?php

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( 'CONJUR', 'Lista de Advogados' ); 
 
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<form action="" method="POST" name="formulario">
<input type='hidden' name="submetido" value="1">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<span style="cursor: pointer" onclick="cadastrarAdvogado();" title="Incluir Advogado">
					<img align="absmiddle" src="/imagens/gif_inclui.gif" /> <a href="#">Incluir Advogado</a>
				</span>
			</td>
		</tr>
	</tbody>
</table>
</form>

<? 
$sql = "SELECT
			'<img
			align=\"absmiddle\"
			src=\"/imagens/alterar.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: editarAdvogado (\''||ent.entnumcpfcnpj||'\','||adv.coonid||');\"
		 	alert(adv.coonid);
			title=\"Editar Advogado\">  
			
			<img
			align=\"absmiddle\"
			src=\"/imagens/excluir.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: excluirAdvogado (\''||ent.entnumcpfcnpj||'\','||adv.coonid||');\"
			title=\"Excluir Advogado\"> 
			',
			usu.usunome,
			conjur.coordAdv(advid) as coord,
			CASE WHEN adv.advstatus = 'A' THEN 'Ativo'
			     ELSE 'Inativo' 
			END as advstatus
		FROM
			conjur.advogados as adv
		INNER join entidade.entidade as ent ON ent.entid=adv.entid
		INNER join seguranca.usuario as usu ON ent.entnumcpfcnpj=usu.usucpf
		ORDER BY usu.usunome ASC";

$cabecalho = array("Ações", "Nome", "Coordenação", "Status" ); 

$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 

?>

<script>

function validaForm(){
	document.formulario.submit();
}

function editarAdvogado (fdpcpf,coonid){

	if (fdpcpf && coonid){ 
			return windowOpen( '?modulo=sistema/geral/cadDadosAdvogado&acao=A&ajaxcpf='+ fdpcpf +'&coorde='+ coonid,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen(  '?modulo=sistema/geral/cadDadosAdvogado&acao=A&ajaxcpf='+ fdpcpf +'&coorde='+ coonid,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}


function cadastrarAdvogado($coonid){
	if ($coonid){ 
			return windowOpen( '?modulo=sistema/geral/cadDadosAdvogado&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen(  '?modulo=sistema/geral/cadDadosAdvogado&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}


function excluirAdvogado(cpf,coonid) {
	   var confirma = confirm("Deseja Excluir o Advogado?")
	   if ( confirma ){


			var req = new Ajax.Request('conjur.php?modulo=sistema/geral/cadDadosAdvogado&acao=A', {
		        method:     'post',

		        onComplete: function (res) {

					window.location.href = '?modulo=sistema/geral/cadDadosAdvogado&acao=A&cpf='+ cpf +'&coorde='+ coonid;
				}
});
		   
	   return true
	   } else {
	   return false
	   }
	   }

</script>