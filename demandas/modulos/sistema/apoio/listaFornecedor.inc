
<?

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Lista de Fornecedores";
monta_titulo( $titulo_modulo, 'Selecione os campos abaixo para pesquisa' );

?>

<form method="post" id="formpesquisa" name="formpesquisa">

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr><td class="SubTituloDireita" width="40%">CNPJ:</td>
		<td>
			<?$cnpjemp = $_REQUEST['cnpjemp'];?>
			<?=campo_texto('cnpjemp','N','S','',21,18,'##.###.###/####-##','');?>
		</td>
	</tr>
	<tr><td class="SubTituloDireita">Nome da empresa:</td><td>
		<?$nomeemp = $_REQUEST['nomeemp'];?>
		<?=campo_texto('nomeemp','N','S','',80,200,'','');?></td>
	</tr>
	<tr>
	<td align="center" colspan="2">
		<input type="button" value="Pesquisar" onclick="pesquisafor();">
		<input type="button" onclick="vertodosfor();" value="Ver Todos">
		<input type="button" onclick="neditafor();" value="Cadastrar Fornecedor">
	</td>
	</tr>
	</table>
	<?
	/* Analisando filtro de pesquisa */
	if($cnpjemp) {
		$cnpjemp = str_replace(".", "", $cnpjemp);
		$cnpjemp = str_replace("/", "", $cnpjemp);
		$cnpjemp = str_replace("-", "", $cnpjemp);
		$filtropesquisa .= " AND ent.entnumcpfcnpj='".$cnpjemp."'";
	}
	
	if($nomeemp) {
		$filtropesquisa .= " AND ent.entnome ilike '%".$nomeemp."%'";
	}
	/* FIM - Analisando filtro de pesquisa */
	
		
		$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"neditafor('|| ent.entid ||');\"></center>' as acoes, ent.entnome, '<center>'||ent.entnumcpfcnpj||'</center>' as cnpj, '<center>'||ende.estuf||'</center>' as uf, '<center>'||mundescricao||'</center>' as mun 
				FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
				WHERE fen.funid = '". FUNID_FORNECEDOR ."'  
				$filtropesquisa 
				ORDER BY ent.entnome";
		//dbg($sql);
		$cabecalho = array("Ações", "Nome", "CNPJ", "UF", "Município");
		
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
</form>


<script>

function vertodosfor(){
	document.formpesquisa.cnpjemp.value='';
	document.formpesquisa.nomeemp.value='';
	document.formpesquisa.submit();
}

function pesquisafor(){
	document.formpesquisa.submit();
}

function neditafor(entid){

	if(entid){
		window.open('?modulo=sistema/apoio/fornecedor&acao=A&entid='+entid,'Fornecedor','scrollbars=yes,height=600,width=700,status=no,toolbar=no,menubar=no,location=no');	
	}
	else{
		window.open('?modulo=sistema/apoio/fornecedor&acao=A','Fornecedor','scrollbars=yes,height=600,width=700,status=no,toolbar=no,menubar=no,location=no');
	}
}

</script>
	
